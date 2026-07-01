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

#include "display_device.h"
#include <errno.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <securec.h>
#include "disp_common.h"
#include "disp_hal.h"
HalFuncs *g_hFuncs = NULL;

static int32_t SetDisplayPowerStatus(uint32_t devId, DispPowerStatus status)
{
    int32_t ret = DISPLAY_FAILURE;
    if (g_hFuncs && g_hFuncs->SetPowerStatus) {
        ret = g_hFuncs->SetPowerStatus(devId, status);
    }
    return ret;
}

static int32_t GetDisplayPowerStatus(uint32_t devId, DispPowerStatus *status)
{
    int32_t ret = DISPLAY_FAILURE;
    if (g_hFuncs && g_hFuncs->GetPowerStatus) {
        ret = g_hFuncs->GetPowerStatus(devId, status);
    }
    return ret;
}

static int32_t SetDisplayBacklight(uint32_t devId, uint32_t level)
{
    int32_t ret = DISPLAY_FAILURE;
    if (g_hFuncs && g_hFuncs->SetBacklight) {
        ret = g_hFuncs->SetBacklight(devId, level);
    }
    return ret;
}

static int32_t GetDisplayBacklight(uint32_t devId, uint32_t *level)
{
    int32_t ret = DISPLAY_FAILURE;
    if (g_hFuncs && g_hFuncs->GetBacklight) {
        ret = g_hFuncs->GetBacklight(devId, level);
    }
    return ret;
}

int32_t DeviceInitialize(DeviceFuncs **funcs)
{
    if (funcs == NULL) {
        HDF_LOGE("%s: funcs is null", __func__);
        return DISPLAY_NULL_PTR;
    }
    DeviceFuncs *dFuncs = (DeviceFuncs *)malloc(sizeof(DeviceFuncs));
    if (dFuncs == NULL) {
        HDF_LOGE("%s: dFuncs is null", __func__);
        return DISPLAY_NULL_PTR;
    }
    (void)memset_s(dFuncs, sizeof(DeviceFuncs), 0, sizeof(DeviceFuncs));
    dFuncs->SetDisplayPowerStatus = SetDisplayPowerStatus;
    dFuncs->GetDisplayPowerStatus = GetDisplayPowerStatus;
    dFuncs->SetDisplayBacklight = SetDisplayBacklight;
    dFuncs->GetDisplayBacklight = GetDisplayBacklight;
    *funcs = dFuncs;

    if (g_hFuncs == NULL) {
        g_hFuncs = GetHalFuncs();
    }

    return DISPLAY_SUCCESS;
}

int32_t DeviceUninitialize(DeviceFuncs *funcs)
{
    if (g_hFuncs != NULL) {
        free(g_hFuncs);
        g_hFuncs = NULL;
    }
    if (funcs == NULL) {
        HDF_LOGE("%s: funcs is null", __func__);
        return DISPLAY_NULL_PTR;
    }
    free(funcs);
    HDF_LOGI("%s: device uninitialize success", __func__);
    return DISPLAY_SUCCESS;
}
