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

#ifndef HI_GV_CONF_H
#define HI_GV_CONF_H

typedef unsigned int HIGV_HANDLE;

#ifndef HIGV_UNUSED
#define HIGV_UNUSED(expr) \
    do {                  \
        (void)(expr);       \
    } while (0)
#endif

#ifdef __LP64__
typedef unsigned long long HI_PARAM;
typedef struct {
    long long x;
    long long y;
    long long w;
    long long h;
} HIGV_RECT_WRAPPER;
#else
typedef unsigned int HI_PARAM;
typedef struct {
    int x;
    int y;
    int w;
    int h;
} HIGV_RECT_WRAPPER;
#endif

typedef HI_PARAM HIGV_COLOR_WRAPPER;

#define HIGV_COMMON_MODID_START 50
#define HIGV_PERFORMACE 1
#define HIGV_USE_ADP_INPUT 1
#define HIGV_KEY_SINGLEQUE 1
#define HI_OS_LINUX 1
#define HI_OS_TYPE HI_OS_LINUX

#define HIGV_USE_TOUCHSCREEN_INPUT 1
#define HIGV_USE_MOUSE_INPUT 1
#define HIGV_USE_WIDGET_BUTTON    1
#define HIGV_USE_WIDGET_SCROLLBAR 1
#define HIGV_USE_WIDGET_LABEL 1
#define HIGV_USE_WIDGET_IMAGE 1
#define HIGV_USE_WIDGET_MSGBOX      1
#define HIGV_USE_WIDGET_IMAGEEX     1
#define HIGV_USE_WIDGET_EDIT        1
#define HIGV_USE_WIDGET_PROGRESSBAR 1
#define HIGV_USE_WIDGET_SCROLLTEXT  1
#define HIGV_USE_WIDGET_SCROLLGRID  1
#define HIGV_USE_WIDGET_CLOCK       1
#define HIGV_USE_WIDGET_LISTBOX     1
#define HIGV_USE_WIDGET_TRACKBAR    1
#define HIGV_USE_WIDGET_SLIDEUNLOCK 1
#define HIGV_USE_WIDGET_WHEELVIEW   1
#define HIGV_USE_MODULE_IME        1
#define HIGV_USE_MODULE_CURSOR     1
#define HIGV_USE_MODULE_TOUCH      1
#define HIGV_USE_WIDGET_SCROLLVIEW 1
#define HIGV_USE_WIDGET_CONTENTBOX 1
#define HIGV_USE_WIDGET_SCALEVIEW 1
#define HIGV_USE_WIDGET_MULTIEDIT 1
#define HIGV_USE_WIDGET_CHARTS 1
#endif // HI_GV_CONF_H
