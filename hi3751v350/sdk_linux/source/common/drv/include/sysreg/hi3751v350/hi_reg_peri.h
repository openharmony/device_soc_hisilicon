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

#ifndef __HI_REG_PERI_H__
#define __HI_REG_PERI_H__

/* Define the union U_START_MODE */
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
        reserved
 */
        unsigned int reserved_2 : 1; /* [8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 2; /* [10..9] */
        // Comment of field: reserved_4
        /*
        sfc_addr_mode 启动SFC地址模式选择。
        0： 3byte地址模式；
        1： 4byte地址模式。
 */
        unsigned int reserved_4 : 1; /* [11] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 8; /* [19..12] */
        // Comment of field: romboot_sel
        /*
        芯片启动方式指示。
 */
        unsigned int romboot_sel : 1; /* [20] */
        // Comment of field: reserved_6
        /*
        连接安全的使能信号chipset_info[7]
 */
        unsigned int reserved_6 : 1; /* [21] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [22] */
        // Comment of field: reserved_8
        /*
        管脚 JTAG_SEL 状态指示信号
        1：使用从管脚过来的jtag_mux选择信号；
        0：使用peri_ctrl配置过来jtag_mux选择信号。
 */
        unsigned int reserved_8 : 1; /* [23] */
        // Comment of field: reserved_9
        /*
        JTAG_MUX选择jtag状态指示信号
        3'b000：选中ARM JTAG；
        3'b010：选中DSP JTAG；
        3'b100：reserved
        3'b110：reserved
 */
        unsigned int reserved_9 : 3; /* [26..24] */
        // Comment of field: reserved_10
        /*
        reserved
 */
        unsigned int reserved_10 : 1; /* [27] */
        // Comment of field: reserved_11
        /*
        保留。
 */
        unsigned int reserved_11 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_start_mode;

/* Define the union U_PERI_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 2; /* [2..1] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 29; /* [31..3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stat;

/* Define the union U_PERI_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        保留。
        SDIO0(SD卡）卡检测信号极性控制。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 8; /* [9..2] */
        // Comment of field: reserved_3
        /*
        保留
 */
        unsigned int reserved_3 : 1; /* [10] */
        // Comment of field: reserved_4
        /*
        保留。
 */
        unsigned int reserved_4 : 2; /* [12..11] */
        // Comment of field: ld_en_out
        /*
        reserved
 */
        unsigned int ld_en_out : 1; /* [13] */
        // Comment of field: en_3d_out
        /*
        reserved
 */
        unsigned int en_3d_out : 1; /* [14] */
        // Comment of field: scn_en_out
        /*
        reserved
 */
        unsigned int scn_en_out : 1; /* [15] */
        // Comment of field: reserved_5
        /*
        保留。
        背光控制信号，极性可配，支持CMOS和OD可选。
        0:开背光；
        1：关背光。
 */
        unsigned int reserved_5 : 1; /* [16] */
        // Comment of field: reserved_6
        /*
        保留
 */
        unsigned int reserved_6 : 1; /* [17] */
        // Comment of field: reserved_7
        /*
        保留
 */
        unsigned int reserved_7 : 1; /* [18] */
        // Comment of field: reserved_8
        /*
        保留
 */
        unsigned int reserved_8 : 10; /* [28..19] */
        // Comment of field: peri_jtagsel
        /*
        FUNC_SEL为0时，JTAG接口选择信号。
 */
        unsigned int peri_jtagsel : 3; /* [31..29] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_ctrl;

/* Define the union U_CPU_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        A53 MP Core0 SMP、AMP指示信号。
        0： SMP模式；
        1： AMP模式。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        A53 MP Core1 SMP、AMP指示信号。
        0： SMP模式；
        1： AMP模式。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        core2_smpnamp
        A53 MP Core2 SMP、AMP指示信号。
        0： SMP模式；
        1： AMP模式。
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        core3_smpnamp
        A53 MP Core3 SMP、AMP指示信号。
        0： SMP模式；
        1： AMP模式。
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        保留。
 */
        unsigned int reserved_5 : 27; /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cpu_stat;

/* Define the union U_PERI_SEC_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留
 */
        unsigned int reserved_0 : 4; /* [3..0] */
        // Comment of field: reserved_1
        /*
        保留
 */
        unsigned int reserved_1 : 4; /* [7..4] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_sec_stat;

/* Define the union U_PERI_EMMC_LP */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
        EMMC IO隔离模式使能
        0：normal mode
        1：isolation mode
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        保留。
        EMMC 对应IO是否进入retention模式
        0：normal mode
        1：isolation mode
 */
        unsigned int reserved_1 : 6; /* [6..1] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 25; /* [31..7] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_emmc_lp;

/* Define the union U_PERI_USB_RESUME_INT_MASK */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: usb_suspend_int_mask
        /*
        USB PHY唤醒中断mask控制位
        bit3: reserved
        bit2：USB2  PHY2 ；
        bit1：USB2  PHY1 ；
        bit0：USB2  PHY0。
        0 mask中断，1不mask。
 */
        unsigned int usb_suspend_int_mask : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_usb_resume_int_mask;

/* Define the union U_PERI_USB_RESUME_INT_RAWSTAT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: usb_suspend_int_rawstat
        /*
        USB PHY suspend中断原始状态寄存器
        bit3: reserved
        bit2：USB2  PHY2 ；
        bit1：USB2  PHY1 ；
        bit0：USB2  PHY0。
 */
        unsigned int usb_suspend_int_rawstat : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_usb_resume_int_rawstat;

/* Define the union U_PERI_USB_RESUME_INT_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: usb_suspend_int_stat
        /*
        USB PHY suspend中断状态寄存器
        bit3: reserved
        bit2：USB2  PHY2 ；
        bit1：USB2  PHY1 ；
        bit0：USB2  PHY0。
 */
        unsigned int usb_suspend_int_stat : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_usb_resume_int_stat;

/* Define the union U_PERI_INT_MCE */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: int_swi_cputomce
        /*
        mce的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int int_swi_cputomce : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_int_mce;

/* Define the union U_PERI_INT_DSP */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        DSP的中断源，是否为VDH测试使用的场景。
        0：中断源是给音频DSP用；
        1：中断源是给VDH DSP测试用。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        DSP的中断源，是否为VDH测试使用的场景。
        0：中断源是给音频DSP用；
        1：中断源是给VDH DSP测试用。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: int_swi0_dsp0
        /*
        dsp的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int int_swi0_dsp0 : 1; /* [2] */
        // Comment of field: int_swi1_dsp0
        /*
        dsp的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int int_swi1_dsp0 : 1; /* [3] */
        // Comment of field: int_swi2_dsp0
        /*
        dsp的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int int_swi2_dsp0 : 1; /* [4] */
        // Comment of field: int_swi3_dsp0
        /*
        dsp的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int int_swi3_dsp0 : 1; /* [5] */
        // Comment of field: reserved_2
        /*
        dsp的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int reserved_2 : 1; /* [6] */
        // Comment of field: reserved_3
        /*
        dsp的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        dsp的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int reserved_4 : 1; /* [8] */
        // Comment of field: reserved_5
        /*
        dsp的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int reserved_5 : 1; /* [9] */
        // Comment of field: reserved_6
        /*
        保留。
 */
        unsigned int reserved_6 : 22; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_int_dsp;

/* Define the union U_PERI_INT_DSP1TOA9 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        dsp到cpu的中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        dsp到cpu的中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_int_dsp1_toa9;

/* Define the union U_PERI_INT_SWI0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: int_swi0_cpu
        /*
        cpu的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int int_swi0_cpu : 1; /* [0] */
        // Comment of field: int_swi1_cpu
        /*
        cpu的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int int_swi1_cpu : 1; /* [1] */
        // Comment of field: int_swi2_cpu
        /*
        cpu的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int int_swi2_cpu : 1; /* [2] */
        // Comment of field: int_swi3_cpu
        /*
        cpu的软中断。
        0：清中断；
        1：发中断请求。
 */
        unsigned int int_swi3_cpu : 1; /* [3] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_int_swi0;

/* Define the union U_PERI_INT_SWI1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        a73的swi1中断。
        0：清中断；
        1：发中断请求。
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
} u_peri_int_swi1;

/* Define the union U_PERI_INT_SWI2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        a73的swi2中断。
        0：清中断；
        1：发中断请求。
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
} u_peri_int_swi2;

/* Define the union U_PERI_QAM_ADC0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        工作模式：
        opm[1：0]=00 ，关电状态；
        opm[1：0]=01或10 ， 低功耗模式；
        opm[1：0]=11， 正常工作模式。
 */
        unsigned int reserved_0 : 2; /* [1..0] */
        // Comment of field: reserved_1
        /*
        se_prev_f=0 ，前向校准在ADC上电时启动；
        use_prev_f=1 ，前向校准不启动，沿用上次校准值 。
 */
        unsigned int reserved_1 : 1; /* [2] */
        // Comment of field: reserved_2
        /*
        PING PONG模式使能，高电平有效。
 */
        unsigned int reserved_2 : 1; /* [3] */
        // Comment of field: reserved_3
        /*
        输出时序格式。
        0：数据从I路输出（逻辑内部已经接死为I路输出）；
        1：数据从IQ两路同时输出（保留）。
 */
        unsigned int reserved_3 : 1; /* [4] */
        // Comment of field: reserved_4
        /*
        占空比控制使能，高电平有效。（须保持关闭状态）
 */
        unsigned int reserved_4 : 1; /* [5] */
        // Comment of field: reserved_5
        /*
        偏置控制字。
 */
        unsigned int reserved_5 : 5; /* [10..6] */
        // Comment of field: reserved_6
        /*
        保留输出数据使能：
        endout=1 ?输出0；
        endout=0?输出正常数据。
 */
        unsigned int reserved_6 : 1; /* [11] */
        // Comment of field: reserved_7
        /*
        输出格式选择：
        selof=0? 二进制；
        selof=1?二进制补码 。
 */
        unsigned int reserved_7 : 1; /* [12] */
        // Comment of field: peri_qamadc_startcal
        /*
        出发校准周期，高电平有效。
 */
        unsigned int peri_qamadc_startcal : 1; /* [13] */
        // Comment of field: reserved_8
        /*
        peri_qamadc_bcal
        后向校准行为控制字。
        bcal[1：0]=00 ?正常校准模式；
        bcal[1：0]=01 ?校准工作停止，仍产生校准信号 。
        bcal[1：0]=10 ?非法；
        bcal[1：0]=11 ?校准工作停止，不产生校准信号 。
 */
        unsigned int reserved_8 : 2; /* [15..14] */
        // Comment of field: reserved_9
        /*
        peri_qamadc_fsctrl
        fsctrl 数据输出端的数字增益控制。增益在1x~1.996x之间单调256步可调。
        fsctrl[7:0]=0x00 ： 1x ；
        fsctrl[7:0]=0xFF ： 1.996x 。
 */
        unsigned int reserved_9 : 8; /* [23..16] */
        // Comment of field: peri_qamadc_adcrdy
        /*
        ADC ready信号，高电平表示ADC可以开始正常工作。
 */
        unsigned int peri_qamadc_adcrdy : 1; /* [24] */
        // Comment of field: reserved_10
        /*
        peri_qamadc_bitosync
        在ppsel=1及chsel=0时使用，用以标示当前的bit所属的通道。
        0： 当前I路输出；
        1：当前Q路输出。
 */
        unsigned int reserved_10 : 1; /* [25] */
        // Comment of field: reserved_11
        /*
        peri_qamadc_rfalgi
        i路数据饱和指示。
 */
        unsigned int reserved_11 : 3; /* [28..26] */
        // Comment of field: reserved_12
        /*
        peri_qamadc_rfalgq
        q路数据饱和指示。
 */
        unsigned int reserved_12 : 3; /* [31..29] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qam_adc0;

/* Define the union U_PERI_QAM_ADC1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_qamadc_i2c_devaddr
        /*
        QAM ADC设备地址。
 */
        unsigned int peri_qamadc_i2c_devaddr : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        保留内部偏置使能。
        0：不使能；
        1：使能。
 */
        unsigned int reserved_0 : 1; /* [7] */
        // Comment of field: peri_qamadc_i2c_rst_n
        /*
        内部数字电路reset，低电平有效
 */
        unsigned int peri_qamadc_i2c_rst_n : 1; /* [8] */
        // Comment of field: reserved_1
        /*
        QAM ADC debug寄存器。
        QAMADC 1CH: 
        bit9:envcmout
        bit10:enaccoupling
        QAMADC 2CH：
        bit11:envcmout i通道
        bit12:enaccoupling i通道
        bit13:envcmout q通道
        bit14:enaccoupling q通道
        详见QAMADC datasheet
 */
        unsigned int reserved_1 : 6; /* [14..9] */
        unsigned int reserved_2 : 1; /* [15] */
        // Comment of field: peri_qamadc_pd
        /*
        详见模拟IP手册
 */
        unsigned int peri_qamadc_pd : 1; /* [16] */
        // Comment of field: peri_qamadc_rst_n
        /*
        详见模拟IP手册
 */
        unsigned int peri_qamadc_rst_n : 1; /* [17] */
        // Comment of field: peri_qamadc_mode_sel
        /*
        详见模拟IP手册
 */
        unsigned int peri_qamadc_mode_sel : 3; /* [20..18] */
        // Comment of field: reserved_3
        /*
        保留。
 */
        unsigned int reserved_3 : 11; /* [31..21] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qam_adc1;

/* Define the union U_PERI_QAM_ADC2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        QAM ADC设备地址。
 */
        unsigned int reserved_0 : 7; /* [6..0] */
        // Comment of field: reserved_1
        /*
        QAM ADC设备地址。
 */
        unsigned int reserved_1 : 7; /* [13..7] */
        // Comment of field: reserved_2
        /*
        内部数字电路reset，低电平有效
 */
        unsigned int reserved_2 : 1; /* [14] */
        // Comment of field: reserved_3
        /*
        内部数字电路reset，低电平有效
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: reserved_4
        /*
        ADC ready信号，高电平表示ADC可以开始正常工作。
 */
        unsigned int reserved_4 : 1; /* [16] */
        // Comment of field: reserved_5
        /*
        ADC ready信号，高电平表示ADC可以开始正常工作。
 */
        unsigned int reserved_5 : 1; /* [17] */
        // Comment of field: reserved_6
        /*
        工作模式：
        opm[1：0]=00 ，关电状态；
        opm[1：0]=01或10 ， 低功耗模式；
        opm[1：0]=11， 正常工作模式。
 */
        unsigned int reserved_6 : 2; /* [19..18] */
        // Comment of field: reserved_7
        /*
        工作模式：
        opm[1：0]=00 ，关电状态；
        opm[1：0]=01或10 ， 低功耗模式；
        opm[1：0]=11， 正常工作模式。
 */
        unsigned int reserved_7 : 2; /* [21..20] */
        // Comment of field: reserved_8
        /*
        输出格式选择：
        selof=0? 二进制；
        selof=1?二进制补码 。
 */
        unsigned int reserved_8 : 1; /* [22] */
        // Comment of field: reserved_9
        /*
        输出格式选择：
        selof=0? 二进制；
        selof=1?二进制补码 。
 */
        unsigned int reserved_9 : 1; /* [23] */
        // Comment of field: reserved_10
        /*
        出发校准周期，高电平有效。
 */
        unsigned int reserved_10 : 1; /* [24] */
        // Comment of field: reserved_11
        /*
        出发校准周期，高电平有效。
 */
        unsigned int reserved_11 : 1; /* [25] */
        // Comment of field: reserved_12
        /*
        I2C_QAMADC的sda_din信号源头选择：
        2'b0:qamadc_1ch；
        2'b1:qamadc_2chi；
        else:qamadc_2chq；
 */
        unsigned int reserved_12 : 2; /* [27..26] */
        // Comment of field: reserved_13
        /*
        保留。
 */
        unsigned int reserved_13 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qam_adc2;

/* Define the union U_PERI_SYS_PLL_LDO */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_sys_pll_ldo;
/* Define the union U_PERI_USB2_REG1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_usb2_dig
        /*
        详见USB2 sheet
 */
        unsigned int peri_usb2_dig : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_usb2_reg1;
/* Define the union U_PERI_USB2_REG0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_usb2_phy
        /*
        详见USB2 sheet
 */
        unsigned int peri_usb2_phy : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_usb2_reg0;
/* Define the union U_UART_DBG_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: uart0_use_lbe_txd_en
        /*
        uart txuart有数据就发送数据到txd使能，
        1：不使能；
        0：使能；
 */
        unsigned int uart0_use_lbe_txd_en : 1; /* [0] */
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
} u_uart_dbg_cfg;

/* Define the union U_DSP_CTRL_COMMON */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        dsp0和dsp1联合调试的方式
        0：xocdmode与操作。这是xtensa给的连接方式
        1：xocdmode或操作
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        dsp0和dsp1联合调试的方式
        0：breakout与操作。这是xtensa给的连接方式
        1：breakout或操作
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 13; /* [15..3] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 2; /* [17..16] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 2; /* [19..18] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 2; /* [21..20] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 2; /* [23..22] */
        // Comment of field: reserved_8
        /*
        保留。
 */
        unsigned int reserved_8 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dsp_ctrl_common;

/* Define the union U_AIAO_DSP_CFG_COM */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        DSP预留配置
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_aiao_dsp_cfg_com;
/* Define the union U_DSP_CTRL1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 5; /* [4..0] */
        // Comment of field: reserved_1
        /*
        DSP MFDE MMU START信号
 */
        unsigned int reserved_1 : 1; /* [5] */
        // Comment of field: reserved_2
        /*
        手动配置dsp0 runstall信号。如果用户需要手动配置runstall信号，需要配置dsp0_runstall_en=0
 */
        unsigned int reserved_2 : 1; /* [6] */
        // Comment of field: reserved_3
        /*
        DSP是否接受breakin信号
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        DSP是否接受runstall信号
 */
        unsigned int reserved_4 : 1; /* [8] */
        // Comment of field: reserved_5
        /*
        select start vector。
        0：default（0xfa820800）；
        1：alternative（0x04000000）。
 */
        unsigned int reserved_5 : 1; /* [9] */
        // Comment of field: reserved_6
        /*
        复位后的debug模式选择：
        0：复位后进入Normal模式；
        1：复位后进入OCD模式。
 */
        unsigned int reserved_6 : 1; /* [10] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [11] */
        // Comment of field: reserved_8
        /*
        使能WDG(For DSP)。
        0：不使能；
        1：使能。
 */
        unsigned int reserved_8 : 1; /* [12] */
        // Comment of field: reserved_9
        /*
        DSP dbg同步选择信号，当需要dbg通过JTAG连接仿真器时需要将此bit配为1
 */
        unsigned int reserved_9 : 1; /* [13] */
        // Comment of field: reserved_10
        /*
        保留。
 */
        unsigned int reserved_10 : 18; /* [31..14] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dsp_ctrl1;

/* Define the union U_DSP_CTRL0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 5; /* [4..0] */
        // Comment of field: reserved_1
        /*
        DSP MFDE MMU START信号
 */
        unsigned int reserved_1 : 1; /* [5] */
        // Comment of field: reserved_2
        /*
        手动配置dsp0 runstall信号。如果用户需要手动配置runstall信号，需要配置dsp0_runstall_en=0
 */
        unsigned int reserved_2 : 1; /* [6] */
        // Comment of field: reserved_3
        /*
        DSP是否接受breakin信号
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: peri_runstall_dsp0
        /*
        DSP是否接受runstall信号
 */
        unsigned int peri_runstall_dsp0 : 1; /* [8] */
        // Comment of field: peri_statvectorsel_dsp0
        /*
        select start vector。
        0：default（0xfa820400）；
        1：alternative（0x03800000）。
 */
        unsigned int peri_statvectorsel_dsp0 : 1; /* [9] */
        // Comment of field: peri_ocdhaltonreset_dsp0
        /*
        复位后的debug模式选择：
        0：复位后进入Normal模式；
        1：复位后进入OCD模式。
 */
        unsigned int peri_ocdhaltonreset_dsp0 : 1; /* [10] */
        // Comment of field: reserved_4
        /*
        保留。
 */
        unsigned int reserved_4 : 1; /* [11] */
        // Comment of field: peri_dsp0_wdg_en
        /*
        使能WDG(For DSP)。
        0：不使能；
        1：使能。
 */
        unsigned int peri_dsp0_wdg_en : 1; /* [12] */
        // Comment of field: peri_syn_dbg_sel_dsp0
        /*
        DSP dbg同步选择信号，当需要dbg通过JTAG连接仿真器时需要将此bit配为1
 */
        unsigned int peri_syn_dbg_sel_dsp0 : 1; /* [13] */
        // Comment of field: reserved_5
        /*
        保留。
 */
        unsigned int reserved_5 : 18; /* [31..14] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dsp_ctrl0;

/* Define the union U_DSP_STATUS1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 2; /* [1..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [2] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [3] */
        // Comment of field: reserved_3
        /*
        保留。
 */
        unsigned int reserved_3 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dsp_status1;

/* Define the union U_DSP_STATUS0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 2; /* [1..0] */
        // Comment of field: peri_pwaitmode_dsp0
        /*
        Indicate WATII mode。
        0：not WATII mode；
        1：WATII mode。
 */
        unsigned int peri_pwaitmode_dsp0 : 1; /* [2] */
        // Comment of field: peri_xocdmode_dsp0
        /*
        Indicate debug mode。
        0：not debug mode；
        1：debug mode。
 */
        unsigned int peri_xocdmode_dsp0 : 1; /* [3] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dsp_status0;

/* Define the union U_PERI_DDRPHY_TEST0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 4; /* [3..0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 4; /* [7..4] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_ddrphy_test0;

/* Define the union U_PERI_CHIP_INFO4 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_chipset_info
        /*
        bit31~bit12:reserved
        bit11~bit5：peri_soc_fuse1[6:0]:给音频预留；
        bit4：peri_soc_fuse0[31]:DBX规格控制；
        bit3：peri_soc_fuse0[30]:Dolby DD+规格控制；
        bit2：peri_soc_fuse0[29]:DTS规格控制；
        bit1：peri_soc_fuse0[28]:reserved; Microvison规格控制,0支持1不支持
        bit0：peri_soc_fuse0[27]:Dolby DD规格控制；
 */
        unsigned int peri_chipset_info : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_chip_info4;
/* Define the union U_PERI_SW_SET */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_sw_set
        /*
        软件标识设置寄存器。
 */
        unsigned int peri_sw_set : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_sw_set;
/* Define the union U_PERI_QOS_CFG00 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tvd_arqos
        /*
        QOS配置信号
 */
        unsigned int tvd_arqos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: tvd_awqos
        /*
        QOS配置信号
 */
        unsigned int tvd_awqos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 9; /* [15..7] */
        // Comment of field: peri_dsp0_arqos
        /*
        QOS配置信号
 */
        unsigned int peri_dsp0_arqos : 3; /* [18..16] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [19] */
        // Comment of field: peri_dsp0_awqos
        /*
        QOS配置信号
 */
        unsigned int peri_dsp0_awqos : 3; /* [22..20] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [23] */
        // Comment of field: gpu_arqos
        /*
        QOS配置信号
 */
        unsigned int gpu_arqos : 3; /* [26..24] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [27] */
        // Comment of field: gpu_awqos
        /*
        QOS配置信号
 */
        unsigned int gpu_awqos : 3; /* [30..28] */
        // Comment of field: reserved_5
        /*
        保留。
 */
        unsigned int reserved_5 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg00;

/* Define the union U_PERI_QOS_CFG01 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ddrt0_arqos
        /*
        QOS配置信号
 */
        unsigned int ddrt0_arqos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: ddrt0_awqos
        /*
        QOS配置信号
 */
        unsigned int ddrt0_awqos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: reserved_2
        /*
        QOS配置信号
 */
        unsigned int reserved_2 : 3; /* [10..8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: vicap_awqos
        /*
        QOS配置信号
 */
        unsigned int vicap_awqos : 3; /* [14..12] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [15] */
        // Comment of field: reserved_5
        /*
        QOS配置信号
 */
        unsigned int reserved_5 : 3; /* [18..16] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [19] */
        // Comment of field: qtc_awqos
        /*
        QOS配置信号
 */
        unsigned int qtc_awqos : 3; /* [22..20] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [23] */
        // Comment of field: aiao_arqos
        /*
        QOS配置信号
 */
        unsigned int aiao_arqos : 3; /* [26..24] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [27] */
        // Comment of field: aiao_awqos
        /*
        QOS配置信号
 */
        unsigned int aiao_awqos : 3; /* [30..28] */
        // Comment of field: reserved_9
        /*
        保留。
 */
        unsigned int reserved_9 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg01;

/* Define the union U_PERI_QOS_CFG02 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: m0_smmu_pref_qos
        /*
        QOS配置信号
 */
        unsigned int m0_smmu_pref_qos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: reserved_1
        /*
        QOS配置信号
 */
        unsigned int reserved_1 : 3; /* [6..4] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [7] */
        // Comment of field: vpss0_arqos
        /*
        QOS配置信号
 */
        unsigned int vpss0_arqos : 3; /* [10..8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: vpss0_awqos
        /*
        QOS配置信号
 */
        unsigned int vpss0_awqos : 3; /* [14..12] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [15] */
        // Comment of field: vpss_mmu_pref_qos
        /*
        QOS配置信号
 */
        unsigned int vpss_mmu_pref_qos : 3; /* [18..16] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 5; /* [23..19] */
        // Comment of field: reserved_6
        /*
        QOS配置信号
 */
        unsigned int reserved_6 : 3; /* [26..24] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [27] */
        // Comment of field: reserved_8
        /*
        QOS配置信号
 */
        unsigned int reserved_8 : 3; /* [30..28] */
        // Comment of field: reserved_9
        /*
        保留。
 */
        unsigned int reserved_9 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg02;

/* Define the union U_PERI_QOS_CFG03 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dtmb_m_awqos
        /*
        QOS配置信号
 */
        unsigned int dtmb_m_awqos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: dtmb_m_arqos
        /*
        QOS配置信号
 */
        unsigned int dtmb_m_arqos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: jpgd_arqos
        /*
        QOS配置信号
 */
        unsigned int jpgd_arqos : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: jpgd_awqos
        /*
        QOS配置信号
 */
        unsigned int jpgd_awqos : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: reserved_4
        /*
        QOS配置信号
 */
        unsigned int reserved_4 : 3; /* [18..16] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [19] */
        // Comment of field: reserved_6
        /*
        QOS配置信号
 */
        unsigned int reserved_6 : 3; /* [22..20] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [23] */
        // Comment of field: reserved_8
        /*
        QOS配置信号
 */
        unsigned int reserved_8 : 3; /* [26..24] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [27] */
        // Comment of field: reserved_10
        /*
        QOS配置信号
 */
        unsigned int reserved_10 : 3; /* [30..28] */
        // Comment of field: reserved_11
        /*
        保留。
 */
        unsigned int reserved_11 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg03;

/* Define the union U_PERI_QOS_CFG04 */
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
        QOS配置信号
 */
        unsigned int reserved_1 : 3; /* [6..4] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [7] */
        // Comment of field: edma_awqos
        /*
        QOS配置信号
 */
        unsigned int edma_awqos : 3; /* [10..8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: edma_arqos
        /*
        QOS配置信号
 */
        unsigned int edma_arqos : 3; /* [14..12] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [15] */
        // Comment of field: reserved_5
        /*
        QOS配置信号
 */
        unsigned int reserved_5 : 3; /* [18..16] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [19] */
        // Comment of field: reserved_7
        /*
        QOS配置信号
 */
        unsigned int reserved_7 : 3; /* [22..20] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [23] */
        // Comment of field: tde_arqos
        /*
        reserved
 */
        unsigned int tde_arqos : 3; /* [26..24] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [27] */
        // Comment of field: tde_awqos
        /*
        reserved
 */
        unsigned int tde_awqos : 3; /* [30..28] */
        // Comment of field: reserved_10
        /*
        保留。
 */
        unsigned int reserved_10 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg04;

/* Define the union U_PERI_QOS_CFG05 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_vdp1_arqos
        /*
        reserved
 */
        unsigned int peri_vdp1_arqos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: peri_vdp1_awqos
        /*
        reserved
 */
        unsigned int peri_vdp1_awqos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: peri_vdp0_arqos
        /*
        QOS配置信号
 */
        unsigned int peri_vdp0_arqos : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: peri_vdp0_awqos
        /*
        QOS配置信号
 */
        unsigned int peri_vdp0_awqos : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: peri_vdp0_mmu_qos
        /*
        QOS配置信号
 */
        unsigned int peri_vdp0_mmu_qos : 3; /* [18..16] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [19] */
        // Comment of field: peri_vdp1_mmu_qos
        /*
        reserved
 */
        unsigned int peri_vdp1_mmu_qos : 3; /* [22..20] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [23] */
        // Comment of field: vicap_mmu_qos
        /*
        QOS配置信号
 */
        unsigned int vicap_mmu_qos : 3; /* [26..24] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [27] */
        // Comment of field: vicap_arqos
        /*
        QOS配置信号
 */
        unsigned int vicap_arqos : 3; /* [30..28] */
        // Comment of field: reserved_7
        /*
        保留。
 */
        unsigned int reserved_7 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg05;

/* Define the union U_PERI_QOS_CFG06 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: fmc_m_hqos
        /*
        reserved
 */
        unsigned int fmc_m_hqos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: peri_v0_0_awqos
        /*
        reserved
 */
        unsigned int peri_v0_0_awqos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: peri_v0_0_arqos
        /*
        reserved
 */
        unsigned int peri_v0_0_arqos : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: peri_v0_0_mmu_qos
        /*
        reserved
 */
        unsigned int peri_v0_0_mmu_qos : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: peri_v0_1_awqos
        /*
        reserved
 */
        unsigned int peri_v0_1_awqos : 3; /* [18..16] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [19] */
        // Comment of field: peri_v0_1_arqos
        /*
        reserved
 */
        unsigned int peri_v0_1_arqos : 3; /* [22..20] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [23] */
        // Comment of field: peri_v0_1_mmu_qos
        /*
        reserved
 */
        unsigned int peri_v0_1_mmu_qos : 3; /* [26..24] */
        // Comment of field: reserved_6
        /*
        保留。
 */
        unsigned int reserved_6 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg06;

/* Define the union U_PERI_QOS_CFG07 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        优先级配置信号
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 5; /* [7..3] */
        // Comment of field: eth0_hqos
        /*
        优先级配置信号
 */
        unsigned int eth0_hqos : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: reserved_3
        /*
        优先级配置信号
 */
        unsigned int reserved_3 : 3; /* [14..12] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [15] */
        // Comment of field: reserved_5
        /*
        PVR优先级配置信号
 */
        unsigned int reserved_5 : 3; /* [18..16] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [19] */
        // Comment of field: reserved_7
        /*
        优先级配置信号
 */
        unsigned int reserved_7 : 3; /* [22..20] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [23] */
        // Comment of field: reserved_9
        /*
        优先级配置信号
 */
        unsigned int reserved_9 : 3; /* [26..24] */
        // Comment of field: reserved_10
        /*
        reserved
 */
        unsigned int reserved_10 : 1; /* [27] */
        // Comment of field: reserved_11
        /*
        优先级配置信号
 */
        unsigned int reserved_11 : 3; /* [30..28] */
        // Comment of field: reserved_12
        /*
        保留。
 */
        unsigned int reserved_12 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg07;

/* Define the union U_PERI_QOS_CFG08 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        QOS配置信号
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        QOS配置信号
 */
        unsigned int reserved_2 : 3; /* [6..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        优先级配置信号
 */
        unsigned int reserved_4 : 3; /* [10..8] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [11] */
        // Comment of field: reserved_6
        /*
        优先级配置信号
 */
        unsigned int reserved_6 : 3; /* [14..12] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [15] */
        // Comment of field: peri_cpu_m1_arqos
        /*
        CPU MASTER口优先级配置信号
 */
        unsigned int peri_cpu_m1_arqos : 3; /* [18..16] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [19] */
        // Comment of field: peri_cpu_m1_awqos
        /*
        CPU MASTER口优先级配置信号
 */
        unsigned int peri_cpu_m1_awqos : 3; /* [22..20] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [23] */
        // Comment of field: usb2_m_hqos
        /*
        优先级配置信号
 */
        unsigned int usb2_m_hqos : 3; /* [26..24] */
        // Comment of field: reserved_10
        /*
        保留。
 */
        unsigned int reserved_10 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg08;

/* Define the union U_PERI_QOS_CFG09 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_emmc_arqos
        /*
        QOS配置信号
 */
        unsigned int peri_emmc_arqos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: peri_emmc_awqos
        /*
        QOS配置信号
 */
        unsigned int peri_emmc_awqos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: reserved_2
        /*
        优先级配置信号
 */
        unsigned int reserved_2 : 3; /* [10..8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: reserved_4
        /*
        优先级配置信号
 */
        unsigned int reserved_4 : 3; /* [14..12] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [15] */
        // Comment of field: reserved_6
        /*
        优先级配置信号
 */
        unsigned int reserved_6 : 3; /* [18..16] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [19] */
        // Comment of field: reserved_8
        /*
        优先级配置信号
 */
        unsigned int reserved_8 : 3; /* [22..20] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [23] */
        // Comment of field: reserved_10
        /*
        优先级配置信号
 */
        unsigned int reserved_10 : 3; /* [26..24] */
        // Comment of field: reserved_11
        /*
        reserved
 */
        unsigned int reserved_11 : 1; /* [27] */
        // Comment of field: reserved_12
        /*
        优先级配置信号
 */
        unsigned int reserved_12 : 3; /* [30..28] */
        // Comment of field: reserved_13
        /*
        保留。
 */
        unsigned int reserved_13 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg09;

/* Define the union U_PERI_QOS_CFG10 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        QOS配置信号
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        QOS配置信号
 */
        unsigned int reserved_2 : 3; /* [6..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        优先级配置信号
 */
        unsigned int reserved_4 : 3; /* [10..8] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [11] */
        // Comment of field: reserved_6
        /*
        优先级配置信号
 */
        unsigned int reserved_6 : 3; /* [14..12] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [15] */
        // Comment of field: reserved_8
        /*
        优先级配置信号
 */
        unsigned int reserved_8 : 3; /* [18..16] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [19] */
        // Comment of field: reserved_10
        /*
        CPU MASTER2口优先级配置信号
 */
        unsigned int reserved_10 : 3; /* [22..20] */
        // Comment of field: reserved_11
        /*
        reserved
 */
        unsigned int reserved_11 : 1; /* [23] */
        // Comment of field: reserved_12
        /*
        CPU MASTER2口优先级配置信号
 */
        unsigned int reserved_12 : 3; /* [26..24] */
        // Comment of field: reserved_13
        /*
        reserved
 */
        unsigned int reserved_13 : 1; /* [27] */
        // Comment of field: reserved_14
        /*
        优先级配置信号
 */
        unsigned int reserved_14 : 3; /* [30..28] */
        // Comment of field: reserved_15
        /*
        保留。
 */
        unsigned int reserved_15 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg10;

/* Define the union U_PERI_QOS_CFG11 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        QOS配置信号
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        QOS配置信号
 */
        unsigned int reserved_2 : 3; /* [6..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: peri_pvr_arqos
        /*
        优先级配置信号
 */
        unsigned int peri_pvr_arqos : 3; /* [10..8] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [11] */
        // Comment of field: peri_pvr_awqos
        /*
        优先级配置信号
 */
        unsigned int peri_pvr_awqos : 3; /* [14..12] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [15] */
        // Comment of field: peri_cipher_arqos
        /*
        cipher读QOS配置信号
 */
        unsigned int peri_cipher_arqos : 3; /* [18..16] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [19] */
        // Comment of field: peri_cipher_awqos
        /*
        cipher写QOS配置信号
 */
        unsigned int peri_cipher_awqos : 3; /* [22..20] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [23] */
        // Comment of field: peri_cipher_pref_qos
        /*
        cipher mmu QOS配置信号
 */
        unsigned int peri_cipher_pref_qos : 3; /* [26..24] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [27] */
        // Comment of field: peri_pvr_pref_qos
        /*
        reserved
 */
        unsigned int peri_pvr_pref_qos : 3; /* [30..28] */
        // Comment of field: reserved_9
        /*
        保留。
 */
        unsigned int reserved_9 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg11;

/* Define the union U_PERI_DSP0_0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_dsp0_0
        /*
        DSP启动代码寄存器0。
 */
        unsigned int peri_dsp0_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp0_0;
/* Define the union U_PERI_DSP0_1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_dsp0_1
        /*
        DSP启动代码寄存器1。
 */
        unsigned int peri_dsp0_1 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp0_1;
/* Define the union U_PERI_DSP0_2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_dsp0_2
        /*
        DSP启动代码寄存器2。
 */
        unsigned int peri_dsp0_2 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp0_2;
/* Define the union U_PERI_DSP0_3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_dsp0_3
        /*
        DSP启动代码寄存器3。
 */
        unsigned int peri_dsp0_3 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp0_3;
/* Define the union U_PERI_DSP0_4 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_dsp0_4
        /*
        DSP启动代码寄存器4。
 */
        unsigned int peri_dsp0_4 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp0_4;
/* Define the union U_PERI_DSP0_5 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_dsp0_5
        /*
        DSP启动代码寄存器5。
 */
        unsigned int peri_dsp0_5 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp0_5;
/* Define the union U_PERI_DSP0_6 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_dsp0_6
        /*
        DSP启动代码寄存器6。
 */
        unsigned int peri_dsp0_6 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp0_6;
/* Define the union U_PERI_DSP0_7 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_dsp0_7
        /*
        DSP启动代码寄存器7。
 */
        unsigned int peri_dsp0_7 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp0_7;
/* Define the union U_PERI_DSP1_0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp1_0;
/* Define the union U_PERI_DSP1_1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp1_1;
/* Define the union U_PERI_DSP1_2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp1_2;
/* Define the union U_PERI_DSP1_3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp1_3;
/* Define the union U_PERI_DSP1_4 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp1_4;
/* Define the union U_PERI_DSP1_5 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp1_5;
/* Define the union U_PERI_DSP1_6 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp1_6;
/* Define the union U_PERI_DSP1_7 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dsp1_7;
/* Define the union U_PERI_OD_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_dimming_pwm_1d1_od_sel
        /*
        reserved
 */
        unsigned int peri_dimming_pwm_1d1_od_sel : 1; /* [0] */
        // Comment of field: peri_sim0_pwren_od_sel
        /*
        od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int peri_sim0_pwren_od_sel : 1; /* [1] */
        // Comment of field: peri_sim0_rst_od_sel
        /*
        od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int peri_sim0_rst_od_sel : 1; /* [2] */
        // Comment of field: peri_dimming_pwm_1d2_od_sel
        /*
        reserved
 */
        unsigned int peri_dimming_pwm_1d2_od_sel : 1; /* [3] */
        // Comment of field: peri_dimming_pwm_od_sel
        /*
        od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int peri_dimming_pwm_od_sel : 1; /* [4] */
        // Comment of field: peri_glass_3d_od_sel
        /*
        reserved
 */
        unsigned int peri_glass_3d_od_sel : 1; /* [5] */
        // Comment of field: peri_sync_3do_od_sel
        /*
        reserved
 */
        unsigned int peri_sync_3do_od_sel : 1; /* [6] */
        // Comment of field: peri_dimming_pwm_1d3_od_sel
        /*
        reserved
 */
        unsigned int peri_dimming_pwm_1d3_od_sel : 1; /* [7] */
        // Comment of field: peri_rf_tagc_od_sel
        /*
        od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int peri_rf_tagc_od_sel : 1; /* [8] */
        // Comment of field: peri_uart0_txd_od_sel
        /*
        od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int peri_uart0_txd_od_sel : 1; /* [9] */
        // Comment of field: reserved_0
        /*
        od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int reserved_0 : 1; /* [10] */
        // Comment of field: reserved_1
        /*
        od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int reserved_1 : 1; /* [11] */
        // Comment of field: peri_dtmb_agc_od_sel
        /*
        od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int peri_dtmb_agc_od_sel : 1; /* [12] */
        // Comment of field: reserved_2
        /*
        od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int reserved_2 : 1; /* [13] */
        // Comment of field: reserved_3
        /*
        od选择信号。
        0:CMOS输出；
        1:OD输出。
 */
        unsigned int reserved_3 : 1; /* [14] */
        // Comment of field: peri_p2p_xstb_od_sel
        /*
        reserved
 */
        unsigned int peri_p2p_xstb_od_sel : 1; /* [15] */
        // Comment of field: peri_p2p_sdlock_od_sel
        /*
        reserved
 */
        unsigned int peri_p2p_sdlock_od_sel : 1; /* [16] */
        // Comment of field: reserved_4
        /*
        保留。
 */
        unsigned int reserved_4 : 15; /* [31..17] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_od_ctrl;

/* Define the union U_QTC_MODE */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        进入到QTC数据通路选择：
        4'b0000:Equator YPbPr信号或者Equator test 信号，通过peri_qtc_equa_sel信号选择；
        4'b0001:Equator CVBS信号
        4'b0010:ADC_1CH数据；
        4'b0011:AIF SIF数据；
        4'b0100:AIF CVBS数据；
        4'b0101:TVD debug数据；
        4'b0110:ADC_2CH的i和q路拼接的24bit数据；
        4'b0111:Audio_Codec adc test数据；
        4'b1000:ADC_2CH的i和q路高位拼接的16bit数据；
        4'b1001:ADC_2CH的i路12bit数据；
        4'b1010:ADC_2CH的q路12bit数据；
        4'b1011:SIF_VBI选择的数据；
        else:TVD debug数据；
 */
        unsigned int reserved_0 : 4; /* [3..0] */
        // Comment of field: peri_vi_ch_sel
        /*
        vicap接口0时钟频率选择：
        0：clk_tvd_13；
        1：clk_pixel_hddec；
        2：clk_hdmi_rx（rx_1p4）
        3：保留。
 */
        unsigned int peri_vi_ch_sel : 2; /* [5..4] */
        unsigned int reserved_1 : 26;    /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_qtc_mode;

/* Define the union U_MODE_ATV */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mode_atv
        /*
        TVD 数据源头选择：
        2'b00：来自AIF；
        2'b01：来自Equator CVBS；
        2'b10：来自Equator Half scart；
        2'b11：reserved。
 */
        unsigned int mode_atv : 2;    /* [1..0] */
        unsigned int reserved_0 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mode_atv;

/* Define the union U_VDAC_EQUA_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        到Equator VDAC0 的Equator数据通路选择：
        2'b00:Equqtor的adc_data_out_1通路
        2'b01:Equqtor的adc_data_out_2通路
        2'b10:Equqtor的adc_data_out_3通路
        2'b11:Equqtor的adc_data_out_3通路
        vdac0_equa_sel
 */
        unsigned int reserved_0 : 2; /* [1..0] */
        // Comment of field: reserved_1
        /*
        保留
 */
        unsigned int reserved_1 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdac_equa_sel;

/* Define the union U_HDMITX_MHLNX_CLK_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留.peri_hdmitx_mhlnx_clk_sel
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
} u_hdmitx_mhlnx_clk_sel;

/* Define the union U_PERI_AGC_SEL */
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
        AGC_SEL管脚信号方向的控制(agc_sel_out的oen控制)
        0：输出
        1：输入
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: peri_agc_sel
        /*
        板级tuner提供agc、片内DEMOD提供agc选择信号（此信号输出到管脚上）
        0: 选择片内RF DEMODr的AGC
        1：选择片内DTV DEMOD的AGC
 */
        unsigned int peri_agc_sel : 1; /* [2] */
        // Comment of field: reserved_2
        /*
        保留
 */
        unsigned int reserved_2 : 29; /* [31..3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_agc_sel;

/* Define the union U_PERI_AIAO_RX1_SD_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。AIAO RX1通道数据选择：
        0：来自Audio_Demod；
        1：来自板级I2S1。
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
} u_peri_aiao_rx1_sd_sel;

/* Define the union U_PERI_QTC_EQUA_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        QTC通路0数据来源选择：
        0：Equator YPbPr数据；
        1：Equator TEST观测信号。
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
} u_peri_qtc_equa_sel;

/* Define the union U_PERI_PENDING0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_pending0;
/* Define the union U_PERI_PENDING1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_pending1;
/* Define the union U_PERI_PENDING2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_pending2;
/* Define the union U_PERI_IO_OEN */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_tso0_oen
        /*
        tso0数据(clk/data/sync/vld）输出使能控制：
        0：输出；
        1：不输出。
 */
        unsigned int peri_tso0_oen : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [1] */
        // Comment of field: reserved_1
        /*
        保留。TSI0与TSI1数据(clk/data/sync/vld)输入选择控制。
        0：pvr inf3数据来自TSI0；
        1：pvr inf3数据来自TSI1。
 */
        unsigned int reserved_1 : 1; /* [2] */
        // Comment of field: peri_tso_loop_sel
        /*
        TSI0与TSO输出数据选择控制。
        0：pvr inf2数据来自TSI0；
        1：pvr inf2数据来自TSOUT。
 */
        unsigned int peri_tso_loop_sel : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        保留。TSI2与TSI1数据(clk/data/sync/vld)输入选择控制。
        0：pvr inf2数据来自TSI1；
        1：pvr inf2数据来自TSI2。
 */
        unsigned int reserved_2 : 1; /* [4] */
        // Comment of field: peri_dvb0_tsi_sel
        /*
        0：内置DEMOD
        1：外置DEMOD，TSI1
 */
        unsigned int peri_dvb0_tsi_sel : 1; /* [5] */
        // Comment of field: reserved_3
        /*
        保留。
 */
        unsigned int reserved_3 : 26; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_io_oen;

/* Define the union U_PERI_HDMIRX */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        保留HDMI_RX_2P0和HDMI_RX_1P4访问选择控制MPLL：
        0：选择HDMI_RX_PORT1.4访问MPLL；
        1：选择HDMI_RX_2.0访问MPLL。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        peri_arc_sel
        HDMIRX ARC PAD控制信号选择：
        0：选择HDMI_RX_PORT1.4访问ARC；
        1：选择HDMI_RX_2.0访问ARC。
 */
        unsigned int reserved_2 : 1;  /* [2] */
        unsigned int reserved_3 : 29; /* [31..3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx;

/* Define the union U_HDMI_RX_2P0_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留hdmirx_2p0的配置信号,控制观测信号的选择。
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hdmi_rx_2_p0_cfg;

/* Define the union U_HDMI_RX_2P0_START */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_hdmirx_2p0_rx_spi_boot_start
        /*
        HDMI_RX_2P0的BOOT_STARTE的使能信号，默认值为1'b1
 */
        unsigned int peri_hdmirx_2p0_rx_spi_boot_start : 1; /* [0] */
        // Comment of field: peri_hdmirx_2p0_spi_apb_sel
        /*
        HDMI_RX_2P0控制选择信号,表示选择SPI还是apb接口信号
        0：表示SPI接口选择(不允许配置)；
        1：表示APB接口选择。
 */
        unsigned int peri_hdmirx_2p0_spi_apb_sel : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hdmi_rx_2_p0_start;

/* Define the union U_GPU_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        gpu在读DDR时DDR端口选择分发模式使能还是bypass。
        0：是分发使能模式；
        1:bypass。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: peri_gpu_mem_dbg_bus
        /*
        reserved
 */
        unsigned int peri_gpu_mem_dbg_bus : 18; /* [18..1] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 13; /* [31..19] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gpu_ctrl;

/* Define the union U_ETH_FEPHY_CTR */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留FEPHY的PATCH的使能信号
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        保留FEPHY的TCLK的使能信号
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        保留FEPHY的MDIO的输入信号
 */
        unsigned int reserved_2 : 16; /* [17..2] */
        // Comment of field: reserved_3
        /*
        保留fephy mdio数据线
 */
        unsigned int reserved_3 : 1; /* [18] */
        // Comment of field: reserved_4
        /*
        保留fephy mdio读写时钟
 */
        unsigned int reserved_4 : 1; /* [19] */
        // Comment of field: reserved_5
        /*
        保留FEPHY的地址信号
 */
        unsigned int reserved_5 : 5; /* [24..20] */
        // Comment of field: reserved_6
        /*
        保留。
 */
        unsigned int reserved_6 : 7; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_eth_fephy_ctr;

/* Define the union U_PERI_EQUATOR_HADD */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_equator_hddec_sel
        /*
        TVD or HDDEC给equator模拟的clamp选择：
        0：TVD；
        1：HDDEC。
 */
        unsigned int peri_equator_hddec_sel : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        保留EQUATOR DAC0时钟和数据选择信号
        2'b00:aif时钟和数据选择信号
        2'b01:cvbs的时钟和数据选择信号；
        2'b10:保留；
        2'b11:afe的时钟和数据选择信号。
 */
        unsigned int reserved_0 : 2; /* [2..1] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 2; /* [4..3] */
        // Comment of field: reserved_2
        /*
        保留TVD or equator数字部分给equator模拟的AGC
        0：TVD；
        1：EQUATOR数字部分。
 */
        unsigned int reserved_2 : 1; /* [5] */
        // Comment of field: reserved_3
        /*
        保留。
 */
        unsigned int reserved_3 : 26; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_equator_hadd;

/* Define the union U_PERI_USB3_STATE */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        测试状态指示信号:
        bit[0] --ALIGN_DETECT;
        bit[1] --RX_STANDBY_STATUS;
        bit[2] --RX_DATA_VALID;
        bit[3] --DATA_BUS_WIDTH_0;
        bit[4] --DATA_BUS_WIDTH_1;
 */
        unsigned int reserved_0 : 5; /* [4..0] */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: reserved_2
        /*
        保留测试输出结果
 */
        unsigned int reserved_2 : 4;  /* [11..8] */
        unsigned int reserved_3 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_usb3_state;

/* Define the union U_PERI_USB3_TEST */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        测试数据信号
 */
        unsigned int reserved_0 : 4; /* [3..0] */
        // Comment of field: reserved_1
        /*
        测试地址信号
 */
        unsigned int reserved_1 : 5; /* [8..4] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 3; /* [11..9] */
        // Comment of field: reserved_3
        /*
        保留。
 */
        unsigned int reserved_3 : 2; /* [13..12] */
        // Comment of field: reserved_4
        /*
        保留。
 */
        unsigned int reserved_4 : 2; /* [15..14] */
        // Comment of field: reserved_5
        /*
        测试写信号
 */
        unsigned int reserved_5 : 1; /* [16] */
        // Comment of field: reserved_6
        /*
        保留。
 */
        unsigned int reserved_6 : 3; /* [19..17] */
        // Comment of field: reserved_7
        /*
        Controls whether the PHY RX is active when the PHY is in P0 or P0s in PCIe mode.
        Controls whether the PHY RX is active when the PHY is in any power state with PCLK on in SATA mode.
        ? 0 – Active
        ? 1 – Standby
 */
        unsigned int reserved_7 : 1; /* [20] */
        // Comment of field: reserved_8
        /*
        保留。
        Transmitter voltage level select, halved when TxSwing is 1
        ? 000 – Normal operating range (1000mV)
        ? 001 – 1100mV
        ? 010 – 900mV
        ? 011 – 700mV
        ? 100 – 600mV
        ? 101 – 500mV
        ? 110 – 400mV
        ? 111
 */
        unsigned int reserved_8 : 3; /* [23..21] */
        // Comment of field: reserved_9
        /*
        保留。
        Transmitter de-emphasis select:
        ? 0: -6dB de-emphasis
        ? 1: -3.5dB de-emphasis
        ? 2: 0dB de-emphasis
        ? 3: 0dB de-emphasis
 */
        unsigned int reserved_9 : 2; /* [25..24] */
        // Comment of field: reserved_10
        /*
        ? 0 – ALIGN
        ? 1 – D24.3
        ? 2 – D10.2
        ? 3 – Reserved
 */
        unsigned int reserved_10 : 2; /* [27..26] */
        // Comment of field: reserved_11
        /*
        保留。
        Transmitter voltage swing level select:
        ? 0 – Full Swing
        ? 1 – Low Swing
 */
        unsigned int reserved_11 : 1; /* [28] */
        // Comment of field: reserved_12
        /*
        保留。
 */
        unsigned int reserved_12 : 3; /* [31..29] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_usb3_test;

/* Define the union U_PERI_USB3_MODE */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        Selects PHY operating mode
        ? 0 – PCI Express
        ? 1 – USB SuperSpeed
        ? 2 – SATA
        ? 3 – Reserved
 */
        unsigned int reserved_0 : 2; /* [1..0] */
        // Comment of field: reserved_1
        /*
        Controls whether the PHY performs 8b/10b encode and decode:
        ? 0 – 8b/10b encode/decode performed normally
        ? 1 – 8b/10b encode/decode bypassed
 */
        unsigned int reserved_1 : 1; /* [2] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 29; /* [31..3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_usb3_mode;

/* Define the union U_PERI_DTMB_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_dtmb_dev_addr
        /*
        DTMB的I2C设备地址配置信号。
 */
        unsigned int peri_dtmb_dev_addr : 2; /* [1..0] */
        // Comment of field: tvd_demo_mem_sel
        /*
        00：unused （逻辑内部实际上DEMOD使用memory）
        01：reserved （逻辑内部实际上TVD使用memory）
        10：DEMOD使用memory
        11：TVD使用memory
 */
        unsigned int tvd_demo_mem_sel : 2; /* [3..2] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [4] */
        // Comment of field: bootmem_demod_sel
        /*
        bootram选通：bootmem_demod_sel=1;
        demo选通：bootmem_demod_sel=0;
 */
        unsigned int bootmem_demod_sel : 1; /* [5] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 26; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dtmb_ctrl;

/* Define the union U_PERI_DDR_BUS_EN */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_ddr32bit_bus_en
        /*
        reserved
 */
        unsigned int peri_ddr32bit_bus_en : 2; /* [1..0] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_ddr_bus_en;

/* Define the union U_PERI_POWEN_OUT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        表示屏的控制信号，在启动的时候默认为0，
        0：表示没电；
        1：表示有电。
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
} u_peri_powen_out;

/* Define the union U_PERI_VBI_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        功能模式和debug模式的选择信号：
        0：表示选择VBI功能模式；
        1：表示选择QTC的debug模式。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: peri_vbi_clk_sel
        /*
        到vbi的时钟和数据源选择信号：
        0：表示选择HDDEC到VBI的信号；
        1：表示选择TVD到VBI的信号。
 */
        unsigned int peri_vbi_clk_sel : 1; /* [1] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_vbi_ctrl;

/* Define the union U_TEST_NOC_PWRDISCTARGPWRACTIVE */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        noc测试信号
 */
        unsigned int reserved_0 : 6;  /* [5..0] */
        unsigned int reserved_1 : 26; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_test_noc_pwrdisctargpwractive;

/* Define the union U_PERI_HDMIRX_REG_BAR_PATTERN_A0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留
        控制DDR PHY的iotst_iotest端口，观测CK和DM模块中的IOTEST/TI/TE信号
        [0]:iotst_iotest[0],CK
        [1]:iotst_iotest[2]
        [6:2]:iotst_iotest[8:4]
        [15:7]:iotst_iotest[52:44]
 */
        unsigned int reserved_0 : 16; /* [15..0] */
        // Comment of field: reserved_1
        /*
        保留HDMI1.4 RX/HDMI2.0RX输出的test pattern colorbar0的内容。
 */
        unsigned int reserved_1 : 14; /* [29..16] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_bar_pattern_a0;

/* Define the union U_PERI_HDMIRX_REG_BAR_PATTERN_A1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。HDMI1.4 RX/HDMI2.0RX输出的test pattern colorbar1的内容
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_bar_pattern_a1;

/* Define the union U_PERI_HDMIRX_REG_BAR_PATTERN_A2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留HDMI1.4 RX/HDMI2.0RX输出的test pattern colorbar2的内容。
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_bar_pattern_a2;

/* Define the union U_PERI_HDMIRX_REG_BAR_PATTERN_A3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留HDMI1.4 RX/HDMI2.0RX输出的test pattern colorbar3的内容。
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_bar_pattern_a3;

/* Define the union U_PERI_HDMIRX_REG_BAR_PATTERN_A4 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留HDMI1.4 RX/HDMI2.0RX输出的test pattern colorbar4的内容。
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_bar_pattern_a4;

/* Define the union U_PERI_HDMIRX_REG_BAR_PATTERN_A5 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留HDMI1.4 RX/HDMI2.0RX输出的test pattern colorbar5的内容。
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_bar_pattern_a5;

/* Define the union U_PERI_HDMIRX_REG_BAR_PATTERN_A6 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留HDMI1.4 RX/HDMI2.0RX输出的test pattern colorbar6的内容。
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_bar_pattern_a6;

/* Define the union U_PERI_HDMIRX_REG_BAR_PATTERN_A7 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留HDMI1.4 RX/HDMI2.0RX输出的test pattern colorbar7的内容。
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_bar_pattern_a7;

/* Define the union U_PERI_HDMIRX_REG_VID_FORMAT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留HDMI1.4 RX/HDMI2.0RX输出的test pattern：
        [31:16] Reserved;
        [15] 0: sel hw value;1:overwrite with software value;
        [14] RGB or YCC;
        [13] 0：colorbar,1：渐变灰阶;
        [12:0] h_resolution;。
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_vid_format;
/* Define the union U_PERI_HDMIRX_REG_PAT_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留HDMI1.4 RX/HDMI2.0RX输出的pattern：
        [31:1] reserved;
        [0] 0：normal pattern,1：test pattern。
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_pat_sel;
/* Define the union U_PERI_HDMIRX_REG_CBAR_WID_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmirx_reg_cbar_wid_sel;
/* Define the union U_PERI_HDMI2VI_ADAPTER */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        [15:14] reserved；
        [13] 表示异步fifo的full状态；
        [12] 表示异步fifo的empty状态；
        [11:6] 表示异步fifo读写指针的最大间距统计值；               
        [5:0] 表示异步fifo读写指针的最小间距统计值。
 */
        unsigned int reserved_0 : 16; /* [15..0] */
        // Comment of field: reserved_1
        /*
        [31:24] Reserved;
        [23] 清除异步fifo的full状态；
        [22] 清除异步fifo的empty状态；
        [21] 清零异步fifo读写指针的间距统计数据；
        [20] reserved；
        [19] software reset；
        [18] 0：使用hsync同步奇偶像素，1：使用DE同步奇偶像素；
        [17] 调整写进异步fifo奇偶像素的位置，一般使用默认值0即可；
        [16] 表示是否使能adapter,0：disable,1：enable。
 */
        unsigned int reserved_1 : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_hdmi2_vi_adapter;

/* Define the union U_PERI_SSP0_LDC_DMA_REQ_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        控制SSP0 LDC是用single dma请求还是burst dma请求:
        0:burst dma请求；
        1:single dma请求。
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
} u_peri_ssp0_ldc_dma_req_sel;

/* Define the union U_PERI_SSP1_LDC_DMA_REQ_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        peri_ssp1_ldc_dma_req_sel
        控制SSP1 LDC是用single dma请求还是burst dma请求:
        0:burst dma请求；
        1:single dma请求。
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
} u_peri_ssp1_ldc_dma_req_sel;

/* Define the union U_PERI_QOS_CFG_09 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        QOS配置信号
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        QOS配置信号
 */
        unsigned int reserved_2 : 3; /* [6..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: vdh_arqos
        /*
        QOS配置信号
 */
        unsigned int vdh_arqos : 3; /* [10..8] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [11] */
        // Comment of field: vdh_awqos
        /*
        QOS配置信号
 */
        unsigned int vdh_awqos : 3; /* [14..12] */
        // Comment of field: reserved_5
        /*
        QOS配置信号
 */
        unsigned int reserved_5 : 1; /* [15] */
        // Comment of field: peri_vdh_mmu_arqos
        /*
        QOS配置信号
 */
        unsigned int peri_vdh_mmu_arqos : 3; /* [18..16] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 5; /* [23..19] */
        // Comment of field: reserved_7
        /*
        QOS配置信号
 */
        unsigned int reserved_7 : 3; /* [26..24] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [27] */
        // Comment of field: jpgd_mmu_pref_qos
        /*
        QOS配置信号
 */
        unsigned int jpgd_mmu_pref_qos : 3; /* [30..28] */
        // Comment of field: reserved_9
        /*
        保留。
 */
        unsigned int reserved_9 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg_09;

/* Define the union U_PERI_I2S0_DIN1_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        peri_i2s0_din1_sel
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
} u_peri_i2_s0_din1_sel;

/* Define the union U_PERI_QOS_CFG_12 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pgd_mmu_pref_qos
        /*
        reserved
 */
        unsigned int pgd_mmu_pref_qos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: tde_mmu_pref_qos
        /*
        reserved
 */
        unsigned int tde_mmu_pref_qos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: reserved_2
        /*
        QOS配置信号
 */
        unsigned int reserved_2 : 3; /* [10..8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: reserved_4
        /*
        QOS配置信号
 */
        unsigned int reserved_4 : 3; /* [14..12] */
        // Comment of field: reserved_5
        /*
        保留。
 */
        unsigned int reserved_5 : 17; /* [31..15] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qos_cfg_12;

/* Define the union U_PERI_I2S1_MCLK_OEN */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_i2s1_mclk_oen
        /*
        reserved
 */
        unsigned int peri_i2s1_mclk_oen : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_i2_s1_mclk_oen;

/* Define the union U_SC_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: remapclear
        /*
        地址重映射清除选择:
        1010、其他：不清除地址重映射；
        0101：清除地址重映射；
        其他：系统重启。
 */
        unsigned int remapclear : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 4; /* [7..4] */
        // Comment of field: remapstat
        /*
        地址重映射的状态。
        0：系统地址分配处于地址重映射清除（REMAP CLEAR）状态；
        1：系统地址分配处于地址重映射（REMAP）状态。
 */
        unsigned int remapstat : 1; /* [8] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 23; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_sc_ctrl;

/* Define the union U_SPI0_SPI1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留SPI1输出时钟和数据使能选择信号：
        0：选择SPI1产生的使能信号；
        1：选择外设控制器产生的peri_spi1_clk_oen使能信号。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        保留SPI1输入选择信号：
        0：输入spi1_data_di来自于io的spi1_di_in（4线模式）；
        1：输入spi1_data_di来自于io的spi1_do_in（3线模式）。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        保留SPI1输出时钟和数据使能：
        0：使能；
        1：不使能。
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        SPI0输出时钟和数据使能选择信号：
        0：选择SPI0产生的使能信号；
        1：选择外设控制器产生的peri_spi0_clk_oen使能信号。
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        SPI0输入选择信号：
        0：输入spi0_data_di来自于io的spi0_di_in（4线模式）；
        1：输入spi0_data_di来自于io的spi0_do_in（3线模式）。
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        SPI0输出时钟和数据使能：
        0：使能；
        1：不使能。
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        保留。
 */
        unsigned int reserved_6 : 26; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_spi0_spi1;

/* Define the union U_GLP_IP_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
        选择信号：
        0：comip给vpss用；
        1：comip给vdp用。
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
} u_glp_ip_sel;

/* Define the union U_SDIOO_CTRL */
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
        保留SDIO0写保护信号极性控制：
        0：写保护信号低电平有效；
        1：写保护信号高电平有效。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        保留。
 */
        unsigned int reserved_2 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_sdioo_ctrl;

/* Define the union U_IO_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: blk_on_out
        /*
        blk_on的信号的输出的值
        0：输出0
        1：输出1
 */
        unsigned int blk_on_out : 1; /* [0] */
        // Comment of field: blk_on_oen
        /*
        blk_on的信号的方向
        0：输出
        1：输入
 */
        unsigned int blk_on_oen : 1; /* [1] */
        // Comment of field: pwr_en_out
        /*
        pwr_en的信号的输出的值
        0：输出0
        1：输出1
 */
        unsigned int pwr_en_out : 1; /* [2] */
        // Comment of field: pmu_en_out
        /*
        reserved
 */
        unsigned int pmu_en_out : 1; /* [3] */
        // Comment of field: peri_lockn_sel
        /*
        reserved
 */
        unsigned int peri_lockn_sel : 1; /* [4] */
        // Comment of field: peri_htpdn_sel
        /*
        reserved
 */
        unsigned int peri_htpdn_sel : 1; /* [5] */
        // Comment of field: peri_emmc_clk_oen
        /*
        emmc_clk的信号的方向
        1：输入
        0：输出
 */
        unsigned int peri_emmc_clk_oen : 1; /* [6] */
        // Comment of field: peri_emmc_rstn_oen
        /*
        emmc_rstn的信号的方向
        1：输入
        0：输出
 */
        unsigned int peri_emmc_rstn_oen : 1; /* [7] */
        // Comment of field: reserved_0
        /*
        ssp管脚选择ssp控制器或spi_ldc控制器
        0：ssp0_ldc
        1：spi0
 */
        unsigned int reserved_0 : 1; /* [8] */
        // Comment of field: reserved_1
        /*
        ssp管脚选择ssp控制器或spi_ldc控制器
        0：ssp1_ldc
        1：spi1
 */
        unsigned int reserved_1 : 1; /* [9] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [10] */
        // Comment of field: reserved_3
        /*
        详见《USB2 USB3观测管脚复用补充说明.xlsx》
 */
        unsigned int reserved_3 : 4; /* [14..11] */
        // Comment of field: peri_sync_3d_sel
        /*
        reserved
 */
        unsigned int peri_sync_3d_sel : 1; /* [15] */
        // Comment of field: peri_ttl_clk_oen
        /*
        正常输出还是三态输出
        0：正常输出
        1：三态输出
 */
        unsigned int peri_ttl_clk_oen : 1; /* [16] */
        // Comment of field: peri_ttl_de_oen
        /*
        正常输出还是三态输出
        0：正常输出
        1：三态输出
 */
        unsigned int peri_ttl_de_oen : 1; /* [17] */
        // Comment of field: peri_ttl_hsync_oen
        /*
        正常输出还是三态输出
        0：正常输出
        1：三态输出
 */
        unsigned int peri_ttl_hsync_oen : 1; /* [18] */
        // Comment of field: peri_ttl_vsync_oen
        /*
        正常输出还是三态输出
        0：正常输出
        1：三态输出
 */
        unsigned int peri_ttl_vsync_oen : 1; /* [19] */
        // Comment of field: reserved_4
        /*
        保留。
 */
        unsigned int reserved_4 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_io_ctrl;

/* Define the union U_PERI_QAM_LDO */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        QAM LDO复位信号
        0:复位LDO
        1:释放复位
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        QAM LDO内部精确电压基准调节
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
 */
        unsigned int reserved_1 : 4; /* [4..1] */
        // Comment of field: reserved_2
        /*
        QAM LDO高阻使能信号
        0:电源端正常输出
        1:电源输出端高阻
 */
        unsigned int reserved_2 : 1; /* [5] */
        // Comment of field: reserved_3
        /*
        调压反馈电阻内部集成
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
        20mV/step
 */
        unsigned int reserved_3 : 4; /* [9..6] */
        // Comment of field: reserved_4
        /*
        保留。
 */
        unsigned int reserved_4 : 22; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_qam_ldo;

/* Define the union U_PERI_MISC_CONTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ssp_ldc vsync的选择
        0：vdp送出的vsync
        1：lvds pwm2模拟的vsync
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ssp_ldc vsync的选择
        0：vdp送出的vsync
        1：lvds pwm3模拟的vsync
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        卡写保护检测信号输入选择
        1：选择SD_CPWR
        0：选择SD_CPWR1
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        reserved
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
} u_peri_misc_contrl;

/* Define the union U_PCIE_PHY_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        pcie phy测试地址
 */
        unsigned int reserved_0 : 5; /* [4..0] */
        // Comment of field: reserved_1
        /*
        pcie phy测试写
 */
        unsigned int reserved_1 : 1; /* [5] */
        // Comment of field: reserved_2
        /*
        保留
 */
        unsigned int reserved_2 : 2; /* [7..6] */
        // Comment of field: reserved_3
        /*
        pcie_phy测试写数据
 */
        unsigned int reserved_3 : 4; /* [11..8] */
        // Comment of field: reserved_4
        /*
        pcie phy测试输出
 */
        unsigned int reserved_4 : 4; /* [15..12] */
        // Comment of field: reserved_5
        /*
        2'b00 Align
        2'b01 D24.3
        2'b10 D10.2
        2'b11 Reserved
 */
        unsigned int reserved_5 : 2; /* [17..16] */
        // Comment of field: reserved_6
        /*
        是否bypass 8b/10b编码
 */
        unsigned int reserved_6 : 1; /* [18] */
        // Comment of field: reserved_7
        /*
        rx_standby状态
 */
        unsigned int reserved_7 : 1; /* [19] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [20] */
        // Comment of field: reserved_9
        /*
        PHY的地址位宽状态，待刷新
 */
        unsigned int reserved_9 : 2; /* [22..21] */
        // Comment of field: reserved_10
        /*
        PHY是否处于l1ss低功耗状态，待刷新
 */
        unsigned int reserved_10 : 1; /* [23] */
        // Comment of field: reserved_11
        /*
        PCIE Clk IO时钟来源
        0: PHY
        1: CRG
 */
        unsigned int reserved_11 : 1; /* [24] */
        // Comment of field: reserved_12
        /*
        保留
 */
        unsigned int reserved_12 : 7; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_pcie_phy_ctrl;

/* Define the union U_UFS_PAD_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        COMBO PHY UFS使能:
        0：不使能；
        1：使能。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        UFS PAD reset输出:
        0：复位；
        1：不复位。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        COMBO PHY EMMC使能：
        0：不使能；
        1：使能。
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        UFS Refclk IO input pull up/dn enable:
        0:pull up/dn disable;
        1:pull up/dn enable.
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        保留
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        UFS Refclk IO input disable:
        0:input enable;
        1:input disable.
 */
        unsigned int reserved_6 : 1; /* [6] */
        // Comment of field: reserved_7
        /*
        UFS Refclk IO pull up/dn select when bit4=1'b1：
        0:pull dn;
        1:pull up.
 */
        unsigned int reserved_7 : 1; /* [7] */
        // Comment of field: reserved_8
        /*
        UFS Refclk IO output driver strength control
 */
        unsigned int reserved_8 : 1; /* [8] */
        // Comment of field: reserved_9
        /*
        UFS Refclk IO output driver strength control
 */
        unsigned int reserved_9 : 1; /* [9] */
        // Comment of field: reserved_10
        /*
        UFS Refclk IO output driver strength control
 */
        unsigned int reserved_10 : 1; /* [10] */
        // Comment of field: reserved_11
        /*
        UFS Refclk IO output enable:
        0: disable;
        1: enable.
 */
        unsigned int reserved_11 : 1; /* [11] */
        // Comment of field: reserved_12
        /*
        UFS Reset IO input pull up/dn enable:
        0:pull up/dn disable;
        1:pull up/dn enable.
 */
        unsigned int reserved_12 : 1; /* [12] */
        // Comment of field: reserved_13
        /*
        保留
 */
        unsigned int reserved_13 : 1; /* [13] */
        // Comment of field: reserved_14
        /*
        UFS Reset IO input disable:
        0:input enable;
        1:input disable.
 */
        unsigned int reserved_14 : 1; /* [14] */
        // Comment of field: reserved_15
        /*
        UFS Reset IO pull up/dn select when bit12=1'b1：
        0:pull dn;
        1:pull up.
 */
        unsigned int reserved_15 : 1; /* [15] */
        // Comment of field: reserved_16
        /*
        UFS Reset IO output driver strength control
 */
        unsigned int reserved_16 : 1; /* [16] */
        // Comment of field: reserved_17
        /*
        UFS Reset IO output driver strength control
 */
        unsigned int reserved_17 : 1; /* [17] */
        // Comment of field: reserved_18
        /*
        UFS Reset IO output driver strength control
 */
        unsigned int reserved_18 : 1; /* [18] */
        // Comment of field: reserved_19
        /*
        UFS Reset IO output enable:
        0: disable;
        1: enable.
 */
        unsigned int reserved_19 : 1; /* [19] */
        // Comment of field: reserved_20
        /*
        保留
 */
        unsigned int reserved_20 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_ufs_pad_ctrl;

/* Define the union U_TDE_STATE */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tde_softrst_state
        /*
        tde软复位状态观测。
 */
        unsigned int tde_softrst_state : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_tde_state;

/* Define the union U_PERI_TS_SEL_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        tso_tu1的ts流选择信号：
        0：选择ts流ts2；
        1：选择外置demod的ts流ts3。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        pvr dvb2的ts流选择信号：
        0：选择tsmix的ts流tso0；
        1：选择环回ts流。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        pvr dvb4的ts流选择信号：
        0：选择tsmix的ts流tso1；
        1：选择环回ts流。
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        pvr dvb5的ts流选择信号：
        0：选择tsmix的ts流tso2；
        1：选择环回ts流。
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        pvr dvb6的ts流选择信号：
        0：选择tsmix的ts流tso3；
        1：选择环回ts流。
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        pvr dvb7的ts流选择信号：
        0：选择tsmix的ts流tso4；
        1：选择环回ts流。
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        输出到卡的ts流选择：
        0：选择pvr输出的ts流；
        1：选择tsmix输出的ts流。
 */
        unsigned int reserved_6 : 1; /* [6] */
        // Comment of field: reserved_7
        /*
        换回ts流选择信号：
        0：卡0的ts流ts0；
        1：输出的ts流。
 */
        unsigned int reserved_7 : 1; /* [7] */
        // Comment of field: reserved_8
        /*
        pvr dvb3和tsmix dvb1的ts流选择信号：
        0：选择ts2或ts3选择后的ts流tso_tu1，选择信号为peri_ts_sel0；
        1：选择ts流ts1。
 */
        unsigned int reserved_8 : 1; /* [8] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 23; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_ts_sel_cfg;

/* Define the union U_PERI_USB_PULL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        使能2port phy的phy2 dp下拉电阻：
        0：不使能；
        1：使能。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        使能2port phy的phy2 dm下拉电阻：
        0：不使能；
        1：使能。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        使能2port phy的phy0 dp下拉电阻：
        0：不使能；
        1：使能。
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        使能2port phy的phy0 dm下拉电阻：
        0：不使能；
        1：使能。
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        使能2port phy的phy1 dp下拉电阻：
        0：不使能；
        1：使能。
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        使能2port phy的phy1 dm下拉电阻：
        0：不使能；
        1：使能。
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        usb2 phy debug控制信号。
 */
        unsigned int reserved_6 : 1; /* [6] */
        // Comment of field: reserved_7
        /*
        usb2 phy debug控制信号。
 */
        unsigned int reserved_7 : 2; /* [8..7] */
        // Comment of field: reserved_8
        /*
        usb2 phy debug控制信号。
 */
        unsigned int reserved_8 : 1; /* [9] */
        // Comment of field: reserved_9
        /*
        usb2 phy debug控制信号。
 */
        unsigned int reserved_9 : 1; /* [10] */
        // Comment of field: reserved_10
        /*
        usb2 phy debug控制信号。
 */
        unsigned int reserved_10 : 2; /* [12..11] */
        // Comment of field: reserved_11
        /*
        usb2 phy debug信号。
 */
        unsigned int reserved_11 : 1; /* [13] */
        // Comment of field: reserved_12
        /*
        usb2 phy debug控制信号。
 */
        unsigned int reserved_12 : 1; /* [14] */
        // Comment of field: reserved_13
        /*
        usb2 phy debug控制信号。
 */
        unsigned int reserved_13 : 2; /* [16..15] */
        // Comment of field: reserved_14
        /*
        usb2 phy debug控制信号。
 */
        unsigned int reserved_14 : 1; /* [17] */
        // Comment of field: reserved_15
        /*
        usb3 phy debug控制信号,选择u3 phy控制信号PWDN_0,PWDN_1，
        RX_EQ_TRAINING_I,TX_DETECT_RX_I,TX_ELECIDLE_I,TX_ONESZEROS_I,
        RX_POLARITY_I,RX_TERMINATION_I来源：
        0：usb控制器；
        1：外设控制器。
 */
        unsigned int reserved_15 : 1; /* [18] */
        // Comment of field: reserved_16
        /*
        usb3 phy debug控制信号。
 */
        unsigned int reserved_16 : 1; /* [19] */
        // Comment of field: reserved_17
        /*
        usb3 phy debug控制信号。
 */
        unsigned int reserved_17 : 1; /* [20] */
        // Comment of field: reserved_18
        /*
        usb3 phy debug控制信号。
 */
        unsigned int reserved_18 : 1; /* [21] */
        // Comment of field: reserved_19
        /*
        usb3 phy debug控制信号。
 */
        unsigned int reserved_19 : 1; /* [22] */
        // Comment of field: reserved_20
        /*
        usb3 phy debug控制信号。
 */
        unsigned int reserved_20 : 1; /* [23] */
        // Comment of field: reserved_21
        /*
        usb3 phy debug控制信号。
 */
        unsigned int reserved_21 : 1; /* [24] */
        // Comment of field: reserved_22
        /*
        usb3 phy debug控制信号。
 */
        unsigned int reserved_22 : 2; /* [26..25] */
        // Comment of field: reserved_23
        /*
        usb3 phy debug控制信号。
 */
        unsigned int reserved_23 : 3; /* [29..27] */
        // Comment of field: reserved_24
        /*
        reserved
 */
        unsigned int reserved_24 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_usb_pull;

/* Define the union U_PERI_USB2_P3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        使能USB2 Port3 dp下拉电阻：
        0：不使能；
        1：使能。
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        使能USB2 Port3 dm下拉电阻：
        0：不使能；
        1：使能。
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        usb2 phy port3 debug控制信号。
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        usb2 phy port3 debug控制信号。
 */
        unsigned int reserved_3 : 2; /* [4..3] */
        // Comment of field: reserved_4
        /*
        usb2 phy port3 debug控制信号。
 */
        unsigned int reserved_4 : 1; /* [5] */
        // Comment of field: reserved_5
        /*
        usb2 phy port3 debug控制信号
 */
        unsigned int reserved_5 : 2; /* [7..6] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_usb2_p3;

/* Define the union U_EDMA_HIGH_ADDR_CTRL */
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
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 13; /* [15..3] */
        // Comment of field: reserved_4
        /*
        与EDMA的32bit地址一起组成34bit读地址。详见地址分配方案。
 */
        unsigned int reserved_4 : 2; /* [17..16] */
        // Comment of field: reserved_5
        /*
        与EDMA的32bit地址一起组成34bit写地址。详见地址分配方案。
 */
        unsigned int reserved_5 : 2; /* [19..18] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 2; /* [21..20] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 2; /* [23..22] */
        // Comment of field: reserved_8
        /*
        保留。
 */
        unsigned int reserved_8 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_edma_high_addr_ctrl;

/* Define the union U_PERI_MEM_ADJUST0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: himdtv_C_RFS_EMA
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rfs_ema : 3; /* [2..0] */
        // Comment of field: himdtv_C_RFS_EMAW
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rfs_emaw : 2; /* [4..3] */
        // Comment of field: himdtv_C_RFS_WABL
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rfs_wabl : 1; /* [5] */
        // Comment of field: himdtv_C_RFS_WABLM
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rfs_wablm : 2; /* [7..6] */
        // Comment of field: himdtv_C_RAS_EMA
        /*
        RAM调速值
 */
        unsigned int himdtv_c_ras_ema : 3; /* [10..8] */
        // Comment of field: himdtv_C_RAS_EMAW
        /*
        RAM调速值
 */
        unsigned int himdtv_c_ras_emaw : 2; /* [12..11] */
        // Comment of field: himdtv_C_RAS_EMAS
        /*
        RAM调速值
 */
        unsigned int himdtv_c_ras_emas : 1; /* [13] */
        // Comment of field: himdtv_C_RAS_WABL
        /*
        RAM调速值
 */
        unsigned int himdtv_c_ras_wabl : 1; /* [14] */
        // Comment of field: himdtv_C_RAS_WABLM
        /*
        RAM调速值
 */
        unsigned int himdtv_c_ras_wablm : 2; /* [16..15] */
        // Comment of field: himdtv_C_RAS_STOV
        /*
        RAM调速值
 */
        unsigned int himdtv_c_ras_stov : 1; /* [17] */
        // Comment of field: reserved_0
        /*
        RAM调速值
 */
        unsigned int reserved_0 : 4; /* [21..18] */
        // Comment of field: himdtv_C_COLLDISN
        /*
        RAM调速值
 */
        unsigned int himdtv_c_colldisn : 1; /* [22] */
        // Comment of field: himdtv_C_RFT_EMAA
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rft_emaa : 3; /* [25..23] */
        // Comment of field: himdtv_C_RFT_EMAB
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rft_emab : 3; /* [28..26] */
        // Comment of field: himdtv_C_RFT_EMASA
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rft_emasa : 1; /* [29] */
        // Comment of field: reserved_1
        /*
        保留。
 */
        unsigned int reserved_1 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_mem_adjust0;

/* Define the union U_PERI_MEM_ADJUST1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: emmc_rft_emab
        /*
        reserved
 */
        unsigned int emmc_rft_emab : 3; /* [2..0] */
        // Comment of field: emmc_rft_emaa
        /*
        reserved
 */
        unsigned int emmc_rft_emaa : 3; /* [5..3] */
        // Comment of field: emmc_rft_emasa
        /*
        reserved
 */
        unsigned int emmc_rft_emasa : 1; /* [6] */
        // Comment of field: reserved_0
        /*
        RAM调速值
 */
        unsigned int reserved_0 : 3; /* [9..7] */
        // Comment of field: reserved_1
        /*
        RAM调速值
 */
        unsigned int reserved_1 : 3; /* [12..10] */
        // Comment of field: reserved_2
        /*
        RAM调速值
 */
        unsigned int reserved_2 : 1; /* [13] */
        // Comment of field: dsp_wabl
        /*
        RAM调速值
 */
        unsigned int dsp_wabl : 1; /* [14] */
        // Comment of field: dsp_emaw
        /*
        RAM调速值
 */
        unsigned int dsp_emaw : 2; /* [16..15] */
        // Comment of field: dsp_ema
        /*
        RAM调速值
 */
        unsigned int dsp_ema : 3; /* [19..17] */
        // Comment of field: dsp_wablm
        /*
        RAM调速值
 */
        unsigned int dsp_wablm : 2; /* [21..20] */
        // Comment of field: msp_rft_colldisn
        /*
        reserved
 */
        unsigned int msp_rft_colldisn : 1; /* [22] */
        // Comment of field: msp_rft_emaa
        /*
        reserved
 */
        unsigned int msp_rft_emaa : 3; /* [25..23] */
        // Comment of field: msp_rft_emab
        /*
        reserved
 */
        unsigned int msp_rft_emab : 3; /* [28..26] */
        // Comment of field: msp_rft_emasa
        /*
        reserved
 */
        unsigned int msp_rft_emasa : 1; /* [29] */
        // Comment of field: reserved_3
        /*
        保留。
 */
        unsigned int reserved_3 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_mem_adjust1;

/* Define the union U_PERI_MEM_ADJUST2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: usb_rfs_ema
        /*
        RAM调速值
 */
        unsigned int usb_rfs_ema : 3; /* [2..0] */
        // Comment of field: usb_rfs_emaw
        /*
        RAM调速值
 */
        unsigned int usb_rfs_emaw : 2; /* [4..3] */
        // Comment of field: usb_rfs_wabl
        /*
        RAM调速值
 */
        unsigned int usb_rfs_wabl : 1; /* [5] */
        // Comment of field: usb_rfs_wablm
        /*
        RAM调速值
 */
        unsigned int usb_rfs_wablm : 2; /* [7..6] */
        // Comment of field: msp_ras_ema
        /*
        RAM调速值
 */
        unsigned int msp_ras_ema : 3; /* [10..8] */
        // Comment of field: msp_ras_emaw
        /*
        RAM调速值
 */
        unsigned int msp_ras_emaw : 2; /* [12..11] */
        // Comment of field: msp_ras_emas
        /*
        RAM调速值
 */
        unsigned int msp_ras_emas : 1; /* [13] */
        // Comment of field: msp_ras_wabl
        /*
        RAM调速值
 */
        unsigned int msp_ras_wabl : 1; /* [14] */
        // Comment of field: msp_ras_wablm
        /*
        RAM调速值
 */
        unsigned int msp_ras_wablm : 2; /* [16..15] */
        // Comment of field: msp_ras_stov
        /*
        RAM调速值
 */
        unsigned int msp_ras_stov : 1; /* [17] */
        // Comment of field: msp_ros_ema
        /*
        RAM调速值
 */
        unsigned int msp_ros_ema : 3; /* [20..18] */
        // Comment of field: himdtv_C_RAT_EMAP
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rat_emap : 1; /* [21] */
        // Comment of field: himdtv_C_RAT_EMAS
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rat_emas : 1; /* [22] */
        // Comment of field: himdtv_C_RAT_STOV
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rat_stov : 1; /* [23] */
        // Comment of field: himdtv_C_RAT_STOVAB
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rat_stovab : 1; /* [24] */
        // Comment of field: himdtv_C_RAT_EMAW
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rat_emaw : 2; /* [26..25] */
        // Comment of field: himdtv_C_RAT_EMA
        /*
        RAM调速值
 */
        unsigned int himdtv_c_rat_ema : 3; /* [29..27] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_mem_adjust2;

/* Define the union U_USB_PHY_TEST_OBS */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        usb2 phy3观测信号。
 */
        unsigned int reserved_0 : 8; /* [7..0] */
        // Comment of field: reserved_1
        /*
        usb2 phy2观测信号。
 */
        unsigned int reserved_1 : 8; /* [15..8] */
        // Comment of field: reserved_2
        /*
        usb2 phy1观测信号。
 */
        unsigned int reserved_2 : 8; /* [23..16] */
        // Comment of field: reserved_3
        /*
        usb2 单口phy观测信号。
 */
        unsigned int reserved_3 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_usb_phy_test_obs;

/* Define the union U_ARBIT_CTRL0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dsp_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int dsp_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: dtmb_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int dtmb_arbit_ctrl : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_arbit_ctrl0;

/* Define the union U_ARBIT_CTRL1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cipher_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int cipher_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: aiao_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int aiao_arbit_ctrl : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_arbit_ctrl1;

/* Define the union U_ARBIT_CTRL2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: edma_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int edma_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: m0_64bit_axi_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int m0_64bit_axi_arbit_ctrl : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_arbit_ctrl2;

/* Define the union U_ARBIT_CTRL3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: m0_h2x_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int m0_h2x_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: qtc_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int qtc_arbit_ctrl : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_arbit_ctrl3;

/* Define the union U_ARBIT_CTRL4 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pvr_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int pvr_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: tvd_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int tvd_arbit_ctrl : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_arbit_ctrl4;

/* Define the union U_ARBIT_CTRL5 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vi_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int vi_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: vi_tvd_dtmb_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int vi_tvd_dtmb_arbit_ctrl : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_arbit_ctrl5;

/* Define the union U_ARBIT_CTRL6 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: emmc_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int emmc_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: reserved_0
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int reserved_0 : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_arbit_ctrl6;

/* Define the union U_ARBIT_CTRL7 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ddrt_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int ddrt_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: vdh_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int vdh_arbit_ctrl : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_arbit_ctrl7;

/* Define the union U_ARBIT_CTRL8 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vpss_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int vpss_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: jpgd_arbit_ctrl
        /*
        外部仲裁控制信号。
        bit[15]表示外部仲裁的使能开关；
        bit[14:0]表示仲裁的超时配置参数
 */
        unsigned int jpgd_arbit_ctrl : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_arbit_ctrl8;

/* Define the union U_PERI_SOC_FUSE0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_soc_fuse
        /*
        peri_soc_fuse0[31]:DBX规格控制；
        peri_soc_fuse0[30]:Dolby DD+规格控制；
        peri_soc_fuse0[29]:DTS规格控制；
        peri_soc_fuse0[28]:reserved; Microvison规格控制,0支持1不支持
        peri_soc_fuse0[27]:Dolby DD规格控制；
        peri_soc_fuse0[26:22]: reserved.  ddr颗粒容量；ddr厂家版本；chip_id；
        peri_soc_fuse0[21]:4K分辨率,0支持1不支持
        peri_soc_fuse0[20]:reserved
        peri_soc_fuse0[19]:VP9 10bit规格控制,0支持1不支持
        peri_soc_fuse0[18]:H265 10bit规格控制,0支持1不支持
        peri_soc_fuse[017]:VP9 8bit规格控制,0支持1不支持（详见备注）；
        peri_soc_fuse0[16]:H265 8bit规格控制,0支持1不支持（详见备注）；
        peri_soc_fuse0[15]:MP2规格控制,0支持1不支持；
        peri_soc_fuse0[14]:AVS规格控制，0支持1不支持；
        peri_soc_fuse0[13]:MP4规格控制,0支持1不支持；
        peri_soc_fuse0[12]:H264规格控制，0支持1不支持；
        peri_soc_fuse0[11]: VP8规格控制,0支持1不支持；
        peri_soc_fuse0[10]:VP6规格控制,0支持1不支持；
        peri_soc_fuse0[9]: VC1规格控制,0支持1不支持；
        peri_soc_fuse0[8]: DIVX3规格控制,0支持1不支持；
        peri_soc_fuse0[7]: Real9规格控制,0支持1不支持；
        peri_soc_fuse0[6]:Real8规格控制，0支持1不支持；
        peri_soc_fuse0[5]: reserved
        peri_soc_fuse0[4]:ISDB-T规格控制,0支持1不支持；
        peri_soc_fuse0[3]: J83B规格控制，0支持1不支持；
        peri_soc_fuse0[2]: DVBC规格控制,0支持1不支持；
        peri_soc_fuse0[1]: DTMB规格控制,0支持1不支持；  ( otp [129] )
        peri_soc_fuse0[0]: ATSC规格控制,0支持1不支持；  ( otp [128] )
 */
        unsigned int peri_soc_fuse : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_soc_fuse0;
/* Define the union U_PERI_SOC_FUSE1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_soc_fuse1
        /*
        peri_soc_fuse1[31]:uart0规格，0支持1不支持；    （otp[287]）
        peri_soc_fuse1[30]:uart1规格，0支持1不支持；
        peri_soc_fuse1[29]:jtag规格，0支持1不支持；
        peri_soc_fuse1[28]:bootrom是否支持UART0打印，0不支持1支持；（bootrom）
        peri_soc_fuse1[27]:emmc_bootrom_freq，0为25M；1为400K   （bootrom）
        peri_soc_fuse1[26]:emmc省70ms启动时间规格控制
        peri_soc_fuse1[25:24]:reserved
        peri_soc_fuse1[23:11]：reserved  (不需要fephy规格控制；)
        peri_soc_fuse1[10]: soc_fuse_mc_info：0 cipher内部key寄存器不可读 1 cipher内部key寄存器不支持；
        peri_soc_fuse1[9]:p2p规格：0支持1不支持；
        peri_soc_fuse1[8]:mLVDS规格：0支持1不支持；
        peri_soc_fuse1[7]:LVDS规格：0支持1不支持；
        peri_soc_fuse1[6:0]:给音频预留；
 */
        unsigned int peri_soc_fuse1 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_soc_fuse1;
/* Define the union U_PERI_SOC_FUSE2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_soc_fuse2
        /*
        reserved
 */
        unsigned int peri_soc_fuse2 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_soc_fuse2;
/* Define the union U_PERI_SOC_FUSE3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_soc_fuse3
        /*
        reserved
 */
        unsigned int peri_soc_fuse3 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_soc_fuse3;
/* Define the union U_PERI_SW_OTP_0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_sw_otp_0
        /*
        软件写efuse
        软件otp[31:0]
 */
        unsigned int peri_sw_otp_0 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_sw_otp_0;
/* Define the union U_PERI_SW_OTP_1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_sw_otp_1
        /*
        软件写efuse
        软件otp[63:32]
 */
        unsigned int peri_sw_otp_1 : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_sw_otp_1;
/* Define the union U_PERI_DW_BRIDGE_PRIORITY0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cipher_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int cipher_port_pri_in : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: aiao_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int aiao_port_pri_in : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: dsp_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int dsp_port_pri_in : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: dtmb_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int dtmb_port_pri_in : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: edma_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int edma_port_pri_in : 3; /* [18..16] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [19] */
        // Comment of field: emmc_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int emmc_port_pri_in : 3; /* [22..20] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [23] */
        // Comment of field: m0_64bit_axi_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int m0_64bit_axi_port_pri_in : 3; /* [26..24] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [27] */
        // Comment of field: m0_h2x_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int m0_h2x_port_pri_in : 3; /* [30..28] */
        // Comment of field: reserved_7
        /*
        保留。
 */
        unsigned int reserved_7 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dw_bridge_priority0;

/* Define the union U_PERI_DW_BRIDGE_PRIORITY1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: qtc_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int qtc_port_pri_in : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: pvr_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int pvr_port_pri_in : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: tvd_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int tvd_port_pri_in : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: vi_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int vi_port_pri_in : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: vi_tvd_dtmb_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int vi_tvd_dtmb_port_pri_in : 3; /* [18..16] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [19] */
        // Comment of field: reserved_5
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int reserved_5 : 3; /* [22..20] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [23] */
        // Comment of field: reserved_7
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int reserved_7 : 3; /* [26..24] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [27] */
        // Comment of field: reserved_9
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int reserved_9 : 3; /* [30..28] */
        // Comment of field: reserved_10
        /*
        保留。
 */
        unsigned int reserved_10 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dw_bridge_priority1;

/* Define the union U_PERI_DW_BRIDGE_PRIORITY2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vpss_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int vpss_port_pri_in : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: jpgd_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int jpgd_port_pri_in : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: ddrt_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int ddrt_port_pri_in : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: vdh_port_pri_in
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int vdh_port_pri_in : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: reserved_4
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int reserved_4 : 3; /* [18..16] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [19] */
        // Comment of field: reserved_6
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int reserved_6 : 3; /* [22..20] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [23] */
        // Comment of field: reserved_8
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int reserved_8 : 3; /* [26..24] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [27] */
        // Comment of field: reserved_10
        /*
        总线桥优先级配置寄存器
        7优先级最高； 
        ~
        0优先级低；
 */
        unsigned int reserved_10 : 3; /* [30..28] */
        // Comment of field: reserved_11
        /*
        保留。
 */
        unsigned int reserved_11 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_dw_bridge_priority2;

/* Define the union U_PERI_GPU_SEC */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: gpu_dbgen
        /*
        reserved
 */
        unsigned int gpu_dbgen : 1; /* [0] */
        // Comment of field: gpu_niden
        /*
        reserved
 */
        unsigned int gpu_niden : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        保留。
 */
        unsigned int reserved_0 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_gpu_sec;

/* Define the union U_VBI_DEBUG_REG */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: peri_vbi_aif_sel_dbgen
        /*
        vbi_aif_sel debug信号。
 */
        unsigned int peri_vbi_aif_sel_dbgen : 1; /* [0] */
        // Comment of field: peri_qamadc_aif_vdac_sel
        /*
        equator vdac选择信号。测试使用。
        2'b00：AIF输出的cvbs数据；
        2'b01：QAMADC_1CH输出的ADC数据；
        2'b00：reserved
        2'b01：reserved
 */
        unsigned int peri_qamadc_aif_vdac_sel : 2; /* [2..1] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 29; /* [31..3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vbi_debug_reg;

/* Define the union U_PERI_PUSH_HURRY4 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        NOC优先级控制信号
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        NOC优先级控制信号
 */
        unsigned int reserved_2 : 3; /* [6..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        NOC优先级控制信号
 */
        unsigned int reserved_4 : 3; /* [10..8] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [11] */
        // Comment of field: reserved_6
        /*
        NOC优先级控制信号
 */
        unsigned int reserved_6 : 3; /* [14..12] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [15] */
        // Comment of field: reserved_8
        /*
        NOC优先级控制信号
 */
        unsigned int reserved_8 : 3; /* [18..16] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [19] */
        // Comment of field: reserved_10
        /*
        NOC优先级控制信号
 */
        unsigned int reserved_10 : 3; /* [22..20] */
        // Comment of field: reserved_11
        /*
        reserved
 */
        unsigned int reserved_11 : 1; /* [23] */
        // Comment of field: reserved_12
        /*
        NOC优先级控制信号
 */
        unsigned int reserved_12 : 3; /* [26..24] */
        // Comment of field: reserved_13
        /*
        reserved
 */
        unsigned int reserved_13 : 1; /* [27] */
        // Comment of field: reserved_14
        /*
        NOC优先级控制信号
 */
        unsigned int reserved_14 : 3; /* [30..28] */
        // Comment of field: reserved_15
        /*
        保留。
 */
        unsigned int reserved_15 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_push_hurry4;

/* ============================================================================== */
/* Define the global struct */
typedef struct {
    volatile u_start_mode start_mode;                                         /* 0x0 */
    volatile u_peri_stat peri_stat;                                           /* 0x4 */
    volatile u_peri_ctrl peri_ctrl;                                           /* 0x8 */
    volatile u_cpu_stat cpu_stat;                                             /* 0xc */
    volatile unsigned int reserved_1[2];                                      /* 2 is reserved reg size,0x10~0x14 */
    volatile u_peri_sec_stat peri_sec_stat;                                   /* 0x18 */
    volatile u_peri_emmc_lp peri_emmc_lp;                                     /* 0x1c */
    volatile unsigned int reserved_2[37];                                     /* 37 is reserved reg size,0x20~0xb0 */
    volatile u_peri_usb_resume_int_mask peri_usb_resume_int_mask;             /* 0xb4 */
    volatile u_peri_usb_resume_int_rawstat peri_usb_resume_int_rawstat;       /* 0xb8 */
    volatile u_peri_usb_resume_int_stat peri_usb_resume_int_stat;             /* 0xbc */
    volatile u_peri_int_mce peri_int_mce;                                     /* 0xc0 */
    volatile unsigned int reserved_3[2];                                      /* 2 is reserved reg size,0xc4~0xc8 */
    volatile u_peri_int_dsp peri_int_dsp;                                     /* 0xcc */
    volatile u_peri_int_dsp1_toa9 peri_int_dsp1_toa9;                         /* 0xd0 */
    volatile unsigned int reserved_4[4];                                      /* 4 is reserved reg size,0xd4~0xe0 */
    volatile u_peri_int_swi0 peri_int_swi0;                                   /* 0xe4 */
    volatile u_peri_int_swi1 peri_int_swi1;                                   /* 0xe8 */
    volatile u_peri_int_swi2 peri_int_swi2;                                   /* 0xec */
    volatile unsigned int reserved_5[5];                                      /* 5 is reserved reg size,0xf0~0x100 */
    volatile u_peri_qam_adc0 peri_qam_adc0;                                   /* 0x104 */
    volatile u_peri_qam_adc1 peri_qam_adc1;                                   /* 0x108 */
    volatile u_peri_qam_adc2 peri_qam_adc2;                                   /* 0x10c */
    volatile unsigned int fpga_ctrl0;                                         /* 0x110 */
    volatile unsigned int fpga_ctrl1;                                         /* 0x114 */
    volatile unsigned int reserved_6;                                         /* 0x118 */
    volatile u_peri_sys_pll_ldo peri_sys_pll_ldo;                             /* 0x11c */
    volatile u_peri_usb2_reg1 peri_usb2_reg1;                                 /* 0x120 */
    volatile unsigned int reserved_7[8];                                      /* 8 is reserved reg size,0x124~0x140 */
    volatile u_peri_usb2_reg0 peri_usb2_reg0;                                 /* 0x144 */
    volatile unsigned int reserved_8[11];                                     /* 11 is reserved reg size,0x148~0x170 */
    volatile u_uart_dbg_cfg uart_dbg_cfg;                                     /* 0x174 */
    volatile u_dsp_ctrl_common dsp_ctrl_common;                               /* 0x178 */
    volatile u_aiao_dsp_cfg_com aiao_dsp_cfg_com;                             /* 0x17c */
    volatile u_dsp_ctrl1 dsp_ctrl1;                                           /* 0x180 */
    volatile u_dsp_ctrl0 dsp_ctrl0;                                           /* 0x184 */
    volatile u_dsp_status1 dsp_status1;                                       /* 0x188 */
    volatile u_dsp_status0 dsp_status0;                                       /* 0x18c */
    volatile unsigned int reserved_9[9];                                      /* 9 is reserved reg size,0x190~0x1b0 */
    volatile u_peri_ddrphy_test0 peri_ddrphy_test0;                           /* 0x1b4 */
    volatile unsigned int reserved_10[10];                                    /* 10 is reserved reg size,0x1b8~0x1dc */
    volatile u_peri_chip_info4 peri_chip_info4;                               /* 0x1e0 */
    volatile unsigned int reserved_11[3];                                     /* 3 is reserved reg size,0x1e4~0x1ec */
    volatile u_peri_sw_set peri_sw_set;                                       /* 0x1f0 */
    volatile unsigned int reserved_12[3];                                     /* 3 is reserved reg size,0x1f4~0x1fc */
    volatile u_peri_qos_cfg00 peri_qos_cfg00;                                 /* 0x200 */
    volatile u_peri_qos_cfg01 peri_qos_cfg01;                                 /* 0x204 */
    volatile u_peri_qos_cfg02 peri_qos_cfg02;                                 /* 0x208 */
    volatile u_peri_qos_cfg03 peri_qos_cfg03;                                 /* 0x20c */
    volatile u_peri_qos_cfg04 peri_qos_cfg04;                                 /* 0x210 */
    volatile u_peri_qos_cfg05 peri_qos_cfg05;                                 /* 0x214 */
    volatile u_peri_qos_cfg06 peri_qos_cfg06;                                 /* 0x218 */
    volatile u_peri_qos_cfg07 peri_qos_cfg07;                                 /* 0x21c */
    volatile u_peri_qos_cfg08 peri_qos_cfg08;                                 /* 0x220 */
    volatile u_peri_qos_cfg09 peri_qos_cfg09;                                 /* 0x224 */
    volatile u_peri_qos_cfg10 peri_qos_cfg10;                                 /* 0x228 */
    volatile u_peri_qos_cfg11 peri_qos_cfg11;                                 /* 0x22c */
    volatile unsigned int reserved_13[116];                                   /* 116 is reserved reg size,0x230~0x3fc */
    volatile u_peri_dsp0_0 peri_dsp0_0;                                       /* 0x400 */
    volatile u_peri_dsp0_1 peri_dsp0_1;                                       /* 0x404 */
    volatile u_peri_dsp0_2 peri_dsp0_2;                                       /* 0x408 */
    volatile u_peri_dsp0_3 peri_dsp0_3;                                       /* 0x40c */
    volatile u_peri_dsp0_4 peri_dsp0_4;                                       /* 0x410 */
    volatile u_peri_dsp0_5 peri_dsp0_5;                                       /* 0x414 */
    volatile u_peri_dsp0_6 peri_dsp0_6;                                       /* 0x418 */
    volatile u_peri_dsp0_7 peri_dsp0_7;                                       /* 0x41c */
    volatile unsigned int reserved_14[248];                                   /* 248 is reserved reg size,0x420~0x7fc */
    volatile u_peri_dsp1_0 peri_dsp1_0;                                       /* 0x800 */
    volatile u_peri_dsp1_1 peri_dsp1_1;                                       /* 0x804 */
    volatile u_peri_dsp1_2 peri_dsp1_2;                                       /* 0x808 */
    volatile u_peri_dsp1_3 peri_dsp1_3;                                       /* 0x80c */
    volatile u_peri_dsp1_4 peri_dsp1_4;                                       /* 0x810 */
    volatile u_peri_dsp1_5 peri_dsp1_5;                                       /* 0x814 */
    volatile u_peri_dsp1_6 peri_dsp1_6;                                       /* 0x818 */
    volatile u_peri_dsp1_7 peri_dsp1_7;                                       /* 0x81c */
    volatile unsigned int reserved_15[7];                                     /* 7 is reserved reg size,0x820~0x838 */
    volatile u_peri_od_ctrl peri_od_ctrl;                                     /* 0x83c */
    volatile unsigned int reserved_16[2];                                     /* 2 is reserved reg size,0x840~0x844 */
    volatile u_qtc_mode qtc_mode;                                             /* 0x848 */
    volatile unsigned int reserved_17[3];                                     /* 3 is reserved reg size,0x84c~0x854 */
    volatile u_mode_atv mode_atv;                                             /* 0x858 */
    volatile u_vdac_equa_sel vdac_equa_sel;                                   /* 0x85c */
    volatile u_hdmitx_mhlnx_clk_sel hdmitx_mhlnx_clk_sel;                     /* 0x860 */
    volatile u_peri_agc_sel peri_agc_sel;                                     /* 0x864 */
    volatile unsigned int reserved_18[10];                                    /* 10 is reserved reg size,0x868~0x88c */
    volatile u_peri_aiao_rx1_sd_sel peri_aiao_rx1_sd_sel;                     /* 0x890 */
    volatile u_peri_qtc_equa_sel peri_qtc_equa_sel;                           /* 0x894 */
    volatile u_peri_pending0 peri_pending0;                                   /* 0x898 */
    volatile u_peri_pending1 peri_pending1;                                   /* 0x89c */
    volatile u_peri_pending2 peri_pending2;                                   /* 0x8a0 */
    volatile unsigned int reserved_19[2];                                     /* 2 is reserved reg size,0x8a4~0x8a8 */
    volatile u_peri_io_oen peri_io_oen;                                       /* 0x8ac */
    volatile u_peri_hdmirx peri_hdmirx;                                       /* 0x8b0 */
    volatile u_hdmi_rx_2_p0_cfg hdmi_rx_2_p0_cfg;                             /* 0x8b4 */
    volatile u_hdmi_rx_2_p0_start hdmi_rx_2_p0_start;                         /* 0x8b8 */
    volatile u_gpu_ctrl gpu_ctrl;                                             /* 0x8bc */
    volatile unsigned int reserved_20[2];                                     /* 2 is reserved reg size,0x8c0~0x8c4 */
    volatile u_eth_fephy_ctr eth_fephy_ctr;                                   /* 0x8c8 */
    volatile u_peri_equator_hadd peri_equator_hadd;                           /* 0x8cc */
    volatile u_peri_usb3_state peri_usb3_state;                               /* 0x8d0 */
    volatile u_peri_usb3_test peri_usb3_test;                                 /* 0x8d4 */
    volatile u_peri_usb3_mode peri_usb3_mode;                                 /* 0x8d8 */
    volatile u_peri_dtmb_ctrl peri_dtmb_ctrl;                                 /* 0x8dc */
    volatile u_peri_ddr_bus_en peri_ddr_bus_en;                               /* 0x8e0 */
    volatile unsigned int reserved_21;                                        /* 0x8e4 */
    volatile u_peri_powen_out peri_powen_out;                                 /* 0x8e8 */
    volatile u_peri_vbi_ctrl peri_vbi_ctrl;                                   /* 0x8ec */
    volatile unsigned int reserved_22;                                        /* 0x8f0 */
    volatile unsigned int fpga_ctrl2;                                         /* 0x8f4 */
    volatile unsigned int reserved_23;                                        /* 0x8f8 */
    volatile u_test_noc_pwrdisctargpwractive test_noc_pwrdisctargpwractive;   /* 0x8fc */
    volatile unsigned int reserved_24;                                        /* 0x900 */
    volatile u_peri_hdmirx_reg_bar_pattern_a0 peri_hdmirx_reg_bar_pattern_a0; /* 0x904 */
    volatile u_peri_hdmirx_reg_bar_pattern_a1 peri_hdmirx_reg_bar_pattern_a1; /* 0x908 */
    volatile u_peri_hdmirx_reg_bar_pattern_a2 peri_hdmirx_reg_bar_pattern_a2; /* 0x90c */
    volatile u_peri_hdmirx_reg_bar_pattern_a3 peri_hdmirx_reg_bar_pattern_a3; /* 0x910 */
    volatile u_peri_hdmirx_reg_bar_pattern_a4 peri_hdmirx_reg_bar_pattern_a4; /* 0x914 */
    volatile u_peri_hdmirx_reg_bar_pattern_a5 peri_hdmirx_reg_bar_pattern_a5; /* 0x918 */
    volatile u_peri_hdmirx_reg_bar_pattern_a6 peri_hdmirx_reg_bar_pattern_a6; /* 0x91c */
    volatile u_peri_hdmirx_reg_bar_pattern_a7 peri_hdmirx_reg_bar_pattern_a7; /* 0x920 */
    volatile u_peri_hdmirx_reg_vid_format peri_hdmirx_reg_vid_format;         /* 0x924 */
    volatile u_peri_hdmirx_reg_pat_sel peri_hdmirx_reg_pat_sel;               /* 0x928 */
    volatile u_peri_hdmirx_reg_cbar_wid_sel peri_hdmirx_reg_cbar_wid_sel;     /* 0x92c */
    volatile u_peri_hdmi2_vi_adapter peri_hdmi2_vi_adapter;                   /* 0x930 */
    volatile u_peri_ssp0_ldc_dma_req_sel peri_ssp0_ldc_dma_req_sel;           /* 0x934 */
    volatile u_peri_ssp1_ldc_dma_req_sel peri_ssp1_ldc_dma_req_sel;           /* 0x938 */
    volatile unsigned int reserved_25;                                        /* 0x93c */
    volatile u_peri_qos_cfg_09 peri_qos_cfg_09;                               /* 0x940 */
    volatile u_peri_i2_s0_din1_sel peri_i2_s0_din1_sel;                       /* 0x944 */
    volatile unsigned int reserved_26;                                        /* 0x948 */
    volatile u_peri_qos_cfg_12 peri_qos_cfg_12;                               /* 0x94c */
    volatile u_peri_i2_s1_mclk_oen peri_i2_s1_mclk_oen;                       /* 0x950 */
    volatile u_peri_sc_ctrl peri_sc_ctrl;                                     /* 0x954 */
    volatile u_spi0_spi1 spi0_spi1;                                           /* 0x958 */
    volatile u_glp_ip_sel glp_ip_sel;                                         /* 0x95c */
    volatile u_sdioo_ctrl sdioo_ctrl;                                         /* 0x960 */
    volatile u_io_ctrl io_ctrl;                                               /* 0x964 */
    volatile u_peri_qam_ldo peri_qam_ldo;                                     /* 0x968 */
    volatile unsigned int reserved_27;                                        /* 0x96c */
    volatile u_peri_misc_contrl peri_misc_contrl;                             /* 0x970 */
    volatile u_pcie_phy_ctrl pcie_phy_ctrl;                                   /* 0x974 */
    volatile u_ufs_pad_ctrl ufs_pad_ctrl;                                     /* 0x978 */
    volatile unsigned int reserved_28;                                        /* 0x97c */
    volatile u_tde_state tde_state;                                           /* 0x980 */
    volatile u_peri_ts_sel_cfg peri_ts_sel_cfg;                               /* 0x984 */
    volatile u_peri_usb_pull peri_usb_pull;                                   /* 0x988 */
    volatile u_peri_usb2_p3 peri_usb2_p3;                                     /* 0x98c */
    volatile u_edma_high_addr_ctrl edma_high_addr_ctrl;                       /* 0x990 */
    volatile u_peri_mem_adjust0 peri_mem_adjust0;                             /* 0x994 */
    volatile u_peri_mem_adjust1 peri_mem_adjust1;                             /* 0x998 */
    volatile u_peri_mem_adjust2 peri_mem_adjust2;                             /* 0x99c */
    volatile u_usb_phy_test_obs usb_phy_test_obs;                             /* 0x9a0 */
    volatile unsigned int reserved_29[23];                                    /* 23 is reserved reg size,0x9a4~0x9fc */
    volatile u_arbit_ctrl0 arbit_ctrl0;                                       /* 0xa00 */
    volatile u_arbit_ctrl1 arbit_ctrl1;                                       /* 0xa04 */
    volatile u_arbit_ctrl2 arbit_ctrl2;                                       /* 0xa08 */
    volatile u_arbit_ctrl3 arbit_ctrl3;                                       /* 0xa0c */
    volatile u_arbit_ctrl4 arbit_ctrl4;                                       /* 0xa10 */
    volatile u_arbit_ctrl5 arbit_ctrl5;                                       /* 0xa14 */
    volatile u_arbit_ctrl6 arbit_ctrl6;                                       /* 0xa18 */
    volatile u_arbit_ctrl7 arbit_ctrl7;                                       /* 0xa1c */
    volatile u_arbit_ctrl8 arbit_ctrl8;                                       /* 0xa20 */
    volatile unsigned int reserved_30[23];                                    /* 23 is reserved reg size,0xa24~0xa7c */
    volatile u_peri_soc_fuse0 peri_soc_fuse0;                                 /* 0xa80 */
    volatile u_peri_soc_fuse1 peri_soc_fuse1;                                 /* 0xa84 */
    volatile u_peri_soc_fuse2 peri_soc_fuse2;                                 /* 0xa88 */
    volatile u_peri_soc_fuse3 peri_soc_fuse3;                                 /* 0xa8c */
    volatile unsigned int reserved_31[4];                                     /* 4 is reserved reg size,0xa90~0xa9c */
    volatile u_peri_sw_otp_0 peri_sw_otp_0;                                   /* 0xaa0 */
    volatile u_peri_sw_otp_1 peri_sw_otp_1;                                   /* 0xaa4 */
    volatile unsigned int reserved_32[22];                                    /* 22 is reserved reg size,0xaa8~0xafc */
    volatile u_peri_dw_bridge_priority0 peri_dw_bridge_priority0;             /* 0xb00 */
    volatile u_peri_dw_bridge_priority1 peri_dw_bridge_priority1;             /* 0xb04 */
    volatile u_peri_dw_bridge_priority2 peri_dw_bridge_priority2;             /* 0xb08 */
    volatile unsigned int reserved_33[193];                                   /* 193 is reserved reg size,0xb0c~0xe0c */
    volatile u_peri_gpu_sec peri_gpu_sec;                                     /* 0xe10 */
    volatile unsigned int reserved_34[5];                                     /* 5 is reserved reg size,0xe14~0xe24 */
    volatile u_vbi_debug_reg vbi_debug_reg;                                   /* 0xe28 */
    volatile u_peri_push_hurry4 peri_push_hurry4;                             /* 0xe2c */
} s_perictrl_regs_type;

#endif /* __HI_REG_PERI_H__ */
