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

/* Key hook go on */
#define HI_GV_KEYHOOK_GOON 0x1000
/* Key hook stop */
#define HI_GV_KEYHOOK_STOP 0x1001

/* The max static handle number for app */
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

HI_S32 HI_GV_App_Create(const HI_CHAR *name, HIGV_HANDLE *appHandle);

HI_S32 HI_GV_App_Start(HIGV_HANDLE appHandle);

HI_S32 HI_GV_App_Stop(HIGV_HANDLE appHandle);

HI_S32 HI_GV_App_Destroy(HIGV_HANDLE appHandle);

HI_S32 HI_GV_App_Active(HI_VOID);

HI_S32 HI_GV_App_DeActive(HI_VOID);

HI_BOOL HI_GV_App_IsActive(HI_VOID);

HI_S32 HI_GV_Init(HI_VOID);

HI_VOID HI_GV_Deinit(HI_VOID);

HI_S32 HI_GV_SetMaxAssignHandle(HIGV_HANDLE maxHandle);

/* brief Get the reserved start static handle. */
HIGV_HANDLE HI_GV_GetReserveStartStaticHandle(HI_VOID);

HI_S32 HI_GV_SetVsyncType(HIGV_VSYNC_E vsyncType);

HIGV_VSYNC_E HI_GV_GetVsyncType(HI_VOID);

HI_S32 HI_GV_SetSWVsyncPeriod(HI_S64 period);

HI_S32 HI_GV_GetSWVsyncPeriod(HI_S64 *period);

HI_S32 HI_GV_SetVsyncLostFrame(HI_BOOL isLostFrame);

HI_BOOL HI_GV_IsVsyncLostFrame(HI_VOID);

HI_S32 HI_GV_SetLostFrameThreshold(HI_U32 threshold);

HI_S32 HI_GV_GetLostFrameThreshold(HI_U32 *threshold);

HI_S32 HI_GV_RenderCmdSync(HI_VOID);

HI_S32 HI_GV_SetRefreshCombine(HI_BOOL isCombine);

HI_BOOL HI_GV_IsRefreshCombine(HI_VOID);

HI_S32 HI_GV_SetSyncDraw(HI_BOOL isSync);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_APP_H */
