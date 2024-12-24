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
 * Description:  dyn mem header file.
 *
 * Create:  2023-04-09
 */

#ifndef DYN_MEM_H
#define DYN_MEM_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* share mem ctl sel */
#define SHARE_RAM_CTL_CFG_RAM_SEL 0x44006c08
#define CFG_RAM_CKEN 0x44006c04
#define BT_EM_GT_MODE 0x49003074
#define RAM_SW_CONFLICT_ST 0x44006C10
#define RAMX_SW_CONFLICT_BASE_ADDR 0x44006C14
#define RAM_SW_CONFLICT_CLR 0x44006C0C

void dyn_mem_cfg(void);

#ifdef __cplusplus
#if __cplusplus
    }
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif