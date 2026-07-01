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

#ifndef DRV_PKE_H
#define DRV_PKE_H

/*************************** Structure Definition ****************************/
/** \addtogroup     pke */
/** @{ */ /** <!-- [pke] */

/* ! Define RSA 1024 key length */
#define RSA_KEY_LEN_1024 128

/* ! Define RSA 1024 key length */
#define RSA_KEY_LEN_2048 256

/* ! Define RSA 1024 key length */
#define RSA_KEY_LEN_3072 384

/* ! Define RSA 1024 key length */
#define RSA_KEY_LEN_4096 512

/* ! \rsa key width */
typedef enum {
    RSA_KEY_WIDTH_1024 = 0x00, /* !<  RSA 1024 */
    RSA_KEY_WIDTH_2048,        /* !<  RSA 2048 */
    RSA_KEY_WIDTH_4096,        /* !<  RSA 4096 */
    RSA_KEY_WIDTH_3072,        /* !<  RSA 3072 */
    RSA_KEY_WIDTH_COUNT,
} rsa_key_width;

/* ! \pke capacity, 0-nonsupport, 1-support */
typedef struct {
    td_u32 sm2 : 1; /* !<  Support SM2 */
    td_u32 ecc : 1; /* !<  Support ECC */
    td_u32 rsa : 1; /* !<  Support RSA */
} pke_capacity;

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      pke */
/** @{ */ /** <!--[pke] */

/* ! Define the length of zero padding for mul-dot */
#define PKE_LEN_BLOCK_IN_WOED 0x02
#define PKE_LEN_BLOCK_IN_BYTE 0x08
#define PKE_LEN_BLOCK_MASK    0x07

/* Define the union for sm2 block */
typedef union {
    td_u8 byte[PKE_LEN_BLOCK_IN_BYTE];
    td_u32 word[PKE_LEN_BLOCK_IN_WOED];
} pke_block;

/**
\brief  Initialize the pke module.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_pke_init(void);

/**
\brief  Deinitialize the pke module.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_pke_deinit(void);

/**
\brief  enable the pke module.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_pke_resume(void);

/**
\brief  disable the pke module.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
void drv_pke_suspend(void);

/* c = a + b mod p */
td_s32 drv_pke_add_mod(const td_u8 *a, const td_u8 *b, const td_u8 *p, td_u8 *c, td_u32 size);

/* c = a - b mod p */
td_s32 drv_pke_sub_mod(const td_u8 *a, const td_u8 *b, const td_u8 *p, td_u8 *c, td_u32 size);

/* c = a * b mod p */
td_s32 drv_pke_mul_mod(const td_u8 *a, const td_u8 *b, const td_u8 *p, td_u8 *c, td_u32 size, td_u32 private);

/* c = a^-1 mod p */
td_s32 drv_pke_inv_mod(const td_u8 *a, const td_u8 *p, td_u8 *c, td_u32 size);

/* c = a mod p */
td_s32 drv_pke_mod(const td_u8 *a, const td_u8 *p, td_u8 *c, td_u32 size);

/* c = a mod p */
td_s32 drv_pke_mod_block(const pke_block *a, td_u32 power, const td_u8 *p, td_u32 plen, td_u8 *c);

/* rsa key generation with public key */
td_s32 drv_rsa_pub_key(td_u32 e, td_u32 klen, cryp_rsa_key *key);

/* c = a * b */
td_s32 drv_pke_mul(const td_u8 *a, const td_u8 *b, td_u8 *c, td_u32 size);

/* R = k . P(x,y) */
td_s32 drv_pke_mul_dot(const ecc_point_t *point_p, ecc_point_t *point_r,
    const td_u8 *k, td_u32 klen, td_u32 private, const ecc_param_t *ecp_id);

/* C = S(x,y) + R(x,y) */
td_s32 drv_pke_add_dot(const td_u8 *sx,
                       const td_u8 *sy,
                       const td_u8 *rx,
                       const td_u8 *ry,
                       td_u8 *cx,
                       td_u8 *cy,
                       td_u32 size,
                       const ecc_param_t *ecp_id);

/**
\brief  clean pke ram .
 */
td_s32 drv_pke_clean_ram(void);

/**
\brief  c=m^e mod n.
 */
td_s32 drv_ifep_rsa_exp_mod(const td_u8 *n, const td_u8 *k,
    const td_u8 *in, td_u8 *out, rsa_key_width width);

/**
\brief  get the pke capacity.
\param[out] capacity The hash capacity.
\retval     NA.
 */
void drv_pke_get_capacity(pke_capacity *capacity);

/** @} */ /** <!-- ==== API declaration end ==== */

#endif /* DRV_PKE_H */
