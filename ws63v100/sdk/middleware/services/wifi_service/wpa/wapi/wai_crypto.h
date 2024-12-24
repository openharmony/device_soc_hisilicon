/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: WAPI Implementation
 */

#ifndef WAI_CRYPTO_H
#define WAI_CRYPTO_H

int wai_hmac_sha256(const unsigned char *data, unsigned int datalen,
    const unsigned char *key, unsigned int keylen, unsigned char *output, unsigned int outlen);
int wai_kd_hmac_sha256(const unsigned char *data, unsigned int datalen,
    const unsigned char *key, unsigned int keylen, unsigned char *output, unsigned int outlen);
int wai_sm4_ofb_decrypt(const unsigned char *iv, unsigned int ivlen,
    const unsigned char *key, unsigned int keylen, const unsigned char *input, unsigned int inlen,
    unsigned char *output, unsigned int *outlen);

#endif /* end of WAI_CRYPTO_H */
