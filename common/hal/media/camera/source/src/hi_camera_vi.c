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

#include "hi_camera_vi.h"
#include <dlfcn.h>
#include "ss_confaccess.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MIPI_DEV_NODE "/dev/ot_mipi_rx"
#define MAX_SENSOR_NUM 2
#define ISP_MAX_DEV_NUM 4
#define THREAD_NAME_SIZE 20
#define INIT_PARAM_KEY_MAX_LEN 128

/* product ini identification */
#define PDT_INIPARAM "productini"
#define PDT_INIPARAM_PATH "/storage/data/media_config.ini"
#define CAMERA_DEV_MODULE "camera_dev"
static pthread_t g_ispPid[ISP_MAX_DEV_NUM] = { 0 };
static uint32_t g_au32IspSnsId[ISP_MAX_DEV_NUM] = { 0, 1 };
static void *g_snsLibHandle[MAX_SENSOR_NUM] = {NULL};
static ot_isp_sns_obj *g_snsObj[MAX_SENSOR_NUM] = {NULL};
static int32_t g_sensorDev[MAX_SENSOR_NUM] = { -1, -1 };
static uint32_t g_sensorCnt = 0;
/* *
Vi dev attr
*/
static ot_vi_dev_attr g_devAttrDefaultBase = {
    OT_VI_INTF_MODE_MIPI,
    OT_VI_WORK_MODE_MULTIPLEX_1,
    { 0xFFF00000, 0x0 },
    OT_VI_SCAN_PROGRESSIVE,
    { -1, -1, -1, -1 },
    OT_VI_DATA_SEQ_YUYV,
    /* port_vsync port_vsync_neg port_hsync port_hsync_neg */
    {
        OT_VI_VSYNC_PULSE,
        OT_VI_VSYNC_NEG_LOW,
        OT_VI_HSYNC_VALID_SIG,
        OT_VI_HSYNC_NEG_HIGH,
        OT_VI_VSYNC_VALID_SIG,
        OT_VI_VSYNC_VALID_NEG_HIGH,

        /* hsync_hfb    hsync_act    hsync_hhb */
        { 0, 1280, 0,
        /* vsync0_vhb vsync0_act vsync0_hhb */
          0, 720, 0,
        /* vsync1_vhb vsync1_act vsync1_hhb */
          0, 0, 0 }
    },
    OT_VI_DATA_TYPE_RAW,
    TD_FALSE,
    { 1920, 1080 },
    OT_DATA_RATE_X1
};

static int32_t GetVbPoolParam(VB_PARAM_CONFIG_S *vbAttr)
{
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };
    td_s32 poolcnt = 0;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, "vb:max_poolcnt", 0, &poolcnt);
    if (poolcnt > OT_VB_MAX_COMMON_POOLS) {
        HI_PRINTF("poolcnt:%d error, max:%d\n", poolcnt, OT_VB_MAX_COMMON_POOLS);
        return TD_FAILURE;
    }

    vbAttr->stMaxPoolCnt = (td_u32)poolcnt;

    for (uint32_t i = 0; i < vbAttr->stMaxPoolCnt; i++) {
        CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
            "vb.pool.%u:image_width", i));
        SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &vbAttr->stImageSize[i].width);
        CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
            "vb.pool.%u:image_height", i));
        SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &vbAttr->stImageSize[i].height);
        CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
            "vb.pool.%u:pix_format", i));
        SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &vbAttr->stPixFormat[i]);
        CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
            "vb.pool.%u:data_bitwidth", i));
        SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &vbAttr->stDataBitWidth[i]);
        CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
            "vb.pool.%u:compress_mode", i));
        SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &vbAttr->stCompressMode[i]);
        CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
            "vb.pool.%u:default_align", i));
        SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &vbAttr->stDefaultAlign[i]);
        CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
            "vb.pool.%u:blk_count", i));
        SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &vbAttr->stBlkCount[i]);
        HI_PRINTF("vb pool %u image width=%u height=%u\n", i,
            vbAttr->stImageSize[i].width, vbAttr->stImageSize[i].height);
    }
    return TD_SUCCESS;
}

static void GetViDevParam(ot_vi_dev_attr *viDevAttr, uint8_t index)
{
    /* init dev attr */
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };

    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.dev:sensor_width", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viDevAttr->in_size.width);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.dev:sensor_height", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viDevAttr->in_size.height);
}

static int32_t GetViMipiParam(combo_dev_attr_t *viMipiAttr, uint8_t index)
{
    /* mipi attr */
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };

    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.mipi:combo_dev_t", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viMipiAttr->devno);
    HI_PRINTF("index %u  devno %u\n", index, viMipiAttr->devno);
    g_sensorDev[index] = viMipiAttr->devno;
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.mipi:input_mode_t", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viMipiAttr->input_mode);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.mipi:mipi_data_rate_t", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viMipiAttr->data_rate);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.mipi:img_rect_x", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viMipiAttr->img_rect.x);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.mipi:img_rect_y", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viMipiAttr->img_rect.y);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.mipi:img_rect_w", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viMipiAttr->img_rect.width);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.mipi:img_rect_h", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viMipiAttr->img_rect.height);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.mipi:data_type_t", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viMipiAttr->mipi_attr.input_data_type);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.mipi:mipi_wdr_mode_t", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viMipiAttr->mipi_attr.wdr_mode);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.mipi:lane_id_num", index));
    td_u32 laneIdNum = 0;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &laneIdNum);
    if (laneIdNum > MIPI_LANE_NUM) {
        HI_PRINTF("lane_id_num:%u error, max:%d\n", laneIdNum, MIPI_LANE_NUM);
        return TD_FAILURE;
    }

    for (uint32_t i = 0; i < laneIdNum; i++) {
        CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
            "vcap%u.mipi:lane_id_%u", index, i));
        SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viMipiAttr->mipi_attr.lane_id[i]);
    }
    for (uint32_t i = laneIdNum; i < MIPI_LANE_NUM; i++) {
        viMipiAttr->mipi_attr.lane_id[i] = -1;
    }

    return TD_SUCCESS;
}

static void GetViIspPubParam(ot_isp_pub_attr *ispPubAttr, uint8_t index)
{
    /* isp_pub attr */
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };

    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:stWndRect_x", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &ispPubAttr->wnd_rect.x);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:stWndRect_y", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &ispPubAttr->wnd_rect.y);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:stWndRect_w", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &ispPubAttr->wnd_rect.width);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:stWndRect_h", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &ispPubAttr->wnd_rect.height);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:outImageSize_x", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &ispPubAttr->sns_size.width);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:outImageSize_y", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &ispPubAttr->sns_size.height);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:frameRate", index));
    td_double frameRate = 0;
    SS_CONFACCESS_GetDouble(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &frameRate);
    ispPubAttr->frame_rate = (td_float)frameRate;
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:bayer_format", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &ispPubAttr->bayer_format);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:wdrMode", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &ispPubAttr->wdr_mode);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:snsMode", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &ispPubAttr->sns_mode);
}

static int32_t GetViChnImageParam(ot_vi_chn_attr *chnAttr, uint32_t pipeId, uint32_t chnId, uint8_t index)
{
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };

    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:out_width", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &chnAttr->size.width);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:out_height", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &chnAttr->size.height);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:pixel_format", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &chnAttr->pixel_format);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:dynamic_range", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &chnAttr->dynamic_range);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:video_format", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &chnAttr->video_format);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:compress_mode", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &chnAttr->compress_mode);
    return TD_SUCCESS;
}

static int32_t GetViChnCtrlParam(ot_vi_chn_attr *chnAttr, uint32_t pipeId, uint32_t chnId, uint8_t index)
{
    int32_t isMirrorEnable = 0;
    int32_t isFlipEnable = 0;
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };

    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:mirror_en", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &isMirrorEnable);
    chnAttr->mirror_en = (isMirrorEnable != 0) ? TD_TRUE : TD_FALSE;
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:flip_en", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &isFlipEnable);
    chnAttr->flip_en = (isFlipEnable != 0) ? TD_TRUE : TD_FALSE;
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:depth", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &chnAttr->depth);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:src_framerate", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0,
        &chnAttr->frame_rate_ctrl.src_frame_rate);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u.chl.%u:dst_framerate", index, pipeId, chnId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0,
        &chnAttr->frame_rate_ctrl.dst_frame_rate);
    return TD_SUCCESS;
}

static int32_t GetViPipeChnParam(uint32_t pipeId, VI_PARAM_CONFIG_S *viAttr, uint8_t index)
{
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };

    /* chn attr */
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:chncnt", index, pipeId));
    uint32_t chnCnt = 0;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &chnCnt);
    if (chnCnt > MAX_CHN_NUM_PER_PIPE) {
        HI_PRINTF("chncnt:%u error, max:%d\n", chnCnt, MAX_CHN_NUM_PER_PIPE);
        return TD_FAILURE;
    }

    for (uint32_t j = 0; j < chnCnt; j++) {
        ot_vi_chn_attr *chnAttr = &viAttr->stChnAttr[pipeId][j];
        if (GetViChnImageParam(chnAttr, pipeId, j, index) != TD_SUCCESS ||
            GetViChnCtrlParam(chnAttr, pipeId, j, index) != TD_SUCCESS) {
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static int32_t GetViPipeBaseParam(ot_vi_pipe_attr *pipeAttr, VI_PARAM_CONFIG_S *viAttr, uint32_t pipeId, uint8_t index)
{
    int32_t isIspBypass = 0;
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };

    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "common.vcap%u.pipe.%u:vivpssmode", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viAttr->pipeMode[pipeId]);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:pipe_bypass_mode", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &pipeAttr->pipe_bypass_mode);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:isp_bypass", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &isIspBypass);
    pipeAttr->isp_bypass = (isIspBypass != 0) ? TD_TRUE : TD_FALSE;
    return TD_SUCCESS;
}

static int32_t GetViPipeImageParam(ot_vi_pipe_attr *pipeAttr, uint32_t pipeId, uint8_t index)
{
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };

    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:width", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &pipeAttr->size.width);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:height", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &pipeAttr->size.height);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:pixel_format", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &pipeAttr->pixel_format);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:compress_mode", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &pipeAttr->compress_mode);
    return TD_SUCCESS;
}

static int32_t GetViPipeFrameParam(ot_vi_pipe_attr *pipeAttr, VI_PARAM_CONFIG_S *viAttr, uint32_t pipeId, uint8_t index)
{
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };

    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:src_framerate", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0,
        &pipeAttr->frame_rate_ctrl.src_frame_rate);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:dst_framerate", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0,
        &pipeAttr->frame_rate_ctrl.dst_frame_rate);

    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:interrupt_type", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0x5,
        &viAttr->frameInterruptType[pipeId]);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.pipe.%u:earlyLine", index, pipeId));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viAttr->earlyLine[pipeId]);
    return TD_SUCCESS;
}

static int32_t GetViPipeParam(VI_PARAM_CONFIG_S *viAttr, uint8_t index)
{
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };
     /* pipe attr */
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.dev:pipecnt", index));
    uint32_t pipeCnt = 0;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &pipeCnt);
    if (pipeCnt > OT_VI_MAX_PIPE_NUM) {
        HI_PRINTF("pipecnt:%u error, max:%d\n", pipeCnt, OT_VI_MAX_PIPE_NUM);
        return TD_FAILURE;
    }

    for (uint32_t i = 0; i < pipeCnt; i++) {
        ot_vi_pipe_attr *pipeAttr = &viAttr->stPipeAttr[i];
        if (GetViPipeBaseParam(pipeAttr, viAttr, i, index) != TD_SUCCESS ||
            GetViPipeImageParam(pipeAttr, i, index) != TD_SUCCESS ||
            GetViPipeFrameParam(pipeAttr, viAttr, i, index) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        /* chn attr */
        if (GetViPipeChnParam(i, viAttr, index) != TD_SUCCESS) {
            HI_PRINTF("GetViPipe[%u]ChnParam failed \n", i);
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static int32_t GetViParam(VI_PARAM_CONFIG_S *viAttr, uint8_t index)
{
    /* init dev attr */
    GetViDevParam(&viAttr->stDevAttr, index);
    char szModuleNodeKey[INIT_PARAM_KEY_MAX_LEN] = { 0 };

    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.dev:sensor_lib", index));
    const char *tmpValue = NULL;
    SS_CONFACCESS_GetString(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, "/usr/lib/libsns_imx415.so", &tmpValue);
    if (tmpValue != NULL) {
        HAL_LOG_DOFUNC(strncpy_s(viAttr->sensorLib, SENSOR_DESC_MAX_LEN, tmpValue, SENSOR_DESC_MAX_LEN - 1));
    }
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.dev:sensor_obj", index));
    SS_CONFACCESS_GetString(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, "g_sns_imx415_obj", &tmpValue);
    if (tmpValue != NULL) {
        HAL_LOG_DOFUNC(strncpy_s(viAttr->sensorObj, SENSOR_DESC_MAX_LEN, tmpValue, SENSOR_DESC_MAX_LEN - 1));
    }

    /* mipi attr */
    if (GetViMipiParam(&viAttr->stMipiAttr, index) != TD_SUCCESS) {
        HI_PRINTF("GetViMipiParam failed \n");
        return TD_FAILURE;
    }

    /* isp_pub attr */
    GetViIspPubParam(&viAttr->stPubAttr, index);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.isp_pub:busId", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viAttr->busId);
    HI_PRINTF(" busid = %d\n", viAttr->busId);
    CHECK_SNPRINTF_PROC(snprintf_s(szModuleNodeKey, INIT_PARAM_KEY_MAX_LEN, INIT_PARAM_KEY_MAX_LEN - 1,
        "vcap%u.dev:sensor_type", index));
    SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, szModuleNodeKey, 0, &viAttr->sensorType);
    HI_PRINTF(" sensor type = %u\n", viAttr->sensorType);
    /* pipe attr */
    if (GetViPipeParam(viAttr, index) != TD_SUCCESS) {
        HI_PRINTF("GetViPipeParam failed \n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

uint8_t GetParamCamera(const char path[PATH_MAX_LEN], InitParam *initParam, uint8_t cameraId)
{
    int32_t ret = SS_CONFACCESS_Init(PDT_INIPARAM, PDT_INIPARAM_PATH);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("SS_CONFACCESS_Init failed\n");
        return cameraId + 1;
    }
    g_sensorCnt = 0;
    g_sensorDev[0] = -1;
    g_sensorDev[1] = -1;

    int32_t sensorCnt = 0;
    ret = SS_CONFACCESS_GetInt(PDT_INIPARAM, CAMERA_DEV_MODULE, "common:sensorcnt", 0,  &sensorCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("SS_CONFACCESS_GetInt failed\n");
    }
    // init vb
    HI_PRINTF("sens count = %d\n", sensorCnt);
    if (GetVbPoolParam(&initParam->stVbAttr) != TD_SUCCESS) {
        HI_PRINTF("GetVbPoolParam failed\n");
        ret = SS_CONFACCESS_Deinit(PDT_INIPARAM);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("SS_CONFACCESS_Deinit failed\n");
        }
        return cameraId + 1;
    }
    uint8_t i = 0;
    for (; i < cameraId && i < sensorCnt; i++) {
        if (GetViParam(&initParam[i].stViAttr, i) != TD_SUCCESS) {
            HI_PRINTF("GetViPipeParam failed\n");
            ret = SS_CONFACCESS_Deinit(PDT_INIPARAM);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("SS_CONFACCESS_Deinit failed\n");
            }
            return cameraId + 1;
        }
    }
    ret = SS_CONFACCESS_Deinit(PDT_INIPARAM);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("SS_CONFACCESS_Deinit failed\n");
    }
    g_sensorCnt = i;
    return i;
}

static int32_t ViSetMipiHsMode(void)
{
    int32_t fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        HI_PRINTF("open hi_mipi dev failed\n");
        return -1;
    }
    lane_divide_mode_t enHsMode;
    if (g_sensorCnt == MAX_SENSOR_NUM && g_sensorDev[0] != g_sensorDev[1]) {
        enHsMode = LANE_DIVIDE_MODE_1; // 2+2
    } else {
        enHsMode = LANE_DIVIDE_MODE_0; // 4
    }
    int32_t ret = ioctl(fd, OT_MIPI_SET_HS_MODE, &enHsMode);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("OT_MIPI_SET_HS_MODE failed\n");
    }
    close(fd);
    return ret;
}

static int32_t ViEnableMipiClock(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    int32_t ret = TD_SUCCESS;
    int32_t fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        HI_PRINTF("open hi_mipi dev failed\n");
        return TD_FAILURE;
    }
    for (uint32_t i = 0; i < cameraCnt; i++) {
        combo_dev_t devno = pstViConfig[i].stSnsInfo.MipiDev;
        ret = ioctl(fd, OT_MIPI_ENABLE_MIPI_CLOCK, &devno);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("MIPI_ENABLE_CLOCK %u failed\n", devno);
            break;
        }
    }
    close(fd);
    return ret;
}

static int32_t ViDisableMipiClock(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    int32_t fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        HI_PRINTF("open hi_mipi dev failed\n");
        return TD_FAILURE;
    }
    int32_t ret = TD_SUCCESS;
    for (uint32_t i = 0; i < cameraCnt; i++) {
        combo_dev_t devno = pstViConfig->stSnsInfo.MipiDev;
        ret = ioctl(fd, OT_MIPI_DISABLE_MIPI_CLOCK, &devno);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("MIPI_DISABLE_CLOCK %u failed\n", devno);
            break;
        }
    }
    close(fd);
    return ret;
}

static int32_t ViResetMipi(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    int32_t ret = TD_SUCCESS;
    int32_t fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        HI_PRINTF("open hi_mipi dev failed\n");
        return TD_FAILURE;
    }
    for (uint32_t i = 0; i < cameraCnt; i++) {
        combo_dev_t devno = pstViConfig[i].stSnsInfo.MipiDev;
        ret = ioctl(fd, OT_MIPI_RESET_MIPI, &devno);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("RESET_MIPI %u failed\n", devno);
            break;
        }
    }
    close(fd);
    return ret;
}

static int32_t ViEnableSensorClock(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    int32_t ret = TD_SUCCESS;
    sns_clk_source_t snsDev;

    int32_t fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        HI_PRINTF("open hi_mipi dev failed\n");
        return TD_FAILURE;
    }
    for (uint32_t i = 0; i < cameraCnt; i++) {
        snsDev = g_sensorDev[i];
        ret = ioctl(fd, OT_MIPI_ENABLE_SENSOR_CLOCK, &snsDev);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("OT_MIPI_ENABLE_SENSOR_CLOCK failed\n");
            break;
        }
    }
    close(fd);
    return ret;
}

static int32_t ViResetSensor(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    (void)pstViConfig;
    int32_t ret = TD_SUCCESS;
    sns_clk_source_t snsDev;

    int32_t fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        HI_PRINTF("open hi_mipi dev failed\n");
        return TD_FAILURE;
    }
    for (uint32_t i = 0; i < cameraCnt; i++) {
        snsDev = g_sensorDev[i];
        ret = ioctl(fd, OT_MIPI_RESET_SENSOR, &snsDev);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("OT_MIPI_RESET_SENSOR failed\n");
            break;
        }
    }
    close(fd);
    return ret;
}

static int32_t ViUnresetSensor(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    int32_t ret = TD_SUCCESS;
    sns_clk_source_t snsDev;

    int32_t fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        HI_PRINTF("open hi_mipi dev failed\n");
        return TD_FAILURE;
    }
    for (uint32_t i = 0; i < cameraCnt; i++) {
        snsDev = g_sensorDev[i];
        ret = ioctl(fd, OT_MIPI_UNRESET_SENSOR, &snsDev);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("OT_MIPI_UNRESET_SENSOR failed\n");
            break;
        }
    }
    close(fd);
    return ret;
}

static int32_t ViUnresetMipi(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    int32_t fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        HI_PRINTF("open hi_mipi dev failed\n");
        return TD_FAILURE;
    }
    for (uint32_t i = 0; i < cameraCnt; i++) {
        combo_dev_t devno = pstViConfig[i].stSnsInfo.MipiDev;
        int32_t ret = ioctl(fd, OT_MIPI_UNRESET_MIPI, &devno);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("UNRESET_MIPI %u failed\n", devno);
            break;
        }
    }
    close(fd);
    return TD_SUCCESS;
}

static int32_t ViSetMipiAttr(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    combo_dev_attr_t stcomboDevAttr;

    int32_t fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        HI_PRINTF("open hi_mipi dev failed\n");
        return TD_FAILURE;
    }

    for (uint32_t i = 0; i < cameraCnt; i++) {
        HAL_LOG_DOFUNC(memcpy_s(&stcomboDevAttr, sizeof(combo_dev_attr_t),
            pstViConfig[i].mipiAttr, sizeof(combo_dev_attr_t)));

        stcomboDevAttr.devno = pstViConfig[i].stSnsInfo.MipiDev;
        int32_t ret = ioctl(fd, OT_MIPI_SET_DEV_ATTR, &stcomboDevAttr);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("OT_MIPI_SET_DEV_ATTR failed:ret= 0x%x dev:%u\n", ret, stcomboDevAttr.devno);
            break;
        }
    }
    close(fd);
    return TD_SUCCESS;
}

static int32_t CameraDisableSensorClock(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    int32_t ret = TD_SUCCESS;

    int32_t fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        HI_PRINTF("open hi_mipi dev failed\n");
        return TD_FAILURE;
    }
    for (uint32_t i = 0; i < cameraCnt; i++) {
        sns_clk_source_t snsDev = pstViConfig[i].stDevInfo.ViDev;
        ret = ioctl(fd, OT_MIPI_DISABLE_SENSOR_CLOCK, &snsDev);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("OT_MIPI_DISABLE_SENSOR_CLOCK failed\n");
            break;
        }
    }
    close(fd);
    return ret;
}

/* ****************************************************************************
 * function : init mipi
 * *************************************************************************** */
static int32_t CameraStartMIPI(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    if (pstViConfig == NULL) {
        HI_PRINTF("%s: null ptr\n", __FUNCTION__);
        return TD_FAILURE;
    }

    int32_t ret = ViSetMipiHsMode();
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_SetMipiHsMode failed!\n");
        return TD_FAILURE;
    }
    ret = ViEnableMipiClock(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_EnableMipiClock failed!\n");
        return TD_FAILURE;
    }
    ret = ViResetMipi(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_ResetMipi failed!\n");
        return TD_FAILURE;
    }
    ret = ViEnableSensorClock(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_EnableSensorClock failed!\n");
        return TD_FAILURE;
    }

    ret = ViResetSensor(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_ResetSensor failed!\n");
        return TD_FAILURE;
    }
    ret = ViSetMipiAttr(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_SetMipiAttr failed!\n");
        return TD_FAILURE;
    }
    ret = ViUnresetMipi(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_UnresetMipi failed!\n");
        return TD_FAILURE;
    }
    ret = ViUnresetSensor(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_UnresetSensor failed!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static int32_t CameraStopMIPI(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    if (pstViConfig == NULL) {
        HI_PRINTF("%s: null ptr\n", __FUNCTION__);
        return TD_FAILURE;
    }
    int32_t ret = ViResetSensor(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_ResetSensor failed!\n");
        return TD_FAILURE;
    }
    ret = CameraDisableSensorClock(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_DisableSensorClock failed!\n");
        return TD_FAILURE;
    }
    ret = ViResetMipi(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_ResetMipi failed!\n");
        return TD_FAILURE;
    }
    ret = ViDisableMipiClock(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_DisableMipiClock failed!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

int32_t HpCameraSetFps(td_float pf32Framerate)
{
    ot_isp_pub_attr stIspPubAttr;
    ot_vi_pipe ViPipe = 0;
    HAL_LOG_DOFUNC(memset_s(&stIspPubAttr, sizeof(ot_isp_pub_attr), 0, sizeof(ot_isp_pub_attr)));
    int32_t ret = ss_mpi_isp_get_pub_attr(ViPipe, &stIspPubAttr);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_isp_get_pub_attr  failed %x\n", ret);
        return TD_FAILURE;
    }
    /* 1.set mater isp pub attr */
    stIspPubAttr.frame_rate = pf32Framerate;
    ret = ss_mpi_isp_set_pub_attr(ViPipe, &stIspPubAttr);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_isp_set_pub_attr  failed %x\n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

int32_t HpCameraGetFps(void)
{
    ot_isp_pub_attr stIspPubAttr;
    ot_vi_pipe ViPipe = 0;
    HAL_LOG_DOFUNC(memset_s(&stIspPubAttr, sizeof(ot_isp_pub_attr), 0, sizeof(ot_isp_pub_attr)));
    int32_t ret = ss_mpi_isp_get_pub_attr(ViPipe, &stIspPubAttr);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_isp_get_pub_attr  failed %x\n", ret);
        return TD_FAILURE;
    }
    /* 1.set mater isp pub attr */
    return (int32_t)stIspPubAttr.frame_rate;
}

static int32_t ConfigVIVPSSMode(const CAMERA_VI_CONFIG_S *viConfig, ot_vi_vpss_mode *viVpssMode)
{
    ot_vi_pipe viPipe = viConfig->stPipeInfo.aPipe[0];
    HI_PRINTF("viPipe = %d\n", viPipe);
    if (viPipe < 0 || viPipe >= OT_VI_MAX_PIPE_NUM) {
        HI_PRINTF("viPipe is illgal %d!\n", viPipe);
        return TD_FAILURE;
    }

    viVpssMode->mode[viPipe] = OT_VI_ONLINE_VPSS_OFFLINE;
    return TD_SUCCESS;
}

static int32_t CameraSetParam(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    ot_vi_vpss_mode stVIVPSSMode = { 0 };

    int32_t ret = ss_mpi_sys_get_vi_vpss_mode(&stVIVPSSMode);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("Get VI-VPSS mode Param failed with %#x!\n", ret);
        return TD_FAILURE;
    }
    for (uint32_t i = 0; i < cameraCnt; i++) {
        ret = ConfigVIVPSSMode(&pstViConfig[i], &stVIVPSSMode);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("ConfigVIVPSSMode failed with %#x!\n", ret);
            return TD_FAILURE;
        }
    }
    ret = ss_mpi_sys_set_vi_vpss_mode(&stVIVPSSMode);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("Set VI-VPSS mode Param failed with %#x!\n", ret);
    }
    return TD_SUCCESS;
}

static int32_t ViGetDevAttr(ot_vi_dev_attr *pstViDevAttr, const CAMERA_VI_CONFIG_S *pstViConfig)
{
    int ret = memcpy_s(pstViDevAttr, sizeof(ot_vi_dev_attr), &g_devAttrDefaultBase, sizeof(ot_vi_dev_attr));
    if (ret != EOK) {
        HI_PRINTF("memcpy_s pstViDevAttr err\n");
    }
    pstViDevAttr->in_size.width = pstViConfig->devAttr->in_size.width;
    pstViDevAttr->in_size.height = pstViConfig->devAttr->in_size.height;

    return TD_SUCCESS;
}

static int32_t ViStartDev(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    ot_vi_dev_attr stViDevAttr = { 0 };

    ot_vi_dev ViDev = pstViConfig->stDevInfo.ViDev;
    ViGetDevAttr(&stViDevAttr, pstViConfig);
    if (pstViConfig->stPipeInfo.enMastPipeMode == OT_VI_PARALLEL_VPSS_OFFLINE ||
        pstViConfig->stPipeInfo.enMastPipeMode == OT_VI_PARALLEL_VPSS_PARALLEL) {
        stViDevAttr.data_rate = OT_DATA_RATE_X2;
    }

    int32_t ret = ss_mpi_vi_set_dev_attr(ViDev, &stViDevAttr);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vi_set_dev_attr failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_vi_enable_dev(ViDev);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vi_enable_dev failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static int32_t ViBindPipeDev(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    int32_t pipeCnt = 0;
    int32_t ret = 0;
    ot_vi_bind_pipe devBindPipe = { 0 };

    for (int32_t i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        ot_vi_pipe aPipe = pstViConfig->stPipeInfo.aPipe[i];
        if (aPipe >= 0 && aPipe < OT_VI_MAX_PIPE_NUM) {
            devBindPipe.pipe_id[pipeCnt] = aPipe;
            pipeCnt++;
            devBindPipe.pipe_num = pipeCnt;
            ret = ss_mpi_vi_bind(pstViConfig->stDevInfo.ViDev, aPipe);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ss_mpi_vi_bind failed with %#x!\n", ret);
                return TD_FAILURE;
            }
        }
    }

    return ret;
}

static int32_t ViStopSingleViPipe(ot_vi_pipe ViPipe)
{
    int32_t ret = ss_mpi_vi_stop_pipe(ViPipe);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vi_stop_pipe failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_vi_destroy_pipe(ViPipe);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vi_destroy_pipe failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    return ret;
}

static int32_t ViCreatePipe(const CAMERA_VI_CONFIG_S *pstViConfig, ot_vi_pipe ViPipe,
    ot_vi_pipe_attr *stPipeAttr, int32_t i)
{
    ot_frame_interrupt_attr frameIntAttr = { 0 };

    int32_t ret = ss_mpi_vi_create_pipe(ViPipe, stPipeAttr);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vi_create_pipe failed with %#x!\n", ret);
        return TD_FAILURE;
    }
    if (pstViConfig->stPipeInfo.bVcNumCfged == TD_TRUE) {
        ret = ss_mpi_vi_set_pipe_vc_number(ViPipe, pstViConfig->stPipeInfo.u32VCNum[i]);
        if (ret != TD_SUCCESS) {
            ss_mpi_vi_destroy_pipe(ViPipe);
            HI_PRINTF("ss_mpi_vi_set_pipe_vc_number failed with %#x!\n", ret);
            return TD_FAILURE;
        }
    }
    if (pstViConfig->stPipeInfo.frameInterruptType < OT_FRAME_INTERRUPT_BUTT &&
        pstViConfig->stDevInfo.wdrMode == OT_WDR_MODE_NONE) {
        frameIntAttr.interrupt_type = pstViConfig->stPipeInfo.frameInterruptType;
        frameIntAttr.early_line = pstViConfig->stPipeInfo.earlyLine;
        ret = ss_mpi_vi_set_pipe_frame_interrupt_attr(ViPipe, &frameIntAttr);
        HI_PRINTF(" ss_mpi_vi_set_pipe_frame_interrupt_attr type %d %u ret = %d\n", frameIntAttr.interrupt_type,
            frameIntAttr.early_line, ret);
    }
    ret = ss_mpi_vi_start_pipe(ViPipe);
    if (ret != TD_SUCCESS) {
        ss_mpi_vi_destroy_pipe(ViPipe);
        HI_PRINTF("ss_mpi_vi_start_pipe failed with %#x!\n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static int32_t ViDoCreatePipe(const CAMERA_VI_CONFIG_S *pstViConfig, ot_vi_pipe ViPipe,
    ot_vi_pipe_attr *stPipeAttr, int32_t index)
{
    int32_t ret;
    if ((pstViConfig->stSnapInfo.bSnap) && (pstViConfig->stSnapInfo.bDoublePipe) &&
        (ViPipe == pstViConfig->stSnapInfo.SnapPipe)) {
        ret = ss_mpi_vi_create_pipe(ViPipe, stPipeAttr);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("ss_mpi_vi_create_pipe failed with %#x!\n", ret);
            for (int32_t j = 0; j < index; j++) {
                ot_vi_pipe stopViPipe = j;
                (void)ViStopSingleViPipe(stopViPipe);
            }
            return ret;
        }
    } else {
        ret = ViCreatePipe(pstViConfig, ViPipe, stPipeAttr, index);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("ss_mpi_vi_create_pipe failed with %#x!\n", ret);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static int32_t ViStartViPipe(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    for (int32_t i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        ot_vi_pipe viPipe = pstViConfig->stPipeInfo.aPipe[i];
        if (viPipe >= 0 && viPipe < OT_VI_MAX_PIPE_NUM) {
            ot_vi_pipe_attr stPipeAttr;
            HAL_LOG_DOFUNC(memcpy_s(&stPipeAttr, sizeof(ot_vi_pipe_attr),
                pstViConfig->pipeAttr, sizeof(ot_vi_pipe_attr)));
            if (pstViConfig->stPipeInfo.ispBypass == TD_TRUE) {
                stPipeAttr.isp_bypass = TD_TRUE;
                stPipeAttr.pixel_format = pstViConfig->stPipeInfo.pixelFormat;
            }
            if ((viPipe == 0x2) || (viPipe == 0x3)) {
                stPipeAttr.compress_mode = OT_COMPRESS_MODE_NONE;
            }
            int32_t ret = ViDoCreatePipe(pstViConfig, viPipe, &stPipeAttr, i);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        }
    }
    return TD_SUCCESS;
}

static int32_t ViEnableChn(ot_vi_pipe ViPipe, ot_vi_chn ViChn, ot_vi_chn_attr stChnAttr, td_bool bNeedChn,
    const CAMERA_VI_CONFIG_S *pstViConfig)
{
    if (bNeedChn) {
        int32_t ret = ss_mpi_vi_set_chn_attr(ViPipe, ViChn, &stChnAttr);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("ss_mpi_vi_set_chn_attr failed with %#x!\n", ret);
            return ret;
        }
        ot_vi_vpss_mode_type enMastPipeMode = pstViConfig->stPipeInfo.enMastPipeMode;
        if (enMastPipeMode == OT_VI_OFFLINE_VPSS_OFFLINE || enMastPipeMode == OT_VI_ONLINE_VPSS_OFFLINE ||
            enMastPipeMode == OT_VI_PARALLEL_VPSS_OFFLINE) {
            ret = ss_mpi_vi_enable_chn(ViPipe, ViChn);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ss_mpi_vi_enable_chn failed with %#x!\n", ret);
                return ret;
            }
        }

#if defined(ENABLE_DISTRIBUTED_CAMERA_LOW_DELAY) && (ENABLE_DISTRIBUTED_CAMERA_LOW_DELAY != 0)
        ot_low_delay_info viLdyInfo;
        ret = ss_mpi_vi_get_chn_low_delay(ViPipe, ViChn, &viLdyInfo);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("ss_mpi_vi_get_chn_low_delay failed with %#x\n", ret);
            return TD_FAILURE;
        }
        viLdyInfo.enable = TD_TRUE;
        viLdyInfo.line_cnt = 16; /* 16:VDEC_LOW_DELAY_MIN_LINE */
        ret = ss_mpi_vi_set_chn_low_delay(ViPipe, ViChn, &viLdyInfo);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("ss_mpi_vi_set_chn_low_delay failed with %#x\n", ret);
            return TD_FAILURE;
        }
#endif
    }
    return TD_SUCCESS;
}

static int32_t ViStartViChn(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    for (int32_t i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        ot_vi_pipe viPipe = pstViConfig->stPipeInfo.aPipe[i];
        if (viPipe >= 0 && viPipe < OT_VI_MAX_PIPE_NUM) {
            ot_vi_chn ViChn = pstViConfig->stChnInfo.ViChn;
            ot_vi_chn_attr stChnAttr;
            HAL_LOG_DOFUNC(memcpy_s(&stChnAttr, sizeof(ot_vi_chn_attr),
                pstViConfig->chnAttr, sizeof(ot_vi_chn_attr)));
            td_bool bNeedChn = TD_FALSE;
            if (pstViConfig->stDevInfo.wdrMode == OT_WDR_MODE_NONE) {
                bNeedChn = TD_TRUE;
            } else {
                bNeedChn = (i > 0) ? TD_FALSE : TD_TRUE;
            }
            int32_t ret = ViEnableChn(viPipe, ViChn, stChnAttr, bNeedChn, pstViConfig);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ViEnableChn failed with %#x!\n", ret);
                return ret;
            }
        }
    }

    return TD_SUCCESS;
}
static int32_t ViStopViPipe(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    for (int32_t i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        ot_vi_pipe viPipe = pstViConfig->stPipeInfo.aPipe[i];
        if (viPipe >= 0 && viPipe < OT_VI_MAX_PIPE_NUM) {
            (void)ViStopSingleViPipe(viPipe);
        }
    }
    return TD_SUCCESS;
}
static int32_t ViStopDev(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    ot_vi_dev viDev = pstViConfig->stDevInfo.ViDev;
    ot_vi_bind_pipe bindPipe = {0};
    int32_t ret = ss_mpi_vi_get_bind_by_dev(viDev, &bindPipe);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vi_get_bind_by_dev failed with %#x!\n", ret);
        return TD_FAILURE;
    }
    for (int32_t i = 1; i <= bindPipe.pipe_num; i++) {
        ret = ss_mpi_vi_unbind(viDev, bindPipe.pipe_id[i]);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("ss_mpi_vi_unbind failed with %#x!\n", ret);
            return TD_FAILURE;
        }
    }
    ret = ss_mpi_vi_disable_dev(viDev);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vi_disable_dev failed with %#x!\n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static int32_t ViCreateSingleVi(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    int32_t ret = ViStartDev(pstViConfig);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_StartDev failed !\n");
        return TD_FAILURE;
    }
    ret = ViBindPipeDev(pstViConfig);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_BindPipeDev failed !\n");
        goto EXIT1;
    }
    ret = ViStartViPipe(pstViConfig);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_StartViPipe failed !\n");
        goto EXIT1;
    }
    ret = ViStartViChn(pstViConfig);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_VI_StartViChn failed !\n");
        goto EXIT2;
    }
    return TD_SUCCESS;
EXIT2:
    (void)ViStopViPipe(pstViConfig);
EXIT1:
    (void)ViStopDev(pstViConfig);
    return ret;
}

static int32_t ViDisableChn(const CAMERA_VI_CONFIG_S *pstViConfig, ot_vi_pipe ViPipe,
    ot_vi_chn ViChn, td_bool bNeedChn)
{
    if (bNeedChn) {
        ot_vi_vpss_mode_type enMastPipeMode = pstViConfig->stPipeInfo.enMastPipeMode;
        if (enMastPipeMode == OT_VI_OFFLINE_VPSS_OFFLINE || enMastPipeMode == OT_VI_ONLINE_VPSS_OFFLINE ||
            enMastPipeMode == OT_VI_PARALLEL_VPSS_OFFLINE) {
            int32_t ret = ss_mpi_vi_disable_chn(ViPipe, ViChn);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ss_mpi_vi_disable_chn failed with %#x!\n", ret);
                return ret;
            }
        }
    }
    return TD_SUCCESS;
}

static int32_t ViStopViChn(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    for (int32_t i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        ot_vi_pipe viPipe = pstViConfig->stPipeInfo.aPipe[i];
        if (viPipe >= 0 && viPipe < OT_VI_MAX_PIPE_NUM) {
            ot_vi_chn viChn = pstViConfig->stChnInfo.ViChn;
            td_bool bNeedChn = TD_FALSE;
            if (pstViConfig->stDevInfo.wdrMode == OT_WDR_MODE_NONE) {
                bNeedChn = TD_TRUE;
            } else {
                bNeedChn = (i > 0) ? TD_FALSE : TD_TRUE;
            }
            int32_t ret = ViDisableChn(pstViConfig, viPipe, viChn, bNeedChn);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ss_mpi_vi_disable_chn failed with %#x!\n", ret);
                return ret;
            }
        }
    }
    return TD_SUCCESS;
}

static int32_t ViDestroySingleVi(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    (void)ViStopViChn(pstViConfig);
    (void)ViStopViPipe(pstViConfig);
    (void)ViStopDev(pstViConfig);
    return TD_SUCCESS;
}
static int32_t CameraCreateVi(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    for (uint32_t i = 0; i < cameraCnt; i++) {
        int32_t ret = ViCreateSingleVi(&(pstViConfig[i]));
        if (ret != TD_SUCCESS) {
            HI_PRINTF("HISI_VI_CreateSingleVi failed !\n");
            (void)ViDestroySingleVi(&pstViConfig[0]);
        }
    }
    return TD_SUCCESS;
}

static td_bool IsValidPath(const char *path, int size)
{
    if ((path == NULL) || (size == 0) || (size > SENSOR_DESC_MAX_LEN)) {
        return TD_FALSE;
    }
    if (strpbrk(path, "\"*+,:;<=>\?[]|\x7F")) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

static void ViGetSnsObj(char *sensorLib, char *sensorObj, uint16_t cameraCnt, uint32_t snsId)
{
    char *pcdlsymError = NULL;
    if (!IsValidPath(sensorLib, cameraCnt)) {
        HI_PRINTF("sensor lib Path is valid Path:%s\n!", sensorLib);
        return;
    }
    if (snsId >= MAX_SENSOR_NUM) {
        HI_PRINTF("sns id error\n");
        return;
    }
    g_snsLibHandle[snsId] = dlopen(sensorLib, RTLD_NOW);
    if (g_snsLibHandle[snsId] == NULL) {
        pcdlsymError = dlerror();
        HI_PRINTF("dlopen sns lib error! %s\n", pcdlsymError);
        return;
    }
    (void)dlerror();
    g_snsObj[snsId] = (ot_isp_sns_obj *)dlsym(g_snsLibHandle[snsId], sensorObj);
    if (g_snsObj[snsId] == NULL) {
        (void)dlclose(g_snsLibHandle[snsId]);
        g_snsLibHandle[snsId] = NULL;
        return;
    }
    pcdlsymError = dlerror();
    if (pcdlsymError != NULL) {
        (void)dlclose(g_snsLibHandle[snsId]);
        g_snsLibHandle[snsId] = NULL;
        return;
    }
}

static const ot_isp_sns_obj *GetSnsObjById(uint32_t snsId)
{
    if (snsId >= MAX_SENSOR_NUM) {
        HI_PRINTF("invalid sensor id: %u\n", snsId);
        return NULL;
    }
    const ot_isp_sns_obj *snsObj = g_snsObj[snsId];
    return snsObj;
}

static int32_t ViSensorRegiterCallback(ot_isp_dev IspDev, uint32_t u32SnsId, const CAMERA_VI_CONFIG_S *pstViConfig)
{
    if (IspDev >= ISP_MAX_DEV_NUM) {
        return TD_FAILURE;
    }
    const ot_isp_sns_obj *snsObj = GetSnsObjById(u32SnsId);
    if (snsObj == NULL) {
        HI_PRINTF("sensor %u not exist!\n", u32SnsId);
        return TD_FAILURE;
    }
    ot_isp_3a_alg_lib stAeLib;
    ot_isp_3a_alg_lib stAwbLib;
    stAeLib.id = IspDev;
    stAwbLib.id = IspDev;
    if (strncpy_s(stAeLib.lib_name, sizeof(stAeLib.lib_name), OT_AE_LIB_NAME, sizeof(OT_AE_LIB_NAME)) != EOK) {
        HI_PRINTF("strncpy_s stAeLib.lib_name failed");
        return TD_FAILURE;
    }
    if (strncpy_s(stAwbLib.lib_name, sizeof(stAwbLib.lib_name), OT_AWB_LIB_NAME, sizeof(OT_AWB_LIB_NAME)) != EOK) {
        HI_PRINTF("strncpy_s stAwbLib.lib_name failed");
        return TD_FAILURE;
    }
    if (snsObj->pfn_register_callback != NULL) {
        int32_t ret = snsObj->pfn_register_callback(IspDev, &stAeLib, &stAwbLib);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("sensor_register_callback failed with %#x!\n", ret);
            return ret;
        }
    } else {
        HI_PRINTF("sensor_register_callback failed with TD_NULL!\n");
    }
    g_au32IspSnsId[IspDev] = u32SnsId;
    return TD_SUCCESS;
}

static int32_t ViIspBindSns(ot_isp_dev IspDev, uint32_t u32SnsId, td_s8 s8SnsDev)
{
    const ot_isp_sns_obj *snsObj = GetSnsObjById(u32SnsId);
    if (snsObj == NULL) {
        HI_PRINTF("sensor %u not exist!\n", u32SnsId);
        return TD_FAILURE;
    }
    ot_isp_sns_commbus uSnsBusInfo = { 0 };
    uSnsBusInfo.i2c_dev = s8SnsDev;
    if (snsObj->pfn_set_bus_info != NULL) {
        int32_t ret = snsObj->pfn_set_bus_info(IspDev, uSnsBusInfo);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("set sensor bus info failed with %#x!\n", ret);
            return ret;
        }
    } else {
        HI_PRINTF("not support set sensor bus info!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static int32_t ViIspAwblibUnCallback(ot_isp_dev IspDev)
{
    ot_isp_3a_alg_lib stAwbLib;
    stAwbLib.id = IspDev;
    HAL_LOG_DOFUNC(strncpy_s(stAwbLib.lib_name, sizeof(stAwbLib.lib_name), OT_AWB_LIB_NAME, sizeof(OT_AWB_LIB_NAME)));
    CHECK_RET(ss_mpi_awb_unregister(IspDev, &stAwbLib), "awblib unregister call back");
    return TD_SUCCESS;
}
static int32_t ViIspAelibUnCallback(ot_isp_dev IspDev)
{
    ot_isp_3a_alg_lib stAeLib;
    stAeLib.id = IspDev;
    HAL_LOG_DOFUNC(strncpy_s(stAeLib.lib_name, sizeof(stAeLib.lib_name), OT_AE_LIB_NAME, sizeof(OT_AE_LIB_NAME)));
    CHECK_RET(ss_mpi_ae_unregister(IspDev, &stAeLib), "aelib unregister call back");
    return TD_SUCCESS;
}

static int32_t ViIspSensorUnRegiterCallback(ot_isp_dev IspDev)
{
    ot_isp_3a_alg_lib stAeLib;
    ot_isp_3a_alg_lib stAwbLib;
    uint32_t snsId = g_au32IspSnsId[IspDev];
    if (snsId >= MAX_SENSOR_NUM) {
        HI_PRINTF("%s: invalid sensor id: %u\n", __FUNCTION__, snsId);
        return TD_FAILURE;
    }
    const ot_isp_sns_obj *snsObj = g_snsObj[snsId];
    if (snsObj == NULL) {
        return TD_FAILURE;
    }
    stAeLib.id = IspDev;
    stAwbLib.id = IspDev;
    HAL_LOG_DOFUNC(strncpy_s(stAeLib.lib_name, sizeof(stAeLib.lib_name), OT_AE_LIB_NAME, sizeof(OT_AE_LIB_NAME)));
    HAL_LOG_DOFUNC(strncpy_s(stAwbLib.lib_name, sizeof(stAwbLib.lib_name), OT_AWB_LIB_NAME, sizeof(OT_AWB_LIB_NAME)));
    if (snsObj->pfn_un_register_callback != NULL) {
        int32_t ret = snsObj->pfn_un_register_callback(IspDev, &stAeLib, &stAwbLib);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("sensor_unregister_callback failed with %#x!\n", ret);
            return ret;
        }
    } else {
        HI_PRINTF("sensor_unregister_callback failed with TD_NULL!\n");
    }
    return TD_SUCCESS;
}

static void ViIspStop(ot_isp_dev IspDev)
{
    if (IspDev >= ISP_MAX_DEV_NUM) {
        return;
    }
    if (g_ispPid[IspDev] != 0) {
        ss_mpi_isp_exit(IspDev);
        pthread_join(g_ispPid[IspDev], NULL);
        ViIspAwblibUnCallback(IspDev);
        ViIspAelibUnCallback(IspDev);
        ViIspSensorUnRegiterCallback(IspDev);
        g_ispPid[IspDev] = 0;
    }
    return;
}

static int32_t ViIspAelibCallback(ot_isp_dev IspDev)
{
    ot_isp_3a_alg_lib stAeLib;
    stAeLib.id = IspDev;
    HAL_LOG_DOFUNC(strncpy_s(stAeLib.lib_name, sizeof(stAeLib.lib_name), OT_AE_LIB_NAME, sizeof(OT_AE_LIB_NAME)));
    CHECK_RET(ss_mpi_ae_register(IspDev, &stAeLib), "aelib register call back");
    return TD_SUCCESS;
}

static int32_t ViIspAwblibCallback(ot_isp_dev IspDev)
{
    ot_isp_3a_alg_lib stAwbLib;
    stAwbLib.id = IspDev;
    HAL_LOG_DOFUNC(strncpy_s(stAwbLib.lib_name, sizeof(stAwbLib.lib_name), OT_AWB_LIB_NAME, sizeof(OT_AWB_LIB_NAME)));
    CHECK_RET(ss_mpi_awb_register(IspDev, &stAwbLib), "awblib register call back");
    return TD_SUCCESS;
}

static void *ViIspThread(void *param)
{
    char szThreadName[THREAD_NAME_SIZE];
    ot_isp_dev IspDev = (ot_isp_dev)param;
    if (snprintf_s(szThreadName, THREAD_NAME_SIZE, THREAD_NAME_SIZE - 1, "ISP%d_RUN", IspDev) >= 0) {
        prctl(PR_SET_NAME, szThreadName, 0, 0, 0);
    }

    HI_PRINTF("ISP Dev %d running !\n", IspDev);
    int32_t ret = ss_mpi_isp_run(IspDev);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_isp_run failed with %#x!\n", ret);
    }
    return NULL;
}

/* *****************************************************************************
 * funciton : ISP Run
 * **************************************************************************** */
static int32_t ViIspRun(ot_isp_dev IspDev)
{
    pthread_attr_t *pstAttr = NULL;
    int32_t ret = pthread_create(&g_ispPid[IspDev], pstAttr, ViIspThread, (void *)IspDev);
    if (ret != 0) {
        HI_PRINTF("create isp running thread failed!, error: %d\n", ret);
        goto out;
    }
out:
    if (pstAttr != NULL) {
        pthread_attr_destroy(pstAttr);
    }
    return ret;
}

static int32_t ViIspPrepare(const CAMERA_VI_CONFIG_S *pstViConfig, ot_vi_pipe ViPipe, uint32_t u32SnsId, int32_t index)
{
    int32_t ret = ViSensorRegiterCallback(ViPipe, u32SnsId, pstViConfig);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("register sensor %u to ISP %d failed\n", u32SnsId, ViPipe);
        ViIspStop(ViPipe);
        return TD_FAILURE;
    }
    if (((pstViConfig->stSnapInfo.bDoublePipe) && (pstViConfig->stSnapInfo.SnapPipe == ViPipe)) ||
        (pstViConfig->stPipeInfo.bMultiPipe && index > 0)) {
        ret = ViIspBindSns(ViPipe, u32SnsId, -1);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("register sensor %u bus id %d failed\n", u32SnsId, pstViConfig->stSnsInfo.s32BusId);
            ViIspStop(ViPipe);
            return TD_FAILURE;
        }
    } else {
        ret = ViIspBindSns(ViPipe, u32SnsId, pstViConfig->stSnsInfo.s32BusId);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("register sensor %u bus id %d failed\n", u32SnsId, pstViConfig->stSnsInfo.s32BusId);
            ViIspStop(ViPipe);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static int32_t ViRunIsp(ot_vi_pipe ViPipe, ot_isp_pub_attr stPubAttr)
{
    int32_t ret = ViIspAelibCallback(ViPipe);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_ISP_Aelib_Callback failed\n");
        ViIspStop(ViPipe);
        return TD_FAILURE;
    }

    ret = ViIspAwblibCallback(ViPipe);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("HISI_ISP_Awblib_Callback failed\n");
        ViIspStop(ViPipe);
        return TD_FAILURE;
    }

    ret = ss_mpi_isp_mem_init(ViPipe);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("Init Ext memory failed with %#x!\n", ret);
        ViIspStop(ViPipe);
        return TD_FAILURE;
    }

    ret = ss_mpi_isp_set_pub_attr(ViPipe, &stPubAttr);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("SetPubAttr failed with %#x!\n", ret);
        ViIspStop(ViPipe);
        return TD_FAILURE;
    }

    ret = ss_mpi_isp_init(ViPipe);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ISP Init failed with %#x!\n", ret);
        ViIspStop(ViPipe);
        return TD_FAILURE;
    }

    ret = ViIspRun(ViPipe);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ISP Run failed with %#x!\n", ret);
        ViIspStop(ViPipe);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static int32_t ViStartIsp(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    if (pstViConfig->pipeAttr->pipe_bypass_mode == OT_VI_PIPE_BYPASS_BE) {
        return TD_SUCCESS;
    }
    for (int32_t i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        ot_vi_pipe viPipe = pstViConfig->stPipeInfo.aPipe[i];
        if (viPipe >= 0 && viPipe < OT_VI_MAX_PIPE_NUM) {
            uint32_t u32SnsId = pstViConfig->stSnsInfo.s32SnsId;
            ot_isp_pub_attr stPubAttr;
            HAL_LOG_DOFUNC(memcpy_s(&stPubAttr, sizeof(ot_isp_pub_attr),
                pstViConfig->ispPubAttr, sizeof(ot_isp_pub_attr)));
            td_bool bNeedPipe = TD_FALSE;
            if (pstViConfig->stDevInfo.wdrMode == OT_WDR_MODE_NONE) {
                bNeedPipe = TD_TRUE;
            } else {
                bNeedPipe = (i > 0) ? TD_FALSE : TD_TRUE;
            }
            if (bNeedPipe != TD_TRUE) {
                continue;
            }
            int32_t ret = ViIspPrepare(pstViConfig, viPipe, u32SnsId, i);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ViIspPrepare failed with %#x!\n", ret);
                ViIspStop(viPipe);
                return TD_FAILURE;
            }
            ret = ViRunIsp(viPipe, stPubAttr);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ISP Run failed with %#x!\n", ret);
                ViIspStop(viPipe);
                return TD_FAILURE;
            }
        }
    }

    return TD_SUCCESS;
}

static int32_t CameraCreateIsp(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    int32_t ret = TD_SUCCESS;
    for (uint32_t i = 0; i < cameraCnt; i++) {
        ret = ViStartIsp(&pstViConfig[i]);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("HISI_VI_StartIsp failed !\n");
            break;
        }
    }
    return ret;
}

static int32_t CameraDestroyVi(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    for (uint32_t i = 0; i < cameraCnt; i++) {
        (void)ViDestroySingleVi(&pstViConfig[i]);
    }
    return TD_SUCCESS;
}

int32_t CameraStartVi(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    if (pstViConfig == NULL) {
        HI_PRINTF("%s: null ptr\n", __FUNCTION__);
        return TD_FAILURE;
    }
    if (cameraCnt > MAX_SENSOR_NUM) {
        HI_PRINTF("ptr cameraCnt %u error\n", cameraCnt);
        return TD_FAILURE;
    }
    HI_PRINTF("lib:%s, obj:%s, len:%u\n", pstViConfig[0].sensorLib, pstViConfig[0].sensorObj, pstViConfig[0].len);
    uint32_t snsId = pstViConfig[0].stSnsInfo.s32SnsId;
    if (snsId >= MAX_SENSOR_NUM) {
        HI_PRINTF("sns id error\n");
        return TD_FAILURE;
    }
    ViGetSnsObj(pstViConfig[0].sensorLib, pstViConfig[0].sensorObj, pstViConfig[0].len, snsId);
    if (cameraCnt == MAX_SENSOR_NUM) {
        uint32_t snsId1 = pstViConfig[1].stSnsInfo.s32SnsId;
        ViGetSnsObj(pstViConfig[1].sensorLib, pstViConfig[1].sensorObj, pstViConfig[1].len, snsId1);
    }
    int32_t ret = CameraStartMIPI(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("CameraStartMIPI failed!\n");
        return TD_FAILURE;
    }
    ret = CameraSetParam(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("CameraSetParam failed!\n");
        return TD_FAILURE;
    }

    ret = CameraCreateVi(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("CameraCreateVi failed!\n");
        return TD_FAILURE;
    }

    ret = CameraCreateIsp(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        CameraDestroyVi(pstViConfig, cameraCnt);
        HI_PRINTF("CameraCreateIsp failed!\n");
        return TD_FAILURE;
    }

    return ret;
}

static int32_t ViStopIsp(const CAMERA_VI_CONFIG_S *pstViConfig)
{
    for (int32_t i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        ot_vi_pipe viPipe = pstViConfig->stPipeInfo.aPipe[i];
        if (viPipe >= 0 && viPipe < OT_VI_MAX_PIPE_NUM) {
            td_bool bNeedPipe = TD_FALSE;
            if (pstViConfig->stDevInfo.wdrMode == OT_WDR_MODE_NONE) {
                bNeedPipe = TD_TRUE;
            } else {
                bNeedPipe = (i > 0) ? TD_FALSE : TD_TRUE;
            }
            if (bNeedPipe != TD_TRUE) {
                continue;
            }
            ViIspStop(viPipe);
        }
    }
    return TD_SUCCESS;
}

static int32_t CameraDestroyIsp(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    int32_t ret = TD_SUCCESS;
    for (uint32_t i = 0; i < cameraCnt; i++) {
        ret = ViStopIsp(&pstViConfig[i]);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("HISI_VI_StopIsp failed !\n");
        }
    }
    return ret;
}

int32_t CameraStopVi(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    if (pstViConfig == NULL) {
        HI_PRINTF("%s: null ptr\n", __FUNCTION__);
        return TD_FAILURE;
    }
    if (cameraCnt > MAX_SENSOR_NUM) {
        HI_PRINTF("ptr len %u error\n", cameraCnt);
        return TD_FAILURE;
    }
    int32_t ret = CameraDestroyIsp(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("CameraDestroyIsp failed !\n");
        return TD_FAILURE;
    }
    ret = CameraDestroyVi(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("CameraDestroyVi failed !\n");
        return TD_FAILURE;
    }

    ret = CameraStopMIPI(pstViConfig, cameraCnt);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("CameraStopMIPI failed !\n");
        return TD_FAILURE;
    }
    uint32_t snsId = pstViConfig->stSnsInfo.s32SnsId;
    if (snsId >= MAX_SENSOR_NUM) {
        HI_PRINTF("sns id error\n");
        return TD_FAILURE;
    }
    if (g_snsLibHandle[snsId] != NULL) {
        (void)dlclose(g_snsLibHandle[snsId]);
        g_snsLibHandle[snsId] = NULL;
        g_snsObj[snsId] = NULL;
        HI_PRINTF("delete sensor lib snsid %u\n", snsId);
    }
    return ret;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

