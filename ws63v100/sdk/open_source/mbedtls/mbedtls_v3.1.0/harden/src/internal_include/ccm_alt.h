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
 * Description: mbedtls harden alg alt internal source file.
 *
 * Create: 2023-05-10
*/
#ifndef CCM_ALT_H
#define CCM_ALT_H
/**
 * \brief    The CCM context-type alt definition. The CCM context is passed
 *           to the APIs called.
 */
typedef struct mbedtls_ccm_context
{
    mbedtls_cipher_context_t MBEDTLS_PRIVATE(cipher_ctx);    /*!< The cipher context used. */
}
mbedtls_ccm_context;

#endif