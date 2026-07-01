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

#ifndef OT_INNER_SYS_H
#define OT_INNER_SYS_H

#include "ot_common.h"
#include "ot_common_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define OT_PAGE_SHIFT      12
#define OT_PAGE_SIZE       (1UL << OT_PAGE_SHIFT)
#define OT_PAGE_MASK       (~(OT_PAGE_SIZE - 1))

typedef enum {
    COMPRESS_LOSSLESS = 0,
    COMPRESS_LOSSY,
    COMPRESS_BUTT,
} compress_strategy;

typedef struct {
    compress_strategy comp_strategy; /* RW; compress strategy */
    td_s32 type;
    td_u32 comp_ratio_8_bit; /* RW, range[0, 100]; lossy compression ratio of 8 bit    */
    td_u32 comp_ratio_10_bit; /* RW, range[0, 100]; lossy compression ratio  of 10 bit */
} compress_parameter;

typedef struct {
    compress_parameter luma_comp_param;
    compress_parameter chroma_comp_param;
} yuv_compress_param;

typedef struct {
    compress_strategy comp_strategy; /* RW; compress strategy */
    td_s32 type;
    td_u32 comp_ratio_8_bit; /* RW, range[0, 100]; lossy compression ratio of 8 bit    */
    td_u32 comp_ratio_10_bit; /* RW, range[0, 100]; lossy compression ratio of 10 bit */
    td_u32 comp_ratio_12_bit; /* RW, range[0, 100]; lossy compression ratio of 12 bit */
    td_u32 comp_ratio_14_bit; /* RW, range[0, 100]; lossy compression ratio of 14 bit */
    td_u32 comp_ratio_16_bit; /* RW, range[0, 100]; lossy compression ratio of 16 bit */
} raw_compress_param;

typedef struct {
    compress_strategy comp_strategy; /* RW; compress strategy */
    td_u32 comp_ratio;
} bnr_compress_param;

typedef struct {
    ot_op_mode operation_mode;
    raw_compress_param raw_line_comp_param;
    raw_compress_param raw_frame_comp_param;
    bnr_compress_param bnr_comp_param;
    yuv_compress_param vi_comp_param;
    yuv_compress_param vpss_comp_param;
    yuv_compress_param vgs_comp_param;
    yuv_compress_param vo_comp_param;
    yuv_compress_param vpss_tile_comp_param;
    yuv_compress_param nr_comp_param;
#ifdef CONFIG_OT_GDC_SUPPORT
    yuv_compress_param gdc_comp_param;
#endif
#ifdef CONFIG_OT_AVS_SUPPORT
    yuv_compress_param avs_comp_param;
#endif
} sys_compress_v2_param;

td_s32 mpi_sys_get_hr_timer(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* OT_INNER_SYS_H */
