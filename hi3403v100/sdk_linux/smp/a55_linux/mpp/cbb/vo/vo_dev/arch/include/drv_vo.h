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

#ifndef DRV_VO_H
#define DRV_VO_H

#include "drv_vo_comm.h"
#include "drv_vo_dev.h"
#include "drv_vo_layer_comm.h"
#include "drv_vo_video.h"
#ifdef CONFIG_OT_VO_CASCADE
#include "drv_vo_cascade.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */
#if vo_desc("UBOOT_VO")

#define VO_SD_VTTH_WATERLINE 100
#define VO_HD_VTTH_WATERLINE 240
#define VO_SD_VTTH_WATERLINE2  2
#define VO_HD_VTTH_WATERLINE2  2
#define VO_LESS_BUF_DEFAULT_VTTH     2
#define VO_USER_NOTIFY_DEFAULT_VTTH  2

/* PLL FREF 24MHz */
#define VO_PLL_FREF 24

/* PLL frac precision */
#define VO_PLL_FRAC_PREC (1 << 24)
#define VO_PLL_FOUTVCO_MIN 800
#define VO_PLL_FOUTVCO_MAX 3200
#define VO_PLL_MAX_CLK_594000 594000
#define VO_PLL_MAX_CLK_297000 297000
#define VO_PLL_MAX_CLK_268500 268500
#define VO_PLL_MAX_CLK_148500 148500

/* LCDMCLKDIV max ((75 / 1188) * 2^27 )(max 75Mhz) */
#define VO_LCDMCLK_DIV_MAX 8473341
#define VO_LCDMCLK_BASE_CLK 1188
#define VO_LCDMCLK_SHIFT 27
#define VO_LCDMCLK_MAX_FREQENCY 75

#define VO_SD_SRC_CLK 54000
#define VO_CLK_MHZ_UNIT 1000
#define VO_SD_DEV_DIV 4

#define VO_DEFAULT_PRE_DIV 1
#define VO_DEFAULT_DEV_DIV 1

/* vou mixer prio id */
typedef enum {
    VOU_MIX_PRIO0 = 0,
    VOU_MIX_PRIO1,
    VOU_MIX_PRIO2,
    VOU_MIX_PRIO3,
    VOU_MIX_PRIO4,
    VOU_MIX_BUTT
} vou_mix_prio;

#define VO_CSC_DEF_VAL      50
#define VO_CSC_LUMA_MAX     100
#define VO_CSC_LUMA_MIN     0
#define VO_CSC_CONT_MAX     100
#define VO_CSC_CONT_MIN     0
#define VO_CSC_HUE_MAX      100
#define VO_CSC_HUE_MIN      0
#define VO_CSC_SAT_MAX      100
#define VO_CSC_SAT_MIN      0

#define VO_VGA_SHARPEN_STRENGTH_MIN  0
#define VO_VGA_SHARPEN_STRENGTH_MAX  0xFF
#define VO_VGA_SHARPEN_STRENGTH_DEF   128
#define VO_VGA_GAIN_MAX                 0x3F

#define VO_DAC_CABLE_CTR_DEF            3

typedef struct {
    vo_dev_type dev_type; /* dev type */
    td_bool support_wbc; /* WBC support or not */
} vo_dev_capability;

typedef struct {
    /* 是否产生了VDAC中断,产生中断代表没有接负载 */
    td_bool int_ocurred;
    td_bool detect_enabled;
    td_bool is_connected;
} vo_drv_load_detect_info;

/* desc : device context, which contains device public attribute. */
typedef struct {
    vo_dev_capability dev_cap; /* 设备能力级 */
    td_bool vo_enable; /* 设备使能标志 */
    td_bool config; /* 设备配置标志 */
    ot_vo_pub_attr vou_attr; /* 设备公共属性 */

    td_u32 layer_num; /* 设备上包含层的数目 */
    td_u32 max_layer_num; /* 该设备上能包含的最多的层数，包括视频层和图形层 */
    ot_vo_layer layer[OT_VO_MAX_PRIORITY + 1]; /* 对应的优先级的层，如aen_layer[0]即该设备上优先级为0的层 */

    td_u32 max_width; /* 设备分辨率最大宽度 */
    td_u32 max_height; /* 设备分辨率最大高度 */

    td_bool less_buf_enable;   /* 省BUF的标记 */
    td_bool user_notify_enable;   /* 通知用户的标记 */
    td_u32 vtth;               /* 提前完成中断垂直时序值 */
    td_u32 vtth2;              /* 省BUF垂直时序值 */
    td_bool bt_param_config;   /* bt_param配置标志，用于单双沿配置 */
    ot_vo_clk_edge clk_edge;   /* 单双沿配置,默认单沿 */

    td_bool dac_power_up;
    vo_drv_load_detect_info load_detect_info; /* 负载检测 */
    td_u32 low_bandwidth_cnt; /* 统计VO低带宽信息 */
    td_u64 bus_err; /* 统计VO bus err */
    td_u32 low_delay_err; /* 统计低延时错误 */

    ot_vo_dev_param dev_param;
} vo_drv_dev;

/* layer structure */
typedef struct {
    td_bool video_enable; /* 视频层使能标志 */
    td_bool video_config; /* 视频层属性配置标志 */

    /* HD设备有效 */
    td_s32 bind_dev; /* 绑定的设备号 */
    td_bool enlarge; /* 是否可以放大 */

    td_u32 chn_num; /* 层最大可以使能的通道数目 */
    ot_vo_video_layer_attr vo_video_attr; /* 视频层属性 */

    ot_vo_csc csc; /* CSC属性 */
    csc_coef_param csc_param; /* 额外的CSC参数 */
    ot_size wbc_in_size;
    ot_crop_info crop_info;
} vo_drv_layer;

#if vo_desc("ctx")
vo_drv_dev *vo_drv_get_dev_ctx(ot_vo_dev vo_dev);
vo_drv_layer *vo_drv_get_layer_ctx(ot_vo_layer layer);
#endif

#if vo_desc("pub")
td_void vou_drv_def_layer_bind_dev(td_void);
td_void vo_drv_default_setting(td_void);
#endif

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#define VOU_INTCLEAR_ALL      0xffffffff

/* one region has 0x30 bytes(0x12000 ~ 0x12028), one region has 16( = 0x30 / 4) registers */
#define REGION_OFFSET         (0x30 / 4)

#if vo_desc("ctx")
vo_coef_addr *vo_drv_get_coef_buf_addr(td_void);
#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of DRV_VO_H */
