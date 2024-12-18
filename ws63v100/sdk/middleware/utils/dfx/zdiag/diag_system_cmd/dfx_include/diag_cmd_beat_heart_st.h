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
 * Description: dfx beat heart st
 * This file should be changed only infrequently and with great care.
 */

#ifndef __DIAG_CMD_BEAT_HEART_ST_H__
#define __DIAG_CMD_BEAT_HEART_ST_H__

#include <stdint.h>

typedef struct {
    uint32_t dir;
    uint32_t random_data;
} diag_beat_heart_cmd_ind_t;

#define DIAG_HEART_BEAT_DIR_UP (0x1)
#define DIAG_HEART_BEAT_DIR_DOWN (0x2)
#endif