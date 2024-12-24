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
#ifndef OHOS_SLE_GATT_SERVER_H
#define OHOS_SLE_GATT_SERVER_H

#include "sle_ssap_server.h"
#include "ohos_sle_errcode.h"
#include "ohos_sle_common.h"

/**
 * @if Eng
 * @brief Struct of add property information.
 * @else
 * @brief 添加特征信息。
 * @endif
 */
typedef struct {
    SleUuid uuid;             /*!< @if Eng UUID of SSAP property.
                                      @else   SSAP 特征 UUID。 @endif */
    uint16_t permissions;        /*!< @if Eng Properity permissions. { @ref SsapPermissionType }
                                      @else   特征权限。{ @ref SsapPermissionType }。 @endif */
    uint32_t operateIndication; /*!< @if Eng Operate Indication. { @ref SsapOperateIndication }
                                      @else   操作指示 { @ref SsapOperateIndication } @endif */
    uint16_t valueLen;          /*!< @if Eng Length of reponse data.
                                      @else   响应的数据长度。 @endif */
    uint8_t *value;              /*!< @if Eng Reponse data.
                                      @else   响应的数据。 @endif */
} SsapsPropertyInfo;

typedef struct {
    SleUuid uuid;             /*!< @if Eng UUID of SSAP descriptor.
                                      @else   SSAP 描述符 UUID。 @endif */
    uint16_t permissions;        /*!< @if Eng descriptor permissions. { @ref SsapPermissionType }.
                                      @else   特征权限。 { @ref SsapPermissionType } @endif */
    uint32_t operateIndication; /*!< @if Eng operate Indication. { @ref SsapOperateIndication }
                                      @else   操作指示 { @ref SsapOperateIndication } @endif */
    uint8_t type;                /*!< @if Eng descriptor type. { @ref SsapPropertyType }.
                                      @else   描述符类型。 { @ref SsapPropertyType } @endif */
    uint16_t valueLen;          /*!< @if Eng data length.
                                      @else   数据长度。 @endif */
    uint8_t *value;              /*!< @if Eng data.
                                      @else   数据。 @endif */
} SsapsDescInfo;

typedef struct {
    uint16_t requestId; /*!< @if Eng Request ID.
                              @else   请求 ID。 @endif */
    uint8_t status;      /*!< @if Eng Status code of read/write. Success:
                                      ERRCODE_SLE_SUCCESS. For details about the exception { @ref errcode_sle_ssap_t }
                              @else   读写结果的状态, 成功ERRCODE_SLE_SUCCESS
                                      异常参考{ @ref errcode_sle_ssap_t } @endif */
    uint16_t valueLen;  /*!< @if Eng Length of reponse data.
                              @else   响应的数据长度。 @endif */
    uint8_t *value;      /*!< @if Eng Reponse data.
                              @else   响应的数据。 @endif */
} SsapsSendRsp;

/**
 * @if Eng
 * @brief Struct of send notification/indication information.
 * @else
 * @brief 发送通知/指示信息。
 * @endif
 */
typedef struct {
    uint16_t handle;      /*!< @if Eng Properity handle.
                               @else   属性句柄。 @endif */
    uint8_t type;         /*!< @if Eng property type { @ref SsapPropertyType }.
                               @else   属性类型 { @ref SsapPropertyType }。 @endif */
    uint16_t valueLen;   /*!< @if Eng Length of notification/indication data.
                               @else   通知/指示数据长度。 @endif */
    uint8_t *value;       /*!< @if Eng Notification/indication data.
                               @else   发送的通知/指示数据。 @endif */
} SsapsNtfInd;



typedef struct {
    uint16_t requestId;  /*!< @if Eng Request id.
                               @else   请求id。 @endif */
    uint16_t handle;      /*!< @if Eng Properity handle of the read request.
                               @else   请求读的属性句柄。 @endif */
    uint8_t type;         /*!< @if Eng property type { @ref SsapPropertyType }.
                               @else   属性类型。 @endif  { @ref SsapPropertyType } */
    bool need_rsp;        /*!< @if Eng Whether response is needed.
                               @else   是否需要发送响应。 @endif */
    bool needAuthorize;  /*!< @if Eng Whether authorize is needed.
                               @else   是否授权。 @endif */
} SsapsReqReadCb;

typedef struct {
    uint16_t requestId;  /*!< @if Eng Request id.
                               @else   请求id。 @endif */
    uint16_t handle;      /*!< @if Eng Properity handle of the write request.
                               @else   请求写的属性句柄。 @endif */
    uint8_t type;         /*!< @if Eng property type { @ref SsapPropertyType }.
                               @else   属性类型。 @endif  { @ref SsapPropertyType } */
    bool need_rsp;        /*!< @if Eng Whether response is needed.
                               @else   是否需要发送响应。 @endif */
    bool needAuthorize;  /*!< @if Eng Whether authorize is needed.
                               @else   是否授权。 @endif */
    uint16_t length;      /*!< @if Eng Length of write request data.
                               @else   请求写的数据长度。 @endif */
    uint8_t *value;       /*!< @if Eng Write request data.
                               @else   请求写的数据。 @endif */
} SsapsReqWriteCb;

/**
 * @if Eng
 * @brief Struct of send notification/indication information.
 * @else
 * @brief 发送通知/指示信息。
 * @endif
 */
typedef struct {
    uint16_t handle;      /*!< @if Eng Properity handle.
                               @else   属性句柄。 @endif */
    uint8_t type;         /*!< @if Eng property type { @ref SsapPropertyType }.
                               @else   属性类型 { @ref SsapPropertyType }。 @endif */
    uint16_t valueLen;   /*!< @if Eng Length of notification/indication data.
                               @else   通知/指示数据长度。 @endif */
    uint8_t *value;       /*!< @if Eng Notification/indication data.
                               @else   发送的通知/指示数据。 @endif */
} SsapsNtfIndByUuid;

ErrCodeType SsapsRegisterServer(SleUuid *appUuid, uint8_t *serverId);

ErrCodeType SsapsUnregisterServer(uint8_t serverId);

ErrCodeType SsapsAddService(uint8_t serviceId, SleUuid *serviceUuid, bool isPrimary, uint16_t *handle);

ErrCodeType  SsapsAddProperty(uint8_t serviceId, uint16_t serviceHandle, SsapsPropertyInfo *property, uint16_t *handle);

ErrCodeType SsapsAddDescriptor(uint8_t serviceId, uint16_t serviceHandle, uint16_t propertyHandle,
    SsapsDescInfo *descriptor);

ErrCodeType SsapsStartService(uint8_t serviceId, uint16_t serviceHandle);

ErrCodeType SsapsDeleteAllServices(uint8_t serviceId);

ErrCodeType SsapsSendResponse(uint8_t serviceId, uint16_t connId, SsapsSendRsp *param);

ErrCodeType SsapsNotifyIndicate(uint8_t serviceId, uint16_t connId, SsapsNtfInd *param);

ErrCodeType SsapsNotifyIndicateByUuid(uint8_t serviceId, uint16_t connId, SsapsNtfIndByUuid *param);

ErrCodeType SsapsSetInfo(uint8_t serviceId, SsapcExchangeInfo *info);

typedef void (*SsapsAddServiceCallback)(uint8_t server_id, SleUuid * uuid, uint16_t handle, errcode_t status);

typedef void (*SsapsAddPropertyCallback)(uint8_t server_id, SleUuid *uuid, 
uint16_t service_handle,uint16_t handle, errcode_t status);

typedef void (*SsapsAddDescriptorCallback)(uint8_t server_id, SleUuid *uuid, 
uint16_t service_handle,uint16_t property_handle, errcode_t status);


typedef void (*SsapsStartServiceCallback)(uint8_t serviceId, uint16_t handle, ErrCodeType status);

typedef void (*SsapsDeleteAllServiceCallback)(uint8_t serviceId, ErrCodeType status);

typedef void (*SsapsReadRequestCallback)(uint8_t serviceId, uint16_t connId, SsapsReqReadCb *readCbPara,
    ErrCodeType status);
	
typedef void (*SsapsWriteRequestCallback)(uint8_t serviceId, uint16_t connId, SsapsReqWriteCb *writeCbPara,
    ErrCodeType status);

typedef void (*SsapsMtuChangedCallback)(uint8_t serviceId, uint16_t connId, 
SsapcExchangeInfo *info, ErrCodeType status);

typedef struct{
    SsapsAddServiceCallback addServiceCb;               /*!< @if Eng Service added callback.
                                                                  @else   添加服务回调函数。 @endif */
    SsapsAddPropertyCallback addPropertyCb;             /*!< @if Eng Characteristc added callback.
                                                                  @else   添加特征回调函数。 @endif */
    SsapsAddDescriptorCallback addDescriptorCb;         /*!< @if Eng Descriptor added callback.
                                                                  @else   添加描述符回调函数。 @endif */
    SsapsStartServiceCallback startServiceCb;           /*!< @if Eng Service started callback.
                                                                  @else   启动服务回调函数。 @endif */
    SsapsDeleteAllServiceCallback deleteAllServiceCb; /*!< @if Eng Service deleted callback.
                                                                  @else   删除服务回调函数。 @endif */
    SsapsReadRequestCallback readRequestCb;             /*!< @if Eng Read request received callback.
                                                                  @else   收到远端读请求回调函数。 @endif */
    SsapsWriteRequestCallback writeRequestCb;           /*!< @if Eng Write request received callback.
                                                                  @else   收到远端写请求回调函数。 @endif */
    SsapsMtuChangedCallback mtuChangedCb;               /*!< @if Eng Mtu changed callback.
                                                                  @else   mtu 大小更新回调函数。 @*/
 
}SsapsCallbacks;

ErrCodeType SsapsRegisterCallbacks(SsapsCallbacks *func);

#endif
