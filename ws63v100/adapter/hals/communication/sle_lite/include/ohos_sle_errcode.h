/*
# Copyright (C) 2024 HiHope Open Source Organization .
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/

#ifndef OHOS_SLE_ERRCODE_H
#define OHOS_SLE_ERRCODE_H

#include "errcode.h"
/**
 * @if Eng
 * @brief  SLE error code base.
 * @else
 * @brief  SLE 错误码起始。
 * @endif
 */
#define OH_ERRCODE_SLE_COMMON_BASE 0x80006000

/**
 * @if Eng
 * @brief  SLE SSAP error code base.
 * @else
 * @brief  SLE SSAP 错误码起始。
 * @endif
 */
#define OH_ERRCODE_SLE_SSAP_BASE   0x80006100

/**
 * @if Eng
 * @brief  SLE SSAP error code end.
 * @else
 * @brief  SLE SSAP 错误码结束。
 * @endif
 */
#define OH_ERRCODE_SLE_SSAP_END    0x800061FF

/**
 * @if Eng
 * @brief  SLE error code base.
 * @else
 * @brief  SLE 错误码结束。
 * @endif
 */
#define OH_ERRCODE_SLE_COMMON_END  0x800067FF


/**
 * @if Eng
 * @brief  SLE error code.
 * @else
 * @brief  SLE 错误码（非协议相关）。
 * @endif
 */
typedef enum {
    OH_ERRCODE_SLE_SUCCESS = 0,                               /*!< @if Eng error code of success
                                                                @else   执行成功错误码 @endif */
    OH_ERRCODE_SLE_CONTINUE = OH_ERRCODE_SLE_COMMON_BASE,        /*!< @if Eng error code of continue
                                                                @else   继续执行错误码 @endif */
    OH_ERRCODE_SLE_DIRECT_RETURN,                             /*!< @if Eng error code of direct return
                                                                @else   直接返回错误码 @endif */
    OH_ERRCODE_SLE_NO_ATTATION,                               /*!< @if Eng error code of no attention
                                                                @else   错误码 @endif */
    OH_ERRCODE_SLE_PARAM_ERR,                                 /*!< @if Eng error code of parameter error
                                                                @else   参数错误错误码 @endif */
    OH_ERRCODE_SLE_FAIL,                                      /*!< @if Eng error code of configure fail
                                                                @else   配置失败错误码 @endif */
    OH_ERRCODE_SLE_TIMEOUT,                                   /*!< @if Eng error code of configure timeout
                                                                @else   配置超时错误码 @endif */
    OH_ERRCODE_SLE_UNSUPPORTED,                               /*!< @if Eng error code of unsupported parameter
                                                                @else   参数不支持错误码 @endif */
    OH_ERRCODE_SLE_GETRECORD_FAIL,                            /*!< @if Eng error code of get current record fail
                                                                @else   获取当前记录失败错误码 @endif */
    OH_ERRCODE_SLE_POINTER_NULL,                              /*!< @if Eng error code of pointer is NULL
                                                                @else   指针为空错误码 @endif */
    OH_ERRCODE_SLE_NO_RECORD,                                 /*!< @if Eng error code of no record return
                                                                @else   无记录返回错误码 @endif */
    OH_ERRCODE_SLE_STATUS_ERR,                                /*!< @if Eng error code of state error
                                                                @else   状态错误错误码 @endif */
    OH_ERRCODE_SLE_NOMEM,                                     /*!< @if Eng error code of no memory
                                                                @else   内存不足错误码 @endif */
    OH_ERRCODE_SLE_AUTH_FAIL,                                 /*!< @if Eng error code of authentication failure
                                                                @else   认证失败错误码 @endif */
    OH_ERRCODE_SLE_AUTH_PKEY_MISS,                            /*!< @if Eng error code of authentication fail due to pin
                                                                        code or key lost
                                                                @else   PIN码或密钥丢失致认证失败错误码 @endif */
    OH_ERRCODE_SLE_RMT_DEV_DOWN,                              /*!< @if Eng error code of remote device down
                                                                @else   对端设备关闭错误码 @endif */
    OH_ERRCODE_SLE_PAIRING_REJECT,                            /*!< @if Eng error code of pair reject
                                                                @else   配对拒绝错误码 @endif */
    OH_ERRCODE_SLE_BUSY,                                      /*!< @if Eng error code of system busy
                                                                @else   系统繁忙错误码 @endif */
    OH_ERRCODE_SLE_NOT_READY,                                 /*!< @if Eng error code of system not ready
                                                                @else   系统未准备好错误码 @endif */
    OH_ERRCODE_SLE_CONN_FAIL,                                 /*!< @if Eng error code of connect fail
                                                                @else   连接失败错误码 @endif */
    OH_ERRCODE_SLE_OUT_OF_RANGE,                              /*!< @if Eng error code of out of range
                                                                @else   越界错误码 @endif */
    OH_ERRCODE_SLE_MEMCPY_FAIL,                               /*!< @if Eng error code of memcpy fail
                                                                @else   拷贝失败错误码 @endif */
    OH_ERRCODE_SLE_MALLOC_FAIL,                               /*!< @if Eng error code of malloc fail
                                                                @else   内存申请失败错误码 @endif */
    OH_ERRCODE_SLE_MAX = OH_ERRCODE_SLE_COMMON_END               /*!< @if Eng maximum of SLE error code
                                                                @else   SLE 错误码最大值 @endif */
} ErrCodeType;


/**
 * @if Eng
 * @brief Enum of sle pairing state.
 * @else
 * @brief 星闪断链原因。
 * @endif
 */
typedef enum {
    OH_SLE_DISCONNECT_BY_REMOTE = 0x10,    /*!< @if Eng disconnect by remote
                                             @else   远端断链 @endif */
    OH_SLE_DISCONNECT_BY_LOCAL  = 0x11,    /*!< @if Eng disconnect by local
                                             @else   本端断链 @endif */
} SleDiscReasonType;

#endif