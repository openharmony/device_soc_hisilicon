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

#ifndef DRV_VO_COEF_COMM_H
#define DRV_VO_COEF_COMM_H

#include "ot_type.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

typedef struct {
    td_s32 csc_coef00;
    td_s32 csc_coef01;
    td_s32 csc_coef02;

    td_s32 csc_coef10;
    td_s32 csc_coef11;
    td_s32 csc_coef12;

    td_s32 csc_coef20;
    td_s32 csc_coef21;
    td_s32 csc_coef22;
} vdp_csc_coef;

typedef struct {
    td_s32 csc_in_dc0;
    td_s32 csc_in_dc1;
    td_s32 csc_in_dc2;

    td_s32 csc_out_dc0;
    td_s32 csc_out_dc1;
    td_s32 csc_out_dc2;
} vdp_csc_dc_coef;

typedef struct {
    /* for old version csc */
    td_s32 csc_coef00;
    td_s32 csc_coef01;
    td_s32 csc_coef02;

    td_s32 csc_coef10;
    td_s32 csc_coef11;
    td_s32 csc_coef12;

    td_s32 csc_coef20;
    td_s32 csc_coef21;
    td_s32 csc_coef22;

    td_s32 csc_in_dc0;
    td_s32 csc_in_dc1;
    td_s32 csc_in_dc2;

    td_s32 csc_out_dc0;
    td_s32 csc_out_dc1;
    td_s32 csc_out_dc2;
} csc_coef;

typedef struct {
    td_s32 csc_scale2p;
    td_s32 csc_clip_min;
    td_s32 csc_clip_max;
} csc_coef_param;

typedef struct {
    td_s32 vga_hsp_tmp0;
    td_s32 vga_hsp_tmp1;
    td_s32 vga_hsp_tmp2;
    td_s32 vga_hsp_tmp3;
    td_u32 vga_hsp_coring;
    td_s32 vga_hsp_gainneg;
    td_s32 vga_hsp_gainpos;
    td_s32 vga_hsp_adpshooten;
    td_u32 vga_hsp_winsize;
    td_u32 vga_hsp_mixratio;
    td_u32 vga_hsp_underth;
    td_u32 vga_hsp_overth;
} hsp_hf_coef;

typedef struct {
    td_u32 vga_hsp_hf_shootdiv;
    td_u32 vga_hsp_lti_ratio;
    td_u32 vga_hsp_ldti_gain;
    td_u32 vga_hsp_cdti_gain;
    td_u32 vga_hsp_peak_ratio;
    td_u32 vga_hsp_glb_overth;
    td_u32 vga_hsp_glb_underth;
} hsp_coef;

/* CVFIR VCOEF */
typedef struct {
    td_s32 vccoef00;
    td_s32 vccoef01;
    td_s32 vccoef02;
    td_s32 vccoef03;
    td_s32 vccoef10;
    td_s32 vccoef11;
    td_s32 vccoef12;
    td_s32 vccoef13;
} cvfir_coef;

/* HFIR VCOEF */
typedef struct {
    td_s32 coef0;
    td_s32 coef1;
    td_s32 coef2;
    td_s32 coef3;
    td_s32 coef4;
    td_s32 coef5;
    td_s32 coef6;
    td_s32 coef7;
} hfir_coef;
#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#define VO_MAX_ZME_PHASE 17
#define VO_MAX_ZME_TAP   8

/*
 * 严重注意:
 * 寄存器上获取的比率 ratio = 输入分辨率/输出分辨率
 * 而算法给出的比率   ratio = 输出分辨率/输入分辨率    (HERE USE)
 */
typedef enum {
    VOU_ZOOM_COEF_UP_1 = 0,
    VOU_ZOOM_COEF_EQU_1,
    VOU_ZOOM_COEF_075,
    VOU_ZOOM_COEF_0666,
    VOU_ZOOM_COEF_05,
    VOU_ZOOM_COEF_04,
    VOU_ZOOM_COEF_0375,
    VOU_ZOOM_COEF_033,
    VOU_ZOOM_COEF_0,
    VOU_ZOOM_COEF_BUTT
} vou_zoom_coef;

typedef enum {
    VOU_ZOOM_TAP_6LH = 0,
    VOU_ZOOM_TAP_4CH,
    VOU_ZOOM_TAP_4LV,
    VOU_ZOOM_TAP_4CV,
    VOU_ZOOM_TAP_BUTT
} vou_zoom_tap;
#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef DRV_VO_COEF_COMM_H */
