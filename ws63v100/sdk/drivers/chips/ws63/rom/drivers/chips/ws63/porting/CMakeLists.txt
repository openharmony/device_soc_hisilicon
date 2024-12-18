#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================

add_subdirectory_if_exist(pinctrl)
add_subdirectory_if_exist(systick)
add_subdirectory_if_exist(tcxo)
add_subdirectory_if_exist(timer)
add_subdirectory_if_exist(watchdog)
add_subdirectory_if_exist(version)