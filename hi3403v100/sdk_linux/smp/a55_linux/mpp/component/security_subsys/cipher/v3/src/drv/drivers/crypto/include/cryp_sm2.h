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

#ifndef CRYP_SM2_H
#define CRYP_SM2_H

#include "drv_osal_lib.h"
#include "drv_pke.h"

/******************************* API Declaration *****************************/
/** \addtogroup      sm2 */
/** @{ */ /** <!--[sm2] */

#define SM2_TRY_CNT 8

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Code *****************************/
/** \addtogroup      cipher drivers */
/** @{ */ /** <!-- [cipher] */

/**
* \brief          Initialize crypto of sm2 *
 */
td_s32 cryp_sm2_init(void);

/**
* \brief          Deinitialize crypto of sm2 *
 */
void cryp_sm2_deinit(void);

/**
\brief SM2 encryption a plaintext with a SM2 key
\      Compute C1(x, y) = k * G(x, y), XY = k * P(x, y)
 */
typedef td_s32 (*func_sm2_encrypt)(const ot_cipher_sm2_public_key *sm2_key,
    ecc_point_t *point_c1, ecc_point_t *point_xy, const ecc_param_t *sm2_id);

/**
\brief SM2 decryption a plaintext with a SM2 key.
\      Compute XY(x, y) = C1(x, y) * d
 */
typedef td_s32 (*func_sm2_decrypt)(const ot_cipher_sm2_private_key *sm2_key,
    ecc_point_t *point_c1, ecc_point_t *point_xy, const ecc_param_t *sm2_id);

/**
\brief SM2 sign a sm3 value with a SM2 private key.
\  1. generate a randnum k, k within [1,n-1],
\  2. (x1,y1)=[k]G,
\  3. r=(e+x1)mod n, if r=0 or r+k=n, return step 1
\  4. s=((1+dA)^-1 * (k-rdA))mod n
 */
typedef td_s32 (*func_sm2_sign)(sm2_sign_t *sign,
    const td_u32 *e, td_u32 e_len, const ecc_param_t *sm2_id);

/**
\brief SM2 verify a sm3 value with a SM2 public key.
\  1. t=(r+s)mod n, if t==0, return fail
\  2. (x1,y1)=[s]G+tP,
\  3. r=(e+x1)mod n, if r==R, return pass
 */
typedef td_s32 (*func_sm2_verify)(const sm2_verify_t *verify,
    const td_u32 *e, td_u32 e_len, const ecc_param_t *sm2_id);

/**
\brief Generate a SM2 private key.
\      Compute P(x,y) = d * G(x,y)
 */
typedef td_s32 (*func_sm2_gen_key)(td_u32 d[SM2_LEN_IN_WROD],
                                   td_u32 px[SM2_LEN_IN_WROD], td_u32 py[SM2_LEN_IN_WROD],
                                   td_u32 length, const ecc_param_t *sm2_id);

/* ! \struct of SM2 function template */
typedef struct {
    func_sm2_encrypt encrypt;
    func_sm2_decrypt decrypt;
    func_sm2_sign sign;
    func_sm2_verify verify;
    func_sm2_gen_key genkey;
} sm2_func;

/**
\brief  Clone the function from template of sm2 engine.
\retval     On success, func is returned.  On error, TD_NULL is returned.
 */
sm2_func *cryp_get_sm2_op(void);

/**
* \brief          get randnum
 */
void cryp_pke_get_randnum(td_u8 *randnum, const td_u8 *max, td_u32 klen);

/**
* \brief check val whether zero or not
 */
td_u32 cryp_pke_is_zero(const td_void *val, td_u32 klen);

/**
* \brief  check val whether less than max or not
 */
td_s32 cryp_pke_range_check(const td_u8 *val, const td_u8 *max, td_u32 klen);

/** @} */ /** <!-- ==== API declaration end ==== */

#endif /* CRYP_SM2_H */
