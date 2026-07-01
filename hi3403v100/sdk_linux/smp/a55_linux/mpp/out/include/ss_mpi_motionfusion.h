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
#ifndef __SS_MPI_MOTIONFUSION_H__
#define __SS_MPI_MOTIONFUSION_H__

#include "ot_type.h"
#include "ot_common_motionfusion.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MOTIONFUSION_DEVICE_ID_0 0
#define MOTIONFUSION_DEVICE_ID_1 1

td_s32 ss_mpi_mfusion_set_attr(const td_u32 fusion_id, const ot_mfusion_attr *mfusion_attr);
td_s32 ss_mpi_mfusion_get_attr(const td_u32 fusion_id, ot_mfusion_attr *mfusion_attr);

td_s32 ss_mpi_mfusion_set_gyro_drift(const td_u32 fusion_id, const ot_mfusion_drift *drift);
td_s32 ss_mpi_mfusion_get_gyro_drift(const td_u32 fusion_id, ot_mfusion_drift *drift);

td_s32 ss_mpi_mfusion_set_gyro_six_side_calibration(const td_u32 fusion_id,
    const ot_mfusion_six_side_calibration *six_side_calibration);
td_s32 ss_mpi_mfusion_get_gyro_six_side_calibration(const td_u32 fusion_id,
    ot_mfusion_six_side_calibration *six_side_calibration);

td_s32 ss_mpi_mfusion_set_gyro_temperature_drift(const td_u32 fusion_id,
    const ot_mfusion_temperature_drift *temperature_drift);
td_s32 ss_mpi_mfusion_get_gyro_temperature_drift(const td_u32 fusion_id,
    ot_mfusion_temperature_drift *temperature_drift);

td_s32 ss_mpi_mfusion_set_gyro_online_temperature_drift(const td_u32 fusion_id,
    const ot_mfusion_temperature_drift *temperature_drift);
td_s32 ss_mpi_mfusion_get_gyro_online_temperature_drift(const td_u32 fusion_id,
    ot_mfusion_temperature_drift *temperature_drift);

td_s32 ss_mpi_mfusion_set_gyro_online_drift(const td_u32 fusion_id, const ot_mfusion_drift *drift);
td_s32 ss_mpi_mfusion_get_gyro_online_drift(const td_u32 fusion_id, ot_mfusion_drift *drift);

#ifdef __cplusplus
}
#endif

#endif /* End of #ifndef __SS_MPI_MONTIONFUSION_H__ */
