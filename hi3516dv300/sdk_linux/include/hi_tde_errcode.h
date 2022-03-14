/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
#ifndef __TDE_ERRCODE_H__
#define __TDE_ERRCODE_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define HI_ERR_TDE_BASE ((HI_S32)(((0x80UL + 0x20UL) << 24) | (100 << 16) | (4 << 13) | 1))

enum {
    HI_ERR_TDE_DEV_NOT_OPEN = HI_ERR_TDE_BASE, /* <  tde device not open yet */
    HI_ERR_TDE_DEV_OPEN_FAILED,                /* <  open tde device failed */
    HI_ERR_TDE_NULL_PTR,                       /* <  input parameters contain null ptr */
    HI_ERR_TDE_NO_MEM,                         /* <  malloc failed  */
    HI_ERR_TDE_INVALID_HANDLE,                 /* <  invalid job handle */
    HI_ERR_TDE_INVALID_PARA,                   /* <  invalid parameter */
    HI_ERR_TDE_NOT_ALIGNED,                    /* <  aligned error for position, stride, width */
    HI_ERR_TDE_MINIFICATION,                   /* <  invalid minification */
    HI_ERR_TDE_CLIP_AREA,                      /* <  clip area and operation area have no intersection */
    HI_ERR_TDE_JOB_TIMEOUT,                    /* <  blocked job wait timeout */
    HI_ERR_TDE_UNSUPPORTED_OPERATION,          /* <  unsupported operation */
    HI_ERR_TDE_QUERY_TIMEOUT,                  /* <  query time out */
    HI_ERR_TDE_INTERRUPT                       /* blocked job was interrupted */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TDE_ERRCODE_H__ */
