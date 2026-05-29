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
 * Description: dtcf manager interface
 */

#ifndef DTCF_MANAGER_H
#define DTCF_MANAGER_H
#include "ot_mw_type.h"
#include "ss_recorder_pro.h"

#define MW_CHECK_RETURN(s32Ret) \
    do { \
        if ((s32Ret) != TD_SUCCESS) { \
            return s32Ret; \
        } \
    } while (0)

td_s32 FileOptInit(const char *path);
td_s32 RequestFileNames(TD_MW_PTR pRecord, td_u32 u32FileCnt, td_char (*paszFileNames)[OT_REC_FILE_NAME_LEN]);
td_s32 RequestTSFileNames(TD_MW_PTR pRecord, td_u32 u32FileCnt, td_char (*paszFileNames)[OT_REC_FILE_NAME_LEN]);

td_s32 AddNorm(const td_char *pszFileName);
td_s32 MoveEmr(const td_char *pszFileName);
td_s32 FileOptDeInit();
td_s32 RequestDualFileNames(TD_MW_PTR pRecord, td_u32 u32FileCnt, td_char (*paszFileNames)[OT_REC_FILE_NAME_LEN]);
td_s32 RequestDualTSFileNames(TD_MW_PTR pRecord, td_u32 u32FileCnt, td_char (*paszFileNames)[OT_REC_FILE_NAME_LEN]);

td_s32 GetLastFileNames(td_u32 *fileCnt, td_char (*fileNames)[OT_REC_FILE_NAME_LEN]);

#endif
