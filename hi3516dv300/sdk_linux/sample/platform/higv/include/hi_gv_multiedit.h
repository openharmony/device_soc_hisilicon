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

#ifndef HI_GV_MULTIEDIT_H
#define HI_GV_MULTIEDIT_H
/* add include here */
#include "hi_gv_widget.h"
#ifdef HIGV_USE_WIDGET_MULTIEDIT
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    HI_U32 charNum; /*  Limit the number of char */
    HI_BOOL isReadOnly; /*  Only read flag */
    HI_BOOL isSingleLine; /*  Single line flag */
    HIGV_HANDLE font; /*  Font handle */
    HI_COLOR selectBgColor; /*  Selected background color */
    HI_COLOR selectFgColor; /*  Selected foreground color */
    HI_COLOR cursorColor; /*  Cursor color */
    HI_U32 lineSpace; /*  Space between line */
    HI_U32 charSpace; /*  Space between char */
} HigvMultiEditStyle;

typedef enum {
    /*  Left side */
    HIGV_MULTIEDIT_SIDE_LEFT = 0,
    /*  Right side */
    HIGV_MULTIEDIT_SIDE_RIGHT = 1,
    /*  Top side */
    HIGV_MULTIEDIT_SIDE_TOP = 2,
    /*  Down side */
    HIGV_MULTIEDIT_SIDE_BOTTOM = 3,
} HigvMultiEditSlide;

/*
\brief Init the multiedit.
\attention \n
N/A.
\param[in]  editHandle multiedit handle.
\param[in]  style The info of multiedit when created.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\retval ::HI_ERR_COMM_NORES
\see \n
N/A.
*/
HI_S32 HI_GV_MULTIEDIT_Init(HIGV_HANDLE editHandle, const HigvMultiEditStyle *style);

/*
\brief Set the content of edit.
\attention \n

\param[in] editHandle  multiedit handle.
\param[in] content  Current content.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
::HI_GV_MULTIEDIT_GetContent
*/
HI_S32 HI_GV_MULTIEDIT_SetContent(HIGV_HANDLE editHandle, const HI_CHAR *content);

/*
\brief Get the display content of edit.
\attention \n
N/A.
\param[in]  editHandle multiedit handle.
\param[out] content The dipslay content.
\param[in/out] length The dipslay content length.
ΪΪҪȡݳ,ΪΪʵݵó
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
::HI_GV_MULTIEDIT_SetContent
::HI_GO_LayoutGetText
*/
HI_S32 HI_GV_MULTIEDIT_GetContent(HIGV_HANDLE editHandle, HI_CHAR *content, HI_U32 *length);

/*
\brief Clear the display content of edit.
\attention \n
N/A.
\param[in]  editHandle multiedit handle.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
::HI_GV_MULTIEDIT_SetContent
*/
HI_S32 HI_GV_MULTIEDIT_ClearContent(HIGV_HANDLE editHandle);

/*
\brief Get select string.
\attention \n
N/A.
\param[in]  editHandle multiedit handle.
\param[out] str UTF8 string to output.
\param[in/out] len Length of output string.
ΪΪҪȡִ,ΪΪʵִĳ
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\retval ::HI_ERR_COMM_EMPTY
\see \n
N/A.
*/
HI_S32 HI_GV_MULTIEDIT_GetSelectString(HIGV_HANDLE editHandle, HI_CHAR *str, HI_U32 *len);
/*
\brief Page up.
\attention \n
N/A.
\param[in]  editHandle multiedit handle.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\see \n
HI_GV_MULTIEDIT_PageDown
*/
HI_S32 HI_GV_MULTIEDIT_PageUp(HIGV_HANDLE editHandle);

/*
\brief Page down.
\attention \n
N/A.
\param[in]  editHandle multiedit handle.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\see \n
HI_GV_MULTIEDIT_PageUp
*/
HI_S32 HI_GV_MULTIEDIT_PageDown(HIGV_HANDLE editHandle);

/*
\brief Set cursor color.
\attention \n
N/A.
\param[in]  editHandle multiedit handle.
\param[in]  color cursor color.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\see \n
N/A.
*/
HI_S32 HI_GV_MULTIEDIT_SetCursorColor(HIGV_HANDLE editHandle, HI_COLOR color);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_MULTIEDIT_H */
