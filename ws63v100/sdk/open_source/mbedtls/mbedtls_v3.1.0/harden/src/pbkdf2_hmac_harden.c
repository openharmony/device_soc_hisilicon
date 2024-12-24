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

#include "pbkdf2_hmac_harden.h"
#include "cipher_adapt.h"
#include "mbedtls/error.h"
#include "hash_harden_common.h"
#include "mbedtls/platform_util.h"
#include "dfx.h"

#if defined(MBEDTLS_PBKDF2_HMAC_ALT)

int pbkdf2_hmac_harden_can_do( mbedtls_md_context_t *ctx, unsigned int iteration_count )
{
    crypto_hash_type hmac_type = CRYPTO_HASH_TYPE_INVALID;

    if( IS_PBKDF2_FUNC_REGISTERED() != TD_TRUE || ctx == NULL || ctx->MBEDTLS_PRIVATE(md_info) == NULL )
        return ( 0 );

    if( iteration_count > 0xFFFF )
        return 0;

    get_hash_info( ctx->MBEDTLS_PRIVATE(md_info)->type, &hmac_type, NULL, NULL );

    return( hmac_type != CRYPTO_HASH_TYPE_INVALID );
}

int pbkdf2_hmac_harden( mbedtls_md_context_t *ctx,
        const unsigned char *password,
        size_t plen, const unsigned char *salt, size_t slen,
        unsigned int iteration_count,
        uint32_t key_length, unsigned char *output )
{
    int ret;
    crypto_kdf_pbkdf2_param kdf;
    crypto_hash_type hmac_type = CRYPTO_HASH_TYPE_INVALID;

    mbedtls_harden_log_func_enter();
    if( ctx == NULL || ctx->MBEDTLS_PRIVATE(md_info) == NULL )
        return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );

    get_hash_info( ctx->MBEDTLS_PRIVATE(md_info)->type, &hmac_type, NULL, NULL );

    kdf.count = iteration_count;
    kdf.password = (unsigned char *)password;
    kdf.plen = plen;
    kdf.salt = (unsigned char *)salt;
    kdf.slen = slen;
    kdf.hash_type = hmac_type;

    ret = CIPHER_PBKDF2( &kdf, output, key_length );
    if( ret != MBEDTLS_HARDEN_SUCCESS )
    {
        mbedtls_harden_log_err("CIPHER_PBKDF2 failed, ret = 0x%x!\n", ret );
        mbedtls_platform_zeroize( &kdf, sizeof( crypto_kdf_pbkdf2_param ) );
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    }
    mbedtls_platform_zeroize( &kdf, sizeof( crypto_kdf_pbkdf2_param ) );

    mbedtls_harden_log_func_exit();

    return( 0 );
}

int mbedtls_pkcs5_pbkdf2_hmac( mbedtls_md_context_t *ctx,
                       const unsigned char *password,
                       size_t plen, const unsigned char *salt, size_t slen,
                       unsigned int iteration_count,
                       uint32_t key_length, unsigned char *output )
{
    if( pbkdf2_hmac_harden_can_do( ctx, iteration_count ) )
    {
        return pbkdf2_hmac_harden( ctx, password, plen, salt, slen, iteration_count, key_length, output );
    }

    return( -1 );
}
#endif /* MBEDTLS_PBKDF2_HMAC_ALT */
