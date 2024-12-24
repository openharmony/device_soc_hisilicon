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
 * Description: 系统适配层内存接口实现源文件（此文件为DEMO，需集成方适配修改）
 */
#include "hilink_mem_adapter.h"
#include <string.h>
#include <stdlib.h>

void *HILINK_Malloc(unsigned int size)
{
    if (size == 0) {
        return NULL;
    }

    return malloc(size);
}

void HILINK_Free(void *pt)
{
    if (pt == NULL) {
        return;
    }
    free(pt);
}

int HILINK_Memcmp(const void *buf1, const void *buf2, unsigned int len)
{
    if ((buf1 == NULL) && (buf2 == NULL)) {
        return 0;
    }
    if ((buf1 != NULL) && (buf2 == NULL)) {
        return 1;
    }
    if ((buf1 == NULL) && (buf2 != NULL)) {
        return -1;
    }

    return memcmp(buf1, buf2, len);
}