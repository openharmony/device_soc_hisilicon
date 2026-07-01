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

#ifndef VB_SUPPLEMENT_H
#define VB_SUPPLEMENT_H

#include "ot_common_vb.h"
#include "ot_common_video.h"
#include "ot_inner_video.h"

#ifdef CONFIG_OT_VB_SUPPLEMENT_MASK_SUPPORT
td_s32 vb_check_supplement(const ot_vb_supplement_cfg *conf);
#endif

td_u32 vb_get_supplement_size(const ot_vb_supplement_cfg *conf, td_bool cached);
td_void vb_value_supplement_cached_addr(td_phys_addr_t phy, td_void *vir,
    ot_video_supplement *supplement, const ot_vb_supplement_cfg *conf);

td_void vb_value_supplement_no_cache_addr(td_phys_addr_t phy, td_void *vir,
    ot_video_supplement *supplement, const ot_vb_supplement_cfg *conf);
#endif
