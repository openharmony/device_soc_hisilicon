/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART01_H_


#line 1 "mipi_rx_hal.c"
/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "ot_osal.h"
#include "type.h"
#include "ot_mipi_rx.h"
#include "mipi_rx_hal.h"
#include "mipi_rx_reg.h"

/* Constants for reported magic-number values. */
#define MIPI_RX_HAL_SHIFT_2                    2
#define MIPI_RX_HAL_SHIFT_3                    3
#define MIPI_RX_HAL_SHIFT_4                    4
#define MIPI_RX_HAL_SHIFT_5                    5
#define MIPI_RX_HAL_SHIFT_8                    8
#define MIPI_RX_HAL_SYNC_CODE_INDEX_2          2
#define MIPI_RX_HAL_SYNC_CODE_INDEX_3          3
#define MIPI_RX_HAL_VALUE_2                    2
#define MIPI_RX_HAL_VALUE_3                    3
#define MIPI_RX_HAL_VALUE_4                    4
#define MIPI_RX_HAL_VALUE_5                    5
#define MIPI_RX_HAL_VALUE_6                    6
#define MIPI_RX_HAL_VALUE_7                    7

__attribute__((unused)) static volatile unsigned long g_mipi_rx_core_reset_addr;

typedef struct {
    unsigned int phy_rg_ext_en;
    unsigned int phy_rg_ext2_en;
    unsigned int phy_rg_int_en;
    unsigned int phy_rg_drveclk2_enz;
    unsigned int phy_rg_drveclk_enz;
} phy_mode_link_t;

typedef enum {
    MIPI_ESC_D0 = 0x1 << 0,
    MIPI_ESC_D1 = 0x1 << 1,
    MIPI_ESC_D2 = 0x1 << 2,
    MIPI_ESC_D3 = 0x1 << 3,
    MIPI_ESC_CLK = 0x1 << 4,
    MIPI_ESC_CLK2 = 0x1 << 5,

    MIPI_TIMEOUT_D0 = 0x1 << 8,
    MIPI_TIMEOUT_D1 = 0x1 << 9,
    MIPI_TIMEOUT_D2 = 0x1 << 10,
    MIPI_TIMEOUT_D3 = 0x1 << 11,
    MIPI_TIMEOUT_CLK = 0x1 << 12,
    MIPI_TIMEOUT_CLK2 = 0x1 << 13,
} phy_err_int_state;

typedef enum {
    MIPI_VC0_MISMATCH = 0x1 << 20, /* VC0,frame's start and frame's end do not match */
    MIPI_VC1_MISMATCH = 0x1 << 21, /* VC1,frame's start and frame's end do not match */
    MIPI_VC2_MISMATCH = 0x1 << 22, /* VC2,frame's start and frame's end do not match */
    MIPI_VC3_MISMATCH = 0x1 << 23, /* VC3,frame's start and frame's end do not match */

    MIPI_VC0_FRAME_NUM_ERR = 0x1 << 16, /* VC0'S frame order error */
    MIPI_VC1_FRAME_NUM_ERR = 0x1 << 17, /* VC1'S frame order error */
    MIPI_VC2_FRAME_NUM_ERR = 0x1 << 18, /* VC2'S frame order error */
    MIPI_VC3_FRAME_NUM_ERR = 0x1 << 19, /* VC3'S frame order error */

    MIPI_VC0_ECC_ERR = 0x1 << 4, /* VC0'S header has errors,and ECC error correction is ok */
    MIPI_VC1_ECC_ERR = 0x1 << 5,
    MIPI_VC2_ECC_ERR = 0x1 << 6,
    MIPI_VC3_ECC_ERR = 0x1 << 7,
    MIPI_MULT_ECC_ERR = 0x1 << 8, /* Header has two error at least ,and ECC error correction is invalid */

    MIPI_VC0_CRC_ERR = 0x1 << 0, /* in the last frame, VC0'S data has a CRC ERROR at least */
    MIPI_VC1_CRC_ERR = 0x1 << 1, /* in the last frame, VC1'S data has a CRC ERROR at least */
    MIPI_VC2_CRC_ERR = 0x1 << 2, /* in the last frame, VC2'S data has a CRC ERROR at least */
    MIPI_VC3_CRC_ERR = 0x1 << 3, /* in the last frame, VC3'S data has a CRC ERROR at least */
} mipi_csi_int_state;

typedef enum {
    CMD_FIFO_WRITE_ERR = 0x1 << 0, /* MIPI_CTRL write command FIFO error */
    DATA_FIFO_WRITE_ERR = 0x1 << 1,
    CMD_FIFO_READ_ERR = 0x1 << 16,
    DATA_FIFO_READ_ERR = 0x1 << 17,
} mipi_ctrl_int_state;

typedef enum {
    LANE0_SYNC_ERR = 0x1 << 0,
    LANE1_SYNC_ERR = 0x1 << 1,
    LANE2_SYNC_ERR = 0x1 << 2,
    LANE3_SYNC_ERR = 0x1 << 3,
    LANE4_SYNC_ERR = 0x1 << 4,
    LANE5_SYNC_ERR = 0x1 << 5,
    LANE6_SYNC_ERR = 0x1 << 6,
    LANE7_SYNC_ERR = 0x1 << 7,

    LINK0_WRITE_ERR = 0x1 << 16,
    LINK1_WRITE_ERR = 0x1 << 17,
    LINK2_WRITE_ERR = 0x1 << 18,
    LINK0_READ_ERR = 0x1 << 20,
    LINK1_READ_ERR = 0x1 << 21,
    LINK2_READ_ERR = 0x1 << 22,
    LVDS_STAT_ERR = 0x1 << 24,
    LVDS_POP_ERR = 0x1 << 25,
    CMD_WR_ERR = 0x1 << 26,
    CMD_RD_ERR = 0x1 << 27,
    LVDS_VSYNC = 0x1 << 28,
} lvds_int_state;

typedef enum {
    ALIGN_FIFO_FULL_ERR = 0x1 << 0,
    ALIGN_LANE0_ERR = 0x1 << 1,
    ALIGN_LANE1_ERR = 0x1 << 2,
    ALIGN_LANE2_ERR = 0x1 << 3,
    ALIGN_LANE3_ERR = 0x1 << 4,
    ALIGN_LANE4_ERR = 0x1 << 5,
    ALIGN_LANE5_ERR = 0x1 << 6,
    ALIGN_LANE6_ERR = 0x1 << 7,
    ALIGN_LANE7_ERR = 0x1 << 8,
} align_int_state;

/* macro definition */
#define MIPI_RX_REGS_ADDR 0x173C0000
#define MIPI_RX_REGS_SIZE 0x10000

#ifdef OT_FPGA
#define MIPI_RX_CRG_ADDR 0x173F1008
#else
#define MIPI_RX_CRG_ADDR 0x11018540
#endif

#define sns_crg_addr(sns_source)     (0x11018440 + (sns_source) * 0x20)
#define mipi_rx_pixel_crg_addr(deno) (0x11018560 + (deno) * 0x20)

#define SNS_RST_CRG_ADDR     0x173F3000
#define MIPI_RX_RST_CRG_ADDR 0x173F1004

#define MIPI_RX_WORK_MODE_ADDR 0x17920000

#define MIPI_RX_IRQ 160
#define MIPI_CIL_FREQ 150 /* MIPI CIL FREQ 150MHZ */

static unsigned int g_reg_map_flag = 0;

#define is_valid_id(id) ((id) != -1)
#define is_data_type_yuv(input_data_type) (((input_data_type) >= DATA_TYPE_YUV420_8BIT_NORMAL) && \
                                           ((input_data_type) <= DATA_TYPE_YUV422_PACKED))

#define SKEW_LINK       0x0
#define MIPI_DESKEW_CAL 0xffff000f
#define MIPI_FSMO_VALUE 0x000d1d0c

/* global variables definition */
mipi_rx_regs_type_t *g_mipi_rx_regs_va = NULL;

unsigned int g_mipi_rx_irq_num = MIPI_RX_IRQ;

static const phy_mode_link_t g_phy_mode[][MIPI_RX_MAX_PHY_NUM] = {
#ifndef OT_FPGA
    {{ 1, 0, 0, 1, 0 }, { 1, 0, 0, 1, 1 }},
#else
    {{ 0, 0, 1, 0, 1 }, { 0, 0, 0, 1, 1 }},
#endif
    {{ 0, 1, 0, 0, 1 }, { 0, 1, 0, 0, 1 }},
    {{ 0, 1, 0, 0, 1 }, { 0, 0, 1, 1, 1 }},
    {{ 0, 0, 1, 1, 1 }, { 0, 0, 1, 1, 1 }},
};

static const unsigned int g_hs_mode[] = {0x0, 0x7, 0xA, 0xB};

static phy_err_int_cnt_t g_phy_err_int_cnt[MIPI_RX_MAX_PHY_NUM];
static mipi_err_int_cnt_t g_mipi_err_int_cnt[MIPI_RX_MAX_DEV_NUM];
static lvds_err_int_cnt_t g_lvds_err_int_cnt[MIPI_RX_MAX_DEV_NUM];
static align_err_int_cnt_t g_align_err_int_cnt[MIPI_RX_MAX_DEV_NUM];

/* function definition */
void mipi_rx_drv_set_irq_num(unsigned int irq_num)
{
    g_mipi_rx_irq_num = irq_num;
}

void mipi_rx_drv_set_regs(mipi_rx_regs_type_t *regs)
{
    g_mipi_rx_regs_va = regs;
}

phy_err_int_cnt_t *mipi_rx_drv_get_phy_err_int_cnt(unsigned int phy_id)
{
    return &g_phy_err_int_cnt[phy_id];
}

mipi_err_int_cnt_t *mipi_rx_drv_get_mipi_err_int_cnt(combo_dev_t devno)
{
    return &g_mipi_err_int_cnt[devno];
}

lvds_err_int_cnt_t *mipi_rx_drv_get_lvds_err_int_cnt(combo_dev_t devno)
{
    return &g_lvds_err_int_cnt[devno];
}

align_err_int_cnt_t *mipi_rx_drv_get_align_err_int_cnt(combo_dev_t devno)
{
    return &g_align_err_int_cnt[devno];
}

static void set_bit(unsigned long value, unsigned long offset,
                    unsigned long addr)
{
    unsigned long t;
    unsigned long mask;

    mask = 1 << offset;
    t = osal_readl((const volatile void *)addr);
    t &= ~mask;
    t |= (value << offset) & mask;
    osal_writel(t, (volatile void *)addr);
}

static void write_reg32(unsigned long addr,
                        unsigned int value,
                        unsigned int mask)
{
    unsigned int t;

    t = osal_readl((void *)addr);
    t &= ~mask;
    t |= value & mask;
    osal_writel(t, (void *)addr);
}

static mipi_rx_phy_cfg_t *get_mipi_rx_phy_regs(int phy_id)
{
    return &g_mipi_rx_regs_va->mipi_rx_phy_cfg[phy_id];
}

static mipi_rx_sys_regs_t *get_mipi_rx_sys_regs(void)
{
    return &g_mipi_rx_regs_va->mipi_rx_sys_regs;
}

static mipi_ctrl_regs_t *get_mipi_ctrl_regs(combo_dev_t devno)
{
    return &g_mipi_rx_regs_va->mipi_rx_ctrl_regs[devno].mipi_ctrl_regs;
}

static lvds_ctrl_regs_t *get_lvds_ctrl_regs(combo_dev_t devno)
{
    return &g_mipi_rx_regs_va->mipi_rx_ctrl_regs[devno].lvds_ctrl_regs;
}

static global_ctrl_regs_t *get_global_ctrl_regs(combo_dev_t devno)
{
    return &g_mipi_rx_regs_va->mipi_rx_ctrl_regs[devno].global_ctrl_regs;
}

#ifndef OT_FPGA
static void mipi_rx_set_cil_int_mask(int phy_id, unsigned int mask)
{
    u_mipi_int_msk mipi_int_msk;
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg = NULL;
    volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    mipi_int_msk.u32 = mipi_rx_sys_regs->mipi_int_msk.u32;

    if (phy_id == 0) {
        mipi_int_msk.bits.int_phycil0_mask = 0x0;
    } else if (phy_id == 1) {
        mipi_int_msk.bits.int_phycil1_mask = 0x0;
    } else if (phy_id == 2) { /* 2 : phy2 */
        mipi_int_msk.bits.int_phycil2_mask = 0x0;
    }
    mipi_rx_sys_regs->mipi_int_msk.u32 = mipi_int_msk.u32;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->mipi_cil_int_msk_link.u32 = mask;
}

static void mipi_rx_set_phy_skew_link(int phy_id, unsigned int value)
{
    volatile u_phy_skew_link phy_skew_link;
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_skew_link.u32 = value;
    mipi_rx_phy_cfg->phy_skew_link.u32 = phy_skew_link.u32;
}

static void mipi_rx_set_phy_deskew_cal_link(int phy_id, unsigned int value)
{
    volatile u_phy_deskew_cal_link phy_deskew_cal_link;
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_deskew_cal_link.u32 = value;
    mipi_rx_phy_cfg->phy_deskew_cal_link.u32 = phy_deskew_cal_link.u32;
}

static void mipi_rx_set_phy_fsmo_link(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->cil_fsm0_link.u32 = value;
}

#endif

static void mipi_rx_set_phy_rg_ext_en(int phy_id, int enable)
{
    volatile u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;
    phy_mode_link.bits.phy_rg_ext_en = enable;
    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

static void mipi_rx_set_phy_rg_ext2_en(int phy_id, int enable)
{
    volatile u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;
    phy_mode_link.bits.phy_rg_ext2_en = enable;
    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

static void mipi_rx_set_phy_rg_int_en(int phy_id, int enable)
{
    volatile u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;
    phy_mode_link.bits.phy_rg_int_en = enable;
    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

static void mipi_rx_set_phy_rg_drveclk2_enz(int phy_id, int enable)
{
    volatile u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;
    phy_mode_link.bits.phy_rg_drveclk2_enz = enable;
    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

static void mipi_rx_set_phy_rg_drveclk_enz(int phy_id, int enable)
{
    volatile u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;
    phy_mode_link.bits.phy_rg_drveclk_enz = enable;
    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

void mipi_rx_drv_set_work_mode(combo_dev_t devno, input_mode_t input_mode)
{
    unsigned long mipi_rx_work_mode_addr;

    mipi_rx_work_mode_addr = (unsigned long)osal_ioremap(MIPI_RX_WORK_MODE_ADDR, (unsigned long)0x4); // len is 0x4
    if (mipi_rx_work_mode_addr == NULL) {
        ot_err("mipi_rx work mode reg ioremap failed!\n");
        return;
    }

    if (input_mode == INPUT_MODE_MIPI) {
        write_reg32(mipi_rx_work_mode_addr, 0x0 << (MIPI_RX_HAL_SHIFT_2 * devno), 0x1 << (MIPI_RX_HAL_SHIFT_2 * devno));
    } else if ((input_mode == INPUT_MODE_SUBLVDS) ||
               (input_mode == INPUT_MODE_LVDS) ||
               (input_mode == INPUT_MODE_HISPI)) {
        write_reg32(mipi_rx_work_mode_addr, 0x1 << (MIPI_RX_HAL_SHIFT_2 * devno), 0x1 << (MIPI_RX_HAL_SHIFT_2 * devno));
    } else {
    }

    osal_iounmap((void *)mipi_rx_work_mode_addr, (unsigned long)0x4); // len is 0x4
}

void mipi_rx_drv_set_mipi_image_rect(combo_dev_t devno, const img_rect_t *p_img_rect)
{
    u_mipi_crop_start_chn0 crop_start_chn0;
    u_mipi_crop_start_chn1 crop_start_chn1;
    u_mipi_crop_start_chn2 crop_start_chn2;
    u_mipi_crop_start_chn3 crop_start_chn3;
    u_mipi_imgsize mipi_imgsize;

    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    crop_start_chn0.u32 = mipi_ctrl_regs->mipi_crop_start_chn0.u32;
    crop_start_chn1.u32 = mipi_ctrl_regs->mipi_crop_start_chn1.u32;
    crop_start_chn2.u32 = mipi_ctrl_regs->mipi_crop_start_chn2.u32;
    crop_start_chn3.u32 = mipi_ctrl_regs->mipi_crop_start_chn3.u32;
    mipi_imgsize.u32 = mipi_ctrl_regs->mipi_imgsize.u32;

    mipi_imgsize.bits.mipi_imgwidth = p_img_rect->width - 1;
    mipi_imgsize.bits.mipi_imgheight = p_img_rect->height - 1;

    crop_start_chn0.bits.mipi_start_x_chn0 = p_img_rect->x;
    crop_start_chn0.bits.mipi_start_y_chn0 = p_img_rect->y;

    crop_start_chn1.bits.mipi_start_x_chn1 = p_img_rect->x;
    crop_start_chn1.bits.mipi_start_y_chn1 = p_img_rect->y;

    crop_start_chn2.bits.mipi_start_x_chn2 = p_img_rect->x;
    crop_start_chn2.bits.mipi_start_y_chn2 = p_img_rect->y;

    crop_start_chn3.bits.mipi_start_x_chn3 = p_img_rect->x;
    crop_start_chn3.bits.mipi_start_y_chn3 = p_img_rect->y;

    mipi_ctrl_regs->mipi_crop_start_chn0.u32 = crop_start_chn0.u32;
    mipi_ctrl_regs->mipi_crop_start_chn1.u32 = crop_start_chn1.u32;
    mipi_ctrl_regs->mipi_crop_start_chn2.u32 = crop_start_chn2.u32;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART01_H_ */
