/*
 * Copyright (c) 2026 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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

#ifndef HUKS_HDI_CIPHER_ADAPTER_H
#define HUKS_HDI_CIPHER_ADAPTER_H
#include "hdf_log.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @brief Generate random numbers and fill them into the specified buffer
 *
 * @param data Indicates the pointer to the data to fill.
 * @param size size.
 *
 * @return Returns <b>0</b> if the operation is successful; returns an error code
 * otherwise.
 */
int cipher_get_random(uint8_t *data, uint32_t size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // HUKS_HDI_CIPHER_ADAPTER_H