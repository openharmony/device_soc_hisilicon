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
 * Description: board ws63 header. \n
 *
 * History: \n
 * 2024-1-23, Create file. \n
 */

#ifndef BOARD_WS63_H
#define BOARD_WS63_H

extern LITE_OS_SEC_TEXT VOID OsExcInfoDisplayContext(const ExcInfo *exc);

VOID OsTaskWaterLineArrayGet(UINT32 *array, UINT32 *len);
VOID OsTaskCBArrayGet(UINT32 *array, UINT32 *len);
VOID OsExcInfoDisplayContextExt(const ExcInfo *exc);
VOID crashinfo_taskinfo_title_print(VOID);
VOID crashinfo_taskinfo_print(const LosTaskCB *allTaskArray, UINT32 *water_line_info_arr, UINT32 flash_save_offset,
    UINT32 task_num);
CHAR *OsCurTaskNameGetExt(VOID);

#endif