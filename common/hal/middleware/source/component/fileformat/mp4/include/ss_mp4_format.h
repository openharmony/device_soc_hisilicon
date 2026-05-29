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
#ifndef OT_MP4_FORMAT_H
#define OT_MP4_FORMAT_H

#include "ot_mw_type.h"
#include "ot_error_def.h"
#include "ot_defs.h"
#include "ss_mp4_format.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
typedef enum OT_APP_MP4_ERR_CODE_E {
    /* general error code */
    OT_APP_MP4_ERR_HANDLE_INVALID = 0x40, /** <mp4 handle invalid */
    OT_APP_MP4_ERR_INVALID_ARG = 0x41,    /** <param is null or invalid */
    OT_APP_MP4_ERR_MALLOC_FAIL = 0x42,    /** <malloc memory fail */
    OT_APP_MP4_ERR_CREATE_MUXER = 0x43,   /** <create mp4 fail */
    OT_APP_MP4_ERR_DESTROY_MUXER = 0x44,  /** <destory mp4  fail */
    OT_APP_MP4_ERR_WRITE_HEAD = 0x45,     /** <start mp4 fail */
    OT_APP_MP4_ERR_WRITE_TAIL = 0x46,     /** <stop mp4 fail */
    OT_APP_MP4_ERR_CREATE_AGAIN = 0x47,   /** <mp4 re created */
    OT_APP_MP4_ERR_NOT_CREATE = 0x48,     /** <mp4 not created */
    OT_APP_MP4_ERR_READ_FRAME = 0x49,     /** <read frame fail */
    OT_APP_MP4_ERR_WRITE_FRAME = 0x50,    /** <write frame fail */
    OT_APP_MP4_ERR_REPAIR = 0x51,         /** <write frame fail */
    OT_APP_MP4_ERR_CREATE_STREAM = 0x52,  /** <create stream fail */
    OT_APP_MP4_ERR_DESTROY_STREAM = 0x53, /** <destory stream  fail */
    OT_APP_MP4_ERR_GET_ATOM = 0x54,       /** <get atom fail */
    OT_APP_MP4_ERR_ADD_ATOM = 0x55,       /** <add atom  fail */
    OT_APP_MP4_ERR_ATOM_LEN = 0x56,       /** <add atom  fail */
    OT_APP_MP4_ERR_DESTROY_REPAIR = 0x59, /** <destory back  fail */
    OT_APP_MP4_ERR_CREATE_REPAIR = 0x60,  /** <create back again */

    /* file related error code */
    OT_APP_MP4_ERR_OPETATION_FAIL = 0x61,     /** <mp4 repair fail */
    OT_APP_MP4_ERR_UNSUPPORT_CODEC = 0x62,    /** <not support codec */
    OT_APP_MP4_ERR_OPEN_FILE = 0x63,          /** <open file error */
    OT_APP_MP4_ERR_CLOSE_FILE = 0x64,         /** <close file error */
    OT_APP_MP4_ERR_READ_FILE = 0x65,          /** <read file error */
    OT_APP_MP4_ERR_SEEK_FILE = 0x66,          /** <seek file error */
    OT_APP_MP4_ERR_WRITE_FILE = 0x67,         /** <write file error */
    OT_APP_MP4_ERR_REMOVE_FILE = 0x68,        /** <remove file error */
    OT_APP_MP4_ERR_FILE_SIZE_OVERFLOW = 0x69, /** <file size overflow error */

    OT_APP_MP4_ERR_BUTT = 0xFF
} OT_APP_MP4_ERR_CODE_E;

/* general error code */
#define OT_ERR_MP4_NULL_PTR       OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, APP_ERR_NULL_PTR)
#define OT_ERR_MP4_HANDLE_INVALID OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_HANDLE_INVALID)
#define OT_ERR_MP4_INVALIDARG     OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_INVALID_ARG)
#define OT_ERR_MP4_MALLOC         OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_MALLOC_FAIL)
#define OT_ERR_MP4_CREATE         OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_CREATE_MUXER)
#define OT_ERR_MP4_DESTROY        OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_DESTROY_MUXER)
#define OT_ERR_MP4_WRITE_HEAD     OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_WRITE_HEAD)
#define OT_ERR_MP4_WRITE_TAIL     OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_WRITE_TAIL)
#define OT_ERR_MP4_CREATE_AGAIN   OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_CREATE_AGAIN)
#define OT_ERR_MP4_NOT_CREATE     OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_NOT_CREATE)
#define OT_ERR_MP4_READ_FRAME     OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_READ_FRAME)
#define OT_ERR_MP4_WRITE_FRAME    OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_WRITE_FRAME)
#define OT_ERR_MP4_REPAIR_UPDATE  OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_REPAIR)
#define OT_ERR_MP4_CREATE_STREAM  OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_CREATE_STREAM)
#define OT_ERR_MP4_DESTROY_STREAM OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_DESTROY_STREAM)
#define OT_ERR_MP4_GET_ATOM       OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_GET_ATOM)
#define OT_ERR_MP4_ADD_ATOM       OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_ADD_ATOM)
#define OT_ERR_MP4_ATOM_LEN       OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_ATOM_LEN)

/* FILE related error code */
#define OT_ERR_MP4_OPERATION_FAILED   OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_OPETATION_FAIL)
#define OT_ERR_MP4_UNSUPPORT_CODEC    OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_UNSUPPORT_CODEC)
#define OT_ERR_MP4_OPEN_FILE          OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_OPEN_FILE)
#define OT_ERR_MP4_END_FILE           OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_CLOSE_FILE)
#define OT_ERR_MP4_READ_FILE          OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_READ_FILE)
#define OT_ERR_MP4_SEEK_FILE          OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_SEEK_FILE)
#define OT_ERR_MP4_WRITE_FILE         OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_WRITE_FILE)
#define OT_ERR_MP4_REMOVE_FILE        OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, OT_APP_MP4_ERR_REMOVE_FILE)
#define OT_ERR_MP4_FILE_SIZE_OVERFLOW OT_APP_DEF_ERR(OT_APPID_MP4, APP_ERR_LEVEL_ERROR, \
    OT_APP_MP4_ERR_FILE_SIZE_OVERFLOW)

/* To comply with Clean Code principles, define some constants in an enum. */
enum MP4_NUM_TYPE_PARAM  {
    OT_MP4_MAX_HDLR_NAME = 256,   /* handler name max length 255, one for '\0' */
    OT_MP4_TRACK_NUM_MAX = 6,
    OT_MP4_MAX_BOX_PATH = 64,     /* atom path max length */
    OT_MP4_MAX_FILE_NAME = 256,   /* file name path max */
    OT_MP4_TYPE_SIZE = 4,         /* box type length */
    OT_MODE_SIZE = 4              /* fopen mode size */
};

typedef enum MP4_CONFIG_TYPE_E {
    OT_MP4_CONFIG_MUXER = 1,
    OT_MP4_CONFIG_DEMUXER,
    OT_MP4_CONFIG_BUTT
} OT_MP4_CONFIG_TYPE_E;

typedef enum MP4_FORMAT_PROFILE_E {
    OT_MP4_FORMAT_MP42 = 0,  // base media/version 2
    OT_MP4_FORMAT_QT,        // quicktime movie,
    OT_MP4_FORMAT_BUTT
} OT_MP4_FORMAT_PROFILE_E;

typedef enum MP4_TRACK_TYPE_E {
    OT_MP4_STREAM_VIDEO = 1,
    OT_MP4_STREAM_AUDIO,
    OT_MP4_STREAM_DATA,
    OT_MP4_STREAM_BUTT
} OT_MP4_TRACK_TYPE_E;

typedef enum MP4_CODECID_E {
    OT_MP4_CODEC_ID_H264 = 0,
    OT_MP4_CODEC_ID_H265,
    OT_MP4_CODEC_ID_MJPEG,
    OT_MP4_CODEC_ID_PRORES_422_PROXY,  // apco
    OT_MP4_CODEC_ID_PRORES_422_LT,     // apcs
    OT_MP4_CODEC_ID_PRORES_422,        // apcn
    OT_MP4_CODEC_ID_PRORES_422_HQ,     // apch
    OT_MP4_CODEC_ID_AACLC,
    OT_MP4_CODEC_ID_MP3,
    OT_MP4_CODEC_ID_G726,
    OT_MP4_CODEC_ID_G711_A,
    OT_MP4_CODEC_ID_G711_U,
    OT_MP4_CODEC_ID_PCM,
    OT_MP4_CODEC_ID_BUTT
} OT_MP4_CODECID_E;

typedef enum  MP4_MOOV_LOCATION_E {
    OT_MP4_MOOV_AT_TAIL = 0, /* write moov after mdat */
    OT_MP4_MOOV_AT_FRONT,    /* write moov before mdat, and user need to set nonzero fileDuration */
    OT_MP4_MOOV_BUTT
} OT_MP4_MOOV_LOCATION_E;

typedef enum MP4_ROTATION_ANG {
    OT_MP4_ROTATION_0 = 0,      /* rotation 0 */
    OT_MP4_ROTATION_90 = 90,    /* rotation 90 */
    OT_MP4_ROTATION_180 = 180,  /* rotation 180 */
    OT_MP4_ROTATION_270 = 270,  /* rotation 270 */
    OT_MP4_ROTATION_BUTT
} OT_MP4_ROTATION_ANG;

typedef struct MP4_MOOV_AT_TAIL_INFO_S {
    TD_U32 reserved;
} OT_MP4_MOOV_AT_TAIL_INFO_S;

typedef struct MP4_MOOV_AT_FRONT_INFO_S {
    TD_U32 fileDuration; /* file duration for calculating the size of moov atom, in second */
} OT_MP4_MOOV_AT_FRONT_INFO_S;

typedef struct MP4_VIDEOINFO_S {
    TD_U32 u32Width;            /* video width */
    TD_U32 u32Height;           /* video height */
    TD_U32 u32BitRate;          /* bitrate bps */
    TD_FLOAT frameRate;         /* frame rate fps */
    OT_MP4_CODECID_E enCodecID; /* codec type */
    TD_U32 u32Rotation;         /* rotation (0, 90, 180, 270) */
} OT_MP4_VIDEOINFO_S;

typedef struct MP4_AUDIOINFO_S {
    TD_U32 u32Channels;         /* audio channel num 2 */
    TD_U32 u32SampleRate;       /* audio sample rate 48k */
    TD_U32 u32SamplePerFrame;   /* audio sample per frame */
    TD_U16 u16SampleSize;       /* bit per sample , 16 */
    OT_MP4_CODECID_E enCodecID; /* codec type */
} OT_MP4_AUDIOINFO_S;

typedef struct MP4_DATAINFO_S {
    TD_U32 u32Width;            /* meta data width */
    TD_U32 u32Height;           /* meta data height */
    TD_FLOAT frameRate;        /* frame rate fps */
    OT_MP4_CODECID_E enCodecID; /* codec type */
} OT_MP4_DATAINFO_S;

typedef struct MP4_TRACK_INFO_S {
    OT_MP4_TRACK_TYPE_E enTrackType;           /* stream type */
    TD_U32 u32TimeScale;                       /* time scale for each trak */
    TD_FLOAT fSpeed;                           /* play speed, (0,1]for slow,(1,~) for fast */
    TD_CHAR aszHdlrName[OT_MP4_MAX_HDLR_NAME]; /* manufacturer  name */
    union {
        OT_MP4_VIDEOINFO_S stVideoInfo; /* video info */
        OT_MP4_AUDIOINFO_S stAudioInfo; /* audio info */
        OT_MP4_DATAINFO_S stDataInfo;   /* metadata info */
    };
} OT_MP4_TRACK_INFO_S;

typedef struct MP4_FRAME_DATA_S {
    TD_U64 u64TimeStamp;   /* frame timestamp */
    TD_U8 *pu8DataBuffer;  /* frame data buffer */
    TD_U32 u32DataLength;  /* frame data len */
    TD_BOOL bKeyFrameFlag; /* key frame flag */
} OT_MP4_FRAME_DATA_S;

typedef struct MP4_ATOM_INFO_S {
    TD_U32 u32DataLen;                     /* user define atom data len */
    TD_CHAR aszType[OT_MP4_TYPE_SIZE + 1]; /* user define atom type */
    TD_U8 *pu8DataBuf;                     /* user define atom data buffer */
} OT_MP4_ATOM_INFO_S;

/**
 * @brief set duration callback.
 * @param[in] hHANDLE TD_MW_PTR :  muxer handle
 * @param[in] u32Timescale TD_U32 : timescale for muxer
 * @param[in,out] pu64Duration TD_U64* : duration for muxer
 * @return   0 success
 * @return  err num  failure
 */
typedef TD_S32 (*OT_MP4_SETDUTAION_FN) (TD_MW_PTR pMp4, TD_U32 u32Timescale, TD_U64 *pu64Duration);

typedef struct MP4_FILE_INFO_S {
    TD_U32 u32Duration;  // ms
    TD_CHAR aszHandlrName[OT_MP4_MAX_HDLR_NAME];
    TD_BOOL bCo64Flag;
    TD_U32 u32TrackNum;
    TD_U32 u32BackupUnit;
} OT_MP4_FILE_INFO_S;

typedef struct MP4_CONFIG_MUXER_S {
    /* pre allocate size in bytes, [0,100M],0 for not use pre allocate function,suggest 20M, unit :byte */
    TD_U32 u32PreAllocUnit;
    TD_U32 u32VBufSize;                       /* set the vbuf size for fwrite (0,5M] unit :byte */
    TD_BOOL bCo64Flag;                        /* if true;use co64,or use stco,used for muxer */
    TD_BOOL bConstantFps;                     /* if true, use constant framerate to calculate time,or use pst delta */
    TD_U32 u32BackupUnit;                     /* stbl group unit [512k,500M],if 0 not use backup data and repair */
    TD_CHAR fopenMode[OT_MODE_SIZE];       /* muxer fopen mode */
    OT_MP4_FORMAT_PROFILE_E enFormatProfile;  /* mpeg-4 file profile,base medie or quicktime */
    OT_MP4_MOOV_LOCATION_E moovLocation;
    union {
        OT_MP4_MOOV_AT_TAIL_INFO_S moovAtTail;
        OT_MP4_MOOV_AT_FRONT_INFO_S moovAtFront;
    };
} OT_MP4_CONFIG_MUXER_S;

typedef struct MP4_CONFIG_DEMUXER_S {
    TD_U32 u32VBufSize; /* set the vbuf size for fwrite (0,5M] unit :byte */
} OT_MP4_CONFIG_DEMUXER_S;

typedef struct MP4_CONFIG_APPENDS_S {
    TD_U32 u32VBufSize; /* set the vbuf size for fwrite (0,5M] unit :byte */
} OT_MP4_CONFIG_APPEND_S;

typedef struct MP4_CONFIG_S {
    TD_CHAR aszFileName[OT_MP4_MAX_FILE_NAME]; /* file path and file name */
    OT_MP4_CONFIG_TYPE_E enConfigType;
    union {
        OT_MP4_CONFIG_MUXER_S stMuxerConfig;
        OT_MP4_CONFIG_DEMUXER_S stDemuxerConfig;
        OT_MP4_CONFIG_APPEND_S stAppendConfig;
    };
} OT_MP4_CONFIG_S;

/**
 * @brief create mp4 instance.
 * @param[out] ppMp4 TD_MW_PTR* : return MP4 handle
 * @param[in] pstMp4Cfg OT_MP4_CONFIG_S* : mp4 cfg
 * @return   0 success
 * @return  err num  failure
 */
TD_S32 SS_MP4_Create(TD_MW_PTR *ppMp4Handle, const OT_MP4_CONFIG_S *mp4Cfg);

/**
 * @brief destroy mp4 instance.
 * @param[in] ppMp4Handle TD_MW_PTR :  MP4 handle
 * @param[out] pu64Duration TD_U64* :  file duration
 * @return   0 success
 * @return  err num  failure
 */

TD_S32 SS_MP4_Destroy(TD_MW_PTR pMp4Handle, TD_U64 *duration);

/**
 * @brief create mp4 track.
 * @param[in] pMp4Handle TD_MW_PTR : MP4 handle
 * @param[out] ppTrack TD_MW_PTR * : return track handle
 * @param[in] pstTrackInfo OT_MP4_TRACK_INFO_S* :stream info
 * @return   0 success
 * @return  err num  failure
 */
TD_S32 SS_MP4_CreateTrack(TD_MW_PTR pMp4Handle, TD_MW_PTR *pTrackHandle, const OT_MP4_TRACK_INFO_S *trackInfo);

/**
 * @brief destroy all mp4 track.
 * @param[in] pMp4Handle TD_MW_PTR :  MP4 handle
 * @return   0 success
 * @return  err num  failure
 */
TD_S32 SS_MP4_DestroyAllTracks(TD_MW_PTR pMp4Handle, OT_MP4_SETDUTAION_FN pfnSetDuration);

/**
 * @brief write mp4 frame data.
 * @param[in] pMp4Handle TD_MW_PTR : MP4 handle
 * @param[in] pTrack TD_MW_PTR : track handle
 * @param[in] pstFrameData OT_MP4_FRAME_DATA_S* : frame data
 * @return   0 success
 * @return  err num  failure
 */
TD_S32 SS_MP4_WriteFrame(TD_MW_PTR pMp4Handle, TD_MW_PTR pTrackHandle, const OT_MP4_FRAME_DATA_S *frameData);

/**
 * @brief add mp4 atom.
 * /   top
 * /moov
 * /moov/trak[1]/mdia/tkhd
 * @param[in] pMp4Handle TD_MW_PTR : MP4 handle
 * @param[in] pTrackHandle TD_CHAR* : box path [1,64]
 * @param[in] frameData OT_MP4_ATOM_INFO_S* : data info
 * @return   0 success
 * @return  err num  failure
 */
TD_S32 SS_MP4_AddAtom(TD_MW_PTR pMp4Handle, const TD_CHAR *atomPath, const OT_MP4_ATOM_INFO_S *atomInfo);

/**
 * @brief read mp4 box content.
 * @param[in] pMp4Handle TD_MW_PTR : MP4 handle
 * @param[out] pszPath TD_CHAR* : box path [1,64]
 * @param[in/out] atomInfo OT_MP4_ATOM_INFO_S* : data info
 * @param[in/out] pBufLen TD_U32* : mp4 box buffer len as readProperty ,output
 * @                     actual box len
 * @return   0 success
 * @return  err num  failure
 */
TD_S32 SS_MP4_GetAtom(TD_MW_PTR mp4Handle, TD_CHAR *atomPath, OT_MP4_ATOM_INFO_S *atomInfo);


/**
 * @brief check mp4 box exist.
 * @param[in] pMp4Handle TD_MW_PTR : MP4 handle
 * @param[out] pszPath TD_CHAR* : box path [1,64]
 * @param[in/out] atomInfo OT_MP4_ATOM_INFO_S* : data info
 * @return   0 success
 * @return  err num  failure
 */
TD_S32 SS_MP4_CheckAtomExist(TD_MW_PTR mp4Handle, TD_CHAR *atomPath, OT_MP4_ATOM_INFO_S *atomInfo);

/**
 * @brief read mp4 info.
 * @param[in] mp4Handle TD_MW_PTR : MP4 handle
 * @param[in,out] atomInfo OT_MP4_FILE_INFO_S* : mp4 info
 * @return   0 success
 * @return  err num  failure
 */
TD_S32 SS_MP4_GetFileInfo(TD_MW_PTR pMp4Handle, OT_MP4_FILE_INFO_S *fileInfo);

/**
 * @brief read mp4 Track by Index.
 * @param[in] pMp4Handle TD_MW_PTR : MP4 Demuxer handle
 * @param[in] u32Index TD_U32 : MP4 track index
 * @param[out] pTrackInfo OT_MP4_TRACK_INFO_S* : mp4 box track Info
 * @param[out] ppTrack TD_MW_PTR* : track handle
 * @return   0 success
 * @return  err num  failure
 */
TD_S32 SS_MP4_GetTrackByIndex(TD_MW_PTR pMp4Handle, TD_U32 index, OT_MP4_TRACK_INFO_S *pTrackInfo,
                              TD_MW_PTR *pTrackHandle);

/**
 * @brief repair mp4 data.
 * @param[in] filename TD_CHAR* : file name
 * @param[in] bParseExtraMdat TD_BOOL : parse extra mdat data after last backup unit flag
 * @return   0 success
 * @return  err num  failure
 */

TD_S32 SS_MP4_RepairFile(const TD_CHAR *fileName, TD_BOOL parseExtraMdat);

/**
 * @brief GET MP4 filesize.
 * @param[in] pMp4 TD_MW_PTR : MP4 Demuxer handle
 * @return filesize
 */

TD_U64 SS_MP4_GetFileSize(TD_MW_PTR pMp4Handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif
