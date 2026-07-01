/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
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

#ifndef PROTOCOL_INTERFACE_H
#define PROTOCOL_INTERFACE_H

#include "plugin_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    PluginDesc desc;

    int (*protocol_find)(const char *url);
    int (*protocol_open)(const char *url, int flags, void **handle);
    int (*protocol_seek)(void *handle, int64_t pos, int whence);
    int (*protocol_seek_stream)(void *handle, int32_t streamId, int64_t pts, uint32_t flags);
    int (*protocol_read)(void *handle, unsigned char *buf, int size);
    int (*protocol_write)(void *handle, const unsigned char *buf, int size);
    int (*protocol_invoke)(void *handle, uint32_t invokeId, void *arg);
    int (*protocol_close)(void *handle);
} ProtocolPluginEntry;

typedef const ProtocolPluginEntry *(*GetProtocolFun)(void);
const ProtocolPluginEntry *GetProtocol(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // PROTOCOL_INTERFACE_H
