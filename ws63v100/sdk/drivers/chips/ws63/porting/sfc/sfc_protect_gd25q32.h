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
 * Description: Provides sfc port template \n
 *
 * History: \n
 * 2022-11-30， Create file. \n
 */
#ifndef SFC_PROTECT_GD25Q32_H
#define SFC_PROTECT_GD25Q32_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SR1_VALID_MASK  0x9C
#define SR1_VALID_VAL   0x1C

#define SR2_VALID_MASK  0x43
#define SR2_VALID_VAL   0x02

errcode_t sfc_port_fix_sr_gd25q32(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
