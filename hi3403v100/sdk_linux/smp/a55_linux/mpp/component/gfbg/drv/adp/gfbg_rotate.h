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

#ifndef GFBG_ROTATE_H
#define GFBG_ROTATE_H

#include "ot_type.h"
#include "ot_common.h"
#include "vgs_ext.h"
#include "gfbg_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef td_void (*vgs_callback)(ot_mod_id, td_s32, td_s32, struct ot_vgs_job_data *);
typedef td_s32 (*gfbg_rotate)(const ot_fb_buf *, const ot_fb_buf *, const gfbg_rotate_opt *, td_bool);

typedef struct {
    td_bool is_support;
    gfbg_rotate gfbg_drv_rotate;
} gfbg_rotate_ops;

td_void gfbg_rotation_register(td_void);
td_bool gfbg_get_rotation_support(td_void);
gfbg_rotate gfbg_get_rotation(td_void);
td_s32 gfbg_drv_rotate_process(const ot_fb_buf *src_img, const ot_fb_buf *dst_img, const gfbg_rotate_opt *rotate_opt,
    td_bool is_refresh_screen);
td_s32 gfbg_drv_set_vgs_rotate_callback(vgs_callback vgs_rot_callback);
td_s32 gfbg_drv_set_tde_rotate_callback(int_callback tde_rot_callback);
int_callback gfbg_drv_get_tde_rotate_callback(td_void);
vgs_callback gfbg_drv_get_vgs_rotate_callback(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* GFBG_ROTATE_H */
