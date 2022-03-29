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

#ifndef HI_GV_WIN_H
#define HI_GV_WIN_H

/* add include here */
#include "hi_type.h"
#include "hi_gv_conf.h"
#ifdef __cplusplus
extern "C" {
#endif

/*
* brief Set window opacity.
* param[in] windowHandle Window handle.
* param[in] opacity Window opacity.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Win_SetOpacity(HIGV_HANDLE windowHandle, HI_U8 opacity);

/*
* brief Get first highlight child widget of widnow
* attention :This api is uded to get the highlight widget for softkeyboard.
*
* param[in] windowHandle Window handle.
* param[out] handleHilghtChild Highlight child.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Win_GetHilightChild(HIGV_HANDLE windowHandle, HIGV_HANDLE *handleHilghtChild);

/*
* brief Reset all highlight child and highlight the first child.
* attention:This API is used to redisplay the softkeyboard.
*
* param[in] windowHandle Window handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Win_ResetHilightChild(HIGV_HANDLE windowHandle);

/*
* brief Sync show win and get widget handle of done this show.
* attention:Return the second arg of HI_GV_Win_EndSyncShow.
* param[in] windowHandle Window handle.
* retval ::HIGV_HANDLE
* retval ::INVALID_HANDLE
* see : HI_GV_Win_EndSyncShow
*/
HI_S32 HI_GV_Win_SyncShow(HIGV_HANDLE windowHandle);

/*
* brief Done the sync show.
* param[in] windowHandle Window handle.
* param[in] handleWidget Widget handle of end window sync show .
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NOOP
* see : HI_GV_Win_SyncShow
*/
HI_S32 HI_GV_Win_EndSyncShow(HIGV_HANDLE windowHandle, HIGV_HANDLE handleWidget);

/*
* brief Get window level.
* param[in] windowHandle Window handle.
* param[out] level Window level.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Win_GetWinLevel(HIGV_HANDLE windowHandle, HI_U32 *level);

/*
* brief Advance window and child to load skin.
* attention : HIGV_STYLE_HIDE_UNLODRES style .
* param[in] windowHandle Window handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Win_LoadSkin(HIGV_HANDLE windowHandle);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_WIN_H */
