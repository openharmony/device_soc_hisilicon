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

#ifndef __OT_USER_H__
#define __OT_USER_H__

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/pagemap.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <uapi/linux/sched/types.h>

#include "ot_vdec_export.h"
#include "ot_vo_export.h"
#include "ot_ao_export.h"
#include "ot_osal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VDEC_NOTIFY   0
#ifdef __LITEOS__
#define VDEC_SET_SCHEDULER 1
#else
#define VDEC_SET_SCHEDULER 0
#endif

#define AO_NOTIFY     0
#define VO_NOTIFY     0

#ifdef __cplusplus
}
#endif

#endif
