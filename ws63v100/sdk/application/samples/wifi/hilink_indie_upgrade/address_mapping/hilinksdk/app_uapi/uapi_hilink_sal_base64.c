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
 * Description: Base64 encoding and decoding adaptation layer interface implementation. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "app_call.h"

int HILINK_SAL_Base64Encode(const unsigned char *inData, unsigned int inLen,
    unsigned char *outData, unsigned int *outLen)
{
    return app_call4(APP_CALL_HILINK_SAL_BASE64_ENCODE, int, const unsigned char *, inData, unsigned int, inLen,
        unsigned char *, outData, unsigned int *, outLen);
}

int HILINK_SAL_Base64Decode(const unsigned char *inData, unsigned int inLen,
    unsigned char *outData, unsigned int *outLen)
{
    return app_call4(APP_CALL_HILINK_SAL_BASE64_DECODE, int, const unsigned char *, inData, unsigned int, inLen,
        unsigned char *, outData, unsigned int *, outLen);
}