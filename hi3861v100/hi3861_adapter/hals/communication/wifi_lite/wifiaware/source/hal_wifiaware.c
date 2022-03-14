/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
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
 */

#include "hi_cipher.h"
#include "hi_wifi_api.h"
#include "hi_wifi_sdp_api.h"
#include "wifiaware.h"

int HalWifiSdpInit(const char* ifname)
{
    if (hi_wifi_sdp_init(ifname) != HISI_OK) {
        return -1;
    }
    return 0;
}

unsigned int HalCipherHashSha256(const char* input, unsigned int inputLen, unsigned char* hash, unsigned hashLen)
{
    if (hi_cipher_hash_sha256((uintptr_t)input, inputLen, hash, hashLen) != HISI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpStartService(const char* svcName, unsigned char localHandle, RecvCallback recvCB, unsigned char role)
{
    if (hi_wifi_sdp_start_service(svcName, localHandle, (hi_wifi_sdp_recv_cb)recvCB, role) != HISI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpSend(unsigned char* macAddr, unsigned char peerHandle, unsigned char localHandle,
    unsigned char* msg, int len)
{
    if (hi_wifi_sdp_send(macAddr, peerHandle, localHandle, msg, len) != HISI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpStopService(unsigned char localHandle, unsigned char role)
{
    if (hi_wifi_sdp_stop_service(localHandle, role) != HISI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpDeinit(void)
{
    if (hi_wifi_sdp_deinit() != HISI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpAdjustTxPower(const char *ifname, signed char power)
{
    if (hi_wifi_sdp_adjust_tx_power(ifname, power) != HISI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpRestoreTxPower(const char *ifname)
{
    if (hi_wifi_sdp_restore_tx_power(ifname) != HISI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpBeaconSwitch(const char *ifname, unsigned char enable)
{
    if (hi_wifi_sdp_beacon_switch(ifname, enable) != HISI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpSetRetryTimes(unsigned int retries)
{
    if (hi_wifi_sdp_set_retry_times(retries) != HISI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpGetSyncMode(void)
{
    return hi_wifi_sdp_get_sync_mode();
}