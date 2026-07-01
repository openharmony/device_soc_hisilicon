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

#ifndef __OT_DRV_PKE_H__
#define __OT_DRV_PKE_H__

/******************************* API Declaration *****************************/
#define SM2_LEN_IN_WROD              8
#define SM2_LEN_IN_BYTE              (SM2_LEN_IN_WROD * 4)

#define RSA_KEY_LEN_2048             256
#define RSA_KEY_LEN_4096             512
#define HASH_LEN_256                 32
#define WORD_WIDTH                   0x04

/**
* \brief RSA verify a ciphertext with a RSA public key.
*/
int32_t drv_rsa_verify_hash(const unsigned char *n, const unsigned char *e,
    const uint8_t *hash, const unsigned char *sign);

/**
\brief   Kapi Init.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
*/
void drv_pke_reset(void);

/** @} */  /** <!-- ==== API Code end ==== */

#endif    /* End of #ifndef __DRV_CIPHER_KAPI_H__ */
