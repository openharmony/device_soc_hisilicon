# @brief    cmake make file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.

set(COMPONENT_NAME "cmn_header")

add_subdirectory_if_exist(driver)
add_subdirectory_if_exist(middleware)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/middleware/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/middleware/services/wifi
    ${CMAKE_CURRENT_SOURCE_DIR}/middleware/services/radar
)

build_component()
