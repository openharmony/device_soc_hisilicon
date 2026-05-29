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
#ifndef MKP_VO_IOCTL_H
#define MKP_VO_IOCTL_H

#include "ot_type.h"
#include "ot_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define VO_BATCH_CMD_MAX_CNT 8192   /* 一个层批量处理命令的最大值，超过后约束以免一直申请内存 */

typedef struct {
    unsigned int cmd;
    td_uintptr_t arg;
    void *private_data;
} vo_ioctl_para;

typedef struct {
    unsigned int cmd;
    td_char *cmd_name;
} vo_ioctl_cmd;

td_s32 vo_device_ioctl(vo_ioctl_para *ioctl_para);
td_s32 vo_device_compat_ioctl(vo_ioctl_para *ioctl_para);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef MKP_VO_IOCTL_H */

