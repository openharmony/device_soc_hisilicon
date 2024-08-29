#===============================================================================
# @brief    cmake file
# Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
#===============================================================================

if(${TARGET_COMMAND} MATCHES "ws63-liteos-xts")
    set(COMPONENT_LIST "begetutil" "devattest_sdk" "hal_token_static" "hilog_lite_static" "huks_test_common" "module_ActsBootstrapTest" "module_ActsSamgrTest" "samgr_adapter" "bootstrap" "fsmanager_static" "hal_update_static" "hilog_static" "inithook" "module_ActsDeviceAttestTest" "module_ActsUpdaterFuncTest" "samgr_source"
            "broadcast" "hal_file_static" "hctest" "hiview_lite_static" "init_log" "module_ActsDfxFuncTest" "native_file" "udidcomm"
            "cjson_static" "hal_sys_param" "hichainsdk" "hota" "init_utils" "module_ActsHieventLiteTest" "param_client_lite"
            "devattest_core" "hal_sysparam" "hievent_lite_static" "huks_3.0_sdk"  "module_ActsHuksHalFunctionTest" "samgr"
            "blackbox_lite"  "hidumper_mini")
elseif(${TARGET_COMMAND} MATCHES "ws63-liteos-app")
set(COMPONENT_LIST

        "begetutil"   "hilog_lite_static" "samgr_adapter" "bootstrap" "fsmanager_static" "hal_update_static" "hilog_static" "inithook"   "samgr_source"
        "broadcast" "hal_file_static"   "init_log"  "native_file" "udidcomm"
        "cjson_static" "hal_sys_param" "hichainsdk" "hota" "init_utils"  "param_client_lite"
        "hiview_lite_static" "hal_sysparam" "hievent_lite_static" "huks_3.0_sdk"   "samgr" "blackbox_lite" "hal_iothardware" "wifiservice"
        "hidumper_mini" "ble_lite"  "sle_lite")
endif()


foreach(COMPONENT ${COMPONENT_LIST})
    set(COMPONENT_NAME ${COMPONENT})
    if (NOT ${COMPONENT_NAME} IN_LIST TARGET_COMPONENT)
        continue()
    endif()
    set(LIBS ${BIN_DIR}/${CHIP}/libs/ohos/${TARGET_COMMAND}/lib${COMPONENT_NAME}.a)
    set(WHOLE_LINK true)
    build_component()
    install_sdk(${BIN_DIR}/${CHIP}/libs/ohos/${TARGET_COMMAND} "*.a")
endforeach()
