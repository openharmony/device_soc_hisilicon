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

#ifndef __OSAL_H__
#define __OSAL_H__

void osal_proc_init(void);
void osal_proc_exit(void);
int ot_media_init(void);
void ot_media_exit(void);
int media_mem_init(void);
void media_mem_exit(void);
int mem_check_module_param(void);
void osal_device_init(void);
void osal_device_exit(void);
#endif
