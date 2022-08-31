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

#include <linux/version.h>
#include "securec.h"
#include "hi_log.h"

#include "drv_i2c.h"
#include "drv_i2c_ioctl.h"
#include "hi_sys_reg.h"
#include "hi_common.h"
#include "hi_drv_i2c.h"
#include "hi_module.h"
#include "hi_error_mpi.h"
#include "hi_osal.h"
#include "hi_drv_gpioi2c.h"
#include "drv_base_ext.h"

#ifdef CFG_HI_USER_DRV
volatile hi_reg_crg *g_pst_reg_crg = NULL;
#endif

#define SONY_I2C_START_EN 0x01 /* < Output START (S) */
#define SONY_I2C_STOP_EN  0x02 /* < Output STOP  (P) */

// Wait time will change with cpu running freq
#define I2C_WAIT_TIME_OUT 0x10000
#define RESTART_TIME      3

#define i2c_write_reg(Addr, Value) ((*(volatile hi_u32 *)(Addr)) = (Value))
#define i2c_read_reg(Addr)         (*(volatile hi_u32 *)(Addr))
#define i2c_scl_clk(a, b)    (((a) / ((b) * 2)) / 2 - 1)

#define I2C_CLEAR_ALL_MASK 0x100
#define I2C_SET_IRQ_MASK 0x181 /* bit 0 & bit 7 */
#define HI_I2C0_IRQ_NO (38 + 32) /* I2C0 Irq No */
#define HI_I2C1_IRQ_NO (39 + 32) /* I2C1 Irq No */
#define I2C1_IRQ_NAME "hi_i2c1_irq"
#define I2C0_IRQ_NAME "hi_i2c0_irq"

typedef struct i2c_drv_irq_info_s {
    hi_u32 irq_send_count;
    hi_u32 irq_recv_count;
    hi_u32 irq_ack_err;
    hi_bool irq_wakeup_flag;
    hi_bool irq_is_send_data;
} i2c_drv_irq_info;

typedef struct i2c_drv_info_s {
    hi_u32 i2c_num;
    hi_u8 i2c_dev_addr;
    hi_u32 i2c_reg_addr;
    hi_u32 i2c_reg_addr_byte_num;
    hi_u8 *data;
    hi_u32 data_len;
    i2c_drv_irq_info irq_info;
} i2c_drv_info;

i2c_drv_info g_i2c_info[HI_STD_I2C_NUM] = {0};
osal_wait g_i2c_wait_queue[HI_STD_I2C_NUM];

osal_semaphore g_i2c_sem;
osal_semaphore g_i2c_sem_rw[HI_STD_I2C_NUM];

static hi_void *g_i2c_kernel_addr[HI_STD_I2C_NUM];
static hi_u32 g_reg_i2c_store[HI_STD_I2C_NUM] = {0};
hi_u32 g_a_i2c_rate[HI_STD_I2C_NUM] = {0};

static i2c_ext_func g_st_i2c_ext_funcs = {
    .pfn_i2c_write_config = hi_drv_i2c_write_config,
    .pfn_i2c_write = hi_drv_i2c_write,
    .pfn_i2c_read = hi_drv_i2c_read,
    .pfn_i2c_write_nostop = hi_drv_i2c_write_no_stop,
    .pfn_i2c_read_directly = hi_drv_i2c_read_directly,
    .pfn_i2c_read_si_labs = hi_drv_i2c_read_si_labs,
};

hi_void i2c_drv_get_i2c_ext_func(i2c_ext_func **st_i2c_export_funcs)
{
    *st_i2c_export_funcs = &g_st_i2c_ext_funcs;
}
EXPORT_SYMBOL(i2c_drv_get_i2c_ext_func);

hi_s32 i2c_drv_get_rate(hi_u32 i2c_num)
{
    if (i2c_num >= HI_STD_I2C_NUM) {
        return HI_FAILURE;
    }
    return g_a_i2c_rate[i2c_num];
}

hi_void i2c_drv_set_rate(hi_u32 i2c_num, hi_u32 i2c_rate)
{
    hi_u32 value, sclh, scll, chip;
    const hi_u32 sys_clock = I2C_DFT_SYSCLK;

    /* 400*1000 HZ : max i2c rate */
    if (i2c_num >= HI_STD_I2C_NUM || i2c_rate > 400 * 1000 || i2c_rate == 0) {
        return;
    }

    chip = hi_drv_i2c_get_chip_version();
    g_a_i2c_rate[i2c_num] = i2c_rate;

    /* read i2c I2C_CTRL register */
    value = i2c_read_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG));

    /* close all i2c  interrupt */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG), (value & (~I2C_UNMASK_TOTAL)));

        sclh = i2c_scl_clk(sys_clock, i2c_rate); // (sys_clock / (i2c_rate * 2)) / 2 - 1
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_SCL_H_REG), sclh);

        scll = i2c_scl_clk(sys_clock, i2c_rate); // (sys_clock / (i2c_rate * 2)) / 2 - 1
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_SCL_L_REG), scll);

    /* enable i2c interrupt, resume original  interrupt */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG), value);

    return;
}

static hi_s32 i2c_drv_wait_write_end(hi_u32 i2c_num)
{
    hi_u32 i2c_sr_reg = 0;
    hi_u32 i = 0;

    do {
        i2c_sr_reg = i2c_read_reg((g_i2c_kernel_addr[i2c_num] + I2C_SR_REG));
        HI_LOG_DBG("i2c_sr_reg = 0x%x!\n", i2c_sr_reg);

        if (i > I2C_WAIT_TIME_OUT) {
            HI_LOG_INFO("i2c_sr_reg = 0x%x!\n", i2c_sr_reg);
            HI_LOG_INFO("wait write data timeout!\n");
            goto reset;
        }

        i++;
    } while ((i2c_sr_reg & I2C_OVER_INTR) != I2C_OVER_INTR);

    if (i2c_sr_reg & I2C_ACK_INTR) {
        HI_LOG_INFO("i2c_sr_reg = 0x%x!\n", i2c_sr_reg);
        HI_LOG_INFO("wait write data timeout!\n");
        goto reset;
    }

    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);

    return HI_SUCCESS;

reset:
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG), 0x0);
    /* enable relative interrupt */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG),
    (I2C_ENABLE | I2C_UNMASK_TOTAL | I2C_UNMASK_ALL));
    /* clear all i2c interrupt */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);
    /* set stop  */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_STOP);

    return HI_FAILURE;
}

static hi_s32 i2c_wait_condition(const hi_void *param)
{
    hi_s32 ret = HI_FALSE;
    hi_u32 i2c_num = *((hi_u32 *)(param));
    if (g_i2c_info[i2c_num].irq_info.irq_wakeup_flag == HI_TRUE || g_i2c_info[i2c_num].irq_info.irq_ack_err > 0) {
        ret = HI_TRUE;
    }
    return ret;
}

static hi_s32 i2c_drv_wait_interrupt(hi_u32 i2c_num)
{
    hi_s32 ret;
    ret = osal_wait_timeout_interruptible(&g_i2c_wait_queue[i2c_num],
        i2c_wait_condition, (const void *)(&i2c_num), 200); /* 200ms */

    if (g_i2c_info[i2c_num].irq_info.irq_ack_err > 0) {
        g_i2c_info[i2c_num].irq_info.irq_ack_err = 0;
        HI_LOG_INFO("wait ack err.\n");
        ret = HI_ERR_I2C_ACK_ERR;
        goto reset;
    }

    if (g_i2c_info[i2c_num].irq_info.irq_wakeup_flag == HI_FALSE) {
        HI_LOG_INFO("i2c timeout, current send count is %d\n", g_i2c_info[i2c_num].irq_info.irq_send_count);
        ret = HI_ERR_I2C_WRITE_TIMEOUT;
        goto reset;
    }
    g_i2c_info[i2c_num].irq_info.irq_wakeup_flag = HI_FALSE;

    return HI_SUCCESS;

reset:
    HI_LOG_ERR("I2C_DRV_Write I2cNum %d I2cDevAddr 0x%x I2cRegAddr 0x%x I2cRegAddrByteNum %d  failed!\n",
        g_i2c_info[i2c_num].i2c_num, g_i2c_info[i2c_num].i2c_dev_addr,
        g_i2c_info[i2c_num].i2c_reg_addr, g_i2c_info[i2c_num].i2c_reg_addr_byte_num);
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG), 0x0);
    /* enable relative interrupt */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG),
    (I2C_ENABLE | I2C_UNMASK_TOTAL | I2C_UNMASK_ALL));
    /* clear all i2c interrupt */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);
    /* set stop  */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_STOP);

    return ret;
}


static hi_s32 i2c_drv_wait_read(hi_u32 i2c_num)
{
    hi_u32 i2c_sr_reg = 0;
    hi_u32 i = 0;

    do {
        i2c_sr_reg = i2c_read_reg((g_i2c_kernel_addr[i2c_num] + I2C_SR_REG));

        if (i > I2C_WAIT_TIME_OUT) {
            HI_LOG_INFO("i2c_sr_reg = 0x%x !\n", i2c_sr_reg);
            i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG), 0x0);
            /* enable relative interrupt */
            i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG),
                (I2C_ENABLE | I2C_UNMASK_TOTAL | I2C_UNMASK_ALL));
            /* clear all i2c interrupt */
            i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);
            /* set stop  */
            i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_STOP);
            HI_LOG_INFO("wait Read data timeout!\n");
            return HI_FAILURE;
        }

        i++;
    } while ((i2c_sr_reg & I2C_RECEIVE_INTR) != I2C_RECEIVE_INTR);

    return HI_SUCCESS;
}

/*
 * I2C write finished acknowledgment function
 * it use to e2prom device ,make sure it finished write operation.
 * i2c master start next write operation must waiting when it acknowledge e2prom write cycle finished.
 */
static hi_s32 i2c_drv_write_config(hi_u32 i2c_num, hi_u8 i2c_dev_addr)
{
    hi_u32 i = 0;
    hi_u32 j = 0;
    hi_u32 i2c_sr_reg = 0;

    do {
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), (i2c_dev_addr & WRITE_OPERATION));
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), (I2C_WRITE | I2C_START));

        j = 0;
        do {
            i2c_sr_reg = i2c_read_reg((g_i2c_kernel_addr[i2c_num] + I2C_SR_REG));

            if (j > I2C_WAIT_TIME_OUT) {
                HI_LOG_ERR("wait write data timeout!\n");
                return HI_FAILURE;
            }

            j++;
        } while ((i2c_sr_reg & I2C_OVER_INTR) != I2C_OVER_INTR);

        i2c_sr_reg = i2c_read_reg((g_i2c_kernel_addr[i2c_num] + I2C_SR_REG));
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);

        i++;

        if (i > 0x200000) { // I2C_WAIT_TIME_OUT)
            HI_LOG_ERR("wait write ack ok timeout!\n");
            return HI_FAILURE;
        }
    } while ((i2c_sr_reg & I2C_ACK_INTR));

    return HI_SUCCESS;
}

static hi_s32 i2c_drv_reset(hi_u32 i2c_num)
{
    /* soft reset */
    switch (i2c_num) {
        case I2C0: {
            g_pst_reg_crg->peri_crg27.u32 = 0x2023F;
            break;
        }
        case I2C1: {
            g_pst_reg_crg->peri_crg27.u32 = 0x4023F;
            break;
        }
        case I2C2: {
            g_pst_reg_crg->peri_crg27.u32 = 0x8023F;
            break;
        }
        case I2C3: {
            g_pst_reg_crg->peri_crg27.u32 = 0x10023F;
            break;
        }
        default: {
            return HI_ERR_I2C_INVALID_PARA;
        }
    }
    /* close soft reset */
    g_pst_reg_crg->peri_crg27.u32 = 0x23F;
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG), 0x0);
    /* clear all i2c interrupt */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);
    /* enable relative interrupt */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_CTRL_REG), (I2C_ENABLE | I2C_UNMASK_TOTAL | I2C_UNMASK_ALL));

    /* reset i2c rate  */
    i2c_drv_set_rate(i2c_num, g_a_i2c_rate[i2c_num]);

    /* set stop  */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_STOP);

    return HI_SUCCESS;
}

static hi_s32 i2c_drv_set_write_mode(hi_u32 i2c_num, hi_u8 i2c_dev_addr)
{
    hi_s32 i = 0;
    hi_s32 ret = 0;

    if (i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("i2c_num = %d, over ", i2c_num);
        return HI_FAILURE;
    }

    for (i = 0 ; i < RESTART_TIME; i++) {
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);

        /* send divide address */
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), (i2c_dev_addr & WRITE_OPERATION));
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), (I2C_WRITE | I2C_START));

        ret = i2c_drv_wait_write_end(i2c_num);
        if (ret == HI_SUCCESS) {
            break;
        }
    }

    if (i > RESTART_TIME) {
        HI_LOG_ERR("i2c_drv_set_write_mode i2c_num %d i2c_dev_addr 0x%x RestartTime %d timeout!\n",
            i2c_num, i2c_dev_addr, i);
        i2c_drv_reset(i2c_num);
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    return HI_SUCCESS;
}

static hi_s32 i2c_drv_set_read_mode(hi_u32 i2c_num, hi_u8 i2c_dev_addr)
{
    if (i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("i2c_num = %d, over ", i2c_num);
        return HI_FAILURE;
    }

    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), (i2c_dev_addr | READ_OPERATION));
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_START | I2C_WRITE);

    if (i2c_drv_wait_write_end(i2c_num)) {
        HI_LOG_ERR("i2c_drv_set_read_mode i2c_num %u i2c_dev_addr 0x%x  timeout!\n",
            i2c_num, i2c_dev_addr);
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    return HI_SUCCESS;
}

/* I2C1 Irq server */
static hi_s32 i2c_read_write_data_by_irq(hi_u32 i2c_num)
{
    hi_u32 int_value;
    volatile hi_void *i2c_usr_addr = HI_NULL;

    i2c_usr_addr = g_i2c_kernel_addr[i2c_num];
    int_value = i2c_read_reg(i2c_usr_addr + I2C_SR_REG);

    /* check interrupt bit */
    if (int_value & 0x1) { /* Transfer completion interrupt flag */
        if (int_value & (1 << 6)) { /* bit 6 Host start conditional transmission end interrupt flag */
            g_i2c_info[i2c_num].irq_info.irq_wakeup_flag = HI_TRUE;
        } else if (int_value & (1 << 5)) { /* bit 5 Host stop condition transmission end interrupt flag */
            g_i2c_info[i2c_num].irq_info.irq_send_count = 0;
            g_i2c_info[i2c_num].irq_info.irq_recv_count = 0;
            g_i2c_info[i2c_num].irq_info.irq_wakeup_flag = HI_TRUE;
        }
        if (int_value & (1 << 4)) { /* bit 4 Host send interrupt flag */
            g_i2c_info[i2c_num].irq_info.irq_wakeup_flag = HI_TRUE;
            if (g_i2c_info[i2c_num].irq_info.irq_is_send_data == HI_TRUE) {
                g_i2c_info[i2c_num].irq_info.irq_send_count++;
                i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);
                if (g_i2c_info[i2c_num].irq_info.irq_send_count < g_i2c_info[i2c_num].data_len) {
                    g_i2c_info[i2c_num].irq_info.irq_wakeup_flag = HI_FALSE;
                    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG),
                        (*(g_i2c_info[i2c_num].data + g_i2c_info[i2c_num].irq_info.irq_send_count)));
                    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_WRITE);
                    return OSAL_IRQ_HANDLED;
                }
            }
        } else if (int_value & (1 << 3)) { /* bit 3 Host recv interrupt flag */
            g_i2c_info[i2c_num].irq_info.irq_wakeup_flag = HI_TRUE;
            g_i2c_info[i2c_num].irq_info.irq_recv_count++;
        }

        if (int_value & (1 << 2)) { /* bit 2 ack err interrupt flag */
            g_i2c_info[i2c_num].irq_info.irq_ack_err++;
        }

        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);
    } else {
        g_i2c_info[i2c_num].irq_info.irq_wakeup_flag = HI_FALSE;
    }

    osal_wait_wakeup(&(g_i2c_wait_queue[i2c_num]));
    return OSAL_IRQ_HANDLED;
}


hi_s32 i2c0_read_write_data_isr(hi_s32 irq, hi_void *i2c_info)
{
    return i2c_read_write_data_by_irq(I2C0);
}

hi_s32 i2c1_read_write_data_isr(hi_s32 irq, hi_void *i2c_info)
{
    return i2c_read_write_data_by_irq(I2C1);
}


static hi_s32 i2c_drv_write_byte(hi_u32 i2c_num, hi_u8 byte)
{
    if (i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("i2c_num = %u, over ", i2c_num);
        return HI_FAILURE;
    }

    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), byte);

    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_WRITE);

    if (i2c_drv_wait_write_end(i2c_num)) {
        HI_LOG_ERR("i2c_set_mode i2c_num %d byte 0x%x !\n", i2c_num, byte);
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    return HI_SUCCESS;
}

static hi_s32 i2c_drv_write(hi_u32 i2c_num, hi_u8 i2c_dev_addr,
    hi_u32 i2c_reg_addr, hi_u32 i2c_reg_addr_byte_num,
    hi_u8 *p_data, hi_u32 data_len,
    HI_BOOL b_with_stop)
{
    hi_u32 i = 0;
    hi_u32 reg_addr = 0;

    if (i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("i2c_num(%u) is wrong, STD_I2C_NUM is %d\n", i2c_num, HI_STD_I2C_NUM);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (i2c_reg_addr_byte_num < 0 || i2c_reg_addr_byte_num > 4) { // reg addr max 4 byte
        HI_LOG_ERR("i2c_reg_addr_byte_num is invalid\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    do {
        /* clear interrupt flag */
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);

        /* send divide address */
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), (i2c_dev_addr & WRITE_OPERATION));
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), (I2C_WRITE | I2C_START));
        i++;
    } while (i2c_drv_wait_write_end(i2c_num) && (i < RESTART_TIME));

    if (i >= RESTART_TIME) {
        HI_LOG_ERR("i2c_drv_write i2c_num %u i2c_dev_addr 0x%x i2c_reg_addr 0x%x i2c_reg_addr_byte_num %u\
            RestartTime %u timeout!\n", i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num, i);
        i2c_drv_reset(i2c_num);
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    /* send register address which will need to write */
    for (i = 0; i < i2c_reg_addr_byte_num; i++) {
        reg_addr = i2c_reg_addr >> ((i2c_reg_addr_byte_num - i - 1) * 8); /* 8bit */
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), reg_addr);

        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_WRITE);

        if (i2c_drv_wait_write_end(i2c_num)) {
            HI_LOG_ERR("i2c_drv_write i2c_num %d i2c_dev_addr 0x%x i2c_reg_addr 0x%x\
                i2c_reg_addr_byte_num %d timeout!\n", i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num);
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    /* send data */
    for (i = 0; i < data_len; i++) {
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), (*(p_data + i)));
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_WRITE);

        if (i2c_drv_wait_write_end(i2c_num)) {
            HI_LOG_ERR("i2c_drv_write i2c_num % i2c_dev_addr 0x%x i2c_reg_addr 0x%x\
                i2c_reg_addr_byte_num %d timeout!\n", i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num);
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    if (b_with_stop) {
        /*   send stop flag bit */
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_STOP);
        if (i2c_drv_wait_write_end(i2c_num)) {
            HI_LOG_ERR("i2c_drv_write i2c_num %u i2c_dev_addr 0x%x i2c_reg_addr 0x%x\
                i2c_reg_addr_byte_num %u timeout!\n", i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num);
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    return HI_SUCCESS;
}

int i2c_drv_send_slave_by_int(i2c_drv_info *i2c_info)
{
    hi_u32 i;
    hi_u32 reg_addr = 0;
    hi_s32 ret;

    /* send divide address */
    i2c_write_reg((g_i2c_kernel_addr[i2c_info->i2c_num] + I2C_TXR_REG), (i2c_info->i2c_dev_addr & WRITE_OPERATION));
    i2c_write_reg((g_i2c_kernel_addr[i2c_info->i2c_num] + I2C_COM_REB), (I2C_WRITE | I2C_START));
    ret = i2c_drv_wait_interrupt(i2c_info->i2c_num);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    /* send register address which will need to write */
    for (i = 0; i < i2c_info->i2c_reg_addr_byte_num; i++) {
        reg_addr = i2c_info->i2c_reg_addr >> ((i2c_info->i2c_reg_addr_byte_num - i - 1) * 8); /* 8 bit */
        i2c_write_reg((g_i2c_kernel_addr[i2c_info->i2c_num] + I2C_TXR_REG), reg_addr);
        i2c_write_reg((g_i2c_kernel_addr[i2c_info->i2c_num] + I2C_COM_REB), I2C_WRITE);
        ret = i2c_drv_wait_interrupt(i2c_info->i2c_num);
        if (ret != HI_SUCCESS) {
            return ret;
        }
        i2c_info->irq_info.irq_send_count = 0;
    }
    return HI_SUCCESS;
}


static 	int i2c_drv_read(hi_u32 i2c_num, hi_u8 i2c_dev_addr, HI_BOOL b_send_slave, hi_u32 i2c_reg_addr,\
    hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len)
{
    hi_u32 data_tmp = 0xff;
    hi_u32 i = 0;
    hi_u32 reg_addr = 0;
    hi_s32 ret = 0;

    if (i2c_num >= HI_STD_I2C_NUM || i2c_reg_addr_byte_num > 4) { /* max reg address len is 4 byte */
        HI_LOG_ERR("i2c param is invalid\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (b_send_slave) {
        ret = i2c_drv_set_write_mode(i2c_num, i2c_dev_addr);
        if (ret != HI_SUCCESS) {
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
        /* send register address which will need to write */
        for (i = 0; i < i2c_reg_addr_byte_num; i++) {
            reg_addr = i2c_reg_addr >> ((i2c_reg_addr_byte_num - i - 1) * 8);  /* 8 bit */
            i2c_drv_write_byte(i2c_num, reg_addr);
        }
    }

    /* send register address which will need to read */
    ret = i2c_drv_set_read_mode(i2c_num, i2c_dev_addr);
    if (ret != HI_SUCCESS) {
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }
    /* repetitivily read data */
    for (i = 0; i < data_len; i++) {
        /*  the last byte don't need send ACK */
        if (i == (data_len - 1)) {
            i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), (I2C_READ | (~I2C_SEND_ACK)));
        } else { /*  if i2c master receive data will send ACK */
            i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_READ);
        }

        if (i2c_drv_wait_read(i2c_num)) {
            goto TIMEOUT;
        }

        data_tmp = i2c_read_reg((g_i2c_kernel_addr[i2c_num] + I2C_RXR_REG));
        *(p_data + i) = data_tmp & 0xff;

        if (i2c_drv_wait_write_end(i2c_num)) {
            goto TIMEOUT;
        }
    }

    /* send stop flag bit */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_STOP);
    if (i2c_drv_wait_write_end(i2c_num)) {
        goto TIMEOUT;
    }

    return HI_SUCCESS;
TIMEOUT:
    HI_LOG_ERR("i2c_drv_read i2c_num %u i2c_dev_addr 0x%x i2c_reg_addr 0x%x i2c_reg_addr_byte_num %u timeout!\n",
        i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num);
    return HI_ERR_I2C_WRITE_TIMEOUT;
}

static int i2c_drv_read_si_labs(hi_u32 i2c_num, hi_u8 i2c_dev_addr, HI_BOOL b_send_slave, hi_u32 i2c_reg_addr,\
    hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len)
{
    hi_u32 data_tmp = 0xff;
    hi_u32 i = 0;

    if (i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("i2c_num(%u) is wrong, STD_I2C_NUM is %d\n", i2c_num, HI_STD_I2C_NUM);
        return HI_ERR_I2C_INVALID_PARA;
    }

    /* clear interrupt flag */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);

    /* send register address which will need to read */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), (i2c_dev_addr | READ_OPERATION));
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_WRITE | I2C_START);

    if (i2c_drv_wait_write_end(i2c_num)) {
        HI_LOG_ERR("i2c_drv_read i2c_num %d i2c_dev_addr 0x%x i2c_reg_addr 0x%x i2c_reg_addr_byte_num %d timeout!\n",
                   i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num);
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    /* repetitivily read data */
    for (i = 0; i < data_len; i++) {
        /*  the last byte don't need send ACK */
        if (i == (data_len - 1)) {
            i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), (I2C_READ | (~I2C_SEND_ACK)));
        } else { /*  if i2c master receive data will send ACK */
            i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_READ);
        }

        if (i2c_drv_wait_read(i2c_num)) {
            HI_LOG_ERR("i2c_drv_read i2c_num %d i2c_dev_addr 0x%x i2c_reg_addr 0x%x\
                i2c_reg_addr_byte_num %d timeout!\n", i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num);
            return HI_ERR_I2C_READ_TIMEOUT;
        }

        data_tmp = i2c_read_reg((g_i2c_kernel_addr[i2c_num] + I2C_RXR_REG));
        *(p_data + i) = data_tmp & 0xff;

        if (i2c_drv_wait_write_end(i2c_num)) {
            HI_LOG_ERR("i2c_drv_read i2c_num %d i2c_dev_addr 0x%x i2c_reg_addr 0x%x\
                i2c_reg_addr_byte_num %d timeout!\n", i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num);
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    /* send stop flag bit */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_STOP);
    if (i2c_drv_wait_write_end(i2c_num)) {
        HI_LOG_ERR("i2c_drv_read i2c_num %u i2c_dev_addr 0x%x i2c_reg_addr 0x%x i2c_reg_addr_byte_num %u timeout!\n",
                   i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num);
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    return HI_SUCCESS;
}

static hi_s32 i2c_drv_write_sony(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr, hi_u32 i2c_reg_addr_byte_num,
    hi_u8 *p_data, hi_u32 data_len, hi_u8 mode)
{
    hi_u32 i = 0;
    hi_u32 reg_addr = 0;

    /*  clear interrupt flag */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_ICR_REG), I2C_CLEAR_ALL);

    /* send divide address */
    i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), (i2c_dev_addr & WRITE_OPERATION));

    if (mode & SONY_I2C_START_EN) {
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), (I2C_WRITE | I2C_START));
    } else {
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_WRITE);
    }

    if (i2c_drv_wait_write_end(i2c_num)) {
        HI_LOG_ERR("wait write data timeout!%s, %d\n", __func__, __LINE__);
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    /* send register address which will need to write */
    for (i = 0; i < i2c_reg_addr_byte_num; i++) {
        reg_addr = i2c_reg_addr >> ((i2c_reg_addr_byte_num - i - 1) * 8); /* 8 bit */
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), reg_addr);

        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_WRITE);

        if (i2c_drv_wait_write_end(i2c_num)) {
            HI_LOG_ERR("wait write data timeout!%s, %d\n", __func__, __LINE__);
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    /* send data */
    for (i = 0; i < data_len; i++) {
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), (*(p_data + i)));
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_WRITE);

        if (i2c_drv_wait_write_end(i2c_num)) {
            HI_LOG_ERR("wait write data timeout!%s, %d\n", __func__, __LINE__);
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    /*   send stop flag bit */
    if (mode & SONY_I2C_STOP_EN) {
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_STOP);
        if (i2c_drv_wait_write_end(i2c_num)) {
            HI_LOG_ERR("wait write data timeout!%s, %d\n", __func__, __LINE__);
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    return HI_SUCCESS;
}

static int i2c_drv_read_sony(hi_u32 i2c_num, hi_u8 i2c_dev_addr,
    hi_u32 i2c_reg_addr, hi_u32 i2c_reg_addr_byte_num,
    hi_u8 *p_data, hi_u32 data_len, hi_u8 mode)
{
    hi_u32 data_tmp = 0xff;
    hi_u32 i = 0;

    if (mode & SONY_I2C_START_EN) {
        /* send register address which will need to read */
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_TXR_REG), (i2c_dev_addr | READ_OPERATION));
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_WRITE | I2C_START);

        if (i2c_drv_wait_write_end(i2c_num)) {
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    /* repetitivily read data */
    for (i = 0; i < data_len; i++) {
        /*  the last byte don't need send ACK */
        if (i == (data_len - 1)) {
            i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), (I2C_READ | (~I2C_SEND_ACK)));
        } else { /*  if i2c master receive data will send ACK */
            i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_READ);
        }

        if (i2c_drv_wait_read(i2c_num)) {
            HI_LOG_ERR("wait read data timeout!\n");
            return HI_ERR_I2C_READ_TIMEOUT;
        }

        data_tmp = i2c_read_reg((g_i2c_kernel_addr[i2c_num] + I2C_RXR_REG));
        *(p_data + i) = data_tmp & 0xff;

        if (i2c_drv_wait_write_end(i2c_num)) {
            HI_LOG_ERR("wait write data timeout!\n");
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    if (mode & SONY_I2C_STOP_EN) {
        /* send stop flag bit */
        i2c_write_reg((g_i2c_kernel_addr[i2c_num] + I2C_COM_REB), I2C_STOP);
        if (i2c_drv_wait_write_end(i2c_num)) {
            HI_LOG_ERR("wait write data timeout!\n");
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    :
 Description  : I2C  mudole suspend function
 Input        : None
 Output       : None
 return value : None
*****************************************************************************/
struct i2c_pm_info {
    unsigned int rsclh;
    unsigned int rscll;
};
static int g_i2c_state = 0;
static struct i2c_pm_info g_pm_status[HI_I2C_MAX_NUM];

/*
because this module have opened in  tuner/e2prom ModeuleInit,
so relational opened operation register need to  store
*/
int i2c_pm_suspend(hi_void *private_data)
{
#if !defined(CFG_HI_USER_DRV)
    int i = 0;
    int ret ;

    ret = osal_sem_trydown(&g_i2c_sem);
    if (ret) {
        HI_LOG_ERR("lock err!\n");
        return -1;
    }

    for (i = 0; i < HI_STD_I2C_NUM; i++) {
        /* disable all i2c interrupt */
        i2c_write_reg((g_i2c_kernel_addr[i] + I2C_CTRL_REG), 0x0);

        /* clear all i2c interrupt */
        i2c_write_reg((g_i2c_kernel_addr[i] + I2C_ICR_REG), I2C_CLEAR_ALL);

        /* store  I2C_SCL_H and  I2C_SCL_L  register */
        g_pm_status[i].rsclh = i2c_read_reg(g_i2c_kernel_addr[i] + I2C_SCL_H_REG);
        g_pm_status[i].rscll = i2c_read_reg(g_i2c_kernel_addr[i] + I2C_SCL_L_REG);
    }

    g_reg_i2c_store[0] = g_pst_reg_crg->peri_crg27.u32;

    osal_sem_up(&g_i2c_sem);
    HI_PRINT("I2C suspend OK\n");
#endif
    return 0;
}

int i2c_pm_resume(hi_void *private_data)
{
    int i = 0;
    int ret;

    ret = osal_sem_trydown(&g_i2c_sem);
    if (ret) {
        HI_LOG_ERR("lock err!\n");
        return -1;
    }

    g_pst_reg_crg->peri_crg27.u32 = g_reg_i2c_store[0];

    for (i = 0; i < HI_STD_I2C_NUM; i++) {
        /* disable all i2c interrupt */
        i2c_write_reg((g_i2c_kernel_addr[i] + I2C_CTRL_REG), 0x0);

        /* resume previous store register before suspend */
        i2c_write_reg((g_i2c_kernel_addr[i] + I2C_SCL_H_REG), g_pm_status[i].rsclh);
        i2c_write_reg((g_i2c_kernel_addr[i] + I2C_SCL_L_REG), g_pm_status[i].rscll);

        /*  config scl clk rate */
        g_a_i2c_rate[i] = I2C_DFT_RATE;
        i2c_drv_set_rate(i, I2C_DFT_RATE);

        /* clear all i2c interrupt */
        i2c_write_reg((g_i2c_kernel_addr[i] + I2C_ICR_REG), I2C_CLEAR_ALL);

        /* enable relative interrupt */
        i2c_write_reg((g_i2c_kernel_addr[i] + I2C_CTRL_REG), (I2C_ENABLE | I2C_UNMASK_TOTAL | I2C_UNMASK_ALL));
    }

    osal_sem_up(&g_i2c_sem);
    HI_PRINT("I2C resume OK\n");
    return 0;
}

hi_s32 i2c_drv_lowpower_enter(hi_void *private_data)
{
    HI_LOG_NOTICE("I2C lowpower enter OK\n");
    return HI_SUCCESS;
}

hi_s32 i2c_drv_lowpower_exit(hi_void *private_data)
{
    HI_LOG_NOTICE("I2C lowpower exit OK\n");
    return HI_SUCCESS;
}


static hi_void hi_drv_i2c_open(hi_void)
{
    hi_s32 ret;
    hi_u32 i = 0;

    if (g_i2c_state == 1) {
        return;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem);
    if (ret) {
        HI_LOG_INFO("lock g_i2c_sem error.\n");
        return;
    }

    for (i = 0; i < HI_STD_I2C_NUM; i++) {
        /* disable all i2c interrupt */
        i2c_write_reg((g_i2c_kernel_addr[i] + I2C_CTRL_REG), 0x0);

        /* enable relative interrupt */
        i2c_write_reg((g_i2c_kernel_addr[i] + I2C_CTRL_REG), (I2C_ENABLE | I2C_UNMASK_TOTAL | I2C_UNMASK_ALL));

        /* clear all i2c interrupt */
        i2c_write_reg((g_i2c_kernel_addr[i] + I2C_ICR_REG), I2C_CLEAR_ALL);

        /* config scl clk rate */
        g_a_i2c_rate[i] = I2C_DFT_RATE;
        i2c_drv_set_rate(i, I2C_DFT_RATE);
    }
#ifdef HI_UNIFIED_IMAGE_SUPPORT
    ret = hi_get_dts_config_u32_byname("i2c", "i2c_rw_use_interrupt", &g_i2c_rw_irq_enable);
    if (ret != HI_SUCCESS) {
        g_i2c_rw_irq_enable = 0;
        HI_LOG_ERR("ERR: call hi_get_dts_config_u32_byname i2c_rw_use_interrupt fail\n");
    }
#endif
    g_i2c_state = 1;

    osal_sem_up(&g_i2c_sem);
    return;
}


static hi_s32 hi_drv_std_i2c_write(i2c_data *i2c_data_ptr)
{
    hi_u8 *p_data = NULL;
    errno_t err_ret;
    hi_s32 ret;
    hi_u32 chip;

    hi_dbg_func_enter();

    if (i2c_data_ptr == NULL || i2c_data_ptr->data_len == 0 || i2c_data_ptr->data_len > HI_I2C_MAX_LENGTH) {
        HI_LOG_ERR("i2c param is invalid.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    p_data = osal_kmalloc(HI_ID_I2C, i2c_data_ptr->data_len, OSAL_GFP_KERNEL);
    if (p_data == NULL) {
        HI_LOG_ERR("i2c kmalloc fail!\n");
        return HI_ERR_I2C_MALLOC_ERR;
    }

    err_ret = memset_s(p_data, i2c_data_ptr->data_len, 0, i2c_data_ptr->data_len);
    if (err_ret != EOK) {
        osal_kfree(HI_ID_I2C, p_data);
        HI_LOG_ERR("secure func call error\n");
        return HI_FAILURE;
    }

    if (osal_copy_from_user(p_data, (hi_void *)(uintptr_t)(i2c_data_ptr->p_data),
        i2c_data_ptr->data_len)) {
        osal_kfree(HI_ID_I2C, p_data);
        p_data = HI_NULL;
        return HI_ERR_I2C_COPY_DATA_ERR;
    }
    chip = hi_drv_i2c_get_chip_version();
    ret = i2c_drv_write(i2c_data_ptr->i2c_num, i2c_data_ptr->i2c_dev_addr, i2c_data_ptr->i2c_reg_addr,
        i2c_data_ptr->i2c_reg_count, p_data, i2c_data_ptr->data_len, HI_TRUE);
    osal_kfree(HI_ID_I2C, p_data);
    p_data = HI_NULL;

    hi_dbg_func_exit();

    return ret;
}

hi_s32 hi_drv_i2c_cmd_write(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    hi_s32 ret;
    i2c_data *i2c_data_ptr = NULL;
    hi_dbg_func_enter();

    i2c_data_ptr = (i2c_data *)arg;

    if (i2c_data_ptr->i2c_num >= HI_I2C_MAX_NUM) {
        HI_LOG_INFO("i2c chanenl (NO:%d) is invalid !\n", i2c_data_ptr->i2c_num);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (HI_STD_I2C_NUM <= i2c_data_ptr->i2c_num) {
        ret = hi_drv_gpioi2c_cmd_write(cmd, arg, private_data);
        return ret;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_data_ptr->i2c_num]);
    if (ret) {
        HI_LOG_INFO("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }

    ret = hi_drv_std_i2c_write(i2c_data_ptr);

    osal_sem_up(&g_i2c_sem_rw[i2c_data_ptr->i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

static hi_s32 hi_drv_std_i2c_read(unsigned int cmd, i2c_data *i2c_data_ptr)
{
    hi_s32 ret;
    hi_u32 chip;
    hi_u8 *p_data = NULL;
    errno_t err_ret;
    hi_bool send_slave = HI_TRUE;

    if (i2c_data_ptr == NULL) {
        HI_LOG_ERR("i2c_data_ptr is null pointer.\n");
        return HI_ERR_I2C_INVALID_PARA;
    }
    if (i2c_data_ptr->data_len > HI_I2C_MAX_LENGTH || i2c_data_ptr->data_len == 0) {
        HI_LOG_ERR("para i2c_data.data_len[%d] is invalid.\n", i2c_data_ptr->data_len);
        return HI_ERR_I2C_INVALID_PARA;
    }

    p_data = osal_kmalloc(HI_ID_I2C, i2c_data_ptr->data_len, OSAL_GFP_KERNEL);
    if (p_data == NULL) {
        HI_LOG_ERR("i2c kmalloc fail!\n");
        return HI_ERR_I2C_MALLOC_ERR;
    }

    err_ret = memset_s(p_data, i2c_data_ptr->data_len, 0, i2c_data_ptr->data_len);
    if (err_ret != EOK) {
        osal_kfree(HI_ID_I2C, p_data);
        HI_LOG_ERR("secure func call error\n");
        return HI_FAILURE;
    }

    if (cmd == CMD_I2C_READ_DIRECT) {
        send_slave = HI_FALSE;
    }

    chip = hi_drv_i2c_get_chip_version();
    if (i2c_data_ptr->i2c_reg_count != 0) {
        ret = i2c_drv_read(i2c_data_ptr->i2c_num, i2c_data_ptr->i2c_dev_addr, send_slave,
            i2c_data_ptr->i2c_reg_addr, i2c_data_ptr->i2c_reg_count,
            p_data, i2c_data_ptr->data_len);
    } else {
        ret = i2c_drv_read_si_labs(i2c_data_ptr->i2c_num, i2c_data_ptr->i2c_dev_addr, \
            HI_TRUE, i2c_data_ptr->i2c_reg_addr, i2c_data_ptr->i2c_reg_count,
            p_data, i2c_data_ptr->data_len);
    }

    if (ret == HI_SUCCESS) {
        if (osal_copy_to_user((hi_void *)((uintptr_t)(i2c_data_ptr->p_data)), p_data, i2c_data_ptr->data_len)) {
            HI_LOG_INFO("copy data to user fail!\n");
            ret = HI_ERR_I2C_COPY_DATA_ERR;
        }
    }

    osal_kfree(HI_ID_I2C, p_data);
    p_data = HI_NULL;

    hi_dbg_func_exit();

    return ret;
}

hi_s32 hi_drv_i2c_cmd_read(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    hi_s32 ret;
    i2c_data *i2c_data_ptr = NULL;

    hi_dbg_func_enter();

    i2c_data_ptr = (i2c_data *)arg;
    if (i2c_data_ptr->i2c_num >= HI_I2C_MAX_NUM) {
        HI_LOG_INFO("i2c chanenl (NO:%d) is invalid !\n", i2c_data_ptr->i2c_num);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (HI_STD_I2C_NUM <= i2c_data_ptr->i2c_num) {
        ret = hi_drv_gpioi2c_cmd_read(cmd, arg, private_data);
        return ret;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_data_ptr->i2c_num]);
    if (ret) {
        HI_LOG_INFO("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }
    ret = hi_drv_std_i2c_read(cmd, i2c_data_ptr);

    osal_sem_up(&g_i2c_sem_rw[i2c_data_ptr->i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

hi_s32 hi_drv_i2c_cmd_set_rate(unsigned int cmd, hi_void *arg, hi_void *private_data)
{
    hi_s32 ret;
    i2c_rate *i2c_rate_ptr = NULL;

    hi_dbg_func_enter();

    i2c_rate_ptr = (i2c_rate *)arg;
    if (i2c_rate_ptr->i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_INFO("i2c chanenl (NO:%d) is invalid !\n", i2c_rate_ptr->i2c_num);
        return HI_ERR_I2C_INVALID_PARA;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_rate_ptr->i2c_num]);
    if (ret) {
        HI_LOG_INFO("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }

    i2c_drv_set_rate(i2c_rate_ptr->i2c_num, i2c_rate_ptr->i2c_rate);
    ret = HI_SUCCESS;

    osal_sem_up(&g_i2c_sem_rw[i2c_rate_ptr->i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

static hi_void init_reg_crg(hi_void)
{
#ifdef CFG_HI_USER_DRV
#define V350_REG_BASE 0xF8A22000
#define V350_REG_REMAP_SIZE 4096
    if (g_pst_reg_crg != NULL) {
        return;
    }
    g_pst_reg_crg = (hi_reg_crg *)osal_ioremap_nocache(V350_REG_BASE, V350_REG_REMAP_SIZE);
    if (g_pst_reg_crg == NULL) {
        HI_LOG_FATAL("g_pst_reg_crg osal_ioremap_nocache failure!\n");
        return;
    }
#endif
    return;
}

hi_s32 hi_drv_i2c_init(hi_void)
{
    hi_u32 reg_val;
    hi_s32 ret;
    hi_u32 i = 0;

    hi_dbg_func_enter();

    if (g_i2c_state == 1) {
        return HI_SUCCESS;
    }

    init_reg_crg();

    ret = osal_exportfunc_register(HI_ID_I2C, "HI_I2C", (hi_void *)&g_st_i2c_ext_funcs);
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO(" I2C Module register failed 0x%x.\n", ret);
        return HI_FAILURE;
    }

    ret = osal_sem_init(&g_i2c_sem, 1);
    if (ret != HI_SUCCESS) {
        HI_LOG_FATAL(" I2C semaphore init failed.\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_STD_I2C_NUM; i++) {
        /* init semaphore for each bus */
        ret = osal_sem_init(&g_i2c_sem_rw[i], 1);
        if (ret != HI_SUCCESS) {
            HI_LOG_FATAL(" I2C rw semaphore init failed.\n");
            return HI_FAILURE;
        }
        /* init wait queue for each bus */
        osal_wait_init(&g_i2c_wait_queue[i]);
        /* clean i2c's reg base address */
        g_i2c_kernel_addr[i] = NULL;
    }

    HI_I2C_REG_REMAP(g_i2c_kernel_addr[0], I2C0_PHY_ADDR); // I2C0 base addr
    HI_I2C_REG_REMAP(g_i2c_kernel_addr[1], I2C1_PHY_ADDR); // I2C1 base addr
    HI_I2C_REG_REMAP(g_i2c_kernel_addr[2], I2C2_PHY_ADDR); // I2C2 base addr
    HI_I2C_REG_REMAP(g_i2c_kernel_addr[3], I2C3_PHY_ADDR); // I2C3 base addr
    HI_I2C_REG_REMAP(g_i2c_kernel_addr[4], I2C_TUNER_ADDR); // 4 I2C_TUNER base addr
    HI_I2C_REG_REMAP(g_i2c_kernel_addr[5], I2C_DTMB_ADDR); // 5 I2C_DTMB base addr
    HI_I2C_REG_REMAP(g_i2c_kernel_addr[6], I2C_ADC_ADDR); // 6 I2C_ADC base addr

    reg_val = g_pst_reg_crg->peri_crg27.u32;
        reg_val &= ~0x27E023F;
        reg_val |= 0x23F;
    g_pst_reg_crg->peri_crg27.u32 = reg_val;

    hi_drv_i2c_open();
    hi_dbg_func_exit();

    return 0;
}

hi_void hi_drv_i2c_de_init(hi_void)
{
    hi_u32 reg_val;
    hi_u32 i = 0;
    i2c_drv_chip chip;
    hi_s32 ret;

    hi_dbg_func_enter();

    chip = hi_drv_i2c_get_chip_version();

    osal_sem_destory(&g_i2c_sem);
    for (i = 0; i < HI_STD_I2C_NUM; i++) {
        osal_sem_destory(&g_i2c_sem_rw[i]);
        osal_wait_destroy(&g_i2c_wait_queue[i]);
    }

    ret = osal_exportfunc_unregister(HI_ID_I2C);
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO(" GPIO Module unregister failed 0x%x.\n", ret);
    }

    reg_val = g_pst_reg_crg->peri_crg27.u32;
        reg_val &= ~0x27E023F;
        reg_val |= 0x27E0000;
    g_pst_reg_crg->peri_crg27.u32 = reg_val;

    /* unmap i2c's reg address */
    for (i = 0; i < HI_STD_I2C_NUM; i++) {
        HI_I2C_REG_UNMAP(g_i2c_kernel_addr[i]);
        g_i2c_kernel_addr[i] = NULL;
    }
    g_i2c_state = 0;

    hi_dbg_func_exit();

    return;
}

hi_s32 hi_drv_i2c_write_config(hi_u32 i2c_num, hi_u8 i2c_dev_addr)
{
    hi_s32 ret;

    hi_dbg_func_enter();

    if (i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("i2c_num(%u) is wrong, STD_I2C_NUM is %d\n", i2c_num, HI_STD_I2C_NUM);
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_num]);
    if (ret) {
        HI_LOG_INFO("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }

    ret = i2c_drv_write_config(i2c_num, i2c_dev_addr);

    osal_sem_up(&g_i2c_sem_rw[i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

static hi_s32 hi_drv_i2c_parm_check(hi_u32 i2c_num, hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len)
{
    if (i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("i2c_num(%d) is wrong, STD_I2C_NUM is %d\n", i2c_num, HI_STD_I2C_NUM);
        return HI_FAILURE;
    }

    if (i2c_reg_addr_byte_num > 4) { /* max reg address len is 4 byte */
        HI_LOG_ERR("para i2c_reg_addr_byte_num is invalid.\n");
        return HI_FAILURE;
    }

    if (p_data == NULL) {
        HI_LOG_ERR("para p_data is null.\n");
        return HI_FAILURE;
    }

    if ((data_len > HI_I2C_MAX_LENGTH) || (data_len == 0)) {
        HI_LOG_ERR("para data_len is invalid.\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

hi_s32 hi_drv_i2c_write(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr,
    hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len)
{
    hi_s32 ret;
    hi_u32 chip;

    hi_dbg_func_enter();

    if (i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("i2c_num(%d) is wrong, STD_I2C_NUM is %d\n", i2c_num, HI_STD_I2C_NUM);
        return HI_FAILURE;
    }

    if (i2c_reg_addr_byte_num > 4) { /* max reg address len is 4 byte */
        HI_LOG_ERR("para i2c_reg_addr_byte_num is invalid.\n");
        return HI_FAILURE;
    }

    if (p_data == NULL) {
        HI_LOG_ERR("para p_data is null.\n");
        return HI_FAILURE;
    }

    if (data_len > HI_I2C_MAX_LENGTH) {
        HI_LOG_ERR("para data_len is invalid.\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_num]);
    if (ret) {
        HI_LOG_INFO("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }

    chip = hi_drv_i2c_get_chip_version();
    ret = i2c_drv_write(i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num, p_data, data_len, HI_TRUE);
    HI_LOG_INFO("ret=0x%x, i2c_num=%u, DevAddr=0x%x, reg_addr=0x%x, Num=%u, Len=%u\n", ret, i2c_num,
                i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num, data_len);

    osal_sem_up(&g_i2c_sem_rw[i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

hi_s32 hi_drv_i2c_write_no_stop(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr,
    hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len)
{
    hi_s32 ret;

    hi_dbg_func_enter();

    if (i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("i2c_num(%d) is wrong, STD_I2C_NUM is %d\n", i2c_num, HI_STD_I2C_NUM);
        return HI_FAILURE;
    }

    if (i2c_reg_addr_byte_num > 4) { /* max reg address len is 4 byte */
        HI_LOG_ERR("para i2c_reg_addr_byte_num is invalid.\n");
        return HI_FAILURE;
    }

    if (p_data == NULL) {
        HI_LOG_ERR("para p_data is null.\n");
        return HI_FAILURE;
    }

    if ((data_len > HI_I2C_MAX_LENGTH)) {
        HI_LOG_ERR("para data_len is invalid.\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_num]);
    if (ret) {
        HI_LOG_INFO("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }

    ret = i2c_drv_write(i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num, p_data, data_len, HI_FALSE);
    HI_LOG_INFO("ret=0x%x, i2c_num=%d, DevAddr=0x%x, reg_addr=0x%x, Num=%d, Len=%d\n", ret, i2c_num,
                i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num, data_len);

    osal_sem_up(&g_i2c_sem_rw[i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

hi_s32 hi_drv_i2c_read(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr,
    hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len)
{
    hi_s32 ret;
    hi_u32 chip;

    hi_dbg_func_enter();

    ret = hi_drv_i2c_parm_check(i2c_num, i2c_reg_addr_byte_num, p_data, data_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO("hi_drv_i2c_parm_check error.\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_num]);
    if (ret) {
        HI_LOG_INFO("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }

    chip = hi_drv_i2c_get_chip_version();
    ret = i2c_drv_read(i2c_num, i2c_dev_addr, HI_TRUE, i2c_reg_addr, i2c_reg_addr_byte_num, p_data, data_len);
    hi_dbg_print_h32(ret);
    hi_dbg_print_s32(i2c_num);
    hi_dbg_print_h32(i2c_dev_addr);
    hi_dbg_print_h32(i2c_reg_addr);
    hi_dbg_print_s32(i2c_reg_addr_byte_num);
    hi_dbg_print_s32(data_len);

    osal_sem_up(&g_i2c_sem_rw[i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

hi_s32 hi_drv_i2c_read_si_labs(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr, hi_u32 i2c_reg_addr_byte_num,
    hi_u8 *p_data, hi_u32 data_len)
{
    hi_s32 ret;

    hi_dbg_func_enter();

    ret = hi_drv_i2c_parm_check(i2c_num, i2c_reg_addr_byte_num, p_data, data_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO("hi_drv_i2c_parm_check error.\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_num]);
    if (ret) {
        HI_LOG_INFO("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }

    ret = i2c_drv_read_si_labs(i2c_num, i2c_dev_addr, HI_TRUE, i2c_reg_addr, i2c_reg_addr_byte_num, p_data, data_len);

    osal_sem_up(&g_i2c_sem_rw[i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

hi_s32 hi_drv_i2c_write_sony(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr, hi_u32 i2c_reg_addr_byte_num,\
    hi_u8 *p_data, hi_u32 data_len, hi_u8 mode)
{
    hi_s32 ret;

    hi_dbg_func_enter();

    if (i2c_num >= HI_STD_I2C_NUM) {
        HI_LOG_ERR("i2c_num(%d) is wrong, STD_I2C_NUM is %d\n", i2c_num, HI_STD_I2C_NUM);
        return HI_FAILURE;
    }

    if (i2c_reg_addr_byte_num > 4) { /* max reg address len is 4 byte */
        HI_LOG_ERR("para i2c_reg_addr_byte_num is invalid.\n");
        return HI_FAILURE;
    }

    if (p_data == NULL) {
        HI_LOG_ERR("para p_data is null.\n");
        return HI_FAILURE;
    }

    if ((data_len > HI_I2C_MAX_LENGTH)) {
        HI_LOG_ERR("para data_len is invalid.\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_num]);
    if (ret) {
        HI_LOG_ERR("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }

    ret = i2c_drv_write_sony(i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num, p_data, data_len, mode);
    HI_LOG_INFO("ret=0x%x, i2c_num=%d, DevAddr=0x%x, reg_addr=0x%x, Num=%d, Len=%d, data0=0x%x\n", ret, i2c_num,
                i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num, data_len, p_data[0]);

    osal_sem_up(&g_i2c_sem_rw[i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

hi_s32 hi_drv_i2c_read_sony(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr,
    hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len, hi_u8 mode)
{
    hi_s32 ret;

    hi_dbg_func_enter();

    ret = hi_drv_i2c_parm_check(i2c_num, i2c_reg_addr_byte_num, p_data, data_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO("hi_drv_i2c_parm_check error.\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_num]);
    if (ret) {
        HI_LOG_ERR("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }

    ret = i2c_drv_read_sony(i2c_num, i2c_dev_addr, i2c_reg_addr, i2c_reg_addr_byte_num, p_data, data_len, mode);
    HI_LOG_INFO("ret=0x%x, i2c_num=%d, DevAddr=0x%x, reg_addr=0x%x, Num=%d, Len=%d\n", ret, i2c_num, i2c_dev_addr,
                i2c_reg_addr, i2c_reg_addr_byte_num, data_len);

    osal_sem_up(&g_i2c_sem_rw[i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

/* Some I2C needn't send slave address before read */
hi_s32 hi_drv_i2c_read_directly(hi_u32 i2c_num, hi_u8 i2c_dev_addr, hi_u32 i2c_reg_addr, \
    hi_u32 i2c_reg_addr_byte_num, hi_u8 *p_data, hi_u32 data_len)
{
    hi_s32 ret;

    hi_dbg_func_enter();

    ret = hi_drv_i2c_parm_check(i2c_num, i2c_reg_addr_byte_num, p_data, data_len);
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO("hi_drv_i2c_parm_check error.\n");
        return HI_FAILURE;
    }

    ret = osal_sem_down_interruptible(&g_i2c_sem_rw[i2c_num]);
    if (ret) {
        HI_LOG_INFO("lock g_i2c_sem error.\n");
        return HI_FAILURE;
    }

    ret = i2c_drv_read(i2c_num, i2c_dev_addr, HI_FALSE, i2c_reg_addr, i2c_reg_addr_byte_num, p_data, data_len);
    HI_LOG_INFO("ret=0x%x, i2c_num=%d, DevAddr=0x%x, reg_addr=0x%x, Num=%d, Len=%d\n", ret, i2c_num, i2c_dev_addr,
                i2c_reg_addr, i2c_reg_addr_byte_num, data_len);

    osal_sem_up(&g_i2c_sem_rw[i2c_num]);

    hi_dbg_func_exit();

    return ret;
}

#ifndef MODULE
EXPORT_SYMBOL(i2c_pm_suspend);
EXPORT_SYMBOL(i2c_pm_resume);
#endif
EXPORT_SYMBOL(i2c_drv_lowpower_enter);
EXPORT_SYMBOL(i2c_drv_lowpower_exit);


EXPORT_SYMBOL(hi_drv_i2c_init);
EXPORT_SYMBOL(hi_drv_i2c_de_init);

EXPORT_SYMBOL(hi_drv_i2c_write_config);
EXPORT_SYMBOL(hi_drv_i2c_write);
EXPORT_SYMBOL(hi_drv_i2c_read);
EXPORT_SYMBOL(hi_drv_i2c_read_sony);
EXPORT_SYMBOL(hi_drv_i2c_write_sony);
EXPORT_SYMBOL(hi_drv_i2c_read_directly);
EXPORT_SYMBOL(hi_drv_i2c_write_no_stop);
EXPORT_SYMBOL(hi_drv_i2c_read_si_labs);
