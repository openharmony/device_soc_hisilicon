#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
set(COMPONENT_NAME "sfc_rom")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sfc.c
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()

install_sdk(${CMAKE_CURRENT_SOURCE_DIR}/ "*.h")
