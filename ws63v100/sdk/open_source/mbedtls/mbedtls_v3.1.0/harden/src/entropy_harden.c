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
 * Description: mbedtls harden adapt internal source file.
 *
 * Create: 2023-05-10
*/

#include "common.h"

#if defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_ENTROPY_HARDWARE_ALT)
#include "mbedtls/error.h"
#include "mbedtls/entropy.h"
#include "entropy_poll.h"
#include "cipher_adapt.h"
#include "dfx.h"

int mbedtls_hardware_poll( void *data, unsigned char *output, size_t len, size_t *olen )
{
    int ret;

    mbedtls_harden_log_func_enter();

    if( IS_TRNG_FUNC_REGISTERED() != TD_TRUE )
#if !defined(MBEDTLS_NO_PLATFORM_ENTROPY)
        return mbedtls_platform_entropy_poll( data, output, len, olen );
#else
        return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
#endif

    if( len == 0 || olen == NULL || output == NULL )
        return( MBEDTLS_ERR_ENTROPY_SOURCE_FAILED );

    (void) data;

    ret = CIPHER_TRNG_GET_MULTI_RANDOM( len, output );
    if( ret != MBEDTLS_HARDEN_SUCCESS )
    {
        mbedtls_harden_log_err("CIPHER_TRNG_GET_MULTI_RANDOM failed, ret = 0x%x!\n", ret );
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    }
    *olen = len;

    mbedtls_harden_log_func_exit();

    return( 0 );
}

#endif /* MBEDTLS_ENTROPY_C && MBEDTLS_ENTROPY_HARDWARE_ALT */