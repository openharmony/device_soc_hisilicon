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

#ifndef HAL_VO_DEF_H
#define HAL_VO_DEF_H

#include "ot_type.h"
#include "ot_defines.h"
#include "hal_vo_def_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

#define VHD_REGS_LEN        0x1000 /* len of V0's regs */
#define GFX_REGS_LEN        0x800
#define DHD_REGS_LEN        0x1000
#define VID_REGS_LEN        0x200 /* len of VID regs */
#define GRF_REGS_LEN        0x200 /* len of GFX regs */

/* offset define */
/* 0x200 bytes, 0x200/4 regs */
#define FDR_VID_OFFSET      (0x200 / 4)

#define ZME_HPREC           (1 << 20)
#define ZME_VPREC           (1 << 12)

#define VO_INPUT_BIT_WIDTH_10  10

#define VO_OUTPUT_BIT_WIDTH_10 10
#define VO_OUTPUT_BIT_WIDTH_6  6

typedef enum {
    VO_DEV_DHD0 = 0, /* ultra high definition device */
    VO_DEV_DHD1 = 1, /* high definition device */
    VO_DEV_BUTT
} vo_hal_dev;

typedef enum {
    VO_HAL_LAYER_VHD0 = 0, /* V0 layer */
    VO_HAL_LAYER_VHD1 = 1, /* V1 layer */
    VO_HAL_LAYER_VHD2 = 2, /* V2 layer */

    VO_HAL_LAYER_BUTT
} vo_hal_layer;

typedef enum {
    VO_SW_LAYER_VHD0 = 0,
    VO_SW_LAYER_VHD1 = 1,
    VO_SW_LAYER_VHD2 = 2,

    VO_SW_LAYER_VIRT0 = ot_vo_get_virt_layer(OT_VO_VIRT_DEV_0),
    VO_SW_LAYER_VIRT1 = ot_vo_get_virt_layer(OT_VO_VIRT_DEV_1),
    VO_SW_LAYER_VIRT2 = ot_vo_get_virt_layer(OT_VO_VIRT_DEV_2),
    VO_SW_LAYER_VIRT3 = ot_vo_get_virt_layer(OT_VO_VIRT_DEV_3),
    VO_SW_LAYER_VIRT31 = (VO_SW_LAYER_VIRT0 + OT_VO_MAX_VIRT_DEV_NUM),

    VOU_SW_LAYER_BUTT
} vo_sw_layer;

typedef enum {
    HAL_DISP_LAYER_VHD0 = 0,
    HAL_DISP_LAYER_VHD1 = 1,
    HAL_DISP_LAYER_VHD2 = 2,

    HAL_DISP_LAYER_GFX0 = 3,
    HAL_DISP_LAYER_GFX1 = 4,
    HAL_DISP_LAYER_GFX3 = 5,
    HAL_DISP_LAYER_BUTT,
} hal_disp_layer;

#define DEV_PHY_START       VO_DEV_DHD0
#define DEV_PHY_END         VO_DEV_DHD1

#define LAYER_VID_START     HAL_DISP_LAYER_VHD0  /* VHD0 */
#define LAYER_VID_END       HAL_DISP_LAYER_VHD2  /* VHD2 */
#define LAYER_PHY_END       VO_SW_LAYER_VHD2

#define LAYER_GFX_START     HAL_DISP_LAYER_GFX0  /* GFX0 */
#define LAYER_GFX_END       HAL_DISP_LAYER_GFX3  /* GFX3 */

typedef struct {
    td_u32 dither_sed_y0;
    td_u32 dither_sed_u0;
    td_u32 dither_sed_v0;
    td_u32 dither_sed_w0;

    td_u32 dither_sed_y1;
    td_u32 dither_sed_u1;
    td_u32 dither_sed_v1;
    td_u32 dither_sed_w1;

    td_u32 dither_sed_y2;
    td_u32 dither_sed_u2;
    td_u32 dither_sed_v2;
    td_u32 dither_sed_w2;

    td_u32 dither_sed_y3;
    td_u32 dither_sed_u3;
    td_u32 dither_sed_v3;
    td_u32 dither_sed_w3;
} vo_dihter_sed;

typedef enum {
    DITHER_IO_MODE_12_10 = 1,
    DITHER_IO_MODE_12_8 = 2, /* for rgb24bit */
    DITHER_IO_MODE_10_8 = 3,
    DITHER_IO_MODE_10_6 = 4, /* for rgb16bit, rgb18bit */
    DITHER_IO_MODE_BUTT
} dither_io_mode;

typedef enum {
    DITHER_MODE_10BIT = 0,
    DITHER_MODE_8BIT = 1,
    DITHER_MODE_BUTT
} dither_mode;

typedef enum {
    DITHER_OWIDTH_MODE_5BIT = 0,
    DITHER_OWIDTH_MODE_6BIT = 1,
    DITHER_OWIDTH_MODE_7BIT = 2,
    DITHER_OWIDTH_MODE_8BIT = 3,
    DITHER_OWIDTH_MODE_9BIT = 4,
    DITHER_OWIDTH_MODE_10BIT = 5,
    DITHER_OWIDTH_MODE_BUTT
} dither_owidth_mode;

typedef enum {
    DITHER_IWIDTH_MODE_8BIT = 0,
    DITHER_IWIDTH_MODE_9BIT = 1,
    DITHER_IWIDTH_MODE_10BIT = 2,
    DITHER_IWIDTH_MODE_11BIT = 3,
    DITHER_IWIDTH_MODE_12BIT = 4,
    DITHER_IWIDTH_MODE_BUTT
} dither_iwidth_mode;

typedef struct {
    dither_io_mode io_mode;

    td_u32 dither_en;
    td_u32 dither_mode;
    td_u32 dither_round;
    td_u32 dither_round_unlim;
    td_u32 i_data_width_dither;
    td_u32 o_data_width_dither;
    td_u32 dither_domain_mode;
    td_u32 dither_tap_mode;
    vo_dihter_sed dither_sed;
    td_u32 dither_thr_max;
    td_u32 dither_thr_min;
} vdp_dither_cfg;

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#define MAX_REGION_NUM        64
#define V0_REGION_NUM         64
#define V1_REGION_NUM         1
#define V2_REGION_NUM         1
#define VIRT_LAYER_REGION_NUM 64

/* 提前6ms（预估值）上报VTTH */
#define VO_DEAULT_VTTH_TIME 6

#define MRG_REGS_LEN        0xc00 /* len of v0 mrg regs */

#define MRG_OFFSET_ADDR     0x12000

/* for CMP and DCMP */
#define CMP_SEG_OFFSET  (0x80 / 4)
#define DCMP_SEG_OFFSET (0x20 / 4)

#define VO_DEV_DSD_START   VO_DEV_DSD0
#define VO_DEV_DSD_END     VO_DEV_DSD0

#define VO_DEV_DHD_START   VO_DEV_DHD0
#define VO_DEV_DHD_END     VO_DEV_DHD1

typedef enum {
    VO_COEF_BUF_REGION_V0 = 8,
    VO_COEF_BUF_REGION_V1 = 9,

    VO_COEF_BUF_BUTT = 10
} vo_coef_buf;

#define VO_ALL_COEF_SIZE      COEF_SIZE_REGION_V0
#define COEF_SIZE_REGION_V0   (192 * 128 / 8)

#define LAYER_MRG_START     HAL_DISP_LAYER_VHD0
#define LAYER_MRG_END       HAL_DISP_LAYER_VHD1

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
#endif /* end of HAL_VO_DEF_H */
