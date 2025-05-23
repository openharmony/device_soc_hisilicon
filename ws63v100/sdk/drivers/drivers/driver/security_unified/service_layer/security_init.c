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
 * Description: security_unified init implement file.
 *
 * Create: 2023-03-29
*/

#include "security_init.h"
#include "crypto_drv_init.h"
#include "kapi_hash.h"
#include "kapi_symc.h"

void uapi_drv_cipher_env_init(void)
{
    crypto_drv_mod_init();
}

void uapi_drv_cipher_wait_func_enable_all(void)
{
    crypto_wait_func_config(TD_TRUE);
}

void uapi_drv_cipher_wait_func_disable_all(void)
{
    crypto_wait_func_config(TD_FALSE);
}

void uapi_drv_cipher_env_deinit(void)
{
    crypto_drv_mod_deinit();
}

void uapi_drv_cipher_env_resume(void)
{
    crypto_drv_mod_resume();
}

void uapi_drv_cipher_env_suspend(void)
{
    crypto_drv_mod_suspend();
}