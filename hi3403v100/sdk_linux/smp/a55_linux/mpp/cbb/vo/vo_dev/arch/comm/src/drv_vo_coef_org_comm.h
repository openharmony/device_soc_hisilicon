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

#ifndef DRV_VO_COEF_ORG_COMM_H
#define DRV_VO_COEF_ORG_COMM_H

#include "drv_vo_coef_comm.h"
#include "hal_vo_def.h"
#include "ot_common_vo.h"
#include "inner_vo.h"

#if vo_desc("UBOOT_VO")
const csc_coef *vo_get_csc_coef(ot_vo_csc_matrix csc_matrix);
const td_s32 *vo_get_sin_table(td_void);
const td_s32 *vo_get_cos_table(td_void);
#endif /* #if vo_desc("UBOOT_VO") */

#endif
