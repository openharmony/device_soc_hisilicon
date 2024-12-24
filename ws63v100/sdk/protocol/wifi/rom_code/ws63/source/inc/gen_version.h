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
* Description: gen VERSION
* Create: 2023-7-5
*/

#ifndef __GEN_VERSION_H_
#define __GEN_VERSION_H_

#ifdef WS53_PRODUCT_FPGA
#define HOST_VER_V 1
#define HOST_VER_R 1
#define HOST_VER_C 10
#define HOST_VER_S 10
#define HOST_VER_B 14
#define DEVICE_VER_V 1
#define DEVICE_VER_R 1
#define DEVICE_VER_C 10
#define DEVICE_VER_S 10
#define DEVICE_VER_B 20
#else
#define HOST_VER_V 1
#define HOST_VER_R 1
#define HOST_VER_C 10
#define HOST_VER_S 10
#define HOST_VER_B 14
#define DEVICE_VER_V 1
#define DEVICE_VER_R 1
#define DEVICE_VER_C 10
#define DEVICE_VER_S 10
#define DEVICE_VER_B 14
#endif

#endif
