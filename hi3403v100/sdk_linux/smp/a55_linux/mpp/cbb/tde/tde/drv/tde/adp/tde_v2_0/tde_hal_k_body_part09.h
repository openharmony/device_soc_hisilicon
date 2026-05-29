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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART09_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART09_H_

{
    td_u32 zme_in_fmt = 0;

    td_bool fmt_argb = (in_drv_fmt <= TDE_DRV_COLOR_FMT_ACLUT88) || ((in_drv_fmt == TDE_DRV_COLOR_FMT_YCBCR444MB)) ||
                       (in_drv_fmt == TDE_DRV_COLOR_FMT_YCBCR400MBP);
    td_bool fmt_yuv422 = (in_drv_fmt == TDE_DRV_COLOR_FMT_YCBCR422MBH) || (in_drv_fmt == TDE_DRV_COLOR_FMT_YCBCR422);
    td_bool fmt_yuv420 = (in_drv_fmt == TDE_DRV_COLOR_FMT_YCBCR420MB) || (in_drv_fmt == TDE_DRV_COLOR_FMT_YCBCR422MBV);

    if (fmt_argb) {
        zme_in_fmt = TDE_ZME_FMT_ARGB;
    }

    if (fmt_yuv422) {
        zme_in_fmt = TDE_ZME_FMT_YUV422;
    }

    if (fmt_yuv420) {
        zme_in_fmt = TDE_ZME_FMT_YUV420;
    }

    return zme_in_fmt;
}

static td_u32 tde_hal_get_hpzme_mode(td_u32 out_rect_width, td_u32 in_rect_width)
{
    td_u32 hpzme_mode;

    if ((out_rect_width * 32) > in_rect_width) { /* 32 alg data */
        hpzme_mode = 1;
    } else if ((out_rect_width * 64) > in_rect_width) { /* 64 alg data */
        hpzme_mode = 3; /* 3 alg data */
    } else if ((out_rect_width * 128) > in_rect_width) { /* 128 alg data */
        hpzme_mode = 7; /* 7 hpzme mode */
    } else {
        hpzme_mode = 15; /* 15 hpzme mode */
    }

    return hpzme_mode;
}

static td_void tde_hal_get_filter_mode(drv_tde_filter_mode filter_mode, tde_filtermode *flt_mode)
{
    td_bool filter_mode_color = (filter_mode == DRV_TDE_FILTER_MODE_COLOR);
    td_bool filter_mode_alpha = (filter_mode == DRV_TDE_FILTER_MODE_ALPHA);
    td_bool filter_mode_both = (filter_mode == DRV_TDE_FILTER_MODE_BOTH);
    td_u32 alpha_en;
    td_u32 luma_en;
    td_u32 chrome_en;

    if (filter_mode_color) {
        chrome_en = 1;
        luma_en = 1;
        alpha_en = 0;
    } else if (filter_mode_alpha) {
        chrome_en = 0;
        luma_en = 0;
        alpha_en = 1;
    } else if (filter_mode_both) {
        chrome_en = 1;
        luma_en = 1;
        alpha_en = 1;
    } else {
        chrome_en = 0;
        luma_en = 0;
        alpha_en = 0;
    }
    flt_mode->alpha_en = alpha_en;
    flt_mode->luma_en = luma_en;
    flt_mode->chrome_en = chrome_en;

    return;
}

static td_s32 tde_hal_get_ver_scale_coeff(td_u32 out_rect_height, td_u32 in_rect_height, td_s32 *ver_scale_coeff)
{
    if ((out_rect_height * 32) >= in_rect_height) { /* 32 alg data */
        *ver_scale_coeff = 2; /* 2 ver scale coeff data */
    } else if ((out_rect_height * 64) >= in_rect_height) { /* 64 alg data */
        *ver_scale_coeff = 4; /* 4 ver scale coeff data */
    } else if ((out_rect_height * 128) >= in_rect_height) { /* 128 alg data */
        *ver_scale_coeff = 8; /* 8 ver scale coeff data */
    } else if (out_rect_height * 256 >= in_rect_height) { /* 256 alg data */
        *ver_scale_coeff = 16; /* 16 ver scale coeff data */
    } else {
        tde_error("Invalid para input!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    return TD_SUCCESS;
}

static td_void tde_hal_node_set_color_cvt_in_rgb2yuv(tde_hw_node *hw_node)
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbitfield-constant-conversion"
#endif
    hw_node->src1_csc_idc0.bits.cscidc0 = 0;
    hw_node->src1_csc_idc0.bits.cscidc1 = 0;
    hw_node->src1_csc_idc1.bits.cscidc2 = 0;
    hw_node->src1_csc_odc0.bits.cscodc0 = 128 * 4; /* 4 * 128 cscodc0 */
    hw_node->src1_csc_odc0.bits.cscodc1 = 128 * 4; /* 4 * 128 cscodc1 */
    hw_node->src1_csc_odc1.bits.cscodc2 = 16 * 4;  /* 4 * 16 cscodc2 */

    hw_node->src1_csc_p0.bits.cscp00 = 66 * 4;  /* 4 * 66 cscp00 */
    hw_node->src1_csc_p0.bits.cscp01 = 129 * 4; /* 4 * 129 cscp01 data */

    hw_node->src1_csc_p1.bits.cscp02 = 25 * 4;  /* 4 * 25 cscp02 */
    hw_node->src1_csc_p1.bits.cscp10 = -38 * 4; /* 4 * -38 cscp10 data */

    hw_node->src1_csc_p2.bits.cscp11 = -74 * 4; /* 4 * -74 cscp11 data */
    hw_node->src1_csc_p2.bits.cscp12 = 112 * 4; /* 112 * 4 cscp12 data */

    hw_node->src1_csc_p3.bits.cscp20 = 112 * 4; /* 4 * 112 cscp20 data */
    hw_node->src1_csc_p3.bits.cscp21 = -94 * 4; /* 4 * -94 cscp21 data */

    hw_node->src1_csc_p4.bits.cscp22 = -18 * 4; /* 4 * -18 cscp22 data */
#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

static td_void tde_hal_node_set_color_cvt_in_yuv2rgb(tde_hw_node *hw_node)
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbitfield-constant-conversion"
#endif
    hw_node->src1_csc_idc0.bits.cscidc0 = -128 * 4; /* 4 * -128 cscidc0 data */
    hw_node->src1_csc_idc0.bits.cscidc1 = -128 * 4; /* 4 * -128 cscidc1 data */
    hw_node->src1_csc_idc1.bits.cscidc2 = -16 * 4;  /* 4 * -16 cscidc2 data */
    hw_node->src1_csc_odc0.bits.cscodc0 = 0;
    hw_node->src1_csc_odc0.bits.cscodc1 = 0;
    hw_node->src1_csc_odc1.bits.cscodc2 = 0;

    hw_node->src1_csc_p0.bits.cscp00 = 297 * 4; /* 4 * 297 cscp00 data */
    hw_node->src1_csc_p0.bits.cscp01 = 0;

    hw_node->src1_csc_p1.bits.cscp02 = 408 * 4; /* 4 * 408 cscp02 data */
    hw_node->src1_csc_p1.bits.cscp10 = 297 * 4; /* 4 * 297 cscp10 data */

    hw_node->src1_csc_p2.bits.cscp11 = -100 * 4; /* 4 * -100 cscp11 data */
    hw_node->src1_csc_p2.bits.cscp12 = -208 * 4; /* 4 * -208 cscp12 data */

    hw_node->src1_csc_p3.bits.cscp20 = 297 * 4; /* 4 * 297 cscp20 data */
    hw_node->src1_csc_p3.bits.cscp21 = 516 * 4; /* 4 * 516 cscp21 data */

    hw_node->src1_csc_p4.bits.cscp22 = 0;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

static td_void tde_hal_node_set_color_cvt_out_rgb2yuv(tde_hw_node *hw_node)
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbitfield-constant-conversion"
#endif
    hw_node->des_csc_idc0.bits.cscidc0 = -128 * 4; /* 4 * -128 cscidc0 data */
    hw_node->des_csc_idc0.bits.cscidc1 = -128 * 4; /* 4 * -128 cscidc1 data */
    hw_node->des_csc_idc1.bits.cscidc2 = -16 * 4;  /* 4 * -16 cscidc2 data */
    hw_node->des_csc_odc0.bits.cscodc0 = 0;
    hw_node->des_csc_odc0.bits.cscodc1 = 0;
    hw_node->des_csc_odc1.bits.cscodc2 = 0;

    hw_node->des_csc_p0.bits.cscp00 = 297 * 4; /* 4 * 297 cscp00 data */
    hw_node->des_csc_p0.bits.cscp01 = 0;

    hw_node->des_csc_p1.bits.cscp02 = 408 * 4; /* 4 * 408 cscp02 data */
    hw_node->des_csc_p1.bits.cscp10 = 297 * 4; /* 4 * 297 cscp10 data */

    hw_node->des_csc_p2.bits.cscp11 = -100 * 4; /* 4 * -100 cscp11 data */
    hw_node->des_csc_p2.bits.cscp12 = -208 * 4; /* 4 * -208 cscp12 data */

    hw_node->des_csc_p3.bits.cscp20 = 297 * 4; /* 4 * 297 cscp20 data */
    hw_node->des_csc_p3.bits.cscp21 = 516 * 4; /* 4 * 516 cscp21 data */
    hw_node->des_csc_p4.bits.cscp22 = 0;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

static td_void tde_hal_node_set_color_cvt_out_yuv2rgb(tde_hw_node *hw_node)
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbitfield-constant-conversion"
#endif
    hw_node->des_csc_idc0.bits.cscidc0 = 0;
    hw_node->des_csc_idc0.bits.cscidc1 = 0;
    hw_node->des_csc_idc1.bits.cscidc2 = 0;
    hw_node->des_csc_odc0.bits.cscodc0 = 128 * 4; /* 4 * 128 cscodc0 data */
    hw_node->des_csc_odc0.bits.cscodc1 = 128 * 4; /* 4 * 128 cscodc1 data */
    hw_node->des_csc_odc1.bits.cscodc2 = 16 * 4;  /* 4 * 16 cscodc2 data */

    hw_node->des_csc_p0.bits.cscp00 = 66 * 4;  /* 4 * 66 cscp00 data */
    hw_node->des_csc_p0.bits.cscp01 = 129 * 4; /* 4 * 129 cscp01 data */

    hw_node->des_csc_p1.bits.cscp02 = 25 * 4;  /* 4 * 25 cscp02 data */
    hw_node->des_csc_p1.bits.cscp10 = -38 * 4; /* 4 * -38 cscp10 data */

    hw_node->des_csc_p2.bits.cscp11 = -74 * 4; /* 4 * -74 cscp11 data */

    hw_node->des_csc_p2.bits.cscp12 = 112 * 4; /* 4 * 112 cscp12 data */

    hw_node->des_csc_p3.bits.cscp20 = 112 * 4; /* 4 * 112 cscp20 data */

    hw_node->des_csc_p3.bits.cscp21 = -94 * 4; /* 4 * -94 cscp21 data */
    hw_node->des_csc_p4.bits.cscp22 = -18 * 4; /* 4 * -18 cscp22 data */
#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

/*
 * Function:      tde_hal_node_set_color_convert
 * Description:   set parameter for color space change
 * Input:         hw_node:pointer of node
 *                conv: parameter of color space change
 */
td_s32 tde_hal_node_set_color_convert(tde_hw_node *hw_node, const tde_conv_mode_cmd *conv)
{
    td_u32 capability = 0;
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (conv == TD_NULL) {
        tde_error("Conv is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    tde_hal_get_capability(&capability);
    if (!(capability & CSCCOVERT)) {
        tde_error("It deos not support CSCCovert\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    if (conv->in_conv) {
        hw_node->src2_csc_mux.bits.src2_csc_mode = 1 - (td_u32)conv->in_src1_conv;

        hw_node->src1_csc_idc0.bits.csc_en = 1;
        hw_node->src1_dither_ctrl.bits.dither_en = 1;
        hw_node->src1_dither_ctrl.bits.dither_round = 0;

        if (conv->in_rgb2_yc) {
            tde_hal_node_set_color_cvt_in_rgb2yuv(hw_node);
        } else {
            tde_hal_node_set_color_cvt_in_yuv2rgb(hw_node);
        }
    }

    if (conv->out_conv) {
        hw_node->src2_csc_mux.bits.src2_csc_mode = 1 - (td_u32)conv->in_src1_conv;

        hw_node->des_csc_idc0.bits.csc_en = 1;
        hw_node->dst_dither_ctrl.bits.dither_en = 1;
        hw_node->dst_dither_ctrl.bits.dither_round = 0;

        if (conv->in_rgb2_yc) {
            tde_hal_node_set_color_cvt_out_rgb2yuv(hw_node);
        } else {
            tde_hal_node_set_color_cvt_out_yuv2rgb(hw_node);
        }
    }
    return TD_SUCCESS;
}

td_s32 tde_set_node_csc(const tde_hw_node *hw_node, drv_tde_csc_opt csc_opt)
{
    ot_unused(hw_node);
    ot_unused(csc_opt);
    return TD_SUCCESS;
}

static td_u32 *tde_hal_make_hf_coef(td_void)
{
    td_u32 i;
    td_u32 *hf_coef = TD_NULL;

    hf_coef = (td_u32 *)tde_malloc(TDE_PARA_HTABLE_SIZE * TDE_PARA_HTABLE_NUM);
    if (hf_coef == TD_NULL) {
        tde_error("Alloc horizontal coef failed!HCoef table size:%d\n", TDE_PARA_HTABLE_SIZE * TDE_PARA_HTABLE_NUM);
        return TD_NULL;
    }

    for (i = 0; i < TDE_PARA_HTABLE_NUM; i++) {
         /* 4 size value */
        if (memcpy_s(hf_coef + i * (TDE_PARA_HTABLE_SIZE / 4), TDE_PARA_HTABLE_SIZE, g_tde_6x32_coef +
            i * (TDE_PARA_HTABLE_ORG_SIZE), (TDE_PARA_HTABLE_ORG_SIZE) * 4) != EOK) { /* 4 size */
            tde_error("memcpy_s failure\n");
            tde_free(hf_coef);
            return TD_NULL;
        }
    }

#ifndef __RTOS__
    g_para_table.hf_coef_addr = wgetphy((td_void *)hf_coef);
#else
    g_para_table.hf_coef_addr = (td_u32)hf_coef;
#endif
    return hf_coef;
}

static td_u32 *tde_hal_make_vf_coef(td_void)
{
    td_u32 i;
    td_u32 *vf_coef = TD_NULL;

    vf_coef = (td_u32 *)tde_malloc(TDE_PARA_VTABLE_SIZE * TDE_PARA_VTABLE_NUM);
    if (vf_coef == TD_NULL) {
        tde_error("Alloc vertical coef failed!VfCoef table size:%d\n", TDE_PARA_VTABLE_SIZE * TDE_PARA_VTABLE_NUM);
        return TD_NULL;
    }
    /*
     * copy parameter according other offer way , CNcomment :According to the parameter table algorithm
     * group provides the structure of the copies
     */
    for (i = 0; i < TDE_PARA_VTABLE_NUM; i++) {
        if (memcpy_s(vf_coef + i * (TDE_PARA_VTABLE_SIZE / 4), TDE_PARA_VTABLE_SIZE, g_org_vf_coef + /* 4 size value */
            i * (TDE_PARA_VTABLE_ORG_SIZE), (TDE_PARA_VTABLE_ORG_SIZE) * 4) != EOK) { /* 4 size value */
            tde_error("secure function failure\n");
            tde_free(vf_coef);
            return TD_NULL;
        }
    }

#ifndef __RTOS__
    g_para_table.vf_coef_addr = wgetphy((td_void *)vf_coef);
#else
    g_para_table.vf_coef_addr = (td_u32)vf_coef;
#endif
    return vf_coef;
}

#if (TDE_CAPABILITY & DEFLICKER)
static td_u32 *tde_hal_make_deflicker_vf_coef(td_void)
{
    td_u32 i;
    td_u32 *deflicker_vf_coef = TD_NULL;

    deflicker_vf_coef = (td_u32 *)tde_malloc(TDE_PARA_VTABLE_SIZE * TDE_PARA_VTABLE_NUM);
    if (deflicker_vf_coef == TD_NULL) {
        tde_error("Alloc Deflicker vertical coef failed!g_deflicker_vf_coef table size:%d\n",
                  TDE_PARA_VTABLE_SIZE * TDE_PARA_VTABLE_NUM);
        return TD_NULL;
    }

    for (i = 0; i < TDE_PARA_VTABLE_NUM; i++) {
        /* 4 alg data */
        if (memcpy_s(deflicker_vf_coef + i * (TDE_PARA_VTABLE_SIZE / 4), TDE_PARA_VTABLE_SIZE, g_deflicker_vf_coef +
            i * (TDE_PARA_VTABLE_ORG_SIZE), (TDE_PARA_VTABLE_ORG_SIZE) * 4) != EOK) { /* 4 alg data */
            tde_error("secure function failure\n");
            tde_free(deflicker_vf_coef);
            return TD_NULL;
        }
    }

#ifndef __RTOS__
    g_para_table.deflicker_vf_coef_addr = wgetphy((td_void *)deflicker_vf_coef);
#else
    g_para_table.deflicker_vf_coef_addr = (td_u32)deflicker_vf_coef;
#endif
    return deflicker_vf_coef;
}
#endif

static td_s32 tde_hal_init_para_table(td_void)
{
    td_u32 *hf_coef = TD_NULL;
    td_u32 *vf_coef = TD_NULL;

#if (TDE_CAPABILITY & DEFLICKER)
    td_u32 *deflicker_vf_coef = TD_NULL;
#endif

    (td_void)memset_s(&g_para_table, sizeof(g_para_table), 0, sizeof(g_para_table));
    hf_coef = tde_hal_make_hf_coef();
    if (hf_coef == TD_NULL) {
        return TD_FAILURE;
    }
    vf_coef = tde_hal_make_vf_coef();
    if (vf_coef == TD_NULL) {
        tde_free(hf_coef);
        return TD_FAILURE;
    }

#if (TDE_CAPABILITY & DEFLICKER)
    deflicker_vf_coef = tde_hal_make_deflicker_vf_coef();
    if (deflicker_vf_coef == TD_NULL) {
        tde_free(hf_coef);
        tde_free(vf_coef);
        return TD_FAILURE;
    }
#endif

    return TD_SUCCESS;
}

/*
 * Function:      tde_hal_cur_node
 * Description:   get the node physics address that is suspended
 * Return:        the address of current running node
 */
td_phys_addr_t tde_hal_cur_node(void)
{
    volatile td_u32 l_addr;
    volatile td_u32 h_addr;
    td_phys_addr_t addr;

    if (g_base_vir_addr == TD_NULL) {
        tde_error("null pointer\n");
        return 0;
    }

    l_addr = tde_read_reg(g_base_vir_addr, TDE_AQ_ADDR_LOW);
    h_addr = tde_read_reg(g_base_vir_addr, TDE_AQ_ADDR_HI);
    addr = ((td_u64)h_addr << 32) | l_addr; /* 32 alg data */
    return addr;
}

/*
 * Function:      tde_hal_getbpp_by_fmt
 * Description:   get bpp according color of driver
 * Input:         fmt: color type
 * Output:        pitch width
 * Return:        -1: wrong format
 */
static td_s32 tde_hal_getbpp_by_fmt(tde_color_fmt fmt)
{
#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART09_H_ */
