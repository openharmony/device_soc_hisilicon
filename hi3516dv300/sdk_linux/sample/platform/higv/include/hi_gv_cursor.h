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

#ifndef HI_GV_CURSOR_H
#define HI_GV_CURSOR_H

#include "hi_type.h"
#include "hi_gv_conf.h"
#include "hi_gv_widget.h"

#ifdef HIGV_USE_MODULE_CURSOR

#ifdef __cplusplus
extern "C" {
#endif

#define IMAGE_MAX_SIZE  64
#define IMAGE_MIN_SIZE  16

/* Cursor  */
typedef struct {
    HIGV_HANDLE imageHandle; /*  The max size of cursor bitmap is 64x64, and small size is 16x16 */
    HI_U32 hotspotX; /*  The x of cursor hotspot,  */
    HI_U32 hotspotY; /* The y of cursor hotspot ,  */
} HigvCursorInfo;

typedef enum {
    HIGV_CURSOR_HW = 0x1,
    HIGV_CURSOR_SW,
    HIGV_CURSOR_BUTT
} HigvCursorType;

/*
* brief Cursor init.
* param[in] type type of cursor.
* retval ::HI_SUCCESS
* see
*/
HI_S32 HI_GV_CURSOR_Init(HigvCursorType cursorType);

/*
* brief Cursor init.
* param N/A.
* retval ::HI_SUCCESS
* see
*/
HI_S32 HI_GV_CURSOR_DeInit(HI_VOID);

/*
* brief Show cursor.
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_CURSOR_Show(HI_VOID);

/*
* brief Hide cursor.
* attention:If you hide cursor, it will not respond the mouse event.
*
* param N/A.
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_CURSOR_Hide(HI_VOID);

/*
* brief Set  map scale of cursor.ӳ
* ӳͼ1024*768 720*576Scale_X = 720/1024, Scale_Y = 576/768
*/
HI_S32 HI_GV_CURSOR_SetMapScale(HI_FLOAT scaleX, HI_FLOAT scaleY);

/*
* brief Set the threshold of cursor.
* The threshold is should generate and equal 3, default is 4.
*
* param[in] threshold of cursor.
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
* see
*/
HI_S32 HI_GV_CURSOR_SetThreshold(HI_U16 threshold);

/*
* brief Set the aceleration of cursor.
* The accelation parameter is should generate 0, default is 4.
*
* param[in] acceleration Acceleration of cursor.
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
* see
*/
HI_S32 HI_GV_CURSOR_SetAcceleration(HI_U16 acceleration);

/*
* brief Move the cursor to the position of screen.
* param[in] screenPosX  x coordinate of screen(unit:pixel).
* param[in] screenPosY y coordinate of screen(unit:pixel).
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_CURSOR_SetScreenPos(HIGV_CORD screenPosX, HIGV_CORD screenPosY);

/*
* brief Get the coordinate of cursor.
* param[out] screenPosX x coordinate of screen(unit:pixel)
* param[out] screenPosY y coordinate of screen(unit:pixel)
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_CURSOR_GetScreenPos(HIGV_CORD *screenPosX, HIGV_CORD *screenPosY);

/*
* brief Set hotspot of cursor.
*
* param[in] cursorInfo ͼƬȵϢ
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_CURSOR_SetImage(const HigvCursorInfo *cursorInfo);

/*
* brief Get the picture information of cursor.
* param[out] cursorInfo Cursor picture information.
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_CURSOR_GetImage(HigvCursorInfo *cursorInfo);

/*
* brief Set the range of cursor move.
* param[in] regionX x coordinate.
* param[in] regionY y coordinate.
* param[in] regionWidth  Width.
* param[in] regionHeight Height.
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_CURSOR_SetCursorRegion(HI_S32 regionX, HI_S32 regionY, HI_S32 regionWidth, HI_S32 regionHeight);

/*
* brief Get the output device.
* attention:When change output device, you should hide the cursor.
*
* param[out] devID  The pointer of output device.
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_CURSOR_GetOutputDev(HI_U32 *devID);

/*
* brief Set width and height of output screen.
* attention:it is different between the real display and the canvas surface size.
*
* GUIڲͼsurface720x576ʱĻʵҪ720x480û720x576ͨŴ720x480ͼ
* λãҲҪݱӦĴ
* param[in] devID  cursor device ID.
* param[in] screenWidth   screen width of cursor output.
* param[in] screenHight   screen height of cursor output.
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_CURSOR_SetDisplayScreen(HI_U32 devID, HI_U32 screenWidth, HI_U32 screenHight);

/*
* brief Attach the cursor to the
* layer.
* Ǵΰ󶨵ͼbufferķΧ
* \attention \n
* you should set the bitmap and create layer first.
*
* param[in] layerId Layer ID.
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_CURSOR_AttchLayer(HIGO_LAYER_E layerId);

/*
* brief Detach cursor from layer.
* attention
* param[in] layerId Layer ID.
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
*/
HI_S32 HI_GV_CURSOR_DettchLayer(HIGO_LAYER_E layerId);

/*
* brief set whether the cursor always show.
* attention if isCursorShow is HI_TRUE, the cursor will always show, even if mouse is not disconnected;
* otherwise the cursor show only if the mouse conntected.
*
* param[in] isCursorShow is cursor always show or not.
* retval N/A.
*/
HI_VOID HI_GV_CURSOR_SetAllwaysShow(const HI_BOOL isCursorShow);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_CURSOR_H */
#endif
