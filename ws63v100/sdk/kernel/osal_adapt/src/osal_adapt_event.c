
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
 * Description: osal adapt atomic event file.
 */
#include "osal_adapt.h"

/************************ osal_event ************************/
int osal_adapt_event_init(osal_event *event_obj)
{
    return osal_event_init(event_obj);
}

int osal_adapt_event_write(osal_event *event_obj, unsigned int mask)
{
    return osal_event_write(event_obj, mask);
}

int osal_adapt_event_read(osal_event *event_obj, unsigned int mask, unsigned int timeout_ms, unsigned int mode)
{
    return osal_event_read(event_obj, mask, timeout_ms, mode);
}

int osal_adapt_event_clear(osal_event *event_obj, unsigned int mask)
{
    return osal_event_clear(event_obj, mask);
}

int osal_adapt_event_destroy(osal_event *event_obj)
{
    return osal_event_destroy(event_obj);
}
