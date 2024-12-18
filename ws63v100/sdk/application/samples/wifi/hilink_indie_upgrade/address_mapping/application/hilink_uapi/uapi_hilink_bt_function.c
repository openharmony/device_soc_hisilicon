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
 * Description: hilink bt function \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */
#include "hilink_call.h"
#include "hilink_bt_function.h"

int HILINK_BT_SetSdkEventCallback(HILINK_BT_SdkEventCallBack callback)
{
    return hilink_call1(HILINK_CALL_HILINK_BT_SET_SDK_EVENT_CALLBACK, int,
        HILINK_BT_SdkEventCallBack, callback);
}