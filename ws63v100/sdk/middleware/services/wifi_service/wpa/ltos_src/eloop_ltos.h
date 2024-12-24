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
 * Description: eloop API
 */


#ifndef ELOOP_LTOS_H
#define ELOOP_LTOS_H

#include "eloop.h"

#define ELOOP_ALREADY_RUNNING          1
#define ELOOP_NOT_RUNNING              0
#define ELOOP_MAX_EVENTS               16

int eloop_terminate_wpa_process(void);
void eloop_run_process_normal_event(unsigned int *ret_flags, struct eloop_data *eloop);
int eloop_start_running(eloop_task_type eloop_task);
int eloop_run_check(eloop_task_type eloop_task);
int global_eloop_is_running(void);
#endif /* ELOOP_LTOS_H */
