/*
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
 * Description: base64编码解码适配层接口mbedTLS实现（此文件为DEMO，需集成方适配修改）
 */
#include "hilink_sal_base64.h"

#include <stddef.h>
#include "hilink_sal_defines.h"
#include "mbedtls/base64.h"

int HILINK_SAL_Base64Encode(const unsigned char *inData, unsigned int inLen,
    unsigned char *outData, unsigned int *outLen)
{
    if ((inData == NULL) || (inLen == 0)) {
        HILINK_SAL_WARN("invalid param\r\n");
        return HILINK_SAL_PARAM_INVALID;
    }
    size_t outSize = *outLen;
    int ret;

    if ((outData == NULL) || (*outLen == 0)) {
        ret = mbedtls_base64_encode(NULL, 0, &outSize, inData, inLen);
        if ((ret != MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) || (outSize == 0)) {
            HILINK_SAL_ERROR("get base64 max size error, ret %d\r\n", ret);
            return HILINK_SAL_PARAM_INVALID;
        }
        *outLen = outSize;
        return HILINK_SAL_OK;
    }

    ret = mbedtls_base64_encode(outData, outSize, &outSize, inData, inLen);
    if (ret != 0) {
        HILINK_SAL_ERROR("base64 encode error, ret %d\r\n", ret);
        return ret;
    }
    *outLen = outSize;

    return HILINK_SAL_OK;
}

int HILINK_SAL_Base64Decode(const unsigned char *inData, unsigned int inLen,
    unsigned char *outData, unsigned int *outLen)
{
    if ((inData == NULL) || (inLen == 0)) {
        HILINK_SAL_WARN("invalid param\r\n");
        return HILINK_SAL_PARAM_INVALID;
    }
    size_t outSize = *outLen;
    int ret;

    if ((outData == NULL) || (*outLen == 0)) {
        ret = mbedtls_base64_decode(NULL, 0, &outSize, inData, inLen);
        if ((ret != MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) || (outSize == 0)) {
            HILINK_SAL_ERROR("get base64 max size error, ret %d\r\n", ret);
            return HILINK_SAL_PARAM_INVALID;
        }
        *outLen = outSize;
        return HILINK_SAL_OK;
    }

    ret = mbedtls_base64_decode(outData, outSize, &outSize, inData, inLen);
    if (ret != 0) {
        HILINK_SAL_ERROR("base64 decode error, ret %d\r\n", ret);
        return ret;
    }
    *outLen = outSize;

    return HILINK_SAL_OK;
}