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

#ifndef OT_TS_FORMAT_H
#define OT_TS_FORMAT_H
#include "ot_error_def.h"
#include "ot_mw_type.h"
#include "ot_defs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
typedef enum OT_APP_TS_ERR_CODE_E {
    /* general error code */
    OT_APP_TS_ERR_HANDLE_INVALID = 0x40, /** <ts handle invalid */
    OT_APP_TS_ERR_INVALID_ARG = 0x41,    /** <param is null or invalid */
    OT_APP_TS_ERR_MALLOC_FAIL = 0x42,    /** <malloc memory fail */
    OT_APP_TS_ERR_CREATE_MUXER = 0x43,   /** <create ts fail */
    OT_APP_TS_ERR_DESTROY_MUXER = 0x44,  /** <destory ts  fail */
    OT_APP_TS_ERR_CREATE_STREAM = 0x45,  /** <create stream fail */
    OT_APP_TS_ERR_DESTROY_STREAM = 0x46, /** <destory stream  fail */
    OT_APP_TS_ERR_NOT_CREATE = 0x47,     /** <ts not created */
    OT_APP_TS_ERR_READ_FRAME = 0x48,     /** <read frame fail */
    OT_APP_TS_ERR_WRITE_FRAME = 0x49,    /** <write frame fail */

    /* file related error code */
    OT_APP_TS_ERR_UNSUPPORT_CODEC = 0x61, /** <not support codec */
    OT_APP_TS_ERR_OPEN_FILE = 0x62,       /** <open file error */
    OT_APP_TS_ERR_CLOSE_FILE = 0x63,      /** <close file error */
    OT_APP_TS_ERR_READ_FILE = 0x64,       /** <read file error */
    OT_APP_TS_ERR_WRITE_FILE = 0x65,      /** <write file error */
    OT_APP_TS_ERR_FALLOCATE_FILE = 0x66,  /** <fallocate error */

    OT_APP_TS_ERR_BUTT = 0xFF
} OT_APP_TS_ERR_CODE_E;

/* general error code */
#define OT_ERR_TS_NULL_PTR       OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, APP_ERR_NULL_PTR)
#define OT_ERR_TS_HANDLE_INVALID OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_HANDLE_INVALID)
#define OT_ERR_TS_INVALIDARG     OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_INVALID_ARG)
#define OT_ERR_TS_MALLOC         OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_MALLOC_FAIL)
#define OT_ERR_TS_CREATE_MUXER   OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_CREATE_MUXER)
#define OT_ERR_TS_DESTROY_MUXER  OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_DESTROY_MUXER)
#define OT_ERR_TS_CREATE_STREAM  OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_CREATE_STREAM)
#define OT_ERR_TS_DESTROY_STREAM OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_DESTROY_STREAM)
#define OT_ERR_TS_NOT_CREATE     OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_NOT_CREATE)
#define OT_ERR_TS_READ_FRAME     OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_READ_FRAME)
#define OT_ERR_TS_WRITE_FRAME    OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_WRITE_FRAME)

/* FILE related error code */
#define OT_ERR_TS_UNSUPPORT_CODEC OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_UNSUPPORT_CODEC)
#define OT_ERR_TS_OPEN_FILE       OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_OPEN_FILE)
#define OT_ERR_TS_END_FILE        OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_CLOSE_FILE)
#define OT_ERR_TS_READ_FILE       OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_READ_FILE)
#define OT_ERR_TS_WRITE_FILE      OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_WRITE_FILE)
#define OT_ERR_TS_FALLOCATE_FILE  OT_APP_DEF_ERR(OT_APPID_TS, APP_ERR_LEVEL_ERROR, OT_APP_TS_ERR_FALLOCATE_FILE)

static const char * const MODULE_NAME_TS = "TS";
#define OT_TS_MAX_AMOUNT        (4)                 /* max ts amount */
#define OT_TS_MAX_STREAM_AMOUNT (6)                 /* max ts track amount */
#define OT_TS_MAX_FILE_NAME     (256)               /* file name path max */
#define OT_TS_VBUF_MAX_SIZE     (5 * 1024 * 1024)   /* VBUF size max */
#define OT_TS_PREALLOC_MAX_SIZE (100 * 1024 * 1024) /* pre allocate unit size max */

typedef enum TS_CONFIG_TYPE_E {
    OT_TS_CONFIG_MUXER = 1,
    OT_TS_CONFIG_DEMUXER,
    OT_TS_CONFIG_BUTT
} OT_TS_CONFIG_TYPE_E;

typedef enum TS_STREAM_TYPE_E {
    OT_TS_STREAM_VIDEO = 1,
    OT_TS_STREAM_AUDIO,
    OT_TS_STREAM_DATA,
    OT_TS_STREAM_BUTT
} OT_TS_STREAM_TYPE_E;

typedef enum TS_CODECID_E {
    OT_TS_CODEC_ID_H264 = 1,
    OT_TS_CODEC_ID_H265,
    OT_TS_CODEC_ID_AAC,
    OT_TS_CODEC_ID_MP3,
    OT_TS_CODEC_ID_JPG,
    OT_TS_CODEC_ID_GPS,
    OT_TS_CODEC_ID_BUTT
} OT_TS_CODECID_E;

typedef struct TS_STREAM_INFO_S {
    OT_TS_CODECID_E enCodecID; /* codec type */
    TD_FLOAT fSpeed;           /* play speed, (0,1]for slow,(1,~) for fast */
} OT_TS_STREAM_INFO_S;

typedef struct TS_FRAME_DATA_S {
    TD_U64 u64TimeStamp;   /* frame timestamp */
    TD_U8 *pu8DataBuffer;  /* frame data buffer */
    TD_U32 u32DataLength;  /* frame date len */
    TD_BOOL bKeyFrameFlag; /* key frame flag, used for video */
} OT_TS_FRAME_DATA_S;

typedef struct TS_MXUER_CONFIG_S {
    /* pre allocate size in bytes,
    [0,100M],0 for not use pre allocate function,suggest 20M,
    unit :byte */
    TD_U32 u32PreAllocUnit;
    TD_U32 u32VBufSize;     /* vbuf size for fwrite (0,5M] unit :byte */
} OT_TS_MUXER_CONFIG_S;

typedef struct TS_DEMUXER_CONFIG_S {
    TD_U32 u32VBufSize; /* set the vbuf size for fwrite (0,5M] unit :byte */
} OT_TS_DEMUXER_CONFIG_S;

typedef struct TS_CONFIG_S {
    TD_CHAR aszFileName[OT_TS_MAX_FILE_NAME]; /* file path and file name */
    OT_TS_CONFIG_TYPE_E enConfigType;
    union {
        OT_TS_MUXER_CONFIG_S stMuxerConfig;
        OT_TS_DEMUXER_CONFIG_S stDemuxerConfig;
    };
} OT_TS_CONFIG_S;

/**
 * @brief create TS.
 * @param[out] ppTs TD_MW_PTR* : return TS object
 * @param[in] pstTsCfg OT_TS_CONFIG_S* : ts configure
 * @return   0 success
 * @return  err number  failure
 */
TD_S32 SS_TS_Create(const OT_TS_CONFIG_S *pstTsCfg, TD_MW_PTR *ppTsMuxer);

/**
 * @brief destroy TS.
 * @param[in] pTs TD_MW_PTR :  TS object
 * @return   0 success
 * @return  err number  failure
 */

TD_S32 SS_TS_Destroy(TD_MW_PTR pTsMuxer);

/**
 * @brief create TS stream.
 * @param[in] pTs TD_MW_PTR : TS object
 * @param[out] ppStream TD_MW_PTR* : return stream object
 * @param[in] pstStreamInfo OT_TS_STREAM_INFO_S* : track info
 * @return   0 success
 * @return  err number  failure
 */
TD_S32 SS_TS_CreateStream(TD_MW_PTR pTsMuxer, TD_MW_PTR *ppStream, const OT_TS_STREAM_INFO_S *pstStreamInfo);

/**
 * @brief destroy all TS stream.
 * @param[in] pTs TD_MW_PTR :  TS object
 * @return   0 success
 * @return  err number  failure
 */
TD_S32 SS_TS_DestroyAllStreams(TD_MW_PTR pTsMuxer);

/**
 * @brief write TS frame data.
 * @param[in] pTs TD_MW_PTR : TS object
 * @param[in] pStream TD_MW_PTR : stream object
 * @param[in] pstFrameData OT_TS_FRAME_DATA_S* : frame date information
 * @return   0 success
 * @return  err number  failure
 */
TD_S32 SS_TS_WriteFrame(TD_MW_PTR pTsMuxer, TD_MW_PTR pStream, const OT_TS_FRAME_DATA_S *pstFrameData);

/**
 * @brief Get ts file size.
 * @param[in] pTs TD_MW_PTR : TS object
 * @return   file size
 * @return  err number  failure
 */
TD_U64 SS_TS_GetFileSize(TD_MW_PTR pTsMuxer);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __OT_TS_FORMAT_H__ */
