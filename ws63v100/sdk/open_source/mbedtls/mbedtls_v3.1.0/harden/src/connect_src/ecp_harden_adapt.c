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
 * Description: ecp harden adapt functions.
 *
 * Create: 2024-07-18
*/

#include "mbedtls_harden_adapt_api.h"
#include "dfx.h"
#include "securec.h"

static mbedtls_alt_ecp_harden_func g_ecp_func;

int mbedtls_alt_ecp_mul(mbedtls_alt_ecp_curve_type curve_type, const mbedtls_alt_ecp_data *k,
    const mbedtls_alt_ecp_point *p, const mbedtls_alt_ecp_point *r)
{
    int ret = -1;
    mbedtls_harden_log_func_enter();
    if (g_ecp_func.ecp_mul == NULL) {
        mbedtls_printf("Error: ecp_mul unregister!\n");
        return -1;
    }
    ret = g_ecp_func.ecp_mul(curve_type, k, p, r);
    mbedtls_harden_log_func_exit();
    return ret;
}

int mbedtls_alt_ecdsa_verify(mbedtls_alt_ecp_curve_type curve_type,
    const unsigned char *hash, unsigned int hash_len,
    const unsigned char *pub_x, const unsigned char *pub_y,
    const unsigned char *sig_r, const unsigned char *sig_s, unsigned klen)
{
    int ret = -1;
    mbedtls_harden_log_func_enter();
    if (g_ecp_func.ecdsa_verify == NULL) {
        mbedtls_printf("Error: ecdsa_verify unregister!\n");
        return -1;
    }
    ret = g_ecp_func.ecdsa_verify(curve_type, hash, hash_len, pub_x, pub_y, sig_r, sig_s, klen);
    mbedtls_harden_log_func_exit();
    return ret;
}

void mbedtls_alt_ecp_register(const mbedtls_alt_ecp_harden_func *ecp_func)
{
    if (ecp_func == NULL) {
        return;
    }
    (void)memcpy_s(&g_ecp_func, sizeof(mbedtls_alt_ecp_harden_func), ecp_func, sizeof(mbedtls_alt_ecp_harden_func));
}