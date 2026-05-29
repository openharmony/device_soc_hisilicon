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
#include <linux/printk.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/rtc.h>
#include <linux/sched/clock.h>
#include "ot_osal.h"

/* Constants for reported magic-number values. */
#define OSAL_TIMER_DIVISOR_1000                1000

struct timer_list_info {
    struct timer_list time_list;
    unsigned long data;
};
typedef void (*timer_callback_fun)(struct timer_list *data);

unsigned long osal_timer_get_private_data(const void *data)
{
    struct timer_list_info *list_info = osal_container_of(data, struct timer_list_info, time_list);

    return list_info->data;
}
EXPORT_SYMBOL(osal_timer_get_private_data);

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
int osal_timer_init(osal_timer_t *timer)
{
    struct timer_list_info *t = NULL;

    if (timer == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    t = (struct timer_list_info *)kmalloc(sizeof(struct timer_list_info), GFP_KERNEL);
    if (t == NULL) {
        osal_trace("%s - kmalloc error!\n", __FUNCTION__);
        return -1;
    }

    t->data = timer->data;
    timer_setup(&t->time_list, (timer_callback_fun)timer->function, 0);
    timer->timer = t;
    return 0;
}
EXPORT_SYMBOL(osal_timer_init);
int osal_set_timer(osal_timer_t *timer, unsigned long interval)
{
    struct timer_list_info *list_info = NULL;
    if ((timer == NULL) || (timer->timer == NULL) || (timer->function == NULL) || (interval == 0)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    list_info = (struct timer_list_info *)timer->timer;
    list_info->data = timer->data;
    list_info->time_list.function = (timer_callback_fun)timer->function;
    return mod_timer(&list_info->time_list, jiffies + msecs_to_jiffies(interval) - 1);
}
EXPORT_SYMBOL(osal_set_timer);

int osal_set_timer_with_affinity(osal_timer_t *timer, unsigned long interval, int cpuset)
{
    struct timer_list_info *list_info = NULL;
    unsigned long expires;

    if ((timer == NULL) || (timer->timer == NULL) || (timer->function == NULL) || (interval == 0)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    if (cpuset != -1 && cpuset >= nr_cpu_ids) {
        osal_trace("fun:%s line:%d cpuset(%d) should less %d!\n", __FUNCTION__, __LINE__, cpuset, nr_cpu_ids);
        return -1;
    }

    expires = jiffies + msecs_to_jiffies(interval) - 1;

    list_info = (struct timer_list_info *)timer->timer;
    list_info->data = timer->data;
    list_info->time_list.function = (timer_callback_fun)timer->function;
    if (cpuset == -1) {
        return mod_timer(&list_info->time_list, expires);
    } else {
        list_info->time_list.expires = expires;
        add_timer_on(&list_info->time_list, cpuset);
    }
    return 0;
}
EXPORT_SYMBOL(osal_set_timer_with_affinity);
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif

int osal_del_timer(osal_timer_t *timer)
{
    struct timer_list *t = NULL;
    if ((timer == NULL) || (timer->timer == NULL) || (timer->function == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    t = timer->timer;
    return del_timer(t);
}
EXPORT_SYMBOL(osal_del_timer);

int osal_timer_destroy(osal_timer_t *timer)
{
    struct timer_list *t = NULL;

    if ((timer == NULL) || (timer->timer == NULL)) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    t = timer->timer;
    del_timer(t);
    kfree(t);
    timer->timer = NULL;
    return 0;
}
EXPORT_SYMBOL(osal_timer_destroy);

void osal_msleep(unsigned int msecs)
{
    msleep(msecs);
}
EXPORT_SYMBOL(osal_msleep);

void osal_udelay(unsigned int usecs)
{
    udelay(usecs);
}
EXPORT_SYMBOL(osal_udelay);

void osal_mdelay(unsigned int msecs)
{
    mdelay(msecs);
}
EXPORT_SYMBOL(osal_mdelay);

unsigned int osal_get_tickcount(void)
{
    return jiffies_to_msecs(jiffies);
}
EXPORT_SYMBOL(osal_get_tickcount);

unsigned long long osal_sched_clock(void)
{
    return sched_clock();
}
EXPORT_SYMBOL(osal_sched_clock);

void osal_gettimeofday(osal_timeval_t *tv)
{
    struct timespec64 t;
    if (tv == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }

    ktime_get_real_ts64(&t);
    tv->tv_sec = t.tv_sec;
    tv->tv_usec = t.tv_nsec/OSAL_TIMER_DIVISOR_1000; /* nanosecond to microseconds */
}
EXPORT_SYMBOL(osal_gettimeofday);

void osal_rtc_time_to_tm(unsigned long time, osal_rtc_time_t *tm)
{
#ifdef CONFIG_RTC_LIB
    struct rtc_time _tm = {0};

    if (tm == NULL) {
        osal_trace("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }
    rtc_time64_to_tm(time, &_tm);

    tm->tm_sec = _tm.tm_sec;
    tm->tm_min = _tm.tm_min;
    tm->tm_hour = _tm.tm_hour;
    tm->tm_mday = _tm.tm_mday;
    tm->tm_mon = _tm.tm_mon;
    tm->tm_year = _tm.tm_year;
    tm->tm_wday = _tm.tm_wday;
    tm->tm_yday = _tm.tm_yday;
    tm->tm_isdst = _tm.tm_isdst;
#else
    osal_trace("rtc is not open!\n");
#endif
}
EXPORT_SYMBOL(osal_rtc_time_to_tm);
