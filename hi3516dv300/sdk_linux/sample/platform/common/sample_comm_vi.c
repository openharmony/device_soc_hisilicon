/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "sample_comm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>

#include "hi_mipi.h"
#include "hi_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MIPI_DEV_NODE "/dev/hi_mipi"

combo_dev_attr_t MIPI_4lane_CHN0_SENSOR_IMX335_12BIT_5M_NOWDR_ATTR = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate = MIPI_DATA_RATE_X1,
    .img_rect = { 0, 0, IMG_5M_WIDTH, IMG_5M_HEIGHT },
    { .mipi_attr = { DATA_TYPE_RAW_12BIT, HI_MIPI_WDR_MODE_NONE, { 0, 1, 2, 3 } } }
};

combo_dev_attr_t MIPI_4lane_CHN0_SENSOR_IMX335_10BIT_5M_WDR2TO1_ATTR = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate = MIPI_DATA_RATE_X1,
    .img_rect = { 0, 0, IMG_5M_WIDTH, IMG_5M_HEIGHT },
    { .mipi_attr = { DATA_TYPE_RAW_10BIT, HI_MIPI_WDR_MODE_VC, { 0, 1, 2, 3 } } }
};

combo_dev_attr_t MIPI_4lane_CHN0_SENSOR_IMX335_12BIT_4M_NOWDR_ATTR = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate = MIPI_DATA_RATE_X1,
    .img_rect = { 0, 204, IMG_4M_WIDTH, IMG_4M_HEIGHT },
    { .mipi_attr = { DATA_TYPE_RAW_12BIT, HI_MIPI_WDR_MODE_NONE, { 0, 1, 2, 3 } } }
};

combo_dev_attr_t MIPI_4lane_CHN0_SENSOR_IMX335_10BIT_4M_WDR2TO1_ATTR = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate = MIPI_DATA_RATE_X1,
    .img_rect = { 0, 204, IMG_4M_WIDTH, IMG_4M_HEIGHT },
    { .mipi_attr = { DATA_TYPE_RAW_10BIT, HI_MIPI_WDR_MODE_VC, { 0, 1, 2, 3 } } }
};

VI_DEV_ATTR_S DEV_ATTR_IMX335_5M_BASE = {
    VI_MODE_MIPI,
    VI_WORK_MODE_1Multiplex,
    { 0xFFF00000, 0x0 },
    VI_SCAN_PROGRESSIVE,
    { -1, -1, -1, -1 },
    VI_DATA_SEQ_YUYV,
    {
        VI_VSYNC_PULSE,
        VI_VSYNC_NEG_LOW,
        VI_HSYNC_VALID_SINGNAL,
        VI_HSYNC_NEG_HIGH,
        VI_VSYNC_VALID_SINGAL,
        VI_VSYNC_VALID_NEG_HIGH,
        {
        /* hsync_hfb hsync_act hsync_hhb */
            0, IMG_1M_WIDTH, 0,
        /* vsync0_vhb vsync0_act vsync0_hhb */
            0, IMG_1M_HEIGHT, 0,
        /* vsync1_vhb vsync1_act vsync1_hhb */
            0, 0, 0
        }
    },
    VI_DATA_TYPE_RGB,
    HI_FALSE,
    { IMG_5M_WIDTH, IMG_5M_HEIGHT },
    {
        { { IMG_5M_WIDTH, IMG_5M_HEIGHT }, },
        { VI_REPHASE_MODE_NONE, VI_REPHASE_MODE_NONE }
    },
    { WDR_MODE_NONE, IMG_5M_HEIGHT },
    DATA_RATE_X1
};

VI_DEV_ATTR_S DEV_ATTR_IMX335_4M_BASE = {
    VI_MODE_MIPI,
    VI_WORK_MODE_1Multiplex,
    { 0xFFF00000, 0x0 },
    VI_SCAN_PROGRESSIVE,
    { -1, -1, -1, -1 },
    VI_DATA_SEQ_YUYV,
    {
        VI_VSYNC_PULSE,
        VI_VSYNC_NEG_LOW,
        VI_HSYNC_VALID_SINGNAL,
        VI_HSYNC_NEG_HIGH,
        VI_VSYNC_VALID_SINGAL,
        VI_VSYNC_VALID_NEG_HIGH,
        {
        /* hsync_hfb    hsync_act    hsync_hhb */
            0, IMG_1M_WIDTH, 0,
        /* vsync0_vhb vsync0_act vsync0_hhb */
            0, IMG_1M_HEIGHT, 0,
        /* vsync1_vhb vsync1_act vsync1_hhb */
            0, 0, 0
        }
    },
    VI_DATA_TYPE_RGB,
    HI_FALSE,
    { IMG_4M_WIDTH, IMG_4M_HEIGHT },
    {
        { { IMG_4M_WIDTH, IMG_4M_HEIGHT }, },
        { VI_REPHASE_MODE_NONE, VI_REPHASE_MODE_NONE }
    },
    { WDR_MODE_NONE, IMG_4M_HEIGHT },
    DATA_RATE_X1
};

static VI_PIPE_ATTR_S PIPE_ATTR_2592x1944_RAW12_420_3DNR_RFR = {
    VI_PIPE_BYPASS_NONE,
    HI_FALSE,
    HI_FALSE,
    IMG_5M_WIDTH,
    IMG_5M_HEIGHT,
    PIXEL_FORMAT_RGB_BAYER_12BPP,
    COMPRESS_MODE_LINE,
    DATA_BITWIDTH_12,
    HI_FALSE,
    { PIXEL_FORMAT_YVU_SEMIPLANAR_420, DATA_BITWIDTH_8, VI_NR_REF_FROM_RFR, COMPRESS_MODE_NONE },
    HI_FALSE,
    { -1, -1 },
    HI_FALSE
};

static VI_PIPE_ATTR_S PIPE_ATTR_2592x1944_RAW10_420_3DNR_RFR = {
    VI_PIPE_BYPASS_NONE,
    HI_FALSE,
    HI_FALSE,
    IMG_5M_WIDTH,
    IMG_5M_HEIGHT,
    PIXEL_FORMAT_RGB_BAYER_10BPP,
    COMPRESS_MODE_LINE,
    DATA_BITWIDTH_10,
    HI_FALSE,
    { PIXEL_FORMAT_YVU_SEMIPLANAR_420, DATA_BITWIDTH_8, VI_NR_REF_FROM_RFR, COMPRESS_MODE_NONE },
    HI_FALSE,
    { -1, -1 },
    HI_FALSE
};

static VI_PIPE_ATTR_S PIPE_ATTR_2592x1536_RAW12_420_3DNR_RFR = {
    VI_PIPE_BYPASS_NONE,
    HI_FALSE,
    HI_FALSE,
    IMG_4M_WIDTH,
    IMG_4M_HEIGHT,
    PIXEL_FORMAT_RGB_BAYER_12BPP,
    COMPRESS_MODE_LINE,
    DATA_BITWIDTH_12,
    HI_FALSE,
    { PIXEL_FORMAT_YVU_SEMIPLANAR_420, DATA_BITWIDTH_8, VI_NR_REF_FROM_RFR, COMPRESS_MODE_NONE },
    HI_FALSE,
    { -1, -1 },
    HI_FALSE
};

static VI_PIPE_ATTR_S PIPE_ATTR_2592x1536_RAW10_420_3DNR_RFR = {
    VI_PIPE_BYPASS_NONE,
    HI_FALSE,
    HI_FALSE,
    IMG_4M_WIDTH,
    IMG_4M_HEIGHT,
    PIXEL_FORMAT_RGB_BAYER_10BPP,
    COMPRESS_MODE_LINE,
    DATA_BITWIDTH_10,
    HI_FALSE,
    { PIXEL_FORMAT_YVU_SEMIPLANAR_420, DATA_BITWIDTH_8, VI_NR_REF_FROM_RFR, COMPRESS_MODE_NONE },
    HI_FALSE,
    { -1, -1 },
    HI_FALSE
};

static VI_CHN_ATTR_S CHN_ATTR_2592x1944_420_SDR8_LINEAR = {
    { IMG_5M_WIDTH, IMG_5M_HEIGHT },
    PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    DYNAMIC_RANGE_SDR8,
    VIDEO_FORMAT_LINEAR,
    COMPRESS_MODE_NONE,
    0,
    0,
    0,
    { -1, -1 }
};

static VI_CHN_ATTR_S CHN_ATTR_2592x1536_420_SDR8_LINEAR = {
    { IMG_4M_WIDTH, IMG_4M_HEIGHT },
    PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    DYNAMIC_RANGE_SDR8,
    VIDEO_FORMAT_LINEAR,
    COMPRESS_MODE_NONE,
    0,
    0,
    0,
    { -1, -1 }
};

HI_S32 SAMPLE_COMM_VI_SetMipiHsMode(lane_divide_mode_t enHsMode)
{
    HI_S32 fd = -1;
    HI_S32 s32Ret = HI_FAILURE;

    fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi dev failed\n");
        return s32Ret;
    }

    s32Ret = ioctl(fd, HI_MIPI_SET_HS_MODE, &enHsMode);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MIPI_SET_HS_MODE failed\n");
    }

    close(fd);
    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_EnableMipiClock(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 fd = -1;
    combo_dev_t devno;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi dev failed\n");
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            goto EXIT;
        }
        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        devno = pstViInfo->stSnsInfo.MipiDev;
        s32Ret = ioctl(fd, HI_MIPI_ENABLE_MIPI_CLOCK, &devno);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("MIPI_ENABLE_CLOCK %d failed\n", devno);
            goto EXIT;
        }
    }

EXIT:
    close(fd);
    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_DisableMipiClock(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 fd = -1;
    combo_dev_t devno;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi dev failed\n");
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            goto EXIT;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        devno = pstViInfo->stSnsInfo.MipiDev;
        s32Ret = ioctl(fd, HI_MIPI_DISABLE_MIPI_CLOCK, &devno);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("MIPI_DISABLE_CLOCK %d failed\n", devno);
            goto EXIT;
        }
    }

EXIT:
    close(fd);
    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_ResetMipi(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 fd = -1;
    combo_dev_t devno;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi dev failed\n");
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            goto EXIT;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        devno = pstViInfo->stSnsInfo.MipiDev;
        s32Ret = ioctl(fd, HI_MIPI_RESET_MIPI, &devno);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("RESET_MIPI %d failed\n", devno);
            goto EXIT;
        }
    }

EXIT:
    close(fd);
    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_UnresetMipi(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 fd = -1;
    combo_dev_t devno;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi dev failed\n");
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            goto EXIT;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        devno = pstViInfo->stSnsInfo.MipiDev;
        s32Ret = ioctl(fd, HI_MIPI_UNRESET_MIPI, &devno);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("UNRESET_MIPI %d failed\n", devno);
            goto EXIT;
        }
    }

EXIT:
    close(fd);
    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_EnableSensorClock(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 fd = -1;
    sns_clk_source_t SnsDev;

    fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi dev failed\n");
        return s32Ret;
    }

    for (SnsDev = 0; SnsDev < SNS_MAX_CLK_SOURCE_NUM; SnsDev++) {
        s32Ret = ioctl(fd, HI_MIPI_ENABLE_SENSOR_CLOCK, &SnsDev);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MIPI_ENABLE_SENSOR_CLOCK failed\n");
            goto EXIT;
        }
    }

EXIT:
    close(fd);
    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_DisableSensorClock(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 fd = -1;
    sns_clk_source_t SnsDev;

    fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi dev failed\n");
        return s32Ret;
    }

    for (SnsDev = 0; SnsDev < SNS_MAX_CLK_SOURCE_NUM; SnsDev++) {
        s32Ret = ioctl(fd, HI_MIPI_DISABLE_SENSOR_CLOCK, &SnsDev);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MIPI_DISABLE_SENSOR_CLOCK failed\n");
            goto EXIT;
        }
    }

EXIT:
    close(fd);
    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_ResetSensor(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 fd = -1;
    sns_rst_source_t SnsDev;

    fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi dev failed\n");
        return s32Ret;
    }

    for (SnsDev = 0; SnsDev < SNS_MAX_RST_SOURCE_NUM; SnsDev++) {
        s32Ret = ioctl(fd, HI_MIPI_RESET_SENSOR, &SnsDev);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MIPI_RESET_SENSOR failed\n");
            goto EXIT;
        }
    }

EXIT:
    close(fd);
    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_UnresetSensor(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 fd = -1;
    sns_rst_source_t SnsDev;

    fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi dev failed\n");
        return s32Ret;
    }

    for (SnsDev = 0; SnsDev < SNS_MAX_RST_SOURCE_NUM; SnsDev++) {
        s32Ret = ioctl(fd, HI_MIPI_UNRESET_SENSOR, &SnsDev);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MIPI_UNRESET_SENSOR failed\n");
            goto EXIT;
        }
    }

EXIT:
    close(fd);
    return s32Ret;
}

HI_VOID SAMPLE_COMM_VI_GetComboAttrBySns(SAMPLE_SNS_TYPE_E enSnsType, combo_dev_t MipiDev,
    combo_dev_attr_t *pstComboAttr)
{
    switch (enSnsType) {
        case SONY_IMX335_MIPI_5M_30FPS_12BIT:
            (hi_void)memcpy_s(pstComboAttr, sizeof(combo_dev_attr_t),
                &MIPI_4lane_CHN0_SENSOR_IMX335_12BIT_5M_NOWDR_ATTR, sizeof(combo_dev_attr_t));
            break;

        case SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1:
            (hi_void)memcpy_s(pstComboAttr, sizeof(combo_dev_attr_t),
                &MIPI_4lane_CHN0_SENSOR_IMX335_10BIT_5M_WDR2TO1_ATTR, sizeof(combo_dev_attr_t));
            break;

        case SONY_IMX335_MIPI_4M_30FPS_12BIT:
            (hi_void)memcpy_s(pstComboAttr, sizeof(combo_dev_attr_t),
                &MIPI_4lane_CHN0_SENSOR_IMX335_12BIT_4M_NOWDR_ATTR, sizeof(combo_dev_attr_t));
            break;

        case SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1:
            (hi_void)memcpy_s(pstComboAttr, sizeof(combo_dev_attr_t),
                &MIPI_4lane_CHN0_SENSOR_IMX335_10BIT_4M_WDR2TO1_ATTR, sizeof(combo_dev_attr_t));
            break;

        default:
            SAMPLE_PRT("not support enSnsType: %d\n", enSnsType);
            (hi_void)memcpy_s(pstComboAttr, sizeof(combo_dev_attr_t),
                &MIPI_4lane_CHN0_SENSOR_IMX335_12BIT_4M_NOWDR_ATTR, sizeof(combo_dev_attr_t));
            break;
    }

    return;
}

HI_S32 SAMPLE_COMM_VI_SetMipiAttr(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 fd = -1;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;
    combo_dev_attr_t stcomboDevAttr;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    fd = open(MIPI_DEV_NODE, O_RDWR);
    if (fd < 0) {
        SAMPLE_PRT("open hi_mipi dev failed.\n");
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            goto EXIT;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        SAMPLE_COMM_VI_GetComboAttrBySns(pstViInfo->stSnsInfo.enSnsType, pstViInfo->stSnsInfo.MipiDev, &stcomboDevAttr);
        stcomboDevAttr.devno = pstViInfo->stSnsInfo.MipiDev;

        SAMPLE_PRT("MipiDev%d SetMipiAttr enWDRMode: %d\n", pstViInfo->stSnsInfo.MipiDev,
            pstViInfo->stDevInfo.enWDRMode);

        s32Ret = ioctl(fd, HI_MIPI_SET_DEV_ATTR, &stcomboDevAttr);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("MIPI_SET_DEV_ATTR failed.\n");
            goto EXIT;
        }
    }

EXIT:
    close(fd);
    return s32Ret;
}

HI_U32 SAMPLE_COMM_VI_GetMipiLaneDivideMode(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    lane_divide_mode_t lane_divide_mode = LANE_DIVIDE_MODE_0;

    return (HI_U32)lane_divide_mode;
}

HI_S32 SAMPLE_COMM_VI_StartMIPI(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 s32Ret = HI_FAILURE;
    lane_divide_mode_t lane_divide_mode;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return s32Ret;
    }

    lane_divide_mode = SAMPLE_COMM_VI_GetMipiLaneDivideMode(pstViConfig);
    s32Ret = SAMPLE_COMM_VI_SetMipiHsMode(lane_divide_mode);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_SetMipiHsMode failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_EnableMipiClock(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_EnableMipiClock failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_ResetMipi(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_ResetMipi failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_EnableSensorClock();
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_EnableSensorClock failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_ResetSensor();
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_ResetSensor failed!\n");
        return s32Ret;
    }

    usleep(10000); /* sleep 10000us to avoid reset signal invalid. */

    s32Ret = SAMPLE_COMM_VI_SetMipiAttr(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_SetMipiAttr failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_UnresetMipi(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_UnresetMipi failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_UnresetSensor();
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_UnresetSensor failed!\n");
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_StopMIPI(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_ResetSensor();
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_ResetSensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_DisableSensorClock();
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_DisableSensorClock failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_ResetMipi(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_ResetMipi failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_DisableMipiClock(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_DisableMipiClock failed!\n");
        return s32Ret;
    }

    return s32Ret;
}

/* when mode[0] is VI_OFFLINE_VPSS_ONLINE, other pipe mode must set VI_OFFLINE_VPSS_ONLINE. */
static HI_VOID SAMPLE_COMM_VI_UpdateVIVPSSMode(VI_VPSS_MODE_S *pstVIVPSSMode)
{
    HI_S32 i;

    if (pstVIVPSSMode->aenMode[0] == VI_OFFLINE_VPSS_ONLINE) {
        for (i = 0; i < VI_MAX_PIPE_NUM; i++) {
            pstVIVPSSMode->aenMode[i] = pstVIVPSSMode->aenMode[0];
        }
    }
}

HI_S32 SAMPLE_COMM_VI_SetParam(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i, j;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    VI_PIPE ViPipe;
    VI_PIPE SnapPipe;
    VI_VPSS_MODE_S stVIVPSSMode;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    s32Ret = HI_MPI_SYS_GetVIVPSSMode(&stVIVPSSMode);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Get VI-VPSS mode Param failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            return s32Ret;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        ViPipe = pstViInfo->stPipeInfo.aPipe[0];
        if (ViPipe >= VI_MAX_PIPE_NUM) {
            continue;
        }
        stVIVPSSMode.aenMode[ViPipe] = pstViInfo->stPipeInfo.enMastPipeMode;

        if ((pstViInfo->stPipeInfo.bMultiPipe == HI_TRUE) ||
            (pstViInfo->stPipeInfo.enMastPipeMode == VI_OFFLINE_VPSS_ONLINE)) {
            SAMPLE_COMM_VI_UpdateVIVPSSMode(&stVIVPSSMode);
            for (j = 1; j < WDR_MAX_PIPE_NUM; j++) {
                ViPipe = pstViInfo->stPipeInfo.aPipe[j];
                if (ViPipe != -1) {
                    stVIVPSSMode.aenMode[ViPipe] = pstViInfo->stPipeInfo.enMastPipeMode;
                }
            }
        }

        if ((pstViInfo->stSnapInfo.bSnap) && (pstViInfo->stSnapInfo.bDoublePipe)) {
            SnapPipe = pstViInfo->stPipeInfo.aPipe[1];
            if (SnapPipe != -1) {
                stVIVPSSMode.aenMode[SnapPipe] = pstViInfo->stSnapInfo.enSnapPipeMode;
            }
        }
    }

    s32Ret = HI_MPI_SYS_SetVIVPSSMode(&stVIVPSSMode);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Set VI-VPSS mode Param failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

HI_VOID SAMPLE_COMM_VI_GetDevAttrBySns(SAMPLE_SNS_TYPE_E enSnsType, VI_DEV_ATTR_S *pstViDevAttr)
{
    switch (enSnsType) {
        case SONY_IMX335_MIPI_5M_30FPS_12BIT:
            (hi_void)memcpy_s(pstViDevAttr, sizeof(VI_DEV_ATTR_S), &DEV_ATTR_IMX335_5M_BASE, sizeof(VI_DEV_ATTR_S));
            break;

        case SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1:
            (hi_void)memcpy_s(pstViDevAttr, sizeof(VI_DEV_ATTR_S), &DEV_ATTR_IMX335_5M_BASE, sizeof(VI_DEV_ATTR_S));
            pstViDevAttr->au32ComponentMask[0] = 0xFFC00000;
            break;

        case SONY_IMX335_MIPI_4M_30FPS_12BIT:
            (hi_void)memcpy_s(pstViDevAttr, sizeof(VI_DEV_ATTR_S), &DEV_ATTR_IMX335_4M_BASE, sizeof(VI_DEV_ATTR_S));
            break;

        case SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1:
            (hi_void)memcpy_s(pstViDevAttr, sizeof(VI_DEV_ATTR_S), &DEV_ATTR_IMX335_4M_BASE, sizeof(VI_DEV_ATTR_S));
            pstViDevAttr->au32ComponentMask[0] = 0xFFC00000;
            break;

        default:
            (hi_void)memcpy_s(pstViDevAttr, sizeof(VI_DEV_ATTR_S), &DEV_ATTR_IMX335_4M_BASE, sizeof(VI_DEV_ATTR_S));
            break;
    }

    return;
}

HI_VOID SAMPLE_COMM_VI_GetPipeAttrBySns(SAMPLE_SNS_TYPE_E enSnsType, VI_PIPE_ATTR_S *pstPipeAttr)
{
    switch (enSnsType) {
        case SONY_IMX335_MIPI_5M_30FPS_12BIT:
            (hi_void)memcpy_s(pstPipeAttr, sizeof(VI_PIPE_ATTR_S), &PIPE_ATTR_2592x1944_RAW12_420_3DNR_RFR,
                sizeof(VI_PIPE_ATTR_S));
            break;

        case SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1:
            (hi_void)memcpy_s(pstPipeAttr, sizeof(VI_PIPE_ATTR_S), &PIPE_ATTR_2592x1944_RAW10_420_3DNR_RFR,
                sizeof(VI_PIPE_ATTR_S));
            break;

        case SONY_IMX335_MIPI_4M_30FPS_12BIT:
            (hi_void)memcpy_s(pstPipeAttr, sizeof(VI_PIPE_ATTR_S), &PIPE_ATTR_2592x1536_RAW12_420_3DNR_RFR,
                sizeof(VI_PIPE_ATTR_S));
            break;

        case SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1:
            (hi_void)memcpy_s(pstPipeAttr, sizeof(VI_PIPE_ATTR_S), &PIPE_ATTR_2592x1536_RAW10_420_3DNR_RFR,
                sizeof(VI_PIPE_ATTR_S));
            break;

        default:
            (hi_void)memcpy_s(pstPipeAttr, sizeof(VI_PIPE_ATTR_S), &PIPE_ATTR_2592x1536_RAW12_420_3DNR_RFR,
                sizeof(VI_PIPE_ATTR_S));
            break;
    }

    return;
}

HI_VOID SAMPLE_COMM_VI_GetChnAttrBySns(SAMPLE_SNS_TYPE_E enSnsType, VI_CHN_ATTR_S *pstChnAttr)
{
    switch (enSnsType) {
        case SONY_IMX335_MIPI_5M_30FPS_12BIT:
        case SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1:
            (hi_void)
                memcpy_s(pstChnAttr, sizeof(VI_CHN_ATTR_S), &CHN_ATTR_2592x1944_420_SDR8_LINEAR, sizeof(VI_CHN_ATTR_S));
            break;

        case SONY_IMX335_MIPI_4M_30FPS_12BIT:
        case SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1:
            (hi_void)
                memcpy_s(pstChnAttr, sizeof(VI_CHN_ATTR_S), &CHN_ATTR_2592x1536_420_SDR8_LINEAR, sizeof(VI_CHN_ATTR_S));
            break;

        default:
            (hi_void)
                memcpy_s(pstChnAttr, sizeof(VI_CHN_ATTR_S), &CHN_ATTR_2592x1536_420_SDR8_LINEAR, sizeof(VI_CHN_ATTR_S));
            break;
    }

    return;
}

HI_S32 SAMPLE_COMM_VI_StartDev(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 s32Ret;
    VI_DEV ViDev;
    SAMPLE_SNS_TYPE_E enSnsType;
    VI_DEV_ATTR_S stViDevAttr;

    if (pstViInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    enSnsType = pstViInfo->stSnsInfo.enSnsType;
    SAMPLE_COMM_VI_GetDevAttrBySns(enSnsType, &stViDevAttr);
    stViDevAttr.stWDRAttr.enWDRMode = pstViInfo->stDevInfo.enWDRMode;

    ViDev = pstViInfo->stDevInfo.ViDev;
    s32Ret = HI_MPI_VI_SetDevAttr(ViDev, &stViDevAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetDevAttr failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VI_EnableDev(ViDev);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_EnableDev failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_StopDev(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 s32Ret;
    VI_DEV ViDev;

    if (pstViInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    ViDev = pstViInfo->stDevInfo.ViDev;
    s32Ret = HI_MPI_VI_DisableDev(ViDev);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_DisableDev failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_BindPipeDev(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 i;
    HI_S32 s32PipeCnt = 0;
    HI_S32 s32Ret;
    VI_DEV_BIND_PIPE_S stDevBindPipe = { 0 };

    if (pstViInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    for (i = 0; i < WDR_MAX_PIPE_NUM; i++) {
        if ((pstViInfo->stPipeInfo.aPipe[i] >= 0) && (pstViInfo->stPipeInfo.aPipe[i] < VI_MAX_PIPE_NUM)) {
            stDevBindPipe.PipeId[s32PipeCnt] = pstViInfo->stPipeInfo.aPipe[i];
            s32PipeCnt++;
            stDevBindPipe.u32Num = s32PipeCnt;
        }
    }

    s32Ret = HI_MPI_VI_SetDevBindPipe(pstViInfo->stDevInfo.ViDev, &stDevBindPipe);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_SetDevBindPipe failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_ModeSwitchCreateSingleViPipe(VI_PIPE ViPipe, VI_PIPE_ATTR_S *pstPipeAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_CreatePipe(ViPipe, pstPipeAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_CreatePipe failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_ModeSwitch_EnableSingleViPipe(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_StartPipe(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        HI_MPI_VI_DestroyPipe(ViPipe);
        SAMPLE_PRT("HI_MPI_VI_StartPipe failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_StopSingleViPipe(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_StopPipe(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_StopPipe failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VI_DestroyPipe(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VI_DestroyPipe failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_ModeSwitch_StartViPipe(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 i, j;
    HI_S32 s32Ret = HI_SUCCESS;
    VI_PIPE ViPipe;
    VI_PIPE_ATTR_S stPipeAttr;

    for (i = 0; i < WDR_MAX_PIPE_NUM; i++) {
        if (pstViInfo->stPipeInfo.aPipe[i] >= 0 && pstViInfo->stPipeInfo.aPipe[i] < VI_MAX_PIPE_NUM) {
            ViPipe = pstViInfo->stPipeInfo.aPipe[i];
            SAMPLE_COMM_VI_GetPipeAttrBySns(pstViInfo->stSnsInfo.enSnsType, &stPipeAttr);
            s32Ret = SAMPLE_COMM_VI_ModeSwitchCreateSingleViPipe(ViPipe, &stPipeAttr);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("SAMPLE_COMM_VI_StartSingleViPipe  %d failed!\n", ViPipe);
                goto EXIT;
            }
        }
    }

    return s32Ret;

EXIT:
    for (j = 0; j < i; j++) {
        ViPipe = j;
        SAMPLE_COMM_VI_StopSingleViPipe(ViPipe);
    }
    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_ModeSwitch_EnableViPipe(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 i, j;
    HI_S32 s32Ret = HI_SUCCESS;
    VI_PIPE ViPipe;
    VI_PIPE_ATTR_S stPipeAttr;

    if (pstViInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    for (i = 0; i < WDR_MAX_PIPE_NUM; i++) {
        if ((pstViInfo->stPipeInfo.aPipe[i] >= 0) && (pstViInfo->stPipeInfo.aPipe[i] < VI_MAX_PIPE_NUM)) {
            ViPipe = pstViInfo->stPipeInfo.aPipe[i];
            SAMPLE_COMM_VI_GetPipeAttrBySns(pstViInfo->stSnsInfo.enSnsType, &stPipeAttr);
            s32Ret = SAMPLE_COMM_VI_ModeSwitch_EnableSingleViPipe(ViPipe);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("SAMPLE_COMM_VI_StartSingleViPipe  %d failed!\n", ViPipe);
                goto EXIT;
            }
        }
    }

    return s32Ret;

EXIT:
    for (j = 0; j < i; j++) {
        ViPipe = j;
        SAMPLE_COMM_VI_StopSingleViPipe(ViPipe);
    }
    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_StartViPipe(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 i, j;
    HI_S32 s32Ret = HI_FAILURE;
    VI_PIPE ViPipe;
    VI_PIPE_ATTR_S stPipeAttr;

    for (i = 0; i < WDR_MAX_PIPE_NUM; i++) {
        if ((pstViInfo->stPipeInfo.aPipe[i] >= 0) && (pstViInfo->stPipeInfo.aPipe[i] < VI_MAX_PIPE_NUM)) {
            ViPipe = pstViInfo->stPipeInfo.aPipe[i];
            SAMPLE_COMM_VI_GetPipeAttrBySns(pstViInfo->stSnsInfo.enSnsType, &stPipeAttr);
            if (pstViInfo->stPipeInfo.bIspBypass == HI_TRUE) {
                stPipeAttr.bIspBypass = HI_TRUE;
                stPipeAttr.enPixFmt = pstViInfo->stPipeInfo.enPixFmt;
                stPipeAttr.enBitWidth = DATA_BITWIDTH_8;
            }

            if ((ViPipe == 2) || (ViPipe == 3)) { /* V500 pipe2 and pipe3 only support cmp_none */
                stPipeAttr.enCompressMode = COMPRESS_MODE_NONE;
            }

            if ((pstViInfo->stSnapInfo.bSnap) && (pstViInfo->stSnapInfo.bDoublePipe) &&
                (ViPipe == pstViInfo->stSnapInfo.SnapPipe)) {
                s32Ret = HI_MPI_VI_CreatePipe(ViPipe, &stPipeAttr);
                if (s32Ret != HI_SUCCESS) {
                    SAMPLE_PRT("HI_MPI_VI_CreatePipe failed with %#x!\n", s32Ret);
                    goto EXIT;
                }
            } else {
                s32Ret = HI_MPI_VI_CreatePipe(ViPipe, &stPipeAttr);
                if (s32Ret != HI_SUCCESS) {
                    SAMPLE_PRT("HI_MPI_VI_CreatePipe failed with %#x!\n", s32Ret);
                    return s32Ret;
                }

                if (pstViInfo->stPipeInfo.bVcNumCfged == HI_TRUE) {
                    s32Ret = HI_MPI_VI_SetPipeVCNumber(ViPipe, pstViInfo->stPipeInfo.u32VCNum[i]);
                    if (s32Ret != HI_SUCCESS) {
                        HI_MPI_VI_DestroyPipe(ViPipe);
                        SAMPLE_PRT("HI_MPI_VI_SetPipeVCNumber failed with %#x!\n", s32Ret);
                        return s32Ret;
                    }
                }

                s32Ret = HI_MPI_VI_StartPipe(ViPipe);
                if (s32Ret != HI_SUCCESS) {
                    HI_MPI_VI_DestroyPipe(ViPipe);
                    SAMPLE_PRT("HI_MPI_VI_StartPipe failed with %#x!\n", s32Ret);
                    return s32Ret;
                }
            }
        }
    }

    return s32Ret;

EXIT:
    for (j = 0; j < i; j++) {
        ViPipe = j;
        SAMPLE_COMM_VI_StopSingleViPipe(ViPipe);
    }
    return s32Ret;
}

HI_VOID SAMPLE_COMM_VI_StopViPipe(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 i;
    VI_PIPE ViPipe;

    for (i = 0; i < WDR_MAX_PIPE_NUM; i++) {
        if ((pstViInfo->stPipeInfo.aPipe[i] >= 0) && (pstViInfo->stPipeInfo.aPipe[i] < VI_MAX_PIPE_NUM)) {
            ViPipe = pstViInfo->stPipeInfo.aPipe[i];
            SAMPLE_COMM_VI_StopSingleViPipe(ViPipe);
        }
    }

    return;
}

static HI_S32 SAMPLE_COMM_VI_ModeSwitch_StartViChn(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 i;
    HI_BOOL bNeedChn;
    HI_S32 s32Ret = HI_SUCCESS;
    VI_PIPE ViPipe;
    VI_CHN ViChn;
    VI_CHN_ATTR_S stChnAttr;

    for (i = 0; i < WDR_MAX_PIPE_NUM; i++) {
        if ((pstViInfo->stPipeInfo.aPipe[i] >= 0) && (pstViInfo->stPipeInfo.aPipe[i] < VI_MAX_PIPE_NUM)) {
            if (pstViInfo->stDevInfo.enWDRMode == WDR_MODE_NONE) {
                bNeedChn = HI_TRUE;
            } else {
                bNeedChn = (i > 0) ? HI_FALSE : HI_TRUE;
            }
            if (bNeedChn) {
                ViPipe = pstViInfo->stPipeInfo.aPipe[i];
                ViChn = pstViInfo->stChnInfo.ViChn;
                SAMPLE_COMM_VI_GetChnAttrBySns(pstViInfo->stSnsInfo.enSnsType, &stChnAttr);
                stChnAttr.enDynamicRange = pstViInfo->stChnInfo.enDynamicRange;
                stChnAttr.enVideoFormat = pstViInfo->stChnInfo.enVideoFormat;
                stChnAttr.enPixelFormat = pstViInfo->stChnInfo.enPixFormat;
                stChnAttr.enCompressMode = pstViInfo->stChnInfo.enCompressMode;
                s32Ret = HI_MPI_VI_SetChnAttr(ViPipe, ViChn, &stChnAttr);
                if (s32Ret != HI_SUCCESS) {
                    SAMPLE_PRT("HI_MPI_VI_SetChnAttr failed with %#x!\n", s32Ret);
                    return s32Ret;
                }
            }
        }
    }

    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_StartViChn(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 i;
    HI_BOOL bNeedChn;
    HI_S32 s32Ret = HI_SUCCESS;
    VI_PIPE ViPipe;
    VI_CHN ViChn;
    VI_CHN_ATTR_S stChnAttr;
    VI_VPSS_MODE_E enMastPipeMode;

    for (i = 0; i < WDR_MAX_PIPE_NUM; i++) {
        if ((pstViInfo->stPipeInfo.aPipe[i] >= 0) && (pstViInfo->stPipeInfo.aPipe[i] < VI_MAX_PIPE_NUM)) {
            if (pstViInfo->stDevInfo.enWDRMode == WDR_MODE_NONE) {
                bNeedChn = HI_TRUE;
            } else {
                bNeedChn = (i > 0) ? HI_FALSE : HI_TRUE;
            }
            if (bNeedChn) {
                ViPipe = pstViInfo->stPipeInfo.aPipe[i];
                ViChn = pstViInfo->stChnInfo.ViChn;
                SAMPLE_COMM_VI_GetChnAttrBySns(pstViInfo->stSnsInfo.enSnsType, &stChnAttr);
                stChnAttr.enDynamicRange = pstViInfo->stChnInfo.enDynamicRange;
                stChnAttr.enVideoFormat = pstViInfo->stChnInfo.enVideoFormat;
                stChnAttr.enPixelFormat = pstViInfo->stChnInfo.enPixFormat;
                stChnAttr.enCompressMode = pstViInfo->stChnInfo.enCompressMode;
                s32Ret = HI_MPI_VI_SetChnAttr(ViPipe, ViChn, &stChnAttr);
                if (s32Ret != HI_SUCCESS) {
                    SAMPLE_PRT("HI_MPI_VI_SetChnAttr failed with %#x!\n", s32Ret);
                    return s32Ret;
                }

                enMastPipeMode = pstViInfo->stPipeInfo.enMastPipeMode;
                if ((enMastPipeMode == VI_OFFLINE_VPSS_OFFLINE) || (enMastPipeMode == VI_ONLINE_VPSS_OFFLINE)) {
                    s32Ret = HI_MPI_VI_EnableChn(ViPipe, ViChn);
                    if (s32Ret != HI_SUCCESS) {
                        SAMPLE_PRT("HI_MPI_VI_EnableChn failed with %#x!\n", s32Ret);
                        return s32Ret;
                    }
                }
            }
        }
    }

    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_StopViChn(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 i;
    HI_BOOL bNeedChn;
    HI_S32 s32Ret = HI_SUCCESS;
    VI_PIPE ViPipe;
    VI_CHN ViChn;
    VI_VPSS_MODE_E enMastPipeMode;

    for (i = 0; i < WDR_MAX_PIPE_NUM; i++) {
        if ((pstViInfo->stPipeInfo.aPipe[i] >= 0) && (pstViInfo->stPipeInfo.aPipe[i] < VI_MAX_PIPE_NUM)) {
            if (pstViInfo->stDevInfo.enWDRMode == WDR_MODE_NONE) {
                bNeedChn = HI_TRUE;
            } else {
                bNeedChn = (i > 0) ? HI_FALSE : HI_TRUE;
            }
            if (bNeedChn) {
                enMastPipeMode = pstViInfo->stPipeInfo.enMastPipeMode;
                if ((enMastPipeMode == VI_OFFLINE_VPSS_OFFLINE) || (enMastPipeMode == VI_ONLINE_VPSS_OFFLINE)) {
                    ViPipe = pstViInfo->stPipeInfo.aPipe[i];
                    ViChn = pstViInfo->stChnInfo.ViChn;
                    s32Ret = HI_MPI_VI_DisableChn(ViPipe, ViChn);
                    if (s32Ret != HI_SUCCESS) {
                        SAMPLE_PRT("HI_MPI_VI_DisableChn failed with %#x!\n", s32Ret);
                        return s32Ret;
                    }
                }
            }
        }
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_CreateSingleVi(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 s32Ret;

    if (pstViInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    s32Ret = SAMPLE_COMM_VI_StartDev(pstViInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartDev failed !\n");
        return s32Ret;
    }

    /* we should bind pipe, then create pipe. */
    s32Ret = SAMPLE_COMM_VI_BindPipeDev(pstViInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_BindPipeDev failed !\n");
        goto EXIT1;
    }

    s32Ret = SAMPLE_COMM_VI_StartViPipe(pstViInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartViPipe failed !\n");
        goto EXIT1;
    }

    s32Ret = SAMPLE_COMM_VI_StartViChn(pstViInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartViChn failed !\n");
        goto EXIT2;
    }

    return s32Ret;

EXIT2:
    SAMPLE_COMM_VI_StopViPipe(pstViInfo);
EXIT1:
    SAMPLE_COMM_VI_StopDev(pstViInfo);
    return s32Ret;
}

HI_S32 SAMPLE_COMM_ModeSwitch_VI_CreateSingleVi(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 s32Ret;

    if (pstViInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_VI_StartDev(pstViInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartDev failed !\n");
        return s32Ret;
    }

    /* we should bind pipe,then create pipe */
    s32Ret = SAMPLE_COMM_VI_BindPipeDev(pstViInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_BindPipeDev failed !\n");
        goto EXIT1;
    }

    s32Ret = SAMPLE_COMM_VI_ModeSwitch_StartViPipe(pstViInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_ModeSwitch_StartViPipe failed !\n");
        goto EXIT1;
    }

    s32Ret = SAMPLE_COMM_VI_ModeSwitch_StartViChn(pstViInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_ModeSwitch_StartViChn failed !\n");
        goto EXIT2;
    }

    return HI_SUCCESS;

EXIT2:
    SAMPLE_COMM_VI_StopViPipe(pstViInfo);
EXIT1:
    SAMPLE_COMM_VI_StopDev(pstViInfo);
    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_StartPipe_Chn(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 s32Ret;

    s32Ret = SAMPLE_COMM_VI_ModeSwitch_EnableViPipe(pstViInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_ModeSwitch_EnableViPipe failed !\n");
        goto EXIT1;
    }

    s32Ret = SAMPLE_COMM_VI_StartViChn(pstViInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartViChn failed !\n");
        goto EXIT2;
    }

    return s32Ret;

EXIT2:
    SAMPLE_COMM_VI_StopViPipe(pstViInfo);
EXIT1:
    SAMPLE_COMM_VI_StopDev(pstViInfo);
    return s32Ret;
}

HI_VOID SAMPLE_COMM_VI_DestroySingleVi(SAMPLE_VI_INFO_S *pstViInfo)
{
    SAMPLE_COMM_VI_StopViChn(pstViInfo);

    SAMPLE_COMM_VI_StopViPipe(pstViInfo);

    SAMPLE_COMM_VI_StopDev(pstViInfo);
}

HI_S32 SAMPLE_COMM_VI_CreateVi(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i, j;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            goto EXIT;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        s32Ret = SAMPLE_COMM_VI_CreateSingleVi(pstViInfo);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("SAMPLE_COMM_VI_CreateSingleVi failed !\n");
            goto EXIT;
        }
    }

    return s32Ret;

EXIT:
    for (j = 0; j < i; j++) {
        s32ViNum = pstViConfig->as32WorkingViId[j];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            continue;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        SAMPLE_COMM_VI_DestroySingleVi(pstViInfo);
    }
    return s32Ret;
}

static HI_S32 SAMPLE_COMM_ModeSwitch_VI_CreateVi(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i, j;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            goto EXIT;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        s32Ret = SAMPLE_COMM_ModeSwitch_VI_CreateSingleVi(pstViInfo);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("SAMPLE_COMM_ModeSwitch_VI_CreateSingleVi failed!\n");
            goto EXIT;
        }
    }

    return s32Ret;

EXIT:
    for (j = 0; j < i; j++) {
        s32ViNum = pstViConfig->as32WorkingViId[j];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            continue;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        SAMPLE_COMM_VI_DestroySingleVi(pstViInfo);
    }
    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_DestroyVi(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            continue;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        SAMPLE_COMM_VI_DestroySingleVi(pstViInfo);
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_StartIsp(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 i;
    HI_BOOL bNeedPipe;
    HI_S32 s32Ret = HI_SUCCESS;
    VI_PIPE ViPipe;
    HI_U32 u32SnsId;
    ISP_PUB_ATTR_S stPubAttr;
    VI_PIPE_ATTR_S stPipeAttr;

    if (pstViInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    SAMPLE_COMM_VI_GetPipeAttrBySns(pstViInfo->stSnsInfo.enSnsType, &stPipeAttr);
    if (stPipeAttr.enPipeBypassMode == VI_PIPE_BYPASS_BE) {
        return s32Ret;
    }

    for (i = 0; i < WDR_MAX_PIPE_NUM; i++) {
        if ((pstViInfo->stPipeInfo.aPipe[i] >= 0) && (pstViInfo->stPipeInfo.aPipe[i] < VI_MAX_PIPE_NUM)) {
            SAMPLE_COMM_ISP_GetIspAttrBySns(pstViInfo->stSnsInfo.enSnsType, &stPubAttr);
            stPubAttr.enWDRMode = pstViInfo->stDevInfo.enWDRMode;
            ViPipe = pstViInfo->stPipeInfo.aPipe[i];
            if (pstViInfo->stDevInfo.enWDRMode == WDR_MODE_NONE) {
                bNeedPipe = HI_TRUE;
            } else {
                bNeedPipe = (i > 0) ? HI_FALSE : HI_TRUE;
            }
            if (bNeedPipe != HI_TRUE) {
                continue;
            }

            u32SnsId = pstViInfo->stSnsInfo.s32SnsId;
            s32Ret = SAMPLE_COMM_ISP_Sensor_Regiter_callback(ViPipe, u32SnsId);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("register sensor %d to ISP %d failed\n", u32SnsId, ViPipe);
                SAMPLE_COMM_ISP_Stop(ViPipe);
                return s32Ret;
            }

            if (((pstViInfo->stSnapInfo.bDoublePipe) && (pstViInfo->stSnapInfo.SnapPipe == ViPipe)) ||
                ((pstViInfo->stPipeInfo.bMultiPipe) && (i > 0))) {
                s32Ret = SAMPLE_COMM_ISP_BindSns(ViPipe, u32SnsId, pstViInfo->stSnsInfo.enSnsType, -1);
                if (s32Ret != HI_SUCCESS) {
                    SAMPLE_PRT("register sensor %d bus id %d failed\n", u32SnsId, pstViInfo->stSnsInfo.s32BusId);
                    SAMPLE_COMM_ISP_Stop(ViPipe);
                    return s32Ret;
                }
            } else {
                s32Ret = SAMPLE_COMM_ISP_BindSns(ViPipe, u32SnsId, pstViInfo->stSnsInfo.enSnsType,
                    pstViInfo->stSnsInfo.s32BusId);
                if (s32Ret != HI_SUCCESS) {
                    SAMPLE_PRT("register sensor %d bus id %d failed\n", u32SnsId, pstViInfo->stSnsInfo.s32BusId);
                    SAMPLE_COMM_ISP_Stop(ViPipe);
                    return s32Ret;
                }
            }

            s32Ret = SAMPLE_COMM_ISP_Aelib_Callback(ViPipe);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("SAMPLE_COMM_ISP_Aelib_Callback failed\n");
                SAMPLE_COMM_ISP_Stop(ViPipe);
                return s32Ret;
            }

            s32Ret = SAMPLE_COMM_ISP_Awblib_Callback(ViPipe);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("SAMPLE_COMM_ISP_Awblib_Callback failed\n");
                SAMPLE_COMM_ISP_Stop(ViPipe);
                return s32Ret;
            }

            s32Ret = HI_MPI_ISP_MemInit(ViPipe);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("Init Ext memory failed with %#x!\n", s32Ret);
                SAMPLE_COMM_ISP_Stop(ViPipe);
                return s32Ret;
            }

            s32Ret = HI_MPI_ISP_SetPubAttr(ViPipe, &stPubAttr);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("SetPubAttr failed with %#x!\n", s32Ret);
                SAMPLE_COMM_ISP_Stop(ViPipe);
                return s32Ret;
            }

            s32Ret = HI_MPI_ISP_Init(ViPipe);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("ISP Init failed with %#x!\n", s32Ret);
                SAMPLE_COMM_ISP_Stop(ViPipe);
                return s32Ret;
            }

            s32Ret = SAMPLE_COMM_ISP_Run(ViPipe);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("ISP Run failed with %#x!\n", s32Ret);
                SAMPLE_COMM_ISP_Stop(ViPipe);
                return s32Ret;
            }
        }
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_StopIsp(SAMPLE_VI_INFO_S *pstViInfo)
{
    HI_S32 i;
    HI_BOOL bNeedPipe;
    VI_PIPE ViPipe;

    if (pstViInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    for (i = 0; i < WDR_MAX_PIPE_NUM; i++) {
        if ((pstViInfo->stPipeInfo.aPipe[i] >= 0) && (pstViInfo->stPipeInfo.aPipe[i] < VI_MAX_PIPE_NUM)) {
            if (pstViInfo->stDevInfo.enWDRMode == WDR_MODE_NONE) {
                bNeedPipe = HI_TRUE;
            } else {
                bNeedPipe = (i > 0) ? HI_FALSE : HI_TRUE;
            }
            if (bNeedPipe != HI_TRUE) {
                continue;
            }
            ViPipe = pstViInfo->stPipeInfo.aPipe[i];
            SAMPLE_COMM_ISP_Stop(ViPipe);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 SAMPLE_COMM_VI_CreateIsp(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            return s32Ret;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        s32Ret = SAMPLE_COMM_VI_StartIsp(pstViInfo);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("SAMPLE_COMM_VI_StartIsp failed !\n");
            return s32Ret;
        }
    }

    return s32Ret;
}

static HI_S32 SAMPLE_COMM_VI_DestroyIsp(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            continue;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        s32Ret = SAMPLE_COMM_VI_StopIsp(pstViInfo);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("SAMPLE_COMM_VI_StopIsp failed !\n");
            return s32Ret;
        }
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_StartVi(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("%s: null ptr\n", __FUNCTION__);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_StartMIPI(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartMIPI failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_SetParam(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_SetParam failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_CreateVi(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_CreateVi failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_CreateIsp(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_COMM_VI_DestroyVi(pstViConfig);
        SAMPLE_PRT("SAMPLE_COMM_VI_CreateIsp failed!\n");
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_StopVi(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 s32Ret;

    s32Ret = SAMPLE_COMM_VI_DestroyIsp(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_DestroyIsp failed !\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_DestroyVi(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_DestroyVi failed !\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_StopMIPI(pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_StopMIPI failed !\n");
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_SwitchISPMode(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i, j;
    HI_S32 s32ViNum;
    HI_S32 s32Ret = HI_FAILURE;
    HI_BOOL bNeedPipe;
    HI_BOOL             bSwitchWDR[VI_MAX_PIPE_NUM] = { HI_FALSE };
    VI_PIPE ViPipe;
    ISP_PUB_ATTR_S stPubAttr;
    ISP_PUB_ATTR_S stPrePubAttr;
    SAMPLE_VI_INFO_S *pstViInfo = HI_NULL;
    ISP_INNER_STATE_INFO_S stInnerStateInfo = { 0 };
    HI_BOOL bSwitchFinish;

    if (pstViConfig == HI_NULL) {
        SAMPLE_PRT("null ptr\n");
        return s32Ret;
    }

    if (pstViConfig->s32WorkingViNum > VI_MAX_DEV_NUM) {
        SAMPLE_PRT("Invalid workingViNum, %d\n", pstViConfig->s32WorkingViNum);
        return s32Ret;
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        if (s32ViNum >= VI_MAX_DEV_NUM) {
            SAMPLE_PRT("Invalid s32ViNum:%d\n", s32ViNum);
            return s32Ret;
        }

        pstViInfo = &pstViConfig->astViInfo[s32ViNum];
        for (j = 0; j < WDR_MAX_PIPE_NUM; j++) {
            ViPipe = pstViInfo->stPipeInfo.aPipe[j];
            if ((ViPipe >= 0) && (ViPipe < VI_MAX_PIPE_NUM)) {
                SAMPLE_COMM_ISP_GetIspAttrBySns(pstViInfo->stSnsInfo.enSnsType, &stPubAttr);
                stPubAttr.enWDRMode = pstViInfo->stDevInfo.enWDRMode;
                SAMPLE_PRT("SAMPLE_COMM_VI_SwitchISPMode enWDRMode is %d!\n", stPubAttr.enWDRMode);
                if (pstViInfo->stDevInfo.enWDRMode == WDR_MODE_NONE) {
                    bNeedPipe = HI_TRUE;
                } else {
                    bNeedPipe = (j > 0) ? HI_FALSE : HI_TRUE;
                }
                if (bNeedPipe != HI_TRUE) {
                    continue;
                }

                s32Ret = HI_MPI_ISP_GetPubAttr(ViPipe, &stPrePubAttr);
                if (s32Ret != HI_SUCCESS) {
                    SAMPLE_PRT("GetPubAttr failed with %#x!\n", s32Ret);
                    SAMPLE_COMM_ISP_Stop(ViPipe);
                    return s32Ret;
                }

                s32Ret = HI_MPI_ISP_SetPubAttr(ViPipe, &stPubAttr);
                if (s32Ret != HI_SUCCESS) {
                    SAMPLE_PRT("SetPubAttr failed with %#x!\n", s32Ret);
                    SAMPLE_COMM_ISP_Stop(ViPipe);
                    return s32Ret;
                }

                if (stPrePubAttr.enWDRMode != stPubAttr.enWDRMode) {
                    bSwitchWDR[ViPipe] = HI_TRUE;
                }
            }
        }
    }

    while (1) {
        bSwitchFinish = HI_TRUE;
        for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
            s32ViNum = pstViConfig->as32WorkingViId[i];
            pstViInfo = &pstViConfig->astViInfo[s32ViNum];
            ViPipe = pstViInfo->stPipeInfo.aPipe[0];
            if ((ViPipe >= 0) && (ViPipe < VI_MAX_PIPE_NUM)) {
                HI_MPI_ISP_QueryInnerStateInfo(ViPipe, &stInnerStateInfo);
                if (bSwitchWDR[ViPipe] == HI_TRUE) {
                    bSwitchFinish = stInnerStateInfo.bWDRSwitchFinish;
                } else {
                    bSwitchFinish = stInnerStateInfo.bResSwitchFinish;
                }
                if (bSwitchFinish == HI_FALSE) {
                    break;
                }
            }
        }

        if (bSwitchFinish == HI_TRUE) {
            SAMPLE_PRT("Switch finish!\n");
            break;
        } else {
            usleep(1000); /* sleep 1000us */
        }
    }

    for (i = 0; i < pstViConfig->s32WorkingViNum; i++) {
        s32ViNum = pstViConfig->as32WorkingViId[i];
        pstViInfo = &pstViConfig->astViInfo[s32ViNum];

        SAMPLE_COMM_VI_StartPipe_Chn(pstViInfo);
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_SwitchMode_StopVI(SAMPLE_VI_CONFIG_S *pstViConfigSrc)
{
    HI_S32 s32Ret;

    s32Ret = SAMPLE_COMM_VI_DestroyVi(pstViConfigSrc);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_DestroyVi failed !\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_StopMIPI(pstViConfigSrc);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_StopMIPI failed !\n");
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_SwitchMode(SAMPLE_VI_CONFIG_S *pstViConfigDes)
{
    HI_S32 s32Ret;

    s32Ret = SAMPLE_COMM_VI_StartMIPI(pstViConfigDes);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartMIPI failed!\n");
        return s32Ret;
    }

    /* create vi without enable chn and enable pipe. */
    s32Ret = SAMPLE_COMM_ModeSwitch_VI_CreateVi(pstViConfigDes);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_ModeSwitch_VI_CreateVi failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_SwitchISPMode(pstViConfigDes);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_COMM_VI_SwitchISPMode failed!\n");
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_GetSizeBySensor(SAMPLE_SNS_TYPE_E enMode, PIC_SIZE_E *penSize)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (penSize == HI_NULL) {
        return s32Ret;
    }

    switch (enMode) {
        case SONY_IMX335_MIPI_5M_30FPS_12BIT:
        case SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1:
            *penSize = PIC_2592x1944;
            break;

        case SONY_IMX335_MIPI_4M_30FPS_12BIT:
        case SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1:
            *penSize = PIC_2592x1536;
            break;

        default:
            *penSize = PIC_2592x1536;
            break;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_GetFrameRateBySensor(SAMPLE_SNS_TYPE_E enMode, HI_U32 *pu32FrameRate)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (pu32FrameRate == HI_NULL) {
        return s32Ret;
    }

    switch (enMode) {
        case SONY_IMX335_MIPI_5M_30FPS_12BIT:
        case SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1:
        case SONY_IMX335_MIPI_4M_30FPS_12BIT:
        case SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1:
            *pu32FrameRate = FPS_30;
            break;

        default:
            *pu32FrameRate = FPS_30;
            break;
    }

    return HI_SUCCESS;
}

HI_VOID SAMPLE_COMM_VI_GetSensorInfo(SAMPLE_VI_CONFIG_S *pstViConfig)
{
    HI_S32 i;

    for (i = 0; i < VI_MAX_DEV_NUM; i++) {
        pstViConfig->astViInfo[i].stSnsInfo.s32SnsId = i;
        pstViConfig->astViInfo[i].stSnsInfo.s32BusId = i;
        pstViConfig->astViInfo[i].stSnsInfo.MipiDev = i;
        (hi_void)
            memset_s(&pstViConfig->astViInfo[i].stSnapInfo, sizeof(SAMPLE_SNAP_INFO_S), 0, sizeof(SAMPLE_SNAP_INFO_S));
        pstViConfig->astViInfo[i].stPipeInfo.bMultiPipe = HI_FALSE;
        pstViConfig->astViInfo[i].stPipeInfo.bVcNumCfged = HI_FALSE;
    }

    pstViConfig->astViInfo[0].stSnsInfo.enSnsType = SENSOR0_TYPE;
    pstViConfig->astViInfo[1].stSnsInfo.enSnsType = SENSOR1_TYPE;
}

combo_dev_t SAMPLE_COMM_VI_GetComboDevBySensor(SAMPLE_SNS_TYPE_E enMode, HI_S32 s32SnsIdx)
{
    combo_dev_t dev = 0;

    switch (enMode) {
        case SONY_IMX335_MIPI_5M_30FPS_12BIT:
        case SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1:
        case SONY_IMX335_MIPI_4M_30FPS_12BIT:
        case SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1:
            dev = s32SnsIdx; /* sensor_id {0, 1} equal to dev_id {0, 1} when LANE_DIVIDE_MODE_1 */
            break;

        default:
            break;
    }

    return dev;
}

static HI_CHAR *PRINT_FPNTYPE(ISP_FPN_TYPE_E enFpnType)
{
    if (ISP_FPN_TYPE_FRAME == enFpnType) {
        return "Frame";
    } else if (ISP_FPN_TYPE_LINE == enFpnType) {
        return "Line";
    } else {
        return "NA";
    }
}

static HI_U32 SAMPLE_VI_PixelFormat2BitWidth(PIXEL_FORMAT_E enPixelFormat)
{
    HI_U32 bitWidth;

    switch (enPixelFormat) {
        case PIXEL_FORMAT_RGB_BAYER_8BPP:
            bitWidth = 8; /* RAW_8PP is 8bit width */
            break;

        case PIXEL_FORMAT_RGB_BAYER_10BPP:
            bitWidth = 10; /* RAW_10PP is 10bit width */
            break;

        case PIXEL_FORMAT_RGB_BAYER_12BPP:
            bitWidth = 12; /* RAW_12PP is 12bit width */
            break;

        case PIXEL_FORMAT_RGB_BAYER_14BPP:
            bitWidth = 14; /* RAW_14PP is 14bit width */
            break;

        case PIXEL_FORMAT_RGB_BAYER_16BPP:
            bitWidth = 16; /* RAW_16PP is 16bit width */
            break;

        default:
            bitWidth = 8; /* default 8bit width */
            break;
    }

    return bitWidth;
}

static HI_U32 SAMPLE_VI_GetValidAlign(HI_U32 u32ByteAlign)
{
    if (u32ByteAlign == 0) {
        return DEFAULT_ALIGN;
    } else if (u32ByteAlign > MAX_ALIGN) {
        return MAX_ALIGN;
    } else {
        return (HI_ALIGN_UP(u32ByteAlign, DEFAULT_ALIGN));
    }
}

static HI_U32 SAMPLE_VI_GetRawStride(PIXEL_FORMAT_E enPixelFormat, HI_U32 u32Width, HI_U32 u32ByteAlign)
{
    HI_U32 u32Stride;
    HI_U32 u32BitWidth;

    u32BitWidth = SAMPLE_VI_PixelFormat2BitWidth(enPixelFormat);

    u32ByteAlign = SAMPLE_VI_GetValidAlign(u32ByteAlign);

    u32Stride = HI_ALIGN_UP(HI_ALIGN_UP(u32Width * u32BitWidth, 8) / 8, u32ByteAlign); /* 8 align */

    return u32Stride;
}

static HI_S32 SAMPLE_VI_GetFrameBlkInfo(SAMPLE_VI_FRAME_CONFIG_S *pstFrmCfg, HI_S32 s32FrmCnt,
    SAMPLE_VI_FRAME_INFO_S *pastViFrameInfo)
{
    HI_S32 i;
    HI_U32 u32Stride;
    HI_U32 u32LStride;
    HI_U32 u32CStride;
    HI_U32 u32LumaSize = 0;
    HI_U32 u32ChrmSize = 0;
    HI_U32 u32Size;
    HI_U64 u64PhyAddr;
    HI_U8 *pVirAddr = HI_NULL;
    VB_POOL u32PoolId;
    VB_BLK VbBlk;
    PIXEL_FORMAT_E enPixelFormat;
    VB_POOL_CONFIG_S stVbPoolCfg;

    enPixelFormat = pstFrmCfg->enPixelFormat;
    if (enPixelFormat == PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
        u32Stride = HI_ALIGN_UP((pstFrmCfg->u32Width * 8 + 8 - 1) / 8, DEFAULT_ALIGN); /* 8 align */
        u32Size = u32Stride * pstFrmCfg->u32Height * 2; /* YUV422 size is 2 * stride * h */
        u32LumaSize = u32Stride * pstFrmCfg->u32Height;
        u32ChrmSize = u32Stride * pstFrmCfg->u32Height / 2; /* one chroma size is (size / 2 / 2). */
    } else if (enPixelFormat == PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
        u32Stride = HI_ALIGN_UP((pstFrmCfg->u32Width * 8 + 8 - 1) / 8, DEFAULT_ALIGN); /* 8 align */
        u32Size = u32Stride * pstFrmCfg->u32Height * 3 / 2; /* YUV420 size is stride * h * 3 / 2 */
        u32LumaSize = u32Stride * pstFrmCfg->u32Height;
        u32ChrmSize = u32Stride * pstFrmCfg->u32Height / 4; /* one chroma size is (stride * h / 4). */
    } else if (enPixelFormat == PIXEL_FORMAT_YUV_400) {
        u32Stride = HI_ALIGN_UP((pstFrmCfg->u32Width * 8 + 8 - 1) / 8, DEFAULT_ALIGN); /* 8 align */
        u32Size = u32Stride * pstFrmCfg->u32Height;
        u32LumaSize = u32Size;
        u32ChrmSize = 0;
    } else { /* RAW8 ~ RAW16 */
        u32Stride = SAMPLE_VI_GetRawStride(enPixelFormat, pstFrmCfg->u32Width, pstFrmCfg->u32ByteAlign);
        u32Size = u32Stride * pstFrmCfg->u32Height;
    }
    u32LStride = u32Stride;
    u32CStride = u32Stride;

    (hi_void)memset_s(&stVbPoolCfg, sizeof(VB_POOL_CONFIG_S), 0, sizeof(VB_POOL_CONFIG_S));
    stVbPoolCfg.u64BlkSize = u32Size;
    stVbPoolCfg.u32BlkCnt = s32FrmCnt;
    stVbPoolCfg.enRemapMode = VB_REMAP_MODE_NONE;
    u32PoolId = HI_MPI_VB_CreatePool(&stVbPoolCfg);
    if (u32PoolId == VB_INVALID_POOLID) {
        SAMPLE_PRT("HI_MPI_VB_CreatePool failed!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < s32FrmCnt; i++) {
        VbBlk = HI_MPI_VB_GetBlock(u32PoolId, u32Size, HI_NULL);
        if (VbBlk == VB_INVALID_HANDLE) {
            SAMPLE_PRT("HI_MPI_VB_GetBlock err! size:%d\n", u32Size);
            return HI_FAILURE;
        }

        u64PhyAddr = HI_MPI_VB_Handle2PhysAddr(VbBlk);
        if (u64PhyAddr == 0) {
            SAMPLE_PRT("HI_MPI_VB_Handle2PhysAddr err!\n");
            return HI_FAILURE;
        }

        pVirAddr = (HI_U8 *)HI_MPI_SYS_Mmap(u64PhyAddr, u32Size);
        if (pVirAddr == HI_NULL) {
            SAMPLE_PRT("HI_MPI_SYS_Mmap err!\n");
            return HI_FAILURE;
        }

        /* PhyAddr[0] -- luma addr, PhyAddr[1] -- chroma_U addr, PhyAddr[2] -- chroma_V addr */
        pastViFrameInfo[i].stVideoFrameInfo.u32PoolId = u32PoolId;
        pastViFrameInfo[i].stVideoFrameInfo.enModId = HI_ID_VI;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64PhyAddr[0] = u64PhyAddr;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64PhyAddr[1] =
            pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64PhyAddr[0] + u32LumaSize;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64PhyAddr[2] = /* PhyAddr[2] -- chroma_V phy_addr */
            pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64PhyAddr[1] + u32ChrmSize;

        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64VirAddr[0] = (HI_U64)(HI_UINTPTR_T)pVirAddr;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64VirAddr[1] =
            pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64VirAddr[0] + u32LumaSize;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64VirAddr[2] = /* VirAddr[2] -- chroma_V vir_addr */
            pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64VirAddr[1] + u32ChrmSize;

        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u32Stride[0] = u32LStride;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u32Stride[1] = u32CStride;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u32Stride[2] = u32CStride; /* Stride[2] -- chroma_V stride */
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u32Width = pstFrmCfg->u32Width;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u32Height = pstFrmCfg->u32Height;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.enPixelFormat = pstFrmCfg->enPixelFormat;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.enVideoFormat = pstFrmCfg->enVideoFormat;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.enCompressMode = pstFrmCfg->enCompressMode;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.enDynamicRange = pstFrmCfg->enDynamicRange;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.enField = VIDEO_FIELD_FRAME;
        pastViFrameInfo[i].stVideoFrameInfo.stVFrame.enColorGamut = COLOR_GAMUT_BT709;
        pastViFrameInfo[i].VbBlk = VbBlk;
        pastViFrameInfo[i].u32Size = u32Size;
    }

    return HI_SUCCESS;
}

static HI_VOID SAMPLE_VI_COMM_ReleaseFrameBlkInfo(HI_S32 s32FrmCnt, SAMPLE_VI_FRAME_INFO_S *pastViFrameInfo)
{
    HI_S32 s32Ret;
    HI_S32 i;
    VB_POOL u32PoolId;
    VB_BLK VbBlk;
    HI_U32 u32Size;

    if (pastViFrameInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return;
    }

    for (i = 0; i < s32FrmCnt; i++) {
        VbBlk = pastViFrameInfo[i].VbBlk;
        s32Ret = HI_MPI_VB_ReleaseBlock(VbBlk);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_VB_ReleaseBlock block 0x%x failure\n", VbBlk);
        }

        u32Size = pastViFrameInfo[i].u32Size;
        s32Ret = HI_MPI_SYS_Munmap((HI_VOID *)(HI_UINTPTR_T)pastViFrameInfo[i].stVideoFrameInfo.stVFrame.u64VirAddr[0],
            u32Size);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("HI_MPI_SYS_Munmap failure!\n");
        }
    }

    u32PoolId = pastViFrameInfo[0].stVideoFrameInfo.u32PoolId;
    HI_MPI_VB_DestroyPool(u32PoolId);

    return;
}

static HI_VOID SAMPLE_COMM_VI_SaveFpnFile(ISP_FPN_FRAME_INFO_S *pVBuf, FILE *pfd)
{
    HI_U8 *pU8VBufVirt_Y = HI_NULL;
    HI_U32 u32FpnHeight;
    HI_U32 i;

    u32FpnHeight = pVBuf->stFpnFrame.stVFrame.u32Height;
    pU8VBufVirt_Y = (HI_U8 *)(HI_UINTPTR_T)pVBuf->stFpnFrame.stVFrame.u64VirAddr[0];

    /* save Y ---------------------------------------------------------------- */
    (HI_VOID)fprintf(stderr, "FPN: saving......Raw data......u32Stride[0]: %d, width: %d, height: %d\n",
        pVBuf->stFpnFrame.stVFrame.u32Stride[0], pVBuf->stFpnFrame.stVFrame.u32Width, u32FpnHeight);
    (HI_VOID)fprintf(stderr, "phy Addr: 0x%llx\n", pVBuf->stFpnFrame.stVFrame.u64PhyAddr[0]);
    (HI_VOID)fprintf(stderr, "Please wait a moment to save data.\n");
    (HI_VOID)fflush(stderr);

    (HI_VOID)fwrite(pU8VBufVirt_Y, pVBuf->u32FrmSize, 1, pfd);

    /* save offset */
    for (i = 0; i < VI_MAX_SPLIT_NODE_NUM; i++) {
        (HI_VOID)fwrite(&pVBuf->u32Offset[i], 4, 1, pfd); /* write 4 bytes data(offset[i]) to pfd. */
    }

    /* save compress mode */
    (HI_VOID)fwrite(&pVBuf->stFpnFrame.stVFrame.enCompressMode, 4, 1, pfd); /* write 4 bytes data(cmpMode) to pfd. */

    /* save fpn frame size */
    (HI_VOID)fwrite(&pVBuf->u32FrmSize, 4, 1, pfd); /* write 4 bytes data(frmSize) to pfd. */

    /* save ISO */
    (HI_VOID)fwrite(&pVBuf->u32Iso, 4, 1, pfd); /* write 4 bytes data(u32Iso) to pfd. */
    (HI_VOID)fflush(pfd);
}

static HI_VOID SAMPLE_COMM_VI_ReadFpnFile(ISP_FPN_FRAME_INFO_S *pstFpnFrmInfo, FILE *pfd)
{
    HI_U32 i;
    VIDEO_FRAME_INFO_S *pstFpnFrame = HI_NULL;

    SAMPLE_PRT("u32FrmSize: %d!!\n", pstFpnFrmInfo->u32FrmSize);

    pstFpnFrame = &pstFpnFrmInfo->stFpnFrame;
    (HI_VOID)fread((HI_U8 *)(HI_UINTPTR_T)pstFpnFrame->stVFrame.u64VirAddr[0], pstFpnFrmInfo->u32FrmSize, 1, pfd);
    for (i = 0; i < VI_MAX_SPLIT_NODE_NUM; i++) {
        (HI_VOID)fread((HI_U8 *)&pstFpnFrmInfo->u32Offset[i], 4, 1, pfd); /* read 4 bytes once. */
    }
    (HI_VOID)fread((HI_U8 *)&pstFpnFrame->stVFrame.enCompressMode, 4, 1, pfd); /* read 4 bytes once. */
    (HI_VOID)fread((HI_U8 *)&pstFpnFrmInfo->u32FrmSize, 4, 1, pfd); /* read 4 bytes once. */
    (HI_VOID)fread((HI_U8 *)&pstFpnFrmInfo->u32Iso, 4, 1, pfd); /* read 4 bytes once. */
}

HI_S32 SAMPLE_COMM_VI_FpnCalibrateConfig(VI_PIPE ViPipe, SAMPLE_VI_FPN_CALIBRATE_INFO_S *pstViFpnCalibrateInfo)
{
    HI_S32 s32Ret;
    ISP_PUB_ATTR_S stPubAttr;
    HI_U32 i;
    FILE *pfd = HI_NULL;
    VI_CHN ViChn = 0;
    HI_CHAR FpnFileName[FILE_NAME_LEN];
    SAMPLE_VI_FRAME_CONFIG_S stFrmCfg;
    SAMPLE_VI_FRAME_INFO_S stViFrameInfo;
    ISP_FPN_CALIBRATE_ATTR_S stCalibrateAttr;

    if (pstViFpnCalibrateInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_ISP_GetPubAttr(ViPipe, &stPubAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get isp pub attr failed!\n");
        return s32Ret;
    }

    stFrmCfg.u32Width = stPubAttr.stWndRect.u32Width;
    if (pstViFpnCalibrateInfo->enFpnType == ISP_FPN_TYPE_LINE) {
        stFrmCfg.u32Height = 1;
    } else {
        stFrmCfg.u32Height = stPubAttr.stWndRect.u32Height;
    }

    stFrmCfg.u32ByteAlign = 0;
    stFrmCfg.enPixelFormat = pstViFpnCalibrateInfo->enPixelFormat;
    stFrmCfg.enCompressMode = pstViFpnCalibrateInfo->enCompressMode;
    stFrmCfg.enVideoFormat = VIDEO_FORMAT_LINEAR;
    stFrmCfg.enDynamicRange = DYNAMIC_RANGE_SDR8;
    s32Ret = SAMPLE_VI_GetFrameBlkInfo(&stFrmCfg, 1, &stViFrameInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("SAMPLE_VI_GetFrameBlkInfo failed!\n");
        return s32Ret;
    }

    printf("Pipe %d Width=%d, Height=%d.\n", ViPipe, stFrmCfg.u32Width, stFrmCfg.u32Height);
    printf("Please turn off camera aperture to start calibrate!\n");
    printf("Please hit Enter key, start calibrate!\n");
    (hi_void)getchar();

    s32Ret = HI_MPI_VI_DisableChn(ViPipe, ViChn);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("disable vi chn %d failed!", ViChn);
        SAMPLE_VI_COMM_ReleaseFrameBlkInfo(1, &stViFrameInfo);
        return s32Ret;
    }

    stCalibrateAttr.enFpnType = pstViFpnCalibrateInfo->enFpnType;
    stCalibrateAttr.u32FrameNum = pstViFpnCalibrateInfo->u32FrameNum;
    stCalibrateAttr.u32Threshold = pstViFpnCalibrateInfo->u32Threshold;
    stCalibrateAttr.stFpnCaliFrame.u32Iso = 0;
    for (i = 0; i < VI_MAX_SPLIT_NODE_NUM; i++) {
        stCalibrateAttr.stFpnCaliFrame.u32Offset[i] = 0;
    }
    stCalibrateAttr.stFpnCaliFrame.u32FrmSize = stViFrameInfo.u32Size;
    (hi_void)memcpy_s(&stCalibrateAttr.stFpnCaliFrame.stFpnFrame, sizeof(VIDEO_FRAME_INFO_S),
        &stViFrameInfo.stVideoFrameInfo, sizeof(VIDEO_FRAME_INFO_S));

    s32Ret = HI_MPI_ISP_FPNCalibrate(ViPipe, &stCalibrateAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_ISP_FPNCalibrate err: 0x%x\n", s32Ret);
        HI_MPI_VI_EnableChn(ViPipe, ViChn);
        SAMPLE_VI_COMM_ReleaseFrameBlkInfo(1, &stViFrameInfo);
        return s32Ret;
    }

    /* save FPN raw data */
    s32Ret = snprintf_s(FpnFileName, FILE_NAME_LEN, FILE_NAME_LEN - 1, VI_DATA_PATH "/FPN_%s_w%d_h%d_%dbit.raw",
        PRINT_FPNTYPE(stCalibrateAttr.enFpnType), stPubAttr.stWndRect.u32Width, stPubAttr.stWndRect.u32Height,
        SAMPLE_VI_PixelFormat2BitWidth(stViFrameInfo.stVideoFrameInfo.stVFrame.enPixelFormat));
    if (s32Ret < 0) {
        SAMPLE_PRT("snprintf_s FpnFileName fail.\n");
        HI_MPI_VI_EnableChn(ViPipe, ViChn);
        SAMPLE_VI_COMM_ReleaseFrameBlkInfo(1, &stViFrameInfo);
        return s32Ret;
    }

    printf("\nafter calibrate ");
    for (i = 0; i < VI_MAX_SPLIT_NODE_NUM; i++) {
        printf("offset[%d] = 0x%x, ", i, stCalibrateAttr.stFpnCaliFrame.u32Offset[i]);
    }
    printf("u32FrmSize = %d\n", stCalibrateAttr.stFpnCaliFrame.u32FrmSize);
    printf("ISO = %d\n", stCalibrateAttr.stFpnCaliFrame.u32Iso);
    printf("save dark frame file: %s!\n", FpnFileName);

    pfd = fopen(FpnFileName, "wb");
    if (pfd == HI_NULL) {
        SAMPLE_PRT("open file %s err!\n", FpnFileName);
        HI_MPI_VI_EnableChn(ViPipe, ViChn);
        SAMPLE_VI_COMM_ReleaseFrameBlkInfo(1, &stViFrameInfo);
        return HI_FAILURE;
    }

    SAMPLE_COMM_VI_SaveFpnFile(&stCalibrateAttr.stFpnCaliFrame, pfd);

    (HI_VOID)fclose(pfd);

    s32Ret = HI_MPI_VI_EnableChn(ViPipe, ViChn);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("enable vi chn %d failed. ret 0x%x!", ViChn, s32Ret);
    }

    SAMPLE_VI_COMM_ReleaseFrameBlkInfo(1, &stViFrameInfo);
    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_FpnCorrectionConfig(VI_PIPE ViPipe, SAMPLE_VI_FPN_CORRECTION_INFO_S *pstViFpnCorrectionInfo)
{
    HI_S32 s32Ret;
    SAMPLE_VI_FRAME_CONFIG_S stFrmCfg;
    SAMPLE_VI_FRAME_INFO_S *pstViFrameInfo = HI_NULL;
    ISP_FPN_ATTR_S stFPNAttr;
    ISP_PUB_ATTR_S stPubAttr;
    FILE *pfd = HI_NULL;
    HI_U32 i;
    HI_CHAR FpnFileName[FILE_NAME_LEN];
    HI_CHAR *RealPath = HI_NULL;

    s32Ret = HI_MPI_ISP_GetPubAttr(ViPipe, &stPubAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get isp pub attr failed!\n");
        return s32Ret;
    }

    if (pstViFpnCorrectionInfo == NULL) {
        SAMPLE_PRT("pstViFpnCorrectionInfo is null!\n");
        return HI_FAILURE;
    }
    if (pstViFpnCorrectionInfo->enFpnType == ISP_FPN_TYPE_LINE) {
        stFrmCfg.u32Height = 1;
    } else {
        stFrmCfg.u32Height = stPubAttr.stWndRect.u32Height;
    }
    stFrmCfg.u32Width = stPubAttr.stWndRect.u32Width;
    stFrmCfg.u32ByteAlign = 0;
    stFrmCfg.enPixelFormat = pstViFpnCorrectionInfo->enPixelFormat;
    stFrmCfg.enCompressMode = pstViFpnCorrectionInfo->enCompressMode;
    stFrmCfg.enVideoFormat = VIDEO_FORMAT_LINEAR;
    stFrmCfg.enDynamicRange = DYNAMIC_RANGE_SDR8;
    pstViFrameInfo = &pstViFpnCorrectionInfo->stViFrameInfo;
    s32Ret = SAMPLE_VI_GetFrameBlkInfo(&stFrmCfg, 1, pstViFrameInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("VI_MST_COMM_GetFrameBlkInfo failed!\n");
        return s32Ret;
    }

    stFPNAttr.stFpnFrmInfo.u32FrmSize = pstViFrameInfo->u32Size;
    (hi_void)memcpy_s(&stFPNAttr.stFpnFrmInfo.stFpnFrame, sizeof(VIDEO_FRAME_INFO_S), &pstViFrameInfo->stVideoFrameInfo,
        sizeof(VIDEO_FRAME_INFO_S));

    /* save FPN raw data */
    s32Ret = snprintf_s(FpnFileName, FILE_NAME_LEN, FILE_NAME_LEN - 1, VI_DATA_PATH "/FPN_%s_w%d_h%d_%dbit.raw",
        PRINT_FPNTYPE(pstViFpnCorrectionInfo->enFpnType), stPubAttr.stWndRect.u32Width, stPubAttr.stWndRect.u32Height,
        SAMPLE_VI_PixelFormat2BitWidth(pstViFrameInfo->stVideoFrameInfo.stVFrame.enPixelFormat));
    if (s32Ret < 0) {
        SAMPLE_PRT("snprintf_s FpnFileName fail.\n");
        goto EXIT0;
    }

    RealPath = realpath(FpnFileName, HI_NULL);
    if (RealPath == HI_NULL) {
        SAMPLE_PRT("file %s realpath error!\n", FpnFileName);
        s32Ret = HI_FAILURE;
        goto EXIT0;
    }

    SAMPLE_PRT("====to open Fpn raw file: %s. \n", RealPath);
    pfd = fopen(RealPath, "rb");
    if (pfd == HI_NULL) {
        SAMPLE_PRT("open file %s err!\n", RealPath);
        s32Ret = HI_FAILURE;
        goto EXIT0;
    }

    SAMPLE_COMM_VI_ReadFpnFile(&stFPNAttr.stFpnFrmInfo, pfd);

    (HI_VOID)fclose(pfd);

    stFPNAttr.bEnable = HI_TRUE;
    stFPNAttr.enOpType = pstViFpnCorrectionInfo->enOpType;
    stFPNAttr.enFpnType = pstViFpnCorrectionInfo->enFpnType;
    stFPNAttr.stManual.u32Strength = pstViFpnCorrectionInfo->u32Strength;

    for (i = 0; i < VI_MAX_SPLIT_NODE_NUM; i++) {
        printf("offset[%d] = 0x%x; ", i, stFPNAttr.stFpnFrmInfo.u32Offset[i]);
    }
    printf("\n");
    printf("u32FrmSize = %d.\n", stFPNAttr.stFpnFrmInfo.u32FrmSize);
    printf("ISO = %d.\n", stFPNAttr.stFpnFrmInfo.u32Iso);
    printf("\nstFPNAttr: u32Width = %d, u32Height = %d\n", stFPNAttr.stFpnFrmInfo.stFpnFrame.stVFrame.u32Width,
        stFPNAttr.stFpnFrmInfo.stFpnFrame.stVFrame.u32Height);
    printf("FrmSize: %d, stride: %d, compress mode: %d\n", stFPNAttr.stFpnFrmInfo.u32FrmSize,
        stFPNAttr.stFpnFrmInfo.stFpnFrame.stVFrame.u32Stride[0],
        stFPNAttr.stFpnFrmInfo.stFpnFrame.stVFrame.enCompressMode);

    s32Ret = HI_MPI_ISP_SetFPNAttr(ViPipe, &stFPNAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("fpn correction fail 0x%x\n", s32Ret);
        goto EXIT0;
    }

    free(RealPath);
    RealPath = HI_NULL;
    return HI_SUCCESS;

EXIT0:
    /* Fpn Correction create vb pool, must destroy. */
    SAMPLE_VI_COMM_ReleaseFrameBlkInfo(1, pstViFrameInfo);
    if (RealPath != HI_NULL) {
        free(RealPath);
        RealPath = HI_NULL;
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_VI_DisableFpnCorrection(VI_PIPE ViPipe, SAMPLE_VI_FPN_CORRECTION_INFO_S *pstViFpnCorrectionInfo)
{
    ISP_FPN_ATTR_S stFPNAttr;
    HI_S32 s32Ret;

    if (pstViFpnCorrectionInfo == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_ISP_GetFPNAttr(ViPipe, &stFPNAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("get fpn correction fail, ret 0x%x\n", s32Ret);
        goto END;
    }

    stFPNAttr.bEnable = HI_FALSE;
    s32Ret = HI_MPI_ISP_SetFPNAttr(ViPipe, &stFPNAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("set fpn correction fail, ret 0x%x\n", s32Ret);
        goto END;
    }

END:
    /* Fpn Correction create vb pool, must destroy. */
    SAMPLE_VI_COMM_ReleaseFrameBlkInfo(1, &pstViFpnCorrectionInfo->stViFrameInfo);

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
