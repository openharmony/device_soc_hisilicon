/**
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
 *
 * Description: Provides version port source. \n
 *
 * History: \n
 * 2023-11-08， Create file. \n
 */

#include "version_porting.h"

char *plat_get_sw_version_str(void)
{
    return SDK_VERSION;
}

void print_version(void)
{
    osal_printk("SDK Version:%s\r\n", SDK_VERSION);
}
