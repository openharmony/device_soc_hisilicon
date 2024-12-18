#===============================================================================
# @brief    cmake make file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
add_subdirectory_if_exist(non_os)
if(DEFINED OS AND NOT ${OS} STREQUAL "non-os")
    add_subdirectory_if_exist(${OS})
endif()
add_subdirectory_if_exist(osal)
add_subdirectory_if_exist(dpal)
add_subdirectory_if_exist(osal_adapt)