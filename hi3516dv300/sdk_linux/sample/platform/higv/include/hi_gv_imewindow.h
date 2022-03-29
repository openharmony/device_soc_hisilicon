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

#ifndef HI_GV_IMEWINDOW_H
#define HI_GV_IMEWINDOW_H

/* add include here */
#include "hi_gv_widget.h"
#ifdef HIGV_USE_MODULE_IME
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HI_GV_IMEWindow_ENGLISH = 0,
    HI_GV_IMEWindow_CAPENGLISH,
    HI_GV_IMEWindow_NUMBER,
    HI_GV_IMEWindow_PINYIN,
    HI_GV_IMEWindow_SYBMOL,
    HI_GV_IMEWindow_STANDARD_A,
    HI_GV_IMEWindow_STANDARD_B,
    HI_GV_IMEWindow_BUTT
} HigvIMEWindowType;

typedef struct {
    HI_U32 pinyinCharRectLeftMargin; /* Left margin of pingyin input rect */
    HI_U32 pinyinCharRectUpMargin; /* Up margin of pingyin input rect */
    HI_U32 pinyinCharRectDownMargin; /* Down margin of pingyin input rect */

    HI_U32 logPicRectRightMargin; /* Right margin of logo */
    HI_U32 logPicRectUpMargin; /* Up margin of logo */
    HI_U32 logPicRectDownMargin; /* Down margin of logo */
    HI_U32 logPicRectWidth; /* The width of logo */
    HI_U32 logPicRectHeight; /* The height of logo */

    HI_U32 logPicAndCharRectSpace; /* The space between logo and input char rect */

    HI_U32 lArrowPicRectLeftMargin; /* The space margin of left arrow rect */
    HI_U32 lArrowPicRectUpMargin; /* The up margin of bottom arrow rect */
    HI_U32 lArrowPicRectDownMargin; /* The down margin of bottom arrow rect */
    HI_U32 lArrowPicRectWidth; /* The height of left arrow rect */
    HI_U32 lArrowPicRectHeight; /* The width of left arrow rect */

    HI_U32 rArrowPicRectRightMargin; /* Right margin of right arrow rect */
    HI_U32 rArrowPicRectUpMargin; /* Up margin of right arrow rect */
    HI_U32 rArrowPicRectDownMargin; /* Bottom margin of right arrow rect */
    HI_U32 rArrowPicRectWidth; /* The height of right arrow rect */
    HI_U32 rArrowPicRectHeight; /* The height of left arrow rect */

    HI_U32 twoArrowSpace; /* The space between left and right margin */

    HI_U32 candidataWordUpMargin; /* Top margin of first level candidate */
    HI_U32 candidataWordDownMargin; /* Down margin of first level candidate */
    HI_U32 candidataWordLeftMargin; /* Left margin of first level candidate */
} HigvIMEWindowInitEx;

typedef struct {
    HIGO_PF_E pixelFormat; /* Input method pixel format */
    HI_PARAM englishLogoIndex; /* english input method logo */
    HI_PARAM capEnglishLogoIndex; /* Caps english input method logo */
    HI_PARAM symbolLogoIndex; /* Symbol input method logo */
    HI_PARAM numberLogoIndex; /* Number input method LOGO */
    HI_PARAM pinyinLogoIndex; /* Pinyin input method LOGO */
    HI_PARAM standardALogoIndex; /* Standard input method A logo */
    HI_PARAM standardBLogoIndex; /* Standard input method B logo */
    HI_PARAM leftArrowPicIndex; /* Left arrow picture */
    HI_PARAM rightArrowPicIndex; /* Right arrow picture */
    HI_PARAM unActiveLeftArrowPicIndex; /* Unactive status of left arrow */
    HI_PARAM unActiveRightArrowPicIndex; /* Unactive status of right arrow */
    HI_PARAM pinYinTablePath; /* path of pinyin code table */
    HI_PARAM pinYinTablePathLen; /* path length of pinyin code table */
    HigvIMEWindowInitEx *exData; /* Change input method size */
} HigvIMEWindowInit;

typedef struct {
    HIGV_HANDLE keyWidget;
    HI_U32 keyValue;
} HigvIMEWindowKeyValuePair;

typedef struct {
    HI_BOOL isNoDispIMW; /* Whether display input method window, default show. */
    HIGV_HANDLE softKB; /* Soft keyboard handle */
    HI_BOOL isDispSoftKB; /* Whether show softkeyboard */
    HigvIMEWindowKeyValuePair *keyValueMap; /* key map */
    HI_U32 keyNum; /* Key number */
} HigvIMEWindowSoftKB;

/*
\brief Create input method window(default height/width:100/42).
\param[in]  creatInfo Create information.
\param[out] handle Input method window handle.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\retval ::HI_EEXIST
\see \n
N/A.
*/
HI_S32 HI_GV_IMEWINDOW_Create(const HIGV_WCREATE_S *creatInfo, HigvIMEWindowInit *initData, HIGV_HANDLE *handle);

/*
\brief Destroy input method window.
\param[in] handle
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_IMEWINDOW_Destroy(HIGV_HANDLE handle);

/*
\brief Switch input method.
\param[in] handle
\param[in] imeType
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_IMEWINDOW_Change(HIGV_HANDLE handle, HigvIMEWindowType imeType);

/*
\brief Open/close the input method.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_IMEWINDOW_Enable(HigvIMEWindowType imeType, HI_BOOL isEnable);

/*
\brief Set the switch window at init stage.
\attention \n
N/A.
\param[in]  handle   Input method window handle.
\param[in]  switchWindowHandle  switch window
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_IMEWINDOW_SetSwitchIMWin(HIGV_HANDLE handle, HIGV_HANDLE switchWindowHandle);

/*
\brief Bind the softkeyboard to the input window at init stage.
\attention \n
N/A.
\param[in]  handle   Input method window handle.
\param[in]  softKBArray Softkey board.
\param[in]  imeNum   Input method number.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_IMEWINDOW_SetSoftKB(HIGV_HANDLE handle, const HigvIMEWindowSoftKB *softKBArray, HI_U32 imeNum);

/*
\brief Get the show status of input window.
뷨л)ʾ״̬
\attention \n
N/A.
\param[in]  handle Input method window handle.
\retval ::HI_TRUE  Show.
\retval ::HI_FALSE Hide.
\see \n
N/A.
*/
HI_BOOL HI_GV_IMEWINDOW_IsShowIM(HIGV_HANDLE handle);

/*
\brief Get the show status of softkeyboard.
\attention \n
N/A.
\param[in]  handle Input mesthod window handle.
\retval ::HI_TRUE  Show.
\retval ::HI_FALSE Hdie.
\see \n
N/A.
*/
HI_BOOL HI_GV_IMEWINDOW_IsShowSoftKB(HIGV_HANDLE handle);

/*
\brief Set the show status of soft keyboard.
\attention \n
N/A.
\param[in]  handle   Input method window handle.
\param[in]  softKBHandle  Softkeyboad winddow handle.
\param[in]  isShow  show/hide.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_IMEWINDOW_SetDispSoftKB(HIGV_HANDLE handle, HIGV_HANDLE softKBHandle, HI_BOOL isShow);

/*
\brief Get the active widget which get the input focus.
\attention \n
N/A.
\param[in]  handle    Input method window handle.
\param[out]  activeWidget  The active widget handle.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_IMEWINDOW_GetIMEActiveWidget(HIGV_HANDLE handle, HIGV_HANDLE *activeWidget);

/*
\brief Get the current input method.
\attention \n
N/A.
\param[in]  handle  Input method window handle.
\param[out]  imeType  The pointer of inpute method.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_IMEWINDOW_GetCurrentIME(HIGV_HANDLE handle, HigvIMEWindowType *imeType);

/*
\brief Reset the key map of softkeyboard.
\attention \n
N/A.
\param[in]  imeType  Input method.
\param[in]  keyValue   The pointer of Key value.
\param[in]  num   Key value number.
\param[out]
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_IMEWINDOW_SetKeyValue(HigvIMEWindowType imeType, const HI_U32 *keyValue, HI_U32 num);

/*
\brief ʾĻĹ̶λ
\attention \n

\param[in]  handle    뷨
\param[in]  x    Ļx
\param[in]  y    Ļy
\retval ::HI_SUCCESS ɹ
\retval ::HI_ERR_LOST Ч
\retval ::HI_ERR_INVAL Ч
\see \n

*/
HI_S32 HI_GV_IMEWINDOW_PinSoftKB(HIGV_HANDLE handle, HI_S32 x, HI_S32 y);

/*
\brief ̶ʾʾλ
\attention \n

\param[in]  handle    뷨
\retval ::HI_SUCCESS ɹ
\retval ::HI_ERR_LOST Ч
\retval ::HI_ERR_INVAL Ч
\see \n

*/
HI_S32 HI_GV_IMEWINDOW_UnPinSoftKB(HIGV_HANDLE handle);

/*
\brief 뷨ں̴ڵλ
\attention \n

\param[in]  handle    뷨
\param[in]  isFlexible    뷨ں̴ڵλõĿ
\retval ::HI_SUCCESS óɹ
\retval ::HI_ERR_LOST Ч
\retval ::HI_ERR_INVAL Ч
\see \n

*/
HI_S32 HI_GV_IMEWINDOW_FlexibleSoftKB(HIGV_HANDLE handle, HI_BOOL isFlexible);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_IMEWINDOW_H */
