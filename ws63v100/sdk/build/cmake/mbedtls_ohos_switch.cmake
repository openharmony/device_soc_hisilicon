#===============================================================================
# @brief    mbedtls OHOS source-switch dual-gate (marker file + EXISTS check)
# Copyright (c) 2026 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
# The marker file mbedtls_ohos_switch.marker is written by OHOS GN
# (third_party/mbedtls/BUILD.gn write_file) on each gn gen with the current
# mbedtls_ohos_switch truth value ("1"/"0", overwritten, no stale residue).
# hm_build.sh copies it to ${BIN_DIR}/${CHIP}/libs/ohos/${TARGET_COMMAND}/.
#
# Source switch is active = marker is "1" AND libmbedtls_ohos.a exists.
# Dual-gate prevents:
#   - Stale .a (switch off but old .a not cleaned) causing false positive
#   - Marker=1 but .a missing (OHOS build error) entering switch path and failing link
# Usage: include this file, then check MBEDTLS_OHOS_SWITCH_ON.
set(_MBEDTLS_OHOS_MK "${BIN_DIR}/${CHIP}/libs/ohos/${TARGET_COMMAND}/mbedtls_ohos_switch.marker")
set(MBEDTLS_OHOS_SWITCH_ON FALSE)
if(EXISTS ${_MBEDTLS_OHOS_MK})
    file(READ ${_MBEDTLS_OHOS_MK} _MBEDTLS_OHOS_MK_VAL)
    string(STRIP "${_MBEDTLS_OHOS_MK_VAL}" _MBEDTLS_OHOS_MK_VAL)
    if(_MBEDTLS_OHOS_MK_VAL STREQUAL "1")
        set(MBEDTLS_OHOS_SWITCH_ON TRUE)
    endif()
endif()
if(NOT EXISTS ${BIN_DIR}/${CHIP}/libs/ohos/${TARGET_COMMAND}/libmbedtls_ohos.a)
    set(MBEDTLS_OHOS_SWITCH_ON FALSE)
endif()
