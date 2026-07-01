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

#ifndef OT_RECORDER_PRO_H
#define OT_RECORDER_PRO_H

#include "ot_mw_type.h"
#include "ot_error_def.h"
#include "ot_defs.h"
#include "ot_track_source.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*************************** Error Definition ****************************/
typedef enum OT_APP_RECORD_ERR_CODE_E {
    /* general error code */
    OT_APP_RECORD_ERR_REC_HANDLE_INVALID = 0x40,      /* recorder handle invalid */
    OT_APP_RECORD_ERR_REC_NO_AVAILABLE_HANDLE = 0x41, /* no available recorder handle */
    OT_APP_RECORD_ERR_MALLOC_FAIL = 0x42,             /* malloc memory fail */

    OT_APP_RECORD_ERR_THREAD_CREATE_FAIL = 0x43, /* thread create failed */
    OT_APP_RECORD_ERR_MUTEX_INIT_FAIL = 0x44,    /* mutex init failed */
    OT_APP_RECORD_ERR_COND_VAR_INIT_FAIL = 0x45, /* cond init failed */
    OT_APP_RECORD_ERR_INIT_TIMER_FAIL = 0x46,

    /* track source relate error */
    OT_APP_RECORD_ERR_STREAM_START_FAIL = 0x60, /* start stream fail */
    OT_APP_RECORD_ERR_STREAM_STOP_FAIL = 0x61,  /* stop stream fail */

    /* recorder opt related error code */
    OT_APP_RECORD_ERR_STOPPING = 0x70,        /* recorder repeated start */
    OT_APP_RECORD_ERR_NOT_ALLOWED = 0x71,     /* recorder permission not allowed */
    OT_APP_RECORD_ERR_DO_MANUAL_SPLIT = 0x72, /* recorder permission not allowed */

    /* MUX & file related error code */
    OT_APP_RECORD_ERR_ALLOC_FILENAME_FAIL = 0x80, /* alloc filename fail */

    /* buffer releated error code */
    OT_APP_RECORD_ERR_BUFFER_CREATE_FAIL = 0x90,
    OT_APP_RECORD_ERR_BUFFER_DESTROY_FAIL = 0x91,
    OT_APP_RECORD_ERR_BUFFER_SET_MODE_FAIL = 0x92,
    OT_APP_RECORD_ERR_BUFFER_WRITE_FRAME_FAIL = 0x93,
    OT_APP_RECORD_ERR_BUFFER_READ_FRAME_FAIL = 0x94,

    OT_APP_RECORD_BUTT = 0xFF
} OT_APP_RECORD_ERR_CODE_E;

/* general error code */
#define OT_ERR_RECORD_NULL_PTR      OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, APP_ERR_NULL_PTR)
#define OT_ERR_RECORD_ILLEGAL_PARAM OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, APP_ERR_ILLEGAL_PARAM)

#define OT_ERR_RECORD_HANDLE_INVALID      OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_REC_HANDLE_INVALID)
#define OT_ERR_RECORD_NO_AVAILABLE_HANDLE OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_REC_NO_AVAILABLE_HANDLE)
#define OT_ERR_RECORD_MALLOC_FAIL         OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_MALLOC_FAIL)
#define OT_ERR_RECORD_THREAD_CREATE_FAIL  OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_THREAD_CREATE_FAIL)
#define OT_ERR_RECORD_MUTEX_INIT_FAIL     OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_MUTEX_INIT_FAIL)
#define OT_ERR_RECORD_COND_VAR_INIT_FAIL  OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_COND_VAR_INIT_FAIL)
#define OT_ERR_RECORD_INIT_TIMER_FAIL     OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_INIT_TIMER_FAIL)

/* track source relate error */
#define OT_ERR_RECORD_STREAM_START_FAIL OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_STREAM_START_FAIL)
#define OT_ERR_RECORD_STREAM_STOP_FAIL  OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_STREAM_STOP_FAIL)

/* filemng & Storagemng related error code */
#define OT_ERR_RECORD_ALLOC_FILENAME_FAIL OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_ALLOC_FILENAME_FAIL)

#define OT_ERR_RECORD_DO_STOPPING     OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, OT_APP_RECORD_ERR_STOPPING)
#define OT_ERR_RECORD_OPT_NOT_ALLOWED OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, OT_APP_RECORD_ERR_NOT_ALLOWED)
#define OT_ERR_RECORD_DO_MANUAL_SPLIT OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_DO_MANUAL_SPLIT)

/* buff relate error code */
#define OT_ERR_RECORD_BUFFER_CREATE_FAIL      OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_BUFFER_CREATE_FAIL)
#define OT_ERR_RECORD_BUFFER_DESTROY_FAIL     OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_BUFFER_DESTROY_FAIL)
#define OT_ERR_RECORD_BUFFER_SET_MODE_FAIL    OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_BUFFER_SET_MODE_FAIL)
#define OT_ERR_RECORD_BUFFER_WRITE_FRAME_FAIL OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_BUFFER_WRITE_FRAME_FAIL)
#define OT_ERR_RECORD_BUFFER_READ_FRAME_FAIL  OT_APP_DEF_ERR(OT_APPID_REC, APP_ERR_LEVEL_ERROR, \
    OT_APP_RECORD_ERR_BUFFER_READ_FRAME_FAIL)

/*************************** Macro Definition ****************************/
/** \addtogroup     REC */
/** @{ */ /** <!-- [REC] */

#define OT_REC_STREAM_MAX_CNT (4)

#define OT_REC_TRACK_MAX_CNT (5)

#define OT_REC_FILE_NAME_LEN (256)

#define OT_REC_FRAME_MAX_SLICE (8)

#define OT_REC_MAX_WATER_LINE_PROC_CACHE_COUNT (5)

#define OT_REC_MAX_WATER_LINE_PROC_WINDOW_COUNT (10)

/* splite define */
/* record split type enum */
typedef enum {
    OT_REC_SPLIT_TYPE_NONE = 0, /* means split is disabled */
    OT_REC_SPLIT_TYPE_TIME,     /* record split when time reaches */
    OT_REC_SPLIT_TYPE_BUTT
} OT_REC_SPLIT_TYPE_E;

/* record split attribute param */
typedef struct {
    OT_REC_SPLIT_TYPE_E enSplitType; /* split type */
    TD_U32 u32SplitTimeLenSec;       /* split time, unit in second(s) */
    TD_U64 maxFileSizeGB;            /* max file size support by file system in GB */
} OT_REC_SPLIT_ATTR_S;

/* record manual split type enum */
typedef enum {
    OT_REC_POST_MANUAL_SPLIT = 0, /* post maunal split type */
    OT_REC_PRE_MANUAL_SPLIT,      /* pre manual split type */
    OT_REC_NORMAL_MANUAL_SPLIT,   /* normal manual split type */
    OT_REC_EXTEND_DURATION_SPLIT, /* extend duration */
    OT_REC_MANUAL_SPLIT_BUTT
} OT_REC_MANUAL_SPLIT_TYPE_E;

/* post manual split attribute */
typedef struct {
    TD_U32 u32AfterSec; /* manual split file will end after u32AfterSec */
} OT_REC_POST_MANUAL_SPLIT_ATTR_S;

/* post manual split attribute */
typedef struct {
    TD_U32 extendDurSec;
} OT_REC_EXTEND_DURATION_SPLIT_ATTR_S;

/* pre manual split attribute */
typedef struct {
    TD_U32 u32DurationSec; /* file duration of manual split file */
} OT_REC_PRE_MANUAL_SPLIT_ATTR_S;

/* normal manual split attribute */
typedef struct {
    TD_U32 u32Rsv; /* reserve */
} OT_REC_NORMAL_MANUAL_SPLIT_ATTR_S;

/* pre manual split attribute */
typedef struct {
    OT_REC_MANUAL_SPLIT_TYPE_E enManualType;                        /* maual split type */
    union {
        OT_REC_POST_MANUAL_SPLIT_ATTR_S stPostSplitAttr;            /* post manual split attr */
        OT_REC_PRE_MANUAL_SPLIT_ATTR_S stPreSplitAttr;              /* pre manual split attr */
        OT_REC_NORMAL_MANUAL_SPLIT_ATTR_S stNormalSplitAttr;        /* normal manual split attr */
        OT_REC_EXTEND_DURATION_SPLIT_ATTR_S stExtendSplitDurAttr;   /* extend duration attr */
    };
} OT_REC_MANUAL_SPLIT_ATTR_S;

/* record type enum */
typedef enum {
    OT_REC_TYPE_NORMAL = 0, /* normal record */
    OT_REC_TYPE_LAPSE,      /* time lapse record, record a frame by an fixed time interval */
    OT_REC_TYPE_BUTT
} OT_REC_TYPE_E;

/* normal record attribute param */
typedef struct {
    TD_U32 u32Rsv; /* reserve */
} OT_REC_NORMAL_ATTR_S;

/* lapse record attribute param */
typedef struct {
    TD_U32 u32IntervalMs; /* lapse record time interval, unit in millisecord(ms) */
} OT_REC_LAPSE_ATTR_S;

/* record event enum */
typedef enum {
    OT_REC_EVENT_START = 0,                     /* record is started */
    OT_REC_EVENT_STOP,                          /* record is stopped */

    OT_REC_EVENT_NEW_FILE_BEGIN,                /* new file start */
    OT_REC_EVENT_NEW_FILE_END,                  /* new file finish */
    OT_REC_EVENT_NEW_MANUAL_SPLIT_FILE_END,     /* new manual splite file finish */
    OT_REC_EVENT_NEW_DIR,                       /* use new dir */

    OT_REC_EVENT_ERR_CREATE_FILE_FAIL,          /* create file failed */
    OT_REC_EVENT_ERR_WRITE_FILE_FAIL,           /* write file failed */
    OT_REC_EVENT_ERR_CLOSE_FILE_FAIL,           /* close file failed */
    OT_REC_EVENT_ERR_READ_DATA_ERROR,           /* rec internal operation fail, must stop rec */
    OT_REC_EVENT_ERR_INTERNAL_OPERATION_FAIL,   /* rec internal operation fail, must stop rec */
    OT_REC_EVENT_ERR_RINGBUF_FULL,

    OT_REC_EVENT_RINGBUF_HIGH_WATER_LINE_REACHED, /* rec buffer high water line reached */
    OT_REC_EVENT_RINGBUF_LOW_WATER_LINE_REACHED,   /* rec buffer low water line reached */
    OT_REC_EVENT_AOV_START_WRITE,
    OT_REC_EVENT_AOV_STOP_WRITE,
    OT_REC_EVENT_BUTT
} OT_REC_EVENT_E;

/* state event info */
typedef struct {
    TD_S32 s32Rsv;
} OT_REC_STATE_EVENT_INFO_S;

/* file event info */
typedef struct {
    TD_CHAR aszFileName[OT_REC_FILE_NAME_LEN];
    TD_MW_PTR pMuxer;
} OT_REC_FILE_EVENT_INFO_S;

/* error event info */
typedef struct {
    TD_CHAR aszFileName[OT_REC_FILE_NAME_LEN];
    TD_S32 s32ErrorCode;
} OT_REC_ERROR_EVENT_INFO_S;

typedef struct {
    TD_U32 u32StreamId;
    TD_U32 u32RingBufRate;
} OT_REC_WATERLINE_EVENT_INFO_S;

/* event information */
typedef struct {
    OT_REC_EVENT_E enEventCode;
    union {
        OT_REC_STATE_EVENT_INFO_S stStateInfo; /* state event info */
        OT_REC_FILE_EVENT_INFO_S stFileInfo;   /* file event info */
        OT_REC_ERROR_EVENT_INFO_S stErrorInfo; /* error event info */
        OT_REC_WATERLINE_EVENT_INFO_S stWaterLineInfo;
    } unEventInfo;
} OT_REC_EVENT_INFO_S;

/* rec frame data struct */
typedef struct {
    TD_U64 u64TimeStamp;
    TD_U8 *apu8SliceAddr[OT_REC_FRAME_MAX_SLICE];
    TD_U32 au32SliceLen[OT_REC_FRAME_MAX_SLICE];
    TD_BOOL bKeyFrameFlag;
    TD_U32 u32SliceCnt;
} OT_REC_FRAME_DATA_S;

/* record event callback function */
typedef TD_S32 (*OT_REC_EVENT_CALLBACK_FN)(TD_MW_PTR pRecorder, const OT_REC_EVENT_INFO_S *pstEventInfo);

/* record create file function */
typedef TD_S32 (*OT_REC_REQUEST_FILE_NAMES_FN)(TD_MW_PTR pRecorder, TD_U32 u32FileCnt,
                                               TD_CHAR (*paszFilename)[OT_REC_FILE_NAME_LEN]);

/* create muxer callback function */
typedef TD_S32 (*OT_REC_CREATE_MUXER_FN)(const TD_CHAR *pszFileName, const TD_VOID *pMuxerCfg, TD_MW_PTR *ppMuxer);

/* destroy muxer callback function */
typedef TD_S32 (*OT_REC_DESTROY_MUXER_FN)(TD_MW_PTR pMuxer);

/* create track callback function */
typedef TD_S32 (*OT_REC_CREATE_TRACK_FN)(TD_MW_PTR pMuxer, OT_Track_Source_Handle pstTrackSrcHandle,
                                         TD_MW_PTR *pTrack);

/* write frame to track callback function */
typedef TD_S32 (*OT_REC_WRITE_FRAME_FN)(TD_MW_PTR pMuxer, TD_MW_PTR hTrackHandle,
                                        const OT_REC_FRAME_DATA_S *pstFrameData, TD_U64 *fileSize);

/* rec stream buffer alloc function */
typedef TD_S32 (*OT_REC_BUF_ALLOC_FN)(TD_VOID *pBufArg, TD_U32 u32BufSize, TD_VOID **ppVmAddr);

/* rec stream buffer free function */
typedef TD_S32 (*OT_REC_BUF_FREE_FN)(TD_VOID *pBufArg, TD_U32 u32BufSize, TD_VOID *pVmAddr);

typedef struct {
    OT_REC_CREATE_MUXER_FN pfnCreateMuxer;   /* create file muxer  */
    OT_REC_DESTROY_MUXER_FN pfnDestroyMuxer; /* destroy file muxer */
    OT_REC_CREATE_TRACK_FN pfnCreateTrack;   /* create track */
    OT_REC_WRITE_FRAME_FN pfnWriteFrame;     /* write frame */
    TD_VOID *pMuxerCfg;                      /* private muxer configure */
} OT_REC_MUXER_OPERATE_FN_S;

/* rec callback set */
typedef struct {
    OT_REC_REQUEST_FILE_NAMES_FN pfnRequestFileNames; /* callback  request file names */
} OT_REC_CALLBACK_S;

/* funcs of rec buffer  abstract */
typedef struct {
    OT_REC_BUF_ALLOC_FN pfnBufAlloc; /*  get the memory addr of the record buffer */
    OT_REC_BUF_FREE_FN pfnBufFree;   /*  release memory space of the record buffer */
    TD_VOID *pBufArg;                /*  buffer private argument pass by buffer alloc and free */
} OT_REC_BUF_ABSTRACTFUNC_S;

typedef struct {
    TD_BOOL bEnableWaterLine;
    TD_U32 u32HighWaterLine;
    TD_U32 u32LowWaterLine;
} OT_REC_WATERLINE_ATTR_S;

typedef struct {
    TD_BOOL bEnableRingBufRateProc;
    TD_U32 u32SlidingWindowCount;
    TD_U32 u32SlidingWindow[OT_REC_MAX_WATER_LINE_PROC_WINDOW_COUNT];
    TD_U32 u32SamplingIntervalSeconds;
    TD_U32 u32StatisticsIntervalSeconds;
    TD_U32 u32CacheCount;
} OT_RINGBUF_RATE_PROC_ATTR_S;

/* stream thread attribute param */
typedef struct {
    TD_U32 streamPriority;
} OT_REC_PARAM;

/* record stream attribute */
typedef struct {
    TD_U32 u32TrackCnt;                                            /* track cnt */
    TD_U32 bufferTimeMSec;                                         /* buffer time */
    OT_Track_Source_Handle aHTrackSrcHandle[OT_REC_TRACK_MAX_CNT]; /* array of track source cnt */
    OT_REC_MUXER_OPERATE_FN_S stMuxerOps;                          /* muxer operates  */
    OT_REC_BUF_ABSTRACTFUNC_S stBufOps;                            /* rec buffer operates */
} OT_REC_STREAM_ATTR_S;

/* record attribute param */
typedef struct {
    OT_REC_TYPE_E enRecType; /* record type */
    union {
        OT_REC_NORMAL_ATTR_S stNormalRecAttr; /* normal record attribute */
        OT_REC_LAPSE_ATTR_S stLapseRecAttr;   /* lapse record attribute */
    } unRecAttr;

    OT_REC_SPLIT_ATTR_S stSplitAttr; /* record split attribute */

    OT_REC_WATERLINE_ATTR_S stWaterLineAttr;
    OT_RINGBUF_RATE_PROC_ATTR_S stRingBufRateProcAttr;

    TD_U32 u32StreamCnt;                                       /*  stream cnt */
    OT_REC_STREAM_ATTR_S astStreamAttr[OT_REC_STREAM_MAX_CNT]; /*  array of stream attr */
    OT_REC_CALLBACK_S stRecCallbacks;                          /*  rec callbak */
    TD_U32 u32PreRecTimeSec;                                   /*  pre record time */
    TD_BOOL bHaveOtherDir;                                     /* backup directory is available, Switch only once */
    TD_BOOL bIgnoreReadBufTimeout;
} OT_REC_ATTR_S;

/*************************** Interface Definition ****************************/
/**
 * @brief create a new recorder
 * @param[in]pstRecAttr : the attribute of recorder
 * @param[out]ppRecorder : pointer of recorder
 * @return 0 success
 * @return others failure
 */
TD_S32 SS_REC_Create(const OT_REC_ATTR_S *pstRecAttr, TD_MW_PTR *ppRecorder);

/**
 * @brief destory a recorder.
 * @param[in]pRecorder : pointer of recorder
 * @return 0 success
 * @return others failure
 */
TD_S32 SS_REC_Destroy(TD_MW_PTR pRecorder);

/**
 * @brief set recorder parameter
 * @param[in]pRecorder, pointer of recorder
 * @param[in]recParam, recorder parameter
 * @return 0 success
 * @return -1 failure
 */
TD_S32 SS_REC_SetParam(TD_MW_PTR pRecorder, const OT_REC_PARAM *recParam);

/**
 * @brief start recorder
 * @param[in]pRecorder, pointer of recorder
 * @return 0 success
 * @return -1 failure
 */
TD_S32 SS_REC_Start(TD_MW_PTR pRecorder);

/**
 * @brief stop recorder
 * @param[in]pRecorder : pointer of recorder
 * @param[in]bQuickMode : quick stop mode flag.
 * @return 0 success
 * @return others failure
 */
TD_S32 SS_REC_Stop(TD_MW_PTR pRecorder, TD_BOOL bQuickMode);

/**
 * @brief manual splite file.
 * @param[in]pRecorder : pointer of recorder
 * @param[in]pstSplitAttr : manual split attr.
 * @return 0 success
 * @return others failure
 */
TD_S32 SS_REC_ManualSplit(TD_MW_PTR pRecorder, const OT_REC_MANUAL_SPLIT_ATTR_S *pstSplitAttr);

/**
 * @brief write frame data to rec.
 * @param[in]pRecorder : pointer of recorder
 * @param[in]pTrackSrcHandle : handle of track source
 * @param[in]pstFrameData : pointer to frame data.
 * @return 0 success
 * @return others failure
 */
TD_S32 SS_REC_WriteData(TD_MW_PTR pRecorder, OT_Track_Source_Handle pTrackSrcHandle,
                        const OT_REC_FRAME_DATA_S *pstFrameData);

/**
 * @brief register recorder envent callback
 * @param[in]pRecorder : pointer of recorder
 * @param[in]pfnEventCallback : callback function
 * @return 0 success
 * @return others failure
 */
TD_S32 SS_REC_RegisterEventCallback(TD_MW_PTR pRecorder, OT_REC_EVENT_CALLBACK_FN pfnEventCallback);


TD_S32 SS_REC_GetRingBufRate(TD_MW_PTR pRecorder, TD_U32 u32StreamIndex, TD_U32 *pu32RingBufRate);

TD_S32 SS_REC_AovEnable(TD_MW_PTR pRecorder, TD_BOOL bEnable);

TD_S32 SS_REC_SetAovBufferLine(TD_MW_PTR pRecorder, TD_U32 u32DownBufferSize, TD_U32 u32UpBufferRate);

/** @} */ /** <!-- ==== REC End ==== */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

