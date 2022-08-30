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
#include "timer_hi35xx.h"
#include "device_resource_if.h"
#include "hdf_base.h"
#include "hdf_log.h"
#include "osal_io.h"
#include "osal_irq.h"
#include "osal_mem.h"

#define HDF_LOG_TAG timer_hi35xx

static int32_t TimerHi35xxRegWrite(uint32_t value, volatile uint8_t *addr)
{
    CHECK_NULL_PTR_RETURN_VALUE(addr, HDF_ERR_INVALID_OBJECT);

    OSAL_WRITEL(value, addr);
    return HDF_SUCCESS;
}

static uint32_t TimerHi35xxRegRead(volatile uint8_t *addr)
{
    CHECK_NULL_PTR_RETURN_VALUE(addr, HDF_ERR_INVALID_OBJECT);

    uint32_t val = OSAL_READL(addr);
    return val;
}

static int32_t TimerHi35xxEnable(struct TimerHi35xxInfo *info, bool enable)
{
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);
    uint32_t value = TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_CONTROL);
    if (enable) {
        value &= ~TIMERx_CONTROL_TIMEREN;
        value |= (0x1 << TIMERx_CONTROL_TIMEREN_SHIFT);
    } else {
        value &= ~TIMERx_CONTROL_TIMEREN;
        value |= (0x0 << TIMERx_CONTROL_TIMEREN_SHIFT);
    }
    TimerHi35xxRegWrite(value, info->regBase + HI35XX_TIMERx_CONTROL);
    TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_CONTROL);
    return HDF_SUCCESS;
}

// once or period
static int32_t TimerHi35xxSetMode(struct TimerHi35xxInfo *info, uint16_t mode)
{
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);
    uint32_t value = TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_CONTROL);
    if ((mode == TIMERx_CONTROL_TIMERMODE_PERIOD) || (mode == TIMERx_CONTROL_TIMERMODE_FREE)) {
        value &= ~TIMERx_CONTROL_TIMERMODE;
        value |= (mode << TIMERx_CONTROL_TIMERMODE_SHIFT);
    } else { // one shot
        value &= ~TIMERx_CONTROL_ONESHOT;
        value |= (TIMERx_CONTROL_TIMERMODE_ONESHOT_ONE << TIMERx_CONTROL_ONESHOT_SHIFT);
    }

    TimerHi35xxRegWrite(value, info->regBase + HI35XX_TIMERx_CONTROL);
    return HDF_SUCCESS;
}

// divide frequence
static int32_t TimerHi35xxSetPre(struct TimerHi35xxInfo *info, uint16_t pre)
{
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);
    uint32_t value = TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_CONTROL);
    uint32_t tmpVal;
    switch (pre) {
        case TIMERx_CONTROL_TIMERPRE_NOT:
            tmpVal = TIMERx_CONTROL_TIMERPRE_NOT;
            break;
        case TIMERx_CONTROL_TIMERPRE_4_LEVEL:
            tmpVal = TIMERx_CONTROL_TIMERPRE_4_LEVEL;
            break;
        case TIMERx_CONTROL_TIMERPRE_8_LEVEL:
        case TIMERx_CONTROL_TIMERPRE_UNDEFINE:
            tmpVal = TIMERx_CONTROL_TIMERPRE_8_LEVEL;
            break;
        default:
            HDF_LOGE("%s: unsupported pre [%hu]", __func__, pre);
            return HDF_ERR_INVALID_PARAM;
    }
    value &= ~TIMERx_CONTROL_TIMERPRE;
    value |= (tmpVal << TIMERx_CONTROL_TIMERPRE_SHIFT);
    TimerHi35xxRegWrite(value, info->regBase + HI35XX_TIMERx_CONTROL);
    TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_CONTROL);
    return HDF_SUCCESS;
}

// mask interrupts true:not mask; false mask
static int32_t TimerHi35xxIntEnable(struct TimerHi35xxInfo *info, bool enable)
{
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);
    uint32_t value = TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_CONTROL);
    if (enable) {
        value &= ~TIMERx_CONTROL_INTENABLE;
        value |= (0x1 << TIMERx_CONTROL_INTENABLE_SHIFT);
    } else {
        value &= ~TIMERx_CONTROL_INTENABLE;
        value |= (0x0 << TIMERx_CONTROL_INTENABLE_SHIFT);
    }
    TimerHi35xxRegWrite(value, info->regBase + HI35XX_TIMERx_CONTROL);
    return HDF_SUCCESS;
}

// clear timer interrupt
static int32_t TimerHi35xxIntClear(struct TimerHi35xxInfo *info)
{
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);
    TimerHi35xxRegWrite(0x0, info->regBase + HI35XX_TIMERx_INTCLR);
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxTimerSize(struct TimerHi35xxInfo *info, bool is32Bit)
{
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);
    uint32_t value = TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_CONTROL);
    if (is32Bit) { // 1:32 bit
        value &= ~TIMERx_CONTROL_TIMERSIZE;
        value |= (0x1 << TIMERx_CONTROL_TIMERSIZE_SHIFT);
    } else { // 0:16 bit
        value &= ~TIMERx_CONTROL_TIMERSIZE;
        value |= (0x0 << TIMERx_CONTROL_TIMERSIZE_SHIFT);
    }
    TimerHi35xxRegWrite(value, info->regBase + HI35XX_TIMERx_CONTROL);
    return HDF_SUCCESS;
}

// timer count value
static int32_t TimerHi35xxTimerLoadSet(struct TimerHi35xxInfo *info, uint32_t value)
{
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);

    TimerHi35xxRegWrite(value, info->regBase + HI35XX_TIMERx_LOAD);
    return HDF_SUCCESS;
}

static void TimerHi35xxTimerGetAllReg(struct TimerHi35xxInfo *info)
{
    TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_LOAD);
    TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_VALUE);
    TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_CONTROL);
    TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_INTCLR);
    TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_RIS);
    TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_MIS);
    TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_BGLOAD);
}

static void TimerDumperAddDatas(struct TimerHi35xxInfo *info)
{
    struct PlatformDumperData datas[] = {
        {"HI35XX_TIMERx_LOAD", PLATFORM_DUMPER_REGISTERB, (void *)(info->regBase + HI35XX_TIMERx_LOAD)},
        {"HI35XX_TIMERx_VALUE", PLATFORM_DUMPER_REGISTERB, (void *)(info->regBase + HI35XX_TIMERx_VALUE)},
        {"HI35XX_TIMERx_CONTROL", PLATFORM_DUMPER_REGISTERB, (void *)(info->regBase + HI35XX_TIMERx_CONTROL)},
        {"HI35XX_TIMERx_INTCLR", PLATFORM_DUMPER_REGISTERB, (void *)(info->regBase + HI35XX_TIMERx_INTCLR)},
        {"HI35XX_TIMERx_RIS", PLATFORM_DUMPER_REGISTERB, (void *)(info->regBase + HI35XX_TIMERx_RIS)},
        {"HI35XX_TIMERx_MIS", PLATFORM_DUMPER_REGISTERB, (void *)(info->regBase + HI35XX_TIMERx_MIS)},
        {"HI35XX_TIMERx_BGLOAD", PLATFORM_DUMPER_REGISTERB, (void *)(info->regBase + HI35XX_TIMERx_BGLOAD)},
    };
    if (info->dumper == NULL) {
        return;
    }

    PlatformDumperAddDatas(info->dumper, datas, sizeof(datas) / sizeof(struct PlatformDumperData));
}

// timer count clk
static void TimerHi35xxScCtrlSet(void)
{
    volatile uint8_t *regBase =
        (volatile void *)OsalIoRemap((uintptr_t)HI35XX_SC_CTRL_REG, TIMER_MAX_REG_SIZE);
    if (regBase == NULL) {
        HDF_LOGE("%s:OsalIoRemap fail", __func__);
        return;
    }
    uint32_t value = TimerHi35xxRegRead(regBase);
    value &= ~HI35XX_SC_CTRL_TIMEREN0OV;
    value |= (0x0 << HI35XX_SC_CTRL_TIMEREN0OV_SHIFT);
    value &= ~HI35XX_SC_CTRL_TIMEREN1OV;
    value |= (0x0 << HI35XX_SC_CTRL_TIMEREN1OV_SHIFT);
    value &= ~HI35XX_SC_CTRL_TIMEREN2OV;
    value |= (0x0 << HI35XX_SC_CTRL_TIMEREN2OV_SHIFT);
    value &= ~HI35XX_SC_CTRL_TIMEREN3OV;
    value |= (0x0 << HI35XX_SC_CTRL_TIMEREN3OV_SHIFT);
    value &= ~HI35XX_SC_CTRL_TIMEREN4OV;
    value |= (0x0 << HI35XX_SC_CTRL_TIMEREN4OV_SHIFT);
    value &= ~HI35XX_SC_CTRL_TIMEREN5OV;
    value |= (0x0 << HI35XX_SC_CTRL_TIMEREN5OV_SHIFT);
    value &= ~HI35XX_SC_CTRL_TIMEREN6OV;
    value |= (0x0 << HI35XX_SC_CTRL_TIMEREN6OV_SHIFT);
    value &= ~HI35XX_SC_CTRL_TIMEREN7OV;
    value |= (0x0 << HI35XX_SC_CTRL_TIMEREN7OV_SHIFT);
    TimerHi35xxRegWrite(value, regBase);
    TimerHi35xxRegRead(regBase);
    OsalIoUnmap((void *)regBase);
    regBase = NULL;
}

static uint32_t TimerHi35xxIrqHandle(uint32_t irqId, void *data)
{
    CHECK_NULL_PTR_RETURN_VALUE(data, HDF_ERR_INVALID_OBJECT);
    struct TimerHi35xxInfo *info = NULL;
    info = (struct TimerHi35xxInfo *)data;

    HDF_LOGD("------------->%s[%s]: timer[%u], irqId [%u] in ",
        __func__, __TIME__, info->number, irqId);

    // clear interrupt
    TimerHi35xxIntClear(info);

    CHECK_NULL_PTR_RETURN_VALUE(info->cb, HDF_ERR_INVALID_OBJECT);
    info->cb(info->number);
    HDF_LOGD("------------->%s: timer[%u], irqId [%u] process success", __func__, info->number, irqId);
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxSet(struct TimerCntrl *cntrl, uint32_t useconds, TimerHandleCb cb)
{
    CHECK_NULL_PTR_RETURN_VALUE(cntrl, HDF_ERR_INVALID_OBJECT);
    CHECK_NULL_PTR_RETURN_VALUE(cntrl->priv, HDF_ERR_INVALID_OBJECT);
    CHECK_NULL_PTR_RETURN_VALUE(cb, HDF_ERR_INVALID_OBJECT);

    cntrl->info.useconds = useconds;
    cntrl->info.cb = cb;
    cntrl->info.isPeriod = true;

    struct TimerHi35xxInfo *info = cntrl->priv;
    info->cb = cb;
    info->isPeriod = true;
    HDF_LOGD("%s: timer[%u][%u][%d] ",
        __func__, info->number, cntrl->info.useconds, cntrl->info.isPeriod);

    if (TimerHi35xxSetMode(info, TIMERx_CONTROL_TIMERMODE_PERIOD) != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxSetMode[%u] fail!", __func__, info->number);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxSetOnce(struct TimerCntrl *cntrl, uint32_t useconds, TimerHandleCb cb)
{
    CHECK_NULL_PTR_RETURN_VALUE(cntrl, HDF_ERR_INVALID_OBJECT);
    CHECK_NULL_PTR_RETURN_VALUE(cntrl->priv, HDF_ERR_INVALID_OBJECT);
    CHECK_NULL_PTR_RETURN_VALUE(cb, HDF_ERR_INVALID_OBJECT);

    cntrl->info.useconds = useconds;
    cntrl->info.cb = cb;
    cntrl->info.isPeriod = false;

    struct TimerHi35xxInfo *info = cntrl->priv;
    info->cb = cb;
    info->isPeriod = false;
    if (TimerHi35xxSetMode(info, TIMERx_CONTROL_TIMERMODE_ONESHOT) != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxSetMode[%u] fail!", __func__, info->number);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxSetTimeout(struct TimerCntrl *cntrl)
{
    CHECK_NULL_PTR_RETURN_VALUE(cntrl, HDF_ERR_INVALID_OBJECT);
    CHECK_NULL_PTR_RETURN_VALUE(cntrl->priv, HDF_ERR_INVALID_OBJECT);
    unsigned int value;
    unsigned int maxCnt = ~0x00; /* 32 bit counter */
    unsigned int maxSeconds = maxCnt / HI35XX_TIMERx_CLOCK_HZ;

    if (cntrl->info.useconds == 0 || (cntrl->info.useconds / HI35XX_TIMERx_US_TRANS_S) > maxSeconds) {
        value = maxCnt;
    } else {
        value = (cntrl->info.useconds / HI35XX_TIMERx_US_TRANS_S) * HI35XX_TIMERx_CLOCK_HZ;
    }

    struct TimerHi35xxInfo *info = cntrl->priv;
    OSAL_WRITEL(value, info->regBase + HI35XX_TIMERx_LOAD);

    HDF_LOGD("%s: timer[%u] [%u][%u][%u] ", __func__, info->number, maxSeconds, value, cntrl->info.useconds);

    return HDF_SUCCESS;
}

static int32_t TimerHi35xxIrqRegister(struct TimerHi35xxInfo *info)
{
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);

    if (OsalRegisterIrq(info->irq, 0, TimerHi35xxIrqHandle, "timer_alarm", (void *)info) != HDF_SUCCESS) {
        HDF_LOGE("%s: OsalRegisterIrq[%u][%u] fail!", __func__, info->irq, info->number);
        return HDF_FAILURE;
    }
    info->isIrqReg = true;
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxIrqUnregister(struct TimerHi35xxInfo *info)
{
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);

    (void)OsalUnregisterIrq(info->irq, (void *)info);
    info->isIrqReg = false;
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxStart(struct TimerCntrl *cntrl)
{
    CHECK_NULL_PTR_RETURN_VALUE(cntrl, HDF_ERR_INVALID_OBJECT);
    CHECK_NULL_PTR_RETURN_VALUE(cntrl->priv, HDF_ERR_INVALID_OBJECT);
    struct TimerHi35xxInfo *info = cntrl->priv;

    int ret = TimerHi35xxIrqRegister(info);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxIrqRegister fail!", __func__);
        return HDF_FAILURE;
    }

    // not mask interrupt
    ret = TimerHi35xxIntEnable(info, true);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxIntEnable fail!", __func__);
        return HDF_FAILURE;
    }

    ret = TimerHi35xxSetTimeout(cntrl);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxSetTimeout fail!", __func__);
        return HDF_FAILURE;
    }

    ret = TimerHi35xxEnable(info, true);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxEnable fail!", __func__);
        return HDF_FAILURE;
    }
    TimerHi35xxTimerGetAllReg(info);
    TimerDumperAddDatas(info);
    PlatformDumperDump(info->dumper);
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxStop(struct TimerCntrl *cntrl)
{
    CHECK_NULL_PTR_RETURN_VALUE(cntrl, HDF_ERR_INVALID_OBJECT);
    CHECK_NULL_PTR_RETURN_VALUE(cntrl->priv, HDF_ERR_INVALID_OBJECT);
    struct TimerHi35xxInfo *info = cntrl->priv;
    int ret = TimerHi35xxEnable(info, false);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxEnable fail!", __func__);
        return HDF_FAILURE;
    }

    ret = TimerHi35xxIntEnable(info, false);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxIntEnable fail!", __func__);
        return HDF_FAILURE;
    }

    ret = TimerHi35xxIrqUnregister(info);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxIrqUnregister fail!", __func__);
        return HDF_FAILURE;
    }
    PlatformDumperDump(info->dumper);
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxOpen(struct TimerCntrl *cntrl)
{
    CHECK_NULL_PTR_RETURN_VALUE(cntrl, HDF_ERR_INVALID_OBJECT);
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxClose(struct TimerCntrl *cntrl)
{
    CHECK_NULL_PTR_RETURN_VALUE(cntrl, HDF_ERR_INVALID_OBJECT);
    TimerHi35xxStop(cntrl);
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxRemove(struct TimerCntrl *cntrl)
{
    CHECK_NULL_PTR_RETURN_VALUE(cntrl, HDF_ERR_INVALID_OBJECT);
    CHECK_NULL_PTR_RETURN_VALUE(cntrl->priv, HDF_ERR_INVALID_OBJECT);
    struct TimerHi35xxInfo *info = cntrl->priv;

    TimerHi35xxStop(cntrl);
    if (info->regBase != NULL) {
        OsalIoUnmap((void *)info->regBase);
        info->regBase = NULL;
    }
    PlatformDumperDestroy(info->dumper);
    OsalMemFree(info->dumperName);

    OsalMemFree(cntrl->priv);
    cntrl->priv = NULL;
    return HDF_SUCCESS;
}

static struct TimerCntrlMethod g_timerCntlrMethod = {
    .Remove = TimerHi35xxRemove,
    .Open = TimerHi35xxOpen,
    .Close = TimerHi35xxClose,
    .Set = TimerHi35xxSet,
    .SetOnce = TimerHi35xxSetOnce,
    .Start = TimerHi35xxStart,
    .Stop = TimerHi35xxStop,
};

static int32_t TimerHi35xxInitRegSet(struct TimerHi35xxInfo *info)
{
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);
    TimerHi35xxScCtrlSet();
    TimerHi35xxEnable(info, false);
    TimerHi35xxSetMode(info, TIMERx_CONTROL_TIMERMODE_PERIOD);
    TimerHi35xxIntEnable(info, false);
    TimerHi35xxSetPre(info, TIMERx_CONTROL_TIMERPRE_NOT);
    TimerHi35xxTimerSize(info, true);
    TimerHi35xxRegRead(info->regBase + HI35XX_TIMERx_CONTROL);

    TimerHi35xxTimerLoadSet(info, HI35XX_TIMERx_LOAD_INIT_VAL);
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxReadHcs(struct TimerHi35xxInfo *info, const struct DeviceResourceNode *node)
{
    int32_t ret;
    uint32_t tmp;
    struct DeviceResourceIface *iface = NULL;

    iface = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    if (iface == NULL || iface->GetUint32 == NULL) {
        HDF_LOGE("%s: invalid drs ops fail!", __func__);
        return HDF_FAILURE;
    }

    ret = iface->GetUint32(node, "number", &info->number, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: read id fail!", __func__);
        return HDF_FAILURE;
    }

    ret = iface->GetUint32(node, "bus_clock", &info->busClock, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: read [%u] bus_clock fail!", __func__, info->number);
        return HDF_FAILURE;
    }

    ret = iface->GetUint32(node, "mode", &info->mode, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: read [%u] mode fail!", __func__, info->number);
        return HDF_FAILURE;
    }

    ret = iface->GetUint32(node, "init_count_val", &info->initCountVal, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: read [%u] init_count_val fail!", __func__, info->number);
        return HDF_FAILURE;
    }

    ret = iface->GetUint32(node, "irq", &info->irq, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: read [%u] irq fail!", __func__, info->number);
        return HDF_FAILURE;
    }

    if (iface->GetUint32(node, "reg_base", &tmp, 0) != HDF_SUCCESS) {
        HDF_LOGE("%s: read [%u] reg_base fail", __func__, info->number);
        return HDF_FAILURE;
    }
    info->regBase = OsalIoRemap(tmp, TIMER_MAX_REG_SIZE);
    if (info->regBase == NULL) {
        HDF_LOGE("%s:OsalIoRemap fail", __func__);
        return HDF_FAILURE;
    }

    HDF_LOGD("%s:number[%u], bus_clock[%u], mode[%u], init_count_val[%u] irq[%u]", __func__,
        info->number, info->busClock, info->mode, info->initCountVal, info->irq);

    return HDF_SUCCESS;
}

static int32_t TimerHi35xxInitHandle(const struct DeviceResourceNode *node, struct TimerHi35xxInfo *info)
{
    CHECK_NULL_PTR_RETURN_VALUE(node, HDF_ERR_INVALID_OBJECT);
    CHECK_NULL_PTR_RETURN_VALUE(info, HDF_ERR_INVALID_OBJECT);

    int32_t ret = TimerHi35xxReadHcs(info, node);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxReadHcs fail!", __func__);
        return ret;
    }

    ret = TimerHi35xxInitRegSet(info);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxInitRegSet fail!", __func__);
        return ret;
    }
    info->isIrqReg = false;

    return HDF_SUCCESS;
}

static void TimerHi35xxInfoFree(struct TimerCntrl *cntrl)
{
    CHECK_NULL_PTR_RETURN(cntrl);
    if (cntrl->priv != NULL) {
        OsalMemFree(cntrl->priv);
        cntrl->priv = NULL;
    }

    if (cntrl != NULL) {
        OsalMemFree(cntrl);
    }
}

static void TimerDumperGet(struct TimerHi35xxInfo *info)
{
    struct PlatformDumper *dumper = NULL;
    char *name = (char *)OsalMemAlloc(TIMER_DUMPER_NAME_LEN);
    if (name == NULL) {
        return;
    }
    if (snprintf_s(name, TIMER_DUMPER_NAME_LEN, TIMER_DUMPER_NAME_LEN - 1, "%s%d",
        TIMER_DUMPER_NAME_PREFIX, info->number) < 0) {
        HDF_LOGE("%s: snprintf_s name fail!", __func__);
        OsalMemFree(name);
        return;
    }
    dumper = PlatformDumperCreate(name);
    if (dumper == NULL) {
        HDF_LOGE("%s: get dumper for %s fail!", __func__, name);
        OsalMemFree(name);
        return;
    }
    info->dumperName = name;
    info->dumper = dumper;
}

static int32_t TimerHi35xxParseAndInit(struct HdfDeviceObject *device, const struct DeviceResourceNode *node)
{
    int32_t ret;
    struct TimerCntrl *cntrl = NULL;
    struct TimerHi35xxInfo *info = NULL;
    (void)device;

    cntrl = (struct TimerCntrl *)OsalMemCalloc(sizeof(*cntrl));
    CHECK_NULL_PTR_RETURN_VALUE(cntrl, HDF_ERR_INVALID_OBJECT);

    info = (struct TimerHi35xxInfo *)OsalMemCalloc(sizeof(*info));
    if (cntrl == NULL) {
        HDF_LOGE("%s: malloc info fail!", __func__);
        TimerHi35xxInfoFree(cntrl);
        return HDF_ERR_MALLOC_FAIL;
    }
    cntrl->priv = (void *)info;

    ret = TimerHi35xxInitHandle(node, info);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerHi35xxInitHandle fail!", __func__);
        TimerHi35xxInfoFree(cntrl);
        return ret;
    }

    cntrl->info.number = info->number;
    cntrl->ops = &g_timerCntlrMethod;
    TimerDumperGet(info);
    ret = TimerCntrlAdd(cntrl);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: TimerCntrlAdd fail!", __func__);
        TimerHi35xxInfoFree(cntrl);
        return ret;
    }
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxDeviceInit(struct HdfDeviceObject *device)
{
    int32_t ret;
    const struct DeviceResourceNode *childNode = NULL;
    if (device == NULL || device->property == NULL) {
        HDF_LOGE("%s: device or property is NULL", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    DEV_RES_NODE_FOR_EACH_CHILD_NODE(device->property, childNode) {
        ret = TimerHi35xxParseAndInit(device, childNode);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("%s:TimerHi35xxParseAndInit fail", __func__);
            return HDF_FAILURE;
        }
    }
    HDF_LOGD("%s: success", __func__);
    return HDF_SUCCESS;
}

static int32_t TimerHi35xxDeviceBind(struct HdfDeviceObject *device)
{
    CHECK_NULL_PTR_RETURN_VALUE(device, HDF_ERR_INVALID_OBJECT);
    HDF_LOGI("%s: success", __func__);
    return HDF_SUCCESS;
}

static void TimerHi35xxRemoveById(const struct DeviceResourceNode *node)
{
    int32_t ret;
    uint32_t timerId;
    struct DeviceResourceIface *drsOps = NULL;

    drsOps = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    if (drsOps == NULL || drsOps->GetUint32 == NULL) {
        HDF_LOGE("%s: invalid drs ops fail!", __func__);
        return;
    }

    ret = drsOps->GetUint32(node, "id", (uint32_t *)&timerId, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: read id fail!", __func__);
        return;
    }

    TimerCntrlRemoveByNumber(timerId);
    return;
}

static void TimerHi35xxDeviceRelease(struct HdfDeviceObject *device)
{
    const struct DeviceResourceNode *childNode = NULL;

    HDF_LOGD("%s: in", __func__);
    if (device == NULL || device->property == NULL) {
        HDF_LOGE("%s: device is NULL", __func__);
        return;
    }

    DEV_RES_NODE_FOR_EACH_CHILD_NODE(device->property, childNode) {
        TimerHi35xxRemoveById(childNode);
    }
}

struct HdfDriverEntry g_hdfTimerDevice = {
    .moduleVersion = 1,
    .moduleName = "hi35xx_timer_driver",
    .Bind = TimerHi35xxDeviceBind,
    .Init = TimerHi35xxDeviceInit,
    .Release = TimerHi35xxDeviceRelease,
};

HDF_INIT(g_hdfTimerDevice);
