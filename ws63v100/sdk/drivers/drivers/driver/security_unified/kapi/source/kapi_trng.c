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
 * Description: trng kernel API function implementation.
 *
 * Create: 2023-05-26
*/

#include "kapi_trng.h"
#include "kapi_inner.h"

#include "drv_trng.h"
#include "crypto_drv_common.h"

#define TRNG_COMPAT_ERRNO(err_code)     KAPI_COMPAT_ERRNO(ERROR_MODULE_TRNG, err_code)

crypto_mutex g_trng_mutex;

#define kapi_trng_mutex_lock() do {          \
    crypto_mutex_lock(&g_trng_mutex);            \
} while (0)

#define kapi_trng_mutex_unlock() do {        \
    crypto_mutex_unlock(&g_trng_mutex);          \
} while (0)

td_s32 kapi_cipher_trng_env_init(td_void)
{
    td_s32 ret = TD_SUCCESS;

    ret = drv_cipher_trng_init();
    if (ret != TD_SUCCESS) {
        crypto_log_err("drv_cipher_trng_init failed, ret is 0x%x\n", ret);
        return ret;
    }

    ret = crypto_mutex_init(&g_trng_mutex);
    if (ret != TD_SUCCESS) {
        crypto_log_err("crypto_mutex_init failed\n");
        goto error_trng_deinit;
    }

error_trng_deinit:
    return ret;
}

td_s32 kapi_cipher_trng_env_deinit(td_void)
{
    crypto_mutex_destroy(&g_trng_mutex);
    drv_cipher_trng_deinit();
    return TD_SUCCESS;
}

td_void inner_kapi_trng_lock(td_void)
{
    kapi_trng_mutex_lock();
}

td_void inner_kapi_trng_unlock(td_void)
{
    kapi_trng_mutex_unlock();
}

td_s32 kapi_cipher_trng_get_random(td_u32 *randnum)
{
    td_s32 ret = TD_FAILURE;
    crypto_chk_return(randnum == TD_NULL, TRNG_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "randnum is NULL\n");
    ret = crypto_get_multi_random(CRYPTO_WORD_WIDTH, (td_u8 *)randnum);
    return ret;
}

td_s32 kapi_cipher_trng_get_multi_random(td_u32 size, td_u8 *randnum)
{
    td_s32 ret = TD_FAILURE;
    crypto_chk_return(randnum == TD_NULL, TRNG_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "randnum is NULL\n");
    ret = crypto_get_multi_random(size, randnum);
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_trng_get_random);