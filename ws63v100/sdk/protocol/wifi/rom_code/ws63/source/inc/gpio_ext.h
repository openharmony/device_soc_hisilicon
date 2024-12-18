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
 * Description: gpio ext header.
 * Create: 2023-01-11
 */

#ifndef __GPIO_EXT_H__
#define __GPIO_EXT_H__

#include "osal_types.h"

typedef struct {
    osal_u32 id;
    osal_u32 val;
} gpio_param_stru;

#endif /* __GPIO_EXT_H__ */