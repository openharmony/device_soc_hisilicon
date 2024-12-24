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
 * Description: reset
 *
 * Create: 2023-01-09
 */

#include "boot_reset.h"
#include "boot_def.h"
#include "boot_init.h"
#include "non_os.h"

#ifdef SUPPORT_GPIO
#include "hal_gpio.h"
#endif
void reset(void)
{
    update_reset_count();
    uapi_reg_write16(BOOT_PORTING_RESET_REG, BOOT_PORTING_RESET_VALUE);
    for (;;) { }
}

void boot_fail(void)
{
    /* wait for it... */
    for (;;) { }
}