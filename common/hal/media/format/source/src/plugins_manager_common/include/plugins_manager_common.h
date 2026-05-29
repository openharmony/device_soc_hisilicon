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

#ifndef PLUGINS_MANAGER_COMMON_H
#define PLUGINS_MANAGER_COMMON_H
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>
#include "plugin_common.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct tafListNode {
    struct tafListNode *prev;
    struct tafListNode *next;
} ListNode;

typedef struct {
    ListNode list;
    const PluginDesc *plugin;
    void *module;
} PluginEntry;

typedef PluginEntry *(*GetPluginEntry)(const char *name, int32_t len);

PluginEntry *PluginFillNode(const PluginDesc *entry, const void *module);
void PluginDeleteNode(PluginEntry *node);
void PluginRemoveAllNodes(PluginEntry *list);
void PluginScanPathAndAddNode(PluginEntry **list, int32_t *nodeNum, GetPluginEntry getEntry);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // PLUGINS_MANAGER_COMMON_H
