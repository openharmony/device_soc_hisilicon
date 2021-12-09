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

#include "wifi_hotspot.h"
#include <securec.h>
#include "hi_wifi_api.h"
#include "lwip/netifapi.h"
#include "wifi_device_util.h"

#define RSSI_LEVEL_4_2_G (-65)
#define RSSI_LEVEL_3_2_G (-75)
#define RSSI_LEVEL_2_2_G (-82)
#define RSSI_LEVEL_1_2_G (-88)

#define RSSI_LEVEL_4_5_G (-65)
#define RSSI_LEVEL_3_5_G (-72)
#define RSSI_LEVEL_2_5_G (-79)
#define RSSI_LEVEL_1_5_G (-85)

#define IP_AP_ADDR0 192
#define IP_AP_ADDR1 168
#define IP_AP_ADDR2 5
#define IP_AP_ADDR3 1
#define GW_AP_ADDR0 192
#define GW_AP_ADDR1 168
#define GW_AP_ADDR2 5
#define GW_AP_ADDR3 1
#define NETMSK_ADDR0 255
#define NETMSK_ADDR1 255
#define NETMSK_ADDR2 255
#define NETMSK_ADDR3 0

#define WIFI_TPC_MAX_POWER 20
#define WIFI_TPC_ID 35
#define WIFI_TPC_LEN 2
#define WLAN_AP_NAME "ap0"

static int g_wifiApStatus = WIFI_HOTSPOT_NOT_ACTIVE;
static HotspotConfig g_wifiApConfig = {0};
static char g_wifiIfName[WIFI_IFNAME_MAX_SIZE + 1] = {0};
typedef struct {
    unsigned char id;
    unsigned char len;
    signed char power;
    unsigned char margin;
} TpcElement;

static WifiErrorCode SetHotspotIpConfig(void)
{
    struct netif *netif = NULL;
    netif = netif_find(WLAN_AP_NAME);
    if (netif == NULL) {
        printf("get netif failed\r\n");
        return ERROR_WIFI_UNKNOWN;
    }
    ip4_addr_t ipAddr;
    ip4_addr_t netMask;
    ip4_addr_t gw;

    IP4_ADDR(&ipAddr, IP_AP_ADDR0, IP_AP_ADDR1, IP_AP_ADDR2, IP_AP_ADDR3);
    IP4_ADDR(&netMask, NETMSK_ADDR0, NETMSK_ADDR1, NETMSK_ADDR2, NETMSK_ADDR3);
    IP4_ADDR(&gw, GW_AP_ADDR0, GW_AP_ADDR1, GW_AP_ADDR2, GW_AP_ADDR3);

    netifapi_netif_set_addr(netif, &ipAddr, &netMask, &gw);

    if (netifapi_dhcps_start(netif, 0, 0) != 0) {
        printf("dhcps shell cmd excute fail!\r\n");
        (void)hi_wifi_softap_stop();
        return ERROR_WIFI_UNKNOWN;
    }
    return WIFI_SUCCESS;
}

WifiErrorCode EnableHotspot(void)
{
    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (g_wifiApStatus == WIFI_HOTSPOT_ACTIVE) {
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_BUSY;
    }

    hi_wifi_softap_config hapdConf = {0};

    hapdConf.channel_num = HOTSPOT_DEFAULT_CHANNEL;
    if (g_wifiApConfig.channelNum) {
        hapdConf.channel_num = g_wifiApConfig.channelNum;
    }
    hapdConf.authmode = HoSecToHiSec(g_wifiApConfig.securityType);

    int cpyErr = memcpy_s(hapdConf.ssid, WIFI_MAX_SSID_LEN, g_wifiApConfig.ssid, HI_WIFI_MAX_SSID_LEN + 1);
    cpyErr += memcpy_s(hapdConf.key, WIFI_MAX_KEY_LEN, g_wifiApConfig.preSharedKey, HI_WIFI_MAX_KEY_LEN + 1);
    if (cpyErr != EOK) {
        printf("[wifi_service]:EnableHotspot memcpy fail, err = %d\n", cpyErr);
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_UNKNOWN;
    }

    int len = sizeof(g_wifiIfName);
    int hiRet = hi_wifi_softap_start(&hapdConf, g_wifiIfName, &len);
    if (hiRet != HISI_OK) {
        printf("[wifi_service]:EnableHotspot softap start fail, err = %d\n", hiRet);
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_NOT_STARTED;
    }
    cpyErr = memset_s(&hapdConf, sizeof(hi_wifi_softap_config), 0, sizeof(hi_wifi_softap_config));
    if (cpyErr != EOK) {
        printf("[wifi_service]:EnableHotspot memset fail, err = %d\n", cpyErr);
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_UNKNOWN;
    }

    g_wifiApStatus = WIFI_HOTSPOT_ACTIVE;
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }

    if (SetHotspotIpConfig() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    return WIFI_SUCCESS;
}

WifiErrorCode DisableHotspot(void)
{
    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (g_wifiApStatus == WIFI_HOTSPOT_NOT_ACTIVE) {
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_NOT_STARTED;
    }

    int hiRet = hi_wifi_softap_stop();
    if (hiRet != HISI_OK) {
        printf("[wifi_service]:DisableHotspot failed to stop softap, err = %d\n", hiRet);
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_NOT_STARTED;
    }

    g_wifiApStatus = WIFI_HOTSPOT_NOT_ACTIVE;
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    return WIFI_SUCCESS;
}

WifiErrorCode SetHotspotConfig(const HotspotConfig* config)
{
    if (config == NULL) {
        return ERROR_WIFI_INVALID_ARGS;
    }

    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    int cpyErr = memcpy_s(&g_wifiApConfig, sizeof(HotspotConfig), config, sizeof(HotspotConfig));
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (cpyErr != EOK) {
        printf("[wifi_service]:SetHotspotConfig memcpy fail, err = %d\n", cpyErr);
        return ERROR_WIFI_UNKNOWN;
    }
    return WIFI_SUCCESS;
}

WifiErrorCode GetHotspotConfig(HotspotConfig* result)
{
    if (result == NULL) {
        return ERROR_WIFI_INVALID_ARGS;
    }

    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    int cpyErr =  memcpy_s(result, sizeof(HotspotConfig), &g_wifiApConfig, sizeof(HotspotConfig));
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (cpyErr != EOK) {
        printf("[wifi_service]:SetHotspotConfig memcpy fail, err = %d\n", cpyErr);
        return ERROR_WIFI_UNKNOWN;
    }
    return WIFI_SUCCESS;
}

int IsHotspotActive(void)
{
    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    int ret = g_wifiApStatus;
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }

    return ret;
}

static int GetPeerSTAWifiIp(const unsigned char *macAddress, int macLen, unsigned int * const ip)
{
    int ret;
    ip4_addr_t clientIP;
    struct netif *netif_node = netif_find(WLAN_AP_NAME);
    if (netif_node == NULL) {
        printf("GetPeerSTAWifiIp netif get fail\r\n");
        return HISI_FAIL;
    }

    ret = netifapi_dhcps_get_client_ip(netif_node, macAddress, macLen, &clientIP);
    if (ret == 0) {
        *ip = ntohl(ip4_addr_get_u32(&clientIP));
        return HISI_OK;
    }

    return HISI_FAIL;
}

WifiErrorCode GetStationList(StationInfo* result, unsigned int* size)
{
    if (result == NULL || size == NULL || *size == 0) {
        return ERROR_WIFI_INVALID_ARGS;
    }

    hi_wifi_ap_sta_info staList[WIFI_MAX_STA_NUM] = {0};
    unsigned int staNum = WIFI_MAX_STA_NUM;

    int hiRet = hi_wifi_softap_get_connected_sta(staList, &staNum);
    if (hiRet != HISI_OK) {
        printf("[wifi_service]:GetStationList get connected sta failed, err = %d\r\n", hiRet);
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    if (*size < staNum) {
        return ERROR_WIFI_INVALID_ARGS;
    }

    int cpyErr;
    for (unsigned int i = 0; i < staNum; i++) {
        cpyErr = memcpy_s(result[i].macAddress, WIFI_MAC_LEN, staList[i].mac, HI_WIFI_MAC_LEN);
        if (cpyErr != EOK) {
            printf("[wifi_service]:GetStationList memcpy fail, err = %d\r\n", cpyErr);
            return ERROR_WIFI_UNKNOWN;
        }
        result[i].name = NULL;

#if LWIP_NETIFAPI_DHCPS_IP
        int ret = GetPeerSTAWifiIp(staList[i].mac, WIFI_MAC_LEN, &(result[i].ipAddress));
        if (ret != HISI_OK) {
            printf("[wifi_service]:GetPeerSTAWifiIp  fail, err = %d\r\n", ret);
            return ERROR_WIFI_UNKNOWN;
        }
#endif
    }
    *size = staNum;
    return WIFI_SUCCESS;
}

WifiErrorCode SetBand(int band)
{
    if (band != HOTSPOT_BAND_TYPE_2G) {
        return ERROR_WIFI_NOT_SUPPORTED;
    }
    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    g_wifiApConfig.band = band;
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    return WIFI_SUCCESS;
}

WifiErrorCode GetBand(int* result)
{
    if (result == NULL) {
        return ERROR_WIFI_INVALID_ARGS;
    }

    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (g_wifiApConfig.band == 0) {
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_NOT_AVAILABLE;
    }
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    *result = HOTSPOT_BAND_TYPE_2G;
    return WIFI_SUCCESS;
}

int GetSignalLevel(int rssi, int band)
{
    if (band == HOTSPOT_BAND_TYPE_2G) {
        if (rssi >= RSSI_LEVEL_4_2_G) {
            return RSSI_LEVEL_4;
        }
        if (rssi >= RSSI_LEVEL_3_2_G) {
            return RSSI_LEVEL_3;
        }
        if (rssi >= RSSI_LEVEL_2_2_G) {
            return RSSI_LEVEL_2;
        }
        if (rssi >= RSSI_LEVEL_1_2_G) {
            return RSSI_LEVEL_1;
        }
    }

    if (band == HOTSPOT_BAND_TYPE_5G) {
        if (rssi >= RSSI_LEVEL_4_5_G) {
            return RSSI_LEVEL_4;
        }
        if (rssi >= RSSI_LEVEL_3_5_G) {
            return RSSI_LEVEL_3;
        }
        if (rssi >= RSSI_LEVEL_2_5_G) {
            return RSSI_LEVEL_2;
        }
        if (rssi >= RSSI_LEVEL_1_5_G) {
            return RSSI_LEVEL_1;
        }
    }

    return ERROR_WIFI_INVALID_ARGS;
}

int GetHotspotChannel(void)
{
    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (g_wifiApStatus == WIFI_HOTSPOT_NOT_ACTIVE) {
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_NOT_STARTED;
    }

    int channel = hi_wifi_get_channel(g_wifiIfName, WIFI_IFNAME_MAX_SIZE + 1);
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (channel == HI_WIFI_INVALID_CHANNEL) {
        return ERROR_WIFI_INVALID_ARGS;
    }

    return channel;
}

WifiErrorCode GetHotspotInterfaceName(char* result, int size)
{
    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (g_wifiApStatus == WIFI_HOTSPOT_NOT_ACTIVE) {
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_NOT_STARTED;
    }

    int cpyErr = memcpy_s(result, size, g_wifiIfName, WIFI_IFNAME_MAX_SIZE + 1);
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (cpyErr != EOK) {
        printf("[wifi_service]:getifname memcpy fail, err = %d\n", cpyErr);
        return ERROR_WIFI_UNKNOWN;
    }
    return WIFI_SUCCESS;
}

WifiErrorCode DisassociateSta(unsigned char* mac, int macLen)
{
    int ret = hi_wifi_softap_deauth_sta(mac, macLen);
    if (ret != HISI_OK) {
        return ERROR_WIFI_UNKNOWN;
    }
    return WIFI_SUCCESS;
}

WifiErrorCode AddTxPowerInfo(int power)
{
    TpcElement tpc = { WIFI_TPC_ID, WIFI_TPC_LEN, 0, 0 };
    if (power > WIFI_TPC_MAX_POWER) {
        printf("[wifi_service]:Invalid TPC Power (%d)\n", power);
        return ERROR_WIFI_INVALID_ARGS;
    } else {
        tpc.power = power;
    }

    int ret = hi_wifi_add_usr_app_ie(HI_WIFI_IFTYPE_AP, HI_WIFI_FRAME_TYPE_BEACON, HI_WIFI_USR_IE_TYPE_DEFAULT,
                                     (const unsigned char *)&tpc, sizeof(tpc));
    if (ret != HISI_OK) {
        printf("[wifi_service]:Wifi Add Beacon IE Fail (%d)\n", ret);
        (void)hi_wifi_delete_usr_app_ie(HI_WIFI_IFTYPE_AP, HI_WIFI_FRAME_TYPE_BEACON, HI_WIFI_USR_IE_TYPE_DEFAULT);
        return ret;
    }

    ret = hi_wifi_add_usr_app_ie(HI_WIFI_IFTYPE_AP, HI_WIFI_FRAME_TYPE_PROBE_RSP, HI_WIFI_USR_IE_TYPE_DEFAULT,
                                 (const unsigned char *)&tpc, sizeof(tpc));
    if (ret != HISI_OK) {
        printf("Wifi Add ProbResp IE Fail (%d)\n", ret);
        (void)hi_wifi_delete_usr_app_ie(HI_WIFI_IFTYPE_AP, HI_WIFI_FRAME_TYPE_PROBE_RSP, HI_WIFI_USR_IE_TYPE_DEFAULT);
        return ret;
    }
    return WIFI_SUCCESS;
}
