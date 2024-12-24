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
 * Description: Provides at cmd service header. Only for AT module. \n
 */

#ifndef    AT_CMD_H
#define    AT_CMD_H

#include "at_product.h"

uint32_t at_cmd_get_entry_total(void);

uint32_t at_cmd_get_all_entrys(const at_cmd_entry_t **entrys, uint32_t cnt);

const at_cmd_entry_t* at_cmd_find_entry(char *str, uint16_t *offset);

const at_cmd_entry_t* at_cmd_find_entry_by_name(const char *name);

uint32_t at_cmd_get_max_struct_size(void);
#endif
