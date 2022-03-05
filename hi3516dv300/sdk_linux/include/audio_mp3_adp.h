/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#ifndef __AUDIO_MP3_ADP_H__
#define __AUDIO_MP3_ADP_H__

#include <stdio.h>

#include "hi_comm_aio.h"
#include "mpi_audio.h"
#include "hi_comm_adec.h"
#include "mp3dec.h"

/* max length of MP3 stream by bytes */
#define MAX_MP3_MAINBUF_SIZE    2048*2
#define MP3_SAMPLES_PER_FRAME   1152
#define NO_ENOUGH_MAIN_DATA_ERROR 11

typedef enum hiMP3_BPS_E {
    MP3_BPS_32K     = 32000,
    MP3_BPS_40K     = 40000,
    MP3_BPS_48K     = 48000,
    MP3_BPS_56K     = 56000,
    MP3_BPS_64K     = 64000,
    MP3_BPS_80K     = 80000,
    MP3_BPS_96K     = 96000,
    MP3_BPS_112K    = 112000,
    MP3_BPS_128K    = 128000,
    MP3_BPS_160K    = 160000,
    MP3_BPS_192K    = 192000,
    MP3_BPS_224K    = 224000,
    MP3_BPS_256K    = 256000,
    MP3_BPS_320K    = 320000,
} MP3_BPS_E;

typedef enum hiMP3_LAYER_E {
    MP3_LAYER_1     = 1,
    MP3_LAYER_2     = 2,
    MP3_LAYER_3     = 3,
} MP3_LAYER_E;

typedef enum hiMP3_VERSION_E {
    MPEG_1          = 1,
    MPEG_2          = 0,
    MPEG_25         = 2,
} MP3_VERSION_E;

typedef struct hiMP3_FRAME_INFO_S {
    HI_S32 s32BitRate;
    HI_S32 s32Chans;                /* 1 or 2 */
    HI_S32 s32SampRate;
    HI_S32 s32BitsPerSample;        /* only support 16 bits */
    HI_S32 s32OutPutSamps;          /* nChans*SamplePerFrame */
    HI_S32 s32Layer;                /* layer,the infomation of stream's encode profile */
    HI_S32 s32Version;              /* version,the infomation of stream's encode profile */
} MP3_FRAME_INFO_S;

typedef struct hiADEC_ATTR_MP3_S {
    HI_U32 resv;
} ADEC_ATTR_MP3_S;

typedef struct hiADEC_MP3_DECODER_S {
    ADEC_ATTR_MP3_S     stMP3Attr;
    HMP3Decoder         pstMP3State;
} ADEC_MP3_DECODER_S;

HI_S32 HI_MPI_ADEC_Mp3Init(HI_VOID);

#endif

