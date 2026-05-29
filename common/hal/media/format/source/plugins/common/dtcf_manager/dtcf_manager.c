/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 * Description: dtcf manager
 */
#include "dtcf_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <errno.h>
#include <mntent.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/vfs.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ss_dtcf.h"
#include "media_hal_common.h"

#ifdef __UCLIBC__
#include <sys/syscall.h>
#define HIDWORD(a)                       ((td_u32)(((td_u64)(a)) >> 32))
#define LODWORD(a)                       ((td_u32)(td_u64)(a))
#define fallocate(fd, mode, offset, len) syscall(__NR_fallocate, \
    fd, mode, LODWORD(offset), HIDWORD(offset), LODWORD(len), HIDWORD(len))
#endif
#define MODULE_NAME "HisiMuxerDtcf"
#define BYTE_UNIT 1024
#define FILE_NAME_SIZE 1024
#define DISK_CAPACITY_CHECK_CYCLE (500 * 1000)
#define NORM_DIR_NUM 4
#define EMR_DIR_NUM 4
#define RM_RECORDERED_START_RATIO 20 /* 20 / 100 */
#define RM_RECORDERED_END_RATIO 30 /* 20 / 100 */
#define PERSENTAGE 100

static td_bool g_isCapacityCheckStop = TD_FALSE;
static pthread_t g_capacityThread = 0;
static td_bool g_isFileOptInited = TD_FALSE;

static pthread_mutex_t g_pthDtcfOptLock = PTHREAD_MUTEX_INITIALIZER;
static void FileOptLock(void)
{
    td_s32 s32LockRet = pthread_mutex_lock(&g_pthDtcfOptLock);
    if (s32LockRet != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, " mutex lock failed %d!", s32LockRet);
    }
}

static void FileOptUnLock(void)
{
    td_s32 s32unLockRet = pthread_mutex_unlock(&g_pthDtcfOptLock);
    if (s32unLockRet != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "mutex unlock failed ret %d!", s32unLockRet);
    }
}

static void *DiskCapacityCheckThread(VoidPtr arg)
{
    (void)arg;
    return NULL;
}

td_s32 FileOptInit(const char *path)
{
    td_u32 u32FileAmount;
    if (g_isFileOptInited) {
        return TD_SUCCESS;
    }
    const td_char apszDirs[DTCF_DIR_BUTT][OT_DIR_LEN_MAX] = {"emr", "emr_s", "norm", "norm_s", "", "",
        "emr_b", "emr_b_s", "norm_b", "norm_b_s", "Photo", "", ""};
    if (path == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "input path is NULL");
        return TD_FAILURE;
    }
    g_isCapacityCheckStop = TD_FALSE;

    td_s32 ret = pthread_create(&g_capacityThread, NULL, DiskCapacityCheckThread, (void *)path);
    if (ret != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "pthread_create failed: %d.", ret);
        return TD_FAILURE;
    }

    ret = SS_DTCF_Init(path, apszDirs);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_DTCF_Init fail %d", ret);
        return TD_FAILURE;
    }
    OT_DTCF_DIR_E azScanDir[DTCF_DIR_BUTT] = {
        DTCF_DIR_EMR_FRONT,
        DTCF_DIR_EMR_FRONT_SUB,
        DTCF_DIR_NORM_FRONT,
        DTCF_DIR_NORM_FRONT_SUB,
        DTCF_DIR_EMR_REAR,
        DTCF_DIR_EMR_REAR_SUB,
        DTCF_DIR_NORM_REAR,
        DTCF_DIR_NORM_REAR_SUB,
    };
    ret = SS_DTCF_Scan(azScanDir, 8, &u32FileAmount); /* 8：Scan file list dircount */
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_DTCF_Scan fail %d", ret);
        return TD_FAILURE;
    }
    g_isFileOptInited = TD_TRUE;
    return TD_SUCCESS;
}

td_s32 FileOptDeInit()
{
    if (!g_isFileOptInited) {
        return TD_SUCCESS;
    }
    if (g_capacityThread != 0) {
        g_isCapacityCheckStop = TD_TRUE;
        pthread_join(g_capacityThread, NULL);
        g_capacityThread = 0;
    }
    g_isFileOptInited = TD_FALSE;
    MEDIA_HAL_LOGD(MODULE_NAME, "FileOptDeInit");
    return SS_DTCF_DeInit();
}

td_s32 AddNorm(const td_char *pszFileName)
{
    OT_DTCF_DIR_E enDir = DTCF_DIR_BUTT;
    if (SS_DTCF_GetFileDirType(pszFileName, &enDir) != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_DTCF_GetFileDirType %s fail", pszFileName);
    }

    FileOptLock();
    if (SS_DTCF_AddFile(pszFileName, enDir) != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_DTCF_AddFile %s fail", pszFileName);
    }
    FileOptUnLock();

    return TD_SUCCESS;
}

td_s32 MoveEmr(const td_char *pszFileName)
{
    td_char aszEmrFilePath[512] = {0};
    if (SS_DTCF_GetEmrFilePath(pszFileName, aszEmrFilePath, 512) != TD_SUCCESS) { /* 512: DTCF file path length  */
        MEDIA_HAL_LOGE(MODULE_NAME, "call SS_DTCF_GetEmrFilePath fail");
        return TD_FAILURE;
    }
    if (rename(pszFileName, aszEmrFilePath) == -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "call rename %s fail %d", aszEmrFilePath, errno);
        return TD_FAILURE;
    }

    const td_char *emrFilePath = aszEmrFilePath;
    OT_DTCF_DIR_E enDir = DTCF_DIR_BUTT;
    if (SS_DTCF_GetFileDirType(emrFilePath, &enDir) != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_DTCF_GetFileDirType %s fail", pszFileName);
    }
    FileOptLock();
    if (SS_DTCF_AddFile(emrFilePath, enDir) != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_DTCF_AddFile %s fail", aszEmrFilePath);
        FileOptUnLock();
        return TD_FAILURE;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "splited file has been moved to emr directory.");
    FileOptUnLock();
    return TD_SUCCESS;
}

td_s32 RequestFileNames(TD_MW_PTR pRecord, td_u32 u32FileCnt, td_char (*paszFileNames)[OT_REC_FILE_NAME_LEN])
{
    MW_UNUSED(pRecord);
    MW_UNUSED(u32FileCnt);
    td_s32 s32Ret = SS_DTCF_CreateFilePath(OT_DTCF_FILE_TYPE_MP4, DTCF_DIR_NORM_FRONT,
        paszFileNames[0], OT_REC_FILE_NAME_LEN);
    MW_CHECK_RETURN(s32Ret);
    s32Ret = SS_DTCF_GetRelatedFilePath(paszFileNames[0], DTCF_DIR_NORM_FRONT_SUB,
        paszFileNames[1], OT_REC_FILE_NAME_LEN);
    return s32Ret;
}

td_s32 RequestTSFileNames(TD_MW_PTR pRecord, td_u32 u32FileCnt, td_char (*paszFileNames)[OT_REC_FILE_NAME_LEN])
{
    MW_UNUSED(pRecord);
    MW_UNUSED(u32FileCnt);
    td_s32 s32Ret = SS_DTCF_CreateFilePath(OT_DTCF_FILE_TYPE_TS, DTCF_DIR_NORM_FRONT,
        paszFileNames[0], OT_REC_FILE_NAME_LEN);
    MW_CHECK_RETURN(s32Ret);
    s32Ret = SS_DTCF_GetRelatedFilePath(paszFileNames[0], DTCF_DIR_NORM_FRONT_SUB,
        paszFileNames[1], OT_REC_FILE_NAME_LEN);
    return s32Ret;
}

td_s32 RequestDualTSFileNames(TD_MW_PTR pRecord,
    td_u32 u32FileCnt, td_char (*paszFileNames)[OT_REC_FILE_NAME_LEN])
{
    MW_UNUSED(pRecord);
    MW_UNUSED(u32FileCnt);
    td_s32 s32Ret = SS_DTCF_CreateFilePath(OT_DTCF_FILE_TYPE_TS, DTCF_DIR_NORM_FRONT,
        paszFileNames[0], OT_REC_FILE_NAME_LEN);
    MW_CHECK_RETURN(s32Ret);
    s32Ret = SS_DTCF_GetRelatedFilePath(paszFileNames[0], DTCF_DIR_NORM_FRONT_SUB,
        paszFileNames[1], OT_REC_FILE_NAME_LEN);
    MW_CHECK_RETURN(s32Ret);
    s32Ret = SS_DTCF_GetRelatedFilePath(paszFileNames[0], DTCF_DIR_NORM_REAR,
        paszFileNames[0x2], OT_REC_FILE_NAME_LEN);
    MW_CHECK_RETURN(s32Ret);
    s32Ret = SS_DTCF_GetRelatedFilePath(paszFileNames[0], DTCF_DIR_NORM_REAR_SUB,
        paszFileNames[0x3], OT_REC_FILE_NAME_LEN);
    return s32Ret;
}

td_s32 RequestDualFileNames(TD_MW_PTR pRecord,
    td_u32 u32FileCnt, td_char (*paszFileNames)[OT_REC_FILE_NAME_LEN])
{
    MW_UNUSED(pRecord);
    MW_UNUSED(u32FileCnt);
    td_s32 ret = SS_DTCF_CreateFilePath(OT_DTCF_FILE_TYPE_MP4, DTCF_DIR_NORM_FRONT,
        paszFileNames[0], OT_REC_FILE_NAME_LEN);
    MW_CHECK_RETURN(ret);
    ret = SS_DTCF_GetRelatedFilePath(paszFileNames[0], DTCF_DIR_NORM_FRONT_SUB,
        paszFileNames[1], OT_REC_FILE_NAME_LEN);
    MW_CHECK_RETURN(ret);
    ret = SS_DTCF_GetRelatedFilePath(paszFileNames[0], DTCF_DIR_NORM_REAR,
        paszFileNames[0x2], OT_REC_FILE_NAME_LEN);
    MW_CHECK_RETURN(ret);
    ret = SS_DTCF_GetRelatedFilePath(paszFileNames[0], DTCF_DIR_NORM_REAR_SUB,
        paszFileNames[0x3], OT_REC_FILE_NAME_LEN);
    return ret;
}

td_s32 GetLastFileNames(td_u32 *fileCnt, td_char (*fileNames)[OT_REC_FILE_NAME_LEN])
{
    if (fileCnt == NULL) {
        return TD_FAILURE;
    }
    OT_DTCF_DIR_E dir = DTCF_DIR_BUTT;
    td_u32 fileAmount = 0;
    td_s32 ret = SS_DTCF_GetFileAmount(&fileAmount);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "get file amount failed, and ret[%d]", ret);
        return ret;
    }
    if (fileAmount == 0) {
        *fileCnt = 0;
        return TD_SUCCESS;
    }
    ret = SS_DTCF_GetFileByIndex(0, fileNames[0], OT_REC_FILE_NAME_LEN, &dir);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "get last file failed, and ret[%d]", ret);
        return ret;
    }
    *fileCnt = 1;
    OT_DTCF_DIR_E relateDir = dir + 1;
    ret = SS_DTCF_GetRelatedFilePath(fileNames[0], relateDir, fileNames[1], OT_REC_FILE_NAME_LEN);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "get related file[%s] failed", fileNames[0]);
        return ret;
    }
    td_char resolvedpath[PATH_MAX] = {0};
    if (strlen(fileNames[1]) >= PATH_MAX || realpath(fileNames[1], resolvedpath) == NULL) {
        return TD_SUCCESS;
    }

    *fileCnt = 2; /* 2: fileCnt size */
    return TD_SUCCESS;
}
