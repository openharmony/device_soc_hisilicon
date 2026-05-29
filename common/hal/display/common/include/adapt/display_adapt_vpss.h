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

#ifndef DISPLYA_ADAPT_VPSS_H
#define DISPLYA_ADAPT_VPSS_H

#include "ss_mpi_vpss.h"
#include "ot_defines.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define VPSS_MAX_GRP_NUM    OT_VPSS_MAX_GRP_NUM
#define VPSS_MAX_PHY_CHN_NUM OT_VPSS_MAX_PHYS_CHN_NUM

#define VPSS_GRP            ot_vpss_grp
#define VPSS_CHN            ot_vpss_chn

typedef ot_vpss_grp_attr VPSS_GRP_ATTR_S;

typedef ot_vpss_chn_attr VPSS_CHN_ATTR_S;

#define VPSS_CHN_MODE_USER          OT_VPSS_CHN_MODE_USER
#define VPSS_CHN_MODE_AUTO          OT_VPSS_CHN_MODE_AUTO

typedef ot_vpss_chn_mode VPSS_CHN_MODE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // DISPLYA_ADAPT_VPSS_H