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

#ifndef HI_GV_SLIDEUNLOCK_H
#define HI_GV_SLIDEUNLOCK_H

#include "hi_gv_conf.h"
#ifdef HIGV_USE_WIDGET_SLIDEUNLOCK
/* add include here */
#include "hi_type.h"
#include "hi_go.h"
#include "hi_gv_resm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SLIDEUNLOCK_LENGTH  40   /* <length of slideunlock */
#define SLIDEUNLOCK_DUATION 250  /* <kickback duation */

typedef enum HI_SLIDEUNLOCK_STYLE {
    SLIDEUNLOCK_H = 0,  /* Horizontal */
    SLIDEUNLOCK_V,   /* Vertical */
    SLIDEUNLOCK_BUT
} SLIDEUNLOCK_STYLE;

typedef enum hiHIGV_SLIDEUNLOCK_E { /*  SlideUnlock type */
    HIGV_SLIDEUNLOCK_NORMAL = 0,
    HIGV_SLIDEUNLOCK_TOUCH,
    HIGV_SLIDEUNLOCK_DONE,
    HIGV_SLIDEUNLOCK_BUTT
} HIGV_SLIDEUNLOCK_E;

#define HIGV_SLIDEUNLOCK_H HIGV_STYLE_DEFINE(HIGV_WIDGET_SLIDEUNLOCK, SLIDEUNLOCK_H)
#define HIGV_SLIDEUNLOCK_V HIGV_STYLE_DEFINE(HIGV_WIDGET_SLIDEUNLOCK, SLIDEUNLOCK_V)

/*
* brief Set skin of trackbar.
* param[in] handleSlideUnlock SlideUnlock handle.
* param[in] handleRes    Picture resource ID.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_SlideUnlock_SetSliImage(HIGV_HANDLE handleSlideUnlock, HI_RESID handleRes);

/*
* brief Set the skin of slideblock.
* param[in] handleSlideUnlock    SlideUnlock handle.
* param[in] handleSkinIndex  Skin index.
* param[in] handleRes   Picture resource ID of handleSlideUnlock.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_SlideUnlock_SetTraImage(HIGV_HANDLE handleSlideUnlock, HI_U32 handleSkinIndex,
    HI_RESID handleRes);

/*
* brief Set status of slideunlock.
* param[in] handleSlideUnlock    SlideUnlock handle.
* param[in] isEnable    enable status.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_SlideUnlock_SetStatus(HIGV_HANDLE handleSlideUnlock, HI_BOOL isEnable);

/*
* brief get status of slideunlock.
* param[in] handleSlideUnlock SlideUnlock handle.
* param[out] isEnable enable status pointer.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_SlideUnlock_GetStatus(HIGV_HANDLE handleSlideUnlock, HI_U32 *isEnable);

/*
* brief back to init status.
* param[in] handleSlideUnlock SlideUnlock handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_SlideUnlock_ReSet(HIGV_HANDLE handleSlideUnlock);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_SLIDEUNLOCK_H */
#endif
