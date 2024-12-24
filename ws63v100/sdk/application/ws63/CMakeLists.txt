#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
add_subdirectory_if_exist(ws63_liteos_application)
add_subdirectory_if_exist(ws63_liteos_testsuite)
add_subdirectory_if_exist(ws63_freertos_application)
add_subdirectory_if_exist(ws63_freertos_testsuite)
add_subdirectory_if_exist(ws63_alios_application)
add_subdirectory_if_exist(ws63_alios_testsuite)
add_subdirectory_if_exist(ws63_ate)
add_subdirectory_if_exist(ws63_liteos_mfg)