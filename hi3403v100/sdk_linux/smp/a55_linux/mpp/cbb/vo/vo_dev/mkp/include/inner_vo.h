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

#ifndef INNER_VO_H
#define INNER_VO_H

#include "ot_common_video.h"
#include "ot_common_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define vo_desc(x)  1

/* cut from "mkp_vo.h" */
typedef struct {
    td_u32 rgn_num; /* W; count of the rgn */
    ot_rect ATTRIBUTE *rgn;  /* W; rgn attribute */
    /*
     * time parameter.less than 0 means waiting until get the luma data,
     * equal to 0 means get the luma data no matter whether it can or not,
     * more than 0 means waiting how long the time parameter it is
     */
    td_s32 milli_sec;

    td_u64 *ATTRIBUTE luma_data; /* luma data of the region */
} vo_region_luma;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* INNER_VO_H */
