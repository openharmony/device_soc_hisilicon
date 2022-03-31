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

#ifndef HI_GV_GESTURE_H
#define HI_GV_GESTURE_H

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef enum hiHIGV_TOUCH_E {
    /* touch start event, the user touch screen */
    HIGV_TOUCH_START = 1,
    /* touch end event, the user release after touch screen */
    HIGV_TOUCH_END,
    /* touch move event, move on the touch screen */
    HIGV_TOUCH_MOVE,
    /* touch cancel event, the current gesture is interrupted. */
    HIGV_TOUCH_CANCEL,
    /* touch pointer start event, when a finger is already on the screen
     * the other finger is touching the screen
     */
    HIGV_TOUCH_POINTER_START,
    /* touch pointer end event, trigger when a finger moves from the screen. */
    HIGV_TOUCH_POINTER_END,
    HIGV_TOUCH_BUTT
} HIGV_TOUCH_E;

/* touch single point param */
typedef struct hiHIGV_TOUCH_POINT_S {
    /* touch event serial number */
    HI_U32 id;
    /* x coordinates relative to the screen */
    HI_S32 x;
    /* y coordinates relative to the screen */
    HI_S32 y;
    /* screen pressure */
    HI_S32 pressure;
    /* event timestamp */
    HI_U64 timeStamp;
    /* event type */
    HIGV_TOUCH_E type;
} HIGV_TOUCH_POINT_S;

/* touch event param */
typedef struct hiHIGV_TOUCH_EVENT_S {
    /* the last event */
    HIGV_TOUCH_POINT_S last;
} HIGV_TOUCH_EVENT_S;

/* gesture event enum */
typedef enum hiHIGV_GESTURE_E {
    /* gesture tap event, touch screen and up */
    HIGV_GESTURE_TAP = 1,
    /* gesture longpress event, long press screen */
    HIGV_GESTURE_LONGTAP,
    /* gesture fling event, user swipe screen */
    HIGV_GESTURE_FLING,
    /* gesture scroll event, user sliding screen */
    HIGV_GESTURE_SCROLL,
    /*  gesture pinch event, zoom on the screen */
    HIGV_GESTURE_PINCH,
    HIGV_GESTURE_BUTT
} HIGV_GESTURE_E;

typedef struct hiHIGV_GESTURE_TAP_S {
    /* tap pointer information */
    HIGV_TOUCH_POINT_S pointer;
} HIGV_GESTURE_TAP_S;

typedef struct hiHIGV_GESTURE_LONGTAP_S {
    /* longtap pointer information */
    HIGV_TOUCH_POINT_S pointer;
} HIGV_GESTURE_LONGTAP_S;

typedef struct hiHIGV_GESTURE_FLING_S {
    /* start pointer information */
    HIGV_TOUCH_POINT_S start;
    /* end pointer information */
    HIGV_TOUCH_POINT_S end;
    /* X axis speed per second */
    HI_S32 velocityX;
    /*  Y axis speed per second */
    HI_S32 velocityY;
} HIGV_GESTURE_FLING_S;

typedef struct hiHIGV_GESTURE_SCROLL_S {
    /* start pointer information */
    HIGV_TOUCH_POINT_S start;
    /* end pointer information */
    HIGV_TOUCH_POINT_S end;
    /* moving distance between two points on X axis unit time */
    HI_S32 distanceX;
    /* moving distance between two points on Y axis unit time */
    HI_S32 distanceY;
} HIGV_GESTURE_SCROLL_S;

typedef struct hiHIGV_GESTURE_PINCH_S {
    /* one flinger pointer information */
    HIGV_TOUCH_POINT_S pointer1;
    /* another flinger pointer information */
    HIGV_TOUCH_POINT_S pointer2;
    /* distance of X axis at two points */
    HI_S32 intervalX;
    /* distance of Y axis at two points */
    HI_S32 intervalY;
} HIGV_GESTURE_PINCH_S;

typedef union hiHIGV_GESTURE_U {
    /* tap param */
    HIGV_GESTURE_TAP_S tap;
    /* longtap param */
    HIGV_GESTURE_LONGTAP_S longtap;
    /* fling param */
    HIGV_GESTURE_FLING_S fling;
    /* scroll param */
    HIGV_GESTURE_SCROLL_S scroll;
    /* pinch param */
    HIGV_GESTURE_PINCH_S pinch;
} HIGV_GESTURE_U;

typedef struct hiHIGV_GESTURE_EVENT_S {
    HIGV_GESTURE_U gesture;
} HIGV_GESTURE_EVENT_S;

typedef struct hiHIGV_TOUCH_INPUTINFO_S {
    /* input id info, one finger or two fingers */
    HI_S32 id;
    /* x coordinate absolute */
    HI_S32 x;
    /* y coordinate absolute */
    HI_S32 y;
    /* is press on screen: 0, 1 */
    HI_U32 pressure;
    /* time stamp */
    HI_U64 timeStamp;
} HIGV_TOUCH_INPUTINFO_S;

typedef struct hiHIGV_TOUCH_DEVICE_INPUT_S {
    /* open touch device function */
    HI_S32 (*OpenDevice)(HI_S32 *fd);
    /* close touch device function */
    HI_S32 (*CloseDevie)(HI_VOID);
    /* read touch data function */
    HI_S32 (*ReadData)(HIGV_TOUCH_INPUTINFO_S *info);
} HIGV_TOUCH_DEVICE_INPUT_S;

/*
 * brief Callback of touch hook.
 * param[in] pTouchEvent  Touch Event.
 * retval ::HI_SUCCESS
 * retval ::HI_FAILURE
 */
typedef HI_S32 (*PTR_TOUCHHOOK_CallBack)(HIGV_TOUCH_POINT_S *touchEvent);

/*
 * brief Touch device register.
 * param[in] inputDevice  Input device function.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_NOTINIT
 */
HI_S32 HI_GV_Gesture_RegisterDevice(const HIGV_TOUCH_DEVICE_INPUT_S *inputDevice);

/*
 * brief Set touch callback function.
 * param[in] touchCallBack  Touch callback function.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_NOTINIT
 */
HI_S32 HI_GV_Gesture_SetTouchHook(PTR_TOUCHHOOK_CallBack touchCallBack);

/*
 * brief Set scroll event first trigger value.
 * param[in] scrollThreshold  First trigger value.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 */
HI_S32 HI_GV_Gesture_SetScrollFTV(HI_U32 scrollThreshold);

/*
 * brief Get scroll event first trigger value.
 * param[in] scrollThreshold  First trigger value.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_NOTINIT
 */
HI_S32 HI_GV_Gesture_GetScrollFTV(HI_U32 *scrollThreshold);

/*
 * brief Set scroll event trigger value.
 * param[in] scrollThreshold  First trigger value.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 */
HI_S32 HI_GV_Gesture_SetScrollTV(HI_U32 scrollThreshold);

/*
 * brief Get scroll event  trigger value.
 * param[in] scrollThreshold First trigger value.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_NOTINIT
 */
HI_S32 HI_GV_Gesture_GetScrollTV(HI_U32 *scrollThreshold);

/*
 * brief Set gesture enable.
 * param[in] isEnable enable tag.
 * retval ::0 success
 * retval ::-1 failure
 * see::HI_GV_Gesture_IsEnabled
 */
HI_S32 HI_GV_Gesture_Enable(HI_BOOL isEnable);

/*
 * brief Check gesture status .
 * retval ::HI_TRUE
 * retval ::HI_FLASE
 * see::HI_GV_Gesture_Enable
 */
HI_BOOL HI_GV_Gesture_IsEnabled(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif
