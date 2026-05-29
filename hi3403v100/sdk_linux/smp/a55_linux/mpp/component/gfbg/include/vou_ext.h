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

#ifndef VOU_EXT_H
#define VOU_EXT_H

#include <ot_common.h>
#include "ot_common_video.h"

typedef enum {
    VO_GFBG_INTTYPE_VO = 0,
    VO_GFBG_INTTYPE_VO_DISP,
    VO_GFBG_INTTYPE_WBC,
    VO_GFBG_INTTYPE_BUTT,
} vo_fb_int_type;

typedef enum {
    VO_PLAY_PLAYING = 0,
    VO_PLAY_PAUSE,
    VO_PLAY_STEP,
    VO_PLAY_BUTT,
} vo_play_type;

typedef struct {
    td_u32 total_repeat_cnt;
    td_u32 predict_repeat_cnt;
    vo_play_type play_type;
} vo_quick_schedule_info;

typedef td_s32 fn_vou_graphics_open_layer(ot_gfx_layer gfx_layer);
typedef td_s32 (*vo_fb_int_call_back)(const td_void *paraml, ot_vo_dev vo_dev, const td_void *paramr);
typedef td_s32 fn_vou_graphics_set_callback(ot_gfx_layer gfx_layer, vo_fb_int_type int_type,
    vo_fb_int_call_back call_back, td_void *arg);
typedef td_s32 fn_vou_query_quick_schedule_info(ot_vo_layer layer, ot_vo_chn chn,
    vo_quick_schedule_info *info);
typedef td_s32 fn_vou_notify_quick_schedule_info(ot_vo_layer layer, ot_vo_chn chn);

typedef struct {
    fn_vou_graphics_open_layer *pfn_vou_graphics_open_layer;
    fn_vou_graphics_set_callback *pfn_vou_graphics_set_callback;
    fn_vou_query_quick_schedule_info *pfn_vou_query_quick_schedule_info;
    fn_vou_notify_quick_schedule_info *pfn_vou_notify_quick_schedule_info;
} vou_export_func;

#define ckfn_vou_entry() check_func_entry(OT_ID_VO)

#define ckfn_vo_query_quick_schedule_info() \
    (func_entry(vou_export_func, OT_ID_VO)->pfn_vou_query_quick_schedule_info != TD_NULL)
#define call_vo_query_quick_schedule_info(layer, chn, info) \
    func_entry(vou_export_func, OT_ID_VO)->pfn_vou_query_quick_schedule_info(layer, chn, info)

#define ckfn_vo_notify_quick_schedule_info() \
    (func_entry(vou_export_func, OT_ID_VO)->pfn_vou_notify_quick_schedule_info != TD_NULL)
#define call_vo_notify_quick_schedule_info(layer, chn) \
    func_entry(vou_export_func, OT_ID_VO)->pfn_vou_notify_quick_schedule_info(layer, chn)

#endif /* VOU_EXT_H */
