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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/sched.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0))
#include <uapi/linux/sched/types.h>
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
#include <linux/sched/types.h>
#endif

#include "hi_osal.h"
#include "securec.h"

osal_task *osal_kthread_create(osal_kthread_handler handler, void *data, const char *name, unsigned int stack_size)
{
    struct task_struct *k = NULL;
    errno_t err;

    osal_task *p = (osal_task *)kmalloc(sizeof(osal_task), GFP_KERNEL);
    if (p == NULL) {
        osal_printk("%s - kmalloc error!\n", __FUNCTION__);
        return NULL;
    }

    err = memset_s(p, sizeof(osal_task), 0, sizeof(osal_task));
    if (err != EOK) {
        kfree(p);
        osal_printk("memset_s is failed.\n");
        return NULL;
    }

    k = kthread_run(handler, data, name);
    if (IS_ERR(k)) {
        osal_printk("%s - kthread create error!\n", __FUNCTION__);
        kfree(p);
        return NULL;
    }
    p->task = k;
    return p;
}
EXPORT_SYMBOL(osal_kthread_create);

void osal_kthread_set_priority(osal_task *task, osal_task_priority priority)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0))
    struct sched_param param;

    switch (priority) {
        case OSAL_TASK_PRIORITY_HIGH:
            param.sched_priority = 99; // 99 is priority
            break;
        case OSAL_TASK_PRIORITY_MIDDLE:
            param.sched_priority = 50; // 50 is priority
            break;
        case OSAL_TASK_PRIORITY_LOW:
            param.sched_priority = 10; // 10 is priority
            break;
        default:
            return;
    }

    sched_setscheduler(task->task, SCHED_RR, &param);
#endif
}
EXPORT_SYMBOL(osal_kthread_set_priority);

void osal_kthread_set_affinity(osal_task *task, int cpu_mask)
{
    struct cpumask cpumask_set;

    if (task == NULL) {
        return;
    }

    if (cpu_mask == 0) {
        return;
    }

    cpumask_clear(&cpumask_set);

    ((OSAL_CPU_0 & (unsigned int)cpu_mask) == 0) ?
        0 : cpumask_set_cpu(0, &cpumask_set);

    ((OSAL_CPU_1 & (unsigned int)cpu_mask) == 0) ?
        0 : cpumask_set_cpu(1, &cpumask_set);

    ((OSAL_CPU_2 & (unsigned int)cpu_mask) == 0) ?
        0 : cpumask_set_cpu(2, &cpumask_set); /* cpu2 */

    ((OSAL_CPU_3 & (unsigned int)cpu_mask) == 0) ?
        0 : cpumask_set_cpu(3, &cpumask_set); /* cpu3 */
}
EXPORT_SYMBOL(osal_kthread_set_affinity);

int osal_kthread_should_stop(void)
{
    return kthread_should_stop();
}
EXPORT_SYMBOL(osal_kthread_should_stop);

void osal_kthread_destroy(osal_task *task, unsigned int stop_flag)
{
    if (task == NULL) {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }
    /* note: When you call the Kthread_stop function, the thread function cannot be finished, otherwise it will oops. */
    if (stop_flag != 0) {
        kthread_stop ((struct task_struct *)(task->task));
    }
    task->task = NULL;
    kfree(task);
}
EXPORT_SYMBOL(osal_kthread_destroy);

