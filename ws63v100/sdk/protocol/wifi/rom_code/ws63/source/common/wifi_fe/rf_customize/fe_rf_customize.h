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
 * Description: rf customize相关
 */

#ifndef _FE_RF_CUSTOMIZE_H_
#define _FE_RF_CUSTOMIZE_H_

#include "cali_outer_hal_if.h"
#include "cali_outer_os_if.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
// rf相关的定制化参数结构体
typedef struct {
    /* cca ed thr */
    hal_cfg_custom_cca_stru cfg_cus_cca;
} fe_rf_custom_stru;
#endif

// 获取定制化数据中的cca门限默认值
osal_s8 fe_rf_get_customized_cca_ed(osal_u32 cca_ed_type, wlan_channel_band_enum_uint8 band);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
