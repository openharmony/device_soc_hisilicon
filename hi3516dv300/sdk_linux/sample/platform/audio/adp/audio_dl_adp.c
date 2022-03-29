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
#include "audio_dl_adp.h"
#include <stdio.h>

#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
#include <dlfcn.h>
#else
#include <los_ld_elflib.h>
#endif

HI_S32 Audio_Dlpath(HI_CHAR *pChLibPath)
{
#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
    (hi_void)pChLibPath;
#else
    if (LOS_PathAdd(pChLibPath) != HI_SUCCESS) {
        printf("add path %s failed!\n", pChLibPath);
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}

HI_S32 Audio_Dlopen(HI_VOID **pLibhandle, HI_CHAR *pChLibName)
{
    if (pLibhandle == HI_NULL || pChLibName == HI_NULL) {
        return HI_FAILURE;
    }

    *pLibhandle = HI_NULL;
#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
    *pLibhandle = dlopen(pChLibName, RTLD_LAZY | RTLD_LOCAL);
#else
    *pLibhandle = LOS_SoLoad(pChLibName);
#endif
    if (*pLibhandle == HI_NULL) {
        printf("dlopen %s failed!\n", pChLibName);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 Audio_Dlsym(HI_VOID **pFunchandle, HI_VOID *Libhandle, HI_CHAR *pChFuncName)
{
    if (pFunchandle == HI_NULL || Libhandle == HI_NULL || pChFuncName == HI_NULL) {
        printf("LibHandle or FuncName is empty!");
        return HI_FAILURE;
    }

    *pFunchandle = HI_NULL;
#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
    *pFunchandle = dlsym(Libhandle, pChFuncName);
#else
    *pFunchandle = LOS_FindSymByName(Libhandle, pChFuncName);
#endif

    if (*pFunchandle == HI_NULL) {
#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
        printf("dlsym %s fail,error msg is %s!\n", pChFuncName, dlerror());
#else
        printf("dlsym %s fail!\n", pChFuncName);
#endif
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 Audio_Dlclose(HI_VOID *Libhandle)
{
    if (Libhandle == HI_NULL) {
        printf("LibHandle is empty!");
        return HI_FAILURE;
    }

#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
    dlclose(Libhandle);
#else
    LOS_ModuleUnload(Libhandle);
#endif

    return HI_SUCCESS;
}
