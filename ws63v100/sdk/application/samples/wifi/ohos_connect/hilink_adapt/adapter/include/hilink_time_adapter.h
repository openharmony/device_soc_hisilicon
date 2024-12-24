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
 * Description: 时间适配层接口（此文件为DEMO，需集成方适配修改）
 */
#ifndef HILINK_TIME_ADAPTER_H
#define HILINK_TIME_ADAPTER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HiLinkTimeval {
    uint32_t sec;
    uint32_t usec;
} HiLinkTimeval;

/*
 * 描述：获取当前设备启动后运行时间
 * 参数：time，出参，表示当前设备启动后运行时间
 * 返回：0成功，其他失败
 */
int HILINK_GetOsTime(HiLinkTimeval *time);

/*
 * 描述：获取当前UTC时间
 * 参数：time，出参
 * 返回：0成功，其他失败
 * 注意: 对于不支持时间同步的设备，该接口不使用
 */
int HILINK_GetUtcTime(HiLinkTimeval *time);

#ifdef __cplusplus
}
#endif

#endif /* HILINK_TIME_ADAPTER_H */