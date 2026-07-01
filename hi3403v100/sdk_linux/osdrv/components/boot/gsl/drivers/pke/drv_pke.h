/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __DRV_PKE_H__
#define __DRV_PKE_H__

/*************************** Structure Definition ****************************/
/** \addtogroup     pke */
/** @{ */  /** <!-- [pke] */
/*! \struct of ecc  */
typedef struct {
    const uint8_t *p; /* Finite field: equal to p in case of prime field curves or
                                           equal to 2^n in case of binary field curves. */
    const uint8_t *a; /* !<  Curve parameter a (q-3 in Suite B). */
    const uint8_t *b; /* !<  Curve parameter b */
    const uint8_t *gx; /* !<  X coordinates of G which is a base point on the curve. */
    const uint8_t *gy; /* !<  Y coordinates of G which is a base point on the curve. */
    const uint8_t *n; /* !<  Prime which is the order of G point. */
    uint32_t h; /*  Cofactor, which is the order of the elliptic curve divided by
                                 the order of the point G. For the Suite B curves, h = 1. */
    uint32_t ksize; /* !<  Key size in bytes. It corresponds to the size in bytes of the prime */
} ecc_param_t;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      pke */
/** @{ */  /** <!--[pke] */
/*! Define the length of zero padding for mul-dot */
#define PKE_LEN_BLOCK_IN_WOED        0x02
#define PKE_LEN_BLOCK_IN_BYTE        0x08
#define PKE_LEN_BLOCK_MASK           0x07

/* Define the union for sm2 block */
typedef union {
    uint8_t  byte[PKE_LEN_BLOCK_IN_BYTE];
    uint32_t word[PKE_LEN_BLOCK_IN_WOED];
} pke_block;

/**
\brief  Initialize the pke module.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
int32_t drv_pke_init(void);

int32_t drv_pke_deinit(void);
/* c = a + b mod p */
int32_t drv_pke_add_mod(const uint8_t *a, const uint8_t *b, const uint8_t *p, uint8_t *c,
    uint32_t size);

/* c=m^e mod n */
int32_t drv_ifep_rsa_exp_mod(const unsigned char *n, const unsigned char *k,
    const unsigned char *in, uint8_t *out, uint32_t klen);

uint32_t get_rand(void);

/** @} */  /** <!-- ==== API declaration end ==== */

#endif
