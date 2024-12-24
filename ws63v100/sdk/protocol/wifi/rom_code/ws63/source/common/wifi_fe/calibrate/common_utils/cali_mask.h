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
 * Description: 校准mask相关
 */

#ifndef __CALI_MASK_H__
#define __CALI_MASK_H__

#include "cali_outer_os_if.h"
#include "cali_outer_hal_if.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#define CALI_DATA_MASK_UPLOAD_ON 0x1  // 校准后数据上报
#define CALI_DATA_MASK_DOWNLOAD_ON 0x2  // 接收下发的数据
#define CALI_DATA_MASK_ALL_CALI 0x4  // 执行上电校准
#define CALI_DATA_MASK_WITHOUT_PWR 0x8 // 不做上电功率校准
#define CALI_DATA_MASK_ONLINE_CALI 0x10 // 动态校准开关
#define CALI_DATA_MASK_DEFAULT 0x17 // 默认开启上电功率 动态校准

osal_void cali_set_cali_mask(const osal_u8 *param);
osal_u32 cali_get_cali_mask(wlan_channel_band_enum_uint8 band);
osal_void cali_set_cali_data_mask(const osal_u8 *param);
osal_u8 cali_get_cali_data_mask(osal_void);
osal_void cali_set_cali_done_flag(osal_u8 is_done);
osal_u8 cali_get_cali_done_flag(osal_void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
