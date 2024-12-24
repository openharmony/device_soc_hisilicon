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
 * Description: mocked shell
 */

#ifndef DIAG_MOCKED_SHELL_H
#define DIAG_MOCKED_SHELL_H

#include "errcode.h"
#include "stdarg.h"
#include "diag.h"

typedef void (*diag_cmd_shell_data_proc)(uint8_t *data, uint32_t data_len);
typedef void (*zdiag_data_put_uart)(uint8_t *data, uint16_t data_len);

int zdiag_mocked_shell_print(const char *fmt, va_list ap);
int zdiag_mocked_shell_uart_puts(uint8_t *data, uint32_t data_len);
void zdiag_mocked_shell_register_directly_uartput(zdiag_data_put_uart uart_hook);
void zdiag_mocked_shell_init(void);
void zdiag_mocked_shell_register_cmd_data_proc(diag_cmd_shell_data_proc data_proc_hook);

void zdiag_mocked_shell_set_enable(bool enable);
bool zdiag_mocked_shell_is_enabled(void);

errcode_t zdiag_cmd_mocked_shell(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
errcode_t zdiag_cmd_mocked_shell_enable(uint16_t cmd_id, void *cmd_param,
                                        uint16_t cmd_param_size, diag_option_t *option);
#endif /* ZDIAG_MOCKED_SHELL_H */

