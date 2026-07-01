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
#include <asm/atomic.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include "securec.h"

int osal_atomic_init(osal_atomic_t *atomic)
{
    atomic_t *p = NULL;

    if (atomic == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (atomic_t *)kmalloc(sizeof(atomic_t), GFP_KERNEL);
    if (p == NULL) {
        osal_trace("%s - kmalloc error!\n", __FUNCTION__);
        return -1;
    }
    (void)memset_s(p, sizeof(atomic_t), 0, sizeof(atomic_t));
    atomic->atomic = p;
    return 0;
}
EXPORT_SYMBOL(osal_atomic_init);
void osal_atomic_destroy(osal_atomic_t *atomic)
{
    if (atomic == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }
    kfree(atomic->atomic);
    atomic->atomic = NULL;
}
EXPORT_SYMBOL(osal_atomic_destroy);
int osal_atomic_read(osal_atomic_t *atomic)
{
    atomic_t *p = NULL;

    if ((atomic == NULL) || (atomic->atomic == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (atomic_t *)(atomic->atomic);
    return atomic_read(p);
}
EXPORT_SYMBOL(osal_atomic_read);
void osal_atomic_set(osal_atomic_t *atomic, int i)
{
    atomic_t *p = NULL;

    if ((atomic == NULL) || (atomic->atomic == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }

    p = (atomic_t *)(atomic->atomic);
    atomic_set(p, i);
}
EXPORT_SYMBOL(osal_atomic_set);
int osal_atomic_inc_return(osal_atomic_t *atomic)
{
    atomic_t *p = NULL;

    if ((atomic == NULL) || (atomic->atomic == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (atomic_t *)(atomic->atomic);
    return atomic_inc_return(p);
}
EXPORT_SYMBOL(osal_atomic_inc_return);
int osal_atomic_dec_return(osal_atomic_t *atomic)
{
    atomic_t *p = NULL;

    if ((atomic == NULL) || (atomic->atomic == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (atomic_t *)(atomic->atomic);
    return atomic_dec_return(p);
}
EXPORT_SYMBOL(osal_atomic_dec_return);

int osal_atomic_fetch_and(int i, osal_atomic_t *atomic)
{
    atomic_t *p = NULL;

    if ((atomic == NULL) || (atomic->atomic == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (atomic_t *)(atomic->atomic);
    return atomic_fetch_and(i, p);
}
EXPORT_SYMBOL(osal_atomic_fetch_and);

int osal_atomic_fetch_or(int i, osal_atomic_t *atomic)
{
    atomic_t *p = NULL;

    if ((atomic == NULL) || (atomic->atomic == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (atomic_t *)(atomic->atomic);
    return atomic_fetch_or(i, p);
}
EXPORT_SYMBOL(osal_atomic_fetch_or);

int osal_atomic64_init(osal_atomic64_t *atomic)
{
    atomic64_t *p = NULL;

    if (atomic == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (atomic64_t *)kmalloc(sizeof(atomic64_t), GFP_KERNEL);
    if (p == NULL) {
        osal_trace("%s - kmalloc error!\n", __FUNCTION__);
        return -1;
    }
    (void)memset_s(p, sizeof(atomic64_t), 0, sizeof(atomic64_t));
    atomic->atomic64 = p;
    return 0;
}
EXPORT_SYMBOL(osal_atomic64_init);

void osal_atomic64_destroy(osal_atomic64_t *atomic)
{
    if (atomic == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }
    kfree(atomic->atomic64);
    atomic->atomic64 = NULL;
}
EXPORT_SYMBOL(osal_atomic64_destroy);

long long osal_atomic64_read(osal_atomic64_t *atomic)
{
    atomic64_t *p = NULL;

    if ((atomic == NULL) || (atomic->atomic64 == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (atomic64_t *)(atomic->atomic64);
    return atomic64_read(p);
}
EXPORT_SYMBOL(osal_atomic64_read);

void osal_atomic64_set(osal_atomic64_t *atomic, long long i)
{
    atomic64_t *p = NULL;

    if ((atomic == NULL) || (atomic->atomic64 == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }

    p = (atomic64_t *)(atomic->atomic64);
    atomic64_set(p, i);
}
EXPORT_SYMBOL(osal_atomic64_set);

long long osal_atomic64_fetch_and(long long i, osal_atomic64_t *atomic)
{
    atomic64_t *p = NULL;

    if ((atomic == NULL) || (atomic->atomic64 == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (atomic64_t *)(atomic->atomic64);
    return atomic64_fetch_and(i, p);
}
EXPORT_SYMBOL(osal_atomic64_fetch_and);

long long osal_atomic64_fetch_or(long long i, osal_atomic64_t *atomic)
{
    atomic64_t *p = NULL;

    if ((atomic == NULL) || (atomic->atomic64 == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (atomic64_t *)(atomic->atomic64);
    return atomic64_fetch_or(i, p);
}
EXPORT_SYMBOL(osal_atomic64_fetch_or);
