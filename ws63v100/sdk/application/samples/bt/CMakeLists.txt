#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
if(DEFINED CONFIG_SAMPLE_SUPPORT_BLE_SAMPLE)
    add_subdirectory_if_exist(ble)
endif()
if(DEFINED CONFIG_SAMPLE_SUPPORT_SLE_SAMPLE)
    add_subdirectory_if_exist(sle)
endif()

set(SOURCES "${SOURCES}" PARENT_SCOPE)
set(PUBLIC_HEADER "${PUBLIC_HEADER}" PARENT_SCOPE)