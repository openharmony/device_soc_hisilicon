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
#include <linux/delay.h>
#include "securec.h"
#include "hi_log.h"
#include "drv_i2c_ioctl.h"
#include "drv_gpio_ioctl.h"
#include "drv_gpioi2c.h"
#include "hi_drv_gpioi2c.h"
#include "hi_drv_gpio.h"
#include "hi_drv_i2c.h"
#include "hi_module.h"
#include "hi_common.h"
#include "hi_error_mpi.h"
#include "hi_osal.h"

#if !defined(CFG_HI_USER_DRV)
#include "hi_drv_sys.h"
#endif

#define BYTE_BITS  8
#define MAX_I2C_CHANEL_NUM 15
static osal_spinlock g_gpio_i2c_lock;

extern volatile hi_void *g_gpio_usr_addr[HI_GPIO_MAX_GROUP_NUM];

i2c_gpio g_st_i2c_gpio[HI_I2C_MAX_NUM];

typedef struct {
    hi_u32 i2c_channel;
    hi_u32 gpio_i2c_scl;
    hi_u32 gpio_i2c_sda;
    volatile hi_u32 *p_gpio_dir_clk;
    volatile hi_u32 *p_gpio_dir_data;
    volatile hi_u32 *p_gpio_i2c_sda_reg;
    volatile hi_u32 *p_gpio_i2c_scl_reg;
} i2c_data_gpio;

static i2c_data_gpio g_st_i2c_da_gpio[HI_I2C_MAX_NUM];
static hi_u8 g_gpio_grp_num;


/* 1: read, S:DevAddr:A:reg_addr:A:S:DevAddr:RecData:Stop
       write, S:DevAddr:A:reg_addr:A:WriteData:Stop
   2: read, S:DevAddr:A:reg_addr:A:Dealy:Stop:S:DevAddr:A:RecData:NoA:S
       write, S:DevAddr:A:reg_addr:A:WriteData:S */
#define I2C_MODE_ONE        1
#define I2C_MODE_SECOND     2
#define I2C_MAX_ADDRESS_LEN 4

#define HW_REG(reg)       *((volatile unsigned int *)(reg))
#define DELAY(i2c_num, us) time_delay_us(i2c_num, us)

osal_semaphore g_gpio_i2_sem;

static gpio_i2c_ext_func g_st_gpioi2c_ext_funcs = {
    // .pfnGpioI2cConfig         = drv_gpioi2c_config,
    .pfn_gpio_i2c_write = hi_drv_gpioi2c_write,
    .pfn_gpio_i2c_read = hi_drv_gpioi2c_read,
    .pfn_gpio_i2c_write_ext = hi_drv_gpioi2c_write_ext,
    .pfn_gpio_i2c_read_ext = hi_drv_gpioi2c_read_ext,
    .pfn_gpio_i2c_read_ext_di_rectly = hi_drv_gpioi2c_read_ext_directly,
    .pfn_gpio_i2c_write_ext_nostop = hi_drv_gpioi2c_write_ext_no_stop,

    // .pfnGpioI2cClose          = DRV_GPIOI2C_Close,
    .pfn_gpio_i2c_sccb_read = hi_drv_gpioi2c_sccb_read,
    // .pfnGpioI2cTunerSendData  = gpio_i2c_senddata_tuner,
    // .pfnGpioI2cTunerReceiveData = gpio_i2c_receivedata_tuner,
    .pfn_gpio_i2c_create_channel = hi_drv_gpioi2c_create_gpio_i2c,
    .pfn_gpio_i2c_destroy_channel = hi_drv_gpioi2c_destroy_gpio_i2c,
    .pfn_gpio_i2c_is_used = drv_gpioi2c_is_used,
};

/*
 * I2C by GPIO simulated  clear 0 routine.
 *
 * @param whichline: GPIO control line
 *
 */
static hi_void i2c_clr(int i2c_num, volatile unsigned int *whichline)
{
    unsigned char regvalue = 0;

    if (whichline == g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg) {
        regvalue = HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_clk);
        regvalue |= g_st_i2c_da_gpio[i2c_num].gpio_i2c_scl;
        HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_clk) = regvalue;

        HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg) = 0;
        return;
    } else if (whichline == g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg) {
        regvalue = HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data);
        regvalue |= g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda;
        HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data) = regvalue;

        HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg) = 0;
        return;
    } else {
        HI_LOG_ERR("i2c_clr Error input.\n");
        return;
    }
}

/*
 * I2C by GPIO simulated  set 1 routine.
 *
 * @param whichline: GPIO control line
 *
 */
static hi_void i2c_set(int i2c_num, volatile unsigned int *whichline)
{
    unsigned char regvalue = 0;

    if (whichline == NULL) {
        HI_LOG_ERR("whichline is NULL pointer.\n");
        return;
    }

    if (whichline == g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg) {
        regvalue = HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_clk);
        regvalue |= g_st_i2c_da_gpio[i2c_num].gpio_i2c_scl;
        HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_clk) = regvalue;

        HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg) = g_st_i2c_da_gpio[i2c_num].gpio_i2c_scl;
        return;
    } else if (whichline == g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg) {
        regvalue = HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data);
        regvalue |= g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda;
        HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data) = regvalue;

        HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg) = g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda;
        return;
    } else {
        HI_LOG_ERR("i2c_set Error input.\n");
        return;
    }
}

/*
 *  delays for a specified number of micro seconds routine.
 *
 *  @param usec: number of micro seconds to pause for
 *
 */
static hi_void time_delay_us(int i2c_num, unsigned int usec)
{
    unsigned int i = 0;

    for (i = 0; i < usec; i++) {
        udelay(1);
    }
}

/*
 * I2C by GPIO simulated  read data routine.
 *
 * @return value: a bit for read
 *
 */
static unsigned char i2c_data_read(int i2c_num)
{
    unsigned char regvalue;

    regvalue = HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data);
    regvalue &= (~g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda);
    HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data) = regvalue;

    regvalue = HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg);
    if ((regvalue & g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda) != 0) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * sends a start bit via I2C routine.
 *
 */
static hi_void i2c_start_bit(int i2c_num)
{
    DELAY(i2c_num, 1);
    i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg);
    i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 1);
    i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg);
    DELAY(i2c_num, 2);
}

/*
 * sends a stop bit via I2C routine.
 *
 */
static hi_void i2c_stop_bit(int i2c_num)
{
    /* actual stop bit */
    i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg);
    DELAY(i2c_num, 1);
    i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 1);
    i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg);
    DELAY(i2c_num, 1);
}

/*
 * sends a character over I2C routine.
 *
 * @param  c: character to send
 *
 */
static hi_void i2c_send_byte(int i2c_num, unsigned char c)
{
    hi_u32 i = 0;
    HI_SIZE_T irq_flags = 0;

    osal_spin_lock_irqsave(&g_gpio_i2c_lock, &irq_flags);
    for (i = 0; i < BYTE_BITS; i++) {
        i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
        DELAY(i2c_num, 1);

        if (c & (1 << (BYTE_BITS - 1 - i))) {
            i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg);
        } else {
            i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg);
        }

        DELAY(i2c_num, 1);
        i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
        DELAY(i2c_num, 1);
        DELAY(i2c_num, 1);
        i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    }

    DELAY(i2c_num, 1);
    osal_spin_unlock_irqrestore(&g_gpio_i2c_lock, &irq_flags);
}

/*  receives a character from I2C routine.
 *
 *  @return value: character received
 *
 */
static unsigned char i2c_receive_byte(int i2c_num)
{
    int j = 0;
    hi_u32 i = 0;
    unsigned char regvalue = 0;
    HI_SIZE_T irq_flags = {0};

    osal_spin_lock_irqsave(&g_gpio_i2c_lock, &irq_flags);
    for (i = 0; i < BYTE_BITS; i++) {
        DELAY(i2c_num, 1);
        i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
        DELAY(i2c_num, 2);
        i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);

        regvalue = HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data);
        regvalue &= (~g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda);
        HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data) = regvalue;
        DELAY(i2c_num, 1);

        if (i2c_data_read(i2c_num)) {
            j += (1 << (BYTE_BITS - 1 - i));
        }

        DELAY(i2c_num, 1);
        i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    }

    osal_spin_unlock_irqrestore(&g_gpio_i2c_lock, &irq_flags);

    DELAY(i2c_num, 1);

    return (unsigned char)j;
}

/*  receives an acknowledge from I2C routine.
 *
 *  @return value: 0--Ack received; 1--Nack received
 *
 */
static int i2c_receive_ack(int i2c_num)
{
    int nack ;
    unsigned char regvalue;

    DELAY(i2c_num, 1);

    regvalue = HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data);
    regvalue &= (~g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda);
    HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data) = regvalue;

    DELAY(i2c_num, 1);
    i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 1);
    i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 1);

    nack = i2c_data_read(i2c_num);

    DELAY(i2c_num, 1);
    i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 2);

    if (nack == 0) {
        return HI_SUCCESS;
    } else {
        HI_LOG_ERR("receive Err ack = 1 \n");
        return HI_FAILURE;
    }
}

/*
 * sends an acknowledge over I2C routine.
 *
 */
static hi_void i2c_send_ack(int i2c_num)
{
    unsigned char regvalue;

    DELAY(i2c_num, 1);
    i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 1);
    i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg);
    DELAY(i2c_num, 1);
    i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 1);
    i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 1);

    regvalue = HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data);
    regvalue &= (~g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda);
    HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data) = regvalue;

    DELAY(i2c_num, 2);
}

static hi_void i2c_send_noack(int i2c_num)
{
    unsigned char regvalue;

    DELAY(i2c_num, 1);
    i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 1);
    i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg);
    DELAY(i2c_num, 1);
    i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 2);
    i2c_clr(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    DELAY(i2c_num, 1);

    regvalue = HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data);
    regvalue &= (~g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda);
    HW_REG(g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data) = regvalue;

    DELAY(i2c_num, 1);
}

/*
 *  read data from the I2C bus by GPIO simulated of a device routine.
 *
 *  @param  devaddress:  address of the device
 *  @param  address: address of register within device
 *
 *  @return value: data from the device read
 *
 */
hi_s32 hi_drv_gpioi2c_read(hi_u32 i2c_num, hi_u8 dev_addr, hi_u8 reg_address, hi_u8 *p_data)
{
    hi_s32 ret;

    if (osal_sem_down_interruptible(&g_gpio_i2_sem)) {
        HI_LOG_ERR("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    if ((i2c_num >= HI_I2C_MAX_NUM) || (p_data == HI_NULL) || (g_st_i2c_gpio[i2c_num].b_used == HI_FALSE)) {
        osal_sem_up(&g_gpio_i2_sem);
        return HI_FAILURE;
    }

    i2c_start_bit(i2c_num);

    i2c_send_byte(i2c_num, (unsigned char)(dev_addr));
    ret = i2c_receive_ack(i2c_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("i2c not receive ack!\n");
    }

    i2c_send_byte(i2c_num, reg_address);
    ret = i2c_receive_ack(i2c_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("i2c not receive ack!\n");
    }

    if (*get_var_i2c_mode() == I2C_MODE_SECOND) {
        i2c_stop_bit(i2c_num);
        DELAY(i2c_num, 5);
    }

    i2c_set(i2c_num, g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    i2c_start_bit(i2c_num);

    i2c_send_byte(i2c_num, (unsigned char)(dev_addr) | 1);
    ret = i2c_receive_ack(i2c_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("i2c not receive ack!\n");
    }

    *p_data = i2c_receive_byte(i2c_num);
    if (*get_var_i2c_mode() == I2C_MODE_SECOND) {
        i2c_send_noack(i2c_num);
    }

    i2c_stop_bit(i2c_num);

    osal_sem_up(&g_gpio_i2_sem);
    HI_LOG_INFO("\t i2c_num=%d, DevAddr=0x%x, reg_addr=0x%x, data0=0x%x\n",
                i2c_num, dev_addr, reg_address, *p_data);

    return HI_SUCCESS;
}

static hi_s32 drv_gpioi2c_read_data(hi_u32 i2c_num, hi_u8 devaddress, hi_u32 address, hi_u32 addresslen,
                                    hi_bool b_send_reg_address, hi_u8 *p_data, hi_u32 data_len)
{
    hi_u32 i = 0;
    hi_u32 ret;  // TC MODIFY
    hi_u8 reval = 0;
    hi_u8 reg_addr = 0;

    if (osal_sem_down_interruptible(&g_gpio_i2_sem)) {
        HI_LOG_ERR("Semaphore lock is   error. \n");
        return HI_FAILURE;
    }

    if ((i2c_num >= HI_I2C_MAX_NUM) || (g_st_i2c_gpio[i2c_num].b_used == HI_FALSE)) {
        HI_LOG_ERR("the i2c_num haven't requested ,can't read operation.\n");
        osal_sem_up(&g_gpio_i2_sem);
        return HI_FAILURE;
    }

    if (b_send_reg_address) {
        // send start condition
        i2c_start_bit(i2c_num);
        // send slave device address
        i2c_send_byte(i2c_num, (unsigned char)(devaddress));
        // TC MODIFY START
        ret = i2c_receive_ack(i2c_num);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERR("i2c not receive ack!\n");
            osal_sem_up(&g_gpio_i2_sem);
            return HI_FAILURE;
        }
        // send register address
        for (i = 0; i < (addresslen); i++) {
            reg_addr = address >> ((addresslen - i - 1) * 8); // 取出每1个从机地址，每1个地址8位
            i2c_send_byte(i2c_num, reg_addr);
            i2c_receive_ack(i2c_num);
        }
    }
    // send start condition
    i2c_start_bit(i2c_num);
    // send slave device address
    i2c_send_byte(i2c_num, (unsigned char)(devaddress) | 1);
    ret = i2c_receive_ack(i2c_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("i2c not receive ack!\n");
    }
    // receive data
    for (i = 0; i < data_len; i++) {
        reval = i2c_receive_byte(i2c_num);

        *(p_data + i) = reval;

        if (i == data_len - 1) {
            i2c_send_noack(i2c_num);
        } else {
            i2c_send_ack(i2c_num);
        }
    }
    // send stop condition
    i2c_stop_bit(i2c_num);
    osal_sem_up(&g_gpio_i2_sem);

    HI_LOG_INFO("\t i2c_num=%d, DevAddr=0x%x, reg_addr=0x%x, Num=%d, Len=%d, data0=0x%x\n", i2c_num, devaddress,
                address, addresslen, data_len, *p_data);
    return HI_SUCCESS;
}


hi_s32 hi_drv_gpioi2c_read_ext(hi_u32 i2c_num, hi_u8 devaddress, hi_u32 address, hi_u32 addresslen, hi_u8 *p_data,
                               hi_u32 data_len)
{
    if (i2c_num >= HI_I2C_MAX_NUM) {
        HI_LOG_ERR("i2c_num(%d) is wrong, I2C_MAX_NUM is %d\n", i2c_num, HI_I2C_MAX_NUM);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (addresslen > I2C_MAX_ADDRESS_LEN) {
        HI_LOG_ERR("para addresslen is invalid.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (p_data == NULL) {
        HI_LOG_ERR("para p_data is null.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    if ((HI_I2C_MAX_LENGTH < data_len) || (data_len == 0)) {
        HI_LOG_ERR("para data_len is invalid.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    return drv_gpioi2c_read_data(i2c_num, devaddress, address, addresslen, HI_TRUE, p_data, data_len);
}

hi_s32 hi_drv_gpioi2c_read_ext_directly(hi_u32 i2c_num, hi_u8 devaddress, hi_u32 address, hi_u32 addresslen,
                                        hi_u8 *p_data, hi_u32 data_len)
{
    if (i2c_num >= HI_I2C_MAX_NUM) {
        HI_LOG_ERR("i2c_num(%d) is wrong, I2C_MAX_NUM is %d\n", i2c_num, HI_I2C_MAX_NUM);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (addresslen > I2C_MAX_ADDRESS_LEN) {
        HI_LOG_ERR("para addresslen is invalid.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (p_data == NULL) {
        HI_LOG_ERR("para p_data is null.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    if ((data_len > HI_I2C_MAX_LENGTH) || (data_len == 0)) {
        HI_LOG_ERR("para data_len is invalid.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    return drv_gpioi2c_read_data(i2c_num, devaddress, address, addresslen, HI_FALSE, p_data, data_len);
}

/*
 *  writes data to a device on the I2C bus routine.
 *
 *  @param  devaddress:  address of the device
 *  @param  address: address of register within device
 *  @param  data:   data for write to device
 *
 */
hi_s32 hi_drv_gpioi2c_write(hi_u32 i2c_num, hi_u8 dev_addr, hi_u8 reg_address, hi_u8 u8Data)
{
    hi_s32 ret;

    if (osal_sem_down_interruptible(&g_gpio_i2_sem)) {
        HI_LOG_ERR("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    if ((i2c_num >= HI_I2C_MAX_NUM) || (g_st_i2c_gpio[i2c_num].b_used == HI_FALSE)) {
        osal_sem_up(&g_gpio_i2_sem);
        return HI_FAILURE;
    }

    HI_LOG_INFO("\t i2c_num=%d, DevAddr=0x%x, reg_addr=0x%x, data0=0x%x\n",
                i2c_num, dev_addr, reg_address, u8Data);

    i2c_start_bit(i2c_num);

    i2c_send_byte(i2c_num, (unsigned char)(dev_addr));
    ret = i2c_receive_ack(i2c_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("i2c not receive ack!\n");
    }

    i2c_send_byte(i2c_num, reg_address);
    ret = i2c_receive_ack(i2c_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("i2c not receive ack!\n");
    }

    i2c_send_byte(i2c_num, u8Data);
    if (*get_var_i2c_mode() == I2C_MODE_SECOND) {
        i2c_receive_ack(i2c_num);
        DELAY(i2c_num, 10);
    }

    i2c_stop_bit(i2c_num);
    osal_sem_up(&g_gpio_i2_sem);

    return HI_SUCCESS;
}

static hi_s32 drv_gpioi2c_write_data(hi_u32 i2c_num, hi_u8 devaddress, hi_u32 address, hi_u32 addresslen,
                                     hi_u8 *p_data, hi_u32 data_len, hi_bool b_send_stop_condtion)
{
    hi_u32 i = 0;
    hi_u8 reg_addr = 0;
    hi_s32 ret;

    if (osal_sem_down_interruptible(&g_gpio_i2_sem)) {
        HI_LOG_ERR("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    if ((i2c_num >= HI_I2C_MAX_NUM) || (g_st_i2c_gpio[i2c_num].b_used == HI_FALSE)) {
        HI_LOG_ERR("the i2c_num haven't requested ,can't write operation.\n");
        osal_sem_up(&g_gpio_i2_sem);
        return HI_FAILURE;
    }

    HI_LOG_INFO("i2c_num=%d, DevAddr=0x%x, reg_addr=0x%x, Num=%d, Len=%d, data0=0x%x\n", i2c_num, devaddress, address,
                addresslen, data_len, p_data[0]);

    i2c_start_bit(i2c_num);

    i2c_send_byte(i2c_num, (unsigned char)(devaddress));
    ret = i2c_receive_ack(i2c_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("i2c not receive ack!\n");
    }

    for (i = 0; i < (addresslen); i++) {
        reg_addr = address >> ((addresslen - i - 1) * 8);  // 取出每1个从机地址，每1个地址8位
        i2c_send_byte(i2c_num, (unsigned char)reg_addr);
        ret = i2c_receive_ack(i2c_num);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERR("i2c not receive ack!\n");
        }
    }

    for (i = 0; i < data_len; i++) {
        i2c_send_byte(i2c_num, p_data[i]);
        ret = i2c_receive_ack(i2c_num);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERR("i2c not receive ack!\n");
        }
    }

    if (b_send_stop_condtion) {
        i2c_stop_bit(i2c_num);
    }

    osal_sem_up(&g_gpio_i2_sem);

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpioi2c_write_ext(hi_u32 i2c_num, hi_u8 devaddress, hi_u32 address, hi_u32 addresslen, hi_u8 *p_data,
                                hi_u32 data_len)
{
    if (i2c_num >= HI_I2C_MAX_NUM) {
        HI_LOG_ERR("i2c_num(%d) is wrong, I2C_MAX_NUM is %d\n", i2c_num, HI_I2C_MAX_NUM);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (addresslen > I2C_MAX_ADDRESS_LEN) {
        HI_LOG_ERR("para I2creg_addrByteNum is invalid.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (p_data == NULL) {
        HI_LOG_ERR("para p_data is null.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (data_len > HI_I2C_MAX_LENGTH) {
        HI_LOG_ERR("para data_len is invalid.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    return drv_gpioi2c_write_data(i2c_num, devaddress, address, addresslen, p_data, data_len, HI_TRUE);
}

hi_s32 hi_drv_gpioi2c_write_ext_no_stop(hi_u32 i2c_num, hi_u8 devaddress, hi_u32 address, hi_u32 addresslen,
                                        hi_u8 *p_data, hi_u32 data_len)
{
    if (i2c_num >= HI_I2C_MAX_NUM) {
        HI_LOG_ERR("i2c_num(%d) is wrong, I2C_MAX_NUM is %d\n", i2c_num, HI_I2C_MAX_NUM);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (addresslen > I2C_MAX_ADDRESS_LEN) {
        HI_LOG_ERR("para addresslen is invalid.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (p_data == NULL) {
        HI_LOG_ERR("para p_data is null.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    if ((data_len > HI_I2C_MAX_LENGTH)) {
        HI_LOG_ERR("para data_len is invalid.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    return drv_gpioi2c_write_data(i2c_num, devaddress, address, addresslen, p_data, data_len, HI_FALSE);
}

/*
 *  read data from the I2C bus by GPIO simulated of a digital camera device routine.
 *
 *  @param  devaddress:  address of the device
 *  @param  address: address of register within device
 *
 */
hi_s32 hi_drv_gpioi2c_sccb_read(hi_u32 i2c_num, hi_u8 dev_addr, hi_u8 reg_address, hi_u8 *p_data)
{
    hi_s32 ret;

    if (osal_sem_down_interruptible(&g_gpio_i2_sem)) {
        HI_LOG_ERR("Semaphore lock is  error. \n");
        return HI_FAILURE;
        ;
    }

    if ((i2c_num >= HI_I2C_MAX_NUM) || (p_data == HI_NULL) || (g_st_i2c_gpio[i2c_num].b_used == HI_FALSE)) {
        osal_sem_up(&g_gpio_i2_sem);
        return HI_FAILURE;
    }

    i2c_start_bit(i2c_num);
    i2c_send_byte(i2c_num, (unsigned char)(dev_addr));
    ret = i2c_receive_ack(i2c_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("i2c not receive ack!\n");
    }
    i2c_send_byte(i2c_num, reg_address);
    ret = i2c_receive_ack(i2c_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("i2c not receive ack!\n");
    }
    i2c_stop_bit(i2c_num);
    i2c_start_bit(i2c_num);
    i2c_send_byte(i2c_num, (unsigned char)(dev_addr) | 1);
    ret = i2c_receive_ack(i2c_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("i2c not receive ack!\n");
    }
    *p_data = i2c_receive_byte(i2c_num);
    i2c_send_ack(i2c_num);
    i2c_stop_bit(i2c_num);
    osal_sem_up(&g_gpio_i2_sem);

    return HI_SUCCESS;
}

hi_s32 drv_gpioi2c_is_used(hi_u32 i2c_num, hi_bool *b_used)
{
    if ((i2c_num > MAX_I2C_CHANEL_NUM) || (b_used == HI_NULL)) {
        return HI_FAILURE;
    }

    *b_used = g_st_i2c_gpio[i2c_num].b_used;
    return HI_SUCCESS;
}

hi_s32 drv_gpioi2c_config(hi_u32 i2c_num, hi_u32 which_gpio_clock, hi_u32 which_gpio_data, hi_u32 clock_bit,
                          hi_u32 data_bit)
{
    volatile hi_void *pv_gpio_base_clock = HI_NULL;
    volatile hi_void *pv_gpio_base_data = HI_NULL;

    if (g_gpio_usr_addr[which_gpio_clock] == NULL
        || g_gpio_usr_addr[which_gpio_data] == NULL) {
        HI_LOG_ERR("gpio addr is invalid!\n");
        return HI_FAILURE;
    }
    pv_gpio_base_clock = g_gpio_usr_addr[which_gpio_clock];
    pv_gpio_base_data = g_gpio_usr_addr[which_gpio_data];

    g_st_i2c_da_gpio[i2c_num].i2c_channel = i2c_num;
    g_st_i2c_da_gpio[i2c_num].gpio_i2c_scl = (1 << clock_bit);
    g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda = (1 << data_bit);

    g_st_i2c_da_gpio[i2c_num].p_gpio_dir_data = pv_gpio_base_data + HI_GPIO_DIR_REG;
    g_st_i2c_da_gpio[i2c_num].p_gpio_dir_clk = pv_gpio_base_clock + HI_GPIO_DIR_REG;
    g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg = (pv_gpio_base_data + \
            ((g_st_i2c_da_gpio[i2c_num].gpio_i2c_sda) << 2)); // 左移2位,适配SDA寄存器
    g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg = (pv_gpio_base_clock + \
            ((g_st_i2c_da_gpio[i2c_num].gpio_i2c_scl) << 2)); // 左移2位,适配SCL寄存器

    HI_LOG_INFO("drv_gpioi2c_config OK(i2c_num=%d, SCL=gpio%d_%d, SDA=gpio%d_%d, g_i2c_mode=%d).\n", i2c_num,
                which_gpio_clock, clock_bit, which_gpio_data, data_bit, *get_var_i2c_mode());

    g_st_i2c_gpio[i2c_num].i2c_num = i2c_num;
    g_st_i2c_gpio[i2c_num].scl_gpio_no = which_gpio_clock * HI_GPIO_BIT_NUM + clock_bit;
    g_st_i2c_gpio[i2c_num].sda_gpio_no = which_gpio_data * HI_GPIO_BIT_NUM + data_bit;
    g_st_i2c_gpio[i2c_num].b_used = HI_TRUE;
    g_st_i2c_gpio[i2c_num].count = 1;

    return HI_SUCCESS;
}

static hi_s32 is_gpio_i2c_used(hi_u32 scl_gpio_no, hi_u32 sda_gpio_no, hi_u32 *p_i2c_num)
{
    hi_u32 i = 0;
    hi_bool b_temp = HI_FALSE;
    hi_u32 tmp_gpio_clk_no = 0;
    hi_u32 tmp_gpio_sda_no = 0;

    for (i = HI_STD_I2C_NUM; i < HI_I2C_MAX_NUM; i++) {
        if (g_st_i2c_gpio[i].b_used) {
            tmp_gpio_clk_no = g_st_i2c_gpio[i].scl_gpio_no;
            tmp_gpio_sda_no = g_st_i2c_gpio[i].sda_gpio_no;

            b_temp = (tmp_gpio_clk_no == scl_gpio_no) && (tmp_gpio_sda_no == sda_gpio_no);
            if (b_temp) {
                g_st_i2c_gpio[i].count++;
                *p_i2c_num = g_st_i2c_gpio[i].i2c_num;
                return HI_SUCCESS;
            }

            b_temp = (scl_gpio_no == tmp_gpio_clk_no) || (sda_gpio_no == tmp_gpio_clk_no);
            if (b_temp) {
                HI_LOG_ERR("GPIO NO. %d is used to GpioClock!\n", tmp_gpio_clk_no);
                return HI_ERR_GPIO_INVALID_PARA;
            }

            b_temp = (scl_gpio_no == tmp_gpio_sda_no) || (sda_gpio_no == tmp_gpio_sda_no);
            if (b_temp) {
                HI_LOG_ERR("GPIO NO. %d is used to GpioData!\n", tmp_gpio_sda_no);
                return HI_ERR_GPIO_INVALID_PARA;
            }
        }
    }
    return HI_FAILURE;
}


hi_s32 hi_drv_gpioi2c_create_gpio_i2c(hi_u32 *p_i2c_num, hi_u32 scl_gpio_no, hi_u32 sda_gpio_no)
{
    hi_s32 ret;
    hi_u32 i = 0;
    hi_u32 Validi2c_num = 0;
    hi_u32 which_gpio_clock;
    hi_u32 which_gpio_data;
    hi_u32 clock_bit;
    hi_u32 data_bit;

    which_gpio_clock = scl_gpio_no / HI_GPIO_BIT_NUM;
    clock_bit = scl_gpio_no % HI_GPIO_BIT_NUM;
    which_gpio_data = sda_gpio_no / HI_GPIO_BIT_NUM;
    data_bit = sda_gpio_no % HI_GPIO_BIT_NUM;

    if (p_i2c_num == NULL) {
        HI_LOG_ERR("p_i2c_num is null!\n");
        return HI_FAILURE;
    }
    if ((which_gpio_clock >= g_gpio_grp_num) || (which_gpio_data >= g_gpio_grp_num)
        || (scl_gpio_no == sda_gpio_no)) {
        HI_LOG_ERR("error: GPIO NO. %d and NO. %d is invalid!\n", scl_gpio_no, sda_gpio_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    /* whether the gpio have used in other i2c_num  */
    ret = is_gpio_i2c_used(scl_gpio_no, sda_gpio_no, &Validi2c_num);
    if (ret != HI_FAILURE) {
        if (ret == HI_SUCCESS) {
            *p_i2c_num = Validi2c_num;
        }
        return ret;
    }

    /* find a idle i2c channel */
    for (i = HI_STD_I2C_NUM; i < HI_I2C_MAX_NUM; i++) {
        if (g_st_i2c_gpio[i].b_used == HI_FALSE) {
            Validi2c_num = i;
            break;
        }
    }

    if (i >= HI_I2C_MAX_NUM) {
        HI_LOG_ERR("i2c channel all  have used ,request i2c channel fail  !\n");
        return HI_FAILURE;
    }

    /* the gpio can be use */
    ret = drv_gpioi2c_config(Validi2c_num, which_gpio_clock, which_gpio_data, clock_bit, data_bit);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERR("config gpio i2c failure !\n");
        return HI_FAILURE;
    }

    *p_i2c_num = Validi2c_num;

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpioi2c_destroy_gpio_i2c(hi_u32 i2c_num)
{
    if (i2c_num < HI_STD_I2C_NUM || i2c_num >= HI_I2C_MAX_NUM) {
        HI_LOG_ERR(" GPIO I2C i2c_num = %d is not used!\n", i2c_num);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (g_st_i2c_gpio[i2c_num].b_used == HI_FALSE) {
        HI_LOG_ERR("i2c_num = %d is not used!\n", i2c_num);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if ((--g_st_i2c_gpio[i2c_num].count) == 0) {
        g_st_i2c_gpio[i2c_num].b_used = HI_FALSE;
    }

    HI_LOG_INFO("gpio_i2c_destroy OK(i2c_num=%d, SCL=gpio No.%d, SDA=gpio No.%d).\n", i2c_num,
                g_st_i2c_gpio[i2c_num].scl_gpio_no, g_st_i2c_gpio[i2c_num].sda_gpio_no);

    return HI_SUCCESS;
}

/*************************************************************************/
osal_semaphore g_gpioi2c_mutex;

int hi_drv_gpioi2c_cmd_write(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    errno_t err_ret;
    hi_u8 *p_data = NULL;
    i2c_data *i2c_data_ptr = NULL;

    hi_dbg_func_enter();

    ret = osal_sem_down_interruptible(&g_gpioi2c_mutex);
    if (ret) {
        HI_LOG_ERR("lock g_i2c_mutex error.\n");
        return HI_FAILURE;
    }

    i2c_data_ptr = (i2c_data *)arg;

    if (i2c_data_ptr->data_len == 0 || i2c_data_ptr->data_len > HI_I2C_MAX_LENGTH) {
        HI_LOG_INFO("i2c_data.data_len is invalid!\n");
        ret = HI_ERR_I2C_INVALID_PARA;
        osal_sem_up(&g_gpioi2c_mutex);
        return ret;
    }

    p_data = osal_kmalloc(HI_ID_GPIO_I2C, i2c_data_ptr->data_len, OSAL_GFP_KERNEL);
    if (p_data == NULL) {
        HI_LOG_ERR("i2c kmalloc fail!\n");
        osal_sem_up(&g_gpioi2c_mutex);
        ret = HI_ERR_I2C_MALLOC_ERR;
        return ret;
    }

    err_ret = memset_s(p_data, i2c_data_ptr->data_len, 0, i2c_data_ptr->data_len);
    if (err_ret != EOK) {
        osal_kfree(HI_ID_GPIO_I2C, p_data);
        HI_LOG_ERR("secure func call error\n");
        osal_sem_up(&g_gpioi2c_mutex);
        return HI_FAILURE;
    }

    if (osal_copy_from_user(p_data, (hi_void *)(uintptr_t)(i2c_data_ptr->p_data), i2c_data_ptr->data_len)) {
        HI_LOG_ERR("copy data from user fail!\n");
        osal_kfree(HI_ID_GPIO_I2C, p_data);
        p_data = HI_NULL;
        ret = HI_ERR_I2C_COPY_DATA_ERR;
        osal_sem_up(&g_gpioi2c_mutex);
        return ret;
    }

    ret = hi_drv_gpioi2c_write_ext(i2c_data_ptr->i2c_num, i2c_data_ptr->i2c_dev_addr, i2c_data_ptr->i2c_reg_addr, \
                                   i2c_data_ptr->i2c_reg_count, p_data, i2c_data_ptr->data_len);
    osal_kfree(HI_ID_GPIO_I2C, p_data);
    p_data = HI_NULL;
    osal_sem_up(&g_gpioi2c_mutex);

    hi_dbg_func_exit();

    return ret;
}

int hi_drv_gpioi2c_cmd_read(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    errno_t err_ret;
    hi_u8 *p_data = NULL;
    i2c_data *i2c_data_ptr = NULL;

    hi_dbg_func_enter();

    ret = osal_sem_down_interruptible(&g_gpioi2c_mutex);
    if (ret) {
        HI_LOG_ERR("lock g_i2c_mutex error.\n");
        return HI_FAILURE;
    }

    i2c_data_ptr = (i2c_data *)arg;

    if (i2c_data_ptr->data_len > HI_I2C_MAX_LENGTH || i2c_data_ptr->data_len == 0) {
        HI_LOG_ERR("para i2c_data.data_len is invalid.\n");
        osal_sem_up(&g_gpioi2c_mutex);
        return HI_ERR_I2C_INVALID_PARA;
    }

    p_data = osal_kmalloc(HI_ID_GPIO_I2C, i2c_data_ptr->data_len, OSAL_GFP_KERNEL);
    if (p_data == NULL) {
        HI_LOG_ERR("i2c kmalloc fail!\n");
        osal_sem_up(&g_gpioi2c_mutex);
        ret = HI_ERR_I2C_MALLOC_ERR;
        return ret;
    }

    err_ret = memset_s(p_data, i2c_data_ptr->data_len, 0, i2c_data_ptr->data_len);
    if (err_ret != EOK) {
        osal_kfree(HI_ID_GPIO_I2C, p_data);
        osal_sem_up(&g_gpioi2c_mutex);
        HI_LOG_ERR("secure func call error\n");
        return HI_FAILURE;
    }

    ret = hi_drv_gpioi2c_read_ext(i2c_data_ptr->i2c_num, i2c_data_ptr->i2c_dev_addr, i2c_data_ptr->i2c_reg_addr, \
                                  i2c_data_ptr->i2c_reg_count, p_data, i2c_data_ptr->data_len);
    if (ret == HI_SUCCESS) {
        if (osal_copy_to_user((hi_void *)(uintptr_t)(i2c_data_ptr->p_data), p_data, i2c_data_ptr->data_len)) {
            HI_LOG_ERR("copy data to user fail!\n");
            ret = HI_ERR_I2C_COPY_DATA_ERR;
        }
    }

    osal_kfree(HI_ID_GPIO_I2C, p_data);
    osal_sem_up(&g_gpioi2c_mutex);
    p_data = HI_NULL;

    hi_dbg_func_exit();

    return ret;
}

int hi_drv_gpioi2c_cmd_config(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    i2c_gpio *i2c_gpio_ptr = NULL;

    hi_dbg_func_enter();

    ret = osal_sem_down_interruptible(&g_gpioi2c_mutex);
    if (ret) {
        HI_LOG_ERR("lock g_i2c_mutex error.\n");
        return HI_FAILURE;
    }

    i2c_gpio_ptr = (i2c_gpio *)arg;

    ret = hi_drv_gpioi2c_create_gpio_i2c(&(i2c_gpio_ptr->i2c_num),
                                         i2c_gpio_ptr->scl_gpio_no, i2c_gpio_ptr->sda_gpio_no);

    osal_sem_up(&g_gpioi2c_mutex);

    hi_dbg_func_exit();

    return ret;
}

int hi_drv_gpioi2c_cmd_destroy(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    long ret;
    i2c_gpio *i2c_gpio_ptr = NULL;

    hi_dbg_func_enter();

    ret = osal_sem_down_interruptible(&g_gpioi2c_mutex);
    if (ret) {
        HI_LOG_ERR("lock g_i2c_mutex error.\n");
        return HI_FAILURE;
    }

    i2c_gpio_ptr = (i2c_gpio *)arg;

    ret = hi_drv_gpioi2c_destroy_gpio_i2c(i2c_gpio_ptr->i2c_num);

    osal_sem_up(&g_gpioi2c_mutex);

    hi_dbg_func_exit();

    return ret;
}

#ifdef GPIO_I2C_PM
int gpio_i2c_suspend(pm_basedev *pdev, pm_message_t state)
{
    HI_PRINT("GPIO_I2C suspend OK\n");
    return HI_SUCCESS;
}

int gpio_i2c_resume(int i2c_num, pm_basedev *pdev)
{
    i2c_set(g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_scl_reg);
    i2c_set(g_st_i2c_da_gpio[i2c_num].p_gpio_i2c_sda_reg);
    HI_PRINT("GPIO_I2C resume OK\n");
    return HI_SUCCESS;
}

#endif

hi_u32 hi_drv_i2c_get_chip_version(hi_void)
{
    HI_CHIP_VERSION_E chip_version = HI_CHIP_VERSION_BUTT;
    i2c_drv_chip chip = I2C_DRV_CHIP_V350;
#if !defined(CFG_HI_USER_DRV)
    HI_CHIP_TYPE_E chip_type;
    hi_drv_sys_get_chip_version(&chip_type, &chip_version);
#endif

    if (chip_version == HI_CHIP_VERSION_BUTT) {
        chip = I2C_DRV_CHIP_V350;
    } else {
        switch (chip_version) {
            case HI_CHIP_VERSION_V350:
            case HI_CHIP_VERSION_V351:
                chip = I2C_DRV_CHIP_V350;
                break;
            default:
                break;
        }
    }

    HI_LOG_DBG("get chip version:0x%x\n", chip);
    return chip;
}

static unsigned int g_gpio_initialized = 0;

/*
 * initializes I2C interface routine.
 *
 * @return value:0--success; 1--error.
 *
 */
hi_s32 hi_drv_gpioi2c_init(hi_void)
{
    hi_u32 i, chip;
    hi_s32 ret;

    ret = osal_exportfunc_register(HI_ID_GPIO_I2C, "HI_GPIO_I2C", (hi_void *)&g_st_gpioi2c_ext_funcs);
    if (ret != HI_SUCCESS) {
        HI_LOG_FATAL(" GPIO_I2C Module register failed 0x%x.\n", ret);
        return HI_FAILURE;
    }

    if (g_gpio_initialized == 0) {
        g_gpio_grp_num = HI_GPIO_GROUP_NUM;
        ret = osal_sem_init(&g_gpioi2c_mutex, 1);
        if (ret != HI_SUCCESS) {
            HI_LOG_FATAL(" GPIO_I2C semaphore init failed.\n");
            return HI_FAILURE;
        }

        ret = osal_sem_init(&g_gpio_i2_sem, 1);
        if (ret != HI_SUCCESS) {
            HI_LOG_FATAL(" GPIO_I2C semaphore init failed.\n");
            return HI_FAILURE;
        }

        ret = osal_spin_lock_init(&g_gpio_i2c_lock);
        if (ret != HI_SUCCESS) {
            HI_LOG_FATAL(" GPIO_I2C spin lock init failed.\n");
            return HI_FAILURE;
        }
        g_gpio_initialized = 1;

        chip = hi_drv_i2c_get_chip_version();
        for (i = HI_STD_I2C_NUM; i < HI_I2C_MAX_NUM; i++) {
            g_st_i2c_gpio[i].b_used = HI_FALSE;
            g_st_i2c_gpio[i].count = 0;
        }
    }

    return HI_SUCCESS;
}

hi_void hi_drv_gpioi2c_de_init(hi_void)
{
    hi_u32 i = 0;
    hi_s32 ret;

    osal_spin_lock_destory(&g_gpio_i2c_lock);
    osal_sem_destory(&g_gpio_i2_sem);
    osal_sem_destory(&g_gpioi2c_mutex);

    ret = osal_exportfunc_unregister(HI_ID_GPIO_I2C);
    if (ret != HI_SUCCESS) {
        HI_LOG_FATAL(" GPIO_I2C Module unregister failed 0x%x.\n", ret);
    }

    g_gpio_initialized = 0;

    for (i = HI_STD_I2C_NUM; i < HI_I2C_MAX_NUM; i++) {
        g_st_i2c_gpio[i].b_used = HI_FALSE;
        g_st_i2c_gpio[i].count = 0;
    }
}

MODULE_LICENSE("GPL");

EXPORT_SYMBOL(hi_drv_i2c_get_chip_version);

EXPORT_SYMBOL(hi_drv_gpioi2c_init);
EXPORT_SYMBOL(hi_drv_gpioi2c_de_init);

EXPORT_SYMBOL(hi_drv_gpioi2c_write);
EXPORT_SYMBOL(hi_drv_gpioi2c_read);
EXPORT_SYMBOL(hi_drv_gpioi2c_write_ext);
EXPORT_SYMBOL(hi_drv_gpioi2c_read_ext);
EXPORT_SYMBOL(hi_drv_gpioi2c_read_ext_directly);
EXPORT_SYMBOL(hi_drv_gpioi2c_write_ext_no_stop);

EXPORT_SYMBOL(hi_drv_gpioi2c_sccb_read);

EXPORT_SYMBOL(hi_drv_gpioi2c_create_gpio_i2c);
EXPORT_SYMBOL(hi_drv_gpioi2c_destroy_gpio_i2c);
EXPORT_SYMBOL(hi_drv_gpioi2c_cmd_read);
EXPORT_SYMBOL(hi_drv_gpioi2c_cmd_write);
EXPORT_SYMBOL(hi_drv_gpioi2c_cmd_config);
EXPORT_SYMBOL(hi_drv_gpioi2c_cmd_destroy);

EXPORT_SYMBOL(g_st_i2c_gpio);
