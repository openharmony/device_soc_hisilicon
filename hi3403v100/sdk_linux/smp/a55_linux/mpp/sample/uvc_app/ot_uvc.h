/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __OT_UVC_H__
#define __OT_UVC_H__

typedef struct ot_uvc {
    int (*init)();
    int (*open)();
    int (*close)();
    int (*run)();
} ot_uvc;

ot_uvc *get_ot_uvc();
void release_ot_uvc(ot_uvc *uvc);

#endif // __OT_UVC_H__
