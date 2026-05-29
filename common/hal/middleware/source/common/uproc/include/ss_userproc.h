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
 */

#ifndef OT_USERPROC_H
#define OT_USERPROC_H

#ifdef CPP_UPROC_USE
#include <string>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifndef CPP_UPROC_USE
#define OT_UPROC_OT_DIR_NAME "ot"
#define OT_UPROC_MW_DIR_NAME "middleware"
#else
const std::string OT_UPROC_OT_DIR_NAME = "ot";
const std::string OT_UPROC_MW_DIR_NAME = "middleware";
#endif
typedef enum {
    UPROC_FALSE = 0,
    UPROC_TRUE  = 1,
} UPROC_BOOL;

typedef enum {
    UPROC_ERR_NULL_PTR = 0x40,
    UPROC_ERR_INVALID_PARAM = 0x41,
    UPROC_ERR_ADD_ERROR = 0x60,
    UPROC_ERR_REMOVE_ERROR = 0x61,
    UPROC_ERR_INIT_ERROR = 0x81,
    UPROC_ERR_DEINIT_ERROR = 0x82,
    UPROC_ERR_BUTT = 0xFF
} OT_APP_UPROC_ERR_CODE_E;

/* * Module ID flags */
typedef enum {
    OT_UPROC_ID_DTCF = 0xC0,
    OT_UPROC_ID_RECORD,
    OT_UPROC_ID_MUXTER,
    OT_UPROC_ID_DEMUXTER,
    OT_UPROC_ID_DCF,
    OT_ID_USR_SAMPLE,
    OT_UPROC_ID_BUTT = 0xFF
} OT_UPROC_MOD_ID_E;

/* Defines user mode proc show buffer */
/* CNcomment: ??PROC buffer */
typedef struct OT_UPROC_PROC_S {
    void *buffer; /* *<Buffer address */ /* *<CNcomment: Buffer? */
    unsigned int  bufferSize; /* *<Buffer size */   /* *<CNcomment: Buffer?им3 */
    unsigned int bufferOffset; /* *<Offset */      /* *<CNcomment: ???? */
} OT_UPROC_SHOW_BUFFER_S;

/* UProc command function */
/* CNcomment: UProc??? */
typedef int (*SS_UPROC_SHOW_FN)(OT_UPROC_SHOW_BUFFER_S *showData, void *privData);

/* UProc command function */
/* CNcomment: UProc?? */
typedef int (*SS_UPROC_CMD_FN)(OT_UPROC_SHOW_BUFFER_S *showBuffer, unsigned int argc,
    unsigned char *argv[], void *privData);

/* Defines user mode proc entry */
/* CNcomment: ??PROC? */
typedef struct {
    char *entryName; /* Entry name CNcomment: ? */
    char *dirName; /* Directory name. If null, the entry will be added to /proc/ot directory */
    /* CNcomment: ?? */
    SS_UPROC_CMD_FN cmdProcFun;   /* * UProc command function  CNcomment: UProc?? */
    SS_UPROC_SHOW_FN showProcFun; /* * UProc show function  CNcomment: UProc??? */
    void *privData;           /* * Private data CNcomment: ??? */
} OT_UPROC_ENTRY_S;

/* ****************************** API declaration **************************** */
void SS_UPROC_Enable(UPROC_BOOL enable);
int SS_UPROC_Init(void);
int SS_UPROC_DeInit(void);
int SS_UPROC_AddDir(const char *dirName);
int SS_UPROC_RemoveDir(const char *dirName);
int SS_UPROC_AddEntry(unsigned int moduleID, const OT_UPROC_ENTRY_S *entry);
int SS_UPROC_RemoveEntry(unsigned int moduleID, const OT_UPROC_ENTRY_S *entry);
void SS_UPROC_Printf(OT_UPROC_SHOW_BUFFER_S *show, const char *fmt, ...);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* OT_USERPROC_H */
