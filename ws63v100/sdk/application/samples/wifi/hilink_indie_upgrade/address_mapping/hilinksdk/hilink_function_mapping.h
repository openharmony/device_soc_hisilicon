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
 * Description: hilink function mapping. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#ifndef HILINK_FUNCTION_MAPPING_H
#define HILINK_FUNCTION_MAPPING_H

#ifdef __cplusplus
extern "C" {
#endif

int *get_app_tbl(void);

extern unsigned int __bss_begin__;
extern unsigned int __bss_end__;
extern unsigned int __data_begin__;
extern unsigned int __data_load__;
extern unsigned int __data_size__;

extern unsigned int __sram_text_begin__;
extern unsigned int __sram_text_load__;
extern unsigned int __sram_text_size__;

#ifdef __cplusplus
}
#endif
#endif
