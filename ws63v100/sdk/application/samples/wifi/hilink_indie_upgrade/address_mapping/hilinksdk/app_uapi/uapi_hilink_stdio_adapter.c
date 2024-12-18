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
 * Description: Implementation of the standard output interface of the system adaptation layer. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include <stddef.h>
#include <stdarg.h>
#include "app_call.h"

int HILINK_Vprintf(const char *format, va_list ap)
{
    return app_call2(APP_CALL_HILINK_VPRINTF, int, const char *, format, va_list, ap);
}

int HILINK_Printf(const char *format, ...)
{
    if (format == NULL) {
        return 0;
    }
    va_list ap;
    va_start(ap, format);
    int ret = HILINK_Vprintf(format, ap);
    va_end(ap);

    return ret;
}

int HILINK_Rand(unsigned char *input, unsigned int len)
{
    return app_call2(APP_CALL_HILINK_RAND, int, unsigned char *, input, unsigned int, len);
}

int HILINK_Trng(unsigned char *input, unsigned int len)
{
    return app_call2(APP_CALL_HILINK_TRNG, int, unsigned char *, input, unsigned int, len);
}