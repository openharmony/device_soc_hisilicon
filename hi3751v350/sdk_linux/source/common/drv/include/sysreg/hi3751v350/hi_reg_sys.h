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
        MCU????????ʱ??ѡ??
        00??24M
        01??100M
        10??100K
        11??????
 */
        unsigned int reserved_0 : 2; /* [1..0] */
        // Comment of field: reserved_1
        /*
        reserved
        MCU????????ʱ??״̬
        00??24M
        01??100M
        10??100K
        11??????
 */
        unsigned int reserved_1 : 2; /* [3..2] */
        // Comment of field: reserved_2
        /*
        reserved
        MCU????????ʱ?ӷ?Ƶѡ??
        00??mcu_bus_clk 1??Ƶ??
        01??mcu_bus_clk 2??Ƶ??
        10??mcu_bus_clk 3??Ƶ??
        11??mcu_bus_clk 4??Ƶ??
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
        POR ??λԴͷѡ??ָʾ?źţ?
        0??ѡ??Ƭ?⸴λԴ??
        1??ѡ??Ƭ??POR??λԴ??
 */
        unsigned int reserved_4 : 1; /* [10] */
        // Comment of field: reserved_5
        /*
        ???����?ʱ????0??дʱ??Ӱ?졣
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
        TVD or HDDEC??equatorģ????clampѡ????
        0??TVD??
        1??HDDEC??
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        EQUATOR DAC0ʱ?Ӻ?????ѡ???ź?
        2'b00:aifʱ?Ӻ?????ѡ???ź?
        2'b01:cvbs??ʱ?Ӻ?????ѡ???źţ?
        2'b10:??????
        2'b11:afe??ʱ?Ӻ?????ѡ???źš?
 */
        unsigned int reserved_1 : 2; /* [2..1] */
        // Comment of field: equator_agc_sel
        /*
        TVD or equator???ֲ??ָ?equatorģ????AGC
        0??TVD??
        1??EQUATOR???ֲ??֡?
 */
        unsigned int equator_agc_sel : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        equator_digital??clk_cvbs_dff??ʱ??ѡ??
        0: ѡ??ģ??IP??????clk_cvbsʱ??
        1??ѡ??clk_150m_occʱ??
 */
        unsigned int reserved_2 : 1; /* [4] */
        // Comment of field: reserved_3
        /*
        equator_digital??clk_cvbs_out??ʱ??ѡ??
        0: ѡ??ģ??IP??????clk_cvbs????2??Ƶ????ʱ??
        1??ѡ??clk_75m_occʱ??
 */
        unsigned int reserved_3 : 1; /* [5] */
        // Comment of field: reserved_4
        /*
        0: ѡ??ģ??IP??????clk_rgbʱ??
        1??ѡ??clk_150m_occʱ??
 */
        unsigned int reserved_4 : 1; /* [6] */
        unsigned int reserved_5 : 2; /* [8..7] */
        // Comment of field: reserved_6
        /*
        Equator_digital???���λ?????ź?
        0??????λ??
        1????λ??
 */
        unsigned int reserved_6 : 1; /* [9] */
        // Comment of field: reserved_7
        /*
        ??????
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
        ???? qamadcʱ??ѡ????2ѡһ??Ĭ??ѡ 54Mhz
        0??core_aif ??ʱ?? 108/2=54MHz??
        1??41.5MHz??
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ???? qamctrl ʱ??ѡ???? 2ѡһ??Ĭ??ѡcrg2??Ƶ
        0??41.5MHz??
        1??qam_adc phy????·ʱ?ӣ?
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        ???? qamadc??????ʱ??ѡ????
        0??QAMADCʱ??ͬ?ࣻ
        1??QAMADCʱ?ӷ??ࡣ
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        ???? qamadc phy ????·ʱ????????ʱ??ѡ????
        0??QAMADCʱ??ͬ?ࣻ
        1??QAMADCʱ?ӷ??ࡣ
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        ??????
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
        PLL?ڶ?????????Ƶϵ????
        
        pll_postdiv1 [26:24]
        PLL??һ????????Ƶϵ????
        
        pll_frac [23:0]:
        PLLС????Ƶϵ????
 */
        unsigned int reserved_0 : 31; /* [30..0] */
        // Comment of field: reserved_1
        /*
        ????
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
        PLL FOUTVCO2X????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
        
        bit[26] pll_bypass
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
        
        bit[25] pll_dsmpd
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
        
        bit[24] pll_dacpd
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
        
        bit[23] pll_foutpostdivpd
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
        
        bit[22] pll_fout4phasepd
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
        
        bit[21] pll_foutvcopd
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
        
        bit[20] pll_pd
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
        
        bit[19:18] ??????
        
        bit[17:12] pll_refdiv
        PLL?ο?ʱ?ӷ?Ƶϵ????
        
        bit[11:0] pll_fbdiv
        PLL??????Ƶϵ????
 */
        unsigned int reserved_0 : 28; /* [27..0] */
        // Comment of field: reserved_1
        /*
        ????
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
        ssmod divval[12??9]
        SSMOD divval???ơ?
        
        ssmod spread[8??4]
        SSMOD spread???ơ?
        0??0??
        1??0.1%??
        2??0.2%
        3??0.3%??
        4??0.4%??
        5??0.5%??
        6??0.6%??
        7??0.7%??
        ...
        31??3.1%
        
        ssmod downspread[3]
        SSMOD downspread???ơ?
        0???м?չƵ??
        1??????չƵ??
        
        ssmod_disable[2]
        SSMOD disable???ơ?
        0??enable??
        1??disable??
        
        ssmod_rst_req [1]
        SSMOD??λ???ơ?
        0??????λ??
        1????λ??
        
        ssmod_cken [0]
        SSMODʱ???ſ?????,Ĭ?Ϲرա?
        0???رգ?
        1???򿪡?
 */
        unsigned int reserved_0 : 13; /* [12..0] */
        // Comment of field: reserved_1
        /*
        ????
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
        ????
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
        ????
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
        ģʽ״ָ̬ʾ?ź?
        0????debug I2Cģʽ
        1??debug I2Cģʽ
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ????
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
           0         X         X     VOUT=0.9V??Ĭ?Ͽ?????LDO??????ѹ???ɵ??ڣ?
           1          1        X      ????
           1          0        0      ????
           1          0        1     VOUT=0.9V??LDO??????ѹ?ɵ??ڣ?Ĭ????????ѹΪ0.9V??
 */
        unsigned int fephy_en_ldo : 1; /* [2] */
        // Comment of field: fephy_patch_enable
        /*
        FEPHY??PATCH??ʹ???ź?
        0??ͨ???????˿?Ϊ?̶?ֵ0x0300
        1??ͨ???????˿?Ϊ???????õĲ???
 */
        unsigned int fephy_patch_enable : 1; /* [3] */
        // Comment of field: fephy_tclk_enable
        /*
        FEPHY??TCLK??ʹ???ź?
 */
        unsigned int fephy_tclk_enable : 1; /* [4] */
        // Comment of field: soft_fephy_gp_i
        /*
        FEPHY??MDIO???????ź?
 */
        unsigned int soft_fephy_gp_i : 16; /* [20..5] */
        // Comment of field: soft_fephy_mdio_i
        /*
        fephy mdio??????
 */
        unsigned int soft_fephy_mdio_i : 1; /* [21] */
        // Comment of field: soft_fephy_mdio_mdc
        /*
        fephy mdio??дʱ??
 */
        unsigned int soft_fephy_mdio_mdc : 1; /* [22] */
        // Comment of field: fephy_phy_addr
        /*
        FEPHY?ĵ?ַ?ź?
 */
        unsigned int fephy_phy_addr : 5; /* [27..23] */
        // Comment of field: afe_tclk_0_sel
        /*
        afe_tclk_0????4??Ƶѡ???ź?
        0?? ѡ??afe_tclk_0
        1?? ѡ??afe_tclk_0??4??Ƶ
 */
        unsigned int afe_tclk_0_sel : 1; /* [28] */
        // Comment of field: sc_fephy_iddq_mode
        /*
        MBIST?????ź?
        0:  ????bandgap??PLL??·
        1:  ?ر?bandgap??PLL??·
 */
        unsigned int sc_fephy_iddq_mode : 1; /* [29] */
        // Comment of field: reserved_0
        /*
        ????
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
        ?ܽ?STANDBY_PWROFF???ơ?
        0???ܽ?STANDBY_PWROFF????Ϊ?͵?ƽ?????Ƶ??????µ磻
        1???ܽ?STANDBY_PWROFF????Ϊ?ߵ?ƽ?????Ƶ????????µ硣
 */
        unsigned int stb_poweroff : 1; /* [1] */
        // Comment of field: mcu_lp_subsys_iso
        /*
        ?????????ơ?
        4'b1011??ʹ?ܣ??????źŽ??и??롣
        ????????ֹ?????????źŽ??и??룻
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
        ?????????ơ?
        4'b1011??ʹ?ܣ??????źŽ??и??롣
        ????????ֹ?????????źŽ??и??룻
 */
        unsigned int pwd_signals_in_mcu_iso : 4; /* [12..9] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 15; /* [27..13] */
        // Comment of field: core0p9v_stable_iso
        /*
        ???ݣ?
        ????ʹ?ܿ????źš???????Ч??
        ????????0.9V??Դ?ȶ??󣬲????ļĴ???״̬?źţ??ܸø???ʹ?ܿ??ơ?
        1??ʹ?ܣ??????źŽ??и??롣
        0????ֹ?????????źŽ??и??룻
 */
        unsigned int core0p9v_stable_iso : 1; /* [28] */
        // Comment of field: por_0p9v_rst_iso_status
        /*
        debug?Ĵ?????por0p9v?źŵĸ???״̬??????Ч??
 */
        unsigned int por_0p9v_rst_iso_status : 1; /* [29] */
        // Comment of field: pwd_signals_in_mcu_iso_status
        /*
        ?µ??�����?????źŵĸ???״̬??????Ч??
 */
        unsigned int pwd_signals_in_mcu_iso_status : 1; /* [30] */
        // Comment of field: mcu_lp_subsys_iso_status
        /*
        ?µ????͸??????????źŵĸ???״̬??????Ч??
        ??sc_locken???ơ?
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
        HDMI RX PWR5V[2:0]?????ܽ??????ɿأ?
        Bit0:??ʾѡ??HDMIRX 2P0 PORT0??
        Bit1:??ʾѡ??HDMIRX 2P0 PORT1??
        Bit2:??ʾѡ??HDMIRX 2P0 PORT2??
        Bit3:reserved
        ???ж?Ӧbit??ʾ????:
        0????????
        1????????
 */
        unsigned int hdmi_rx_pwr_pctrl : 4; /* [3..0] */
        // Comment of field: hdmi_rx_hpd_pctrl
        /*
        HDMI RX HPD[2:0]?????ܽ??????ɿأ?
        Bit4:??ʾѡ??HDMIRX 2P0 PORT0??
        Bit5:??ʾѡ??HDMIRX 2P0 PORT1??
        Bit6:??ʾѡ??HDMIRX 2P0 PORT2??
        Bit7:reserved
        ???ж?Ӧbit??ʾ????:
        0????????
        1????????
 */
        unsigned int hdmi_rx_hpd_pctrl : 4; /* [7..4] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 4; /* [11..8] */
        // Comment of field: hdmi_rx_5v_det_mux
        /*
        ??ʾѡ??????detect ѡ???????ź?
        Bit12:??ʾѡ??HDMIRX 2P0 PORT0??
        Bit13:??ʾѡ??HDMIRX 2P0 PORT1??
        Bit14:??ʾѡ??HDMIRX 2P0 PORT2??
        Bit15:reserved
 */
        unsigned int hdmi_rx_5v_det_mux : 4; /* [15..12] */
        // Comment of field: hdmi_rx_5v_det_reg
        /*
        ??ʾϵͳ??????????��?Ĵ??????ѼĴ?????̽???źš?
        Bit16:??ʾѡ??HDMIRX 2P0 PORT0??
        Bit17:??ʾѡ??HDMIRX 2P0 PORT1??
        Bit18:??ʾѡ??HDMIRX 2P0 PORT2??
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
        MCEʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        MCE?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        pwmʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        pwm?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        IRʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        IRģ???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        lsadcʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_6 : 1; /* [6] */
        // Comment of field: reserved_7
        /*
        lsadcģ???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_7 : 1; /* [7] */
        // Comment of field: reserved_8
        /*
        ??????
 */
        unsigned int reserved_8 : 1; /* [8] */
        // Comment of field: reserved_9
        /*
        ??????
 */
        unsigned int reserved_9 : 1; /* [9] */
        // Comment of field: reserved_10
        /*
        timer01ʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_10 : 1; /* [10] */
        // Comment of field: reserved_11
        /*
        timer01ģ???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_11 : 1; /* [11] */
        // Comment of field: reserved_12
        /*
        UART1ʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_12 : 1; /* [12] */
        // Comment of field: reserved_13
        /*
        UART1?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_13 : 1; /* [13] */
        // Comment of field: reserved_14
        /*
        hdmirx_osc?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_14 : 1; /* [14] */
        // Comment of field: reserved_15
        /*
        hdmirx_apb?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_15 : 1; /* [15] */
        // Comment of field: reserved_16
        /*
        hdmirx_cec?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_16 : 1; /* [16] */
        // Comment of field: reserved_17
        /*
        hdmirx?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_17 : 1; /* [17] */
        // Comment of field: reserved_18
        /*
        mosdc_clkʱ??Դͷѡ????
        0:24MHz????ʱ?ӣ?
        1:20MHz
 */
        unsigned int reserved_18 : 1; /* [18] */
        // Comment of field: reserved_19
        /*
        cec_clkʱ??Դͷѡ????
        0:2MHz??????ʱ??12??Ƶ????
        1:2.02MHz
 */
        unsigned int reserved_19 : 1; /* [19] */
        // Comment of field: reserved_20
        /*
        HDMI1.4 RX coreʱ??Դͷѡ????
        0:24MHz????ʱ?ӣ?
        1:27MHz
 */
        unsigned int reserved_20 : 1; /* [20] */
        // Comment of field: reserved_21
        /*
        cbus_clkʱ??Դͷѡ????
        0:24MHz????ʱ?ӣ?
        1:20MHz
 */
        unsigned int reserved_21 : 1; /* [21] */
        // Comment of field: reserved_22
        /*
        hdmirx_moscʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_22 : 1; /* [22] */
        // Comment of field: reserved_23
        /*
        hdmirx_oscʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_23 : 1; /* [23] */
        // Comment of field: reserved_24
        /*
        hdmirx_apbʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_24 : 1; /* [24] */
        // Comment of field: reserved_25
        /*
        hdmirx_cecʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_25 : 1; /* [25] */
        // Comment of field: reserved_26
        /*
        hdmirx_mpllrefʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_26 : 1; /* [26] */
        // Comment of field: reserved_27
        /*
        hdmirx_mpllref_pllʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_27 : 1; /* [27] */
        // Comment of field: pd_rst_req
        /*
        ϵͳ?µ縴λ??????
        0??????λ??
        1????λ??
 */
        unsigned int pd_rst_req : 1; /* [28] */
        // Comment of field: reserved_28
        /*
        hdmirx_edidʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_28 : 1; /* [29] */
        // Comment of field: reserved_29
        /*
        hdmirx_cbusʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_29 : 1; /* [30] */
        // Comment of field: reserved_30
        /*
        hdmirx edid??ѡ???źţ?
        0??2Mʱ??ѡ??
        1????ʾ2.02Mʱ??ѡ????
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
        ??????
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
        ????????WDG_RST?ź?Դͷ??
        0??WDG0_RST????
        1??TIMER01?жϣ?
        ????ȫ???���λ???ơ?
 */
        unsigned int wdg_rst_ctrl : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [1] */
        // Comment of field: reserved_1
        /*
        ??????
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
        ??????
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
        ddrphy0 retention ???ܿ??ƼĴ?????
        ????ȫ???���λ???ơ?
 */
        unsigned int ddrphy_lp_en : 2; /* [1..0] */
        // Comment of field: ddrphy_lp_en1
        /*
        reserved
 */
        unsigned int ddrphy_lp_en1 : 2; /* [3..2] */
        // Comment of field: reserved_0
        /*
        ??????
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
        ??????
 */
        unsigned int reserved_1 : 5; /* [5..1] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 1; /* [6] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        ????
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
        ??????
 */
        unsigned int reserved_0 : 10; /* [9..0] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [10] */
        // Comment of field: reserved_2
        /*
        ????
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
        mcu_ldo_vset????ֵ??
        0000??????0.74V
        0001??????0.76V
        0010??????0.78V
        0011??????0.80V
        0100??????0.82V
        0101??????0.84V
        0110??????0.86V
        0111??????0.88V
        1000??????0.90V??Ĭ?ϣ?
        1001??????0.92V
        1010??????0.94V
        1011??????0.96V
        1100??????0.98V
        1101??????1.00V
        1110??????1.02V
        1111??????1.04V
        ????ȫ???���λ???ơ?
 */
        unsigned int mcu_ldo_vset : 4; /* [3..0] */
        // Comment of field: mcu_ldo_fuse
        /*
        ?????ڲ???ȷ??ѹ??׼:
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
        1111??+ 68mV
        ????ȫ???���λ???ơ?
 */
        unsigned int mcu_ldo_fuse : 4; /* [7..4] */
        // Comment of field: reserved_0
        /*
        ????
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
        ????
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
        ????
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
        ?弶??ƵAMP MUTE???ܿ????źţ?
        0???ر?MUTE????
        1??????MUTE????
 */
        unsigned int amp_mute_out : 1; /* [0] */
        // Comment of field: amp_mute_oen
        /*
        ?弶??ƵAMP MUTE????ʹ?ܣ?
        0??????ʹ?ܣ?оƬ????mute???ƣ?
        1???弶mute??·????
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
        cbusʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        cecʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        edidʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        oscʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        prifʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        shemanʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        aphy_p0ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_6 : 1; /* [6] */
        // Comment of field: reserved_7
        /*
        aphy_p1ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_7 : 1; /* [7] */
        // Comment of field: reserved_8
        /*
        aphy_p2ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_8 : 1; /* [8] */
        // Comment of field: reserved_9
        /*
        fephyʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_9 : 1; /* [9] */
        // Comment of field: reserved_10
        /*
        usb2 phy?Ĳο?ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_10 : 1; /* [10] */
        // Comment of field: reserved_11
        /*
        cec ʱ??ѡ????
        0??2M??
        1??2.02M??
 */
        unsigned int reserved_11 : 1; /* [11] */
        // Comment of field: reserved_12
        /*
        cbusʱ??ѡ????
        0??24M??
        1??20M??
 */
        unsigned int reserved_12 : 1; /* [12] */
        // Comment of field: reserved_13
        /*
        edid ʱ??ѡ????
        0??2M??
        1??2.02M??
 */
        unsigned int reserved_13 : 1; /* [13] */
        // Comment of field: reserved_14
        /*
        shemanʱ??ѡ????
        0??24M??
        1??27M??
 */
        unsigned int reserved_14 : 1; /* [14] */
        // Comment of field: reserved_15
        /*
        aphy_p0ʱ??ѡ????
        0??ѡ??hdmi_phy_p0??????ʱ??(AD_CLKO_DVI_DVI32_P0)
        1??ѡ??DFTʱ??
 */
        unsigned int reserved_15 : 1; /* [15] */
        // Comment of field: reserved_16
        /*
        aphy_p1ʱ??ѡ????
        0??ѡ??hdmi_phy_p1??????ʱ??(AD_CLKO_DVI_DVI32_P1)
        1??ѡ??DFTʱ??
 */
        unsigned int reserved_16 : 1; /* [16] */
        // Comment of field: reserved_17
        /*
        aphy_p2ʱ??ѡ????
        0??ѡ??hdmi_phy_p2??????ʱ??(AD_CLKO_DVI_DVI32_P2)
        1??ѡ??DFTʱ??
 */
        unsigned int reserved_17 : 1; /* [17] */
        // Comment of field: reserved_18
        /*
        cec?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_18 : 1; /* [18] */
        // Comment of field: reserved_19
        /*
        osc?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_19 : 1; /* [19] */
        // Comment of field: reserved_20
        /*
        prif?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_20 : 1; /* [20] */
        // Comment of field: reserved_21
        /*
        sheman?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int reserved_21 : 1; /* [21] */
        // Comment of field: reserved_22
        /*
        aphy_p0?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int reserved_22 : 1; /* [22] */
        // Comment of field: reserved_23
        /*
        aphy_p1?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int reserved_23 : 1; /* [23] */
        // Comment of field: reserved_24
        /*
        aphy_p2?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int reserved_24 : 1; /* [24] */
        // Comment of field: reserved_25
        /*
        fephy???���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int reserved_25 : 1; /* [25] */
        // Comment of field: reserved_26
        /*
        usb2 phy0_p1???���λ?????źţ????ڸ?λUTM״̬??
        0??????λ??
        1????λ??
 */
        unsigned int reserved_26 : 1; /* [26] */
        // Comment of field: reserved_27
        /*
        usb2 phy0_p1???���λ?????źţ?????usb2_phy0_p1???ϵ縴λ
        0??????λ??
        1????λ??
 */
        unsigned int reserved_27 : 1; /* [27] */
        // Comment of field: reserved_28
        /*
        ???ڲ???usb2_phy0_p1???���λ?????ź?
        0??????λ??
        1????λ??
 */
        unsigned int reserved_28 : 1; /* [28] */
        // Comment of field: reserved_29
        /*
        usb2_phy0_p1??apb_clk??ʱ???ſ?ʹ??
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_29 : 1; /* [29] */
        // Comment of field: reserved_30
        /*
        ??????
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
        VGA??debug?Ĵ?????
 */
        unsigned int vga_ddc_timeout : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: vga_ddc_sda_del_en
        /*
        VGA??sda·????ʱʹ?ܡ?
        0????ʹ??;
        1??ʹ?ܡ?
 */
        unsigned int vga_ddc_sda_del_en : 1; /* [4] */
        // Comment of field: vga_reg_ddc_del_en
        /*
        VGA??scl·????ʱʹ?ܡ?
        0????ʹ??;
        1??ʹ?ܡ?
 */
        unsigned int vga_reg_ddc_del_en : 1; /* [5] */
        // Comment of field: vga_reg_ddc_en
        /*
        VGA??ddc_edidʹ?ܡ?
        0????ʹ??;
        1??ʹ?ܡ?
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
        ??????
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
        vga??scl_in??sda_in?????ܽ?ѡ???źţ?
        0??��?Գ??????ܽţ?
        1??reserved??
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: uart1_use_lbe_txd_en
        /*
        uart txuart?????ݾͷ??????ݵ?txdʹ?ܣ?
        1????ʹ?ܣ?
        0??ʹ?ܣ?
 */
        unsigned int uart1_use_lbe_txd_en : 1; /* [1] */
        // Comment of field: sc_uart0_sel
        /*
        uart0 IOѡ????
        0??uart  rxd
        1??uart rxd cp
 */
        unsigned int sc_uart0_sel : 1; /* [2] */
        // Comment of field: sc_uart1_sel
        /*
        uart1 IOѡ????
        0??uart rxd
        1??uart rxd cp
 */
        unsigned int sc_uart1_sel : 1; /* [3] */
        // Comment of field: sc_jtag_dbg_release
        /*
        ?˳?2??JTAG???ԡ?
        ????Ϊ1??????Ϊ0???˳?2??JTAG???ԡ?
        ??????֧??2??JTAG???ԡ?
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
        uart1???????ݵ?odѡ???źš?
        0:CMOS??????
        1:OD??????
 */
        unsigned int peri_uart1_od_sel : 1; /* [7] */
        // Comment of field: hdmirx_det_io_sel
        /*
        hdmirx detect ioѡ????
        0:HDMIRX DETѡ????5V IO??
        1:HDMIRX DETѡ????ͨ????IO??
 */
        unsigned int hdmirx_det_io_sel : 1; /* [8] */
        // Comment of field: backup_edid_sel
        /*
        UART??VGA EDID IO?Զ??л?ʹ?ܿ??ơ?
        0:UART??VGA_EDID IO?Զ??л???
        1:UART??VGA_EDID IO???Զ??л???ʹ????ͨ?ĹܽŸ??á?
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
        memory?????źţ?????BES memory????˵??
 */
        unsigned int ir_rft_emasa : 1; /* [0] */
        // Comment of field: ir_rft_emab
        /*
        memory?????źţ?????BES memory????˵??
 */
        unsigned int ir_rft_emab : 3; /* [3..1] */
        // Comment of field: ir_rft_emaa
        /*
        memory?????źţ?????BES memory????˵??
 */
        unsigned int ir_rft_emaa : 3; /* [6..4] */
        // Comment of field: ir_rft_colldisn
        /*
        memory?????źţ?????BES memory????˵??
 */
        unsigned int ir_rft_colldisn : 1; /* [7] */
        // Comment of field: mce_rfs_emaw
        /*
        ??????mce??fephy memory?????źţ?????BES memory????˵??
 */
        unsigned int mce_rfs_emaw : 2; /* [9..8] */
        // Comment of field: mce_rfs_ema
        /*
        ??????mce??fephy memory?????źţ?????BES memory????˵??
 */
        unsigned int mce_rfs_ema : 3; /* [12..10] */
        // Comment of field: mce_ras_emaw
        /*
        ??????mce??fephy memory?????źţ?????BES memory????˵??
 */
        unsigned int mce_ras_emaw : 2; /* [14..13] */
        // Comment of field: mce_ras_ema
        /*
        ??????mce??fephy memory?????źţ?????BES memory????˵??
 */
        unsigned int mce_ras_ema : 3; /* [17..15] */
        // Comment of field: mce_ras_wabl
        /*
        ??????mce??fephy memory?????źţ?????BES memory????˵??
 */
        unsigned int mce_ras_wabl : 1; /* [18] */
        // Comment of field: mce_ras_stov
        /*
        ??????mce??fephy memory?????źţ?????BES memory????˵??
 */
        unsigned int mce_ras_stov : 1; /* [19] */
        // Comment of field: mce_ras_emas
        /*
        ??????mce??fephy memory?????źţ?????BES memory????˵??
 */
        unsigned int mce_ras_emas : 1; /* [20] */
        // Comment of field: mce_ras_wablm
        /*
        ??????mce??fephy memory?????źţ?????BES memory????˵??
 */
        unsigned int mce_ras_wablm : 2; /* [22..21] */
        // Comment of field: mce_rfs_wabl
        /*
        ??????mce??fephy memory?????źţ?????BES memory????˵??
 */
        unsigned int mce_rfs_wabl : 1; /* [23] */
        // Comment of field: mce_rfs_wablm
        /*
        ??????mce??fephy memory?????źţ?????BES memory????˵??
 */
        unsigned int mce_rfs_wablm : 2; /* [25..24] */
        // Comment of field: fephy_ros_ema
        /*
        memory?????źţ?????BES memory????˵??
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
        hdmirx 5v new io???ԼĴ???
 */
        unsigned int hpd5v_io_0_test : 8; /* [7..0] */
        // Comment of field: hpd5v_io_1_test
        /*
        hdmirx 5v new io???ԼĴ???
 */
        unsigned int hpd5v_io_1_test : 8; /* [15..8] */
        // Comment of field: hpd5v_io_2_test
        /*
        hdmirx 5v new io???ԼĴ???
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
