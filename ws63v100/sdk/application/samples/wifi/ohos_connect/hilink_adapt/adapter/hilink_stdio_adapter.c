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
 * Description: 系统适配层标准输出接口实现（此文件为DEMO，需集成方适配修改）
 */

#include "hilink_stdio_adapter.h"
#include <stddef.h>
#include <stdlib.h>
#include <los_printf.h>
#include <los_config.h>
#include <los_printf_pri.h>
#include "securec.h"
#include "hilink_sal_defines.h"

int HILINK_Vprintf(const char *format, va_list ap)
{
    ConsoleVprintf(format, ap); // 此接口无返回值
    return HILINK_SAL_OK;
}

int HILINK_Printf(const char *format, ...)
{
    if (format == NULL) {
        return HILINK_SAL_OK;
    }
    va_list ap;
    va_start(ap, format);
    int ret = HILINK_Vprintf(format, ap);
    va_end(ap);

    return ret;
}

int HILINK_Rand(unsigned char *input, unsigned int len)
{
    (void)input;
    (void)len;
    return HILINK_SAL_NOK;
}

int HILINK_Trng(unsigned char *input, unsigned int len)
{
    (void)input;
    (void)len;
    return HILINK_SAL_NOK;
}