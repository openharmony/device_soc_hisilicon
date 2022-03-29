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

#ifndef HI_GV_IMAGE_H
#define HI_GV_IMAGE_H

#include "hi_type.h"
#include "hi_go.h"
#include "hi_gv_resm.h"

#ifdef HIGV_USE_WIDGET_IMAGE
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    HI_CHAR *pAddr; /*  Memory pointer address */
    HI_U32 Length; /*  Length */
} HI_GV_MemInfo;

/*
* brief Set image resource.
* param[in] imageHandle Image handle.
* param[in] image The resource ID of image.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Image_SetImage(HIGV_HANDLE imageHandle, HI_RESID image);

/*
* brief Draw surface image.
* attention :
* User should call HI_GV_Image_FreeMemSurface to release before call this api.
*
* Զػؼ
* param[in] imageHandle Image handle.
* param[in] memInfo Memory information.
* param[in] imageHeight Image hegiht.
* param[in] imageWidth Image width.
* param[in] srcRect Source rect.
* param[in] dstRect Destination rect.
* param[in] blitOpt Bit block transfer operation.
* param[in] transparent Transparent.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see :: HI_GV_Image_FreeMemSurface
*/
HI_S32 HI_GV_Image_DrawMemImage(HIGV_HANDLE imageHandle, const HI_GV_MemInfo *memInfo, HI_U32 imageHeight,
    HI_U32 imageWidth, const HI_RECT *srcRect, const HI_RECT *dstRect, const HIGO_BLTOPT_S *blitOpt,
    HI_BOOL transparent);


/*
* brief Draw surface picture.
* param[in] imageHandle Image handle.
* param[in] handleSrcSurface Higo surface handle.
* param[in] srcRect Source rect(null indicate full surface).
* ΪNULLʱsurface
* param[in] dstRect Destination rect(null indicate full surface).
* Ƶimageؼľ,ΪNULLʱӿؼ
* 0,0ʼ \param[in] blitOpt Bit block transfer operation.
* \param[in] transparent
* transparent.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Image_DrawSurface(HIGV_HANDLE imageHandle, HIGV_HANDLE handleSrcSurface,
    const HI_RECT *srcRect, const HI_RECT *dstRect, const HIGO_BLTOPT_S *blitOpt, HI_BOOL transparent);

/*
* brief Release the picture surface.
* attention ::
* Release the surface when unload the resource, and the widget
* will not redraw, so you should send repaint message to higv.
*
* ؼػ棬ػ뷢ͻϢ
* param[in] imageHandle Image handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* see :: HI_GV_Image_DrawMemImage
*/
HI_S32 HI_GV_Image_FreeMemSurface(HIGV_HANDLE imageHandle);

/*
* brief Set bit block transfer operation.
* param[in] imageHandle Image handle.
* param[in] blitOpt Bit block transfer operation.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Image_SetBlitOpt(HIGV_HANDLE imageHandle, const HIGO_BLTOPT_S *blitOpt);

/*
* brief Get bit block transfer operation.
* param[in] imageHandle Image handle.
* param[out] blitOpt Bit block transfer operation.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NOTINIT
*/
HI_S32 HI_GV_Image_GetBlitOpt(HIGV_HANDLE imageHandle, HIGO_BLTOPT_S *blitOpt);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_IMAGE_H */
