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
#include "sample_gyro_dis.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>

#include "ot_common_motionsensor.h"
#include "ot_motionsensor_chip_cmd.h"
#include "ot_common_motionfusion.h"
#include "ss_mpi_motionfusion.h"
#include "sample_fov2ldc.h"
#include "sample_comm.h"
#include "sample_dis.h"
#include "ot_osal.h"
#include "ot_osal_user.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define FOV_TO_LDCV2   0

#define NUMS_IN_BUF            4000  /* max numbers of gyro data or acc data in buffer */
#define GYRO_BUF_SIZE          (sizeof(ot_msensor_sample_data) * NUMS_IN_BUF)
#define ACC_BUF_SIZE           (sizeof(ot_msensor_sample_data) * NUMS_IN_BUF)

#define RAW_GYRO_PREC_BITS   15
#define DRIFT_GYRO_PREC_BITS 15
#define IMU_TEMP_RANGE_MIN   (-40)
#define IMU_TEMP_RANGE_MAX   85
#define IMU_TEMP_PREC_BITS   16
#define INT_MAX  0x7fffffff        /* max value for an int */

#define GYRO_DATA_IPC_RANGE   250
#define GYRO_DATA_DV_RANGE    1000
#define GYRO_DATA_ODR   1000
#define ACC_DATA_RANGE  16
#define ACC_DATA_ODR    1000
#define PIPE_NUM            2
const td_s32 g_imu_rotation_matrix[OT_MFUSION_MATRIX_NUM] = {
    -32768, 0, 0,
    0, 32768, 0,
    0, 0, -32768};
td_double g_gyro_data_range = 250;  /* IPC:250; DV:1000 */
td_s32 g_msensor_dev_fd = -1;
td_s32 g_msensor_mng_dev_fd = -1;
td_bool g_gyro_started = TD_FALSE;
ot_msensor_buf_attr g_msensor_attr;

const td_u32 g_frame_rate = 30;
const td_u32 g_input_frame_rate = 25;
#define WIDTH_NUM           3840
#define HEIGHT_NUM          2160

td_s32 sample_spi_init(void)
{
    td_s32 fd;
    td_s32 ret;
    td_s32 mode = SPI_MODE_3; /* | SPI_LSB_FIRST | SPI_LOOP | SPI_CS_HIGH */
    td_s32 bits = 8;
    td_u64 speed = 10000000;

    fd = OSAL_OPEN("/dev/spidev1.0", O_RDWR);
    if (fd < 0) {
        sample_print("open");
        return TD_FAILURE;
    }

    /* set spi mode */
    ret = OSAL_IOCTL(fd, SPI_IOC_WR_MODE, &mode); /* SPI_IOC_WR_MODE32 */
    if (ret != TD_SUCCESS) {
        sample_print("can't set spi mode");
        goto end;
    }

    ret = OSAL_IOCTL(fd, SPI_IOC_RD_MODE, &mode); /* SPI_IOC_RD_MODE32 */
    if (ret != TD_SUCCESS) {
        sample_print("can't get spi mode");
        goto end;
    }

    /*
     * bits per word
     */
    ret = OSAL_IOCTL(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret != TD_SUCCESS) {
        sample_print("can't set bits per word");
        goto end;
    }

    ret = OSAL_IOCTL(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret != TD_SUCCESS) {
        sample_print("can't get bits per word");
        goto end;
    }

    /* set spi max speed */
    ret = OSAL_IOCTL(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret != TD_SUCCESS) {
        sample_print("can't set bits max speed HZ");
        goto end;
    }

    ret = OSAL_IOCTL(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret != TD_SUCCESS) {
        sample_print("can't set bits max speed HZ");
        goto end;
    }

    printf("spi mode: 0x%x\n", mode);
    printf("bits per word: %d\n", bits);
    printf("max speed: %lld Hz (%lld KHz)\n", speed, speed / 1000); /* speed with precision of 1000 */

end:
    OSAL_CLOSE(fd);
    return ret;
}

td_s32 sample_motionsensor_init(ot_dis_pdt_type pdt_type)
{
    td_s32 ret;
    td_u32 buf_size = GYRO_BUF_SIZE + ACC_BUF_SIZE;
    ot_msensor_param msensor_param_set;

    g_msensor_dev_fd = OSAL_OPEN("/dev/motionsensor_chip", O_RDWR);
    if (g_msensor_dev_fd < 0) {
        sample_print("Error: cannot open MotionSensor device.may not load motionsensor driver !\n");
        return TD_FAILURE;
    }

    ret = ss_mpi_sys_mmz_alloc((td_phys_addr_t *)&g_msensor_attr.phys_addr,
                               (td_void **)&g_msensor_attr.virt_addr, "MotionsensorData", NULL, buf_size);
    if (ret != TD_SUCCESS) {
        sample_print("alloc mmz for Motionsensor failed,ret:%x !\n", ret);
        ret =  OT_ERR_VI_NO_MEM;
        return ret;
    }

    (td_void)memset_s((td_void *)(td_uintptr_t)g_msensor_attr.virt_addr, buf_size, 0, buf_size);

    g_msensor_attr.buf_len = buf_size;

    if (pdt_type == OT_DIS_PDT_TYPE_DV) {
        g_gyro_data_range = GYRO_DATA_DV_RANGE;
    } else if (pdt_type == OT_DIS_PDT_TYPE_IPC) {
        g_gyro_data_range = GYRO_DATA_IPC_RANGE;
    }

    /* set device work mode */
    msensor_param_set.attr.device_mask = OT_MSENSOR_DEVICE_GYRO | OT_MSENSOR_DEVICE_ACC;
    msensor_param_set.attr.temperature_mask = OT_MSENSOR_TEMP_GYRO | OT_MSENSOR_TEMP_ACC;
    /* set gyro samplerate and full scale range */
    msensor_param_set.config.gyro_config.odr = GYRO_DATA_ODR * OT_MSENSOR_GRADIENT;
    msensor_param_set.config.gyro_config.fsr = g_gyro_data_range * OT_MSENSOR_GRADIENT;
    /* set accel samplerate and full scale range */
    msensor_param_set.config.acc_config.odr = ACC_DATA_ODR * OT_MSENSOR_GRADIENT;
    msensor_param_set.config.acc_config.fsr = ACC_DATA_RANGE * OT_MSENSOR_GRADIENT;

    (td_void)memcpy_s(&msensor_param_set.buf_attr, sizeof(ot_msensor_buf_attr),
                      &g_msensor_attr, sizeof(ot_msensor_buf_attr));

    ret =  OSAL_IOCTL(g_msensor_dev_fd, MSENSOR_CMD_INIT, &msensor_param_set);
    if (ret) {
        sample_print("MSENSOR_CMD_INIT");
        return -1;
    }

    return ret;
}

td_void sample_motionsensor_deinit(void)
{
    td_s32 ret;
    if (g_msensor_dev_fd < 0) {
        return;
    }

    ret = OSAL_IOCTL(g_msensor_dev_fd, MSENSOR_CMD_DEINIT, NULL);
    if (ret != TD_SUCCESS) {
        sample_print("motionsensor deinit failed , ret:0x%x !\n", ret);
    }

    ret = ss_mpi_sys_mmz_free((td_phys_addr_t)g_msensor_attr.phys_addr,
                              (td_void *)(td_uintptr_t)g_msensor_attr.virt_addr);
    if (ret != TD_SUCCESS) {
        sample_print("motionsensor mmz free failed, ret:0x%x !\n", ret);
    }

    g_msensor_attr.phys_addr = 0;
    g_msensor_attr.virt_addr = (td_u64)(td_uintptr_t)NULL;

    close(g_msensor_dev_fd);
    g_msensor_dev_fd = -1;

    return;
}

td_s32 sample_motionsensor_start()
{
    td_s32 ret;
    ret =  OSAL_IOCTL(g_msensor_dev_fd, MSENSOR_CMD_START, NULL);
    if (ret) {
        perror("OSAL_IOCTL_CMD_START_MPU");
        return -1;
    }

    g_gyro_started = TD_TRUE;
    return ret;
}

td_s32 sample_motionsensor_stop(void)
{
    td_s32 ret;
    ret = OSAL_IOCTL(g_msensor_dev_fd, MSENSOR_CMD_STOP, NULL);
    if (ret != TD_SUCCESS) {
        sample_print("stop motionsensor failed!\n");
    }

    return ret;
}

static td_s32 sample_dis_set_mfusion_attr(td_u32 fusion_id, ot_dis_pdt_type mode)
{
    ot_mfusion_attr mfusion_attr;

    mfusion_attr.steady_detect_attr.steady_time_thr = 3; /* 3: default value for IPC */
    mfusion_attr.steady_detect_attr.gyro_offset = (td_s32)(10 * (1 << 15)); /* 10, 15: default value */
    mfusion_attr.steady_detect_attr.acc_offset = (td_s32)(0.1 * (1 << 15)); /* 0.1, 15: default value */
    mfusion_attr.steady_detect_attr.gyro_rms = (td_s32)(0.054 * (1 << 15)); /* 0.054, 15: default value */
    mfusion_attr.steady_detect_attr.acc_rms =
        (td_s32)(1.3565 * (1 << 15) / 1000); /* 1.3565, 15, 1000: default value */
    mfusion_attr.steady_detect_attr.gyro_offset_factor = (td_s32)(2 * (1 << 4)); /* 2, 4: default value */
    mfusion_attr.steady_detect_attr.acc_offset_factor = (td_s32)(2 * (1 << 4)); /* 2, 4: default value */
    mfusion_attr.steady_detect_attr.gyro_rms_factor = (td_s32)(8 * (1 << 4)); /* 8, 4: default value */
    mfusion_attr.steady_detect_attr.acc_rms_factor = (td_s32)(10 * (1 << 4)); /* 10, 4: default value */

    mfusion_attr.device_mask      = OT_MFUSION_DEVICE_GYRO | OT_MFUSION_DEVICE_ACC;
    mfusion_attr.temperature_mask = OT_MFUSION_TEMPERATURE_GYRO | OT_MFUSION_TEMPERATURE_ACC;

    if (mode == OT_DIS_PDT_TYPE_IPC) {
        mfusion_attr.steady_detect_attr.steady_time_thr = 3; /* 3: default value for IPC */
        mfusion_attr.steady_detect_attr.gyro_rms_factor = (td_s32)(8 * (1 << 4)); /* 8, 4: default value for IPC */
        mfusion_attr.steady_detect_attr.acc_rms_factor = (td_s32)(10 * (1 << 4)); /* 10, 4: default value for IPC */
    } else if (mode == OT_DIS_PDT_TYPE_DV) {
        mfusion_attr.steady_detect_attr.steady_time_thr = 1; /* 1: default value for DV */
        mfusion_attr.steady_detect_attr.gyro_rms_factor = (td_s32)(12.5 * (1 << 4)); /* 12.5,4:default value for DV */
        mfusion_attr.steady_detect_attr.acc_rms_factor = (td_s32)(100 * (1 << 4)); /* 100, 4: default value for DV */
    }

    return ss_mpi_mfusion_set_attr(fusion_id, &mfusion_attr);
}

static td_s32 sample_dis_set_temperature_drift(td_u32 fusion_id)
{
    ot_mfusion_temperature_drift temperature_drift;
    td_u32 i;
    td_s32 ret;

    temperature_drift.enable = TD_TRUE;

    temperature_drift.mode = OT_IMU_TEMPERATURE_DRIFT_LUT;
    temperature_drift.temperature_lut.range_min = 20 * 1024; /* 1024: 2^10, 10 bit precision; 20 degree */
    temperature_drift.temperature_lut.range_max = 49 * 1024; /* 1024: 2^10, 10 bit precision; 49 degree */
    temperature_drift.temperature_lut.step = 1 * 1024; /* 1024: 2^10, 10 bit precision */

    for (i = 0; i < OT_MFUSION_TEMPERATURE_LUT_SAMPLES; i++) {
        temperature_drift.temperature_lut.gyro_lut_status[i][0] = INT_MAX;
        temperature_drift.temperature_lut.gyro_lut_status[i][1] = INT_MAX;
    }

    (td_void)memset_s(temperature_drift.temperature_lut.imu_lut,
        OT_MFUSION_TEMPERATURE_LUT_SAMPLES * OT_MFUSION_AXIS_NUM * sizeof(td_s32),
        0, OT_MFUSION_TEMPERATURE_LUT_SAMPLES * OT_MFUSION_AXIS_NUM * sizeof(td_s32));

    ret = ss_mpi_mfusion_set_gyro_online_temperature_drift(fusion_id, &temperature_drift);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_motionfusion_set_gyro_online_temp_drift failed!\n");
    }

    return ret;
}

td_s32 sample_motionfusion_init_param(ot_dis_pdt_type mode)
{
    td_s32 ret;
    const td_u32 fusion_id = 0;
    td_s32 gyro_drift[OT_MFUSION_AXIS_NUM] = {0, 0, 0};
    ot_mfusion_six_side_calibration six_side_calibration;
    ot_mfusion_drift drift;

    six_side_calibration.enable = TD_TRUE;
    (td_void)memcpy_s(six_side_calibration.matrix, OT_MFUSION_MATRIX_NUM * sizeof(td_s32),
                      g_imu_rotation_matrix, OT_MFUSION_MATRIX_NUM * sizeof(td_s32));

    drift.enable = TD_TRUE;
    (td_void)memcpy_s(drift.drift, OT_MFUSION_AXIS_NUM * sizeof(td_s32),
                      gyro_drift, OT_MFUSION_AXIS_NUM * sizeof(td_s32));

    ret = sample_dis_set_mfusion_attr(fusion_id, mode);
    if (ret != TD_SUCCESS) {
        goto end;
    }

    ret = ss_mpi_mfusion_set_gyro_six_side_calibration(fusion_id, &six_side_calibration);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_motionfusion_set_gyro_six_side_cal failed!\n");
        goto end;
    }

    if (mode == OT_DIS_PDT_TYPE_IPC) {
        ret = ss_mpi_mfusion_set_gyro_online_drift(fusion_id, &drift);
        if (ret != TD_SUCCESS) {
            sample_print("mpi_motionfusion_set_gyro_online_drift failed!\n");
            goto end;
        }
    } else if (mode == OT_DIS_PDT_TYPE_DV) {
        ret = sample_dis_set_temperature_drift(fusion_id);
        if (ret != TD_SUCCESS) {
            goto end;
        }
    }

end:
    return ret;
}

td_s32 sample_motionfusion_deinit_param()
{
    const td_u32 fusion_id = 0;
    td_s32 ret;
    td_s32 gyro_drift[OT_MFUSION_AXIS_NUM] = {0};
    ot_mfusion_drift drift = {0};
    ot_mfusion_temperature_drift temperature_drift = {0};
    drift.enable = TD_TRUE;
    (td_void)memcpy_s(drift.drift, OT_MFUSION_AXIS_NUM * sizeof(td_s32),
        gyro_drift, OT_MFUSION_AXIS_NUM * sizeof(td_s32));

    ret = ss_mpi_mfusion_get_gyro_online_drift(fusion_id, &drift);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_mfusion_get_gyro_online_drift failed!\n");
        goto end;
    }

    drift.enable = TD_FALSE;

    ret = ss_mpi_mfusion_set_gyro_online_drift(fusion_id, &drift);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_mfusion_set_gyro_online_drift failed!\n");
        goto end;
    }

    temperature_drift.enable = TD_TRUE;
    ret = ss_mpi_mfusion_get_gyro_online_temperature_drift(fusion_id, &temperature_drift);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_mfusion_get_gyro_online_temperature_drift failed!\n");
        goto end;
    }

    temperature_drift.enable = TD_FALSE;
    ret = ss_mpi_mfusion_set_gyro_online_temperature_drift(fusion_id, &temperature_drift);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_mfusion_set_gyro_online_temperature_drift failed!\n");
        goto end;
    }

    sleep(1);
end:
    return ret;
}

td_s32 sample_dis_start_gyro(ot_dis_pdt_type pdt_type)
{
    td_s32 ret;

    ret = sample_motionsensor_init(pdt_type);
    if (ret != TD_SUCCESS) {
        sample_print("init gyro fail.ret:0x%x !\n", ret);
        return ret;
    }

    ret = sample_motionsensor_start();
    if (ret != TD_SUCCESS) {
        sample_print("start gyro fail.ret:0x%x !\n", ret);
        goto motionsensor_start_fail;
    }

    ret = sample_motionfusion_init_param(pdt_type);
    if (ret != TD_SUCCESS) {
        sample_print("motionfusion set param fail.ret:0x%x !\n", ret);
        goto motionfusion_init_fail;
    }

    return TD_SUCCESS;

motionsensor_start_fail:
    sample_motionsensor_stop();
motionfusion_init_fail:
    sample_motionsensor_deinit();
    return ret;
}

td_void sample_dis_stop_gyro()
{
    sample_motionfusion_deinit_param();
    sample_motionsensor_stop();
    sample_motionsensor_deinit();
}

td_s32 sample_dis_start_gyro_sample(sample_vi_cfg *vi_cfg, sample_vo_cfg *vo_cfg, ot_size *img_size,
    ot_dis_pdt_type pdt_type)
{
    td_s32 ret;

    ret = sample_dis_start_sample(vi_cfg, vo_cfg, img_size);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = sample_dis_start_gyro(pdt_type);
    if (ret != TD_SUCCESS) {
        sample_dis_stop_sample(vi_cfg, vo_cfg);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_void sample_dis_stop_gyro_sample(sample_vi_cfg *vi_cfg, sample_vo_cfg *vo_cfg)
{
    sample_dis_stop_sample_without_sys_exit(vi_cfg, vo_cfg);

    sample_dis_stop_gyro();

    sample_comm_sys_exit();
}

td_s32 sample_dis_get_ldc_v2_attr_from_fov(const ot_size *size, ot_ldc_attr *ldc_attr)
{
#if FOV_TO_LDCV2
    ot_fov_attr fov_attr;
    td_s32 ret;

    fov_attr.width  = size->width;
    fov_attr.height = size->height;
    fov_attr.type   = OT_FOV_TYPE_DIAGONAL;
    fov_attr.fov    = 90 * (1 << FOV_PREC_BITS); /* 90 degree */

    ret = ot_sample_fov_to_ldcv2(&fov_attr, &ldc_attr->ldc_v2_attr);
    if (ret == TD_SUCCESS) {
        return TD_SUCCESS;
    }

    sample_print("sample_fov_to_ldcv2 failed.ret:0x%x !\n", ret);
#endif
    return TD_FAILURE;
}

/* ldc v2 attr can get with len calibration tool, see more in PQTools */
td_void sample_dis_get_ldc_v2_attr(const ot_size *size, ot_ldc_attr *ldc_attr)
{
    td_s32 ratio_x;
    td_s32 ratio_y;
    ldc_attr->enable = TD_TRUE;
    ldc_attr->ldc_version = OT_LDC_V2;

    if (sample_dis_get_ldc_v2_attr_from_fov(size, ldc_attr) == TD_SUCCESS) {
        return;
    }

    ratio_x = WIDTH_NUM / size->width; /* 4k */
    ratio_y = HEIGHT_NUM / size->height; /* 4k */
    /* no LDC */
    ldc_attr->ldc_v2_attr.focal_len_x = 2281550 / ratio_x; /* 2281550, 2: get from PQTools */
    ldc_attr->ldc_v2_attr.focal_len_y = 2303229 / ratio_y; /* 2303229, 2: get from PQTools */
    ldc_attr->ldc_v2_attr.coord_shift_x = 191950 / ratio_y; /* 191950, 2: get from PQTools */
    ldc_attr->ldc_v2_attr.coord_shift_y = 107950 / ratio_y; /* 107950, 2: get from PQTools */

    ldc_attr->ldc_v2_attr.src_calibration_ratio[0] = 100000;  /* 100000: fixed value */
    ldc_attr->ldc_v2_attr.src_calibration_ratio[1] = 0; /* index 1 */
    ldc_attr->ldc_v2_attr.src_calibration_ratio[2] = 0; /* index 2 */
    ldc_attr->ldc_v2_attr.src_calibration_ratio[3] = 0; /* index 3 */
    ldc_attr->ldc_v2_attr.src_calibration_ratio[4] = 0; /* index 4 */
    ldc_attr->ldc_v2_attr.src_calibration_ratio[5] = 0; /* index 5 */
    ldc_attr->ldc_v2_attr.src_calibration_ratio[6] = 0; /* index 6 */
    ldc_attr->ldc_v2_attr.src_calibration_ratio[7] = 0; /* index 7 */
    ldc_attr->ldc_v2_attr.src_calibration_ratio[8] = 800000; /* 800000: get from PQTools */

    ldc_attr->ldc_v2_attr.dst_calibration_ratio[0] = 100000;  /* 100000: fixed value */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[1] = 0; /* index 1 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[2] = 0; /* index 2 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[3] = 0; /* index 3 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[4] = 0; /* index 4 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[5] = 0; /* index 5 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[6] = 0; /* index 6 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[7] = 0; /* index 7 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[8] = 0; /* index 8 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[9] = 0; /* index 9 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[10] = 0; /* index 10 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[11] = 0; /* index 11 */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[12] = 800000; /* index 12; 800000: get from PQTools */
    ldc_attr->ldc_v2_attr.dst_calibration_ratio[13] = 800000; /* index 13; 800000: get from PQTools */
    ldc_attr->ldc_v2_attr.max_du = (td_s32)(16 * (1 << 16));  /* 16: max value */
}

/* ldc v3 attr can get with len calibration tool, see more in PQTools */
td_void sample_dis_get_ldc_v3_attr(ot_ldc_attr *ldc_attr)
{
    ldc_attr->ldc_version = OT_LDC_V3;

    /* 8 mm,, no LDC 4K */
    ldc_attr->enable = TD_TRUE;

    ldc_attr->ldc_v3_attr.aspect = TD_FALSE;
    ldc_attr->ldc_v3_attr.x_ratio = 100; /* 100: max x_ratio */
    ldc_attr->ldc_v3_attr.y_ratio = 100; /* 100: max x_ratio */
    ldc_attr->ldc_v3_attr.xy_ratio = 100; /* 100: max xy_ratio */

    ldc_attr->ldc_v3_attr.focal_len_x = 2281550; /* 2281550: get from PQTools */
    ldc_attr->ldc_v3_attr.focal_len_y = 2303229; /* 2303229: get from PQTools */
    ldc_attr->ldc_v3_attr.coord_shift_x = 191950; /* 191950: get from PQTools */
    ldc_attr->ldc_v3_attr.coord_shift_y = 107950; /* 107950: get from PQTools */

    ldc_attr->ldc_v3_attr.src_calibration_ratio[0] = 100000; /* 100000: fixed value */
    ldc_attr->ldc_v3_attr.src_calibration_ratio[1] = 0; /* index 1 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio[2] = 0; /* index 2 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio[3] = 0; /* index 3 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio[4] = 0; /* index 4 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio[5] = 0; /* index 5 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio[6] = 0; /* index 6 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio[7] = 0; /* index 7 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio[8] = 3200000; /* index 8; 3200000: get from PQTools */

    ldc_attr->ldc_v3_attr.src_calibration_ratio_next[0] = 100000;  /* 100000: fixed value */
    ldc_attr->ldc_v3_attr.src_calibration_ratio_next[1] = 0; /* index 1 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio_next[2] = 0; /* index 2 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio_next[3] = 0; /* index 3 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio_next[4] = 0; /* index 4 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio_next[5] = 0; /* index 5 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio_next[6] = 0; /* index 6 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio_next[7] = 0; /* index 7 */
    ldc_attr->ldc_v3_attr.src_calibration_ratio_next[8] = 3200000; /* index 8; 3200000: get from PQTools */

    ldc_attr->ldc_v3_attr.coef_intp_ratio = 32768;  /* 32768: max coef_intp_ratio value */
}

static td_void sample_dis_get_gyro_dis_cfg(const ot_size *size, ot_dis_pdt_type pdt_type,
    ot_dis_cfg *dis_cfg, ot_dis_attr *dis_attr)
{
    const td_s32 imu_filter_delay_time = 2900; /* according to imu datesheet */
    const td_s32 read_out_delay_time = 92; /* according to sensor datesheet */

    dis_cfg->mode = OT_DIS_MODE_GYRO;
    dis_cfg->motion_level = OT_DIS_MOTION_LEVEL_NORM;
    dis_cfg->crop_ratio = 80; /* 80: tipical crop ratio value */
    dis_cfg->buf_num = 5; /* 5: tipical buffer num value */
    dis_cfg->scale = TD_FALSE;
    dis_cfg->frame_rate = g_frame_rate;
    dis_cfg->pdt_type = pdt_type;
    dis_cfg->camera_steady = pdt_type == OT_DIS_PDT_TYPE_IPC ? TD_TRUE : TD_FALSE;

    dis_attr->enable = TD_TRUE;
    dis_attr->moving_subject_level = 0;
    dis_attr->rolling_shutter_coef = 0;
    dis_attr->still_crop = TD_FALSE;
    dis_attr->hor_limit = 512; /* 512: tipical hor_limit value when camera_steady is false */
    dis_attr->ver_limit = 512; /* 512: tipical ver_limit value when camera_steady is false */
    dis_attr->strength = 1024; /* 1024: max strength */

    dis_attr->timelag = imu_filter_delay_time - read_out_delay_time;
}

td_s32 sample_dis_start_gyro_dis(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_dis_cfg *dis_cfg, ot_dis_attr *dis_attr,
    ot_ldc_attr *ldc_attr)
{
    td_s32 ret;

    ret = ss_mpi_vi_set_chn_ldc_attr(vi_pipe, vi_chn, ldc_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set ldc config failed.ret:0x%x !\n", ret);
        return ret;
    }

    ret = ss_mpi_vi_set_chn_dis_cfg(vi_pipe, vi_chn, dis_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("set dis config failed.ret:0x%x !\n", ret);
        return ret;
    }

    ret = ss_mpi_vi_set_chn_dis_attr(vi_pipe, vi_chn, dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 sample_dis_gyro_switch_dis(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_dis_attr *dis_attr)
{
    td_s32 ret;

    printf("\nplease hit the Enter key to Disable DIS!\n");
    sample_dis_pause();

    dis_attr->enable = TD_FALSE;
    ret = ss_mpi_vi_set_chn_dis_attr(vi_pipe, vi_chn, dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
        return ret;
    }

    printf("\nplease hit the Enter key to enable DIS!\n");
    sample_dis_pause();

    dis_attr->enable = TD_TRUE;
    ret = ss_mpi_vi_set_chn_dis_attr(vi_pipe, vi_chn, dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_dis_gyro(ot_vo_intf_type vo_intf_type, ot_dis_pdt_type pdt_type)
{
    td_s32 ret;
    ot_size size;
    sample_vi_cfg vi_cfg;
    sample_vo_cfg vo_cfg;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;

    ot_dis_cfg dis_cfg     = {0};
    ot_dis_attr dis_attr   = {0};

    ot_ldc_attr ldc_attr = {0};
    sample_dis_set_save_stream(TD_TRUE);

    sample_comm_vi_get_default_vi_cfg(SENSOR0_TYPE, &vi_cfg);

    sample_comm_vi_get_size_by_sns_type(vi_cfg.sns_info.sns_type, &size);

    printf("input size:%dx%d, frame rate:%d\n", size.width, size.height, g_frame_rate);

    sample_comm_vo_get_def_config(&vo_cfg);
    vo_cfg.vo_intf_type = vo_intf_type;

    ret = sample_dis_start_gyro_sample(&vi_cfg, &vo_cfg, &size, pdt_type);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    sample_dis_get_ldc_v2_attr(&size, &ldc_attr);
    ldc_attr.enable = TD_FALSE;
    sample_dis_get_gyro_dis_cfg(&size, pdt_type, &dis_cfg, &dis_attr);

    ret = sample_dis_start_gyro_dis(vi_pipe, vi_chn, &dis_cfg, &dis_attr, &ldc_attr);
    if (ret != TD_SUCCESS) {
        goto stop_sample;
    }

    ret = sample_dis_gyro_switch_dis(vi_pipe, vi_chn, &dis_attr);

    printf("\nplease hit the Enter key to exit!\n");
    sample_dis_pause();

stop_sample:
    sample_dis_stop_gyro_sample(&vi_cfg, &vo_cfg);
    return ret;
}

static td_s32 sample_dis_send_data(ot_vo_intf_type vo_intf_type, ot_size *input_size)
{
    td_s32 ret;
    ot_size size;
    sample_vi_cfg vi_cfg;
    sample_vo_cfg vo_cfg;
    const ot_vi_pipe vi_pipe = 3;
    const ot_vi_chn vi_chn = 0;
    sample_dis_set_save_stream(TD_TRUE);
    size.width = input_size->width;
    size.height = input_size->height;

    sample_comm_vi_get_default_vi_cfg(SENSOR0_TYPE, &vi_cfg);
    vi_cfg.grp_info.fusion_grp_attr[0].pipe_id[0] = vi_pipe;
    vi_cfg.bind_pipe.pipe_id[0] = vi_pipe;
    vi_cfg.grp_info.fusion_grp_attr[0].cache_line = input_size->height;
    vi_cfg.sns_info.bus_id = -1;
    vi_cfg.dev_info.dev_attr.in_size.width = size.width;
    vi_cfg.dev_info.dev_attr.in_size.height = size.height;
    vi_cfg.pipe_info[0].isp_info.isp_pub_attr.wnd_rect.width = size.width;
    vi_cfg.pipe_info[0].isp_info.isp_pub_attr.wnd_rect.height = size.height;
    vi_cfg.pipe_info[0].pipe_attr.size.width = size.width;
    vi_cfg.pipe_info[0].pipe_attr.size.height = size.height;
    vi_cfg.pipe_info[0].chn_info[0].chn_attr.size.width = size.width;
    vi_cfg.pipe_info[0].chn_info[0].chn_attr.size.height = size.height;
    vi_cfg.pipe_info[0].pipe_attr.compress_mode = OT_COMPRESS_MODE_NONE;
    vi_cfg.pipe_info[0].chn_info[vi_chn].chn_attr.compress_mode = OT_COMPRESS_MODE_NONE;
    vi_cfg.pipe_info[0].pipe_attr.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    sample_dis_set_venc_chn_size(size);

    printf("input size:%dx%d, frame rate:%d\n", size.width, size.height, g_input_frame_rate);

    sample_comm_vo_get_def_config(&vo_cfg);
    vo_cfg.vo_intf_type = vo_intf_type;
    sample_dis_set_send_data_statue(TD_TRUE);
    ret = sample_dis_start_sample(&vi_cfg, &vo_cfg, &size);
    if (ret != TD_SUCCESS) {
        printf("sample_dis_start_sample failed!!!\n");
        return TD_FAILURE;
    }

    printf("waiting for send data\n");
    sample_dis_pause();
    sample_dis_stop_sample(&vi_cfg, &vo_cfg);
    return ret;
}

td_s32 sample_dis_gyro_two_pipe(ot_vo_intf_type vo_intf_type, ot_dis_pdt_type pdt_type)
{
    td_s32 ret;
    ot_size size;
    sample_vi_cfg vi_cfg;
    sample_vo_cfg vo_cfg;
    td_u32 frame_rate = g_frame_rate;
    const ot_vi_pipe dis_pipe = 0;
    const ot_vi_pipe vi_pipe = 1;

    const ot_vi_chn vi_chn = 0;
    const ot_vo_chn vo_chn = 1;
    ot_dis_cfg dis_cfg     = {0};
    ot_dis_attr dis_attr         = {0};
    ot_ldc_attr ldc_attr = {0};
    sample_dis_set_save_stream(TD_FALSE);

    sample_comm_vi_get_default_vi_cfg(SENSOR0_TYPE, &vi_cfg);
    vi_cfg.bind_pipe.pipe_num = PIPE_NUM;
    vi_cfg.bind_pipe.pipe_id[vi_pipe] = 1;
    (td_void)memcpy_s(&vi_cfg.pipe_info[vi_pipe], sizeof(sample_vi_pipe_info), &vi_cfg.pipe_info[dis_pipe],
        sizeof(sample_vi_pipe_info));

    sample_comm_vi_get_size_by_sns_type(vi_cfg.sns_info.sns_type, &size);

    printf("input size:%dx%d, frame rate:%d\n", size.width, size.height, frame_rate);

    sample_comm_vo_get_def_config(&vo_cfg);
    vo_cfg.vo_intf_type = vo_intf_type;
    vo_cfg.vo_mode = VO_MODE_4MUX;

    ret = sample_dis_start_gyro_sample(&vi_cfg, &vo_cfg, &size, pdt_type);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (sample_comm_vi_bind_vo(vi_pipe, vi_chn, vo_cfg.vo_dev, vo_chn) != TD_SUCCESS) {
        goto stop_sample;
    }

    sample_dis_get_ldc_v2_attr(&size, &ldc_attr);
    ldc_attr.enable = TD_FALSE;
    sample_dis_get_gyro_dis_cfg(&size, pdt_type, &dis_cfg, &dis_attr);

    ret = sample_dis_start_gyro_dis(dis_pipe, vi_chn, &dis_cfg, &dis_attr, &ldc_attr);
    if (ret != TD_SUCCESS) {
        goto un_bind_vo;
    }

    printf("\nplease hit the Enter key to exit!\n");
    sample_dis_pause();

un_bind_vo:
    sample_comm_vi_un_bind_vo(1, vi_chn, vo_cfg.vo_dev, vo_chn);
stop_sample:
    sample_dis_stop_gyro_sample(&vi_cfg, &vo_cfg);
    sample_dis_set_save_stream(TD_TRUE);
    return ret;
}

td_s32 sample_dis_ipc_gyro(ot_vo_intf_type vo_intf_type)
{
    return sample_dis_gyro(vo_intf_type, OT_DIS_PDT_TYPE_IPC);
}

td_s32 sample_dis_dv_gyro(ot_vo_intf_type vo_intf_type)
{
    return sample_dis_gyro(vo_intf_type, OT_DIS_PDT_TYPE_DV);
}

td_s32 sample_dis_gyro_demo(ot_vo_intf_type vo_intf_type)
{
    return sample_dis_gyro_two_pipe(vo_intf_type, OT_DIS_PDT_TYPE_DV);
}

td_s32 sample_dis_send(ot_vo_intf_type vo_intf_type, ot_size *input_size)
{
    return sample_dis_send_data(vo_intf_type, input_size);
}

td_s32 sample_dis_gyro_switch_ldcv2(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_dis_attr *dis_attr)
{
    td_s32 ret;

    printf("\nNow LDCV2 and GYRO_DIS is enable, please hit the Enter key to switch to LDCV2!\n");
    sample_dis_pause();
    dis_attr->still_crop = TD_TRUE;
    ret = ss_mpi_vi_set_chn_dis_attr(vi_pipe, vi_chn, dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
        return ret;
    }

    printf("\nNow LDCV2 is enable, please hit the Enter key to switch to LDCV2 and GYRO_DIS!\n");
    sample_dis_pause();
    dis_attr->still_crop = TD_FALSE;
    ret = ss_mpi_vi_set_chn_dis_attr(vi_pipe, vi_chn, dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_dis_gyro_switch_ldcv3(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_dis_attr *dis_attr,
    ot_ldc_attr *ldc_v2_attr, ot_ldc_attr *ldc_v3_attr)
{
    td_s32 ret;

    printf("\nNow LDCV2 and GYRO_DIS is enable, please hit the Enter key to switch to LDCV3!\n");
    sample_dis_pause();
    dis_attr->enable = TD_FALSE;
    ret = ss_mpi_vi_set_chn_dis_attr(vi_pipe, vi_chn, dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
        return ret;
    }

    ret = ss_mpi_vi_set_chn_ldc_attr(vi_pipe, vi_chn, ldc_v3_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set ldc config failed.ret:0x%x !\n", ret);
        return ret;
    }

    printf("\nNow LDCV3 is enable, please hit the Enter key to switch to LDCV2 and GYRO_DIS!\n");
    sample_dis_pause();

    ret = ss_mpi_vi_set_chn_ldc_attr(vi_pipe, vi_chn, ldc_v2_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set ldc config failed.ret:0x%x !\n", ret);
        return ret;
    }

    dis_attr->enable = TD_TRUE;
    ret = ss_mpi_vi_set_chn_dis_attr(vi_pipe, vi_chn, dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_dis_gyro_switch(ot_vo_intf_type vo_intf_type, ot_ldc_version ldc_version)
{
    td_s32 ret;
    ot_size size;
    sample_vi_cfg vi_cfg;
    sample_vo_cfg vo_cfg;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;

    ot_ldc_attr ldc_v2_attr = {0};
    ot_ldc_attr ldc_v3_attr = {0};
    ot_dis_cfg dis_cfg = {0};
    ot_dis_attr dis_attr = {0};
    sample_dis_set_save_stream(TD_TRUE);

    sample_comm_vi_get_default_vi_cfg(SENSOR0_TYPE, &vi_cfg);

    sample_comm_vi_get_size_by_sns_type(vi_cfg.sns_info.sns_type, &size);

    printf("input size:%dx%d, frame rate:%d\n", size.width, size.height, g_frame_rate);

    sample_comm_vo_get_def_config(&vo_cfg);
    vo_cfg.vo_intf_type = vo_intf_type;

    ret = sample_dis_start_gyro_sample(&vi_cfg, &vo_cfg, &size, OT_DIS_PDT_TYPE_IPC);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    sample_dis_get_ldc_v2_attr(&size, &ldc_v2_attr);
    sample_dis_get_gyro_dis_cfg(&size, OT_DIS_PDT_TYPE_IPC, &dis_cfg, &dis_attr);
    ret = sample_dis_start_gyro_dis(vi_pipe, vi_chn, &dis_cfg, &dis_attr, &ldc_v2_attr);
    if (ret != TD_SUCCESS) {
        goto stop_sample;
    }

    if (ldc_version == OT_LDC_V2) {
        ret = sample_dis_gyro_switch_ldcv2(vi_pipe, vi_chn, &dis_attr);
    } else {
        sample_dis_get_ldc_v3_attr(&ldc_v3_attr);
        ret = sample_dis_gyro_switch_ldcv3(vi_pipe, vi_chn, &dis_attr, &ldc_v2_attr, &ldc_v3_attr);
    }

    printf("\nplease hit the Enter key to exit!\n");
    sample_dis_pause();

stop_sample:
    sample_dis_stop_gyro_sample(&vi_cfg, &vo_cfg);
    return ret;
}

td_s32 sample_dis_ipc_gyro_two_sensor(ot_size *size)
{
    ot_vi_pipe vi_pipe[2] = {0, 1}; /* 2 pipe */
    ot_vi_chn vi_chn = 0;
    td_s32 ret;
    ot_dis_cfg dis_cfg   = {0};
    ot_dis_attr dis_attr = {0};
    ot_ldc_attr ldc_attr = {0};
    ot_dis_pdt_type pdt_type = OT_DIS_PDT_TYPE_IPC;

    ret = sample_dis_start_gyro(pdt_type);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    sample_dis_get_ldc_v2_attr(size, &ldc_attr);
    ldc_attr.enable = TD_FALSE;
    sample_dis_get_gyro_dis_cfg(size, pdt_type, &dis_cfg, &dis_attr);

    ret = sample_dis_start_gyro_dis(vi_pipe[0], vi_chn, &dis_cfg, &dis_attr, &ldc_attr);
    if (ret != TD_SUCCESS) {
        sample_print("\n start gyro pipe: %d failed!\n", vi_pipe[0]);
        goto stop_sample;
    }
    ret = sample_dis_start_gyro_dis(vi_pipe[1], vi_chn, &dis_cfg, &dis_attr, &ldc_attr);
    if (ret != TD_SUCCESS) {
        sample_print("\n start gyro pipe: %d failed!\n", vi_pipe[1]);
        goto stop_sample;
    }

    printf("\nplease hit the Enter key to exit!\n");
    sample_dis_pause();
    return TD_SUCCESS;

stop_sample:
    sample_dis_stop_gyro();
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of __cplusplus */

