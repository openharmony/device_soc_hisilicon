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
 * Description: hash romable functions header file.
 *
 * Create: 2024-01-25
*/

#ifndef HASH_ROMABLE_H
#define HASH_ROMABLE_H

#include "crypto_hash_struct.h"

int drv_rom_hash(crypto_hash_type hash_type, const unsigned char *data, unsigned int data_length,
    unsigned char *hash, unsigned int hash_length, unsigned int hash_chn);

#endif  /* HASH_ROMABLE_H */
