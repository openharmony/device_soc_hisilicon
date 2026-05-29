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

#ifndef MKP_LOG_H
#define MKP_LOG_H

#include "osal_ioctl.h"
#include "mkp_ioctl.h"
#include "ot_common.h"
#include "ot_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    IOC_NR_LOG_SETLOGLEVEL = 0,
    IOC_NR_LOG_GETLOGLEVEL,
    IOC_NR_LOG_CLOSE,
    IOC_NR_LOG_SETWAITFLAG,
} ioc_nr_log;

#define LOG_SETLOGLEVEL_CTRL OSAL_IOW(IOC_TYPE_LOG, IOC_NR_LOG_SETLOGLEVEL, ot_log_level_cfg)
#define LOG_GETLOGLEVEL_CTRL OSAL_IOWR(IOC_TYPE_LOG, IOC_NR_LOG_GETLOGLEVEL, ot_log_level_cfg)
#define LOG_CLOSE_CTRL       OSAL_IO(IOC_TYPE_LOG, IOC_NR_LOG_CLOSE)
#define LOG_SETWAITFLAG_CTRL OSAL_IOW(IOC_TYPE_LOG, IOC_NR_LOG_SETWAITFLAG, td_bool)

#define base_trace_cmpi(level, fmt, ...)                                                                       \
    do {                                                                                                       \
        OT_TRACE(level, OT_ID_CMPI, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)


#define CMPI_LOG_MAX_MMZ_NAME   18
#define LOG_BUFFER_LEN  64 /* unit is KB. the ring buffer's default length is 64KB */
#define LOG_MAX_ITEMLEN 512 /* unit is byte. suppose one log is short than 512 byte */
#define LOG_BUFFER_UNIT_LEN 1024
#define LOG_STATE_INIT  0
#define LOG_STATE_OPEN  1
#define LOG_STATE_READ  2
#define LOG_STATE_WRITE 3
#define LOG_STATE_CLOSE 4
#define LOG_STATE_READING 5

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* MKP_LOG_H */

