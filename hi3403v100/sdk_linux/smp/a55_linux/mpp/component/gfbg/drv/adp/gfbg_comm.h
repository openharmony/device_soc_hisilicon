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

#ifndef GFBG_COMM_H
#define GFBG_COMM_H

#include "ot_type.h"
#include "gfbg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define gfbg_unequal_eok_return(ret)                       \
    do {                                                   \
        if ((ret) != EOK) {                                \
            gfbg_error("secure function failure\n");       \
            return TD_FAILURE;                             \
        }                                                  \
    } while (0)

#define gfbg_unequal_eok_return_void(ret)                  \
    do {                                                   \
        if ((ret) != EOK) {                                \
            gfbg_error("secure function failure\n");       \
            return;                             \
        }                                                  \
    } while (0)

#define gfbg_unlock_unequal_eok_return(ret, lock, lock_flag)           \
    do {                                                               \
        if ((ret) != EOK) {                                            \
            gfbg_error("secure function failure\n");                   \
            gfbg_spin_unlock_irqrestore(lock, lock_flag);              \
            return TD_FAILURE;                                         \
        }                                                              \
    } while (0)

#define gfbg_unlock_unequal_eok_return_void(ret, lock, lock_flag)      \
    do {                                                               \
        if ((ret) != EOK) {                                            \
            gfbg_error("secure function failure\n");                   \
            gfbg_spin_unlock_irqrestore(lock, lock_flag);              \
            return;                                                    \
        }                                                              \
    } while (0)

typedef struct {
    td_bool is_losslessa;
    td_bool is_lossless;
    td_u32 width;
    ot_fb_color_format format;
} gfbg_stride_attr;

td_void gfbg_recalculate_stride(td_u32 *cmp_stride, td_u32 *uncmp_stride, const gfbg_stride_attr *attr);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* GFBG_COMM_H */
