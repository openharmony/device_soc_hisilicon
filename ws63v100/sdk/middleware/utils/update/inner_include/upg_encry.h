/*
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
 * Description: UPG encry Interface Header.
 */
#ifndef UPG_ENCRY_H
#define UPG_ENCRY_H

#ifdef CONFIG_MIDDLEWARE_SUPPORT_UPG_COMPRESS_ENCRY
#include "td_type.h"
#include "upg.h"
#include "upg_lzmadec.h"
#include "upg_encry_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

uint32_t upg_encry_and_write_pkt(upg_lzma_decode2_data_t *data, const upg_image_header_t *image, td_bool *first_pkt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* UPG_LZMADEC_H */
#endif
