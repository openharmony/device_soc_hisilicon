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
 * Description: HiLink register to get function ACkeyV2 \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "hilink_call.h"
#include "hilink_device.h"

void HILINK_RegisterGetAcV2Func(HILINK_GetAcKeyFunc func)
{
    hilink_call1(HILINK_CALL_HILINK_REGISTER_GET_AC_V2_FUNC, void, HILINK_GetAcKeyFunc, func);
}