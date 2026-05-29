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

#ifndef OT_MOTIONSENSOR_MNG_CMD_H
#define OT_MOTIONSENSOR_MNG_CMD_H

#include "osal_ioctl.h"
#include "ot_common_motionsensor.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define MSENSOR_TYPE_MNG    10

typedef enum {
    IOC_NR_MSENSOR_MNG_GET_DATA = 0,
    IOC_NR_MSENSOR_MNG_RELEASE_BUF,
    IOC_NR_MSENSOR_MNG_ADD_USER,
    IOC_NR_MSENSOR_MNG_DELETE_USER,
    IOC_NR_MSENSOR_MNG_SEND_DATA,
    IOC_NR_MSENSOR_MNG_BUTT
} ioc_nr_msensor_mng;

#define MSENSOR_CMD_GET_DATA    OSAL_IOWR(MSENSOR_TYPE_MNG, IOC_NR_MSENSOR_MNG_GET_DATA,    ot_msensor_data_info)
#define MSENSOR_CMD_RELEASE_BUF OSAL_IOWR(MSENSOR_TYPE_MNG, IOC_NR_MSENSOR_MNG_RELEASE_BUF, ot_msensor_data_info)
#define MSENSOR_CMD_ADD_USER    OSAL_IOWR(MSENSOR_TYPE_MNG, IOC_NR_MSENSOR_MNG_ADD_USER,    td_s32)
#define MSENSOR_CMD_DELETE_USER OSAL_IOWR(MSENSOR_TYPE_MNG, IOC_NR_MSENSOR_MNG_DELETE_USER, td_s32)
#define MSENSOR_CMD_SEND_DATA   OSAL_IOW(MSENSOR_TYPE_MNG,  IOC_NR_MSENSOR_MNG_SEND_DATA,   ot_msensor_data)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif
