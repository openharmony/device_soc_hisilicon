/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
