/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HI_GV_RESM_H
#define HI_GV_RESM_H

/* add include here */
#include "hi_type.h"
#include "hi_go.h"
#include "hi_gv_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UNLIMITED_REPEATE_COUNT (-1)
/* Invalid resource ID */
#define INVALID_RESID 0
/* The max length of resource file name */
#define HIGV_RESM_MAX_FILENAME_LEN 256

/* The max alpha value */
#define HIGV_RESM_MAX_ALPHA 255

/* Resource ID */
typedef HI_PARAM HI_RESID;

/* Resource type */
typedef enum hiHIGV_RESTYPE_E {
    HIGV_RESTYPE_IMG,
    /* Picture resource */

    HIGV_RESTYPE_FONT,
    /* Font resource */

    HIGV_RESTYPE_BUTT
} HIGV_RESTYPE_E;

/* Skin type */
typedef enum hiHIGV_STYLE_TYPE_E {
    HIGV_STYLETYPE_COLOR = 0,
    /* Color type */
    HIGV_STYLETYPE_PIC /* Picture type */
} HIGV_STYLE_TYPE_E;

/*  Style union */
typedef union hiHIGV_STYLE_MEMBER_U {
    HI_COLOR Color; /* Color value  */
    HI_RESID ResId; /* Resource ID */
} HIGV_STYLE_MEMBER_U;

/*  The style structure */
typedef struct hiHIGV_STYLE_S {
    HIGV_STYLE_TYPE_E StyleType; /* Style type */
    HIGV_STYLE_MEMBER_U
    Top;     /* Top side (color or picture resource) */
    HIGV_STYLE_MEMBER_U Bottom;
    /* Bottom side (color or picture resource) */
    HIGV_STYLE_MEMBER_U Left; /* Left side (color or picture resource */
    HIGV_STYLE_MEMBER_U Right;    /* Right side (color or picture resource) */

    HIGV_STYLE_MEMBER_U LeftTop;    /* Top left co (color or picture resource) */

    HIGV_STYLE_MEMBER_U LeftBottom;    /* Top side (color or picture resource) */

    HIGV_STYLE_MEMBER_U RightTop;    /* Right top side (color or picture resource) */

    HIGV_STYLE_MEMBER_U RightBottom;    /* Right bottom side (color or picture resource) */

    HIGV_STYLE_MEMBER_U BackGround;    /* Background (color or picture resource) */

    HI_COLOR FontColor; /* Font color */
    HI_U32 bNoDrawBg;   /* Is not draw background */
    HI_U32 LineWidth;   /* Line width */
    HI_U32 isRoundRect;
    HI_U32 roundRadius;
} HIGV_STYLE_S;

typedef struct hiHIGV_FONT_S {
    HI_RESID SbFontID;
    HI_RESID MbFontID;
    HI_U32 Size;
    HI_BOOL bBold;
    HI_BOOL bItalic;
    HI_U32 lineSpace;
} HIGV_FONT_S;

typedef struct hiHIGV_DEC_SUFINFO_S {
    HIGO_PF_E PixFormat;
    HIGO_MEMTYPE_E MemType;
    HI_BOOL IsPubPalette;
} HIGV_DEC_SUFINFO_S;

typedef struct {
    HI_CHAR *pTopName;         /* Resource file name of top side */
    HI_CHAR *pBottomName;      /* Resource file name of bottom side */
    HI_CHAR *pLeftName;        /* Resource file name of left side */
    HI_CHAR *pRightName;       /* Resource file name of right side */
    HI_CHAR *pLeftTopName;     /* Resource file name of top and left side */
    HI_CHAR *pLeftBottomName;  /* Resource file name of bottom and left side */
    HI_CHAR *pRightTopName;    /* Resource file name of right and up side */
    HI_CHAR *pRightBottomName; /* Resource file name of right and bottom  side */
    HI_CHAR *pBackGroundName;  /* Resource file name of background */
} HIGV_STYLEFILENAME_S;

/*  The Anim structure */
typedef enum hiHIGV_ANIM_TYPE_E {
    /* Translate type */
    HIGV_ANIMTYPE_TRANSLATE = 0,
    /* Alpha type */
    HIGV_ANIMTYPE_ALPHA,
    /* Roll type */
    HIGV_ANIMTYPE_ROLL,
    /* Any type */
    HIGV_ANIMTYPE_ANY,
    HIGV_ANIMTYPE_INVALIDATE
} HIGV_ANIM_TYPE_E;

/* Animation repeat type */
typedef enum hiHIGV_ANIM_REPEAT_TYPE_E {
    /* Repeate type restart */
    HIGV_REPEATE_TYPE_RESTART = 0,
    /* Repeate type reverse */
    HIGV_REPEATE_TYPE_REVERSE
} HIGV_ANIM_REPEAT_TYPE_E;

/* Animation roll type */
typedef enum {
    HIGV_ANIM_ROLL_UP = 0, /* Up */
    HIGV_ANIM_ROLL_DOWN,   /* Down */
    HIGV_ANIM_ROLL_LEFT,   /* Letf */
    HIGV_ANIM_ROLL_RIGHT   /* Right */
} HIGV_ANIM_ROLL_DIRECTION_E;

/* Translate info */
typedef struct hiHIGV_ANIM_TRANSLATE_INFO_S {
    HI_S32 FromX; /* ʼX */
    HI_S32 FromY; /* ʼY */
    HI_S32 ToX;   /* X */
    HI_S32 ToY;   /* Y */
} HIGV_ANIM_TRANSLATE_INFO_S;

/* Alpha info */
typedef struct hiHIGV_ANIM_ALPHA_INFO_S {
    HI_U32 FromApha; /*  ʼAlpha */
    HI_U32 ToAlpha;  /*  Alpha */
} HIGV_ANIM_ALPHA_INFO_S;

/* Any info */

typedef struct hiHIGV_ANIM_ANY_INFO_S {
    HI_S32 FromValue; /*  ʼ Value */
    HI_S32 ToValue;   /*   Value */
} HIGV_ANIM_ANY_INFO_S;

/* Roll info */

typedef struct hiHIGV_ANIM_ROLL_INFO_S {
    HIGV_ANIM_ROLL_DIRECTION_E DirectionType; /*  */
    HI_S32 FromLoc;                           /* ʼλ */
    HI_S32 ToLoc;                             /* λ */
} HIGV_ANIM_ROLL_INFO_S;

/* Animation info */

typedef struct hiHIGV_ANIM_INFO_S {
    HI_U32 AnimHandle;                  /* Anim ID */
    HI_U32 DurationMs;                  /* Duration Times */
    HI_S32 RepeatCount;                 /* Repeat Times */
    HIGV_ANIM_REPEAT_TYPE_E RepeatMode;
    HI_U32 DelayStart;
    HIGV_ANIM_TYPE_E AnimType;
    union {
        HIGV_ANIM_TRANSLATE_INFO_S Translate;
        HIGV_ANIM_ALPHA_INFO_S Alpha;
        HIGV_ANIM_ROLL_INFO_S Roll;
        HIGV_ANIM_ANY_INFO_S Any;
    } AnimParam;
} HIGV_ANIM_INFO_S;

 /*
* brief Create resource ID.
* attention \n
* 1. The picture use ID in the system.
* 2. This function is effected by resource environment var
* param[in]  fileName    Picture file name.
* param[in]  resType Resource type.
* param[out] resId   Resource ID.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_NORES No memory.
* see ::HI_GV_Res_DestroyID
*/
HI_S32 HI_GV_Res_CreateID(const HI_CHAR *fileName, HIGV_RESTYPE_E resType, HI_RESID *resID);

/*
* brief Create resource ID no res prefix path.
* attention:
* The picture use ID in the system.
*
* param[in]  fileName    Picture file name.
* param[in]  resType Resource type.
* param[out] resId   Resource ID.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_NORES No memory.
* see ::HI_GV_Res_DestroyID
*/
HI_S32 HI_GV_Res_CreateID_NoPrefixPath(const HI_CHAR *fileName, HIGV_RESTYPE_E resType, HI_RESID *resID);

/*
* brief Disable application use resource environment variables.
* If you set the reource environment variable, and don't want usnet the env variable,
* you can call this function disable.
* [HIGV_RES_FONT_PATH, HIGV_RES_IMAGE_PATH] \attention \n N/A.
* retval ::HI_SUCCESS Success.
* see :: HI_GV_Res_CreateID_NoPrefixPath
*/
HI_S32 HI_GV_Res_DisablePrefixPath(const HI_BOOL disable);

/*
* brief Destroy resource id.
* param[in] resID Rouce ID.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the resource id .
* retval ::HI_ERR_RES_USED The resource is used.
* see :: HI_GV_Res_CreateID
*/
HI_S32 HI_GV_Res_DestroyID(HI_RESID resID);

/*
* brief Generate style.
* param[in]  style   Generate the information of the skin.
* param[out] styleHandle  Skin handle.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_NORES No memory.
* retval ::HI_ERR_COMM_INVAL Invalid parameter.
\* see :: HI_GV_Res_DestroyStyle
*/
HI_S32 HI_GV_Res_CreateStyle(const HIGV_STYLE_S *style, HIGV_HANDLE *styleHandle);

/*
* brief Generate Style by the resource ID.
* param[in] style Style pointer.
* param[in] styleHandle  Style handle.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_NORES No memory.
* retval ::HI_ERR_COMM_INVAL Invalid parater.
*/
HI_S32 HI_GV_Res_CreateStyleByHandle(const HIGV_STYLE_S *style, HIGV_HANDLE styleHandle);

/*
* brief Destroy style.
* param[in] styleHandle  Style handle.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the resource id.
* see ::HI_GV_Res_CreateStyle
*/
HI_S32 HI_GV_Res_DestroyStyle(HIGV_HANDLE styleHandle);

/*
* brief Set the resource to persistent memory.
* param[in] resID Resource handle.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the resource id.
*/
HI_S32 HI_GV_Res_SetResident(HI_RESID resID);

/*
* brief Create font.
* param[in]  fontInfo the point of Font information.
* param[out] fontHandle     Font handle.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_NORES No memory.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the resource id.
* see ::HI_GV_Font_Destroy
*/
HI_S32 HI_GV_Font_Create(const HIGV_FONT_S *fontInfo, HIGV_HANDLE *fontHandle);

/*
* brief Create font by the handle.
* param[in] fontInfo
* param[in] fontHandle
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_NORES No memory.
* retval ::HI_ERR_RES_UNKNOWRES
*/
HI_S32 HI_GV_Font_CreateByHandle(const HIGV_FONT_S *fontInfo, HIGV_HANDLE fontHandle);

/*
* brief Destroy font.
* param[in] fontHandle  Font handle.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the font.
* see ::HI_GV_Font_Create
*/
HI_S32 HI_GV_Font_Destroy(HIGV_HANDLE fontHandle);

/*
* brief Set system default font.
* param[in] fontHandle Font handle.
* retval ::HI_SUCCESS
* see ::HI_GV_Font_GetSystemDefault
*/
HI_S32 HI_GV_Font_SetSystemDefault(HIGV_HANDLE fontHandle);

/*
* brief Get system default font.
* param[out] fontHandle Font handle.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_INVAL Invalid handle.
* see ::HI_GV_Font_SetSystemDefault
*/
HI_S32 HI_GV_Font_GetSystemDefault(HIGV_HANDLE *fontHandle);

/*
* brief Get String width and height.
* param[in] fontHandle Font handle.
* param[in] strHandle String handle.
* param[out] width String width.
* param[out] height String height.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_INVAL Invalid handle.
*/
HI_S32 HI_GV_Font_GetTextExtent(HIGV_HANDLE fontHandle, HI_RESID strHandle, HI_S32 *width, HI_S32 *height);

/*
* brief Destroy all style.
* attention \n
* Release the resource of style and don't rlease the picture resource.
*
* param N/A.
* retval ::HI_SUCCESS
* retval ::HI_ERR_RES_USED
*/
HI_S32 HI_GV_Resm_DestroyAllStyle(HI_VOID);

/*
* brief Destroy all resource.
* param N/A.
* retval ::HI_SUCCESS   Success.
*/
HI_S32 HI_GV_Resm_DestroyAllRes(HI_VOID);

/*
* brief Force to releaee all resource.
* param N/A.
* retval ::HI_SUCCESS   Success.
*/
HI_S32 HI_GV_Resm_ForceUnloadAllRes(HI_VOID);

/*
* brief Set the information of decode surface.
* param[in] DecSufinfo The information of decode surface.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_INVAL Invalid parameter.
*/
HI_S32 HI_GV_Resm_SetDecSurfInfo(const HIGV_DEC_SUFINFO_S *decSufInfo);

/*
* brief Get the information of decode surface.
* param[out] decSufinfo The surface information of decode surface.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_INVAL Invalid parameter.
*/
HI_S32 HI_GV_Resm_GetDecSurfInfo(HIGV_DEC_SUFINFO_S *decSufInfo);

/*
* brief Get the information of the surface which is for picture reource ID.
* param[in]  resID Resource ID.
* param[out] resHandle  the handle of resource surface.
* retval ::HI_SUCCESS  Success.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the resource id.
* retval ::HI_ERR_RES_LOAD   Load failed.
*/
HI_S32 HI_GV_Res_GetResInfo(HI_RESID resID, HIGV_HANDLE *resHandle);

/*
* brief Free the information of the surface which is for picture reource ID.
* param[in]  resID Resource ID.
* retval ::HI_SUCCESS  Success.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the resource id.
* retval ::HI_ERR_RES_NOLOAD   No Load.
*/
HI_S32 HI_GV_Res_ReleaseResInfo(HI_RESID resID);

/*
* brief Create font set.
* attention
*
* param[out] fontHandle
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the font.
* see::HI_GV_FontSet_Destroy
*/
HI_S32 HI_GV_FontSet_Create(HIGV_HANDLE *fontSet);

/*
* brief Add font to font set.
* attention
* supportLan
* param[in] fontHandle Font handle.
* param[in]  supportLan The language which font support.
* Դ?뽨ʹISO-639 ׼ \param[in]  fontSetHandle  The handle of font set.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_NORES No memory.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the reouce id.
* see ::HI_GV_FontSet_Destroy
*/
HI_S32 HI_GV_FontSet_AddFont(HIGV_HANDLE fontHandle, const HI_CHAR *supportLan, HIGV_HANDLE fontSetHandle);

/*
* brief  Query the font style which is the same as fontHandle from font set.
* param[in]  lan
* param[in] fontHandle     Font handle.
* param[out]  supprotFontHandle
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
* retval ::HI_ERR_COMM_NORES No memory.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the resource id.
* see ::HI_GV_FontSet_Destroy
*/
HI_S32 HI_GV_FontSet_Query(const HI_CHAR *lan, HIGV_HANDLE fontHandle, HIGV_HANDLE *supprotFontHandle);

/*
* brief Destroy font set.
* retval ::HI_SUCCESS Success.
* see :: HI_GV_Font_Create
*/
HI_S32 HI_GV_FontSet_Destroy(HI_VOID);

/*
* brief Get style file name.
* param[in]  styleHandle     Style handle.
* param[out] styleFileNameInfo  the file names of resource.
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_Res_GetStyleFileName(HI_RESID styleHandle, HIGV_STYLEFILENAME_S *styleFileNameInfo);

/*
* brief print resource information.
* param N/A.
* retval ::HI_SUCCESS
*/
HI_S32 HI_GV_Res_PrintCurLoadResInfo(HI_VOID);

/*
* brief Create animtion information.
* param[in]  animInfo the point of animtion information.
* param[out] animHandle  animtion information handle.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_NORES No memory.
* see ::HI_GV_AnimInfo_Destroy
*/
HI_S32 HI_GV_AnimInfo_Create(const HIGV_ANIM_INFO_S *animInfo, HIGV_HANDLE *animHandle);

/*
* brief Create animtion information by the handle.
* param[in]  animInfo the point of animation information.
* param[in]  animHandle  Anim handle.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_LOST Invalid handle.
* reval ::HI_ERR_COMM_NORES No memory.
*/
HI_S32 HI_GV_AnimInfo_CreateByHandle(const HIGV_ANIM_INFO_S *animInfo, HIGV_HANDLE animHandle);

/*
* brief Destroy font.
* param[in]  animHandle  Anim handle.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_RES_UNKNOWRES Coun't find the anim.
* see ::HI_GV_AnimInfo_Create
*/
HI_S32 HI_GV_AnimInfo_Destroy(HIGV_HANDLE animHandle);

/*
* brief Get system default font.
* param[in]  animHandle  Anim handle.
* param[out] animInfo   Anim info.
* retval ::HI_SUCCESS Success.
* retval ::HI_ERR_COMM_INVAL Invalid handle.
* see ::HI_GV_AnimInfo_Create
*/
HI_S32 HI_GV_AnimInfo_Get(HIGV_HANDLE animHandle, HIGV_ANIM_INFO_S *animInfo);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_RESM_H */
