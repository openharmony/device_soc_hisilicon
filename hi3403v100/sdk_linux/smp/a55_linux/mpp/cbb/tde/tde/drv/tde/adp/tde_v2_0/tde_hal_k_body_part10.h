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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART10_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART10_H_

    if (tde_hal_getbpp_by_fmt16(fmt) == 16) {        /* 16 fmt data */
        return 16;                                   /* 16 fmt data */
    } else if (tde_hal_getbpp_by_fmt24(fmt) == 24) { /* 24 fmt data */
        return 24;                                   /* 24 fmt data */
    } else if (tde_hal_getbpp_by_fmt32(fmt) == 32) { /* 32 fmt data */
        return 32;                                   /* 32 fmt data */
    } else if (tde_hal_getbpp_by_fmt1(fmt) == 1) {
        return 1;
    } else if (tde_hal_getbpp_by_fmt2(fmt) == 2) {   /* 2 fmt data */
        return 2;                                    /* 2 fmt data */
    } else if (tde_hal_getbpp_by_fmt4(fmt) == 4) {   /* 4 fmt data */
        return 4;                                    /* 4 fmt data */
    } else if (tde_hal_getbpp_by_fmt8(fmt) == 8) {   /* 8 fmt data */
        return 8;                                    /* 8 fmt data */
    } else {
        return -1;
    }
}

static td_s32 tde_hal_getbpp_by_fmt16(tde_color_fmt fmt)
{
    switch (fmt) {
        case TDE_DRV_COLOR_FMT_RGB444:
        case TDE_DRV_COLOR_FMT_RGB555:
        case TDE_DRV_COLOR_FMT_RGB565:
        case TDE_DRV_COLOR_FMT_ARGB4444:
        case TDE_DRV_COLOR_FMT_ARGB1555:
        case TDE_DRV_COLOR_FMT_ACLUT88:
        case TDE_DRV_COLOR_FMT_YCBCR422:
        case TDE_DRV_COLOR_FMT_HALFWORD:
        case TDE_DRV_COLOR_FMT_PKGVYUY:
            return 16; /* 16 fmt data */

        default:
            return -1;
    }
}

static td_s32 tde_hal_getbpp_by_fmt24(tde_color_fmt fmt)
{
    switch (fmt) {
        case TDE_DRV_COLOR_FMT_RGB888:
        case TDE_DRV_COLOR_FMT_ARGB8565:
        case TDE_DRV_COLOR_FMT_YCBCR888:
            return 24; /* 24 fmt data */
        default:
            return -1;
    }
}

static td_s32 tde_hal_getbpp_by_fmt32(tde_color_fmt fmt)
{
    switch (fmt) {
        case TDE_DRV_COLOR_FMT_ARGB8888:
        case TDE_DRV_COLOR_FMT_AYCBCR8888:
        case TDE_DRV_COLOR_FMT_RABG8888:
            return 32; /* 32 fmt data */
        default:
            return -1;
    }
}

static td_s32 tde_hal_getbpp_by_fmt1(tde_color_fmt fmt)
{
    switch (fmt) {
        case TDE_DRV_COLOR_FMT_CLUT1:
        case TDE_DRV_COLOR_FMT_A1:
        case TDE_DRV_COLOR_FMT_CLUT1B:
        case TDE_DRV_COLOR_FMT_A1B:
            return 1;
        default:
            return -1;
    }
}

static td_s32 tde_hal_getbpp_by_fmt2(tde_color_fmt fmt)
{
    switch (fmt) {
        case TDE_DRV_COLOR_FMT_CLUT2:
        case TDE_DRV_COLOR_FMT_CLUT2B:
            return 2; /* 2 function return bpp */
        default:
            return -1; /* -1  return failure */
    }
}
static td_s32 tde_hal_getbpp_by_fmt4(tde_color_fmt fmt)
{
    switch (fmt) {
        case TDE_DRV_COLOR_FMT_CLUT4:
        case TDE_DRV_COLOR_FMT_CLUT4B:
            return 4; /* 4 alg data */
        default:
            return -1;
    }
}

static td_s32 tde_hal_getbpp_by_fmt8(tde_color_fmt fmt)
{
    switch (fmt) {
        case TDE_DRV_COLOR_FMT_CLUT8:
        case TDE_DRV_COLOR_FMT_ACLUT44:
        case TDE_DRV_COLOR_FMT_A8:
        case TDE_DRV_COLOR_FMT_BYTE:
            return 8; /* 8 fmt data */
        default:
            return -1;
    }
}

/*
 * Function:      tde_hal_get_resize_para_htable
 * Description:   get index table according the step
 * Input:         step: input step
 * Return:        index table address
 */
static td_u32 tde_hal_get_resize_para_htable(td_u32 step)
{
    td_u32 index;

    if (step < TDE_RESIZE_8X32_AREA_0) {
        index = 0;
    } else if (step == TDE_RESIZE_8X32_AREA_0) {
        index = 1;
    } else if ((step > TDE_RESIZE_8X32_AREA_0) && (step <= TDE_RESIZE_8X32_AREA_1)) {
        index = 2; /* 2 ResizeParaHTable */
    } else if ((step > TDE_RESIZE_8X32_AREA_1) && (step <= TDE_RESIZE_8X32_AREA_2)) {
        index = 3; /* 3 ResizeParaHTable */
    } else if ((step > TDE_RESIZE_8X32_AREA_2) && (step <= TDE_RESIZE_8X32_AREA_3)) {
        index = 4; /* 4 ResizeParaHTable */
    } else if ((step > TDE_RESIZE_8X32_AREA_3) && (step <= TDE_RESIZE_8X32_AREA_4)) {
        index = 5; /* 5 ResizeParaHTable */
    } else {
        index = 6; /* 6 ResizeParaHTable */
    }
    return index;
}

/*
 * Function:      tde_hal_get_resize_para_vtable
 * Description:   get table of parameter for resize
 * Input:         step:input step
 * Return:        address of table
 */
static td_u32 tde_hal_get_resize_para_vtable(td_u32 step)
{
    td_u32 index;

    /* get index table according step, CNcomment: According to the step length to find index table */
    if (step < TDE_RESIZE_PARA_AREA_0) {
        index = 0;
    } else if (step == TDE_RESIZE_PARA_AREA_0) {
        index = 1;
    } else if ((step > TDE_RESIZE_PARA_AREA_0) && (step <= TDE_RESIZE_PARA_AREA_1)) {
        index = 2; /* 2 alg data */
    } else if ((step > TDE_RESIZE_PARA_AREA_1) && (step <= TDE_RESIZE_PARA_AREA_2)) {
        index = 3; /* 3 alg data */
    } else if ((step > TDE_RESIZE_PARA_AREA_2) && (step <= TDE_RESIZE_PARA_AREA_3)) {
        index = 4; /* 4 alg data */
    } else if ((step > TDE_RESIZE_PARA_AREA_3) && (step <= TDE_RESIZE_PARA_AREA_4)) {
        index = 5; /* 5 alg data */
    } else {
        index = 6; /* 6 alg data */
    }
    return index;
}

/*
 * Function:      tde_hal_init_queue
 * Description:   Initialize Aq list,config the operation which is needed
 */
static td_void tde_hal_init_queue(td_void)
{
    if (g_base_vir_addr == TD_NULL) {
        tde_error("null pointer\n");
        return;
    }
  /*
   * write 0 to Aq list start address register
   * CNcomment: 0 will be written Aq list first address register
   */
    tde_write_reg(g_base_vir_addr, TDE_AQ_NADDR_HI, 0);
    tde_write_reg(g_base_vir_addr, TDE_AQ_NADDR_LOW, 0);
}

/*
 * Function:      tde_hal_set_deflicer_level
 * Description:   SetDeflicerLevel
 * Input:         deflicker_level:anti-flicker levels including:auto,low,middle,high
 */
#if (TDE_CAPABILITY & DEFLICKER)
td_s32 tde_hal_set_deflicer_level(drv_tde_deflicker_level deflicker_level)
{
    g_deflicker_level = deflicker_level;
    return TD_SUCCESS;
}

td_s32 tde_hal_get_deflicer_level(drv_tde_deflicker_level *deflicker_level)
{
    if (deflicker_level == TD_NULL) {
        tde_error("null pointer\n");
        return TD_FAILURE;
    }
    *deflicker_level = g_deflicker_level;
    return TD_SUCCESS;
}
#endif

td_s32 tde_hal_set_alpha_threshold(td_u8 alpha_threshold_value)
{
    g_alpha_threshold_value = alpha_threshold_value;

    return TD_SUCCESS;
}

td_s32 tde_hal_get_alpha_threshold(td_u8 *alpha_threshold_value)
{
    if (alpha_threshold_value == TD_NULL) {
        tde_error("null pointer\n");
        return TD_FAILURE;
    }
    *alpha_threshold_value = g_alpha_threshold_value;

    return TD_SUCCESS;
}

td_s32 tde_hal_set_alpha_threshold_state(td_bool alpha_threshold_en)
{
    g_alpha_threshold = alpha_threshold_en;

    return TD_SUCCESS;
}

td_s32 tde_hal_get_alpha_threshold_state(td_bool *alpha_threshold_en)
{
    if (alpha_threshold_en == TD_NULL) {
        tde_error("null pointer\n");
        return TD_FAILURE;
    }
    *alpha_threshold_en = g_alpha_threshold;

    return TD_SUCCESS;
}

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
/* see define of tde_hw_node */
td_u8 *g_update[] = {
    "src1_ctrl            ", /* 0x0000 */
    "src1_ch0_addr_high   ", /* 0x0004 */
    "src1_ch0_addr_low    ", /* 0x0008 */
    "src1_ch1_addr_high   ", /* 0x000c */
    "src1_ch1_addr_low    ", /* 0x0010 */
    "src1_ch0_stride      ", /* 0x0014 */
    "src1_ch1_stride      ", /* 0x0018 */
    "src1_imgsize         ", /* 0x001c */
    "src1_fill            ", /* 0x0020 */
    "src1_alpha           ", /* 0x0024 */
    "src1_pix_offset      ", /* 0x0028 */
    "src2_ctrl            ", /* 0x002c */
    "src2_ch0_addr_high   ", /* 0x0030 */
    "src2_ch0_addr_low    ", /* 0x0034 */
    "src2_ch1_addr_high   ", /* 0x0038 */
    "src2_ch1_addr_low    ", /* 0x003c */
    "src2_ch0_stride      ", /* 0x0040 */
    "src2_ch1_stride      ", /* 0x0044 */
    "src2_imgsize         ", /* 0x0048 */
    "src2_fill            ", /* 0x004c */
    "src2_alpha           ", /* 0x0050 */
    "src2_pix_offset      ", /* 0x0054 */
    "des_ctrl             ", /* 0x00f8 */
    "des_ch0_addr_high    ", /* 0x00fc */
    "des_ch0_addr_low     ", /* 0x0100 */
    "des_ch1_addr_high    ", /* 0x0104 */
    "des_ch1_addr_low     ", /* 0x0108 */
    "des_ch0_stride       ", /* 0x010c */
    "des_ch1_stride       ", /* 0x0110 */
    "des_imgsize          ", /* 0x0114 */
    "des_alpha            ", /* 0x0118 */
    "des_crop_pos_st      ", /* 0x011c */
    "des_crop_pos_ed      ", /* 0x0120 */
    "des_pix_offset       ", /* 0x0124 */
    "src1_hsp             ", /* 0x200 */
    "src1_hloffset        ", /* 0x204 */
    "src1_hcoffset        ", /* 0x208 */
    "src1_vsp             ", /* 0x20c */
    "src1_vsr             ", /* 0x210 */
    "src1_voffset         ", /* 0x214 */
    "src1_zmeoreso        ", /* 0x218 */
    "src1_zmeireso        ", /* 0x21c */
    "src1_hpzme           ", /* 0x228 */
    "src1_hpzme_size      ", /* 0x22c */
    "src1_csc_idc0        ", /* 0x230 */
    "src1_csc_idc1        ", /* 0x234 */
    "src1_csc_odc0        ", /* 0x238 */
    "src1_csc_odc1        ", /* 0x23c */
    "src1_csc_p0          ", /* 0x240 */
    "src1_csc_p1          ", /* 0x244 */
    "src1_csc_p2          ", /* 0x248 */
    "src1_csc_p3          ", /* 0x24c */
    "src1_csc_p4          ", /* 0x250 */
    "src1_dither_ctrl     ", /* 0x254 */
    "src2_hsp             ", /* 0x280 */
    "src2_hloffset        ", /* 0x284 */
    "src2_hcoffset        ", /* 0x288 */
    "src2_vsp             ", /* 0x28c */
    "src2_vsr             ", /* 0x290 */
    "src2_voffset         ", /* 0x294 */
    "src2_zmeoreso        ", /* 0x298 */
    "src2_zmeireso        ", /* 0x29c */
    "src2_hpzme           ", /* 0x2a8 */
    "src2_hpzme_size      ", /* 0x2ac */
    "src2_csc_mux         ", /* 0x2b0 */
    "des_csc_idc0         ", /* 0x2b4 */
    "des_csc_idc1         ", /* 0x2b8 */
    "des_csc_odc0         ", /* 0x2bc */
    "des_csc_odc1         ", /* 0x2c0 */
    "des_csc_p0           ", /* 0x2c4 */
    "des_csc_p1           ", /* 0x2c8 */
    "des_csc_p2           ", /* 0x2cc */
    "des_csc_p3           ", /* 0x2d0 */
    "des_csc_p4           ", /* 0x2d4 */
    "des_dither_ctrl      ", /* offset 0x2d0 + 0x08 */
    "des_dswn             ", /* 0x2dc */
    "src2_rtt_ctrl        ", /* 0x3f0 */
    "cbmctrl              ", /* 0x300 */
    "cbmbkg               ", /* 0x304 */
    "cbmcolorize          ", /* 0x308 */
    "cbmalupara           ", /* 0x30c */
    "cbmkeypara           ", /* 0x310 */
    "cbmkeymin            ", /* 0x314 */
    "cbmkeymax            ", /* 0x318 */
    "cbmkeymask           ", /* 0x31c */
    "src1_cbmpara         ", /* 0x320 */
    "src1_cbmstpos        ", /* 0x324 */
    "src2_cbmpara         ", /* 0x328 */
    "src2_cbmstpos        ", /* 0x32c */
    "src1_zme_lhaddr_high ", /* 0x480 */
    "src1_zme_lhaddr_low  ", /* 0x484 */
    "src1_zme_lvaddr_high ", /* 0x488 */
    "src1_zme_lvaddr_low  ", /* 0x48c */
    "src1_zme_chaddr_high ", /* 0x490 */
    "src1_zme_chaddr_low  ", /* 0x494 */
    "src1_zme_cvaddr_high ", /* 0x498 */
    "src1_zme_cvaddr_low  ", /* 0x49c */
    "src2_zme_lhaddr_high ", /* 0x4a0 */
    "src2_zme_lhaddr_low  ", /* 0x4a4 */
    "src2_zme_lvaddr_high ", /* 0x4a8 */
    "src2_zme_lvaddr_low  ", /* 0x4ac */
    "src2_zme_chaddr_high ", /* 0x4b0 */
    "src2_zme_chaddr_low  ", /* 0x4b4 */
    "src2_zme_cvaddr_high ", /* 0x4b8 */
    "src2_zme_cvaddr_low  ", /* 0x4bc */
    "clut_addr_high       ", /* 0x4c0 */
    "clut_addr_low        ", /* 0x4c4 */
};


static td_void tde_hal_node_print_31d(osal_proc_entry_t *p, const td_u32 *cur_node)
{
    td_s32 i;
    /* 22 SRC1_CTRL 0x0 */
    for (i = 0; i < 22; i++) {
        osal_seq_printf(p, "(%s):\t0x%08x\n", g_update[i], *(cur_node + i));
    }
    /* 12 DES_CTRL  0x0f8 */
    for (i = 0; i < 12; i++) {
        osal_seq_printf(p, "(%s):\t0x%08x\n", g_update[i + 22], *(cur_node + i + 62)); /* 22 62 alg data */
    }
    /* 8 SRC1_HSP 0x200 */
    for (i = 0; i < 8; i++) {
        osal_seq_printf(p, "(%s):\t0x%08x\n", g_update[i + 34], *(cur_node + i + 128)); /* 34 128 alg data */
    }
    /* 12 SRC1_HPZME 0x228 */
    for (i = 0; i < 12; i++) {
        osal_seq_printf(p, "(%s):\t0x%08x\n", g_update[i + 42], *(cur_node + i + 138)); /* 42 138 alg data */
    }
    /* 8 SRC2_HSP 0x280 */
    for (i = 0; i < 8; i++) {
        osal_seq_printf(p, "(%s):\t0x%08x\n", g_update[i + 54], *(cur_node + i + 160)); /* 54 160 alg data */
    }
    /* 14 SRC2_HPZME 0x2a8 */
    for (i = 0; i < 14; i++) {
        osal_seq_printf(p, "(%s):\t0x%08x\n", g_update[i + 62], *(cur_node + i + 170)); /* 62 170 alg data */
    }
    /* SRC2_RTT_CTRL 0x3f0 */
    for (i = 0; i < 1; i++) {
        osal_seq_printf(p, "(%s):\t0x%08x\n", g_update[i + 76], *(cur_node + i + 252)); /* 76 252 alg data */
    }
    /* 12 CBMCTRL 0x300 */
    for (i = 0; i < 12; i++) {
        osal_seq_printf(p, "(%s):\t0x%08x\n", g_update[i + 77], *(cur_node + i + 192)); /* 77 192 alg data */
    }
    /* 18 TDE_SRC1_ZME_LHADDR 0x480 */
    for (i = 0; i < 18; i++) {
        osal_seq_printf(p, "(%s):\t0x%08x\n", g_update[i + 89], *(cur_node + i + 288)); /* 89 288 alg data */
    }
}


osal_proc_entry_t *tde_hal_node_print_info(osal_proc_entry_t *p, const td_u32 *cur_node)
{
    if (p == TD_NULL) {
        return 0;
    }
    /* print node information */
    osal_seq_printf(p, "\n--------- tde node params info ---------\n");

    tde_hal_node_print_31d(p, cur_node);

    return p;
}
#endif


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART10_H_ */
