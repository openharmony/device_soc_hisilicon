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
 * Description: HiLink独立升级入口
 */
#ifdef CONFIG_SUPPORT_HILINK_INDIE_UPGRADE
#include <stdio.h>
#include "hilink.h"
#include "app_function_mapping.h"

int hilink_indie_upgrade_main(void)
{
    hilink_func_map_init();

    /* 修改任务属性 */
    HILINK_SdkAttr *sdkAttr = HILINK_GetSdkAttr();
    if (sdkAttr == NULL) {
        printf("sdkAttr is null");
    }
    sdkAttr->deviceMainTaskStackSize = 0x4000;
    sdkAttr->otaCheckTaskStackSize = 0x3250;
    sdkAttr->otaUpdateTaskStackSize = 0x3250;
    HILINK_SetSdkAttr(*sdkAttr);

    return 0;
}
#endif