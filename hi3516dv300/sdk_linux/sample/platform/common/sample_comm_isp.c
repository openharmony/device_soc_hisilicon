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
#include "sample_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MAX_SENSOR_NUM          2
#define ISP_MAX_DEV_NUM         4
#define ISP_THREAD_NAME_LEN     20

static pthread_t g_IspPid[ISP_MAX_PIPE_NUM] = {0};
static HI_U32 g_au32IspSnsId[ISP_MAX_DEV_NUM] = { 0, 1 };

SAMPLE_SNS_TYPE_E g_enSnsType[MAX_SENSOR_NUM] = {
    SENSOR0_TYPE,
    SENSOR1_TYPE
};

ISP_PUB_ATTR_S ISP_PUB_ATTR_IMX335_MIPI_5M_30FPS = {
    { 0, 0, IMG_5M_WIDTH, IMG_5M_HEIGHT },
    { SENSOR_5M_WIDTH, SENSOR_5M_HEIGHT },
    FPS_30,
    BAYER_RGGB,
    WDR_MODE_NONE,
    0,
};

ISP_PUB_ATTR_S ISP_PUB_ATTR_IMX335_MIPI_5M_30FPS_WDR2TO1 = {
    { 0, 0, IMG_5M_WIDTH, IMG_5M_HEIGHT },
    { SENSOR_5M_WIDTH, SENSOR_5M_HEIGHT },
    FPS_30,
    BAYER_RGGB,
    WDR_MODE_2To1_LINE,
    0,
};

ISP_PUB_ATTR_S ISP_PUB_ATTR_IMX335_MIPI_4M_30FPS = {
    { 0, 0, IMG_4M_WIDTH, IMG_4M_HEIGHT },
    { SENSOR_5M_WIDTH, SENSOR_5M_HEIGHT },
    FPS_30,
    BAYER_RGGB,
    WDR_MODE_NONE,
    0,
};

ISP_PUB_ATTR_S ISP_PUB_ATTR_IMX335_MIPI_4M_30FPS_WDR2TO1 = {
    { 0, 0, IMG_4M_WIDTH, IMG_4M_HEIGHT },
    { SENSOR_5M_WIDTH, SENSOR_5M_HEIGHT },
    FPS_30,
    BAYER_RGGB,
    WDR_MODE_2To1_LINE,
    0,
};

HI_S32 SAMPLE_COMM_ISP_GetIspAttrBySns(SAMPLE_SNS_TYPE_E enSnsType, ISP_PUB_ATTR_S *pstPubAttr)
{
    if (pstPubAttr == NULL) {
        SAMPLE_PRT("null pointer\n");
        return HI_FAILURE;
    }
    switch (enSnsType) {
        case SONY_IMX335_MIPI_5M_30FPS_12BIT:
            (HI_VOID)memcpy_s(pstPubAttr, sizeof(ISP_PUB_ATTR_S), &ISP_PUB_ATTR_IMX335_MIPI_5M_30FPS,
                sizeof(ISP_PUB_ATTR_S));
            break;

        case SONY_IMX335_MIPI_4M_30FPS_12BIT:
            (HI_VOID)memcpy_s(pstPubAttr, sizeof(ISP_PUB_ATTR_S), &ISP_PUB_ATTR_IMX335_MIPI_4M_30FPS,
                sizeof(ISP_PUB_ATTR_S));
            break;

        case SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1:
            (HI_VOID)memcpy_s(pstPubAttr, sizeof(ISP_PUB_ATTR_S), &ISP_PUB_ATTR_IMX335_MIPI_5M_30FPS_WDR2TO1,
                sizeof(ISP_PUB_ATTR_S));
            break;

        case SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1:
            (HI_VOID)memcpy_s(pstPubAttr, sizeof(ISP_PUB_ATTR_S), &ISP_PUB_ATTR_IMX335_MIPI_4M_30FPS_WDR2TO1,
                sizeof(ISP_PUB_ATTR_S));
            break;

        default:
            (HI_VOID)memcpy_s(pstPubAttr, sizeof(ISP_PUB_ATTR_S), &ISP_PUB_ATTR_IMX335_MIPI_4M_30FPS,
                sizeof(ISP_PUB_ATTR_S));
            break;
    }

    return HI_SUCCESS;
}

static ISP_SNS_OBJ_S *SAMPLE_COMM_ISP_GetSnsObj(HI_U32 u32SnsId)
{
    SAMPLE_SNS_TYPE_E enSnsType;

    enSnsType = g_enSnsType[u32SnsId];
    switch (enSnsType) {
        case SONY_IMX335_MIPI_5M_30FPS_12BIT:
        case SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1:
        case SONY_IMX335_MIPI_4M_30FPS_12BIT:
        case SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1:
            return &stSnsImx335Obj;

        default:
            return HI_NULL;
    }
}

static void *SAMPLE_COMM_ISP_Thread(void *param)
{
    HI_S32 s32Ret;
    ISP_DEV IspDev;
    HI_CHAR szThreadName[ISP_THREAD_NAME_LEN];

    IspDev = (ISP_DEV)(HI_UINTPTR_T)param;
    s32Ret = snprintf_s(szThreadName, ISP_THREAD_NAME_LEN, ISP_THREAD_NAME_LEN - 1, "ISP%d_RUN", IspDev);
    if (s32Ret < 0) {
        SAMPLE_PRT("snprintf_s err !\n");
        return NULL;
    }

    prctl(PR_SET_NAME, szThreadName, 0, 0, 0);

    SAMPLE_PRT("ISP Dev %d running !\n", IspDev);
    s32Ret = HI_MPI_ISP_Run(IspDev);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_ISP_Run failed with %#x!\n", s32Ret);
    }

    return NULL;
}

HI_S32 SAMPLE_COMM_ISP_Aelib_Callback(ISP_DEV IspDev)
{
    ALG_LIB_S stAeLib;

    stAeLib.s32Id = IspDev;
    (HI_VOID)strncpy_s(stAeLib.acLibName, sizeof(HI_AE_LIB_NAME) + 1, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
    CHECK_RET(HI_MPI_AE_Register(IspDev, &stAeLib), "aelib register call back");
    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_ISP_Aelib_UnCallback(ISP_DEV IspDev)
{
    ALG_LIB_S stAeLib;

    stAeLib.s32Id = IspDev;
    (HI_VOID)strncpy_s(stAeLib.acLibName, sizeof(HI_AE_LIB_NAME) + 1, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
    CHECK_RET(HI_MPI_AE_UnRegister(IspDev, &stAeLib), "aelib unregister call back");
    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_ISP_Awblib_Callback(ISP_DEV IspDev)
{
    ALG_LIB_S stAwbLib;

    stAwbLib.s32Id = IspDev;
    (HI_VOID)strncpy_s(stAwbLib.acLibName, sizeof(HI_AWB_LIB_NAME) + 1, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
    CHECK_RET(HI_MPI_AWB_Register(IspDev, &stAwbLib), "awblib register call back");
    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_ISP_Awblib_UnCallback(ISP_DEV IspDev)
{
    ALG_LIB_S stAwbLib;

    stAwbLib.s32Id = IspDev;
    (HI_VOID)strncpy_s(stAwbLib.acLibName, sizeof(HI_AWB_LIB_NAME) + 1, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
    CHECK_RET(HI_MPI_AWB_UnRegister(IspDev, &stAwbLib), "awblib unregister call back");
    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_ISP_Run(ISP_DEV IspDev)
{
    HI_S32 s32Ret = 0;
    pthread_attr_t *pstAttr = NULL;

    s32Ret = pthread_create(&g_IspPid[IspDev], pstAttr, SAMPLE_COMM_ISP_Thread, (HI_VOID *)(HI_UINTPTR_T)IspDev);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("create isp running thread failed!, error: %d\r\n", s32Ret);
    }

    if (pstAttr != NULL) {
        pthread_attr_destroy(pstAttr);
    }

    return s32Ret;
}

HI_S32 SAMPLE_COMM_ISP_Sensor_Regiter_callback(ISP_DEV IspDev, HI_U32 u32SnsId)
{
    ALG_LIB_S stAeLib;
    ALG_LIB_S stAwbLib;
    const ISP_SNS_OBJ_S *pstSnsObj = HI_NULL;
    HI_S32 s32Ret = HI_FAILURE;

    if (u32SnsId >= MAX_SENSOR_NUM) {
        SAMPLE_PRT("invalid sensor id: %d\n", u32SnsId);
        return s32Ret;
    }

    pstSnsObj = SAMPLE_COMM_ISP_GetSnsObj(u32SnsId);
    if (pstSnsObj == HI_NULL) {
        SAMPLE_PRT("sensor %d not exist!\n", u32SnsId);
        return s32Ret;
    }

    stAeLib.s32Id = IspDev;
    stAwbLib.s32Id = IspDev;
    (HI_VOID)strncpy_s(stAeLib.acLibName, sizeof(HI_AE_LIB_NAME) + 1, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
    (HI_VOID)strncpy_s(stAwbLib.acLibName, sizeof(HI_AWB_LIB_NAME) + 1, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));

    if (pstSnsObj->pfnRegisterCallback != HI_NULL) {
        s32Ret = pstSnsObj->pfnRegisterCallback(IspDev, &stAeLib, &stAwbLib);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("sensor_register_callback failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    } else {
        SAMPLE_PRT("sensor_register_callback failed with HI_NULL!\n");
    }

    g_au32IspSnsId[IspDev] = u32SnsId;

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_ISP_Sensor_UnRegiter_callback(ISP_DEV IspDev)
{
    ALG_LIB_S stAeLib;
    ALG_LIB_S stAwbLib;
    HI_U32 u32SnsId;
    const ISP_SNS_OBJ_S *pstSnsObj = HI_NULL;
    HI_S32 s32Ret = HI_FAILURE;

    u32SnsId = g_au32IspSnsId[IspDev];
    if (u32SnsId >= MAX_SENSOR_NUM) {
        SAMPLE_PRT("%s: invalid sensor id: %d\n", __FUNCTION__, u32SnsId);
        return s32Ret;
    }

    pstSnsObj = SAMPLE_COMM_ISP_GetSnsObj(u32SnsId);
    if (pstSnsObj == HI_NULL) {
        return s32Ret;
    }

    stAeLib.s32Id = IspDev;
    stAwbLib.s32Id = IspDev;
    (HI_VOID)strncpy_s(stAeLib.acLibName, sizeof(HI_AE_LIB_NAME) + 1, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
    (HI_VOID)strncpy_s(stAwbLib.acLibName, sizeof(HI_AWB_LIB_NAME) + 1, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
    if (pstSnsObj->pfnUnRegisterCallback != HI_NULL) {
        s32Ret = pstSnsObj->pfnUnRegisterCallback(IspDev, &stAeLib, &stAwbLib);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("sensor_unregister_callback failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    } else {
        SAMPLE_PRT("sensor_unregister_callback failed with HI_NULL!\n");
    }

    return HI_SUCCESS;
}

/* function : stop ISP, and stop isp thread */
HI_VOID SAMPLE_COMM_ISP_Stop(ISP_DEV IspDev)
{
    if (g_IspPid[IspDev]) {
        HI_MPI_ISP_Exit(IspDev);
        pthread_join(g_IspPid[IspDev], NULL);
        SAMPLE_COMM_ISP_Awblib_UnCallback(IspDev);
        SAMPLE_COMM_ISP_Aelib_UnCallback(IspDev);
        SAMPLE_COMM_ISP_Sensor_UnRegiter_callback(IspDev);
        g_IspPid[IspDev] = 0;
    }

    return;
}

HI_VOID SAMPLE_COMM_All_ISP_Stop(HI_VOID)
{
    ISP_DEV IspDev;

    for (IspDev = 0; IspDev < ISP_MAX_PIPE_NUM; IspDev++) {
        SAMPLE_COMM_ISP_Stop(IspDev);
    }
}

static ISP_SNS_TYPE_E SAMPLE_COMM_GetSnsBusType(SAMPLE_SNS_TYPE_E enSnsType)
{
    return ISP_SNS_I2C_TYPE;
}

HI_S32 SAMPLE_COMM_ISP_BindSns(ISP_DEV IspDev, HI_U32 u32SnsId, SAMPLE_SNS_TYPE_E enSnsType, HI_S8 s8SnsDev)
{
    ISP_SNS_COMMBUS_U uSnsBusInfo;
    ISP_SNS_TYPE_E enBusType;
    const ISP_SNS_OBJ_S *pstSnsObj = HI_NULL;
    HI_S32 s32Ret;

    if (u32SnsId >= MAX_SENSOR_NUM) {
        SAMPLE_PRT("invalid sensor id: %d\n", u32SnsId);
        return HI_FAILURE;
    }

    pstSnsObj = SAMPLE_COMM_ISP_GetSnsObj(u32SnsId);
    if (pstSnsObj == HI_NULL) {
        SAMPLE_PRT("sensor %d not exist!\n", u32SnsId);
        return HI_FAILURE;
    }

    enBusType = SAMPLE_COMM_GetSnsBusType(enSnsType);
    if (enBusType == ISP_SNS_I2C_TYPE) {
        uSnsBusInfo.s8I2cDev = s8SnsDev;
    } else {
        uSnsBusInfo.s8SspDev.bit4SspDev = s8SnsDev;
        uSnsBusInfo.s8SspDev.bit4SspCs = 0;
    }

    if (pstSnsObj->pfnSetBusInfo != HI_NULL) {
        s32Ret = pstSnsObj->pfnSetBusInfo(IspDev, uSnsBusInfo);
        if (s32Ret != HI_SUCCESS) {
            SAMPLE_PRT("set sensor bus info failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    } else {
        SAMPLE_PRT("not support set sensor bus info!\n");
        return HI_FAILURE;
    }

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
