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
#ifndef _OT_MEDIA_BASE_H_
#define _OT_MEDIA_BASE_H_

#include "ot_media.h"

int ot_media_bus_init(void);
void ot_media_bus_exit(void);

int ot_media_device_register(struct ot_media_device *pdev);

void ot_media_device_unregister(struct ot_media_device *pdev);

struct ot_media_driver *ot_media_driver_register(const char *name, struct module *owner);

void ot_media_driver_unregister(struct ot_media_driver *pdrv);

#endif
