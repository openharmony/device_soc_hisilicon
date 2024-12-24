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
 * Description: patch section header.
 *
 * Create: 2023-04-13
 */
#ifndef __PATCH_SECTION_H__
#define __PATCH_SECTION_H__

#define PATCH_T_I __attribute__((section(".patch.text.on.itcm")))
#define PATCH_R_I __attribute__((section(".patch.rodata.on.itcm")))
#define PATCH_D_I __attribute__((section(".patch.data.on.itcm")))
#define PATCH_B_I __attribute__((section(".patch.bss.on.itcm")))

#define PATCH_T_S __attribute__((section(".patch.text.on.sram")))
#define PATCH_R_S __attribute__((section(".patch.rodata.on.sram")))
#define PATCH_D_S __attribute__((section(".patch.data.on.sram")))
#define PATCH_B_S __attribute__((section(".patch.bss.on.sram")))

#define PATCH_T_F __attribute__((section(".patch.text.on.flash")))
#define PATCH_R_F __attribute__((section(".patch.rodata.on.flash")))
#define PATCH_D_F __attribute__((section(".patch.data.on.flash")))
#define PATCH_B_F __attribute__((section(".patch.bss.on.flash")))

#endif
