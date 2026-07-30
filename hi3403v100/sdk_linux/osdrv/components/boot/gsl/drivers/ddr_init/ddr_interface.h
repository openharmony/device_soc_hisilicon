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

#ifndef DDR_INTERFACE_H
#define DDR_INTERFACE_H

#include <types.h>

#ifndef CONFIG_MINI_BOOT
extern char g_ddr_training_cmd_start[];  /* DDR training code start address */
extern char g_ddr_training_cmd_end[];    /* DDR training code end address */
#endif

#define reg_read(addr)                (*(volatile unsigned int *)((uintptr_t)(addr)))
#define reg_write(val, addr)          ((*(volatile unsigned int *)((uintptr_t)(addr))) = (val))

#define DDR_PHY_BYTE_MAX                  4
#define DDR_PHY_BIT_NUM                   8
/* support max bit 32 */
#define DDR_PHY_BIT_MAX                   (DDR_PHY_BYTE_MAX * DDR_PHY_BIT_NUM)

#define DDR_REG_NAME_MAX              32       /* register name */
#define DDR_CA_ADDR_MAX               10

#define DDR_SUPPORT_PHY_MAX           3        /* support max phy number */
#define DDR_SUPPORT_RANK_MAX          2        /* support max rank number */
#define DDR_SUPPORT_DMC_MAX           4        /* support max dmc number */
#define DDR_CK_RESULT_MAX             2        /* DCC CK result number */

/*
 * DDR training register number:
 * WDQS              4
 * WDQ Phase         4
 * WDQ BDL           8
 * WDM               4
 * Write DQ/DQS OE   4
 * RDQS              4
 * RDQ BDL           8
 * Gate              4
 * CS                1
 * CLK               1
 * Host Vref         4
 * DRAM Vref         4
 * CA Phase          1
 * CA BDL            5
 * -------------------
 *                  60
 */
#define DDR_TRAINING_REG_NUM          60
/* register max. */
#define DDR_TRAINING_REG_MAX        (DDR_TRAINING_REG_NUM * DDR_SUPPORT_PHY_MAX)

#define DDR_TRAINING_CMD_SW           (1 << 0)
#define DDR_TRAINING_CMD_HW           (1 << 1)
#define DDR_TRAINING_CMD_MPR          (1 << 2)
#define DDR_TRAINING_CMD_WL           (1 << 3)
#define DDR_TRAINING_CMD_GATE         (1 << 4)
#define DDR_TRAINING_CMD_DATAEYE      (1 << 5)
#define DDR_TRAINING_CMD_VREF         (1 << 6)
#define DDR_TRAINING_CMD_AC           (1 << 7)
#define DDR_TRAINING_CMD_LPCA         (1 << 8)
#define DDR_TRAINING_CMD_SW_NO_WL     (1 << 9)
#define DDR_TRAINING_CMD_CONSOLE      (1 << 10)
#define DDR_TRAINING_CMD_DCC          (1 << 11)
#define DDR_TRAINING_CMD_PCODE        (1 << 12)
#define DDR_TRAINING_CMD_DPMC         (1 << 13)

/*******log level ********************/
#define DDR_LOG_INFO_STR              "info"
#define DDR_LOG_DEBUG_STR             "debug"
#define DDR_LOG_WARNING_STR           "warning"
#define DDR_LOG_ERROR_STR             "error"
#define DDR_LOG_FATAL_STR             "fatal"

#define DDR_LOG_INFO                  (1 << 0)
#define DDR_LOG_DEBUG                 (1 << 1)
#define DDR_LOG_WARNING               (1 << 2)
#define DDR_LOG_ERROR                 (1 << 3)
#define DDR_LOG_FATAL                 (1 << 4)

#define DDR_TRAINING_BOOT_RESULT_ADDR (TEXT_BASE + 0x1000000)  /* boot + 16M */

#define DDR_TRAINING_VER "V2.2.0 20200826"
#define DDR_VERSION    0x220
struct training_data {
    unsigned int ddr_bit_result[DDR_PHY_BIT_MAX];
    unsigned int ddr_bit_best[DDR_PHY_BIT_MAX];
    unsigned int ddr_win_sum;
};

struct ddr_training_data_st {
    unsigned int base_phy;
    unsigned int byte_num;
    unsigned int rank_idx;
    struct training_data read;
    struct training_data write;
    unsigned int ca_addr[DDR_CA_ADDR_MAX];
};

struct rank_data_st {
    unsigned int item;
    struct ddr_training_data_st ddrtr_data;
};

struct phy_data_st {
    unsigned int rank_num;
    struct rank_data_st rank_st[DDR_SUPPORT_RANK_MAX];
};

struct ddr_training_result_st {
    unsigned int phy_num;
    struct phy_data_st phy_st[DDR_SUPPORT_PHY_MAX];
};

struct ddr_reg_val_st {
    unsigned int rank_index;
    unsigned int byte_index;
    unsigned int offset;
    unsigned int val;
    char name[DDR_REG_NAME_MAX];
};

struct ddr_cmd_st {
    unsigned int cmd;
    unsigned int level;
    unsigned int start;
    unsigned int length;
};

typedef struct ddr_training_result_st *(*ddr_cmd_entry_func)
    (const struct ddr_cmd_st *cmd_st);

/* DDR training interface before boot */
int ddr_pcode_training_if(void);
int ddr_sw_training_if(void);
int ddr_hw_training_if(void);
int ddr_training_console_if(void);
int ddr_hw_training_init(void);
void ddr_retrain_anti_aging_enable(void);
void ddr_dmc_auto_power_down_cfg(void);
void ddr_set_rdqbdl_def_val(void);

/* DDR training check interface when boot */
struct ddr_training_result_st *ddr_cmd_training_if(const struct ddr_cmd_st *cmd_st);
int check_ddr_training(void);

/* DDR training command interface after boot */
void ddr_reg_result_display(const struct ddr_training_result_st *ddrtr_result);
void ddr_cmd_result_display(const struct ddr_training_result_st *ddrtr_result, unsigned int cmd);
void *ddr_cmd_get_entry(void);
void ddr_cmd_prepare_copy(void);
void ddr_cmd_site_save(void);
void ddr_cmd_site_restore(void);
#endif /* DDR_INTERFACE_H */

