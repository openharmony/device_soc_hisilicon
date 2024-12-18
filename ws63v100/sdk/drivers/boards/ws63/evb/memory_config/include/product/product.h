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
 * Description:  product config
 *
 * Create:  2020-10-23
 */
#ifndef PRODUCT_H
#define PRODUCT_H
#include "product_fpga_standard.h"

#ifndef YES
#define YES (1)
#endif

#ifndef NO
#define NO (0)
#endif

#define DUAL_DSP_EXIST                      NO

#define BTH_MOUSE_MANAGER_SUPPORT               YES
#define IPC_SHARE_NUMS                          2
#define APPLICATION_VERSION_STRING "B040"
#endif
