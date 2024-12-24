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
 * Description: Implementation of the character string interface at the system adaptation layer. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "app_call.h"

unsigned int HILINK_Strlen(const char *src)
{
    return app_call1(APP_CALL_HILINK_STRLEN, unsigned int, const char *, src);
}

char *HILINK_Strchr(const char *str, int ch)
{
    return app_call2(APP_CALL_HILINK_STRCHR, char *, const char *, str, int, ch);
}

char *HILINK_Strrchr(const char *str, int ch)
{
    return app_call2(APP_CALL_HILINK_STRRCHR, char *, const char *, str, int, ch);
}

int HILINK_Atoi(const char *str)
{
    return app_call1(APP_CALL_HILINK_ATOI, int, const char *, str);
}

char *HILINK_Strstr(const char *str1, const char *str2)
{
    return app_call2(APP_CALL_HILINK_STRSTR, char *, const char *, str1, const char *, str2);
}

int HILINK_Strcmp(const char *str1, const char *str2)
{
    return app_call2(APP_CALL_HILINK_STRCMP, int, const char *, str1, const char *, str2);
}

int HILINK_Strncmp(const char *str1, const char *str2, unsigned int len)
{
    return app_call3(APP_CALL_HILINK_STRNCMP, int, const char *, str1, const char *, str2, unsigned int, len);
}