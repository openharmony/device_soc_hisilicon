#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
set(SOURCES
    "${SOURCES}"
    "${CMAKE_CURRENT_SOURCE_DIR}/radar_m_sample.c"
    PARENT_SCOPE)

set(PUBLIC_HEADER
    "${PUBLIC_HEADER}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../include/middleware/services/radar/radar_service.h"
    PARENT_SCOPE)
