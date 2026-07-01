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

#ifndef COMM_EXT_H
#define COMM_EXT_H

#include "ot_defines.h"
#include "ot_common_video.h"

/*
 * 有些模块支持场图像转帧图像，拷贝后field需适配
 * 支持HDR的模块可能会修改color_gamut和dynamic_range，拷贝后dynamic_range需适配
 * 各模块紧凑段压缩未开mirror/flip时标志需清空
 */
__inline static td_void comm_copy_frame_param(ot_video_frame *dst_v_frame, const ot_video_frame *src_v_frame)
{
    td_u32 i;

    dst_v_frame->field = src_v_frame->field;
    dst_v_frame->dynamic_range = src_v_frame->dynamic_range;
    dst_v_frame->color_gamut = src_v_frame->color_gamut;
    dst_v_frame->time_ref = src_v_frame->time_ref;
    dst_v_frame->pts = src_v_frame->pts;
    dst_v_frame->frame_flag = src_v_frame->frame_flag;
    for (i = 0; i < OT_MAX_USER_DATA_NUM; i++) {
        dst_v_frame->user_data[i] = src_v_frame->user_data[i];
    }
}

#endif /* COMM_EXT_H */

