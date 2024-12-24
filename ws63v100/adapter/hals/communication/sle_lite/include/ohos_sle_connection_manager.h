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
#ifndef OHOS_SLE_CONNECTION_MANAGER_H
#define OHOS_SLE_CONNECTION_MANAGER_H


#include "sle_connection_manager.h"
#include "ohos_sle_errcode.h"
#include "ohos_sle_common.h"

typedef struct {
    uint16_t connId;             /*!< @if Eng connection ID
                                       @else   连接ID @endif */
    uint16_t intervalMin;        /*!< @if Eng minimum interval
                                       @else   链路调度最小间隔，单位slot @endif */
    uint16_t intervalMax;        /*!< @if Eng maximum interval
                                       @else   链路调度最大间隔，单位slot @endif */
    uint16_t maxLatency;         /*!< @if Eng maximum latency
                                       @else   延迟周期，单位slot @endif */
    uint16_t supervisionTimeout; /*!< @if Eng timeout
                                       @else   超时时间，单位10ms @endif */
} SleConnectionParamUpdate;

typedef struct {
    uint16_t interval;              /*!< @if Eng interval
                                         @else   链路调度间隔，单位slot @endif */
    uint16_t latency;               /*!< @if Eng latency
                                         @else   延迟周期，单位slot @endif */
    uint16_t supervision;           /*!< @if Eng timeout
                                         @else   超时时间，单位10ms @endif */
} SleConnectionParamUpdateEvt;


ErrCodeType SleGetPairedDevicesNum(uint16_t *number);

ErrCodeType SleGetPairedDevices(SleAddr *addr, uint16_t *number);

ErrCodeType SleGetPairState(const SleAddr *addr, uint8_t *state);

ErrCodeType SleRemovePairedRemoteDevice(const SleAddr *addr);

ErrCodeType SleRemoveAllPairs(void);

ErrCodeType SleReadRemoteDeviceRssi(uint16_t connId);

ErrCodeType SleUpdateConnectParam(SleConnectionParamUpdate *params);

ErrCodeType SleConnectRemoteDevice(const SleAddr *addr);

ErrCodeType SleDisconnectRemoteDevice(const SleAddr *addr);

ErrCodeType SlePairRemoteDevice(const SleAddr *addr);




typedef void (*SleConnectStateChangedCallback)(uint16_t connId, const SleAddr *addr, SleAcbStateType connState,
 SlePairStateType pairState, SleDiscReasonType discReason);

typedef void (*SlePairCompleteCallback)(uint16_t connId, const SleAddr *addr, ErrCodeType status);

typedef void (*SleReadRssiCallback)(uint16_t connId, int8_t rssi, ErrCodeType status);

typedef void (*SleConnectParamUpdateCallback)(uint16_t connId, ErrCodeType status, const SleConnectionParamUpdateEvt *param);

typedef void (*SleConnectParamUpdateReqCallback)(uint16_t conn_id, errcode_t status,const sle_connection_param_update_req_t *param);

typedef void (*SleAuthCompleteCallback)(uint16_t conn_id, const SleAddr *addr, errcode_t status,const sle_auth_info_evt_t* evt);

typedef struct {
   SleConnectStateChangedCallback connectStateChangedCb; /*!< @if Eng Connect state changed callback.
                                                                          @else   连接状态改变回调函数。 @endif */
   SleConnectParamUpdateReqCallback connectParamUpdateReqCb;   /*!< @if Eng Connect param updated callback. 
                                                                          @else   连接参数更新回调函数。 @endif */                                                                          
   SleConnectParamUpdateCallback connectParamUpdateCb;           /*!< @if Eng Connect param updated callback.
                                                                            @else   连接参数更新回调函数。 @endif */
    SleAuthCompleteCallback authCompleteCb;                         /*!< @if Eng Authentication complete callback.
                                                                            @else   认证完成回调函数。 @endif */
    SlePairCompleteCallback pairCompleteCb;                         /*!< @if Eng Pairing complete callback.
                                                                            @else   配对完成回调函数。 @endif */
    SleReadRssiCallback readRssiCb;                                 /*!< @if Eng Read rssi callback.
                                                                            @else   读取rssi回调函数。 @endif */
} SleConnectionCallbacks;


ErrCodeType SleConnectionRegisterCallbacks(SleConnectionCallbacks *func);

#endif
