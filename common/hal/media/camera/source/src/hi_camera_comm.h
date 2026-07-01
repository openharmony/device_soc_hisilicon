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

#ifndef OHOS_CAMERA_COMM_H
#define OHOS_CAMERA_COMM_H


#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "ot_type.h"
#include "ot_buffer.h"
#include "ot_common_adec.h"
#include "ot_common_aenc.h"
#include "ot_common_aio.h"
#include "ot_common_isp.h"
#include "ot_common_region.h"
#include "ot_common_sys.h"
#include "ot_common_vb.h"
#include "ot_common_venc.h"
#include "ot_common_vgs.h"
#include "ot_common_vi.h"
#include "ot_common_vo.h"
#include "ot_common_vpss.h"
#include "ot_common.h"
#include "ot_defines.h"
#include "ot_math.h"
#include "ot_mipi_rx.h"
#include "ot_sns_ctrl.h"
#include "ss_mpi_ae.h"
#include "ss_mpi_audio.h"
#include "ss_mpi_awb.h"
#include "ss_mpi_isp.h"
#include "ss_mpi_region.h"
#include "ss_mpi_sys.h"
#if !defined (__SS928V100__)
#include "ss_mpi_sys_bind.h"
#include "ss_mpi_sys_mem.h"
#endif
#include "ss_mpi_vb.h"
#include "ss_mpi_venc.h"
#include "ss_mpi_vgs.h"
#include "ss_mpi_vi.h"
#include "ss_mpi_vo.h"
#include "ss_mpi_vpss.h"

#ifdef HAVE_DISPLAY
#include "ot_mipi_tx.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* ******************************************************
    macro define
****************************************************** */
#define FILE_NAME_LEN 128

#define CHECK_CHN_RET(express, Chn, name)                                                                  \
    do {                                                                                                   \
        int32_t ret = (express);                                                                           \
        if (TD_SUCCESS != ret) {                                                                           \
            printf("\033[0;31m%s chn %d failed at %s: LINE: %d with %#x!\033[0;39m\n", (name), (Chn),      \
                __FUNCTION__, __LINE__, ret);                                                              \
            (void)fflush(stdout);                                                                          \
            return ret;                                                                                    \
        }                                                                                                  \
    } while (0)

#define CHECK_RET(express, name)                                                                                    \
    do {                                                                                                            \
        int32_t ret;                                                                                                 \
        ret = express;                                                                                              \
        if (TD_SUCCESS != ret) {                                                                                    \
            printf("\033[0;31m%s failed at %s: LINE: %d with %#x!\033[0;39m\n", name, __FUNCTION__, __LINE__, ret); \
            return ret;                                                                                             \
        }                                                                                                           \
    } while (0)
#define HISI_PIXEL_FORMAT OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420
#define SENSOR_DESC_MAX_LEN  64
#define WDR_MAX_PIPE_NUM 4
#define MAX_CHN_NUM_PER_PIPE 2
#define MAX_FRAME_WIDTH 8192

#ifndef __HuaweiLite__
#define VI_DATA_PATH "."
#else
#define VI_DATA_PATH "/sharefs"
#endif

#define SUCCESS 0
#define FAILURE (-1)

#define DEBUG_LOG_SWITCH 1
#define INFO_LOG_SWITCH 1
#define ERROR_LOG_SWITCH 1
#define TAG "CamHal"
#ifdef ERROR_LOG_SWITCH
#define HAL_LOGE(fmt, args...) do {                              \
        printf("E: %s %s-%d: " fmt, TAG, __func__, __LINE__, ##args); \
    } while (0)

#define HAL_LOGI(fmt, args...) do {                               \
        printf("I: %s %s-%d: " fmt, TAG, __func__, __LINE__, ##args); \
    } while (0)

#define HAL_LOGD(fmt, args...) do {                               \
        printf("D: %s %s-%d: " fmt, TAG, __func__, __LINE__, ##args); \
    } while (0)
#else
#define HAL_LOGE(...)
#define HAL_LOGI(...)
#define HAL_LOGD(...)
#endif

#define HAL_ENTER() HAL_LOGI("enter\n")
#define HAL_EXIT() HAL_LOGI("exit\n")
#define LOG_CHK_RETURN(val) \
    do {                       \
        if ((val)) {           \
            HAL_LOGE("\n");      \
            return;            \
        };                     \
    } while (0)

#define LOG_CHK_RETURN_ERR(val, ret) \
    do {                                \
        if ((val)) {                    \
            HAL_LOGE("\n");               \
            return ret;                 \
        }                               \
    } while (0)

#define HI_PRINTF(fmt...) do {  \
        printf("[%s]-%d: ", __FUNCTION__, __LINE__); \
        printf(fmt);                                 \
    } while (0)

#define HAL_LOG_DOFUNC(func) \
    do { \
        int32_t ret = func; \
        if (ret != 0) { \
            printf("[%s]-%d:  error = 0x%x", __FUNCTION__, __LINE__, ret); \
        } \
    } while (0)

#define HAL_LOG_DOFUNC_RETURN(func) \
    do { \
        int32_t ret = func; \
        if (ret != 0) { \
            printf("[%s]-%d:  error = 0x%x", __FUNCTION__, __LINE__, ret); \
            return ret; \
        } \
    } while (0)

#define HI_CHECK_NULL_PTR(ptr) do { \
        if ((ptr) == NULL) { \
            printf("func:%s,line:%d, NULL pointer\n", __FUNCTION__, __LINE__); \
            return TD_FAILURE;                                                 \
        } \
    } while (0)

#define CHECK_SNPRINTF_PROC(ret) do { \
    if ((ret) < 0) { \
        printf("[%s:%d], snprintf_s failed\n", __FUNCTION__, __LINE__); \
    } \
} while (0)

/* ******************************************************
    structure define
****************************************************** */
typedef struct INI_VB_PARAM_CONFIG_S {
    uint32_t stMaxPoolCnt;
    ot_size stImageSize[OT_VB_MAX_COMMON_POOLS];
    uint32_t stPixFormat[OT_VB_MAX_COMMON_POOLS];
    uint32_t stDataBitWidth[OT_VB_MAX_COMMON_POOLS];
    uint32_t stCompressMode[OT_VB_MAX_COMMON_POOLS];
    uint32_t stDefaultAlign[OT_VB_MAX_COMMON_POOLS];
    uint32_t stBlkCount[OT_VB_MAX_COMMON_POOLS];
} VB_PARAM_CONFIG_S;

typedef struct {
    int32_t s32SnsId;
    int32_t s32BusId;
    combo_dev_t MipiDev;
} HISI_SENSOR_INFO_S;

typedef struct {
    td_bool bSnap;
    td_bool bDoublePipe;
    ot_vi_pipe VideoPipe;
    ot_vi_pipe SnapPipe;
    ot_vi_vpss_mode_type enVideoPipeMode;
    ot_vi_vpss_mode_type enSnapPipeMode;
} HISI_SNAP_INFO_S;

typedef struct {
    ot_vi_dev ViDev;
    ot_wdr_mode wdrMode;
} HISI_DEV_INFO_S;

typedef struct {
    ot_vi_pipe aPipe[OT_VI_MAX_PIPE_NUM];
    ot_vi_vpss_mode_type enMastPipeMode;
    td_bool bMultiPipe;
    td_bool bVcNumCfged;
    td_bool ispBypass;
    ot_pixel_format pixelFormat;
    uint32_t u32VCNum[OT_VI_MAX_PIPE_NUM];
    td_u32 frameInterruptType;
    td_u32 earlyLine;
} HISI_PIPE_INFO_S;

typedef struct {
    ot_vi_chn ViChn;
    ot_pixel_format enPixFormat;
    ot_dynamic_range dynamicRange;
    ot_video_format videoFormat;
    ot_compress_mode compressMode;
} HISI_CHN_INFO_S;

typedef struct {
    HISI_SENSOR_INFO_S stSnsInfo;
    HISI_DEV_INFO_S stDevInfo;
    HISI_PIPE_INFO_S stPipeInfo;
    HISI_CHN_INFO_S stChnInfo;
    HISI_SNAP_INFO_S stSnapInfo;
    combo_dev_attr_t *mipiAttr;
    ot_vi_dev_attr *devAttr;
    ot_vi_pipe_attr *pipeAttr;
    ot_vi_chn_attr *chnAttr;
    ot_isp_pub_attr *ispPubAttr;
    char *sensorLib;
    char *sensorObj;
    uint16_t len;
} CAMERA_VI_CONFIG_S;

typedef struct INI_VI_PARAM_CONFIG_S {
    ot_vi_vpss_mode_type pipeMode[OT_VI_MAX_PIPE_NUM];
    char sensorLib[SENSOR_DESC_MAX_LEN];
    char sensorObj[SENSOR_DESC_MAX_LEN];
    ot_vi_dev_attr stDevAttr;
    combo_dev_attr_t stMipiAttr;
    ot_vi_pipe_attr stPipeAttr[OT_VI_MAX_PIPE_NUM];
    ot_isp_pub_attr stPubAttr;
    ot_vi_chn_attr stChnAttr[OT_VI_MAX_PIPE_NUM][MAX_CHN_NUM_PER_PIPE];
    td_u32 frameInterruptType[OT_VI_MAX_PIPE_NUM];
    td_u32 earlyLine[OT_VI_MAX_PIPE_NUM];
    int8_t busId;
    uint32_t sensorType;
} VI_PARAM_CONFIG_S;

typedef struct INI_VPSS_CHN_ATTR_CONFIG_S {
    uint32_t stMaxPortCnt;
    ot_vpss_chn_attr stVpssChnAttr[OT_VPSS_MAX_PHYS_CHN_NUM];
    ot_vpss_chn_attr stVpssExtChnAttr;
} VPSS_CHN_ATTR_CONFIG_S;

typedef struct {
    VB_PARAM_CONFIG_S stVbAttr;
    VI_PARAM_CONFIG_S stViAttr;
} InitParam;


enum CameraAttrOperatorMode {
    CAMERA_ATTR_OP_MODE_MANUAL,
    CAMERA_ATTR_OP_MODE_AUTO,
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef OHOS_CAMERA_COMM_H */
