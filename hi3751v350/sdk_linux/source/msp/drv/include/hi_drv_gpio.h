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

#ifndef __HI_DRV_GPIO_H__
#define __HI_DRV_GPIO_H__

#include "hi_type.h"
#include "drv_gpio_struct.h"
#include "hi_osal.h"

#define HI_GPIO_GROUP_NUM 23

#define GPIO_ERROR_MIN_NO  16
#define GPIO_ERROR_MAX_NO  17

#define HI_GPIO_BIT_NUM     8
#define HI_GPIO_MAX_BIT_NUM (HI_GPIO_GROUP_NUM * HI_GPIO_BIT_NUM)

#define HI_GPIO_SPACE_SIZE 0x1000

#define HI_GPIO_REG_REMAP(pvAddr, u32RegAddr)                            \
    do {                                                                 \
        (pvAddr) = osal_ioremap_nocache((u32RegAddr), HI_GPIO_SPACE_SIZE);        \
        if (NULL == (pvAddr)) {                                            \
            HI_LOG_ERR("ioremap REG_ADDR[0x%x] failure!\n", (u32RegAddr)); \
            return HI_FAILURE;                                           \
        }                                                                \
    } while (0)

#ifdef __KERNEL__
#define HI_GPIO_REG_UNMAP(pvAddr) osal_iounmap(pvAddr)
#else
#define HI_GPIO_REG_UNMAP(pvAddr) osal_iounmap(pvAddr, HI_GPIO_SPACE_SIZE)
#endif

#define HI_GPIO_MAX_GROUP_NUM 23
#define HI_gpio_irqTB0_NO   (126 + 32)
#define HI_GPIO_IRQ_NO        (108 + 32)

#define HI_GPIO_0_ADDR  0xF8B20000
#define HI_GPIO_1_ADDR  0xF8B21000
#define HI_GPIO_2_ADDR  0xF8B22000
#define HI_GPIO_3_ADDR  0xF8B23000
#define HI_GPIO_4_ADDR  0xF8B24000
#define HI_GPIO_5_ADDR  0xF8B25000
#define HI_GPIO_6_ADDR  0xF8B26000
#define HI_GPIO_7_ADDR  0xF8B27000
#define HI_GPIO_8_ADDR  0xF8B28000
#define HI_GPIO_9_ADDR  0xF8B29000
/* V350 :GPIO_10-GPIO_15 is reserved */
#define HI_GPIO_10_ADDR 0xF8B2A000
#define HI_GPIO_11_ADDR 0xF8B2B000
#define HI_GPIO_12_ADDR 0xF8B2C000
#define HI_GPIO_13_ADDR 0xF8B2D000
#define HI_GPIO_14_ADDR 0xF8B2E000
#define HI_GPIO_15_ADDR 0xF8B2F000

#define HI_GPIO_18_ADDR 0xF8004000 /* STB_GPIO_PWM18 */
#define HI_GPIO_19_ADDR 0xF8009000 /* STB_GPIO_PWM19 */
#define HI_GPIO_20_ADDR 0xF800B000 /* STB_GPIO_PWM20 */
/* V350 :GPIO_21、GPIO_22 is reserved */
#define HI_GPIO_21_ADDR 0xF800C000 /* STB_GPIO_PWM21 */
#define HI_GPIO_22_ADDR 0xF8036000 /* STB_GPIO_PWM22 */

#define HI_GPIO_DIR_REG 0x400
#define HI_GPIO_IS_REG  0x404
#define HI_GPIO_IBE_REG 0x408
#define HI_GPIO_IEV_REG 0x40c
#define HI_GPIO_IE_REG  0x410
#define HI_GPIO_RIS_REG 0x414
#define HI_GPIO_MIS_REG 0x418
#define HI_GPIO_IC_REG  0x41c

#define HI_GPIO_OUTPUT        1
#define HI_GPIO_INPUT         0
#define REG_USR_ADDR(RegAddr) *((volatile hi_u32*)(RegAddr))

typedef struct {
    hi_u32 irq_no;
    hi_char name[16]; // 存放名称的字符最大长度为16
} gpio_irq;

hi_s32 hi_drv_gpio_init(hi_void);
hi_void hi_drv_gpio_de_init(hi_void);

hi_s32 hi_drv_gpio_set_dir_bit(hi_u32 gpio_no, hi_u32 dir_bit);
hi_s32 hi_drv_gpio_get_dir_bit(hi_u32 gpio_no, hi_u32 *p_dir_bit);
hi_s32 hi_drv_gpio_write_bit(hi_u32 gpio_no, hi_u32 bit_value);
hi_s32 hi_drv_gpio_read_bit(hi_u32 gpio_no, hi_u32 *p_bit_value);
hi_s32 hi_drv_gpio_get_gpio_num(gpio_get_gpionum *gpio_num);

hi_s32 hi_drv_gpio_set_int_type(hi_u32 gpio_no, gpio_inttpye en_int_tri_mode);
hi_s32 hi_drv_gpio_set_bit_int_enable(hi_u32 gpio_no, HI_BOOL b_enable);
hi_s32 hi_drv_gpio_register_server_func(hi_u32 gpio_no, hi_void(*func)(hi_u32));
hi_s32 hi_drv_gpio_unregister_server_func(hi_u32 gpio_no);
hi_s32 hi_drv_gpio_clear_group_int(hi_u32 group_no);
hi_s32 hi_drv_gpio_clear_bit_int(hi_u32 gpio_no);
hi_s32 hi_drv_gpio_get_usr_addr(hi_u32 group_no, hi_void **pv_vir_addr);

typedef hi_s32 (*fn_gpio_get_bit)(hi_u32, hi_u32 *);
typedef hi_s32 (*fn_gpio_set_bit)(hi_u32, hi_u32);
typedef hi_s32 (*fn_gpio_get_num)(gpio_get_gpionum *);

typedef hi_s32 (*fn_gpio_register_server_func)(hi_u32, hi_void (*func)(hi_u32));
typedef hi_s32 (*fn_gpio_unregister_server_func)(hi_u32);
typedef hi_s32 (*fn_gpio_set_int_type)(hi_u32, gpio_inttpye);
typedef hi_s32 (*fn_gpio_set_int_enable)(hi_u32, hi_bool);
typedef hi_s32 (*fn_gpio_clear_group_int)(hi_u32);
typedef hi_s32 (*fn_gpio_clear_bit_int)(hi_u32);
typedef hi_s32 (*fn_gpio_get_usr_addr)(hi_u32, hi_void **);

typedef struct {
    fn_gpio_get_bit pfn_gpio_dir_get_bit;
    fn_gpio_set_bit pfn_gpio_dir_set_bit;
    fn_gpio_get_bit pfn_gpio_read_bit;
    fn_gpio_set_bit pfn_gpio_write_bit;
    fn_gpio_get_num pfn_gpio_get_num;
    fn_gpio_register_server_func pfn_gpio_register_server_func;
    fn_gpio_unregister_server_func pfn_gpio_unregister_server_func;
    fn_gpio_set_int_type pfn_gpio_set_int_type;
    fn_gpio_set_int_enable pfn_gpio_set_int_enable;
    fn_gpio_clear_group_int pfn_gpio_clear_group_int;
    fn_gpio_clear_bit_int pfn_gpio_clear_bit_int;
    fn_gpio_get_usr_addr pfn_gpio_get_usr_addr;
} gpio_ext_func;
hi_void gpio_drv_get_gpio_ext_func(gpio_ext_func **st_gpio_export_funcs);

#endif /* End of #ifndef __HI_DRV_GPIO_H__ */
