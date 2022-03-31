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

#ifndef HI_GV_IMAGEEX_H
#define HI_GV_IMAGEEX_H

/* add include here */
#include "hi_type.h"
#include "hi_gv_conf.h"

#ifdef HIGV_USE_WIDGET_IMAGEEX
#ifdef __cplusplus
extern "C" {
#endif

/*  Alignment of image */
typedef enum {
    /* Center */
    HIGV_IMGPOS_CENTER,
    /* Stretch */
    HIGV_IMGPOS_STRETCH,
    /* Autostrech */
    HIGV_IMGPOS_AUTOSTRETCH,

    HIGV_IMGPOS_BUTT
} HIGV_IMGPOS_E;

/*
* brief Set the content.
* param[in] imageExHandle Imagex handle.
* param[in] imgFile  The path file of image.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_EXIST Timer is exist.
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_ImageEx_SetImage(HIGV_HANDLE imageExHandle, const HI_CHAR *imgFile);

/*
* brief Set the count of playback cycles.
* param[in] imageExHandle Imagex handle.
* param[in] repeatCount The count of playback cycles .
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_ImageEx_SetRepeatCount(HIGV_HANDLE imageExHandle, HI_S32 repeatCount);

/*
* brief Set the position of picture to the widget.
* param[in] imageExHandle Imagex handle.
* param[in] posType  Position type of picture.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_ImageEx_SetPos(HIGV_HANDLE imageExHandle, HIGV_IMGPOS_E posType);

/*
* brief Set the interval of image display.
* attention ::
* it is only valid to dynamic gif format.
*
* param[in] imageExHandle Imagex handle.
* param[in] interval Interval of picture display.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_ImageEx_SetInterval(HIGV_HANDLE imageExHandle, HI_U32 interval);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_IMAGEEX_H */
