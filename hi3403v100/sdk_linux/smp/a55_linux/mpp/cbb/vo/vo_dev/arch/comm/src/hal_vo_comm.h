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

#ifndef HAL_VO_COMM_H
#define HAL_VO_COMM_H

#include "hal_vo_def.h"
#include "inner_vo.h"
#ifdef OT_DEBUG
#include "ot_osal.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

#ifdef OT_DEBUG
#define VO_HAL_CHECK_DEBUG /* vo hal check debug, debug only */
#endif

#define REG_BYTES 4   /* 1reg 4bytes */

#if vo_desc("hal pub")
td_void hal_vo_init(td_void);
td_void hal_write_reg(td_u32 *address, td_u32 value);
td_u32 hal_read_reg(const td_u32 *address);
#endif

#if vo_desc("hal check")
td_bool vo_hal_is_phy_dev(ot_vo_dev dev);
td_bool vo_hal_is_video_layer(hal_disp_layer layer);
td_bool vo_hal_is_gfx_layer(hal_disp_layer layer);
td_bool vo_hal_is_video_gfx_layer(hal_disp_layer layer);
#endif

#ifdef VO_HAL_CHECK_DEBUG

#define vo_hal_check_phy_dev_return(dev)                              \
    do {                                                              \
        if (vo_hal_is_phy_dev(dev) != TD_TRUE) {                      \
            OT_PRINT("err dev in %s: L%d\n", __FUNCTION__, __LINE__); \
            return;                                                   \
        }                                                             \
    } while (0)

#define vo_hal_check_video_layer_return(layer)                          \
    do {                                                                \
        if (vo_hal_is_video_layer(layer) != TD_TRUE) {                  \
            OT_PRINT("err layer in %s: L%d\n", __FUNCTION__, __LINE__); \
            return;                                                     \
        }                                                               \
    } while (0)

#define vo_hal_check_gfx_layer_return(layer)                            \
    do {                                                                \
        if (vo_hal_is_gfx_layer(layer) != TD_TRUE) {                    \
            OT_PRINT("err layer in %s: L%d\n", __FUNCTION__, __LINE__); \
            return;                                                     \
        }                                                               \
    } while (0)

#define vo_hal_check_video_gfx_layer_return(layer)                      \
    do {                                                                \
        if (vo_hal_is_video_gfx_layer(layer) != TD_TRUE) {              \
            OT_PRINT("err layer in %s: L%d\n", __FUNCTION__, __LINE__); \
            return;                                                     \
        }                                                               \
    } while (0)

#else

#define vo_hal_check_phy_dev_return(dev)
#define vo_hal_check_video_layer_return(layer)
#define vo_hal_check_gfx_layer_return(layer)
#define vo_hal_check_video_gfx_layer_return(layer)

#endif

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

typedef enum {
    HAL_VO_DCMP_TYPE_BS_ERR,
    HAL_VO_DCMP_TYPE_CONSUME,
    HAL_VO_DCMP_TYPE_FORGIVE,
    HAL_VO_DCMP_TYPE_BUTT,
} hal_vo_dcmp_type;

td_void hal_vo_deinit(td_void);

#ifdef VO_HAL_CHECK_DEBUG

td_bool vo_hal_is_mrg_video_layer(hal_disp_layer layer);

#define vo_hal_check_mrg_video_layer_return(layer)                      \
    do {                                                                \
        if (vo_hal_is_mrg_video_layer(layer) != TD_TRUE) {              \
            OT_PRINT("err layer in %s: L%d\n", __FUNCTION__, __LINE__); \
            return;                                                     \
        }                                                               \
    } while (0)

#else

#define vo_hal_check_mrg_video_layer_return(layer)

#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of HAL_VO_COMM_H */
