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

#include "ot_mpi_cipher.h"
#include "common_check_param.h"
#include "sys_cipher.h"
#include "drv_cipher_debug.h"
#include "securec.h"

#define BYTE_BITS               8
#define CIPHER_MAX_MULTIPAD_NUM 5000
#define TRNG_TIMEOUT            10000

/*
 * brief  Init the cipher device.
 */
td_s32 ot_mpi_cipher_init(td_void)
{
    return sys_symc_init();
}

/*
 * brief  Deinit the cipher device.
 */
td_s32 ot_mpi_cipher_deinit(td_void)
{
    return sys_symc_deinit();
}

/*
 * brief Obtain a cipher handle for encryption and decryption.
 */
td_s32 ot_mpi_cipher_create(td_handle *handle, const ot_cipher_attr *cipher_attr)
{
    td_s32 ret;
    td_u32 id = 0;

    func_enter();

    chk_ptr_err_return(handle);
    chk_ptr_err_return(cipher_attr);
    chk_param_err_return(cipher_attr->cipher_type >= OT_CIPHER_TYPE_BUTT);

    ret = sys_symc_create(&id, cipher_attr->cipher_type);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_create, ret);
        return ret;
    }

    *handle = (td_handle)id;

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief Destroy the existing cipher handle.
 */
td_s32 ot_mpi_cipher_destroy(td_handle handle)
{
    td_s32 ret;

    func_enter();

    ret = sys_symc_destroy(handle);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_destroy, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief Configures the cipher control information.
 */
td_s32 ot_mpi_cipher_set_cfg(td_handle handle, const ot_cipher_ctrl *ctrl)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(ctrl);

    ret = sys_symc_config(handle, ctrl);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_config, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_get_cfg(td_handle handle, ot_cipher_ctrl *ctrl)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(ctrl);

    ret = sys_symc_get_config(handle, ctrl);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_get_config, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief Performs encryption.
 */
td_s32 ot_mpi_cipher_encrypt(td_handle handle,
    td_phys_addr_t src_phys_addr, td_phys_addr_t dst_phys_addr, td_u32 byte_len)
{
    td_s32 ret;
    compat_addr in;
    compat_addr out;

    func_enter();

    addr_u64(in) = src_phys_addr;
    addr_u64(out) = dst_phys_addr;

    ret = sys_symc_crypto(handle, in, out, byte_len, SYMC_OPERATION_ENCRYPT);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_crypto, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief Performs description.
 */
td_s32 ot_mpi_cipher_decrypt(td_handle handle,
    td_phys_addr_t src_phys_addr, td_phys_addr_t dst_phys_addr, td_u32 byte_len)
{
    td_s32 ret;
    compat_addr in;
    compat_addr out;

    func_enter();

    addr_u64(in) = src_phys_addr;
    addr_u64(out) = dst_phys_addr;

    ret = sys_symc_crypto(handle, in, out, byte_len, SYMC_OPERATION_DECRYPT);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_crypto, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief Performs encryption.
 */
td_s32 ot_mpi_cipher_encrypt_virt(td_handle handle, const td_u8 *src_data, td_u8 *dst_data, td_u32 byte_len)
{
    td_s32 ret;
    compat_addr in;
    compat_addr out;

    func_enter();

    chk_ptr_err_return(src_data);
    chk_ptr_err_return(dst_data);

    addr_via_const(in) = src_data;
    addr_via(out) = dst_data;

    ret = sys_symc_crypto(handle, in, out, byte_len, SYMC_OPERATION_ENCRYPT_VIA);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_crypto, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief Performs decryption.
 */
td_s32 ot_mpi_cipher_decrypt_virt(td_handle handle, const td_u8 *src_data, td_u8 *dst_data, td_u32 byte_len)
{
    td_s32 ret;
    compat_addr in;
    compat_addr out;

    func_enter();

    chk_ptr_err_return(src_data);
    chk_ptr_err_return(dst_data);

    addr_via_const(in) = src_data;
    addr_via(out) = dst_data;

    ret = sys_symc_crypto(handle, in, out, byte_len, SYMC_OPERATION_DECRYPT_VIA);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_crypto, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief Encrypt multiple packaged data.
 */
td_s32 ot_mpi_cipher_encrypt_multi_pack(td_handle handle, const ot_cipher_data *data_pack, td_u32 data_pack_num)
{
    td_u32 i;
    td_s32 ret;
    compat_addr in;
    compat_addr out;
    ot_cipher_ctrl ctrl = {0};

    func_enter();

    chk_ptr_err_return(data_pack);
    chk_param_err_return(data_pack_num == 0x00);
    chk_param_err_return(data_pack_num > CIPHER_MAX_MULTIPAD_NUM);

    ret = sys_symc_get_config(handle, &ctrl);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_get_config, ret);
        return ret;
    }
    chk_param_err_return(ctrl.work_mode > OT_CIPHER_WORK_MODE_CTR);
    for (i = 0; i < data_pack_num; i++) {
        ret = sys_symc_config(handle, &ctrl);
        if (ret != TD_SUCCESS) {
            print_func_errno(sys_symc_config, ret);
            return ret;
        }
        addr_u64(in) = data_pack[i].src_phys_addr;
        addr_u64(out) = data_pack[i].dst_phys_addr;
        ret = sys_symc_crypto(handle, in, out, data_pack[i].byte_len, SYMC_OPERATION_ENCRYPT);
        if (ret != TD_SUCCESS) {
            print_func_errno(sys_symc_crypto, ret);
            return ret;
        }
    }

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief Encrypt multiple packaged data.
 */
td_s32 ot_mpi_cipher_decrypt_multi_pack(td_handle handle, const ot_cipher_data *data_pack, td_u32 data_pack_num)
{
    td_u32 i;
    td_s32 ret;
    compat_addr in;
    compat_addr out;
    ot_cipher_ctrl ctrl = {0};

    func_enter();

    chk_ptr_err_return(data_pack);
    chk_param_err_return(data_pack_num == 0x00);
    chk_param_err_return(data_pack_num > CIPHER_MAX_MULTIPAD_NUM);

    ret = sys_symc_get_config(handle, &ctrl);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_get_config, ret);
        return ret;
    }
    chk_param_err_return(ctrl.work_mode > OT_CIPHER_WORK_MODE_CTR);
    for (i = 0; i < data_pack_num; i++) {
        ret = sys_symc_config(handle, &ctrl);
        if (ret != TD_SUCCESS) {
            print_func_errno(sys_symc_config, ret);
            return ret;
        }
        addr_u64(in) = data_pack[i].src_phys_addr;
        addr_u64(out) = data_pack[i].dst_phys_addr;
        ret = sys_symc_crypto(handle, in, out, data_pack[i].byte_len, SYMC_OPERATION_DECRYPT);
        if (ret != TD_SUCCESS) {
            print_func_errno(sys_symc_crypto, ret);
            return ret;
        }
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_get_tag(td_handle handle, td_u8 *tag, td_u32 tag_len)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(tag);

    ret = sys_aead_get_tag(handle, tag, tag_len);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_aead_get_tag, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_get_random_num(td_u32 *random_num)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(random_num);

    ret = sys_trng_get_random(random_num, TRNG_TIMEOUT);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_trng_get_random, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_hash_init(const ot_cipher_hash_attr *hash_attr, td_handle *handle)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(hash_attr);
    chk_ptr_err_return(handle);

    ret = sys_hash_start(handle, hash_attr->sha_type, hash_attr->hmac_key, hash_attr->hmac_key_len);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_hash_start, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_hash_update(td_handle handle, const td_u8 *in_data, td_u32 in_data_len)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(in_data);

    ret = sys_hash_update(handle, in_data, in_data_len, HASH_CHUNCK_SRC_USER);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_hash_update, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_hash_final(td_handle handle, td_u8 *out_hash, td_u32 out_hash_len)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(out_hash);
    chk_param_err_return(out_hash_len == 0);
    chk_param_err_return(handle == (td_handle)OT_INVALID_HANDLE);

    ret = sys_hash_finish(handle, out_hash, out_hash_len);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_hash_finish, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_void mpi_rsa_init_private_key(cryp_rsa_key *key, const ot_cipher_rsa_private_key *rsa_key)
{
    (td_void)memset_s(key, sizeof(cryp_rsa_key), 0, sizeof(cryp_rsa_key));
    key->public  = TD_FALSE;
    key->klen    = rsa_key->n_len;
    key->n       = rsa_key->n;
    key->d       = rsa_key->d;
    key->p       = rsa_key->p;
    key->q       = rsa_key->q;
    key->dp      = rsa_key->dp;
    key->dq      = rsa_key->dq;
    key->qp      = rsa_key->qp;
}

td_s32 ot_mpi_cipher_rsa_public_encrypt(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt)
{
    td_s32 ret;
    cryp_rsa_key key;

    sys_rsa_crypto_data rsa_data = {0};
    ot_cipher_rsa_encrypt_scheme rsa_scheme = OT_CIPHER_RSA_ENCRYPT_SCHEME_BUTT;

    ret = cipher_rsa_public_encrypt_check(rsa_key, plain_txt, cipher_txt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    rsa_data.in = plain_txt->data;
    rsa_data.in_len = plain_txt->data_len;
    rsa_data.out = cipher_txt->data;
    rsa_data.out_buf_len = rsa_key->n_len;
    rsa_data.out_len = &cipher_txt->data_len;

    ret = mpi_rsa_get_crypto_attr(&rsa_scheme, scheme, sha_type);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_get_crypto_attr, ret);
        return ret;
    }

    ret = cipher_rsa_pubkey_len_check(scheme, rsa_key, plain_txt, sha_type);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_rsa_keylen_check, ret);
        return ret;
    }
    (td_void)memset_s(&key, sizeof(key), 0, sizeof(key));
    key.public = TD_TRUE;
    key.klen   = rsa_key->n_len;
    key.n      = rsa_key->n;

    cipher_get_pub_exponent(&key, rsa_key);

    ret = sys_rsa_encrypt(rsa_scheme, &key, &rsa_data);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_rsa_encrypt, ret);
    }
    (td_void)memset_s(&key, sizeof(key), 0, sizeof(key));
    return ret;
}

td_s32 ot_mpi_cipher_rsa_private_decrypt(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_private_key *rsa_key,
    const ot_cipher_common_data *cipher_data, ot_cipher_common_data *plain_data)
{
    td_s32 ret;
    cryp_rsa_key key;
    sys_rsa_crypto_data crypto_data = {0};
    ot_cipher_rsa_encrypt_scheme rsa_scheme = OT_CIPHER_RSA_ENCRYPT_SCHEME_BUTT;

    ret = mpi_rsa_chk_private_key(rsa_key);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_chk_private_key, ret);
        return ret;
    }

    chk_ptr_err_return(plain_data);
    chk_ptr_err_return(plain_data->data);
    chk_param_err_return(plain_data->data_len == 0);

    chk_ptr_err_return(cipher_data);
    chk_ptr_err_return(cipher_data->data);
    chk_param_err_return((cipher_data->data_len == 0) || (cipher_data->data_len != rsa_key->n_len));

    crypto_data.in = cipher_data->data;
    crypto_data.in_len = cipher_data->data_len;
    crypto_data.out = plain_data->data;
    crypto_data.out_buf_len = (plain_data->data_len < rsa_key->n_len) ? plain_data->data_len : rsa_key->n_len;
    crypto_data.out_len = &plain_data->data_len;

    ret = mpi_rsa_get_crypto_attr(&rsa_scheme, scheme, sha_type);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_get_crypto_attr, ret);
        return ret;
    }

    mpi_rsa_init_private_key(&key, rsa_key);

    ret = sys_rsa_decrypt(rsa_scheme, &key, &crypto_data);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_rsa_decrypt, ret);
    }
    (td_void)memset_s(&key, sizeof(key), 0, sizeof(key));
    return ret;
}

td_s32 ot_mpi_cipher_rsa_private_encrypt(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_private_key *rsa_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt)
{
    td_s32 ret;
    cryp_rsa_key key;
    sys_rsa_crypto_data rsa_data = {0};
    ot_cipher_rsa_encrypt_scheme rsa_scheme = OT_CIPHER_RSA_ENCRYPT_SCHEME_BUTT;

    ret = mpi_rsa_chk_private_key(rsa_key);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_chk_private_key, ret);
        return ret;
    }

    chk_ptr_err_return(plain_txt);
    chk_ptr_err_return(plain_txt->data);
    chk_param_err_return(plain_txt->data_len == 0);

    chk_ptr_err_return(cipher_txt);
    chk_ptr_err_return(cipher_txt->data);
    chk_param_err_return((cipher_txt->data_len == 0) || (cipher_txt->data_len < rsa_key->n_len));

    rsa_data.in = plain_txt->data;
    rsa_data.in_len = plain_txt->data_len;
    rsa_data.out = cipher_txt->data;
    rsa_data.out_buf_len = rsa_key->n_len;
    rsa_data.out_len = &cipher_txt->data_len;

    ret = mpi_rsa_get_crypto_attr(&rsa_scheme, scheme, sha_type);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_get_crypto_attr, ret);
        return ret;
    }
    ret = cipher_rsa_prikey_len_check(scheme, rsa_key, plain_txt, sha_type);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_rsa_keylen_check, ret);
        return ret;
    }

    mpi_rsa_init_private_key(&key, rsa_key);

    ret = sys_rsa_encrypt(rsa_scheme, &key, &rsa_data);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_rsa_encrypt, ret);
    }
    (td_void)memset_s(&key, sizeof(key), 0, sizeof(key));
    return ret;
}

td_s32 ot_mpi_cipher_rsa_public_decrypt(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt)
{
    td_s32 ret;
    cryp_rsa_key key;

    sys_rsa_crypto_data rsa_data = {0};
    ot_cipher_rsa_encrypt_scheme rsa_scheme = OT_CIPHER_RSA_ENCRYPT_SCHEME_BUTT;

    ret = cipher_rsa_public_decrypt_check(rsa_key, cipher_txt, plain_txt);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_rsa_public_decrypt_check, ret);
        return ret;
    }
    rsa_data.in = cipher_txt->data;
    rsa_data.in_len = cipher_txt->data_len;
    rsa_data.out = plain_txt->data;
    rsa_data.out_buf_len = (plain_txt->data_len < rsa_key->n_len) ? plain_txt->data_len : rsa_key->n_len;
    rsa_data.out_len = &plain_txt->data_len;

    ret = mpi_rsa_get_crypto_attr(&rsa_scheme, scheme, sha_type);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_get_crypto_attr, ret);
        return ret;
    }

    (td_void)memset_s(&key, sizeof(key), 0, sizeof(key));
    key.public  = TD_TRUE;
    key.klen    = rsa_key->n_len;
    key.n       = rsa_key->n;
    cipher_get_pub_exponent(&key, rsa_key);

    ret = sys_rsa_decrypt(rsa_scheme, &key, &rsa_data);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_rsa_decrypt, ret);
    }
    (td_void)memset_s(&key, sizeof(key), 0, sizeof(key));
    return ret;
}

static td_s32 mpi_cipher_hash(ot_cipher_hash_type sha_type,
    const td_u8 *in_data, td_u32 in_len, td_u8 *hash_data, td_u32 hash_len)
{
    td_s32 ret;
    td_handle hash_id;

    ret = sys_hash_start(&hash_id, sha_type, TD_NULL, 0);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_hash_start, ret);
        return ret;
    }

    ret = sys_hash_update(hash_id, in_data, in_len, HASH_CHUNCK_SRC_USER);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_hash_update, ret);
        return ret;
    }

    ret = sys_hash_finish(hash_id, hash_data, hash_len);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_hash_finish, ret);
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_rsa_sign(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_private_key *rsa_key,
    const ot_cipher_sign_in_data *rsa_data, ot_cipher_common_data *sign_data)
{
    td_s32 ret;
    cryp_rsa_key key;
    td_u8 result[HASH_RESULT_MAX_SIZE] = {0};
    sys_rsa_sign_data rsa_sign_data;
    ot_cipher_rsa_sign_scheme rsa_scheme = OT_CIPHER_RSA_SIGN_SCHEME_BUTT;

    ret = mpi_rsa_chk_sign_param(rsa_key, rsa_data, sign_data);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_chk_sign_param, ret);
        return ret;
    }

    (td_void)memset_s(&rsa_sign_data, sizeof(rsa_sign_data), 0, sizeof(rsa_sign_data));
    ret = mpi_rsa_get_sign_attr(&rsa_scheme, &rsa_sign_data.hash_len, scheme, sha_type);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_get_sign_attr, ret);
        return ret;
    }

    /* hash value of context, if OT_CIPHER_SIGN_TYPE_HASH, compute result = Hash(in_data). */
    if (rsa_data->sign_type == OT_CIPHER_SIGN_TYPE_MSG) {
        ret = mpi_cipher_hash(sha_type, rsa_data->input, rsa_data->input_len, result, sizeof(result));
        if (ret != TD_SUCCESS) {
            print_func_errno(mpi_cipher_hash, ret);
            return ret;
        }
        rsa_sign_data.hash = result;
    } else if (rsa_data->sign_type == OT_CIPHER_SIGN_TYPE_HASH) {
        rsa_sign_data.hash = rsa_data->input;
        if (rsa_data->input_len != rsa_sign_data.hash_len) {
            ot_cipher_error("Invalid input len %u\n", rsa_data->input_len);
            return OT_ERR_CIPHER_INVALID_PARAM;
        }
    } else {
        ot_cipher_error("Invalid sign type %d! should be in [%d, %d]!\n", rsa_data->sign_type,
            OT_CIPHER_SIGN_TYPE_MSG, OT_CIPHER_SIGN_TYPE_HASH);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }
    rsa_sign_data.sign = sign_data->data;
    rsa_sign_data.sign_len = rsa_key->n_len;

    mpi_rsa_init_private_key(&key, rsa_key);

    ret = sys_rsa_sign_hash(rsa_scheme, &key, &rsa_sign_data);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_rsa_sign_hash, ret);
    }
    sign_data->data_len = rsa_sign_data.sign_len;
    (td_void)memset_s(&key, sizeof(key), 0, sizeof(key));
    return ret;
}

td_s32 ot_mpi_cipher_rsa_verify(ot_cipher_rsa_scheme scheme,
    ot_cipher_hash_type sha_type, const ot_cipher_rsa_public_key *rsa_key,
    const ot_cipher_sign_in_data *rsa_data, const ot_cipher_common_data *sign_data)
{
    td_s32 ret;
    cryp_rsa_key key;

    td_u8 hash_result[HASH_RESULT_MAX_SIZE] = {0};
    sys_rsa_sign_data sys_data;
    ot_cipher_rsa_sign_scheme rsa_scheme = OT_CIPHER_RSA_SIGN_SCHEME_BUTT;

    ret = mpi_rsa_chk_verify_param(rsa_key, rsa_data, sign_data);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_chk_verify_param, ret);
        return ret;
    }

    (td_void)memset_s(&sys_data, sizeof(sys_data), 0, sizeof(sys_data));
    ret = mpi_rsa_get_sign_attr(&rsa_scheme, &sys_data.hash_len, scheme, sha_type);
    if (ret != TD_SUCCESS) {
        print_func_errno(mpi_rsa_get_sign_attr, ret);
        return ret;
    }

    /* hash value of context, if OT_CIPHER_SIGN_TYPE_HASH, compute hash_result = Hash(in_data). */
    if (rsa_data->sign_type == OT_CIPHER_SIGN_TYPE_MSG) {
        ret = mpi_cipher_hash(sha_type, rsa_data->input, rsa_data->input_len, hash_result, sizeof(hash_result));
        if (ret != TD_SUCCESS) {
            print_func_errno(mpi_cipher_hash, ret);
            return ret;
        }
        sys_data.hash = hash_result;
    } else if (rsa_data->sign_type == OT_CIPHER_SIGN_TYPE_HASH) {
        sys_data.hash = rsa_data->input;
        if (rsa_data->input_len != sys_data.hash_len) {
            ot_cipher_error("Invalid input len %u\n", rsa_data->input_len);
            return OT_ERR_CIPHER_INVALID_PARAM;
        }
    } else {
        ot_cipher_error("Invalid sign type %d! should be in [%d, %d]!\n", rsa_data->sign_type,
            OT_CIPHER_SIGN_TYPE_MSG, OT_CIPHER_SIGN_TYPE_HASH);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }
    sys_data.sign = sign_data->data;
    sys_data.sign_len = sign_data->data_len;

    (td_void)memset_s(&key, sizeof(key), 0, sizeof(key));
    key.public = TD_TRUE;
    key.klen   = rsa_key->n_len;
    key.n      = rsa_key->n;
    cipher_get_pub_exponent(&key, rsa_key);
    ret = sys_rsa_verify_hash(rsa_scheme, &key, &sys_data);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_rsa_verify_hash, ret);
    }

    (td_void)memset_s(&key, sizeof(key), 0, sizeof(key));
    return ret;
}

td_s32 ot_mpi_cipher_sm2_sign(const ot_cipher_sm2_sign *sm2_sign,
    const ot_cipher_sign_in_data *sm2_data, ot_cipher_sm2_sign_data *sign_data)
{
    td_s32 ret;
    ot_cipher_common_data msg;

    func_enter();

    chk_ptr_err_return(sm2_sign);
    chk_ptr_err_return(sm2_sign->id);
    chk_param_err_return(sm2_sign->id_len > SM2_ID_MAX_LEN);

    chk_ptr_err_return(sm2_data);
    chk_ptr_err_return(sm2_data->input);
    chk_param_err_return(sm2_data->sign_type != OT_CIPHER_SIGN_TYPE_MSG);
    chk_param_err_return(sm2_data->input_len == 0);

    chk_ptr_err_return(sign_data);

    msg.data = sm2_data->input;
    msg.data_len = sm2_data->input_len;

    ret = sys_sm2_sign(sm2_sign, &msg, sign_data);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_sm2_sign, ret);
        return ret;
    }

    func_exit();
    return ret;
}

td_s32 ot_mpi_cipher_sm2_verify(const ot_cipher_sm2_verify *sm2_verify,
    const ot_cipher_sign_in_data *sm2_data, const ot_cipher_sm2_sign_data *sign_data)
{
    td_s32 ret;
    ot_cipher_common_data msg;

    func_enter();

    chk_ptr_err_return(sm2_verify);
    chk_ptr_err_return(sm2_verify->id);
    chk_param_err_return(sm2_verify->id_len > SM2_ID_MAX_LEN);

    chk_ptr_err_return(sm2_data);
    chk_ptr_err_return(sm2_data->input);
    chk_param_err_return(sm2_data->sign_type != OT_CIPHER_SIGN_TYPE_MSG);
    chk_param_err_return(sm2_data->input_len == 0);

    chk_ptr_err_return(sign_data);

    msg.data = sm2_data->input;
    msg.data_len = sm2_data->input_len;

    ret = sys_sm2_verify(sm2_verify, &msg, sign_data);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_sm2_verify, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_sm2_encrypt(const ot_cipher_sm2_public_key *sm2_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(sm2_key);

    chk_ptr_err_return(plain_txt);
    chk_ptr_err_return(plain_txt->data);
    chk_param_err_return(plain_txt->data_len == 0);

    chk_ptr_err_return(cipher_txt);
    chk_ptr_err_return(cipher_txt->data);
    chk_param_err_return(cipher_txt->data_len == 0);

    ret = sys_sm2_encrypt(sm2_key, plain_txt, cipher_txt);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_sm2_encrypt, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_sm2_decrypt(const ot_cipher_sm2_private_key *sm2_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(sm2_key);

    chk_ptr_err_return(plain_txt);
    chk_ptr_err_return(plain_txt->data);
    chk_param_err_return(plain_txt->data_len == 0);

    chk_ptr_err_return(cipher_txt);
    chk_ptr_err_return(cipher_txt->data);
    chk_param_err_return(cipher_txt->data_len == 0);

    ret = sys_sm2_decrypt(sm2_key, cipher_txt, plain_txt);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_sm2_decrypt, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 ot_mpi_keyslot_create(const ot_keyslot_attr *attr, td_handle *keyslot)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(attr);
    chk_ptr_err_return(keyslot);

    ret = sys_keyslot_create(attr, keyslot);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_keyslot_create, ret);
        return ret;
    }

    func_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_keyslot_destroy(td_handle keyslot)
{
    td_s32 ret;

    func_enter();

    ret = sys_keyslot_destroy(keyslot);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_keyslot_destroy, ret);
        return ret;
    }

    func_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_attach(td_handle cipher, td_handle keyslot)
{
    td_s32 ret;

    func_enter();

    ret = sys_cipher_attach(cipher, keyslot);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_cipher_attach, ret);
        return ret;
    }

    func_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_cipher_detach(td_handle cipher, td_handle keyslot)
{
    td_s32 ret;

    func_enter();

    ret = sys_cipher_detach(cipher, keyslot);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_cipher_detach, ret);
        return ret;
    }

    func_exit();

    return TD_SUCCESS;
}

/** @} */ /** <!-- ==== Compat Code end ==== */
