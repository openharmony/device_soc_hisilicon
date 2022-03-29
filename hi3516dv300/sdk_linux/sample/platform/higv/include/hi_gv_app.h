/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HI_GV_APP_H
#define HI_GV_APP_H

/* add include here */
#include "hi_type.h"
#include "hi_gv_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Key hook go on ; */
#define HI_GV_KEYHOOK_GOON 0x1000
/* Key hook stop ; */
#define HI_GV_KEYHOOK_STOP 0x1001

/* The max static handle number for app ; */
#define HIGV_RESERVE_STATIC_HANDLE_NUM 1000

typedef enum {
    COMM_LEVEL_EMERGENCY = 0, /*  emergency event */
    COMM_LEVEL_NOTIFY,        /*  notify event */
    COMM_LEVEL_BUTT
} COMM_LEVEL_E;

typedef struct {
    HI_U32 id;          /*  message sequence id */
    HI_U32 msg;         /*  message type */
    COMM_LEVEL_E level; /*  message level */
} COMM_MSG_S;

typedef enum {
    HIGV_VSYNC_HW = 0, /*  hardware vsync signal */
    HIGV_VSYNC_SW,     /*  software vsync signal */
    HIGV_VSYNC_BUTT
} HIGV_VSYNC_E;

/*
 * brief Create application.
 * attention : A application corresponding a message.
 *
 * param[in] name  Application name.
 * param[in] appHandle Application handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_MSGM_VTOPMSGCREATE
 * retval ::HI_ERR_COMM_NORES
 * see :: HI_GV_App_Destroy
 */
HI_S32 HI_GV_App_Create(const HI_CHAR *name, HIGV_HANDLE *appHandle);

/*
 * brief Start application.
 * attention : The app will not accept any message if you not start the app.
 *
 * param[in] appHandle Application handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_NORES
 * see :: HI_GV_App_Stop
 */
HI_S32 HI_GV_App_Start(HIGV_HANDLE appHandle);

/*
 * brief Stop the application.
 * param[in] appHandle Application instance.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * see :: HI_GV_App_Start
 */
HI_S32 HI_GV_App_Stop(HIGV_HANDLE appHandle);

/*
 * brief Destroy the application.
 * param[in] appHandle
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_TASK_TASKNOTSTOP
 * see:: HI_GV_App_Create
 */
HI_S32 HI_GV_App_Destroy(HIGV_HANDLE appHandle);

/*
 * brief Active higv app to display on screen.
 * When Mutile higv app start up on the same time, This only support one active app display on the screen.
 * retval ::HI_SUCCESS
 * retval ::HI_FAILURE
 * see:: HI_GV_App_DeActive
 */
HI_S32 HI_GV_App_Active(HI_VOID);

/*
 * brief Enable app run in background.
 * retval ::HI_SUCCESS
 * retval ::HI_FAILURE
 * see :: HI_GV_App_DeActive
 */
HI_S32 HI_GV_App_DeActive(HI_VOID);

/*
 * brief Get the app whether active
 * retval ::HI_SUCCESS
 * retval ::HI_FAILURE
 * see:
 * HI_GV_App_Active
 * HI_GV_App_DeActive
 */
HI_BOOL HI_GV_App_IsActive(HI_VOID);

/*
 * brief Init HIGV.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_MSGM_VTOPMSGSERVER
 * retval ::HI_ERR_COMM_OTHER
 * see :: HI_GV_Deinit
 */
HI_S32 HI_GV_Init(HI_VOID);

/*
 * brief Deinit HIGV.
 * see::HI_GV_Init
 */
HI_VOID HI_GV_Deinit(HI_VOID);

/*
 * brief Set the max value of handle.
 * param[in] maxHandle Handle
 * retval ::HI_SUCCESS
 * retval ::HI_FAILURE
 */
HI_S32 HI_GV_SetMaxAssignHandle(HIGV_HANDLE maxHandle);

/* brief Get the reserved start static handle. */
HIGV_HANDLE HI_GV_GetReserveStartStaticHandle(HI_VOID);

/*
 * brief Set the type of  vsync.
 * param[in] vsyncType  vsync type.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 * retval ::HI_ERR_COMM_PAERM
 */
HI_S32 HI_GV_SetVsyncType(HIGV_VSYNC_E vsyncType);

/*
 * brief Get the type of  vsync.
 * retval ::HIGV_VSYNC_HW
 * retval ::HIGV_VSYNC_SW
 */
HIGV_VSYNC_E HI_GV_GetVsyncType(HI_VOID);

/*
 * brief Set the period of software vsync.
 * param[in] period  Sw vsync period.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 */
HI_S32 HI_GV_SetSWVsyncPeriod(HI_S64 period);

/*
 * brief Get the period of software vsync.
 * param[out] period Sw vsync period.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 */
HI_S32 HI_GV_GetSWVsyncPeriod(HI_S64 *period);

/*
 * brief Set vsync lost frame policy.
 * param[in] isLostFrame Is lost frame.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 */
HI_S32 HI_GV_SetVsyncLostFrame(HI_BOOL isLostFrame);

/*
 * brief Is vsync lost frame.
 * retval ::HI_TRUEʾ֡
 * retval ::HI_FALSEʾ֡
 */
HI_BOOL HI_GV_IsVsyncLostFrame(HI_VOID);

/*
 * brief Set the threshold of lost frame.
 * param[in] threshold Lost frame threshold.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 */
HI_S32 HI_GV_SetLostFrameThreshold(HI_U32 threshold);

/*
 * brief Get the threshold of lost frame.
 * param[out] threshold Lost frame threshold.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 */
HI_S32 HI_GV_GetLostFrameThreshold(HI_U32 *threshold);

/*
 * brief Sync the render cmd.
 * retval ::HI_SUCCESS
 */
HI_S32 HI_GV_RenderCmdSync(HI_VOID);

/*
 * brief Set render refresh cmd combine.
 * param[in] isCombine Is combine.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 */
HI_S32 HI_GV_SetRefreshCombine(HI_BOOL isCombine);

/*
 * brief Is refresh cmd combine.
 * retval ::HI_TRUEʾϲ
 * retval ::HI_FALSEʾϲ
 */
HI_BOOL HI_GV_IsRefreshCombine(HI_VOID);

/*
 * brief Set render refresh cmd combine.
 * ˽ӿHI_GV_Initʼ֮ǰ
 * param[in] isSync Is sync.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 */
HI_S32 HI_GV_SetSyncDraw(HI_BOOL isSync);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_APP_H */
