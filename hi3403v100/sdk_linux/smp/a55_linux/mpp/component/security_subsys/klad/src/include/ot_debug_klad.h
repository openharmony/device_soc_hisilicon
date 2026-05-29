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

#ifndef OT_DEBUG_KLAD_H
#define OT_DEBUG_KLAD_H

#include "ot_type.h"
#include "ot_debug.h"

#define OT_KLAD_LVL_ERR     0
#define OT_KLAD_LVL_WARN    1
#define OT_KLAD_LVL_INFO    2
#define OT_KLAD_LVL_LOG     3

#define klad_trace(level, msg, fmt, ...)                                                                \
        do {                                                                                            \
            if (OT_KLAD_DEBUG >= level) {                                                               \
                OT_PRINT("[%4s Line:%04d Func:%s] "fmt, msg, __LINE__, __FUNCTION__, ##__VA_ARGS__);    \
            }                                                                                           \
        } while (0)

#define ot_klad_error(fmt, ...)      klad_trace(OT_KLAD_LVL_ERR, "ERR", fmt, ##__VA_ARGS__)
#if OT_KLAD_DEBUG
#define ot_klad_warn(fmt, ...)       klad_trace(OT_KLAD_LVL_WARN, "WARN", fmt, ##__VA_ARGS__)
#define ot_klad_info(fmt, ...)       klad_trace(OT_KLAD_LVL_INFO, "INFO", fmt, ##__VA_ARGS__)
#define ot_klad_enter()              klad_trace(OT_KLAD_LVL_LOG, "LOG", "function enter\n")
#define ot_klad_exit()               klad_trace(OT_KLAD_LVL_LOG, "LOG", "function exit\n")
#else
#define ot_klad_warn(fmt, ...)
#define ot_klad_info(fmt, ...)
#define ot_klad_enter()
#define ot_klad_exit()
#endif

#define ot_klad_func_fail_return(_func, _formula, _errno) \
    do { \
        if (_formula) { \
            ot_klad_error("call %s failed, ret = 0x%08X\n", # _func, _errno); \
            return _errno; \
        } \
    } while (0)

#define ot_klad_func_fail_goto(_func, _formula, _errno, _label) \
    do { \
        if (_formula) { \
            ot_klad_error("call %s failed, ret = 0x%08X\n", # _func, _errno); \
            goto _label; \
        } \
    } while (0)

#define ot_klad_formula_fail_return(_formula, _errno) \
    do { \
        if (_formula) { \
            ot_klad_error("%s is invalid, ret = 0x%08X\n", # _formula, _errno); \
            return _errno; \
        } \
    } while (0)

#define chk_handle_modid(handle, modid)                         \
    do {                                                        \
        if (td_handle_get_modid(handle) != (modid)) {           \
            ot_klad_error("invalid handle 0x%x\n", handle);     \
            return OT_ERR_KLAD_INVALID_HANDLE;                  \
        }                                                       \
    } while (0)

#define chk_handle_private_data(handle, data)                   \
    do {                                                        \
        if (td_handle_get_private_data(handle) != (data)) {     \
            ot_klad_error("invalid handle 0x%x\n", handle);     \
            return OT_ERR_KLAD_INVALID_HANDLE;                  \
        }                                                       \
    } while (0)

#define chk_handle_chnid(handle, threshold)                     \
    do {                                                        \
        if (td_handle_get_chnid(handle) >= (threshold)) {       \
            ot_klad_error("invalid handle 0x%x\n", handle);     \
            return OT_ERR_KLAD_INVALID_HANDLE;                  \
        }                                                       \
    } while (0)

#endif /* OT_DEBUG_KLAD_H */
