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
#include "drv_i2c.h"
#include "hi_drv_i2c.h"
#include "hi_drv_gpioi2c.h"
#include "drv_i2c_ioctl.h"
#include "hi_common.h"
#include "hi_module.h"
#include "hi_osal.h"

#define i2c_write_reg(Addr, Value) ((*(volatile hi_u32 *)(Addr)) = (Value))
#define i2c_read_reg(Addr)         (*(volatile hi_u32 *)(Addr))

static osal_dev g_i2c_register_data;
static unsigned int g_i2c_id = 0;
static gpio_i2c_ext_func *g_pst_gpio_i2c_ext_funcs = HI_NULL;

#ifdef HI_GPIOI2C_SUPPORT
extern i2c_gpio g_st_i2c_gpio[HI_I2C_MAX_NUM];
#endif

typedef struct {
    hi_u32 i2c_no;
    hi_u32 dev_addr;
    hi_u32 reg_addr;
    hi_u32 reg_addr_len;
    hi_u32 data_len;
} i2c_msg;

typedef struct {
    hi_u8 send_data[32]; // 发送数据缓存长度32
    hi_u32 len;
} send_msg;

static hi_void i2c_drv_proc_help(hi_void)
{
    HI_PRINT("\nUsage:\n");
    HI_PRINT("Read data: echo read 'bus' 'device address'\
                'Register address' 'Register address length' > /proc/msp/i2c\n");
    HI_PRINT("Write one Byte data: echo write 'bus'\
                'device address' 'Register address' 'Register address length' 'data' > /proc/msp/i2c\n");
    HI_PRINT("Write more Byte data:echo write 'bus' 'device address' 'Register address' 'Register address length'\
                'data number n(n<=32)' 'data1' ...'datan' > /proc/msp/i2c\n");
    HI_PRINT("Set Standard i2c rate: echo setrate 'bus'\
                'rate' > /proc/msp/i2c .such as:echo setrate 1 100000  > /proc/msp/i2c\n");
    HI_PRINT("Look over i2c info: cat /proc/msp/i2c\n");
}

static hi_s32 i2c_drv_proc_help_print(unsigned int argc,
    char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], hi_void *private)
{
    i2c_drv_proc_help();
    return HI_SUCCESS;
}

hi_s32 i2c_proc_read(hi_void *p, hi_void *v)
{
    hi_u8 ii = 0;
    hi_s32 i2c_rate = 0;
    if (p == HI_NULL) {
        HI_PRINT("p pointer is null\n");
        return HI_FAILURE;
    }

    osal_proc_print(p, "---------Hisilicon Standard I2C Info---------\n");
    osal_proc_print(p, "No.            Rate\n");

    for (ii = 0; ii < HI_STD_I2C_NUM; ii++) {
        i2c_rate = i2c_drv_get_rate(ii);
        osal_proc_print(p, "%hhu             %d\n", ii, i2c_rate);
    }

#ifdef HI_GPIOI2C_SUPPORT
    osal_proc_print(p, "---------Hisilicon GPIO simulate I2C Info---------\n");
    osal_proc_print(p, "No.            SCL_IO       SDA_IO\n");
    for (ii = 0; ii < HI_I2C_MAX_NUM; ii++) {
        if (g_st_i2c_gpio[ii].b_used) {
            osal_proc_print(p, "%d                %d           %d\n", g_st_i2c_gpio[ii].i2c_num,
                            g_st_i2c_gpio[ii].scl_gpio_no, g_st_i2c_gpio[ii].sda_gpio_no);
        }
    }
#endif

    return HI_SUCCESS;
}

hi_s32 i2c_proc_wr_set_rate(unsigned int arg_count,
                            char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], hi_void *private)
{
    hi_u32 i2c_no;
    hi_u32 rate;

    if (arg_count < 3) { // 至少是3个参数 setrate、i2c_no和rate
        i2c_drv_proc_help();
        return HI_FAILURE;
    }

    i2c_no = (hi_u32)osal_strtoul(argv[1], NULL, 10); // 命令的第1个参数 字符串转成10进制整数，获取i2c口编号
    rate = (hi_u32)osal_strtoul(argv[2], NULL, 10); // 命令的第2个参数 字符串转成10进制整数

    if (i2c_no >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("I2c NO.%u not support rate setting!\n", i2c_no);
        return HI_FAILURE;
    }
    i2c_drv_set_rate(i2c_no, rate);
    return HI_SUCCESS;
}

static hi_s32 i2c_proc_wr_read_get_dev_msg(hi_u8 arg_count,
                                           char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], i2c_msg *p_data)
{
    if ((p_data == HI_NULL) || (argv == HI_NULL)) {
        HI_PRINT("error param!!!\n");
        return HI_FAILURE;
    }

    if (arg_count < 5) { // 至少是5个参数
        i2c_drv_proc_help();
        return HI_FAILURE;
    }

    p_data->i2c_no  = (hi_u32)osal_strtoul(argv[1], NULL, 16); // 命令的第1个参数 字符串转成16进制的正整数
    p_data->dev_addr = (hi_u32)osal_strtoul(argv[2], NULL, 16); // 命令的第2个参数 字符串转成16进制的正整数
    p_data->reg_addr = (hi_u32)osal_strtoul(argv[3], NULL, 16); // 命令的第3个参数 字符串转成16进制的正整数
    p_data->reg_addr_len = (hi_u32)osal_strtoul(argv[4], NULL, 16); // 命令的第4个参数 字符串转成16进制的正整数
    p_data->data_len = (hi_u32)osal_strtoul(argv[5], NULL, 16); // 命令的第5个参数 字符串转成16进制的正整数

    hi_dbg_print_s32(p_data->i2c_no);
    hi_dbg_print_h32(p_data->dev_addr);
    hi_dbg_print_h32(p_data->reg_addr);
    hi_dbg_print_s32(p_data->reg_addr_len);
    hi_dbg_print_s32(p_data->data_len);

    return HI_SUCCESS;
}

static hi_s32 i2c_proc_wr_read(unsigned int arg_count,
    char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], hi_void *private)
{
    i2c_msg i2c_dev_msg = {0};
    hi_s32 ret;
    hi_u32 i = 0;
    hi_u8 p_data[32] = {0}; // 数据发送缓冲区长度32
    hi_bool b_used = HI_FALSE;
    hi_bool b_temp = HI_FALSE;

    ret = i2c_proc_wr_read_get_dev_msg(arg_count, argv, &i2c_dev_msg);
    if (ret != HI_SUCCESS) {
        HI_PRINT("i2c_proc_wr_read_get_dev_msg is failed!!!\n");
        return ret;
    }

    HI_I2C_ERR_PRINT(i2c_dev_msg.data_len >= sizeof(p_data), ret, "data len is too long\n\n");

    if ((i2c_dev_msg.i2c_no) < HI_STD_I2C_NUM) {
        ret = hi_drv_i2c_read(i2c_dev_msg.i2c_no, (hi_u8)(i2c_dev_msg.dev_addr), \
                              i2c_dev_msg.reg_addr, i2c_dev_msg.reg_addr_len, p_data, i2c_dev_msg.data_len);

        HI_I2C_ERR_PRINT(ret != HI_SUCCESS, ret, "Read failed\n\n");

        HI_PRINT("Read Data: \n");

        for (i = 0; i < i2c_dev_msg.data_len; i++) {
            HI_PRINT("0x%02x \n", p_data[i]);
        }

        HI_PRINT("\n");
    }
    else {
        if (g_pst_gpio_i2c_ext_funcs == HI_NULL) {
            ret = osal_exportfunc_get(HI_ID_GPIO_I2C, (hi_void **)&g_pst_gpio_i2c_ext_funcs);
            b_temp = (ret != HI_SUCCESS) || (g_pst_gpio_i2c_ext_funcs == HI_NULL);

            HI_I2C_ERR_PRINT(b_temp, ret, "GPIO_I2C Function ERR: ret:0x%08x, func:%p\n",
                             ret, g_pst_gpio_i2c_ext_funcs);
        }

        b_temp = g_pst_gpio_i2c_ext_funcs && g_pst_gpio_i2c_ext_funcs->pfn_gpio_i2c_is_used;
        HI_I2C_INFO_PRINT(!b_temp, ret, "Read failed(I2cNum=%d not valid)! \n", i2c_dev_msg.i2c_no);

        ret = g_pst_gpio_i2c_ext_funcs->pfn_gpio_i2c_is_used(i2c_dev_msg.i2c_no, &b_used);

        b_temp = (ret == HI_SUCCESS) && (b_used == HI_TRUE);
        HI_I2C_INFO_PRINT(!b_temp, ret, "Read failed(I2cNum=%d not valid)! \n", i2c_dev_msg.i2c_no);

        if (g_pst_gpio_i2c_ext_funcs->pfn_gpio_i2c_read_ext) {
            ret = g_pst_gpio_i2c_ext_funcs->pfn_gpio_i2c_read_ext(i2c_dev_msg.i2c_no, i2c_dev_msg.dev_addr, \
                                                                  i2c_dev_msg.reg_addr, i2c_dev_msg.reg_addr_len, \
                                                                  p_data, i2c_dev_msg.data_len);

            HI_I2C_ERR_PRINT(ret != HI_SUCCESS, ret, "Read failed\n");

            HI_PRINT("Read Data: \n");

            for (i = 0; i < i2c_dev_msg.data_len; i++) {
                HI_PRINT("0x%02x \n", p_data[i]);
            }

            HI_PRINT("\n");
        }
    }
    return ret;
}

static hi_s32 i2c_proc_wr_write_get_dev_msg(hi_u8 arg_count, char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX],
                                            send_msg *send, i2c_msg *p_data)
{
    hi_u32 val;
    hi_u32 i = 0;
    hi_bool b_temp = HI_FALSE;

    if ((p_data == HI_NULL) || (argv == HI_NULL)) {
        HI_PRINT("error param!!!\n");
        return HI_FAILURE;
    }

    if (arg_count < 6) { // 至少是6个参数
        i2c_drv_proc_help();
        return HI_FAILURE;
    }

    p_data->i2c_no = (hi_u32)osal_strtoul(argv[1], NULL, 16); // 命令的第1个参数 字符串转成16进制的正整数
    p_data->dev_addr = (hi_u32)osal_strtoul(argv[2], NULL, 16); // 命令的第2个参数 字符串转成16进制的正整数
    p_data->reg_addr = (hi_u32)osal_strtoul(argv[3], NULL, 16); // 命令的第3个参数 字符串转成16进制的正整数
    p_data->reg_addr_len = (hi_u32)osal_strtoul(argv[4], NULL, 16); // 命令的第4个参数 字符串转成16进制的正整数
    val = (hi_u32)osal_strtoul(argv[5], NULL, 16); // 命令的第5个参数 字符串转成16进制的正整数

    if (arg_count > 6) { // 第6个参数开始是数据
        b_temp = (val != (arg_count - 6)) || ((arg_count - 6) > sizeof(send->send_data)); // 表示第6个参数开始是数据

        if (b_temp) {
            i2c_drv_proc_help();
            return HI_FAILURE;
        }

        for (i = 0; i < val; i++) {
            send->send_data[i] =
                (hi_u8)osal_strtoul(argv[6 + i], NULL, 16); /* 第6个参数开始是数据 字符串转成16进制的正整数 */
            hi_dbg_print_s32(send->send_data[i]);
        }

        send->len = val;
    } else {
        send->send_data[0] = val;
        send->len = 1;
    }

    HI_PRINT("Write: \n");
    hi_dbg_print_s32(p_data->i2c_no);
    hi_dbg_print_h32(p_data->dev_addr);
    hi_dbg_print_h32(p_data->reg_addr);
    hi_dbg_print_s32(send->len);

    return HI_SUCCESS;
}

hi_s32 i2c_proc_wr_write(unsigned int arg_count,
                         char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], hi_void *private)
{
    hi_s32 ret;
    hi_bool b_temp = HI_FALSE;
    hi_bool b_used = HI_FALSE;

    send_msg send = {0};
    i2c_msg i2c_dev_msg = {0};

    ret = i2c_proc_wr_write_get_dev_msg(arg_count, argv, &send, &i2c_dev_msg);
    if (ret != HI_SUCCESS) {
        HI_PRINT("i2c_proc_wr_write_get_dev_msg is failed!!!\n");
        return ret;
    }

    if (i2c_dev_msg.i2c_no < HI_STD_I2C_NUM) {
        ret = hi_drv_i2c_write(i2c_dev_msg.i2c_no, (hi_u8)(i2c_dev_msg.dev_addr), \
                               i2c_dev_msg.reg_addr, i2c_dev_msg.reg_addr_len, send.send_data, send.len);
        HI_I2C_ERR_PRINT(ret != HI_SUCCESS, ret, "Write failed\n\n");
    }
    else {
        if (g_pst_gpio_i2c_ext_funcs == HI_NULL) {
            ret = osal_exportfunc_get(HI_ID_GPIO_I2C, (hi_void **)&g_pst_gpio_i2c_ext_funcs);

            b_temp = (ret != HI_SUCCESS) || (g_pst_gpio_i2c_ext_funcs == HI_NULL);

            HI_I2C_ERR_PRINT(b_temp, ret, "GPIO_I2C Function ERR: ret:0x%08x, func:%p\n",
                             ret, g_pst_gpio_i2c_ext_funcs);
        }

        b_temp = g_pst_gpio_i2c_ext_funcs && g_pst_gpio_i2c_ext_funcs->pfn_gpio_i2c_is_used;
        HI_I2C_INFO_PRINT(!b_temp, ret, "Write failed(I2cNum=%d not valid)! \n", i2c_dev_msg.i2c_no);

        ret = g_pst_gpio_i2c_ext_funcs->pfn_gpio_i2c_is_used(i2c_dev_msg.i2c_no, &b_used);

        b_temp = (ret == HI_SUCCESS) && (b_used == HI_TRUE);

        HI_I2C_INFO_PRINT(!b_temp, ret, "Write failed(I2cNum=%d not valid)! \n", i2c_dev_msg.i2c_no);

        if (g_pst_gpio_i2c_ext_funcs->pfn_gpio_i2c_write_ext) {
            ret = g_pst_gpio_i2c_ext_funcs->pfn_gpio_i2c_write_ext(i2c_dev_msg.i2c_no, i2c_dev_msg.dev_addr, \
                                                                   i2c_dev_msg.reg_addr, i2c_dev_msg.reg_addr_len,
                                                                   send.send_data, send.len);

            HI_I2C_ERR_PRINT(ret != HI_SUCCESS, ret, "Write failed(Ret:0x%x)\n\n", ret);
        }
    }
    return ret;
}

static osal_pmops g_i2c_pmops = {
    .pm_lowpower_enter = i2c_drv_lowpower_enter,
    .pm_lowpower_exit = i2c_drv_lowpower_exit,
    .pm_poweroff = NULL,
    .pm_suspend = i2c_pm_suspend,
    .pm_resume = i2c_pm_resume,
};


hi_s32 i2c_open(hi_void *private_data)
{
    hi_s32 ret;

    ret = osal_exportfunc_get(HI_ID_GPIO_I2C, (hi_void **)&g_pst_gpio_i2c_ext_funcs);
    if ((ret != HI_SUCCESS) || (g_pst_gpio_i2c_ext_funcs == HI_NULL)) {
        HI_LOG_INFO("Get GPIO_I2C Function ERR: ret:0x%08x, func:0x%08x\n", ret, g_pst_gpio_i2c_ext_funcs);
    }
    return HI_SUCCESS;
}

hi_s32 i2c_close(hi_void *private_data)
{
    return HI_SUCCESS;
}

static osal_ioctl_cmd g_i2c_ioctl_info[] = {
    {CMD_I2C_WRITE, hi_drv_i2c_cmd_write},
    {CMD_I2C_READ, hi_drv_i2c_cmd_read},
    {CMD_I2C_SET_RATE, hi_drv_i2c_cmd_set_rate},
    {CMD_I2C_CONFIG, hi_drv_gpioi2c_cmd_config},
    {CMD_I2C_DESTROY, hi_drv_gpioi2c_cmd_destroy},
    {CMD_I2C_READ_DIRECT, hi_drv_i2c_cmd_read}
};

static osal_proc_cmd g_i2c_proc_cmd[] = {
    {"help", i2c_drv_proc_help_print},
    {"read", i2c_proc_wr_read},
    {"write", i2c_proc_wr_write},
    {"setrate", i2c_proc_wr_set_rate},
};


static osal_fileops g_i2c_fops = {
    .open = i2c_open,
    .release = i2c_close,
    .cmd_list = g_i2c_ioctl_info,
    .cmd_cnt = 0,
};

hi_s32 i2c_drv_proc_add(hi_void)
{
    osal_proc_entry *i2c_proc_entry;

    g_i2c_id = HI_ID_I2C;

    i2c_proc_entry = osal_proc_add(HI_MOD_I2C, strlen(HI_MOD_I2C));
    if (i2c_proc_entry == HI_NULL) {
        HI_LOG_ERR("I2C add proc failed!\n");
        return HI_FAILURE;
    }

    i2c_proc_entry->read = i2c_proc_read;
    i2c_proc_entry->cmd_cnt = sizeof(g_i2c_proc_cmd) / sizeof(osal_proc_cmd);
    i2c_proc_entry->cmd_list = g_i2c_proc_cmd;
    i2c_proc_entry->private = &g_i2c_id;

    return HI_SUCCESS;
}


hi_s32 i2c_drv_mod_init(hi_void)
{
    hi_s32 ret;
    /*
    HI_MCE_SUPPORT=yes or HI_MCE_SUPPORT=no, all will call hi_drv_i2c_init,
    make sure HI_i2c_open call it
    */
    if (hi_drv_i2c_init() != HI_SUCCESS) {
        HI_LOG_FATAL("i2c init failed.\n");
        return HI_FAILURE;
    }

    g_i2c_fops.cmd_cnt = sizeof(g_i2c_ioctl_info) / sizeof(osal_ioctl_cmd);
    HI_LOG_INFO("g_i2c_fops cmd_cnt = %d\n", g_i2c_fops.cmd_cnt);

    /* I2C driver register */
    ret = snprintf_s(g_i2c_register_data.name, sizeof(g_i2c_register_data.name),
                     sizeof(g_i2c_register_data.name) - 1, UMAP_DEVNAME_I2C);
    if (ret < 0) {
        HI_LOG_ERR("secure func call error\n");
        return HI_FAILURE;
    }
    g_i2c_register_data.minor = UMAP_MIN_MINOR_I2C;
    g_i2c_register_data.fops = &g_i2c_fops;
    g_i2c_register_data.pmops = &g_i2c_pmops;
    if (osal_dev_register(&g_i2c_register_data) < 0) {
        HI_LOG_FATAL("register I2C failed.\n");
        return HI_FAILURE;
    }

    /* register i2c PROC function */
    ret = i2c_drv_proc_add();
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO("add I2C proc failed.\n");
        osal_dev_unregister(&g_i2c_register_data);
        return HI_FAILURE;
    }

#if defined(MODULE) || defined(CFG_HI_USER_DRV)
    HI_PRINT("Load hi_i2c.ko success.  \t(%s)\n", VERSION_STRING);
#endif
    return 0;
}

hi_void i2c_drv_mod_exit(hi_void)
{
    osal_proc_remove(HI_MOD_I2C, strlen(HI_MOD_I2C));
    osal_dev_unregister(&g_i2c_register_data);

    hi_drv_i2c_de_init();

    return;
}

#if defined(MODULE) || defined(CFG_HI_USER_DRV)
module_init(i2c_drv_mod_init);
module_exit(i2c_drv_mod_exit);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");

