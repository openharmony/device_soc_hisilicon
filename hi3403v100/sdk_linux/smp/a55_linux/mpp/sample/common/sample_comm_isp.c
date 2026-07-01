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

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#ifdef __LITEOS__
#include <sys/select.h>
#endif
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <signal.h>
#include <math.h>
#include <poll.h>
#include <sys/prctl.h>

#include "ss_mpi_awb.h"
#include "ss_mpi_ae.h"
#include "ot_sns_ctrl.h"
#include "ss_mpi_isp.h"
#include "ot_common_isp.h"
#include "ot_common_video.h"
#include "sample_comm.h"

static ot_isp_sns_type g_sns_type[OT_VI_MAX_PIPE_NUM] = {(ot_isp_sns_type)SNS_TYPE_BUTT};
static pthread_t g_isp_pid[OT_VI_MAX_DEV_NUM] = {0};

/* IspPub attr */
static ot_isp_pub_attr g_isp_pub_attr_hy_s0603_mipi_8m_30fps = {
    {0, 32, 3840, 2160},
    {3840, 2160},
    30,
    OT_ISP_BAYER_BGGR,
    OT_WDR_MODE_NONE,
    0,
    0,
    0,
    {
        0,
        {0, 0, 3840, 2180},
    },
};

static ot_isp_pub_attr g_isp_pub_attr_hy_s0603_mipi_8m_30fps_wdr2to1 = {
    {0, 32, 3840, 2160},
    {3840, 2160},
    30,
    OT_ISP_BAYER_BGGR,
    OT_WDR_MODE_2To1_LINE,
    0,
    0,
    0,
    {
        0,
        {0, 0, 3840, 2180},
    },
};

static ot_isp_pub_attr g_isp_pub_attr_os08a20_mipi_8m_30fps = {
    {0, 24, 3840, 2160},
    {3840, 2160},
    30,
    OT_ISP_BAYER_BGGR,
    OT_WDR_MODE_NONE,
    0,
    0,
    0,
    {
        0,
        {0, 0, 3840, 2180},
    },
};

static ot_isp_pub_attr g_isp_pub_attr_os08a20_mipi_8m_30fps_wdr2to1 = {
    {0, 24, 3840, 2160},
    {3840, 2160},
    30,
    OT_ISP_BAYER_BGGR,
    OT_WDR_MODE_2To1_LINE,
    0,
    0,
    0,
    {
        0,
        {0, 0, 3840, 2180},
    },
};

static ot_isp_pub_attr g_isp_pub_attr_sc450ai_mipi_4m_30fps = {
    {0, 0, WIDTH_2688, HEIGHT_1520},
    {WIDTH_2688, HEIGHT_1520},
    30,
    OT_ISP_BAYER_BGGR,
    OT_WDR_MODE_NONE,
    0,
    0,
    0,
    {
        0,
        {0, 0, 2688, 1520},
    },
};

static ot_isp_pub_attr g_isp_pub_attr_sc450ai_mipi_4m_30fps_wdr2to1 = {
    {0, 0, WIDTH_2688, HEIGHT_1520},
    {WIDTH_2688, HEIGHT_1520},
    30,
    OT_ISP_BAYER_BGGR,
    OT_WDR_MODE_2To1_LINE,
    0,
    0,
    0,
    {
        0,
        {0, 0, 2688, 1520},
    },
};

td_s32 sample_comm_isp_get_pub_attr_by_sns(sample_sns_type sns_type, ot_isp_pub_attr *pub_attr)
{
    switch (sns_type) {
        case HY_S0603_MIPI_8M_30FPS_12BIT:
            (td_void)memcpy_s(pub_attr, sizeof(ot_isp_pub_attr),
                &g_isp_pub_attr_hy_s0603_mipi_8m_30fps, sizeof(ot_isp_pub_attr));
            break;
        case HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1:
            (td_void)memcpy_s(pub_attr, sizeof(ot_isp_pub_attr),
                &g_isp_pub_attr_hy_s0603_mipi_8m_30fps_wdr2to1, sizeof(ot_isp_pub_attr));
            break;
        case OV_OS08A20_MIPI_8M_30FPS_12BIT:
            (td_void)memcpy_s(pub_attr, sizeof(ot_isp_pub_attr),
                &g_isp_pub_attr_os08a20_mipi_8m_30fps, sizeof(ot_isp_pub_attr));
            break;
        case OV_OS08A20_MIPI_8M_30FPS_12BIT_WDR2TO1:
            (td_void)memcpy_s(pub_attr, sizeof(ot_isp_pub_attr),
                &g_isp_pub_attr_os08a20_mipi_8m_30fps_wdr2to1, sizeof(ot_isp_pub_attr));
            break;
        case SC450AI_MIPI_4M_30FPS_10BIT:
            (td_void)memcpy_s(pub_attr, sizeof(ot_isp_pub_attr),
                &g_isp_pub_attr_sc450ai_mipi_4m_30fps, sizeof(ot_isp_pub_attr));
            break;
        case SC450AI_MIPI_4M_30FPS_10BIT_WDR2TO1:
            (td_void)memcpy_s(pub_attr, sizeof(ot_isp_pub_attr),
                &g_isp_pub_attr_sc450ai_mipi_4m_30fps_wdr2to1, sizeof(ot_isp_pub_attr));
            break;
        default:
            (td_void)memcpy_s(pub_attr, sizeof(ot_isp_pub_attr),
                &g_isp_pub_attr_hy_s0603_mipi_8m_30fps, sizeof(ot_isp_pub_attr));
            break;
    }

    return TD_SUCCESS;
}

ot_isp_sns_obj *sample_comm_isp_get_sns_obj(sample_sns_type sns_type)
{
    switch (sns_type) {
        case HY_S0603_MIPI_8M_30FPS_12BIT:
        case HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1:
            return hy_s0603_get_sns_obj();
        case OV_OS08A20_MIPI_8M_30FPS_12BIT:
        case OV_OS08A20_MIPI_8M_30FPS_12BIT_WDR2TO1:
            return os08a20_get_sns_obj();
        case SC450AI_MIPI_4M_30FPS_10BIT:
        case SC450AI_MIPI_4M_30FPS_10BIT_WDR2TO1:
            return sc450ai_get_sns_obj();
        default:
            return TD_NULL;
    }
}

ot_isp_sns_type sample_comm_get_sns_bus_type(sample_sns_type sns_type)
{
    ot_unused(sns_type);
    return OT_ISP_SNS_I2C_TYPE;
}

/******************************************************************************
* funciton : ISP init
******************************************************************************/
td_s32 sample_comm_isp_sensor_regiter_callback(ot_isp_dev isp_dev, sample_sns_type sns_type)
{
    td_s32 ret;
    ot_isp_3a_alg_lib ae_lib;
    ot_isp_3a_alg_lib awb_lib;
    ot_isp_sns_obj *sns_obj;

    sns_obj = sample_comm_isp_get_sns_obj(sns_type);
    if (sns_obj == TD_NULL) {
        printf("sensor %d not exist!\n", sns_type);
        return TD_FAILURE;
    }

    ae_lib.id = isp_dev;
    awb_lib.id = isp_dev;
    strncpy_s(ae_lib.lib_name, sizeof(ae_lib.lib_name), OT_AE_LIB_NAME, sizeof(OT_AE_LIB_NAME));
    strncpy_s(awb_lib.lib_name, sizeof(awb_lib.lib_name), OT_AWB_LIB_NAME, sizeof(OT_AWB_LIB_NAME));
    if (sns_obj->pfn_register_callback != TD_NULL) {
        ret = sns_obj->pfn_register_callback(isp_dev, &ae_lib, &awb_lib);
        if (ret != TD_SUCCESS) {
            printf("sensor_register_callback failed with %#x!\n", ret);
            return ret;
        }
    } else {
        printf("sensor_register_callback failed with TD_NULL!\n");
    }

    g_sns_type[isp_dev] = (ot_isp_sns_type)sns_type;

    return TD_SUCCESS;
}

td_s32 sample_comm_isp_sensor_unregiter_callback(ot_isp_dev isp_dev)
{
    ot_isp_3a_alg_lib ae_lib;
    ot_isp_3a_alg_lib awb_lib;
    ot_isp_sns_obj *sns_obj;
    td_s32 ret;

    sns_obj = sample_comm_isp_get_sns_obj((sample_sns_type)g_sns_type[isp_dev]);
    if (sns_obj == TD_NULL) {
        printf("sensor %d not exist!\n", g_sns_type[isp_dev]);
        return TD_FAILURE;
    }

    ae_lib.id = isp_dev;
    awb_lib.id = isp_dev;
    strncpy_s(ae_lib.lib_name, sizeof(ae_lib.lib_name), OT_AE_LIB_NAME, sizeof(OT_AE_LIB_NAME));
    strncpy_s(awb_lib.lib_name, sizeof(awb_lib.lib_name), OT_AWB_LIB_NAME, sizeof(OT_AWB_LIB_NAME));
    if (sns_obj->pfn_un_register_callback != TD_NULL) {
        ret = sns_obj->pfn_un_register_callback(isp_dev, &ae_lib, &awb_lib);
        if (ret != TD_SUCCESS) {
            printf("sensor_unregister_callback failed with %#x!\n", ret);
            return ret;
        }
    } else {
        printf("sensor_unregister_callback failed with TD_NULL!\n");
    }

    g_sns_type[isp_dev] = (ot_isp_sns_type)SNS_TYPE_BUTT;

    return TD_SUCCESS;
}


td_s32 sample_comm_isp_bind_sns(ot_isp_dev isp_dev, sample_sns_type sns_type, td_s8 sns_dev)
{
    ot_isp_sns_commbus sns_bus_info;
    ot_isp_sns_type    bus_type;
    ot_isp_sns_obj    *sns_obj;
    td_s32 ret;

    sns_obj = sample_comm_isp_get_sns_obj(sns_type);
    if (sns_obj == TD_NULL) {
        printf("sensor %d not exist!\n", sns_type);
        return TD_FAILURE;
    }

    bus_type = sample_comm_get_sns_bus_type(sns_type);
    if (bus_type == OT_ISP_SNS_I2C_TYPE) {
        sns_bus_info.i2c_dev = sns_dev;
    } else {
        sns_bus_info.ssp_dev.bit4_ssp_dev = sns_dev;
        sns_bus_info.ssp_dev.bit4_ssp_cs  = 0;
    }

    if (sns_obj->pfn_set_bus_info != TD_NULL) {
        ret = sns_obj->pfn_set_bus_info(isp_dev, sns_bus_info);
        if (ret != TD_SUCCESS) {
            printf("set sensor bus info failed with %#x!\n", ret);
            return ret;
        }
    } else {
        printf("not support set sensor bus info!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_isp_ae_lib_callback(ot_isp_dev isp_dev)
{
    td_s32 ret;
    ot_isp_3a_alg_lib ae_lib;

    ae_lib.id = isp_dev;
    strncpy_s(ae_lib.lib_name, sizeof(ae_lib.lib_name), OT_AE_LIB_NAME, sizeof(OT_AE_LIB_NAME));

    ret = ss_mpi_ae_register(isp_dev, &ae_lib);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_ae_register failed with %#x!\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_isp_ae_lib_uncallback(ot_isp_dev isp_dev)
{
    td_s32 ret;
    ot_isp_3a_alg_lib ae_lib;

    ae_lib.id = isp_dev;
    strncpy_s(ae_lib.lib_name, sizeof(ae_lib.lib_name), OT_AE_LIB_NAME, sizeof(OT_AE_LIB_NAME));

    ret = ss_mpi_ae_unregister(isp_dev, &ae_lib);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_ae_unregister failed with %#x!\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_isp_awb_lib_callback(ot_isp_dev isp_dev)
{
    td_s32 ret;
    ot_isp_3a_alg_lib awb_lib;

    awb_lib.id = isp_dev;
    strncpy_s(awb_lib.lib_name, sizeof(awb_lib.lib_name), OT_AWB_LIB_NAME, sizeof(OT_AWB_LIB_NAME));

    ret = ss_mpi_awb_register(isp_dev, &awb_lib);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_awb_register failed with %#x!\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_isp_awb_lib_uncallback(ot_isp_dev isp_dev)
{
    td_s32 ret;
    ot_isp_3a_alg_lib awb_lib;

    awb_lib.id = isp_dev;
    strncpy_s(awb_lib.lib_name, sizeof(awb_lib.lib_name), OT_AWB_LIB_NAME, sizeof(OT_AWB_LIB_NAME));
    ret = ss_mpi_awb_unregister(isp_dev, &awb_lib);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_awb_unregister failed with %#x!\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

/******************************************************************************
* funciton : ISP Run
******************************************************************************/
static void *sample_comm_isp_thread(td_void *param)
{
    td_s32 ret;
    ot_isp_dev isp_dev;
    td_char thread_name[20];
    isp_dev = (ot_isp_dev)(td_ulong)param;
    errno_t err;

    err = snprintf_s(thread_name, sizeof(thread_name)*20, 19, "ISP%d_RUN", isp_dev); /* 20,19 chars */
    if (err < 0) {
        return NULL;
    }
    prctl(PR_SET_NAME, thread_name, 0, 0, 0);

    printf("ISP Dev %d running !\n", isp_dev);
    ret = ss_mpi_isp_run(isp_dev);
    if (ret != TD_SUCCESS) {
        printf("OT_MPI_ISP_Run failed with %#x!\n", ret);
        return NULL;
    }

    return NULL;
}

td_s32 sample_comm_isp_sensor_founction_cfg(ot_vi_pipe vi_pipe, sample_sns_type sns_type)
{
    return TD_SUCCESS;
}

td_s32 sample_comm_isp_run(ot_isp_dev isp_dev)
{
    td_s32 ret;
    pthread_attr_t *thread_attr = NULL;

    ret = pthread_create(&g_isp_pid[isp_dev], thread_attr, sample_comm_isp_thread, (td_void*)(td_ulong)isp_dev);
    if (ret != 0) {
        printf("create isp running thread failed!, error: %d\r\n", ret);
    }

    if (thread_attr != TD_NULL) {
        pthread_attr_destroy(thread_attr);
    }
    return ret;
}

td_void sample_comm_isp_stop(ot_isp_dev isp_dev)
{
    if (isp_dev >= OT_VI_MAX_DEV_NUM) {
        return;
    }
    
    if (g_isp_pid[isp_dev]) {
        pthread_join(g_isp_pid[isp_dev], NULL);
        g_isp_pid[isp_dev] = 0;
    }
    return;
}


td_void sample_comm_all_isp_stop(td_void)
{
    ot_isp_dev isp_dev;

    for (isp_dev = 0; isp_dev < OT_VI_MAX_DEV_NUM; isp_dev++) {
        sample_comm_isp_stop(isp_dev);
    }
}
