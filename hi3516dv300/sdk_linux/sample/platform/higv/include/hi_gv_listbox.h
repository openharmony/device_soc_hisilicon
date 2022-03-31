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

#ifndef HI_GV_LIST_H
#define HI_GV_LIST_H
#include "hi_gv_conf.h"
#ifdef HIGV_USE_WIDGET_LISTBOX
/* add include here */
#include "hi_gv_resm.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  Max col number */
#define HIGV_LISTBOX_COL_MAX 20

typedef enum {
    LIST_COLTYPE_TXT = 0,
    /* Text */
    LIST_COLTYPE_IMG,
    /* Picture */
    LIST_COLTYPE_TXTICONLEFT,
    /* Text and icon,icon on left */
    LIST_COLTYPE_TXTICONRIGHT,
    /* Text and icon,icon on right */
    LIST_COLTYPE_BUTT
} HIGV_LIST_COLTYPE_E;

typedef HI_S32 (*HIGV_LIST_CONV_CB)(HIGV_HANDLE listHandle, HI_U32 col, HI_U32 item, const HI_CHAR *srcStr,
                                    HI_CHAR *dstStr, HI_U32 length);

/* At cell focus mode, the callback function is used to get the cell width */

typedef HI_S32 (*HIGV_GET_WIDTH_CB)(HIGV_HANDLE listHandle, HI_U32 item, HI_U32 col);

typedef struct {
    HIGV_LIST_COLTYPE_E Type; /*  Column type */
    HI_U32 Width; /*  Column width */
    HI_U32 Align; /*  Alignment text */
    HI_COLOR Fgidx; /*  Font color of column text */
    HIGV_HANDLE hImage;  /* Picture surface handle (only support type is LIST_COLTYPE_IMG)
                            */
    HI_U32 FieldColIdxInDb; /* Column index in db */
    HIGV_LIST_CONV_CB ConvString;  /* function which convert string (only support type is LIST_COLTYPE_TXT)
                                    */
} HIGV_LIST_COLATTR_S;

typedef struct {
    HI_U32 RowNum; /* Row number */
    HI_U32 ColNum; /* Column number */
    HI_BOOL NoFrame; /* Is no frame Draw */
    HI_BOOL Cyc; /* Is cycle */
    HI_BOOL IsCellActive; /* Is cell active */
    HI_BOOL AutoSwitchCell; /* If cell active auto suwich active cell */
    HI_BOOL Scroll; /* Is scroll */
    HI_BOOL Fromleft; /* Is Scroll from left */
    HI_U32 Timeinterval; /* Time interval */
    HI_U32 Step; /* Scroll step */
    HI_U32 ScrollCol; /* If item active,scroll col of item */
    HIGV_GET_WIDTH_CB GetWidthCb;  /*  Function get cell width from call back */
    HIGV_LIST_COLATTR_S *pColAttr;  /* List column attributes array */
} HIGV_LIST_ATTRIBUTE_S;

/*
* brief Init the listbox.
* param[in] listHandle
* param[in] rowNum  Row number of display.
* param[in] colNum   Column number.
* param[in] colAttr Column attribute array.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_List_Init(HIGV_HANDLE listHandle, HI_U32 rowNum, HI_U32 colNum, const HIGV_LIST_COLATTR_S *colAttr);

/*
* brief Init the listbox.
* param[in] listHandle
* param[in] listAttr  Init listbox attributes.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_List_InitEx(HIGV_HANDLE listHandle, const HIGV_LIST_ATTRIBUTE_S *listAttr);

/*
* brief Get the content of listbox.
* param[in] listHandle   Listbox handle.
* param[in] item  Item index.
* param[in] col   Column index.
* param[out] value Cell's content.
* param[in] length Length of pValue.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_List_GetCell(HIGV_HANDLE listHandle, HI_U32 item, HI_U32 col, HI_CHAR *value, HI_U32 length);

/*
* brief Set skin of focus item.
* param[in] listHandle    Listbox handle.
* param[in] selSkin   The skin of Selected status.
* param[in] normSkin  The skin of normal status.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_List_SetRowSkin(HIGV_HANDLE listHandle, HIGV_HANDLE selSkin, HIGV_HANDLE normSkin);

/*
* brief Set selected item of listbox.
* attention ::The api is only support the selected item and apply this api at the init stage.
*
* param[in] listHandle Listbox handle.
* param[in] item  Item index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_PAERM Focus moe error.
* see :: HI_GV_List_GetSelItem
*/
HI_S32 HI_GV_List_SetSelItem(HIGV_HANDLE listHandle, HI_U32 item);

/*
* brief Get the item of listbox.
* param[in] listHandle  Listbox handle.
* param[out] item Item index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see :: HI_GV_List_SetSelItem
*/
HI_S32 HI_GV_List_GetSelItem(HIGV_HANDLE listHandle, HI_U32 *item);

/*
* brief Get the total item number of listbox.
* param[in]  listHandle  Listbox handle.
* param[out] itemNum Toal item number.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_List_GetItemNum(HIGV_HANDLE listHandle, HI_U32 *itemNum);

/*
* brief
* param[in]  listHandle
* retval ::HI_TRUE
* retval ::HI_FALSE
*/
HI_BOOL HI_GV_List_IsListBoxType(HIGV_HANDLE listHandle);

/*
* brief Set the attribute of listbox.
* param[in]  listHandle  Listbox handle.
* param[in]  horizontalLineHeight  The height of horizontal line.
* param[in]  horizontalLineColor  The color of horizontal line.
* param[in]  verticalLineWidth The width of vertical line.
* param[in]  verticalLineColor  The color of vertical line.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_List_GetGridding
*/
HI_S32 HI_GV_List_SetGridding(HIGV_HANDLE listHandle, HI_U32 horizontalLineHeight,
    HI_COLOR horizontalLineColor, HI_U32 verticalLineWidth, HI_COLOR verticalLineColor);

/*
* brief Get the attribute of listbox.
* param[in]  listHandle  Listbox handle.ؼ
* param[out]  horizontalLineHeight The height of horizontal line.
* param[out]  horizontalLineColor  The color of horizontal line.
* param[out]  verticalLineWidth  The width of vertical line.
* param[out]  verticalLineColor  The color of vertical line.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_List_SetGridding
*/
HI_S32 HI_GV_List_GetGridding(HIGV_HANDLE listHandle, HI_U32 *horizontalLineHeight,
    HI_COLOR *horizontalLineColor, HI_U32 *verticalLineWidth, HI_COLOR *verticalLineColor);

/*
* brief Change the image of column.
* param[in] listHandle Listbox handle.
* param[in] col    Column index.
* param[in] imageHandle Picture handle.
* retval N/A.
*/
HI_S32 HI_GV_List_ChangeImage(HIGV_HANDLE listHandle, HI_U32 col, HIGV_HANDLE imageHandle);

/*
* brief Get the start index of current page.
* param[in] listHandle Listbox handle.
* param[out] item Item.
* retval N/A.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_List_GetStartItem(HIGV_HANDLE listHandle, HI_U32 *item);

/*
* brief Set the start index of current page.
* param[in] listHandle Listbox handle.
* param[in] item Item.
* retval N/A.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_List_SetStartItem(HIGV_HANDLE listHandle, HI_U32 item);

/*
* brief Get the index of the end item.
* param[in] listHandle Listbox handle.
* param[out] item Item.
* retval N/A.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_List_GetEndItem(HIGV_HANDLE listHandle, HI_U32 *item);

/*
* brief Set the foucus of listbox.
* attention : The api is only support the selected cell.
*
* param[in] listHandle  Listbox handle.
* param[in] item   Row index.
* param[in] col    Column index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_PAERM Error at focus mode.
* see ::HI_GV_List_GetSelCell
*/
HI_S32 HI_GV_List_SetSelCell(HIGV_HANDLE listHandle, HI_U32 item, HI_U32 col);

/*
* brief Get the position of cell which selected.
* param[in] listHandle  Listbox handle.
* param[out] item   row index.
* param[out] col   Column index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_List_SetSelCell
*/
HI_S32 HI_GV_List_GetSelCell(HIGV_HANDLE listHandle, HI_U32 *item, HI_U32 *col);

/*
* brief Get the width of cell at focus mode.
* attention : if the cell not display, it will return 0.
*
* param[in] listHandle  Listbox handle.
* param[in] item   item index.
* param[in] col    Column index.
* param[out] pWidth  The pointer of width.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_List_GetCellWidth(HIGV_HANDLE listHandle, HI_U32 item, HI_U32 col, HI_U32 *width);

/*
* brief Set scroll step of text.
* attention : it is only valid at focus mode.
*
* param[in] listHandle Listbox handle.
* param[in] step Scroll step(unit:pixel, default:10piexl).
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_PAERM Error at focus mode.
*/
HI_S32 HI_GV_List_SetStep(HIGV_HANDLE listHandle, HI_U32 step);

/*
* brief Set the scroll direction of listbox.
* attention : it is only valid at focus mode.
*
* param[in] listHandle Listbox handle.
* param[in] fromLeft Scroll direction.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_PAERM Error at focus mode.
*/
HI_S32 HI_GV_List_SetDirection(HIGV_HANDLE listHandle, HI_BOOL fromLeft);

/*
* brief  Set the status of widget.
* attention : it is only valid at focus mode.
*
* param[in] listHandle Listbox handle.
* param[in] scroll Scroll status.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_PAERM Error at focus mode.
*/
HI_S32 HI_GV_List_SetScroll(HIGV_HANDLE listHandle, HI_BOOL scroll);

/*
* brief Set the foreground color of column.
* attention :it is only support the column type is LIST_COLTYPE_TXT,
* if col is -1, it will effect the color of text to all column .
*
* param[in] listHandle Listbox handle.
* param[in] col   Column index.
* param[in] index Foreground color index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_PAERM Unsupported the column to set picture.
*/
HI_S32 HI_GV_List_SetColFgIdx(HIGV_HANDLE listHandle, HI_U32 col, HI_COLOR index);

/*
* brief Set the exterior line of listbox.
* param[in] listHandle Listbox handle.
* param[in] noFrame Is not draw.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_List_SetNoFrame(HIGV_HANDLE listHandle, HI_BOOL noFrame);

/*
* brief Set the focus whether cycle.
* attention :It is only valid at focus mode.
*
* param[in] listHandle Listbox handle.
* param[in] cycle Focus cycle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_List_SetCyc(HIGV_HANDLE listHandle, HI_BOOL cycle);

/*
* brief Set scroll param.
\attention \n
N/A.
* param[in] listHandle Listbox handle.
* param[in] scrollParam scroll index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_List_SetScrollParam(HIGV_HANDLE listHandle, HI_FLOAT scrollParam);

/*
* brief Set fling param.
* param[in] listHandle Listbox handle.
* param[in] flingParam fling index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_List_SetFlingParam(HIGV_HANDLE listHandle, HI_S32 flingParam);

#ifdef __cplusplus
}
#endif
#endif
#endif /* HI_GV_LIST_H */
