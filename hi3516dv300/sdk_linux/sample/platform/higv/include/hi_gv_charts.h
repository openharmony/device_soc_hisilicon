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

#ifndef HI_GV_CHARTS_H
#define HI_GV_CHARTS_H
#include "hi_gv_conf.h"
#include "hi_gv.h"

#ifdef HIGV_USE_WIDGET_CHARTS
#include "hi_gv_resm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SUB_BAR_CNT  28
#define MAX_LINE_CNT 28
#define MAX_BAR_CNT 28
#define MAX_LINE_LABEL_TEXT 128

/* Chart type [CN]:ͼؼ */
typedef enum {
    HIGV_CHARTS_TYPE_INVAILD = 0,
    HIGV_CHARTS_TYPE_LINE = 1,   /* line chart.[CN]:ͼ */
    HIGV_CHARTS_TYPE_BAR = 2,   /* histogram. [CN]:״ͼ */
    HIGV_CHARTS_TYPE_CLUSTER = 3,  /* histogram(cluster). [CN]:״Ŀͼ */
    HIGV_CHARTS_TYPE_BUFFER
} HigvChartsTypes;

/* Charts control public property structure[CN]:ChartsؼԽṹ */
typedef struct {
    HI_U32 chartType;  // Control type, [CN]:ͼ
    HI_U32 barWidth;  // Histogram width in single histogram mode.[CN]:״ͼģʽµ״ͼ
    HI_U32 subBarWidth;  // Sub-item width in item cluster mode.[CN]:Ŀģʽ£ĿĿ
    HI_U32 subBarDist;  // Spacing of sub-items in item cluster mode.[CN]:Ŀ£Ŀľ
    HI_U32 subBarCnt;  //  Number of sub-items in item cluster mode.[CN]:ͼ
    HI_U32 lineCnt;  // Number of polylines.[CN]:߸
    HI_U32 lineWidth;  // Polyline width, [CN]߿
    HI_COLOR borderColor;  // Border color.[CN]:߿ɫ
    HI_U32 borderWidth;  // Border width.[CN]:߿
    HIGV_HANDLE titleFont;  // Title font.[CN]:
    HI_COLOR titleColor;  // Title color,[CN]:ɫ
    HIGV_HANDLE legendFont;  // Legend font,[CN]:ͼ
    HI_COLOR legendColor;  // Legend color, [CN]:ͼɫ
    HI_BOOL drawBorder;  // Whether to draw a border.[CN]Ƿ񻭱߿
    HI_CHAR  titleContent[MAX_LINE_LABEL_TEXT];  // title content.[CN]:
} HIGV_CHARTS_COMM_S;

/* Charts control axis property structure.[CN]:ChartsؼԽṹ */
typedef struct {
    HI_U32 axisLeftSpace;  // Left margin of the axis and border, [CN]:߿ľ
    HI_U32 axisRightSpace;  // Right margin of the axis and border,[CN]:߿ұ߾
    HI_U32 axisTopSpace;  // Top margin of the axis and border, [CN]:߿Ķ߾
    HI_U32 axisBottonSpace;  // Bottom margin of the axis and border. [CN]:߿ĵױ߾
    HI_U32 axisWidth;  // Axis width: [CN]:
    HI_COLOR axisColor;  // Axis color: [CN]:ɫ
    HI_U32 axisArrowLength;  // Axis arrow length, [CN]: ͷ
    HIGV_HANDLE axisLabelFont;  // Axis font labels, [CN]:ǩ
    HI_COLOR axisLabelColor;  // Axis font labels color, [CN]:ǩɫ
    HI_U32 axisXMinValue;  // X coordinate axis minimum, [CN]: X Сֵ
    HI_U32 axisXMaxValue;  // X coordinate axis maximum, [CN]: X ֵ
    HI_U32 axisYMinValue;  // Y coordinate axis minimum, [CN]: Y Сֵ
    HI_U32 axisYMaxValue;  // Y coordinate axis maximum, [CN]: Y ֵ
    HI_U32 axisXTickNum;  // X-axis axis division factor [CN]: X ȷϵ
    HI_U32 axisYTickNum;  // Y-axis axis division factor [CN]: Y ȷϵ
    HI_U32 axisTickLength;  // Axis scale height [CN]:̶ȸ߶
    HI_CHAR axisXLabelContent[MAX_LINE_LABEL_TEXT];  // X axis label content, [CN]:Xǩ
    HI_CHAR axisYLabelContent[MAX_LINE_LABEL_TEXT];  // Y axis label content, [CN]:Y ǩ
} HIGV_CHARTS_AXIS_S;

/* Charts control standard line attribute structure.[CN]:Chartsؼ׼Խṹ */
typedef struct {
    HI_U32 stLineMinValue;  // Standard line minimum, [CN]:׼Сֵ
    HI_COLOR stLineMinColor;  // Standard line minimum color, [CN]:С ׼ɫ
    HI_U32 stLineMaxValue;  // Standard line maximum, [CN]:׼ֵ
    HI_COLOR stLineMaxColor;  // Standard line maximum color, [CN]:׼ɫ
} HIGV_CHARTS_STLINE_S;

/* Chart Widget's structure, [CN]:Chartؼṹ   */
typedef struct {
    HIGV_CHARTS_COMM_S comAttr;  // Charts control public property structure[CN]:ChartsؼԽṹ
    HIGV_CHARTS_AXIS_S axisAttr;  // Charts control axis property structure.[CN]:ChartsؼԽṹ
    HIGV_CHARTS_STLINE_S stLineAttr;  // Charts control standard line attribute structure.[CN]:Chartsؼ׼Խṹ
    HI_BOOL isDrawStLine;  // Whether to draw standard lines, [CN]:ǷƱ׼
    HI_BOOL gridLine;  // Whether to draw grid lines, [CN]:Ƿ
    HI_COLOR gridColor;  //  draw grid lines color, [CN]:ɫ
    HI_BOOL textOnBar;  // Whether to draw the corresponding text on the bar chart, [CN]:ǷڸͼϷĻ
    HIGV_HANDLE textOnBarFont;  // Font for drawing corresponding text on bar chart, [CN]:ͼ
    HI_COLOR textOnBarColor;  // Font color of corresponding text drawn on bar chart, [CN]:ͼʾɫ
    HI_U32 axisXIndentation;  // Indentation of scale X axis, [CN]:X ᷽Сʾ
    HI_U32 axisYIndentation;  // Indentation of scale Y axis,[CN]:Y ᷽Сʾ
    HI_BOOL isReplaceXLabel;  // Used to replace the scale value on the X axis, such as the date display;
                                                // [CN] :ڴXϿ̶ֵʾ
} HIGV_CHARTS_STYLE;

/* Child item cluster structure in Charts control.[CN]: ChartsؼĿؽṹ */
typedef struct  {
    HI_COLOR barColor; /*  Sub-histogram color, [CN]:Ŀɫ */
                                            /* [CN]:ͼĿϷʾɫ */
    HI_U32 subBarTitle;  /*  Sub-histogram title for display at legend */
                                     /*  [CN]:״ͼ⣬ͼʾ  */
    HI_U32 indexDB;        /* The number of the column in the data burst, [CN]: DBе */
} HIGV_CHARTS_BAR_S;

/* Structures at points on polylines, [CN]:  ϵĽṹ */
typedef struct {
    HI_U32 x;  /* X coordinate point, [CN]:X   */
    HI_U32 y;  /* Y coordinate point,[CN]: Y  */
} HIGV_LINECHARTS_POINT_S;

/* Histogram private data structure,  ״ͼ˽ݽṹ */
typedef struct {
    HI_U32 value;  /* Histogram data value corresponding to ordinate, [CN]:Ӧ */
    HI_COLOR color;  /* Color value corresponding to the above data, [CN]:Ӧɫֵ */
} HIGV_BARCHARTS_VALUE_S;

HI_S32 HI_GV_CHARTS_Init(HIGV_HANDLE handle, const HIGV_CHARTS_STYLE *style, const HIGV_CHARTS_BAR_S *barAttr,
    HI_U32 barCnt);

HI_S32 HI_GV_CHARTS_BindLineData(HIGV_HANDLE handle, HI_U32 pos, HI_COLOR color,
    const HIGV_LINECHARTS_POINT_S *lineChartPoint, HI_U32 length);

HI_S32 HI_GV_CHARTS_SetLineTitle(HIGV_HANDLE handle, const HI_CHAR *name, HI_U32 length, HI_U32 pos);

HI_S32 HI_GV_CHARTS_BindBarData(HIGV_HANDLE handle,
    const HIGV_BARCHARTS_VALUE_S *barChartValue, HI_U32 length);

HI_S32 HI_GV_CHARTS_SetAxisXBarTitle(HIGV_HANDLE handle, const HI_CHAR *name, HI_U32 length, HI_U32 pos);

HI_S32 HI_GV_CHARTS_SetAxisXReplace(HIGV_HANDLE handle, HI_BOOL replace);

HI_S32 HI_GV_CHARTS_SetAxisXReplaceTitle(HIGV_HANDLE handle, const HI_CHAR *name, HI_U32 length, HI_U32 pos);

#ifdef __cplusplus
}

#endif
#endif

#endif /* HI_GV_HIGVCHARTS_H */
