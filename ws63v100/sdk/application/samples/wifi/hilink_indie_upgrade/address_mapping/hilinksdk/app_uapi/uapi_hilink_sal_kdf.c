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
 * Description: Key derivation algorithm adaptation layer interface implementation. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "app_call.h"
#include "hilink_sal_kdf.h"

int HILINK_SAL_Hkdf(const HiLinkHkdfParam *param, unsigned char *key, unsigned int keyLen)
{
    return app_call3(APP_CALL_HILINK_SAL_HKDF, int,
        const HiLinkHkdfParam *, param, unsigned char *, key, unsigned int, keyLen);
}

int HILINK_SAL_Pkcs5Pbkdf2Hmac(const HiLinkPbkdf2HmacParam *param, unsigned char *key, unsigned int keyLen)
{
    return app_call3(APP_CALL_HILINK_SAL_PKCS5_PBKDF2_HMAC, int,
        const HiLinkPbkdf2HmacParam *, param, unsigned char *, key, unsigned int, keyLen);
}