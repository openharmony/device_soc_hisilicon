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

#ifndef GFBG_COEF_H
#define GFBG_COEF_H

#include "ot_type.h"
#include "ot_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define GFBG_COEF_BIT_RANGE 12
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

typedef struct {
    td_s32 bits_0 : 10;
    td_s32 bits_1 : 10;
    td_s32 bits_2 : 10;
    td_s32 bits_32 : 2;
    td_s32 bits_38 : 8;
    td_s32 bits_4 : 10;
    td_s32 bits_5 : 10;
    td_s32 bits_64 : 4;
    td_s32 bits_66 : 6;
    td_s32 bits_7 : 10;
    td_s32 bits_8 : 10;
    td_s32 bits_96 : 6;
    td_s32 bits_94 : 4;
    td_s32 bits_10 : 10;
    td_s32 bits_11 : 10;
    td_s32 bits_12 : 8;
} vo_zoom_bit;

typedef struct {
    td_u32 size;
    vo_zoom_bit bit[GFBG_COEF_BIT_RANGE];
} vo_zoom_bit_array;

/* COLOR SPACE CONVERT DEFINITION */
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
    // for old version csc
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

    td_s32 new_csc_scale2p;
    td_s32 new_csc_clip_min;
    td_s32 new_csc_clip_max;
} csc_coef;

typedef struct {
    td_s32 csc_scale2p;
    td_s32 csc_clip_min;
    td_s32 csc_clip_max;
} csc_coef_param;

/* Vga Sharpen HF Coefficient */
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
    td_u32 vga_hsp_glb_unferth;
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

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* GFBG_COEF_H */
