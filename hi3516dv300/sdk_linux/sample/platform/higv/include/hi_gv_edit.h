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

#ifndef HI_GV_EDIT_H
#define HI_GV_EDIT_H

/* add include here */
#include "hi_gv_widget.h"
#ifdef HIGV_USE_WIDGET_EDIT
#ifdef __cplusplus
extern "C" {
#endif

/* Edit type */
typedef enum {
    EDIT_TYPE_DEFAULT   = 0x0,      /* Default style */
    EDIT_TYPE_REPLACE   = 0x4,      /* replace style */
    EDIT_TYPE_RDONLY    = 0x10,     /* Readonly style */
} HigvEditType;

/*  Edit type define */
#define HIGV_EDITTYPE_DEFINE(type) ((HIGV_WIDGET_EDIT << 24) | ((type & 0xff) << 16))
#define HIGV_EDITTYPE_REPLACE HIGV_EDITTYPE_DEFINE(EDIT_TYPE_REPLACE)
#define HIGV_EDITTYPE_RDONLY HIGV_EDITTYPE_DEFINE(EDIT_TYPE_RDONLY)
#define HIGV_EDIT_PARSERTYPE(editType) (((editType) >> 16) & 0xff)

/*
* brief Set the limit count of edit.
* param[in] handle Edit handle.
* param[in] countLimit Count limit of edit.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NORES
* see
* HI_GV_Edit_SetCountLimit
*/
HI_S32 HI_GV_EDIT_SetCountLimit(HIGV_HANDLE handle, HI_U32 countLimit);

/*
* brief Get the limit count of edit.
* param[in]  handle Edit handle.
* param[out] count Edit count.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_LOST
* see
* HI_GV_EDIT_SetCountLimit
*/
HI_S32 HI_GV_EDIT_GetCountLimit(HIGV_HANDLE handle, HI_U32 *count);

/*
* brief Reset the index of cursor.
* param[in] handle Edit handle.
* param[in] cursorIndex Cursor Index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see
* HI_GV_EDIT_GetCursorIndex
*/
HI_S32 HI_GV_EDIT_SetCursorIndex(HIGV_HANDLE handle, HI_U32 cursorIndex);

/*
* brief Get index of cursor.
* param[in]  handle  Edit handle.
* param[out] cursorIndex  Cursor Index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see
* HI_GV_EDIT_SetCursorIndex
*/
HI_S32 HI_GV_EDIT_GetCursorIndex(HIGV_HANDLE handle, HI_U32 *cursorIndex);

/*
* brief Reset the position of cursor.
* param[in] handle Edit handle.
* param[in] cursorPos Cursor position.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see
* HI_GV_EDIT_GetCursorPos
*/
HI_S32 HI_GV_EDIT_SetCursorPos(HIGV_HANDLE handle, HI_U32 cursorPos);

/*
* brief Get position of cursor.
* param[in]  handle  Edit handle.
* param[out] cursorPos  Cursor position.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see
* HI_GV_EDIT_SetCursorPos
*/
HI_S32 HI_GV_EDIT_GetCursorPos(HIGV_HANDLE handle, HI_U32 *cursorPos);

/*
* brief Set the content of edit.
* param[in] handle  Edit handle.
* param[in] content  Current content.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see
* HI_GV_EDIT_GetContent
*/
HI_S32 HI_GV_EDIT_SetContent(HIGV_HANDLE handle, const HI_CHAR *content);

/*
* brief Get the display content of edit.
* param[in]  handle Edit handle.
* param[out] content The dipslay content.
* param[in]  length The length of dipslay buffer pContent.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see
* HI_GV_EDIT_SetContent
*/
HI_S32 HI_GV_EDIT_GetContent(HIGV_HANDLE handle, HI_CHAR *content, HI_U32 length);

/*
* brief Set the style of edit.
* param[in] handle Edit handle.
* param[in] type Edit type.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see
* HI_GV_EDIT_GetType
*/
HI_S32 HI_GV_EDIT_SetType(HIGV_HANDLE handle, HI_U32 type);

/*
* brief Get the type of edit.
* param[in]  handle edit handle.
* param[out] type edit type.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see
* HI_GV_EDIT_SetType
*/
HI_S32 HI_GV_EDIT_GetType(HIGV_HANDLE handle, HI_U32 *type);

/*
* brief Get the type of edit.
* param[in]  handle edit handle.
* param[out] isShow isshow content.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_EDIT_ShowReplace(HIGV_HANDLE handle, HI_BOOL isShow);

/*
* brief Clear content in edit box.
* param[in]  handle edit handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_EDIT_CleanContent(HIGV_HANDLE handle);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_EDIT_H */
