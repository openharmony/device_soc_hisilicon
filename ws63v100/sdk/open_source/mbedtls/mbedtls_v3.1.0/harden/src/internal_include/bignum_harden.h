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

#ifndef MBEDTLS_BIGNUM_HARDEN_H
#define MBEDTLS_BIGNUM_HARDEN_H

#include "mbedtls/bignum.h"

int check_exp_mod_harden_can_do( const mbedtls_mpi *A,
        const mbedtls_mpi *E, const mbedtls_mpi *N );

int exp_mod_harden( mbedtls_mpi *X, const mbedtls_mpi *A,
        const mbedtls_mpi *E, const mbedtls_mpi *N);

int check_mod_harden_can_do( const mbedtls_mpi *A, const mbedtls_mpi *B );

int mod_harden( mbedtls_mpi *R, const mbedtls_mpi *A, const mbedtls_mpi *B );

#endif /* MBEDTLS_BIGNUM_HARDEN_H */