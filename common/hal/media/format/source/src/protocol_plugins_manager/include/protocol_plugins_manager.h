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

#ifndef PROTOCOL_PLUGINS_MANAGER_H
#define PROTOCOL_PLUGINS_MANAGER_H

#include "plugins_manager_common.h"
#include "protocol_interface.h"
#include "protocol_plugins_manager.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

void ProtocolPluginManagerInit(void);
void ProtocolPluginManagerDeInit(void);
int32_t ProtocolPluginManagerFindPlugin(const char *url, int32_t len, ProtocolFun *func);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // PROTOCOL_PLUGINS_MANAGER_H
/** @} */
