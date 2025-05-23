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
 * Description: big number API implement file.
 *
 * Create: 2023-03-15
*/

#include "hal_pke.h"
#include "alg/rsa.h"
#include "drv_common_pke.h"
#include "drv_pke_cal.h"

td_s32 drv_cipher_pke_lock_secure(void)
{
    return crypto_drv_pke_common_resume();
}

td_s32 drv_cipher_pke_unlock_secure(void)
{
    crypto_drv_pke_common_suspend();
    return TD_SUCCESS;
}

td_s32 drv_cipher_pke_add_mod(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data *p,
    const drv_pke_data *c)
{
    td_s32 ret = TD_FAILURE;
    ret = rsa_add_mod(sec_arg_add_cs(a, b, p, c));
    return ret;
}

td_s32 drv_cipher_pke_sub_mod(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data *p,
    const drv_pke_data *c)
{
    td_s32 ret = TD_FAILURE;
    ret = rsa_sub_mod(sec_arg_add_cs(a, b, p, c));
    return ret;
}

td_s32 drv_cipher_pke_mul_mod(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data *p,
    const drv_pke_data *c)
{
    td_s32 ret = TD_FAILURE;
    ret = rsa_mul_mod(sec_arg_add_cs(a, b, p, c));
    return ret;
}

td_s32 drv_cipher_pke_inv_mod(const drv_pke_data *a, const drv_pke_data *p, const drv_pke_data *c)
{
    td_s32 ret = TD_FAILURE;
    ret = rsa_inv_mod(sec_arg_add_cs(a, p, c));
    return ret;
}

td_s32 drv_cipher_pke_mod(const drv_pke_data *a, const drv_pke_data *p, const drv_pke_data *c)
{
    td_s32 ret = TD_FAILURE;
    ret = rsa_mod(sec_arg_add_cs(a, p, c));
    return ret;
}

td_s32 drv_cipher_pke_mul(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data *c)
{
    td_s32 ret = TD_FAILURE;
    ret = rsa_regular_mul(sec_arg_add_cs(a, b, c));
    return ret;
}

td_s32 drv_cipher_pke_exp_mod(const drv_pke_data *n, const drv_pke_data *k, const drv_pke_data *in,
    const drv_pke_data *out)
{
    td_s32 ret = TD_FAILURE;
    ret = rsa_exp_mod(sec_arg_add_cs(n, k, in, out));
    return ret;
}