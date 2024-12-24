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
#ifndef OHOS_SLE_DEVICE_DISCOVERY_H
#define OHOS_SLE_DEVICE_DISCOVERY_H

#include "sle_device_discovery.h"
#include "ohos_sle_errcode.h"
#include "ohos_sle_common.h"

/**
 * @if Eng
 * @brief  Announce data.
 * @else
 * @brief  设备公开数据。
 * @endif
 */
typedef struct SleAnnounceData {
    uint16_t announceDataLen; /*!< @if Eng announce data length
                                      @else   设备公开数据长度 @endif */
    uint16_t seekRspDataLen;  /*!< @if Eng scan response data length
                                      @else   扫描响应数据长度 @endif */
    uint8_t  *announceData;    /*!< @if Eng announce data
                                      @else   设备公开数据 @endif */
    uint8_t  *seekRspData;     /*!< @if Eng scan response data
                                      @else   扫描响应数据 @endif */
} SleAnnounceData;


typedef struct SleSeekParam {
    uint8_t ownaddrtype;                        /*!< @if Eng own address type
                                                       @else   本端地址类型 @endif */
    uint8_t filterduplicates;                    /*!< @if Eng duplicates filter
                                                       @else   重复过滤开关，0：关闭，1：开启 @endif */
    uint8_t seekfilterpolicy;                   /*!< @if Eng scan filter policy { @ref SleSeekFilterType }
                                                       @else   扫描设备使用的过滤类型，
                                                               { @ref SleSeekFilterType } @endif */
    uint8_t seekphys;                            /*!< @if Eng scan PHY type { @ref SleSeekPhyType }
                                                       @else   扫描设备所使用的PHY，{ @ref SleSeekPhyType }
                                                       @endif */
    uint8_t seekType[OH_SLE_SEEK_PHY_NUM_MAX];      /*!< @if Eng scan type { @ref sle_seek_scan_t }
                                                       @else   扫描类型，{ @ref SleSeekType }
                                                       @endif */
    uint16_t seekInterval[OH_SLE_SEEK_PHY_NUM_MAX]; /*!< @if Eng scan interval
                                                       @else   扫描间隔，取值范围[0x0004, 0xFFFF]，time = N * 0.125ms
                                                       @endif */
    uint16_t seekWindow[OH_SLE_SEEK_PHY_NUM_MAX];   /*!< @if Eng scan window
                                                       @else   扫描窗口，取值范围[0x0004, 0xFFFF]，time = N * 0.125ms
                                                       @endif */
} SleSeekParam;

typedef struct SleSeekResultInfo {
    uint8_t eventType;                /*!< @if Eng event type
                                            @else   上报事件类型 @endif */
    SleAddr addr;                   /*!< @if Eng address
                                            @else   地址 @endif */
    SleAddr directAddr;            /*!< @if Eng direct address
                                            @else   定向发现地址 @endif */
    uint8_t rssi;                      /*!< @if Eng rssi
                                            @else   信号强度指示，取值范围[-127dBm, 20dBm]，0x7F表示不提供信号强度指示
                                            @endif */
    uint8_t dataStatus;               /*!< @if Eng data status
                                            @else   数据状态 @endif */
    uint8_t dataLength;               /*!< @if Eng data length
                                            @else   数据长度 @endif */
    uint8_t *data;                     /*!< @if Eng data
                                            @else   数据 @endif */
} SleSeekResultInfo;


typedef struct SleAnnounceParam {
    uint8_t  announceHandle;              /*!< @if Eng announce handle
                                                 @else   设备公开句柄，取值范围[0, 0xFF] @endif */
    uint8_t  announceMode;                /*!< @if Eng announce mode { @ref SleAnnounceModeType }
                                                 @else   设备公开类型， { @ref SleAnnounceModeType } @endif */
    uint8_t  announceGtRole;             /*!< @if Eng G/T role negotiation indication
                                                         { @ref SleAnnounceGtRoleType }
                                                 @else   G/T 角色协商指示，
                                                         { @ref SleAnnounceGtRoleType } @endif */
    uint8_t  announceLevel;               /*!< @if Eng announce level
                                                         { @ref SleAnnounceLevelType }
                                                 @else   发现等级，
                                                         { @ref SleAnnounceLevelType } @endif */
    uint32_t announceIntervalMin;        /*!< @if Eng minimum of announce interval
                                                 @else   最小设备公开周期, 0x000020~0xffffff, 单位125us @endif */
    uint32_t announceIntervalMax;        /*!< @if Eng maximum of announce interval
                                                 @else   最大设备公开周期, 0x000020~0xffffff, 单位125us @endif */
    uint8_t  announceChannelMap;         /*!< @if Eng announce channel map
                                                 @else   设备公开信道, 0:76, 1:77, 2:78 @endif */
    
    uint8_t announce_tx_power;
    
    SleAddr ownAddr;                    /*!< @if Eng own address
                                                 @else   本端地址 @endif */
    SleAddr peerAddr;                   /*!< @if Eng peer address
                                                 @else   对端地址 @endif */
    uint16_t connIntervalMin;             /*!< @if Eng minimum of connection interval
                                                 @else   连接间隔最小取值，取值范围[0x001E,0x3E80]，
                                                         announce_gt_role 为 OH_SLE_ANNOUNCE_ROLE_T_NO_NEGO
                                                         时无需配置 @endif */
    uint16_t connIntervalMax;             /*!< @if Eng maximum of connection interval
                                                 @else   连接间隔最大取值，取值范围[0x001E,0x3E80]，
                                                         announce_gt_role 为 OH_SLE_ANNOUNCE_ROLE_T_NO_NEGO
                                                         无需配置 @endif */
    uint16_t connMaxLatency;              /*!< @if Eng max connection latency
                                                 @else   最大休眠连接间隔，取值范围[0x0000,0x01F3]，
                                                         announce_gt_role 为 OH_SLE_ANNOUNCE_ROLE_T_NO_NEGO
                                                         无需配置 @endif */
    uint16_t connSupervisionTimeout;      /*!< @if Eng connect supervision timeout
                                                 @else   最大超时时间，取值范围[0x000A,0x0C80]，
                                                         announce_gt_role 为 OH_SLE_ANNOUNCE_ROLE_T_NO_NEGO
                                                         无需配置 @endif */
    void *extParam;                        /*!< @if Eng extend parameter, default value is NULL
                                                 @else   扩展设备公开参数, 缺省时置空 @endif */
} SleAnnounceParam;



bool EnableSle(void);

bool DisableSle(void);

bool  SetLocalSleAddress(SleAddr *addr);

SleAddr* GetLocalSleAddress(void);

ErrCodeType SleSetLocalName(const uint8_t *name, uint8_t len);

ErrCodeType SleGetLocalName(uint8_t *name, uint8_t *len);

ErrCodeType SleSetAnnounceData(uint8_t announceId, const SleAnnounceData *data);

ErrCodeType SleStartAnnounce(uint8_t announceId);

ErrCodeType SleSetAnnounceParam(uint8_t announceId, const SleAnnounceParam *param);

ErrCodeType SleStopAnnounce(uint8_t announceId);

ErrCodeType SleSetSeekParam(SleSeekParam *param);

ErrCodeType SleStartSeek(void);

ErrCodeType SleStopSeek(void);



int SleSetSecurityIoCap(uint16_t IoCapacity);

int SleSetSecurityAuthReq(uint16_t secAuthReq);

int SleSsapSecurityRsp(SleAddr *addr, bool accept);

typedef void (*SleEnableCallback)(ErrCodeType status);

typedef void (*SleDisableCallback)(ErrCodeType status);

typedef void (*SleAnnounceEnableCallback)(uint32_t announceId, ErrCodeType status);

typedef void (*SleAnnounceDisableCallback)(uint32_t announceId, ErrCodeType status);

typedef void (*SleAnnounceTerminalCallback)(uint32_t announceId);

typedef void (*SleSeekResultCallback)(SleSeekResultInfo *SeekResultData);


typedef struct{
	SleEnableCallback sleEnableCb;
	SleDisableCallback sleDisableCb;
	SleAnnounceEnableCallback sleAnnounceEnableCb;
	SleAnnounceDisableCallback sleAnnounceDisableCb;
	SleAnnounceTerminalCallback sleAnnounceTerminalCb;
	SleSeekResultCallback sleSeekResultCb;
	
}SleAnnounceSeekCallbacks;


ErrCodeType SleAnnounceSeekRegisterCallbacks(SleAnnounceSeekCallbacks *func);
#endif