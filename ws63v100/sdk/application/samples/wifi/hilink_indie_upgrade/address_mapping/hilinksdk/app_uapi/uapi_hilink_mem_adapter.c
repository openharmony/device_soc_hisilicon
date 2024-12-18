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
 * Description: Implementation of the memory interface at the system adaptation layer. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "app_call.h"

void *HILINK_Malloc(unsigned int size)
{
    return app_call1(APP_CALL_HILINK_MALLOC, void *, unsigned int, size);
}

void HILINK_Free(void *pt)
{
    app_call1(APP_CALL_HILINK_FREE, void, void *, pt);
}

int HILINK_Memcmp(const void *buf1, const void *buf2, unsigned int len)
{
    return app_call3(APP_CALL_HILINK_MEMCMP, int, const void *, buf1, const void *, buf2, unsigned int, len);
}