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

#ifndef OT_CONFACCESS_H
#define OT_CONFACCESS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ot_mw_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*
 * load config, including common config and all the submode config
 */
TD_S32 SS_CONFACCESS_Init(const TD_CHAR *cfgName, const TD_CHAR *cfgPath);

TD_S32 SS_CONFACCESS_Deinit(const TD_CHAR *cfgName);

/*
 * get string configure item value with specified cfgName and module
 * if item not found the default will be given
 */
TD_S32 SS_CONFACCESS_GetString(const TD_CHAR *cfgName, const TD_CHAR *module, const TD_CHAR *confItem,
    TD_CHAR *defValue, TD_CHAR ** const value);

TD_S32 SS_CONFACCESS_GetInt(const TD_CHAR *cfgName, const TD_CHAR *module, const TD_CHAR *confItem, TD_S32 defValue,
    TD_S32 * const value);

TD_S32 SS_CONFACCESS_GetDouble(const TD_CHAR *cfgName, const TD_CHAR *module, const TD_CHAR *confItem,
    TD_DOUBLE defValue, TD_DOUBLE * const value);

TD_S32 SS_CONFACCESS_GetBool(const TD_CHAR *cfgName, const TD_CHAR *module, const TD_CHAR *confItem, TD_BOOL defValue,
    TD_BOOL *value);

TD_S32 SS_CONFACCESS_SetString(const TD_CHAR *cfgName, const TD_CHAR *module, const TD_CHAR *confItem,
    const TD_CHAR *value);

TD_S32 SS_CONFACCESS_IsModuleExist(const TD_CHAR *cfgName, const TD_CHAR *module, TD_BOOL *isExist);

TD_S32 SS_CONFACCESS_IsConfItemExist(const TD_CHAR *cfgName, const TD_CHAR *module, const TD_CHAR *confItem,
    TD_BOOL *isExist);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* OT_CONFACCESS_H */
