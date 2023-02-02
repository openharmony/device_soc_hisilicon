/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#include "hi_common.h"

#ifndef __HI_VOU_EXP_H__
#define __HI_VOU_EXP_H__

typedef struct hiVOU_EXPORT_CALLBACK_S {
    void (*pfnVoNotify)(int module_id, int vodev);
} VOU_EXPORT_CALLBACK_S;

typedef HI_S32 FN_VO_RegisterExternCallback(VOU_EXPORT_CALLBACK_S *pstExpCallback);

typedef struct hiVOU_EXPORT_SYMBOL_S {
    FN_VO_RegisterExternCallback *pfnVoRegisterExpCallback;
} VOU_EXPORT_SYMBOL_S;

#endif /* __HI_VOU_EXP_H__ */
