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

#ifndef AACPARSER_H
#define AACPARSER_H

#include "ot_mw_type.h"

#define FFMPEG_AAC_ADTS_LEN  7
typedef enum {
    FF_AAC_PROFILE_MAIN = 0,
    FF_AAC_PROFILE_LC,
    FF_AAC_PROFILE_SSR,
    FF_AAC_PROFILE_RESERVED,
} FfAacProfile;

typedef struct {
    /* fixed */
    TD_U32 sync;        /* syncword */
    TD_U8 id;            /* MPEG bit - should be 1 */
    TD_U8 layer;         /* MPEG layer - should be 0 */
    TD_U8 protectBit;    /* 0 : CRC word follows, 1 : no CRC word */
    TD_U8 pofile;       /* 0 : main, 1 : LC, 2 : SSR, 3 : reserved */
    TD_U8 sampRateIdx;   /* sample rate index range : [0, 11] */
    TD_U8 privateBit;    /* ignore */
    TD_U8 channelConfig; /* 0 : implicit, >0 : use default table */
    TD_U8 origCopy;      /* 0 : copy, 1 : original */
    TD_U8 home;          /* ignore */

    /* variable */
    TD_U8 copyBit;          /* 1 bit of the 72-bit copyright ID (transmitted as 1 bit per frame) */
    TD_U8 copyStart;        /* 1 : this bit starts the 72-bit ID, 0 : it does not */
    TD_U32 frameLength;    /* length of frame */
    TD_U32 bufferFull;     /* number of 32-bit words left in enc buffer,0x7FF : VBR */
    TD_U8 numRawDataBlocks; /* number of raw data blocks in frame */

    /* CRC */
    TD_S32 crcCheckWord; /* 16-bit CRC check word (present if protectBit : 0) */
} AacAdtsHeader;

typedef struct {
    TD_U32 sync;
    TD_U32 sampleRate;
    TD_U32 channelNum;
    TD_U32 profile;
    TD_U32 frameLength;
} AacMediaInfo;

TD_U32 FfAacParserGenAdts(const AacMediaInfo *aac, TD_U8 *adtsHeadBuf, TD_U8 bufLen);
TD_S32 FfAacParserParseFrame(const TD_U8 *frame, TD_U32 len, AacMediaInfo *aacInfo);
#endif
