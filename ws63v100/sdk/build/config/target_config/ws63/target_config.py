#!/usr/bin/env python3
# encoding=utf-8
# =========================================================================
# @brief    Target Definitions File
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
# =========================================================================
import os
import copy

codesize_flags = {
    'ccflags':[
        '--short-enums',
        "-madjust-regorder",
        "-madjust-const-cost",
        "-freorder-commu-args",
        "-fimm-compare-expand",
        "-frmv-str-zero",
        "-mfp-const-opt",
        "-frtl-sequence-abstract",
        "-frtl-hoist-sink",
        "-fsafe-alias-multipointer",
        "-finline-optimize-size",
        "-fmuliadd-expand",
        "-mlli-expand",
        "-Wa,-mcjal-expand",
        "-foptimize-reg-alloc",
        "-fsplit-multi-zero-assignments",
        "-floop-optimize-size",
        "-Wa,-mlli-relax",
        "-mpattern-abstract",
        "-foptimize-pro-and-epilogue",
    ],
    'linkflags':[
        "-Wl,--cjal-relax",
        "-Wl,--dslf",
    ],
    'rom_ccflags':[
        '--short-enums',
        "-madjust-regorder",
        "-madjust-const-cost",
        "-freorder-commu-args",
        "-fimm-compare-expand",
        "-frmv-str-zero",
        "-mfp-const-opt",
        "-frtl-sequence-abstract",
        "-frtl-hoist-sink",
        "-fsafe-alias-multipointer",
        "-finline-optimize-size",
        "-fmuliadd-expand",
        "-mlli-expand",
        "-Wa,-mcjal-expand",
        "-foptimize-reg-alloc",
        "-fsplit-multi-zero-assignments",
        "-floop-optimize-size",
        "-foptimize-pro-and-epilogue",
        "-fno-inline-functions-called-once",
        "-fno-inline-small-functions",
    ]
}

fp_flags = {
    '-:-mabi=ilp32', '-:-march=rv32imc', '-mabi=ilp32f', '-march=rv32imfc'
}

target_template = {
    'target_ws63_app_rom_template': {
        'chip': 'ws63',
        'core': 'acore',
        'arch': 'riscv31',
        'tool_chain': 'riscv32_musl_100_fp',
        'board': 'evb',
        'build_type': 'COMPILE',
        'CONFIG_TIMER_USING_V150': 'y',
        'std_libs': ['m', 'c', 'gcc'],
        'defines': ["SDK_VERSION=\"1.10.101\"", '-:CHIP_WS63=1', 'LIBCPU_UTILS', 'LIBLIB_UTILS', 'LIBPANIC', 'LIBAPP_VERSION',
            'VERSION_STANDARD', 'LIBBUILD_VERSION',
            "CONFIG_TIMER_MAX_NUM=3", "CONFIG_TIMER_MAX_TIMERS_NUM_0=0", "CONFIG_TIMER_MAX_TIMERS_NUM_1=6", "CONFIG_TIMER_MAX_TIMERS_NUM_2=4",
            "CONFIG_TIMER_CLOCK_VALUE=24000000", "CONFIG_TIMER_0_WIDTH_64=0", "CONFIG_ADC_SUPPORT_AUTO_SCAN", "CONFIG_TCXO_WITH_TWO_DATA_REGS",
            "CONFIG_SFC_SUPPORT_DMA", "CONFIG_SFC_ALREADY_INIT", "USE_WS63_ROM_REPO", "CONFIG_DMA_UART_SUPPORT_V151",
            "EFUSE_BIT_OPERATION", "USE_ROM_SFC", 'CONFIG_UART_SUPPORT_TX_INT',
        ],
        'defines_set': ['libsec_defines', 'chip_defines', 'version_defines'],

        'ram_component': [
            'osal', 'osal_adapt', 'common_headers', 'ws63_mem_config',
            'driver_header', 'security_unified_header',
            'efuse', 'hal_efuse_v151', 'efuse_port',
            'tsensor', 'hal_tsensor', 'tsensor_port',
            'pwm', 'hal_pwm', 'pwm_port', 'app_init',
            'adc', 'hal_adc', 'adc_port',
            'dma', 'hal_dma_v151', 'dma_port',
            'spi', 'hal_spi', 'spi_porting',
            'sfc_port_ws63', 'sfc_flash_config_ws63',
            'pmp', 'hal_pmp', 'pmp_port',
            'dyn_mem',
            'plat_patch', 'wifi_rom_data', 'bgtp_rom_data',
            'version_port', 'mac_addr_ws63',
            'partition', 'partition_ws63',
        ],
        'ram_component_set': [
            'i2c', 'uart', 'hso_log', 'security_unified', 'dfx_set', 'sio_v151', 'i2s'
        ],
        'rom_component': [
            'wifi_driver_rom', "wifi_alg_radar_sensor_rom",
            'sfc_rom', 'hal_sfc_rom',
            'watchdog_rom', 'hal_watchdog_rom', 'watchdog_port_rom',
            'pinctrl_rom', 'hal_pinctrl_rom', 'pinctrl_port_ws63_rom',
            'systick_rom', 'systick_port_rom',
            'drv_timer_rom', 'hal_timer_rom', 'timer_port_rom',
            'tcxo_rom', 'hal_tcxo_rom', 'tcxo_port_rom',
            'libboundscheck_rom', 'version_rom',
            'bgtp_rom', 'bt_host_rom', 'bg_common_rom',
        ],
        'bin_name': 'application',
        'ccflags': [
            *fp_flags,
            *codesize_flags['ccflags'],
        ],
        'linkflags': codesize_flags['linkflags'],
        'rom_ccflags': [
            *fp_flags,
            *codesize_flags['rom_ccflags'],
            "-DCONFIG_ROM_COMPILE",
            "-DINLINE_TO_FORCEINLINE",
            "-:-Wno-error=main",
            "-:-Wno-type-limits",
            "-:-Wno-implicit-fallthrough",
            "-Wfloat-equal",
            '-UFPGA_DEBUG', '-UPRE_FPGA', '-UBOARD_FPGA', '-UHI_BOARD_FPGA', '-UBOARD_ASIC'
        ],
        'arch': 'riscv31',
        'build_rom_callback': True,
        'rom_ram_check': True,
        'rom_ram_compare': True,
        'patch': True,
        'fixed_rom': True,
        'fixed_rom_path': '<root>/interim_binary/ws63/bin/rom_bin/ws63-liteos_rom.bin',
        'rom_sym_path': '<root>/drivers/chips/ws63/rom_config/acore/acore.sym',
        "rom_in_one": True,
        'pke_rom_bin': True,
        "fixed_pke": True,
        "fixed_pke_path": '<root>/interim_binary/ws63/bin/rom_bin/pke_rom.bin',
        'packet': True,
    },
    'target_ws63_boot_template': {
        'chip': 'ws63',
        'core': 'acore',
        'board': 'evb',
        'tool_chain': 'riscv32_musl_100',
        'build_type': 'COMPILE',
        'os': 'non-os',
        'std_libs': ['-:m', '-:c', 'gcc'],
        'defines': ["SDK_VERSION=\"1.10.101\"", 'CHIP_WS63=1', "CONFIG_UART_FIFO_DEPTH=64", "OSAL_NO_PAL",
            "CONFIG_TIMER_MAX_NUM=3", "CONFIG_TIMER_MAX_TIMERS_NUM_0=0", "CONFIG_TIMER_MAX_TIMERS_NUM_1=6",
            "CONFIG_TIMER_MAX_TIMERS_NUM_2=4", "CONFIG_TCXO_WITH_TWO_DATA_REGS"],
        'defines_set': ['libsec_defines'],
        'ram_component': ['-:port_pinctrl', 'pinctrl_port_ws63', 'driver_header', 'security_unified_header'],
        'ram_component_set': [],
        'bin_name': 'boot',
        'ccflags': ['--short-enums',],
        'linkflags': [],
        'arch': 'riscv31',
        'copy_files_to_interim': True
    },
    'target_ws63_xts_rom_template': {
        'chip': 'ws63',
        'core': 'acore',
        'arch': 'riscv31',
        'tool_chain': 'riscv32_musl_100_fp',
        'board': 'evb',
        'build_type': 'COMPILE',
        'CONFIG_TIMER_USING_V150': 'y',
        'std_libs': ['m', 'c', 'gcc', 'atomic'],
        'defines': ["SDK_VERSION=\"1.10.101\"", '-:CHIP_WS63=1', 'LIBCPU_UTILS', 'LIBLIB_UTILS', 'LIBPANIC', 'LIBAPP_VERSION',
            'VERSION_STANDARD', 'LIBBUILD_VERSION',
            "CONFIG_TIMER_MAX_NUM=3", "CONFIG_TIMER_MAX_TIMERS_NUM_0=0", "CONFIG_TIMER_MAX_TIMERS_NUM_1=6", "CONFIG_TIMER_MAX_TIMERS_NUM_2=4",
            "CONFIG_TIMER_CLOCK_VALUE=24000000", "CONFIG_TIMER_0_WIDTH_64=0", "CONFIG_ADC_SUPPORT_AUTO_SCAN", "CONFIG_TCXO_WITH_TWO_DATA_REGS",
            "CONFIG_SFC_SUPPORT_DMA", "CONFIG_SFC_ALREADY_INIT", "USE_WS63_ROM_REPO", "CONFIG_DMA_UART_SUPPORT_V151",
            "EFUSE_BIT_OPERATION", "USE_ROM_SFC", 'CONFIG_UART_SUPPORT_TX_INT',
        ],
        'defines_set': ['libsec_defines', 'chip_defines', 'version_defines'],

        'ram_component': [
            'osal', 'osal_adapt', 'common_headers', 'ws63_mem_config',
            'driver_header', 'security_unified_header',
            'efuse', 'hal_efuse_v151', 'efuse_port',
            'tsensor', 'hal_tsensor', 'tsensor_port',
            'pwm', 'hal_pwm', 'pwm_port', 'app_init',
            'adc', 'hal_adc', 'adc_port',
            'dma', 'hal_dma_v151', 'dma_port',
            'spi', 'hal_spi', 'spi_porting',
            'sfc_port_ws63', 'sfc_flash_config_ws63',
            'pmp', 'hal_pmp', 'pmp_port',
            'dyn_mem', 'libboundscheck_sscanf_s',
            'plat_patch', 'wifi_rom_data', 'bgtp_rom_data',
            'version_port', 'mac_addr_ws63',
            'partition', 'partition_ws63',
        ],
        'ram_component_set': [
            'i2c', 'uart', 'hso_log', 'security_unified', 'dfx_set', 'sio_v151', 'i2s'
        ],
        'rom_component': [
            'wifi_driver_rom', "wifi_alg_radar_sensor_rom",
            'sfc_rom', 'hal_sfc_rom',
            'watchdog_rom', 'hal_watchdog_rom', 'watchdog_port_rom',
            'pinctrl_rom', 'hal_pinctrl_rom', 'pinctrl_port_ws63_rom',
            'systick_rom', 'systick_port_rom',
            'drv_timer_rom', 'hal_timer_rom', 'timer_port_rom',
            'tcxo_rom', 'hal_tcxo_rom', 'tcxo_port_rom',
            'libboundscheck_rom', 'version_rom',
            'bgtp_rom', 'bt_host_rom', 'bg_common_rom',
        ],
        'bin_name': 'application',
        'ccflags': [
            *fp_flags,
            *codesize_flags['ccflags'],
        ],
        'linkflags': codesize_flags['linkflags'],
        'rom_ccflags': [
            *fp_flags,
            *codesize_flags['rom_ccflags'],
            "-DCONFIG_ROM_COMPILE",
            "-DINLINE_TO_FORCEINLINE",
            "-:-Wno-error=main",
            "-:-Wno-type-limits",
            "-:-Wno-implicit-fallthrough",
            "-Wfloat-equal",
            '-UFPGA_DEBUG', '-UPRE_FPGA', '-UBOARD_FPGA', '-UHI_BOARD_FPGA', '-UBOARD_ASIC'
        ],
        'arch': 'riscv31',
        'build_rom_callback': True,
        'rom_ram_check': True,
        'rom_ram_compare': True,
        'patch': True,
        'fixed_rom': True,
        'fixed_rom_path': '<root>/interim_binary/ws63/bin/rom_bin/ws63-liteos_rom.bin',
        'rom_sym_path': '<root>/drivers/chips/ws63/rom_config/acore/acore.sym',
        "rom_in_one": True,
        'pke_rom_bin': True,
        "fixed_pke": True,
        "fixed_pke_path": '<root>/interim_binary/ws63/bin/rom_bin/pke_rom.bin',
        'packet': True,
    },
}
