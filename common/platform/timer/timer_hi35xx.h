/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#ifndef TIMER_HI35XX_H
#define TIMER_HI35XX_H

#include "hdf_base.h"
#include "los_vm_zone.h"
#include "platform_dumper.h"
#include "timer_core.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TIMER_MAX_REG_SIZE (64 * 1024)
#define TIMER_DUMPER_NAME_PREFIX "timer_dumper_"
#define TIMER_DUMPER_NAME_LEN 64
#define TIMER_DUMPER_DATAS_REGISTER_SIZE 10

struct TimerHi35xxInfo {
    uint32_t number;
    uint32_t busClock;
    uint32_t mode;
    uint32_t initCountVal;
    uint32_t irq;
    bool isIrqReg;
    bool isPeriod;
    volatile uint8_t *regBase;
    TimerHandleCb cb;
    char *dumperName;
    struct PlatformDumper *dumper;
};

#define HI35XX_SC_CTRL_REG (0x12020000 + 0x0000)
#define HI35XX_SC_CTRL_TIMEREN0OV (0x1 << 16)
#define HI35XX_SC_CTRL_TIMEREN0OV_SHIFT 16
#define HI35XX_SC_CTRL_TIMEREN1OV (0x1 << 17)
#define HI35XX_SC_CTRL_TIMEREN1OV_SHIFT 17
#define HI35XX_SC_CTRL_TIMEREN2OV (0x1 << 18)
#define HI35XX_SC_CTRL_TIMEREN2OV_SHIFT 18
#define HI35XX_SC_CTRL_TIMEREN3OV (0x1 << 19)
#define HI35XX_SC_CTRL_TIMEREN3OV_SHIFT 19
#define HI35XX_SC_CTRL_TIMEREN4OV (0x1 << 20)
#define HI35XX_SC_CTRL_TIMEREN4OV_SHIFT 20
#define HI35XX_SC_CTRL_TIMEREN5OV (0x1 << 21)
#define HI35XX_SC_CTRL_TIMEREN5OV_SHIFT 21
#define HI35XX_SC_CTRL_TIMEREN6OV (0x1 << 22)
#define HI35XX_SC_CTRL_TIMEREN6OV_SHIFT 22
#define HI35XX_SC_CTRL_TIMEREN7OV (0x1 << 23)
#define HI35XX_SC_CTRL_TIMEREN7OV_SHIFT 23

#define HI35XX_TIMERx_LOAD 0x000
#define HI35XX_TIMERx_VALUE 0x004
#define HI35XX_TIMERx_CONTROL 0x008
#define HI35XX_TIMERx_INTCLR 0x00C
#define HI35XX_TIMERx_RIS 0x010
#define HI35XX_TIMERx_MIS 0x014
#define HI35XX_TIMERx_BGLOAD 0x018

#define HI35XX_TIMERx_CLOCK_HZ (3 * 1000 * 1000)
#define HI35XX_TIMERx_LOAD_INIT_VAL 0x1000
#define HI35XX_TIMERx_US_TRANS_S 1000

#define HI35XX_TIMER0_1_INT_NUM 33
#define HI35XX_TIMER2_3_INT_NUM 34
#define HI35XX_TIMER4_5_INT_NUM 35
#define HI35XX_TIMER6_7_INT_NUM 36

// HI35XX_TIMERx_CONTROL reg control info
#define TIMERx_CONTROL_TIMEREN (0x1 << 7)
#define TIMERx_CONTROL_TIMEREN_SHIFT 7
#define TIMERx_CONTROL_TIMERMODE (0x1 << 6)
#define TIMERx_CONTROL_TIMERMODE_SHIFT 6
#define TIMERx_CONTROL_INTENABLE (0x1 << 5)
#define TIMERx_CONTROL_INTENABLE_SHIFT 5
#define TIMERx_CONTROL_TIMERPRE (0x3 << 2)
#define TIMERx_CONTROL_TIMERPRE_SHIFT 2
#define TIMERx_CONTROL_TIMERSIZE (0x1 << 1)
#define TIMERx_CONTROL_TIMERSIZE_SHIFT 1
#define TIMERx_CONTROL_ONESHOT (0x1 << 0)
#define TIMERx_CONTROL_ONESHOT_SHIFT 0

#define TIMERx_CONTROL_TIMERMODE_FREE 0x0
#define TIMERx_CONTROL_TIMERMODE_PERIOD 0x1
#define TIMERx_CONTROL_TIMERMODE_ONESHOT 0x2

#define TIMERx_CONTROL_TIMERMODE_ONESHOT_PERIOD 0x0
#define TIMERx_CONTROL_TIMERMODE_ONESHOT_ONE 0x1

#define TIMERx_CONTROL_TIMERPRE_NOT 0x0
#define TIMERx_CONTROL_TIMERPRE_4_LEVEL 0x01
#define TIMERx_CONTROL_TIMERPRE_8_LEVEL 0x10
#define TIMERx_CONTROL_TIMERPRE_UNDEFINE 0x11

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* TIMER_HI35XX_H */
