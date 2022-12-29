/*
 * Copyright (c) 2020-2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#include "device_resource_if.h"
#include "gpio/gpio_core.h"
#ifdef PL061_GPIO_USER_SUPPORT
#include "gpio_dev.h"
#endif
#include "hdf_device_desc.h"
#include "hdf_log.h"
#include "osal_io.h"
#include "osal_irq.h"
#include "osal_mem.h"
#include "osal_spinlock.h"

#define HDF_LOG_TAG gpio_hi35xx
#define PL061_GROUP_MAX 32
#define PL061_BIT_MAX   16
#define GPIO_MAX_INFO_NUM 96

#define PL061_GPIO_DATA_ALL(base)   ((base) + 0x3FC)
#define PL061_GPIO_DATA(base, bit)  ((base) + 0x000 + (1 << ((bit) + 2)))
#define PL061_GPIO_DIR(base)        ((base) + 0x400)
#define PL061_GPIO_IS(base)         ((base) + 0x404)
#define PL061_GPIO_IBE(base)        ((base) + 0x408)
#define PL061_GPIO_IEV(base)        ((base) + 0x40C)
#define PL061_GPIO_IE(base)         ((base) + 0x410)
#define PL061_GPIO_RIS(base)        ((base) + 0x414)
#define PL061_GPIO_MIS(base)        ((base) + 0x418)
#define PL061_GPIO_IC(base)         ((base) + 0x41C)

struct Pl061GpioGroup {
    struct GpioCntlr cntlr;
    volatile unsigned char *regBase;
    unsigned int index;
    unsigned int irq;
    OsalIRQHandle irqFunc;
    OsalSpinlock lock;
    uint32_t irqSave;
    bool irqShare;
};

struct Pl061GpioData {
    volatile unsigned char *regBase;
    uint32_t phyBase;
    uint32_t regStep;
    uint32_t irqStart;
    uint16_t groupNum;
    uint16_t bitNum;
    uint8_t irqShare;
    struct Pl061GpioGroup *groups;
    struct GpioInfo *gpioInfo;
    void *priv;
};

static struct Pl061GpioData g_pl061 = {
    .groups = NULL,
    .groupNum = PL061_GROUP_MAX,
    .bitNum = PL061_BIT_MAX,
};

static void Pl061GpioRegDump(struct Pl061GpioGroup *group)
{
    HDF_LOGI("%s: GROUP-%u: DATA-%x DIR-%x, IS-%x, IBE-%x, IEV-%x, IE-%x, RIS-%x, MIS-%x, IC-%x",
        __func__, group->index,
        OSAL_READL(PL061_GPIO_DATA_ALL(group->regBase)),
        OSAL_READL(PL061_GPIO_DIR(group->regBase)),
        OSAL_READL(PL061_GPIO_IS(group->regBase)),
        OSAL_READL(PL061_GPIO_IBE(group->regBase)),
        OSAL_READL(PL061_GPIO_IEV(group->regBase)),
        OSAL_READL(PL061_GPIO_IE(group->regBase)),
        OSAL_READL(PL061_GPIO_RIS(group->regBase)),
        OSAL_READL(PL061_GPIO_MIS(group->regBase)),
        OSAL_READL(PL061_GPIO_IC(group->regBase))
    );
}

static int32_t Pl061GpioSetDir(struct GpioCntlr *cntlr, uint16_t local, uint16_t dir)
{
    unsigned int val;
    volatile unsigned char *addr = NULL;
    unsigned int bitNum = local;
    struct Pl061GpioGroup *group = (struct Pl061GpioGroup *)cntlr;

    HDF_LOGV("%s: local:%hu, dir:%hu", __func__, local, dir);

    if (OsalSpinLockIrqSave(&group->lock, &group->irqSave) != HDF_SUCCESS) {
        return HDF_ERR_DEVICE_BUSY;
    }
    addr = PL061_GPIO_DIR(group->regBase);
    val = OSAL_READL(addr);
    if (dir == GPIO_DIR_IN) {
        val &= ~(1 << bitNum);
    } else if (dir == GPIO_DIR_OUT) {
        val |= 1 << bitNum;
    }
    OSAL_WRITEL(val, addr);
    (void)OsalSpinUnlockIrqRestore(&group->lock, &group->irqSave);

    return HDF_SUCCESS;
}

static int32_t Pl061GpioGetDir(struct GpioCntlr *cntlr, uint16_t local, uint16_t *dir)
{
    unsigned int val;
    volatile unsigned char *addr = NULL;
    unsigned int bitNum = local;
    struct Pl061GpioGroup *group = (struct Pl061GpioGroup *)cntlr;

    HDF_LOGV("%s: local:%hu", __func__, local);

    addr = PL061_GPIO_DIR(group->regBase);
    val = OSAL_READL(addr);
    if (val & (1 << bitNum)) {
        *dir = GPIO_DIR_OUT;
    } else {
        *dir = GPIO_DIR_IN;
    }
    return HDF_SUCCESS;
}

static int32_t Pl061GpioWrite(struct GpioCntlr *cntlr, uint16_t local, uint16_t val)
{
    unsigned int valCur;
    volatile unsigned char *addr = NULL;
    unsigned int bitNum = local;
    struct Pl061GpioGroup *group = (struct Pl061GpioGroup *)cntlr;

    if (OsalSpinLockIrqSave(&group->lock, &group->irqSave) != HDF_SUCCESS) {
        return HDF_ERR_DEVICE_BUSY;
    }
    addr = PL061_GPIO_DATA(group->regBase, bitNum);
    valCur = OSAL_READL(addr);
    if (val == GPIO_VAL_LOW) {
        valCur &= ~(1 << bitNum);
    } else {
        valCur |= (1 << bitNum);
    }
    OSAL_WRITEL(valCur, addr);
    (void)OsalSpinUnlockIrqRestore(&group->lock, &group->irqSave);

    return HDF_SUCCESS;
}

static int32_t Pl061GpioRead(struct GpioCntlr *cntlr, uint16_t local, uint16_t *val)
{
    unsigned int valCur;
    volatile unsigned char *addr = NULL;
    unsigned int bitNum = local;
    struct Pl061GpioGroup *group = (struct Pl061GpioGroup *)cntlr;

    addr = PL061_GPIO_DATA(group->regBase, bitNum);
    valCur = OSAL_READL(addr);
    if (valCur & (1 << bitNum)) {
        *val = GPIO_VAL_HIGH;
    } else {
        *val = GPIO_VAL_LOW;
    }
    return HDF_SUCCESS;
}

static uint32_t Pl061IrqHandleShare(uint32_t irq, void *data)
{
    (void)irq;
    (void)data;
    return HDF_SUCCESS;
}

static uint32_t Pl061IrqHandleNoShare(uint32_t irq, void *data)
{
    unsigned int i;
    unsigned int val;
    struct Pl061GpioGroup *group = (struct Pl061GpioGroup *)data;

    if (data == NULL) {
        HDF_LOGW("%s: data is NULL!", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    val = OSAL_READL(PL061_GPIO_MIS(group->regBase));
    OSAL_WRITEL(val, PL061_GPIO_IC(group->regBase));
    if (val == 0) {
        HDF_LOGW("%s: share irq(%u) trigerred but not hit any, mis=%x", __func__, irq, val);
        Pl061GpioRegDump(group);
        return HDF_FAILURE;
    }
    for (i = 0; i < group->cntlr.count && val != 0; i++, val >>= 1) {
        if ((val & 0x1) != 0) {
            GpioCntlrIrqCallback(&group->cntlr, i);
        }
    }
    return HDF_SUCCESS;
}

static int32_t Pl061GpioRegisterGroupIrqUnsafe(struct Pl061GpioGroup *group)
{
    int ret;
    if (group->irqShare == 1) {
        ret = OsalRegisterIrq(group->irq, 0, Pl061IrqHandleShare, "GPIO", NULL);
        if (ret != 0) {
            HDF_LOGE("%s: share irq:%u reg fail:%d!", __func__, group->irq, ret);
            return HDF_FAILURE;
        }
        group->irqFunc = Pl061IrqHandleShare;
    } else {
        ret = OsalRegisterIrq(group->irq, 0, Pl061IrqHandleNoShare, "GPIO", group);
        if (ret != 0) {
            (void)OsalUnregisterIrq(group->irq, group);
            ret = OsalRegisterIrq(group->irq, 0, Pl061IrqHandleNoShare, "GPIO", group);
        }
        if (ret != 0) {
            HDF_LOGE("%s: noshare irq:%u reg fail:%d!", __func__, group->irq, ret);
            return HDF_FAILURE;
        }
        ret = OsalEnableIrq(group->irq);
        if (ret != 0) {
            HDF_LOGE("%s: noshare irq:%u enable fail:%d!", __func__, group->irq, ret);
            (void)OsalUnregisterIrq(group->irq, group);
            return HDF_FAILURE;
        }
        group->irqFunc = Pl061IrqHandleNoShare;
    }

    return HDF_SUCCESS;
}

static void Pl061GpioClearIrqUnsafe(struct Pl061GpioGroup *group, uint16_t bitNum)
{
    unsigned int val;

    val = OSAL_READL(PL061_GPIO_IC(group->regBase));
    val |= 1 << bitNum;
    OSAL_WRITEL(val, PL061_GPIO_IC(group->regBase));
}

static void Pl061GpioSetIrqEnableUnsafe(struct Pl061GpioGroup *group, uint16_t bitNum, int flag)
{
    unsigned int val;
    volatile unsigned char *addr = NULL;

    addr = PL061_GPIO_IE(group->regBase);
    val = OSAL_READL(addr);
    if (flag == 1) {
        val |= (1 << bitNum);
    } else {
        val &= ~(1 << bitNum);
    }
    OSAL_WRITEL(val, addr);
}

static int32_t Pl061GpioEnableIrq(struct GpioCntlr *cntlr, uint16_t local)
{
    unsigned int bitNum = local;
    struct Pl061GpioGroup *group = (struct Pl061GpioGroup *)cntlr;

    if (OsalSpinLockIrqSave(&group->lock, &group->irqSave) != HDF_SUCCESS) {
        return HDF_ERR_DEVICE_BUSY;
    }
    Pl061GpioSetIrqEnableUnsafe(group, bitNum, 1);
    (void)OsalSpinUnlockIrqRestore(&group->lock, &group->irqSave);
    return HDF_SUCCESS;
}

static int32_t Pl061GpioDisableIrq(struct GpioCntlr *cntlr, uint16_t local)
{
    unsigned int bitNum = local;
    struct Pl061GpioGroup *group = (struct Pl061GpioGroup *)cntlr;

    if (OsalSpinLockIrqSave(&group->lock, &group->irqSave) != HDF_SUCCESS) {
        return HDF_ERR_DEVICE_BUSY;
    }
    Pl061GpioSetIrqEnableUnsafe(group, bitNum, 0);
    (void)OsalSpinUnlockIrqRestore(&group->lock, &group->irqSave);
    return HDF_SUCCESS;
}

static void Pl061GpioSetIrqTypeUnsafe(struct Pl061GpioGroup *group, uint16_t bitNum, uint16_t mode)
{
    unsigned int gpioIbe;
    unsigned int gpioIs;
    unsigned int gpioIev;

    if (mode == 0) {
        return;
    }

    gpioIs = OSAL_READL(PL061_GPIO_IS(group->regBase));
    gpioIev = OSAL_READL(PL061_GPIO_IEV(group->regBase));

    if ((mode & (OSAL_IRQF_TRIGGER_LOW | OSAL_IRQF_TRIGGER_HIGH)) != 0) {
        gpioIs |= 1 << bitNum;
        if ((mode & OSAL_IRQF_TRIGGER_HIGH) != 0) {
            gpioIev |= 1 << bitNum;
        } else {
            gpioIev &= ~(1 << bitNum);
        }
    } else {
        gpioIbe = OSAL_READL(PL061_GPIO_IBE(group->regBase));
        gpioIs &= ~(1 << bitNum);
        if (((mode & OSAL_IRQF_TRIGGER_RISING) != 0) && ((mode & OSAL_IRQF_TRIGGER_FALLING) != 0)) {
            gpioIbe |= 1 << bitNum;
        } else if ((mode & OSAL_IRQF_TRIGGER_RISING) != 0) {
            gpioIbe &= ~(1 << bitNum);
            gpioIev |= 1 << bitNum;
        } else if ((mode & OSAL_IRQF_TRIGGER_FALLING) != 0) {
            gpioIbe &= ~(1 << bitNum);
            gpioIev &= ~(1 << bitNum);
        }
        OSAL_WRITEL(gpioIbe, PL061_GPIO_IBE(group->regBase));
    }
    OSAL_WRITEL(gpioIs, PL061_GPIO_IS(group->regBase));
    OSAL_WRITEL(gpioIev, PL061_GPIO_IEV(group->regBase));
}

static int32_t Pl061GpioSetIrq(struct GpioCntlr *cntlr, uint16_t local, uint16_t mode)
{
    int32_t ret;
    unsigned int bitNum = local;
    struct Pl061GpioGroup *group = (struct Pl061GpioGroup *)cntlr;

    if (OsalSpinLockIrqSave(&group->lock, &group->irqSave) != HDF_SUCCESS) {
        return HDF_ERR_DEVICE_BUSY;
    }
    Pl061GpioSetIrqTypeUnsafe(group, bitNum, mode);
    Pl061GpioSetIrqEnableUnsafe(group, bitNum, 0); // disable irq on set
    Pl061GpioClearIrqUnsafe(group, bitNum);        // clear irq on set

    if (group->irqFunc != NULL) {
        (void)OsalSpinUnlockIrqRestore(&group->lock, &group->irqSave);
        HDF_LOGI("%s: group irq(%u) already registered!", __func__, group->irq);
        return HDF_SUCCESS;
    }
    ret = Pl061GpioRegisterGroupIrqUnsafe(group);
    (void)OsalSpinUnlockIrqRestore(&group->lock, &group->irqSave);
    HDF_LOGI("%s: group irq(%u) registered!", __func__, group->irq);
    return ret;
}

static int32_t Pl061GpioUnsetIrq(struct GpioCntlr *cntlr, uint16_t local)
{
    unsigned int bitNum = local;
    struct Pl061GpioGroup *group = (struct Pl061GpioGroup *)cntlr;

    if (OsalSpinLockIrqSave(&group->lock, &group->irqSave) != HDF_SUCCESS) {
        return HDF_ERR_DEVICE_BUSY;
    }
    Pl061GpioSetIrqEnableUnsafe(group, bitNum, 0); // disable irq when unset
    Pl061GpioClearIrqUnsafe(group, bitNum);        // clear irq when unset
    (void)OsalSpinUnlockIrqRestore(&group->lock, &group->irqSave);
    return HDF_SUCCESS;
}

static struct GpioMethod g_method = {
    .request = NULL,
    .release = NULL,
    .write = Pl061GpioWrite,
    .read = Pl061GpioRead,
    .setDir = Pl061GpioSetDir,
    .getDir = Pl061GpioGetDir,
    .toIrq = NULL,
    .setIrq = Pl061GpioSetIrq,
    .unsetIrq = Pl061GpioUnsetIrq,
    .enableIrq = Pl061GpioEnableIrq,
    .disableIrq = Pl061GpioDisableIrq,
};

static int32_t Pl061GpioInitGroups(struct Pl061GpioData *pl061)
{
    int32_t ret;
    uint16_t i;
    struct Pl061GpioGroup *groups = NULL;

    if (pl061 == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }

    groups = (struct Pl061GpioGroup *)OsalMemCalloc(sizeof(*groups) * pl061->groupNum);
    if (groups == NULL) {
        return HDF_ERR_MALLOC_FAIL;
    }
    pl061->groups = groups;

    for (i = 0; i < pl061->groupNum; i++) {
        groups[i].index = i;
        groups[i].regBase = pl061->regBase + i * pl061->regStep;
        groups[i].irq = pl061->irqStart + i;
        groups[i].irqShare = pl061->irqShare;
        groups[i].cntlr.start = i * pl061->bitNum;
        groups[i].cntlr.count = pl061->bitNum;
        groups[i].cntlr.ops = &g_method;
        groups[i].cntlr.ginfos = &pl061->gpioInfo[i * pl061->bitNum];

        if ((ret = OsalSpinInit(&groups[i].lock)) != HDF_SUCCESS) {
            goto ERR_EXIT;
        }

        ret =  GpioCntlrAdd(&groups[i].cntlr);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("%s: err add controller(%hu:%hu):%d", __func__,
                groups[i].cntlr.start, groups[i].cntlr.count, ret);
            (void)OsalSpinDestroy(&groups[i].lock);
            goto ERR_EXIT;
        }
    }
    return HDF_SUCCESS;

ERR_EXIT:
    while (i-- > 0) {
        GpioCntlrRemove(&groups[i].cntlr);
        (void)OsalSpinDestroy(&groups[i].lock);
    }
    pl061->groups = NULL;
    OsalMemFree(groups);
    return ret;
}

static void Pl061GpioUninitGroups(struct Pl061GpioData *pl061)
{
    uint16_t i;
    struct Pl061GpioGroup *group = NULL;

    for (i = 0; i < pl061->groupNum; i++) {
        group = &pl061->groups[i];
        GpioCntlrRemove(&group->cntlr);
    }

    OsalMemFree(pl061->groups);
    pl061->groups = NULL;
}

static int32_t Pl061GpioReadInfo(struct Pl061GpioData *pl061, const struct DeviceResourceNode *node,
    struct DeviceResourceIface *drsOps)
{
    int32_t ret;
    uint32_t gpioIndex = 0;
    const struct DeviceResourceNode *childNode = NULL;
    const char *tempName = NULL;

    DEV_RES_NODE_FOR_EACH_CHILD_NODE(node, childNode) {
        if (gpioIndex >= GPIO_MAX_INFO_NUM) {
            break;
        }
        ret = drsOps->GetString(childNode, "gpioCustomName", &tempName, "");
        if (ret != HDF_SUCCESS) {
            HDF_LOGW("%s: failed to read gpioCustomName!", __func__);
        }
        ret = strcpy_s(pl061->gpioInfo[gpioIndex].name, GPIO_NAME_LEN, tempName);
        if (ret != EOK) {
            HDF_LOGE("%s: failed to copy name for gpioInfo name:%d", __func__, ret);
            return HDF_ERR_IO;
        }

        gpioIndex++;
    }
    return HDF_SUCCESS;
}

static int32_t Pl061GpioReadDrs(struct Pl061GpioData *pl061, const struct DeviceResourceNode *node)
{
    int32_t ret;
    struct DeviceResourceIface *drsOps = NULL;

    drsOps = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    if (drsOps == NULL || drsOps->GetUint32 == NULL || drsOps->GetUint16 == NULL ||
        drsOps->GetUint8 == NULL || drsOps->GetUint8 == NULL || drsOps->GetString == NULL) {
        HDF_LOGE("%s: invalid drs ops!", __func__);
        return HDF_FAILURE;
    }

    ret = drsOps->GetUint32(node, "regBase", &pl061->phyBase, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: failed to read regBase!", __func__);
        return ret;
    }

    ret = drsOps->GetUint32(node, "regStep", &pl061->regStep, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: failed to read regStep!", __func__);
        return ret;
    }

    ret = drsOps->GetUint16(node, "groupNum", &pl061->groupNum, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: failed to read groupNum!", __func__);
        return ret;
    }

    ret = drsOps->GetUint16(node, "bitNum", &pl061->bitNum, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: failed to read bitNum!", __func__);
        return ret;
    }

    ret = drsOps->GetUint32(node, "irqStart", &pl061->irqStart, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: failed to read irqStart!", __func__);
        return ret;
    }

    ret = drsOps->GetUint8(node, "irqShare", &pl061->irqShare, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: failed to read irqShare!", __func__);
        return ret;
    }

    ret = Pl061GpioReadInfo(pl061, node, drsOps);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: failed to read info!", __func__);
        return ret;
    }

    return HDF_SUCCESS;
}

static void Pl061GpioDebug(const struct Pl061GpioData *pl061)
{
#ifdef PL061_GPIO_DEBUG
    uint16_t i;
    struct Pl061GpioGroup *group = NULL;

    HDF_LOGI("%s:groupNum:%hu, bitNum:%hu", __func__, pl061->groupNum, pl061->bitNum);
    for (i = 0; i < pl061->groupNum; i++) {
        group = &pl061->groups[i];
        HDF_LOGI("group[%hu]: index:%u, regBase:0x%x, irq:%u(cntlr:%hu:%hu)",
            i, group->index, group->regBase, group->irq, group->cntlr.start, group->cntlr.count);
    }
#else
    (void)pl061;
#endif
}

static int32_t Pl061GpioBind(struct HdfDeviceObject *device)
{
    (void)device;
    return HDF_SUCCESS;
}

static int32_t Pl061GpioInit(struct HdfDeviceObject *device)
{
    int32_t ret;
    struct Pl061GpioData *pl061 = &g_pl061;

    if (device == NULL || device->property == NULL) {
        HDF_LOGE("%s: device or property null!", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    pl061->gpioInfo = OsalMemCalloc(sizeof(struct GpioInfo) * GPIO_MAX_INFO_NUM);
    if (pl061->gpioInfo == NULL) {
        HDF_LOGE("%s: failed to calloc gpioInfo!", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }

    ret = Pl061GpioReadDrs(pl061, device->property);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: failed to read drs:%d", __func__, ret);
        return ret;
    }

    if (pl061->groupNum > PL061_GROUP_MAX || pl061->groupNum <= 0 ||
        pl061->bitNum > PL061_BIT_MAX || pl061->bitNum <= 0) {
        HDF_LOGE("%s: err groupNum:%hu, bitNum:%hu", __func__, pl061->groupNum, pl061->bitNum);
        return HDF_ERR_INVALID_PARAM;
    }

    pl061->regBase = OsalIoRemap(pl061->phyBase, pl061->groupNum * pl061->regStep);
    if (pl061->regBase == NULL) {
        HDF_LOGE("%s: err remap phy:0x%x", __func__, pl061->phyBase);
        return HDF_ERR_IO;
    }

    ret = Pl061GpioInitGroups(pl061);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: err init groups:%d", __func__, ret);
        OsalIoUnmap((void *)pl061->regBase);
        pl061->regBase = NULL;
        return ret;
    }
    pl061->priv = (void *)device->property;
    device->priv = (void *)pl061;
    Pl061GpioDebug(pl061);

#ifdef PL061_GPIO_USER_SUPPORT
    if (GpioAddVfs(pl061->bitNum) != HDF_SUCCESS) {
        HDF_LOGE("%s: add vfs fail!", __func__);
    }
#endif
    HDF_LOGI("%s: dev service:%s init success!", __func__, HdfDeviceGetServiceName(device));
    return HDF_SUCCESS;
}

static void Pl061GpioRelease(struct HdfDeviceObject *device)
{
    struct Pl061GpioData *pl061 = NULL;

    HDF_LOGI("%s: enter", __func__);
    if (device == NULL) {
        HDF_LOGE("%s: device is null!", __func__);
        return;
    }

#ifdef PL061_GPIO_USER_SUPPORT
    GpioRemoveVfs();
#endif

    pl061 = (struct Pl061GpioData *)device->priv;
    if (pl061 == NULL) {
        HDF_LOGE("%s: device priv is null", __func__);
        return;
    }

    Pl061GpioUninitGroups(pl061);
    OsalMemFree(pl061->gpioInfo);
    pl061->gpioInfo = NULL;
    OsalIoUnmap((void *)pl061->regBase);
    pl061->regBase = NULL;
}

struct HdfDriverEntry g_gpioDriverEntry = {
    .moduleVersion = 1,
    .Bind = Pl061GpioBind,
    .Init = Pl061GpioInit,
    .Release = Pl061GpioRelease,
    .moduleName = "hisi_pl061_driver",
};
HDF_INIT(g_gpioDriverEntry);
