/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __HI_COMMON_H__
#define __HI_COMMON_H__

#include "hi_type.h"
#include "hi_log.h"
#ifndef __KERNEL__
#include <time.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*******************************Structure declaration *****************************/
/* @addtogroup     COMMON */
/* @{ */ /** <!--  [COMMON] */

#define HI_USLEEP(us)                       \
    do {                                    \
        struct timespec ts;                 \
        ts.tv_sec = us / 1000000;           \
        ts.tv_nsec = (us % 1000000) * 1000; \
        nanosleep(&ts, NULL);               \
    } while (0)

/** Global config structure */
typedef struct hiSYS_CONF_S {
    /* <Not used, reserved for extension */
    /* <CNcomment: ÔÝÊ±Ã»ÓÝÊ¹ÓÃ£¬Ýô´ýÀ©Õ¹ */
    hi_u32 u32Reverse;
} HI_SYS_CONF_S;

/* Define the chip type. */
typedef enum hiCHIP_TYPE_E {
    HI_CHIP_TYPE_HI3751,

    HI_CHIP_TYPE_BUTT
} HI_CHIP_TYPE_E;

/* Define the chip version. */
typedef enum hiCHIP_VERSION_E {
    HI_CHIP_VERSION_V300 = 0x300,
    HI_CHIP_VERSION_V310 = 0x310,
    HI_CHIP_VERSION_V320 = 0x320,
    HI_CHIP_VERSION_V330 = 0x330,
    HI_CHIP_VERSION_V350 = 0x350,
    HI_CHIP_VERSION_V351 = 0x351,
    HI_CHIP_VERSION_BUTT
} HI_CHIP_VERSION_E;

/* Define the chip support attrs */
typedef enum hiCHIP_CAP_E {
    HI_CHIP_CAP_DOLBY,
    HI_CHIP_CAP_DTS,
    HI_CHIP_CAP_ADVCA,
    HI_CHIP_CAP_MACROVISION,
    HI_CHIP_CAP_DD,
    HI_CHIP_CAP_DBX,

    HI_CHIP_CAP_BUTT
} HI_CHIP_CAP_E;

/* System version, that is, the version of the software developer's kit (SDK) */
typedef struct hiSYS_VERSION_S {
    HI_CHIP_TYPE_E
    /* <Chip type corresponding to the SDK */
    /* <CNcomment:  SDKÈí¼þ¶ÔÓ¦µÄÝ¾Æ¬ÀàÝÝ */
    enChipTypeSoft;
    HI_CHIP_TYPE_E
    /* <Chip type that is detected when the SDK is running */
    /* <CNcomment:  SDKÔËÝÝÊ±¼ì²âµ½µÄÝ¾Æ¬ÀàÝÝ */
    enChipTypeHardWare;
    HI_CHIP_VERSION_E
    /* <Chip version that is detected when the SDK is running */
    /* <CNcomment: SDKÔËÝÝÊ±¼ì²âµ½Ý¾Æ¬°æ±¾ºÅ */
    enChipVersion;
    /* <Version string of the SDK */
    /* <CNcomment:  SDKÈí¼þ°æ±¾ºÅ×Ö·û´® */
    hi_char aVersion[80]; /* 80 ��ʾSDK Version��󳤶� */
    /* <Version string of the Boot */
    /* <CNcomment:  Boot°æ±¾ºÅ×Ö·û´® */
    hi_char BootVersion[80]; /* 80 ��ʾBoot Version��󳤶� */
} HI_SYS_VERSION_S;

/* Define the chip attributes */
typedef struct hiSYS_CHIP_ATTR_S {
    /* <Whether this chip support dolby or not */
    /* <CNcomment:Ý¾Æ¬ÊÇ·ñÖ§³Ö¶Å±È */
    hi_bool bDolbySupport;
    /* <the unique chipid */
    /* <CNcomment:Ý¾Æ¬Î¨Ò»ID */
    hi_u64 u64ChipID;
} HI_SYS_CHIP_ATTR_S;

/* Maximum bytes of a buffer name */
#define MAX_BUFFER_NAME_SIZE 32

/* Structure of an MMZ buffer */
typedef struct hiMMZ_BUF_S {
    /* <Strings of an MMZ buffer name */
    /* <CNcomment:  MMZ bufferÃû×Ö×Ö·û´® */
    hi_char bufname[MAX_BUFFER_NAME_SIZE];
    /* <Physical address of an MMZ buffer */
    /* <CNcomment:  MMZ bufferÎïÀíµØÖ· */
    hi_u32 phyaddr;
    /* <Kernel-state virtual address of an MMZ buffer */
    /* <CNcomment:  MMZ bufferÄÚºËÌ¬ÝéÄâµØÖ· */
    hi_u8 *kernel_viraddr;
    /* <User-state virtual address of an MMZ buffer */
    /* <CNcomment:  MMZ bufferÓÃ»§Ì¬ÝéÄâµØÖ· */
    hi_u8 *user_viraddr;
    /* <Size of an MMZ buffer */
    /* <CNcomment:  MMZ buffer´óÝ¡ */
    hi_u32 bufsize;
    /* <Overflow threshold of an MMZ buffer, in percentage. For example, the value 100 indicates 100%. */
    /* <CNcomment:  MMZ bufferÉÝÒçË®Ýß£¬°´°Ù·Ö±ÈÉèÖÃ£¬ÀýÈç: 100 indicates 100%. */
    hi_u32 overflow_threshold;
    /* <Underflow threshold of an MMZ buffer, in percentage. For example, the value 0 indicates 0%. */
    /* <CNcomment:  MMZ bufferÝÂÒçË®Ýß£¬°´°Ù·Ö±ÈÉèÖÃ£¬ÀýÈç: 0 indicates 0%. */
    hi_u32 underflow_threshold;
} HI_MMZ_BUF_S;

typedef struct hiRECT_S {
    hi_s32 s32X;
    hi_s32 s32Y;
    hi_s32 s32Width;
    hi_s32 s32Height;
} HI_RECT_S;

typedef struct hiRANGE_S {
    hi_u32 u32Min; /* <the minimum value */ /* <CNcomment È¡Öµ·¶Î§×îÝ¡Öµ */
    hi_u32 u32Max; /* <the maximum value */ /* <CNcomment È¡Öµ·¶Î§×î´óÖµ */
} HI_RANGE_S;

typedef struct hiCOLOR_S {
    hi_u32 u32R; /* <the value of red color */   /* <CNcomment ºìÉ«·ÖÝ¿ */
    hi_u32 u32G; /* <the value of green color */ /* <CNcomment ÂÌÉ«·ÖÝ¿ */
    hi_u32 u32B; /* <the value of blue color */  /* <CNcomment À¶É«·ÖÝ¿ */
} HI_COLOR_S;

typedef enum hiLAYER_ZORDER_E {
    HI_LAYER_ZORDER_MOVETOP = 0,
    /* <Move to the top */ /* <CNcomment:  ÒÆµ½×î¶¥²¿ */
    HI_LAYER_ZORDER_MOVEUP,
    /* <Move up */ /* <CNcomment:  ÝòÉÝÒÆµ½ */
    HI_LAYER_ZORDER_MOVEBOTTOM,
    /* <Move to the bottom */ /* <CNcomment:  ÒÆµ½×îµ×²¿ */
    HI_LAYER_ZORDER_MOVEDOWN,
    /* <Move down */ /* <CNcomment:  ÝòÝÂÒÆµ½ */
    HI_LAYER_ZORDER_BUTT
} HI_LAYER_ZORDER_E;

typedef enum hiSYS_POLICY_TYPE {
    HI_SYS_POLICY_EXTRA_BUFF = 0,
    HI_SYS_POLICY_CTS_FLAG = 1,
    HI_SYS_POLICY_GTS_FLAG = 2,
    HI_SYS_POLICY_DISP_WIDTH = 3,
    HI_SYS_POLICY_VCODEC_WIDTH = 4,
    HI_SYS_POLICY_BUTT
} HI_SYS_POLICY_TYPE_E;

typedef struct {
    hi_bool bPolicyValid;
    hi_u32 u32PolicyValue;
} HI_SYS_POLICY_S;

/* Defines user mode proc show buffer */
/* CNcomment: ÓÃ»§Ì¬PROC buffer¶¨Òå */
typedef struct hiPROC_SHOW_BUFFER_S {
    hi_u8 *pu8Buf; /* <Buffer address */ /* <CNcomment: BufferµØÖ· */
    hi_u32 u32Size; /* <Buffer size */ /* <CNcomment: Buffer´óÝ¡ */
    hi_u32 u32Offset; /* <Offset */ /* <CNcomment: ´òÓ¡Æ«ÒÆµØÖ· */
} HI_PROC_SHOW_BUFFER_S;

/* Proc show function */
/* CNcomment: ProcÝÅÝ¢ÝÔÊ¾»Øµ÷º¯Êý */
typedef hi_s32 (*HI_PROC_SHOW_FN)(HI_PROC_SHOW_BUFFER_S *pstBuf, hi_void *pPrivData);

/* Proc command function */
/* CNcomment: Proc¿ØÖÆ»Øµ÷º¯Êý */
typedef hi_s32 (*HI_PROC_CMD_FN)(HI_PROC_SHOW_BUFFER_S *pstBuf, hi_u32 u32Argc, hi_u8 *pu8Argv[],
    hi_void *pPrivData);

/* Defines user mode proc entry */
/* CNcomment: ÓÃ»§Ì¬PROCÈë¿Ú¶¨Òå */
typedef struct hiPROC_ENTRY_S {
    hi_char *pszEntryName; /* <Entry name */ /* <CNcomment: Èë¿ÚÎÄ¼þÃû */
    /* <Directory name. If null, the entry will be added to /proc/hisi directory */
    hi_char *pszDirectory;
    /* <CNcomment: Ä¿Â¼Ãû£¬Èç¹ûÎª¿Õ£¬½«´´½¨µ½/proc/hisiÄ¿Â¼ÝÂ */
    HI_PROC_SHOW_FN
    pfnShowProc; /* <Proc show function */ /* <CNcomment: ProcÝÅÝ¢ÝÔÊ¾»Øµ÷º¯Êý */
    HI_PROC_CMD_FN pfnCmdProc; /* <Proc command function */ /* <CNcomment: Proc¿ØÖÆ»Øµ÷º¯Êý */
    hi_void *pPrivData; /* <Private data */ /* <CNcomment: BufferµØÖ· */
} HI_PROC_ENTRY_S;

/* Defines DDR configuration type struct */
/* CNcomment: DDR ÅäÖÃÀàÝÝ½á¹¹¶¨Òå */
typedef struct hiSYS_MEM_CONFIG_S {
    hi_u32 u32TotalSize; /* Total memory size(MB) */ /* <CNcomment: ×ÜÄÚ´æ´óÝ¡(MB) */
    hi_u32 u32MMZSize; /* MMZ memory size(MB) */ /* <CNcomment: MMZÄÚ´æ´óÝ¡(MB) */
} HI_SYS_MEM_CONFIG_S;

/* Defines common flag type struct */
typedef struct {
    hi_s32 cts_flag;
    hi_s32 gts_flag;
} HI_SYS_COMM_FLAG_S;

#ifndef __KERNEL__
/* @} */ /* <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/* \addtogroup      COMMON */
/* @{ */ /* <!-- [COMMON] */

/**
@brief Initializes the system. CNcomment: ÝµÝ³³õÊ¼»¯ CNend
@attention \n
You must call this API to initialize the system before using the APIs of all modules.
Though you can call other APIs successfully before calling this API, the subsequent operations may fail.\n
CNcomment: ÔÚÊ¹ÓÃËùÓÝÄ£¿éµÄ½Ó¿ÚÖ®Ç°¶¼ÝèÒªÝÈµ÷ÓÃ´Ë½Ó¿Ú¶ÔÝµÝ³½øÝÝ³õÊ¼»¯\n
ÔÚµ÷ÓÃÕâ¸ö½Ó¿ÚÖ®Ç°µ÷ÓÃÆäËû½Ó¿Ú£¬²»»á·µ»ØÊ§°Ü£¬µ«ÊÇ²»±£Ö¤Ö´ÝÝµÄÕýÈ·ÝÔ CNend
@param N/A CNcomment: ÎÞ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_Init(void);

/**
@brief Deinitializes the system. CNcomment: ÝµÝ³È¥³õÊ¼»¯ CNend
@attention \n
If all modules are not used, you need to call this API to deinitialize the system.\n
CNcomment: ËùÓÝÄ£¿é¶¼²»ÔÙÊ¹ÓÃºóµ÷ÓÃ´Ë½Ó¿ÚÈ¥³õÊ¼»¯ CNend
@param N/A CNcomment: ÎÞ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_DeInit(void);

/**
@brief Obtains the compiled time of a version. CNcomment: »ñÈ¡°æ±¾µÄ±àÒëÊ±¼ä CNend
@attention \n
The compiled time is the time during which the common module is made again.
CNcomment: Ê±¼äÎª½øÝÝcommonÄ£¿éÖØÝÂmakeµÄÊ±¼ä CNend
@param[out] pstTime Pointer to the compiled time of a version (output).
    CNcomment: Ö¸ÕëÀàÝÝ£¬Êä³ö°æ±¾±àÒëµÄÊ±¼ä¡£ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_GetBuildTime(struct tm *pstTime);

/**
@brief Obtains the version number. CNcomment: »ñÈ¡°æ±¾ºÅ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[out] pstVersion Pointer to the version number (output). CNcomment: Ö¸ÕëÀàÝÝ£¬Êä³ö°æ±¾ºÅ¡£ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_GetVersion(HI_SYS_VERSION_S *pstVersion);

/**
@brief Obtains the chip support attributes. CNcomment: »ñÈ¡Ý¾Æ¬Ö§³ÖµÄÄÜÝ¦ÊôÝÔ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] enChipCap Indicate which chip capability. CNcomment: ÊäÈë²ÎÊý£¬Ö¸Ã÷»ñÈ¡ºÎÖÖÝ¾Æ¬ÄÜÝ¦ÊôÝÔCNend
@param[out] pbSupport Pointer to the chip whether support the attributes(output).
    CNcomment: Êä³ö²ÎÊý£¬±£´æ»ñÈ¡½á¹û CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_GetChipCapability(HI_CHIP_CAP_E enChipCap, hi_bool *pbSupport);

/**
@brief Obtains the chip attributes. CNcomment: »ñÈ¡Ý¾Æ¬ÊôÝÔ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[out] pstChipAttr Pointer to the chip attributes(output). CNcomment: Ö¸ÕëÀàÝÝ£¬Êä³öÝ¾Æ¬ÊôÝÔ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_GetChipAttr(HI_SYS_CHIP_ATTR_S *pstChipAttr);

/**
@brief Obtains the chip attributes. CNcomment: »ñÈ¡ÄÚ´æÅäÖÃÝÅÝ¢ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[out] pstConfig Pointer to address for memory configuration(output).
    CNcomment: Ö¸ÕëÀàÝÝ£¬Êä³öÄÚ´æÅäÖÃÝÅÝ¢Ö¸Õë CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_GetMemConfig(HI_SYS_MEM_CONFIG_S *pstConfig);

/**
@brief Performs global system configuration. CNcomment: ÉèÖÃÝµÝ³µÄÈ«¾ÖÅäÖÃ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] pstSysConf Pointer to the address for system configuration
    CNcomment: Ö¸ÕëÀàÝÝ£¬ÝµÝ³ÅäÖÃÖ¸ÕëµØÖ·¡£ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_SetConf(const HI_SYS_CONF_S *pstSysConf);

/**
@brief Obtains global system configuration. CNcomment: »ñÈ¡ÝµÝ³µÄÈ«¾ÖÅäÖÃ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[out] pstSysConf Pointer to the system configuration (output). CNcomment: Ö¸ÕëÀàÝÝ£¬Êä³öÝµÝ³ÅäÖÃ¡£ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_GetConf(HI_SYS_CONF_S *pstSysConf);

/**
@brief Get policy. CNcomment: CNend
@attention \n
N/A CNcomment:CNend
@param[out] policy Pointer to the value (output) CNcomment:CNend
@retval ::HI_SUCCESS Success CNcomment:CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment:CNend
@see \n
N/A CNcomment:CNend
*/
hi_s32 HI_SYS_GetPolicy(HI_SYS_POLICY_TYPE_E enPolicyType, HI_SYS_POLICY_S *pstPolicy);

/**
@brief Set policy. CNcomment: CNend
@attention \n
N/A CNcomment:CNend
@param[out] policy Pointer to the value (output) CNcomment:CNend
@retval ::HI_SUCCESS Success CNcomment:CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment:CNend
@see \n
N/A CNcomment:CNend
*/
hi_s32 HI_SYS_SetPolicy(HI_SYS_POLICY_TYPE_E enPolicyType, HI_SYS_POLICY_S *pstPolicy);

/**
@brief Sets the debugging information level of a module. CNcomment: ÉèÖÃÄ£¿éµÄµ÷ÊÔÝÅÝ¢¼¶±ð CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] enModId Debugging ID of a module CNcomment: Ä£¿éµÄµ÷ÊÔID¡£ CNend
@param[in] enLogLevel Debugging information level of a module CNcomment: Ä£¿éµÄµ÷ÊÔÝÅÝ¢¼¶±ð¡£ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
*/
hi_s32 HI_SYS_SetLogLevel(HI_MOD_ID_E enModId, HI_LOG_LEVEL_E enLogLevel);

/**
@brief The interface of enable or disable log store. CNcomment: ʹ�ܻ�ȥʹ�� SDK�ں˴�ӡ��Ϣ���浽�ļ��Ĳ��� CNend
@attention \n
N/A CNcomment: �� CNend
@param[in]  enable_store .enable log store or disable it. CNcomment: ʹ�ܻ�ȥʹ�� CNend
@retval ::HI_SUCCESS Success CNcomment: �ɹ� CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: API����ʧ�� CNend
@see \n
*/
hi_s32 HI_SYS_LogStore(hi_bool bEnable);

/**
@brief Writes to a register or a memory. CNcomment:  Ý´¼Ä´æÆ÷»òÄÚ´æ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32RegAddr Physical address of a register or a memory CNcomment: ¼Ä´æÆ÷»òÄÚ´æµÄÎïÀíµØÖ·¡£ CNend
@param[in] u32Value Value of a register CNcomment:  ¼Ä´æÆ÷µÄÖµ¡£ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_WriteRegister(hi_u32 u32RegAddr, hi_u32 u32Value);

/**
@brief Reads a register or a memory. CNcomment: ¶Ý¼Ä´æÆ÷»òÄÚ´æ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32RegAddr Physical address of a register or a memory CNcomment: ¼Ä´æÆ÷»òÄÚ´æµÄÎïÀíµØÖ·¡£ CNend
@param[out] pu32Value Pointer to the register value (output) CNcomment:  Ö¸ÕëÀàÝÝ£¬Êä³ö¼Ä´æÆ÷µÄÖµ¡£ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_ReadRegister(hi_u32 u32RegAddr, hi_u32 *pu32Value);

/**
@brief Get timestamp. CNcomment: »ñÈ¡Ê±¼ä´Ý¡£ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[out] pu32TimeMs Pointer to the timestamp value (output) CNcomment: Êä³öÊ±¼ä´Ý¡£ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_SYS_GetTimeStampMs(hi_u32 *pu32TimeMs);

/**
@brief Applies for a media memory zone (MMZ)/SMMU and maps the user-state address.
CNcomment:  ÉêÇëMMZ/SMMUÄÚ´æ£¬²¢Ó³ÉäÓÃ»§Ì¬µØÖ· CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in/out] pstBuf Structure of the buffer information. Bufname and bufsize are inputs, and the physical address and
    user-state virtual address are outputs.
    CNcomment: bufferÝÅÝ¢½á¹¹£¬bufnameºÝbufsize×÷ÎªÊäÈë,ÎïÀíµØÖ·ºÝÓÃ»§Ì¬ÝéÄâµØÖ·×÷ÎªÊä³ö CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_MMZ_Malloc(HI_MMZ_BUF_S *pstBuf);

/**
@brief Unmaps the user-state address and releases the MMZ/SMMU.
    CNcomment: ½â³ýÓÃ»§Ì¬µØÖ·µÄÓ³Éä£¬²¢ÊÝ·ÅMMZÄÚ´æ CNend
@attention \n
Ensure that the lengths of the transferred physical address and user-state virtual address are correct.
CNcomment: ±£Ö¤´«ÈëµÄÎïÀíµØÖ·¡¢ÓÃ»§Ì¬ÝéÄâµØÖ·ºÝ³¤¶ÈÕýÈ· CNend
@param[in] pstBuf Structure of the buffer information CNcomment: bufferÝÅÝ¢½á¹¹ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_MMZ_Free(HI_MMZ_BUF_S *pstBuf);

/**
@brief pplies for an MMZ/SMMU with a specified name and obtains its physical address or SMMU address.
CNcomment: Ö¸¶¨MMZ/SMMUµÄÃû×ÖÉêÇëMMZ/SMMUÄÚ´æ£¬·µ»ØÎïÀíµØÖ·»òSMMUµØÖ· CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32Size Buffer size CNcomment: buffer´óÝ¡ CNend
@param[in] u32Align Alignment mode CNcomment: ¶ÔÆë·½Ê½ CNend
@param[in] ps8MMZName Name of an MMZ in the buffer. If the MMZ name is set to NULL, an MMZ is anonymously applied for.
    CNcomment: buffer·ÖÇøµÄÃû×Ö£¬´«ÈëNULLÄäÃûÉêÇë CNend
@param[in] ps8BufName Buffer name CNcomment: buffer¿éµÄÃû×Ö CNend
@retval ::NULL The application fails. CNcomment: ÉêÇëÊ§°Ü CNend
@retval Physical address CNcomment: ÎïÀíµØÖ·»òSMMUµØÖ· CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_u32 HI_MMZ_New(hi_u32 u32Size, hi_u32 u32Align, hi_char *ps8MMZName, hi_char *ps8MMBName);

/**
@brief Releases an MMZ/SMMU based on its physical address. CNcomment: Ý¨¹ýÎïÀíµØÖ·ÊÝ·ÅMMZ/SMMUÄÚ´æ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32PhysAddr Physical address of a buffer CNcomment: bufferÎïÀíµØÖ· CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_MMZ_Delete(hi_u32 u32PhysAddr);

/**
@brief Maps the physical address of an MMZ/SMMU applied for to a user-state virtual address.
    You can determine whether to cache the address.
CNcomment: ½«MMZ/SMMUÉêÇëµÄÎïÀíµØÖ·Ó³Éä³ÉÓÃ»§Ì¬ÝéÄâµØÖ·£¬¿ÉÒÔÖ¸¶¨ÊÇ·ñcached CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32PhysAddr Physical address of a buffer CNcomment: bufferÎïÀíµØÖ· CNend
@param[in] u32Cached Whether to cache the address. 0: no; 1: yes CNcomment: ÊÇ·ñÊ¹ÓÃcache£¬0²»Ê¹ÓÃ£¬1Ê¹ÓÃ CNend
@retval ::NULL The application fails. CNcomment: ÉêÇëÊ§°Ü CNend
@retval User-state virtual address CNcomment: ÓÃ»§Ì¬ÝéµØÖ· CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_void *HI_MMZ_Map(hi_u32 u32PhysAddr, hi_u32 u32Cached);

/**
@brief Unmaps the user-state address of an MMZ/SMMU. CNcomment: ½â³ýMMZ/SMMUÄÚ´æÓÃ»§Ì¬µØÖ·µÄÓ³Éä CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32PhysAddr Physical address of a buffer CNcomment: bufferÎïÀíµØÖ· CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_MMZ_Unmap(hi_u32 u32PhysAddr);

/**
@brief Flushes D-cache to the cached MMZ/SMMU. CNcomment: ¶ÔÓÚcachedÀàÝÝMMZ/SMMU£¬Ë¢Dcacheµ½ÄÚ´æ CNend
@attention \n
For MMZ/SMMU, if the value 0 is transferred, all D-caches are refreshed; otherwise,
              only the transferred memory is refreshed.
CNcomment: ¶ÔÓÚMMZ/SMMU£¬Èç¹û´«Èë0£¬ÔòË¢ÝÂËùÓÝµÄDcache£»·ñÔòÖ»Ë¢´«ÈëµÄÄÇ¿éÄÚ´æ CNend
@param[in] u32PhysAddr Physical address of a buffer CNcomment: bufferÎïÀíµØÖ· CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_MMZ_Flush(hi_u32 u32PhysAddr);

/**
@brief Maps a physical address to a user-state virtual address. CNcomment: ½«ÎïÀíµØÖ·Ó³Éä³ÉÓÃ»§Ì¬ÝéÄâµØÖ· CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32PhyAddr Physical address of a buffer CNcomment: bufferÎïÀíµØÖ· CNend
@param[in] u32Size Buffer size CNcomment: bufferµÄ´óÝ¡ CNend
@retval ::NULL The application fails. CNcomment: ÉêÇëÊ§°Ü CNend
@retval User-state virtual address CNcomment: ÓÃ»§Ì¬ÝéµØÖ· CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_void *HI_MEM_Map(hi_u32 u32PhyAddr, hi_u32 u32Size);

/**
@brief Unmaps a user-state address. CNcomment: ½â³ýÓÃ»§Ì¬µØÖ·µÄÓ³Éä CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] pAddrMapped User-state virtual address of a buffer. CNcomment: bufferµÄÓÃ»§Ì¬ÝéµØÖ· CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_MEM_Unmap(hi_void *pAddrMapped);

/**
@brief Obtains the physical address and size based on the virtual address.
    CNcomment: ¸ù¾ÝÝéÄâµØÖ·»ñÈ¡ÎïÀíµØÖ·£¬ÒÔ¼°´óÝ¡ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] pVir User-state virtual address CNcomment: ÓÃ»§Ì¬ÝéµØÖ· CNend
@param[out] pu32Phyaddr Physical address  CNcomment: ÎïÀíµØÖ· CNend
@param[out] pu32Size Size CNcomment: ´óÝ¡ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_MMZ_GetPhyaddr(hi_void *pVir, hi_u32 *pu32Phyaddr, hi_u32 *pu32Size);

/**
@brief Register one module to manager. CNcomment:Ä£¿é×¢²á£¬ÓÃÓÚ¹ÜÀí CNend
@attention Before manager someone module, calling this interface. CNcomment:ÈçÝè¹ÜÀíÄ£¿é£¬ÓÃ´Ë½Ó¿ÚÝÈ×¢²á CNend
@param[in] pszModuleName The module name CNcomment:Ä£¿éÃû³Æ CNend
@param[in] u32ModuleID   The module ID. CNcomment:Ä£¿éID CNend
@retval ::HI_SUCCESS Success CNcomment:³É¹¦ CNend
@retval ::HI_FAILURE Failure CNcomment:Ê§°Ü CNend
@see \n
N/A CNcomment:ÎÞ CNend
*/
hi_s32 HI_MODULE_Register(hi_u32 u32ModuleID, const hi_char *pszModuleName);

/**
@brief Register one moudle by name. CNcomment:Ä£¿é×¢²á£¬IDÓÉÝµÝ³·ÖÅä CNend
@attention Before manager someone module, calling this interface. CNcomment:ÈçÝè¹ÜÀíÄ£¿é£¬ÓÃ´Ë½Ó¿ÚÝÈ×¢²á CNend
@param[in] pszModuleName The module name CNcomment:Ä£¿éÃû³Æ CNend
@param[out] pu32ModuleID The module id allocated by system. CNcomment:ÝµÝ³·ÖÅäµÄÄ£¿éID CNend
@retval ::HI_SUCCESS Success CNcomment:³É¹¦ CNend
@retval ::HI_FAILURE Failure CNcomment:Ê§°Ü CNend
@see \n
N/A CNcomment:ÎÞ CNend
*/
hi_s32 HI_MODULE_RegisterByName(const hi_char *pszModuleName, hi_u32 *pu32ModuleID);

/**
@brief UnRegister one module to trace. CNcomment:Ä£¿éÒÆ³ý CNend
@attention Before stopping to manage someone module, calling this interface.
    CNcomment:²»ÝèÒª¹ÜÀí´ËÄ£¿éÊ±£¬Ê¹ÓÃ´Ë½Ó¿ÚÒÆ³ýÄ£¿é CNend
@param[in] u32ModuleID The module ID. CNcomment:Ä£¿éID CNend
@param[out] None CNcomment:ÎÞ CNend
@retval ::HI_SUCCESS Success CNcomment:³É¹¦ CNend
@retval ::HI_FAILURE Failure CNcomment:Ê§°Ü CNend
@see \n
N/A CNcomment:ÎÞ CNend
*/
hi_s32 HI_MODULE_UnRegister(hi_u32 u32ModuleID);

/**
@brief User mode proc cretea directory. CNcomment:ÓÃ»§Ì¬proc´´½¨Ä¿Â¼ CNend
@attention You need register module before calling this API. Only support create one level directory.
    CNcomment:ÝèÒªÝÈ×¢²áÄ£¿é£¬Ö»Ö§³Ö´´½¨Ò»¼¶Ä¿Â¼ CNend
@param[in] pszName The directory name. CNcomment:Ä¿Â¼Ãû CNend
@param[out] None CNcomment:ÎÞ CNend
@retval ::HI_SUCCESS Success CNcomment:³É¹¦ CNend
@retval ::HI_FAILURE Failure CNcomment:Ê§°Ü CNend
@see \n
N/A CNcomment:ÎÞ CNend
*/
hi_s32 HI_PROC_AddDir(const hi_char *pszName);

/**
@brief User mode proc remove directory. CNcomment:ÓÃ»§Ì¬procÉ¾³ýÄ¿Â¼ CNend
@attention It will return fail if there are entries in the directory.
    CNcomment:Èç¹ûÄ¿Â¼ÝÂ»¹ÓÝÈë¿ÚÎÄ¼þ,½«»áÉ¾³ýÊ§°Ü CNend
@param[in] pszName The directory name. CNcomment:Ä¿Â¼Ãû CNend
@retval ::HI_SUCCESS Success CNcomment:³É¹¦ CNend
@retval ::HI_FAILURE Failure CNcomment:Ê§°Ü CNend
@see \n
N/A CNcomment:ÎÞ CNend
*/
hi_s32 HI_PROC_RemoveDir(const hi_char *pszName);

/**
@brief User mode proc add entry. CNcomment:ÓÃ»§Ì¬proc´´½¨Èë¿Ú CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32ModuleID Module ID. CNcomment:Ä£¿éID CNend
@param[in] pstEntry Parameter of entry. CNcomment:´´½¨Èë¿Ú²ÎÊý CNend
@retval ::HI_SUCCESS Success CNcomment:³É¹¦ CNend
@retval ::HI_FAILURE Failure CNcomment:Ê§°Ü CNend
@see \n
N/A CNcomment:ÎÞ CNend
*/
hi_s32 HI_PROC_AddEntry(hi_u32 u32ModuleID, const HI_PROC_ENTRY_S *pstEntry);

/**
@brief User mode proc remove entry. CNcomment:ÓÃ»§Ì¬procÉ¾³ýÈë¿Ú CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32ModuleID Module ID. CNcomment:Ä£¿éID CNend
@param[in] pstEntry Parameter of entry. CNcomment:É¾³ýÈë¿Ú²ÎÊý CNend
@retval ::HI_SUCCESS Success CNcomment:³É¹¦ CNend
@retval ::HI_FAILURE Failure CNcomment:Ê§°Ü CNend
@see \n
N/A CNcomment:ÎÞ CNend
*/
hi_s32 HI_PROC_RemoveEntry(hi_u32 u32ModuleID, const HI_PROC_ENTRY_S *pstEntry);

/**
@brief User mode proc print function. CNcomment:ÓÃ»§Ì¬proc´òÓ¡ÄÚÈÝµÄº¯Êý CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] pstBuf Output buffer parameter. CNcomment:Êä³öbuffer²ÎÊý CNend
@param[in] pFmt   Format parameter. CNcomment:´òÓ¡¸ñÊ½»¯²ÎÊý CNend
@retval ::HI_SUCCESS Success CNcomment:³É¹¦ CNend
@retval ::HI_FAILURE Failure CNcomment:Ê§°Ü CNend
@see \n
N/A CNcomment:ÎÞ CNend
*/
hi_s32 HI_PROC_Printf(HI_PROC_SHOW_BUFFER_S *pstBuf, const hi_char *pFmt, ...);

/**
@brief malloc the pointed size from system heap. CNcomment:´ÓÝµÝ³ÖÝ·ÖÅäÖ¸¶¨´óÝ¡µÄÄÚ´æ CNend
@attention None CNcomment:ÎÞ CNend
@param[in] u32ModuleID The module ID, who need to request memory. CNcomment:Ä£¿éID CNend
@param[in] u32Size The size of requesting. CNcomment:ÇëÇó·ÖÅäµÄ´óÝ¡£¬µ¥Î»ÊÇ×Ö½Ú CNend
@param[out] None CNcomment:ÎÞ CNend
@retval ::Valid memory address Success CNcomment:³É¹¦·µ»Ø·ÖÅäµ½µÄ¿Õ¼äÊ×µØÖ· CNend
@retval ::NULL Failure CNcomment:Ê§°Ü·µ»ØNULL CNend
@see \n
N/A CNcomment:ÎÞ CNend
*/
hi_void *HI_MEM_Malloc(hi_u32 u32ModuleID, hi_u32 u32Size);

/**
@brief Free the requsted memory by hi_malloc. CNcomment:ÊÝ·Å·ÖÅäµÄÄÚ´æ CNend
@attention when stopping to use the memory, calling this interface.
    CNcomment:²»ÔÙÝèÒªÕâ¿éÄÚ´æÊ±£¬Ê¹ÓÃ´Ë½Ó¿Ú½øÝÝÊÝ·Å CNend
@param[in] u32ModuleID The module ID, who need to free memory. CNcomment:Ä£¿éID CNend
@param[in] pMemAddr The memory address to free CNcomment:ÊÝ·Å¿Õ¼äµÄÊ×µØÖ· CNend
@param[out] None CNcomment:ÎÞ CNend
@retval ::None CNcomment:ÎÞ CNend
@see \n
N/A CNcomment:ÎÞ CNend
*/
hi_void HI_MEM_Free(hi_u32 u32ModuleID, hi_void *pMemAddr);

/**
@brief Calloc memory, with u32MemBlock blocks and u32Size size per. CNcomment:·ÖÅäÝ¬Ýø´óÝ¡µÄÄÚ´æ¿é CNend
@attention None CNcomment:ÎÞ CNend
@param[in] u32ModuleID The module id, who need to calloc memory. CNcomment:Ä£¿éID CNend
@param[in] u32MemBlock The requesting block number. CNcomment:·ÖÅäµÄ¿éÊý CNend
@param[in] u32Size The requesting size per block. CNcomment:Ã¿¿éµÄ´óÝ¡£¬µ¥Î»ÊÇ×Ö½Ú CNend
@param[out] None CNcomment:ÎÞ CNend
@retval ::Valid memory address Success CNcomment:³É¹¦Ôò·µ»Ø·ÖÅäµ½µÄÄÚ´æÊ×µØÖ· CNend
@retval ::NULL Failure CNcomment:Ê§°Ü·µ»ØNULL CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_void *HI_MEM_Calloc(hi_u32 u32ModuleID, hi_u32 u32MemBlock, HI_U32 u32Size);

#ifdef MMZ_V2_SUPPORT
/**
@brief pplies for an MMZ with a specified name and obtains its physical address.
    CNcomment: Ö¸¶¨mmzµÄÃû×ÖÉêÇëmmzÄÚ´æ£¬·µ»ØÎïÀíµØÖ· CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32Size Buffer size CNcomment: buffer´óÝ¡ CNend
@param[in] u32Align Alignment mode CNcomment: ¶ÔÆë·½Ê½ CNend
@param[in] ps8MMZName Name of an MMZ in the buffer. If the MMZ name is set to NULL, an MMZ is anonymously applied for.
    CNcomment: buffer·ÖÇøµÄÃû×Ö£¬´«ÈëNULLÄäÃûÉêÇë CNend
@param[in] ps8MMBName Buffer name CNcomment: buffer¿éµÄÃû×Ö CNend
@retval ::NULL The application fails. CNcomment: ÉêÇëÊ§°Ü CNend
@retval Physical address CNcomment: ÎïÀíµØÖ· CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_void *HI_MMZ_New_Share(hi_u32 u32Size, hi_u32 u32Align, hi_char *ps8MMZName, hi_char *ps8MMBName);

/**
@brief pplies for an MMZ with a specified name and obtains its physical address.
    CNcomment: Ö¸¶¨mmzµÄÃû×ÖÉêÇëmmzÄÚ´æ£¬·µ»ØÎïÀíµØÖ· CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32Size Buffer size CNcomment: buffer´óÝ¡ CNend
@param[in] u32Align Alignment mode CNcomment: ¶ÔÆë·½Ê½ CNend
@param[in] ps8MMZName Name of an MMZ in the buffer. If the MMZ name is set to NULL, an MMZ is anonymously applied for.
    CNcomment: buffer·ÖÇøµÄÃû×Ö£¬´«ÈëNULLÄäÃûÉêÇë CNend
@param[in] ps8MMBName Buffer name CNcomment: buffer¿éµÄÃû×Ö CNend
@retval ::NULL The application fails. CNcomment: ÉêÇëÊ§°Ü CNend
@retval Physical address CNcomment: ÎïÀíµØÖ· CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_void *HI_MMZ_New_Shm_Com(hi_u32 u32Size, hi_u32 u32Align, hi_char *ps8MMZName, hi_char *ps8MMBName);

/**
@brief Get physical address and size of chm or com type MMZ. CNcomment: »ñÈ¡shm»òcomÝÝMMZÎïÀíµØÖ·ºÝ´óÝ¡ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[out] pu32PhysAddr Physical address of the buffer CNcomment: bufferÎïÀíµØÖ· CNend
@param[out] pu32Size     Size of the buffer             CNcomment: buffer´óÝ¡ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_MMZ_Get_Shm_Com(hi_u32 *pu32PhysAddr, hi_u32 *pu32Size);

/**
@brief Force releases an MMZ based on its physical address. CNcomment: Ý¨¹ýÎïÀíµØÖ·Ç¿ÝÝÊÝ·ÅmmzÄÚ´æ CNend
@attention \n
N/A CNcomment: ÎÞ CNend
@param[in] u32PhysAddr Physical address of a buffer CNcomment: bufferÎïÀíµØÖ· CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Calling this API fails. CNcomment: APIÝµÝ³µ÷ÓÃÊ§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_MMZ_Force_Delete(hi_u32 u32PhysAddr);

/**
@brief Flushes MMZ. CNcomment: Flush MMZÊý¾Ý CNend
@attention \n
@param[in] u32PhysAddr Physical address of a buffer CNcomment: bufferÎïÀíµØÖ· CNend
@param[in] u32VirtAddr Virtual address of a buffer  CNcomment: bufferÝéÄâµØÖ· CNend
@param[in] u32Size     Size of a buffer             CNcomment: buffer¿Õ¼ä´óÝ¡ CNend
@retval ::HI_SUCCESS Success CNcomment: ³É¹¦ CNend
@retval ::HI_FAILURE Fail. CNcomment: Ê§°Ü CNend
@see \n
N/A CNcomment: ÎÞ CNend
*/
hi_s32 HI_MMZ_Flush_Dirty(hi_u32 u32PhysAddr, hi_u32 u32VirtAddr, hi_u32 u32Size);

#endif /* endif MMZ_V2_SUPPORT */

/** @} */ /** <!-- ==== API declaration end ==== */

#endif /* endif __KERNEL__ */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_COMMON_H__ */

