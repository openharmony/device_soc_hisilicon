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

#ifndef HI_GV_SCROLLGRID_H
#define HI_GV_SCROLLGRID_H
#include "hi_gv_conf.h"

#ifdef HIGV_USE_WIDGET_SCROLLGRID
/* add include here */
#include "hi_gv_resm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HIGV_SCROLLGRID_COL_MAX 20

typedef enum {
    SCROLLGRID_COLTYPE_TXT = 0, /* text */
    SCROLLGRID_COLTYPE_IMG, /* picture */
    SCROLLGRID_COLTYPE_IMG_MEM,  /* memory picture */
    SCROLLGRID_COLTYPE_BUTT
} HIGV_SCROLLGRID_COLTYPE_E;

/*  The callback function is uded to convert string */
typedef HI_S32 (*HIGV_SCROLLGRID_CONV_CB)(HIGV_HANDLE scrollGridPara, HI_U32 cellCol, HI_U32 cell,
    const HI_CHAR *srcStr, HI_CHAR *dstStr, HI_U32 length);

typedef struct _HIGV_SCROLLGRID_COLATTR_S { /* the column attribute structure; */
    HIGV_SCROLLGRID_COLTYPE_E Type; /* The type of column ; */
    HI_U32 Top; /* The top position of cell */
    HI_U32 Left; /* The top position of cell */
    HI_U32 Width; /* The width of cell */
    HI_U32 Height; /* The height of cell */
    HI_U32 Align; /* The alignment of text */
    HIGV_HANDLE hImage;  /*  The picture handle(only support the cloumn type is LIST_COLTYPE_IMG)
                            */
    HI_U32 FieldColIdxInDb;  /* The index in database corresponding column in scrollgrid */
    HIGV_SCROLLGRID_CONV_CB ConvString;  /* The convert string callback */
} HIGV_SCROLLGRID_COLATTR_S;

typedef enum _SCROLLGRID_DIRECTION_E {  /* Scroll direction */
    SCROLLGRID_DIRECTION_HORI,      /* <Horizal direction */
    SCROLLGRID_DIRECTION_VERT,  /* Vertical direction */
    SCROLLGRID_DIRECTION_BUTT
} HIGV_SCROLLGRID_DIRECTION_E;

typedef struct _HIGV_SCROLLGRID_STYLE_S {   /*  Layout structure */
    HI_U32 RowNum; /* Row number in a page */
    HI_U32 ColNum; /*  Column number in a page */
    HI_U32 LeftMargin; /* left margin of the page */
    HI_U32 RightMargin; /* Right margin of the page */
    HI_U32 TopMargin; /* Up margin of the page */
    HI_U32 BtmMargin; /* Bottom margin of the page */
    HI_U32 RowSpace; /* The space of row */
    HI_U32 ColSpace; /* The space of column */
    HI_U32 HLHeight; /* The height of horizontal line */
    HI_COLOR HLColor; /* The color of horizontal line  */
    HI_U32 VLWidth; /* The width of vertical line */
    HI_COLOR VLColor; /* The color of vertical line  */
    HI_U32 FocusRectAnchor; /* The anchor position of focus item */
    HI_U32 IsFocusAtBg; /* The focus is drawn at the background */
    HI_U32 ImgDecIndex; /* The index of decode picture */
    HI_U32 IsDynamicDecImg; /* Is Dynamic decode image */
    HIGV_SCROLLGRID_DIRECTION_E Direction; /* Scroll direction */
    HI_RESID FocusRectSkin;
    HI_RESID FocusRectNormSkin;
    HI_RESID replaceImage;
} HIGV_SCROLLGRID_STYLE_S;

/*
* brief Initiatlize the scrollgird.
* attention : Set the head of scrollgrid and row num.
*
* param[in] scrollGridPara    Scrollgrid handle.
* param[in] style   Scrollgrid handle.
* param[in] colAttr Scrollgrid handle.
* param[in] cellColNum Scrollgrid handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_ScrollGrid_Init(HIGV_HANDLE scrollGridPara, const HIGV_SCROLLGRID_STYLE_S *style,
    const HIGV_SCROLLGRID_COLATTR_S *colAttr, HI_U32 cellColNum);

/*
* brief Get the content of scrollgrid.
* param[in] scrollGridPara   Scrollgrid handle.
* param[in] cell   Cell.
* param[in] cellCol The column attribute of cell.
* param[out] value the content of cell.
* param[in] length String length.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST Maybe the handle is invaldi or no data.
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_GetCellValue(HIGV_HANDLE scrollGridPara, HI_U32 cell,
    HI_U32 cellCol, HI_CHAR *value, HI_U32 length);

/*
* brief Set the selected cell of the scrollgrid.
* attention : This api only support the current selected item.
*
* param[in] scrollGrid
* param[in] cell  Cell index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_ScrollGrid_GetSelCell
*/
HI_S32 HI_GV_ScrollGrid_SetSelCell(HIGV_HANDLE scrollGridPara, HI_U32 cell);

/*
* brief Get the selected of cell.
* param[in] scrollGridPara  Scrollgrid handle.
* param[out] cell Cell index.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_ScrollGrid_SetSelCell
*/
HI_S32 HI_GV_ScrollGrid_GetSelCell(HIGV_HANDLE scrollGridPara, HI_U32 *cell);

/*
* brief Get the cell number of scrollgrid.
* param[in]  scrollGridPara  Scrollgrid handle.
* param[out] cellNum Cell number.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_GetCellNum(HIGV_HANDLE scrollGridPara, HI_U32 *cellNum);

/*
* brief Page up or page right.
* param[in]  scrollGridPara  Scrollgrid handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_PageForward(HIGV_HANDLE scrollGridPara);

/*
* brief Page up or page left.
* param[in]  scrollGridPara  Scrollgrid handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_PageBackward(HIGV_HANDLE scrollGridPara);

/*
* brief Back to origin.
* param[in]  scrollGridPara  Scrollgrid handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_MoveToOrigin(HIGV_HANDLE scrollGridPara);

/*
* brief Move to last.
* param[in]  scrollGridPara  Scrollgrid handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_MoveToLast(HIGV_HANDLE scrollGridPara);

/*
* brief Set the attribute of scrollgird.
* param[in]  scrollGridPara  ScrollGrid handle.
* param[in]  horizontalLineHeight  The height of horizontal line.
* param[in]  horizontalLineColor  The color of horizontal line.
* param[in]  verticalLineWidth The width of vertical line.
* param[in]  verticalLineColor  The color of vertical line.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see ::HI_GV_ScrollGrid_GetGridding
*/
HI_S32 HI_GV_ScrollGrid_SetGridding(HIGV_HANDLE scrollGridPara, HI_U32 horizontalLineHeight,
    HI_COLOR horizontalLineColor, HI_U32 verticalLineWidth, HI_COLOR verticalLineColor);

/*
* brief Get the attribute of scrollgird.
* param[in]  scrollGridPara  ScrollGrid handle.ؼ
* param[out]  horizontalLineHeight The height of horizontal line.
* param[out]  horizontalLineColor  The color of horizontal line.
* param[out]  verticalLineWidth  The width of vertical line.
* param[out]  verticalLineColor  The color of vertical line.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see::HI_GV_ScrollGrid_SetGridding
*/
HI_S32 HI_GV_ScrollGrid_GetGridding(HIGV_HANDLE scrollGridPara, HI_U32 *horizontalLineHeight,
    HI_COLOR *horizontalLineColor, HI_U32 *verticalLineWidth, HI_COLOR *verticalLineColor);

/*
* brief Get touch diff.
* param[in]  scrollGridPara  Scrollgrid handle.
* param[out]  diff  diff distance.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_GetTouchDiff(HIGV_HANDLE scrollGridPara, HI_S32 *diff);

/*
* brief Set content location.
* param[in]  scrollGridPara  Scrollgrid handle.
* param[in]  diff  diff distance.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_Adjust(HIGV_HANDLE scrollGridPara, HI_S32 diff);

/*
* brief Set gesture enable.
* param[in]  scrollGridPara  Scrollgrid handle.
* param[in]  enable   Is enable gesture.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_EnableGesture(HIGV_HANDLE scrollGridPara, HI_BOOL enable);

/*
* brief Get gesture enable.
* param[in]  scrollGridPara  Scrollgrid handle.
* param[in]  enable   Is enable gesture.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_IsGestureEnable(HIGV_HANDLE scrollGridPara, HI_BOOL *enable);

/*
* brief Register scrollgrid widget.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_ScrollGrid_RegisterWidget(HI_VOID);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_LIST_H */
#endif
