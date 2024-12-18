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
 * Description: 系统适配层字符串接口实现（此文件为DEMO，需集成方适配修改）
 */
#include "hilink_str_adapter.h"
#include <stdlib.h>
#include <string.h>

unsigned int HILINK_Strlen(const char *src)
{
    if (src == NULL) {
        return 0;
    }
    return (unsigned int)strlen(src);
}

char *HILINK_Strchr(const char *str, int ch)
{
    if (str == NULL) {
        return NULL;
    }

    return strchr(str, ch);
}

char *HILINK_Strrchr(const char *str, int ch)
{
    if (str == NULL) {
        return NULL;
    }

    return strrchr(str, ch);
}

int HILINK_Atoi(const char *str)
{
    if (str == NULL) {
        return 0;
    }
    return atoi(str);
}

char *HILINK_Strstr(const char *str1, const char *str2)
{
    if ((str1 == NULL) || (str2 == NULL)) {
        return NULL;
    }
    return strstr(str1, str2);
}

int HILINK_Strcmp(const char *str1, const char *str2)
{
    if ((str1 == NULL) && (str2 == NULL)) {
        return 0;
    }
    if ((str1 != NULL) && (str2 == NULL)) {
        return 1;
    }
    if ((str1 == NULL) && (str2 != NULL)) {
        return -1;
    }

    return strcmp(str1, str2);
}

int HILINK_Strncmp(const char *str1, const char *str2, unsigned int len)
{
    if ((str1 == NULL) && (str2 == NULL)) {
        return 0;
    }
    if ((str1 != NULL) && (str2 == NULL)) {
        return 1;
    }
    if ((str1 == NULL) && (str2 != NULL)) {
        return -1;
    }

    return strncmp(str1, str2, len);
}