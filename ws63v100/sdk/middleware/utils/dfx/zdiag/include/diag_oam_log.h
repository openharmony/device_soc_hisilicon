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
 * Description: diag oam log
 * This file should be changed only infrequently and with great care.
 */
#ifndef __DIAG_OAM_LOG_H__
#define __DIAG_OAM_LOG_H__

#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
} zdiag_log_msg0_t;
typedef struct {
    uint32_t data0;
} zdiag_log_msg1_t;

typedef struct {
    uint32_t data0;
    uint32_t data1;
} zdiag_log_msg2_t;

typedef struct {
    uint32_t data0;
    uint32_t data1;
    uint32_t data2;
} zdiag_log_msg3_t;

typedef struct {
    uint32_t data0;
    uint32_t data1;
    uint32_t data2;
    uint32_t data3;
} zdiag_log_msg4_t;

errcode_t oam_log_print0_press_prv(uint32_t msg_id, uint32_t mod_id);
errcode_t oam_log_print1_press_prv(uint32_t msg_id, uint32_t mod_id, uint32_t param_1);
errcode_t oam_log_print2_press_prv(uint32_t msg_id, uint32_t mod_id, uint32_t param_1, uint32_t param_2);
errcode_t oam_log_print3_press_prv(uint32_t msg_id, uint32_t mod_id, zdiag_log_msg3_t *olm);
errcode_t oam_log_print4_press_prv(uint32_t msg_id, uint32_t mod_id, zdiag_log_msg4_t *olm);
errcode_t oam_log_print_alterable_press_prv(uint32_t msg_id, uint32_t mod_id, uint32_t param_num, ...);
errcode_t oam_log_print_buff_press_prv(uint32_t msg_id, uint32_t mod_id, uint8_t *data, uint32_t data_size);

errcode_t oam_log_print0_press(uint32_t msg_id);
errcode_t oam_log_print1_press(uint32_t msg_id, uint32_t param_1);
errcode_t oam_log_print2_press(uint32_t msg_id, uint32_t param_1, uint32_t param_2);
errcode_t oam_log_print3_press(uint32_t msg_id, zdiag_log_msg3_t *olm);
errcode_t oam_log_print4_press(uint32_t msg_id, zdiag_log_msg4_t *olm);
errcode_t oam_log_print_alterable_press(uint32_t msg_id, uint32_t param_num, ...);
errcode_t oam_log_print_buff_press(uint32_t msg_id, uint8_t *data, uint32_t data_size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
