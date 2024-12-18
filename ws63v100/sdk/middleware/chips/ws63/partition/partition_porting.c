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
 * Description: Partition manage module -- seperate configuration for each product.
 */

#include <stdint.h>
#include <stddef.h>
#include "partition.h"
#include "partition_porting.h"

#define PARTITION_START_ADDRESS 0x200380
#define uapi_array_size(_array)  (sizeof(_array) / sizeof((_array)[0]))

static partition_path_map_t g_image_path[] = {};
uintptr_t partition_get_addr(void)
{
    return PARTITION_START_ADDRESS;
}

uint32_t partition_get_path_map(partition_path_map_t **path_map)
{
    *path_map = g_image_path;
    return uapi_array_size(g_image_path);
}
