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
 */
#ifndef _LINUX_CRC32_H
#define _LINUX_CRC32_H

#include "los_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* import definition only, do not directly call it */
extern unsigned int Crc32Part(unsigned int crc32val, const char *src, size_t len);

/**
 * @ingroup  crc32
 * @brief Accumulate the value of crc32.
 *
 * @par Description:
 * This API is used to accumulate the value of crc32.
 *
 * @attention
 * <ul>
 * <li>please make sure the parameter s is valid, the parameter len is the length of parameter s,
 * s is not NULL, otherwise the system maybe crash!</li>
 * </ul>
 *
 * @param  val [IN/OUT] Type #unsigned int  the seed of crc32 calculation.
 * @param  s   [IN] Type #unsigned char  the data needs to crc32 calculate..
 * @param  len [IN] Type #int  the length of the data needs to crc32 calculate..
 *
 * @retval unsigned int return the crc value.
 * @par Dependency:
 * <ul><li>Crc32.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
#define crc32(val, s, len) Crc32Part(val, (const char *)s, len)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
