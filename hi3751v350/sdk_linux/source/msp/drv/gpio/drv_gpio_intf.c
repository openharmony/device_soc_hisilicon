/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/module.h>
#include <linux/version.h>
#include "securec.h"
#include "hi_log.h"
#include "hi_common.h"
#include "hi_error_mpi.h"
#include "drv_gpio.h"
#include "drv_gpio_ioctl.h"

#ifndef __KERNEL__
#ifdef HI_MOD_GPIO
#undef HI_MOD_GPIO
#define HI_MOD_GPIO "gpio_user"
#endif
#endif

#define GPIO_MAX_NUM 1
static osal_semaphore g_gpio_sem_intf;

typedef struct {
    hi_u32 cmd;
    hi_u32 group;
    hi_u32 bit;
    hi_u32 bit_value;
} gpio_proc_data;

static hi_void gpio_proc_write_cmd_process(hi_u32 cmd, hi_u32 group, hi_u32 bit, hi_u32 bit_value)
{
    hi_u32 ret = HI_SUCCESS;

    switch (cmd) {
        case 0: { // u32cmd为0，获取当前io的输入输出状态
            hi_u32 input = 0;

            ret = hi_drv_gpio_get_dir_bit(group * 8 + bit, &input); // 每组8个io口
            if (ret != HI_SUCCESS) {
                HI_PRINT("read dir error , group is %d , bit is %d\n", group, bit);
                break;
            }

            HI_PRINT("dir[%d][%d] = %s\n", group, bit, input ? "input" : "output");

            break;
        }
        case 1: { // u32cmd为1，设置io口输入输出状态
            ret = hi_drv_gpio_set_dir_bit(group * 8 + bit, bit_value); // 每组8个io口
            if (ret != HI_SUCCESS) {
                HI_PRINT("write dir error, group is %d , bit is %d\n", group, bit);
                break;
            }

            HI_PRINT("dir[%d][%d] = %s\n", group, bit, bit_value ? "input" : "output");

            break;
        }
        case 2: { // u32cmd为2，读取io口数据
            hi_u32 value = 0;

            ret = hi_drv_gpio_read_bit(group * 8 + bit, &value); // 每组8个io口
            if (ret != HI_SUCCESS) {
                HI_PRINT("read value error, group is %d , bit is %d\n", group, bit);
                break;
            }

            HI_PRINT("Value [%d][%d] = %s\n", group, bit, value ? "high" : "low");

            break;
        }
        case 3: { // u32cmd为3，对io口置位
            ret = hi_drv_gpio_write_bit(group * 8 + bit, bit_value); // 每组8个io口
            if (ret != HI_SUCCESS) {
                HI_PRINT("write value error, group is %d , bit is %d\n", group, bit);
                break;
            }

            HI_PRINT("Value [%d][%d] = %s\n", group, bit, bit_value ? "high" : "low");
            break;
        }
        default: {
            return;
        }
    }
}

static hi_s32 gpio_proc_read(hi_void *seqfile, hi_void *private)
{
    return HI_SUCCESS;
}

static hi_s32 gpio_drv_proc_help(unsigned int argc, char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], hi_void *private)
{
    HI_PRINT("---------------------------------Hisilicon GPIO Debug--------------------------------\n");
    HI_PRINT("echo command para1 para2         path          explanation\n");
    HI_PRINT("echo rdir  [group][bit]       > /proc/msp/gpio Read gpio direction\n");
    HI_PRINT("echo wdir  [group][bit] [0/1] > /proc/msp/gpio Write gpio direction,1:input 0:output\n");
    HI_PRINT("echo read  [group][bit]       > /proc/msp/gpio Read gpio level\n");
    HI_PRINT("echo write [group][bit] [0/1] > /proc/msp/gpio Write gpio output level\n");
    HI_PRINT("-------------------------------------------------------------------------------------\n");
    return HI_SUCCESS;
}

static hi_s32 gpio_drv_proc_rdir(unsigned int argc, char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], hi_void *private)
{
    gpio_proc_data proc_data = {0};

    if (argc < 3) { // num of str is 3
        HI_PRINT("---------------------------------Hisilicon GPIO Debug--------------------------------\n");
        HI_PRINT("echo command para1 para2         path          explanation\n");
        HI_PRINT("echo rdir  [group][bit]       > /proc/msp/gpio Read gpio direction\n");
        HI_PRINT("echo wdir  [group][bit] [0/1] > /proc/msp/gpio Write gpio direction,1:input 0:output\n");
        HI_PRINT("echo read  [group][bit]       > /proc/msp/gpio Read gpio level\n");
        HI_PRINT("echo write [group][bit] [0/1] > /proc/msp/gpio Write gpio output level\n");
        HI_PRINT("-------------------------------------------------------------------------------------\n");
        return HI_FAILURE;
    }

    proc_data.group = osal_strtol(argv[1], NULL, 10); // 字符串转成10进制
    proc_data.bit = osal_strtol(argv[2], NULL, 10); // 第2组数据转成10进制
    proc_data.cmd = 0;
    gpio_proc_write_cmd_process(proc_data.cmd, proc_data.group, proc_data.bit, proc_data.bit_value);
    return HI_SUCCESS;
}

static hi_s32 gpio_drv_proc_wdir(unsigned int argc, char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], hi_void *private)
{
    gpio_proc_data proc_data = {0};

    if (argc < 4) { // num of  str is 4
        HI_PRINT("---------------------------------Hisilicon GPIO Debug--------------------------------\n");
        HI_PRINT("echo command para1 para2         path          explanation\n");
        HI_PRINT("echo rdir  [group][bit]       > /proc/msp/gpio Read gpio direction\n");
        HI_PRINT("echo wdir  [group][bit] [0/1] > /proc/msp/gpio Write gpio direction,1:input 0:output\n");
        HI_PRINT("echo read  [group][bit]       > /proc/msp/gpio Read gpio level\n");
        HI_PRINT("echo write [group][bit] [0/1] > /proc/msp/gpio Write gpio output level\n");
        HI_PRINT("-------------------------------------------------------------------------------------\n");
        return HI_FAILURE;
    }

    proc_data.group = osal_strtol(argv[1], NULL, 10); // 字符串转成10进制
    proc_data.bit = osal_strtol(argv[2], NULL, 10); // 第2组数据转成10进制
    proc_data.bit_value = osal_strtol(argv[3], NULL, 10); // 第3组数据转成10进制
    proc_data.cmd = 1; // 1 :  set dir bit

    if (proc_data.bit_value > 1) {
        HI_PRINT("please set value is 0 or 1\n");
        return HI_FAILURE;
    }

    gpio_proc_write_cmd_process(proc_data.cmd, proc_data.group, proc_data.bit, proc_data.bit_value);
    return HI_SUCCESS;
}

static hi_s32 gpio_drv_proc_read(unsigned int argc, char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], hi_void *private)
{
    gpio_proc_data proc_data = {0};

    if (argc < 3) { // num of  str is 3
        HI_PRINT("---------------------------------Hisilicon GPIO Debug--------------------------------\n");
        HI_PRINT("echo command para1 para2         path          explanation\n");
        HI_PRINT("echo rdir  [group][bit]       > /proc/msp/gpio Read gpio direction\n");
        HI_PRINT("echo wdir  [group][bit] [0/1] > /proc/msp/gpio Write gpio direction,1:input 0:output\n");
        HI_PRINT("echo read  [group][bit]       > /proc/msp/gpio Read gpio level\n");
        HI_PRINT("echo write [group][bit] [0/1] > /proc/msp/gpio Write gpio output level\n");
        HI_PRINT("-------------------------------------------------------------------------------------\n");
        return HI_FAILURE;
    }
    proc_data.group = osal_strtol(argv[1], NULL, 10); // 字符串转成10进制
    proc_data.bit = osal_strtol(argv[2], NULL, 10); // 第2组数据转成10进制
    proc_data.cmd = 2; // 2 :  read gpio bit

    gpio_proc_write_cmd_process(proc_data.cmd, proc_data.group, proc_data.bit, proc_data.bit_value);
    return HI_SUCCESS;
}

static hi_s32 gpio_drv_proc_write(unsigned int argc, char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], hi_void *private)
{
    gpio_proc_data proc_data = {0};

    if (argc < 4) { // num of  str is 4
        HI_PRINT("---------------------------------Hisilicon GPIO Debug--------------------------------\n");
        HI_PRINT("echo command para1 para2         path          explanation\n");
        HI_PRINT("echo rdir  [group][bit]       > /proc/msp/gpio Read gpio direction\n");
        HI_PRINT("echo wdir  [group][bit] [0/1] > /proc/msp/gpio Write gpio direction,1:input 0:output\n");
        HI_PRINT("echo read  [group][bit]       > /proc/msp/gpio Read gpio level\n");
        HI_PRINT("echo write [group][bit] [0/1] > /proc/msp/gpio Write gpio output level\n");
        HI_PRINT("-------------------------------------------------------------------------------------\n");
        return HI_FAILURE;
    }

    proc_data.group = osal_strtol(argv[1], NULL, 10); // 字符串转成10进制
    proc_data.bit = osal_strtol(argv[2], NULL, 10); // 第2组数据转成10进制
    proc_data.bit_value = osal_strtol(argv[3], NULL, 10); // 第3组数据转成10进制
    proc_data.cmd = 3; // 3 :  write gpio bit

    if (proc_data.bit_value > 1) {
        HI_PRINT("please set value is 0 or 1\n");
        return HI_FAILURE;
    }

    gpio_proc_write_cmd_process(proc_data.cmd, proc_data.group, proc_data.bit, proc_data.bit_value);
    return HI_SUCCESS;
}

static osal_proc_cmd g_gpio_proc_cmd[] = {
    {"help", gpio_drv_proc_help},
    {"rdir",  gpio_drv_proc_rdir},
    {"wdir", gpio_drv_proc_wdir},
    {"read", gpio_drv_proc_read},
    {"write", gpio_drv_proc_write},
};

typedef struct hi_gpio_state {
    hi_handle gpio_handle[GPIO_MAX_NUM];
} gpio_state;

static gpio_state g_gpio_state;

static hi_s32 gpio_drv_lowpower_enter(hi_void *private_data)
{
    HI_LOG_NOTICE("Gpio lowpower enter OK\n");
    return HI_SUCCESS;
}

static hi_s32 gpio_drv_lowpower_exit(hi_void *private_data)
{
    HI_LOG_NOTICE("Gpio lowpower exit OK\n");
    return HI_SUCCESS;
}

static HI_S32 gpio_suspend(hi_void *private_data)
{
    HI_PRINT("GPIO suspend OK\n");
    return 0;
}

static HI_S32 gpio_resume(hi_void *private_data)
{
    HI_PRINT("GPIO resume OK\n");
    return 0;
}

static hi_s32 hi_gpio_ioctl_set_int_type(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    gpio_data st_data = { 0, 0, GPIO_INTTYPE_BUTT };
    errno_t err;

    if (arg == HI_NULL) {
        HI_LOG_ERR("arg is null!\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_gpio_sem_intf);
    if (ret) {
        HI_LOG_ERR("Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    err = memcpy_s(&st_data, sizeof(st_data), arg, sizeof(gpio_data));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    ret = hi_drv_gpio_set_int_type(st_data.gpio_no, st_data.en_int_type);

    osal_sem_up(&g_gpio_sem_intf);

    return ret;
}

static hi_s32 hi_gpio_ioctl_set_int_enable(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    gpio_data st_data = { 0, 0, GPIO_INTTYPE_BUTT };
    errno_t err;

    if (arg == HI_NULL) {
        HI_LOG_ERR("arg is null!\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_gpio_sem_intf);
    if (ret) {
        HI_LOG_ERR("Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    err = memcpy_s(&st_data, sizeof(st_data), arg, sizeof(gpio_data));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    if (st_data.b_enable == HI_TRUE) {
        ret = hi_drv_gpio_clear_bit_int(st_data.gpio_no);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERR("call hi_drv_gpio_clear_bit_int fail!\n");
            osal_sem_up(&g_gpio_sem_intf);
            return ret;
        }
    }

    ret = hi_drv_gpio_set_bit_int_enable(st_data.gpio_no, st_data.b_enable);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("call hi_drv_gpio_set_bit_int_enable fail!\n");
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    osal_sem_up(&g_gpio_sem_intf);
    return ret;
}

static hi_s32 hi_gpio_ioctl_get_int(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    gpio_int gpio_int_value = {0, 0};
    errno_t err;

    if (arg == HI_NULL) {
        HI_LOG_ERR("arg is null!\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_gpio_sem_intf);
    if (ret) {
        HI_LOG_ERR("Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    err = memcpy_s(&gpio_int_value, sizeof(gpio_int_value), arg, sizeof(gpio_int));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    ret = drv_gpio_query_int(&gpio_int_value);
    if (ret == HI_SUCCESS) {
        err = memcpy_s((gpio_int *)arg, sizeof(gpio_int), &gpio_int_value, sizeof(gpio_int_value));
        if (err != EOK) {
            HI_LOG_ERR("memcpy_s fail!\n");
            ret = HI_FAILURE;
            osal_sem_up(&g_gpio_sem_intf);
            return ret;
        }
    } else {
        ret = HI_ERR_GPIO_FAILED_GETINT;
    }
    osal_sem_up(&g_gpio_sem_intf);
    return ret;
}

static hi_s32 hi_gpio_ioctl_get_gpio_num(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    gpio_get_gpionum gpio_num = { 0, 0 };
    errno_t err;

    if (arg == HI_NULL) {
        HI_LOG_ERR("arg is null!\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_gpio_sem_intf);
    if (ret) {
        HI_LOG_ERR("Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    hi_drv_gpio_get_gpio_num(&gpio_num);

    err = memcpy_s((gpio_get_gpionum *)arg, sizeof(gpio_get_gpionum), &gpio_num, sizeof(gpio_num));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }
    ret = HI_SUCCESS;
    osal_sem_up(&g_gpio_sem_intf);
    return ret;
}

static hi_s32 hi_gpio_ioctl_set_output_type(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    gpio_output_type st_output_type = { 0, GPIO_OUTPUTTYPE_BUTT };
    errno_t err;

    if (arg == HI_NULL) {
        HI_LOG_ERR("arg is null!\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_gpio_sem_intf);
    if (ret) {
        HI_LOG_ERR("Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    err = memcpy_s(&st_output_type, sizeof(st_output_type),
        (gpio_output_type *)(uintptr_t)arg, sizeof(gpio_output_type));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    ret = drv_gpio_set_output_type(st_output_type.gpio_no, st_output_type.en_output_type);

    osal_sem_up(&g_gpio_sem_intf);
    return ret;
}


static hi_s32 hi_gpio_ioctl_get_output_type(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    gpio_output_type st_output_type = { 0, GPIO_OUTPUTTYPE_BUTT };
    errno_t err;

    if (arg == HI_NULL) {
        HI_LOG_ERR("arg is null!\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_gpio_sem_intf);
    if (ret) {
        HI_LOG_ERR("Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    err = memcpy_s(&st_output_type, sizeof(st_output_type), arg, sizeof(gpio_output_type));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    ret = drv_gpio_get_output_type(st_output_type.gpio_no, &st_output_type.en_output_type);

    err = memcpy_s((gpio_output_type *)arg, sizeof(gpio_output_type), &st_output_type, sizeof(st_output_type));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    osal_sem_up(&g_gpio_sem_intf);
    return ret;
}

static hi_s32 hi_gpio_ioctl_set_direct(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    gpio_direct st_direct = { 0, HI_FALSE };
    errno_t err;

    if (arg == HI_NULL) {
        HI_LOG_ERR("arg is null!\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_gpio_sem_intf);
    if (ret) {
        HI_LOG_ERR("Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    err = memcpy_s(&st_direct, sizeof(st_direct), arg, sizeof(gpio_direct));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    ret = hi_drv_gpio_set_dir_bit(st_direct.gpio_no, st_direct.b_input);

    osal_sem_up(&g_gpio_sem_intf);

    return ret;
}

static hi_s32 hi_gpio_ioctl_get_direct(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    gpio_direct st_direct = { 0, HI_FALSE };
    errno_t err;
    hi_u32 st_direct_input_temp = st_direct.b_input;

    if (arg == HI_NULL) {
        HI_LOG_ERR("arg is null!\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_gpio_sem_intf);
    if (ret) {
        HI_LOG_ERR("Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    err = memcpy_s(&st_direct, sizeof(st_direct), arg, sizeof(gpio_direct));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    ret = hi_drv_gpio_get_dir_bit(st_direct.gpio_no, &st_direct_input_temp);

    err = memcpy_s((gpio_direct *)arg, sizeof(gpio_direct), &st_direct, sizeof(st_direct));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    osal_sem_up(&g_gpio_sem_intf);

    return ret;
}

static hi_s32 hi_gpio_ioctl_set_value(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    gpio_value st_value = { 0, HI_FALSE };
    errno_t err;

    if (arg == HI_NULL) {
        HI_LOG_ERR("arg is null!\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_gpio_sem_intf);
    if (ret) {
        HI_LOG_ERR("Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    err = memcpy_s(&st_value, sizeof(st_value), arg, sizeof(gpio_value));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    ret = hi_drv_gpio_write_bit(st_value.gpio_no, st_value.b_high_volt);

    osal_sem_up(&g_gpio_sem_intf);

    return ret;
}

static hi_s32 hi_gpio_ioctl_get_value(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    gpio_value st_value = { 0, HI_FALSE };
    errno_t err;
    hi_u32 st_value_high_volt_temp = st_value.b_high_volt;

    if (arg == HI_NULL) {
        HI_LOG_ERR("arg is null!\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_gpio_sem_intf);
    if (ret) {
        HI_LOG_ERR("Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    err = memcpy_s(&st_value, sizeof(st_value), arg, sizeof(gpio_value));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    ret = hi_drv_gpio_read_bit(st_value.gpio_no, &st_value_high_volt_temp);

    err = memcpy_s((gpio_value *)arg, sizeof(gpio_value), &st_value, sizeof(st_value));
    if (err != EOK) {
        HI_LOG_ERR("memcpy_s fail!\n");
        ret = HI_FAILURE;
        osal_sem_up(&g_gpio_sem_intf);
        return ret;
    }

    osal_sem_up(&g_gpio_sem_intf);

    return ret;
}

static osal_ioctl_cmd g_gpio_cmd_list[] = {
    { CMD_GPIO_SET_INT_TYPE,               hi_gpio_ioctl_set_int_type },
    { CMD_GPIO_SET_INT_ENABLE,           hi_gpio_ioctl_set_int_enable },
    { CMD_GPIO_GET_INT,                       hi_gpio_ioctl_get_int },
    { CMD_GPIO_GET_GPIONUM,               hi_gpio_ioctl_get_gpio_num },
    { CMD_GPIO_SET_OUTPUTTYPE,          hi_gpio_ioctl_set_output_type },
    { CMD_GPIO_GET_OUTPUTTYPE,          hi_gpio_ioctl_get_output_type },
    { CMD_GPIO_SET_DIRECT,                  hi_gpio_ioctl_set_direct },
    { CMD_GPIO_GET_DIRECT,                  hi_gpio_ioctl_get_direct },
    { CMD_GPIO_SET_VALUE,                    hi_gpio_ioctl_set_value },
    { CMD_GPIO_GET_VALUE,                   hi_gpio_ioctl_get_value },
};

static osal_fileops g_gpio_fileops = {
    .read = NULL,
    .write = NULL,
    .open = drv_gpio_open,
    .release = drv_gpio_close,
    .cmd_list = g_gpio_cmd_list,
    .cmd_cnt = 0,
};

static osal_pmops g_gpio_pmops = {
    .pm_suspend = gpio_suspend,
    .pm_resume =  gpio_resume,
    .pm_lowpower_enter = gpio_drv_lowpower_enter,
    .pm_lowpower_exit = gpio_drv_lowpower_exit,
    .pm_poweroff = NULL,
    .private_data = &g_gpio_state,
};

static osal_dev g_gpio_device = {
    .minor = UMAP_MIN_MINOR_GPIO,
    .fops = &g_gpio_fileops,
    .pmops = &g_gpio_pmops,
};


hi_s32 gpio_drv_proc_add(hi_void)
{
    hi_s32 ret;
    HI_CHAR proc_name[16] = {0}; // 存放驱动名，最大长度16
    osal_proc_entry *gpio_proc_entry = NULL;
    hi_u32 len;
    ret = snprintf_s(proc_name, sizeof(proc_name), sizeof(proc_name) - 1, "%s", HI_MOD_GPIO);
    if (ret < 0) {
        HI_LOG_ERR("secure func call error\n");
        return HI_FAILURE;
    }

    len = strlen(proc_name);
    gpio_proc_entry = osal_proc_add(proc_name, len);
    if (gpio_proc_entry == HI_NULL) {
        HI_LOG_ERR("gpio add proc failed!\n");
        return HI_FAILURE;
    }

    len = sizeof(g_gpio_proc_cmd) / sizeof(osal_proc_cmd);
    gpio_proc_entry->read = gpio_proc_read;
    gpio_proc_entry->cmd_cnt = len;
    gpio_proc_entry->cmd_list = g_gpio_proc_cmd;
    gpio_proc_entry->private = NULL;

    return HI_SUCCESS;
}

hi_s32 gpio_drv_proc_del(hi_s8 *proc_name)
{
    hi_u32 len;

    if (proc_name == NULL) {
        HI_LOG_ERR("proc_name is null\n");
        return HI_FAILURE;
    }

    len = strlen(proc_name);
    osal_proc_remove(proc_name, len);
    return HI_SUCCESS;
}

HI_S32 gpio_drv_mod_init(hi_void)
{
    hi_s32 ret;

    osal_sem_init(&g_gpio_sem_intf, 1);

#ifndef HI_MCE_SUPPORT
#ifndef HI_KEYLED_CT1642_KERNEL_SUPPORT
    hi_drv_gpio_init();
#endif
#endif

    g_gpio_fileops.cmd_cnt = sizeof(g_gpio_cmd_list) / sizeof(osal_ioctl_cmd);

    ret = snprintf_s(g_gpio_device.name, sizeof(g_gpio_device.name),
        sizeof(g_gpio_device.name) - 1, "%s", UMAP_DEVNAME_GPIO);
    if (ret < 0) {
        HI_LOG_ERR("snprintf_s failed!\n");
        goto err0;
    }

    ret = osal_dev_register(&g_gpio_device);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("register gpio failed.\n");
        return HI_FAILURE;
    }

    ret = gpio_drv_proc_add();
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("call gpio_drv_proc_add failed!\n");
        osal_dev_unregister(&g_gpio_device);
        return HI_FAILURE;
    }

#ifdef MODULE
    HI_PRINT("Load hi_gpio.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;

err0:
#ifndef HI_MCE_SUPPORT
#ifndef HI_KEYLED_CT1642_KERNEL_SUPPORT
    hi_drv_gpio_de_init();
#endif
#endif
    return HI_FAILURE;
}

hi_void gpio_drv_mod_exit(hi_void)
{
    hi_s32 ret;
    errno_t sec_errno;
    HI_CHAR proc_name[16] = {0}; // 存放驱动名，最大长度16
    osal_sem_destory(&g_gpio_sem_intf);
    sec_errno = snprintf_s(proc_name, sizeof(proc_name), sizeof(proc_name) - 1, "%s", HI_MOD_GPIO);
    if (sec_errno < 0) {
        HI_LOG_ERR("secure func call error\n");
        return;
    }
    osal_dev_unregister(&g_gpio_device);
#if 1
    ret = gpio_drv_proc_del(proc_name);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("gpio_drv_proc_del failure!\n");
    }
#endif
#ifndef HI_MCE_SUPPORT
#ifndef HI_KEYLED_CT1642_KERNEL_SUPPORT
    hi_drv_gpio_de_init();
#endif
#endif

#ifdef MODULE
    HI_PRINT("remove hi_gpio.ko ok!\n");
#endif
    return;
}
#ifdef __KERNEL__
#if defined(MODULE) || defined(CFG_HI_USER_DRV)
module_init(gpio_drv_mod_init);
module_exit(gpio_drv_mod_exit);
#endif
#endif

MODULE_LICENSE("GPL");

