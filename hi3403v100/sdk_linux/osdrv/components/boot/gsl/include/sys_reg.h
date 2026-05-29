/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __SYS_REG_H__
#define __SYS_REG_H__

/* TEE ctrl register */
#define REG_TEECTRL_BASE            0x10115000                      /* TEECTRL: 0x00B6_0000 */

/* HCTRL */
#define REG_HCTRL_BASE              0x00B20000

/* Sys ctrl register */
#define SC_CTRL                 (REG_SYSCTRL_BASE)
#define SC_SYSRES               (REG_SYSCTRL_BASE + 0x40)
#define SC_WDG_RST_CTRL         (REG_SYSCTRL_BASE + 0x44)
#define SC_GEN0                 (REG_SYSCTRL_BASE + 0x1400)
#define SC_GEN1                 (REG_SYSCTRL_BASE + 0x1404)  /* check the bus is stable or not */
#define SC_GEN2                 (REG_SYSCTRL_BASE + 0x1408)  /* self boot type: 0 - no self_boot; 1 - uart; 2 - usb */
#define SC_GEN3                 (REG_SYSCTRL_BASE + 0x140c)
#define SC_GEN5                 (REG_SYSCTRL_BASE + 0x1414)
#define SC_GEN9                 (REG_SYSCTRL_BASE + 0x1424)
#define SC_GEN12                (REG_SYSCTRL_BASE + 0x1430)  /* self_boot flag for boot download */
#define SC_GEN14                (REG_SYSCTRL_BASE + 0x1438)
#define SC_GEN19                (REG_SYSCTRL_BASE + 0x144C)
#define SC_GEN21                (REG_SYSCTRL_BASE + 0x1454)  /* hpp rom version */
#define SC_GEN22                (REG_SYSCTRL_BASE + 0x1458)  /* date of building hpp rom image */
#define SC_GEN23                (REG_SYSCTRL_BASE + 0x145C)  /* time of building hpp rom image */
#define SC_GEN24                (REG_SYSCTRL_BASE + 0x1800)
#define SC_GEN25                (REG_SYSCTRL_BASE + 0x1804)
#define SC_GEN26                (REG_SYSCTRL_BASE + 0x1808)

#define SC_SYSID                (REG_SYSCTRL_BASE + 0x10)

#define REG_DDRCA_RANDOM0       (REG_SYSCTRL_BASE + 0x06e0)
#define REG_DDRCA_RANDOM1       (REG_SYSCTRL_BASE + 0x06e4)

/* PERI ctrl register */
#define PERI_START_MODE         (REG_PERI_BASE + 0x100)
#define PERI_SOC_FUSE_1         (REG_PERI_BASE + 0x144)            /* 0xA1_0144 */
#define PERI_SOC_FUSE_2         (REG_PERI_BASE + 0x148)            /* 0xA1_0148 */

/* TEE CTRL register */
#define ACPU_RAM_SEC_CTRL       (REG_TEECTRL_BASE + 0x00)
#define TEE_CRG_CTRL            (REG_TEECTRL_BASE + 0x304)

/* DBC register */
#define REG_UART_DBG_INFO       (REG_DBC_BASE + 0xC0)

/* SV100 CSR register */
#define REG_RND_POWER_CTRL      0x7f0
#define REG_POWER_CTRL          0x7f2
#define REG_RND_POWER_FREQ_CTRL 0x7f3
#define REG_IDBUS_TIMEOUT       0x7e9

#define DUMMY_FETCH_ENABLE      0x5
#define DUMMY_LOAD_ENABLE       0x5
#define FIXED_DIVREM_ENABLE     0x5
#define FIXED_BRANCH_ENABLE     0x5
#define IDBUS_TIMEROUT_CFG      0x40
#define RND_POWER_FREQ_VALUE    0x22222220

/* Standby control */
#define CFG_SC_STANDBY          0x66031013

/* Security control values */
#define SEC_KEY                 0x05EC
#define CLK_ALWAYSON_ENABLE     0xA
#define POLARITY_BUS_WR_EN      0xA
#define POLARITY_BUS_RD_EN      0xA

#define IFLUSH_CNT_PERIOD       0x8
#define IFLUSH_CNT_WIDTH        0x5
#define IFLUSH_EN               0xA

#define MAX_DSENSOR_NUM         0x18
#define MAX_SHIELD_NUM          0x2

#define MAX_PC_CHECKER_POINT    0x6
#define PC_CHECKER_SEED_NUM     0x5
#define PC_CHECKER_NUM_IN_ROM   0x2
#define PC_CHECKER_ENABLE       0x5

#define SCIPHER_DFA_EN          0x5

#define SECURE_CLOCK_EN         0xA

#define MCIPHER_ACCESS_UNIT     0x400

#define SCIPHER2DDR_WR_ENABLE   0xA
#define SCIPHER2DDR_WR_DISABLE  0x5
#define SCPU2SYS_DISABLE        0x5
#define SCIPHER2SYS_DISABLE     0x5
#define SCIPHER2DDR_DISABLE     0x5

#define UART_STATUS_OPEN        0x5
#define UART_STATUS_PASSWORD    0xA
#define UART_STATUS_CLOSE       0x0

/* PERI ctrl register definition */
typedef union {
    struct {
        uint32_t reserved_0              : 9; /* [8:0] */
        uint32_t boot_sel                : 3; /* [11:9] */
        uint32_t reserved_1              : 8; /* [19:12] */
        uint32_t romboot_sel             : 1; /* [20] */
        uint32_t reserved_2              : 1; /* [21] */
        uint32_t otp_clk_sel_in          : 1; /* [22] */
        uint32_t jtag_sel_in             : 1; /* [23] */
        uint32_t self_boot               : 1; /* [24] */
        uint32_t reserved_3              : 3; /* [27:25] */
        uint32_t pin_secure_boot_en      : 4; /* [31:28] */
    } bits;
    uint32_t u32;
} peri_start_mode;              /* 0x100 START_MODE */

/* TRNG register definition */
typedef union {
    struct {
        uint32_t reserved_0              : 8; /* [7:0] */
        uint32_t trng_fifo_data_cnt      : 8; /* [15:8] */
        uint32_t reserved_1              : 16; /* [31:16] */
    } bits;
    uint32_t u32;
} trng_data_status;             /* 0x208 */

/* IO pad configuration definition */
typedef union {
    struct {
        uint32_t pin_mux                 : 4; /* [3:0] */
        uint32_t pad_driver              : 5; /* [8:4] SR/DS3/DS2/DS1/DS0 */
        uint32_t reserved_0              : 3; /* [11:9] */
        uint32_t pull_up                 : 1; /* [12] */
        uint32_t pull_down               : 1; /* [13] */
        uint32_t reserved_1              : 18; /* [31:14] */
    } bits;
    uint32_t u32;
} pad_config;

/* HCTRL register definition */
typedef union {
    struct {
        uint32_t scpu_crg_random_disable : 4;  /* [3:0] */
        uint32_t reserved_0              : 28; /* [31:4] */
    } bits;
    uint32_t u32;
} sec_hpp_crg_ctrl1;             /* 0x80 */

typedef union {
    struct {
        uint32_t int_status_ds_dla       : 1; /* [0] */
        uint32_t reserved_0              : 7; /* [7:1] */
        uint32_t int_status_fsensor      : 1; /* [8] */
        uint32_t reserved_1              : 23; /* [31:9] */
    } bits;
    uint32_t u32;
} sec_hpp_interrupt_status;             /* 0x110 */

/* DBC register definition */
typedef union {
    struct {
        uint32_t uart0_dbgen             : 4; /* [3:0] */
        uint32_t uart2_dbgen             : 4; /* [7:4] */
        uint32_t uart3_dbgen             : 4; /* [11:8] */
        uint32_t uart4_dbgen             : 4; /* [15:12] */
        uint32_t uart5_dbgen             : 4; /* [19:16] */
        uint32_t reserved_1              : 12; /* [31:20] */
    } bits;
    uint32_t u32;
} uart_debug_info;             /* 0xc0 */

/* TEE register definition */
typedef union {
    struct {
        uint32_t pke_srst_req        : 1; /* 0 */
        uint32_t reserved_0        : 3; /* 3:1 */
        uint32_t pke_cken        : 1; /* [4] */
        uint32_t reserved_1        : 3; /* [7:5] */
        uint32_t spacc_srst_req        : 1; /* [8] */
        uint32_t reserved_2        : 3; /* [11:9] */
        uint32_t spacc_cken        : 1; /* [12] */
        uint32_t reserved_3        : 3; /* [15:13] */
        uint32_t spacc_reset_ok        : 1; /* [16] */
        uint32_t reserved_4        : 15; /* [31:17] */
    } bits;
    uint32_t u32;
} tee_crg_ctrl;              /* 0xc0 */

/* CSR register definition */
typedef union {
    struct {
        uint32_t reserved0               : 20; /* [19:0] */
        uint32_t dummy_fetch             : 4; /* [23:20] */
        uint32_t dummy_load              : 4; /* [27:24] */
        uint32_t reserved1               : 4; /* [31:28] */
    } bits;
    uint32_t u32;
} random_power_ctrl;              /* 0x7f0 */

typedef union {
    struct {
        uint32_t fixed_divrem            : 4; /* [3:0] */
        uint32_t fixed_branch            : 4; /* [7:4] */
        uint32_t reserved0               : 24; /* [31:8] */
    } bits;
    uint32_t u32;
} power_ctrl;                       /* 0x7f2 */

#endif /* __SYS_REG_H__ */

