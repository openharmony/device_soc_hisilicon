#===============================================================================
# @brief    cmake make file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
set(COMPONENT_NAME "hal_sfc")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_sfc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_sfc_v150.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_sfc_v150_regs_op.c
)

set(PRIVATE_HEADER
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

set(COMPONENT_CCFLAGS
)

set(WHOLE_LINK
    true
)
build_component()
