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
 * Description: OS Abstract Layer.
 */


unsigned int osal_irq_lock(void)
{
    non_os_enter_critical();
    return 0;
}


void osal_irq_restore(unsigned int irq)
{
    non_os_exit_critical();
    return;
}

int osal_in_interrupt(void)
{
    return 0;
}
