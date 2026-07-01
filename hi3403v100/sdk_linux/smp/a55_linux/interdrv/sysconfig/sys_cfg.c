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

#include "sys_cfg.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include <asm/io.h>
#include "sys_ctrl.h"
#include "clk_cfg.h"
#include "pin_mux.h"
#include "securec.h"

#define SENSOR_LIST_CMDLINE_LEN 256
#define SENSOR_NAME_LEN 64
#define SENSOR_NAME_MAX 8
#define SENSOR_NUM 4
#define NAME_LEN 16
#define VO_INTF_NAME_LEN 16

#define SENSOR_NAME_HY_S0603 "hy_s0603"
#define SENSOR_NAME_OS08A20 "os08a20"
#define SENSOR_NAME_SC450AI "sc450ai"
#define MIPI_AD "mipi_ad"

static void *g_reg_crg_base = 0;
static void *g_reg_sys_base = 0;
static void *g_reg_ddr_base = 0;
static void *g_reg_misc_base = 0;
static void *g_reg_iocfg_base = 0;
static void *g_reg_iocfg2_base = 0;
static void *g_reg_gpio_base = 0;
static void *g_reg_mipi_tx_base = 0;

void *sys_config_get_reg_crg(void)
{
    return g_reg_crg_base;
}

void *sys_config_get_reg_sys(void)
{
    return g_reg_sys_base;
}

void *sys_config_get_reg_ddr(void)
{
    return g_reg_ddr_base;
}

void *sys_config_get_reg_misc(void)
{
    return g_reg_misc_base;
}

void *sys_config_get_reg_iocfg(void)
{
    return g_reg_iocfg_base;
}

void *sys_config_get_reg_iocfg2(void)
{
    return g_reg_iocfg2_base;
}

void *sys_config_get_reg_gpio(void)
{
    return g_reg_gpio_base;
}

void *sys_config_get_reg_mipi_tx(void)
{
    return g_reg_mipi_tx_base;
}

void write_reg32(unsigned long addr, unsigned long value, unsigned long mask)
{
    unsigned long t;

    t = sys_read((const volatile void *)(uintptr_t)addr);
    t &= ~mask;
    t |= value & mask;

    sys_writel((volatile void *)(uintptr_t)addr, t);
}

static int g_chip = CHIP_SS928V100;
static char g_sensor_list[SENSOR_LIST_CMDLINE_LEN] = "sns0=hy_s0603,sns1=hy_s0603,sns2=hy_s0603,sns3=hy_s0603";
static int g_vi_intf_type = VI_MIPI_RX_MODE;
static char g_vi_sensor_name[SENSOR_NUM][SENSOR_NAME_LEN] = {SENSOR_NAME_HY_S0603, SENSOR_NAME_HY_S0603,
                                                             SENSOR_NAME_HY_S0603, SENSOR_NAME_HY_S0603};
static int g_vo_intf_type = VO_MIPI_TX_MODE;
/* bt.1120 bt.656 mipi_tx rgb_6bit rgb_8bit rgb_16bit rgb_18bit rgb_24bit */
static char g_vo_intf[VO_INTF_NAME_LEN] = VO_INTF_NAME_MIPI_TX;
static int g_hdmi_en = 1;
static int g_i2c_en = 1;
static int g_audio_en = 1;
static int g_gpio11_2_3_en = 1;

module_param_string(sensors, g_sensor_list, SENSOR_LIST_CMDLINE_LEN, 0600);
MODULE_PARM_DESC(sensors, "sns0=hy_s0603,sns1=hy_s0603,sns2=hy_s0603,sns3=hy_s0603,sns4=hy_s0603");
module_param_string(vo_intf, g_vo_intf, VO_INTF_NAME_LEN, 0600);
module_param(g_hdmi_en, int, 0600);
module_param(g_i2c_en, int, 0600);
module_param(g_audio_en, int, 0600);
module_param(g_gpio11_2_3_en, int, 0600);

int sys_config_get_chip_type(void)
{
    return g_chip;
}

int sys_config_get_vo_intf_type(void)
{
    return g_vo_intf_type;
}

int sys_config_get_vi_intf_type(void)
{
    return g_vi_intf_type;
}

int sys_config_get_hdmi_en(void)
{
    return g_hdmi_en;
}

int sys_config_get_i2c_en(void)
{
    return g_i2c_en;
}

int sys_config_get_audio_en(void)
{
    return g_audio_en;
}

int sys_config_get_gpio11_2_3_en(void)
{
    return g_gpio11_2_3_en;
}

#define sys_config_ioremap_return(reg_name, addr, reg_len) \
    do {                                                   \
        (reg_name) = (void *)ioremap((addr), (reg_len));   \
        if ((reg_name) == NULL) {                          \
            return (-1);                                   \
        }                                                  \
    } while (0)

#define sys_config_iounmap(reg_name) \
    do {                             \
        if ((reg_name) != NULL) {    \
            iounmap((reg_name));     \
            (reg_name) = 0;          \
        }                            \
    } while (0)

static int sysconfig_init(void)
{
    sys_config_ioremap_return(g_reg_crg_base, 0x11010000, 0x10000);
    sys_config_ioremap_return(g_reg_sys_base, 0x11020000, 0x4000);
    sys_config_ioremap_return(g_reg_misc_base, 0x11024000, 0x5000);
    sys_config_ioremap_return(g_reg_ddr_base, 0x11140000, 0x10000);
    sys_config_ioremap_return(g_reg_iocfg_base, 0x10230000, 0x10000);
    sys_config_ioremap_return(g_reg_iocfg2_base, 0x102f0000, 0x10000);
    sys_config_ioremap_return(g_reg_gpio_base, 0x11090000, 0x12000);
    sys_config_ioremap_return(g_reg_mipi_tx_base, 0x17A80000, 0x10000);
    return 0;
}

static void sysconfig_exit(void)
{
    sys_config_iounmap(g_reg_crg_base);
    sys_config_iounmap(g_reg_sys_base);
    sys_config_iounmap(g_reg_misc_base);
    sys_config_iounmap(g_reg_ddr_base);
    sys_config_iounmap(g_reg_iocfg_base);
    sys_config_iounmap(g_reg_iocfg2_base);
    sys_config_iounmap(g_reg_gpio_base);
    sys_config_iounmap(g_reg_mipi_tx_base);
}

static int vo_intf_is_name_equal(const char *intf_name)
{
    return (strncmp(intf_name, g_vo_intf, strlen(intf_name)) == 0) ? 1 : 0;
}

static int sys_cfg_vo_get_intf_mode(void)
{
    int vo_intf_mode = sys_config_get_vo_intf_type();
    char vo_bt1120_name[2][VO_INTF_NAME_LEN] = {"bt.1120", "bt1120"};        /* 2 name */
    char vo_bt656_name[2][VO_INTF_NAME_LEN] = {"bt.656", "bt656"};           /* 2 name */
    char vo_mipi_tx_name[2][VO_INTF_NAME_LEN] = {"mipitx", "mipi_tx"};       /* 2 name */
    char vo_rgb_6bit_name[2][VO_INTF_NAME_LEN] = {"rgb_6bit", "rgb6bit"};  /* 2 name */
    char vo_rgb_8bit_name[2][VO_INTF_NAME_LEN] = {"rgb_8bit", "rgb8bit"};  /* 2 name */
    char vo_rgb_16bit_name[2][VO_INTF_NAME_LEN] = {"rgb_16bit", "rgb16bit"}; /* 2 name */
    char vo_rgb_18bit_name[2][VO_INTF_NAME_LEN] = {"rgb_18bit", "rgb18bit"}; /* 2 name */
    char vo_rgb_24bit_name[2][VO_INTF_NAME_LEN] = {"rgb_24bit", "rgb24bit"}; /* 2 name */
    char vo_none_name[1][VO_INTF_NAME_LEN] = {"none"};                      /* 1 name */

    if (vo_intf_is_name_equal(vo_bt1120_name[0]) || vo_intf_is_name_equal(vo_bt1120_name[1])) {
        vo_intf_mode = VO_BT1120_MODE;
    } else if (vo_intf_is_name_equal(vo_bt656_name[0]) || vo_intf_is_name_equal(vo_bt656_name[1])) {
        vo_intf_mode = VO_BT656_MODE;
    } else if (vo_intf_is_name_equal(vo_mipi_tx_name[0]) || vo_intf_is_name_equal(vo_mipi_tx_name[1])) {
        vo_intf_mode = VO_MIPI_TX_MODE;
    } else if (vo_intf_is_name_equal(vo_rgb_6bit_name[0]) || vo_intf_is_name_equal(vo_rgb_6bit_name[1])) {
        vo_intf_mode = VO_RGB_6BIT_MODE;
    } else if (vo_intf_is_name_equal(vo_rgb_8bit_name[0]) || vo_intf_is_name_equal(vo_rgb_8bit_name[1])) {
        vo_intf_mode = VO_RGB_8BIT_MODE;
    } else if (vo_intf_is_name_equal(vo_rgb_16bit_name[0]) || vo_intf_is_name_equal(vo_rgb_16bit_name[1])) {
        vo_intf_mode = VO_RGB_16BIT_MODE;
    } else if (vo_intf_is_name_equal(vo_rgb_18bit_name[0]) || vo_intf_is_name_equal(vo_rgb_18bit_name[1])) {
        vo_intf_mode = VO_RGB_18BIT_MODE;
    } else if (vo_intf_is_name_equal(vo_rgb_24bit_name[0]) || vo_intf_is_name_equal(vo_rgb_24bit_name[1])) {
        vo_intf_mode = VO_RGB_24BIT_MODE;
    } else if (vo_intf_is_name_equal(vo_none_name[0])) {
        vo_intf_mode = VO_NONE_MODE;
    } else {
        sys_config_print("vo_intf: %s is invalid, vo_intf_mode will keep the default %u(%s)\n", g_vo_intf,
            vo_intf_mode, VO_INTF_NAME_MIPI_TX);
    }

    return vo_intf_mode;
}

static void sys_config_parse_vo_intf(void)
{
    /* parse vo intf */
    g_vo_intf_type = sys_cfg_vo_get_intf_mode();
}

static int parse_sensor_index(char *s, int s_len)
{
    char tmp[SENSOR_NAME_MAX] = {0};
    int i;
    char *line = NULL;
    int index = -1;

    if (s_len == 0) {
        sys_config_print("name len is 0!\n");
        return index;
    }
    line = strsep(&s, "=");
    if (line == NULL) {
        printk("FUNC:%s line:%d  err sensor index: [%s] \n", __FUNCTION__, __LINE__, s);
        return index;
    }

    for (i = 0; i < 4; i++) { /* max sensor num 4 */
        (void)snprintf_s(tmp, sizeof(tmp), sizeof(tmp) - 1, "sns%d", i);

        if (strncmp(tmp, line, SENSOR_NAME_MAX) == 0) {
            index = i;
            return index;
        }
    }

    printk("FUNC:%s line:%d  SNS prefix:[%s] is not supported !\n", __FUNCTION__, __LINE__, line);
    return index;
}

static int parse_sensor_name(char *s, char *name, unsigned int name_len)
{
    int ret;
    unsigned int len;
    char *line = NULL;

    if (name_len == 0) {
        sys_config_print("name len is 0!\n");
        return -1;
    }
    line = strsep(&s, "=");
    if (line == NULL) {
        return -1;
    }

    line = strsep(&s, "=");
    if (line != NULL) {
        len = strlen(line);
        if (len >= SENSOR_NAME_LEN) {
            printk("FUNC:%s line:%d  name:[%s] is too long, can not longer than %d\n", __FUNCTION__, __LINE__, line,
                SENSOR_NAME_LEN);
            return -1;
        }

        ret = strncpy_s(name, SENSOR_NAME_LEN, line, SENSOR_NAME_LEN - 1);
        if (ret != EOK) {
            return -1;
        }
        return 0;
    }

    return -1;
}

static void sensor_clock_config(int index, unsigned int clock)
{
    int offset = 0x8440;
    offset += index * (0x20);
    sys_writel(g_reg_crg_base + offset, clock);
}

static unsigned int parse_sensor_clock(const char *name, unsigned int name_len)
{
    unsigned int clock = 0x0;
    unsigned int len;

    if (name_len == 0) {
        sys_config_print("name len is 0!\n");
        return clock;
    }
    len = SENSOR_NAME_LEN;

    /* get clock from sensor name. */
    if ((strncmp(SENSOR_NAME_HY_S0603, name, len) == 0) ||
        (strncmp(SENSOR_NAME_SC450AI, name, len) == 0)) {
        clock = 0xA001; /* 0xA001: 27MHz */
    } else if (strncmp(SENSOR_NAME_OS08A20, name, len) == 0) {
        clock = 0x4001;
    } else if (strncmp(MIPI_AD, name, len) == 0) {
        clock = 0x4010;
    } else {
        sys_config_print("no sensor name matched!\n");
        return clock;
    }

    return clock;
}

static int is_coms(const char *name, unsigned int name_len)
{
    unsigned int len;

    if (name_len == 0) {
        sys_config_print("name len is 0!\n");
        return 0;
    }
    len = SENSOR_NAME_LEN;

    if ((strncmp("bt1120", name, len) == 0) || (strncmp("bt656", name, len) == 0) ||
        (strncmp("bt601", name, len) == 0)) {
        return 1;
    } else {
        return 0;
    }
}

static int is_thermo_sensor(const char *name, unsigned int name_len)
{
    unsigned int len;

    if (name_len == 0) {
        sys_config_print("name len is 0!\n");
        return 0;
    }
    len = SENSOR_NAME_LEN;

    if ((strncmp("t0", name, len) == 0) ||
        (strncmp("t1", name, len) == 0) ||
        (strncmp("t2", name, len) == 0) ||
        (strncmp("t3", name, len) == 0)) {
        return 1;
    } else {
        return 0;
    }
}

static void coms_clock_config(int index)
{
    if (index == 3) { /* only port3 support cmos */
        sys_writel(g_reg_crg_base + 0x91C4, 0x5010);
        sys_writel(g_reg_crg_base + 0x8460, 0x3); /* sensor1 */
        sys_writel(g_reg_crg_base + 0x8460, 0xA010);
    } else {
        sys_config_print("SNS%d is not supported by cmos!\n", index);
    }
}

static void thermo_clock_config(const char *name, int index)
{
    if (strncmp("t0", name, SENSOR_NAME_LEN) == 0) {
        sys_writel(g_reg_crg_base + 0x9184, 0x5001);
        sys_writel(g_reg_crg_base + 0x4648, 0x5077);
        sys_writel(g_reg_crg_base + 0x4648, 0x205070);
        sys_writel(g_reg_crg_base + 0x4640, 0x11);
        sys_writel(g_reg_crg_base + 0x4640, 0x10);
    } else if (strncmp("t1", name, SENSOR_NAME_LEN) == 0) {
        sys_writel(g_reg_crg_base + 0x9184, 0x5001);
        sys_writel(g_reg_crg_base + 0x4648, 0x2077);
        sys_writel(g_reg_crg_base + 0x4648, 0x2070);
        sys_writel(g_reg_crg_base + 0x4640, 0x11);
        sys_writel(g_reg_crg_base + 0x4640, 0x10);
    } else if (strncmp("t2", name, SENSOR_NAME_LEN) == 0) {
        sys_writel(g_reg_crg_base + 0x9184, 0x5001);
        sys_writel(g_reg_crg_base + 0x4648, 0x207F);
        sys_writel(g_reg_crg_base + 0x4648, 0x2070);
        sys_writel(g_reg_crg_base + 0x4640, 0x11);
        sys_writel(g_reg_crg_base + 0x4640, 0x10);
    } else {
        sys_writel(g_reg_crg_base + 0x91C4, 0x5010);
        sys_writel(g_reg_crg_base + 0x8460, 0x3); /* sensor1 */
        sys_writel(g_reg_crg_base + 0x8460, 0xA010);

        sys_writel(g_reg_crg_base + 0x9184, 0x7010);
        sys_writel(g_reg_crg_base + 0x4648, 0x0000007F);
        sys_writel(g_reg_crg_base + 0x4648, 0x00300070);
        sys_writel(g_reg_crg_base + 0x4640, 0x11);
        sys_writel(g_reg_crg_base + 0x4640, 0x10);
    }
}

static void prase_sensor(char *s)
{
    int ret;
    int index;
    char sensor_name[SENSOR_NAME_LEN];

    ret = parse_sensor_name(s, sensor_name, SENSOR_NAME_LEN);
    if (ret >= 0) {
        index = parse_sensor_index(s, strlen(s));
        if (index >= 0 && index < SENSOR_NUM) {
            ret = strncpy_s(g_vi_sensor_name[index], SENSOR_NAME_LEN, sensor_name, SENSOR_NAME_LEN - 1);
            if (ret != EOK) {
                return;
            }
        }
        /* set vi_intf_type by sensor_name, default: VI_MIPI_RX_MODE */
        if (index == 3 && is_coms(sensor_name, SENSOR_NAME_LEN)) { /* only port3 support cmos */
            g_vi_intf_type = VI_CMOS_BT1120_MODE;
        } else if (is_thermo_sensor(sensor_name, SENSOR_NAME_LEN)) {
            g_vi_intf_type = VI_THERMO_MODE;
            if (strncmp("t3", sensor_name, SENSOR_NAME_LEN) == 0) {
                g_vi_intf_type = VI_THERMO_T3_MODE;
            }
        }
    }
}

static int prase_sensor_mode(char *s, unsigned int str_len)
{
    char *line = NULL;
    char *argv[SENSOR_NAME_MAX] = {NULL};
    int i;

    if (str_len == 0) {
        sys_config_print("name len is 0!\n");
        return 0;
    }

    if (strncmp("none", s, 5) == 0) { /* 5 charater */
        sys_config_print("==========no sensor config==========\n");
        g_vi_intf_type = VI_NONE_MODE;
        return 0;
    }

    line = strsep(&s, ":");
    while (line != NULL) {
        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL; i++) {
            prase_sensor(argv[i]);

            if (i == (ARRAY_SIZE(argv) - 1)) {
                break;
            }
        }
        line = strsep(&s, ":");
    }

    return 0;
}

static void sys_config_parse_mod_param(void)
{
    sys_config_parse_vo_intf();
    prase_sensor_mode(g_sensor_list, SENSOR_LIST_CMDLINE_LEN);
}

static void sensor_cfg(void)
{
    int i;
    unsigned int clock;

    for (i = 0; i < SENSOR_NUM; i++) {
        sys_config_print("==========sensor%d: %s==========\n", i, g_vi_sensor_name[i]);
        clock = parse_sensor_clock(g_vi_sensor_name[i], SENSOR_NAME_LEN);

        if (is_coms(g_vi_sensor_name[i], SENSOR_NAME_LEN)) {
            coms_clock_config(i);
        } else if (is_thermo_sensor(g_vi_sensor_name[i], SENSOR_NAME_LEN)) {
            thermo_clock_config(g_vi_sensor_name[i], i);
        } else {
            sensor_clock_config(i, clock);
        }
    }
}

static int __init ot_sysconfig_init(void)
{
    int ret;

    ret = sysconfig_init();
    if (ret != 0) {
        goto end;
    }

    sys_config_parse_mod_param();
    sys_ctl(VI_OFFLINE_VIDEO_ADVANCED_VPSS_OFFLINE);
    clk_cfg();
    pin_mux();

    if (g_vi_intf_type != VI_NONE_MODE) {
        sensor_cfg();
    }

    return 0;

end:
    sysconfig_exit();
    return 0;
}

static void __exit ot_sysconfig_exit(void)
{
    sysconfig_exit();
    return;
}

module_init(ot_sysconfig_init);
module_exit(ot_sysconfig_exit);

MODULE_LICENSE("GPL");
