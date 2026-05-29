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

#ifndef MOTIONSENSOR_CHIP_CMD_H
#define MOTIONSENSOR_CHIP_CMD_H

#include "osal_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define MSENSOR_TYPE_CHIP   12

typedef enum {
    IOC_NR_MSENSOR_CHIP_START = 0,
    IOC_NR_MSENSOR_CHIP_STOP,
    IOC_NR_MSENSOR_CHIP_INIT,
    IOC_NR_MSENSOR_CHIP_DEINIT,
    IOC_NR_MSENSOR_CHIP_GET_PARAM,
    IOC_NR_MSENSOR_CHIP_MNG_INIT,
    IOC_NR_MSENSOR_CHIP_BUTT
} ioc_nr_msensor_chip;

#define MSENSOR_CMD_START     OSAL_IO(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_START)
#define MSENSOR_CMD_STOP      OSAL_IO(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_STOP)
#define MSENSOR_CMD_INIT      OSAL_IOW(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_INIT, ot_msensor_param)
#define MSENSOR_CMD_DEINIT    OSAL_IO(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_DEINIT)
#define MSENSOR_CMD_GET_PARAM OSAL_IOR(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_GET_PARAM, ot_msensor_param)
#define MSENSOR_CMD_MNG_INIT  OSAL_IOW(MSENSOR_TYPE_CHIP, IOC_NR_MSENSOR_CHIP_MNG_INIT, ot_msensor_param)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif
