/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef _HIMEDIA_BASE_H_
#define _HIMEDIA_BASE_H_

#include "himedia.h"

// bus
int himedia_bus_init(void);
void himedia_bus_exit(void);

// device
int himedia_device_register(pm_basedev *pdev);
void himedia_device_unregister(pm_basedev *pdev);
int himedia_device_add(pm_basedev *pdev);
void himedia_device_del(pm_basedev *pdev);
void himedia_device_put(pm_basedev *pdev);
pm_basedev *himedia_device_alloc(const char *name, int id);

// driver
int himedia_driver_register(pm_basedrv *drv);
void himedia_driver_unregister(pm_basedrv *drv);
void himedia_driver_release(pm_basedrv *drv);
pm_basedrv *himedia_driver_alloc(const char *name, struct module *owner,
    pm_baseops *ops);

#endif
