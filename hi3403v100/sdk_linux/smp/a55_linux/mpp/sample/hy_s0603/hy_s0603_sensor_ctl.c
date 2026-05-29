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

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef OT_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "ot_i2c.h"
#endif

#include "hy_s0603_cmos.h"
#include "ss_mpi_isp.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

static int g_fd[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = -1};

int hy_s0603_i2c_init(ot_vi_pipe vi_pipe)
{
    int ret;
    char dev_file[I2C_DEV_FILE_NUM] = {0};
    td_u8 dev_num;

    if (g_fd[vi_pipe] >= 0) {
        return TD_SUCCESS;
    }
#ifdef OT_GPIO_I2C
    g_fd[vi_pipe] = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (g_fd[vi_pipe] < 0) {
        isp_err_trace("Open gpioi2c_ex error!\n");
        return TD_FAILURE;
    }
#else
    ot_isp_sns_commbus *hy_s0603businfo = TD_NULL;
    hy_s0603businfo = hy_s0603_get_bus_info(vi_pipe);
    dev_num = hy_s0603businfo->i2c_dev;
    if (snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num) < 0) {
        isp_err_trace("snprintf_s error!\n");
        return TD_FAILURE;
    }

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        isp_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return TD_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_I2C_SLAVE_FORCE, (HY_S0603_I2C_ADDR >> 1));
    if (ret < 0) {
        isp_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return TD_SUCCESS;
}

int hy_s0603_i2c_exit(ot_vi_pipe vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 hy_s0603_read_register(ot_vi_pipe vi_pipe, td_u32 addr)
{
    ot_unused(vi_pipe);
    ot_unused(addr);
    return TD_SUCCESS;
}

td_s32 hy_s0603_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data)
{
    td_s32 ret;
    if (g_fd[vi_pipe] < 0) {
        return TD_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = HY_S0603_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = HY_S0603_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = HY_S0603_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        isp_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    td_u32 idx = 0;
    td_u8 buf[I2C_BUF_NUM];

    if (HY_S0603_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (HY_S0603_DATA_BYTE == 2) {  /* 2 byte */
        buf[idx] = (data >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
    } else {
        buf[idx] = data & 0xff;
    }

    ret = write(g_fd[vi_pipe], buf, HY_S0603_ADDR_BYTE + HY_S0603_DATA_BYTE);
    if (ret < 0) {
        isp_err_trace("I2C_WRITE error!\n");
        return TD_FAILURE;
    }

#endif
    return TD_SUCCESS;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000); /* 1ms: 1000us */
    return;
}

void hy_s0603_prog(ot_vi_pipe vi_pipe, const td_u32 *rom)
{
    ot_unused(vi_pipe);
    ot_unused(rom);
    return;
}

void hy_s0603_standby(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

void hy_s0603_restart(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

static void hy_s0603_vc_wdr_2t1_8m30_10bit_init(ot_vi_pipe vi_pipe);
static void hy_s0603_linear_8m30_12bit_init(ot_vi_pipe vi_pipe);

void hy_s0603_init(ot_vi_pipe vi_pipe)
{
    ot_wdr_mode      wdr_mode;
    td_u8            img_mode;
    td_bool          init;
    td_s32           ret;
    ot_isp_sns_state *pasthy_s0603 = TD_NULL;

    pasthy_s0603 = hy_s0603_get_ctx(vi_pipe);
    init        = pasthy_s0603->init;
    wdr_mode    = pasthy_s0603->wdr_mode;
    img_mode    = pasthy_s0603->img_mode;

    ret = hy_s0603_i2c_init(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    /* When quick start mode, sensor init in boot, don't init sensor here */
    if (init == TD_FALSE) {
        if (OT_WDR_MODE_2To1_LINE == wdr_mode) {
            if (HY_S0603_8M_30FPS_10BIT_2TO1_VC_MODE == img_mode) {  /* HY_S0603_VMAX_8M_30FPS_12BIT_2TO1_WDR */
                hy_s0603_vc_wdr_2t1_8m30_10bit_init(vi_pipe);
            } else {
            }
        } else {
            hy_s0603_linear_8m30_12bit_init(vi_pipe);
        }
    } else {
        if (OT_WDR_MODE_2To1_LINE == wdr_mode) {
            if (HY_S0603_8M_30FPS_10BIT_2TO1_VC_MODE == img_mode) {
                hy_s0603_vc_wdr_2t1_8m30_10bit_init(vi_pipe);
            } else {
            }
        } else {
            hy_s0603_linear_8m30_12bit_init(vi_pipe);
        }
    }

    pasthy_s0603->init = TD_TRUE;
    return;
}

void hy_s0603_exit(ot_vi_pipe vi_pipe)
{
    td_s32 ret;

    ret = hy_s0603_i2c_exit(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("HY_S0603 exit failed!\n");
    }

    return;
}

static td_s32 hy_s0603_linear_8m30_12bit_init_part1(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += hy_s0603_write_register(vi_pipe, 0x0103, 0x01);
    delay_ms(100); /* 100ms */
    ret += hy_s0603_write_register(vi_pipe, 0x0100, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x36e9, 0x80);
    ret += hy_s0603_write_register(vi_pipe, 0x36f9, 0x80);
    ret += hy_s0603_write_register(vi_pipe, 0x36ea, 0x08);
    ret += hy_s0603_write_register(vi_pipe, 0x36eb, 0x0c);
    ret += hy_s0603_write_register(vi_pipe, 0x36ec, 0x4a);
    ret += hy_s0603_write_register(vi_pipe, 0x36ed, 0x24);
    ret += hy_s0603_write_register(vi_pipe, 0x36fa, 0x0b);
    ret += hy_s0603_write_register(vi_pipe, 0x36fb, 0x13);
    ret += hy_s0603_write_register(vi_pipe, 0x36fc, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x36fd, 0x17);
    ret += hy_s0603_write_register(vi_pipe, 0x36e9, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x36f9, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3018, 0x7a);
    ret += hy_s0603_write_register(vi_pipe, 0x3019, 0xf0);
    ret += hy_s0603_write_register(vi_pipe, 0x301a, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x301e, 0x3c);
    ret += hy_s0603_write_register(vi_pipe, 0x301f, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x302a, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3031, 0x0c);
    ret += hy_s0603_write_register(vi_pipe, 0x3032, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3033, 0x22);
    ret += hy_s0603_write_register(vi_pipe, 0x3037, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x303e, 0xb4);
    ret += hy_s0603_write_register(vi_pipe, 0x320c, 0x04);
    ret += hy_s0603_write_register(vi_pipe, 0x320d, 0x4c);
    ret += hy_s0603_write_register(vi_pipe, 0x3226, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3227, 0x03);
    ret += hy_s0603_write_register(vi_pipe, 0x3250, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3253, 0x08);
    ret += hy_s0603_write_register(vi_pipe, 0x327e, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3280, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3281, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3301, 0x3c);
    ret += hy_s0603_write_register(vi_pipe, 0x3304, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x3306, 0xe8);
    ret += hy_s0603_write_register(vi_pipe, 0x3308, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x3309, 0x70);
    ret += hy_s0603_write_register(vi_pipe, 0x330a, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x330b, 0xe0);
    ret += hy_s0603_write_register(vi_pipe, 0x330d, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x3314, 0x92);

    return ret;
}

static td_s32 hy_s0603_linear_8m30_12bit_init_part2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += hy_s0603_write_register(vi_pipe, 0x331e, 0x29);
    ret += hy_s0603_write_register(vi_pipe, 0x331f, 0x69);
    ret += hy_s0603_write_register(vi_pipe, 0x3333, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x3347, 0x05);
    ret += hy_s0603_write_register(vi_pipe, 0x3348, 0xd0);
    ret += hy_s0603_write_register(vi_pipe, 0x3352, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3356, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x335d, 0x60);
    ret += hy_s0603_write_register(vi_pipe, 0x3362, 0x70);
    ret += hy_s0603_write_register(vi_pipe, 0x338f, 0x80);
    ret += hy_s0603_write_register(vi_pipe, 0x33af, 0x48);
    ret += hy_s0603_write_register(vi_pipe, 0x33fe, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3400, 0x12);
    ret += hy_s0603_write_register(vi_pipe, 0x3406, 0x04);
    ret += hy_s0603_write_register(vi_pipe, 0x3410, 0x12);
    ret += hy_s0603_write_register(vi_pipe, 0x3416, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x3433, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3440, 0x12);
    ret += hy_s0603_write_register(vi_pipe, 0x3446, 0x08);
    ret += hy_s0603_write_register(vi_pipe, 0x3478, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3479, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x347a, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x347b, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x347c, 0x04);
    ret += hy_s0603_write_register(vi_pipe, 0x347d, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3616, 0x0c);
    ret += hy_s0603_write_register(vi_pipe, 0x3620, 0x92);
    ret += hy_s0603_write_register(vi_pipe, 0x3622, 0x74);
    ret += hy_s0603_write_register(vi_pipe, 0x3629, 0x74);
    ret += hy_s0603_write_register(vi_pipe, 0x362a, 0xf0);
    ret += hy_s0603_write_register(vi_pipe, 0x362b, 0x0f);
    ret += hy_s0603_write_register(vi_pipe, 0x362d, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3630, 0x68);
    ret += hy_s0603_write_register(vi_pipe, 0x3633, 0x22);
    ret += hy_s0603_write_register(vi_pipe, 0x3634, 0x22);
    ret += hy_s0603_write_register(vi_pipe, 0x3635, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3637, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x3638, 0x26);
    ret += hy_s0603_write_register(vi_pipe, 0x363b, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x363c, 0x08);
    ret += hy_s0603_write_register(vi_pipe, 0x363d, 0x05);
    ret += hy_s0603_write_register(vi_pipe, 0x363e, 0x8f);
    ret += hy_s0603_write_register(vi_pipe, 0x3648, 0xe0);
    ret += hy_s0603_write_register(vi_pipe, 0x3649, 0x0a);
    ret += hy_s0603_write_register(vi_pipe, 0x364a, 0x06);

    return ret;
}

static td_s32 hy_s0603_linear_8m30_12bit_init_part3(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += hy_s0603_write_register(vi_pipe, 0x364c, 0x6a);
    ret += hy_s0603_write_register(vi_pipe, 0x3650, 0x3d);
    ret += hy_s0603_write_register(vi_pipe, 0x3654, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3656, 0x68);
    ret += hy_s0603_write_register(vi_pipe, 0x3657, 0x0f);
    ret += hy_s0603_write_register(vi_pipe, 0x3658, 0x3d);
    ret += hy_s0603_write_register(vi_pipe, 0x365c, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x365e, 0x68);
    ret += hy_s0603_write_register(vi_pipe, 0x3901, 0x04);
    ret += hy_s0603_write_register(vi_pipe, 0x3904, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3905, 0x91);
    ret += hy_s0603_write_register(vi_pipe, 0x391e, 0x83);
    ret += hy_s0603_write_register(vi_pipe, 0x3928, 0x04);
    ret += hy_s0603_write_register(vi_pipe, 0x3933, 0xa0);
    ret += hy_s0603_write_register(vi_pipe, 0x3934, 0x0a);
    ret += hy_s0603_write_register(vi_pipe, 0x3935, 0x68);
    ret += hy_s0603_write_register(vi_pipe, 0x3936, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3937, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3938, 0x0a);
    ret += hy_s0603_write_register(vi_pipe, 0x3946, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3961, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3962, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3963, 0xc8);
    ret += hy_s0603_write_register(vi_pipe, 0x3964, 0xc8);
    ret += hy_s0603_write_register(vi_pipe, 0x3965, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3966, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3967, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x39cd, 0xc8);
    ret += hy_s0603_write_register(vi_pipe, 0x39ce, 0xc8);
    ret += hy_s0603_write_register(vi_pipe, 0x3e01, 0x82);
    ret += hy_s0603_write_register(vi_pipe, 0x3e02, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e0e, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x3e0f, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e1c, 0x0f);
    ret += hy_s0603_write_register(vi_pipe, 0x3e23, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e24, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e53, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e54, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e68, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e69, 0x80);
    ret += hy_s0603_write_register(vi_pipe, 0x3e73, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e74, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e86, 0x03);
    ret += hy_s0603_write_register(vi_pipe, 0x3e87, 0x40);

    return ret;
}

static td_s32 hy_s0603_linear_8m30_12bit_init_part4(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += hy_s0603_write_register(vi_pipe, 0x3f02, 0x24);
    ret += hy_s0603_write_register(vi_pipe, 0x4424, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x4501, 0xc4);
    ret += hy_s0603_write_register(vi_pipe, 0x4509, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x4561, 0x12);
    ret += hy_s0603_write_register(vi_pipe, 0x4800, 0x24);
    ret += hy_s0603_write_register(vi_pipe, 0x4837, 0x0f);
    ret += hy_s0603_write_register(vi_pipe, 0x4900, 0x24);
    ret += hy_s0603_write_register(vi_pipe, 0x4937, 0x0f);
    ret += hy_s0603_write_register(vi_pipe, 0x5000, 0x0e);
    ret += hy_s0603_write_register(vi_pipe, 0x500f, 0x35);
    ret += hy_s0603_write_register(vi_pipe, 0x5020, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x5787, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x5788, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x5789, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x578a, 0x18);
    ret += hy_s0603_write_register(vi_pipe, 0x578b, 0x0c);
    ret += hy_s0603_write_register(vi_pipe, 0x578c, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x5790, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x5791, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x5792, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x5793, 0x18);
    ret += hy_s0603_write_register(vi_pipe, 0x5794, 0x0c);
    ret += hy_s0603_write_register(vi_pipe, 0x5795, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x5799, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x57a2, 0x60);
    ret += hy_s0603_write_register(vi_pipe, 0x59e0, 0xfe);
    ret += hy_s0603_write_register(vi_pipe, 0x59e1, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x59e2, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59e3, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59e4, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x59e5, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59e6, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59e7, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x59e8, 0x3f);
    ret += hy_s0603_write_register(vi_pipe, 0x59e9, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59ea, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59eb, 0x3f);
    ret += hy_s0603_write_register(vi_pipe, 0x59ec, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59ed, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59ee, 0xfe);
    ret += hy_s0603_write_register(vi_pipe, 0x59ef, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x59f4, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59f5, 0x30);

    return ret;
}

static td_s32 hy_s0603_linear_8m30_12bit_init_part5(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += hy_s0603_write_register(vi_pipe, 0x59f6, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x59f7, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59f8, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59f9, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x59fa, 0x3f);
    ret += hy_s0603_write_register(vi_pipe, 0x59fb, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59fc, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59fd, 0x3f);
    ret += hy_s0603_write_register(vi_pipe, 0x59fe, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59ff, 0x30);

    ret += hy_s0603_write_register(vi_pipe, 0x5000, 0x0); /* sensor dpc off */
    ret += hy_s0603_write_register(vi_pipe, 0x5002, 0x0);
    ret += hy_s0603_write_register(vi_pipe, 0x33b0, 0x0);

    ret += hy_s0603_write_register(vi_pipe, 0x320a, 0x08); /* OB Area */
    ret += hy_s0603_write_register(vi_pipe, 0x320b, 0x90);
    ret += hy_s0603_write_register(vi_pipe, 0x3900, 0x07);
    ret += hy_s0603_write_register(vi_pipe, 0x3902, 0xd0);
    ret += hy_s0603_write_register(vi_pipe, 0x3901, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x3904, 0x22);
    ret += hy_s0603_write_register(vi_pipe, 0x5010, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3213, 0x02);

    ret += hy_s0603_write_register(vi_pipe, 0x0100, 0x01);

    return ret;
}


static void hy_s0603_linear_8m30_12bit_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += hy_s0603_linear_8m30_12bit_init_part1(vi_pipe);
    ret += hy_s0603_linear_8m30_12bit_init_part2(vi_pipe);
    ret += hy_s0603_linear_8m30_12bit_init_part3(vi_pipe);
    ret += hy_s0603_linear_8m30_12bit_init_part4(vi_pipe);
    ret += hy_s0603_linear_8m30_12bit_init_part5(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("hy_s0603 write register failed!\n");
        return;
    }

    printf("========================================================================\n");
    printf("vi_pipe:%d,== HY_S0603_MIPI_27MInput_4lane_1080Mbps_12bit_30fps_3840x2160 Init OK! ==\n", vi_pipe);
    printf("========================================================================\n");
    return;
}

static td_s32 hy_s0603_vc_wdr_2t1_8m30_10bit_init_part1(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += hy_s0603_write_register(vi_pipe, 0x0103, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x0100, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x36e9, 0x80);
    ret += hy_s0603_write_register(vi_pipe, 0x36f9, 0x80);
    ret += hy_s0603_write_register(vi_pipe, 0x36ea, 0x09);
    ret += hy_s0603_write_register(vi_pipe, 0x36eb, 0x0c);
    ret += hy_s0603_write_register(vi_pipe, 0x36ec, 0x4b);
    ret += hy_s0603_write_register(vi_pipe, 0x36ed, 0x34);
    ret += hy_s0603_write_register(vi_pipe, 0x36fa, 0x0b);
    ret += hy_s0603_write_register(vi_pipe, 0x36fb, 0x13);
    ret += hy_s0603_write_register(vi_pipe, 0x36fc, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x36fd, 0x17);
    ret += hy_s0603_write_register(vi_pipe, 0x36e9, 0x24);
    ret += hy_s0603_write_register(vi_pipe, 0x36f9, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3018, 0x7a);
    ret += hy_s0603_write_register(vi_pipe, 0x3019, 0xf0);
    ret += hy_s0603_write_register(vi_pipe, 0x301a, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x301e, 0x3c);
    ret += hy_s0603_write_register(vi_pipe, 0x301f, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x3028, 0x77);
    ret += hy_s0603_write_register(vi_pipe, 0x302a, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3031, 0x0a);
    ret += hy_s0603_write_register(vi_pipe, 0x3032, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3033, 0x22);
    ret += hy_s0603_write_register(vi_pipe, 0x3037, 0x60);
    ret += hy_s0603_write_register(vi_pipe, 0x303e, 0xb4);
    ret += hy_s0603_write_register(vi_pipe, 0x3201, 0x98);
    ret += hy_s0603_write_register(vi_pipe, 0x3203, 0x0c);
    ret += hy_s0603_write_register(vi_pipe, 0x3205, 0xa7);
    ret += hy_s0603_write_register(vi_pipe, 0x3207, 0x83);
    ret += hy_s0603_write_register(vi_pipe, 0x320c, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x320d, 0x26);
    ret += hy_s0603_write_register(vi_pipe, 0x320e, 0x11);
    ret += hy_s0603_write_register(vi_pipe, 0x320f, 0x94);
    ret += hy_s0603_write_register(vi_pipe, 0x3211, 0x08);
    ret += hy_s0603_write_register(vi_pipe, 0x3213, 0x2c);
    ret += hy_s0603_write_register(vi_pipe, 0x3223, 0xd0);
    ret += hy_s0603_write_register(vi_pipe, 0x3226, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x3227, 0x03);
    ret += hy_s0603_write_register(vi_pipe, 0x3230, 0x11);
    ret += hy_s0603_write_register(vi_pipe, 0x3231, 0x93);
    ret += hy_s0603_write_register(vi_pipe, 0x3250, 0xff);
    ret += hy_s0603_write_register(vi_pipe, 0x3253, 0x08);
    ret += hy_s0603_write_register(vi_pipe, 0x327e, 0x55);
    ret += hy_s0603_write_register(vi_pipe, 0x3280, 0x00);
    return ret;
}

static td_s32 hy_s0603_vc_wdr_2t1_8m30_10bit_init_part2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += hy_s0603_write_register(vi_pipe, 0x3281, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3301, 0x24);
    ret += hy_s0603_write_register(vi_pipe, 0x3304, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x3306, 0x54);
    ret += hy_s0603_write_register(vi_pipe, 0x3308, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x3309, 0x60);
    ret += hy_s0603_write_register(vi_pipe, 0x330a, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x330b, 0xa0);
    ret += hy_s0603_write_register(vi_pipe, 0x330d, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x3314, 0x92);
    ret += hy_s0603_write_register(vi_pipe, 0x331e, 0x29);
    ret += hy_s0603_write_register(vi_pipe, 0x331f, 0x59);
    ret += hy_s0603_write_register(vi_pipe, 0x3333, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x3347, 0x05);
    ret += hy_s0603_write_register(vi_pipe, 0x3348, 0x50);
    ret += hy_s0603_write_register(vi_pipe, 0x3352, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3356, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x335d, 0x60);
    ret += hy_s0603_write_register(vi_pipe, 0x3362, 0x70);
    ret += hy_s0603_write_register(vi_pipe, 0x338f, 0x80);
    ret += hy_s0603_write_register(vi_pipe, 0x33af, 0x48);
    ret += hy_s0603_write_register(vi_pipe, 0x33fe, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x3400, 0x12);
    ret += hy_s0603_write_register(vi_pipe, 0x3406, 0x04);
    ret += hy_s0603_write_register(vi_pipe, 0x3410, 0x12);
    ret += hy_s0603_write_register(vi_pipe, 0x3416, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x3433, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3440, 0x12);
    ret += hy_s0603_write_register(vi_pipe, 0x3446, 0x08);
    ret += hy_s0603_write_register(vi_pipe, 0x3478, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3479, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x347a, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x347b, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x347c, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x347d, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3616, 0x0c);
    ret += hy_s0603_write_register(vi_pipe, 0x3620, 0x94);
    ret += hy_s0603_write_register(vi_pipe, 0x3622, 0x74);
    ret += hy_s0603_write_register(vi_pipe, 0x3629, 0x74);
    ret += hy_s0603_write_register(vi_pipe, 0x362a, 0xf0);
    ret += hy_s0603_write_register(vi_pipe, 0x362b, 0x0f);
    ret += hy_s0603_write_register(vi_pipe, 0x362d, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3630, 0x68);
    ret += hy_s0603_write_register(vi_pipe, 0x3633, 0x24);
    ret += hy_s0603_write_register(vi_pipe, 0x3634, 0x22);
    return ret;
}

static td_s32 hy_s0603_vc_wdr_2t1_8m30_10bit_init_part3(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += hy_s0603_write_register(vi_pipe, 0x3635, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3637, 0x18);
    ret += hy_s0603_write_register(vi_pipe, 0x3638, 0x26);
    ret += hy_s0603_write_register(vi_pipe, 0x363b, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x363c, 0x08);
    ret += hy_s0603_write_register(vi_pipe, 0x363d, 0x05);
    ret += hy_s0603_write_register(vi_pipe, 0x363e, 0x8f);
    ret += hy_s0603_write_register(vi_pipe, 0x3648, 0xe0);
    ret += hy_s0603_write_register(vi_pipe, 0x3649, 0x0a);
    ret += hy_s0603_write_register(vi_pipe, 0x364a, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x364c, 0x6a);
    ret += hy_s0603_write_register(vi_pipe, 0x3650, 0x3d);
    ret += hy_s0603_write_register(vi_pipe, 0x3654, 0x70);
    ret += hy_s0603_write_register(vi_pipe, 0x3656, 0x68);
    ret += hy_s0603_write_register(vi_pipe, 0x3657, 0x0f);
    ret += hy_s0603_write_register(vi_pipe, 0x3658, 0x3d);
    ret += hy_s0603_write_register(vi_pipe, 0x365c, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x365e, 0x68);
    ret += hy_s0603_write_register(vi_pipe, 0x3901, 0x04);
    ret += hy_s0603_write_register(vi_pipe, 0x3902, 0xf1);
    ret += hy_s0603_write_register(vi_pipe, 0x3904, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3905, 0x91);
    ret += hy_s0603_write_register(vi_pipe, 0x391e, 0x03);
    ret += hy_s0603_write_register(vi_pipe, 0x391f, 0x11);
    ret += hy_s0603_write_register(vi_pipe, 0x3928, 0x04);
    ret += hy_s0603_write_register(vi_pipe, 0x3933, 0xa0);
    ret += hy_s0603_write_register(vi_pipe, 0x3934, 0x0a);
    ret += hy_s0603_write_register(vi_pipe, 0x3935, 0x68);
    ret += hy_s0603_write_register(vi_pipe, 0x3936, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3937, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3938, 0x0a);
    ret += hy_s0603_write_register(vi_pipe, 0x3946, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x3961, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3962, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3963, 0xc8);
    ret += hy_s0603_write_register(vi_pipe, 0x3964, 0xc8);
    ret += hy_s0603_write_register(vi_pipe, 0x3965, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3966, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3967, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x39cd, 0xc8);
    ret += hy_s0603_write_register(vi_pipe, 0x39ce, 0xc8);
    ret += hy_s0603_write_register(vi_pipe, 0x3e00, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3e01, 0x00);
    return ret;
}

static td_s32 hy_s0603_vc_wdr_2t1_8m30_10bit_init_part4(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += hy_s0603_write_register(vi_pipe, 0x3e02, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e04, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x3e05, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e0e, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x3e0f, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e1c, 0x0f);
    ret += hy_s0603_write_register(vi_pipe, 0x3e23, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x3e24, 0x0a);
    ret += hy_s0603_write_register(vi_pipe, 0x3e53, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e54, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e68, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e69, 0x80);
    ret += hy_s0603_write_register(vi_pipe, 0x3e73, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e74, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3e86, 0x03);
    ret += hy_s0603_write_register(vi_pipe, 0x3e87, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x3f02, 0x24);
    ret += hy_s0603_write_register(vi_pipe, 0x4424, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x4501, 0xb4);
    ret += hy_s0603_write_register(vi_pipe, 0x4503, 0x60);
    ret += hy_s0603_write_register(vi_pipe, 0x4509, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x4561, 0x12);
    ret += hy_s0603_write_register(vi_pipe, 0x4800, 0x24);
    ret += hy_s0603_write_register(vi_pipe, 0x4837, 0x16);
    ret += hy_s0603_write_register(vi_pipe, 0x4853, 0xf8);
    ret += hy_s0603_write_register(vi_pipe, 0x4900, 0x24);
    ret += hy_s0603_write_register(vi_pipe, 0x4937, 0x16);
    ret += hy_s0603_write_register(vi_pipe, 0x4953, 0xf8);
    ret += hy_s0603_write_register(vi_pipe, 0x5000, 0x0e);
    ret += hy_s0603_write_register(vi_pipe, 0x500f, 0x35);
    ret += hy_s0603_write_register(vi_pipe, 0x5020, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x5787, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x5788, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x5789, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x578a, 0x18);
    ret += hy_s0603_write_register(vi_pipe, 0x578b, 0x0c);
    ret += hy_s0603_write_register(vi_pipe, 0x578c, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x5790, 0x10);
    ret += hy_s0603_write_register(vi_pipe, 0x5791, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x5792, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x5793, 0x18);
    ret += hy_s0603_write_register(vi_pipe, 0x5794, 0x0c);
    return ret;
}

static td_s32 hy_s0603_vc_wdr_2t1_8m30_10bit_init_part5(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += hy_s0603_write_register(vi_pipe, 0x5795, 0x01);
    ret += hy_s0603_write_register(vi_pipe, 0x5799, 0x06);
    ret += hy_s0603_write_register(vi_pipe, 0x57a2, 0x60);
    ret += hy_s0603_write_register(vi_pipe, 0x59e0, 0xfe);
    ret += hy_s0603_write_register(vi_pipe, 0x59e1, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x59e2, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59e3, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59e4, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x59e5, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59e6, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59e7, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x59e8, 0x3f);
    ret += hy_s0603_write_register(vi_pipe, 0x59e9, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59ea, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59eb, 0x3f);
    ret += hy_s0603_write_register(vi_pipe, 0x59ec, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59ed, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59ee, 0xfe);
    ret += hy_s0603_write_register(vi_pipe, 0x59ef, 0x40);
    ret += hy_s0603_write_register(vi_pipe, 0x59f4, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59f5, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59f6, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x59f7, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59f8, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59f9, 0x20);
    ret += hy_s0603_write_register(vi_pipe, 0x59fa, 0x3f);
    ret += hy_s0603_write_register(vi_pipe, 0x59fb, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59fc, 0x30);
    ret += hy_s0603_write_register(vi_pipe, 0x59fd, 0x3f);
    ret += hy_s0603_write_register(vi_pipe, 0x59fe, 0x38);
    ret += hy_s0603_write_register(vi_pipe, 0x59ff, 0x30);

    ret += hy_s0603_write_register(vi_pipe, 0x320a, 0x08); /* OB Area */
    ret += hy_s0603_write_register(vi_pipe, 0x320b, 0x90);
    ret += hy_s0603_write_register(vi_pipe, 0x3900, 0x07);
    ret += hy_s0603_write_register(vi_pipe, 0x3902, 0xd0);
    ret += hy_s0603_write_register(vi_pipe, 0x3901, 0x02);
    ret += hy_s0603_write_register(vi_pipe, 0x3904, 0x22);
    ret += hy_s0603_write_register(vi_pipe, 0x5010, 0x00);
    ret += hy_s0603_write_register(vi_pipe, 0x3213, 0x02);

    delay_ms(1);
    ret += hy_s0603_write_register(vi_pipe, 0x0100, 0x01);

    return ret;
}

static void hy_s0603_vc_wdr_2t1_8m30_10bit_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += hy_s0603_vc_wdr_2t1_8m30_10bit_init_part1(vi_pipe);
    ret += hy_s0603_vc_wdr_2t1_8m30_10bit_init_part2(vi_pipe);
    ret += hy_s0603_vc_wdr_2t1_8m30_10bit_init_part3(vi_pipe);
    ret += hy_s0603_vc_wdr_2t1_8m30_10bit_init_part4(vi_pipe);
    ret += hy_s0603_vc_wdr_2t1_8m30_10bit_init_part5(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("hy_s0603 write register failed!\n");
        return;
    }

    printf("==========================================================================================\n");
    printf("vi_pipe:%d,== HY_S0603_MIPI_27MInput_4lane_1458Mbps_10bit_WDR2T1_30fps_3840x2160 Init OK! ==\n", vi_pipe);
    printf("==========================================================================================\n");
    return;
}
