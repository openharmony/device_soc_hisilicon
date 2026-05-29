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

#ifndef VOU_H
#define VOU_H

#include "ot_defines.h"
#include "ot_debug.h"
#include "drv_vo.h"
#include "ot_common.h"
#include "ot_math.h"
#include "ot_vo_export.h"
#include "ot_inner_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define VO_PRIORITY_MAX                     OT_VO_MAX_CHN_NUM /* priority max value on device */
#define VO_CHN_PRIORITY_MAX                 OT_VO_MAX_CHN_NUM

/* the align format of memory assign */
#define VO_RECT_ALIGN                       2

#define VO_QUERY_INTF_STATUS_WAIT_TIMEOUT  2000
#define VO_TIME_ONE_SECOND_UNIT_MICROSECOND         1000000
#define VO_TIME_TEN_SECONDS_UNIT_MICROSECOND        10000000ul

/* pts control structure */
typedef struct {
    td_u32 pts_span; /* 两帧之间的时间差，即gap值 */
    td_u32 toleration; /* toleration for pts error */
} vo_pts_contrl;

td_void vo_exit_dev(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifdef __VOU_H__ */
