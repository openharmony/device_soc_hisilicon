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

#ifndef DRV_CIPHER_DEBUG_H
#define DRV_CIPHER_DEBUG_H

#include "ot_common_cipher.h"
#include "ot_debug.h"

#define CIPHER_LEVEL_ERROR      0
#define CIPHER_LEVEL_WARN       1
#define CIPHER_LEVEL_ENTRY      2
#define CIPHER_LEVEL_DBG        3
#define LOG_DEFAULT             CIPHER_LEVEL_WARN


#define cipher_trace(level, lab, pline, fmt, ...)                                               \
    do {                                                                                        \
        if (level <= LOG_DEFAULT) {                                                             \
            if (pline) {                                                                        \
                OT_PRINT(lab "[%s-%d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);            \
            } else {                                                                            \
                OT_PRINT("%s " fmt, __FUNCTION__, ##__VA_ARGS__);                               \
            }                                                                                   \
        }                                                                                       \
    } while (0)

#define ot_cipher_error(fmt, ...)     cipher_trace(CIPHER_LEVEL_ERROR, "ERROR ", TD_TRUE, fmt, ##__VA_ARGS__)
#if (LOG_DEFAULT > CIPHER_LEVEL_WARN)
#define ot_cipher_warn(fmt, ...)      cipher_trace(CIPHER_LEVEL_WARN, "INFO ", TD_TRUE, fmt, ##__VA_ARGS__)
#define ot_cipher_dbg(fmt, ...)       cipher_trace(CIPHER_LEVEL_DBG, "DBG ", TD_TRUE, fmt, ##__VA_ARGS__)
#define func_enter()            cipher_trace(CIPHER_LEVEL_ENTRY, "", TD_FALSE, "function enter\n")
#define func_exit()             cipher_trace(CIPHER_LEVEL_ENTRY, "", TD_FALSE, "function exit\n\n")
#else
#define ot_cipher_warn(fmt, ...)
#define ot_cipher_dbg(fmt, ...)
#define func_enter()
#define func_exit()
#endif

/* Function trace log, strictly prohibited to expand */
#define print_func_errno(func, errno)       ot_cipher_error("Call %s return [0x%08X]\n", #func, (unsigned int)(errno))
#define print_errno(errno)                  ot_cipher_error("Error Code: [0x%08X]\n", (unsigned int)(errno))

#define chk_init_err_return(init_count)                         \
    do {                                                        \
        if ((init_count) == 0) {                                \
            print_errno(OT_ERR_CIPHER_NOT_INIT);                \
            return OT_ERR_CIPHER_NOT_INIT;                      \
        }                                                       \
    } while (0)

#define chk_param_err_return(formula)                           \
    do {                                                        \
        if (formula) {                                          \
            print_errno(OT_ERR_CIPHER_INVALID_PARAM);           \
            return OT_ERR_CIPHER_INVALID_PARAM;                 \
        }                                                       \
    } while (0)

#define chk_ptr_err_return(ptr)                                 \
    do {                                                        \
        if ((ptr) == TD_NULL) {                                 \
            print_errno(OT_ERR_CIPHER_INVALID_POINT);           \
            return OT_ERR_CIPHER_INVALID_POINT;                 \
        }                                                       \
    } while (0)

#define chk_func_err_goto(expr)                                 \
    do {                                                        \
        ret = expr;                                             \
        if (ret != TD_SUCCESS) {                                \
            print_func_errno((expr), ret);                      \
            goto exit__;                                        \
        }                                                       \
    } while (0)

#define chk_func_return_err(func, formula, errno)               \
    do {                                                        \
        if (formula) {                                          \
            print_func_errno(# func, errno);                    \
            return errno;                                       \
        }                                                       \
    } while (0)

#define chk_func_goto_err(func, formula, errno)                 \
    do {                                                        \
        if (formula) {                                          \
            print_func_errno(# func, errno);                    \
            ret = errno;                                        \
            goto goto_exit;                                     \
        }                                                       \
    } while (0)

#define chk_handle_modid(handle, modid)                         \
    do {                                                        \
        if (td_handle_get_modid(handle) != (modid)) {           \
            ot_cipher_error("invalid handle 0x%x\n", handle);         \
            return OT_ERR_CIPHER_INVALID_HANDLE;                \
        }                                                       \
    } while (0)

#define chk_handle_private_data(handle, data)                   \
    do {                                                        \
        if (td_handle_get_private_data(handle) != (data)) {     \
            ot_cipher_error("invalid handle 0x%x\n", handle);         \
            return OT_ERR_CIPHER_INVALID_HANDLE;                \
        }                                                       \
    } while (0)

#define chk_handle_chnid(handle, threshold)                     \
    do {                                                        \
        if (td_handle_get_chnid(handle) >= (threshold)) {       \
            ot_cipher_error("invalid handle 0x%x\n", handle);         \
            return OT_ERR_CIPHER_INVALID_HANDLE;                \
        }                                                       \
    } while (0)

#endif /* DRV_CIPHER_DEBUG_H */
