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
 * Description: header file of alg_gla.c
 */

#ifndef __ALG_GLA_H__
#define __ALG_GLA_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "alg_transplant.h"

#include "hal_device.h"
#include "msg_alg_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
   函数声明
*****************************************************************************/
osal_u32 alg_gla_para_sync_fill(alg_param_sync_stru *data);
oal_bool_enum_uint8 alg_host_get_gla_switch_enable(alg_gla_id_enum_uint32 alg_id,
    alg_gla_switch_type_enum_uint8 gla_switch_type);
osal_u32 alg_set_gal_switch_enable(alg_gla_id_enum_uint32 alg_id, alg_gla_switch_type_enum_uint8 gla_switch_type);
osal_u32 alg_set_gal_switch_disable(alg_gla_id_enum_uint32 alg_id, alg_gla_switch_type_enum_uint8 gla_switch_type);
osal_void alg_hmac_gla_init_usual_switch(osal_void);
osal_void alg_hmac_gla_init_optional_switch(osal_void);
osal_void alg_hmac_gla_init(osal_void);
osal_void alg_hmac_gla_exit(osal_void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
