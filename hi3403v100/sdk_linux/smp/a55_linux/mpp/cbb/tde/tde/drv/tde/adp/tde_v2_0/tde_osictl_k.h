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

#ifndef TDE_OSICTL_K_H
#define TDE_OSICTL_K_H
#include "ot_drv_tde.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TDE_MAX_LINE_NUM 4

typedef struct {
    drv_tde_color_fmt in_fmt;
    drv_tde_rect *in_rect;
    drv_tde_rect *out_rect;
} tde_rect_opt;

typedef struct {
    drv_tde_alpha_blending alpha_blending_cmd;
    drv_tde_rop_mode rop_code_color;
    drv_tde_rop_mode rop_code_alpha;
    td_bool single_sr2_rop;
} tde_rop_opt;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TDE_OSICTL_K_H */