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
 * Description: UPG alloc memory functions header file
 */

#ifndef UPG_ALLOC_H
#define UPG_ALLOC_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * 内存申请接口
 * size：申请内存的大小
 */
void *upg_malloc(uint32_t size);

/*
 * 内存释放接口
 * addr：待释放的内存地址
 */
void upg_free(void *addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* UPG_ALLOC_H */