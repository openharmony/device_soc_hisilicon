/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
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
#ifndef __RSA_PAD_PSS_H__
#define __RSA_PAD_PSS_H__

typedef struct {
    uint32_t hlen;
    uint32_t klen;
    uint32_t em_bit;
    uint8_t *in_data;
    uint32_t in_len;
} rsa_padding_s;

uint32_t rsa_get_bit_num(const uint8_t *big_num, uint32_t num_len);
int32_t rsa_padding_check_pkcs1_pss(rsa_padding_s *pad, const uint8_t *mhash);

#endif
