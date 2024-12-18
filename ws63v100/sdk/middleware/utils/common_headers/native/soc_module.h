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
 * Description: Module ID definition.
 */

#ifndef __SOC_MODULE_H__
#define __SOC_MODULE_H__

#define SOC_INVALID_MODULE_ID (0xffffffff)

typedef enum {
    /* common */ /* CNcomment: 系统通用模块 */
    SOC_ID_RESERVED = 0x00,
    SOC_ID_LIBC = 0x01,
    SOC_ID_BSP = 0x02,
    SOC_ID_SYS = 0x03,
    SOC_ID_MAIN = 0x04,
    
    /* Peripheral */ /* CNcomment: 外设相关模块 */
    SOC_ID_VAU = 0x05,
    SOC_ID_DPU = 0x06,
    SOC_ID_AIDSP = 0x07,
    SOC_ID_CGRA = 0x08,

    /* Component */ /* CNcomment: 组件相关模块 */
    DIAG_MOD_ID_WIFI_HOST               = 0x00,
    DIAG_MOD_ID_WIFI_DEVICE             = 0x00,
    DIAG_MOD_ID_OAM_HOST                = 0x00,

    DIAG_MOD_ID_PLC_NM                  = 0x20,

    SOC_ID_MAX = 0x3F
} soc_mod_id;

#endif /* __SOC_MODULE_H__ */

