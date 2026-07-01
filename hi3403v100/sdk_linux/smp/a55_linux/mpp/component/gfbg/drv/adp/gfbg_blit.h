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

#ifndef GFBG_BLIT_H
#define GFBG_BLIT_H

#include "ot_type.h"
#include "gfbg_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define fb_drv_tde_end_job_val(end_job, thandle, tis_block, ttime_out, tis_sync) \
    do {                                                                         \
        (end_job).handle = thandle;                                              \
        (end_job).is_block = tis_block;                                          \
        (end_job).is_sync = tis_sync;                                            \
        (end_job).time_out = ttime_out;                                          \
    } while (0)

td_bool gfbg_tde_is_support_fmt(ot_fb_color_format format);
td_s32 gfbg_drv_fill(const ot_fb_buf *dst_img, td_u32 fill_data);
td_s32 gfbg_drv_blit(const ot_fb_buf *src_img, const ot_fb_buf *dst_img, const gfbg_blit_opt *opt,
    td_bool is_refresh_screen);
td_s32 gfbg_drv_set_tde_callback(int_callback tde_callback);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* GFBG_BLIT_H */

