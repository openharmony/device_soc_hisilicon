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
 *
 * Description: security api for romboot. \n
 *
 */

#ifndef DRV_ROM_CIPHER_H
#define DRV_ROM_CIPHER_H

int drv_rom_cipher_init(void);
int drv_rom_cipher_deinit(void);

/* support data_length not 64 bytes aligned. */
int drv_rom_cipher_sha256(const unsigned char *data, unsigned int data_length, unsigned char *hash,
    unsigned int hash_length);

int drv_rom_cipher_get_randnum(unsigned int *randnum);

/* ecc verify. */
typedef struct {
    unsigned char *x;
    unsigned char *y;
    unsigned int length;
} drv_rom_cipher_ecc_point;

typedef struct {
    unsigned int length;
    unsigned char *data;
} drv_rom_cipher_data;

typedef struct {
    unsigned char  *r;
    unsigned char  *s;
    unsigned int length;
} drv_rom_cipher_ecc_sig;

int drv_rom_cipher_pke_bp256r_verify(const drv_rom_cipher_ecc_point *pub_key, const drv_rom_cipher_data *hash,
    const drv_rom_cipher_ecc_sig *sig);
#endif