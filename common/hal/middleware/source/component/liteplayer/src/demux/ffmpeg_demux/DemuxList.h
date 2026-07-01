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
#ifndef DEMUXLIST_H
#define DEMUXLIST_H

#include <pthread.h>
#include "ot_mw_type.h"
#include "ot_list.h"
#include "securec.h"
#include "ff_demux_log.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef struct {
    OtListHead header;
    pthread_mutex_t listLock;
} DemuxListInstance;

typedef struct {
    OtListHead listPtr;
    TD_MW_PTR priv;
} DemuxListNode;

TD_S32 DemuxListAddNode(DemuxListInstance *listInstance, const void *priv);

TD_S32 DemuxListDelNode(DemuxListInstance *listInstance, const void *priv);

TD_BOOL DemuxListCheckExist(DemuxListInstance *listInstance, const void *priv);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* DEMUXLIST_H */

