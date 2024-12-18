/**
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
 *
 * Description: Provides version port header. \n
 *
 * History: \n
 * 2023-11-08， Create file. \n
 */

#ifndef VERSION_PORTING_H
#define VERSION_PORTING_H

#include <stdint.h>
#include "soc_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct {
    uint32_t c : 8;
    uint32_t r : 8;
    uint32_t v : 16;
} plat_version_t;

typedef struct {
    uint32_t c : 8;
    uint32_t r : 8;
    uint32_t v : 8;
    uint32_t b : 8;
} plat_version_mod_t;

char *plat_get_sw_version_str(void);

void print_version(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif

