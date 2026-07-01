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

#ifndef DRV_IOCTL_KLAD_H
#define DRV_IOCTL_KLAD_H

#include <asm-generic/ioctl.h>
#include "ot_common_klad.h"
#include "ot_common.h"
#include "mkp_ioctl.h"
#include "dev_ext.h"
#include "osal_ioctl.h"

typedef struct {
    td_handle klad;
} klad_ctl_handle;

typedef struct {
    td_handle klad;
    td_handle target;
} klad_ctl_target;

typedef struct {
    td_handle klad;
    ot_klad_attr attr;
} klad_ctl_attr;

typedef struct {
    td_handle klad;
    ot_klad_session_key key;
} klad_ctl_session_key;

typedef struct {
    td_handle klad;
    ot_klad_content_key key;
} klad_ctl_content_key;

typedef struct {
    td_handle klad;
    ot_klad_clear_key key;
} klad_ctl_clear_key;

typedef enum {
    KLAD_IOC_NR_CREATE = 0,
    KLAD_IOC_NR_DESTROY,
    KLAD_IOC_NR_ATTACH,
    KLAD_IOC_NR_DETACH,
    KLAD_IOC_NR_SET_ATTR,
    KLAD_IOC_NR_GET_ATTR,
    KLAD_IOC_NR_SESSION_KEY,
    KLAD_IOC_NR_CONTENT_KEY,
    KLAD_IOC_NR_CLEAR_KEY,
    KLAD_IOC_NR_BUTT,
} klad_ioc_nr;

#define CMD_KLAD_CREATE_HANDLE      OSAL_IOWR(IOC_TYPE_KLAD, KLAD_IOC_NR_CREATE,      klad_ctl_handle)
#define CMD_KLAD_DESTROY_HANDLE     OSAL_IOW (IOC_TYPE_KLAD, KLAD_IOC_NR_DESTROY,     klad_ctl_handle)
#define CMD_KLAD_ATTACH_TARGET      OSAL_IOW (IOC_TYPE_KLAD, KLAD_IOC_NR_ATTACH,      klad_ctl_target)
#define CMD_KLAD_DETACH_TARGET      OSAL_IOW (IOC_TYPE_KLAD, KLAD_IOC_NR_DETACH,      klad_ctl_target)
#define CMD_KLAD_SET_ATTR           OSAL_IOW (IOC_TYPE_KLAD, KLAD_IOC_NR_SET_ATTR,    klad_ctl_attr)
#define CMD_KLAD_GET_ATTR           OSAL_IOWR(IOC_TYPE_KLAD, KLAD_IOC_NR_GET_ATTR,    klad_ctl_attr)
#define CMD_KLAD_SESSION_KEY        OSAL_IOW (IOC_TYPE_KLAD, KLAD_IOC_NR_SESSION_KEY, klad_ctl_session_key)
#define CMD_KLAD_CONTENT_KEY        OSAL_IOW (IOC_TYPE_KLAD, KLAD_IOC_NR_CONTENT_KEY, klad_ctl_content_key)
#define CMD_KLAD_CLEAR_KEY          OSAL_IOW (IOC_TYPE_KLAD, KLAD_IOC_NR_CLEAR_KEY,   klad_ctl_clear_key)
#define CMD_KLAD_MAX_NUM            KLAD_IOC_NR_BUTT

#endif /* DRV_IOCTL_KLAD_H */
