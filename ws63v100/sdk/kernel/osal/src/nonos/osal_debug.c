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

static unsigned char g_bug_on_flag = 0;
void osal_bug_on(unsigned char condition)
{
    if (condition != 0 && g_bug_on_flag == 0) {
        g_bug_on_flag = 1;
        *(volatile unsigned long*)0 = 0; // Invoke a system instruction to trigger an exception.
    }
}
