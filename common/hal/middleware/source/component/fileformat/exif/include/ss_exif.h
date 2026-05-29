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
#ifndef OT_EXIF_H
#define OT_EXIF_H

#include "ot_mw_type.h"
#include "ot_error_def.h"
#include "ot_defs.h"

/** \addtogroup     EXIF */
/** @{ */ /** <!-- [EXIF] */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum OT_APP_EXIF_ERR_CODE_E {
    OT_APP_EXIF_ERR_NO_AVAILABLE_HANDLE = 0x40,  /** <no available tag editor handle */
    OT_APP_EXIF_ERR_NO_INVALID_HANDLE = 0x41,    /** < invalid tag editor handle */
    OT_APP_EXIF_ERR_STATUS = 0x45,               /** < err status */
    OT_APP_EXIF_ERR_INNER_FAILED = 0x46,         /** < inner call fail */
    OT_APP_EXIF_ERR_MALLOC_FAILED = 0x47,        /** < malloc fail */
    OT_APP_EXIF_ERR_RES_OCCUPY = 0x48,           /** < res used fail */
    OT_APP_EXIF_ERR_UNSUPPORT_DATA = 0x49,       /** < unsupport this data  */
    OT_APP_EXIF_INSUFFICIENT_FIXED_SPACE = 0x50, /** < insufficient prealloc space  */
    OT_APP_EXIF_ERR_FILE_OPERATE = 0x51,         /** < call system file operate fail  */
    OT_APP_EXIF_ERR_READ_DATA = 0x52,            /** < read  data fail  */
    OT_APP_EXIF_ERR_OPEN_FILE = 0x53,            /** < open file fail  */
    OT_APP_EXIF_ERR_UNSUPPORT_FILE = 0x54,       /** < unsupport this FILE  */
    OT_APP_EXIF_ERR_RES_NOT_RELEASE = 0x55,      /** < ExifInstance  not closed */
    OT_APP_EXIF_ERR_SAFE_FUNCTION = 0x56,        /** < safe function  exec fail */
    OT_APP_EXIF_ERR_TAG_NOT_EXIST = 0x57,        /** < tag not exist */
    OT_APP_EXIF_BUTT = 0xFF
} OT_APP_EXIF_ERR_CODE_E;

/* general error code */
#define OT_ERR_EXIF_NULL_PTR                 OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, APP_ERR_NULL_PTR)
#define OT_ERR_EXIF_ILLEGAL_PARAM            OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, APP_ERR_ILLEGAL_PARAM)
#define OT_ERR_EXIF_HANDLE_INVALID           OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_NO_INVALID_HANDLE)
#define OT_ERR_EXIF_NO_AVAILABLE_HANDLE      OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_NO_AVAILABLE_HANDLE)
#define OT_ERR_EXIF_STATUS                   OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_STATUS)
#define OT_ERR_EXIF_INNER_FAILED             OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_INNER_FAILED)
#define OT_ERR_EXIF_MALLOC_FAILED            OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_MALLOC_FAILED)
#define OT_ERR_EXIF_RES_OCCUPY               OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_RES_OCCUPY)
#define OT_ERR_EXIF_UNSUPPORT_DATA           OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_UNSUPPORT_DATA)
#define OT_ERR_EXIF_INSUFFICIENT_FIXED_SPACE OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_INSUFFICIENT_FIXED_SPACE)
#define OT_ERR_EXIF_FILE_OPERATE             OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_FILE_OPERATE)
#define OT_ERR_EXIF_READ_DATA                OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_READ_DATA)
#define OT_ERR_EXIF_OPEN_FILE                OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_OPEN_FILE)
#define OT_ERR_EXIF_UNSUPPORT_FILE           OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_UNSUPPORT_FILE)
#define OT_ERR_EXIF_RES_NOT_RELEASE          OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_RES_NOT_RELEASE)
#define OT_ERR_EXIF_SAFE_FUNCTION            OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_SAFE_FUNCTION)
#define OT_ERR_EXIF_TAG_NOT_EXIST            OT_APP_DEF_ERR(OT_APPID_TAG_EDITOR, \
    APP_ERR_LEVEL_ERROR, OT_APP_EXIF_ERR_TAG_NOT_EXIST)

enum OT_EXIF_CNT_PARAM {
    OT_EXIF_SUBIMG_MAX_CNT = 6,
    OT_EXIF_DATA_SEG_MAX_CNT = 8
};

typedef struct EXIF_RATIONAL_S {
    TD_U32 numerator;
    TD_U32 denominator;
} OT_EXIF_RATIONAL_S;

typedef struct EXIF_SRATIONAL_S {
    TD_S32 numerator;
    TD_S32 denominator;
} OT_EXIF_SRATIONAL_S;

typedef enum EXIF_DATA_TYPE_E {
    OT_EXIF_DATA_TYPE_BYTE = 1,        // !< 8-bit unsigned integer
    OT_EXIF_DATA_TYPE_ASCII = 2,       // !< 8-bit bytes w/ last byte null
    OT_EXIF_DATA_TYPE_SHORT = 3,       // !< 16-bit unsigned integer
    OT_EXIF_DATA_TYPE_LONG = 4,        // !< 32-bit unsigned integer
    OT_EXIF_DATA_TYPE_RATIONAL = 5,    // !< 64-bit unsigned fraction
    OT_EXIF_DATA_TYPE_UNDEFINED = 7,   // !< 8-bit untyped data
    OT_EXIF_DATA_TYPE_SLONG = 9,       // !< 32-bit signed integer */
    OT_EXIF_DATA_TYPE_SRATIONAL = 10,  // !< 64-bit signed fraction */
} OT_EXIF_DATA_TYPE_E;

typedef enum EXIF_TAG_LOCATION_E {
    OT_EXIF_APP1_IFD0 = 0,
    OT_EXIF_APP1_EXIFIFD,
    OT_EXIF_APP1_EXIFINTEROP_IFD,
    OT_EXIF_APP1_IFD0_GPSINFO_IFD,
    OT_EXIF_APP1_IFD1,
} OT_EXIF_TAG_LOCATION_E;

typedef struct EXIF_SUBIMG_S {
    TD_U32 u32Width;    /** < The height of the media file's resolution */
    TD_U32 u32Height;   /** < The width of the media file's resolution */
    TD_U32 u32DataLen;  /** < The data len of sub img */
    TD_BOOL bThumbnail; /* true:this is the thumbnail */
} OT_EXIF_SUBIMG_S;

typedef struct EXIF_INFO_S {
    TD_S64 s64FileSize;
    OT_EXIF_SUBIMG_S stSubImg[OT_EXIF_SUBIMG_MAX_CNT];
} OT_EXIF_INFO_S;

typedef struct EXIF_DATA_S {
    TD_U32 u32PicIndex;
    TD_U8 *pu8Data;
    TD_U32 u32DataLen;
    TD_U32 u32HasReadDataLen; /* output param */
} OT_EXIF_DATA_S;

typedef struct EXIF_TAG_ENTRY_S {
    TD_U16 u16TagNum;
    OT_EXIF_DATA_TYPE_E enDataType;
    TD_U32 u32Cnt;
    const void *pValue; /** < int array, char array and so on */
} OT_EXIF_TAG_ENTRY_S;

typedef struct EXIF_CONFIG_DEMUXER_S {
    /** < the input image file name ,
     * the length of pszFileName shoud be in (0,PATH_MAX]
     */
    const char *pszFileName;
} OT_EXIF_CONFIG_DEMUXER_S;

typedef struct EXIF_CONFIG_EDITOR_S {
    /** < the output file name  for img data , the length of pszFileName shoud be in (0,PATH_MAX] */
    const char *pszFileName;
    /** < TD_FALSE:if  pszFileName exist then return error; TD_TRUE: if pszFileName exist then overwrite it */
    TD_BOOL bIsOverWrite;
    /** < 0: do not limit file size,  other value: the output file size can not be larger than  u32FixedFileSize */
    TD_U32 u32FixedFileSize;
} OT_EXIF_CONFIG_EDITOR_S;

typedef enum EXIF_CONFIG_TYPE_E {
    OT_EXIF_CONFIG_DEMUXER = 0, /** < exif module for demuxer */
    OT_EXIF_CONFIG_EDITOR,      /** < exif module for editor */
} OT_EXIF_CONFIG_TYPE_E;

typedef struct EXIF_ATTR_S {
    OT_EXIF_CONFIG_TYPE_E enExifConfigType;
    union {
        OT_EXIF_CONFIG_EDITOR_S stEditorConfig;
        OT_EXIF_CONFIG_DEMUXER_S stDemuxerConfig;
    };
} OT_EXIF_ATTR_S;

typedef struct EXIF_BUFFER_SOURCE_S {
    TD_VOID *pImgBuf[OT_EXIF_DATA_SEG_MAX_CNT];
    TD_U32 u32BufSize[OT_EXIF_DATA_SEG_MAX_CNT];
} OT_EXIF_BUFFER_SOURCE_S;

/**
*   @brief hasInit exif module
*   @param[in] N/A
*   @retval  0 success,others fail
*/
TD_S32 SS_EXIF_Init(TD_VOID);

/**
*   @brief open exif ExifInstance
*   @param[out] ppExif: the address of the pointer of exif
*   @param[in] pstExifAttr : the attribute of the exif ExifInstance
*   @retval  0 success,others fail
*/
TD_S32 SS_EXIF_Open(TD_MW_PTR *exif, OT_EXIF_ATTR_S *exifAttr);

/**
*   @brief get picture infomation
*   @param[in] exif : pointer of exif ExifInstance
*   @param[out] pstExifInfo : the address of exif  image info
*   @retval  0 success,others fail
*/
TD_S32 SS_EXIF_GetInfo(TD_MW_PTR exif, OT_EXIF_INFO_S *exifInfo);

/**
*   @brief get picture tag infomation
*   @param[in] exif : pointer of exif ExifInstance
*   @param[in] enExifTagLocation : the location where the tag insert
*   @param[in] u16TagNum : tag number
*   @param[out] te : pointer of tag entry  info
*   @retval  0 success,others fail
*/
TD_S32 SS_EXIF_GetTag(TD_MW_PTR exif, OT_EXIF_TAG_LOCATION_E tagLocation, TD_U16 tag, OT_EXIF_TAG_ENTRY_S *te);

/**
*   @brief read picture data
*   @param[in] exif : pointer of exif ExifInstance
*   @param[out] pstExifData : the address of exif data info
*   @retval  0 success,others fail
*/
TD_S32 SS_EXIF_ReadPicData(TD_MW_PTR exif, OT_EXIF_DATA_S *exifData);

/**
*   @brief set the picture memory data
*   @param[in] exif : pointer of exif ExifInstance
*   @param[in] pstBufSource :  image data source info include data address and data size.
*   @retval  0 success,others fail
*/
TD_S32 SS_EXIF_SetBufferSource(TD_MW_PTR exif, OT_EXIF_BUFFER_SOURCE_S *bufSource);

/**
*   @brief set or update tag of the exif picture
*   @param[in] exif : pointer of exif ExifInstance
*   @param[in] pstTagEntry : pointer of tag entry  info
*   @param[in] enExifTagLocation : the location where the tag insert
*   @retval  0 success,others fail
*/
TD_S32 SS_EXIF_SetTag(TD_MW_PTR exif, OT_EXIF_TAG_ENTRY_S *te, OT_EXIF_TAG_LOCATION_E tagLocation);

/**
*   @brief close exif ExifInstance
*   @param[in] exif : pointer of exif ExifInstance
*   @retval  0 success,others fail
*/
TD_S32 SS_EXIF_Close(TD_MW_PTR exif);

/**
*   @brief deinit exif module
*   @param[in] N/A
*   @retval  0 success,others fail
*/
TD_S32 SS_EXIF_DeInit(TD_VOID);

/** @} */ /** <!-- ==== EXIF  End ==== */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __OT_EXIF_H__ */
