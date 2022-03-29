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

#ifndef HI_GV_MSGBOX_H
#define HI_GV_MSGBOX_H

/* add include here */
#include "hi_gv_conf.h"
#include "hi_type.h"
#include "hi_gv_widget.h"

#ifdef HIGV_USE_WIDGET_MSGBOX

#ifdef __cplusplus
extern "C" {
#endif

#define MAXBUTTONNUM   4

/* Standard Message button ID */
#define MSG_BUTTONID_INVALID    0
#define MSG_BUTTONID_OK         1
#define MSG_BUTTONID_CANCEL     2
#define MSG_BUTTONID_ABORT      3
#define MSG_BUTTONID_RETRY      4
#define MSG_BUTTONID_IGNORE     5
#define MSG_BUTTONID_YES        6
#define MSG_BUTTONID_NO         7

/* customized style */
typedef enum {
    MSGBOX_STYLE_NONE = 0,               /* None button */
    MSGBOX_STYLE_OK = 1,                 /* Only OK */
    MSGBOX_STYLE_OKCANCEL = 2,           /* OK+cancel */
    MSGBOX_STYLE_RETRYCANCEL = 3,        /* Retry+cancel */
    MSGBOX_STYLE_ABORTRETRYIGNORE = 4,   /* Abort+retry+ignore */
    MSGBOX_STYLE_YESNOCANCEL = 5,        /* Yes+no+cancel */
    MSGBOX_STYLE_USERDEF = 6             /* User-Defined */
} HigvMsgBoxStyle;


/* MessageBox title info struction */
typedef struct {
    HI_U32  titleHeight;                /* Title height */
    HI_U32  titleAlignment;             /* Title alignment */
    HI_RESID titleSkin;                 /* Title skin style */
    HIGV_HANDLE titleFont;                /* Text font of title */
} HigvMsgBoxTitle;


/* MessageBox button info struction */
typedef struct {
    HI_U32  buttonAreaHeight;       /* Button area height */
    HI_U32  buttonCount;            /* Button count */
    HI_U32  buttonHeight;           /* Button widget height */
    HI_U32  buttonWidth;            /* Button widget width */
    HI_RESID buttonNormalSkin;      /* Button widget normal skin style */
    HI_RESID buttonActiveSkin;      /* Button widget active skin style */
    HI_RESID buttonHighlightSkin;   /* Button widget highlight skin style */
    HI_RESID buttonMouseDownSkin;   /* Button widget mouse down skin style */
    HI_RESID buttonAreaSkin;        /* Button area skin style */
    HIGV_HANDLE buttonFont;           /* Text font of button widget */
} HigvMsgBoxButton;


/* MessageBox info struction for create */
typedef struct {
    HigvMsgBoxStyle msgBoxStyle;    /* Message box style */
    HigvMsgBoxTitle titleInfo;      /* Title info */
    HigvMsgBoxButton buttonInfo;    /* Button info */
    HI_U32 showTimes;               /* Show times */
    HI_RESID icon;                  /* Icon res ID */
} HigvMsgBoxCreateInfo;

/*
\brief Init Message box.
\attention \n
N/A.
\param[in]  handle  Message box handle.
\param[in]  createInfo  Create information.

\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL

\see \
N/A.
*/
HI_S32 HI_GV_MSGBOX_Init(HIGV_HANDLE handle, const HigvMsgBoxCreateInfo* createInfo);

/*
\brief Set title text.
\attention \n
N/A.
\param[in]  handle  Msg box handle.
\param[in]  text    Text of Message box title.Cncomment:ı

\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\retval ::HI_ERR_COMM_NOTREADY

\see \
:: HI_GV_MSGBOX_GetTitleText
*/
HI_S32 HI_GV_MSGBOX_SetTitleText(HIGV_HANDLE handle, const HI_CHAR* text);

/*
\brief Get title text.
\attention \n
N/A.
\param[in] handle   Widget handle.
\param[out] buf     Text content.
\param[in] bufLen    Buffer length.

\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\retval ::HI_ERR_COMM_NOTREADY
\see \n
:: HI_GV_MSGBOX_SetTitleText
*/
HI_S32  HI_GV_MSGBOX_GetTitleText(HIGV_HANDLE handle, HI_CHAR* buf, HI_U32 bufLen);

/*
\brief Set title text by ID.
\attention \n
N/A.
\param[in] handle  Widget handle.
\param[in] strID    String ID.

\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_NOTREADY
\see \n
::HI_GV_MSGBOX_GetTitleTextID
*/
HI_S32 HI_GV_MSGBOX_SetTitleTextByID(HIGV_HANDLE handle, const HI_U32 strID);

/*
\brief Get title text by ID.
\attention \n
N/A.
\param[in] handle  Widget handle.
\param[out] strID   String ID.

\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\retval ::HI_ERR_COMM_NOTREADY
\see \n
:: HI_GV_ MSGBOX_SetTitleTextByID
*/
HI_S32  HI_GV_MSGBOX_GetTitleTextID(HIGV_HANDLE handle, HI_U32* strID);

/*
\brief Set icon res.
\attention \n
N/A.
\param[in] handle  Widget handle.
\param[in] icon     Icon res ID.

\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval:: HI_ERR_COMM_NOTREADY

\see \n
N/A.
*/
HI_S32 HI_GV_MSGBOX_SetIcon(HIGV_HANDLE handle, HI_RESID icon);

/*
\brief Set button layout.
\attention \n
Only User-Defined style can availability.
The button index from left to right.
\param[in] handle  Widget handle.
\param[in] Button[] The button ID array.
\param[in] length The length of button ID array.

\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_NOTREADY

\see \n
N/A.
*/
HI_S32 HI_GV_MSGBOX_ButtonLayout(HIGV_HANDLE handle, const HI_S32 buttonID[MAXBUTTONNUM], HI_S32 length);

/*
\brief Set inchoative button index.
\attention \n
N/A.
\param[in] handle   Widget handle.
\param[in] buttonID  Button ID.

\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\retval ::HI_ERR_COMM_NOTREADY

\see \n
N/A.
*/
HI_S32 HI_GV_MSGBOX_SetInitFocusButton(HIGV_HANDLE handle,  HI_S32 buttonID);

/*
\brief Get child button handle.
\attention \n
N/A.
\param[in] handle   Widget handle.
\param[out] buttonArray  Button Handle Array.
\param[in] length  The length of Button Handle Array.

\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\retval ::HI_ERR_COMM_NOTREADY

\see \n
N/A.
*/
HI_S32 HI_GV_MSGBOX_GetButtonHandle(HIGV_HANDLE handle,  HIGV_HANDLE buttonArray[MAXBUTTONNUM], HI_S32 length);

/*
\brief Sync get key down button index.
\attention \n
N/A.
\param[in] handle  Msg box handle.

\retval :: MSG_BUTTONID
\retval :: MSG_BUTTONID_INVALID

\see \n
N/A.
*/
HI_S32 HI_GV_MSGBOX_Show(HIGV_HANDLE handle);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_MSGBOX_H */
