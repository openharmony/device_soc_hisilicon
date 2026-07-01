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
#ifndef __OTP_DRV_H__
#define __OTP_DRV_H__

uint32_t is_tee_enable(void);
uint32_t get_tee_owner(void);
uint32_t get_oem_msid();
uint32_t get_oem_version();
uint32_t is_backup_image_enable(void);
uint32_t is_bload_dec_en_enable(void);
uint32_t is_secure_boot_en_enable(void);
uint32_t is_double_sign_en_enable(void);
uint32_t is_scs_dbg_enable(void);
uint32_t is_boot_info_lv_debug_enable(void);
uint32_t is_func_jtag_enable(void);
uint32_t get_tee_msid(void);
uint32_t get_tee_sec_version(uint32_t mask_ext);

#endif /* __OTP_DRV_H__ */
