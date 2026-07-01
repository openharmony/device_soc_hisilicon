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
#ifndef GFBG_INIT_H
#define GFBG_INIT_H
#include <linux/dma-buf.h>
#include "ot_type.h"
#include "gfbg_vou_graphics.h"
#include "gfbg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_void set_video_name(char *temp_video);

#ifndef __LITEOS__

struct dma_buf *gfbg_memblock_export(phys_addr_t base, size_t size, int flags);
#endif
char *gfbg_get_layer_mmz_names(td_u32 layer_id);

int gfbg_pts_init(void);

void gfbg_pts_exit(void);

int gfbg_vsync_init(void);

void gfbg_vsync_exit(void);

int gfbg_vsync_notify(void);

td_void gfbg_cleanup(td_void);

td_s32 gfbg_init(td_void);

#ifndef CONFIG_OT_VO_FB_SEPARATE
td_void set_global_name(void);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
