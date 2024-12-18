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
 * Description: HiLink SDK提供的demo示例代码（此文件为DEMO，需集成方适配修改）
 */
#include <stdio.h>
#include <stddef.h>
#include "hilink.h"
#include "hilink_device.h"
#include "cmsis_os2.h"
#include "hilink_socket_adapter.h"

static int GetAcV2Func(unsigned char *acKey, unsigned int acLen)
{
    /* key文件在开发者平台获取 */
    return -1;
}

int hilink_wifi_main(void)
{
    /* 注册ACKey函数 */
    HILINK_RegisterGetAcV2Func(GetAcV2Func);

    /* 设置配网方式 */
    HILINK_SetNetConfigMode(HILINK_NETCONFIG_WIFI);

#ifdef CONFIG_SUPPORT_HILINK_INDIE_UPGRADE
    if (HILINK_RegisterErrnoCallback(get_os_errno) != 0) {
        printf("reg errno cb err\r\n");
    }
#endif

    /* 修改任务属性 */
    HILINK_SdkAttr *sdkAttr = HILINK_GetSdkAttr();
    if (sdkAttr == NULL) {
        printf("sdkAttr is null");
    }
    sdkAttr->monitorTaskStackSize = 0x600;
    HILINK_SetSdkAttr(*sdkAttr);

    /* 启动Hilink SDK */
    if (HILINK_Main() != 0) {
        printf("HILINK_Main start error");
    }

    return 0;
}
