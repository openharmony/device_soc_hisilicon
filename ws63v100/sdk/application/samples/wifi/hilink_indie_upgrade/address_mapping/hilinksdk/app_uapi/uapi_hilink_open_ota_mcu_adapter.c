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
 * Description: Upgrade and adaptation of the external MCU. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "app_call.h"

int HILINK_GetMcuVersion(char *version, unsigned int inLen, unsigned int *outLen)
{
    return app_call3(APP_CALL_HILINK_GET_MCU_VERSION, int,
        char *, version, unsigned int, inLen, unsigned int *, outLen);
}

int HILINK_NotifyOtaStatus(int flag, unsigned int len, unsigned int type)
{
    return app_call3(APP_CALL_HILINK_NOTIFY_OTA_STATUS, int, int, flag, unsigned int, len, unsigned int, type);
}

int HILINK_NotifyOtaData(const unsigned char *data, unsigned int len, unsigned int offset)
{
    return app_call3(APP_CALL_HILINK_NOTIFY_OTA_DATA, int,
        const unsigned char *, data, unsigned int, len, unsigned int, offset);
}