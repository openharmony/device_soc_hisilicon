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

#ifndef OT_PROC_H
#define OT_PROC_H

#include "ot_type.h"
#include "osal_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_PROC_CMD_LEN 50
#define MAX_PROC_NAME_LEN 50
#define OT_PROC_BUF_SIZE 65536
#define OT_PROC_WRITE_CMD_SIZE 32

#define OT_PROC_DEVICE_NAME "ot_proc"
#define OT_PROC_PFX "ot_proc: "

#define OT_USER_PROC_READ_CMD "__read"
#define OT_USER_PROC_WRITE_CMD "__write"

typedef struct {
    td_char *buf;
    td_u32 size;
    td_u32 offset;
} ot_proc_show_buf;

typedef struct {
    td_char buf[OT_PROC_WRITE_CMD_SIZE];
    td_u32 count;
    td_s64 ppos;
} ot_proc_write_buf;

typedef struct {
    td_char cmd[MAX_PROC_CMD_LEN];
    osal_proc_entry_t entry;
    ot_proc_write_buf write_buf;
} ot_proc_para;

typedef struct {
    td_char name[MAX_PROC_NAME_LEN];
} ot_proc_name;

#define OT_ID_PROC 'Y'

#define USER_CREATE_PROC_ENTRY           OSAL_IOW(OT_ID_PROC, 1, ot_proc_name)
#define USER_REMOVE_PROC_ENTRY           OSAL_IOW(OT_ID_PROC, 2, ot_proc_name)
#define USER_PROC_GET_CMD                OSAL_IOWR(OT_ID_PROC, 5, ot_proc_para)
#define USER_PROC_WAKE_GET_CMD           OSAL_IOW(OT_ID_PROC, 4, ot_proc_para)
#define USER_PROC_WAKE_READ_TASK         OSAL_IOW(OT_ID_PROC, 6, ot_proc_show_buf)
#define USER_PROC_WAKE_WRITE_TASK        OSAL_IOW(OT_ID_PROC, 7, ot_proc_show_buf)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
