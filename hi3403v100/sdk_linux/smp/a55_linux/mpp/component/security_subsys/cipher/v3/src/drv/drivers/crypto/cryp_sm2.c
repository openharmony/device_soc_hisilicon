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
#include "drv_pke.h"
#include "cryp_sm2.h"
#include "cryp_trng.h"
#include "ext_alg.h"

#ifdef CHIP_SM2_SUPPORT

#define SM2_SIGN_BUF_CNT          18
#define SM2_GET_RANDNUM_WAITDONE  0x1000
#define SM2_TRY_CNT               8

#ifdef CHIP_PKE_SUPPORT
/* ! Define the constant value */
static const td_u8 g_sm2n1[SM2_LEN_IN_BYTE] = {
    0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x72, 0x03, 0xDF, 0x6B, 0x21, 0xC6, 0x05, 0x2B, 0x53, 0xBB, 0xF4, 0x09, 0x39, 0xD5, 0x41, 0x22
};

static const td_u8 g_sm2_p[SM2_LEN_IN_BYTE] = {
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03
};

static const td_u8 g_sm2_n[SM2_LEN_IN_BYTE] = {
    0x1E, 0xB5, 0xE4, 0x12, 0xA2, 0x2B, 0x3D, 0x3B, 0x62, 0x0F, 0xC8, 0x4C, 0x3A, 0xFF, 0xE0, 0xD4,
    0x34, 0x64, 0x50, 0x4A, 0xDE, 0x6F, 0xA2, 0xFA, 0x90, 0x11, 0x92, 0xAF, 0x7C, 0x11, 0x4F, 0x20
};

static const td_u8 g_sm21[SM2_LEN_IN_BYTE] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
};

static const td_u8 g_sm20[SM2_LEN_IN_BYTE] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const td_u8 g_sm2ma[SM2_LEN_IN_BYTE] = {
    0xFF, 0xFF, 0xFF, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC
};

static const td_u8 g_sm2mb[SM2_LEN_IN_BYTE] = {
    0x24, 0x0F, 0xE1, 0x88, 0xBA, 0x20, 0xE2, 0xC8, 0x52, 0x79, 0x81, 0x50, 0x5E, 0xA5, 0x1C, 0x3C,
    0x71, 0xCF, 0x37, 0x9A, 0xE9, 0xB5, 0x37, 0xAB, 0x90, 0xD2, 0x30, 0x63, 0x2B, 0xC0, 0xDD, 0x42
};

#define drv_sm2_add_mod(a, b, p, out) \
    drv_pke_add_mod((td_u8 *)a, (td_u8 *)b, (td_u8 *)p, (td_u8 *)out, SM2_LEN_IN_BYTE)

#define drv_sm2_sub_mod(a, b, p, out) \
    drv_pke_sub_mod((td_u8 *)a, (td_u8 *)b, (td_u8 *)p, (td_u8 *)out, SM2_LEN_IN_BYTE)

#define drv_sm2_mul_modp(a, b, c, private) \
    drv_pke_mul_mod((td_u8 *)a, (td_u8 *)b, (td_u8 *)sm2_id->p, (td_u8 *)c, SM2_LEN_IN_BYTE, private)

#define drv_sm2_mul_modn(a, b, c, private) \
    drv_pke_mul_mod((td_u8 *)a, (td_u8 *)b, (td_u8 *)sm2_id->n, (td_u8 *)c, SM2_LEN_IN_BYTE, private)

#define drv_sm2_inv_mod(a, p, c) \
    drv_pke_inv_mod((td_u8 *)a, (td_u8 *)p, (td_u8 *)c, SM2_LEN_IN_BYTE)

#define drv_sm2_mod(a, p, c) \
    drv_pke_mod((td_u8 *)a, (td_u8 *)p, (td_u8 *)c, SM2_LEN_IN_BYTE)

#define drv_sm2_mul(a, b, c) \
    drv_pke_mul((td_u8 *)a, (td_u8 *)b, (td_u8 *)c, SM2_LEN_IN_BYTE)

#define drv_sm2_mul_dot(point_p, point_r, k, klen, private) \
    drv_pke_mul_dot(point_p, point_r, (td_u8 *)k, klen, private, sm2_id)

#define drv_sm2_add_dot(sx, sy, rx, ry, cx, cy) \
    drv_pke_add_dot((td_u8 *)sx, (td_u8 *)sy, (td_u8 *)rx, (td_u8 *)ry, \
    (td_u8 *)cx, (td_u8 *)cy, SM2_LEN_IN_BYTE, sm2_id)

#define cryp_sm2_get_randnum(a, b) cryp_pke_get_randnum(a, b, SM2_LEN_IN_BYTE)
#define cryp_sm2_is_zero(a)        cryp_pke_is_zero(a, SM2_LEN_IN_BYTE)
#define cryp_sm2_rang_check(a)     cryp_pke_range_check(a, g_sm2n1, SM2_LEN_IN_BYTE)
#endif

/* sm2 function list */
sm2_func g_sm2_descriptor;

/* ! rsa mutex */
static CRYPTO_MUTEX g_sm2_mutex;

#define kapi_sm2_lock()                  \
    do { \
        ret = crypto_mutex_lock(&g_sm2_mutex); \
        if (ret != TD_SUCCESS) {             \
            ot_cipher_error("error, sm2 lock failed\n");    \
            func_exit();               \
            return ret; \
        } \
    } while (0)

#define kapi_sm2_unlock() crypto_mutex_unlock(&g_sm2_mutex)

typedef struct {
    td_u8 *buf;
    td_u8 *ee;
    td_u8 *rk;
    td_u8 *my;
    td_u8 *mr;
    td_u8 *mk;
    td_u8 *mrk;
    td_u8 *mrky;
    td_u8 *ta;
    td_u8 *m_ta;
    td_u8 *m_ty;
    td_u8 *minv;
    td_u8 *mrk1;
    td_u8 *mrky1;
    td_u8 *m_tz;
    td_u8 *ms;
    td_u8 *k;
} sm2_buf_t;

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Code *****************************/
/** \addtogroup      sm2 drivers */
/** @{ */ /** <!-- [sm2] */

#ifdef CHIP_PKE_SUPPORT

static td_s32 cryp_sm2_buf_init(sm2_buf_t *sm2_buf, ecc_point_t *point_r)
{
    td_u32 id = 0;

    sm2_buf->buf = (td_u8 *)crypto_calloc(SM2_SIGN_BUF_CNT, SM2_LEN_IN_BYTE);
    if (sm2_buf->buf == TD_NULL) {
        print_func_errno(crypto_calloc, OT_ERR_CIPHER_FAILED_MEM);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    point_r->x = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    point_r->y = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->ee = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->rk = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->my = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->mr = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->mk = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->mrk = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->mrky = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->ta = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->m_ta = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->m_ty = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->minv = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->mrk1 = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->mrky1 = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->m_tz = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->ms = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;
    sm2_buf->k = sm2_buf->buf + SM2_LEN_IN_BYTE * id++;

    return TD_SUCCESS;
}

/***************************************
  1. generate a randnum k, k[1,n-1],
  2. (x1,y1)=[k]G,
  3. r=(e+x1)mod n, if r=0 or r+k=n, return step 1
  4. s=((1+dA)^-1 (k-rdA))mod n
****************************************/
static td_s32 cryp_sm2_sign_prepare_r(sm2_sign_t *sign, const td_u32 *e, const ecc_param_t *sm2_id,
    sm2_buf_t *sm2_buf, ecc_point_t *point_r, td_bool *need_retry)
{
    td_s32 ret;
    td_u32 i;
    td_u32 j = 0;

    *need_retry = TD_FALSE;
    for (i = 0; i < SM2_TRY_CNT; i++) {
        cryp_sm2_get_randnum(sm2_buf->buf + SM2_LEN_IN_WROD * i, g_sm2n1);
    }
    chk_func_err_goto(cryp_trng_get_random(&j, -1));
    j %= SM2_TRY_CNT;
    (td_void)memcpy_s(sm2_buf->k, SM2_LEN_IN_BYTE, sm2_buf->buf + SM2_LEN_IN_WROD * j, SM2_LEN_IN_BYTE);

    chk_func_err_goto(drv_sm2_mul_dot(&sm2_id->g, point_r, sm2_buf->k, SM2_LEN_IN_BYTE, TD_TRUE));
    chk_func_err_goto(drv_sm2_add_mod(e, g_sm20, sm2_id->n, sm2_buf->ee));
    chk_func_err_goto(drv_sm2_add_mod(sm2_buf->ee, point_r->x, sm2_id->n, sign->r));
    if (cryp_sm2_is_zero(sign->r)) {
        *need_retry = TD_TRUE;
        return TD_SUCCESS;
    }

    chk_func_err_goto(drv_sm2_add_mod(sign->r, sm2_buf->k, sm2_id->n, sm2_buf->rk));
    if (cryp_sm2_is_zero(sm2_buf->rk)) {
        *need_retry = TD_TRUE;
        return TD_SUCCESS;
    }

exit__:
    return ret;
}

static td_s32 cryp_sm2_sign_blind(const sm2_sign_t *sign, const ecc_param_t *sm2_id, sm2_buf_t *sm2_buf)
{
    td_s32 ret;

    cryp_sm2_get_randnum(sm2_buf->my, g_sm2n1);
    chk_func_err_goto(drv_sm2_mul_modn(sign->r, g_sm2_n, sm2_buf->mr, TD_FALSE));
    chk_func_err_goto(drv_sm2_mul_modn(sm2_buf->k, g_sm2_n, sm2_buf->mk, TD_FALSE));
    chk_func_err_goto(drv_sm2_add_mod(sm2_buf->mr, sm2_buf->mk, sm2_id->n, sm2_buf->mrk));
    chk_func_err_goto(drv_sm2_mul_modn(sm2_buf->mrk, sm2_buf->my, sm2_buf->mrky, TD_FALSE));
    chk_func_err_goto(drv_sm2_mul_modn(sm2_buf->my, sign->d, sm2_buf->ta, TD_TRUE));
    chk_func_err_goto(drv_sm2_mul_modn(sm2_buf->ta, g_sm2_n, sm2_buf->m_ta, TD_FALSE));
    chk_func_err_goto(drv_sm2_add_mod(sm2_buf->m_ta, sm2_buf->my, sm2_id->n, sm2_buf->m_ty));
    chk_func_err_goto(drv_sm2_inv_mod(sm2_buf->m_ty, sm2_id->n, sm2_buf->minv));
    chk_func_err_goto(drv_sm2_add_mod(sm2_buf->mr, sm2_buf->mk, sm2_id->n, sm2_buf->mrk1));
    chk_func_err_goto(drv_sm2_mul_modn(sm2_buf->mrk1, sm2_buf->my, sm2_buf->mrky1, TD_FALSE));
    if (memcmp(sm2_buf->mrky1, sm2_buf->mrky, SM2_LEN_IN_BYTE) != 0) {
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

exit__:
    return ret;
}

static td_s32 cryp_sm2_sign_finish(sm2_sign_t *sign, const ecc_param_t *sm2_id,
    sm2_buf_t *sm2_buf, td_bool *need_retry)
{
    td_s32 ret;

    *need_retry = TD_FALSE;
    chk_func_err_goto(drv_sm2_mul_modn(sm2_buf->mrky, sm2_buf->minv, sm2_buf->m_tz, TD_FALSE));
    chk_func_err_goto(drv_sm2_sub_mod(sm2_buf->m_tz, sm2_buf->mr, sm2_id->n, sm2_buf->ms));
    if (cryp_sm2_is_zero(sm2_buf->ms)) {
        *need_retry = TD_TRUE;
        return TD_SUCCESS;
    }

    chk_func_err_goto(drv_sm2_mul_modn(sm2_buf->ms, g_sm21, sign->s, TD_FALSE));
    chk_func_err_goto(drv_sm2_add_mod(sign->r, sm2_buf->k, sm2_id->n, sm2_buf->rk));
    if (cryp_sm2_is_zero(sm2_buf->rk) != 0) {
        *need_retry = TD_TRUE;
    }

exit__:
    return ret;
}

static td_s32 cryp_sm2_sign(sm2_sign_t *sign,
    const td_u32 *e, td_u32 e_len, const ecc_param_t *sm2_id)
{
    td_s32 ret;
    sm2_buf_t sm2_buf = {0};
    ecc_point_t point_r = {TD_NULL, TD_NULL, SM2_LEN_IN_BYTE};
    td_u32 re_try;
    td_bool need_retry;

    chk_param_err_return(e_len != SM2_LEN_IN_BYTE);

    /* d < n ? */
    if (memcmp(sign->d, sm2_id->n, SM2_LEN_IN_BYTE) >= 0) {
        print_func_errno(memcmp, OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    ret = cryp_sm2_buf_init(&sm2_buf, &point_r);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_sm2_k_init, ret);
        return ret;
    }

    kapi_sm2_lock();

    chk_func_err_goto(drv_pke_resume());

    for (re_try = 0; re_try < SM2_TRY_CNT; re_try++) {
        ret = cryp_sm2_sign_prepare_r(sign, e, sm2_id, &sm2_buf, &point_r, &need_retry);
        if (ret != TD_SUCCESS) {
            goto exit__;
        }
        if (need_retry == TD_TRUE) {
            continue;
        }
        ret = cryp_sm2_sign_blind(sign, sm2_id, &sm2_buf);
        if (ret != TD_SUCCESS) {
            goto exit__;
        }
        ret = cryp_sm2_sign_finish(sign, sm2_id, &sm2_buf, &need_retry);
        if (ret != TD_SUCCESS) {
            goto exit__;
        }
        if (need_retry == TD_TRUE) {
            continue;
        }
        break;
    }
    if (re_try == SM2_TRY_CNT) {
        ot_cipher_error("Error! K is Invalid!\n");
        ret = OT_ERR_CIPHER_INVALID_PARAM;
    }

exit__:
    drv_pke_clean_ram();
    drv_pke_suspend();
    (td_void)memset_s(sm2_buf.buf, SM2_LEN_IN_BYTE * SM2_SIGN_BUF_CNT, 0, SM2_LEN_IN_BYTE * SM2_SIGN_BUF_CNT);
    kapi_sm2_unlock();
    crypto_free(sm2_buf.buf);
    return ret;
}

/***************************************
  1. t=(r+s)mod n, if t==0, return fail
  2. (x1,y1)=[s]G+tP,
  3. r=(e+x1)mod n, if r==R, return pass
****************************************/
static td_s32 cryp_sm2_verify(const sm2_verify_t *verify,
    const td_u32 *e, td_u32 e_len, const ecc_param_t *sm2_id)
{
    td_s32 ret = TD_FAILURE;
    td_u8 t[SM2_LEN_IN_BYTE] = {0};
    td_u8 sgx[SM2_LEN_IN_BYTE] = {0};
    td_u8 sgy[SM2_LEN_IN_BYTE] = {0};
    td_u8 tpax[SM2_LEN_IN_BYTE] = {0};
    td_u8 tpay[SM2_LEN_IN_BYTE] = {0};
    td_u8 rx[SM2_LEN_IN_BYTE] = {0};
    td_u8 ry[SM2_LEN_IN_BYTE] = {0};
    td_u8 ee[SM2_LEN_IN_BYTE] = {0};
    td_u8 v[SM2_LEN_IN_BYTE] = {0};
    ecc_point_t point_sg = {sgx, sgy, SM2_LEN_IN_BYTE};
    ecc_point_t point_tpa = {tpax, tpay, SM2_LEN_IN_BYTE};
    ecc_point_t point_p = {(td_u8 *)verify->px, (td_u8 *)verify->py, SM2_LEN_IN_BYTE};

    chk_param_err_return(e_len != SM2_LEN_IN_BYTE);

    kapi_sm2_lock();

    chk_func_err_goto(drv_pke_resume());

    /* t: r + s mod n */
    chk_func_err_goto(cryp_sm2_rang_check((td_u8 *)verify->r)); /* *** 1<=r<=n-1 ** */
    chk_func_err_goto(cryp_sm2_rang_check((td_u8 *)verify->s)); /* *** 1<=s<=n-1 ** */
    chk_func_err_goto(drv_sm2_add_mod(verify->r, verify->s, sm2_id->n, t));

    /* t ?= 0 */
    if (cryp_sm2_is_zero(t)) {
        print_func_errno(cryp_sm2_is_zero, OT_ERR_CIPHER_INVALID_PARAM);
        ret = OT_ERR_CIPHER_INVALID_PARAM;
        goto exit__;
    }

    /* sG: s * G */
    chk_func_err_goto(drv_sm2_mul_dot(&sm2_id->g, &point_sg, verify->s, SM2_LEN_IN_BYTE, TD_FALSE));

    /* tPA: t * PA */
    chk_func_err_goto(drv_sm2_mul_dot(&point_p, &point_tpa, t, SM2_LEN_IN_BYTE, TD_FALSE));

    /* R: sG + tPA */
    chk_func_err_goto(drv_sm2_add_dot(sgx, sgy, tpax, tpay, rx, ry));

    /* e`: e + 0 mod n */
    chk_func_err_goto(drv_sm2_add_mod(e, g_sm20, sm2_id->n, ee));

    /* v: e` + Rx mod n */
    chk_func_err_goto(drv_sm2_add_mod(ee, rx, sm2_id->n, v));

    /* v ?= r */
    if (memcmp(verify->r, v, SM2_LEN_IN_BYTE) != 0) {
        print_func_errno(memcmp, OT_ERR_CIPHER_INVALID_PARAM);
        ret = OT_ERR_CIPHER_INVALID_PARAM;
    }

exit__:
    drv_pke_clean_ram();
    drv_pke_suspend();
    kapi_sm2_unlock();
    return ret;
}

/* Compute C1(x, y) = k * G(x, y), XY = k * P(x, y) */
static td_s32 cryp_sm2_encrypt(const ot_cipher_sm2_public_key *sm2_key,
    ecc_point_t *point_c1, ecc_point_t *point_xy, const ecc_param_t *sm2_id)
{
    td_s32 ret;
    td_u8 k[SM2_LEN_IN_BYTE] = {0};
    td_u32 retry = 0;
    ecc_point_t point_p = {(td_u8 *)sm2_key->px, (td_u8 *)sm2_key->py, SM2_LEN_IN_BYTE};

    /* t = 0 ? */
    if (cryp_sm2_is_zero(point_p.x) || cryp_sm2_is_zero(point_p.y)) {
        print_func_errno(cryp_sm2_is_zero, OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    kapi_sm2_lock();

    ret = drv_pke_resume();
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_sm2_is_zero, ret);
        kapi_sm2_unlock();
        return ret;
    }

    while (1) {
        if (retry++ > SM2_TRY_CNT) {
            ot_cipher_error("Error! K is Invalid!\n");
            ret = OT_ERR_CIPHER_INVALID_PARAM;
            goto exit__;
        }

        /* generate randnum k, 1<=k<=n-1 */
        cryp_sm2_get_randnum(k, g_sm2n1);

        /* *** C1=k*G *** */
        chk_func_err_goto(drv_sm2_mul_dot(&sm2_id->g, point_c1, k, SM2_LEN_IN_BYTE, TD_FALSE));

        /* *** c1 = 0 ? *** */
        if (cryp_sm2_is_zero(point_c1->x) || cryp_sm2_is_zero(point_c1->y)) {
            continue;
        }

        /* *** kPB = k * PB *** */
        chk_func_err_goto(drv_sm2_mul_dot(&point_p, point_xy, k, SM2_LEN_IN_BYTE, TD_FALSE));

        /* *** xy = 0 ? *** */
        if (cryp_sm2_is_zero(point_xy->x) || cryp_sm2_is_zero(point_xy->y)) {
            continue;
        }
        break;
    }

exit__:
    drv_pke_clean_ram();
    drv_pke_suspend();
    kapi_sm2_unlock();
    return ret;
}

/* check y^2=x^3+ax+b ? */
static td_s32 cryp_sm2_dot_check(ecc_point_t *point, td_u32 check, const ecc_param_t *sm2_id)
{
    td_s32 ret = TD_FAILURE;
    td_u8 mx[SM2_LEN_IN_BYTE] = {0};
    td_u8 my[SM2_LEN_IN_BYTE] = {0};
    td_u8 mx2[SM2_LEN_IN_BYTE] = {0};
    td_u8 mx3[SM2_LEN_IN_BYTE] = {0};
    td_u8 max[SM2_LEN_IN_BYTE] = {0};
    td_u8 mt[SM2_LEN_IN_BYTE] = {0};
    td_u8 ms[SM2_LEN_IN_BYTE] = {0};
    td_u8 my2[SM2_LEN_IN_BYTE] = {0};
    ecc_point_t point_m = {mx, my, SM2_LEN_IN_BYTE};

    /* xy = 0 ? */
    if (cryp_sm2_is_zero(point->x) || cryp_sm2_is_zero(point->y)) {
        ot_cipher_error("Error! X or Y is zero!\n");
        print_func_errno(cryp_sm2_is_zero, TD_TRUE);
        return TD_FAILURE;
    }

    ot_cipher_dbg("1. ma = a*P mod p\n");
    ot_cipher_dbg("2. mb = b*P mod p\n");
    ot_cipher_dbg("3. mx=C1x*P mod p\n");
    chk_func_err_goto(drv_sm2_mul_modp(point->x, g_sm2_p, mx, TD_FALSE));

    ot_cipher_dbg("4. my=C1y*P mod p\n");
    chk_func_err_goto(drv_sm2_mul_modp(point->y, g_sm2_p, my, TD_FALSE));

    ot_cipher_dbg("5. mx2=mx* mx mod p\n");
    chk_func_err_goto(drv_sm2_mul_modp(mx, mx, mx2, TD_FALSE));

    ot_cipher_dbg("6. mx3=mx2* mx mod p\n");
    chk_func_err_goto(drv_sm2_mul_modp(mx2, mx, mx3, TD_FALSE));

    ot_cipher_dbg("7. max=ma*mx mod p\n");
    chk_func_err_goto(drv_sm2_mul_modp(g_sm2ma, mx, max, TD_FALSE));

    ot_cipher_dbg("8. mt= mx3+max mod p\n");
    chk_func_err_goto(drv_sm2_add_mod(mx3, max, sm2_id->p, mt));

    ot_cipher_dbg("9. ms= mt+mb mod p\n");
    chk_func_err_goto(drv_sm2_add_mod(mt, g_sm2mb, sm2_id->p, ms));

    ot_cipher_dbg("10. my2=my*my mod p\n");
    chk_func_err_goto(drv_sm2_mul_modp(my, my, my2, TD_FALSE));

    ot_cipher_dbg("11. my2 ?= ms\n");
    if (memcmp(ms, my2, SM2_LEN_IN_BYTE) != 0) {
        print_func_errno(memcmp, OT_ERR_CIPHER_FAILED_DECRYPT);
        ret = OT_ERR_CIPHER_FAILED_DECRYPT;
        goto exit__;
    }

    ot_cipher_dbg("12. S= h * C1\n");
    if (check) {
        chk_func_err_goto(drv_sm2_mul_dot(point, &point_m, g_sm21, SM2_LEN_IN_BYTE, TD_FALSE));
    }

exit__:
    drv_pke_clean_ram();
    return ret;
}

/* Compute XY(x, y) = C1(x, y) * d */
static td_s32 cryp_sm2_decrypt(const ot_cipher_sm2_private_key *sm2_key,
    ecc_point_t *point_c1, ecc_point_t *point_xy, const ecc_param_t *sm2_id)
{
    td_s32 ret;
    td_u32 mx[SM2_LEN_IN_WROD] = {0};
    td_u32 my[SM2_LEN_IN_WROD] = {0};
    ecc_point_t point_m = {(td_u8 *)mx, (td_u8 *)my, SM2_LEN_IN_BYTE};

    chk_param_err_return(point_xy->len != SM2_LEN_IN_BYTE);

    /* c1 = 0 ? */
    if (cryp_sm2_is_zero(point_c1->x) || cryp_sm2_is_zero(point_c1->y)) {
        ot_cipher_error("Error! C1 is zero!\n");
        print_func_errno(cryp_sm2_is_zero, TD_TRUE);
        return TD_FAILURE;
    }

    /* d < n ? */
    if (memcmp(sm2_key->d, sm2_id->n, SM2_LEN_IN_BYTE) >= 0) {
        print_func_errno(memcmp, OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    kapi_sm2_lock();

    chk_func_err_goto(drv_pke_resume());

    /* check C1y^2=C1x^3+aC1x+b ? */
    chk_func_err_goto(cryp_sm2_dot_check(point_c1, TD_TRUE, sm2_id));

    /* *** Compute M(x,y) *** */
    chk_func_err_goto(drv_sm2_mul_dot(point_c1, &point_m, sm2_key->d, SM2_LEN_IN_BYTE, TD_TRUE));

    /* check C1y^2=C1x^3+aC1x+b ? */
    chk_func_err_goto(cryp_sm2_dot_check(point_c1, TD_TRUE, sm2_id));

    /* check My^2=Mx^3+aMx+b ? */
    chk_func_err_goto(cryp_sm2_dot_check(&point_m, TD_FALSE, sm2_id));

    (td_void)memcpy_s(point_xy->x, SM2_LEN_IN_BYTE, mx, SM2_LEN_IN_BYTE);
    (td_void)memcpy_s(point_xy->y, SM2_LEN_IN_BYTE, my, SM2_LEN_IN_BYTE);

exit__:
    (td_void)memset_s(mx, sizeof(mx), 0, sizeof(mx));
    (td_void)memset_s(my, sizeof(my), 0, sizeof(my));
    drv_pke_clean_ram();
    drv_pke_suspend();
    kapi_sm2_unlock();
    return ret;
}
#endif

int cryp_sm2_init(void)
{
    pke_capacity capacity;

    func_enter();

    crypto_mutex_init(&g_sm2_mutex);

    (td_void)memset_s(&g_sm2_descriptor, sizeof(g_sm2_descriptor), 0, sizeof(g_sm2_descriptor));
    (td_void)memset_s(&capacity, sizeof(capacity), 0, sizeof(capacity));

#ifdef CHIP_SM2_SUPPORT
    drv_pke_get_capacity(&capacity);

    /* replace the sm2 function of tomcrypt */
    if (capacity.sm2) {
        g_sm2_descriptor.sign = cryp_sm2_sign;
        g_sm2_descriptor.verify = cryp_sm2_verify;
        g_sm2_descriptor.encrypt = cryp_sm2_encrypt;
        g_sm2_descriptor.decrypt = cryp_sm2_decrypt;
        g_sm2_descriptor.genkey = TD_NULL;
    } else {
#endif
#ifdef SOFT_SM2_SUPPORT
        g_sm2_descriptor.sign = mbedtls_sm2_sign;
        g_sm2_descriptor.verify = mbedtls_sm2_verify;
        g_sm2_descriptor.encrypt = mbedtls_sm2_encrypt;
        g_sm2_descriptor.decrypt = mbedtls_sm2_decrypt;
        g_sm2_descriptor.genkey = mbedtls_sm2_gen_key;
#endif
#ifdef CHIP_SM2_SUPPORT
    }
#endif

#ifdef CHIP_PKE_SUPPORT
    {
        td_s32 ret;
        ret = drv_pke_init();
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_pke_init, ret);
            crypto_mutex_destroy(&g_sm2_mutex);
            return ret;
        }
    }
#endif

    func_exit();
    return TD_SUCCESS;
}

void cryp_sm2_deinit(void)
{
#ifdef CHIP_PKE_SUPPORT
    drv_pke_deinit();
#endif
    crypto_mutex_destroy(&g_sm2_mutex);
}

sm2_func *cryp_get_sm2_op(void)
{
    return &g_sm2_descriptor;
}
#endif

#ifdef CHIP_SM2_SUPPORT
void cryp_pke_get_randnum(td_u8 *randnum, const td_u8 *max, td_u32 klen)
{
    td_u32 i;
    td_u32 val = 0;
    td_s32 ret;

    for (i = 0; i < klen; i += WORD_WIDTH) {
        ret = cryp_trng_get_random(&val, -1);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_trng_get_random, ret);
        }
        if (memcpy_s(&randnum[i], klen - i, &val, WORD_WIDTH) != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        }
    }

    /* make sure randnum <= max */
    for (i = 0; i < klen; i++) {
        if (randnum[i] < max[i]) {
            break;
        }
        randnum[i] = max[i];
    }

    return;
}

/* check val whether zero or not */
td_u32 cryp_pke_is_zero(const td_void *val, td_u32 klen)
{
    td_u32 i;
    const td_u8 *p = val;

    for (i = 0; i < klen; i++) {
        if (p[i] != 0x00) {
            return TD_FALSE;
        }
    }
    return TD_TRUE;
}

/* check val whether less than max or not */
td_s32 cryp_pke_range_check(const td_u8 *val, const td_u8 *max, td_u32 klen)
{
    td_u32 i;

    if (cryp_pke_is_zero(val, klen)) {
        return OT_ERR_CIPHER_ILLEGAL_DATA;
    }

    for (i = 0; i < klen; i++) {
        if (val[i] < max[i]) {
            return TD_SUCCESS;
        } else if (val[i] > max[i]) {
            return OT_ERR_CIPHER_ILLEGAL_DATA;
        }
    }

    return TD_SUCCESS;
}
#endif

