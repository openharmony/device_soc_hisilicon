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
 * Description: RISCV31 print config interface for LiteOS
 *
 * Create:  2021-10-20
 */
#ifndef PRINT_CONFIG_H
#define PRINT_CONFIG_H

#include "debug_print.h"

#define print_liteos(fmt, args...)  PRINT(fmt, ##args)

#define oam_trace_print(fmt, args...)  PRINT(fmt, ##args)

#endif