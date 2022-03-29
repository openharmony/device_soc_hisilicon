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

#ifndef HI_GV_MLAYER_H
#define HI_GV_MLAYER_H

#include "hi_go.h"
#include "hi_gv_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The event of window move or size change */
#define HIGV_MSG_WINDOW_MOVE 0x502

typedef enum {
    HIGV_ROTATE_NONE = 0,
    HIGV_ROTATE_90,
    /* <Rotate 90 degrees clockwise  , */
    HIGV_ROTATE_180,
    /* <Rotate 180 degrees clockwise , */
    HIGV_ROTATE_270,
    /* <Rotate 270 degrees clockwise , */
    HIGV_ROTATE_BUTT
} HIGV_ROTATE_E;

/*
* brief Create layer.
* param[in]  layerInfo Layer information.
* param[out] layerHandle The pointer of layer handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Layer_Create(const HIGO_LAYER_INFO_S *layerInfo, HIGV_HANDLE *layerHandle);

/*
* brief Create Layer, the handle is used by xml.
* param[in]  layerInfo Layer information.
* param[in]  layerHandle Layer handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Layer_CreateEx(const HIGO_LAYER_INFO_S *layerInfo, HIGV_HANDLE layerHandle);

/*
* brief Destroy layer.
* param[in] layerHandle Layer handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Layer_Destroy(HIGV_HANDLE layerHandle);

/*
* brief show/hide the layer.
* param[in] layerHandle Layer handle.
* param[in] visible The flag indicate show or hide.
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_Layer_Show(HIGV_HANDLE layerHandle, HI_BOOL visible);

/*
* brief Set default layer.
*        默认为此图层
* param[in] layerHandle Layer handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Layer_SetDefault(HIGV_HANDLE layerHandle);

/*
* brief Get default layer.
* param[out] defaultLayer The pointer of default layer.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_EMPTY
*/
HI_S32 HI_GV_Layer_GetDefault(HIGV_HANDLE *defaultLayer);

/*
* brief Get the active window handle.
* param[in] layerHandle Layer handle.
* param[out] window Active window handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_EMPTY
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Layer_GetActiveWindow(HIGV_HANDLE layerHandle, HIGV_HANDLE *window);

/*
* brief Get the higo layer by higv layer handle.
* param[in] layerHandle Layer handle.
* param[out] higoLayer  higo layer handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_EMPTY
*/
HI_S32 HI_GV_Layer_GetHigoLayer(HIGV_HANDLE layerHandle, HIGV_HANDLE *higoLayer);

/*
* brief Set the layer the share source.
* param[in] ownerLayerHandle   Layer handle.
* param[in] cientLayerHandle  The share source between the layer.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_UNSUPPORT
*/
HI_S32 HI_GV_Layer_ShareSource(HIGV_HANDLE ownerLayerHandle, HIGV_HANDLE cientLayerHandle);

/*
* brief Get the active layer handle.
* param[out] layerHandle   Layer handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_EMPTY
*/
HI_S32 HI_GV_Layer_GetActiveLayer(HIGV_HANDLE *layerHandle);

/*
* brief Set active layer.
* param[in] layerHandle   Layer handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Layer_SetActiveLayer(HIGV_HANDLE layerHandle);

/*
* brief set rotate mode.
* attention depend on higo support\n
* attention only support HIGO_PF_4444 & HIGO_PF_1555 Pixel Format \n
* param[in] layerHandle   Layer handle.
* param[in] rotate   rotate.
* retval ::HI_SUCCESS
* retval ::HI_FAILURE
* see : HI_GV_Layer_SetRotateMode
*/
HI_S32 HI_GV_Layer_SetRotateMode(HIGV_HANDLE layerHandle, HIGV_ROTATE_E rotate);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_MLAYER_H */
