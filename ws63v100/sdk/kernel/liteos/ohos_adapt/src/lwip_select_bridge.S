/*
 * Copyright (c) 2026 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: select() -> lwip_select() link-time bridge for ws63 XTS firmware.
 */

    .section .text
    .globl __wrap_select
    .type __wrap_select, @function
__wrap_select:
    li      t0, 256                 /* WS63_LWIP_SOCKET_OFFSET */
    ble     a0, t0, 1f
    tail    lwip_select
1:
    tail    __real_select
    .size __wrap_select, .-__wrap_select
