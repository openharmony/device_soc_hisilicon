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

#ifndef HI_GV_ANIMATION_H
#define HI_GV_ANIMATION_H

#include "hi_type.h"
#include "hi_gv_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Listener notify function */
typedef struct _HIGV_LISTENER_S HIGV_LISTENER_S;
typedef HI_VOID (*higv_notify_func_t)(HIGV_LISTENER_S *listener, void *data);

#define HIGV_ANIMATION_TWEEN_NUM 16

/* Thansition type */
typedef enum {
    HIGV_TWEEN_TRANSITION_LINEAR,  /* Linear transition */
    HIGV_TWEEN_TRANSITION_SINE,    /* Sine transition */
    HIGV_TWEEN_TRANSITION_QUINT,   /* Quint transition */
    HIGV_TWEEN_TRANSITION_QUART,   /* Quart transition */
    HIGV_TWEEN_TRANSITION_EXPO,    /* Expo transition */
    HIGV_TWEEN_TRANSITION_ELASTIC, /* Elastic transition */
    HIGV_TWEEN_TRANSITION_CUBIC,   /* Cubic transition */
    HIGV_TWEEN_TRANSITION_BOUNCE,  /* Bounce transition */
    HIGV_TWEEN_TRANSITION_BACK,    /* Back transition */
    HIGV_TWEEN_TRANSITION_BUTT
} HIGV_TWEEN_TRANSITION_E;

/* Easing type */
typedef enum {
    HIGV_TWEEN_EASE_IN,     /* Acceleration */
    HIGV_TWEEN_EASE_OUT,    /* Deceleration */
    HIGV_TWEEN_EASE_IN_OUT, /* Accelerate after the deceleration */
    HIGV_TWEEN_EASE_BUTT
} HIGV_TWEEN_EASE_E;

HIGV_HANDLE HI_GV_TweenAnimCreate(HI_VOID);

HI_S32 HI_GV_TweenAnimSetDuration(HIGV_HANDLE animHandle, HI_U32 ms);

HI_S32 HI_GV_TweenAnimGetDuration(HIGV_HANDLE animHandle, HI_S32 *duration);

HI_S32 HI_GV_TweenAnimGetCurrentTime(HIGV_HANDLE animHandle, HI_S64 *currentTime);

HI_S32 HI_GV_TweenAnimSetDelay(HIGV_HANDLE animHandle, HI_U32 ms);

HI_S32 HI_GV_TweenAnimSetLoops(HIGV_HANDLE animHandle, HI_U32 loopCount);

HI_S32 HI_GV_TweenAnimSetStartedListener(HIGV_HANDLE animHandle, higv_notify_func_t func);

HI_S32 HI_GV_TweenAnimSetFinishedListener(HIGV_HANDLE animHandle, higv_notify_func_t func);

HI_S32 HI_GV_TweenAnimSetExecListener(HIGV_HANDLE animHandle, higv_notify_func_t func);

HI_S32 HI_GV_TweenAnimStart(HIGV_HANDLE animHandle);

HI_S32 HI_GV_TweenAnimStop(HIGV_HANDLE animHandle);

HI_S32 HI_GV_TweenAnimResume(HIGV_HANDLE animHandle);

HI_S32 HI_GV_TweenAnimPause(HIGV_HANDLE animHandle);

HI_S32 HI_GV_TweenAnimAddTween(HIGV_HANDLE animHandle, HIGV_TWEEN_TRANSITION_E transition,
    HIGV_TWEEN_EASE_E easing, HI_FLOAT initialValue, HI_FLOAT endValue);

HI_FLOAT HI_GV_TweenAnimGetTweenValue(HIGV_HANDLE animHandle, HI_U32 index);

HI_S32 HI_GV_TweenAnimSetTweenRange(HIGV_HANDLE animHandle, HI_U32 index, HI_FLOAT initialValue,
    HI_FLOAT endValue);

HI_S32 HI_GV_TweenAnimDestroy(HIGV_HANDLE animHandle);

HI_S32 HI_GV_Anim_CreateInstance(HIGV_HANDLE animInfoHandle, HIGV_HANDLE widgetHandle, HIGV_HANDLE *instanceHandle);

HI_S32 HI_GV_Anim_DestroyInstance(HIGV_HANDLE windowHandle, HIGV_HANDLE animHandle);

HI_S32 HI_GV_Anim_GetInfo(HIGV_HANDLE windowHandle, HIGV_HANDLE animHandle,
    HIGV_HANDLE *animInfoHandle, HIGV_HANDLE *widgetHandle);

HI_S32 HI_GV_Anim_Start(HIGV_HANDLE windowHandle, HIGV_HANDLE animHandle);

HI_S32 HI_GV_Anim_Stop(HIGV_HANDLE windowHandle, HIGV_HANDLE animHandle);

#ifdef __cplusplus
}
#endif
#endif
