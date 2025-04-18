/*
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
 */
#ifndef _LIUNX_DEVICE_H
#define _LIUNX_DEVICE_H

#include "linux/pm.h"
#include "los_driverbase.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct device_driver;
struct dev_pm_op;

/* device define */
/**
 * @ingroup driver
 * Define the structure of the parameters used for device creation.
 */
struct device {
    struct LosDevice losdevice;
    const CHAR *name;
    VOID *driver_data;
    VOID *platform_data;            /**< Platform specific data, device core doesn't touch it */
    struct dev_pm_info power;
};

/* driver define */
/**
 * @ingroup driver
 * Define the structure of the parameters used for driver creation.
 */
struct device_driver {
    struct LosDriver losdriver;
    const CHAR *name;
    const struct dev_pm_op *pm;
};

struct dev_pm_op {
    INT32 (*suspend)(struct device *ptr);
    INT32 (*resume)(struct device *ptr);
    INT32 (*prepare)(struct device *dev);
    INT32 (*complete)(struct device *dev);
};

STATIC INLINE VOID *dev_get_platdata(const struct device *dev)
{
    if (dev == NULL) {
        PRINT_WARN("dev_get_platdata :the input dev is NULL!\n");
        return NULL;
    }
    return dev->platform_data;
}

STATIC INLINE VOID dev_set_platdata(struct device *dev, VOID *data)
{
    if (dev == NULL) {
        PRINT_WARN("dev_set_platdata :the input dev is NULL!\n");
        return;
    }
    dev->platform_data = data;
}

STATIC INLINE VOID *dev_get_drvdata(const struct device *dev)
{
    if (dev == NULL) {
        PRINT_WARN("dev_get_drvdata :the input dev is NULL!\n");
        return NULL;
    }
    return dev->driver_data;
}

STATIC INLINE VOID dev_set_drvdata(struct device *dev, VOID *data)
{
    if (dev == NULL) {
        PRINT_WARN("dev_set_drvdata :the input dev is NULL!\n");
        return;
    }
    dev->driver_data = data;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
