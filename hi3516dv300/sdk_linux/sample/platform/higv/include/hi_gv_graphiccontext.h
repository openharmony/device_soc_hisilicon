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

#ifndef HI_GV_GRAPHICCONTEXT_H
#define HI_GV_GRAPHICCONTEXT_H
#include "hi_type.h"
#include "hi_go.h"
#include "hi_gv_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

HI_S32 HI_GV_GraphicContext_Create(HIGV_HANDLE widgetHandle, HIGV_HANDLE *contextHandle);

/*
* brief Destroy GraphicContext.
* attention
* param[in] contextHandle GraphicContext handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* HI_GV_GraphicContext_Destroy
*/
HI_S32 HI_GV_GraphicContext_Destroy(HIGV_HANDLE contextHandle);

/*
* brief Begin draw,add the default clip rect .
* param[in] contextHandle GraphicContext handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_LOST
* see
* HI_GV_GraphicContext_End
*/
HI_S32 HI_GV_GraphicContext_Begin(HIGV_HANDLE contextHandle);

/*
* brief End draw,empty clip rect.
* param[in] contextHandle GraphicContext handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* see : HI_GV_GraphicContext_Begin
*/
HI_S32 HI_GV_GraphicContext_End(HIGV_HANDLE contextHandle);

/*
* brief Add clip rect.
* Keep old clip rect,add new clip rect.
* param[in] contextHandle GraphicContext handle.
* param[in] rect Clip rect.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_GraphicContext_AddClipRect(HIGV_HANDLE contextHandle, const HI_RECT *rect);

/*
* brief Reset clip rect.
* Clean old clip rect,reset new clip rect.
* param[in] contextHandle GraphicContext handle.
* param[in] rect Clip rect.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_GraphicContext_SetClipRect(HIGV_HANDLE contextHandle, const HI_RECT *rect);

/*
* brief Decode a locale picture.
* If imgWidth or imgHeight is 0,it indicates that the original picture attributes are retained.
*
* param[in] fileName Picture path.
* param[in] imgWidth Image surface width.
* param[in] imgHeight Image surface height.
* param[out] imgSurface Image surface handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_LOST
* retval ::HIGO_ERR_NULLPTR
* retval ::HI_FAILURE
* retval ::HI_NULL
* retval ::HIGO_ERR_INVIMGDATA
* see::
* hi_go_decoder.h
* HI_GV_GraphicContext_FreeImageSurface
*/
HI_S32 HI_GV_GraphicContext_DecodeImg(const HI_CHAR *fileName, HI_U32 imgWidth, HI_U32 imgHeight,
    HIGV_HANDLE *imgSurface);

/*
* brief Decode memory picture.
* If imgWidth or imgHeight is 0,it indicates that the original picture attributes are retained.
*
* param[in] addr The memory picture address.
* param[in] Length The memory length.
* param[in] imgWidth Image surface width.
* param[in] imgHeight Image surface height.
* param[out] imgSurface Image surface handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_LOST
* retval ::HIGO_ERR_NULLPTR
* retval ::HI_FAILURE
* retval ::HI_NULL
* retval ::HIGO_ERR_INVIMGDATA
* see::
* hi_go_decoder.h
* HI_GV_GraphicContext_FreeImageSurface
*/
HI_S32 HI_GV_GraphicContext_DecodeMemImg(HI_CHAR *addr, HI_U32 Length, HI_U32 imgWidth, HI_U32 imgHeight,
    HIGV_HANDLE *imgSurface);

/*
* brief Destroy image surface.
* param[in] imgSurface Image surface handle.
* retval ::HI_SUCCESS
* retval ::HIGO_ERR_NOTINIT
* retval ::HIGO_ERR_INVHANDLE
* retval ::HIGO_ERR_INUSE
* see ::
* HI_GV_GraphicContext_DecodeImg
* HI_GV_GraphicContext_DecodeMemImg
*/
HI_S32 HI_GV_GraphicContext_FreeImageSurface(HIGV_HANDLE imgSurface);

/*
* brief Set foreground color.
* param[in] contextHandle GraphicContext handle.
* param[in] foregroundColor Foreground color.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_GraphicContext_SetFgColor(HIGV_HANDLE contextHandle, HI_COLOR foregroundColor);

/*
* brief Set background color.
* Set text background colorN/A.
* param[in] contextHandle GraphicContext handle.
* param[in] backgroundColor text background color.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_GraphicContext_SetBgColor(HIGV_HANDLE contextHandle, HI_COLOR backgroundColor);

/*
* brief Draw line .
* The (startX,startY,endX,endY) >= 0 && < Widget width.
* param[in] contextHandle GraphicContext handle.
* param[in] startX,startY Start point.
* param[in] endX,endY End point.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_INVSIBLE
* retval ::HIGO_ERR_UNSUPPORTED
* retval ::HIGO_ERR_INVPIXELFMT
* retval ::HIGO_ERR_DEPEND_TDE
*/
HI_S32 HI_GV_GraphicContext_DrawLine(HIGV_HANDLE contextHandle, HI_U32 startX, HI_U32 startY,
    HI_U32 endX, HI_U32 endY, HI_U32 width);

/*
* brief Draw surface picture.
* param[in] contextHandle GraphicContext handle.
* param[in] dstRect Destination rect(null indicate full surface).
* 0,0ʼ \param[in] imageHandle Image surface handle.
* surface).
* operation.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_INVSIBLE
*/
HI_S32 HI_GV_GraphicContext_DrawImage(HIGV_HANDLE contextHandle, const HI_RECT *dstRect,
    HIGV_HANDLE imageHandle, const HI_RECT *srcRect, const HIGO_BLTOPT_S *blitOpt);

/*
* brief Draw string.
* param[in] contextHandle GraphicContext handle.
* param[in] text String.
* param[in] rect Paint rect on the bind widget surface.
* param[in] align Alignment of text.
* HIGV_ALIGN_RIGHT
* HIGV_ALIGN_HCENTER
* HIGV_ALIGN_TOP
* HIGV_ALIGN_BOTTOM
* HIGV_ALIGN_VCENTER
* HIGV_ALIGN_WRAP
* HIGV_ALIGN_WORDELLIPSIS
* HIGV_ALIGN_FORCE_LEFT
* HIGV_ALIGN_FORCE_RIGHT
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_INVSIBLE
* retval ::HIGO_ERR_NULLPTR
*/
HI_S32 HI_GV_GraphicContext_DrawText(HIGV_HANDLE contextHandle, const HI_CHAR *text,
    const HI_RECT *rect, HI_U32 align);

/*
* brief Draw text by string ID.
* param[in] contextHandle GraphicContext handle.
* param[in] strID String ID.
* param[in] rect Paint rect on the bind widget surface.
* param[in] align Alignment of text.
* HIGV_ALIGN_RIGHT
* HIGV_ALIGN_HCENTER
* HIGV_ALIGN_TOP
* HIGV_ALIGN_BOTTOM
* HIGV_ALIGN_VCENTER
* HIGV_ALIGN_WRAP
* HIGV_ALIGN_WORDELLIPSIS
* HIGV_ALIGN_FORCE_LEFT
* HIGV_ALIGN_FORCE_RIGHT
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_INVSIBLE
* retval ::HIGO_ERR_NULLPTR
*/
HI_S32 HI_GV_GraphicContext_DrawTextByID(HIGV_HANDLE contextHandle, const HI_U32 strID,
    const HI_RECT *rect, HI_U32 align);

/*
* brief Set text font.
* param[in] contextHandle GraphicContext handle.
* param[in] fontHandle Font handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_GraphicContext_SetFont(HIGV_HANDLE contextHandle, HIGV_HANDLE fontHandle);

/*
* brief Fill rect.
* param[in] contextHandle GraphicContext handle.
* param[in] rect Fill rect on the bind widget surface.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_INVSIBLE
* see :: HI_GV_GraphicContext_SetBgColor
*/
HI_S32 HI_GV_GraphicContext_FillRect(HIGV_HANDLE contextHandle, const HI_RECT *rect);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* HI_GV_GRAPHICCONTEXT_H */
