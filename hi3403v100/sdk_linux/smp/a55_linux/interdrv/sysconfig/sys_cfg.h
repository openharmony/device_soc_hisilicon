/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
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
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef SYS_CFG_H
#define SYS_CFG_H

#include <linux/printk.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define sys_config_print(format, args...) \
    printk("%s,%d: "format, __FUNCTION__, __LINE__, ##args) \

#define sys_writel(addr, value) ((*((volatile unsigned int *)(addr))) = (value))
#define sys_read(addr) (*((volatile int *)(addr)))
void write_reg32(unsigned long addr, unsigned long value, unsigned long mask);

void *sys_config_get_reg_crg(void);
void *sys_config_get_reg_sys(void);
void *sys_config_get_reg_ddr(void);
void *sys_config_get_reg_misc(void);
void *sys_config_get_reg_iocfg(void);
void *sys_config_get_reg_iocfg2(void);
void *sys_config_get_reg_gpio(void);
void *sys_config_get_reg_mipi_tx(void);

#define CHIP_SS928V100 0x0
#define VO_INTF_NAME_MIPI_TX "mipi_tx"

int sys_config_get_main_board_type(void);
int sys_config_get_chip_type(void);
int sys_config_get_vi_vpss_mode(void);
int sys_config_get_vo_intf_type(void);
int sys_config_get_vi_intf_type(void);
int sys_config_get_hdmi_en(void);
int sys_config_get_i2c_en(void);
int sys_config_get_audio_en(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SYS_CFG_H */
