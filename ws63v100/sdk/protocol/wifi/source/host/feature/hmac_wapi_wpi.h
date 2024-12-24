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
 * Description: hmac_wapi_wpi.c的头文件.
 */

#ifndef __HMAC_WAPI_WPI_H__
#define __HMAC_WAPI_WPI_H__

#include "oal_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 头文件包含
*****************************************************************************/

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WAPI_WPI_H

/*****************************************************************************
  2 宏定义
*****************************************************************************/


/*****************************************************************************
  3 枚举定义
*****************************************************************************/


/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/
osal_u32 hmac_wpi_encrypt_etc(osal_u8 *iv, osal_u8 *bufin, osal_u32 buflen,
    osal_u8 *key, osal_u8 *bufout);

osal_u32 hmac_wpi_decrypt_etc(osal_u8 *iv, osal_u8 *bufin, osal_u32 buflen,
    osal_u8 *key, osal_u8 *bufout);

osal_void hmac_wpi_swap_pn_etc(osal_u8 *pn, osal_u8 len);

osal_u32 hmac_wpi_pmac_etc(const osal_u8 *iv, osal_u8 *buf, osal_u32 pamclen,
    osal_u8 *key, osal_u8 *mic);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_wapi_wpi.h */
