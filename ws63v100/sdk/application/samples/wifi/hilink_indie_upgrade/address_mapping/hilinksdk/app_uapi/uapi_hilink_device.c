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
 * Description: Source file for HiLink adaptation. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "app_call.h"
#include "hilink_device.h"

#include "hilink_stdio_adapter.h"
int HILINK_GetDevInfo(HILINK_DevInfo *devinfo)
{
    return app_call1(APP_CALL_HILINK_GET_DEV_INFO, int, HILINK_DevInfo *, devinfo);
}

int HILINK_GetSvcInfo(HILINK_SvcInfo *svcInfo[], unsigned int size)
{
    return app_call2(APP_CALL_HILINK_GET_SVC_INFO, int, HILINK_SvcInfo *[], svcInfo, unsigned int, size);
}

unsigned char *HILINK_GetAutoAc(void)
{
    return app_call0(APP_CALL_HILINK_GET_AUTO_AC, unsigned char *);
}

int HILINK_PutCharState(const char *svcId, const char *payload, unsigned int len)
{
    return app_call3(APP_CALL_HILINK_PUT_CHAR_STATE, int,
        const char *, svcId, const char *, payload, unsigned int, len);
}

int HILINK_ControlCharState(const char *payload, unsigned int len)
{
    return app_call2(APP_CALL_HILINK_CONTROL_CHAR_STATE, int, const char *, payload, unsigned int, len);
}

int HILINK_GetCharState(const char *svcId, const char *in, unsigned int inLen, char **out, unsigned int *outLen)
{
    return app_call5(APP_CALL_HILINK_GET_CHAR_STATE, int, const char *, svcId,
        const char *, in, unsigned int, inLen, char **, out, unsigned int *, outLen);
}

int HILINK_GetPinCode(void)
{
    return app_call0(APP_CALL_HILINK_GET_PIN_CODE, int);
}

void HILINK_NotifyDevStatus(int status)
{
    app_call1(APP_CALL_HILINK_NOTIFY_DEV_STATUS, void, int, status);
}

int HILINK_ProcessBeforeRestart(int flag)
{
    return app_call1(APP_CALL_HILINK_PROCESS_BEFORE_RESTART, int, int, flag);
}
