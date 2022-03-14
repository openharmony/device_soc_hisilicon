/*
 * Copyright (c) 2021 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 */

#include <hi_dumper.h>
#include <hi_dumper_adapter.h>

#include <securec.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cpup_diag_dfx.h>
#include <hi_at.h>
#include <hi_cpu.h>
#include <hi_fs.h>
#include <hi_mem.h>
#include <hi_os_stat.h>
#include <hi_task.h>
#include <hi_ver.h>
#include <hi_watchdog.h>
#include <hi_at.h>
#include <hi_errno.h>
#include <los_compiler.h>
#include <los_task_pri.h>

#include "sal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define FAULT_ADDR 0x123
#define FAULT_VALUE 0x456
#define DDR_START_ADDR 0xd8000
#define DDR_SIZE 0x46000
#define HEX_NUM_PER_LINE 16
#define FAULT_LOG_PATH "/kernel_fault.log"

extern unsigned int at_hidumper(unsigned int argc, const char **argv);

WEAK unsigned int at_hidumper(unsigned int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    return 0;
}

static const at_cmd_func g_hidumperAT[] = {
    {"+HIDUMPER", 9, HI_NULL, HI_NULL, (at_call_back_func)at_hidumper, (at_call_back_func)at_hidumper},
};

int DumpSysInfo(void)
{
    printf("SDK Version: %s\n", hi_get_sdk_version());
    printf("Boot Version: 0x%x\n", hi_get_boot_ver());
    printf("Kernel Version: 0x%x\n", hi_get_kernel_ver());

    return 0;
}

int DumpCpuUsage(void)
{
    cmd_get_cpup(0, HI_NULL);
    sal_show_run_time();

    return 0;
}

int DumpMemUsage(void)
{
    hi_os_resource_use_stat os_resource_stat = {0};
    hi_mdm_mem_info mem_inf = {0};

    (hi_void)hi_os_get_resource_status(&os_resource_stat);
    (hi_void)hi_mem_get_sys_info(&mem_inf);
    printf("Unit: Byte\n");
    printf("total=%d,", (mem_inf.total + mem_inf.total_lmp));
    printf("used=%d,", (mem_inf.used + mem_inf.used_lmp));
    printf("free=%d,", (mem_inf.free + mem_inf.free_lmp));
    printf("peek_size=%d\n", mem_inf.peek_size);
    printf("os_resource:\r\n");
    printf("timer_usage=%d,", os_resource_stat.timer_usage);
    printf("task_usage=%d,", os_resource_stat.task_usage);
    printf("sem_usage=%d,", os_resource_stat.sem_usage);
    printf("queue_usage=%d,", os_resource_stat.queue_usage);
    printf("mux_usage=%d,", os_resource_stat.mux_usage);
    printf("event_usage=%d\n", os_resource_stat.event_usage);

    return 0;
}

int DumpTaskInfo(void)
{
#ifndef CONFIG_FACTORY_TEST_MODE
    TSK_INFO_S* ptask_info = HI_NULL;
    hi_u32 i;

    ptask_info = (TSK_INFO_S*)hi_malloc(HI_MOD_ID_SAL_DFX, sizeof(TSK_INFO_S));
    if (ptask_info == HI_NULL) {
        printf("hi_malloc failed!\n");
        return -1;
    }

    printf("TID  Status  Prio  TaskSem    TaskMux     EventMask"
        "   StackSize   TopOfStack   BottomOfStack   mstatus    mepc"
        "        tp          ra              SP        SatckCurrUsed"
        "   SatckPeakUsed  SatckOverFlow     taskName\n");
    for (i = 0; i < g_taskMaxNum; ++i) {
        memset_s(ptask_info, sizeof(TSK_INFO_S), 0, sizeof(TSK_INFO_S));
        hi_u32 ret = LOS_TaskInfoGet(i, ptask_info);
        if (ret == HI_ERR_SUCCESS) {
            printf("%#3d", ptask_info->uwTaskID);
            printf("  %#6d", ptask_info->usTaskStatus);
            printf("  %#4d", ptask_info->usTaskPrio);
            printf("  0x%08x", ptask_info->pTaskSem);
            printf("  0x%08x", ptask_info->pTaskMux);
            printf("  0x%08x", ptask_info->uwEventMask);
            printf("  0x%08x", ptask_info->uwStackSize);
            printf("  0x%08x", ptask_info->uwTopOfStack);
            printf("  0x%08x", ptask_info->uwBottomOfStack);
            printf("     0x%08x", ptask_info->mstatus);
            printf("  0x%08x", ptask_info->mepc);
            printf("  0x%08x", ptask_info->tp);
            printf("  0x%08x", ptask_info->ra);
            printf("  0x%08x", ptask_info->uwSP);
            printf("  0x%08x", ptask_info->uwCurrUsed);
            printf("     0x%08x", ptask_info->uwPeakUsed);
            printf("     %#7d", ptask_info->bOvf);
            printf("        %s\n", ptask_info->acName);
        }
    }
    hi_free(HI_MOD_ID_SAL_DFX, ptask_info);
#else
    printf("Unsupported!\n");
#endif

    return 0;
}

int DumpFaultLog(void)
{
    hi_s32 fd;
    hi_char buf[128];

    fd = hi_open(FAULT_LOG_PATH, HI_FS_O_RDONLY);
    if (fd < 0) {
        printf("Open [%s] failed [0x%x]!\n", FAULT_LOG_PATH,
            hi_get_fs_error());
        return -1;
    }

    memset_s(buf, sizeof(buf), 0, sizeof(buf));
    while (hi_read(fd, buf, sizeof(buf) - 1) > 0) {
        printf("%s", buf);
        memset_s(buf, sizeof(buf), 0, sizeof(buf));
    }
    hi_close(fd);

    return 0;
}

int DumpMemRegion(unsigned long long addr, unsigned long long size)
{
    hi_u32 i;
    if (size == 0 || size > DDR_SIZE) {
        printf("Invalid DDR size: 0x%x, it must be: (0, 0x%x]\n",
            size, DDR_SIZE);
        return -1;
    }

    if (addr < DDR_START_ADDR || addr >= (DDR_START_ADDR + DDR_SIZE)) {
        printf("Invalid DDR start addr: 0x%x, it must be: [0x%x, 0x%x)",
            addr, DDR_START_ADDR, (DDR_START_ADDR + DDR_SIZE));
        return -1;
    }

    hi_watchdog_disable();
    printf("DDR start addr: 0x%x, size: 0x%x\n", addr, size);
    for (i = 0; i < size; ++i) {
        printf("%02X ", *((hi_u8 *)addr + i));
        if ((i + 1) % HEX_NUM_PER_LINE == 0) {
            printf("\n");
        }
    }
    hi_watchdog_enable();

    return 0;
}

int DumpAllMem(void)
{
    return DumpMemRegion(DDR_START_ADDR, DDR_SIZE);
}

int PlatformHiDumperIinit(void)
{
    if (hi_at_register_cmd(g_hidumperAT, sizeof(g_hidumperAT) /
        sizeof(g_hidumperAT[0])) != HI_ERR_SUCCESS) {
            printf("Register hidumper AT failed!\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif