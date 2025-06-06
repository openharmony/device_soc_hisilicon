/**
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Description: Timer Sample Source. \n
 *
 * History: \n
 * 2023-07-18, Create file. \n
 */
#include "timer.h"
#include "tcxo.h"
#include "chip_core_irq.h"
#include "common_def.h"
#include "soc_osal.h"
#include "app_init.h"

#define TIMER_TIMERS_NUM            4
#define TIMER_INDEX                 1
#define TIMER_PRIO                  1
#define TIMER_DELAY_INT             5
#define TIMER1_DELAY_1000US         1000
#define TIMER2_DELAY_2000US         2000
#define TIMER3_DELAY_3000US         3000
#define TIMER4_DELAY_4000US         4000
#define TIMER_MS_2_US               1000

#define TIMER_TASK_PRIO             24
#define TIMER_TASK_STACK_SIZE       0x1000

typedef struct timer_info {
    uint32_t start_time;
    uint32_t end_time;
    uint32_t delay_time;
} timer_info_t;

static uint32_t g_timer_int_count = 0;
static timer_info_t g_timers_info[TIMER_TIMERS_NUM] = {
    {0, 0, TIMER1_DELAY_1000US},
    {0, 0, TIMER2_DELAY_2000US},
    {0, 0, TIMER3_DELAY_3000US},
    {0, 0, TIMER4_DELAY_4000US}
};

/* Timed task callback function list. */
static void timer_timeout_callback(uintptr_t data)
{
    uint32_t timer_index = (uint32_t)data;
    g_timers_info[timer_index].end_time = uapi_tcxo_get_ms();
    g_timer_int_count++;
}

static void *timer_task(const char *arg)
{
    unused(arg);
    timer_handle_t timer_index[TIMER_TIMERS_NUM] = { 0 };
    uapi_timer_init();
    uapi_timer_adapter(TIMER_INDEX, TIMER_1_IRQN, TIMER_PRIO);

    for (uint32_t i = 0; i < TIMER_TIMERS_NUM; i++) {
        uapi_timer_create(TIMER_INDEX, &timer_index[i]);
        g_timers_info[i].start_time = uapi_tcxo_get_ms();
        uapi_timer_start(timer_index[i], g_timers_info[i].delay_time, timer_timeout_callback, i);
        osal_msleep(TIMER_DELAY_INT);
    }

    while (g_timer_int_count < TIMER_TIMERS_NUM) {
        osal_msleep(TIMER_DELAY_INT);
    }

    for (uint32_t i = 0; i < TIMER_TIMERS_NUM; i++) {
        uapi_timer_stop(timer_index[i]);
        uapi_timer_delete(timer_index[i]);
        osal_printk("real time[%d] = %dms  ", i, (g_timers_info[i].end_time -  g_timers_info[i].start_time));
        osal_printk("  delay = %dms\r\n", g_timers_info[i].delay_time / TIMER_MS_2_US);
    }
    return NULL;
}

static void timer_entry(void)
{
    osal_task *task_handle = NULL;
    osal_kthread_lock();
    task_handle = osal_kthread_create((osal_kthread_handler)timer_task, 0, "TimerTask", TIMER_TASK_STACK_SIZE);
    if (task_handle != NULL) {
        osal_kthread_set_priority(task_handle, TIMER_TASK_PRIO);
        osal_kfree(task_handle);
    }
    osal_kthread_unlock();
}

/* Run the timer_entry. */
app_run(timer_entry);