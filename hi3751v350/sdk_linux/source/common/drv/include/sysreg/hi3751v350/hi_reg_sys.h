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

#ifndef __HI_REG_SYS_H__
#define __HI_REG_SYS_H__

/* Define the union U_SC_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
        MCU区域总线时钟选择
        00：24M
        01：100M
        10：100K
        11：保留
 */
        unsigned int reserved_0 : 2; /* [1..0] */
        // Comment of field: reserved_1
        /*
        reserved
        MCU区域总线时钟状态
        00：24M
        01：100M
        10：100K
        11：保留
 */
        unsigned int reserved_1 : 2; /* [3..2] */
        // Comment of field: reserved_2
        /*
        reserved
        MCU区域总线时钟分频选择
        00：mcu_bus_clk 1分频；
        01：mcu_bus_clk 2分频；
        10：mcu_bus_clk 3分频；
        11：mcu_bus_clk 4分频；
 */
        unsigned int reserved_2 : 2; /* [5..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 4; /* [9..6] */
        // Comment of field: reserved_4
        /*
        reserved
        POR 复位源头选择指示信号：
        0：选择片外复位源；
        1：选择片内POR复位源；
 */
        unsigned int reserved_4 : 1; /* [10] */
        // Comment of field: reserved_5
        /*
        保留。读时返回0，写时无影响。
 */
        unsigned int reserved_5 : 21; /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_ctrl;

/* Define the union U_SC_EQUATOR_HADD */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        TVD or HDDEC给equator模拟的clamp选择：
        0：TVD；
        1：HDDEC。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        EQUATOR DAC0时钟和数据选择信号
        2'b00:aif时钟和数据选择信号
        2'b01:cvbs的时钟和数据选择信号；
        2'b10:保留；
        2'b11:afe的时钟和数据选择信号。
 */
        unsigned int reserved_1 : 2; /* [2..1] */
        // Comment of field: equator_agc_sel
        /*
        TVD or equator数字部分给equator模拟的AGC
        0：TVD；
        1：EQUATOR数字部分。
 */
        unsigned int equator_agc_sel : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        equator_digital的clk_cvbs_dff的时钟选择
        0: 选择模拟IP输出的clk_cvbs时钟
        1：选择clk_150m_occ时钟
 */
        unsigned int reserved_2 : 1; /* [4] */
        // Comment of field: reserved_3
        /*
        equator_digital的clk_cvbs_out的时钟选择
        0: 选择模拟IP输出的clk_cvbs进行2分频后的时钟
        1：选择clk_75m_occ时钟
 */
        unsigned int reserved_3 : 1; /* [5] */
        // Comment of field: reserved_4
        /*
        0: 选择模拟IP输出的clk_rgb时钟
        1：选择clk_150m_occ时钟
 */
        unsigned int reserved_4 : 1; /* [6] */
        unsigned int reserved_5 : 2; /* [8..7] */
        // Comment of field: reserved_6
        /*
        Equator_digital的软复位请求信号
        0：不复位；
        1：复位。
 */
        unsigned int reserved_6 : 1; /* [9] */
        // Comment of field: reserved_7
        /*
        保留。
 */
        unsigned int reserved_7 : 22; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_equator_hadd;

/* Define the union U_SC_QAM_CRG_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留 qamadc时钟选择：2选一，默认选 54Mhz
        0：core_aif 的时钟 108/2=54MHz；
        1：41.5MHz；
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        保留 qamctrl 时钟选择： 2选一，默认选crg2分频
        0：41.5MHz；
        1：qam_adc phy的随路时钟；
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        保留 qamadc正反向时钟选择：
        0：QAMADC时钟同相；
        1：QAMADC时钟反相。
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        保留 qamadc phy 的随路时钟正反向时钟选择：
        0：QAMADC时钟同相；
        1：QAMADC时钟反相。
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        保留。
 */
        unsigned int reserved_4 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_qam_crg_ctrl;

/* Define the union U_SC_FPLL_CTRL0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        pll_postdiv2 [30:28] 
        PLL第二级输出分频系数。
        
        pll_postdiv1 [26:24] 
        PLL第一级输出分频系数。
        
        pll_frac [23:0]:
        PLL小数分频系数。
 */
        unsigned int reserved_0 : 31; /* [30..0] */
        // Comment of field: reserved_1
        /*
        保留
 */
        unsigned int reserved_1 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_fpll_ctrl0;

/* Define the union U_SC_FPLL_CTRL1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        bit[27] pll_foutvco2xpd
        PLL FOUTVCO2X输出Power Down控制。
        0：正常时钟输出；
        1：不输出时钟。
        
        bit[26] pll_bypass
        PLL时钟分频旁路（bypass）控制。
        0：非旁路；
        1：旁路。
        
        bit[25] pll_dsmpd
        PLL 小数分频控制。
        0：小数模式；
        1：整数模式。
        
        bit[24] pll_dacpd
        PLL 测试信号控制。
        0：正常工作状态；
        1：power down工作状态。
        
        bit[23] pll_foutpostdivpd
        PLL FOUTPOSTDIV输出Power Down控制。
        0：正常时钟输出；
        1：不输出时钟。
        
        bit[22] pll_fout4phasepd
        PLL FOUT0~4输出Power Down控制。
        0：正常时钟输出；
        1：不输出时钟。
        
        bit[21] pll_foutvcopd
        PLL FOUTVCO输出Power Down控制。
        0：正常时钟输出；
        1：不输出时钟。
        
        bit[20] pll_pd
        PLL Power Down控制。
        0：正常工作状态；
        1：power down工作状态。
        
        bit[19:18] 保留。
        
        bit[17:12] pll_refdiv
        PLL参考时钟分频系数。
        
        bit[11:0] pll_fbdiv
        PLL整数倍频系数。
 */
        unsigned int reserved_0 : 28; /* [27..0] */
        // Comment of field: reserved_1
        /*
        保留
 */
        unsigned int reserved_1 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_fpll_ctrl1;

/* Define the union U_SC_FPLL_SSMOD_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ssmod divval[12：9]
        SSMOD divval控制。
        
        ssmod spread[8：4]
        SSMOD spread控制。
        0：0；
        1：0.1%；
        2：0.2%
        3：0.3%；
        4：0.4%；
        5：0.5%；
        6：0.6%；
        7：0.7%。
        ...
        31：3.1%
        
        ssmod downspread[3]
        SSMOD downspread控制。
        0：中间展频；
        1：向下展频。
        
        ssmod_disable[2]
        SSMOD disable控制。
        0：enable；
        1：disable。
        
        ssmod_rst_req [1]
        SSMOD复位控制。
        0：不复位；
        1：复位。
        
        ssmod_cken [0]
        SSMOD时钟门控配置,默认关闭。
        0：关闭；
        1：打开。
 */
        unsigned int reserved_0 : 13; /* [12..0] */
        // Comment of field: reserved_1
        /*
        保留
 */
        unsigned int reserved_1 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_fpll_ssmod_ctrl;

/* Define the union U_SC_FPLL_LOCK */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        保留
 */
        unsigned int reserved_1 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_fpll_lock;

/* Define the union U_SC_HDMI_ATOP_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 3; /* [3..1] */
        // Comment of field: reserved_2
        /*
        保留
 */
        unsigned int reserved_2 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_hdmi_atop_ctrl;

/* Define the union U_SC_DBGI2C_SW */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        模式状态指示信号
        0：非debug I2C模式
        1：debug I2C模式
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        保留
 */
        unsigned int reserved_1 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_dbgi2_c_sw;

/* Define the union U_SC_FEPHY_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fephy_rstn_ldo : 1; /* [0] */
        unsigned int fephy_enz_ldo : 1;  /* [1] */
        // Comment of field: fephy_en_ldo
        /*
        RSTN_LDO  ENZ_LDO   EN_LDO   VOUT
           0         X         X     VOUT=0.9V（默认开启，LDO输出电压不可调节）
           1          1        X      高阻
           1          0        0      高阻
           1          0        1     VOUT=0.9V（LDO输出电压可调节，默认输出电压为0.9V）
 */
        unsigned int fephy_en_ldo : 1; /* [2] */
        // Comment of field: fephy_patch_enable
        /*
        FEPHY的PATCH的使能信号
        0：通用输入端口为固定值0x0300
        1：通用输入端口为软件配置的参数
 */
        unsigned int fephy_patch_enable : 1; /* [3] */
        // Comment of field: fephy_tclk_enable
        /*
        FEPHY的TCLK的使能信号
 */
        unsigned int fephy_tclk_enable : 1; /* [4] */
        // Comment of field: soft_fephy_gp_i
        /*
        FEPHY的MDIO的输入信号
 */
        unsigned int soft_fephy_gp_i : 16; /* [20..5] */
        // Comment of field: soft_fephy_mdio_i
        /*
        fephy mdio数据线
 */
        unsigned int soft_fephy_mdio_i : 1; /* [21] */
        // Comment of field: soft_fephy_mdio_mdc
        /*
        fephy mdio读写时钟
 */
        unsigned int soft_fephy_mdio_mdc : 1; /* [22] */
        // Comment of field: fephy_phy_addr
        /*
        FEPHY的地址信号
 */
        unsigned int fephy_phy_addr : 5; /* [27..23] */
        // Comment of field: afe_tclk_0_sel
        /*
        afe_tclk_0和其4分频选择信号
        0： 选择afe_tclk_0
        1： 选择afe_tclk_0的4分频
 */
        unsigned int afe_tclk_0_sel : 1; /* [28] */
        // Comment of field: sc_fephy_iddq_mode
        /*
        MBIST相关信号
        0:  打开bandgap和PLL电路
        1:  关闭bandgap和PLL电路
 */
        unsigned int sc_fephy_iddq_mode : 1; /* [29] */
        // Comment of field: reserved_0
        /*
        保留
 */
        unsigned int reserved_0 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_fephy_ctrl;

/* Define the union U_SC_LOW_POWER_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: stb_poweroff
        /*
        管脚STANDBY_PWROFF控制。
        0：管脚STANDBY_PWROFF输出为低电平，控制掉电区下电；
        1：管脚STANDBY_PWROFF输出为高电平，控制掉电区不下电。
 */
        unsigned int stb_poweroff : 1; /* [1] */
        // Comment of field: mcu_lp_subsys_iso
        /*
        隔离区控制。
        4'b1011：使能，即对信号进行隔离。
        其他：禁止，即不对信号进行隔离；
 */
        unsigned int mcu_lp_subsys_iso : 4; /* [5..2] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [6] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [7] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [8] */
        // Comment of field: pwd_signals_in_mcu_iso
        /*
        隔离区控制。
        4'b1011：使能，即对信号进行隔离。
        其他：禁止，即不对信号进行隔离；
 */
        unsigned int pwd_signals_in_mcu_iso : 4; /* [12..9] */
        // Comment of field: reserved_4
        /*
        保留。
 */
        unsigned int reserved_4 : 15; /* [27..13] */
        // Comment of field: core0p9v_stable_iso
        /*
        备份：
        隔离使能控制信号。，高有效。
        待机唤醒0.9V电源稳定后，产生的寄存器状态信号，受该隔离使能控制。
        1：使能，即对信号进行隔离。
        0：禁止，即不对信号进行隔离；
 */
        unsigned int core0p9v_stable_iso : 1; /* [28] */
        // Comment of field: por_0p9v_rst_iso_status
        /*
        debug寄存器。por0p9v信号的隔离状态，高有效。
 */
        unsigned int por_0p9v_rst_iso_status : 1; /* [29] */
        // Comment of field: pwd_signals_in_mcu_iso_status
        /*
        下电区穿常电区信号的隔离状态，高有效。
 */
        unsigned int pwd_signals_in_mcu_iso_status : 1; /* [30] */
        // Comment of field: mcu_lp_subsys_iso_status
        /*
        下电区送给常电区的信号的隔离状态，高有效。
        受sc_locken控制。
 */
        unsigned int mcu_lp_subsys_iso_status : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_low_power_ctrl;

/* Define the union U_SC_HDMI_RX_HPD_PWR_PCTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hdmi_rx_pwr_pctrl
        /*
        HDMI RX PWR5V[2:0]输入管脚正反可控：
        Bit0:表示选择HDMIRX 2P0 PORT0；
        Bit1:表示选择HDMIRX 2P0 PORT1；
        Bit2:表示选择HDMIRX 2P0 PORT2；
        Bit3:reserved
        其中对应bit表示如下:
        0：正向；
        1：反向。
 */
        unsigned int hdmi_rx_pwr_pctrl : 4; /* [3..0] */
        // Comment of field: hdmi_rx_hpd_pctrl
        /*
        HDMI RX HPD[2:0]输出管脚正反可控：
        Bit4:表示选择HDMIRX 2P0 PORT0；
        Bit5:表示选择HDMIRX 2P0 PORT1；
        Bit6:表示选择HDMIRX 2P0 PORT2；
        Bit7:reserved
        其中对应bit表示如下:
        0：正向；
        1：反向。
 */
        unsigned int hdmi_rx_hpd_pctrl : 4; /* [7..4] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 4; /* [11..8] */
        // Comment of field: hdmi_rx_5v_det_mux
        /*
        表示选择控制detect 选择控制信号
        Bit12:表示选择HDMIRX 2P0 PORT0；
        Bit13:表示选择HDMIRX 2P0 PORT1；
        Bit14:表示选择HDMIRX 2P0 PORT2；
        Bit15:reserved
 */
        unsigned int hdmi_rx_5v_det_mux : 4; /* [15..12] */
        // Comment of field: hdmi_rx_5v_det_reg
        /*
        表示系统控制输入进来的待机唤醒寄存器的探测信号。
        Bit16:表示选择HDMIRX 2P0 PORT0；
        Bit17:表示选择HDMIRX 2P0 PORT1；
        Bit18:表示选择HDMIRX 2P0 PORT2；
        Bit19:reserved
 */
        unsigned int hdmi_rx_5v_det_reg : 4; /* [19..16] */
        unsigned int reserved_1 : 12;        /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_hdmi_rx_hpd_pwr_pctrl;

/* Define the union U_SC_CLKGATE_SRST_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        MCE时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        MCE软复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        pwm时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        pwm软复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        IR时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        IR模块软复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        lsadc时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_6 : 1; /* [6] */
        // Comment of field: reserved_7
        /*
        lsadc模块软复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int reserved_7 : 1; /* [7] */
        // Comment of field: reserved_8
        /*
        保留。
 */
        unsigned int reserved_8 : 1; /* [8] */
        // Comment of field: reserved_9
        /*
        保留。
 */
        unsigned int reserved_9 : 1; /* [9] */
        // Comment of field: reserved_10
        /*
        timer01时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_10 : 1; /* [10] */
        // Comment of field: reserved_11
        /*
        timer01模块软复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int reserved_11 : 1; /* [11] */
        // Comment of field: reserved_12
        /*
        UART1时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_12 : 1; /* [12] */
        // Comment of field: reserved_13
        /*
        UART1软复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int reserved_13 : 1; /* [13] */
        // Comment of field: reserved_14
        /*
        hdmirx_osc软复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int reserved_14 : 1; /* [14] */
        // Comment of field: reserved_15
        /*
        hdmirx_apb软复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int reserved_15 : 1; /* [15] */
        // Comment of field: reserved_16
        /*
        hdmirx_cec软复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int reserved_16 : 1; /* [16] */
        // Comment of field: reserved_17
        /*
        hdmirx软复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int reserved_17 : 1; /* [17] */
        // Comment of field: reserved_18
        /*
        mosdc_clk时钟源头选择：
        0:24MHz晶振时钟；
        1:20MHz
 */
        unsigned int reserved_18 : 1; /* [18] */
        // Comment of field: reserved_19
        /*
        cec_clk时钟源头选择：
        0:2MHz（晶振时钟12分频）；
        1:2.02MHz
 */
        unsigned int reserved_19 : 1; /* [19] */
        // Comment of field: reserved_20
        /*
        HDMI1.4 RX core时钟源头选择：
        0:24MHz晶振时钟；
        1:27MHz
 */
        unsigned int reserved_20 : 1; /* [20] */
        // Comment of field: reserved_21
        /*
        cbus_clk时钟源头选择：
        0:24MHz晶振时钟；
        1:20MHz
 */
        unsigned int reserved_21 : 1; /* [21] */
        // Comment of field: reserved_22
        /*
        hdmirx_mosc时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_22 : 1; /* [22] */
        // Comment of field: reserved_23
        /*
        hdmirx_osc时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_23 : 1; /* [23] */
        // Comment of field: reserved_24
        /*
        hdmirx_apb时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_24 : 1; /* [24] */
        // Comment of field: reserved_25
        /*
        hdmirx_cec时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_25 : 1; /* [25] */
        // Comment of field: reserved_26
        /*
        hdmirx_mpllref时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_26 : 1; /* [26] */
        // Comment of field: reserved_27
        /*
        hdmirx_mpllref_pll时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_27 : 1; /* [27] */
        // Comment of field: pd_rst_req
        /*
        系统下电复位请求。
        0：不复位；
        1：复位。
 */
        unsigned int pd_rst_req : 1; /* [28] */
        // Comment of field: reserved_28
        /*
        hdmirx_edid时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_28 : 1; /* [29] */
        // Comment of field: reserved_29
        /*
        hdmirx_cbus时钟门控。
        0：关断；
        1：打开。
 */
        unsigned int reserved_29 : 1; /* [30] */
        // Comment of field: reserved_30
        /*
        hdmirx edid的选择信号；
        0：2M时钟选择
        1：表示2.02M时钟选择。
 */
        unsigned int reserved_30 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_clkgate_srst_ctrl;

/* Define the union U_SC_CA_RST_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_ca_rst_ctrl;

/* Define the union U_SC_WDG_RST_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: wdg_rst_ctrl
        /*
        软件控制WDG_RST信号源头。
        0：WDG0_RST；；
        1：TIMER01中断；
        不受全局软复位控制。
 */
        unsigned int wdg_rst_ctrl : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [1] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_wdg_rst_ctrl;

/* Define the union U_SC_DBGI2C_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 7; /* [6..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_dbgi2_c_ctrl;

/* Define the union U_SC_DDRPHY_LP_EN */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ddrphy_lp_en
        /*
        ddrphy0 retention 功能控制寄存器。
        不受全局软复位控制。
 */
        unsigned int ddrphy_lp_en : 2; /* [1..0] */
        // Comment of field: ddrphy_lp_en1
        /*
        reserved
 */
        unsigned int ddrphy_lp_en1 : 2; /* [3..2] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_ddrphy_lp_en;

/* Define the union U_SC_MCU_HPM_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 5; /* [5..1] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 1; /* [6] */
        // Comment of field: reserved_3
        /*
        保留。
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        保留
 */
        unsigned int reserved_4 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_mcu_hpm_ctrl;

/* Define the union U_SC_MCU_HPM_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 10; /* [9..0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 1; /* [10] */
        // Comment of field: reserved_2
        /*
        保留
 */
        unsigned int reserved_2 : 21; /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_mcu_hpm_stat;

/* Define the union U_SC_MCU_LDO_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mcu_ldo_vset
        /*
        mcu_ldo_vset配置值。
        0000：输出0.74V
        0001：输出0.76V
        0010：输出0.78V
        0011：输出0.80V
        0100：输出0.82V
        0101：输出0.84V
        0110：输出0.86V
        0111：输出0.88V
        1000：输出0.90V（默认）
        1001：输出0.92V
        1010：输出0.94V
        1011：输出0.96V
        1100：输出0.98V
        1101：输出1.00V
        1110：输出1.02V
        1111：输出1.04V
        不受全局软复位控制。
 */
        unsigned int mcu_ldo_vset : 4; /* [3..0] */
        // Comment of field: mcu_ldo_fuse
        /*
        调节内部精确电压基准:
        0000: +/- 0mV
        0001: - 8.5mV
        0010: - 17mV
        0011: - 25.5V
        0100: - 34mV
        0101: - 42.5mV
        0110: - 51mV
        0111: - 59.5mV
        1000: + 8.5mV
        1001: + 17mV
        1010: + 25.5mV
        1011: + 34mV
        1100: + 42.5mV
        1101: + 51mV
        1110: + 59.5mV
        1111：+ 68mV
        不受全局软复位控制。
 */
        unsigned int mcu_ldo_fuse : 4; /* [7..4] */
        // Comment of field: reserved_0
        /*
        保留
 */
        unsigned int reserved_0 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_mcu_ldo_ctrl;

/* Define the union U_SC_QAM_LDO_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 4; /* [3..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 4; /* [7..4] */
        // Comment of field: reserved_2
        /*
        保留
 */
        unsigned int reserved_2 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_qam_ldo_ctrl;

/* Define the union U_SC_SYS_DBG0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 7; /* [7..1] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [9] */
        // Comment of field: reserved_4
        /*
        保留
 */
        unsigned int reserved_4 : 22; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_sys_dbg0;

/* Define the union U_AMP_MUTE */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: amp_mute_out
        /*
        板级音频AMP MUTE功能控制信号：
        0：关闭MUTE功能
        1：开启MUTE功能
 */
        unsigned int amp_mute_out : 1; /* [0] */
        // Comment of field: amp_mute_oen
        /*
        板级音频AMP MUTE输出使能：
        0：输出使能，芯片输出mute控制；
        1：板级mute电路控制
 */
        unsigned int amp_mute_oen : 1; /* [1] */
        unsigned int reserved_0 : 30;  /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_amp_mute;

/* Define the union U_SC_2P0_CLKGATE_SRST_CTRL_ADDR */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        cbus时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        cec时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        edid时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        osc时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        prif时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        sheman时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        aphy_p0时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_6 : 1; /* [6] */
        // Comment of field: reserved_7
        /*
        aphy_p1时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_7 : 1; /* [7] */
        // Comment of field: reserved_8
        /*
        aphy_p2时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_8 : 1; /* [8] */
        // Comment of field: reserved_9
        /*
        fephy时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_9 : 1; /* [9] */
        // Comment of field: reserved_10
        /*
        usb2 phy的参考时钟信号门控：
        0：关断；
        1：打开。
 */
        unsigned int reserved_10 : 1; /* [10] */
        // Comment of field: reserved_11
        /*
        cec 时钟选择：
        0：2M；
        1：2.02M。
 */
        unsigned int reserved_11 : 1; /* [11] */
        // Comment of field: reserved_12
        /*
        cbus时钟选择：
        0：24M；
        1：20M。
 */
        unsigned int reserved_12 : 1; /* [12] */
        // Comment of field: reserved_13
        /*
        edid 时钟选择：
        0：2M；
        1：2.02M。
 */
        unsigned int reserved_13 : 1; /* [13] */
        // Comment of field: reserved_14
        /*
        sheman时钟选择：
        0：24M；
        1：27M。
 */
        unsigned int reserved_14 : 1; /* [14] */
        // Comment of field: reserved_15
        /*
        aphy_p0时钟选择：
        0：选择hdmi_phy_p0输出的时钟(AD_CLKO_DVI_DVI32_P0)
        1：选择DFT时钟
 */
        unsigned int reserved_15 : 1; /* [15] */
        // Comment of field: reserved_16
        /*
        aphy_p1时钟选择：
        0：选择hdmi_phy_p1输出的时钟(AD_CLKO_DVI_DVI32_P1)
        1：选择DFT时钟
 */
        unsigned int reserved_16 : 1; /* [16] */
        // Comment of field: reserved_17
        /*
        aphy_p2时钟选择：
        0：选择hdmi_phy_p2输出的时钟(AD_CLKO_DVI_DVI32_P2)
        1：选择DFT时钟
 */
        unsigned int reserved_17 : 1; /* [17] */
        // Comment of field: reserved_18
        /*
        cec软复位请求：
        0：不复位；
        1：复位。
 */
        unsigned int reserved_18 : 1; /* [18] */
        // Comment of field: reserved_19
        /*
        osc软复位请求：
        0：不复位；
        1：复位。
 */
        unsigned int reserved_19 : 1; /* [19] */
        // Comment of field: reserved_20
        /*
        prif软复位请求：
        0：不复位；
        1：复位。
 */
        unsigned int reserved_20 : 1; /* [20] */
        // Comment of field: reserved_21
        /*
        sheman软复位请求：
        0：不复位；
        1：复位。
 */
        unsigned int reserved_21 : 1; /* [21] */
        // Comment of field: reserved_22
        /*
        aphy_p0软复位请求信号：
        0：不复位；
        1：复位。
 */
        unsigned int reserved_22 : 1; /* [22] */
        // Comment of field: reserved_23
        /*
        aphy_p1软复位请求信号：
        0：不复位；
        1：复位。
 */
        unsigned int reserved_23 : 1; /* [23] */
        // Comment of field: reserved_24
        /*
        aphy_p2软复位请求信号：
        0：不复位；
        1：复位。
 */
        unsigned int reserved_24 : 1; /* [24] */
        // Comment of field: reserved_25
        /*
        fephy的软复位请求信号：
        0：不复位；
        1：复位。
 */
        unsigned int reserved_25 : 1; /* [25] */
        // Comment of field: reserved_26
        /*
        usb2 phy0_p1的软复位请求信号，用于复位UTM状态机
        0：不复位；
        1：复位。
 */
        unsigned int reserved_26 : 1; /* [26] */
        // Comment of field: reserved_27
        /*
        usb2 phy0_p1的软复位请求信号，用于usb2_phy0_p1的上电复位
        0：不复位；
        1：复位。
 */
        unsigned int reserved_27 : 1; /* [27] */
        // Comment of field: reserved_28
        /*
        用于测试usb2_phy0_p1的软复位请求信号
        0：不复位；
        1：复位。
 */
        unsigned int reserved_28 : 1; /* [28] */
        // Comment of field: reserved_29
        /*
        usb2_phy0_p1的apb_clk的时钟门控使能
        0：关断；
        1：打开。
 */
        unsigned int reserved_29 : 1; /* [29] */
        // Comment of field: reserved_30
        /*
        保留。
 */
        unsigned int reserved_30 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sc_2_p0_clkgate_srst_ctrl_addr;

/* Define the union U_PERI_DDC_EDID_TIMEOUT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vga_ddc_timeout
        /*
        VGA的debug寄存器。
 */
        unsigned int vga_ddc_timeout : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: vga_ddc_sda_del_en
        /*
        VGA的sda路径延时使能。
        0：不使能;
        1：使能。
 */
        unsigned int vga_ddc_sda_del_en : 1; /* [4] */
        // Comment of field: vga_reg_ddc_del_en
        /*
        VGA的scl路径延时使能。
        0：不使能;
        1：使能。
 */
        unsigned int vga_reg_ddc_del_en : 1; /* [5] */
        // Comment of field: vga_reg_ddc_en
        /*
        VGA的ddc_edid使能。
        0：不使能;
        1：使能。
 */
        unsigned int vga_reg_ddc_en : 1; /* [6] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: vga_ddc_filter_sel
        /*
        reserved
 */
        unsigned int vga_ddc_filter_sel : 2; /* [9..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [10] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [12] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [13] */
        // Comment of field: reserved_6
        /*
        保留。
 */
        unsigned int reserved_6 : 18; /* [31..14] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_ddc_edid_timeout;

/* Define the union U_RNG_RAND_LOCK */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_rng_rand_lock;

/* Define the union U_AON_PERI_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        vga的scl_in和sda_in输入管脚选择信号：
        0：来自常电区管脚；
        1：reserved。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: uart1_use_lbe_txd_en
        /*
        uart txuart有数据就发送数据到txd使能，
        1：不使能；
        0：使能；
 */
        unsigned int uart1_use_lbe_txd_en : 1; /* [1] */
        // Comment of field: sc_uart0_sel
        /*
        uart0 IO选择：
        0：uart  rxd
        1：uart rxd cp
 */
        unsigned int sc_uart0_sel : 1; /* [2] */
        // Comment of field: sc_uart1_sel
        /*
        uart1 IO选择：
        0：uart rxd
        1：uart rxd cp
 */
        unsigned int sc_uart1_sel : 1; /* [3] */
        // Comment of field: sc_jtag_dbg_release
        /*
        退出2线JTAG调试。
        配置为1再配置为0：退出2线JTAG调试。
        其他：支持2线JTAG调试。
 */
        unsigned int sc_jtag_dbg_release : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [5] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [6] */
        // Comment of field: peri_uart1_od_sel
        /*
        uart1输出数据的od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int peri_uart1_od_sel : 1; /* [7] */
        // Comment of field: hdmirx_det_io_sel
        /*
        hdmirx detect io选择。
        0:HDMIRX DET选择新5V IO；
        1:HDMIRX DET选择普通数字IO。
 */
        unsigned int hdmirx_det_io_sel : 1; /* [8] */
        // Comment of field: backup_edid_sel
        /*
        UART和VGA EDID IO自动切换使能控制。
        0:UART和VGA_EDID IO自动切换；
        1:UART和VGA_EDID IO不自动切换，使用普通的管脚复用。
 */
        unsigned int backup_edid_sel : 1; /* [9] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 2; /* [11..10] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_aon_peri_ctrl;

/* Define the union U_AON_MEM_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ir_rft_emasa
        /*
        memory调速信号，详见BES memory调速说明
 */
        unsigned int ir_rft_emasa : 1; /* [0] */
        // Comment of field: ir_rft_emab
        /*
        memory调速信号，详见BES memory调速说明
 */
        unsigned int ir_rft_emab : 3; /* [3..1] */
        // Comment of field: ir_rft_emaa
        /*
        memory调速信号，详见BES memory调速说明
 */
        unsigned int ir_rft_emaa : 3; /* [6..4] */
        // Comment of field: ir_rft_colldisn
        /*
        memory调速信号，详见BES memory调速说明
 */
        unsigned int ir_rft_colldisn : 1; /* [7] */
        // Comment of field: mce_rfs_emaw
        /*
        控制了mce和fephy memory调速信号，详见BES memory调速说明
 */
        unsigned int mce_rfs_emaw : 2; /* [9..8] */
        // Comment of field: mce_rfs_ema
        /*
        控制了mce和fephy memory调速信号，详见BES memory调速说明
 */
        unsigned int mce_rfs_ema : 3; /* [12..10] */
        // Comment of field: mce_ras_emaw
        /*
        控制了mce和fephy memory调速信号，详见BES memory调速说明
 */
        unsigned int mce_ras_emaw : 2; /* [14..13] */
        // Comment of field: mce_ras_ema
        /*
        控制了mce和fephy memory调速信号，详见BES memory调速说明
 */
        unsigned int mce_ras_ema : 3; /* [17..15] */
        // Comment of field: mce_ras_wabl
        /*
        控制了mce和fephy memory调速信号，详见BES memory调速说明
 */
        unsigned int mce_ras_wabl : 1; /* [18] */
        // Comment of field: mce_ras_stov
        /*
        控制了mce和fephy memory调速信号，详见BES memory调速说明
 */
        unsigned int mce_ras_stov : 1; /* [19] */
        // Comment of field: mce_ras_emas
        /*
        控制了mce和fephy memory调速信号，详见BES memory调速说明
 */
        unsigned int mce_ras_emas : 1; /* [20] */
        // Comment of field: mce_ras_wablm
        /*
        控制了mce和fephy memory调速信号，详见BES memory调速说明
 */
        unsigned int mce_ras_wablm : 2; /* [22..21] */
        // Comment of field: mce_rfs_wabl
        /*
        控制了mce和fephy memory调速信号，详见BES memory调速说明
 */
        unsigned int mce_rfs_wabl : 1; /* [23] */
        // Comment of field: mce_rfs_wablm
        /*
        控制了mce和fephy memory调速信号，详见BES memory调速说明
 */
        unsigned int mce_rfs_wablm : 2; /* [25..24] */
        // Comment of field: fephy_ros_ema
        /*
        memory调速信号，详见BES memory调速说明
 */
        unsigned int fephy_ros_ema : 3; /* [28..26] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 3; /* [31..29] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_aon_mem_ctrl;

/* Define the union U_HDMIRX_HPD_TEST */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hpd5v_io_0_test
        /*
        hdmirx 5v new io测试寄存器
 */
        unsigned int hpd5v_io_0_test : 8; /* [7..0] */
        // Comment of field: hpd5v_io_1_test
        /*
        hdmirx 5v new io测试寄存器
 */
        unsigned int hpd5v_io_1_test : 8; /* [15..8] */
        // Comment of field: hpd5v_io_2_test
        /*
        hdmirx 5v new io测试寄存器
 */
        unsigned int hpd5v_io_2_test : 8; /* [23..16] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hdmirx_hpd_test;

/* ============================================================================= */
/* Define the global struct */
typedef struct {
    volatile u_sc_ctrl sc_ctrl;
    volatile unsigned int sc_sysres;
    volatile u_sc_equator_hadd sc_equator_hadd;
    volatile u_sc_qam_crg_ctrl sc_qam_crg_ctrl;
    volatile u_sc_fpll_ctrl0 sc_fpll_ctrl0;
    volatile u_sc_fpll_ctrl1 sc_fpll_ctrl1;
    volatile u_sc_fpll_ssmod_ctrl sc_fpll_ssmod_ctrl;
    volatile u_sc_fpll_lock sc_fpll_lock;
    volatile u_sc_hdmi_atop_ctrl sc_hdmi_atop_ctrl;
    volatile u_sc_dbgi2_c_sw sc_dbgi2_c_sw;
    volatile u_sc_fephy_ctrl sc_fephy_ctrl;
    volatile unsigned int sc_core0_p9_v_stable_status;
    volatile u_sc_low_power_ctrl sc_low_power_ctrl;
    volatile u_sc_hdmi_rx_hpd_pwr_pctrl sc_hdmi_rx_hpd_pwr_pctrl;
    volatile u_sc_clkgate_srst_ctrl sc_clkgate_srst_ctrl;
    volatile u_sc_ca_rst_ctrl sc_ca_rst_ctrl;
    volatile u_sc_wdg_rst_ctrl sc_wdg_rst_ctrl;
    volatile u_sc_dbgi2_c_ctrl sc_dbgi2_c_ctrl;
    volatile u_sc_ddrphy_lp_en sc_ddrphy_lp_en;
    volatile u_sc_mcu_hpm_ctrl sc_mcu_hpm_ctrl;
    volatile u_sc_mcu_hpm_stat sc_mcu_hpm_stat;
    volatile u_sc_mcu_ldo_ctrl sc_mcu_ldo_ctrl;
    volatile u_sc_qam_ldo_ctrl sc_qam_ldo_ctrl;
    volatile unsigned int sc_gen0;
    volatile unsigned int sc_gen1;
    volatile unsigned int sc_gen2;
    volatile unsigned int sc_gen3;
    volatile unsigned int sc_gen4;
    volatile unsigned int sc_gen5;
    volatile unsigned int sc_gen6;
    volatile unsigned int sc_gen7;
    volatile unsigned int sc_gen8;
    volatile unsigned int sc_gen9;
    volatile unsigned int sc_gen10;
    volatile unsigned int sc_gen11;
    volatile unsigned int sc_gen12;
    volatile unsigned int sc_gen13;
    volatile unsigned int sc_gen14;
    volatile unsigned int sc_gen15;
    volatile unsigned int sc_gen16;
    volatile unsigned int sc_gen17;
    volatile unsigned int sc_gen18;
    volatile unsigned int sc_gen19;
    volatile unsigned int sc_gen20;
    volatile unsigned int sc_gen21;
    volatile unsigned int sc_gen22;
    volatile unsigned int sc_gen23;
    volatile unsigned int sc_gen24;
    volatile unsigned int sc_gen25;
    volatile unsigned int sc_gen26;
    volatile unsigned int sc_gen27;
    volatile unsigned int sc_gen28;
    volatile unsigned int sc_gen29;
    volatile unsigned int sc_gen30;
    volatile unsigned int sc_gen31;
    volatile u_sc_sys_dbg0 sc_sys_dbg0;
    volatile unsigned int sc_locken;
    volatile unsigned int sc_sysid;
    volatile u_amp_mute amp_mute;
    volatile unsigned int sc_gen32;
    volatile unsigned int sc_gen33;
    volatile unsigned int sc_gen34;
    volatile unsigned int sc_gen35;
    volatile unsigned int sc_gen36;
    volatile unsigned int sc_gen37;
    volatile unsigned int sc_gen38;
    volatile unsigned int sc_gen39;
    volatile unsigned int sc_gen40;
    volatile unsigned int sc_gen41;
    volatile unsigned int sc_gen42;
    volatile unsigned int sc_gen43;
    volatile unsigned int sc_gen44;
    volatile unsigned int sc_gen45;
    volatile unsigned int sc_gen46;
    volatile unsigned int sc_gen47;
    volatile unsigned int sc_gen48;
    volatile unsigned int sc_gen49;
    volatile unsigned int sc_gen50;
    volatile unsigned int sc_gen51;
    volatile unsigned int sc_gen52;
    volatile unsigned int sc_gen53;
    volatile unsigned int sc_gen54;
    volatile unsigned int sc_gen55;
    volatile unsigned int sc_gen56;
    volatile unsigned int sc_gen57;
    volatile unsigned int sc_gen58;
    volatile unsigned int sc_gen59;
    volatile unsigned int sc_gen60;
    volatile unsigned int sc_gen61;
    volatile unsigned int sc_gen62;
    volatile unsigned int sc_gen63;
    volatile u_sc_2_p0_clkgate_srst_ctrl_addr sc_2_p0_clkgate_srst_ctrl_addr;
    volatile u_peri_ddc_edid_timeout peri_ddc_edid_timeout;
    volatile u_rng_rand_lock rng_rand_lock;
    volatile unsigned int rng_rand_d0;
    volatile unsigned int rng_rand_d1;
    volatile u_aon_peri_ctrl aon_peri_ctrl;
    volatile u_aon_mem_ctrl aon_mem_ctrl;
    volatile u_hdmirx_hpd_test hdmirx_hpd_test;
    volatile unsigned int xtal_io_ctrl;
} s_sysctrl_regs_type;

#endif /* __HI_REG_PERI_H__ */
