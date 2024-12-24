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
 * Description: Provides common utils for security_unified service layer. \n
 *
 * History: \n
 * 2023-03-14, Create file. \n
 */
#include "sl_common.h"
#include "crypto_errno.h"

typedef struct crypto_errcode_item {
    int32_t from_errcode;
    errcode_t to_errcode;
} crypto_errcode_item_t;

#define ERROR_SECURITY_UNIFIED      0x80001500
errcode_t crypto_sl_common_get_errcode(int32_t from_errcode)
{
    errcode_t ret = (uint32_t)from_errcode & 0xFF;
    if ((uint32_t)from_errcode == CRYPTO_SUCCESS) {
        return ERRCODE_SUCC;
    } else if ((uint32_t)from_errcode == CRYPTO_FAILURE) {
        return ERRCODE_FAIL;
    }
    return ERROR_SECURITY_UNIFIED + ret;
}
