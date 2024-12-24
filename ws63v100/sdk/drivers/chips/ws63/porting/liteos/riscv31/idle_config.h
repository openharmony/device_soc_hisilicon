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
 * Description: RISCV31 idle task config interface for LiteOS
 *
 * Create:  2021-10-20
 */
#ifndef IDLE_CONFIG_H
#define IDLE_CONFIG_H

/**
 * @brief  los idle task config
 */
void idle_task_config(void);
typedef void (*watchdog_port_idle_kick_callback)(void);
void watchdog_port_idle_kick_register(watchdog_port_idle_kick_callback callback);
#endif
