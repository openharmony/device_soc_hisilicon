#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import os

# ========== Kconfig 配置文件动态修改功能 ==========
def modify_kconfig_file(config_file, changes):
    """
    修改 Kconfig 配置文件
    策略：如果配置不一致，则注释掉之前的，并挨着增加一行新配置
    Args:
        config_file: .config 文件路径
        changes: dict, key为配置项名称(可含或不含CONFIG_前缀), value为新值
                 value可以是: 'y', 'n', 具体值字符串, None/False(删除)
    """
    import re
    
    if not os.path.exists(config_file):
        print(f"Warning: Kconfig file {config_file} not found, skip modification")
        return False
    
    with open(config_file, 'r') as f:
        lines = f.readlines()
    
    modified_lines = []
    modified = False
    
    for line in lines:
        line_stripped = line.rstrip('\n')
        matched = False
        
        for key, value in changes.items():
            # 智能处理配置名前缀：
            # - CONFIG_ 开头：OpenHarmony Kconfig 配置
            # - LOSCFG_ 开头：LiteOS Kconfig 配置
            # - 其他：自动添加 CONFIG_ 前缀（默认行为）
            if key.startswith('CONFIG_') or key.startswith('LOSCFG_'):
                config_name = key
            else:
                config_name = f'CONFIG_{key}'
            
            # 检查当前行是否匹配这个配置项
            # 匹配模式1: CONFIG_XXX=y 或 CONFIG_XXX=value
            pattern_set = f'^{config_name}='
            # 匹配模式2: # CONFIG_XXX is not set
            pattern_unset = f'^# {config_name} is not set'
            
            if re.match(pattern_set, line_stripped) or re.match(pattern_unset, line_stripped):
                matched = True
                
                # 判断是否需要修改
                need_change = False
                if value is None or value == False or value == 'n':
                    # 目标是禁用：# CONFIG_XXX is not set
                    if not re.match(pattern_unset, line_stripped):
                        need_change = True
                        new_line = f'# {config_name} is not set'
                elif value == True or value == 'y':
                    # 目标是启用：CONFIG_XXX=y
                    if not re.match(f'^{config_name}=y$', line_stripped):
                        need_change = True
                        new_line = f'{config_name}=y'
                else:
                    # 目标是设置具体值：CONFIG_XXX=value
                    if not re.match(f'^{config_name}={re.escape(str(value))}$', line_stripped):
                        need_change = True
                        new_line = f'{config_name}={value}'
                
                if need_change:
                    # 注释掉旧行（添加标记以便识别）
                    if line_stripped.startswith('# [MODIFIED]'):
                        # 已经被标记过，保持不动
                        modified_lines.append(line_stripped)
                    elif line_stripped.startswith('#'):
                        # 原本是注释，添加 MODIFIED 标记
                        modified_lines.append(f'# [MODIFIED]{line_stripped[1:]}')
                    else:
                        # 原本是激活的配置，注释掉
                        modified_lines.append(f'# [MODIFIED] {line_stripped}')
                    # 添加新行
                    modified_lines.append(new_line)
                    modified = True
                    print(f"  [Kconfig] Modified: {line_stripped.strip()} -> {new_line}")
                else:
                    # 配置已经是期望的值，无需修改
                    modified_lines.append(line_stripped)
                break
        
        if not matched:
            # 当前行不匹配任何要修改的配置，保持原样
            modified_lines.append(line_stripped)
    
    if modified:
        with open(config_file, 'w') as f:
            f.write('\n'.join(modified_lines) + '\n')
        print(f"  [Kconfig] File updated: {config_file}")
        return True
    else:
        print(f"  [Kconfig] No changes needed for {config_file}")
        return False

def patch_target(target):
    target["ws63-liteos-xts"]["hso_enable_bt"] = False
    target["ws63-liteos-xts"]["hso_enable"] = False
    target["ws63-liteos-xts"]["ram_component_set"] = [
        'i2c', 'uart', 'hso_log', 
        '-:security_unified',  # Removed: security driver not needed
        'dfx_set', 'sio_v151', 'i2s'
    ]
    target["ws63-liteos-xts"]['rom_component']= [
        '-:bt_host_rom',      # Delete: bluetooth ROM component
    ]
    target["ws63-liteos-xts"]['skip_rom_patch'] = True  # Skip creating GENERAT_ROM_PATCH target

    target["ws63-liteos-xts"]["defines"] = [
        "USE_CMSIS_OS",
        "USE_LITEOS",
        "_ALL_SOURCE",
        "__LITEOS__",
        "-:CHIP_WS63=1",
        "LIBUTIL_COMPAT",
        "CONFIG_SUPPORT_NEW_DIAG",
        "NO_TCXO_SUPPORT",
        "UNSUPPORT_OTHER_MEM",
        "SW_UART_DEBUG",
        "SW_UART_CHIP_DEFINE",
        "AT_COMMAND",
        "DUMP_MEM_SUPPORT",
        "BUILD_APPLICATION_STANDARD",
        'CMD_ENABLE',
        "CONFIG_IPERF_SUPPORT",
        "CONFIG_SENDTEST_SUPPORT",
        "CONFIG_CACHE_MISS_SUPPORT",
        "_PRE_WLAN_FEATURE_BTCOEX",
        "_PRE_LWIP_ZERO_COPY",
        #"WIFI_TCM_OPTIMIZE",
        #"LWIP_TCM_OPTIMIZE",
        "CHECKSUM_CHECK_TCP=0",
        "CHECKSUM_CHECK_UDP=0",
        "MBEDTLS_AES_ROM_TABLES",
        "UPDATE_WIFI_STATIC_LIB",
        "UPDATE_BTC_STATIC_LIB",
        "MBEDTLS_HARDEN_OPEN",
        'CONFIG_UART_SUPPORT_LPM',
        "_PRE_RADAR_CCA_SW_OPT",
        "BUILD_WS63_MINIMAL",
        "AT_STACK_SIZE=0x700"
    ]
 
    target["ws63-liteos-xts"]["ram_component"] = [
        'ws63_liteos_app',
        'ws63_liteos_xts_lds',
        'liteos_port',
        'irmalloc',
        'non_os',
        '-:rtc',
        'arch_port',
        'board_config', # 被 pinctrl 组件依赖
        'lpm', # 被 pwm 组件依赖
        'chip_ws63', 'pmp_cfg_ws63',
        'reboot', 'hal_reboot', 'reboot_port', 'cpu_utils', 'hal_cpu_core',
        #'testsuite',
        'gpio','hal_gpio_v150','gpio_port',
        "dfx_port_ws63", 
        # "algorithm", 
        "cmn_header", 
        # "lwip", 
        # "lwip_tcm",
        #"wifi_service", 
        #"wpa_supplicant",
        "at", 
        # "wifi_driver_hmac", "wifi_driver_dmac", "wifi_driver_tcm", "wifi_at", "wifi_csa", "wifi_alg_txbf", "wifi_alg_temp_protect", "wifi_tx_amsdu", "wifi_sdp",
        # "wifi_auto_adjust_freq", "wifi_promisc", "wifi_alg_anti_interference", "wifi_alg_edca_opt", "wifi_alg_cca_opt", "wifi_sr",
        # "wifi_frag", "wifi_mbo", "wifi_bsrp_nfrp", "wifi_slp", "wifi_radar_sensor", "wifi_apf", "wifi_repeater", "wifi_csi", "wifi_wapi", "wifi_wps", "wifi_psd", "wifi_blacklist", "wifi_m2u",
        # "wifi_latency", "wifi_roam", "wifi_uapsd_sta", "wifi_ant_sel", "wifi_twt", "wifi_11k", "wifi_11v", "wifi_11r",  'wifi_btcoex', "wifi_uapsd_ap", "wifi_dnb",
        'liteos_208_5_0', 
        'rtc_unified', 'hal_rtc_unified', 'rtc_unified_port',
        # 'nv', 'nv_ws63', 'nv_zdiag_ws63', 
        #'plt_at',
        #'update_common', 'update_common_ws63', 'update_ab_ws63', 'factory_ws63', 'update_storage', 'update_storage_ws63',
        # 'pm_port_ws63',
        # 'gmssl_hmac_sm3',

        'samples',

        # 'mips',
        # 'hal_mips',
        'soc_port',
        # 'radar_sensing',
        # 'radar_at',
        'printf_adapt',
        "mbedtls",  # Required for mbedtls_cipher_adapt_register_func

        'xo_trim_port',
        'product'
    ]

    ram_component_sdk_bt_base = [
        'bt_host',
        'bt_at',
        'bg_common',
        'bth_gle',
        'bth_sdk',
        'bts_header',
        "bgtp",
        'bt_app',
    ]


    ram_component_ohos_base = [
        'ohos_adapt',
        "init_utils" ,       
        "begetutil", 
        "bootstrap", 
        'little_fs', 
        'littlefs_adapt_ws63',
        "hal_sysparam"  , 
        "param_client_lite",
        "udidcomm",
        "init_log",       
        "hal_file_static" ,
        "hal_sys_param",  
        "inithook",

        "hilog_static",     
        "hievent_lite_static" , 
        "hilog_lite_static" , 
        "hiview_lite_static",
        
        "samgr_source",
        "samgr_adapter",
        "samgr",
        "broadcast",

        "native_file",           
        "cjson_static",  
    ]
    ram_component_ohos_xts = [
        "module_ActsDfxFuncTest",
        "module_ActsHieventLiteTest",
        "module_ActsBootstrapTest",
        "module_ActsParameterTest",
        "module_ActsSamgrTest", 
        "hctest", 
    ]

    # ========== 根据环境变量动态调整配置 ==========
    BUILD_MODE = os.environ.get('BUILD_MODE', 'normal')
    BUILD_XTS = os.environ.get('BUILD_XTS', False)
    print("\n" + "="*60)
    print("WS63 Dynamic Configuration Applied:")
    print(f"  Build Mode:        {BUILD_MODE}")
    print(f"  Build XTS:         {BUILD_XTS}")

    target["ws63-liteos-xts"]["ram_component"] += ram_component_sdk_bt_base if 0 else []
    target["ws63-liteos-xts"]["ram_component"] += ram_component_ohos_base if 1 else []
    target["ws63-liteos-xts"]["ram_component"] += ram_component_ohos_xts if BUILD_XTS else []

    # 根据环境变量修改 Kconfig 配置
    kconfig_xts = os.path.join(os.path.dirname(__file__), 'menuconfig', 'acore', 'ws63_liteos_xts.config')
    kconfig_changes = {}
    kconfig_changes['CONFIG_MIDDLEWARE_SUPPORT_FTM'] = 'n'
    kconfig_changes['CONFIG_MIDDLEWARE_SUPPORT_NV'] = 'n'
    kconfig_changes['CONFIG_MIDDLEWARE_NV_SUPPORT_HSO_ACCESS'] = 'n'
    kconfig_changes['CONFIG_PARTITION_FEATURE_SUPPORT'] = 'n'
    kconfig_changes['CONFIG_MIDDLEWARE_SUPPORT_UPG'] = 'n'
    kconfig_changes['CONFIG_MIDDLEWARE_SUPPORT_UPG_AB'] = 'n'
    kconfig_changes['CONFIG_AT_SUPPORT_PLT'] = 'n'

    print("\nApplying Kconfig modifications:")
    modify_kconfig_file(kconfig_xts, kconfig_changes)

    kconfig_liteos_xts = os.path.join(os.path.dirname(__file__), '../../../../kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/tools/build/config/ws63_xts.config')
    kconfig_changes = {
        "LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE": "1024"
    }    
    modify_kconfig_file(kconfig_liteos_xts, kconfig_changes)
