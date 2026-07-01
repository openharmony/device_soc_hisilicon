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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

int osal_platform_driver_register(void *drv)
{
    return __platform_driver_register((struct platform_driver *)drv, THIS_MODULE);
}
EXPORT_SYMBOL(osal_platform_driver_register);

void osal_platform_driver_unregister(void *drv)
{
    return platform_driver_unregister((struct platform_driver *)drv);
}
EXPORT_SYMBOL(osal_platform_driver_unregister);

void *osal_platform_get_resource_byname(void *dev, unsigned int type,
                                        const char *name)
{
    return (void *)platform_get_resource_byname((struct platform_device *)dev, type, name);
}
EXPORT_SYMBOL(osal_platform_get_resource_byname);

void *osal_platform_get_resource(void *dev, unsigned int type, unsigned int num)
{
    return (void *)platform_get_resource((struct platform_device *)dev, type, num);
}
EXPORT_SYMBOL(osal_platform_get_resource);

int osal_platform_get_irq(void *dev, unsigned int num)
{
    return platform_get_irq((struct platform_device *)dev, num);
}
EXPORT_SYMBOL(osal_platform_get_irq);

int osal_platform_get_irq_byname(void *dev, const char *name)
{
    return platform_get_irq_byname((struct platform_device *)dev, name);
}
EXPORT_SYMBOL(osal_platform_get_irq_byname);

