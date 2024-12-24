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
 * limitations under the License. \n
 *
 * Description: Provides at channel service header. Only for AT module. \n
 */

#ifndef    AT_CHANNEL_H
#define    AT_CHANNEL_H

#include "at_product.h"

typedef enum {
    AT_CHANNEL_UNINITIALIZED = 0x0,
    AT_CHANNEL_FREE,
    AT_CHANNEL_INTERACTIVITY,
    AT_CHANNEL_BLOCK
} at_channel_state_t;

at_write_func_t at_channel_get_write_func(at_channel_id_t id);

void at_channel_check_and_enable(void);

uint8_t* at_channel_get_data(at_channel_id_t id);

void at_channel_data_reset(at_channel_id_t id);

void at_channel_set_state(at_channel_id_t id, at_channel_state_t state);

void at_channel_data_wait_interactivity(at_channel_id_t id);

#endif
