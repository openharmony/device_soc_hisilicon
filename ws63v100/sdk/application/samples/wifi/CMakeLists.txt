#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
if(DEFINED CONFIG_SAMPLE_SUPPORT_STA_SAMPLE)
    add_subdirectory_if_exist(sta_sample)
endif()

if(DEFINED CONFIG_SAMPLE_SUPPORT_SOFTAP_SAMPLE)
    add_subdirectory_if_exist(softap_sample)
endif()

if(DEFINED CONFIG_SAMPLE_SUPPORT_BLE_WIFI_CFG_SAMPLE)
    add_subdirectory_if_exist(ble_wifi_cfg_sample)
endif()

if(DEFINED CONFIG_SAMPLE_SUPPORT_SYSCHANNEL_DEV)
    add_subdirectory_if_exist(syschannel_dev)
endif()

if(DEFINED CONFIG_SUPPORT_HILINK)
    add_subdirectory_if_exist(ohos_connect)
endif()

if (DEFINES MATCHES "CONFIG_SUPPORT_HILINK_INDIE_UPGRADE")
    add_subdirectory_if_exist(hilink_indie_upgrade)
endif()

if(DEFINED CONFIG_SUPPORT_UAPI)
    add_subdirectory_if_exist(uhapi)
endif()

set(SOURCES "${SOURCES}" PARENT_SCOPE)