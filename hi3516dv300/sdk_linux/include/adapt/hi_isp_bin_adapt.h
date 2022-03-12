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

#ifndef __HI_ISP_BIN_ADAPT_H__
#define __HI_ISP_BIN_ADAPT_H__

#include "hi_type.h"
#include "hi_comm_isp_adapt.h"
#include "hi_isp_bin.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* GENERAL STRUCTURES */
typedef struct {
    hi_u32  addr;    /* register addr */
    hi_u8   start_bit; /* start bit of register addr */
    hi_u8   end_bit;   /* end bit of register addr */
} hi_isp_bin_reg_attr;

/*
 * The base addr of ISP logic register
 * The base addr of ISP ext register
 * The base addr of Hisi AE ext register
 * The base addr of Hisi AWB ext register
 */
hi_isp_bin_reg_attr g_isp_bin_reg_attr[ISP_MAX_PIPE_NUM][MAX_BIN_REG_NUM] = {
    [0 ...(ISP_MAX_PIPE_NUM - 1)] = {0}
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __HI_ISP_BIN_ADAPT_H__ */
