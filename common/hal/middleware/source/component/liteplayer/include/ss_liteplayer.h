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

#ifndef OT_LITEPLAYER_H
#define OT_LITEPLAYER_H

#include "ot_mw_type.h"
#include "ss_demuxer.h"

/** \addtogroup     PLAYER */
/* [PLAYER] */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define OT_LITEPLAYER_MAX_FILE_SURFIX_CNT   10
#define OT_LITEPLAYER_MAX_FILE_SURFIX_LEN   32
#define OT_LITEPLAYER_FILE_PATH_MAX_LEN     1024
#define OT_LITEPLAYER_AUD_DECODER_PATH_LEN  256
#define OT_LITEPLAYER_DEMUX_PATH_LEN        256

/** Error information of the player */
typedef enum LITEPLAYER_ERROR_E {
    OT_LITEPLAYER_ERROR_VID_PLAY_FAIL = 0x0, /* The video fails to be played. */
    OT_LITEPLAYER_ERROR_AUD_PLAY_FAIL,       /* The audio fails to be played. */
    OT_LITEPLAYER_ERROR_DEMUX_FAIL,          /* The file fails to be played. */
    OT_LITEPLAYER_ERROR_TIMEOUT,             /* Operation timeout. For example, reading data timeout. */
    OT_LITEPLAYER_ERROR_NOT_SUPPORT,         /* The file format is not supportted. */
    OT_LITEPLAYER_ERROR_UNKNOW,              /* Unknown error. */
    OT_LITEPLAYER_ERROR_ILLEGAL_STATEACTION, /* illegal action at cur state. */
    OT_LITEPLAYER_ERROR_BUTT,
} OT_LITEPLAYER_ERROR_E;

/** Player status */
typedef enum LITEPLAYER_STATE_E {
    OT_LITEPLAYER_STATE_IDLE = 0, /* The player state before init . */
    OT_LITEPLAYER_STATE_INIT,     /* The player is in the initial state.
                                   * It changes to the initial state after being created.
                                   */
    OT_LITEPLAYER_STATE_PREPARED, /* The player is in the prepared state. */
    OT_LITEPLAYER_STATE_PLAY,     /* The player is in the playing state. */
    OT_LITEPLAYER_STATE_TPLAY,    /* The player is in the trick playing state. */
    OT_LITEPLAYER_STATE_PAUSE,    /* The player is in the pause state. */
    OT_LITEPLAYER_STATE_ERR,      /* The player is in the err state. */
    OT_LITEPLAYER_STATE_BUTT
} OT_LITEPLAYER_STATE_E;

typedef enum LITEPLAYER_EVENT_E {
    OT_LITEPLAYER_EVENT_STATE_CHANGED = 0x0, /* the player status changed */
    OT_LITEPLAYER_EVENT_EOF,                 /* the player is playing the end */
    OT_LITEPLAYER_EVENT_SOF,                 /* the player backward tplay to the start of file */
    OT_LITEPLAYER_EVENT_PROGRESS,            /* current playing progress. it will be called every one second.
                                              * the additional value that in the unit of ms is current playing time
                                              */
    OT_LITEPLAYER_EVENT_SEEK_END,            /* seek time jump, the additional value is the seek value */
    OT_LITEPLAYER_EVENT_ERROR,               /* play error */
    OT_LITEPLAYER_EVENT_BUTT
} OT_LITEPLAYER_EVENT_E;

typedef enum LITEPLAYER_PLAY_SPEED_E {
    OT_LITEPLAYER_PLAY_SPEED_BASE = 100,
    OT_LITEPLAYER_PLAY_SPEED_2X_FAST = 2 * OT_LITEPLAYER_PLAY_SPEED_BASE,     /* 2 x speed fast */
    OT_LITEPLAYER_PLAY_SPEED_4X_FAST = 4 * OT_LITEPLAYER_PLAY_SPEED_BASE,     /* 4 x speed fast */
    OT_LITEPLAYER_PLAY_SPEED_8X_FAST = 8 * OT_LITEPLAYER_PLAY_SPEED_BASE,     /* 8 x speed fast */
    OT_LITEPLAYER_PLAY_SPEED_16X_FAST = 16 * OT_LITEPLAYER_PLAY_SPEED_BASE,   /* 16 x speed fast */
    OT_LITEPLAYER_PLAY_SPEED_32X_FAST = 32 * OT_LITEPLAYER_PLAY_SPEED_BASE,   /* 32 x speed fast */
    OT_LITEPLAYER_PLAY_SPEED_64X_FAST = 64 * OT_LITEPLAYER_PLAY_SPEED_BASE,   /* 64 x speed fast */
    OT_LITEPLAYER_PLAY_SPEED_128X_FAST = 128 * OT_LITEPLAYER_PLAY_SPEED_BASE, /* 128 x speed fast */
    OT_LITEPLAYER_PLAY_SPEED_BUTT                                             /* Invalid speed value */
} OT_LITEPLAYER_PLAY_SPEED_E;

typedef enum LITEPLAYER_TPLAY_DIRECTION_E {
    OT_LITEPLAYER_TPLAY_FORWARD,
    OT_LITEPLAYER_TPLAY_BACKWARD,
    OT_LITEPLAYER_TPLAY_BUTT
} OT_LITEPLAYER_TPLAY_DIRECTION_E;

typedef enum LITEPLAYER_DATA_PLAY_TYPE_E {
    OT_LITEPLAYER_DATA_TYPE_JPEG,
    OT_LITEPLAYER_DATA_TYPE_BUTT
} OT_LITEPLAYER_DATA_PLAY_TYPE_E;

typedef enum LITEPLAYER_VOUT_TYPE_E {
    OT_LITEPLAYER_VOUT_TYPE_VPSS,
    OT_LITEPLAYER_VOUT_TYPE_VO,
    OT_LITEPLAYER_VOUT_TYPE_BUTT
} OT_LITEPLAYER_VOUT_TYPE_E;

typedef struct LITEPLAYER_TPLAY_ATTR_S {
    OT_LITEPLAYER_TPLAY_DIRECTION_E enDirection;
    OT_LITEPLAYER_PLAY_SPEED_E enSpeed;
} OT_LITEPLAYER_TPLAY_ATTR_S;

typedef struct LITEPLAYER_ATTR_S {
    TD_S32 s32VidStreamId; /* Video stream ID. */
    TD_S32 s32AudStreamId; /* Audio stream ID. */
} OT_LITEPLAYER_ATTR_S;

typedef struct LITEPLAYER_PARAM_S {
    TD_U32 u32PlayPosNotifyIntervalMs; /* Current playing Time notify interval in Ms. */
    TD_U32 u32VideoEsBufSize;          /* video es stream cache buf size.[1k,200M] unit:byte;
                                        * if 0,use the inner default value
                                        */
    TD_U32 u32AudioEsBufSize;          /* audio es stream cache buf size.[1k,100M] unit:byte;
                                        * if 0,use the inner default value
                                        */
    TD_U32 u32VdecFrameBufCnt;         /* Number of VB blocks for storing decoded image frames;
                                        * if 0,use the inner default value
                                        */
    TD_BOOL bPauseMode;                /* pause after first frame render mode flag,
                                        * TD_FALSE:do not pause, TD_TRUE:pause after first frame render.
                                        */
    TD_BOOL useMmz;                    /* use mmz flag, TD_TRUE:use mmz, TD_FALSE:not use mmz */
} OT_LITEPLAYER_PARAM_S;

typedef struct LITEPLAYER_EXTRA_FILESURFIX_S {
    TD_U32 u32SurfixCnt;                /* file surfixes count. */
    TD_CHAR aszFileSurfix[OT_LITEPLAYER_MAX_FILE_SURFIX_CNT][OT_LITEPLAYER_MAX_FILE_SURFIX_LEN];
} OT_LITEPLAYER_EXTRA_FILESURFIX_S;

typedef struct LITEPLAYER_VOUT_OPT_S {
    OT_LITEPLAYER_VOUT_TYPE_E enVoutType;
    TD_HANDLE hModHdl;
    TD_HANDLE hChnHdl;
} OT_LITEPLAYER_VOUT_OPT_S;

typedef struct LITEPLAYER_AOUT_OPT_S {
    TD_HANDLE hAudDevHdl;    // device id
    TD_HANDLE hAudTrackHdl;  // chn id
} OT_LITEPLAYER_AOUT_OPT_S;

typedef TD_VOID (*OT_LITEPLAYER_EVENT_FN)(TD_MW_PTR pPlayer, OT_LITEPLAYER_EVENT_E enEvent, TD_VOID *pData);

typedef TD_S32 (*OT_LITEPLAYER_ON_VB_POOL_GET_FN)(TD_VOID *pPlayer, TD_U32 u32FrameBufSize,
                                                  TD_U32 u32FrameBufCnt,
                                                  TD_U32 *pVbPoolId,
                                                  TD_VOID *pPriv);

typedef TD_S32 (*OT_LITEPLAYER_ON_VB_POOL_PUT_FN)(TD_VOID *pPlayer,
                                                  TD_U32 u32VbPoolId,
                                                  TD_VOID *pPriv);

/*
*   @brief init the player
*   @param[in] N/A
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_Init(TD_VOID);
/*
*   @brief deinit of the player
*   @param[in] : N/A
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_DeInit(TD_VOID);

/*
*   @brief  register other demuxer,with supported extra file surfixes
*   @param[in] dllName : TD_CHAR: demuxer lib file
*   @param[in] pstExtraFileSurfix : OT_LITEPLAYER_EXTRA_FILESURFIX_S: supported extra file surfixes
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_RegisterDemuxer(const TD_CHAR *pszDllName,
                                     const OT_LITEPLAYER_EXTRA_FILESURFIX_S *pstExtraFileSurfix);

/*
*   @brief unregister other demuxer
*   @param[in] dllName : TD_CHAR: demuxer lib file
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_UnRegisterDemuxer(const TD_CHAR *pszDllName);

/*
*   @brief  register other audio Decoder
*   @param[in] dllName : TD_CHAR: audio lib file
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_RegisterAudioDecoder(const TD_CHAR *pszAudioDecLib);

/*
*   @brief unregister other audio Decoder
*   @param[in] dllName : TD_CHAR: audio Decoder lib file
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_UnRegisterAudioDecoder(const TD_CHAR *pszAudioDecLib);

/*
*   @brief create the player
*   @param[out] ppPlayer : TD_MW_PTR*: handle of the player
*   @param[in] pstPlayParam : OT_LITEPLAYER_PARAM_S*: input player params
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_Create(TD_MW_PTR *ppPlayer, const OT_LITEPLAYER_PARAM_S *pstCreateParam);

/*
*   @brief  set the vo handle to the player
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @param[in] pstVoutOpt : OT_LITEPLAYER_VOUT_OPT_S*: indicate vout attr
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_SetVOHandle(TD_MW_PTR pPlayer, const OT_LITEPLAYER_VOUT_OPT_S *pstVoutOpt);

/*
*   @brief  set the ao handle to the  player
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @param[in] pstAoutOpt : OT_LITEPLAYER_AOUT_OPT_S: attr of audio out device
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_SetAOHandle(TD_MW_PTR pPlayer, const OT_LITEPLAYER_AOUT_OPT_S *pstAoutOpt);

/*
*   @brief  destroy the player
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_Destroy(TD_MW_PTR pPlayer);

/*
*   @brief    set the file for playing
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @param[in] filePath : TD_CHAR: media file path
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_SetDataSource(TD_MW_PTR pPlayer, const TD_CHAR *pszfilePath);

/*
*   @brief prepare for the playing
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_Prepare(TD_MW_PTR pPlayer);

/*
*   @brief  do play of the stream
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_Play(TD_MW_PTR pPlayer);

/*
*   @brief stop the stream playing, and release the resource
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_Stop(TD_MW_PTR pPlayer);

/*
*   @brief pause the stream playing
*   @param[in] hPlayer : TD_MW_PTR: handle of the player
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_Pause(TD_MW_PTR pPlayer);

/*
*   @brief seek by the time
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @param[in] s64TimeInMs : TD_S64: seek time
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_Seek(TD_MW_PTR pPlayer, TD_S64 s64TimeInMs);

/*
*   @brief register call back fun
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @param[in] pfnCallback : OT_LITEPLAYER_EVENT_FN: call back fun
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_RegCallback(TD_MW_PTR pPlayer, OT_LITEPLAYER_EVENT_FN pfnCallback);

/*
*   @brief get the info about the media file
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @param[out] pstFormatInfo : OT_FORMAT_FILE_INFO_S: media info abort the media file
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_GetFileInfo(TD_MW_PTR pPlayer, OT_FORMAT_FILE_INFO_S *pstFormatInfo);

/*
*   @brief set the attribute about the media file
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @param[out] pstMediaAttr : OT_LITEPLAYER_ATTR_S: attribute of the media file
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_SetMedia(TD_MW_PTR pPlayer, const OT_LITEPLAYER_ATTR_S *pstMediaAttr);

/*
*   @brief get the  current play status
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @param[out] penState : OT_LITEPLAYER_STATE_E*: play state
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_GetPlayStatus(TD_MW_PTR pPlayer, OT_LITEPLAYER_STATE_E *penState);

/*
*   @brief trick play current video stream
*   @param[in] pPlayer : TD_MW_PTR: handle of the player
*   @param[out] pstTrickPlayAttr : OT_LITEPLAYER_TPLAY_ATTR_S: trick play attribute
*   @retval  0 success,others failed
 */
TD_S32 SS_LITEPLAYER_TPlay(TD_MW_PTR pPlayer, const OT_LITEPLAYER_TPLAY_ATTR_S *pstTrickPlayAttr);

/* PLAYER End */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
