#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
add_subdirectory_if_exist(host)
add_subdirectory_if_exist(slave)
if(syschannel_host_ko IN_LIST TARGET_COMPONENT)
install_sdk(${CMAKE_CURRENT_SOURCE_DIR}/host "*")
install_sdk(${CMAKE_CURRENT_SOURCE_DIR}/cfg "*")
install_sdk(${CMAKE_CURRENT_SOURCE_DIR}/comm "*")
endif()