#===============================================================================
# @brief    cmake file
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
#===============================================================================
set(COMPONENT_NAME "watchdog_rom")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/watchdog.c
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_WATCHDOG
    SUPPORT_CHIP_WATCHDOG
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
