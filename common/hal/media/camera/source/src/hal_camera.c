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

#include "hal_camera.h"
#include <dirent.h>
#include "hi_camera_vi.h"

#include "display_layer.h"
#include "media_hal_common.h"
#include "ot_common_isp.h"
#include "ss_mpi_isp.h"
#include "ot_common_awb.h"
#include "ss_mpi_awb.h"

#ifdef KERNEL_USER_MODE
#include "sdk.h"
#endif


#define STREAM_ABILITY_NUM 3
#define JPEG_VPSS_CHN 0
#define PREEV_VPSS_CHN 1
#define VIDEO_VPSS_CHN 2
#define CALLBACK_VPSS_CHN 2
#define CALLBACK_EXT_CHN 6
#define VIDEO_EXT_MIN_CHN (OT_VPSS_MAX_PHYS_CHN_NUM + 1)
#define VIDEO_EXT_MAX_CHN (OT_VPSS_MAX_PHYS_CHN_NUM + 2)
#define HI_ALIGN_DOWN_NUM 2
#define HI_ALIGN_UP_NUM 2
#define MAX_FPS 60
#define MIN_FPS 1
#define FRAME_TIME_OUT 1000
#define SUPPORT_MAX_CAMERA_NUM 2
#define CAEMRA_MAX_STREAM_NUM 10

#define CAMERA_TOF "CAMERA_TOF"
#define CAMERA_RGB "CAMERA_RGB"
#define KEY_EXPOSURE_MODE "exposuremode"
#define KEY_SATURATION_MODE "saturationmode"
#define KEY_SHARPEN_MODE "sharpenmode"
#define KEY_DRC_MODE "drcmode"
#define KEY_WB_MODE "wbmode"

#define CAMERA_INIT_PARAM_PATH "/storage/data/cameradev.ini"
#define MODE_NAME_MAX_LEN 64
#define MODE_NAME_MAX_NUM 16
#define CAMERA_INIT_DOC_PATH "/storage/data/"
#define ENLARGE_VALUE 2
#define DISPLAY_DEVID 0

/* default layer width */
#define DEFAULT_WIDTH_FOR_HDMI 1920
#define DEFAULT_WIDTH_FOR_LCD 480
/* default layer height */
#define DEFAULT_HEIGHT_FOR_HDMI 1080
#define DEFAULT_HEIGHT_FOR_LCD 960
#define DEFAULT_DISPLAY_FPS 60
#define MIN_WIDTH 16
#define MIN_HEIGHT 16
#define RESOLUTION_2592X1944_W 2592
#define RESOLUTION_2592X1944_H 1944
#define RESOLUTION_1080P_W 1920
#define RESOLUTION_1080P_H 1080
#define RESOLUTION_720P_W 1280
#define RESOLUTION_720P_H 720
#define RESOLUTION_D1_NTSC_W 720
#define RESOLUTION_D1_NTSC_H 480
#define RESOLUTION_360P_W 640
#define RESOLUTION_360P_H 360
#define FPS_60 60
#define MAX_KEY_STR_SIZE 30
#define MAX_KEY_VALUE_SIZE 512
#define BLACK_DELAY_FRAME 10
#define ANTIFLIKER_FREQUENCY 50
#define EXP_TIME_RANGE_MAX 4000
#define EXP_TIME_RANGE_MIN 10
#define NUMBER_BASE_10 10

/* Maximum number of display devices supported */
#define DEV_ID_NUM 1
/* Byte align number */
#define BYTE_ALIGN_NUM           2
/* Video layer buffer len */
#define VIDEO_LAYER_BUFF_LEN     3
/* Vo device number */
#define VO_LAYER_ID              0

/* Rotate enable */
#define VO_ROTATE_ENABLE         1

LayerFuncs *g_layerInterface = NULL;

typedef struct {
    bool isMirror;
    bool isFlip;
    ot_rotation_attr rotation;
    uint32_t fps;
    uint32_t width;
    uint32_t height;
    ot_vpss_crop_info cropInfo;
    uint32_t maxW;
    uint32_t maxH;
    int32_t vpssGrp;
} ControlInfo;

typedef enum {
    HAL_CENTER_MIRROR = 0,     /**< Central mirroring */
    HAL_HORIZONTAL_MIRROR = 1, /**< Horizontal mirroring */
    HAL_VETICALLY_MIRROR = 2,  /**< Vertical mirroring */
    HAL_ROTATION_90 = 3,       /**< 90-degree rotation */
    HAL_ROTATION_180 = 4,      /**< 180-degree rotation  */
    HAL_ROTATION_BUTT,
} CameraHalInvertMode;

typedef struct {
    int32_t vpssChn;
    PosInfo pos;
    bool isUsed;
    bool isSetDepth;
    ot_video_frame_info videoFrame;
} InternalStreamInfo;

typedef struct {
    BufferAvailable bufferCallBack;
    CameraResultCb resultCb;
    StreamAttr streamAttr[CAEMRA_MAX_STREAM_NUM];
    InternalStreamInfo internalStreamInfo[CAEMRA_MAX_STREAM_NUM];
    bool isOpen;
    CameraType type;
    int32_t vpssGrp;
    uint32_t layerId;
} CameraInfo;

typedef struct {
    uint8_t modeNum;
    uint8_t index;
    char modeNameList[MODE_NAME_MAX_NUM][MODE_NAME_MAX_LEN];
} ModeInfo;
static ModeInfo g_modeInfo = { 0 };

static CameraInfo g_cameraInfo[SUPPORT_MAX_CAMERA_NUM] = { 0 };
static InitParam g_initParam[SUPPORT_MAX_CAMERA_NUM] = { 0 };
static CameraDetectCb g_cameraDetect = NULL;
static uint8_t g_cameraNum = 2;
static bool g_isInit = false;

static td_void AUDIO_init(void)
{
    td_s32 ret;

    ret = ot_aiao_mod_init();
    if (ret != 0) {
        HAL_LOGE("aiao init error.\n");
    }

    ret = ot_ai_mod_init();
    if (ret != 0) {
        HAL_LOGE("ai init error.\n");
    }

    ret = ot_ao_mod_init();
    if (ret != 0) {
        HAL_LOGE("ao init error.\n");
    }

    ret = ot_aenc_mod_init();
    if (ret != 0) {
        HAL_LOGE("aenc init error.\n");
    }

    ret = ot_adec_mod_init();
    if (ret != 0) {
        HAL_LOGE("adec init error.\n");
    }

    ret = ot_acodec_mod_init();
    if (ret != 0) {
        HAL_LOGE("acodec init error.\n");
    }
}

static td_s32 basic_mod_init()
{
    td_s32 ret;
    ret = osal_init();
    if (ret != 0) {
        HAL_LOGE("osal_init init error.\n");
    }

    ret = ot_base_mod_init();
    if (ret != 0) {
        HAL_LOGE("base init error.\n");
    }

    ret = ot_sys_mod_init();
    if (ret != 0) {
        HAL_LOGE("sys init error.\n");
    }

    ret = ot_chnl_mod_init();
    if (ret != 0) {
        HAL_LOGE("chnl init error.\n");
    }
    return ret;
}

static td_s32 common_mod_init()
{
    td_s32 ret;
    ret = ot_rgn_mod_init();
    if (ret != 0) {
        HAL_LOGE("rgn init error.\n");
    }
    struct {
        td_u32 u32_max_gdc_job;
        td_u32 u32_max_gdc_task;
        td_u32 u32_max_gdc_node;
        td_u32 au32_gdc_en[1];
    } gdcModuleParam;
    gdcModuleParam.u32_max_gdc_job  = 32; /* 32 -- max job num */
    gdcModuleParam.u32_max_gdc_task  = 64; /* 64 -- max task num */
    gdcModuleParam.u32_max_gdc_node  = 64; /* 64 -- max node num */
    gdcModuleParam.au32_gdc_en[0]   = 1;
    ret = ot_gdc_mod_init(&gdcModuleParam);
    if (ret != 0) {
        HAL_LOGE("gdc init error.\n");
    }

    struct {
        td_u32 max_vgs_job;
        td_u32 max_vgs_task;
        td_u32 max_vgs_node;
    } vgsModuleParam;
    vgsModuleParam.max_vgs_job  = 64;  /* 64 -- max job num */
    vgsModuleParam.max_vgs_task = 200; /* 200 -- max task num */
    vgsModuleParam.max_vgs_node = 200; /* 200 -- max node num */
    ret = ot_vgs_mod_init(&vgsModuleParam);
    if (ret != 0) {
        HAL_LOGE("vgs init error.\n");
    }

    ret = ot_dis_mod_init();
    if (ret != 0) {
        HAL_LOGE("dis init error.\n");
    }

    ret = ot_vpp_mod_init();
    if (ret != 0) {
        HAL_LOGE("vpp init error.\n");
    }
    return ret;
}

static td_s32 business_mod_init()
{
    td_s32 ret;
    ret = ot_vi_mod_init();
    if (ret != 0) {
        HAL_LOGE("vi init error.\n");
    }

    ret = ot_isp_mod_init();
    if (ret != 0) {
        HAL_LOGE("isp init error.\n");
    }

    ret = ot_vpss_mod_init(NULL);
    if (ret != 0) {
        HAL_LOGE("vpss init error.\n");
    }

    ret = ot_vo_mod_init();
    if (ret != 0) {
        HAL_LOGE("vo init error.\n");
    }

    ret = ot_vedu_mod_init();
    if (ret != 0) {
        HAL_LOGE("vedu init error.\n");
    }

    ret = ot_rc_mod_init();
    if (ret != 0) {
        HAL_LOGE("rc init error.\n");
    }

    td_u32 max_chn = 64;
    ret = ot_venc_mod_init(max_chn);
    if (ret != 0) {
        HAL_LOGE("venc init error.\n");
    }

    ret = ot_h264e_mod_init();
    if (ret != 0) {
        HAL_LOGE("H264e init error.\n");
    }

    ret = ot_h265e_mod_init(NULL);
    if (ret != 0) {
        HAL_LOGE("H265e init error.\n");
    }

    ret = ot_jpege_mod_init();
    if (ret != 0) {
        HAL_LOGE("jpege init error.\n");
    }
    return ret;
}

static td_s32 business_mod_init1()
{
    td_s32 ret;

    ret = ot_jpegd_mod_init();
    if (ret != 0) {
        HAL_LOGE("jpegd init error.\n");
    }

    ret = ot_vfmw_mod_init(32); /* 32 */
    if (ret != 0) {
        HAL_LOGE("vfmw init error.\n");
    }

    ret = ot_vdec_mod_init();
    if (ret != 0) {
        HAL_LOGE("vdec init error.\n");
    }

    AUDIO_init();

    return ret;
}

td_void SDK_init(void)
{
    td_s32 ret;
    ret = basic_mod_init();
    if (ret != 0) {
        HAL_LOGE("basic_mod_init error.\n");
        return;
    }

    ret = common_mod_init();
    if (ret != 0) {
        HAL_LOGE("common_mod_init error.\n");
        return;
    }

    ret = business_mod_init();
    if (ret != 0) {
        HAL_LOGE("business_mod_init error.\n");
        return;
    }

    ret = business_mod_init1();
    if (ret != 0) {
        HAL_LOGE("basic_mod_init error.\n");
        return;
    }

    ot_dev_init();

    HAL_LOGI("SDK init ok...\n");
}

static td_void AUDIO_exit(void)
{
    ot_acodec_mod_exit();
    ot_adec_mod_exit();
    ot_aenc_mod_exit();
    ot_ao_mod_exit();
    ot_ai_mod_exit();
    ot_aiao_mod_exit();
}

td_void SDK_exit(void)
{
    ot_dev_exit();
    AUDIO_exit();
    ot_vdec_mod_exit();
    ot_vfmw_mod_exit();
    ot_jpegd_mod_exit();
    ot_jpege_mod_exit();
    ot_h265e_mod_exit();
    ot_h264e_mod_exit();
    ot_venc_mod_exit();
    ot_rc_mod_exit();
    ot_vedu_mod_exit();
    ot_vo_mod_exit();
    ot_vpss_mod_exit();
    ot_isp_mod_exit();
    ot_vi_mod_exit();
    ot_vpp_mod_exit();
    ot_dis_mod_exit();
    ot_vgs_mod_exit();
    ot_gdc_mod_exit();
    ot_rgn_mod_exit();
    ot_chnl_mod_exit();
    ot_sys_mod_exit();
    ot_base_mod_exit();
    osal_exit();
    HAL_LOGI("SDK exit ok...\n");
}

static int Cmp(const void *a, const void *b)
{
    return strcmp((char *)a, (char *)b);
}

static void InitModeInfo(void)
{
    DIR *dir = opendir(CAMERA_INIT_DOC_PATH);
    struct dirent *ptr = NULL;
    if (dir == NULL) {
        HAL_LOGE("Open dir error...\n");
        return;
    }

    uint8_t i = 0;
    while (i < MODE_NAME_MAX_NUM) {
        ptr = readdir(dir);
        if (ptr == NULL) {
            HAL_LOGI("break\n");
            break;
        }
        if (strncmp(ptr->d_name, "camera", 0x6) == 0) {
            HAL_LOGI("file name:%s\n", ptr->d_name);
            HAL_LOG_DOFUNC(strncpy_s(&g_modeInfo.modeNameList[i][0], MODE_NAME_MAX_LEN - 1,
                ptr->d_name, ptr->d_reclen));
            i++;
        }
    }

    g_modeInfo.modeNum = i;
    HAL_LOGI("mode num =%u\n", g_modeInfo.modeNum);
    qsort(g_modeInfo.modeNameList, i, MODE_NAME_MAX_LEN, Cmp);
    closedir(dir);
}

static uint8_t GetCurrentMode(void)
{
    ot_vb_cfg gotConfig = { 0 };
    uint8_t index = 0;

    int32_t ret = ss_mpi_vb_get_cfg(&gotConfig);
    if (ret == TD_SUCCESS && gotConfig.max_pool_cnt != 0) {
        index = (uint8_t)((gotConfig.common_pool[0].blk_size - ENLARGE_VALUE) / ENLARGE_VALUE);
    }
    if (index >= g_modeInfo.modeNum) {
        HI_PRINTF("curr mode is wrong %u\n", index);
        index = 0;
    }
    HI_PRINTF("CurrentMode is %u\n", index);
    return index;
}

static void CameraSaveModeIndexWithVb(ot_vb_cfg *vbConfig)
{
    vbConfig->max_pool_cnt = 1;
    vbConfig->common_pool[0].blk_size = g_modeInfo.index * ENLARGE_VALUE + ENLARGE_VALUE;
    vbConfig->common_pool[0].blk_cnt = 1;
}

static void CameraSysConfigVb(ot_vb_cfg *pstVbConfig, const VB_PARAM_CONFIG_S *pstInitParam)
{
    uint32_t blkSize;
    pstVbConfig->max_pool_cnt += pstInitParam->stMaxPoolCnt;
    // The modeIndex of camera is save the NO.0, So starting with the 1st
    for (uint32_t i = 1; i < pstVbConfig->max_pool_cnt; i++) {
        uint32_t paramIndex = i - 1; // initParam starting with index 0
        if (pstInitParam->stPixFormat[paramIndex] == OT_PIXEL_FORMAT_RGB_BAYER_12BPP) {
            ot_pic_buf_attr buf_attr = {0};
            buf_attr.width = pstInitParam->stImageSize[paramIndex].width;
            buf_attr.height = pstInitParam->stImageSize[paramIndex].height;
            buf_attr.pixel_format = pstInitParam->stPixFormat[paramIndex];
            buf_attr.bit_width = pstInitParam->stDataBitWidth[paramIndex];
            buf_attr.compress_mode = pstInitParam->stCompressMode[paramIndex];
            buf_attr.align = pstInitParam->stDefaultAlign[paramIndex];
            blkSize = ot_common_get_pic_buf_size(&buf_attr);
        } else if (pstInitParam->stPixFormat[paramIndex] == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
            ot_pic_buf_attr buf_attr = {0};
            buf_attr.width = pstInitParam->stImageSize[paramIndex].width;
            buf_attr.height = pstInitParam->stImageSize[paramIndex].height;
            buf_attr.pixel_format = pstInitParam->stPixFormat[paramIndex];
            buf_attr.bit_width = pstInitParam->stDataBitWidth[paramIndex];
            buf_attr.compress_mode = pstInitParam->stCompressMode[paramIndex];
            buf_attr.align = pstInitParam->stDefaultAlign[paramIndex];
            blkSize = ot_common_get_pic_buf_size(&buf_attr);
            pstVbConfig->common_pool[i].blk_size = blkSize;
            pstVbConfig->common_pool[i].blk_cnt = pstInitParam->stBlkCount[paramIndex];
        } else {
            HI_PRINTF("other vb:%u,%u\n", paramIndex, pstInitParam->stPixFormat[paramIndex]);
        }
    }
}

static int32_t CameraVbInit(ot_vb_cfg *pstVbConfig)
{
    int32_t ret = ss_mpi_vb_set_cfg(pstVbConfig);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vb_set_cfg failed 0x%x!\n", ret);
        return TD_FAILURE;
    }
    ot_vb_supplement_cfg stSupplementConf = { 0 };
    stSupplementConf.supplement_cfg = TD_TRUE;
    ret = ss_mpi_vb_set_supplement_cfg(&stSupplementConf);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vb_set_supplement_cfg failed!\n");
        return TD_FAILURE;
    }
    ret = ss_mpi_vb_init();
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vb_init failed!\n");
        return TD_FAILURE;
    }
    HAL_LOGI(" vb config success \n");
    return TD_SUCCESS;
}

static int32_t CameraCreateVbPool(void)
{
    ot_vb_cfg vbConfig = { 0 };
    ot_vb_cfg gotConfig = { 0 };
    CameraSaveModeIndexWithVb(&vbConfig);
    CameraSysConfigVb(&vbConfig, &g_initParam[0].stVbAttr);
    td_s32 ret = ss_mpi_vb_get_cfg(&gotConfig);
    if (ret != TD_SUCCESS || gotConfig.max_pool_cnt == 0) {
        ret = CameraVbInit(&vbConfig);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("system init failed with %d!\n", ret);
            return TD_FAILURE;
        }
    }
    ret = MediaSystemInit();
    if (ret != TD_SUCCESS) {
        HI_PRINTF("MediaSystemInit failed.s32Ret:0x%x !\n", ret);
    }
    return TD_SUCCESS;
}

static void CameraDestoryVbPool(void)
{
    int32_t ret = MediaSystemDeinit();
    if (ret != TD_SUCCESS) {
        HI_PRINTF("MediaSystemDeinit failed 0x%x!\n", ret);
        return;
    }
    ret = ss_mpi_vb_exit();
    if (ret != TD_SUCCESS) {
        ret = ss_mpi_vb_exit();
        if (ret != TD_SUCCESS) {
            HI_PRINTF("ss_mpi_vb_exit 1failed 0x%x!\n", ret);
        }
    }
}
static void CameraGetVpssGrpConfig(ot_vpss_grp_attr *pstVpssGrpAttr, const InitParam *initParam)
{
    if (memset_s(pstVpssGrpAttr, sizeof(ot_vpss_grp_attr), 0, sizeof(ot_vpss_grp_attr)) != EOK) {
        HI_PRINTF("memset_s pstVpssGrpAttr failed!");
    }
    pstVpssGrpAttr->frame_rate.src_frame_rate = FPS_60;
    pstVpssGrpAttr->frame_rate.dst_frame_rate = FPS_60;
    pstVpssGrpAttr->dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    pstVpssGrpAttr->pixel_format = initParam->stViAttr.stChnAttr[0][0].pixel_format;
    pstVpssGrpAttr->max_width = initParam->stViAttr.stChnAttr[0][0].size.width;
    pstVpssGrpAttr->max_height = initParam->stViAttr.stChnAttr[0][0].size.height;
}

static void CameraGetVpssChnConfig(ot_vpss_chn_attr pstVpssChnAttr[OT_VPSS_MAX_PHYS_CHN_NUM],
    bool chnEnable[OT_VPSS_MAX_PHYS_CHN_NUM], const InitParam *initParam)
{
    for (uint32_t i = 0; i < OT_VPSS_MAX_PHYS_CHN_NUM; i++) {
        pstVpssChnAttr[i].width = initParam->stViAttr.stChnAttr[0][0].size.width;
        pstVpssChnAttr[i].height = initParam->stViAttr.stChnAttr[0][0].size.height;
        pstVpssChnAttr[i].chn_mode = OT_VPSS_CHN_MODE_USER;
        pstVpssChnAttr[i].compress_mode = OT_COMPRESS_MODE_NONE;
        pstVpssChnAttr[i].dynamic_range = OT_DYNAMIC_RANGE_SDR8;
        pstVpssChnAttr[i].video_format = OT_VIDEO_FORMAT_LINEAR;
        pstVpssChnAttr[i].pixel_format = initParam->stViAttr.stChnAttr[0][0].pixel_format;
        pstVpssChnAttr[i].frame_rate.src_frame_rate = FPS_60;
        pstVpssChnAttr[i].frame_rate.dst_frame_rate = FPS_60;
        pstVpssChnAttr[i].depth = 0;
        pstVpssChnAttr[i].mirror_en = 0;
        pstVpssChnAttr[i].flip_en = 0;
        pstVpssChnAttr[i].border_en = 0;
        pstVpssChnAttr[i].aspect_ratio.mode = OT_ASPECT_RATIO_NONE;
    }
    chnEnable[0] = true;
}

static int32_t CameraVideoProcStart(ot_vpss_grp VpssGrp, bool chnEnable[OT_VPSS_MAX_PHYS_CHN_NUM],
    ot_vpss_grp_attr *pstVpssGrpAttr, ot_vpss_chn_attr astVpssChnAttr[OT_VPSS_MAX_PHYS_CHN_NUM])
{
    int32_t ret = ss_mpi_vpss_create_grp(VpssGrp, pstVpssGrpAttr);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vpss_create_grp(grp:%d) failed with %#x!\n", VpssGrp, ret);
        return TD_FAILURE;
    }
    HI_PRINTF("ss_mpi_vpss_create_grp(grp:%d) success\n", VpssGrp);
    for (int32_t j = 0; j < OT_VPSS_MAX_PHYS_CHN_NUM; j++) {
        if (chnEnable[j]) {
            ot_vpss_chn VpssChn = j;
            ret = ss_mpi_vpss_set_chn_attr(VpssGrp, VpssChn, &astVpssChnAttr[VpssChn]);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ss_mpi_vpss_set_chn_attr failed with %#x\n", ret);
                return TD_FAILURE;
            }
            ret = ss_mpi_vpss_enable_chn(VpssGrp, VpssChn);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ss_mpi_vpss_enable_chn failed with %#x\n", ret);
                return TD_FAILURE;
            }

#if defined(ENABLE_DISTRIBUTED_CAMERA_LOW_DELAY) && (ENABLE_DISTRIBUTED_CAMERA_LOW_DELAY != 0)
            ot_low_delay_info vpssLdyInfo;
            ret = ss_mpi_vpss_get_chn_low_delay(VpssGrp, VpssChn, &vpssLdyInfo);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ss_mpi_vpss_get_chn_low_delay failed with %#x\n", ret);
                return TD_FAILURE;
            }
            vpssLdyInfo.enable = TD_TRUE;
            vpssLdyInfo.line_cnt = 16; /* 16:VDEC_LOW_DELAY_MIN_LINE */
            ret = ss_mpi_vpss_set_chn_low_delay(VpssGrp, VpssChn, &vpssLdyInfo);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("ss_mpi_vpss_set_chn_low_delay failed with %#x\n", ret);
                return TD_FAILURE;
            }
#endif
        }
    }
    ret = ss_mpi_vpss_start_grp(VpssGrp);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vpss_start_grp failed with %#x\n", ret);
        return TD_FAILURE;
    }
    HI_PRINTF("CameraVideoProcStart success\n");
    return TD_SUCCESS;
}

int32_t CameraVpssEnableLowDelay(void)
{
#if defined(ENABLE_DISTRIBUTED_CAMERA_LOW_DELAY) && (ENABLE_DISTRIBUTED_CAMERA_LOW_DELAY != 0)
    ot_low_delay_info vpssLdyInfo;
    int32_t ret = ss_mpi_vpss_get_chn_low_delay(0, 0, &vpssLdyInfo);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vpss_get_chn_low_delay failed with %#x\n", ret);
        return TD_FAILURE;
    }

    vpssLdyInfo.enable = TD_TRUE;
    vpssLdyInfo.line_cnt = 16; /* 16:VDEC_LOW_DELAY_MIN_LINE */
    ret = ss_mpi_vpss_set_chn_low_delay(0, 0, &vpssLdyInfo);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vpss_set_chn_low_delay failed with %#x\n", ret);
        return TD_FAILURE;
    }
#endif
    return TD_SUCCESS;
}

static int32_t CameraViBindVproc(ot_vi_pipe ViPipe, ot_vi_chn ViChn, ot_vpss_grp VpssGrp)
{
    ot_mpp_chn stSrcChn;
    ot_mpp_chn stDestChn;
    stSrcChn.mod_id = OT_ID_VI;
    stSrcChn.dev_id = ViPipe;
    stSrcChn.chn_id = ViChn;
    stDestChn.mod_id = OT_ID_VPSS;
    stDestChn.dev_id = VpssGrp;
    stDestChn.chn_id = 0;
    CHECK_RET(ss_mpi_sys_bind(&stSrcChn, &stDestChn), "ss_mpi_sys_bind(VI-VPSS)");
    return TD_SUCCESS;
}

static int32_t CameraVideoProcStop(ot_vpss_grp VpssGrp, bool chnEnable[OT_VPSS_MAX_PHYS_CHN_NUM])
{
    int32_t ret;
    for (int32_t j = 0; j < OT_VPSS_MAX_PHYS_CHN_NUM; j++) {
        if (chnEnable[j]) {
            ot_vpss_chn VpssChn = j;
            ret = ss_mpi_vpss_disable_chn(VpssGrp, VpssChn);
            if (ret != TD_SUCCESS) {
                HI_PRINTF("failed with %#x!\n", ret);
                return TD_FAILURE;
            }
        }
    }
    ret = ss_mpi_vpss_stop_grp(VpssGrp);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("failed with %#x!\n", ret);
        return TD_FAILURE;
    }
    ret = ss_mpi_vpss_destroy_grp(VpssGrp);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("failed with %#x!\n", ret);
        return TD_FAILURE;
    }
    HI_PRINTF("CameraVideoProcStop success");
    return TD_SUCCESS;
}

static int32_t CameraViUnBindVproc(ot_vi_pipe viPipe, ot_vi_chn viChn, ot_vpss_grp vpssGrp)
{
    ot_mpp_chn stSrcChn;
    ot_mpp_chn stDestChn;
    stSrcChn.mod_id = OT_ID_VI;
    stSrcChn.dev_id = viPipe;
    stSrcChn.chn_id = viChn;
    stDestChn.mod_id = OT_ID_VPSS;
    stDestChn.dev_id = vpssGrp;
    stDestChn.chn_id = 0;
    CHECK_RET(ss_mpi_sys_unbind(&stSrcChn, &stDestChn), "ss_mpi_sys_unbind(VI-VPSS)");

    return TD_SUCCESS;
}

static void CameraInitParam(uint32_t mode)
{
    char path[PATH_MAX_LEN] = { 0 };

    if (snprintf_s(path, PATH_MAX_LEN, PATH_MAX_LEN - 1, "%s%s", CAMERA_INIT_DOC_PATH,
        g_modeInfo.modeNameList[mode]) < 0) {
        HI_PRINTF("snprintf_s failed\n");
        return;
    }

    HI_PRINTF("path = %s\n", path);
    if (memset_s(&g_initParam, sizeof(g_initParam), 0, sizeof(g_initParam)) != EOK) {
        HI_PRINTF("memset_s g_initParam failed\n");
    }
    uint8_t ret = GetParamCamera(path, g_initParam, SUPPORT_MAX_CAMERA_NUM);
    if (ret < SUPPORT_MAX_CAMERA_NUM) {
        g_cameraNum = ret;
    } else {
        g_cameraNum = SUPPORT_MAX_CAMERA_NUM - 1;
    }
    HI_PRINTF("lib_:%s, obj_:%s\n", g_initParam[g_cameraNum].stViAttr.sensorLib,
        g_initParam[g_cameraNum].stViAttr.sensorObj);
}

static void ReadRangCap(uint32_t cameraId, StreamCap* streamCap, uint32_t streamNum)
{
    InitParam *initParam = &g_initParam[cameraId];
    for (uint32_t i = 0; i < streamNum; i++) {
        streamCap[i].format = FORMAT_YVU420;
        streamCap[i].u.range.maxFps = initParam->stViAttr.stChnAttr[0][0].frame_rate_ctrl.dst_frame_rate;
        streamCap[i].u.range.minFps = MIN_FPS;
        streamCap[i].u.range.maxHeight = initParam->stViAttr.stChnAttr[0][0].size.height;
        streamCap[i].u.range.minHeight = MIN_HEIGHT;
        streamCap[i].u.range.maxWidth = initParam->stViAttr.stChnAttr[0][0].size.width;
        streamCap[i].u.range.minWidth = MIN_WIDTH;
    }
}

static void ReadEnumCap(uint32_t cameraId, StreamCap* streamCap, uint32_t streamNum)
{
    InitParam *initParam = &g_initParam[cameraId];
    for (uint32_t i = 0; i < streamNum; i++) {
        streamCap[i].format = FORMAT_YVU420;
        streamCap[i].u.formatEnum.frame_rate[0] = \
            initParam->stViAttr.stChnAttr[0][0].frame_rate_ctrl.dst_frame_rate;
        streamCap[i].u.formatEnum.frame_rate_num = 1;
        if (i == 0) {
            streamCap[i].u.formatEnum.width = RESOLUTION_2592X1944_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_2592X1944_H;
        } else if (i == 1) {
            streamCap[i].u.formatEnum.width = RESOLUTION_1080P_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_1080P_H;
        } else if (i == 0x2) {
            streamCap[i].u.formatEnum.width = RESOLUTION_720P_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_720P_H;
        } else if (i == 0x3) {
            streamCap[i].u.formatEnum.width = RESOLUTION_D1_NTSC_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_D1_NTSC_H;
        } else if (i == 0x4) {
            streamCap[i].u.formatEnum.width = RESOLUTION_360P_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_360P_H;
        } else {
            streamCap[i].u.formatEnum.width = RESOLUTION_1080P_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_1080P_H;
        }
    }
}

static void DisableVpssExtChn(ot_vpss_grp vpssGrp)
{
    for (ot_vpss_chn chn = OT_VPSS_MAX_PHYS_CHN_NUM; chn <= CALLBACK_EXT_CHN; chn++) {
        ss_mpi_vpss_disable_chn(vpssGrp, chn);
    }
}

static int32_t ParamCheck(const StreamAttr *stream, const ControlInfo *config)
{
    if (stream->invertMode >= HAL_ROTATION_BUTT) {
        HI_PRINTF("rotation param invalid\n");
        return TD_FAILURE;
    }
    if (stream->fps > MAX_FPS || stream->fps == 0) {
        HI_PRINTF("fps param invalid\n");
        return TD_FAILURE;
    }
    if (stream->width <= 0 || stream->width > MAX_FRAME_WIDTH ||
        stream->height <= 0 || stream->height > MAX_FRAME_WIDTH) {
        HI_PRINTF("width height param invalid\n");
        return TD_FAILURE;
    }
    if (stream->crop.x == 0 && stream->crop.y == 0 &&
        stream->crop.w == 0 && stream->crop.h == 0) {
        return TD_SUCCESS;
    }
    if (stream->crop.x + stream->crop.w > config->maxW ||
        stream->crop.y + stream->crop.h > config->maxH) {
        HI_PRINTF("crop param invalid\n");
        return TD_FAILURE;
    }
    if (stream->invertMode == HAL_ROTATION_90) {
        if (stream->crop.w < stream->height || stream->crop.h < stream->width) {
            HI_PRINTF("crop param invalid\n");
            return TD_FAILURE;
        }
    } else if (stream->crop.h < stream->height || stream->crop.w < stream->width) {
        HI_PRINTF("crop param invalid\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

#ifdef USE_EXT_CHN
static void GetVpss(const StreamAttr *stream, const CameraInfo *cameraInfo,
    ot_vpss_chn *vpssChn, ot_vpss_chn *vpssPhyChn)
{
    *vpssChn = -1;
    static ot_vpss_chn videoChn = VIDEO_EXT_MIN_CHN;
    if (stream->type == STREAM_VIDEO) {
        if (videoChn > VIDEO_EXT_MAX_CHN) {
            videoChn = VIDEO_EXT_MIN_CHN;
        }
        *vpssChn = videoChn++;
        *vpssPhyChn = VIDEO_VPSS_CHN;
    } else if (stream->type == STREAM_PREVIEW) {
        *vpssChn = PREEV_VPSS_CHN;
        *vpssPhyChn = PREEV_VPSS_CHN;
    } else if (stream->type == STREAM_CAPTURE) {
        *vpssChn = OT_VPSS_MAX_PHYS_CHN_NUM;
        *vpssPhyChn = JPEG_VPSS_CHN;
    } else {
        *vpssChn = CALLBACK_EXT_CHN;
        *vpssPhyChn = CALLBACK_VPSS_CHN;
    }
}
#else
static void GetVpss(const StreamAttr *stream, const CameraInfo *cameraInfo,
    ot_vpss_chn *vpssChn, ot_vpss_chn *vpssPhyChn)
{
    (void)stream;
    *vpssChn = -1;
    *vpssPhyChn = -1;
    for (td_u8 i = 0; i < CAEMRA_MAX_STREAM_NUM; i++) {
        if (cameraInfo->internalStreamInfo[i].isUsed == true ||
            cameraInfo->internalStreamInfo[i].vpssChn >= OT_VPSS_MAX_PHYS_CHN_NUM) {
            continue;
        }
        *vpssChn = i;
        *vpssPhyChn = i;
        HI_PRINTF("GetVpss chnn :%u\n", i);
        return;
    }
}
#endif

static uint32_t GetDeviceId(uint32_t chn)
{
    return (((OT_ID_VPSS << 0x10) & 0xff0000) + (chn & 0xff)); // 16
}

static bool IsValidChnn(ot_vpss_chn vpssChn)
{
    if (vpssChn >= 0 && vpssChn < OT_VPSS_MAX_CHN_NUM) {
        return true;
    }
    return false;
}

static void ConvertCrop(ot_vpss_chn vpssChn, const StreamAttr *stream, ControlInfo *config)
{
    if (stream->crop.w == 0 || stream->crop.h == 0) {
        config->cropInfo.enable = TD_FALSE;
        config->cropInfo.crop_mode = OT_COORD_ABS;
        config->cropInfo.crop_rect.x = 0;
        config->cropInfo.crop_rect.y = 0;
        config->cropInfo.crop_rect.width = config->maxW;
        config->cropInfo.crop_rect.height = config->maxH;
        return;
    }
    config->cropInfo.enable = TD_TRUE;
    config->cropInfo.crop_mode = OT_COORD_ABS;
    if (vpssChn < OT_VPSS_MAX_PHYS_CHN_NUM) {
        if (config->rotation.rotation_fixed == OT_ROTATION_90) {
            config->cropInfo.crop_rect.width = ALIGN_DOWN(stream->height, HI_ALIGN_DOWN_NUM);
            config->cropInfo.crop_rect.height = ALIGN_DOWN(stream->width, HI_ALIGN_DOWN_NUM);
            td_u32 x = stream->crop.x * stream->height / stream->crop.w;
            td_u32 y = stream->crop.y * stream->width / stream->crop.h;
            config->cropInfo.crop_rect.x = ALIGN_DOWN(x, HI_ALIGN_DOWN_NUM);
            config->cropInfo.crop_rect.y = ALIGN_DOWN(y, HI_ALIGN_DOWN_NUM);
        } else {
            config->cropInfo.crop_rect.width = ALIGN_DOWN(stream->width, HI_ALIGN_DOWN_NUM);
            config->cropInfo.crop_rect.height = ALIGN_DOWN(stream->height, HI_ALIGN_DOWN_NUM);
            td_u32 x = stream->crop.x * stream->width / stream->crop.w;
            td_u32 y = stream->crop.y * stream->height / stream->crop.h;
            config->cropInfo.crop_rect.x = ALIGN_DOWN(x, HI_ALIGN_DOWN_NUM);
            config->cropInfo.crop_rect.y = ALIGN_DOWN(y, HI_ALIGN_DOWN_NUM);
        }
    } else {
        if (config->rotation.rotation_fixed == OT_ROTATION_90) {
            config->cropInfo.crop_rect.x = ALIGN_DOWN(stream->crop.y, HI_ALIGN_DOWN_NUM);
            config->cropInfo.crop_rect.y = ALIGN_DOWN(stream->crop.x, HI_ALIGN_DOWN_NUM);
            config->cropInfo.crop_rect.width = ALIGN_DOWN(stream->crop.h, HI_ALIGN_DOWN_NUM);
            config->cropInfo.crop_rect.height = ALIGN_DOWN(stream->crop.w, HI_ALIGN_DOWN_NUM);
        } else {
            config->cropInfo.crop_rect.x = ALIGN_DOWN(stream->crop.x, HI_ALIGN_DOWN_NUM);
            config->cropInfo.crop_rect.y = ALIGN_DOWN(stream->crop.y, HI_ALIGN_DOWN_NUM);
            config->cropInfo.crop_rect.width = ALIGN_DOWN(stream->crop.w, HI_ALIGN_DOWN_NUM);
            config->cropInfo.crop_rect.height = ALIGN_DOWN(stream->crop.h, HI_ALIGN_DOWN_NUM);
        }
    }
}

static void ConvetToControlInfo(ot_vpss_chn vpssChn, const StreamAttr *stream, ControlInfo *config)
{
    if (IsValidChnn(vpssChn) == false) {
        return;
    }
    config->fps = stream->fps;
    config->width = stream->width;
    config->height = stream->height;
    config->rotation.enable = TD_TRUE;
    config->rotation.rotation_type = OT_ROTATION_ANG_FIXED;
    if (stream->invertMode == HAL_HORIZONTAL_MIRROR) {
        config->isFlip = TD_TRUE;
    } else if (stream->invertMode == HAL_VETICALLY_MIRROR) {
        config->isMirror = TD_TRUE;
    } else if (stream->invertMode == HAL_ROTATION_90) {
        config->rotation.rotation_fixed = OT_ROTATION_90;
    } else if (stream->invertMode == HAL_ROTATION_180) {
        config->rotation.rotation_fixed = OT_ROTATION_180;
    } else {
        config->isFlip = TD_FALSE;
        config->isMirror = TD_FALSE;
        config->rotation.rotation_fixed = OT_ROTATION_0;
    }

    ConvertCrop(vpssChn, stream, config);
    HAL_LOGI("flip = %d rota = %d crop w = %u enable = %d\n", config->isFlip, config->rotation.rotation_fixed,
        config->cropInfo.crop_rect.width, config->cropInfo.enable);
}

static void GetVpssExtChnConfig(ot_vpss_ext_chn_attr *extChnAttr, const ControlInfo *config,
    ot_vpss_chn_attr *vpssChnAttr, ot_vpss_chn vpssPhyChn)
{
    extChnAttr->bind_chn  = vpssPhyChn;
    extChnAttr->pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    extChnAttr->frame_rate.src_frame_rate = vpssChnAttr->frame_rate.dst_frame_rate;
    extChnAttr->frame_rate.dst_frame_rate = config->fps;
    extChnAttr->width = config->width;
    extChnAttr->height = config->height;
}

static int32_t GetFreeStreamIndex(const CameraInfo *cameraInfo, uint32_t *streamIndex)
{
    for (int32_t i = 0; i < CAEMRA_MAX_STREAM_NUM; i++) {
        if (!cameraInfo->internalStreamInfo[i].isUsed) {
            *streamIndex = i;
            return TD_SUCCESS;
        }
    }
    HAL_LOGE("get free stream index failed\n");
    return TD_FAILURE;
}

static void AddStream(CameraInfo *cameraInfo, uint32_t streamIndex, ot_vpss_chn vpssChn, const StreamAttr *stream)
{
    LOG_CHK_RETURN(streamIndex >= CAEMRA_MAX_STREAM_NUM);
    cameraInfo->streamAttr[streamIndex] = *stream;
    cameraInfo->internalStreamInfo[streamIndex].isUsed = true;
    cameraInfo->internalStreamInfo[streamIndex].vpssChn = vpssChn;
    HAL_LOGI("stream %u vpss = %d\n", streamIndex, vpssChn);
}

static void DeleteStream(CameraInfo *cameraInfo, uint32_t streamIndex)
{
    ot_vpss_grp vpssGrp = 0;
    ot_vpss_chn vpssChn = cameraInfo->internalStreamInfo[streamIndex].vpssChn;
    HAL_LOG_DOFUNC(memset_s(&cameraInfo->streamAttr[streamIndex], sizeof(StreamAttr), 0, sizeof(StreamAttr)));
    HAL_LOG_DOFUNC(memset_s(&cameraInfo->internalStreamInfo[streamIndex], sizeof(InternalStreamInfo),
        0, sizeof(InternalStreamInfo)));
    for (uint8_t i = 0; i < CAEMRA_MAX_STREAM_NUM; i++) {
        if (cameraInfo->internalStreamInfo[i].isUsed && cameraInfo->internalStreamInfo[i].vpssChn == vpssChn) {
            return;
        }
    }
    if (vpssChn >= OT_VPSS_MAX_PHYS_CHN_NUM) {
        ss_mpi_vpss_disable_chn(vpssGrp, vpssChn);
    }
}

static int32_t EnablePreview(ot_vpss_chn vpssChn, uint32_t cameraId, PosInfo *pos, const StreamAttr *streamAttr)
{
    IRect displayRect;
    displayRect.x = pos->x;
    displayRect.y = pos->y;
    displayRect.w = streamAttr->width;
    displayRect.h = streamAttr->height;
    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    int32_t vpssGrp = cameraInfo->vpssGrp;
    LayerInfo layerInfo = {0};
    layerInfo.width = streamAttr->width;
    layerInfo.height = streamAttr->height;
    layerInfo.type = LAYER_TYPE_OVERLAY;
    layerInfo.bpp = 8;  // 8: Number of bits occupied by each pixel
    layerInfo.pixFormat = PIXEL_FMT_YCRCB_420_SP;
    layerInfo.fps = streamAttr->fps;
    LOG_CHK_RETURN_ERR(g_layerInterface == NULL, TD_FAILURE);
    HAL_LOG_DOFUNC_RETURN(g_layerInterface->CreateLayer(DISPLAY_DEVID, &layerInfo, &cameraInfo->layerId));
    HAL_LOG_DOFUNC_RETURN(g_layerInterface->SetLayerSize(DISPLAY_DEVID, cameraInfo->layerId, &displayRect));
    HAL_LOG_DOFUNC_RETURN(g_layerInterface->InvokeLayerCmd(DISPLAY_DEVID, cameraInfo->layerId,
        OVERLAYER_CMD_VO_BIND_VPSS, vpssChn, vpssGrp));
    return TD_SUCCESS;
}

static void DisablePreview(ot_vpss_chn vpssChn, uint32_t cameraId)
{
    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    int32_t vpssGrp = cameraInfo->vpssGrp;
    LOG_CHK_RETURN(g_layerInterface == NULL);
    g_layerInterface->InvokeLayerCmd(DISPLAY_DEVID, cameraInfo->layerId,
        OVERLAYER_CMD_VO_UNBIND_VPSS, vpssChn, vpssGrp);
    g_layerInterface->CloseLayer(DISPLAY_DEVID, cameraInfo->layerId);
    cameraInfo->layerId = -1;
}

static int32_t CovertToBuffer(const ot_video_frame_info *videoFrame, HalBuffer *halBuffer)
{
    if (halBuffer->format == FORMAT_PRIVATE) {
        HalBuffer getBuf;
        getBuf.stride0 = (int32_t)videoFrame->video_frame.stride[0];
        getBuf.stride1 = (int32_t)videoFrame->video_frame.stride[1];
        getBuf.height = (int32_t)videoFrame->video_frame.height;
        getBuf.width = (int32_t)videoFrame->video_frame.width;
        getBuf.timeStamp = (int64_t)videoFrame->video_frame.pts;
        int32_t size = getBuf.height * getBuf.width * 0x3 / 0x2; // YUV size
        if (halBuffer->size <= size) {
            HAL_LOGE("%d <= %d\n", halBuffer->size, size);
            return TD_FAILURE;
        }
        getBuf.size =  size;
        getBuf.format = FORMAT_YVU420;
        getBuf.phyAddr = videoFrame->video_frame.phys_addr[0];
        HAL_LOG_DOFUNC_RETURN(memcpy_s(halBuffer->virAddr, halBuffer->size, &getBuf, sizeof(HalBuffer)));
        return TD_SUCCESS;
    } else if (videoFrame->video_frame.pixel_format == OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
        halBuffer->format = FORMAT_YVU420;
    } else if (videoFrame->video_frame.pixel_format == OT_PIXEL_FORMAT_RGB_BAYER_12BPP) {
        halBuffer->format = FORMAT_RGB_BAYER_12BPP;
    } else {
    }
    halBuffer->stride0 = (int32_t)videoFrame->video_frame.stride[0];
    halBuffer->stride1 = (int32_t)videoFrame->video_frame.stride[1];
    halBuffer->height = (int32_t)videoFrame->video_frame.height;
    halBuffer->width = (int32_t)videoFrame->video_frame.width;
    halBuffer->timeStamp = (int64_t)videoFrame->video_frame.pts;
    halBuffer->size = halBuffer->height * halBuffer->width * 0x3 / 0x2; // YUV size
    halBuffer->virAddr = (void *) ss_mpi_sys_mmap(videoFrame->video_frame.phys_addr[0], halBuffer->size);
    return TD_SUCCESS;
}


static void CameraGetViConfig(CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt)
{
    for (uint32_t i = 0; i < cameraCnt; i++) {
        pstViConfig[i].stSnsInfo.s32SnsId = g_initParam[i].stViAttr.stMipiAttr.devno;
        HI_PRINTF("sns id = %u\n", g_initParam[i].stViAttr.stMipiAttr.devno);
        pstViConfig[i].stSnsInfo.s32BusId = g_initParam[i].stViAttr.busId;
        pstViConfig[i].stSnsInfo.MipiDev = g_initParam[i].stViAttr.stMipiAttr.devno;
        pstViConfig[i].stDevInfo.ViDev = g_initParam[i].stViAttr.stMipiAttr.devno;
        HAL_LOG_DOFUNC(memset_s(&pstViConfig[i].stSnapInfo, sizeof(HISI_SNAP_INFO_S), 0, sizeof(HISI_SNAP_INFO_S)));
        pstViConfig[i].stPipeInfo.bMultiPipe = TD_FALSE;
        pstViConfig[i].stPipeInfo.bVcNumCfged = TD_FALSE;
        pstViConfig[i].stPipeInfo.enMastPipeMode = g_initParam[i].stViAttr.pipeMode[0];
        pstViConfig[i].stPipeInfo.aPipe[0] = g_initParam[i].stViAttr.stMipiAttr.devno;
        pstViConfig[i].stPipeInfo.aPipe[0] = 0;
        for (int32_t j = 0x1; j < OT_VI_MAX_PIPE_NUM; j++) {
            pstViConfig[i].stPipeInfo.aPipe[j] = -1;
        }
        if (cameraCnt == 1 && g_cameraNum == SUPPORT_MAX_CAMERA_NUM) { // 1 sensor(2 pipe) analog 2 sensors
            pstViConfig[0].stPipeInfo.aPipe[1] = pstViConfig[0].stPipeInfo.aPipe[0] + 1;
        }

        pstViConfig[i].stPipeInfo.frameInterruptType = g_initParam[i].stViAttr.frameInterruptType[0];
        pstViConfig[i].stPipeInfo.earlyLine = g_initParam[i].stViAttr.earlyLine[0];
        pstViConfig[i].stChnInfo.ViChn = 0;
        pstViConfig[i].stChnInfo.enPixFormat = g_initParam[i].stViAttr.stChnAttr[0][0].pixel_format;
        pstViConfig[i].stChnInfo.dynamicRange = g_initParam[i].stViAttr.stChnAttr[0][0].dynamic_range;
        pstViConfig[i].stChnInfo.videoFormat = g_initParam[i].stViAttr.stChnAttr[0][0].video_format;
        pstViConfig[i].stChnInfo.compressMode = g_initParam[i].stViAttr.stChnAttr[0][0].compress_mode;
        pstViConfig[i].mipiAttr = &g_initParam[i].stViAttr.stMipiAttr;
        pstViConfig[i].devAttr = &g_initParam[i].stViAttr.stDevAttr;
        pstViConfig[i].pipeAttr = &(g_initParam[i].stViAttr.stPipeAttr[0]);
        pstViConfig[i].chnAttr = &(g_initParam[i].stViAttr.stChnAttr[0][0]);
        pstViConfig[i].ispPubAttr = &g_initParam[i].stViAttr.stPubAttr;
        pstViConfig[i].sensorLib = g_initParam[i].stViAttr.sensorLib;
        pstViConfig[i].sensorObj = g_initParam[i].stViAttr.sensorObj;
        pstViConfig[i].len = SENSOR_DESC_MAX_LEN;
    }
}

static void SetChnDepth(InternalStreamInfo *internalInfo, ImageFormat format)
{
    if (internalInfo->isSetDepth == true) {
        return;
    }
    if (format == FORMAT_YVU420 || format == FORMAT_PRIVATE) {
        if (internalInfo->vpssChn < OT_VPSS_MAX_PHYS_CHN_NUM) {
            ot_vpss_chn_attr chnAttr = { 0 };
            ss_mpi_vpss_get_chn_attr(0, internalInfo->vpssChn, &chnAttr);
            chnAttr.depth = 1;
            HAL_LOG_DOFUNC(ss_mpi_vpss_set_chn_attr(0, internalInfo->vpssChn, &chnAttr));
        } else {
            ot_vpss_ext_chn_attr extChnAttr = { 0 };
            ss_mpi_vpss_get_ext_chn_attr(0, internalInfo->vpssChn, &extChnAttr);
            extChnAttr.depth = 1;
            HAL_LOG_DOFUNC(ss_mpi_vpss_set_ext_chn_attr(0, internalInfo->vpssChn, &extChnAttr));
        }
    } else if (format == FORMAT_RGB_BAYER_12BPP) {
        HAL_LOGI("set vi dump attr\n");
        ot_vi_frame_dump_attr attr = {true, 1};
        HAL_LOG_DOFUNC(ss_mpi_vi_set_pipe_frame_dump_attr(0, &attr));
    } else {
        HAL_LOGI("not support format\n");
        return;
    }
    internalInfo->isSetDepth = true;
}

static void UpdateVpssAttr(ot_vpss_chn vpssChn, const StreamAttr *stream, const ControlInfo *config,
                           ot_vpss_chn_attr *vpssChnAttr)
{
    vpssChnAttr->flip_en = config->isFlip;
    vpssChnAttr->mirror_en = config->isMirror;
    if (vpssChn < OT_VPSS_MAX_PHYS_CHN_NUM) {
        if (config->cropInfo.crop_rect.width <= stream->crop.w &&
            config->cropInfo.crop_rect.height <= stream->crop.h &&
            stream->crop.w != 0 && stream->crop.h != 0) {
            vpssChnAttr->width = config->maxW * config->cropInfo.crop_rect.width / stream->crop.w;
            vpssChnAttr->height = config->maxH * config->cropInfo.crop_rect.height / stream->crop.h;
            vpssChnAttr->width = ALIGN_UP(vpssChnAttr->width, HI_ALIGN_UP_NUM);
            vpssChnAttr->height = ALIGN_UP(vpssChnAttr->height, HI_ALIGN_UP_NUM);
        } else if (config->rotation.rotation_fixed == OT_ROTATION_90) {
            vpssChnAttr->width = ALIGN_DOWN(stream->height, HI_ALIGN_DOWN_NUM);
            vpssChnAttr->height = ALIGN_DOWN(stream->width, HI_ALIGN_DOWN_NUM);
        } else {
            vpssChnAttr->width = ALIGN_DOWN(stream->width, HI_ALIGN_DOWN_NUM);
            vpssChnAttr->height = ALIGN_DOWN(stream->height, HI_ALIGN_DOWN_NUM);
        }
        vpssChnAttr->frame_rate.src_frame_rate = FPS_60;
        vpssChnAttr->frame_rate.dst_frame_rate = FPS_60;
        vpssChnAttr->pixel_format = OT_PIXEL_FORMAT_YUV_SEMIPLANAR_420;
        HI_PRINTF("chnn attr: chnn = %d w = %u h = %u\n", vpssChn, vpssChnAttr->width, vpssChnAttr->height);
    }
    vpssChnAttr->chn_mode = OT_VPSS_CHN_MODE_USER;
}

static td_s32 CreateStream(ot_vpss_chn vpssChn, ot_vpss_chn vpssPhyChn,
    const StreamAttr *stream, const ControlInfo *config)
{
    HI_PRINTF("CreateImage w h %d %d chan %d vpGroup %d\n", stream->width, stream->height, vpssChn, config->vpssGrp);
    const ot_vpss_grp vpssGrp = config->vpssGrp;
    ot_vpss_chn_attr vpssChnAttr = { 0 };
    if (IsValidChnn(vpssChn) == false) {
        return TD_FAILURE;
    }
    td_u32 ret = (td_u32)ss_mpi_vpss_get_chn_attr(vpssGrp, vpssPhyChn, &vpssChnAttr);
    if (ret != TD_SUCCESS) {
        HI_PRINTF(" get attr failed %#x!, vpssGrp=%d, vpssPhyChn=%d\n", ret, vpssGrp, vpssPhyChn);
    }

    UpdateVpssAttr(vpssChn, stream, config, &vpssChnAttr);
    HI_PRINTF("vpssPhyChn = %d rotation.enable = %d\n", vpssPhyChn, config->rotation.enable);
    ret |= (td_u32)ss_mpi_vpss_set_chn_attr(vpssGrp, vpssPhyChn, &vpssChnAttr);
    if (config->rotation.enable == TD_TRUE) {
        ret |= (td_u32)ss_mpi_vpss_set_chn_rotation(vpssGrp, vpssPhyChn, &(config->rotation));
    }
    if (vpssChn < OT_VPSS_MAX_PHYS_CHN_NUM && config->cropInfo.enable == TD_TRUE) {
        ret |= (td_u32)ss_mpi_vpss_set_chn_crop(vpssGrp, vpssChn, &(config->cropInfo));
    }
    ret |= (td_u32)ss_mpi_vpss_enable_chn(vpssGrp, vpssPhyChn);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("ss_mpi_vpss_set_chn_attr failed with %#x\n", ret);
        return TD_FAILURE;
    }
    if (vpssChn >= OT_VPSS_MAX_PHYS_CHN_NUM) {
        HI_PRINTF("ext vpssChn = %d\n", vpssChn);
        ot_vpss_ext_chn_attr extChnAttr = { 0 };
        GetVpssExtChnConfig(&extChnAttr, config, &vpssChnAttr, vpssPhyChn);
        ret = (td_u32)ss_mpi_vpss_set_ext_chn_attr(vpssGrp, vpssChn, &extChnAttr);
        ret |= (td_u32)ss_mpi_vpss_set_chn_crop(vpssGrp, vpssChn, &(config->cropInfo));
        ret |= (td_u32)ss_mpi_vpss_enable_chn(vpssGrp, vpssChn);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("ss_mpi_vpss_set_ext_chn_attr failed with %#x\n", ret);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static int32_t CheckSameChn(const CameraInfo *cameraInfo, const StreamAttr *stream, ot_vpss_chn *sameChn)
{
    *sameChn = -1;
    for (td_u8 i = 0; i < CAEMRA_MAX_STREAM_NUM; i++) {
        if (cameraInfo->internalStreamInfo[i].isUsed) {
            if (cameraInfo->streamAttr[i].width == stream->width &&
                cameraInfo->streamAttr[i].height == stream->height &&
                cameraInfo->streamAttr[i].fps == stream->fps &&
                cameraInfo->streamAttr[i].crop.w == stream->crop.w &&
                cameraInfo->streamAttr[i].crop.h == stream->crop.h &&
                cameraInfo->streamAttr[i].crop.x == stream->crop.x &&
                cameraInfo->streamAttr[i].crop.y == stream->crop.y &&
                cameraInfo->streamAttr[i].invertMode == stream->invertMode) {
                    *sameChn = cameraInfo->internalStreamInfo[i].vpssChn;
                    return TD_SUCCESS;
            }
        }
    }
    return TD_FAILURE;
}

int32_t HalCameraInit(void)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(g_isInit == true, TD_SUCCESS);
#ifdef KERNEL_USER_MODE
    sdk_init();
#endif
    SDK_init();
    InitModeInfo();
    CameraInitParam(GetCurrentMode());
    HAL_LOG_DOFUNC_RETURN(CameraCreateVbPool());
    HAL_LOG_DOFUNC(LayerInitialize(&g_layerInterface));
    if (g_layerInterface != NULL && g_layerInterface->InitDisplay != NULL) {
        HAL_LOG_DOFUNC(g_layerInterface->InitDisplay(1));
    }
    g_isInit = true;
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraDeinit(void)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(g_isInit == false, TD_SUCCESS);
    if (g_layerInterface != NULL && g_layerInterface->DeinitDisplay != NULL) {
        HAL_LOG_DOFUNC(g_layerInterface->DeinitDisplay(0));
    }
    HAL_LOG_DOFUNC(LayerUninitialize(g_layerInterface));
#ifdef KERNEL_USER_MODE
    sdk_exit();
#endif
    SDK_exit();
    g_isInit = false;
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraGetModeNum(uint8_t *num)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(num == NULL, TD_FAILURE);
    *num = g_modeInfo.modeNum;
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraSetMode(uint8_t index)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(g_isInit == false || index >= g_modeInfo.modeNum, TD_FAILURE);
    for (int8_t i = 0; i < SUPPORT_MAX_CAMERA_NUM; i++) {
        if (g_cameraInfo[i].isOpen) {
            HAL_LOGE("camera is opened: %d\n", i);
            return TD_FAILURE;
        }
    }
    CameraDestoryVbPool();
    g_isInit = false;
    CameraInitParam(index);
    g_modeInfo.index = index;
    HAL_LOG_DOFUNC_RETURN(CameraCreateVbPool());
    LOG_CHK_RETURN_ERR(g_layerInterface == NULL, TD_FAILURE);
    g_isInit = true;
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraSetDeviceDetectCb(const CameraDetectCb cb)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(cb == NULL || g_cameraDetect != NULL, TD_FAILURE);
    g_cameraDetect = cb;
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraGetDeviceNum(uint8_t *num)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(num == NULL, TD_FAILURE);
    *num = g_cameraNum;
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraGetDeviceList(char cameraList[][CAMERA_NAME_MAX_LEN], uint8_t listNum)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(cameraList);

    for (uint8_t i = 0; i < g_cameraNum && i < listNum; i++) {
        if (snprintf_s(cameraList[i], CAMERA_NAME_MAX_LEN, CAMERA_NAME_MAX_LEN - 1, "%u", i) < 0) {
            HAL_LOGE("snprintf_s failed\n");
            return TD_FAILURE;
        }
    }
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraGetStreamCapNum(const char *camera, uint32_t *num)
{
    HI_CHECK_NULL_PTR(num);
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum, TD_FAILURE);
    *num = CAEMRA_MAX_STREAM_NUM;
    return TD_SUCCESS;
}

int32_t HalCameraGetStreamCap(const char *camera, StreamCap *streamCap, uint32_t streamNum)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(streamCap == NULL || camera == NULL, TD_FAILURE);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum, TD_FAILURE);
    uint32_t num;
    HAL_LOG_DOFUNC_RETURN(HalCameraGetStreamCapNum(camera, &num));
    LOG_CHK_RETURN_ERR(streamNum < num, TD_FAILURE);
    if (streamCap->type == CAP_DESC_RANGE) {
        ReadRangCap(cameraId, streamCap, num);
    } else if (streamCap->type == CAP_DESC_ENUM) {
        ReadEnumCap(cameraId, streamCap, num);
    }
    HAL_EXIT();
    return TD_SUCCESS;
}

static uint32_t GetCameraCnt(void)
{
    if (g_cameraNum == SUPPORT_MAX_CAMERA_NUM &&
        g_initParam[0].stViAttr.stMipiAttr.devno == g_initParam[1].stViAttr.stMipiAttr.devno) {
        return 1;
    } else {
        return g_cameraNum;
    }
}

static bool  CheckViStart(uint32_t cameraId)
{
    for (uint32_t i = 0; i < g_cameraNum; i++) {
        if (i == cameraId) {
            continue;
        }
        if (g_cameraInfo[i].isOpen) {
            return true;
        }
    }

    return false;
}

static int32_t CameraStartVpss(const CAMERA_VI_CONFIG_S *viConfig, uint32_t cameraId, ot_vpss_grp vpssGrp)
{
    CameraInfo *cameraInfo = &g_cameraInfo[cameraId];
    InitParam *initParam = &g_initParam[cameraId];
    uint32_t cameraCnt = GetCameraCnt();
    bool chnEnable[OT_VPSS_MAX_PHYS_CHN_NUM] = { 0 };
    ot_vpss_grp_attr vpssGrpAttr;
    /* config vpss group */
    CameraGetVpssGrpConfig(&vpssGrpAttr, initParam);
        /* config vpss chn */
    ot_vpss_chn_attr vpssChnAttr[OT_VPSS_MAX_PHYS_CHN_NUM];
    CameraGetVpssChnConfig(vpssChnAttr, chnEnable, initParam);
    /* start vpss */
    int32_t ret = CameraVideoProcStart(vpssGrp, chnEnable, &vpssGrpAttr, vpssChnAttr);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("start vpss group failed. ret: 0x%x !\n", ret);
        return ret;
    }
    /* vi bind vpss */
    ot_vi_chn viChn = 0;
    ot_vi_pipe viPipe;
    if (g_cameraNum == SUPPORT_MAX_CAMERA_NUM && cameraCnt == 1) {
        viPipe = viConfig[0].stPipeInfo.aPipe[0];
    } else {
        viPipe = viConfig[cameraId].stPipeInfo.aPipe[0];
    }
    ret = CameraViBindVproc(viPipe, viChn, vpssGrp);
    if (ret != TD_SUCCESS) {
        HI_PRINTF("vi bind vpss failed. ret: 0x%x !\n", ret);
        CameraVideoProcStop(vpssGrp, chnEnable);
        return ret;
    }
    cameraInfo->vpssGrp = vpssGrp;
    return ret;
}

static void CameraStopVpss(const CAMERA_VI_CONFIG_S *viConfig, uint32_t cameraId)
{
    CameraInfo *cameraInfo = &g_cameraInfo[cameraId];
    InitParam *initParam = &g_initParam[cameraId];
    uint32_t cameraCnt = GetCameraCnt();
    ot_vi_pipe viPipe;
    if (g_cameraNum == SUPPORT_MAX_CAMERA_NUM && cameraCnt == 1) {
        viPipe = viConfig[0].stPipeInfo.aPipe[0];
    } else {
        viPipe = viConfig[cameraId].stPipeInfo.aPipe[0];
    }
    ot_vi_chn viChn = 0;
    ot_vpss_grp vpssGrp = cameraInfo->vpssGrp;
    CameraViUnBindVproc(viPipe, viChn, vpssGrp);
    bool chnEnable[OT_VPSS_MAX_PHYS_CHN_NUM] = { 0 };
    ot_vpss_chn_attr astVpssChnAttr[OT_VPSS_MAX_PHYS_CHN_NUM];
    CameraGetVpssChnConfig(astVpssChnAttr, chnEnable, initParam);
    DisableVpssExtChn(vpssGrp);
    CameraVideoProcStop(vpssGrp, chnEnable);
    cameraInfo->vpssGrp = -1;
}

int32_t HalCameraDeviceOpen(const char *camera)
{
    HAL_ENTER();
    HI_PRINTF(" open camera = %s\n", camera);
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum, TD_FAILURE);
     /* get vi config */
    int32_t ret;
    CAMERA_VI_CONFIG_S viConfig[SUPPORT_MAX_CAMERA_NUM] = { 0 };
    CameraInfo *cameraInfo = &g_cameraInfo[cameraId];
    InitParam *initParam = &g_initParam[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->isOpen == true, TD_SUCCESS);
    cameraInfo->type = initParam->stViAttr.sensorType;

    uint32_t cameraCnt = GetCameraCnt();
    CameraGetViConfig(viConfig, cameraCnt);
    HI_PRINTF("lib0:%s, obj0:%s, len0:%u\n", initParam->stViAttr.sensorLib,
        initParam->stViAttr.sensorObj, viConfig[0].len);
    ot_vpss_grp vpssGrp = 0;
    if (CheckViStart(cameraId)) {
        HI_PRINTF("Vi is Started. Next, bind the vpssGrp.\n");
        vpssGrp = 1;
    } else {
        /* start vi */
        ret = CameraStartVi(viConfig, cameraCnt);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("start vi failed.ret:0x%x !\n", ret);
            goto EXIT;
        }
    }

    if (cameraInfo->type != CAM_TYPE_TRUE_DEAPTH) {
        ret = CameraStartVpss(viConfig, cameraId, vpssGrp);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("CameraStartVpss failed. ret:0x%x !\n", ret);
            goto EXIT1;
        }
    }

    cameraInfo->isOpen = true;
    HAL_EXIT();
    return TD_SUCCESS;

EXIT1:
    if (!CheckViStart(cameraId)) {
        CameraStopVi(viConfig, cameraCnt);
    }
EXIT:
    return ret;
}

int32_t HalCameraDeviceClose(const char *camera)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    HI_PRINTF("cameraId=%u\n", cameraId);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum, TD_FAILURE);
    CameraInfo *cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->isOpen == false, TD_SUCCESS);
    CAMERA_VI_CONFIG_S viConfig[SUPPORT_MAX_CAMERA_NUM] = {};
    uint32_t cameraCnt = GetCameraCnt();
    CameraGetViConfig(viConfig, cameraCnt);

    if (cameraInfo->type != CAM_TYPE_TRUE_DEAPTH) {
        CameraStopVpss(viConfig, cameraId);
    }
    if (!CheckViStart(cameraId)) {
        CameraStopVi(&viConfig, cameraCnt);
    }
    cameraInfo->isOpen = false;
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraStreamCreate(const char *camera, const StreamAttr *stream, uint32_t *streamId)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(stream == NULL || streamId == NULL || cameraId >= g_cameraNum, TD_FAILURE);
    ControlInfo config = { 0 };
    config.maxW = g_initParam[cameraId].stViAttr.stChnAttr[0][0].size.width;
    config.maxH = g_initParam[cameraId].stViAttr.stChnAttr[0][0].size.height;
    if (config.maxW > MAX_FRAME_WIDTH || config.maxH > MAX_FRAME_WIDTH) {
        HI_PRINTF("maxW maxH param invalid\n");
        return TD_FAILURE;
    }
    HAL_LOG_DOFUNC_RETURN(ParamCheck(stream, &config));

    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    config.vpssGrp = cameraInfo->vpssGrp;
    uint32_t streamIndex = 0;
    ot_vpss_chn vpssChn = -1;
    ot_vpss_chn vpssPhyChn = -1;
    ot_vpss_chn sameChn = -1;
    HAL_LOG_DOFUNC_RETURN(GetFreeStreamIndex(cameraInfo, &streamIndex));

    if (CheckSameChn(cameraInfo, stream, &sameChn) == TD_SUCCESS) {
        vpssChn = sameChn;
    } else if (cameraInfo->type != CAM_TYPE_TRUE_DEAPTH) {
        GetVpss(stream, cameraInfo, &vpssChn, &vpssPhyChn);
        if (vpssChn == -1 && vpssPhyChn == -1) {
            HI_PRINTF(" GetVpss failed, vpssChn=%d\n", vpssChn);
            return TD_FAILURE;
        }
        ConvetToControlInfo(vpssChn, stream, &config);
        td_s32 ret = CreateStream(vpssChn, vpssPhyChn, stream, &config);
        if (ret != TD_SUCCESS) {
            HI_PRINTF("CreateStream failed ret:%d\n", ret);
            return TD_FAILURE;
        }
    }
    AddStream(cameraInfo, streamIndex, vpssChn, stream);
    *streamId = streamIndex;
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraGetDeviceId(const char *camera, uint32_t streamId, uint32_t *deviceId)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || streamId >= CAEMRA_MAX_STREAM_NUM || deviceId == NULL, TD_FAILURE);
    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->internalStreamInfo[streamId].isUsed == false, TD_FAILURE);
    *deviceId = GetDeviceId((uint32_t)cameraInfo->internalStreamInfo[streamId].vpssChn);
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraStreamDestroy(const char *camera, uint32_t streamId)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || streamId >= CAEMRA_MAX_STREAM_NUM, TD_FAILURE);
    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->internalStreamInfo[streamId].isUsed == false, TD_FAILURE);
    DeleteStream(cameraInfo, streamId);
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraStreamOn(const char *camera, uint32_t streamId)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || streamId >= CAEMRA_MAX_STREAM_NUM, TD_FAILURE);
    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->internalStreamInfo[streamId].isUsed == false, TD_FAILURE);
    if (cameraInfo->streamAttr[streamId].type == STREAM_PREVIEW && cameraInfo->type != CAM_TYPE_TRUE_DEAPTH) {
        HAL_LOG_DOFUNC_RETURN(EnablePreview(cameraInfo->internalStreamInfo[streamId].vpssChn,
            cameraId,
            &cameraInfo->internalStreamInfo[streamId].pos,
            &cameraInfo->streamAttr[streamId]));
    }
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraStreamOff(const char *camera, uint32_t streamId)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || streamId >= CAEMRA_MAX_STREAM_NUM, TD_FAILURE);
    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->internalStreamInfo[streamId].isUsed == false, TD_FAILURE);
    if (cameraInfo->streamAttr[streamId].type == STREAM_PREVIEW && cameraInfo->type != CAM_TYPE_TRUE_DEAPTH) {
        DisablePreview(cameraInfo->internalStreamInfo[streamId].vpssChn, cameraId);
    }
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraDequeueBuf(const char *camera, uint32_t streamId, HalBuffer *buffer)
{
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);

    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || streamId >= CAEMRA_MAX_STREAM_NUM || buffer == NULL, TD_FAILURE);
    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->internalStreamInfo[streamId].isUsed == false, TD_FAILURE);
    ot_vpss_grp vpssGrp = 0;
    if (buffer->format == FORMAT_PRIVATE) {
        SetChnDepth(&(cameraInfo->internalStreamInfo[streamId]), buffer->format);
        HAL_LOG_DOFUNC_RETURN(ss_mpi_vpss_get_chn_frame(vpssGrp, cameraInfo->internalStreamInfo[streamId].vpssChn,
            &cameraInfo->internalStreamInfo[streamId].videoFrame, FRAME_TIME_OUT));
    } else {
        buffer->format = cameraInfo->streamAttr[streamId].format;
        SetChnDepth(&(cameraInfo->internalStreamInfo[streamId]), buffer->format);
        if (buffer->format == FORMAT_YVU420) {
            HAL_LOG_DOFUNC_RETURN(ss_mpi_vpss_get_chn_frame(vpssGrp, cameraInfo->internalStreamInfo[streamId].vpssChn,
                &cameraInfo->internalStreamInfo[streamId].videoFrame, FRAME_TIME_OUT));
        } else if (buffer->format == FORMAT_RGB_BAYER_12BPP) {
            HAL_LOG_DOFUNC_RETURN(ss_mpi_vi_get_pipe_frame(0, &cameraInfo->internalStreamInfo[streamId].videoFrame,
                FRAME_TIME_OUT));
        } else {
            HAL_LOGI("not support format\n");
            return TD_FAILURE;
        }
    }
    CovertToBuffer(&cameraInfo->internalStreamInfo[streamId].videoFrame, buffer);
    return TD_SUCCESS;
}

int32_t HalCameraQueueBuf(const char *camera, uint32_t streamId, const HalBuffer *buffer)
{
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);

    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || streamId >= CAEMRA_MAX_STREAM_NUM || buffer == NULL, TD_FAILURE);
    ot_vpss_grp vpssGrp = 0;
    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->internalStreamInfo[streamId].isUsed == false, TD_FAILURE);
    if (buffer->format == FORMAT_PRIVATE) {
        HAL_LOG_DOFUNC_RETURN(ss_mpi_vpss_release_chn_frame(vpssGrp, cameraInfo->internalStreamInfo[streamId].vpssChn,
            &cameraInfo->internalStreamInfo[streamId].videoFrame));
        return TD_SUCCESS;
    }
    HAL_LOG_DOFUNC(ss_mpi_sys_munmap(buffer->virAddr, buffer->size));
    if (buffer->format == FORMAT_YVU420) {
        HAL_LOG_DOFUNC_RETURN(ss_mpi_vpss_release_chn_frame(vpssGrp, cameraInfo->internalStreamInfo[streamId].vpssChn,
            &cameraInfo->internalStreamInfo[streamId].videoFrame));
    } else if (buffer->format == FORMAT_RGB_BAYER_12BPP) {
        HAL_LOG_DOFUNC_RETURN(ss_mpi_vi_release_pipe_frame(0, &cameraInfo->internalStreamInfo[streamId].videoFrame));
    } else {
        HAL_LOGI("not support format\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

int32_t HalCameraSetBufferCallback(const char *camera, const BufferAvailable callback)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || callback == NULL, TD_FAILURE);
    CameraInfo *cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->bufferCallBack != NULL, TD_FAILURE);
    cameraInfo->bufferCallBack = callback;
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraStreamSetInfo(const char *camera, uint32_t streamId, const StreamInfo *info)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || streamId >= CAEMRA_MAX_STREAM_NUM || info == NULL, TD_FAILURE);
    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->internalStreamInfo[streamId].isUsed == false, TD_FAILURE);
    if (info->type == STREAM_INFO_POS) {
        cameraInfo->internalStreamInfo[streamId].pos.x = info->u.pos.x;
        cameraInfo->internalStreamInfo[streamId].pos.y = info->u.pos.y;
    } else {
        HAL_LOGI("not support now\n");
    }
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraStreamGetInfo(const char *camera, uint32_t streamId, StreamInfo *info)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || streamId >= CAEMRA_MAX_STREAM_NUM || info == NULL, TD_FAILURE);
    CameraInfo* cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->internalStreamInfo[streamId].isUsed == false, TD_FAILURE);
    if (info->type == STREAM_INFO_POS) {
        info->u.pos.x = cameraInfo->internalStreamInfo[streamId].pos.x;
        info->u.pos.y = cameraInfo->internalStreamInfo[streamId].pos.y;
    } else {
        HAL_LOGI("not support now\n");
    }
    HAL_LOGI("not support now\n");
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraDeviceSetInfo(const char *camera, const DeviceInfo *info)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || info == NULL, TD_FAILURE);
    HAL_LOGI("not support now\n");
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraDeviceGetInfo(const char *camera, DeviceInfo *info)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || info == NULL, TD_FAILURE);
    HAL_LOGI("not support now\n");
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraGetAbility(const char *camera, AbilityInfo *ability)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || ability == NULL, TD_FAILURE);
    HAL_LOG_DOFUNC(memset_s(ability, sizeof(AbilityInfo), 0, sizeof(AbilityInfo)));
    if (cameraId == 0) {
        HAL_LOG_DOFUNC(strncpy_s(ability->cameraDesc, DESC_MAX_LEN - 1, CAMERA_RGB, sizeof(CAMERA_RGB)));
        ability->type = CAM_TYPE_WIDE_ANGLE;
    } else {
        HAL_LOG_DOFUNC(strncpy_s(ability->cameraDesc, DESC_MAX_LEN - 1, CAMERA_TOF, sizeof(CAMERA_TOF)));
        ability->type = CAM_TYPE_TRUE_DEAPTH;
    }
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraSetResultCb(const char *camera, CameraResultCb cb)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    char *endPtr = NULL;
    uint32_t cameraId = (uint32_t)strtol(camera, &endPtr, NUMBER_BASE_10);
    LOG_CHK_RETURN_ERR(endPtr == camera || *endPtr != '\0', TD_FAILURE);
    LOG_CHK_RETURN_ERR(cameraId >= g_cameraNum || cb == NULL, TD_FAILURE);
    CameraInfo *cameraInfo = &g_cameraInfo[cameraId];
    LOG_CHK_RETURN_ERR(cameraInfo->resultCb != NULL, TD_FAILURE);
    cameraInfo->resultCb = cb;
    HAL_EXIT();
    return TD_SUCCESS;
}

int32_t HalCameraUpdateSaturationMode(const char *value, uint32_t len)
{
    int32_t saturationMode;
    int32_t saturationValue;
    int32_t ret = sscanf_s(value, "%d#%d", &saturationMode, &saturationValue);
    if (ret == -1 || ret == 0 || ret == 1) {
        HAL_LOGE("saturation mode sscanf_s failed!");
        return ret;
    }
    ot_vi_pipe viPipe = 0;
    ot_isp_saturation_attr satAttr;
    ss_mpi_isp_get_saturation_attr(viPipe, &satAttr);
    if (saturationMode == CAMERA_ATTR_OP_MODE_MANUAL) {
        satAttr.op_type = OT_OP_MODE_MANUAL;
        satAttr.manual_attr.saturation = saturationValue;
        ret = ss_mpi_isp_set_saturation_attr(viPipe, &satAttr);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("set exposure attr failed ret = %d", ret);
            return TD_FAILURE;
        }
        HAL_LOGI("HalCameraUpdateSaturationMode: set saturation mode MANUAL success!");
    } else if (saturationMode == CAMERA_ATTR_OP_MODE_AUTO) {
        satAttr.op_type = OT_OP_MODE_AUTO;
        ret = ss_mpi_isp_set_saturation_attr(viPipe, &satAttr);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("set auto exposure attr failed ret = %d", ret);
            return TD_FAILURE;
        }
        HAL_LOGI("HalCameraUpdateSaturationMode: set saturation mode AUTO success!");
    } else {
        HAL_LOGE("not support saturation mode = %d", saturationMode);
        ret = TD_FAILURE;
    }
    return ret;
}

int32_t HalCameraUpdateSharpenMode(const char *value, uint32_t len)
{
    int32_t sharpenMode;
    int32_t sharpenValue;
    int32_t ret = sscanf_s(value, "%d#%d", &sharpenMode, &sharpenValue);
    if (ret == -1 || ret == 0 || ret == 1) {
        HAL_LOGE("sharpen mode sscanf_s failed!");
        return ret;
    }
    ot_vi_pipe viPipe = 0;
    ot_isp_sharpen_attr shpAttr;
    ss_mpi_isp_get_sharpen_attr(viPipe, &shpAttr);
    if (sharpenMode == CAMERA_ATTR_OP_MODE_MANUAL) {
        shpAttr.op_type = OT_OP_MODE_MANUAL;
        shpAttr.manual_attr.max_sharp_gain = sharpenValue;
        ret = ss_mpi_isp_set_sharpen_attr(viPipe, &shpAttr);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("set sharpen attr failed ret = %d", ret);
            return TD_FAILURE;
        }
        HAL_LOGI("HalCameraUpdateSharpenMode: set sharpen mode MANUAL success!\n");
    } else if (sharpenMode == CAMERA_ATTR_OP_MODE_AUTO) {
        shpAttr.op_type = OT_OP_MODE_AUTO;
        ret = ss_mpi_isp_set_sharpen_attr(viPipe, &shpAttr);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("set auto sharpen attr failed ret = %d", ret);
            return TD_FAILURE;
        }
        HAL_LOGI("HalCameraUpdateSharpenMode: set sharpen mode AUTO success!");
    } else {
        HAL_LOGE("not support sharpen mode = %d", sharpenMode);
        ret = TD_FAILURE;
    }
    return ret;
}

int32_t HalCameraUpdateDrcMode(const char *value, uint32_t len)
{
    int32_t drcMode;
    int32_t drcValue;
    int32_t ret = sscanf_s(value, "%d#%d", &drcMode, &drcValue);
    if (ret == -1 || ret == 0 || ret == 1) {
        HAL_LOGE("drc mode sscanf_s failed!");
        return ret;
    }
    ot_vi_pipe viPipe = 0;
    ot_isp_drc_attr drcAttr;
    ss_mpi_isp_get_drc_attr(viPipe, &drcAttr);
    if (drcMode == CAMERA_ATTR_OP_MODE_MANUAL) {
        drcAttr.enable = true;
        drcAttr.op_type = OT_OP_MODE_MANUAL;
        drcAttr.manual_attr.strength = drcValue;
        ret = ss_mpi_isp_set_drc_attr(viPipe, &drcAttr);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("set drc attr failed ret = %d", ret);
            return TD_FAILURE;
        }
        HAL_LOGI("HalCameraUpdateDrcMode: set drc mode MANUAL success!");
    } else if (drcMode == CAMERA_ATTR_OP_MODE_AUTO) {
        drcAttr.enable = false;
        drcAttr.op_type = OT_OP_MODE_AUTO;
        ret = ss_mpi_isp_set_drc_attr(viPipe, &drcAttr);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("set auto drc attr failed ret = %d", ret);
            return TD_FAILURE;
        }
        HAL_LOGI("HalCameraUpdateDrcMode: set drc mode AUTO success!");
    } else {
        HAL_LOGE("not support drc mode = %d", drcMode);
        ret = TD_FAILURE;
    }
    return ret;
}

int32_t HalCameraUpdateWbMode(const char *value, uint32_t len)
{
    int32_t wbMode;
    int32_t wbValue;
    int32_t ret = sscanf_s(value, "%d#%d", &wbMode, &wbValue);
    if (ret == -1 || ret == 0 || ret == 1) {
        HAL_LOGE("wb mode sscanf_s failed!");
        return ret;
    }
    ot_vi_pipe viPipe = 0;
    ot_isp_wb_attr wbAttr;
    ss_mpi_isp_get_wb_attr(viPipe, &wbAttr);
    if (wbMode == CAMERA_ATTR_OP_MODE_MANUAL) {
        wbAttr.op_type = OT_OP_MODE_MANUAL;
        wbAttr.manual_attr.r_gain = wbValue;
        ret = ss_mpi_isp_set_wb_attr(viPipe, &wbAttr);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("set wb attr failed ret = %d", ret);
            return TD_FAILURE;
        }
        HAL_LOGI("HalCameraUpdateWbMode: set wb mode MANUAL success!");
    } else if (wbMode == CAMERA_ATTR_OP_MODE_AUTO) {
        wbAttr.op_type = OT_OP_MODE_AUTO;
        ret = ss_mpi_isp_set_wb_attr(viPipe, &wbAttr);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("set auto sharpen attr failed ret = %d", ret);
            return TD_FAILURE;
        }
        HAL_LOGI("HalCameraUpdateWbMode: set wb mode AUTO success!");
    } else {
        HAL_LOGE("not support wb mode = %d", wbMode);
        ret = TD_FAILURE;
    }
    return ret;
}

int32_t HalCameraUpdateExposureMode(const char *value, uint32_t len)
{
    int32_t exposureMode;
    int32_t exposureValue;
    int32_t ret = sscanf_s(value, "%d#%d", &exposureMode, &exposureValue);
    if (ret == -1 || ret == 0 || ret == 1) {
        HAL_LOGE("exposure mode sscanf_s failed!");
        return ret;
    }
    ot_vi_pipe viPipe = 0;
    ot_isp_exposure_attr expAttr;
    ss_mpi_isp_get_exposure_attr(viPipe, &expAttr);
    if (exposureMode == CAMERA_ATTR_OP_MODE_MANUAL) {
        expAttr.bypass = TD_FALSE;
        expAttr.op_type = OT_OP_MODE_MANUAL;
        expAttr.manual_attr.a_gain_op_type = OT_OP_MODE_MANUAL;
        expAttr.manual_attr.d_gain_op_type = OT_OP_MODE_MANUAL;
        expAttr.manual_attr.ispd_gain_op_type = OT_OP_MODE_MANUAL;
        expAttr.manual_attr.exp_time_op_type = OT_OP_MODE_MANUAL;
        expAttr.manual_attr.isp_d_gain = exposureValue;
        ret = ss_mpi_isp_set_exposure_attr(viPipe, &expAttr);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("set exposure attr failed ret = %d", ret);
            return TD_FAILURE;
        }
        HAL_LOGI("HalCameraUpdateExposureMode: set exposure mode MANUAL success!");
    } else if (exposureMode == CAMERA_ATTR_OP_MODE_AUTO) {
        expAttr.bypass = TD_FALSE;
        expAttr.op_type = OT_OP_MODE_AUTO;
        ret = ss_mpi_isp_set_exposure_attr(viPipe, &expAttr);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("set auto exposure attr failed ret = %d", ret);
            return TD_FAILURE;
        }
        HAL_LOGI("HalCameraUpdateExposureMode: set exposure mode AUTO success!");
    } else {
        HAL_LOGE("not support exposure mode = %d", exposureMode);
        ret = TD_FAILURE;
    }
    return ret;
}

int32_t HalCameraUpdateSettings(const char *camera, const char *value, uint32_t len)
{
    HAL_ENTER();
    HI_CHECK_NULL_PTR(camera);
    HI_CHECK_NULL_PTR(value);
    char keyFlag[MAX_KEY_STR_SIZE] = { 0 };
    char valueFlag[MAX_KEY_VALUE_SIZE] = { 0 };
    int32_t ret = sscanf_s(value, "%s = %s", keyFlag, MAX_KEY_STR_SIZE,
        valueFlag, MAX_KEY_VALUE_SIZE);
    if (ret == -1) {
        HAL_LOGE("sscanf_s failed, ret = % d", ret);
        return ret;
    }
    if (strcmp(keyFlag, KEY_EXPOSURE_MODE) == 0) {
        ret = HalCameraUpdateExposureMode(valueFlag, MAX_KEY_VALUE_SIZE);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("Update exposure mode failed!");
            return ret;
        }
    } else if (strcmp(keyFlag, KEY_SATURATION_MODE) == 0) {
        ret = HalCameraUpdateSaturationMode(valueFlag, MAX_KEY_VALUE_SIZE);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("Update saturation mode failed!");
            return ret;
        }
    } else if (strcmp(keyFlag, KEY_SHARPEN_MODE) == 0) {
        ret = HalCameraUpdateSharpenMode(valueFlag, MAX_KEY_VALUE_SIZE);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("Update sharpen mode failed!");
            return ret;
        }
    } else if (strcmp(keyFlag, KEY_DRC_MODE) == 0) {
        ret = HalCameraUpdateDrcMode(valueFlag, MAX_KEY_VALUE_SIZE);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("Update drc mode failed!");
            return ret;
        }
    } else if (strcmp(keyFlag, KEY_WB_MODE) == 0) {
        ret = HalCameraUpdateWbMode(valueFlag, MAX_KEY_VALUE_SIZE);
        if (ret != TD_SUCCESS) {
            HAL_LOGE("Update wb mode failed!");
            return ret;
        }
    } else {
        HAL_LOGE("Unsupported Parameter Settings!");
    }
    return TD_SUCCESS;
}

static HalCameraManager g_localHalCameraManager = {
    .HalCameraInit = HalCameraInit,
    .HalCameraDeinit = HalCameraDeinit,
    .HalCameraGetModeNum = HalCameraGetModeNum,
    .HalCameraSetMode = HalCameraSetMode,
    .HalCameraSetDeviceDetectCb = HalCameraSetDeviceDetectCb,
    .HalCameraGetDeviceNum = HalCameraGetDeviceNum,
    .HalCameraGetDeviceList = HalCameraGetDeviceList,
    .HalCameraGetStreamCapNum = HalCameraGetStreamCapNum,
    .HalCameraGetStreamCap = HalCameraGetStreamCap,
    .HalCameraDeviceOpen = HalCameraDeviceOpen,
    .HalCameraDeviceClose = HalCameraDeviceClose,
    .HalCameraStreamCreate = HalCameraStreamCreate,
    .HalCameraStreamDestroy = HalCameraStreamDestroy,
    .HalCameraGetDeviceId = HalCameraGetDeviceId,
    .HalCameraStreamOn = HalCameraStreamOn,
    .HalCameraStreamOff = HalCameraStreamOff,
    .HalCameraDequeueBuf = HalCameraDequeueBuf,
    .HalCameraQueueBuf = HalCameraQueueBuf,
    .HalCameraSetBufferCallback = HalCameraSetBufferCallback,
    .HalCameraStreamSetInfo = HalCameraStreamSetInfo,
    .HalCameraStreamGetInfo = HalCameraStreamGetInfo,
    .HalCameraDeviceSetInfo = HalCameraDeviceSetInfo,
    .HalCameraDeviceGetInfo = HalCameraDeviceGetInfo,
    .HalCameraGetAbility = HalCameraGetAbility,
    .HalCameraSetResultCb = HalCameraSetResultCb,
    .HalCameraUpdateSettings = HalCameraUpdateSettings,
};

HalCameraManager *GetHalCameraFuncs(void)
{
    return &g_localHalCameraManager;
}
