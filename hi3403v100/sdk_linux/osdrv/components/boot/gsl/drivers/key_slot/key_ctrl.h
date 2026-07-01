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
#ifndef __KEY_CTRL_KS_H__
#define __KEY_CTRL_KS_H__
#define KC_MC_SLOT_NUM 16
#define KC_TS_SLOT_NUM 256

typedef enum {
    KC_SLOT_MC               = 0x0,
    KC_SLOT_TS               = 0x1,
    KC_SLOT_MAX
} kc_slot_ind_e;

int32_t kc_slot_unlock(const kc_slot_ind_e slot_ind, const uint32_t slot_num);
int32_t kc_slot_auto_lock(const kc_slot_ind_e slot_ind, uint32_t *slot_num);
int32_t kc_slot_is_lock(const kc_slot_ind_e slot_ind, const uint32_t slot_num);

#endif /* __KEY_CTRL_KS_H__ */
