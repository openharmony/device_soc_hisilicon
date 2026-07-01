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

#ifndef __AUDIO_AAC_INTF_H__
#define __AUDIO_AAC_INTF_H__

#include <stdio.h>

#include "ss_mpi_audio.h"
#include "ot_common_aenc.h"
#include "ot_common_adec.h"
#include "ot_common_aio.h"
#include "ot_aacenc.h"
#include "ot_aacdec.h"
#include "codec_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* samples per frame for OT_AACLC and aacPlus */
#define AACLD_SAMPLES_PER_FRAME         512
#define AACLC_SAMPLES_PER_FRAME         1024
#define AACPLUS_SAMPLES_PER_FRAME       2048

/* max length of AAC stream by bytes */
#define MAX_AAC_MAINBUF_SIZE    (768 * 2)

typedef enum {
    AAC_TYPE_AACLC    = 0, /* AAC LC */
    AAC_TYPE_EAAC     = 1, /* eAAC  (HEAAC or AAC+  or aacPlusV1) */
    AAC_TYPE_EAACPLUS = 2, /* eAAC+ (AAC++ or aacPlusV2) */
    AAC_TYPE_AACLD    = 3,
    AAC_TYPE_AACELD   = 4,
    AAC_TYPE_BUTT,
} AacType;

typedef enum {
    AAC_BPS_8K   = 8000,
    AAC_BPS_16K  = 16000,
    AAC_BPS_22K  = 22000,
    AAC_BPS_24K  = 24000,
    AAC_BPS_32K  = 32000,
    AAC_BPS_48K  = 48000,
    AAC_BPS_64K  = 64000,
    AAC_BPS_96K  = 96000,
    AAC_BPS_128K = 128000,
    AAC_BPS_132K = 132000,
    AAC_BPS_144K = 144000,
    AAC_BPS_192K = 192000,
    AAC_BPS_256K = 256000,
    AAC_BPS_265K = 265000,
    AAC_BPS_288K = 288000,
    AAC_BPS_320K = 320000,
    AAC_BPS_BUTT
} AacBsp;

typedef enum {
    AAC_TRANS_TYPE_ADTS = 0,
    AAC_TRANS_TYPE_LOAS = 1,
    AAC_TRANS_TYPE_LATM_MCP1 = 2,
    AAC_TRANS_TYPE_BUTT
}AacTransType;


typedef struct {
    td_s32 s32Samplerate; /* sample rate */
    td_s32 s32BitRate;    /* bitrate */
    td_s32 s32Profile;    /* profile */
    td_s32 s32TnsUsed;    /* TNS Tools */
    td_s32 s32PnsUsed;    /* PNS Tools */
} AacFrameInfo;

/*
    AAC Commendatory Parameter:
    Sampling Rate(HZ)    LC BitRate(Kbit/s)    OT_EAAC BitRate (Kbit/s)    OT_EAAC+ BitRate (Kbit/s)
    48000                128                    48                        32,24
    44100                128                    48                        32,24
    32000                96                    22                        16
    24000                64
    22050                64
    16000                48
*/

typedef struct {
    AacType          enAACType;   /* AAC profile type */
    /* AAC bitrate (LC:16~320, OT_EAAC:24~128, OT_EAAC+:16~64, OT_AACLD:16~320, OT_AACELD:32~320) */
    AacBsp           enBitRate;
    /* AAC sample rate (LC:8~48, OT_EAAC:16~48, OT_EAAC+:16~48, OT_AACLD:8~48, OT_AACELD:8~48) */
    ot_audio_sample_rate enSmpRate;
    ot_audio_bit_width   enBitWidth;  /* AAC bit width (only support 16bit) */
    ot_audio_snd_mode  enSoundMode; /* sound mode of inferent audio frame */
    AacTransType    enTransType;
    td_s16              s16BandWidth; /* targeted audio bandwidth in Hz (0 or 1000~enSmpRate/2), the default is 0 */
}AencAacAttr;

typedef struct {
    ot_aac_encoder       *pstAACState;
    AencAacAttr     stAACAttr;
} AencAacEncoder;

typedef struct {
    AacTransType enTransType;
}AdecAacAttr;

typedef struct {
    ot_aac_decoder         pstAACState;
    AdecAacAttr     stAACAttr;
} AdecAacDecoder;

td_s32 HiMpiAencAacInit(void);

td_s32 HiMpiAencAacDeInit(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_AAC_INTF_H__ */

