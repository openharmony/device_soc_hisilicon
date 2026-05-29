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
#ifndef CHECKUP_H
#define CHECKUP_H

void failure_process(void);

uint32_t set_key_slot(uint32_t slot);
uint32_t unset_key_slot(void);

uint32_t get_gsl_code_area_len();
uint32_t get_boot_image_int_ddr_addr();
uint32_t get_boot_image_total_len();
uint32_t get_bootloader_unchecked_area_len();
uint32_t get_ddr_decrypt_bootloader_addr();
int handle_bootloader_key_area(void);
int handle_bootloader_params_area(void);
int handle_bootloader_code_area(void);

void disable_sec_ddr_bypass(void);
void config_tee_img_non_sec_ddr(void);
void config_key_area_sec_ddr(void);
void config_tee_img_sec_ddr(void);
void config_non_sec_ddr(void);

int handle_tee_key_area(void);
int handle_atf_area(void);
int handle_tee_code_area(void);

#endif /* CHECKUP_H */
