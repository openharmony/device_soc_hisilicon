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

#include "sample_venc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include "sample_comm.h"
#include "ot_stream.h"
#include "uvc_media.h"

/* Stream Control Operation Functions End */
static struct processing_unit_ops g_venc_pu_ops = {
    .brightness_get = _venc_brightness_get,
    .contrast_get = _venc_contrast_get,
    .hue_get = _venc_hue_get,
    .power_line_frequency_get = _venc_power_line_frequency_get,
    .saturation_get = _venc_saturation_get,
    .white_balance_temperature_auto_get = _venc_white_balance_temperature_auto_get,
    .white_balance_temperature_get = _venc_white_balance_temperature_get,

    .brightness_set = _venc_brightness_set,
    .contrast_set = _venc_contrast_set,
    .hue_set = _venc_hue_set,
    .power_line_frequency_set = _venc_power_line_frequency_set,
    .saturation_set = _venc_saturation_set,
    .white_balance_temperature_auto_set = _venc_white_balance_temperature_auto_set,
    .white_balance_temperature_set = _venc_white_balance_temperature_set,
};

static struct input_terminal_ops g_venc_it_ops = {
    .exposure_ansolute_time_get = _venc_exposure_ansolute_time_get,
    .exposure_auto_mode_get = _venc_exposure_auto_mode_get,
    .exposure_ansolute_time_set = _venc_exposure_ansolute_time_set,
    .exposure_auto_mode_set = _venc_exposure_auto_mode_set,
};

static struct stream_control_ops g_venc_sc_ops = {
    .init = sample_venc_init,
    .deinit = sample_venc_deinit,
    .startup = sample_venc_startup,
    .shutdown = sample_venc_shutdown,
    .set_idr = sample_venc_set_idr,
    .set_property = sample_venc_set_property,
};

td_void sample_venc_config(td_void)
{
    printf("\n@@@@@ UVC App Sample @@@@@\n\n");

    ot_stream_register_mpi_ops(&g_venc_sc_ops, &g_venc_pu_ops, &g_venc_it_ops, TD_NULL);
}
