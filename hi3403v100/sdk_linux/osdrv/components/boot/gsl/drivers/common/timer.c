/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 */
#include <types.h>
#include <platform.h>
#include <lib.h>

/* Constants for reported magic-number values. */
#define GSL_TIMER_DELAY_US_1000                1000

static  unsigned long timestamp;
static  unsigned long lastdec;
#define TIMER_LOAD_VAL 0xffffffff

#define TIMER_DIVIDER_MS  (TIMER_FEQ / TIMER_DIV / 1000)
#define CONFIG_WD_PERIOD  (10 * 1000 * 1000)   /* 10 seconds default*/
#define TIME_COVERT_RATIO 1000 /* 1000000us = 1000ms = 1s*/

void reset_timer_masked(void);

void timer_init()
{
    uint32_t val;

    val = reg_get(REG_BASE_SCTL + REG_SC_CTRL);
    val = val & (~TIMEREN0_SEL); /* select 3M clk */
    reg_set(REG_BASE_SCTL + REG_SC_CTRL, val);

    reg_set(REG_BASE_TIMER0 + TIMER_LOAD, 0);
    reg_set(REG_BASE_TIMER0 + TIMER_CONTROL, TIMER_EN | TIMER_MODE |
        TIMER_PRE | TIMER_SIZE);
}

void timer_deinit()
{
    reg_set(REG_BASE_TIMER0 + TIMER_CONTROL, 0);
}

void timer_start()
{
    uint32_t val;

    val = reg_get(REG_BASE_TIMER0 + TIMER_CONTROL);
    val &= (~TIMER_EN);
    reg_set(REG_BASE_TIMER0 + TIMER_CONTROL, val);

    reg_set(REG_BASE_TIMER0 + TIMER_LOAD, 0xffffffff);

    val |= TIMER_EN;
    reg_set(REG_BASE_TIMER0 + TIMER_CONTROL, val);

    /* init the timestamp and lastdec value */
    reset_timer_masked();
}

unsigned long timer_get_val()
{
    return 0xffffffff - reg_get(REG_BASE_TIMER0 + TIMER_VALUE);
}
unsigned long timer_get_divider()
{
    return TIMER_DIVIDER_MS;
}

unsigned long get_timer_masked(void)
{
    unsigned long now = READ_TIMER;         /* current tick value */

    if (lastdec >= now) {
        /* normal mode, not roll back */
        timestamp += lastdec - now;
    } else {
        /* rollback */
        timestamp += lastdec + TIMER_LOAD_VAL - now;
    }
    lastdec = now;

    return timestamp;
}
unsigned long get_timer(unsigned long base)
{
    return get_timer_masked() - base;
}

void reset_timer_masked(void)
{
    /* reset time */
    lastdec = READ_TIMER;  /* capure current decrementer value time */
    timestamp = 0;         /* start "advancing" time stamp from 0 */
}

/* delay x useconds AND perserve advance timstamp value */
void timer_udelay_inner(unsigned long usec)
{
    unsigned long tmo;
    unsigned long tmp;
    unsigned long now;

    /* if "big" number, spread normalization to seconds */
    if (usec >= TIME_COVERT_RATIO) {
        /* start to normalize for usec to ticks per sec */
        tmo = usec / TIME_COVERT_RATIO;
        /* find number of "ticks" to wait to achieve target */
        tmo *= CONFIG_SYS_HZ;
        /* finish normalize. */
        tmo /= TIME_COVERT_RATIO;
    } else {
        /* else small number, don't kill it prior to HZ multiply */
        tmo = usec * CONFIG_SYS_HZ;
        tmo /= (TIME_COVERT_RATIO * TIME_COVERT_RATIO);
    }

    tmp = get_timer(0);             /* get current timestamp */
    /* if setting this fordward will roll time stamp */
    if ((tmo + tmp + 1) < tmp) {
        /* reset "advancing" timestamp to 0, set lastdec value */
        reset_timer_masked();
    } else {
        tmo += tmp;
    }

    /* loop till event */
    now = get_timer_masked();
    while (now < tmo) {
        now = get_timer_masked();
    }
}

void udelay(unsigned long usec)
{
    unsigned long kv;

    do {
        kv = usec > CONFIG_WD_PERIOD ? CONFIG_WD_PERIOD : usec;
        timer_udelay_inner(kv);
        usec -= kv;
    } while (usec);
}

void mdelay(unsigned long msec)
{
    while (msec--) {
        udelay(GSL_TIMER_DELAY_US_1000);
    }
}
