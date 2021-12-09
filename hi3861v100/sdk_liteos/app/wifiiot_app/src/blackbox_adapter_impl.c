/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

/*-----------includes---------*/
#include <hi_blackbox.h>
#include <hi_blackbox_adapter.h>
#include <hi_crash.h>
#include <hi_flash.h>
#include <hi_fs.h>
#include <hi_partition_table.h>
#include <hi_reset.h>
#include <hi_stdlib.h>

/* ------------ local macroes -----------*/
#define FAULT_LOG_SIZE      0x800 /*2KB*/
#define LONG_BUF_SIZE       512
#define SHORT_BUF_SIZE      64
#define LOG_SAVED_FLAG      "LOGSAVED!"
#define SYSERR_INFO_FLAG   "====fault info====\r\n"
#define EXCP_INFO_FLAG     "====exc info====\r\n"
#define REG_INFO_FLAG      "====reg info====\r\n"
#define MEMORY_INFO_FLAG   "====mem info====\r\n"
#define TASK_INFO_FLAG     "====task info====\r\n"
#define TRACK_INFO_FLAG    "====track info====\r\n"
#define CALL_STACK_START   "====call stack====\r\n"
#define FAULT_LOG_PATH     "/kernel_fault.log"

/* --------global variables----------*/
extern hi_u32 __rom_text_begin__;
extern hi_u32 __rom_text_end__;
extern hi_u32 __ram_text_start;
extern hi_u32 __ram_text_end;
extern hi_u32 __text_cache_start1_;
extern hi_u32 __text_cache_end1_;
extern hi_u32 __text_cache_start2_;
extern hi_u32 __text_cache_end2_;

/* ----function definitions---------*/
static void SaveSysInfo(const char* filePath, struct ErrorInfo *errInfo,
    hi_syserr_info *sysErrInfo)
{
    hi_s32 len;
    hi_char buf[LONG_BUF_SIZE];

    if (filePath == NULL || errInfo == NULL || sysErrInfo == NULL){
        printf("filePath: %p, errInfo: %p, sysErrInfo: %p!\r\n",
            filePath, errInfo, sysErrInfo);
        return;
    }

    /* save header */
    (hi_void)FullWriteFile(filePath, SYSERR_INFO_FLAG, 
        strlen(SYSERR_INFO_FLAG), 0);

    /* format and save useful log */
    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        ERROR_INFO_HEADER_FORMAT "Ver:%s\r\n",
        errInfo->event, errInfo->module, errInfo->errorDesc,
        sysErrInfo->basic_info.kernel_ver);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    } else {
        printf("snprintf_s failed or the buffer is too short!\r\n");
    }
}

static void SaveExcpInfo(const char *filePath, hi_syserr_info *sysErrInfo)
{
    hi_s32 len;
    hi_char buf[LONG_BUF_SIZE];

    if (filePath == NULL || sysErrInfo == NULL) {
        printf("filePath: %p, sysErrInfo: %p\r\n",
            filePath, sysErrInfo);
        return;
    }

    /* save neader*/
    (hi_void)FullWriteFile(filePath, EXCP_INFO_FLAG,
        strlen(EXCP_INFO_FLAG), 1);

    /* format and save usefull log*/
    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "TaskName:%s\r\n"
        "TaskID:%d\r\n"
        "CurTaskID:%d\r\n"
        "StackSize:0x%x\r\n"
        "ExcType:0x%x\r\n",
        sysErrInfo->os_info.task.name,
        sysErrInfo->os_info.task.id,
        sysErrInfo->core_info.cur_task_id,
        sysErrInfo->os_info.task.stack_size,
        sysErrInfo->basic_info.rid);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    } else {
        printf("snprintf_s failed or the buffer is too short!\r\n");
    }
}

static void SaveRegInfo(const char* filePath, hi_syserr_info *sysErrInfo)
{
    hi_s32 len;
    hi_char buf[LONG_BUF_SIZE];

    if (filePath == NULL || sysErrInfo == NULL) {
        printf("filePath: %p, sysErrInfo: %p!\r\n",
            filePath, sysErrInfo);
        return;
    }

    /* save header */
    (hi_void)FullWriteFile(filePath, REG_INFO_FLAG, strlen(REG_INFO_FLAG), 1);
    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "mepc:0x%x\r\nmstatus:0x%x\r\nmtval:0x%x\r\n"
        "mcause:0x%x\r\nccause:0x%x\r\n", sysErrInfo->reg_info.mepc,
        sysErrInfo->core_info.mstatus, sysErrInfo->core_info.mtval,
        sysErrInfo->core_info.mcause, sysErrInfo->core_info.ccause);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }

    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "ra:0x%x\r\nsp:0x%x\r\ngp:0x%x\r\ntp:0x%x\r\n",
        sysErrInfo->reg_info.ra, sysErrInfo->reg_info.sp,
        sysErrInfo->reg_info.gp, sysErrInfo->reg_info.tp);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }

    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "t0:0x%x\r\nt1:0x%x\r\nt2:0x%x\r\ns0:0x%x\r\n",
        sysErrInfo->reg_info.t0, sysErrInfo->reg_info.t1,
        sysErrInfo->reg_info.t2, sysErrInfo->reg_info.s0);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }

    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "s1:0x%x\r\na0:0x%x\r\na1:0x%x\r\na2:0x%x\r\n",
        sysErrInfo->reg_info.s1, sysErrInfo->reg_info.a0,
        sysErrInfo->reg_info.a1, sysErrInfo->reg_info.a2);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }

    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "a3:0x%x\r\na4:0x%x\r\na5:0x%x\r\na6:0x%x\r\n",
        sysErrInfo->reg_info.a3, sysErrInfo->reg_info.a4,
        sysErrInfo->reg_info.a5, sysErrInfo->reg_info.a6);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }

    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "a7:0x%x\r\ns2:0x%x\r\ns3:0x%x\r\ns4:0x%x\r\n",
        sysErrInfo->reg_info.a7, sysErrInfo->reg_info.s2,
        sysErrInfo->reg_info.s3, sysErrInfo->reg_info.s4);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }

    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "s5:0x%x\r\ns6:0x%x\r\ns7:0x%x\r\ns8:0x%x\r\n",
        sysErrInfo->reg_info.s5, sysErrInfo->reg_info.s6,
        sysErrInfo->reg_info.s7, sysErrInfo->reg_info.s8);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }

    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "s9:0x%x\r\ns10:0x%x\r\ns11:0x%x\r\n",
        sysErrInfo->reg_info.s9, sysErrInfo->reg_info.s10,
        sysErrInfo->reg_info.s11);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }

    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "t3:0x%x\r\nt4:0x%x\r\nt5:0x%x\r\nt6:0x%x\r\n",
        sysErrInfo->reg_info.t3, sysErrInfo->reg_info.t4,
        sysErrInfo->reg_info.t5, sysErrInfo->reg_info.t6);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }
}

static void SaveMemInfo(const char *filePath, hi_syserr_info *sysErrInfo)
{
    hi_s32 len;
    hi_char buf[LONG_BUF_SIZE];

    if (filePath == NULL || sysErrInfo == NULL) {
        printf("filePath: %p, sysErrInfo: %p!\r\n",
            filePath, sysErrInfo);
        return;
    }

    /* save header */
    (hi_void)FullWriteFile(filePath, MEMORY_INFO_FLAG, 
        strlen(MEMORY_INFO_FLAG), 1);

    /* format and save usefull log*/
    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "PoolAddr:0x%x\r\n"
        "PoolSize:0x%x\r\n"
        "FailCount:0x%x\r\n"
        "PeekSize:0x%x\r\n"
        "UsedSize:0x%x\r\n",
        sysErrInfo->os_info.mem.pool_addr,
        sysErrInfo->os_info.mem.pool_size,
        sysErrInfo->os_info.mem.fail_count,
        sysErrInfo->os_info.mem.peek_size,
        sysErrInfo->os_info.mem.cur_use_size);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    } else {
        printf("snprintf_s failed or the buffer is too short!\r\n");
    }
}

static void SaveTaskInfo(const char *filePath, hi_syserr_info *sysErrInfo)
{
    hi_s32 len;
    hi_char buf[LONG_BUF_SIZE];

    if (filePath == NULL || sysErrInfo == NULL) {
        printf("filePath: %p, sysErrInfo: %p!\r\n",
            filePath, sysErrInfo);
        return;
    }

    /* save header */
    (hi_void)FullWriteFile(filePath, TASK_INFO_FLAG,
        strlen(TASK_INFO_FLAG), 1);

    /* format and save usefull log */
    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "Status:0x%x\r\n"
        "StackIndex:0x%x\r\n"
        "StackPeak:0x%x\r\n"
        "StackSize:0x%x\r\n",
        sysErrInfo->os_info.task.status,
        sysErrInfo->os_info.task.stack_data_index,
        sysErrInfo->os_info.task.stack_peak,
        sysErrInfo->os_info.task.stack_size);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }

    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "SP:0x%x\r\n"
        "Stack:0x%x to 0x%x\r\n"
        "RealSP:0x%x\r\n"
        "StackOverflow:%d\r\n",
        sysErrInfo->os_info.task.sp,
        sysErrInfo->os_info.task.stack[0], sysErrInfo->os_info.task.stack[1],
        sysErrInfo->os_info.task.real_sp,
        sysErrInfo->os_info.task.overflow_flag);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }
}

static void SaveTrackInfo(const char *filePath, hi_syserr_info *sysErrInfo)
{
    hi_s32 i;
    hi_s32 len;
    hi_char buf[LONG_BUF_SIZE];

    if (filePath == NULL || sysErrInfo == NULL) {
        printf("filePath: %p, sysErrInfo: %p!\r\n",
            filePath, sysErrInfo);
        return;
    }

    /* save header */
    (hi_void)FullWriteFile(filePath, TRACK_INFO_FLAG,
        strlen(TRACK_INFO_FLAG), 1);
    len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
        "CurItem:0x%x\r\n"
        "ItemCnt:%d\r\n"
        "Index Type ID CurTime Data1 Data2\r\n",
        sysErrInfo->track_info.current_item,
        sysErrInfo->track_info.item_cnt);
    if (len > 0) {
        (hi_void)FullWriteFile(filePath, buf, len, 1);
    }

    for (i = 0; i < sysErrInfo->track_info.item_cnt; i++) {
        len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
            "%04d %04d %04d 0x%x 0x%x 0x%x\r\n",
            i,
            sysErrInfo->track_info.item[i].type,
            sysErrInfo->track_info.item[i].id,
            sysErrInfo->track_info.item[i].timestamp,
            sysErrInfo->track_info.item[i].data,
            sysErrInfo->track_info.item[i].entry);
        if (len > 0) {
            (hi_void)FullWriteFile(filePath, buf, len, 1);
        }
    }
}

static hi_bool IsValidText(hi_u32 addr)
{
    return ((addr >= (hi_u32)&__rom_text_begin__ &&
        addr <= (hi_u32)&__rom_text_end__) ||
        (addr >= (hi_u32)&__text_cache_start2_ &&
        addr <= (hi_u32)&__text_cache_end2_) ||
        (addr >= (hi_u32)&__ram_text_start &&
        addr <= (hi_u32)&__ram_text_end) ||
        (addr >= (hi_u32)&__text_cache_start1_ &&
        addr <= (hi_u32)&__text_cache_end1_));
}

static void SaveCallBack(const char *filePath, hi_syserr_info *sysErrInfo)
{
    hi_char buf[SHORT_BUF_SIZE];
    hi_bool findMepc = HI_FALSE;
    hi_s32 len;
    hi_u32 addr;
    hi_u32 size;
    hi_u32 i;
    hi_u32 index = 0;
    hi_u32 stackSize = sysErrInfo->os_info.task.stack[1] - 
        sysErrInfo->os_info.task.stack[0];
    hi_u32 *pData;

    if (filePath == NULL || sysErrInfo == NULL) {
        printf("filePath: %p, sysErrInfo: %p!\r\n",
            filePath, sysErrInfo);
        return;
    }

    if (hi_get_crash_partition_table(&addr, &size) != 0) {
        printf("hi_get_crash_partition_table failed!\r\n");
        return;
    }

    pData = hi_malloc(HI_MOD_ID_SAL_DFX, stackSize);
    if (pData == NULL) {
        printf("hi_malloc failed!\r\n");
        return;
    }

    if (hi_flash_read(addr + size - stackSize, stackSize,
        (hi_u8 *)pData) != HI_ERR_SUCCESS) {
        printf("Read stack from crash partition failed!\r\n");
        hi_free(HI_MOD_ID_SAL_DFX, pData);
        return;
    }

    /* save header */
    (hi_void) FullWriteFile(filePath, CALL_STACK_START,
        strlen(CALL_STACK_START), 1);
    for (i = 0; i < stackSize / sizeof(hi_u32); ++i) {
        hi_u32 *ptemp = pData + i;
        if (!findMepc && *ptemp == sysErrInfo->reg_info.mepc) {
            findMepc = HI_TRUE;
        }
        if (IsValidText(*ptemp) && findMepc) {
            len = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
                "#%d %x\r\n", index, *ptemp);
            if (len > 0) {
                (hi_void)FullWriteFile(filePath, buf, len, 1);
            }
            ++index;
        }
    }
    hi_free(HI_MOD_ID_SAL_DFX, pData);
}

static void SaveFaultLog(struct ErrorInfo *errInfo, hi_syserr_info *sysErrInfo)
{
    if (errInfo == NULL || sysErrInfo == NULL) {
        printf("errInfo: %p, sysErrInfo: %p!\r\n",
            errInfo, sysErrInfo);
        return;
    }

    SaveSysInfo(FAULT_LOG_PATH, errInfo, sysErrInfo);
    SaveExcpInfo(FAULT_LOG_PATH, sysErrInfo);
    SaveRegInfo(FAULT_LOG_PATH, sysErrInfo);
    SaveMemInfo(FAULT_LOG_PATH, sysErrInfo);
    SaveTaskInfo(FAULT_LOG_PATH, sysErrInfo);
    SaveTrackInfo(FAULT_LOG_PATH, sysErrInfo);
    SaveCallBack(FAULT_LOG_PATH, sysErrInfo);
}

static void SetLogSavedFlag(hi_void)
{
    hi_u32 addr;
    hi_u32 size;

    if (hi_get_crash_partition_table(&addr, &size) != 0) {
        printf("hi_get_crash_partition_table failed!\r\n");
        return;
    }

    if (hi_flash_write(addr, sizeof(LOG_SAVED_FLAG), 
        (hi_u8 *)LOG_SAVED_FLAG, HI_TRUE) != HI_ERR_SUCCESS) {
            printf("Set log saved flag failed!\r\n");
        }
}

static char *GetEvent(hi_syserr_eid err_id)
{
    switch (err_id) {
    case HI_SYSERR_EID_POWER_DOWN:
        return EVENT_POWEROFF;
    case HI_SYSERR_EID_FAULT_IN_TASK:
    case HI_SYSERR_EID_FAULT_IN_ISR:
        return EVENT_PANIC;
    case HI_SYSERR_EID_WATCHDOG_ISR:
    case HI_SYSERR_EID_WATCHDOG_TSK:
        return EVENT_SYS_WATCHDOG;
    case HI_SYSERR_EID_SYS_SOFT_REBOOT:
        return EVENT_SYSREBOOT;
    default:
        break;
    }

    return "Unknow";
}

static char *GetErrorDesc(hi_syserr_eid err_id)
{
    switch (err_id) {
        case HI_SYSERR_EID_POWER_DOWN:
            return "Power down or hard reboot";
        case HI_SYSERR_EID_FAULT_IN_TASK:
            return "fault in task";
        case HI_SYSERR_EID_FAULT_IN_ISR:
            return "fault in interrupt";
        case HI_SYSERR_EID_WATCHDOG_TSK:
            return "watch dog fault in task";
        case HI_SYSERR_EID_WATCHDOG_ISR:
            return "watch dog fault in interrupt";
        case HI_SYSERR_EID_SYS_SOFT_REBOOT:
            return "soft reboot";
        default:
            break;
    }

    return "Unknown fault type!";
}

int FullWriteFile(const char *filePath, const char *buf, size_t bufSize, int isAppend)
{
    int fd;
    int totalToWrite = (int) bufSize;
    int totalWrite = 0;

    if (filePath == NULL || buf == NULL || bufSize == 0) {
        printf("filePath: %p, buf: %p, bufSize: %lu!\n", filePath, buf, bufSize);
        return -1;
    }

    fd = hi_open(filePath, HI_FS_O_WRONLY | HI_FS_O_CREAT |
        (isAppend ? HI_FS_O_APPEND : HI_FS_O_TRUNC));
    if (fd < 0) {
        printf("Create file [%s] failed!\n", filePath);
        return -1;
    }

    while (totalToWrite > 0) {
        int writeThisTime = hi_write(fd, buf, totalToWrite);
        if (writeThisTime < 0) {
            printf("Failed to write file: %s\n", filePath);
            (void)hi_close(fd);
            return -1;
        }
        buf += writeThisTime;
        totalToWrite -= writeThisTime;
        totalWrite += writeThisTime;
    }
    (void)hi_close(fd);

    return totalWrite == (int)bufSize ? 0 : -1;
}

char *GetFaultLogPath(void)
{
    return FAULT_LOG_PATH;
}

void RebootSystem(void)
{
    hi_hard_reboot(HI_SYS_REBOOT_CAUSE_CMD);
}

void SystemModuleDump(const char *logDir, struct ErrorInfo *info)
{
    int len;
    char *buf;
    (void)logDir;

    if (logDir == NULL || info == NULL) {
        printf("logDir: %p, info: %p\r\n", logDir, info);
        return;
    }

    buf = hi_malloc(HI_MOD_ID_SAL_DFX, FAULT_LOG_SIZE);
    if (buf == NULL) {
        printf("malloc failed!\r\n");
        return;
    }
    (void)memset_s(buf, FAULT_LOG_SIZE, 0, FAULT_LOG_SIZE);
    len = snprintf_s(buf, FAULT_LOG_SIZE, FAULT_LOG_SIZE - 1,
        ERROR_INFO_HEADER ERROR_INFO_HEADER_FORMAT,
        info->event, info->module, info->errorDesc);
    if (len <= 0) {
        printf("buf isn't enough or vsnprintf_s failed\r\n");
        hi_free(HI_MOD_ID_SAL_DFX, buf);
        return;
    }
    (hi_void)FullWriteFile(FAULT_LOG_PATH, buf, len, 0);
    hi_free(HI_MOD_ID_SAL_DFX, buf);
}

void SystemModuleReset(struct ErrorInfo *info)
{
    if (info == NULL) {
        printf("info: %p\r\n", info);
        return;
    }
}

int SystemModuleGetLastLogInfo(struct ErrorInfo *info)
{
    hi_u16 err_id = 0;
    hi_u32 rid = 0;
    hi_u32 ret;
    hi_syserr_info *errInfo;
    char *data;

    if (info == NULL) {
        printf("info: %p\r\n", info);
        return -1;
    }

    errInfo = hi_malloc(HI_MOD_ID_SAL_DFX, sizeof(*errInfo));
    if (errInfo == NULL) {
        printf("hi_malloc failed!\r\n");
        return -1;
    }
    memset_s(errInfo, sizeof(*errInfo), 0, sizeof(*errInfo));
    ret = hi_syserr_get(errInfo);
    if (ret != HI_ERR_SUCCESS) {
        printf("No crash dump found!\r\n");
        hi_free(HI_MOD_ID_SAL_DFX, errInfo);
        return -1;
    }
    hi_free(HI_MOD_ID_SAL_DFX, errInfo);
    ret = hi_syserr_get_reboot_reason(&err_id, &rid);
    if (ret == HI_ERR_SUCCESS) {
        printf("latest reboot reason:\r\neid:%d,"
            " rid:%d:\r\n", err_id, rid);
    }
    (void)memset_s(info, sizeof(*info), 0, sizeof(*info));
    data = GetEvent(err_id);
    (void)strncpy_s(info->event, sizeof(info->event) - 1,
        data, strlen(data));
    (void)strncpy_s(info->module, sizeof(info->module) - 1,
        MODULE_SYSTEM, strlen(MODULE_SYSTEM));
    data = GetErrorDesc(err_id);
    (void)strncpy_s(info->errorDesc, sizeof(info->errorDesc) - 1,
        data, strlen(data));

    return 0;
}

int SystemModuleSaveLastLog(const char *logDir, struct ErrorInfo *info)
{
    hi_u32 ret;
    hi_syserr_info *errInfo;
    (void)logDir;

    errInfo = hi_malloc(HI_MOD_ID_SAL_DFX, sizeof(*errInfo));
    if (errInfo == NULL) {
        printf("hi_malloc failed!\r\n");
        return -1;
    }
    memset_s(errInfo, sizeof(*errInfo), 0, sizeof(*errInfo));
    ret = hi_syserr_get(errInfo);
    if (ret != HI_ERR_SUCCESS) {
        printf("No crash dump found!\r\n");
        return -1;
    } else {
        SaveFaultLog(info, errInfo);
        SetLogSavedFlag();
    }
    hi_free(HI_MOD_ID_SAL_DFX, errInfo);

    return 0;
}