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
 * Description: 系统适配层接口（此文件为DEMO，需集成方适配修改）
 */
#ifndef HILINK_SYS_ADAPTER_H
#define HILINK_SYS_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum HiLinkBootReason {
    HILINK_NORMAL_BOOT = 0,      /* 正常上电 */
    HILINK_WIFIDOG_REBOOT = 100, /* WIFI挂死重启 */
    HILINK_SOFTDOG_REBOOT = 101, /* HiLink任务挂死重启 */
    HILINK_HARDDOG_REBOOT = 102, /* 整机挂死导致的硬件狗复位重启 */
    HILINK_HOTA_REBOOT = 103,    /* Hota升级成功后重启 */
    HILINK_SCHEDULED_REBOOT = 104, /* 定时重启 */
    HILINK_HOTADOG_REBOOT = 105,  /* HOTA挂死重启 */
    HILINK_BOOT_REASON_BUTT      /* 获取失败 */
} HiLinkBootReason;

/*
 * 描述: 重启HiLink SDK
 * 返回: 0表示成功，其他表示失败
 * 注意: 若系统不可重启，建议重启HiLink进程
 */
int HILINK_Restart(void);

/*
 * 描述: 获取系统启动原因
 * 返回: 见HiLinkBootReason
 */
unsigned char HILINK_GetSystemBootReason(void);

#ifdef __cplusplus
}
#endif

#endif /* HILINK_SYS_ADAPTER_H */
