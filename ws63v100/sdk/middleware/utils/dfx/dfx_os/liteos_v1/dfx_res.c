/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: dfx res
 * This file should be changed only infrequently and with great care.
 */
#include "dfx_res.h"
#include "osal_inner.h"

#define ENABLE_LOS_CUSTOM_DEBUG 0
#if defined(SOCMN1_PRODUCT_EVB) || defined(SOCMN1_PRODUCT_EVB_K)
#if ENABLE_LOS_CUSTOM_DEBUG
#include "los_custom_debug.h"
#endif
#endif

errcode_t dfx_os_get_resource_status(const osal_os_resource_use_stat_t *os_resource_stat)
{
    if (os_resource_stat == NULL) {
        return ERRCODE_FAIL;
    }
#if ENABLE_LOS_CUSTOM_DEBUG
#if defined(SOCMN1_PRODUCT_EVB) || defined(SOCMN1_PRODUCT_EVB_K)
    os_resource_stat->timer_usage = (uint8_t)LOS_SwtmrUsage();
    os_resource_stat->task_usage = (uint8_t)LOS_TaskUsage(NULL);
    os_resource_stat->sem_usage = (uint8_t)LOS_SemUsage();
    os_resource_stat->queue_usage = (uint8_t)LOS_QueueUsage();
    os_resource_stat->mux_usage = (uint8_t)LOS_MuxUsage();
#endif
#endif
    return ERRCODE_SUCC;
}