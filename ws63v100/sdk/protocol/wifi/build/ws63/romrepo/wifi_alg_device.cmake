set(project_name ws63)
add_path_with_alias_if_exist(${WIFI_DEVICE_DIR}/source/alg wifi_alg_device)
# 添加ws63单独的alg代码
set(
    WIFI_ALG_WS63_SOURCES
#add alg src file
)
set(
    WIFI_ALG_WS63_INCLUDES
#add alg inc file
)
set(WIFI_ALG_WS63_DEFINES
#add alg defines
    _PRE_WLAN_FEATURE_TEMP_PROTECT_CLDO_MODE
)
set_property(GLOBAL PROPERTY WIFI_ALG_WS63_SOURCES ${WIFI_ALG_WS63_SOURCES})
set_property(GLOBAL PROPERTY WIFI_ALG_WS63_INCLUDES ${WIFI_ALG_WS63_INCLUDES})
set_property(GLOBAL PROPERTY WIFI_ALG_WS63_DEFINES ${WIFI_ALG_WS63_DEFINES})

set(ws63_alg_wifi_device_ram
    WIFI_SOURCES # 分隔符
# 添加 src 组件
    WIFI_ALG_COMMON_DEVICE_RAM_SOURCES
    WIFI_ALG_IOT_DEVICE_RAM_SOURCES
    WIFI_ALG_WS63_SOURCES
    WIFI_INCLUDES # 分隔符
# 添加 inc 组件
    WIFI_ALG_COMMON_INCLUDES
    WIFI_ALG_IOT_DEVICE_ROM_INCLUDES
    WIFI_ALG_IOT_DEVICE_RAM_INCLUDES
    WIFI_ALG_WS63_INCLUDES
    WIFI_DEFINES # 分隔符
# 添加 defines 组件
    WIFI_ALG_WS63_DEFINES
    WIFI_ALG_IOT_DEFINES
)

set(ws63_alg_wifi_rom
    WIFI_SOURCES # 分隔符
# 添加 src 组件
    WIFI_ALG_COMMON_DEVICE_ROM_SOURCES
    WIFI_ALG_IOT_DEVICE_ROM_SOURCES

    WIFI_INCLUDES # 分隔符
# 添加 inc 组件
    WIFI_ALG_COMMON_INCLUDES
    WIFI_ALG_IOT_HOST_INCLUDES
    WIFI_ALG_IOT_DEVICE_ROM_INCLUDES
    WIFI_ALG_IOT_DEVICE_RAM_INCLUDES
    WIFI_ALG_WS63_INCLUDES
    WIFI_DEFINES # 分隔符
# 添加 defines 组件
    WIFI_ALG_WS63_DEFINES
    WIFI_ALG_IOT_DEFINES
)
