#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================

if((DEFINED CONFIG_ADC_USING_V154) OR (DEFINED CONFIG_ADC_USING_V155))
    set(SOURCES "${SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}/adc_demo_inc.c" PARENT_SCOPE)
else()
    set(SOURCES "${SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}/adc_demo.c" PARENT_SCOPE)
endif()