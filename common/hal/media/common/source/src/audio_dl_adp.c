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

#include "audio_dl_adp.h"
#include <stdio.h>
#ifndef __LITEOS__
#include <dlfcn.h>
#else
#include <los_ld_elflib.h>
#endif

td_s32 audio_dlpath(td_char *lib_path)
{
#ifndef __LITEOS__
    (td_void)lib_path;
#else
    if (LOS_PathAdd(lib_path) != TD_SUCCESS) {
        printf("add path %s failed!\n", lib_path);
        return TD_FAILURE;
    }
#endif

    return TD_SUCCESS;
}

td_s32 audio_dlopen(td_void **lib_handle, td_char *lib_name)
{
    if ((lib_handle == TD_NULL) || (lib_name == TD_NULL)) {
        return TD_FAILURE;
    }

    *lib_handle = TD_NULL;
#ifndef __LITEOS__
    *lib_handle = dlopen(lib_name, RTLD_LAZY | RTLD_LOCAL);
#else
    *lib_handle = LOS_SoLoad(lib_name);
#endif

    if (*lib_handle == TD_NULL) {
        printf("dlopen %s failed!\n", lib_name);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 audio_dlsym(td_void **func_handle, td_void *lib_handle, td_char *func_name)
{
    if (lib_handle == TD_NULL) {
        printf("LibHandle is empty!");
        return TD_FAILURE;
    }

    *func_handle = TD_NULL;
#ifndef __LITEOS__
    *func_handle = dlsym(lib_handle, func_name);
#else
    *func_handle = LOS_FindSymByName(lib_handle, func_name);
#endif

    if (*func_handle == TD_NULL) {
#ifndef __LITEOS__
        printf("dlsym %s fail, error msg is %s!\n", func_name, dlerror());
#else
        printf("dlsym %s fail!\n", func_name);
#endif
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 audio_dlclose(td_void *lib_handle)
{
    if (lib_handle == TD_NULL) {
        printf("LibHandle is empty!");
        return TD_FAILURE;
    }

#ifndef __LITEOS__
    dlclose(lib_handle);
#else
    LOS_ModuleUnload(lib_handle);
#endif

    return TD_SUCCESS;
}
