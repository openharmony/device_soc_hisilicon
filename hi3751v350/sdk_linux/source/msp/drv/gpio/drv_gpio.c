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
#ifdef __KERNEL__
#include <linux/interrupt.h>
#endif

#include "drv_gpio.h"
#include "hi_log.h"
#include "hi_error_mpi.h"
#include "hi_drv_gpio.h"
#include "hi_common.h"
#include "hi_module.h"

#if !defined(CFG_HI_USER_DRV)
#include "hi_drv_sys.h"
#endif

#define GPIO_MAX_BUF  100
#define GPIO_ONE_GROUP_BIT_NUM 8

#define GPIO_BUF_HEAD g_st_gpio_attr.gpio_int_buf[g_st_gpio_attr.head]
#define GPIO_BUF_TAIL g_st_gpio_attr.gpio_int_buf[g_st_gpio_attr.tail]

#define inc_buf(x, len) (((x) + 1) % (len))

#define INT_TYPE_NONE 0x0
#define INT_TYPE_EDGE_RISING 0x1
#define INT_TYPE_EDGE_FALLING 0x2
#define INT_TYPE_EDGE_BOTH  (INT_TYPE_EDGE_FALLING | INT_TYPE_EDGE_RISING)
#define INT_TYPE_LEVEL_HIGH 0x4
#define INT_TYPE_LEVEL_LOW  0x8
#define GPIO_IRQF_ONESHOT 0x00002000

typedef struct {
    hi_bool gpio_int_enable;
    gpio_inttpye gpio_int_type;
    hi_void (*gpio_server)(hi_u32 gpio_no);
} gpio_int_attr;

typedef struct {
    hi_u32 head;
    hi_u32 tail;
    hi_u32 gpio_int_buf[GPIO_MAX_BUF];
    hi_u32 gpio_buf_len;
    hi_u32 gpio_block_time;
    gpio_int_attr gpio_int_attr[HI_GPIO_MAX_BIT_NUM];
    osal_wait  gpio_int_wait_queue;
} gpio_attr;

static gpio_attr g_st_gpio_attr;

#define GPIO_IRQ_NUM 2

static gpio_ext_func g_st_gpio_export_funcs = {
    .pfn_gpio_dir_get_bit = hi_drv_gpio_get_dir_bit,
    .pfn_gpio_dir_set_bit = hi_drv_gpio_set_dir_bit,
    .pfn_gpio_read_bit = hi_drv_gpio_read_bit,
    .pfn_gpio_write_bit = hi_drv_gpio_write_bit,
    .pfn_gpio_get_num = hi_drv_gpio_get_gpio_num,
    .pfn_gpio_register_server_func = hi_drv_gpio_register_server_func,
    .pfn_gpio_unregister_server_func = hi_drv_gpio_unregister_server_func,
    .pfn_gpio_set_int_type = hi_drv_gpio_set_int_type,
    .pfn_gpio_set_int_enable = hi_drv_gpio_set_bit_int_enable,
    .pfn_gpio_clear_group_int = hi_drv_gpio_clear_group_int,
    .pfn_gpio_clear_bit_int = hi_drv_gpio_clear_bit_int,
    .pfn_gpio_get_usr_addr = hi_drv_gpio_get_usr_addr
};

static gpio_get_gpionum g_gpio_num;
static hi_u32 g_gpio_inttype[HI_GPIO_MAX_BIT_NUM] = {0};
static hi_s32 g_gpio_to_irq[HI_GPIO_MAX_BIT_NUM] = {0};
#ifndef CHIP_TYPE_hi3751v350
static hi_u8 *g_gpio_irq_name[] = {
    "hi_gpio_irq",
    "hi_stb_gpio_irq"
};
#endif
#define GPIO_OUTPUT        0
#define GPIO_INPUT         1

#ifndef HI_FPGA
#define GPIO_IRQ_STB_NO  (126 + 32)
#define GPIO_IRQ_NO      (108 + 32)
static gpio_irq g_gpio_irq_table[] = {
    { GPIO_IRQ_NO, "hi_gpio_irq" },
    { GPIO_IRQ_STB_NO, "hi_stb_gpio_irq" }
};
#endif

volatile hi_void *g_gpio_usr_addr[HI_GPIO_MAX_GROUP_NUM];
EXPORT_SYMBOL(g_gpio_usr_addr);

static HI_U8 g_gpio_grp_num;
static HI_U8 g_gpio_max_num;

hi_s32 gpio_isr(hi_s32 irq, hi_void *dev_id);
hi_s32 drv_gpio_isr(hi_s32 irq, hi_void *dev_id);

hi_void gpio_drv_get_gpio_ext_func(gpio_ext_func **st_gpio_export_funcs)
{
    *st_gpio_export_funcs = &g_st_gpio_export_funcs;
}
EXPORT_SYMBOL(gpio_drv_get_gpio_ext_func);

hi_u32 gpio_drv_get_chip_version(hi_void)
{
    HI_CHIP_VERSION_E chip_version = HI_CHIP_VERSION_BUTT;
    gpio_drv_chip chip = GPIO_DRV_CHIP_V350;
#if !defined(CFG_HI_USER_DRV)
    HI_CHIP_TYPE_E chip_type;
    hi_drv_sys_get_chip_version(&chip_type, &chip_version);
#else
    chip_version = HI_CHIP_VERSION_V350;
#endif

    if (chip_version == HI_CHIP_VERSION_BUTT) {
        chip = GPIO_DRV_CHIP_V350;
    } else {
        switch (chip_version) {
            case HI_CHIP_VERSION_V350:
            case HI_CHIP_VERSION_V351:
                chip = GPIO_DRV_CHIP_V350;
                break;
            default:
                break;
        }
    }

    HI_LOG_DBG("get chip version:0x%x\n", chip);
    return chip;
}

#ifndef CHIP_TYPE_hi3751v350
static hi_u32 gpio_get_vir_no(hi_u32 chip, hi_u32 gpio_grp, hi_u32 gpio_bit)
{
    hi_u32 vir_gpio_no;

    vir_gpio_no = gpio_grp * GPIO_ONE_GROUP_BIT_NUM + gpio_bit;

    if (chip == GPIO_DRV_CHIP_V350) {
        if (gpio_grp > 17) { /* 17 : unused gpio group */
            vir_gpio_no += GPIO_ONE_GROUP_BIT_NUM * 2; /* 2:unused gpio group num */
        } else if (gpio_grp < 16) { /* 16 : unused gpio group */
            vir_gpio_no += GPIO_ONE_GROUP_BIT_NUM * 8; /* 8:unused gpio group num */
        }
    }

    return vir_gpio_no;
}
#endif

static hi_bool drv_gpio_convert(hi_u32 gpio_no, hi_u32 *p_group_no, hi_u32 *p_bit_no)
{
    if (gpio_no >= g_gpio_max_num || p_group_no == HI_NULL || p_bit_no == HI_NULL) {
        HI_LOG_ERR("para gpio_no is invalid.\n");
        hi_err_print_u32(gpio_no);
        return HI_FALSE;
    }

    if (g_gpio_usr_addr[(gpio_no / HI_GPIO_BIT_NUM)] == NULL) {
        HI_LOG_ERR("para gpio_no is reserved.\n");
        hi_err_print_u32(gpio_no);
        return HI_FALSE;
    }
    *p_group_no = gpio_no / HI_GPIO_BIT_NUM;
    *p_bit_no = gpio_no % HI_GPIO_BIT_NUM;

    return HI_TRUE;
}

hi_s32 hi_drv_gpio_set_dir_bit(hi_u32 gpio_no, hi_u32 dir_bit)
{
    hi_u32 gpio_dirt_value;
    volatile hi_void *gpio_usr_addr = HI_NULL;
    hi_u32 group_no = 0;
    hi_u32 bit_no = 0;

    hi_dbg_func_enter();
    if (HI_FALSE == drv_gpio_convert(gpio_no, &group_no, &bit_no)) {
        HI_LOG_ERR("para gpio_no is invalid.GpioNo is %u\n", gpio_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (dir_bit != HI_TRUE && dir_bit != HI_FALSE) {
        HI_LOG_ERR("para dir_bit is invalid.\n");
        hi_err_print_u32(group_no);
        hi_err_print_u32(bit_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    gpio_usr_addr = g_gpio_usr_addr[group_no];
    gpio_dirt_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_DIR_REG);

    if (!dir_bit) { /* 0 output */
        gpio_dirt_value |= (HI_GPIO_OUTPUT << bit_no);
        REG_USR_ADDR(gpio_usr_addr + HI_GPIO_DIR_REG) = gpio_dirt_value;
    } else { /* 1 input */
        gpio_dirt_value &= (~(1 << bit_no));
        REG_USR_ADDR(gpio_usr_addr + HI_GPIO_DIR_REG) = gpio_dirt_value;
    }

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_get_dir_bit(hi_u32 gpio_no, hi_u32 *pdir_bit)
{
    hi_u32 group_no, bit_no;
    hi_u32 gpio_dirt_value;
    volatile hi_void *gpio_usr_addr = HI_NULL;

    hi_dbg_func_enter();

    if (drv_gpio_convert(gpio_no, &group_no, &bit_no) == HI_FALSE) {
        HI_LOG_ERR("para gpio_no is invalid. GpioNo is %u\n", gpio_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (pdir_bit == NULL) {
        HI_LOG_ERR("para pdir_bit is null.\n");
        hi_err_print_u32(group_no);
        hi_err_print_u32(bit_no);
        return HI_ERR_GPIO_NULL_PTR;
    }

    gpio_usr_addr = g_gpio_usr_addr[group_no];
    gpio_dirt_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_DIR_REG);
    gpio_dirt_value &= (1 << bit_no);
    if (gpio_dirt_value == 0) {
        *pdir_bit = HI_TRUE;
    } else {
        *pdir_bit = HI_FALSE;
    }

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_write_bit(hi_u32 gpio_no, hi_u32 bit_value)
{
    hi_u32 gpio_dirt_value;
    volatile hi_void *gpio_usr_addr = HI_NULL;
    hi_u32 group_no = 0;
    hi_u32 bit_no = 0;

    hi_dbg_func_enter();

    if (HI_FALSE == drv_gpio_convert(gpio_no, &group_no, &bit_no)) {
        HI_LOG_ERR("para gpio_no is invalid. GpioNo is %u\n", gpio_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if ((bit_value != HI_TRUE)
        && (bit_value != HI_FALSE)) {
        HI_LOG_ERR("para bit_value is invalid.\n");
        hi_err_print_u32(group_no);
        hi_err_print_u32(bit_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    gpio_usr_addr = g_gpio_usr_addr[group_no];
    gpio_dirt_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_DIR_REG);
    gpio_dirt_value &= (1 << bit_no);
    gpio_dirt_value = gpio_dirt_value >> bit_no;
    if (gpio_dirt_value != HI_GPIO_OUTPUT) {
        HI_LOG_ERR("GPIO Direction is input, write can not operate.\n");
        hi_err_print_u32(group_no);
        hi_err_print_u32(bit_no);
        return HI_ERR_GPIO_INVALID_OPT;
    }

    if (bit_value) {
        REG_USR_ADDR(gpio_usr_addr + (4 << bit_no)) = 1 << bit_no; // 4 :offsetr
    } else {
        REG_USR_ADDR(gpio_usr_addr + (4 << bit_no)) = 0; // 4 :offsetr
    }

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_read_bit(hi_u32 gpio_no, hi_u32 *bit_value)
{
    volatile hi_void *gpio_usr_addr = HI_NULL;
    hi_u32 group_no = 0;
    hi_u32 bit_no = 0;

    hi_dbg_func_enter();

    if (HI_FALSE == drv_gpio_convert(gpio_no, &group_no, &bit_no)) {
        HI_LOG_ERR("para gpio_no is invalid.\n");
        hi_err_print_u32(group_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (bit_value == NULL) {
        HI_LOG_ERR("para bit_value is null.\n");
        hi_err_print_u32(group_no);
        hi_err_print_u32(bit_no);
        return HI_ERR_GPIO_NULL_PTR;
    }

    gpio_usr_addr = g_gpio_usr_addr[group_no];
    *bit_value = REG_USR_ADDR(gpio_usr_addr +
        (4 << bit_no)) >> bit_no; // 4: 0ffset

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

/*
gpio_group: gpio group number
u32BitX:      gpio bit offset
en_int_tri_mode: interruput type ,please see gpio_inttpye
 */
static hi_void gpio_set_edge_trigger_type(hi_u32 bit_no,
    hi_u32 group_no, gpio_inttpye en_int_tri_mode)
{
    volatile hi_void *gpio_usr_addr = HI_NULL;
    hi_u32 gpio_value, gpio_no;

    gpio_no = group_no * 8 + bit_no; /* 8 : one group bit */
    gpio_usr_addr = g_gpio_usr_addr[group_no];
    gpio_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IS_REG);
    gpio_value &= (~(1 << bit_no));
    REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IS_REG) = gpio_value;
    gpio_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IBE_REG);

    if (en_int_tri_mode == GPIO_INTTYPE_UP) {
        gpio_value &= (~(1 << bit_no)); /* single edge trigger */
        REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IBE_REG) = gpio_value;

        gpio_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IEV_REG);
        gpio_value |= (1 << bit_no); /*  up edge */
        REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IEV_REG) = gpio_value;
        g_gpio_inttype[gpio_no] = INT_TYPE_EDGE_RISING;
    } else if (en_int_tri_mode == GPIO_INTTYPE_DOWN) {
        gpio_value &= (~(1 << bit_no)); /*  single edge trigger */
        REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IBE_REG) = gpio_value;

        gpio_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IEV_REG);
        gpio_value &= (~(1 << bit_no)); /* down edge */
        REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IEV_REG) = gpio_value;
        g_gpio_inttype[gpio_no] = INT_TYPE_EDGE_FALLING;
    } else {
        gpio_value |= (1 << bit_no); /* double edge */
        REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IBE_REG) = gpio_value;
        g_gpio_inttype[gpio_no] = INT_TYPE_EDGE_BOTH;
    }
    return;
}

static hi_void gpio_set_level_trigger_type(hi_u32 bit_no,
    hi_u32 group_no, gpio_inttpye en_int_tri_mode)
{
    volatile hi_void *gpio_usr_addr = HI_NULL;
    hi_u32 gpio_value, gpio_no;

    gpio_no = group_no * 8 + bit_no; /* 8 : one group bit */
    gpio_usr_addr = g_gpio_usr_addr[group_no];
    gpio_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IS_REG);
    gpio_value |= (1 << bit_no);
    REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IS_REG) = gpio_value;

    if (en_int_tri_mode == GPIO_INTTYPE_HIGH) {
        gpio_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IEV_REG);
        gpio_value |= (1 << bit_no); /*  high level trigger */
        REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IEV_REG) = gpio_value;
        /* level trigger type must use IRQF_ONESHOT interrupt is not re-enabled after the hardirq handler finished */
        g_gpio_inttype[gpio_no] = INT_TYPE_LEVEL_HIGH | GPIO_IRQF_ONESHOT;
    } else {
        gpio_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IEV_REG);
        gpio_value &= (~(1 << bit_no)); /* low level trigger */
        REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IEV_REG) = gpio_value;
        /* level trigger type must use IRQF_ONESHOT interrupt is not re-enabled after the hardirq handler finished */
        g_gpio_inttype[gpio_no] = INT_TYPE_LEVEL_HIGH | GPIO_IRQF_ONESHOT;
    }
    return;
}

hi_s32 hi_drv_gpio_set_int_type(hi_u32 gpio_no, gpio_inttpye en_int_tri_mode)
{
    hi_u32 group_no = 0;
    hi_u32 bit_no = 0;

    hi_dbg_func_enter();

    if (HI_FALSE == drv_gpio_convert(gpio_no, &group_no, &bit_no)) {
        HI_LOG_ERR("para gpio_no is invalid. GpioNo is %u\n", gpio_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (en_int_tri_mode >= GPIO_INTTYPE_BUTT) {
        HI_LOG_ERR("para en_int_tri_mode is invalid. GpioNo is %u\n", gpio_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    /* edge trigger */
    if (en_int_tri_mode <= GPIO_INTTYPE_UPDOWN) {
        gpio_set_edge_trigger_type(bit_no, group_no, en_int_tri_mode);
    } else if (en_int_tri_mode > GPIO_INTTYPE_UPDOWN) { /* level trigger */
        gpio_set_level_trigger_type(bit_no, group_no, en_int_tri_mode);
    }

    hi_dbg_print_u32(group_no);
    hi_dbg_print_u32(bit_no);
    hi_dbg_print_u32(en_int_tri_mode);
    hi_dbg_func_exit();

    return HI_SUCCESS;
}

static hi_void gpio_set_irq_ctrl_reg(hi_u32 group_no, hi_u32 bit_no, hi_bool enable)
{
    hi_u32 gpio_value, gpio_no;
    volatile hi_void *gpio_usr_addr = HI_NULL;

    gpio_no = group_no * GPIO_ONE_GROUP_BIT_NUM + bit_no;
    gpio_usr_addr = g_gpio_usr_addr[group_no];
    gpio_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IE_REG);

    if (enable) { /* not marsk  */
        gpio_value |= (1 << bit_no);
        g_st_gpio_attr.gpio_int_attr[gpio_no].gpio_int_enable = HI_TRUE;
    } else { /*  marsk  */
        gpio_value &= (~(1 << bit_no));
        g_st_gpio_attr.gpio_int_attr[gpio_no].gpio_int_enable = HI_FALSE;
    }
    REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IE_REG) = gpio_value;
    return;
}

static hi_void gpio_set_bit_irq_disable(hi_u32 chip, hi_u32 gpio_no, hi_u32 group_no, hi_u32 bit_no)
{
    hi_u32 index;

    index = (group_no <= 15) ? (0) : (1); /* hi_gpio_irq: group 0 ~ 15; hi_stb_gpio_irq: group 18 ~ 22 */
#ifdef ENABLE_GPIO_IRQ_FREE
    osal_irq_free(g_gpio_irq_table[index].irq_no, NULL);
#endif
    g_st_gpio_attr.gpio_int_attr[gpio_no].gpio_int_enable = HI_FALSE;

    return;
}

static hi_s32 gpio_set_bit_irq_enable(hi_u32 chip, hi_u32 gpio_no, hi_u32 group_no, hi_u32 bit_no)
{
    hi_s32 ret, index;

    index = (group_no <= 15) ? (0) : (1); /* hi_gpio_irq: group 0 ~ 15; hi_stb_gpio_irq: group 18 ~ 22 */
    ret = osal_irq_request(g_gpio_irq_table[index].irq_no, drv_gpio_isr, 0,
                           g_gpio_irq_table[index].name, HI_NULL);
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO("register GPIO INT failed return 0x%x.\n", ret);
        return HI_FAILURE;
    }
    g_st_gpio_attr.gpio_int_attr[gpio_no].gpio_int_enable = HI_TRUE;

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_set_bit_int_enable(hi_u32 gpio_no, hi_bool b_enable)
{
    hi_u32 chip;
    hi_u32 group_no = 0;
    hi_u32 bit_no = 0;
    hi_s32 ret;

    hi_dbg_func_enter();

    if (drv_gpio_convert(gpio_no, &group_no, &bit_no) == HI_FALSE) {
        HI_LOG_ERR("para gpio_no is invalid\n");
        hi_err_print_u32(group_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if ((b_enable != HI_TRUE) && (b_enable != HI_FALSE)) {
        HI_LOG_ERR("para b_enable is invalid. \n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    chip = gpio_drv_get_chip_version();
    gpio_set_irq_ctrl_reg(group_no, bit_no, b_enable);

    if (b_enable == HI_FALSE ||
        ((g_gpio_inttype[gpio_no] == INT_TYPE_LEVEL_HIGH || g_gpio_inttype[gpio_no] == INT_TYPE_LEVEL_LOW)
        && g_st_gpio_attr.gpio_int_attr[gpio_no].gpio_int_enable == HI_TRUE)) {
        gpio_set_bit_irq_disable(chip, gpio_no, group_no, bit_no);
        return HI_SUCCESS;
    }

    ret = gpio_set_bit_irq_enable(chip, gpio_no, group_no, bit_no);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    HI_LOG_DBG("group_no = %u, group_no = %u, b_enable = %u\n", group_no, bit_no, b_enable);
    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_set_group_int_enable(hi_u32 group_no, hi_bool b_enable)
{
    hi_u32 gpio_value, chip;
    volatile hi_void *gpio_usr_addr = HI_NULL;

    hi_dbg_func_enter();

    if (group_no >= g_gpio_num.gpio_grp_num || g_gpio_usr_addr[group_no] == HI_NULL) {
        HI_LOG_ERR("invalid param. GroupNo is %u.\n", group_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if ((b_enable != HI_TRUE) && (b_enable != HI_FALSE)) {
        HI_LOG_ERR("para b_enable is invalid. \n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    chip = gpio_drv_get_chip_version();
    gpio_usr_addr = g_gpio_usr_addr[group_no];

    gpio_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IE_REG);

    if (b_enable) { /* enable one group */
        gpio_value |= 0xff;
    } else { /* disable one group */
        gpio_value = 0;
    }
    REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IE_REG) = gpio_value;
    HI_LOG_INFO("gpio_group = %d b_enable = %d \n", group_no, b_enable);

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_set_all_int_enable(hi_bool b_enable)
{
    hi_s32 ret = HI_SUCCESS;
    hi_u32 i = 0;

    hi_dbg_func_enter();

    if ((b_enable != HI_TRUE) && (b_enable != HI_FALSE)) {
        HI_LOG_ERR("para b_enable is invalid. \n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    for (i = 0; i < g_gpio_num.gpio_grp_num; i++) {
        if (g_gpio_usr_addr[i] == NULL) {
            continue;
        }

        ret = hi_drv_gpio_set_group_int_enable(i, b_enable);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERR("GpioGrpNum %d is invalid.\n", i);
            return HI_FAILURE;
        }
    }

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

static hi_s32 osal_wait_condition (const hi_void *param)
{
    hi_s32 ret = 0;
    if (g_st_gpio_attr.head != g_st_gpio_attr.tail) {
        ret = 1;
        return ret;
    }
    return ret;
}

hi_s32 drv_gpio_query_int(gpio_int *p_gpio_int_value)
{
    hi_s32 ret = 0;
    hi_u32 gpio_int_num = 0;

    if (p_gpio_int_value == NULL) {
        HI_LOG_ERR("param null ptr.\n");
        return HI_FAILURE;
    }

    g_st_gpio_attr.gpio_block_time = p_gpio_int_value->timeout_ms;

    while ((g_st_gpio_attr.head) == (g_st_gpio_attr.tail)) {
        if (g_st_gpio_attr.gpio_block_time == 0xffffffff) {
            ret = osal_wait_interruptible(&g_st_gpio_attr.gpio_int_wait_queue, osal_wait_condition, NULL);
            if (ret < 0) {
                HI_LOG_INFO("wait data err.\n");
                return -ERESTARTSYS;
            }
        } else {
            ret = osal_wait_timeout_interruptible(&g_st_gpio_attr.gpio_int_wait_queue,
                osal_wait_condition, NULL, g_st_gpio_attr.gpio_block_time);
            if (ret < 0) {
                HI_LOG_INFO("wait data err.\n");
                return -ERESTARTSYS;
            } else if (ret == 0) {
                HI_LOG_INFO("wait data timeout.\n");
                return HI_ERR_GPIO_GETINT_TIMEOUT;
            }
        }
    }

    if ((g_st_gpio_attr.head) != (g_st_gpio_attr.tail)) {
        gpio_int_num = GPIO_BUF_TAIL;
        g_st_gpio_attr.tail = inc_buf(g_st_gpio_attr.tail, g_st_gpio_attr.gpio_buf_len);
        p_gpio_int_value->gpio_no = gpio_int_num;
    }

    return HI_SUCCESS;
}

hi_s32 drv_gpio_open(hi_void *private_data)
{
    g_st_gpio_attr.head = 0;
    g_st_gpio_attr.tail = 0;
    return HI_SUCCESS;
}

hi_s32 drv_gpio_close(hi_void *private_data)
{
    g_st_gpio_attr.head = 0;
    g_st_gpio_attr.tail = 0;
    return HI_SUCCESS;
}

hi_s32 drv_gpio_set_output_type(hi_u32 gpio_no, gpio_outputtype en_output_type)
{
    if (gpio_no >= g_gpio_max_num) {
        HI_LOG_ERR("para gpio_no is invalid.GpioNo is %u\n", gpio_no);
        return HI_FALSE;
    }

    if (en_output_type >= GPIO_OUTPUTTYPE_BUTT) {
        HI_LOG_ERR("invalid param.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 drv_gpio_get_output_type(hi_u32 gpio_no, gpio_outputtype *en_output_type)
{
    if (gpio_no >= g_gpio_max_num) {
        HI_LOG_ERR("para gpio_no is invalid.GpioNo is %u\n", gpio_no);
        return HI_FALSE;
    }

    if (en_output_type == NULL) {
        HI_LOG_ERR("param null ptr.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_register_server_func(hi_u32 gpio_no, hi_void (*func)(hi_u32))
{
    hi_dbg_func_enter();

    if (gpio_no >= g_gpio_num.gpio_max_num) {
        HI_LOG_INFO(" para gpio number out of range , int_value = %d\n", gpio_no);
        return HI_FAILURE;
    }

    if (g_gpio_usr_addr[gpio_no / HI_GPIO_BIT_NUM] == HI_NULL) {
        HI_LOG_ERR("para gpio_no is invalid.GpioNo is %u\n", gpio_no);
        return HI_FALSE;
    }

    if (HI_NULL == func) {
        HI_LOG_INFO(" register func para is null, int_value = %d\n", gpio_no);
        return HI_FAILURE;
    }

    if (g_st_gpio_attr.gpio_int_attr[gpio_no].gpio_server != HI_NULL) {
        HI_LOG_INFO(" GPIO %d had registered gpio server pragram \n", gpio_no);
        return HI_FAILURE;
    }

    g_st_gpio_attr.gpio_int_attr[gpio_no].gpio_server = func;
    HI_LOG_INFO("gpio %d finished register gpio server function \n", gpio_no);

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_unregister_server_func(hi_u32 gpio_no)
{
    hi_dbg_func_enter();

    if (gpio_no >= g_gpio_num.gpio_max_num) {
        HI_LOG_INFO(" para gpio number out of range , int_value = %u\n", gpio_no);
        return HI_FAILURE;
    }

    if (g_gpio_usr_addr[gpio_no / HI_GPIO_BIT_NUM] == HI_NULL) {
        HI_LOG_ERR("para gpio_no is invalid.GpioNo is %u\n", gpio_no);
        return HI_FALSE;
    }

    g_st_gpio_attr.gpio_int_attr[gpio_no].gpio_server = HI_NULL;

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_clear_bit_int(hi_u32 gpio_no)
{
    volatile hi_void *gpio_usr_addr = NULL;
    hi_u32 reg_val;
    hi_u32 group_no = 0;
    hi_u32 bit_no = 0;

    hi_dbg_func_enter();

    if (HI_FALSE == drv_gpio_convert(gpio_no, &group_no, &bit_no)) {
        HI_LOG_ERR("para gpio_no is invalid. GpioNo is %u\n", gpio_no);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    gpio_usr_addr = g_gpio_usr_addr[group_no];
    reg_val = REG_USR_ADDR((gpio_usr_addr + HI_GPIO_IC_REG));
    reg_val |= (0x1 << bit_no);

    REG_USR_ADDR((gpio_usr_addr + HI_GPIO_IC_REG)) = reg_val;

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_get_usr_addr(hi_u32 group_no, hi_void **pv_vir_addr)
{
    hi_dbg_func_enter();

    if (group_no >= g_gpio_grp_num || g_gpio_usr_addr[group_no] == HI_NULL) {
        HI_LOG_ERR("para group_no is invalid.group_no is %u\n", group_no);
        return HI_FALSE;
    }

    if (pv_vir_addr == NULL) {
        HI_LOG_ERR("param null ptr.\n");
        return HI_FAILURE;
    }

    *(volatile hi_void **)pv_vir_addr = g_gpio_usr_addr[group_no];

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_s32 hi_drv_gpio_clear_group_int(hi_u32 gpio_group)
{
    volatile hi_void *gpio_usr_addr = HI_NULL;

    hi_dbg_func_enter();

    if (gpio_group >= g_gpio_num.gpio_grp_num || g_gpio_usr_addr[gpio_group] == NULL) {
        HI_LOG_ERR("invalid parameter(%u).\n", gpio_group);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    gpio_usr_addr = g_gpio_usr_addr[gpio_group];
    REG_USR_ADDR((gpio_usr_addr + HI_GPIO_IC_REG)) = 0xff;

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_s32 gpio_clear_all_int(hi_void)
{
    hi_s32 ret = HI_SUCCESS;
    hi_u32 i = 0;

    for (i = 0; i < g_gpio_num.gpio_grp_num; i++) {
        if (g_gpio_usr_addr[i] == NULL) {
            continue;
        }

        ret = hi_drv_gpio_clear_group_int(i);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERR("GpioGrpNum %u is invalid.\n", i);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

/* add this for 350 */
hi_void gpio_isr_response(hi_u32 gpio_num)
{
    if (g_st_gpio_attr.gpio_int_attr[gpio_num].gpio_int_enable == HI_TRUE) {
        if (g_st_gpio_attr.gpio_int_attr[gpio_num].gpio_server != HI_NULL) {
            g_st_gpio_attr.gpio_int_attr[gpio_num].gpio_server(gpio_num);
            HI_LOG_INFO(" ----- execute gpio[%d].gpio_server \n", gpio_num);
        }
        GPIO_BUF_HEAD = gpio_num;
        g_st_gpio_attr.head = inc_buf(g_st_gpio_attr.head, g_st_gpio_attr.gpio_buf_len);
        osal_wait_wakeup(&(g_st_gpio_attr.gpio_int_wait_queue));
    }

    return;
}

hi_s32 drv_gpio_isr(hi_s32 irq, hi_void *dev_id)
{
    hi_u32 int_value, gpio_value, gpio_num;
    hi_u32 gpio_group = 0;
    hi_u32 i = 0;
    volatile hi_void *gpio_usr_addr = HI_NULL;
    // Traverse all groups, a total of 22 groups, 10-17 groups, 21, 22 groups are invalid
    for (gpio_group = 0; gpio_group <= 22; ++gpio_group) {
        if (gpio_group == GPIO_ERROR_MIN_NO || gpio_group == GPIO_ERROR_MAX_NO ||
            (gpio_group >= HI_GPIO_10_VIR_ADDR && gpio_group <=  HI_GPIO_15_VIR_ADDR)
            || gpio_group > HI_GPIO_20_VIR_ADDR) {
            continue;
        }

        gpio_usr_addr = g_gpio_usr_addr[gpio_group];
        gpio_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IE_REG);
        if (gpio_value == 0) {
            continue;
        }

        gpio_usr_addr = g_gpio_usr_addr[gpio_group];
        int_value = REG_USR_ADDR(gpio_usr_addr + HI_GPIO_MIS_REG);

        for (i = 0; i < 8; i++) { // Each group has 8 io ports
            if ((int_value & (1 << i)) == 0) { /* check each bit */
                continue;
            }
            gpio_num = gpio_group * 8 + i; // A group has 8 io, so the calculation can get the io number
            gpio_isr_response(gpio_num);
            REG_USR_ADDR((gpio_usr_addr + HI_GPIO_IC_REG)) = (0X1 << i); /* clear one bit interruput flag */
            /* trigger type is high/low level, disable interrupt */
            if (g_gpio_inttype[gpio_num] == (INT_TYPE_LEVEL_HIGH | GPIO_IRQF_ONESHOT)
                || g_gpio_inttype[gpio_num] == (INT_TYPE_LEVEL_LOW | GPIO_IRQF_ONESHOT)) {
                gpio_value &= (~(1 << i));
                g_st_gpio_attr.gpio_int_attr[gpio_num].gpio_int_enable = HI_FALSE;
                REG_USR_ADDR(gpio_usr_addr + HI_GPIO_IE_REG) = gpio_value;
            }
            break;
        }
    }

    return OSAL_IRQ_HANDLED;
}

hi_s32 gpio_isr(hi_s32 irq, hi_void *dev_id)
{
    hi_u32 gpio_num = 0;
    (void)dev_id;
    for (gpio_num = 0; gpio_num < HI_GPIO_MAX_BIT_NUM; ++gpio_num) {
        if (g_gpio_to_irq[gpio_num] == irq) {
            gpio_isr_response(gpio_num);
            break;
        }
    }

    return OSAL_IRQ_HANDLED;
}

hi_s32 hi_drv_gpio_get_gpio_num(gpio_get_gpionum *gpio_num)
{
    hi_dbg_func_enter();

    if (gpio_num == HI_NULL) {
        HI_LOG_ERR("param null ptr.\n");
        return HI_ERR_GPIO_NULL_PTR;
    }

    gpio_num->gpio_grp_num = g_gpio_grp_num;
    gpio_num->gpio_max_num = g_gpio_max_num;

    hi_dbg_func_exit();

    return HI_SUCCESS;
}

static hi_s32 gpio_usr_addr_remap(hi_u32 chip)
{
    hi_s32 i = 0;

    /* remap gpio reg address */
    for (i = 0; i < HI_GPIO_MAX_GROUP_NUM; i++) {
        g_gpio_usr_addr[i] = NULL;
    }

    HI_GPIO_REG_REMAP(g_gpio_usr_addr[0], HI_GPIO_0_ADDR); /* gpio 0 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[1], HI_GPIO_1_ADDR); /* gpio 1 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[2], HI_GPIO_2_ADDR); /* gpio 2 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[3], HI_GPIO_3_ADDR); /* gpio 3 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[4], HI_GPIO_4_ADDR); /* gpio 4 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[5], HI_GPIO_5_ADDR); /* gpio 5 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[6], HI_GPIO_6_ADDR); /* gpio 6 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[7], HI_GPIO_7_ADDR); /* gpio 7 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[8], HI_GPIO_8_ADDR); /* gpio 8 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[9], HI_GPIO_9_ADDR); /* gpio 9 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[18], HI_GPIO_18_ADDR); /* gpio 18 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[19], HI_GPIO_19_ADDR); /* gpio 19 base addr */
    HI_GPIO_REG_REMAP(g_gpio_usr_addr[20], HI_GPIO_20_ADDR); /* gpio 20 base addr */

    return HI_SUCCESS;
}

static hi_void gpio_set_gpio_attr(hi_void)
{
    hi_s32 ret;
    hi_s32 i = 0;
    hi_u32 gpio_grp;

    g_st_gpio_attr.head = 0;
    g_st_gpio_attr.tail = 0;
    g_st_gpio_attr.gpio_buf_len = GPIO_MAX_BUF;
    g_st_gpio_attr.gpio_block_time = 0xffffffff;

    ret = hi_drv_gpio_get_gpio_num(&g_gpio_num);
    if (ret != HI_SUCCESS) {
        return;
    }

    for (i = 0; i < g_gpio_num.gpio_max_num; i++) {
        g_gpio_to_irq[i] = -1;
        gpio_grp = i / HI_GPIO_BIT_NUM;
        if (gpio_grp >= HI_GPIO_MAX_GROUP_NUM) {
            HI_LOG_ERR("param invalid.\n");
            return;
        }
        if (g_gpio_usr_addr[gpio_grp] == NULL) {
            continue;
        }

        g_st_gpio_attr.gpio_int_attr[i].gpio_int_enable = HI_FALSE;
        g_st_gpio_attr.gpio_int_attr[i].gpio_int_type = GPIO_INTTYPE_DOWN;
        g_st_gpio_attr.gpio_int_attr[i].gpio_server = HI_NULL;
    }

    return;
}

hi_void hi_drv_gpio_set_irq_ctrl(hi_u32 gpio_no, hi_bool b_enable)
{
    hi_u32 group_no = 0;
    hi_u32 bit_no = 0;

    if (drv_gpio_convert(gpio_no, &group_no, &bit_no) == HI_FALSE) {
        HI_LOG_ERR("para gpio_no is invalid\n");
        hi_err_print_u32(group_no);
        return;
    }

    if ((b_enable != HI_TRUE) && (b_enable != HI_FALSE)) {
        HI_LOG_ERR("para b_enable is invalid. \n");
        return;
    }

    gpio_set_irq_ctrl_reg(group_no, bit_no, b_enable);
}

hi_s32 hi_drv_gpio_init(hi_void)
{
    hi_s32 ret;
    hi_u32 i, chip;

    hi_dbg_func_enter();

    i = 0; /* don't delete it */

    ret = osal_exportfunc_register(HI_ID_GPIO, "HI_GPIO", (hi_void *)&g_st_gpio_export_funcs);
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO(" GPIO Module register failed 0x%x.\n", ret);
        return HI_FAILURE;
    }

    chip = gpio_drv_get_chip_version();

    g_gpio_grp_num = HI_GPIO_GROUP_NUM;
    g_gpio_max_num = g_gpio_grp_num * HI_GPIO_BIT_NUM;

    ret = gpio_usr_addr_remap(chip);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    gpio_set_gpio_attr();

    osal_wait_init(&g_st_gpio_attr.gpio_int_wait_queue);
#ifdef __KERNEL__
    ret = hi_drv_gpio_set_all_int_enable(HI_FALSE);
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO(" GPIO disable INT failed 0x%x.\n", ret);
        return HI_FAILURE;
    }

    ret = gpio_clear_all_int();
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO(" GPIO clear all INT failed 0x%x.\n", ret);
        return HI_FAILURE;
    }
#endif
    hi_dbg_func_exit();

    return HI_SUCCESS;
}

hi_void hi_drv_gpio_de_init(hi_void)
{
    hi_u32 i, chip, gpio_grp;
    hi_s32 ret;

    hi_dbg_func_enter();

    chip = gpio_drv_get_chip_version();

    ret = hi_drv_gpio_set_all_int_enable(HI_FALSE);  // disable all group interruput
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO(" GPIO disable INT failed 0x%x.\n", ret);
    }

    for (i = 0; i < g_gpio_num.gpio_max_num; i++) {
        gpio_grp = i / HI_GPIO_BIT_NUM;
        if (gpio_grp >= HI_GPIO_MAX_GROUP_NUM) {
            HI_LOG_ERR("param invalid.\n");
            return;
        }
        if (g_gpio_usr_addr[gpio_grp] == NULL) {
            continue;
        }

        g_st_gpio_attr.gpio_int_attr[i].gpio_int_enable = HI_FALSE;
        g_st_gpio_attr.gpio_int_attr[i].gpio_int_type = GPIO_INTTYPE_DOWN;
        g_st_gpio_attr.gpio_int_attr[i].gpio_server = HI_NULL;
    }

    ret = osal_exportfunc_unregister(HI_ID_GPIO);
    if (ret != HI_SUCCESS) {
        HI_LOG_INFO(" GPIO Module unregister failed 0x%x.\n", ret);
    }

    /* unmap gpio reg address */
    for (i = 0; i < HI_GPIO_MAX_GROUP_NUM; i++) {
        if (g_gpio_usr_addr[i] == NULL) {
            continue;
        }

        HI_GPIO_REG_UNMAP((hi_void *)g_gpio_usr_addr[i]);
        g_gpio_usr_addr[i] = NULL;
    }

    hi_dbg_func_exit();

    return;
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");
#ifdef MODULE
EXPORT_SYMBOL(hi_drv_gpio_init);
EXPORT_SYMBOL(hi_drv_gpio_de_init);
#endif

EXPORT_SYMBOL(hi_drv_gpio_get_dir_bit);
EXPORT_SYMBOL(hi_drv_gpio_set_dir_bit);
EXPORT_SYMBOL(hi_drv_gpio_write_bit);
EXPORT_SYMBOL(hi_drv_gpio_read_bit);
EXPORT_SYMBOL(hi_drv_gpio_get_gpio_num);
EXPORT_SYMBOL(hi_drv_gpio_register_server_func);
EXPORT_SYMBOL(hi_drv_gpio_unregister_server_func);
EXPORT_SYMBOL(hi_drv_gpio_set_int_type);
EXPORT_SYMBOL(hi_drv_gpio_set_bit_int_enable);
EXPORT_SYMBOL(hi_drv_gpio_clear_group_int);
EXPORT_SYMBOL(hi_drv_gpio_clear_bit_int);
EXPORT_SYMBOL(hi_drv_gpio_set_irq_ctrl);
