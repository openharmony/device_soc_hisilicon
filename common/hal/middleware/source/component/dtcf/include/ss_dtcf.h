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

#ifndef OT_DTCF_H_
#define OT_DTCF_H_

#include "ot_mw_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/** \addtogroup     DTCF */
/* [DTCF] */

#define MODULE_NAME_DTCF  "DTCF"

#define OT_FILE_PATH_LEN_MAX         (256)
#define OT_DIR_LEN_MAX               (64)

/* DataTime Error number base */
#define OT_DTCF_ERRNO_BASE 0x2A00
/* Parameter is invalid */
#define OT_DTCF_ERR_EINVAL_PAEAMETER    (OT_DTCF_ERRNO_BASE + 1)
/* Null pointer */
#define OT_DTCF_ERR_NULL_PTR            (OT_DTCF_ERRNO_BASE + 2)
/* failure caused by malloc memory */
#define OT_DTCF_ERR_NOMEM               (OT_DTCF_ERRNO_BASE + 3)
/* not support file path */
#define OT_DTCF_UNSUPPORT_PATH          (OT_DTCF_ERRNO_BASE + 4)
/* the same directory */
#define OT_DTCF_SAME_DIR_PATH           (OT_DTCF_ERRNO_BASE + 5)
/* Status error, operation not allowed */
#define OT_DTCF_ERR_STATUS_ERROR        (OT_DTCF_ERRNO_BASE + 6)
/* Empty directory */
#define OT_DTCF_ERR_EMPTEY_DIR          (OT_DTCF_ERRNO_BASE + 7)
/* directory not define */
#define OT_DTCF_ERR_UNDEFINE_DIR        (OT_DTCF_ERRNO_BASE + 8)
/* System error */
#define OT_DTCF_ERR_SYSTEM_ERROR        (OT_DTCF_ERRNO_BASE + 9)
/* the same filename */
#define OT_DTCF_SAME_FILENAME_PATH      (OT_DTCF_ERRNO_BASE + 10)
/* the path is not a directory */
#define OT_DTCF_PATH_IS_NOT_DIR_ERROR   (OT_DTCF_ERRNO_BASE + 11)

/**
 * File Directory Type
 */
typedef enum {
    DTCF_DIR_EMR_FRONT = 0,
    DTCF_DIR_EMR_FRONT_SUB,
    DTCF_DIR_NORM_FRONT,
    DTCF_DIR_NORM_FRONT_SUB,
    DTCF_DIR_PARK_FRONT,
    DTCF_DIR_PARK_FRONT_SUB,
    DTCF_DIR_EMR_REAR,
    DTCF_DIR_EMR_REAR_SUB,
    DTCF_DIR_NORM_REAR,
    DTCF_DIR_NORM_REAR_SUB,
    DTCF_DIR_PARK_REAR,
    DTCF_DIR_PARK_REAR_SUB,
    DTCF_DIR_PHOTO_FRONT,
    DTCF_DIR_PHOTO_REAR,
    DTCF_DIR_BUTT
} OT_DTCF_DIR_E;

/**
 * File Type.
 */
typedef enum {
    OT_DTCF_FILE_TYPE_MP4,
    OT_DTCF_FILE_TYPE_JPG,
    OT_DTCF_FILE_TYPE_TS,
    OT_DTCF_FILE_TYPE_DNG,
    OT_DTCF_FILE_TYPE_BUTT
} OT_DTCF_FILE_TYPE_E;

/**
  *    @brief init DTCF
  *
  *    init DTCF
  *    @param[in]  pazRootDir:  root of DTCF
  *    @param[in]  azDirNames: level two directory names
  *    @return     0 success, non-zero error code
  */
TD_S32 SS_DTCF_Init(const TD_CHAR *pazRootDir, const TD_CHAR azDirNames[DTCF_DIR_BUTT][OT_DIR_LEN_MAX]);

/**
  *    @brief get level two directory names
  *
  *    create DTCF
  *    @param[out] pazDirNames: level two directory names
  *    @param[in]  u32DirAmount: level two directory amount
  *    @return     0 success, non-zero error code
  */
TD_S32 SS_DTCF_GetDirNames(TD_CHAR (*pazDirNames)[OT_DIR_LEN_MAX], TD_U32 u32DirAmount);

/**
  *    @brief DeInit DTCF
  *
  *    DeInit DTCF
  *    @return     0 success, non-zero error code
  */
TD_S32 SS_DTCF_DeInit(void);

/**
  *    @brief scan files
  *
  *    scan files in some directories.
  *    @param[in]    enDirs:  directories to scan.
  *    @param[in]    u32DirCount: count of directory.
  *    @param[out]   pu32FileAmount:files amount.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_Scan(const OT_DTCF_DIR_E enDirs[DTCF_DIR_BUTT], TD_U32 u32DirCount, TD_U32 *pu32FileAmount);

/**
  *    @brief get file by index.
  *    @param[in]    u32Index:  index of file to get.
  *    @param[out]   pazFileName: file name.
  *    @param[in]    u32Length:  file length .
  *    @param[out]   penDir: file directory.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_GetFileByIndex(TD_U32 u32Index, TD_CHAR *pazFileName, TD_U32 u32Length, OT_DTCF_DIR_E *penDir);

/**
  *    @brief delete file by index.
  *    @param[in]    u32Index:  index of file to delete.
  *    @param[out]   pu32FileAmount: file amount.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_DelFileByIndex(TD_U32 u32Index, TD_U32 *pu32FileAmount);

/**
  *    @brief add file.
  *    @param[in]    pazSrcFilePath:  file path to add.
  *    @param[in]    enDir:  file dir type to add.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_AddFile(const TD_CHAR *pazSrcFilePath, OT_DTCF_DIR_E enDir);

/**
  *    @brief get oldest file index.
  *    @param[in]    enDirs[DTCF_DIR_BUTT]:  the oldest file enDir.
  *    @param[in]    u32DirCount:  the oldest file enDir count.
  *    @param[out]    pu32Index:  the oldest file index.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_GetOldestFileIndex(const OT_DTCF_DIR_E enDirs[DTCF_DIR_BUTT], TD_U32 u32DirCount, TD_U32 *pu32Index);

/**
  *    @brief get file amount.
  *    @param[out]   pu32FileAmount: file amount.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_GetFileAmount(TD_U32 *pu32FileAmount);

/**
  *    @brief get oldest file in directory.
  *    @param[in]    enDirs:  directory type.
  *    @param[out]   pazFilePath: get oldest file full path.
  *    @param[in]   u32Length: get oldest file full path length.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_GetOldestFilePath(OT_DTCF_DIR_E enDir, TD_CHAR *pazFilePath, TD_U32 u32Length);

/**
  *    @brief create file full path
  *    @param[in]    enFileType:  type of file.
  *    @param[in]    enDir: type of directory.
  *    @param[out]   pazFilePath: file full path.
  *    @param[in]   u32Length: file full path length.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_CreateFilePath(OT_DTCF_FILE_TYPE_E enFileType, OT_DTCF_DIR_E enDir,
    TD_CHAR *pazFilePath, TD_U32 u32Length);

/**
  *    @brief get related file full path.
  *    @param[in]    pazSrcFilePath:  source file full path.
  *    @param[in]    enDir: related file directory.
  *    @param[out]   pazDstFilePath : related file full path.
  *    @param[in]   u32Length: file full path length.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_GetRelatedFilePath(const TD_CHAR *pazSrcFilePath, OT_DTCF_DIR_E enDir,
    TD_CHAR *pazDstFilePath, TD_U32 u32Length);

/**
  *    @brief get emergency file full path.
  *    @param[in]    pazSrcFilePath:  source file full path.
  *    @param[out]   pazDstFilePath : Emr file full path.
  *    @param[in]   u32Length: file full path length.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_GetEmrFilePath(const TD_CHAR *pazSrcFilePath, TD_CHAR *pazDstFilePath, TD_U32 u32Length);

/**
  *    @brief get file directory type.
  *    @param[in]    pazSrcFilePath:  source file full path.
  *    @param[out]   penDir : file directory type.
  *    @return       0 success, non-zero error code
  */
TD_S32 SS_DTCF_GetFileDirType(const TD_CHAR *pazSrcFilePath, OT_DTCF_DIR_E *penDir);

/* DTCF End */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* OT_DTCF_H_ */
