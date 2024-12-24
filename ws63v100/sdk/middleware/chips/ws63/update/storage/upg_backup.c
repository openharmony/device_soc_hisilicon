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
 * Description: UPG backup image functions for different chip
 */

#include <stddef.h>
#include <stdint.h>
#include "errcode.h"
#include "securec.h"
#include "common_def.h"
#include "partition.h"
#include "main.h"

#include "upg_definitions.h"
#include "upg_porting.h"
#include "upg_alloc.h"
#include "upg_debug.h"
#include "upg_common_porting.h"

errcode_t upg_image_backups_update(void)
{
    return ERRCODE_SUCC;
}
