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

#ifndef HI_GV_BUTTON_H
#define HI_GV_BUTTON_H

/* add include here */
#include "hi_gv_resm.h"
#include "hi_gv_conf.h"

#ifdef HIGV_USE_WIDGET_BUTTON
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    HI_U32 buttonStyle;
    HIGV_HANDLE checkedNormal;
    HIGV_HANDLE checkedDisable;
    HIGV_HANDLE checkedHighlight;
    HIGV_HANDLE checkedActive;
    HIGV_HANDLE checkedMouseDown;
    HI_U32 textOn;
    HI_U32 textOff;
    HI_U32 switchLayout;
    HI_COLOR toggleOnColor;
    HI_COLOR toggleOffColor;
} HIGV_BUTTON_PRIVATE;

/* Normal button */
#define HIGV_BS_PUSH HIGV_STYLE_DEFINE(HIGV_WIDGET_BUTTON, BUTTON_STYLE_PUSH)

/*  Radio button */
#define HIGV_BS_RADIO HIGV_STYLE_DEFINE(HIGV_WIDGET_BUTTON, BUTTON_STYLE_RADIO)

/*  Check button */
#define HIGV_BS_CHECK HIGV_STYLE_DEFINE(HIGV_WIDGET_BUTTON, BUTTON_STYLE_CHECK)

/*  Softkey button */
#define HIGV_BS_SOFTKEY HIGV_STYLE_DEFINE(HIGV_WIDGET_BUTTON, BUTTON_STYLE_SOFTKEY)

/*  Switch button */
#define HIGV_BS_SWITCH HIGV_STYLE_DEFINE(HIGV_WIDGET_BUTTON, BUTTON_STYLE_SWITCH)

/*  Toggle button */
#define HIGV_BS_TOGGLE HIGV_STYLE_DEFINE(HIGV_WIDGET_BUTTON, BUTTON_STYLE_TOGGLE)

/*  Paint toggle min height */
#define HIGV_BUTTON_TOGGLE_MIN_HEIGHT 5

/*  Paint toggle min width */
#define HIGV_BUTTON_TOGGLE_MIN_WIDTH 19

/* Button type */
typedef enum {
    BUTTON_STYLE_PUSH = 0,
    /* Push button , */
    BUTTON_STYLE_RADIO,
    /* Radio button , */
    BUTTON_STYLE_CHECK,
    /* Check button , */
    BUTTON_STYLE_SOFTKEY,
    /* Softkey button , */
    BUTTON_STYLE_SWITCH,
    /* Switch button , */
    BUTTON_STYLE_TOGGLE,
    /* Toggle button , */
    BUTTON_STYLE_BUTT
} HIGV_BUTTON_SYLE_E;

/* Button status , */
typedef enum {
    BUTTON_STATUS_UNCHECKED = 0,
    /* Unchecked status , */
    BUTTON_STATUS_CHECKED,
    /* Checked status , */
    BUTTON_STATUS_BUTT
} HIGV_BUTTON_STATUS_E;

/* Button switch status , */
typedef enum {
    BUTTON_SWITCH_OFF = 0,
    /* Switch off , */
    BUTTON_SWITCH_ON,
    /* Switch on , */
    BUTTON_SWITCH_BUTT
} HIGV_BUTTON_SWITCH_E;

/* Switch button text layout , */
typedef enum {
    BUTTON_SWITCHLAYOUT_OFF_ON = 0,
    /* "off:on" layout , */
    BUTTON_SWITCHLAYOUT_ON_OFF,
    /* "on:off" layout , */
    BUTTON_SWITCHLAYOUT_BUTT
} HIGV_BUTTON_SWITCHLAYOUT_E;

HI_S32 HI_GV_Button_SetCheck(HIGV_HANDLE buttonHandle, HIGV_BUTTON_STATUS_E status);

HI_S32 HI_GV_Button_GetCheck(HIGV_HANDLE buttonHandle, HIGV_BUTTON_STATUS_E *status);

HI_S32 HI_GV_Button_SetSwitch(HIGV_HANDLE buttonHandle, HIGV_BUTTON_SWITCH_E switchStatus);

HI_S32 HI_GV_Button_GetSwitch(HIGV_HANDLE buttonHandle, HIGV_BUTTON_SWITCH_E *switchStatus);

HI_S32 HI_GV_Button_SetToggleColor(HIGV_HANDLE buttonHandle, HI_COLOR toggleOnColor, HI_COLOR toggleOffColor);

HI_S32 HI_GV_Button_SetSwitchTextByID(HIGV_HANDLE buttonHandle, const HI_U32 onStrID, const HI_U32 offStrID);

HI_S32 HI_GV_Button_SetSwitchLayout(HIGV_HANDLE buttonHandle, HIGV_BUTTON_SWITCHLAYOUT_E textLayout);

#ifdef __cplusplus
}
#endif
#endif
#endif /*  HI_GV_BUTTON_H */
