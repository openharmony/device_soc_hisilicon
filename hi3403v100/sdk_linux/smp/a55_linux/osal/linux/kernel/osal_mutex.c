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

#include "ot_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/mutex.h>
#include <linux/slab.h>

int osal_mutex_init(osal_mutex_t *mutex)
{
    struct mutex *p = NULL;

    if (mutex == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = kmalloc(sizeof(struct mutex), GFP_KERNEL);
    if (p == NULL) {
        osal_trace("%s - kmalloc error!\n", __FUNCTION__);
        return -1;
    }
    mutex_init(p);
    mutex->mutex = p;
    return 0;
}
EXPORT_SYMBOL(osal_mutex_init);

int osal_mutex_lock(osal_mutex_t *mutex)
{
    struct mutex *p = NULL;

    if ((mutex == NULL) || (mutex->mutex == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (struct mutex *)(mutex->mutex);
    mutex_lock(p);
    return 0;
}
EXPORT_SYMBOL(osal_mutex_lock);

int osal_mutex_lock_interruptible(osal_mutex_t *mutex)
{
    struct mutex *p = NULL;

    if ((mutex == NULL) || (mutex->mutex == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (struct mutex *)(mutex->mutex);
    return mutex_lock_interruptible(p);
}
EXPORT_SYMBOL(osal_mutex_lock_interruptible);

int osal_mutex_trylock(osal_mutex_t *mutex)
{
    struct mutex *p = NULL;

    if (mutex == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (struct mutex *)(mutex->mutex);

    return mutex_trylock(p);
}
EXPORT_SYMBOL(osal_mutex_trylock);

void osal_mutex_unlock(osal_mutex_t *mutex)
{
    struct mutex *p = NULL;

    if ((mutex == NULL) || (mutex->mutex == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }

    p = (struct mutex *)(mutex->mutex);

    mutex_unlock(p);
}
EXPORT_SYMBOL(osal_mutex_unlock);

void osal_mutex_destroy(osal_mutex_t *mutex)
{
    struct mutex *p = NULL;

    if ((mutex == NULL) || (mutex->mutex == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }

    p = (struct mutex *)(mutex->mutex);
    kfree(p);
    p = NULL;
    mutex->mutex = NULL;
}
EXPORT_SYMBOL(osal_mutex_destroy);
