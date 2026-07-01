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

#include "vpss_resmng.h"
#define MODULE_NAME "VpssResMng"
#define VPSS_MAX_SIDE 1920
#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080
#define DEFAULT_FPS 60

bool g_vpssGrpUsed[VPSS_MAX_GRP_NUM];

int32_t CreateVpssGrp(const VPSS_GRP_ATTR_S *vpssGrpAttr, VPSS_GRP *vpssGrp)
{
    CHECK_NULLPOINTER_RETURN_VALUE(vpssGrpAttr, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(vpssGrp, DISPLAY_NULL_PTR);
    int32_t ret = DISPLAY_FAILURE;
    int32_t i = 0;
    for (i = VPSS_GRP_START_ID; i < VPSS_MAX_GRP_NUM; i++) {
        if (!g_vpssGrpUsed[i]) {
            break;
        }
    }
    if (i == VPSS_MAX_GRP_NUM) {
        HDF_LOGI("CreateVpssGrp faild, no vpss grounp available");
        return DISPLAY_FAILURE;
    }
    ret = ss_mpi_vpss_create_grp(i, vpssGrpAttr);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("ss_mpi_vpss_create_grp faild. ret:%x", ret);
        return ret;
    }
    g_vpssGrpUsed[i] = true;
    *vpssGrp = i;
    return ret;
}

int32_t DestroyVpssGrp(VPSS_GRP vpssGrp)
{
    if (vpssGrp >= VPSS_MAX_GRP_NUM) {
        HDF_LOGI("invalid vpssGrp, vpssGrp=%d", vpssGrp);
        return DISPLAY_PARAM_ERR;
    }
    int32_t ret = ss_mpi_vpss_stop_grp(vpssGrp);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("call ss_mpi_vpss_stop_grp failed, ret=0x%x", ret);
        return ret;
    }
    ret = ss_mpi_vpss_destroy_grp(vpssGrp);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("call ss_mpi_vpss_destroy_grp failed, ret=0x%x", ret);
        return ret;
    }
    g_vpssGrpUsed[vpssGrp] = false;
    return ret;
}

int32_t OpenVpssChn(VPSS_GRP vpssGrp, const VPSS_CHN_ATTR_S *vpssChnAttr, VPSS_CHN *vpsschn)
{
    CHECK_NULLPOINTER_RETURN_VALUE(vpssChnAttr, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(vpsschn, DISPLAY_NULL_PTR);
    int32_t ret = DISPLAY_FAILURE;
    ret = ss_mpi_vpss_set_chn_attr(vpssGrp, 0, vpssChnAttr);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("call ss_mpi_vpss_set_chn_attr faild, ret=0x%x ", ret);
        return ret;
    }
    ret = ss_mpi_vpss_enable_chn(vpssGrp, 0);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("call ss_mpi_vpss_enable_chn faild, ret=0x%x ", ret);
        return ret;
    }

    ret = ConfigVpssLowDelay(vpssGrp);
    if (ret != TD_SUCCESS) {
        return DISPLAY_FAILURE;
    }

    *vpsschn = 0;

    return DISPLAY_SUCCESS;
}

int32_t CloseVpssChn(VPSS_GRP vpssGrp, VPSS_CHN vpssChn)
{
    static bool vpssChnUsed[VPSS_MAX_PHY_CHN_NUM];
    if (vpssChn >= VPSS_MAX_PHY_CHN_NUM) {
        HDF_LOGE("vpssChn is invalid!, vpssChn=%d", vpssChn);
        return DISPLAY_PARAM_ERR;
    }

    int32_t ret = ss_mpi_vpss_disable_chn(vpssGrp, vpssChn);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("call ss_mpi_vpss_disable_chn failed, ret=0x%x", ret);
        return ret;
    }
    vpssChnUsed[vpssChn] = false;

    return DISPLAY_SUCCESS;
}

static void GetVpssChnAttr(VPSS_CHN_ATTR_S *vpssChnAttr, const LayerInfo *layerInfo)
{
    (void)layerInfo;
    vpssChnAttr->width                    = DEFAULT_WIDTH;
    vpssChnAttr->height                   = DEFAULT_HEIGHT;
    vpssChnAttr->chn_mode                   = VPSS_CHN_MODE_USER;
    vpssChnAttr->compress_mode              = COMPRESS_MODE_NONE;
    vpssChnAttr->dynamic_range              = DYNAMIC_RANGE_SDR8;
    vpssChnAttr->video_format               = VIDEO_FORMAT_LINEAR;
    vpssChnAttr->pixel_format               = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vpssChnAttr->frame_rate.src_frame_rate = DEFAULT_FPS;
    vpssChnAttr->frame_rate.dst_frame_rate = DEFAULT_FPS;
    vpssChnAttr->depth                    = 1;
    vpssChnAttr->mirror_en                     = TD_FALSE;
    vpssChnAttr->flip_en                       = TD_FALSE;
    vpssChnAttr->aspect_ratio.mode        = ASPECT_RATIO_NONE;
    vpssChnAttr->aspect_ratio.bg_color    = 0x0000ff;
}

int32_t OpenVpssRes(const LayerInfo *layerInfo, VPSS_GRP *vpssGrp, VPSS_CHN *vpssChn)
{
    int32_t ret;
    VPSS_GRP_ATTR_S vpssGrpAttr = {};
    vpssGrpAttr.frame_rate.src_frame_rate = DEFAULT_FPS;
    vpssGrpAttr.frame_rate.dst_frame_rate = DEFAULT_FPS;
    vpssGrpAttr.dynamic_range = DYNAMIC_RANGE_SDR8;
    vpssGrpAttr.pixel_format = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vpssGrpAttr.max_width = DEFAULT_WIDTH;
    vpssGrpAttr.max_height = DEFAULT_HEIGHT;
    ret = CreateVpssGrp(&vpssGrpAttr, vpssGrp);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("CreateVpssGrp failed, ret=%d", ret);
        goto CREATE_GRP_FAIL;
    }
    VPSS_CHN_ATTR_S vpssChnAttr = {};
    GetVpssChnAttr(&vpssChnAttr, layerInfo);
    ret = OpenVpssChn(*vpssGrp, &vpssChnAttr, vpssChn);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("OpenVPSSChn failed, ret=%d", ret);
        goto OPEN_CHN_FAIL;
    }
    ret = ss_mpi_vpss_start_grp(*vpssGrp);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("ss_mpi_vpss_start_grp failed, ret=0x%x", ret);
        goto START_GRP_FAIL;
    }
    HDF_LOGI("OpenVPSSRes success, vpssGrp=%d vpssChn=%d", *vpssGrp, *vpssChn);
    return DISPLAY_SUCCESS;

START_GRP_FAIL:
    (void)CloseVpssChn(*vpssGrp, *vpssChn);
OPEN_CHN_FAIL:
    (void)DestroyVpssGrp(*vpssGrp);
CREATE_GRP_FAIL:
    return ret;
}

int32_t CloseVpssRes(VPSS_GRP vpssGrp, VPSS_CHN vpssChn)
{
    if (vpssGrp >= VPSS_MAX_GRP_NUM) {
        HDF_LOGE("vpssGrp is invalid!, vpssGrp=%d", vpssGrp);
        return DISPLAY_PARAM_ERR;
    }
    int32_t ret = ss_mpi_vpss_stop_grp(vpssGrp);
    CHECK_CALL_MPI_RET(ret, ss_mpi_vpss_stop_grp);
    ret = CloseVpssChn(vpssGrp, vpssChn);
    ret = DestroyVpssGrp(vpssGrp);
    HDF_LOGI("CloseVpssRes success, vpssGrp=%d vpssChn=%d", vpssGrp, vpssChn);
    return DISPLAY_SUCCESS;
}
