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
 *
 * Description: acodec
 */

#ifndef AUDIO_PORT_PLUGIN_COMMON_H
#define AUDIO_PORT_PLUGIN_COMMON_H
#include <stdbool.h>
#include "ot_common_aio.h"
#include "ss_confaccess.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define ACODEC_FILE     "/dev/acodec"

#define AUDIO_STEREO_SOUND_MODE_CHN_CNT 2
#define AUDIO_MONO_SOUND_MODE_CHN_CNT   1

/* number of sampling points in per frame */
#define AUDIO_AAC_PCM_PTNUMPERFRM 1024
/* samples per frame for G711 and G726 */
#define AUDIO_G711_G726_PTNUMPERFRM 480

#define ADUIO_VQE_INI_FILE_PATH "/storage/data/audio_vqe.ini"

/* product ini identification */
#define PDT_INIPARAM "productini"
#define PDT_INIPARAM_PATH "/storage/data/media_config.ini"
#define AUDIO_VQE_MODULE "audio_vqe"

int32_t AudioAcodecInit(ot_audio_sample_rate enSampleRate);

int32_t AudioAcodecDeInit(void);

int32_t AudioAcodecSetVol(int32_t s32Vol);

int32_t AudioAcodecGetVol(int32_t *s32Vol);

int32_t AudioAcodecMute(void);

int32_t AudioAcodecUnMute(void);

bool AudioConvertSampleRateToHAL(int32_t sampleRate, ot_audio_sample_rate *enSamplerate);

bool AudioConvertChannelToHAL(int32_t channel, ot_audio_snd_mode *enSoundMode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif
