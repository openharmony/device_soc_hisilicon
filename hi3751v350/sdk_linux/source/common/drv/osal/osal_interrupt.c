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

#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/slab.h>
#include "securec.h"

static OSAL_LIST_HEAD(g_irq_list);

typedef struct irq_info_ {
    int irq;
    int __irq;
    struct osal_list_head node;
}irq_info;

irq_info *osal_irq_find_node(int irq, struct osal_list_head *list)
{
    irq_info *irq_node = NULL;

    osal_list_for_each_entry(irq_node, list, node) {
        if (irq_node->irq == irq) {
            return irq_node;
        }
    }

    return NULL;
}

extern int hi_get_irq_byname(char *name);
int osal_irq_request(unsigned int irq, osal_irq_handler handler, osal_irq_handler thread_fn, const char *name,
                     void *dev)
{
    int __irq = irq;
    unsigned long flags = IRQF_SHARED;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
    irq_info *irq_node = NULL;
#endif

    if (name == NULL) {
        printk("name is NULL ! \n");
        return 0;
    }

    if (dev == NULL) {
        flags = 0;
    }

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
    irq_node = osal_irq_find_node(irq, &g_irq_list);
    if (irq_node != NULL) {
        printk("irq %s, irq_num =%dr repeat!!!\n", name, irq);
        return -1;
    }

    irq_node = kmalloc(sizeof(irq_info), GFP_KERNEL);
    if (irq_node == NULL) {
        printk("kmalloc irq_node:%s failed.\n", name);
        return -1;
    }

    __irq = hi_get_irq_byname((char *)name);
    if (__irq == -1) {
        printk("hi_get_irq_byname:%s failed.\n", name);
        kfree(irq_node);
        return -1;
    }
    irq_node->__irq = __irq;
    irq_node->irq = irq;
#endif

    osal_list_add_tail(&irq_node->node, &g_irq_list);

    return request_threaded_irq(__irq, (irq_handler_t)handler, (irq_handler_t)thread_fn, flags, name, dev);
}
EXPORT_SYMBOL(osal_irq_request);

void osal_irq_free(unsigned int irq, void *dev)
{
    int __irq = irq;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
    irq_info *irq_node = NULL;

    irq_node = osal_irq_find_node(irq, &g_irq_list);
    if (irq_node != NULL) {
        __irq = irq_node->__irq;
        osal_list_del(&irq_node->node);
        kfree(irq_node);
    } else {
        printk("osal_irq_free irq=%u, is not exist\n", irq);
        osal_dump_stack();
        return;
    }
#endif

    free_irq(__irq, dev);
}
EXPORT_SYMBOL(osal_irq_free);

int osal_irq_set_affinity(unsigned int irq, const char *name, int cpu_mask)
{
    int __irq = irq;
    struct cpumask cpumask_set = {0};

    cpumask_clear(&cpumask_set);

    ((OSAL_CPU_0 & (unsigned int)cpu_mask) == 0) ?
        0 : cpumask_set_cpu(0, &cpumask_set); // cpu0

    ((OSAL_CPU_1 & (unsigned int)cpu_mask) == 0) ?
        0 : cpumask_set_cpu(1, &cpumask_set); // cpu1

    ((OSAL_CPU_2 & (unsigned int)cpu_mask) == 0) ?
        0 : cpumask_set_cpu(2, &cpumask_set); // cpu2

    ((OSAL_CPU_3 & (unsigned int)cpu_mask) == 0) ?
        0 : cpumask_set_cpu(3, &cpumask_set); // cpu 3

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
    __irq = hi_get_irq_byname((char *)name);
#endif

    return irq_set_affinity_hint(__irq, &cpumask_set);
}
EXPORT_SYMBOL(osal_irq_set_affinity);

int osal_in_interrupt(void)
{
    return in_interrupt();
}
EXPORT_SYMBOL(osal_in_interrupt);

/*
 * enable_irq - enable handling of an irq
 * @irq: Interrupt to enable
 *
 * Undoes the effect of one call to disable_irq().  If this
 * matches the last disable, processing of interrupts on this
 * IRQ line is re-enabled.
 * This function may be called from IRQ context only when
 * desc->irq_data.chip->bus_lock and desc->chip->bus_sync_unlock
 * are NULL !
 */
void osal_irq_enable(unsigned int irq)
{
    int __irq = irq;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
    irq_info *irq_node = NULL;

    irq_node = osal_irq_find_node(irq, &g_irq_list);
    if (irq_node != NULL) {
        __irq = irq_node->__irq;
    } else {
        printk("osal_irq_enable irq=%u, is not exist\n", irq);
        return;
    }
#endif

    enable_irq(__irq);

    return;
}
EXPORT_SYMBOL(osal_irq_enable);

/*
 * disable_irq - disable an irq and wait for completion
 * irq: Interrupt to disable
 *
 * Disable the selected interrupt line.  Enables and Disables are
 * nested.
 * This function waits for any pending IRQ handlers for this interrupt
 * to complete before returning. If you use this function while
 * holding a resource the IRQ handler may need you will deadlock.
 * This function may be called - with care - from IRQ context.
 **/
void osal_irq_disable(unsigned int irq)
{
    int __irq = irq;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
    irq_info *irq_node = NULL;

    irq_node = osal_irq_find_node(irq, &g_irq_list);
    if (irq_node != NULL) {
        __irq = irq_node->__irq;
    } else {
        printk("osal_irq_disable irq=%d, is not exist\n", irq);
        return;
    }
#endif

    disable_irq(__irq);

    return;
}
EXPORT_SYMBOL(osal_irq_disable);

/* tasklet is running only on one CPU simultaneously */
int osal_tasklet_init(osal_tasklet *tasklet)
{
    int ret = 0;
    struct tasklet_struct *tasklet_local = NULL;
    atomic_t i = ATOMIC_INIT(0);

    if (!tasklet) {
        printk("init tasklet is NULL ! \n");
        return -1;
    }

    tasklet_local = (struct tasklet_struct *)kmalloc(sizeof(struct tasklet_struct), GFP_KERNEL);
    if (!tasklet_local) {
        printk("Tasklet initialize when malloc memory failed\n");
        return -1;
    }

    ret = memset_s(tasklet_local, sizeof(struct tasklet_struct), 0, sizeof(struct tasklet_struct));
    if (ret != 0) {
        return -1;
    }

    tasklet_local->next  = NULL;
    tasklet_local->state = 0;
    tasklet_local->count = i;
    tasklet_local->func  = tasklet->handler;
    tasklet_local->data  = tasklet->data;

    tasklet->tasklet = (void*)tasklet_local;

    return 0;
}
EXPORT_SYMBOL(osal_tasklet_init);

/*
 * tasklet update should be called after tasklet init
 **/
int osal_tasklet_update(osal_tasklet *tasklet)
{
    struct tasklet_struct *tasklet_local = NULL;

    if (!tasklet) {
        printk("tasklet setdata input is NULL\n");
        return -1;
    }

    tasklet_local = (struct tasklet_struct *) tasklet->tasklet;
    tasklet_local->data = tasklet->data;
    tasklet_local->func = tasklet->handler;

    return 0;
}
EXPORT_SYMBOL(osal_tasklet_update);

/*
 * add tasklet to tasklet_hi_vec and start tasklet
 **/
int osal_tasklet_schedule(osal_tasklet *tasklet)
{
    if (!tasklet) {
        printk("schedule tasklet is NULL ! \n");
        return -1;
    }

    tasklet_schedule((struct tasklet_struct *)tasklet->tasklet);

    return 0;
}
EXPORT_SYMBOL(osal_tasklet_schedule);

/* kill tasklet */
int osal_tasklet_kill(osal_tasklet *tasklet)
{
    if (!tasklet) {
        printk("kill tasklet is NULL \n");
        return -1;
    }

    tasklet_kill((struct tasklet_struct *)tasklet->tasklet);
    kfree(tasklet->tasklet);
    tasklet->tasklet = NULL;

    return 0;
}
EXPORT_SYMBOL(osal_tasklet_kill);
