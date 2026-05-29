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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART03_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART03_H_

    0x4,
    0xb01000,
    0xb04411,
    0x4,

    /* ratio is less than 0.33,and not less than 0.25 */
    0xf01c00,
    0x703c13,
    0x1,
    0xf01c00,
    0x803c12,
    0x1,
    0xe01c00,
    0x803c12,
    0x2,
    0xe01c00,
    0x803c12,
    0x2,
    0xe01c00,
    0x803c12,
    0x2,
    0xe01800,
    0x904011,
    0x2,
    0xd01800,
    0x904012,
    0x2,
    0xd01800,
    0x904012,
    0x2,
    0xd01800,
    0x904011,
    0x3,
    0xd01400,
    0x904012,
    0x3,
    0xc01400,
    0xa04012,
    0x3,
    0xc01400,
    0xa04012,
    0x3,
    0xc01400,
    0xa04411,
    0x3,
    0xc01400,
    0xa04411,
    0x3,
    0xc01000,
    0xb04410,
    0x4,
    0xb01000,
    0xb04411,
    0x4,
    0xb01000,
    0xb04411,
    0x4,

    /* ratio is less than 0.25 */
    0xe02000,
    0x803812,
    0x2,
    0xe02000,
    0x803811,
    0x3,
    0xe02000,
    0x803811,
    0x3,
    0xe01c00,
    0x903c10,
    0x3,
    0xd01c00,
    0x903c11,
    0x3,
    0xd01c00,
    0x903c11,
    0x3,
    0xd01c00,
    0x903c11,
    0x3,
    0xd01c00,
    0x903c11,
    0x3,
    0xd01800,
    0xa03c10,
    0x4,
    0xd01800,
    0xa03c10,
    0x4,
    0xc01800,
    0xa03c11,
    0x4,
    0xc01800,
    0xa04010,
    0x4,
    0xc01800,
    0xa04010,
    0x4,
    0xc01400,
    0xb04010,
    0x4,
    0xc01400,
    0xb0400f,
    0x5,
    0xb01400,
    0xb04010,
    0x5,
    0xb01400,
    0xb04010,
    0x5,
};

#ifdef TDE_TIME_COUNT
tde_timeval g_time_start;
#endif

/* Head address of argument table used as config */
typedef struct {
    td_phys_addr_t hf_coef_addr;
    td_phys_addr_t vf_coef_addr;

#if (TDE_CAPABILITY & DEFLICKER)
    td_phys_addr_t deflicker_vf_coef_addr;
#endif
} tde_para_table;

typedef struct {
    td_u32 alpha_en;
    td_u32 luma_en;
    td_u32 chrome_en;
} tde_filtermode;

/* Base addr of register after mapping */
volatile td_u32 *g_base_vir_addr = TD_NULL;
static td_bool g_use_dtsi = TD_TRUE;

/* Pointer of TDE clock register after mapping */
static volatile td_u32 *g_tde_clock_vir = TD_NULL;
/* Head address of config argument table */
static tde_para_table g_para_table = { 0 };

/* Deflicker level, default is auto */
#if (TDE_CAPABILITY & DEFLICKER)
static drv_tde_deflicker_level g_deflicker_level = DRV_TDE_DEFLICKER_LEVEL_AUTO;
#endif

/* alpha threshold switch */
static td_bool g_alpha_threshold = TD_FALSE;

/* alpha threshold value */
static td_u8 g_alpha_threshold_value = 0xff;

static td_u32 g_rgb_truncation_mode = 1; /* 1 for rgb truncation mode */

static tde_alu_mode g_cbmctrl_alu_mode[TDE_BUTT + 1] = {
    TDE_SRC2_BYPASS, TDE_ALU_NONE, TDE_ALU_ROP, TDE_SRC2_BYPASS, TDE_ALU_MASK_ROP1, TDE_ALU_MASK_ROP2,
    TDE_ALU_MASK_BLEND, TDE_SRC2_BYPASS, TDE_SRC2_BYPASS, TDE_SRC2_BYPASS, TDE_SRC2_BYPASS, TDE_BUTT
};

static td_s32 tde_hal_init_para_table(td_void);
static td_void tde_hal_node_set_clutcolor_key_para(tde_hw_node *hw_node, const tde_color_key_cmd *color_key);
static td_void tde_hal_node_set_ycbcr_color_key_para(tde_hw_node *hw_node, const tde_color_key_cmd *color_key);
static td_void tde_hal_node_set_argbcolor_key_para(tde_hw_node *hw_node, const tde_color_key_cmd *color_key);
static td_u32 tde_hal_get_color_key_mode(const drv_tde_color_key_comp *color_key);
static td_u32 tde_hal_get_ycb_cr_key_mask(td_u8 cr, td_u8 cb, td_u8 cy, td_u8 alpha);
static td_u32 tde_hal_get_clut_key_mask(td_u8 clut, td_u8 alpha);
static td_u32 tde_hal_get_argb_key_mask(td_u8 blue, td_u8 green, td_u8 red, td_u8 alpha);
static td_s32 tde_hal_getbpp_by_fmt(tde_color_fmt fmt);
static td_u32 tde_hal_get_resize_para_htable(td_u32 step);
static td_u32 tde_hal_get_resize_para_vtable(td_u32 step);

static td_void tde_hal_init_queue(td_void);
td_void tde_hal_set_clock(td_bool enable);
static td_s32 tde_hal_getbpp_by_fmt1(tde_color_fmt fmt);
static td_s32 tde_hal_getbpp_by_fmt2(tde_color_fmt fmt);
static td_s32 tde_hal_getbpp_by_fmt4(tde_color_fmt fmt);
static td_s32 tde_hal_getbpp_by_fmt8(tde_color_fmt fmt);
static td_s32 tde_hal_getbpp_by_fmt16(tde_color_fmt fmt);
static td_s32 tde_hal_getbpp_by_fmt24(tde_color_fmt fmt);
static td_s32 tde_hal_getbpp_by_fmt32(tde_color_fmt fmt);

#define registder_flush() mb()

static td_bool tde_hal_is_operation_support(tde_alu_mode alu_mode, td_u32 capability, td_u32 oper_mode);
static td_u32 tde_hal_get_zme_in_fmt(tde_color_fmt in_drv_fmt);
static td_u32 tde_hal_get_hpzme_mode(td_u32 out_rect_width, td_u32 in_rect_width);
static td_void tde_hal_get_filter_mode(drv_tde_filter_mode filter_mode, tde_filtermode *flt_mode);
static td_s32 tde_hal_get_ver_scale_coeff(td_u32 out_rect_height, td_u32 in_rect_height, td_s32 *ver_scale_coeff);

static td_void tde_fill_data_by_fmt(tde_hw_node *hw_node, td_u32 data, tde_color_fmt fmt, td_u8 flag)
{
    td_u32 bpp;
    td_u32 i;
    td_u32 cell;
    bpp = (td_u32)tde_hal_getbpp_by_fmt(fmt);
    if (bpp != 0xffffffff) {
        cell = data & (0xffffffff >> (32 - bpp)); /* 32 bpp The not */
        for (i = 0; i < (32 / bpp); i++) { /* 32 bpp fmt */
            if (flag) {
                (hw_node->src2_fill.bits.src2_color_fill) |= (cell << (i * bpp));
            } else {
                (hw_node->src1_fill.bits.src1_color_fill) |= (cell << (i * bpp));
            }
        }
    } else {
        if (flag) {
            (hw_node->src2_fill.bits.src2_color_fill) = data;
        } else {
            (hw_node->src1_fill.bits.src1_color_fill) = data;
        }
    }
    return;
}

td_void tde_hal_set_base_vir_addr(td_u32 *temp_base_vir_addr)
{
    g_base_vir_addr = temp_base_vir_addr;
}

td_void tde_init_set_rgb_truncation_mode(td_u32 rgb_truncation_mode)
{
    g_rgb_truncation_mode = rgb_truncation_mode;
}

#ifdef TDE_COREDUMP_DEBUG
volatile td_u32 *tde_hal_get_base_vir_addr(td_void)
{
    return g_base_vir_addr;
}
#endif

/*
 * Function:      tde_hal_init
 * Description:   map the base address for tde
 * Input:         base_addr: the base address of tde
 * Return:        success/fail
 */
td_s32 tde_hal_init(td_u32 base_addr)
{
    /* init the pool memory of tde, CNcomment: Initialize the TDE memory pool */
    if (wmeminit() != TD_SUCCESS) {
        goto TDE_INIT_ERR;
    }
    /* config start address for the parameter, CNcomment: The first address configuration parameter table */
    if (tde_hal_init_para_table() != TD_SUCCESS) {
        wmemterm();
        goto TDE_INIT_ERR;
    }

    /* map address for the register, CNcomment: Register mapping */
    g_use_dtsi = TD_TRUE;
    if (g_base_vir_addr == TD_NULL) {
        g_base_vir_addr = (volatile td_u32 *)tde_reg_map(base_addr, TDE_REG_SIZE);
        g_use_dtsi = TD_FALSE;
    }

    if (g_base_vir_addr == TD_NULL) {
        wmemterm();
        goto TDE_INIT_ERR;
    }
    /* set limit of clock and div, CNcomment: Set the clock threshold, clock frequency division */
    g_tde_clock_vir = (volatile td_u32 *)tde_reg_map(TDE_REG_CLOCK, 4); /* 4 size */
    if (g_tde_clock_vir == TD_NULL) {
        wmemterm();
        tde_reg_unmap(g_base_vir_addr, TDE_REG_SIZE);
        goto TDE_INIT_ERR;
    }

    return TD_SUCCESS;

TDE_INIT_ERR:
    return TD_FAILURE;
}

td_void tde_hal_resume_init(td_void)
{
    tde_hal_set_clock(TD_TRUE);

    tde_hal_ctl_reset();

    tde_hal_init_queue();

    return;
}

td_s32 tde_hal_open(td_void)
{
    return TD_SUCCESS;
}

/*
 * Function:      tde_hal_release
 * Description:   release the address that had map
 * Return:        success/fail
 */
td_void tde_hal_release(td_void)
{
    td_void *buf = TD_NULL;
    if (g_para_table.hf_coef_addr != 0) {
        buf = (td_void *)wgetvrt(g_para_table.hf_coef_addr);
        if (buf != TD_NULL) {
            tde_free(buf);
            g_para_table.hf_coef_addr = 0;
        }
    }
    if (g_para_table.vf_coef_addr != 0) {
        buf = (td_void *)wgetvrt(g_para_table.vf_coef_addr);
        if (buf != TD_NULL) {
            tde_free(buf);
            g_para_table.vf_coef_addr = 0;
        }
    }

#if (TDE_CAPABILITY & DEFLICKER)
    if (g_para_table.deflicker_vf_coef_addr != 0) {
        buf = (td_void *)wgetvrt(g_para_table.deflicker_vf_coef_addr);
        if (buf != TD_NULL) {
            tde_free(buf);
            g_para_table.deflicker_vf_coef_addr = 0;
        }
    }
#endif

    /* unmap, CNcomment: Remove the mapping */
    if (g_tde_clock_vir != TD_NULL) {
        tde_reg_unmap(g_tde_clock_vir, 4); /* 4 size */
    }
    g_tde_clock_vir = TD_NULL;
    /* unmap the base address, CNcomment: Reflect the base address */
    if (g_use_dtsi == TD_FALSE) {
        tde_reg_unmap(g_base_vir_addr, TDE_REG_SIZE);
    }
    g_base_vir_addr = TD_NULL;

    /* free the pool of memory, CNcomment: TDE memory pool to initialize */
    wmemterm();

    return;
}

td_bool tde_hal_ctl_is_idle(td_void)
{
    if (g_base_vir_addr == TD_NULL) {
        tde_error("null pointer\n");
        return TD_FALSE;
    }
    return (td_bool)(!(tde_read_reg(g_base_vir_addr, TDE_STA) & 0x1));
}

/*
 * Function:      tde_hal_ctl_is_idle_safely
 * Description:   get the state of tde one more time ,make sure it's idle
 * Return:        True: Idle/False: Busy
 */
td_bool tde_hal_ctl_is_idle_safely(td_void)
{
    td_u32 i;

    /*
     * get the state of tde one more time ,make sure it's idle
     * CNcomment: Continuous read hardware status for many times, make sure complete TDE
     */
    for (i = 0; i < TDE_MAX_READ_STATUS_TIME; i++) {
        if (!tde_hal_ctl_is_idle()) {
            return TD_FALSE;
        }
    }
    return TD_TRUE;
}

td_u32 tde_hal_ctl_int_status(td_void)
{
    volatile td_u32 value;

    if (g_base_vir_addr == TD_NULL) {
        tde_error("null pointer\n");
        return 0;
    }

    value = tde_read_reg(g_base_vir_addr, TDE_INT);
    /* clear all status */
    tde_write_reg(g_base_vir_addr, TDE_INTCLR, value);
    return value;
}

td_void tde_hal_ctl_reset(td_void)
{
    if (g_base_vir_addr == TD_NULL) {
        tde_error("null pointer\n");
        return;
    }
    tde_write_reg(g_base_vir_addr, TDE_INTCLR, 0xf);
#ifdef CONFIG_TDE_MISCELLANEOUS_OUTSTANDING
    tde_write_reg(g_base_vir_addr, TDE_MISCELLANEOUS, 0x0100645a);
#else
    tde_write_reg(g_base_vir_addr, TDE_MISCELLANEOUS, 0x0100647f);
#endif
    return;
}

/*
 * Function:      tde_hal_set_clock
 * Description:   enable or disable the clock of TDE
 * Input:         td_bool bEnable:enable/disable
 */


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART03_H_ */
