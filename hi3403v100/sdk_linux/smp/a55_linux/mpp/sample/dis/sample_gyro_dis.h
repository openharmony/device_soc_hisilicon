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

#ifndef OT_SAMPLE_GYRO_DIS_H
#define OT_SAMPLE_GYRO_DIS_H

#include "ot_type.h"
#include "ot_common_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 sample_dis_ipc_gyro(ot_vo_intf_type vo_intf_type);

td_s32 sample_dis_dv_gyro(ot_vo_intf_type vo_intf_type);

td_s32 sample_dis_gyro_switch(ot_vo_intf_type vo_intf_type, ot_ldc_version ldc_version);

td_s32 sample_dis_ipc_gyro_two_sensor(ot_size *size);

td_void sample_dis_stop_gyro(td_void);

td_s32 sample_dis_gyro_demo(ot_vo_intf_type vo_intf_type);

td_s32 sample_dis_send(ot_vo_intf_type vo_intf_type, ot_size *input_size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of __cplusplus */

#endif /* __OT_SAMPLE_GYRO_DIS_H__ */

