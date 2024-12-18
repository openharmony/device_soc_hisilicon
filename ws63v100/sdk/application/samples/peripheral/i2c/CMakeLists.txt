#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
if(DEFINED CONFIG_SAMPLE_SUPPORT_I2C_MASTER)
    set(SOURCES "${SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}/i2c_master_demo.c" PARENT_SCOPE)
elseif(DEFINED CONFIG_SAMPLE_SUPPORT_I2C_SLAVE)
    set(SOURCES "${SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}/i2c_slave_demo.c" PARENT_SCOPE)
endif()