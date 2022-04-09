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
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
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
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/i2c.h>
#include "hi_drv_i2c.h"
#include "hi_osal.h"
#include "hi_module.h"
#include "hi_drv_gpio.h"

#define I2C_NUM         (1)
#define FST_ADDR        (0xBB)

#define RST_GPIO_NUM   (33) // GPIO4_1 = 4*8+1=33
#define INT_GPIO_NUM    (34) // GPIO4_2 = 4*8+2=34
#define RST_GPIO_NUM_REG    0xF8A21470
#define INT_GPIO_NUM_REG    0xF8A21474
#define I2C1_SDA_REG    0xF8A21000
#define I2C1_SCL_REG    0xF8A21004

#define RST_GPIO_CHIP   (1)
#define RST_GPIO_OFFSET (0)
#define INT_GPIO_CHIP   (1)
#define INT_GPIO_OFFSET (1)
#define gpio_num(gpio_chip_num, gpio_offset_num)  ((gpio_chip_num) * 8 + (gpio_offset_num))

#define GT911_BIT0    0x01
#define GT911_BIT1    0x02
#define GT911_DATA_RECVIVED    0x80
#define FINGGER_DATA_NUM (4)
#define I2C_BYTE_NUM  (2)
#define INT_DELAY_TIME  (20)
#define GT911_STAY_HIGH_TIME  (100)
#define GT911_STAY_LOW_TIME  (20)

/* start define of ft */
static struct input_dev* tp_ts_dev = NULL;

volatile unsigned long gpio_base;

/**The TP can support 5 points, but we only use 1*/
#define MAX_TOUCH_POINTS             (1)
#define OUCH_POINTS_LIMIT             (5)

/**screen resolution*/
#define TP_SCREEN_WIDTH_NUM             (1024)
#define TP_SCREEN_HEIGHT_NUM            (600)

#define TP_EVENT_PRESS_DOWN             (0)
#define TP_EVENT_LIFT_UP                (1)
#define TP_EVENT_CONTACT                (2)

#define GT_BUFFER_STAT_ADDR             (0x814E)
#define GT_COORDINATE_X_LOWBYPE_BASE    (0x8150)
#define GT_COORDINATE_X_HIGHBYPE_BASE   (0x8151)
#define GT_COORDINATE_Y_LOWBYPE_BASE    (0x8152)
#define GT_COORDINATE_Y_HIGHBYPE_BASE   (0x8153)

#define DEFAULT_MD_LEN (128)

static const struct i2c_device_id ft_id[] = {
    { "ft", },
    { }
};

typedef struct tagts_event {
    u16 au16_x[MAX_TOUCH_POINTS]; /* x coordinate */
    u16 au16_y[MAX_TOUCH_POINTS]; /* y coordinate */
    u16 pressure[MAX_TOUCH_POINTS];
    u8 au8_touch_event[MAX_TOUCH_POINTS]; /* touch event: 0 -- down; 1-- up; 2 -- contact */
    u8 au8_finger_id[MAX_TOUCH_POINTS];   /* touch ID */
    u8 area[MAX_TOUCH_POINTS];
    u8 touch_point;
    u8 point_num;
} ts_event;

MODULE_DEVICE_TABLE(i2c, ft_id);

static int touch_set_reg(unsigned int Addr, unsigned int Value)
{
    void* pmem = ioremap(Addr, DEFAULT_MD_LEN);
    if (pmem == NULL) {
        return -1;
    }

    *(unsigned int*)pmem = Value;
    iounmap(pmem);
    return 0;
}

static int touch_reset_buffer(void)
{
    int i = 0;
    i2c_ext_func *i2c_func = NULL;
    hi_u8 data[1] = {0};
    i2c_drv_get_i2c_ext_func(&i2c_func);
    if (i2c_func == NULL) {
        printk("HI_ID_I2C handle get failed \r\n");
        return 0;
    }

    for (i = 0; i < OUCH_POINTS_LIMIT; i++) {
        i2c_func->pfn_i2c_write(1, FST_ADDR, GT_BUFFER_STAT_ADDR, I2C_BYTE_NUM, data, 1);
    }
    return 0;
}

/**only support onetouch*/
static int touch_event_handler(ts_event *event)
{
    hi_s32 ret = -1;
    int i = 0;
    int fingernum = 0;
    int coord_x = 0;
    int coord_y = 0;
    static bool pretouch = 0;
    unsigned char data = 0;
    i2c_ext_func *i2c_func = NULL;
    memset(event, 0, sizeof(ts_event));

    i2c_drv_get_i2c_ext_func(&i2c_func);
    if (i2c_func == NULL) {
        printk("HI_ID_I2C handle get failed \r\n");
        return 0;
    }

    ret = i2c_func->pfn_i2c_read(1, FST_ADDR, GT_BUFFER_STAT_ADDR, I2C_BYTE_NUM, &data, 1);
    if (ret != 0) {
        printk("i2c_func->pfn_i2c_read failed \r\n");
        return 0;
    }

    if (data == GT911_DATA_RECVIVED) {
        if (pretouch) {
            event->point_num = 1;
            event->au8_finger_id[0] = 0;
            event->au8_touch_event[0] = TP_EVENT_LIFT_UP;
        }
        pretouch = 0;
        goto exit_work_func;
    } else if (data == 0) {
        return 0;
    }
    pretouch = 1;
    fingernum = data & (GT911_BIT0 | GT911_BIT1);

    if (fingernum > 0) {
        unsigned char finggerdata[FINGGER_DATA_NUM] = {0};
        ret = i2c_func->pfn_i2c_read(1, FST_ADDR, GT_COORDINATE_X_LOWBYPE_BASE, I2C_BYTE_NUM, finggerdata,
            FINGGER_DATA_NUM);
        coord_x = (finggerdata[0] & 0xFF) | ((finggerdata[1] & 0xFF) << 8); // 8 for low position
        coord_y = (finggerdata[2] & 0xFF) | ((finggerdata[3] & 0xFF) << 8); // 8 for low position 2 3 if fingger number
        memset(event, 0, sizeof(ts_event));
        event->point_num = fingernum;
        event->touch_point = 0;

        for (i = 0; i < MAX_TOUCH_POINTS; i++) {
            event->touch_point++;
            /**Notice the  array bound !!*/
            event->au16_x[i] = coord_x;
            event->au16_y[i] = coord_y;
            event->au8_touch_event[i] = TP_EVENT_CONTACT;
            event->au8_finger_id[i] = 0;

            if ((event->au8_touch_event[i] == 0 || event->au8_touch_event[i] == TP_EVENT_CONTACT) &&
                (event->point_num == 0)) {
                printk("abnormal touch data from fw");
                return -1;
            }
        }
    } else {
        printk("The touch_handler is other number. %d. The finggernum is %d.\n", data, fingernum);
    }
exit_work_func:
    touch_reset_buffer();
    return 0;
}

hi_void tpint_irq(hi_u32 irq)
{
    int i = 0;
    int ret = 0;
    bool act;
    struct input_dev* input = tp_ts_dev;
    ts_event event;
    gpio_ext_func *g_pst_gpio_func = HI_NULL;

    gpio_drv_get_gpio_ext_func(&g_pst_gpio_func);
    if (g_pst_gpio_func == HI_NULL) {
        printk("tpint_irq get gpio export function failed!\n");
    }

    touch_event_handler(&event);
    if (ret != 0) {
        printk("\n buffer_read failed \n");
    }

    if (event.point_num != 0) {
        for (i = 0; i < event.touch_point; i++) {
            input_mt_slot(input, i);
            act = (event.au8_touch_event[i] == TP_EVENT_PRESS_DOWN || event.au8_touch_event[i] == TP_EVENT_CONTACT);
            input_mt_report_slot_state(input, MT_TOOL_FINGER, act);
            if (!act)
                continue;
            input_report_abs(input, ABS_MT_POSITION_X, event.au16_x[i]);
            input_report_abs(input, ABS_MT_POSITION_Y, event.au16_y[i]);
        }
    }
    input_mt_sync_frame(input);
    input_sync(input);
    mdelay(INT_DELAY_TIME);

    g_pst_gpio_func->pfn_gpio_clear_bit_int(INT_GPIO_NUM);
    return;
}

static int tp_irq_register(void)
{
    gpio_ext_func *g_pst_gpio_func = HI_NULL;

    gpio_drv_get_gpio_ext_func(&g_pst_gpio_func);
    if (g_pst_gpio_func == NULL) {
        printk("tpint_irq get gpio export function failed!\n");
        return 0;
    }

    g_pst_gpio_func->pfn_gpio_set_int_type(INT_GPIO_NUM, GPIO_INTTYPE_UPDOWN);
    g_pst_gpio_func->pfn_gpio_register_server_func(INT_GPIO_NUM, tpint_irq);
    g_pst_gpio_func->pfn_gpio_clear_bit_int(INT_GPIO_NUM);
    g_pst_gpio_func->pfn_gpio_set_int_enable(INT_GPIO_NUM, 1);

    return 0;
}

static int devinput_init(void)
{
    int error = 0;
    /* 1. distribution a "input_dev" structure */
    tp_ts_dev = input_allocate_device();
    if (tp_ts_dev == NULL) {
        printk(" func:%s line:%d \r\n", __FUNCTION__, __LINE__);
        return -1;
    }
    tp_ts_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
    set_bit(EV_SYN, tp_ts_dev->evbit);
    set_bit(EV_KEY, tp_ts_dev->evbit);
    set_bit(EV_ABS, tp_ts_dev->evbit);
    set_bit(INPUT_PROP_DIRECT, tp_ts_dev->propbit);
    input_set_abs_params(tp_ts_dev, ABS_MT_POSITION_X, 0, TP_SCREEN_WIDTH_NUM, 0, 0);
    input_set_abs_params(tp_ts_dev, ABS_MT_POSITION_Y, 0, TP_SCREEN_HEIGHT_NUM, 0, 0);
    input_set_abs_params(tp_ts_dev, ABS_MT_TOUCH_MAJOR, 0, 0xff, 0, 0);
    input_set_abs_params(tp_ts_dev, ABS_MT_PRESSURE, 0, 0xff, 0, 0);
    input_set_abs_params(tp_ts_dev, ABS_MT_TRACKING_ID, 0, 0xff, 0, 0);
    error = input_mt_init_slots(tp_ts_dev, MAX_TOUCH_POINTS, INPUT_MT_DIRECT | INPUT_MT_DROP_UNUSED);
    if (error != 0) {
        return error;
    }
    tp_ts_dev->name = "tp";
    tp_ts_dev->id.bustype = BUS_I2C;

    error = input_register_device(tp_ts_dev);
    if (error != 0) {
        dev_err(NULL, "failed to register input device: %d\n", error);
        return error;
    }

    error = tp_irq_register();
    if (error != 0) {
        dev_err(NULL, "failed to register input device: %d\n", error);
        return error;
    }

    return 0;
}

static int __init tp_init(void)
{
    int ret = 0;
    gpio_ext_func *g_pst_gpio_func = HI_NULL;
    touch_set_reg(RST_GPIO_NUM_REG, 0x0); // GPIO4_1 reset 0
    touch_set_reg(INT_GPIO_NUM_REG, 0x0); // GPIO4_2 int   0
    touch_set_reg(I2C1_SDA_REG, 0x03); // GPIO0_4 I2C    011：I2C1_SDA；
    touch_set_reg(I2C1_SCL_REG, 0x03); // GPIO0_5 I2C    011：I2C1_SCL；
    msleep(1);

    ret = hi_drv_i2c_init();
    if (ret != 0) {
        dev_err(NULL, "I2C init failed: %d\n", ret);
        goto error_end;
    }

    gpio_drv_get_gpio_ext_func(&g_pst_gpio_func);
    if (g_pst_gpio_func == NULL) {
        printk("get gpio export function failed!\n");
        goto error_end;
    }

    /**set INT and GPIO to be output*/
    g_pst_gpio_func->pfn_gpio_dir_set_bit(RST_GPIO_NUM, 0); // set GPIO1_0 Reset output
    g_pst_gpio_func->pfn_gpio_dir_set_bit(INT_GPIO_NUM, 0); // set GPIO1_1 INT output
    msleep(1);

    /**Set Reset*/
    g_pst_gpio_func->pfn_gpio_write_bit(RST_GPIO_NUM, 0); // set Reset low level
    g_pst_gpio_func->pfn_gpio_write_bit(INT_GPIO_NUM, 0); // set INT low level
    msleep(GT911_STAY_LOW_TIME);

    g_pst_gpio_func->pfn_gpio_write_bit(RST_GPIO_NUM, 1); // set Reset high level
    msleep(GT911_STAY_HIGH_TIME);

    g_pst_gpio_func->pfn_gpio_dir_set_bit(INT_GPIO_NUM, 1); // set GPIO1_1 INT input

    ret = devinput_init();
    if (ret != 0) {
        dev_err(NULL, " devinput_init fail!\n");
        goto error_end;
    }

    return 0;
error_end:
    return -1;
}

static void __exit tp_exit(void)
{
    free_irq(gpio_to_irq(INT_GPIO_NUM), tp_ts_dev);
    input_unregister_device(tp_ts_dev);
    input_free_device(tp_ts_dev);
    hi_drv_i2c_de_init();
}

module_init(tp_init);
module_exit(tp_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hisilicon");
