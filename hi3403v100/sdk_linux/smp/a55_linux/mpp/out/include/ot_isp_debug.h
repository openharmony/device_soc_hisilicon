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

#ifndef OT_ISP_DEBUG_H
#define OT_ISP_DEBUG_H

#include "ot_debug.h"

#define isp_emerg_trace(fmt, ...) \
    OT_EMERG_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define isp_alert_trace(fmt, ...)  \
    OT_ALERT_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define isp_crit_trace(fmt, ...) \
    OT_CRIT_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define isp_err_trace(fmt, ...) \
    OT_ERR_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define isp_warn_trace(fmt, ...) \
    OT_WARN_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define isp_notice_trace(fmt, ...) \
    OT_NOTICE_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define isp_info_trace(fmt, ...) \
    OT_INFO_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define isp_debug_trace(fmt, ...) \
    OT_DEBUG_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

/* To avoid divide-0 exception in code. */
#define div_0_to_1(a)   (((a) == 0) ? 1 : (a))
#define div_0_to_1_float(a) ((((a) < 1E-10) && ((a) > (-1E-10))) ? 1 : (a))

#endif     /* OT_ISP_DEBUG_H */
