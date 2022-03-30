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
#ifndef AUDIO_DL_ADP_H
#define AUDIO_DL_ADP_H

#include "hi_type.h"

HI_S32 Audio_Dlpath(HI_CHAR *pChLibPath);

HI_S32 Audio_Dlopen(HI_VOID **pLibhandle, HI_CHAR *pChLibName);

HI_S32 Audio_Dlsym(HI_VOID **pFunchandle, HI_VOID *Libhandle, HI_CHAR *pChFuncName);

HI_S32 Audio_Dlclose(HI_VOID *Libhandle);

#endif