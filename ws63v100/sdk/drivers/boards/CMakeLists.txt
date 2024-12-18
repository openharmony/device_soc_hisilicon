#===============================================================================
# @brief    cmake make file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}_${BOARD})
    add_subdirectory(${CHIP}_${BOARD})
endif()

add_subdirectory_if_exist(${CHIP})