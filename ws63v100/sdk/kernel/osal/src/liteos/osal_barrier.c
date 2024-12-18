/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: barrier
 *
 * Create: 2021-12-16
 */

#include <asm/barrier.h>
#include <los_config.h>

void osal_mb(void)
{
}
void osal_rmb(void)
{
}
void osal_wmb(void)
{
}
void osal_smp_mb(void)
{
}
void osal_smp_rmb(void)
{
}
void osal_smp_wmb(void)
{
}
void osal_isb(void)
{
#ifdef HW_LITEOS_OPEN_VERSION_NUM
    ISB();
#else
    Isb();
#endif
}
void osal_dsb(void)
{
#ifdef HW_LITEOS_OPEN_VERSION_NUM
    dsb();
#else
    Dsb();
#endif
}
void osal_dmb(void)
{
#ifdef HW_LITEOS_OPEN_VERSION_NUM
    dmb();
#else
    Dmb();
#endif
}
