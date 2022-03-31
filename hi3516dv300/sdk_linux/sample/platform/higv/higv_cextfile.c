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
#include "higv_cextfile.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*  __cplusplus  */

HIGV_MSG_PROC g_pfunHIGVAppEventFunc[18] = {
    (HIGV_MSG_PROC)BUTTON_WIN_Onrefresh,
    (HIGV_MSG_PROC)BUTTON_WIN_Ontimer,
    (HIGV_MSG_PROC)BUTTON_WIN_Onhide,
    (HIGV_MSG_PROC)IMAGE_WIN_Onshow,
    (HIGV_MSG_PROC)IMAGE_WIN_Onhide,
    (HIGV_MSG_PROC)IMAGE_WIN_Ontimer,
    (HIGV_MSG_PROC)LABEL_WIN_Onrefresh,
    (HIGV_MSG_PROC)LABEL_WIN_Ontimer,
    (HIGV_MSG_PROC)LABEL_WIN_Onhide,
    (HIGV_MSG_PROC)LISTBOX_WIN_Onshow,
    (HIGV_MSG_PROC)LISTBOX_WIN_Onrefresh,
    (HIGV_MSG_PROC)LISTBOX_WIN_Ontimer,
    (HIGV_MSG_PROC)LISTBOX_WIN_Onhide,
    (HIGV_MSG_PROC)SCROLLVIEW_WIN_Onshow,
    (HIGV_MSG_PROC)SCROLLVIEW_WIN_Onrefresh,
    (HIGV_MSG_PROC)SCROLLVIEW_WIN_Ontimer,
    (HIGV_MSG_PROC)SCROLLVIEW_WIN_Onhide,
    (HIGV_MSG_PROC)OnViewMoveListener
};


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
