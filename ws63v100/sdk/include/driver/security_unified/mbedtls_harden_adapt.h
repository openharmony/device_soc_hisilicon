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
 * Description: The header file of the functions, which will be registered to mbedtls.
 *
 * Create: 2023-07-12
*/

#ifndef CRYPTO_MBEDTLS_HARDEN_ADAPT_H
#define CRYPTO_MBEDTLS_HARDEN_ADAPT_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup security_unified_mbedtls_harden_adapt mbedtls_harden_adapt
 * @ingroup  drivers_driver_security_unified
 * @{
 */

/**
 * @if Eng
 * @brief  mbedtls harden adapt functions register, called by system mbedtls harden.
 * @else
 * @brief  security_unified 模块对接第三方 mbedtls 接口需要注册的函数，若开启第三方对接宏 MBEDTLS_HARDEN_OPEN，
 * 该接口由对接适配层调用。
 * @endif
 */
int32_t mbedtls_adapt_register_func(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* CRYPTO_MBEDTLS_HARDEN_ADAPT_H */