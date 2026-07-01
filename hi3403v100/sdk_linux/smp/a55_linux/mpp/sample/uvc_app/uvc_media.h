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

#ifndef __SAMPLE_UVC_MEDIA_H__
#define __SAMPLE_UVC_MEDIA_H__

#include "ot_common_video.h"
#include "ot_common_vpss.h"
#include "sample_comm.h"
#include "ot_camera.h"

typedef struct {
    sample_sns_type            sns_type;
    ot_size                    sensor_size;

    ot_vi_video_mode           video_mode;
    ot_vi_vpss_mode_type       vi_vpss_mode_type;
    ot_vpss_grp_attr           vpss_grp_attr;
    td_bool                    vpss_chn_enable[OT_VPSS_MAX_PHYS_CHN_NUM];
    ot_vpss_chn_attr           vpss_chn_attr[OT_VPSS_MAX_PHYS_CHN_NUM];
    sample_comm_venc_chn_param venc_chn_param;

    ot_vi_pipe      vi_pipe;
    ot_vi_chn       vi_chn;
    sample_vi_cfg   vi_cfg;
    ot_vpss_grp     vpss_grp;
    ot_vpss_chn     vpss_chn;
    ot_venc_chn     venc_chn;
    td_s32          venc_chn_num;

    td_u32          supplement_cfg;
} uvc_media_cfg;

#ifdef __cplusplus
extern "C" {
#endif

uint16_t _venc_brightness_get(td_void);
uint16_t _venc_contrast_get(td_void);
uint16_t _venc_hue_get(td_void);
uint8_t _venc_power_line_frequency_get(td_void);
uint16_t _venc_saturation_get(td_void);
uint8_t _venc_white_balance_temperature_auto_get(td_void);
uint16_t _venc_white_balance_temperature_get(td_void);
td_void _venc_brightness_set(uint16_t v);
td_void _venc_contrast_set(uint16_t v);
td_void _venc_hue_set(uint16_t v);
td_void _venc_power_line_frequency_set(uint8_t v);
td_void _venc_saturation_set(uint16_t v);
td_void _venc_white_balance_temperature_auto_set(uint8_t v);
td_void _venc_white_balance_temperature_set(uint16_t v);
uint8_t _venc_exposure_auto_mode_get(td_void);
uint32_t _venc_exposure_ansolute_time_get(td_void);
td_void _venc_exposure_auto_mode_set(uint8_t v);
td_void _venc_exposure_ansolute_time_set(uint32_t v);

td_s32 sample_venc_set_idr(td_void);
td_s32 sample_venc_init(td_void);
td_s32 sample_venc_deinit(td_void);
td_s32 sample_venc_startup(td_void);
td_s32 sample_venc_shutdown(td_void);

td_s32 sample_venc_set_property(encoder_property *p);
td_void sample_uvc_get_encoder_property(encoder_property *p);

#ifdef __cplusplus
}
#endif

#endif /* __SAMPLE_UVC_MEDIA_H__ */
