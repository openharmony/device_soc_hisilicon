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
 * Description: mbedtls harden adapt internal header file.
 *
 * Create: 2023-05-10
*/

#ifndef MBEDTLS_PLATFORM_HARDWARE_CONFIG_H
#define MBEDTLS_PLATFORM_HARDWARE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define MBEDTLS_HARDEN_SUCCESS           TD_SUCCESS
#define MBEDTLS_HARDEN_TRUE              TD_TRUE
#define MBEDTLS_HARDEN_FALSE             TD_FALSE

#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_HARDWARE_ALT

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_PLATFORM_HARDWARE_CONFIG_H */