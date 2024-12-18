#===============================================================================
# @brief    cmake make file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
add_subdirectory_if_exist(chips)
add_subdirectory_with_alias_if_exist(services ${PROJECT_BINARY_DIR}/services)
add_subdirectory_with_alias_if_exist(utils ${PROJECT_BINARY_DIR}/utils)