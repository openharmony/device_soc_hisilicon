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
 *
 * Description: Provides trng driver source \n
 *
 * History: \n
 * 2023-03-22, Create file. \n
 */

#include "trng.h"
#include "kapi_trng.h"

#include "sl_common.h"

errcode_t uapi_drv_cipher_trng_get_random(uint32_t *randnum)
{
    uapi_pm_add_sleep_veto(PM_SECURITY_VETO_ID);
    int32_t ret = kapi_cipher_trng_get_random(randnum);
    uapi_pm_remove_sleep_veto(PM_SECURITY_VETO_ID);
    return crypto_sl_common_get_errcode(ret);
}

errcode_t uapi_drv_cipher_trng_get_random_bytes(uint8_t *randnum, uint32_t size)
{
    uapi_pm_add_sleep_veto(PM_SECURITY_VETO_ID);
    int32_t ret = kapi_cipher_trng_get_multi_random(size, randnum);
    uapi_pm_remove_sleep_veto(PM_SECURITY_VETO_ID);
    return crypto_sl_common_get_errcode(ret);
}