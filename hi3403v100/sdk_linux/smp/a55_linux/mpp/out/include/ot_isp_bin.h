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

#ifndef OT_ISP_BIN_H
#define OT_ISP_BIN_H

#include "ot_type.h"
#include "ot_common_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* MACRO DEFINITION */
#define MAX_BIN_REG_NUM 5

/* GENERAL STRUCTURES */
typedef struct {
    td_u32  addr;    /* register addr */
    td_u8   start_bit; /* start bit of register addr */
    td_u8   end_bit;   /* end bit of register addr */
} ot_isp_bin_reg_attr;

/*
 * The base addr of ISP logic register
 * The base addr of ISP ext register
 * The base addr of AE ext register
 * The base addr of AWB ext register
 */
ot_isp_bin_reg_attr g_isp_bin_reg_attr[OT_ISP_MAX_PIPE_NUM][MAX_BIN_REG_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = {0}
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_ISP_BIN_H */
