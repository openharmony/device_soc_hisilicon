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
#ifndef __TZASC_H__
#define __TZASC_H__

#define TZASC_ATTR_SEC_R    (0x5 << 28)
#define TZASC_ATTR_SEC_W    (0x5 << 24)
#define TZASC_ATTR_NOSEC_R  (0x5 << 20)
#define TZASC_ATTR_NOSEC_W  (0x5 << 16)
#define TZASC_ATTR_MID_INV  (1 << 9)
#define TZASC_ATTR_MID_EN   (1 << 8)
#define TZASC_ATTR_SEC_INV  (1 << 4)

#define TZASC_ALGIN_BITS	12 // 4k alignment

void tzasc_bypass_disable();
void tzasc_set_rgn_attr(uint8_t rgn, uint32_t attr);
// Notice: addr and size of function "tzasc_set_rgn_map" must align to 4K
void tzasc_set_rgn_map(uint8_t rgn, uint64_t addr, size_t size);
void tzasc_rgn_enable(uint8_t rgn);

#endif /* __TZASC_H__ */
