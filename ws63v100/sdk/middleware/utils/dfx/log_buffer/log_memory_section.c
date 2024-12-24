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
 * Description:  LOG MEMORY SECTION MODULE.
 * Author:
 * Create:  2018-10-15
 */

#include "log_memory_section.h"

/*
 * Public function definitions
 */
void log_memory_section_get(log_memory_region_section_t section, log_memory_section_params_t *section_data)
{
    const log_memory_region_control_t *log_region_control =
        (const log_memory_region_control_t *)(uintptr_t)LOG_MEMORY_REGION_CONTROL_BLOCK_POINTER;
    section_data->start = (uint8_t *)(uintptr_t)(LOGGING_REGION_START + log_region_control->offset[section]);
#if CORE_NUMS > 1
    // section_data has been previously initialized by the security core.
    if (section == LOG_MEMORY_REGION_MAX_NUMBER - 1) {  // If it is the last section use the length in stead
        section_data->length = log_region_control->length - log_region_control->offset[section];
    } else {
        section_data->length = log_region_control->offset[(uint8_t)section + 1] -
                               log_region_control->offset[(uint8_t)section];
    }
#else
    if (section == LOG_MEMORY_REGION_MAX_NUMBER - 1) {  // If it is the last section use the length in stead
        section_data->length = log_region_control->length - log_region_control->offset[section];
    }
#endif
}

log_memory_section_control_t *log_memory_section_get_control(log_memory_region_section_t section)
{
    log_memory_region_control_t *log_region_control =
        (log_memory_region_control_t *)LOG_MEMORY_REGION_CONTROL_BLOCK_POINTER;
    return &log_region_control->section_control[section];
}
