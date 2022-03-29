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

#ifndef HI_GV_WIDGET_H
#define HI_GV_WIDGET_H

#include "hi_gv_conf.h"
#include "hi_go.h"
#include "hi_gv_errno.h"
#include "hi_gv_resm.h"

#ifdef HIGV_USE_WIDGET_BUTTON
#include "hi_gv_button.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Widget type */

typedef enum HI_WIDGET {
    /*  window */
    HIGV_WIDGET_WINDOW = 0,
    /*  containder  */
    HIGV_WIDGET_CONTAINER = 1,
    /*  button */
    HIGV_WIDGET_BUTTON = 2,
    /*  label */
    HIGV_WIDGET_LABEL = 3,
    /*  image */
    HIGV_WIDGET_IMAGE = 4,
    /*  extend image */
    HIGV_WIDGET_IMAGEEX = 5,
    /*  grid */
    HIGV_WIDGET_GRID = 6,
    /*  groupbox */
    HIGV_WIDGET_GROUPBOX = 7,
    /*  scrollbar */
    HIGV_WIDGET_SCROLLBAR = 8,
    /*  edit */
    HIGV_WIDGET_EDIT = 9,
    /*  spin box **/
    HIGV_WIDGET_SPINBOX = 10,
    /*  progressbar */
    HIGV_WIDGET_PROGRESS = 11,
    /*  scrollbox */
    HIGV_WIDGET_SCROLLBOX = 12,
    /*  scrolltext */
    HIGV_WIDGET_SCROLLTEXT = 13,
    /*  image cipher */
    HIGV_WIDGET_IMAGECIPHER = 14,
    /*  clock */
    HIGV_WIDGET_CLOCK = 15,
    /*  listbox */
    HIGV_WIDGET_LISTBOX = 16,
    /*  timetrack */
    HIGV_WIDGET_TIMETRACK = 17,
    /*  scrollgrid */
    HIGV_WIDGET_SCROLLGRID = 18,
    /*  trackbar */
    HIGV_WIDGET_TRACKBAR = 19,
    /*  ipedit */
    HIGV_WIDGET_IPEDIT = 20,
    /*  calendar */
    HIGV_WIDGET_CALENDAR = 21,
    /*  videoshelter */
    HIGV_WIDGET_VIDEOSHELTER = 22,
    /*  movemonito */
    HIGV_WIDGET_MOVEMONITOR = 23,
    /*  combobox */
    HIGV_WIDGET_COMBOBOX = 24,
    /*  pop window of combobox */
    HIGV_WIDGET_CPOPBOX = 25,
    /*  input method window  */
    HIGV_WIDGET_IMEWINDOW = 26,
    /*  timebar  */
    HIGV_WIDGET_TIMEBAR = 27,
    /*  dialog box  */
    HIGV_WIDGET_MSGBOX = 28,
    /*  multi edit */
    HIGV_WIDGET_MULTIEDIT = 29,
    /*  scroll view  */
    HIGV_WIDGET_SCROLLVIEW = 30,
    /*  content box  */
    HIGV_WIDGET_CONTENTBOX = 31,
    /*  slide unlock  */
    HIGV_WIDGET_SLIDEUNLOCK = 32,
    /*  wheel view */
    HIGV_WIDGET_WHEELVIEW = 33,
    /*  image view */
    HIGV_WIDGET_IMAGEVIEW = 34,
    /*  image view */
    HIGV_WIDGET_SCALEVIEW = 35,
   /*  charts view */
    HIGV_WIDGET_CHARTS  = 36,
    HIGV_WIDGET_BUTT
} HIGV_WIDGET_E;

typedef struct {
    HI_U32 style;
    const HI_RECT *rect;
    HIGV_HANDLE parentHandle;
    HI_U32 level;
} WidgetBasicParam;

/* The sequence of user defined widget */
#define HIGV_EXTWIDGET_START HIGV_WIDGET_BUTT
/* The callback of user defined widget */
typedef HI_VOID *(*HIGV_WIDGET_INIT_FUNC)(const WidgetBasicParam *widgetBasicParam);

typedef HI_S32 (*HIGV_WIDGET_HDL2ADDR_PFN)(HI_U32 u32Idx, HI_U8 u8MemType, HI_PARAM *pAddr);

typedef struct hiHIGV_WIDGET_TYPEINFO_S {
    /* Widget name */
    HI_CHAR Name[33];  // 33 represents this array can save 33 HI_CHAR type data
    /* The sequence of user defined widget */
    HI_U32 TypeID;
    HIGV_WIDGET_INIT_FUNC WidgetInitFunc;
    /* The callback of user defined widget */
    /* The size of widget that pasered by parser mode, it is should seted
    if the widget is defined in xml, otherwise it can be set 0 */

    HI_U32 WidgetPaserSize;
} HIGV_WIDGET_TYPEINFO_S;

/* coordinate type  */
typedef HI_S32 HIGV_CORD;

/* The button of mouse which pressed */
typedef enum {
    HIGV_MOUSEBUTTON_NONE = 0x00,
    HIGV_MOUSEBUTTON_L = 0x01,
    HIGV_MOUSEBUTTON_M = 0x02,
    HIGV_MOUSEBUTTON_R = 0x04,
    HIGV_MOUSEBUTTON_BUTT
} HIGV_MOUSEBUTTON_E;

/* The status of timer running  */
typedef enum {
    HIGV_TIMERMSG_STATUS_UNEXIST = 0,
    /* unexit status */
    HIGV_TIMERMSG_STATUS_IDLE,
    /* idle status */
    HIGV_TIMERMSG_STATUS_FRYING,
    /* frying status */
    HIGV_TIMERMSG_STATUS_RESET,
    /* reset status */
    HIGV_TIMERMSG_STATUS_BUTT
} HIGV_TIMERMSG_STATUS_E;

/* The language direction of widget  */
typedef enum {
    HIGV_TEXTDIR_NEUTRAL = 0,
    HIGV_TEXTDIR_LTR,
    HIGV_TEXTDIR_RTL,
    HIGV_TEXTDIR_BUTT
} HIGV_TEXTDIR_E;

/* Set widget focus switch mode  */
typedef enum {
    HIGV_FOCUS_SWITCH_AUTO = 0,
    /* auto */
    HIGV_FOCUS_SWITCH_MANUAL,
    /* manual */
    HIGV_FOCUS_SWITCH_BUTT
} HIGV_FOCUS_SWITCH_E;

/* Focus switch state at manual focus mode  */
typedef enum {
    HIGV_FOCUS_STATE_SWITCH = 0,
    /* Focus switch state */
    HIGV_FOCUS_STATE_EDIT,
    /* Widget can operate */
    HIGV_FOCUS_STATE_BUTT
} HIGV_FOCUS_STATE_E;

/* The type of widget paint  */
typedef enum {
    HIGV_PAINT_TYPE_HIDE = 0,
    /* hide widget  */
    HIGV_PAINT_TYPE_SHOW,
    /* show widget  */
    HIGV_PAINT_TYPE_BUTT
} HIGV_PAINT_TYPE_E;

/* Window style   */
/* Default style, it is used 2bytes  */
#define HIGV_STYLE_DEFAULT 0x00
/* Don't draw the background  */
#define HIGV_STYLE_TRANSPARENT 0x02
/* Couldn't get foucssuch Label  */
#define HIGV_STYLE_NOFOCUS 0x04
/* Release the resource when hidden  */
#define HIGV_STYLE_HIDE_UNLODRES 0x10
/* It is indicated that the widget can change language, it can trigger the language msg to user */

#define HIGV_STYLE_LANCHNG 0x20
/* Modal window */

#define HIGV_STYLE_MODAL 0x40
/* Foce draw background of parent */

#define HIGV_STYLE_FORCE_PAINT_PARENT 0x80
/* if container is seted this flag, it's child also highlighted,
and lost the focus, the child will not be highlight */
/*
 * ʧȥʱʧȥ״̬ */
#define HIGV_STYLE_HILIGHT_CHILD 0x100

/* The extend style of widget which only support 8 */

#define HIGV_STYLE_DEFINE(WidgetType, style) ((((HI_U32)(WidgetType)) << (24)) | ((1) << ((16) +   \
    (HI_U32)((style) & (0x7)))))

/* Common style mask of widget */

#define HIGV_STYLE_MASK 0xffffff

/* Skin index, it is only used to skin */

/* No focus */
#define HIGV_SKIN_NORMAL 0x01
/* Disable state */

#define HIGV_SKIN_DISABLE 0x02
/* Highlight state */

#define HIGV_SKIN_HIGHLIGHT 0x03
/* Active state */

#define HIGV_SKIN_ACITVE 0x04
/* Mouse down state */

#define HIGV_SKIN_MOUSEDOWN 0x05
/* Checked state */

#define HIGV_SKIN_BUTTON_CHECKED_NORMAL 0x06
/* Disable at check state */

#define HIGV_SKIN_BUTTON_CHECKED_DISABLE 0x07
/* Highlight at check state */

#define HIGV_SKIN_BUTTON_CHECKED_HIGHLIGHT 0x08
/* Focus at check state */

#define HIGV_SKIN_BUTTON_CHECKED_ACITVE 0x09
/* Pressed at check state */

#define HIGV_SKIN_BUTTON_CHECKED_MOUSEDOWN 0xa

/* Window manager handle */

#define HIGV_WND_MANAGER_HANDLE ((HI_U32)(-1))

/* It is used to HIGV_MSG_STATE_CHANGE message */

typedef enum {
    /* enable */
    HIGV_STATENAME_ENABLE,
    /* highlight */
    HIGV_STATENAME_HIGHLIGHT,
    HIGV_STATENAME_BUTT
} HIGV_STATENAME_E;

/* The order of the message which user register to higv */
typedef enum {
    /* It is first order to call user function */

    HIGV_PROCORDER_BEFORE,
    /* It is after order to call user function */

    HIGV_PROCORDER_AFTER,
    HIGV_PROCORDER_BUTT
} HIGV_PROCORDER_E;

#define HIGV_RECT_TOU32PARAM(rc, param1, param2)                                  \
    do {                                                                          \
        param1 = ((((HI_U32)(rc).x & 0xFFFF) << 16) | ((HI_U32)(rc).y & 0xFFFF)); \
        param2 = ((((HI_U32)(rc).w & 0xFFFF) << 16) | ((HI_U32)(rc).h & 0xFFFF)); \
    } while (0)

#define HIGV_U32PARAM_TORECT(param1, param2, rc)      \
    do {                                              \
        (rc).x = (HI_S32)(((param1) >> 16) & 0xFFFF); \
        (rc).y = (HI_S32)((param1)&0xFFFF);           \
        (rc).w = (HI_S32)(((param2) >> 16) & 0xFFFF); \
        (rc).h = (HI_S32)((param2)&0xFFFF);           \
    } while (0)

/* Message of window */
/* Paint message [wparam, lparam] = [PaintRect<x,y>, PaintRect<w,h>] =
[((((rc).x & 0xFFFF)<<16) | ((rc).y & 0xFFFF)),
 *  ((((rc).w & 0xFFFF)<<16) | ((rc).h & 0xFFFF))] */
/*
[((((rc).x & 0xFFFF)<<16) | ((rc).y & 0xFFFF)), ((((rc).w & 0xFFFF)<<16) | ((rc).h & 0xFFFF))] */
#define HIGV_MSG_PAINT 0x03
/* Key down message [wparam, lparam]= [HIGV_KEYCODE_E, nil] */

#define HIGV_MSG_KEYDOWN 0x04
/* Key up message [wparam, lparam]  = [HIGV_KEYCODE_E, nil]* */

#define HIGV_MSG_KEYUP 0x05
/* Get focus message [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_GET_FOCUS 0x06
/* Lost focus message [wparam, lparam]  = [nil, nil] */
/* **/
#define HIGV_MSG_LOST_FOCUS 0x07
/* Show message [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_SHOW 0x08
/* Hide message [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_HIDE 0x09
/* Data change message [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_DATA_CHANGE 0x10
/* Timer message [wparam, lparam]  = [TimerID, nil]  */

#define HIGV_MSG_TIMER 0x11
/* button down message  [wparam, lparam]  = [nil, nil] */
/*
                                                           */
#define HIGV_MSG_CLICK 0x12
/* State change message */

#define HIGV_MSG_STATE_CHANGE 0x13
/* Item selected message */

#define HIGV_MSG_ITEM_SELECT 0x14
/* Scroll text message */
#define HIGV_MSG_ST_UPDATE 0x15
/* Update time message [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_CLOCK_UPDATE 0x16
/* Input over for imagecipher [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_IC_INPUTEND 0x17
/* Input over for edit [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_EDIT_INPUTEND 0x18
/* Scroll over in one cycle for scrolltext  [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_ST_SCROLLONETIME 0x19
/* The value changed when opreate */
/*
    [wparam,  lparam]  = [(SlidePos << 16) | ContentLength, ((touchMove << 16)| handle)] */
#define HIGV_MSG_SCROLLBAR_CHANGE 0x1a
/* State change of checkbox or radio button [wparam, lparam]  = [HIGV_BUTTON_STATUS_E, nil] */
/*
 * [wparam, lparam]  =[HIGV_BUTTON_STATUS_E,nil] */
#define HIGV_MSG_BUTTON_STATUSCHANGE 0x1b
/* Cell is selected [m_SelCell.Item,m_SelCell.Col] */

#define HIGV_MSG_LISTBOX_CELLSEL 0x1c
/* Item is selected [wparam, lparam]  =  [Item, nil] */

#define HIGV_MSG_COMBOBOX_ITEMSEL 0x1d
/* Container widget change active child [wparam, lparam] = [Last active child handle, new active child handle] */

#define HIGV_MSG_CONTAINER_CHANGE_FOCUSE 0x1e

/* The event of scrollgrid */
/* The cell selected event  [wparam, lparam]  = [SelCell.Item, SelCell.Col] */

#define HIGV_MSG_SCROLLGRID_CELL_COLSEL 0x23
/* The focus move the top margin will genterate the event [wparam, lparam]  = [SelCell, nil] */

#define HIGV_MSG_SCROLLGRID_REACH_TOP 0x24
/* The focus move the bottom margin will genterate the event  [wparam, lparam]  = [SelCell, nil] */

#define HIGV_MSG_SCROLLGRID_REACH_BTM 0x25
/* The focus move the left margin will genterate the event  [wparam, lparam]  = [SelCell, nil]  */

#define HIGV_MSG_SCROLLGRID_REACH_LEFT 0x26
/* The focus move the right margin will genterate the event  [wparam, lparam]  = [SelCell, nil] */

#define HIGV_MSG_SCROLLGRID_REACH_RIGHT 0x27
/* The update event of item for scrollgrid [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_SCROLLGRID_ITEM_UPDATE 0x28
/* The uddate event of scrollgrid [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_SCROLLGRID_FOCUS_UPDATE 0x29
/* The begin event of decode image for scrollgrid [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_SCROLLGRID_DECIMG_BEGIN 0x2a
/* The finish event of decode image for scrollgrid [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_SCROLLGRID_DECIMG_FINISH 0x2b
/* The finish event of fling for scrollgrid [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_SCROLLGRID_FLING_FINISH 0x2c

/* The event of timetrack */
/* The drag event of mouse for timetrack [wparam, lparam]  = [m_CurGraduation, nil]  */

#define HIGV_MSG_TIMETRACK_DRAGBEGIN 0x30
/* The position change of timetrack  [wparam, lparam]  = [m_CurGraduation, nil]   */

#define HIGV_MSG_TIMETRACK_CHANGE 0x31
/* The end event when mouse move through timetrack [wparam, lparam]  = [m_CurGraduation, nil] */

#define HIGV_MSG_TIMETRACK_DRAGEND 0x32
/* The graduation value change event when drag trackbar [wparam, lparam]  = [m_CurValue, nil]   */

#define HIGV_MSG_VALUEONCHANGE 0x33

/* The update event of scrollbox  [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_SCROLLBOX_UPDATE 0x34

/* The switch event of input method[wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_IME_SWITCH 0x35
/* The show event of input method [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_IME_SHOW 0x36
/* *The hide event of input method [wparam, lparam]  = [nil, nil] */
/* **/
#define HIGV_MSG_IME_HIDE 0x37

/* The event of timebar */

/* The drag start event of mouse for timebar  [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_TIMEBAR_DRAGBEGIN 0x40
/* The change event of timebar  [wparam, lparam]  = [GraduationIndex, nil] */

#define HIGV_MSG_TIMEBAR_CHANGE 0x41
/* The drag end event of mouse for timebar  [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_TIMEBAR_DRAGEND 0x42

/* Change the focus of clock [wparam, lparam]  =[m_FocusSeg, 0] */

#define HIGV_MSG_CLOCK_FOCUS_MOVE 0x50
/* Set clock. [wparam, lparam]  =  [nil, nil] */

#define HIGV_MSG_CLOCK_TIME_ADJUST 0x51
/* Set widget state. [wparam, lparam]  =  [nil, nil] */

#define HIGV_MSG_MANUAL_FOCUS_STATE_CHANGE 0x52

/* The event of message box */
/* The timeout event of MessageBox show  [wparam, lparam]  = [ButtonID, show times] */

#define HIGV_MSG_MSGBOX_TIMEOUT 0x60
/* The message box click button message [wparam, lparam]  = [ButtonID, nil] */

#define HIGV_MSG_MSGBOX_SELECT 0x61
/* The message box hide message. attention:Don't use it */

#define HIGV_MSG_MSGBOX_HIDE 0x62

/* The event of multi edit */

/* Input over for multi edit [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_MULTIEDIT_INPUT 0x70
/* Begin select string from text [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_MULTIEDIT_SELECT_OUT 0x71
/* End select string from text [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_MULTIEDIT_SELECT_END 0x72
/* Move reach side [wparam, lparam]  = [HigvMultiEditSlide, nil] */

#define HIGV_MSG_MULTIEDIT_REACH_SIDE 0x73
/* Draw cursor [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_MULTIEDIT_DRAW_CURSOR 0x74

/* The event of scroll view */
/* Scroll view scroll event [wparam, lparam] =
[(((content rect).x & 0xFFFF)<<16) | (y & 0xFFFF), ((w & 0xFFFF)<<16) |
 *  (h & 0xFFFF)] */
/*
[(((content rect).x & 0xFFFF)<<16) | (y & 0xFFFF), ((w & 0xFFFF)<<16) | (h & 0xFFFF)] */
#define HIGV_MSG_SCROLLVIEW_SCROLL 0x80
/* The event of finish fling event */
/* finish fling event [wparam, lparam] = [(((content rect).x & 0xFFFF)<<16) | (y & 0xFFFF), ((w & 0xFFFF)<<16) | (h &
 * 0xFFFF)] */
/*
[(((content rect).x & 0xFFFF)<<16) | (y & 0xFFFF), ((w & 0xFFFF)<<16) | (h & 0xFFFF)] */
#define HIGV_MSG_SCROLLVIEW_FINISHFLING 0x81

/* Language change event */

#define HIGV_MSG_LAN_CHANGE 0x100
/* Exit edit status at auto manual focus mode [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_FOCUS_EDIT_EXIT 0x101
/* Mouse in event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_MOUSEIN 0x200
/* The first event of mouse */
#define HIGV_MSG_FIRSTMOUSEMSG HIGV_MSG_MOUSEIN
/* Mouse down event [wparam, lparam]  = [HIGV_MOUSEBUTTON_L, (ScreenX << 16)|(ScreenY & 0xffff)] */

#define HIGV_MSG_MOUSEDOWN 0x201
/* Mouse out event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_MOUSEOUT 0x202
/* Mouse up event [wparam, lparam]  = [HigvMouseState, (ScreenX << 16)|(ScreenY & 0xffff)] */

#define HIGV_MSG_MOUSEUP 0x203
/* Mouse move event [wparam, lparam]  = [nil, (ScreenX << 16)|(ScreenY & 0xffff)] */

#define HIGV_MSG_MOUSEMOVE 0x204
/* Mouse wheel event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_MOUSEWHEEL 0x205
/* The press down of left mouse [wparam, lparam]  = [nil, nil] */
/* ¼ [wparam, lparam]  = [nil, nil] */
#define HIGV_MSG_MOUSELONGDOWN 0x206
/* The double click of  mouse [wparam, lparam]  = [nil, nil] */
/* ˫¼[wparam, lparam]  = [nil, nil] */
#define HIGV_MSG_MOUSEDBCLICK 0x207
/* The last message of  mouse */

#define HIGV_MSG_LASTMOUSEMSG HIGV_MSG_MOUSEDBCLICK
/* The virtual mouse up event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_VIRTUAL_MOUSEUP 0x208
/* The virtual mouse down event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_VIRTUAL_MOUSEDOWN 0x209

/* Refresh the window to screen [wparam, lparam]  =
[(((rc).x & 0xFFFF)<<16) | ((rc).y & 0xFFFF), (((rc).w & 0xFFFF)<<16)
 * | ((rc).h & 0xFFFF)] */
/*
[(((rc).x & 0xFFFF)<<16) | ((rc).y & 0xFFFF), (((rc).w & 0xFFFF)<<16) | ((rc).h & 0xFFFF)] */
#define HIGV_MSG_REFRESH_WINDOW 0x500
/* After refresh the window to screen [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_AFTERREFRESH 0x501
/* The data change message for adm */

#define HIGV_MSG_ADM_DATACHANGE 0x502
/* Force to update the window to the screen */
/*
*  0xFFFF)<<16) | ((rc).y & 0xFFFF), (((rc).w & 0xFFFF)<<16) | ((rc).h &
* 0xFFFF)] */
#define HIGV_MSG_FORCE_REFRESH_WINDOW 0x503

/* Refresh message which is used to avoid loss combined message [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_FAKE_REFRESH_WINDOW 0x504

/* Force to update the window state */

#define HIGV_MSG_WINDOW_CHANGE_STATE 0x505

/* Business event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_EVENT 0x900
/* The base value of user defined [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_USER 0x1000
/* touch screen event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_TOUCH 0x2001
/* tap gesture event[wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_GESTURE_TAP 0x2002
/* long press gesture event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_GESTURE_LONGTAP 0x2003
/* fling gesture event [wparam, lparam]  = [nil, nil] */
/*
 */
#define HIGV_MSG_GESTURE_FLING 0x2004
/* scroll gesture event [wparam, lparam]  = [nil, nil] */
/* **/
#define HIGV_MSG_GESTURE_SCROLL 0x2005
/* pinch gesture event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_GESTURE_PINCH 0x2006
/* unlock done event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_UNLOCK 0x2007
/* finish seek trackbar [wparam, lparam]  = [m_CurValue, nil]   */

#define HIGV_MSG_FINISHSEEK 0x2008
/* move event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_MOVE 0x2009
/* kick back event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_KICKBACK 0x2010
/* clock position event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_POSITION 0x2011
/* Touch in event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_TOUCHIN 0x2012
/* Touch out event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_TOUCHOUT 0x2013
/* wheelview position event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_WHEELVIEW_ITEM 0x3001
/* imageview git finish repeat event [wparam, lparam]  = [nil, nil] */

#define HIGV_MSG_IMAGEEX_ENDOF_REPEAT 0x3101

/* animation start event [wparam, lparam]   = [nil, nil] */

#define HIGV_MSG_ANIM_START 0x5001
/* animation running event [wparam, lparam] = [nil, nil] */

#define HIGV_MSG_ANIM_UPDATE 0x5002
/* animation stop event [wparam, lparam]    = [nil, nil] */

#define HIGV_MSG_ANIM_STOP 0x5003

/* animation start event [wparam, lparam]   = [nil, nil] */

#define HIGV_MSG_REBOUND_START 0x5101
/* animation running event [wparam, lparam] = [nil, nil] */

#define HIGV_MSG_REBOUND_UPDATE 0x5102
/* animation stop event [wparam, lparam]    = [nil, nil] */

#define HIGV_MSG_REBOUND_STOP 0x5103
/* item scale event [wparam, lparam] = [nil, nil] */

#define HIGV_MSG_SCALE 0x5104
/* selector focus position report event [wparam, lparam] = [nil, nil] */

#define HIGV_MSG_SCALEVIEW_ITEM 0x3002
/* TAP reported event [wparam, lparam] = [nil, nil] */

#define HIGV_MSG_SCALEVIEW_TAPLISTENER 0x3003

/* it is defined the message whether process continue */

/* The message will go on */
#define HIGV_PROC_GOON 100
/* Stop process message, it is not continue */

#define HIGV_PROC_STOP 200

/* Get the x coordinate of mouse which is relative the x coordinate of widget */

#define HIGV_GET_MOUSE_X(lParam) ((HIGV_CORD)((HI_S16)(((HI_U32)(lParam)) >> (16))))
/* Get the y coordinate of mouse which is relative the y coordinate of widget */

#define HIGV_GET_MOUSE_Y(lParam) ((HIGV_CORD)((HI_S16)(((HI_U32)(lParam)) & (0xFFFF))))
/* Get step of mouse moving */
#define HIGV_GET_MOUSE_STEP(wParam) ((HI_S32)((HI_S16)(((HI_U32)(wParam)) >> (16))))
/* Get state of mouse button */
#define HIGV_GET_MOUSE_BUTTON(wParam) ((HI_U32)((HI_S16)(((HI_U32)(wParam)) & (0xFFFF))))

/* Mouse wheel direction */
typedef enum hiHIGV_WHEEL_DIRECTION {
    HIGV_WHEEL_DIRECTION_DOWN = 0,
    HIGV_WHEEL_DIRECTION_UP = 1,
    HIGV_WHEEL_DIRECTION_BUTT
} HIGV_WHEEL_DIRECTION;

/* Get step and diection of mouse wheel */

#define HIGV_GET_MOUSE_WHEEL(wParam, Step, Direction) \
    do {                                              \
        Step = HIGV_GET_MOUSE_STEP(wParam);           \
        if ((Step) >> 7) {                              \
            Step = 0xFF - (Step) + 1;                   \
            Direction = HIGV_WHEEL_DIRECTION_UP;      \
        } else {                                      \
            Direction = HIGV_WHEEL_DIRECTION_DOWN;    \
        }                                             \
    } while (0)

/* Message callback */
typedef HI_S32 (*HIGV_MSG_PROC)(HIGV_HANDLE widgetHandle, HI_PARAM wParam, HI_PARAM lParam);

/* The attribute of widget to use create  */
typedef struct hiHIGV_WCREATE_S {
    HI_U32 type; /* Control class  */
    HI_RECT rect;                    /* The position and size of widget */
    HIGV_HANDLE hParent;             /* Parent widget handle */
    HI_U32 style;                    /* Widget style */
    HI_U32 level;
    /* Window level, it is only effect to window, the range is [0,15]
    */
    HI_VOID *pPrivate; /*  The private data of widget */
} HIGV_WCREATE_S;

/* The private data of widnow  */
typedef struct hiHIGV_WINCREATE_S {
    HIGV_HANDLE hLayer;    /* Layer of window */
    HIGO_PF_E PixelFormat; /* Pixel format of window */
    HI_BOOL bVideoHole;    /* For video hole */
} HIGV_WINCREATE_S;

/* The build tree of widget */
typedef struct hiHIGV_WIDGETNODE_S {
    struct hiHIGV_WIDGETNODE_S *pChildHead;  /* first child node */
    struct hiHIGV_WIDGETNODE_S *pNextSibing; /* sibling node */
    HI_VOID *pWidgetInfo;
    /* The build parameter when create the widget
    */
    HI_BOOL bCreateHide;   /* whether hide the widget when create */
    HI_VOID *pPrivateData; /* User private data */
} HIGV_WIDGETNODE_S;

/* The type of text alignment */
/* Horizontal left of text alignment */
#define HIGV_ALIGN_LEFT LAYOUT_LEFT
/* Horizontal right of text alignment */
#define HIGV_ALIGN_RIGHT LAYOUT_RIGHT
/* Horizontal center of text alignment */
#define HIGV_ALIGN_HCENTER LAYOUT_HCENTER
/* Top style of text alignment */
#define HIGV_ALIGN_TOP LAYOUT_TOP
/* Vertical bottom style  of text alignment */
#define HIGV_ALIGN_BOTTOM LAYOUT_BOTTOM
/* Vertical center style  of text alignment */
#define HIGV_ALIGN_VCENTER LAYOUT_VCENTER
/* Wap style  of text alignment */
#define HIGV_ALIGN_WRAP LAYOUT_WRAP

/* Ellipsis style of text alignment  */
#define HIGV_ALIGN_WORDELLIPSIS LAYOUT_WORDELLIPSIS
/* Force to left of Horizontal  */
#define HIGV_ALIGN_FORCE_LEFT LAYOUT_FORCE_LEFT
/* Force to right of Horizontal  */
#define HIGV_ALIGN_FORCE_RIGHT LAYOUT_FORCE_RIGHT
/* Support the type of input method  */
/* Support all input method by default */
#define HIGV_IMETYPE_ALLTYPE 0
/* Lowcase english input method  */
#define HIGV_IMETYPE_ENGLISH (1 << 0)
/* Caps english input method  */
#define HIGV_IMETYPE_CAPENGLISH (1 << 1)
/* Number input method  */
#define HIGV_IMETYPE_NUMBER (1 << 2)
/* Pingyin input method  */
#define HIGV_IMETYPE_PINYIN (1 << 3)
/* Symbol method   */
#define HIGV_IMETYPE_SYMBOL (1 << 4)
/* Standard input method A  */
#define HIGV_IMETYPE_STANDARD_A (1 << 5)
/* Standard input method B */
#define HIGV_IMETYPE_STANDARD_B (1 << 6)

/*
* brief Active widget.
* param[in] widgetHandle Widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_FAILURE
*/
extern HI_S32 HI_GV_Widget_Active(HIGV_HANDLE widgetHandle);

/*
* brief Create widget.
* param[in] creatInfo The information of widget.
* param[out] widgetHandle   Widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
* see :HI_GV_Widget_Destroy
*/
HI_S32 HI_GV_Widget_Create(const HIGV_WCREATE_S *creatInfo, HIGV_HANDLE *widgetHandle);

/*
* brief Destroy widget.
* param[in] widgetHandle Widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NORES
* see:HI_GV_Widget_Create
*/
HI_S32 HI_GV_Widget_Destroy(HIGV_HANDLE widgetHandle);

/*
* brief enable widget, but it is not effect to window.
* param[in] widgetHandle Widget handle.
* param[in] enable  enable flag.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NOOP Unsupported operation, such as the operation is to window.
* ָ֧ôڣԴڽиò \retval ::HI_ERR_COMM_PAERM Not allow, such as,
* it is not allowed when the parent
* window is disabled.
*
*/
HI_S32 HI_GV_Widget_Enable(HIGV_HANDLE widgetHandle, HI_BOOL enable);

/*
* brief Get the active child widget.
* attention :You can get the active window if parentHandle = HIGV_WND_MANAGER_HANDLE.
*
* param[in] parentHandle  Parent widget handle.
* param[out] activeWidgetHandle The pointer of active childe widget.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST Invalid parater, such as the parent
* doesn't has active child or widget is invalid.
* ؼЧ߸ûл \retval ::HI_ERR_COMM_PAERM No support operation, parent window is not a
* container.
* see:HI_GV_Widget_SetActiveWidget
*/
HI_S32 HI_GV_Widget_GetActiveWidget(HIGV_HANDLE parentHandle, HIGV_HANDLE *activeWidgetHandle);

/*
* brief Get font.
* param[in] widgetHandle Widget handle.
* param[out] fontHandle Pointer of font.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_GetFont(HIGV_HANDLE widgetHandle, HIGV_HANDLE *fontHandle);

/*
* brief Get parent widget, if it is window ,it will return NULL.
* param[in] widgetHandle Widget handle.
* param[out] parentHandle Pointer of parent widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_FAILURE Failure, such as, no parent.
*/
HI_S32 HI_GV_Widget_GetParent(HIGV_HANDLE widgetHandle, HIGV_HANDLE *parentHandle);

/*
* brief Get sibings of widget, it is mainly used to change focus.
* param[in] widgetHandle Widget handle.
* param[out] leftSibingHandle  Left sibing widget.
* param[out] rightSibingHandle Right sibing widget.
* param[out] upSibingHandle    Up sibing widget.
* param[out] downSibingHandle  Down sibing widget.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_GetSiblings(HIGV_HANDLE widgetHandle, HIGV_HANDLE *leftSibingHandle,
    HIGV_HANDLE *rightSibingHandle, HIGV_HANDLE *upSibingHandle, HIGV_HANDLE *downSibingHandle);

/*
* brief Hide widget.
* param[in] widgetHandle Widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_Hide(HIGV_HANDLE widgetHandle);

/*
* brief Get the widget active state.
* param[in] Widget handle.
* retval ::HI_TRUE  The widget is active.
* retval ::HI_FALSE The widget is inactive.
*/
HI_BOOL HI_GV_Widget_IsActive(HIGV_HANDLE widgetHandle);

/*
* brief Get the widget enable state.
* param[in] widgetHandle Widget handle.
* retval ::HI_TRUE  enable state.
* retval ::HI_FALSE Disable state.
*/
HI_BOOL HI_GV_Widget_IsEnable(HIGV_HANDLE widgetHandle);

/*
* brief The widget whether show.
* param[in] widgetHandle  Widget handle.
* retval ::HI_TRUE  Show state.
* retval ::HI_FALSE  Hide state.
*/
HI_BOOL HI_GV_Widget_IsShow(HIGV_HANDLE widgetHandle);

/*
* brief The widget whether show.
* param[in] widgetHandle  Widget handle.
* retval ::HI_TRUE  Refreshed state.
* retval ::HI_FALSE  Other state.
*/
HI_BOOL HI_GV_Widget_IsRefresh(HIGV_HANDLE widgetHandle);

/*
* brief Get widget surface.
* param[in] widgetHandle Widget handle.
* param[out] surfaceHandle Surface pointer
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_GetSurface(HIGV_HANDLE widgetHandle, HIGV_HANDLE *surfaceHandle);

/*
* brief Paint the widget, it will emit a asynchronism message.
* param[in] widgetHandle Widget handle.
* param[in] rect The area of update .
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_FAILURE Failure such as the widget is not show.
*/
HI_S32 HI_GV_Widget_Paint(HIGV_HANDLE widgetHandle, const HI_RECT *rect);

/*
* brief Set font.
* param[in] widgetHandle Widget handle.
* param[in] fontHandle The font of widget.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_SetFont(HIGV_HANDLE widgetHandle, HIGV_HANDLE fontHandle);

/*
* brief Register message processor.
* param[in] widgetHandle   Widget handle.
* param[in] msg        Message.
* param[in] customProc Message processor.
* param[in] procOrder  Message prioprity.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_Widget_SetMsgProc(HIGV_HANDLE widgetHandle, HI_U32 msg, HIGV_MSG_PROC customProc,
    HIGV_PROCORDER_E procOrder);

/*
* brief Set skin of widget.
* param[in] widgetHandle   Widget handle.
* param[in] skinIndexHandle Skin index.
* param[in] skinHandle     Skin handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NORES
* see:HI_GV_Widget_SetCharset
*/
HI_S32 HI_GV_Widget_SetSkin(HIGV_HANDLE widgetHandle, HI_U32 skinIndexHandle, HI_RESID skinHandle);

/*
* brief Set siblings of widget.
* param[in] widgetHandle  Widget handle.
* param[in] leftHandle      Left siblings widet.
* param[in] rightHandle    Right siblings widet.
* param[in] upHandle        Up siblings widet.
* param[in] downHandle    Down siblings widet.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_LOST
* see : HI_GV_Widget_GetSiblings
*/
HI_S32 HI_GV_Widget_SetSiblings(HIGV_HANDLE widgetHandle, HIGV_HANDLE leftHandle, HIGV_HANDLE rightHandle,
    HIGV_HANDLE upHandle, HIGV_HANDLE downHandle);

/*
* brief Show widget.
* param[in] widgetHandle   Widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* see: HI_GV_Widget_Hide
*/
HI_S32 HI_GV_Widget_Show(HIGV_HANDLE widgetHandle);

/*
* brief Move widget.
* param[in] widgetHandle Widget handle.
* param[in] x       the x coordinate that moved after.
* param[in] y       the y coordinate  that moved after.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_Move(HIGV_HANDLE widgetHandle, HIGV_CORD x, HIGV_CORD y);

/*
* brief Move window to top.
* param[in] widgetHandle  Widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NOOP Unsupported operation, such as the widget except window not support the api.
* ָ֧òԿؼøýӿ
*/
HI_S32 HI_GV_Widget_MoveToTop(HIGV_HANDLE widgetHandle);

/*
* brief Highlight widget.
* param[in] widgetHandle   Widget handle.
* param[in] highlight
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NOOP Unsupported operation, such as the widget is disabed.
*/
HI_S32 HI_GV_Widget_Highlight(HIGV_HANDLE widgetHandle, HI_BOOL highlight);

/*
* brief Whether highlight the widget.
* param[in] widgetHandle   Widget handle.
* retval ::HI_TRUE Highlight.
* retval ::HI_FALSE it is not highlight.
*/
HI_BOOL HI_GV_Widget_IsHighlight(HIGV_HANDLE widgetHandle);

/*
* brief Set transparent of widget.
* param[in] widgetHandle  Widget handle.
* param[in] isTrans  Transparent flag.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_SetTransparent(HIGV_HANDLE widgetHandle, HI_BOOL isTrans);

/*
* brief Set alignment of text.
* param[in] widgetHandle Widget handle.
* param[in] align  Alignment of text.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_SetAlign(HIGV_HANDLE widgetHandle, HI_U32 align);

/*
* brief Set text.
* param[in] widgetHandle  Widget handle.
* param[in] text   Text of wiget.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES No enough memory.
* see
* ::HI_GV_Widget_GetText
* ::HI_GV_Widget_SetTextByID
*/
HI_S32 HI_GV_Widget_SetText(HIGV_HANDLE widgetHandle, const HI_CHAR *text);

/*
* brief Set text by ID.
* param[in] widgetHandle  Widget handle.
* param[in] strID      String ID.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* see:
* HI_GV_Widget_GetText
* HI_GV_Widget_GetTextID
* HI_GV_Widget_SetText
*/
HI_S32 HI_GV_Widget_SetTextByID(HIGV_HANDLE widgetHandle, const HI_U32 strID);

/*
* brief Get text.
* param[in] widgetHandle Widget handle.
* param[out] buf     Text content.
* param[in] bufLen     Buffer length.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL Null pointer and the buffer length is 0.
* see
* HI_GV_Widget_SetText
*/
HI_S32 HI_GV_Widget_GetText(HIGV_HANDLE widgetHandle, HI_CHAR *buffer, HI_U32 bufferLen);

/*
* brief Get text by ID.
* param[in] widgetHandle Widget handle.
* param[out] strID    String ID.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see :HI_GV_Widget_SetTextByID
*/
HI_S32 HI_GV_Widget_GetTextID(HIGV_HANDLE widgetHandle, HI_U32 *strID);

/*
* brief Bind database to widget.
* param[in] widgetHandle Widget handle.
* param[in] dataSource Data source object.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* see:HI_GV_Widget_UnbindDataSource
*/
HI_S32 HI_GV_Widget_BindDataSource(HIGV_HANDLE widgetHandle, HIGV_HANDLE dataSource);

/*
* brief Unbind database to widget.
* param[in] widgetHandle  Widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
* see :HI_GV_Widget_BindDataSource
*/
HI_S32 HI_GV_Widget_UnbindDataSource(HIGV_HANDLE widgetHandle);

/*
* brief Get datasource handle.
* param[in] widgetHandle  Widget handle.
* param[out] dataSource The pointer of datasource.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_GetDataSource(HIGV_HANDLE widgetHandle, HIGV_HANDLE *dataSource);

/*
* brief Get window which the widget attach.
* param[in] widgetHandle Widget handle.
* param[out] windowHandle  the pointer of parent widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_GetWindow(HIGV_HANDLE widgetHandle, HIGV_HANDLE *windowHandle);

/*
* brief Immediate repaint control.
* param[in] widgetHandle  Widget handle.
* param[in] rect    The area of update
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_Update(HIGV_HANDLE widgetHandle, const HI_RECT *rect);

/*
* brief Immediate update the area of screen.
* param[in] widgetHandle Widget handle.
* param[in] rect  The area of update.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_Refresh(HIGV_HANDLE widgetHandle, const HI_RECT *refreshRect);

/*
* brief Get data from db and notify the widget to update data.
* param[in] widgetHandle  Widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_SyncDB(HIGV_HANDLE widgetHandle);

/*
* brief Get area of widget, the widget coordate is refre to the parent, and window is refer to the screen.
* ȡؼ򴰿ڵ丸Ļ \attention \n N/A.
*
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_GetRect(HIGV_HANDLE widgetHandle, HI_RECT *rect);

/*
* brief Set colorkey of window.
* param[in] windowHandle Window handle.
* param[in] colorKey colorekey value.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_SetWinColorKey(HIGV_HANDLE windowHandle, HI_COLOR colorKey);

/*
* brief is it a window.
* param[in] widgetHandle   Widget handle.
* retval ::HI_TRUE
* retval ::HI_FALSE
*/
HI_BOOL HI_GV_Widget_IsWindow(HIGV_HANDLE widgetHandle);

/*
* brief Cancel paint.
* param[in] widgetHandle Widet handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_Depaint(HIGV_HANDLE widgetHandle);

/*
* brief Convert the screen coordaite to widget coordinate.
* param[in] widgetHandle Widget handle.
* param[in] screenX The x coordinate of screen.
* param[in] screenY The y coordinate of screen.
* param[out] widgetX The x coordinate of widget.
* param[out] widgetY The y coordinate of widget.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_Screen2Widget(HIGV_HANDLE widgetHandle, HIGV_CORD screenX, HIGV_CORD screenY, HIGV_CORD *widgetX,
    HIGV_CORD *widgetY);

/*
* brief Convert thewidget coordaite to  screen coordinate.
* param[in] widgetHandle Widget handle.
* param[in] widgetX The x coordinate of widget.
* param[in] widgetY The y coordinate of widget.
* param[out] screenX The x coordinate of screen.
* param[out] screenY The y coordinate of screen.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_Widget2Screen(HIGV_HANDLE widgetHandle, HIGV_CORD widgetX, HIGV_CORD widgetY, HIGV_CORD *screenX,
    HIGV_CORD *screenY);

/*
* brief Get widget corresponding coordate.
* param[in]  screenX The x coordinate of widget.
* param[in]  screenY The x coordinate of widget.
* param[out] widgetHandle The x coordinate of screen.
* retval ::HI_SUCCESS
* retval ::HI_EINVAL
* retval ::HI_ENOTINIT
* retval ::HI_FAILURE
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_GetWidgetByPos(HIGV_CORD screenX, HIGV_CORD screenY, HIGV_HANDLE *widgetHandle);

/*
* brief Get widget corresponding coordate.
* param[in]  screenX The x coordinate of widget.
* param[in]  screenY The x coordinate of widget.
* param[out] widgetHandle The x coordinate of screen.
* retval ::HI_SUCCESS
* retval ::HI_EINVAL
* retval ::HI_ENOTINIT
* retval ::HI_FAILURE
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_GetWidgetByPos_TouchDevice(HIGV_CORD screenX, HIGV_CORD screenY, HIGV_HANDLE *widgetHandle);

/*
* brief Bind the scrollbar to widget.
* param[in] widgetHandle  Widget handle.
* param[in] scrollBarHandle Scrollbar handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_BindScrollBar(HIGV_HANDLE widgetHandle, HIGV_HANDLE scrollBarHandle);

/*
* brief Get the scrollbar of widget.
* param[in] widgetHandle  widget handle.
* param[in] scrollBarHandle  Binding scrollbar
* retval N/A.
*/
HI_S32 HI_GV_Widget_GetScrollBar(HIGV_HANDLE widgetHandle, HIGV_HANDLE *scrollBarHandle);

#ifdef HIGV_USE_WIDGET_BUTTON
/*
* brief Get the state of all checkbox.
* param[in] parentHandle Parent widget handle.
* param[in] status  Status.
* retval N/A.
*/
HI_S32 HI_GV_Widget_SetAllCheckStatus(HIGV_HANDLE parentHandle, HIGV_BUTTON_STATUS_E status);
#endif

/*
* brief Change window size, it is not effect widget.
* param[in] widgetHandle  Widget handle.
* param[in] width   Window width.
* param[in] height    widnow height.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_NOOP
*/
HI_S32 HI_GV_Widget_Resize(HIGV_HANDLE widgetHandle, HI_S32 width, HI_S32 height);

/*
* brief Query the widget whether support the input method window.
* param[in] widgetHandle      Widget handle.
* param[in] supportImeType  Input method type.
*             ȡֵΪλ
*             ֧У  HIGV_IMETYPE_ALLTYPE
*             Ӣģ      HIGV_IMETYPE_ENGLISH
*             дӢģ  HIGV_IMETYPE_CAPENGLISH
*             ֣      HIGV_IMETYPE_NUMBER
*             ţ      HIGV_IMETYPE_SYMBOL
*             ƴ      HIGV_IMETYPE_PINYIN
* retval HI_TRUE          Is need input method window.
* return HI_FALSE          it is not need.
*/
HI_BOOL HI_GV_Widget_IsNeedIMEWindow(HIGV_HANDLE widgetHandle, HI_U32 *supportImeType);

/*
* brief Get the layer corresponding widget.
* param[in]  widgetHandle       Widget handle.
* param[out] layerHandle       Layer.
* retval HI_SUCCESS
* return HI_ERR_COMM_LOST
* return HI_ERR_COMM_EMPTY
*/
HI_S32 HI_GV_Widget_GetLayer(HIGV_HANDLE widgetHandle, HIGV_HANDLE *layerHandle);

/*
* brief Set the private data of widget.
* attention :The memory of private data should be released by user.
*
* param[in]  widgetHandle      Widget handle.
* param[in] privateData    The pointer of private data.
* retval ::HI_SUCCESS
* return ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_SetPrivate(HIGV_HANDLE widgetHandle, const HI_VOID *privateData);

/*
* brief
* The memory of private data should be released by user.
*
* aram[in]  widgetHandle    Widget handle.
* param[out] privateData    The pointer of private data.
* retval ::HI_SUCCESS
* return ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_GetPrivate(HIGV_HANDLE widgetHandle, HI_VOID **privateData);

/*
* brief Get the clone handle corresponding layer.
* attention \n
* param[in]  widgetHandle  Widget handle.
* param[in]  layerHandle     Layer handle.
* param[out] cloneHandle   Clone handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_INTER
* retval ::HI_ERR_COMM_OVERFLOW The layer number is over.
*/
HI_S32 HI_GV_Widget_GetCloneHandle(HIGV_HANDLE widgetHandle, HIGV_HANDLE layerHandle, HIGV_HANDLE *cloneHandle);

/*
* brief Whether errasing the bakground.
* attention
* param[in]  widgetHandle  Widget handle.
* param[in] isErasebg
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_EraseBackground(HIGV_HANDLE widgetHandle, HI_BOOL isErasebg);

/*
* brief Register the information of widget type, user can apply this api to register their own widget.
* עؼϢûͨýӿ עԶĿؼϢ \attention \n N/A.
* info of widget type
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_EXIST
* retval ::HI_ERR_COMM_INVAL
* retval ::HI_ERR_COMM_NORES
*/
HI_S32 HI_GV_Widget_RgisterWidget(HIGV_WIDGET_TYPEINFO_S typeInfo);

/*
* brief Unregister the widget.
* param[in] typeID The type ID of widget.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_UnRgisterWidget(HI_U32 typeID);

/*
* brief Whether active the widget, This api is added for pop window of combobox.
* ΪӵĽӿڣʹģ̬ڣģ̬ϵĵ ҲԼ \attention \n it is should attention to us this
* api, otherwise the modal window will loss the function.
* widgetHandle   Widget handle.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_CanActive(HIGV_HANDLE widgetHandle);

/*
* brief enable/disable mirror the widget when change language.
* param[in] widgetHandle Widget handle.
* param[in] posMirror   Whether mirror widget pos.
* param[in] interiorMirror  Whether mirror widget interior.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* see : HI_GV_Widget_GetMirrorAttr
*/
HI_S32 HI_GV_Widget_EnableMirror(HIGV_HANDLE widgetHandle, HI_BOOL posMirror, HI_BOOL interiorMirror);

/*
* brief Get the mirror attribute.CNcommment:ȡؼ
* param[in] widgetHandle  Widget handle.
* param[out] posMirror   Whether mirror widget pos.
* param[out] interiorMirror  Whether mirror widget interior.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_EMPTY
* see : HI_GV_Widget_EnableMirror
*/
HI_S32 HI_GV_Widget_GetMirrorAttr(HIGV_HANDLE widgetHandle, HI_BOOL *posMirror, HI_BOOL *interiorMirror);

/*
* brief Set text direction of the widget.CNcommment:ǿƿؼıķ
* param[in] widgetHandle  Widget handle.
* param[in] direction  Text direction.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_SetTextDir(HIGV_HANDLE widgetHandle, HIGV_TEXTDIR_E direction);

/*
* brief Set focus switch mode of the widget.CNcommment:ÿؼлģʽ
* param[in] widgetHandle  Widget handle.
* param[in] fousMode.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_SetFocusMode(HIGV_HANDLE widgetHandle, HIGV_FOCUS_SWITCH_E focusMode);

/*
* brief Set text or image margin.
* attention:
* For button,label,trackbar,listbox,msgbox,timerbar.
*
* param[in] widgetHandle  Widget handle.
* param[in] leftMargin  Left margin.
* param[in] rightMargin  Right margin.
* param[in] topMargin  Top margin.
* param[in] bottomMargin  Bottom margin.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_SetMargin(HIGV_HANDLE widgetHandle, HI_U32 leftMargin, HI_U32 rightMargin, HI_U32 topMargin,
    HI_U32 bottomMargin);

/*
* brief Get the margin.
* param[in] widgetHandle  Widget handle.
* param[out] leftMargin Left margin.
* param[out] rightMargin  Right margin.
* param[out] topMargin  Top margin.
* param[out] bottomMargin Bottom margin.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Widget_GetMargin(HIGV_HANDLE widgetHandle, HI_U32 *leftMargin, HI_U32 *rightMargin, HI_U32 *topMargin,
    HI_U32 *bottomMargin);

/*
* brief enable config image res path.CNcommment:ʹͼƬԴ·
* param[in] widgetHandle  Widget handle.
* param[in] enable enable or disable image res path.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_EnableResPach(HIGV_HANDLE widgetHandle, HI_BOOL enable);

/*
* brief enable widget forget focus.CNcommment: show & activeؼ
* param[in] widgetHandle  Widget handle.
* param[in] activeShow.
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_Widget_ActiveShow(HIGV_HANDLE widgetHandle, HI_BOOL activeShow);

/*
* brief Set bit block transfer operation of picture type skin.CNcommment: ÿؼͼƬƤİƻϲ
* param[in] widgetHandle  Widget handle.
* param[in] blitOpt Bit block transfer operation.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
*/
HI_S32 HI_GV_Widget_SetSkinBlitOpt(HIGV_HANDLE widgetHandle, const HIGO_BLTOPT_S *blitOpt);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* HI_GV_WIDGET_H */
