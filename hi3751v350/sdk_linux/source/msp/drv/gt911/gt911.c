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

#include <linux/delay.h>
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/property.h>
#include <asm/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/slab.h>
#include <linux/string.h>
#include "hi_drv_i2c.h"
#include "hi_osal.h"
#include "hi_module.h"
#include "hi_drv_gpio.h"

#define FST_ADDR (0xBB)
#define DEFAULT_MD_LEN (128)

#define I2C_NUM (1)
#define I2C_BYTE_NUM (2)
#define I2C1_SDA_REG 0xF8A21000
#define I2C1_SCL_REG 0xF8A21004

#define RST_GPIO_NUM (33) // GPIO4_1 = 4*8+1=33
#define INT_GPIO_NUM (34) // GPIO4_2 = 4*8+2=34
#define RST_GPIO_NUM_REG 0xF8A21470
#define INT_GPIO_NUM_REG 0xF8A21474

/* *screen resolution */
#define GT_BIT0 0x01
#define GT_BIT1 0x02
#define GT_DATA_RECVIVED 0x80
#define GT_FINGGER_DATA_NUM (8)
#define GT_STAY_HIGH_TIME (100)
#define GT_STAY_LOW_TIME (20)
#define GT_BUFFER_STAT_ADDR (0x814E)
#define GT_FINGGER_DATA_BASE (0x814F)
#define GT_COORDINATE_X_LOWBYPE_BASE (0x8150)
#define GT_COORDINATE_X_HIGHBYPE_BASE (0x8151)
#define GT_COORDINATE_Y_LOWBYPE_BASE (0x8152)
#define GT_COORDINATE_Y_HIGHBYPE_BASE (0x8153)

#define TP_MAX_TOUCH_POINTS (5)
#define TP_OUCH_POINTS_LIMIT (5)
/* *screen resolution */
#define TP_SCREEN_WIDTH_NUM (1024)
#define TP_SCREEN_HEIGHT_NUM (600)
#define TP_EVENT_PRESS_DOWN (0)
#define TP_EVENT_LIFT_UP (1)
#define TP_EVENT_CONTACT (2)
#define TP_SLOT_REPORT 0
#define TP_BIT_OFFSET1 1
#define TP_BIT_OFFSET2 2
#define TP_BIT_OFFSET3 3
#define TP_BIT_OFFSET4 4
#define TP_BIT_OFFSET5 5
#define TP_BIT_OFFSET6 6
#define TP_BIT_OFFSET7 7
#define TP_BIT_OFFSET8 8
/* start define of ft */
volatile unsigned long gpio_base;
static i2c_ext_func *g_i2c_func = HI_NULL;
static gpio_ext_func *g_pst_gpio_func = HI_NULL;
static struct work_struct g_work;
static struct input_dev *g_ts_dev = HI_NULL;
static struct workqueue_struct *g_tp_workqueue;

extern hi_void hi_drv_gpio_set_irq_ctrl(hi_u32 gpio_no, hi_bool b_enable);

static const struct i2c_device_id ft_id[] = {
    { "ft", },
    { }
};

MODULE_DEVICE_TABLE(i2c, ft_id);

static hi_void tp_touch_down(struct input_dev *input_dev, hi_s32 id, hi_s32 x, hi_s32 y, hi_s32 w)
{
#if TP_SLOT_REPORT
    input_mt_slot(input_dev, id);
    input_report_abs(input_dev, ABS_MT_TRACKING_ID, id);
    input_report_abs(input_dev, ABS_MT_POSITION_X, x);
    input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
    input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, w);
    input_report_abs(input_dev, ABS_MT_WIDTH_MAJOR, w);
#else
    input_report_key(input_dev, BTN_TOUCH, 1);
    input_report_abs(input_dev, ABS_MT_POSITION_X, x);
    input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
    input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, w);
    input_report_abs(input_dev, ABS_MT_WIDTH_MAJOR, w);
    input_report_abs(input_dev, ABS_MT_TRACKING_ID, id);
    input_mt_sync(input_dev);
#endif
}

static hi_void tp_touch_up(struct input_dev *input_dev, hi_s32 id)
{
#if TP_SLOT_REPORT
    input_mt_slot(input_dev, id);
    input_report_abs(input_dev, ABS_MT_TRACKING_ID, -1);
#else
    input_report_key(input_dev, BTN_TOUCH, 0);
#endif
}

static hi_s32 tp_set_reg(hi_u32 addr, hi_u32 value)
{
    hi_void *pmem = ioremap(addr, DEFAULT_MD_LEN);
    if (pmem == HI_NULL) {
        return -1;
    }

    *(hi_u32 *)pmem = value;
    iounmap(pmem);
    return 0;
}

static hi_s32 tp_reset_buffer(hi_void)
{
    hi_s32 i;
    hi_u8 data[1] = {0};

    for (i = 0; i < TP_OUCH_POINTS_LIMIT; i++) {
        g_i2c_func->pfn_i2c_write(1, FST_ADDR, GT_BUFFER_STAT_ADDR, I2C_BYTE_NUM, data, 1);
    }
    return 0;
}

static void tp_report_by_slot(struct input_dev *input, hi_u16 *pretouch, hi_s32 fingernum)
{
    hi_s32 i;
    hi_s32 id = 0;
    hi_s32 input_x = 0;
    hi_s32 input_y = 0;
    hi_s32 input_w = 0;
    hi_s32 pos = 0;
    hi_u16 touch_index = 0;
    hi_u8 report_num = 0;
    hi_u8 finggerdata[GT_FINGGER_DATA_NUM * TP_MAX_TOUCH_POINTS] = {0};
    hi_s32 ret = g_i2c_func->pfn_i2c_read(I2C_NUM, FST_ADDR, GT_FINGGER_DATA_BASE, I2C_BYTE_NUM, finggerdata,
        GT_FINGGER_DATA_NUM * TP_MAX_TOUCH_POINTS);

    if (fingernum) {
        id = finggerdata[pos] & 0x0F;
        touch_index |= (0x01 << id);
    }

    for (i = 0; i < TP_MAX_TOUCH_POINTS; i++) {
        if ((touch_index & (0x01 << i))) {
            input_x = finggerdata[pos + TP_BIT_OFFSET1] | (finggerdata[pos + TP_BIT_OFFSET2] << TP_BIT_OFFSET8);
            input_y = finggerdata[pos + TP_BIT_OFFSET3] | (finggerdata[pos + TP_BIT_OFFSET4] << TP_BIT_OFFSET8);
            input_w = finggerdata[pos + TP_BIT_OFFSET5] | (finggerdata[pos + TP_BIT_OFFSET6] << TP_BIT_OFFSET8);

            tp_touch_down(input, id, input_x, input_y, input_w);
            *pretouch |= 0x01 << i;

            report_num++;
            if (report_num < fingernum) {
                pos += TP_BIT_OFFSET8;
                id = finggerdata[pos] & 0x0F;
                touch_index |= (0x01 << id);
            }
        } else {
            tp_touch_up(input, i);
            *pretouch &= ~(0x01 << i);
        }
    }
}

static void tp_report_by_point(struct input_dev *input, hi_u16 *pretouch, hi_s32 fingernum)
{
    hi_s32 i;
    hi_s32 id = 0;
    hi_s32 ret = 0;
    hi_s32 input_x = 0;
    hi_s32 input_y = 0;
    hi_s32 input_w = 0;
    if (fingernum > 0) {
        hi_u8 finggerdata[GT_FINGGER_DATA_NUM * TP_MAX_TOUCH_POINTS] = {0};
        ret = g_i2c_func->pfn_i2c_read(I2C_NUM, FST_ADDR, GT_FINGGER_DATA_BASE, I2C_BYTE_NUM, finggerdata,
            GT_FINGGER_DATA_NUM * TP_MAX_TOUCH_POINTS);

        for (i = 0; i < fingernum; i++) {
            /* *Notice the  array bound !! */
            input_x = (finggerdata[i * GT_FINGGER_DATA_NUM + TP_BIT_OFFSET1] & 0xFF) |
                ((finggerdata[i * GT_FINGGER_DATA_NUM + TP_BIT_OFFSET2] & 0xFF) << TP_BIT_OFFSET8);
            input_y = (finggerdata[i * GT_FINGGER_DATA_NUM + TP_BIT_OFFSET3] & 0xFF) |
                ((finggerdata[i * GT_FINGGER_DATA_NUM + TP_BIT_OFFSET4] & 0xFF) << TP_BIT_OFFSET8);
            input_w = (finggerdata[i * GT_FINGGER_DATA_NUM + TP_BIT_OFFSET5] & 0xFF) |
                ((finggerdata[i * GT_FINGGER_DATA_NUM + TP_BIT_OFFSET6] & 0xFF) << TP_BIT_OFFSET8);
            id = finggerdata[i * GT_FINGGER_DATA_NUM] & 0x0F;
            tp_touch_down(input, id, input_x, input_y, input_w);
        }
    } else if (*pretouch) {
        tp_touch_up(input, 0);
    }

    *pretouch = fingernum;
}

static hi_s32 tp_event_handler(struct input_dev *input)
{
    hi_u8 data = 0;
    hi_s32 fingernum = 0;
    static hi_u16 pretouch = 0;

    hi_s32 ret = g_i2c_func->pfn_i2c_read(I2C_NUM, FST_ADDR, GT_BUFFER_STAT_ADDR, I2C_BYTE_NUM, &data, 1);
    if (ret != 0) {
        printk("g_i2c_func->pfn_i2c_read failed \r\n");
        return 0;
    }

    if (data == 0) {
        return 0;
    }

    if ((data & GT_DATA_RECVIVED) == 0) {
        goto exit_work_func;
    }

    fingernum = data & (GT_BIT0 | GT_BIT1);

#if TP_SLOT_REPORT
    if (pretouch || fingernum) {
        tp_report_by_slot(input, &pretouch, fingernum);
    }
#else
    tp_report_by_point(input, &pretouch, fingernum);
#endif

    input_sync(input);

exit_work_func:
    tp_reset_buffer();
    return 0;
}

hi_void tp_irq_handler(hi_u32 irq)
{
    g_pst_gpio_func->pfn_gpio_set_int_enable(INT_GPIO_NUM, 0);
    queue_work(g_tp_workqueue, &g_work);
}

static hi_s32 tp_irq_register(hi_void)
{
    g_pst_gpio_func->pfn_gpio_set_int_type(INT_GPIO_NUM, GPIO_INTTYPE_UPDOWN);
    g_pst_gpio_func->pfn_gpio_register_server_func(INT_GPIO_NUM, tp_irq_handler);
    g_pst_gpio_func->pfn_gpio_clear_bit_int(INT_GPIO_NUM);
    g_pst_gpio_func->pfn_gpio_set_int_enable(INT_GPIO_NUM, 1);
    return 0;
}

static hi_s32 tp_devinput_init(hi_void)
{
    hi_s32 ret;
    g_ts_dev = input_allocate_device();
    if (g_ts_dev == HI_NULL) {
        printk(" func:%s line:%d \r\n", __FUNCTION__, __LINE__);
        return -1;
    }
    g_ts_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
    set_bit(EV_SYN, g_ts_dev->evbit);
    set_bit(EV_KEY, g_ts_dev->evbit);
    set_bit(EV_ABS, g_ts_dev->evbit);
    set_bit(INPUT_PROP_DIRECT, g_ts_dev->propbit);
    input_set_abs_params(g_ts_dev, ABS_MT_POSITION_X, 0, TP_SCREEN_WIDTH_NUM, 0, 0);
    input_set_abs_params(g_ts_dev, ABS_MT_POSITION_Y, 0, TP_SCREEN_HEIGHT_NUM, 0, 0);
    input_set_abs_params(g_ts_dev, ABS_MT_TOUCH_MAJOR, 0, 0xff, 0, 0);
    input_set_abs_params(g_ts_dev, ABS_MT_PRESSURE, 0, 0xff, 0, 0);
    input_set_abs_params(g_ts_dev, ABS_MT_TRACKING_ID, 0, 0xff, 0, 0);

#if TP_SLOT_REPORT
    input_mt_init_slots(g_ts_dev, 16, INPUT_MT_DIRECT | INPUT_MT_DROP_UNUSED); // 16 in case of "out of memory"
#else
    g_ts_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
#endif

    g_ts_dev->name = "tp";
    g_ts_dev->id.bustype = BUS_I2C;

    ret = input_register_device(g_ts_dev);
    if (ret != 0) {
        dev_err(HI_NULL, "failed to register input device: %d\n", ret);
        return ret;
    }

    ret = tp_irq_register();
    if (ret != 0) {
        dev_err(HI_NULL, "failed to register input device: %d\n", ret);
        return ret;
    }

    return 0;
}

static hi_void tp_ts_work_func(struct work_struct *work)
{
    struct input_dev *input = g_ts_dev;
    hi_drv_gpio_set_irq_ctrl(INT_GPIO_NUM, 0);
    tp_event_handler(input);

    g_pst_gpio_func->pfn_gpio_clear_bit_int(INT_GPIO_NUM);
    hi_drv_gpio_set_irq_ctrl(INT_GPIO_NUM, 1);
    return;
}

#ifdef MODULE
static hi_s32 __init tp_init(hi_void)
#else
hi_s32 tp_init(hi_void)
#endif
{
    hi_s32 ret = 0;
    tp_set_reg(RST_GPIO_NUM_REG, 0x0); // GPIO4_1 reset 0
    tp_set_reg(INT_GPIO_NUM_REG, 0x0); // GPIO4_2 hi_s32   0
    tp_set_reg(I2C1_SDA_REG, 0x03);    // GPIO0_4 I2C    011：I2C1_SDA；
    tp_set_reg(I2C1_SCL_REG, 0x03);    // GPIO0_5 I2C    011：I2C1_SCL；
    msleep(1);

    ret = hi_drv_i2c_init();
    if (ret != 0) {
        dev_err(HI_NULL, "I2C init failed: %d\n", ret);
        goto error_end;
    }

    gpio_drv_get_gpio_ext_func(&g_pst_gpio_func);
    if (g_pst_gpio_func == HI_NULL) {
        printk("get gpio export function failed!\n");
        goto error_end;
    }

    i2c_drv_get_i2c_ext_func(&g_i2c_func);
    if (g_i2c_func == HI_NULL) {
        printk("HI_ID_I2C handle get failed \r\n");
        return 0;
    }

    /* *set INT and GPIO to be output */
    g_pst_gpio_func->pfn_gpio_dir_set_bit(RST_GPIO_NUM, 0); // set GPIO1_0 Reset output
    g_pst_gpio_func->pfn_gpio_dir_set_bit(INT_GPIO_NUM, 0); // set GPIO1_1 INT output
    msleep(1);

    /* *Set Reset */
    g_pst_gpio_func->pfn_gpio_write_bit(RST_GPIO_NUM, 0); // set Reset low level
    g_pst_gpio_func->pfn_gpio_write_bit(INT_GPIO_NUM, 0); // set INT low level
    msleep(GT_STAY_LOW_TIME);

    g_pst_gpio_func->pfn_gpio_write_bit(RST_GPIO_NUM, 1); // set Reset high level
    msleep(GT_STAY_HIGH_TIME);

    g_pst_gpio_func->pfn_gpio_dir_set_bit(INT_GPIO_NUM, 1); // set GPIO1_1 INT input

    g_tp_workqueue = create_singlethread_workqueue("g_tp_workqueue");
    if (!g_tp_workqueue) {
        printk("Create workqueue failed.");
        return -ENOMEM;
    }
    INIT_WORK(&g_work, tp_ts_work_func);
    ret = tp_devinput_init();
    if (ret != 0) {
        dev_err(HI_NULL, " tp_devinput_init fail!\n");
        goto error_end;
    }

    return 0;
error_end:
    return -1;
}

#ifdef MODULE
static hi_void __exit tp_exit(hi_void)
#else
hi_void tp_exit(hi_void)
#endif
{
    g_pst_gpio_func->pfn_gpio_set_int_enable(INT_GPIO_NUM, 0);
    input_unregister_device(g_ts_dev);
    input_free_device(g_ts_dev);
    hi_drv_i2c_de_init();
    if (g_tp_workqueue) {
        destroy_workqueue(g_tp_workqueue);
    }
}

#if defined(MODULE) || defined(CFG_HI_USER_DRV)
module_init(tp_init);
module_exit(tp_exit);
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hisilicon");
