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

#ifndef OT_INNER_VIDEO_H
#define OT_INNER_VIDEO_H

#include "ot_type.h"
#include "ot_common_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define DIS_MOTIONDATA_NUM (9)

/*
 * low delay struct, 128 byte
 * line addr to vdp: 64bytes align.
 */
typedef struct {
    td_u16 y_height;
    td_u32 ysize;
    td_u8 reserve1[26];
    td_u16 c_height;
    td_u32 csize;
    td_u8 reserve2[26];

    td_u8 frame_ok;
    td_u8 tunl_frame;
    td_u8 frame_finish;
    td_u8 miss_vedu;
    td_u8 miss_jpege;
    td_u32 buf_line;
    td_u8 all_online;
    td_u8 reserve3[54];
} __attribute__((packed)) low_delay_info;

typedef struct {
    td_s64 motion_data[DIS_MOTIONDATA_NUM];
} dis_inner_motion_data;

typedef struct {
    dis_inner_motion_data data;
    td_bool valid;
} dis_motion_data_info;

typedef struct {
    dis_motion_data_info gme_motion_data;
    dis_motion_data_info fliter_motion_data;
    dis_motion_data_info gyro_motion_data;
} dis_inner_motion_data_info;

typedef struct {
    td_u32 pool_id;
    td_phys_addr_t rnt0_phys_addr;
    td_phys_addr_t rnt1_phys_addr;
    td_u32 width;
    td_u32 height;
    td_u32 stride;
    td_bool valid;
} bnr_mot_info;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

