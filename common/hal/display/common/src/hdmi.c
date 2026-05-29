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

#include <securec.h>
#include "display_layer_internal.h"
#include "display_type.h"
#include "disp_common.h"
#include "display_adapt_hdmi.h"


static TD_BOOL g_hdmiInitialized = TD_FALSE;
TD_BOOL IsHdmiInitialized(void)
{
    return g_hdmiInitialized;
}

#ifdef __HDMI_SUPPORT__
static void SetHdmiParam(HI_HDMI_ATTR_S *attr)
{
    attr->hdmi_en = TD_TRUE;
    attr->video_format = HI_HDMI_VIDEO_FMT_1080P_60;
    attr->deep_color_mode = HI_HDMI_DEEP_COLOR_24BIT;
    attr->audio_en = TD_TRUE;
    attr->sample_rate = OT_HDMI_SAMPLE_RATE_48K;
    attr->bit_depth = HI_HDMI_BIT_DEPTH_16;
}

int32_t HdmiStart(void)
{
    int32_t ret;
    HI_HDMI_ID_E hdmiId = HI_HDMI_ID_0;
    HI_HDMI_ATTR_S attr = {0};

    ret = ss_mpi_hdmi_init();
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: ss_mpi_hdmi_init fail, ret = 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }
    ret = ss_mpi_hdmi_open(hdmiId);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: ss_mpi_hdmi_open fail, ret = 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }
    ret = ss_mpi_hdmi_get_attr(hdmiId, &attr);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: ss_mpi_hdmi_get_attr fail, ret = 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }
    SetHdmiParam(&attr);
    ret = ss_mpi_hdmi_set_attr(hdmiId, &attr);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: ss_mpi_hdmi_set_attr fail, ret = 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }
    ret = ss_mpi_hdmi_start(hdmiId);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: ss_mpi_hdmi_start fail, ret = 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }
    HDF_LOGI("%s: start hdmi success", __func__);
    return DISPLAY_SUCCESS;
}

TD_BOOL CheckHdmiConnect(void)
{
    HI_HDMI_SINK_CAPABILITY_S sinkCap = {0};
    /* start hdmi */
    int32_t ret = HdmiStart();
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: HdmiStart fail", __func__);
        return TD_FALSE;
    }
    /* after hdmi start, need to sleep 1~2 second to call get sink cap */
    sleep(1);
    ret = ss_mpi_hdmi_get_sink_capability(HI_HDMI_ID_0, &sinkCap);
    if (ret != DISPLAY_SUCCESS) {
        return TD_FALSE;
    }
    if (sinkCap.is_connected) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static void HdmiStop(void)
{
    ss_mpi_hdmi_stop(HI_HDMI_ID_0);
    ss_mpi_hdmi_close(HI_HDMI_ID_0);
    ss_mpi_hdmi_deinit();
}

int32_t EnableVOWithHdmi(uint32_t devId)
{
    int32_t ret;
    VO_PUB_ATTR_S voPubAttr = {0};
    int32_t voDev = devId > INT32_MAX ? INT32_MAX : (int32_t)devId;

    voPubAttr.intf_type = VO_INTF_HDMI;
    voPubAttr.intf_sync = VO_OUTPUT_1080P60;
    voPubAttr.bg_color = COLOR_RGB_BLUE;
    ret = ss_mpi_vo_set_pub_attr(voDev, &voPubAttr);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: ss_mpi_vo_set_pub_attr failure, ret 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }

    VO_MOD_PARAM_S modParam = {0};
    ss_mpi_vo_get_mod_param(&modParam);
    modParam.exit_dev_en = TD_FALSE;
    ret = ss_mpi_vo_set_mod_param(&modParam);
    HDF_LOGI("%s: HDMI ss_mpi_vo_set_mod_param, ret 0x%x", __func__, ret);
    ret = ss_mpi_vo_enable(voDev);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: HDMI ss_mpi_vo_enable failure, ret 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }
    ret = HdmiStart();
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: HdmiStart fail", __func__);
        (void)ss_mpi_vo_disable(voDev);
        return DISPLAY_FAILURE;
    }
    g_hdmiInitialized = TD_TRUE;
    HDF_LOGI("%s: hdmi was initialized", __func__);
    return DISPLAY_SUCCESS;
}

int32_t DisableVOWithHdmi(uint32_t devId)
{
    HdmiStop();
    int32_t voDev = devId > INT32_MAX ? INT32_MAX : (int32_t)devId;
    (void)ss_mpi_vo_disable(voDev);
    return DISPLAY_SUCCESS;
}
#endif
