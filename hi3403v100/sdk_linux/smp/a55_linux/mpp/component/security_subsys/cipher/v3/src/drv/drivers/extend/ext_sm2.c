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
#include "cryp_trng.h"
#include "cryp_sm2.h"
#include "cryp_rsa.h"

#ifdef SOFT_SM2_SUPPORT
#include "mbedtls/ecp.h"

/************************* Internal Structure Definition ********************/
/** \addtogroup      sm2 */
/** @{ */ /** <!-- [sm2] */

#define SM256R1_P \
    "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF"
#define SM256R1_A \
    "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC"
#define SM256R1_B \
    "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93"
#define SM256R1_GX \
    "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7"
#define SM256R1_GY \
    "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0"
#define SM256R1_N \
    "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123"

#define RADIX_HEX      16
#define KEY_MAX_TRIES  10

/**
* \brief          print a bignum string of  mbedtls*
 */
void mbedtls_mpi_print(const mbedtls_mpi *x, const char *name);

#define mpi_print(x) mbedtls_mpi_print(x, #x)

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Code *****************************/
/** \addtogroup      sm2 drivers */
/** @{ */ /** <!-- [sm2] */

/*
 * Make group available from embedded constants
 */
static int ecp_group_load_sm2(mbedtls_ecp_group *grp)
{
    int ret;

    chk_func_err_goto(mbedtls_mpi_read_string(&grp->P, RADIX_HEX, SM256R1_P));
    chk_func_err_goto(mbedtls_mpi_read_string(&grp->A, RADIX_HEX, SM256R1_A));
    chk_func_err_goto(mbedtls_mpi_read_string(&grp->B, RADIX_HEX, SM256R1_B));
    chk_func_err_goto(mbedtls_mpi_read_string(&grp->N, RADIX_HEX, SM256R1_N));
    chk_func_err_goto(mbedtls_mpi_read_string(&grp->G.X, RADIX_HEX, SM256R1_GX));
    chk_func_err_goto(mbedtls_mpi_read_string(&grp->G.Y, RADIX_HEX, SM256R1_GY));
    chk_func_err_goto(mbedtls_mpi_lset(&grp->G.Z, 1));

    grp->pbits = mbedtls_mpi_bitlen(&grp->P);
    grp->nbits = mbedtls_mpi_bitlen(&grp->N);

    grp->h = 0;

exit__:
    return (ret);
}

/*
 * Derive a suitable integer for group grp from a buffer of length len
 * SEC1 4.1.3 step 5 aka SEC1 4.1.4 step 3
 */
static int derive_mpi(const mbedtls_ecp_group *grp, mbedtls_mpi *x,
                      const unsigned char *buf, size_t blen)
{
    int ret;
    size_t n_size = (grp->nbits + BITS_IN_BYTE - 1) / BITS_IN_BYTE;
    size_t use_size = (blen > n_size) ? n_size : blen;

    chk_func_err_goto(mbedtls_mpi_read_binary(x, buf, use_size));
    if (use_size * BITS_IN_BYTE > grp->nbits) {
        chk_func_err_goto(mbedtls_mpi_shift_r(x, use_size * BITS_IN_BYTE - grp->nbits));
    }

    /* While at it, reduce modulo N */
    if (mbedtls_mpi_cmp_mpi(x, &grp->N) >= 0) {
        chk_func_err_goto(mbedtls_mpi_sub_mpi(x, x, &grp->N));
    }

exit__:
    return (ret);
}

typedef struct {
    mbedtls_ecp_group grp;
    mbedtls_ecp_point point_r;
    mbedtls_mpi k;
    mbedtls_mpi e;
    mbedtls_mpi rk;
    mbedtls_mpi md_a;
    mbedtls_mpi inv;
} sm2_sign_ctx;

static void sm2_sign_ctx_init(sm2_sign_ctx *ctx)
{
    mbedtls_mpi_init(&ctx->k);
    mbedtls_mpi_init(&ctx->e);
    mbedtls_mpi_init(&ctx->rk);
    mbedtls_mpi_init(&ctx->md_a);
    mbedtls_mpi_init(&ctx->inv);
    mbedtls_ecp_group_init(&ctx->grp);
    mbedtls_ecp_point_init(&ctx->point_r);
}

static void sm2_sign_ctx_free(sm2_sign_ctx *ctx)
{
    mbedtls_mpi_free(&ctx->k);
    mbedtls_mpi_free(&ctx->e);
    mbedtls_mpi_free(&ctx->rk);
    mbedtls_mpi_free(&ctx->md_a);
    mbedtls_mpi_free(&ctx->inv);
    mbedtls_ecp_point_free(&ctx->point_r);
    mbedtls_ecp_group_free(&ctx->grp);
}

static int sm2_sign_check_param(const mbedtls_mpi *d, const unsigned char *buf, size_t blen)
{
    if ((buf == NULL) || (blen == 0)) {
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }
    if (mbedtls_mpi_cmp_int(d, 0) == 0) {
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }
    return TD_SUCCESS;
}

static int sm2_sign_make_r(mbedtls_mpi *r, sm2_sign_ctx *ctx, const unsigned char *buf, size_t blen,
                           int (*f_rng)(td_void *, unsigned char *, size_t), td_void *p_rng,
                           int *key_tries, int *need_retry)
{
    int ret = TD_FAILURE;

    *need_retry = TD_FALSE;
    chk_func_err_goto(mbedtls_mpi_fill_random(&ctx->k, mbedtls_mpi_size(&ctx->grp.N), f_rng, p_rng));
    chk_func_err_goto(mbedtls_ecp_mul(&ctx->grp, &ctx->point_r, &ctx->k, &ctx->grp.G, NULL, NULL));
    chk_func_err_goto(derive_mpi(&ctx->grp, &ctx->e, buf, blen));
    chk_func_err_goto(mbedtls_mpi_add_mpi(r, &ctx->e, &ctx->point_r.X));
    chk_func_err_goto(mbedtls_mpi_mod_mpi(r, r, &ctx->grp.N));
    if ((*key_tries)++ > KEY_MAX_TRIES) {
        return MBEDTLS_ERR_ECP_RANDOM_FAILED;
    }

    if (mbedtls_mpi_cmp_int(r, 0) == 0) {
        *need_retry = TD_TRUE;
        return TD_SUCCESS;
    }
    chk_func_err_goto(mbedtls_mpi_add_mpi(&ctx->rk, r, &ctx->k));
    if (mbedtls_mpi_cmp_int(&ctx->rk, 0) == 0) {
        *need_retry = TD_TRUE;
    }

exit__:
    return ret;
}

static int sm2_sign_make_s(mbedtls_mpi *s, mbedtls_mpi *r, mbedtls_mpi *d,
                           sm2_sign_ctx *ctx, int *sign_tries)
{
    int ret = TD_FAILURE;

    chk_func_err_goto(mbedtls_mpi_mul_mpi(&ctx->md_a, r, d));
    chk_func_err_goto(mbedtls_mpi_sub_mpi(&ctx->md_a, &ctx->k, &ctx->md_a));
    chk_func_err_goto(mbedtls_mpi_add_int(&ctx->inv, d, 1));
    chk_func_err_goto(mbedtls_mpi_inv_mod(&ctx->inv, &ctx->inv, &ctx->grp.N));
    chk_func_err_goto(mbedtls_mpi_mul_mpi(s, &ctx->inv, &ctx->md_a));
    chk_func_err_goto(mbedtls_mpi_mod_mpi(s, s, &ctx->grp.N));
    if ((*sign_tries)++ > KEY_MAX_TRIES) {
        return MBEDTLS_ERR_ECP_RANDOM_FAILED;
    }

exit__:
    return ret;
}

int sm2_sign(mbedtls_mpi *r, mbedtls_mpi *s, mbedtls_mpi *d, /* Private Key */
             unsigned char *buf, size_t blen,
             int (*f_rng)(td_void *, unsigned char *, size_t), td_void *p_rng)
{
    int ret = TD_FAILURE;
    int key_tries = 0;
    int sign_tries = 0;
    int need_retry;
    sm2_sign_ctx ctx;

    ret = sm2_sign_check_param(d, buf, blen);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    sm2_sign_ctx_init(&ctx);

    chk_func_err_goto(ecp_group_load_sm2(&ctx.grp));

    do {
        ret = sm2_sign_make_r(r, &ctx, buf, blen, f_rng, p_rng, &key_tries, &need_retry);
        if (ret != TD_SUCCESS) {
            goto exit__;
        }
        if (need_retry == TD_TRUE) {
            continue;
        }
        ret = sm2_sign_make_s(s, r, d, &ctx, &sign_tries);
        if (ret != TD_SUCCESS) {
            goto exit__;
        }
    } while (mbedtls_mpi_cmp_int(s, 0) == 0);

exit__:
    sm2_sign_ctx_free(&ctx);
    return ret;
}

int sm2_verify(unsigned char *buf, size_t blen, mbedtls_ecp_point *q, /* Public key */
               mbedtls_mpi *r, mbedtls_mpi *s)
{
    int ret;
    mbedtls_mpi e;
    mbedtls_mpi t;
    mbedtls_mpi mpi_r;
    mbedtls_ecp_point p;
    mbedtls_ecp_group grp;

    chk_param_err_return(mbedtls_ecp_is_zero(q) == TD_TRUE);
    chk_param_err_return(mbedtls_mpi_cmp_int(r, 0) == 0);
    chk_param_err_return(mbedtls_mpi_cmp_int(s, 0) == 0);

    mbedtls_mpi_init(&e);
    mbedtls_mpi_init(&t);
    mbedtls_mpi_init(&mpi_r);
    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_point_init(&p);

    chk_func_err_goto(ecp_group_load_sm2(&grp));
    chk_func_err_goto(mbedtls_mpi_read_binary(&e, buf, blen));

    /*
     * Step 1: make sure r and s are in range 1..n-1
     */
    if (mbedtls_mpi_cmp_int(r, 1) < 0 || mbedtls_mpi_cmp_mpi(r, &grp.N) >= 0 ||
        mbedtls_mpi_cmp_int(s, 1) < 0 || mbedtls_mpi_cmp_mpi(s, &grp.N) >= 0) {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto exit__;
    }

    /*
     * Additional precaution: make sure Q is valid
     */
    chk_func_err_goto(mbedtls_ecp_check_pubkey(&grp, q));

    /*
     * Step 3: derive MPI from hashed message
     */
    chk_func_err_goto(derive_mpi(&grp, &e, buf, blen));

    /*
     * Step 4: t = (r+s) mod n
     */
    chk_func_err_goto(mbedtls_mpi_add_mpi(&t, r, s));
    chk_func_err_goto(mbedtls_mpi_mod_mpi(&t, &t, &grp.N));

    /*
     * Step 5: P = [s]G + [t]Q
     *
     * Since we're not using any secret data, no need to pass a RNG to
     * mbedtls_ecp_mul() for countermesures.
     */
    chk_func_err_goto(mbedtls_ecp_muladd(&grp, &p, s, &grp.G, &t, q));

    if (mbedtls_ecp_is_zero(&p)) {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto exit__;
    }

    /*
     * Step 6: convert xR to an integer (no-op)
     * Step 7: R = (e+Px) mod n
     */
    chk_func_err_goto(mbedtls_mpi_add_mpi(&mpi_r, &e, &p.X));
    chk_func_err_goto(mbedtls_mpi_mod_mpi(&mpi_r, &mpi_r, &grp.N));

    /*
     * Step 8: check if v (that is, mpi_r.X) is equal to r
     */
    if (mbedtls_mpi_cmp_mpi(&mpi_r, r) != 0) {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
    }

exit__:
    mbedtls_mpi_free(&e);
    mbedtls_mpi_free(&t);
    mbedtls_mpi_free(&mpi_r);
    mbedtls_ecp_point_free(&p);
    mbedtls_ecp_group_free(&grp);
    return ret;
}

/* q: Public key; k[1,n-1], R = [k]G, Z = [k]Q */
static int sm2_encrypt(mbedtls_ecp_point *z, mbedtls_ecp_point *q,
    mbedtls_ecp_point *r, int (*f_rng)(td_void *, unsigned char *, size_t), td_void *p_rng)
{
    int ret;
    int key_tries = 0;
    mbedtls_mpi k;
    mbedtls_ecp_group grp;

    func_enter();

    if (mbedtls_ecp_is_zero(q)) {
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    mbedtls_mpi_init(&k);
    mbedtls_ecp_group_init(&grp);

    chk_func_err_goto(ecp_group_load_sm2(&grp));

    do {
        if (key_tries++ > SM2_TRY_CNT) {
            ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
            goto exit__;
        }

        /*
         * Steps 1: generate a randnum k, 1<=k<=n-1
         */
        chk_func_err_goto(mbedtls_mpi_fill_random(&k, mbedtls_mpi_size(&grp.N), f_rng, p_rng));

        /*
         * Step 2: compute R = [k]G
         */
        chk_func_err_goto(mbedtls_ecp_mul(&grp, r, &k, &grp.G, NULL, NULL));
        if (mbedtls_ecp_is_zero(r)) {
            continue;
        }

        /*
         * Additional precaution: make sure Q is valid
         */
        chk_func_err_goto(mbedtls_ecp_check_pubkey(&grp, q));

        /*
         * Step 3: compute Z = [k]Q
         */
        chk_func_err_goto(mbedtls_ecp_mul(&grp, z, &k, q, NULL, NULL));
    } while (mbedtls_ecp_is_zero(z));

exit__:
    mbedtls_mpi_free(&k);
    mbedtls_ecp_group_free(&grp);

    func_exit();

    return (ret);
}

/*
 * d: Private key
 * Z = [d]R
 */
static int sm2_decrypt(mbedtls_ecp_point *z, mbedtls_mpi *d, mbedtls_ecp_point *r)
{
    int ret;
    mbedtls_mpi u1;
    mbedtls_mpi u2;
    mbedtls_ecp_group grp;

    func_enter();

    if (mbedtls_mpi_cmp_int(d, 0) == 0) {
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    if (mbedtls_ecp_is_zero(r)) {
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    mbedtls_mpi_init(&u1);
    mbedtls_mpi_init(&u2);
    mbedtls_ecp_group_init(&grp);

    chk_func_err_goto(ecp_group_load_sm2(&grp));

    chk_func_err_goto(mbedtls_mpi_mul_mpi(&u1, &r->X, &r->X));   /* *** u1 = Rx*Rx *** */
    chk_func_err_goto(mbedtls_mpi_mul_mpi(&u1, &u1, &r->x));     /* *** u1 = u1 * Rx *** */
    chk_func_err_goto(mbedtls_mpi_mul_mpi(&u2, &grp.A, &r->X));  /* *** u2 = aRx *** */
    chk_func_err_goto(mbedtls_mpi_add_mpi(&u1, &u1, &u2));       /* *** u1 = u1 + u2 *** */
    chk_func_err_goto(mbedtls_mpi_add_mpi(&u1, &u1, &grp.B));    /* *** u1 = u1 + b *** */
    chk_func_err_goto(mbedtls_mpi_mul_mpi(&u2, &r->Y, &r->Y));   /* *** u2 = Ry*Ry *** */
    chk_func_err_goto(mbedtls_mpi_mod_mpi(&u1, &u1, &grp.P));    /* *** u1 = u1 mod p *** */
    chk_func_err_goto(mbedtls_mpi_mod_mpi(&u2, &u2, &grp.P));    /* *** u2 = u2 mod p *** */
    if (mbedtls_mpi_cmp_mpi(&u1, &u2) != 0) {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto exit__;
    }

    /* Step 2: compute Z = [d]R */
    chk_func_err_goto(mbedtls_ecp_mul(&grp, z, d, r, NULL, NULL));
    if (mbedtls_ecp_is_zero(z)) {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto exit__;
    }

exit__:
    mbedtls_ecp_group_free(&grp);
    mbedtls_mpi_free(&u1);
    mbedtls_mpi_free(&u2);

    func_exit();

    return (ret);
}

td_s32 mbedtls_sm2_sign(td_u32 e[SM2_LEN_IN_WROD], td_u32 d[SM2_LEN_IN_WROD],
                        td_u32 r[SM2_LEN_IN_WROD], td_u32 s[SM2_LEN_IN_WROD], td_u32 length)
{
    int ret = TD_SUCCESS;
    mbedtls_mpi md;
    mbedtls_mpi mr;
    mbedtls_mpi ms;

    func_enter();

    chk_param_err_return(length != SM2_LEN_IN_BYTE);

    mbedtls_mpi_init(&md);
    mbedtls_mpi_init(&mr);
    mbedtls_mpi_init(&ms);

    chk_func_err_goto(mbedtls_mpi_read_binary(&md, (td_u8 *)d, SM2_LEN_IN_BYTE));
    chk_func_err_goto(sm2_sign(&mr,
        &ms, &md, (td_u8 *)e, SM2_LEN_IN_BYTE, mbedtls_get_random, TD_NULL));

    chk_func_err_goto(mbedtls_mpi_write_binary(&mr, (td_u8 *)r, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_write_binary(&ms, (td_u8 *)s, SM2_LEN_IN_BYTE));

exit__:
    mbedtls_mpi_free(&md);
    mbedtls_mpi_free(&mr);
    mbedtls_mpi_free(&ms);

    func_exit();

    return ret;
}

/***************************************
  1. t=(r+s)mod n, if t==0, return fail
  2. (x1,y1)=[s]G+tP,
  3. r=(e+x1)mod n, if r==R, return pass
****************************************/
td_s32 mbedtls_sm2_verify(td_u32 e[SM2_LEN_IN_WROD],
                          td_u32 px[SM2_LEN_IN_WROD], td_u32 py[SM2_LEN_IN_WROD],
                          td_u32 r[SM2_LEN_IN_WROD], td_u32 s[SM2_LEN_IN_WROD], td_u32 length)
{
    int ret = TD_FAILURE;
    mbedtls_mpi mr;
    mbedtls_mpi ms;
    mbedtls_ecp_point q;

    func_enter();

    chk_param_err_return(length != SM2_LEN_IN_BYTE);

    mbedtls_mpi_init(&mr);
    mbedtls_mpi_init(&ms);
    mbedtls_ecp_point_init(&q);

    chk_func_err_goto(mbedtls_mpi_read_binary(&mr, (td_u8 *)r, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_read_binary(&ms, (td_u8 *)s, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_read_binary(&q.X, (td_u8 *)px, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_read_binary(&q.Y, (td_u8 *)py, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_lset(&q.Z, 1));
    chk_func_err_goto(sm2_verify((td_u8 *)e, SM2_LEN_IN_BYTE, &q, &mr, &ms));

exit__:
    mbedtls_mpi_free(&mr);
    mbedtls_mpi_free(&ms);
    mbedtls_ecp_point_free(&q);

    func_exit();

    return ret;
}

/* Compute C1(x, y) = k * G(x, y), XY = k * P(x, y) */
td_s32 mbedtls_sm2_encrypt(td_u32 px[SM2_LEN_IN_WROD], td_u32 py[SM2_LEN_IN_WROD],
                           td_u32 c1x[SM2_LEN_IN_WROD], td_u32 c1y[SM2_LEN_IN_WROD],
                           td_u32 x2[SM2_LEN_IN_WROD], td_u32 y2[SM2_LEN_IN_WROD], td_u32 length)
{
    int ret = TD_SUCCESS;
    mbedtls_ecp_point q;
    mbedtls_ecp_point r;
    mbedtls_ecp_point z;

    func_enter();

    chk_param_err_return(length != SM2_LEN_IN_BYTE);

    mbedtls_ecp_point_init(&q);
    mbedtls_ecp_point_init(&z);
    mbedtls_ecp_point_init(&r);

    chk_func_err_goto(mbedtls_mpi_read_binary(&q.X, (td_u8 *)px, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_read_binary(&q.Y, (td_u8 *)py, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_lset(&q.Z, 1));

    chk_func_err_goto(sm2_encrypt(&z, &q, &r, mbedtls_get_random, TD_NULL));

    chk_func_err_goto(mbedtls_mpi_write_binary(&r.X, (td_u8 *)c1x, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_write_binary(&r.Y, (td_u8 *)c1y, SM2_LEN_IN_BYTE));

    chk_func_err_goto(mbedtls_mpi_write_binary(&z.X, (td_u8 *)x2, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_write_binary(&z.Y, (td_u8 *)y2, SM2_LEN_IN_BYTE));

exit__:
    mbedtls_ecp_point_free(&q);
    mbedtls_ecp_point_free(&r);
    mbedtls_ecp_point_free(&z);

    func_exit();

    return ret;
}

/* Compute M(x, y) = C1(x, y) * d */
td_s32 mbedtls_sm2_decrypt(td_u32 d[SM2_LEN_IN_WROD],
                           td_u32 c1x[SM2_LEN_IN_WROD], td_u32 c1y[SM2_LEN_IN_WROD],
                           td_u32 x2[SM2_LEN_IN_WROD], td_u32 y2[SM2_LEN_IN_WROD], td_u32 length)
{
    td_s32 ret = TD_SUCCESS;
    mbedtls_mpi md;
    mbedtls_ecp_point r;
    mbedtls_ecp_point z;

    func_enter();

    chk_param_err_return(length != SM2_LEN_IN_BYTE);

    mbedtls_mpi_init(&md);
    mbedtls_ecp_point_init(&z);
    mbedtls_ecp_point_init(&r);

    chk_func_err_goto(mbedtls_mpi_read_binary(&md, (td_u8 *)d, SM2_LEN_IN_BYTE));

    /* Import td_u8 array C1 ro point R */
    chk_func_err_goto(mbedtls_mpi_read_binary(&r.X, (td_u8 *)c1x, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_read_binary(&r.Y, (td_u8 *)c1y, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_lset(&r.Z, 1));

    /* compute Z = [d]R */
    chk_func_err_goto(sm2_decrypt(&z, &md, &r));

    /* Export R to td_u8 array C1(x2,y2) */
    chk_func_err_goto(mbedtls_mpi_write_binary(&z.X, (td_u8 *)x2, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_write_binary(&z.Y, (td_u8 *)y2, SM2_LEN_IN_BYTE));

exit__:
    mbedtls_mpi_free(&md);
    mbedtls_ecp_point_free(&r);
    mbedtls_ecp_point_free(&z);

    func_exit();

    return ret;
}

/* Compute P(x,y) = d * G(x,y) */
td_s32 mbedtls_sm2_gen_key(td_u32 d[SM2_LEN_IN_WROD],
    td_u32 px[SM2_LEN_IN_WROD], td_u32 py[SM2_LEN_IN_WROD], td_u32 length)
{
    td_s32 ret = TD_SUCCESS;
    mbedtls_ecp_group grp;
    mbedtls_ecp_point q;
    mbedtls_mpi m;
    td_u32 i;
    td_u32 j = 0;
    td_u32 random[SM2_TRY_CNT][SM2_LEN_IN_WROD];

    func_enter();

    chk_param_err_return(length != SM2_LEN_IN_BYTE);

    mbedtls_mpi_init(&m);
    mbedtls_ecp_point_init(&q);
    mbedtls_ecp_group_init(&grp);

    chk_func_err_goto(ecp_group_load_sm2(&grp));

    ot_cipher_dbg("Step 1. generate randnum d, 1<=k<=n-2");
    for (i = 0; i < SM2_TRY_CNT; i++) {
        chk_func_err_goto(cryp_sm2_get_randnum(random[i], sm2n1, sizeof(sm2n1)));
    }
    chk_func_err_goto(cryp_trng_get_random(&j, -1));
    j %= SM2_TRY_CNT;
    (td_void)memcpy_s(d, SM2_LEN_IN_BYTE, random[j], SM2_LEN_IN_BYTE);

    ot_cipher_dbg("Step 2. PA=dA*G");

    /* Step 2: P = d * G */
    chk_func_err_goto(mbedtls_mpi_read_binary(&m, (td_u8 *)d, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_ecp_mul(&grp, &q, &m, &grp.G, NULL, NULL));
    if (mbedtls_ecp_is_zero(&q)) {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto exit__;
    }

    chk_func_err_goto(mbedtls_mpi_write_binary(&q.X, (td_u8 *)px, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_write_binary(&q.Y, (td_u8 *)py, SM2_LEN_IN_BYTE));
    chk_func_err_goto(mbedtls_mpi_lset(&q.Z, 1));

exit__:
    mbedtls_mpi_free(&m);
    mbedtls_ecp_point_free(&q);
    mbedtls_ecp_group_free(&grp);

    func_exit();

    return ret;
}

#endif
/** @} */ /** <!-- ==== API Code end ==== */
