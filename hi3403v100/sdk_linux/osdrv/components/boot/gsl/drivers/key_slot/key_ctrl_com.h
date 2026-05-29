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

#ifndef __COMMON_KS_H__
#define __COMMON_KS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define KC_BASE                 0x10112000

#define KC_TEE_LOCK_CMD         0xB00
#define KC_REE_LOCK_CMD         0xB04
#define KC_TEE_FLUSH_BUSY       0xB10
#define KC_REE_FLUSH_BUSY       0xB14
#define KC_SEND_DBG             0xB20
#define KC_ROB_ALARM            0xB24
#define KC_RD_SLOT_NUM          0xB30
#define KC_RD_LOCK_STATUS       0xB34

typedef enum {
    KC_SLOT_STAT_UN_LOCK     = 0x0,
    KC_SLOT_STAT_REE_LOCK    = 0x1,
    KC_SLOT_STAT_TEE_LOCK    = 0x2,
    KC_SLOT_STAT_MAX
} kc_slot_stat_e;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __common_ks_h__ */

