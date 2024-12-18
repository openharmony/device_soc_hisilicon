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
 * Description: wal common msg api.
 */
 
#ifndef __MAC_ADDR_H__
#define __MAC_ADDR_H__
 
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IFTYPE_BLE 0xF1
#define IFTYPE_SLE 0xF2
typedef uint32_t(*mac_derivation_ptr)(uint8_t* origin_mac, uint8_t num, uint8_t type,
    uint8_t *output_mac, uint8_t out_put_num);
void set_mac_derivation_ptr(mac_derivation_ptr ptr);
void init_dev_addr(void);
uint32_t get_dev_addr(uint8_t *pc_addr, uint8_t addr_len, uint8_t type);
uint32_t set_dev_addr(const uint8_t *pc_addr, uint8_t mac_len, uint8_t type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
 
#endif
