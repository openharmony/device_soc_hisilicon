#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
set(COMPONENT_NAME "osal_adapt")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osal_adapt_atomic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osal_adapt_event.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osal_adapt_string.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osal_adapt_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osal_adapt_timer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osal_adapt_debug.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

set(WHOLE_LINK true)

build_component()
