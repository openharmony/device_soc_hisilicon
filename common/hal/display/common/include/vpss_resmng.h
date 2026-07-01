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

#ifndef VPSS_RESMNG_H
#define VPSS_RESMNG_H

#include "display_adapt_vpss.h"
#include "display_adapt_video.h"
#include "display_layer_internal.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @brief create idle vpssGrp.
 *
 * @param vpssChnAttr Indicates the vpssg roup attribute.
 * @param vpssGrp Indicates the vpss group id. the vpssGrp is returned after create success.
 *
 * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
 * otherwise.
 */
int32_t CreateVpssGrp(const VPSS_GRP_ATTR_S *vpssGrpAttr, VPSS_GRP *vpssGrp);

/**
 * @brief destroy vpssGrp.
 *
 * @param vpssGrp Indicates the vpss group id.
 *
 * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
 * otherwise.
 */
int32_t DestroyVpssGrp(VPSS_GRP vpssGrp);

/**
 * @brief enable a idle vpss channel.
 *
 * @param vpssGrp Indicates the vpss group id.
 *
 * @param vpssChnAttr Indicates the vpss channel attribute.
 *
 * @param vpsschn Indicates the vpss channel id. the vpsschn is returned after enable success.
 *
 * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
 * otherwise.
 */
int32_t OpenVpssChn(VPSS_GRP vpssGrp, const VPSS_CHN_ATTR_S *vpssChnAttr, VPSS_CHN *vpsschn);

/**
 * @brief disable a woking vpss channel.
 *
 * @param vpssGrp Indicates the vpss group id.
 *
 * @param vpsschn Indicates the vpss channel id.
 *
 * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
 * otherwise.
 */
int32_t CloseVpssChn(VPSS_GRP vpssGrp, VPSS_CHN vpssChn);

int32_t OpenVpssRes(const LayerInfo *layerInfo, VPSS_GRP *vpssGrp, VPSS_CHN *vpssChn);

int32_t CloseVpssRes(VPSS_GRP vpssGrp, VPSS_CHN vpssChn);

int32_t ConfigVpssLowDelay(VPSS_GRP vpssGrp);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* VPSS_RESMNG_H */