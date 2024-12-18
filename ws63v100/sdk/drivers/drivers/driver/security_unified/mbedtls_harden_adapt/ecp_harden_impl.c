/**
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: mbedtls ecp harden implement.
 *
 * Create: 2024-3-13
*/

#include "ecp_harden_impl.h"
#include "crypto_drv_common.h"

#include "kapi_pke.h"
#include "kapi_pke_cal.h"

int mbedtls_alt_ecp_mul_impl(mbedtls_alt_ecp_curve_type curve_type, const mbedtls_alt_ecp_data *k,
    const mbedtls_alt_ecp_point *p, const mbedtls_alt_ecp_point *r)
{
    return kapi_pke_mul_dot(
        (drv_pke_ecc_curve_type)curve_type,
        (const drv_pke_data *)k,
        (const drv_pke_ecc_point *)p,
        (const drv_pke_ecc_point *)r);
}

int mbedtls_alt_ecdsa_verify_impl(mbedtls_alt_ecp_curve_type curve_type,
    const unsigned char *hash, unsigned int hash_len,
    const unsigned char *pub_x, const unsigned char *pub_y,
    const unsigned char *sig_r, const unsigned char *sig_s, unsigned klen)
{
    drv_pke_data hash_data = {
        .data = (unsigned char *)hash,
        .length = hash_len
    };
    drv_pke_ecc_point pub_key = {
        .x = (unsigned char *)pub_x,
        .y = (unsigned char *)pub_y,
        .length = klen
    };
    drv_pke_ecc_sig sig = {
        .r = (unsigned char *)sig_r,
        .s = (unsigned char *)sig_s,
        .length = klen
    };
    return kapi_pke_ecdsa_verify(
        (drv_pke_ecc_curve_type)curve_type,
        &pub_key, &hash_data, &sig);
}

void __attribute__((__weak__)) mbedtls_alt_ecp_register(const mbedtls_alt_ecp_harden_func *ecp_func)
{
    crypto_unused(ecp_func);
    return;
}

int mbedtls_alt_ecp_init(void)
{
    mbedtls_alt_ecp_harden_func func_list = {
        .ecp_mul = mbedtls_alt_ecp_mul_impl,
        .ecdsa_verify = mbedtls_alt_ecdsa_verify_impl
    };
    mbedtls_alt_ecp_register(&func_list);
    return 0;
}

int mbedtls_alt_ecp_deinit(void)
{
    mbedtls_alt_ecp_harden_func func_list = { 0 };
    mbedtls_alt_ecp_register(&func_list);
    return 0;
}