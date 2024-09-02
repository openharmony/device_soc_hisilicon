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
#ifndef OHOS_SLE_GATT_CLIENT_H
#define OHOS_SLE_GATT_CLIENT_H


#include "sle_ssap_client.h"
#include "ohos_sle_errcode.h"
#include "ohos_sle_common.h"

typedef struct {
    uint8_t    type;      /*!< @if Eng find type { @ref SsapFindType }.
                               @else   查找类型 { @ref SsapFindType }。 @endif */
    uint16_t   startHdl; /*!< @if Eng start handle.
                               @else   起始句柄 @endif */
    uint16_t   endHdl;   /*!< @if Eng end handle.
                               @else   结束句柄 @endif */
    SleUuid uuid;      /*!< @if Eng uuid, only valid when find structure by uuid.
                               @else   uuid，按照uuid查找时生效，其余不生效 @endif */
    uint8_t    reserve;   /*!< @if Eng reserve, "0" for default value.
                               @else   预留，默认值写0 @endif */
} SsapcFindStructureParam;

typedef struct {
    uint16_t handle;    /*!< @if Eng property handle.
                             @else   属性句柄。 @endif */
    uint8_t  type;      /*!< @if Eng property type.
                             @else   属性类型。 @endif */
    uint16_t dataLen;  /*!< @if Eng Data Length.
                             @else   数据长度。 @endif */
    uint8_t  *data;     /*!< @if Eng Data.
                             @else   数据内容。 @endif */
} SsapcHandleValue, SsapcWriteParam;


typedef struct {
    uint8_t    type;      /*!< @if Eng find type { @ref SsapFindType }.
                               @else   查找类型 { @ref SsapFindType }。 @endif */
    SleUuid uuid;      /*!< @if Eng uuid
                               @else   uuid @endif */
} SsapcFindStructureResult;

typedef struct {
    uint16_t handle;  /*!< @if Eng property handle.
                           @else   属性句柄。 @endif */
    uint8_t  type;    /*!< @if Eng property type.
                           @else   属性类型。 @endif */
} SsapcWriteResult;



ErrCodeType SsapcRegister(SleUuid *appUuid, uint8_t *clientId);
ErrCodeType SsapcRegisterUnregister(uint8_t clientId);
ErrCodeType SsapcFindStructure(uint8_t clientId, uint16_t connId, SsapcFindStructureParam *param);
ErrCodeType SsapcReadReq(uint8_t clientId, uint16_t connId, uint16_t handle, uint8_t type);
ErrCodeType SsapcWriteReq(uint8_t clientId, uint16_t connId, SsapcWriteParam *param);
ErrCodeType SsapcExchangeInfoReq(uint8_t clientId, uint16_t connId, SsapcExchangeInfo* param);


typedef void (*SsapcFindStructureCompleteCallback)(uint8_t clientId, uint16_t connId,
    SsapcFindStructureResult *structureResult, ErrCodeType status);	
typedef void (*SsapcReadCfmCallback)(uint8_t clientId, uint16_t connId, SsapcHandleValue *read_data,
    ErrCodeType status);
typedef void (*SsapcWriteCfmCallback)(uint8_t clientId, uint16_t connId, 
SsapcWriteResult *writeResult, ErrCodeType status);	
typedef void (*SsapcExchangeInfoCallback)(uint8_t clientId, uint16_t connId, 
SsapcExchangeInfo *param, ErrCodeType status);
typedef void (*SsapcIndicationCallback)(uint8_t clientId, uint16_t connId,
 SsapcHandleValue *data, ErrCodeType status);

typedef struct {
	SsapcFindStructureCompleteCallback ssapcFindStructureCompleteCb;
	SsapcReadCfmCallback ssapcReadCfmCb;
	SsapcWriteCfmCallback ssapcWriteCfmCb;
	SsapcExchangeInfoCallback ssapcExchangeInfoCb;
	SsapcIndicationCallback ssapcIndicationCb;
}SsapcCallbacks;

ErrCodeType SsapcRegisterCallbacks(SsapcCallbacks *func);

#endif
