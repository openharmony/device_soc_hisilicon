#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================

add_subdirectory_if_exist(bootloader)
add_subdirectory_if_exist(drivers)
add_subdirectory_if_exist(middleware)
add_subdirectory_if_exist(open_source)
add_subdirectory_if_exist(include)
add_subdirectory_if_exist(kernel)