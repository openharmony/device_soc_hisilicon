/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "drv_osal_lib.h"
#include "cryp_sm2.h"

#ifdef CHIP_SM2_SUPPORT
/************************* Internal Structure Definition *********************/
/** \addtogroup      sm2 */
/** @{ */ /** <!-- [sm2] */

/* max length of SM2 ID */
#define SM2_ID_MAX_LEN 0x1FFF

/* SM2 PC of uncompress */
#define SM2_PC_UNCOMPRESS 0x04

/* SM2 encrypt try count */
#define SM2_ENCRYPT_TRY_CNT 10

#define BYTE_BITS       8
#define SM2_ENC_PAD_LEN (SM2_LEN_IN_BYTE * 3)

static const ecc_param_t g_sm2_id = {
    .p = (td_u8 *)"\xFF\xFF\xFF\xFE\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                  "\xFF\xFF\xFF\xFF\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF",
    .a = (td_u8 *)"\xFF\xFF\xFF\xFE\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                  "\xFF\xFF\xFF\xFF\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFC",
    .b = (td_u8 *)"\x28\xE9\xFA\x9E\x9D\x9F\x5E\x34\x4D\x5A\x9E\x4B\xCF\x65\x09\xA7"
                  "\xF3\x97\x89\xF5\x15\xAB\x8F\x92\xDD\xBC\xBD\x41\x4D\x94\x0E\x93",
    .n = (td_u8 *)"\xFF\xFF\xFF\xFE\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                  "\x72\x03\xDF\x6B\x21\xC6\x05\x2B\x53\xBB\xF4\x09\x39\xD5\x41\x23",
    .g = {
        (td_u8 *)"\x32\xC4\xAE\x2C\x1F\x19\x81\x19\x5F\x99\x04\x46\x6A\x39\xC9\x94"
                 "\x8F\xE3\x0B\xBF\xF2\x66\x0B\xE1\x71\x5A\x45\x89\x33\x4C\x74\xC7",
        (td_u8 *)"\xBC\x37\x36\xA2\xF4\xF6\x77\x9C\x59\xBD\xCE\xE3\x6B\x69\x21\x53"
                 "\xD0\xA9\x87\x7C\xC6\x2A\x47\x40\x02\xDF\x32\xE5\x21\x39\xF0\xA0"
    },
};

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Code *****************************/
/** \addtogroup      sm2 */
/** @{ */ /** <!-- [kapi] */

/**
\brief   Kapi Init.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_sm2_init(void)
{
    td_s32 ret;

    func_enter();

    ret = cryp_sm2_init();
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_sm2_init, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

/**
\brief   Kapi Deinitialize.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 kapi_sm2_deinit(void)
{
    func_enter();

    cryp_sm2_deinit();

    func_exit();
    return TD_SUCCESS;
}

static td_s32 sm2_kdf(const ecc_point_t *point, td_u8 *k, td_u32 klen)
{
    td_s32 ret;
    td_u32 sm3id = 0;
    td_u8 h[SM2_LEN_IN_BYTE] = { 0 };
    td_u32 i;
    td_u32 block;
    td_u32 hashlen;

    func_enter();

    chk_ptr_err_return(k);

    if (klen == 0) {
        return TD_SUCCESS;
    }

    block = (klen + SM2_LEN_IN_BYTE - 1) / SM2_LEN_IN_BYTE;

    for (i = 0; i < block; i++) {
        td_u32 ct = crypto_cpu_to_be32(i + 1);

        /* *** H = SM3(X || Y || CT) *** */
        chk_func_err_goto(kapi_hash_start(&sm3id, OT_CIPHER_HASH_TYPE_SM3, TD_NULL, 0));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)point->x, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)point->y, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)&ct, sizeof(ct), HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_finish(sm3id, h, sizeof(h), &hashlen));
        if (i == (block - 1)) {
            ret = memcpy_s(k + i * SM2_LEN_IN_BYTE, SM2_LEN_IN_BYTE, h, klen - i * SM2_LEN_IN_BYTE);
        } else {
            ret = memcpy_s(k + i * SM2_LEN_IN_BYTE, SM2_LEN_IN_BYTE, h, sizeof(h));
        }
        if (ret != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
            ret = OT_ERR_CIPHER_FAILED_SEC_FUNC;
        }
    }

    /* check k != 0 */
    for (i = 0; i < klen; i++) {
        if (k[i] != 0) {
            break;
        }
    }

    if (i >= klen) {
        ot_cipher_error("Invalid key.\n");
        return OT_ERR_CIPHER_ILLEGAL_KEY;
    }

    func_exit();
    return TD_SUCCESS;

exit__:
    return ret;
}

td_s32 kapi_sm2_sign(sm2_sign_t *sign, const td_u8 *id, td_u16 idlen, hash_chunk_src src)
{
    td_s32 ret;
    td_u32 sm3id = 0;
    td_u32 e[SM2_LEN_IN_WROD] = {0};
    td_u32 hashlen = 0;
    sm2_func *func = TD_NULL;

    chk_ptr_err_return(sign);
    chk_param_err_return((id == TD_NULL) || (idlen > SM2_ID_MAX_LEN));
    chk_param_err_return((sign->msglen > 0) && (sign->msg == TD_NULL));

    func = cryp_get_sm2_op();
    chk_ptr_err_return(func);
    chk_ptr_err_return(func->sign);

    /* msg is the result of sm3 ? */
    if ((idlen == SM3_RESULT_SIZE) && (sign->msglen == 0x00)) {
        /* id is the sm3 result when msglen = 0 and idlen = 32 */
        if (memcpy_s(e, sizeof(e), id, idlen) != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
            return OT_ERR_CIPHER_FAILED_SEC_FUNC;
        }
    } else {
        /* compute e */
        td_u8 za[SM2_LEN_IN_BYTE] = {0};
        td_u8 entla[2] = {0}; /* 2 byte entla */

        entla[0] = (td_u8)((idlen * BYTE_BITS) >> BYTE_BITS);
        entla[1] = (td_u8)((idlen * BYTE_BITS));

        /* *** ZA=H256(ENTLA||IDA||a||b||xG||yG||xA||yA) *** */
        /* SM2 and sm3 use two different lock, so have to use SM3 Instead of MPI */
        chk_func_err_goto(kapi_hash_start(&sm3id, OT_CIPHER_HASH_TYPE_SM3, TD_NULL, 0));
        chk_func_err_goto(kapi_hash_update(sm3id, entla, sizeof(entla), HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, id, idlen, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)g_sm2_id.a, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)g_sm2_id.b, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)g_sm2_id.g.x, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)g_sm2_id.g.y, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)sign->px, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)sign->py, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_finish(sm3id, za, sizeof(za), &hashlen));

        /* *** e=H256(ZA||M) *** */
        chk_func_err_goto(kapi_hash_start(&sm3id, OT_CIPHER_HASH_TYPE_SM3, TD_NULL, 0));
        chk_func_err_goto(kapi_hash_update(sm3id, za, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, sign->msg, sign->msglen, src));
        chk_func_err_goto(kapi_hash_finish(sm3id, (u8 *)e, sizeof(e), &hashlen));
    }

    /***************************************
      1. generate a randnum k, k?[1,n-1],
      2. (x1,y1)=[k]G,
      3. r=(e+x1)mod n, if r=0 or r+k=n, return step 1
      4. s=((1+dA)^-1 ?(k-r?dA))mod n
    ****************************************/
    ret = func->sign(sign, e, sizeof(e), &g_sm2_id);
    if (ret != TD_SUCCESS) {
        print_func_errno(func->sign, ret);
    }

exit__:
    (td_void)memset_s(e, sizeof(e), 0, sizeof(e));
    return ret;
}

td_s32 kapi_sm2_verify(sm2_verify_t *verify, const td_u8 *id, td_u16 idlen, hash_chunk_src src)
{
    td_s32 ret;
    td_u32 sm3id;
    td_u32 e[SM2_LEN_IN_WROD] = { 0 };
    td_u32 hashlen = 0;
    sm2_func *func = TD_NULL;

    chk_ptr_err_return(verify);
    chk_param_err_return((id == TD_NULL) || idlen > SM2_ID_MAX_LEN);
    chk_param_err_return((verify->msglen > 0) && (verify->msg == TD_NULL));

    func = cryp_get_sm2_op();
    chk_ptr_err_return(func);
    chk_ptr_err_return(func->verify);

    /* msg is the result of sm3 ? */
    if ((idlen == SM3_RESULT_SIZE) && (verify->msglen == 0x00)) {
        /* id is the sm3 result when msglen = 0 and idlen = 32 */
        if (memcpy_s(e, sizeof(e), id, idlen) != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
            return OT_ERR_CIPHER_FAILED_SEC_FUNC;
        }
    } else {
        td_u8 entla[2] = { 0 };  /* 2 byte entla */
        td_u8 za[SM2_LEN_IN_BYTE] = { 0 };

        /* compute e */
        entla[0] = (td_u8)((idlen * BYTE_BITS) >> BYTE_BITS);
        entla[1] = (td_u8)((idlen * BYTE_BITS));

        /* *** ZA=H256(ENTLA||IDA||a||b||xG||yG||xA||yA) *** */
        chk_func_err_goto(kapi_hash_start(&sm3id, OT_CIPHER_HASH_TYPE_SM3, TD_NULL, 0));
        chk_func_err_goto(kapi_hash_update(sm3id, entla, sizeof(entla), HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, id, idlen, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)g_sm2_id.a, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)g_sm2_id.b, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)g_sm2_id.g.x, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)g_sm2_id.g.y, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)verify->px, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)verify->py, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_finish(sm3id, za, sizeof(za), &hashlen));

        /* *** e=H256(ZA||M) *** */
        chk_func_err_goto(kapi_hash_start(&sm3id, OT_CIPHER_HASH_TYPE_SM3, TD_NULL, 0));
        chk_func_err_goto(kapi_hash_update(sm3id, za, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
        chk_func_err_goto(kapi_hash_update(sm3id, verify->msg, verify->msglen, src));
        chk_func_err_goto(kapi_hash_finish(sm3id, (u8 *)e, sizeof(e), &hashlen));
    }

    /***************************************
      1. t=(r+s)mod n, if t==0, return fail
      2. (x1,y1)=[s]G+tP,
      3. r=(e+x1)mod n, if r==R, return pass
    ****************************************/
    ret = func->verify(verify, e, sizeof(e), &g_sm2_id);
    if (ret != TD_SUCCESS) {
        print_func_errno(func->verify, ret);
    }

exit__:
    (td_void)memset_s(e, sizeof(e), 0, sizeof(e));
    return ret;
}

td_s32 kapi_sm2_encrypt(const ot_cipher_sm2_public_key *sm2_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt)
{
    td_u32 c1_index;
    td_u32 c2_index;
    td_u32 c3_index;
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    td_u32 c1x[SM2_LEN_IN_WROD] = {0};
    td_u32 c1y[SM2_LEN_IN_WROD] = {0};
    td_u32 x2[SM2_LEN_IN_WROD] = {0};
    td_u32 y2[SM2_LEN_IN_WROD] = {0};
    td_u32 sm3id = 0;
    td_u32 hashlen = 0;
    sm2_func *func = TD_NULL;
    ecc_point_t point_c1 = {(td_u8 *)c1x, (td_u8 *)c1y, SM2_LEN_IN_BYTE};
    ecc_point_t point_xy = {(td_u8 *)x2, (td_u8 *)y2, SM2_LEN_IN_BYTE};

    chk_ptr_err_return(sm2_key);
    chk_param_err_return((plain_txt == TD_NULL) || (plain_txt->data == TD_NULL) || (plain_txt->data_len == 0));
    chk_param_err_return((cipher_txt == TD_NULL) || (cipher_txt->data == TD_NULL) || (cipher_txt->data_len == 0));
    chk_param_err_return(cipher_txt->data_len < (plain_txt->data_len + SM2_ENCRYPT_PAD_LEN));

    func = cryp_get_sm2_op();
    chk_param_err_return((func == TD_NULL) || (func->encrypt == TD_NULL));

    c1_index = 0;
    c3_index = c1_index + SM2_LEN_IN_BYTE + SM2_LEN_IN_BYTE;
    c2_index = c3_index + SM2_LEN_IN_BYTE;
    cipher_txt->data_len = plain_txt->data_len + SM2_ENC_PAD_LEN;

    /* C1 = k * G
     * C1(x, y) = k * G(x, y), XY = k * P(x, y)
     */
    chk_func_err_goto(func->encrypt(sm2_key, &point_c1, &point_xy, &g_sm2_id));

    /* the key consist of PC||x1||y1, PC = 4 when uncompress */
    if ((memcpy_s(&cipher_txt->data[c1_index],
                  cipher_txt->data_len - c1_index, c1x, sizeof(c1x)) != EOK) ||
        (memcpy_s(&cipher_txt->data[c1_index + SM2_LEN_IN_BYTE],
                  cipher_txt->data_len - c1_index - SM2_LEN_IN_BYTE, c1y, sizeof(c1y)) != EOK)) {
        print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }

    /* gen mask for msg encrypt */
    chk_func_err_goto(sm2_kdf(&point_xy, &cipher_txt->data[c2_index], plain_txt->data_len));

    /* C2=M ^ k */
    for (i = 0; i < plain_txt->data_len; i++) {
        cipher_txt->data[c2_index++] ^= plain_txt->data[i];
    }

    /* *** C3 =Hash(x2||M||y2) *** */
    chk_func_err_goto(kapi_hash_start(&sm3id, OT_CIPHER_HASH_TYPE_SM3, TD_NULL, 0));
    chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)x2, sizeof(x2), HASH_CHUNCK_SRC_LOCAL));
    chk_func_err_goto(kapi_hash_update(sm3id, plain_txt->data, plain_txt->data_len, HASH_CHUNCK_SRC_LOCAL));
    chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)y2, sizeof(y2), HASH_CHUNCK_SRC_LOCAL));
    chk_func_err_goto(kapi_hash_finish(sm3id, &cipher_txt->data[c3_index], SM2_LEN_IN_BYTE, &hashlen));

exit__:
    return ret;
}

td_s32 kapi_sm2_decrypt(const ot_cipher_sm2_private_key *sm2_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt)
{
    td_u32 c1_index;
    td_u32 c2_index;
    td_u32 c3_index;
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    td_u8 h[SM2_LEN_IN_BYTE] = {0};
    td_u32 c1x[SM2_LEN_IN_WROD] = {0};
    td_u32 c1y[SM2_LEN_IN_WROD] = {0};
    td_u32 x2[SM2_LEN_IN_WROD] = {0};
    td_u32 y2[SM2_LEN_IN_WROD] = {0};
    td_u32 sm3id = 0;
    td_u32 hashlen = 0;
    sm2_func *func = TD_NULL;
    ecc_point_t point_c1 = {(td_u8 *)c1x, (td_u8 *)c1y, SM2_LEN_IN_BYTE};
    ecc_point_t point_xy = {(td_u8 *)x2, (td_u8 *)y2, SM2_LEN_IN_BYTE};

    chk_ptr_err_return(sm2_key);
    chk_param_err_return((plain_txt == TD_NULL) || (plain_txt->data == TD_NULL) || (plain_txt->data_len == 0));
    chk_param_err_return((cipher_txt == TD_NULL) || (cipher_txt->data == TD_NULL) || (cipher_txt->data_len == 0));
    chk_param_err_return(cipher_txt->data_len < SM2_ENCRYPT_PAD_LEN);
    chk_param_err_return(plain_txt->data_len < (cipher_txt->data_len - SM2_ENCRYPT_PAD_LEN));

    func = cryp_get_sm2_op();
    chk_param_err_return((func == TD_NULL) || (func->decrypt == TD_NULL));

    c1_index = 0;
    c3_index = c1_index + SM2_LEN_IN_BYTE + SM2_LEN_IN_BYTE;
    c2_index = c3_index + SM2_LEN_IN_BYTE;
    plain_txt->data_len = cipher_txt->data_len - SM2_ENC_PAD_LEN;  /* *** C2 = C - C1 -C3 *** */

    /* C1 */
    if ((memcpy_s(c1x, sizeof(c1x), &cipher_txt->data[c1_index], SM2_LEN_IN_BYTE) != EOK) ||
        (memcpy_s(c1y, sizeof(c1y), &cipher_txt->data[c1_index + SM2_LEN_IN_BYTE], SM2_LEN_IN_BYTE) != EOK)) {
        print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }

    /* Compute M(x, y) = C1(x, y) * d */
    chk_func_err_goto(func->decrypt(sm2_key, &point_c1, &point_xy, &g_sm2_id));

    /* Compute mask for msg */
    chk_func_err_goto(sm2_kdf(&point_xy, plain_txt->data, plain_txt->data_len));

    /* Get clear msg from C2 */
    for (i = 0; i < plain_txt->data_len; i++) {
        plain_txt->data[i] ^= cipher_txt->data[c2_index++];
    }

    /* Check C3 */
    chk_func_err_goto(kapi_hash_start(&sm3id, OT_CIPHER_HASH_TYPE_SM3, TD_NULL, 0));
    chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)x2, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
    chk_func_err_goto(kapi_hash_update(sm3id, plain_txt->data, plain_txt->data_len, HASH_CHUNCK_SRC_LOCAL));
    chk_func_err_goto(kapi_hash_update(sm3id, (td_u8 *)y2, SM2_LEN_IN_BYTE, HASH_CHUNCK_SRC_LOCAL));
    chk_func_err_goto(kapi_hash_finish(sm3id, h, sizeof(h), &hashlen));

    if (memcmp(&cipher_txt->data[c3_index], h, SM2_LEN_IN_BYTE) != 0) {
        print_func_errno(memcmp, OT_ERR_CIPHER_FAILED_DECRYPT);
        return OT_ERR_CIPHER_FAILED_DECRYPT;
    }
exit__:
    return ret;
}
#endif

/** @} */ /** <!-- ==== Structure Definition end ==== */
