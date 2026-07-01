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

#ifndef HAL_VO_DEF_COMM_H
#define HAL_VO_DEF_COMM_H

#include "ot_type.h"
#include "ot_defines.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

typedef enum {
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_400 = 0x1,
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_420 = 0x2,
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_422 = 0x3,
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_444 = 0x4,
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_411_4X1 = 0x6,
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_422_2X1 = 0x7,

    HAL_INPUTFMT_CBYCRY_PACKAGE_422 = 0x9,
    HAL_INPUTFMT_YCBYCR_PACKAGE_422 = 0xa,
    HAL_INPUTFMT_YCRYCB_PACKAGE_422 = 0xb,
    HAL_INPUTFMT_YCBCR_PACKAGE_444 = 0x1000,

    HAL_INPUTFMT_CLUT_1BPP = 0x00,
    HAL_INPUTFMT_CLUT_2BPP = 0x10,
    HAL_INPUTFMT_CLUT_4BPP = 0x20,
    HAL_INPUTFMT_CLUT_8BPP = 0x30,
    HAL_INPUTFMT_ACLUT_44 = 0x38,

    HAL_INPUTFMT_RGB_444 = 0x40,
    HAL_INPUTFMT_RGB_555 = 0x41,
    HAL_INPUTFMT_RGB_565 = 0x42,
    HAL_INPUTFMT_CBYCRY_PACKAGE_422_GRC = 0x43,
    HAL_INPUTFMT_YCBYCR_PACKAGE_422_GRC = 0x44,
    HAL_INPUTFMT_YCRYCB_PACKAGE_422_GRC = 0x45,
    HAL_INPUTFMT_ACLUT_88 = 0x46,
    HAL_INPUTFMT_ARGB_4444 = 0x48,
    HAL_INPUTFMT_ARGB_1555 = 0x49,

    HAL_INPUTFMT_RGB_888 = 0x50,
    HAL_INPUTFMT_YCBCR_888 = 0x51,
    HAL_INPUTFMT_ARGB_8565 = 0x5a,
    HAL_INPUTFMT_ARGB_6666 = 0x5b,

    HAL_INPUTFMT_KRGB_888 = 0x60,
    HAL_INPUTFMT_ARGB_8888 = 0x68,
    HAL_INPUTFMT_AYCBCR_8888 = 0x69,

    HAL_INPUTFMT_RGBA_4444 = 0xc8,
    HAL_INPUTFMT_RGBA_5551 = 0xc9,

    HAL_INPUTFMT_RGBA_6666 = 0xd8,
    HAL_INPUTFMT_RGBA_5658 = 0xda,

    HAL_INPUTFMT_RGBA_8888 = 0xe8,
    HAL_INPUTFMT_YCBCRA_8888 = 0xe9,

    HAL_DISP_PIXELFORMAT_BUTT
} hal_disp_pixel_format;

typedef enum {
    HAL_DISP_ZME_OUTFMT420 = 0,
    HAL_DISP_ZME_OUTFMT422,
    HAL_DISP_ZME_OUTFMT444,
    HAL_DISP_ZME_OUTFMT_BUTT
} hal_disp_zme_outfmt;

/* vou CBM MIXER */
typedef enum {
    HAL_CBMMIX1 = 0,
    HAL_CBMMIX2 = 1,
    HAL_CBMMIX3 = 2,

    HAL_CBMMIX1_BUTT
} hal_cbmmix;

typedef enum {
    HAL_HFIRMODE_MEDEN = 0,  /* median filtering enable */
    HAL_HFIRMODE_COPY,  /* chroma HFIR copy */
    HAL_HFIRMODE_DOUBLE,  /* bilinear interpolation */
    HAL_HFIRMODE_6TAPFIR,  /* 6 order FIR */

    HAL_HFIRMODE_BUTT
} hal_hfirmode;

typedef struct {
    td_u32 syncm;
    td_u32 iop;
    td_u8 intfb;

    td_u16 vact;
    td_u16 vbb;
    td_u16 vfb;

    td_u16 hact;
    td_u16 hbb;
    td_u16 hfb;
    td_u16 hmid;

    td_u16 bvact;
    td_u16 bvbb;
    td_u16 bvfb;

    td_u16 hpw;
    td_u16 vpw;

    td_u32 idv;
    td_u32 ihs;
    td_u32 ivs;
} hal_disp_syncinfo;

typedef struct {
    td_u32 f_inv;
    td_u32 vs_inv;
    td_u32 hs_inv;
    td_u32 dv_inv;
} hal_disp_syncinv;

typedef enum {
    HAL_MULTICHN_EN_1P1C = 0x1,  /* 1 pixel / 1 cycle */
    HAL_MULTICHN_EN_2P1C = 0x2,  /* 2 pixel / 1 cycle */
} hal_multichn_en;

typedef struct {
    td_u16 bkg_a;
    td_u16 bkg_y;
    td_u16 bkg_cb;
    td_u16 bkg_cr;
} hal_disp_bkcolor;

typedef struct {
    td_u16 clip_low_y;
    td_u16 clip_low_cb;
    td_u16 clip_low_cr;

    td_u16 clip_high_y;
    td_u16 clip_high_cb;
    td_u16 clip_high_cr;
} hal_disp_clip;

typedef struct {
    td_s32 luma;
    td_s32 cont;
    td_s32 hue;
    td_s32 satu;
} hal_csc_value;

typedef struct {
    td_u32 ck_gt_en;
    td_u32 out_pro;
    td_u32 out_fmt;
    td_u64 in_height;
    td_u64 out_height;
    td_u64 in_width;
    td_u64 out_width;
    td_u32 cvfir_en;
    td_u32 cvmid_en;
    td_u32 cvfir_mode;

    td_u32 hfir_order;
    td_u32 lhfir_en;
    td_u32 chfir_en;
    td_u32 lhmid_en;
    td_u32 chmid_en;
    td_u32 non_lnr_en;
    td_u32 lhfir_mode;
    td_u32 chfir_mode;
    td_u32 hl_shootctrl_en;
    td_u32 hl_shootctrl_mode;
    td_u32 hc_shootctrl_en;
    td_u32 hc_shootctrl_mode;

    td_u32 in_pro;
    td_u32 graphdet_en;

    td_u32 lvfir_en;
    td_u32 lvmid_en;
    td_u32 vfir_1tap_en;
    td_u32 lvfir_mode;
    td_u32 vl_shootctrl_en;
    td_u32 vl_shootctrl_mode;
    td_u32 vc_shootctrl_en;
    td_u32 vc_shootctrl_mode;
} vdp_v1_cvfir_cfg;

typedef struct {
    td_u32 vluma_offset;
    td_u32 vchroma_offset;
    td_u32 vbluma_offset;
    td_u32 vbchroma_offset;
    td_u32 lhfir_offset;
    td_u32 chfir_offset;
    td_u32 vl_flatdect_mode;
    td_u32 vl_coringadj_en;
    td_u32 vl_gain;
    td_u32 vl_coring;
    td_u32 vc_flatdect_mode;
    td_u32 vc_coringadj_en;
    td_u32 vc_gain;
    td_u32 vc_coring;
    td_u32 hl_flatdect_mode;
    td_u32 hl_coringadj_en;
    td_u32 hl_gain;
    td_u32 hl_coring;
    td_u32 hc_flatdect_mode;
    td_u32 hc_coringadj_en;
    td_u32 hc_gain;
    td_u32 hc_coring;
} vo_zme_comm_pq_cfg;

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

typedef enum {
    HAL_DISP_BIT_WIDTH_1 = 0x0,
    HAL_DISP_BIT_WIDTH_2 = 0x1,
    HAL_DISP_BIT_WIDTH_3 = 0x3,
    HAL_DISP_BIT_WIDTH_BUTT
} hal_disp_bit_width;

typedef struct {
    td_u16 bkg_y;
    td_u16 bkg_cb;
    td_u16 bkg_cr;
} hal_video_layer_bkcolor;

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
#endif /* end of HAL_VO_DEF_COMM_H */
