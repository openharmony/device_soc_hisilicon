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
        sfc_addr_mode ????SFC??¿¿¿¿????
        0?? 3byte??¿¿¿??
        1?? 4byte??¿¿¿??
 */
        unsigned int reserved_4 : 1; /* [11] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 8; /* [19..12] */
        // Comment of field: romboot_sel
        /*
        ¿¿?¿?¿¿¿??
 */
        unsigned int romboot_sel : 1; /* [20] */
        // Comment of field: reserved_6
        /*
        l?¿?¿??¿???¿?chipset_info[7]
 */
        unsigned int reserved_6 : 1; /* [21] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [22] */
        // Comment of field: reserved_8
        /*
        ?¿? JTAG_SEL ¿¿¿¿?¿?
        1??¿?ô¿¿¿?4??jtag_mux¿???¿¿?
        0??¿??peri_ctrl???ù?4jtag_mux¿???¿¿?
 */
        unsigned int reserved_8 : 1; /* [23] */
        // Comment of field: reserved_9
        /*
        JTAG_MUX¿??jtag¿¿¿¿?¿?
        3'b000??¿??ARM JTAG??
        3'b010??¿??DSP JTAG??
        3'b100??reserved
        3'b110??reserved
 */
        unsigned int reserved_9 : 3; /* [26..24] */
        // Comment of field: reserved_10
        /*
        reserved
 */
        unsigned int reserved_10 : 1; /* [27] */
        // Comment of field: reserved_11
        /*
        ??????
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
        ??????
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 2; /* [2..1] */
        // Comment of field: reserved_2
        /*
        ??????
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
        ??????
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ??????
        SDIO0(SD???????????¿¿??¿??¿?
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 8; /* [9..2] */
        // Comment of field: reserved_3
        /*
        ????
 */
        unsigned int reserved_3 : 1; /* [10] */
        // Comment of field: reserved_4
        /*
        ??????
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
        ??????
        ?????????¿¿????¿??¿¿??CMOS??OD??¿??
        0:?????¿
        1???¿??¿
 */
        unsigned int reserved_5 : 1; /* [16] */
        // Comment of field: reserved_6
        /*
        ????
 */
        unsigned int reserved_6 : 1; /* [17] */
        // Comment of field: reserved_7
        /*
        ????
 */
        unsigned int reserved_7 : 1; /* [18] */
        // Comment of field: reserved_8
        /*
        ????
 */
        unsigned int reserved_8 : 10; /* [28..19] */
        // Comment of field: peri_jtagsel
        /*
        FUNC_SEL¿0¿??JTAG?¿?¿???¿¿?
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
        A53 MP Core0 SMP??AMP¿¿?¿¿?
        0?? SMP¿¿??
        1?? AMP¿¿??
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        A53 MP Core1 SMP??AMP¿¿?¿¿?
        0?? SMP¿¿??
        1?? AMP¿¿??
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        core2_smpnamp
        A53 MP Core2 SMP??AMP¿¿?¿¿?
        0?? SMP¿¿??
        1?? AMP¿¿??
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        core3_smpnamp
        A53 MP Core3 SMP??AMP¿¿?¿¿?
        0?? SMP¿¿??
        1?? AMP¿¿??
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        ??????
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
        ????
 */
        unsigned int reserved_0 : 4; /* [3..0] */
        // Comment of field: reserved_1
        /*
        ????
 */
        unsigned int reserved_1 : 4; /* [7..4] */
        // Comment of field: reserved_2
        /*
        ??????
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
        ??????
        EMMC IO????¿¿¿??
        0??normal mode
        1??isolation mode
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ??????
        EMMC ??¿IO?¿?????retention¿¿
        0??normal mode
        1??isolation mode
 */
        unsigned int reserved_1 : 6; /* [6..1] */
        // Comment of field: reserved_2
        /*
        ??????
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
        USB PHY?????¿?mask????¿
        bit3: reserved
        bit2??USB2  PHY2 ??
        bit1??USB2  PHY1 ??
        bit0??USB2  PHY0??
        0 mask?¿¿?1??mask??
 */
        unsigned int usb_suspend_int_mask : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        ??????
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
        USB PHY suspend?¿?¿¿¿¿?¿???
        bit3: reserved
        bit2??USB2  PHY2 ??
        bit1??USB2  PHY1 ??
        bit0??USB2  PHY0??
 */
        unsigned int usb_suspend_int_rawstat : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        ??????
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
        USB PHY suspend?¿?¿¿?¿???
        bit3: reserved
        bit2??USB2  PHY2 ??
        bit1??USB2  PHY1 ??
        bit0??USB2  PHY0??
 */
        unsigned int usb_suspend_int_stat : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        ??????
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
        mce?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int int_swi_cputomce : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
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
        DSP???¿?¿???¿?¿VDH????¿?õ¿?????
        0???¿?¿?¿???¿DSP?ã?
        1???¿?¿?¿?VDH DSP?????á?
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        DSP???¿?¿???¿?¿VDH????¿?õ¿?????
        0???¿?¿?¿???¿DSP?ã?
        1???¿?¿?¿?VDH DSP?????á?
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: int_swi0_dsp0
        /*
        dsp?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int int_swi0_dsp0 : 1; /* [2] */
        // Comment of field: int_swi1_dsp0
        /*
        dsp?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int int_swi1_dsp0 : 1; /* [3] */
        // Comment of field: int_swi2_dsp0
        /*
        dsp?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int int_swi2_dsp0 : 1; /* [4] */
        // Comment of field: int_swi3_dsp0
        /*
        dsp?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int int_swi3_dsp0 : 1; /* [5] */
        // Comment of field: reserved_2
        /*
        dsp?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int reserved_2 : 1; /* [6] */
        // Comment of field: reserved_3
        /*
        dsp?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        dsp?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int reserved_4 : 1; /* [8] */
        // Comment of field: reserved_5
        /*
        dsp?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int reserved_5 : 1; /* [9] */
        // Comment of field: reserved_6
        /*
        ??????
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
        dsp??cpu???¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        dsp??cpu???¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        ??????
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
        cpu?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int int_swi0_cpu : 1; /* [0] */
        // Comment of field: int_swi1_cpu
        /*
        cpu?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int int_swi1_cpu : 1; /* [1] */
        // Comment of field: int_swi2_cpu
        /*
        cpu?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int int_swi2_cpu : 1; /* [2] */
        // Comment of field: int_swi3_cpu
        /*
        cpu?????¿¿?
        0?????¿¿?
        1?????¿???????
 */
        unsigned int int_swi3_cpu : 1; /* [3] */
        // Comment of field: reserved_0
        /*
        ??????
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
        a73??swi1?¿¿?
        0?????¿¿?
        1?????¿???????
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
} u_peri_int_swi1;

/* Define the union U_PERI_INT_SWI2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        a73??swi2?¿¿?
        0?????¿¿?
        1?????¿???????
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
} u_peri_int_swi2;

/* Define the union U_PERI_QAM_ADC0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ????¿¿??
        opm[1??0]=00 ???¿?¿¿??
        opm[1??0]=01??10 ?? ?¿???¿¿??
        opm[1??0]=11?? ????????¿¿??
 */
        unsigned int reserved_0 : 2; /* [1..0] */
        // Comment of field: reserved_1
        /*
        se_prev_f=0 ??¿??¿¿??ADC?¿?¿?¿?
        use_prev_f=1 ??¿??¿¿???¿??????¿?¿¿¿ ??
 */
        unsigned int reserved_1 : 1; /* [2] */
        // Comment of field: reserved_2
        /*
        PING PONG¿¿¿?¿??¿?¿??¿??
 */
        unsigned int reserved_2 : 1; /* [3] */
        // Comment of field: reserved_3
        /*
        ????¿????¿??
        0?????¿?I·???????¿??¿??¿?????¿I·???¿?
        1?????¿?IQ}·¿¿???¿??¿?
 */
        unsigned int reserved_3 : 1; /* [4] */
        // Comment of field: reserved_4
        /*
        ¿?¿¿???¿?¿??¿?¿??¿?????¿?¿¿?¿¿??
 */
        unsigned int reserved_4 : 1; /* [5] */
        // Comment of field: reserved_5
        /*
        ¿?ÿ????¿?
 */
        unsigned int reserved_5 : 5; /* [10..6] */
        // Comment of field: reserved_6
        /*
        ????????????¿?¿?
        endout=1 ?????0??
        endout=0????????????¿?
 */
        unsigned int reserved_6 : 1; /* [11] */
        // Comment of field: reserved_7
        /*
        ??????¿¿????
        selof=0? ?????¿?
        selof=1??????¿??? ??
 */
        unsigned int reserved_7 : 1; /* [12] */
        // Comment of field: peri_qamadc_startcal
        /*
        ????¿¿???¿??¿?¿??¿??
 */
        unsigned int peri_qamadc_startcal : 1; /* [13] */
        // Comment of field: reserved_8
        /*
        peri_qamadc_bcal
        ????¿¿??¿?????¿?
        bcal[1??0]=00 ?????¿¿¿¿??
        bcal[1??0]=01 ?¿¿????¿¿???¿???¿¿?¿? ??
        bcal[1??0]=10 ??¿???
        bcal[1??0]=11 ?¿¿????¿¿????????¿¿?¿? ??
 */
        unsigned int reserved_8 : 2; /* [15..14] */
        // Comment of field: reserved_9
        /*
        peri_qamadc_fsctrl
        fsctrl ?????????¿????????????¿???????1x~1.996x¿?¿??256???¿???
        fsctrl[7:0]=0x00 ?? 1x ??
        fsctrl[7:0]=0xFF ?? 1.996x ??
 */
        unsigned int reserved_9 : 8; /* [23..16] */
        // Comment of field: peri_qamadc_adcrdy
        /*
        ADC ready?¿¿??¿?¿??¿ADC???¿?¿??????????
 */
        unsigned int peri_qamadc_adcrdy : 1; /* [24] */
        // Comment of field: reserved_10
        /*
        peri_qamadc_bitosync
        ??ppsel=1??chsel=0¿¿?ã????¿?¿??¿??bit??????¿?!?
        0?? ??¿I·??????
        1????¿Q·??????
 */
        unsigned int reserved_10 : 1; /* [25] */
        // Comment of field: reserved_11
        /*
        peri_qamadc_rfalgi
        i·???¿???¿¿??
 */
        unsigned int reserved_11 : 3; /* [28..26] */
        // Comment of field: reserved_12
        /*
        peri_qamadc_rfalgq
        q·???¿???¿¿??
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
        QAM ADC?¿??¿??
 */
        unsigned int peri_qamadc_i2c_devaddr : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        ?????¿?¿??¿?¿?
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_0 : 1; /* [7] */
        // Comment of field: peri_qamadc_i2c_rst_n
        /*
        ?¿????¿?·reset???¿?¿??¿
 */
        unsigned int peri_qamadc_i2c_rst_n : 1; /* [8] */
        // Comment of field: reserved_1
        /*
        QAM ADC debug?¿?????
        QAMADC 1CH:
        bit9:envcmout
        bit10:enaccoupling
        QAMADC 2CH??
        bit11:envcmout i¿??
        bit12:enaccoupling i¿??
        bit13:envcmout q¿??
        bit14:enaccoupling q¿??
        ????QAMADC datasheet
 */
        unsigned int reserved_1 : 6; /* [14..9] */
        unsigned int reserved_2 : 1; /* [15] */
        // Comment of field: peri_qamadc_pd
        /*
        ????¿??IP?¿?
 */
        unsigned int peri_qamadc_pd : 1; /* [16] */
        // Comment of field: peri_qamadc_rst_n
        /*
        ????¿??IP?¿?
 */
        unsigned int peri_qamadc_rst_n : 1; /* [17] */
        // Comment of field: peri_qamadc_mode_sel
        /*
        ????¿??IP?¿?
 */
        unsigned int peri_qamadc_mode_sel : 3; /* [20..18] */
        // Comment of field: reserved_3
        /*
        ??????
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
        QAM ADC?¿??¿??
 */
        unsigned int reserved_0 : 7; /* [6..0] */
        // Comment of field: reserved_1
        /*
        QAM ADC?¿??¿??
 */
        unsigned int reserved_1 : 7; /* [13..7] */
        // Comment of field: reserved_2
        /*
        ?¿????¿?·reset???¿?¿??¿
 */
        unsigned int reserved_2 : 1; /* [14] */
        // Comment of field: reserved_3
        /*
        ?¿????¿?·reset???¿?¿??¿
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: reserved_4
        /*
        ADC ready?¿¿??¿?¿??¿ADC???¿?¿??????????
 */
        unsigned int reserved_4 : 1; /* [16] */
        // Comment of field: reserved_5
        /*
        ADC ready?¿¿??¿?¿??¿ADC???¿?¿??????????
 */
        unsigned int reserved_5 : 1; /* [17] */
        // Comment of field: reserved_6
        /*
        ????¿¿??
        opm[1??0]=00 ???¿?¿¿??
        opm[1??0]=01??10 ?? ?¿???¿¿??
        opm[1??0]=11?? ????????¿¿??
 */
        unsigned int reserved_6 : 2; /* [19..18] */
        // Comment of field: reserved_7
        /*
        ????¿¿??
        opm[1??0]=00 ???¿?¿¿??
        opm[1??0]=01??10 ?? ?¿???¿¿??
        opm[1??0]=11?? ????????¿¿??
 */
        unsigned int reserved_7 : 2; /* [21..20] */
        // Comment of field: reserved_8
        /*
        ??????¿¿????
        selof=0? ?????¿?
        selof=1??????¿??? ??
 */
        unsigned int reserved_8 : 1; /* [22] */
        // Comment of field: reserved_9
        /*
        ??????¿¿????
        selof=0? ?????¿?
        selof=1??????¿??? ??
 */
        unsigned int reserved_9 : 1; /* [23] */
        // Comment of field: reserved_10
        /*
        ????¿¿???¿??¿?¿??¿??
 */
        unsigned int reserved_10 : 1; /* [24] */
        // Comment of field: reserved_11
        /*
        ????¿¿???¿??¿?¿??¿??
 */
        unsigned int reserved_11 : 1; /* [25] */
        // Comment of field: reserved_12
        /*
        I2C_QAMADC??sda_din?¿?¿¿¿????
        2'b0:qamadc_1ch??
        2'b1:qamadc_2chi??
        else:qamadc_2chq??
 */
        unsigned int reserved_12 : 2; /* [27..26] */
        // Comment of field: reserved_13
        /*
        ??????
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
        ??????
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
        ????USB2 sheet
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
        ????USB2 sheet
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
        uart txuart?????¿¿??????¿?txd¿?¿?
        1????¿?¿?
        0??¿?¿?
 */
        unsigned int uart0_use_lbe_txd_en : 1; /* [0] */
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
} u_uart_dbg_cfg;

/* Define the union U_DSP_CTRL_COMMON */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        dsp0??dsp1j?¿??¿¿?¿
        0??xocdmode????????????xtensa????l?¿?¿
        1??xocdmode??????
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        dsp0??dsp1j?¿??¿¿?¿
        0??breakout????????????xtensa????l?¿?¿
        1??breakout??????
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
        ??????
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
        DSP¿??????
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
        ??????
 */
        unsigned int reserved_0 : 5; /* [4..0] */
        // Comment of field: reserved_1
        /*
        DSP MFDE MMU START?¿?
 */
        unsigned int reserved_1 : 1; /* [5] */
        // Comment of field: reserved_2
        /*
        ?¿?????dsp0 runstall?¿¿??????û???¿?¿?????runstall?¿¿???¿????dsp0_runstall_en=0
 */
        unsigned int reserved_2 : 1; /* [6] */
        // Comment of field: reserved_3
        /*
        DSP?¿?????breakin?¿?
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        DSP?¿?????runstall?¿?
 */
        unsigned int reserved_4 : 1; /* [8] */
        // Comment of field: reserved_5
        /*
        select start vector??
        0??default??0xfa820800????
        1??alternative??0x04000000????
 */
        unsigned int reserved_5 : 1; /* [9] */
        // Comment of field: reserved_6
        /*
        ??¿????debug¿¿¿????
        0????¿??????Normal¿¿??
        1????¿??????OCD¿¿??
 */
        unsigned int reserved_6 : 1; /* [10] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [11] */
        // Comment of field: reserved_8
        /*
        ¿??WDG(For DSP)??
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_8 : 1; /* [12] */
        // Comment of field: reserved_9
        /*
        DSP dbg¿??¿???¿¿?????¿dbg¿??JTAGl?¿?????¿??¿????bit??¿1
 */
        unsigned int reserved_9 : 1; /* [13] */
        // Comment of field: reserved_10
        /*
        ??????
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
        ??????
 */
        unsigned int reserved_0 : 5; /* [4..0] */
        // Comment of field: reserved_1
        /*
        DSP MFDE MMU START?¿?
 */
        unsigned int reserved_1 : 1; /* [5] */
        // Comment of field: reserved_2
        /*
        ?¿?????dsp0 runstall?¿¿??????û???¿?¿?????runstall?¿¿???¿????dsp0_runstall_en=0
 */
        unsigned int reserved_2 : 1; /* [6] */
        // Comment of field: reserved_3
        /*
        DSP?¿?????breakin?¿?
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: peri_runstall_dsp0
        /*
        DSP?¿?????runstall?¿?
 */
        unsigned int peri_runstall_dsp0 : 1; /* [8] */
        // Comment of field: peri_statvectorsel_dsp0
        /*
        select start vector??
        0??default??0xfa820400????
        1??alternative??0x03800000????
 */
        unsigned int peri_statvectorsel_dsp0 : 1; /* [9] */
        // Comment of field: peri_ocdhaltonreset_dsp0
        /*
        ??¿????debug¿¿¿????
        0????¿??????Normal¿¿??
        1????¿??????OCD¿¿??
 */
        unsigned int peri_ocdhaltonreset_dsp0 : 1; /* [10] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 1; /* [11] */
        // Comment of field: peri_dsp0_wdg_en
        /*
        ¿??WDG(For DSP)??
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int peri_dsp0_wdg_en : 1; /* [12] */
        // Comment of field: peri_syn_dbg_sel_dsp0
        /*
        DSP dbg¿??¿???¿¿?????¿dbg¿??JTAGl?¿?????¿??¿????bit??¿1
 */
        unsigned int peri_syn_dbg_sel_dsp0 : 1; /* [13] */
        // Comment of field: reserved_5
        /*
        ??????
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
        ??????
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
        Indicate WATII mode??
        0??not WATII mode??
        1??WATII mode??
 */
        unsigned int peri_pwaitmode_dsp0 : 1; /* [2] */
        // Comment of field: peri_xocdmode_dsp0
        /*
        Indicate debug mode??
        0??not debug mode??
        1??debug mode??
 */
        unsigned int peri_xocdmode_dsp0 : 1; /* [3] */
        // Comment of field: reserved_1
        /*
        ??????
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
        ??????
 */
        unsigned int reserved_0 : 4; /* [3..0] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 4; /* [7..4] */
        // Comment of field: reserved_2
        /*
        ??????
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
        bit11~bit5??peri_soc_fuse1[6:0]:????¿¿????
        bit4??peri_soc_fuse0[31]:DBX???????¿?
        bit3??peri_soc_fuse0[30]:Dolby DD+???????¿?
        bit2??peri_soc_fuse0[29]:DTS???????¿?
        bit1??peri_soc_fuse0[28]:reserved; Microvison????????,0¿??1??¿??
        bit0??peri_soc_fuse0[27]:Dolby DD???????¿?
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
        ??????¿???ü¿?????
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
        QOS?????¿?
 */
        unsigned int tvd_arqos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: tvd_awqos
        /*
        QOS?????¿?
 */
        unsigned int tvd_awqos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 9; /* [15..7] */
        // Comment of field: peri_dsp0_arqos
        /*
        QOS?????¿?
 */
        unsigned int peri_dsp0_arqos : 3; /* [18..16] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [19] */
        // Comment of field: peri_dsp0_awqos
        /*
        QOS?????¿?
 */
        unsigned int peri_dsp0_awqos : 3; /* [22..20] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [23] */
        // Comment of field: gpu_arqos
        /*
        QOS?????¿?
 */
        unsigned int gpu_arqos : 3; /* [26..24] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [27] */
        // Comment of field: gpu_awqos
        /*
        QOS?????¿?
 */
        unsigned int gpu_awqos : 3; /* [30..28] */
        // Comment of field: reserved_5
        /*
        ??????
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
        QOS?????¿?
 */
        unsigned int ddrt0_arqos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: ddrt0_awqos
        /*
        QOS?????¿?
 */
        unsigned int ddrt0_awqos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: reserved_2
        /*
        QOS?????¿?
 */
        unsigned int reserved_2 : 3; /* [10..8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: vicap_awqos
        /*
        QOS?????¿?
 */
        unsigned int vicap_awqos : 3; /* [14..12] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [15] */
        // Comment of field: reserved_5
        /*
        QOS?????¿?
 */
        unsigned int reserved_5 : 3; /* [18..16] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [19] */
        // Comment of field: qtc_awqos
        /*
        QOS?????¿?
 */
        unsigned int qtc_awqos : 3; /* [22..20] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [23] */
        // Comment of field: aiao_arqos
        /*
        QOS?????¿?
 */
        unsigned int aiao_arqos : 3; /* [26..24] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [27] */
        // Comment of field: aiao_awqos
        /*
        QOS?????¿?
 */
        unsigned int aiao_awqos : 3; /* [30..28] */
        // Comment of field: reserved_9
        /*
        ??????
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
        QOS?????¿?
 */
        unsigned int m0_smmu_pref_qos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: reserved_1
        /*
        QOS?????¿?
 */
        unsigned int reserved_1 : 3; /* [6..4] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [7] */
        // Comment of field: vpss0_arqos
        /*
        QOS?????¿?
 */
        unsigned int vpss0_arqos : 3; /* [10..8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: vpss0_awqos
        /*
        QOS?????¿?
 */
        unsigned int vpss0_awqos : 3; /* [14..12] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [15] */
        // Comment of field: vpss_mmu_pref_qos
        /*
        QOS?????¿?
 */
        unsigned int vpss_mmu_pref_qos : 3; /* [18..16] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 5; /* [23..19] */
        // Comment of field: reserved_6
        /*
        QOS?????¿?
 */
        unsigned int reserved_6 : 3; /* [26..24] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [27] */
        // Comment of field: reserved_8
        /*
        QOS?????¿?
 */
        unsigned int reserved_8 : 3; /* [30..28] */
        // Comment of field: reserved_9
        /*
        ??????
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
        QOS?????¿?
 */
        unsigned int dtmb_m_awqos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: dtmb_m_arqos
        /*
        QOS?????¿?
 */
        unsigned int dtmb_m_arqos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: jpgd_arqos
        /*
        QOS?????¿?
 */
        unsigned int jpgd_arqos : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: jpgd_awqos
        /*
        QOS?????¿?
 */
        unsigned int jpgd_awqos : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: reserved_4
        /*
        QOS?????¿?
 */
        unsigned int reserved_4 : 3; /* [18..16] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [19] */
        // Comment of field: reserved_6
        /*
        QOS?????¿?
 */
        unsigned int reserved_6 : 3; /* [22..20] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [23] */
        // Comment of field: reserved_8
        /*
        QOS?????¿?
 */
        unsigned int reserved_8 : 3; /* [26..24] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [27] */
        // Comment of field: reserved_10
        /*
        QOS?????¿?
 */
        unsigned int reserved_10 : 3; /* [30..28] */
        // Comment of field: reserved_11
        /*
        ??????
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
        QOS?????¿?
 */
        unsigned int reserved_1 : 3; /* [6..4] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [7] */
        // Comment of field: edma_awqos
        /*
        QOS?????¿?
 */
        unsigned int edma_awqos : 3; /* [10..8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: edma_arqos
        /*
        QOS?????¿?
 */
        unsigned int edma_arqos : 3; /* [14..12] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [15] */
        // Comment of field: reserved_5
        /*
        QOS?????¿?
 */
        unsigned int reserved_5 : 3; /* [18..16] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [19] */
        // Comment of field: reserved_7
        /*
        QOS?????¿?
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
        ??????
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
        QOS?????¿?
 */
        unsigned int peri_vdp0_arqos : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: peri_vdp0_awqos
        /*
        QOS?????¿?
 */
        unsigned int peri_vdp0_awqos : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: peri_vdp0_mmu_qos
        /*
        QOS?????¿?
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
        QOS?????¿?
 */
        unsigned int vicap_mmu_qos : 3; /* [26..24] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [27] */
        // Comment of field: vicap_arqos
        /*
        QOS?????¿?
 */
        unsigned int vicap_arqos : 3; /* [30..28] */
        // Comment of field: reserved_7
        /*
        ??????
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
        ??????
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
        ???¿??????¿?
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 5; /* [7..3] */
        // Comment of field: eth0_hqos
        /*
        ???¿??????¿?
 */
        unsigned int eth0_hqos : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: reserved_3
        /*
        ???¿??????¿?
 */
        unsigned int reserved_3 : 3; /* [14..12] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [15] */
        // Comment of field: reserved_5
        /*
        PVR???¿??????¿?
 */
        unsigned int reserved_5 : 3; /* [18..16] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [19] */
        // Comment of field: reserved_7
        /*
        ???¿??????¿?
 */
        unsigned int reserved_7 : 3; /* [22..20] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [23] */
        // Comment of field: reserved_9
        /*
        ???¿??????¿?
 */
        unsigned int reserved_9 : 3; /* [26..24] */
        // Comment of field: reserved_10
        /*
        reserved
 */
        unsigned int reserved_10 : 1; /* [27] */
        // Comment of field: reserved_11
        /*
        ???¿??????¿?
 */
        unsigned int reserved_11 : 3; /* [30..28] */
        // Comment of field: reserved_12
        /*
        ??????
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
        QOS?????¿?
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        QOS?????¿?
 */
        unsigned int reserved_2 : 3; /* [6..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        ???¿??????¿?
 */
        unsigned int reserved_4 : 3; /* [10..8] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [11] */
        // Comment of field: reserved_6
        /*
        ???¿??????¿?
 */
        unsigned int reserved_6 : 3; /* [14..12] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [15] */
        // Comment of field: peri_cpu_m1_arqos
        /*
        CPU MASTER?????¿??????¿?
 */
        unsigned int peri_cpu_m1_arqos : 3; /* [18..16] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [19] */
        // Comment of field: peri_cpu_m1_awqos
        /*
        CPU MASTER?????¿??????¿?
 */
        unsigned int peri_cpu_m1_awqos : 3; /* [22..20] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [23] */
        // Comment of field: usb2_m_hqos
        /*
        ???¿??????¿?
 */
        unsigned int usb2_m_hqos : 3; /* [26..24] */
        // Comment of field: reserved_10
        /*
        ??????
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
        QOS?????¿?
 */
        unsigned int peri_emmc_arqos : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: peri_emmc_awqos
        /*
        QOS?????¿?
 */
        unsigned int peri_emmc_awqos : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: reserved_2
        /*
        ???¿??????¿?
 */
        unsigned int reserved_2 : 3; /* [10..8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: reserved_4
        /*
        ???¿??????¿?
 */
        unsigned int reserved_4 : 3; /* [14..12] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [15] */
        // Comment of field: reserved_6
        /*
        ???¿??????¿?
 */
        unsigned int reserved_6 : 3; /* [18..16] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [19] */
        // Comment of field: reserved_8
        /*
        ???¿??????¿?
 */
        unsigned int reserved_8 : 3; /* [22..20] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [23] */
        // Comment of field: reserved_10
        /*
        ???¿??????¿?
 */
        unsigned int reserved_10 : 3; /* [26..24] */
        // Comment of field: reserved_11
        /*
        reserved
 */
        unsigned int reserved_11 : 1; /* [27] */
        // Comment of field: reserved_12
        /*
        ???¿??????¿?
 */
        unsigned int reserved_12 : 3; /* [30..28] */
        // Comment of field: reserved_13
        /*
        ??????
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
        QOS?????¿?
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        QOS?????¿?
 */
        unsigned int reserved_2 : 3; /* [6..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        ???¿??????¿?
 */
        unsigned int reserved_4 : 3; /* [10..8] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [11] */
        // Comment of field: reserved_6
        /*
        ???¿??????¿?
 */
        unsigned int reserved_6 : 3; /* [14..12] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [15] */
        // Comment of field: reserved_8
        /*
        ???¿??????¿?
 */
        unsigned int reserved_8 : 3; /* [18..16] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [19] */
        // Comment of field: reserved_10
        /*
        CPU MASTER2?????¿??????¿?
 */
        unsigned int reserved_10 : 3; /* [22..20] */
        // Comment of field: reserved_11
        /*
        reserved
 */
        unsigned int reserved_11 : 1; /* [23] */
        // Comment of field: reserved_12
        /*
        CPU MASTER2?????¿??????¿?
 */
        unsigned int reserved_12 : 3; /* [26..24] */
        // Comment of field: reserved_13
        /*
        reserved
 */
        unsigned int reserved_13 : 1; /* [27] */
        // Comment of field: reserved_14
        /*
        ???¿??????¿?
 */
        unsigned int reserved_14 : 3; /* [30..28] */
        // Comment of field: reserved_15
        /*
        ??????
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
        QOS?????¿?
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        QOS?????¿?
 */
        unsigned int reserved_2 : 3; /* [6..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: peri_pvr_arqos
        /*
        ???¿??????¿?
 */
        unsigned int peri_pvr_arqos : 3; /* [10..8] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [11] */
        // Comment of field: peri_pvr_awqos
        /*
        ???¿??????¿?
 */
        unsigned int peri_pvr_awqos : 3; /* [14..12] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [15] */
        // Comment of field: peri_cipher_arqos
        /*
        cipher??QOS?????¿?
 */
        unsigned int peri_cipher_arqos : 3; /* [18..16] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [19] */
        // Comment of field: peri_cipher_awqos
        /*
        cipher¿QOS?????¿?
 */
        unsigned int peri_cipher_awqos : 3; /* [22..20] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [23] */
        // Comment of field: peri_cipher_pref_qos
        /*
        cipher mmu QOS?????¿?
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
        ??????
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
        DSP?¿????¿???0??
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
        DSP?¿????¿???1??
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
        DSP?¿????¿???2??
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
        DSP?¿????¿???3??
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
        DSP?¿????¿???4??
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
        DSP?¿????¿???5??
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
        DSP?¿????¿???6??
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
        DSP?¿????¿???7??
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
        od¿???¿¿?
        0:CMOS??????
        1:OD??????
 */
        unsigned int peri_sim0_pwren_od_sel : 1; /* [1] */
        // Comment of field: peri_sim0_rst_od_sel
        /*
        od¿???¿¿?
        0:CMOS??????
        1:OD??????
 */
        unsigned int peri_sim0_rst_od_sel : 1; /* [2] */
        // Comment of field: peri_dimming_pwm_1d2_od_sel
        /*
        reserved
 */
        unsigned int peri_dimming_pwm_1d2_od_sel : 1; /* [3] */
        // Comment of field: peri_dimming_pwm_od_sel
        /*
        od¿???¿¿?
        0:CMOS??????
        1:OD??????
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
        od¿???¿¿?
        0:CMOS??????
        1:OD??????
 */
        unsigned int peri_rf_tagc_od_sel : 1; /* [8] */
        // Comment of field: peri_uart0_txd_od_sel
        /*
        od¿???¿¿?
        0:CMOS??????
        1:OD??????
 */
        unsigned int peri_uart0_txd_od_sel : 1; /* [9] */
        // Comment of field: reserved_0
        /*
        od¿???¿¿?
        0:CMOS??????
        1:OD??????
 */
        unsigned int reserved_0 : 1; /* [10] */
        // Comment of field: reserved_1
        /*
        od¿???¿¿?
        0:CMOS??????
        1:OD??????
 */
        unsigned int reserved_1 : 1; /* [11] */
        // Comment of field: peri_dtmb_agc_od_sel
        /*
        od¿???¿¿?
        0:CMOS??????
        1:OD??????
 */
        unsigned int peri_dtmb_agc_od_sel : 1; /* [12] */
        // Comment of field: reserved_2
        /*
        od¿???¿¿?
        0:CMOS??????
        1:OD??????
 */
        unsigned int reserved_2 : 1; /* [13] */
        // Comment of field: reserved_3
        /*
        od¿???¿¿?
        0:CMOS??????
        1:OD??????
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
        ??????
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
        ???¿QTC????¿·¿????
        4'b0000:Equator YPbPr?¿¿???Equator test ?¿¿?¿??peri_qtc_equa_sel?¿?¿????
        4'b0001:Equator CVBS?¿?
        4'b0010:ADC_1CH???¿?
        4'b0011:AIF SIF???¿?
        4'b0100:AIF CVBS???¿?
        4'b0101:TVD debug???¿?
        4'b0110:ADC_2CH??i??q·¿?¿?24bit???¿?
        4'b0111:Audio_Codec adc test???¿?
        4'b1000:ADC_2CH??i??q·??¿¿?¿?16bit???¿?
        4'b1001:ADC_2CH??i·12bit???¿?
        4'b1010:ADC_2CH??q·12bit???¿?
        4'b1011:SIF_VBI¿???????¿?
        else:TVD debug???¿?
 */
        unsigned int reserved_0 : 4; /* [3..0] */
        // Comment of field: peri_vi_ch_sel
        /*
        vicap?¿?0¿??¿??¿????
        0??clk_tvd_13??
        1??clk_pixel_hddec??
        2??clk_hdmi_rx??rx_1p4??
        3????????
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
        TVD ????¿¿¿????
        2'b00??4??AIF??
        2'b01??4??Equator CVBS??
        2'b10??4??Equator Half scart??
        2'b11??reserved??
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
        ??Equator VDAC0 ??Equator????¿·¿????
        2'b00:Equqtor??adc_data_out_1¿·
        2'b01:Equqtor??adc_data_out_2¿·
        2'b10:Equqtor??adc_data_out_3¿·
        2'b11:Equqtor??adc_data_out_3¿·
        vdac0_equa_sel
 */
        unsigned int reserved_0 : 2; /* [1..0] */
        // Comment of field: reserved_1
        /*
        ????
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
        ????.peri_hdmitx_mhlnx_clk_sel
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
        AGC_SEL?¿??¿¿????¿???(agc_sel_out??oen????)
        0??????
        1??????
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: peri_agc_sel
        /*
        ?¿tuner?¿agc??¿??DEMOD?¿agc¿???¿¿????¿????¿¿??¿?
        0: ¿??¿??RF DEMODr??AGC
        1??¿??¿??DTV DEMOD??AGC
 */
        unsigned int peri_agc_sel : 1; /* [2] */
        // Comment of field: reserved_2
        /*
        ????
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
        ??????AIAO RX1¿??????¿????
        0??4??Audio_Demod??
        1??4?¿¿I2S1??
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
} u_peri_aiao_rx1_sd_sel;

/* Define the union U_PERI_QTC_EQUA_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        QTC¿·0????4¿¿????
        0??Equator YPbPr???¿?
        1??Equator TEST?¿??¿¿?
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
} u_peri_qtc_equa_sel;

/* Define the union U_PERI_PENDING0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ??????
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
        ??????
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
        ??????
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
        tso0????(clk/data/sync/vld??????¿?¿??¿?
        0????????
        1??????????
 */
        unsigned int peri_tso0_oen : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [1] */
        // Comment of field: reserved_1
        /*
        ??????TSI0??TSI1????(clk/data/sync/vld)????¿?????¿?
        0??pvr inf3????4??TSI0??
        1??pvr inf3????4??TSI1??
 */
        unsigned int reserved_1 : 1; /* [2] */
        // Comment of field: peri_tso_loop_sel
        /*
        TSI0??TSO????????¿?????¿?
        0??pvr inf2????4??TSI0??
        1??pvr inf2????4??TSOUT??
 */
        unsigned int peri_tso_loop_sel : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        ??????TSI2??TSI1????(clk/data/sync/vld)????¿?????¿?
        0??pvr inf2????4??TSI1??
        1??pvr inf2????4??TSI2??
 */
        unsigned int reserved_2 : 1; /* [4] */
        // Comment of field: peri_dvb0_tsi_sel
        /*
        0??????DEMOD
        1??????DEMOD??TSI1
 */
        unsigned int peri_dvb0_tsi_sel : 1; /* [5] */
        // Comment of field: reserved_3
        /*
        ??????
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
        ??????
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ????HDMI_RX_2P0??HDMI_RX_1P4????¿??????MPLL??
        0??¿??HDMI_RX_PORT1.4????MPLL??
        1??¿??HDMI_RX_2.0????MPLL??
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        peri_arc_sel
        HDMIRX ARC PAD?????¿?¿????
        0??¿??HDMI_RX_PORT1.4????ARC??
        1??¿??HDMI_RX_2.0????ARC??
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
        ????hdmirx_2p0???????¿?,???¿¿??¿¿?¿????
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
        HDMI_RX_2P0??BOOT_STARTE??¿???¿¿?¿??¿¿1'b1
 */
        unsigned int peri_hdmirx_2p0_rx_spi_boot_start : 1; /* [0] */
        // Comment of field: peri_hdmirx_2p0_spi_apb_sel
        /*
        HDMI_RX_2P0????¿???¿?,??¿¿??SPI????apb?¿??¿?
        0????¿SPI?¿?¿??(??????????)??
        1????¿APB?¿?¿????
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
        gpu?¿?DDR¿DDR?¿?¿???¿?¿¿¿?¿???bypass??
        0???¿¿?¿??¿¿??
        1:bypass??
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
        ????FEPHY??PATCH??¿???¿?
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ????FEPHY??TCLK??¿???¿?
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        ????FEPHY??MDIO???????¿?
 */
        unsigned int reserved_2 : 16; /* [17..2] */
        // Comment of field: reserved_3
        /*
        ????fephy mdio??????
 */
        unsigned int reserved_3 : 1; /* [18] */
        // Comment of field: reserved_4
        /*
        ????fephy mdio??¿¿??
 */
        unsigned int reserved_4 : 1; /* [19] */
        // Comment of field: reserved_5
        /*
        ????FEPHY?¿?¿?¿?
 */
        unsigned int reserved_5 : 5; /* [24..20] */
        // Comment of field: reserved_6
        /*
        ??????
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
        TVD or HDDEC??equator¿????clamp¿????
        0??TVD??
        1??HDDEC??
 */
        unsigned int peri_equator_hddec_sel : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ????EQUATOR DAC0¿?¿?????¿???¿?
        2'b00:aif¿?¿?????¿???¿?
        2'b01:cvbs??¿?¿?????¿???¿¿?
        2'b10:??????
        2'b11:afe??¿?¿?????¿???¿¿?
 */
        unsigned int reserved_0 : 2; /* [2..1] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 2; /* [4..3] */
        // Comment of field: reserved_2
        /*
        ????TVD or equator???¿??¿?equator¿????AGC
        0??TVD??
        1??EQUATOR???¿??¿?
 */
        unsigned int reserved_2 : 1; /* [5] */
        // Comment of field: reserved_3
        /*
        ??????
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
        ????¿¿¿¿?¿?:
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
        ????????????????
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
        ?????????¿?
 */
        unsigned int reserved_0 : 4; /* [3..0] */
        // Comment of field: reserved_1
        /*
        ???¿?¿?¿?
 */
        unsigned int reserved_1 : 5; /* [8..4] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [11..9] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 2; /* [13..12] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 2; /* [15..14] */
        // Comment of field: reserved_5
        /*
        ????¿?¿?
 */
        unsigned int reserved_5 : 1; /* [16] */
        // Comment of field: reserved_6
        /*
        ??????
 */
        unsigned int reserved_6 : 3; /* [19..17] */
        // Comment of field: reserved_7
        /*
        Controls whether the PHY RX is active when the PHY is in P0 or P0s in PCIe mode.
        Controls whether the PHY RX is active when the PHY is in any power state with PCLK on in SATA mode.
        ? 0 ?C Active
        ? 1 ?C Standby
 */
        unsigned int reserved_7 : 1; /* [20] */
        // Comment of field: reserved_8
        /*
        ??????
        Transmitter voltage level select, halved when TxSwing is 1
        ? 000 ?C Normal operating range (1000mV)
        ? 001 ?C 1100mV
        ? 010 ?C 900mV
        ? 011 ?C 700mV
        ? 100 ?C 600mV
        ? 101 ?C 500mV
        ? 110 ?C 400mV
        ? 111
 */
        unsigned int reserved_8 : 3; /* [23..21] */
        // Comment of field: reserved_9
        /*
        ??????
        Transmitter de-emphasis select:
        ? 0: -6dB de-emphasis
        ? 1: -3.5dB de-emphasis
        ? 2: 0dB de-emphasis
        ? 3: 0dB de-emphasis
 */
        unsigned int reserved_9 : 2; /* [25..24] */
        // Comment of field: reserved_10
        /*
        ? 0 ?C ALIGN
        ? 1 ?C D24.3
        ? 2 ?C D10.2
        ? 3 ?C Reserved
 */
        unsigned int reserved_10 : 2; /* [27..26] */
        // Comment of field: reserved_11
        /*
        ??????
        Transmitter voltage swing level select:
        ? 0 ?C Full Swing
        ? 1 ?C Low Swing
 */
        unsigned int reserved_11 : 1; /* [28] */
        // Comment of field: reserved_12
        /*
        ??????
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
        ? 0 ?C PCI Express
        ? 1 ?C USB SuperSpeed
        ? 2 ?C SATA
        ? 3 ?C Reserved
 */
        unsigned int reserved_0 : 2; /* [1..0] */
        // Comment of field: reserved_1
        /*
        Controls whether the PHY performs 8b/10b encode and decode:
        ? 0 ?C 8b/10b encode/decode performed normally
        ? 1 ?C 8b/10b encode/decode bypassed
 */
        unsigned int reserved_1 : 1; /* [2] */
        // Comment of field: reserved_2
        /*
        ??????
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
        DTMB??I2C?¿??¿?????¿¿?
 */
        unsigned int peri_dtmb_dev_addr : 2; /* [1..0] */
        // Comment of field: tvd_demo_mem_sel
        /*
        00??unused ???¿??¿?¿????DEMOD¿??memory??
        01??reserved ???¿??¿?¿????TVD¿??memory??
        10??DEMOD¿??memory
        11??TVD¿??memory
 */
        unsigned int tvd_demo_mem_sel : 2; /* [3..2] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [4] */
        // Comment of field: bootmem_demod_sel
        /*
        bootram¿¿??bootmem_demod_sel=1;
        demo¿¿??bootmem_demod_sel=0;
 */
        unsigned int bootmem_demod_sel : 1; /* [5] */
        // Comment of field: reserved_1
        /*
        ??????
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
        ??????
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
        ??¿?u¿????¿¿????¿?¿??¿??¿0??
        0????¿û?¿
        1????¿?¿¿
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
} u_peri_powen_out;

/* Define the union U_PERI_VBI_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ????¿¿??debug¿¿??¿???¿¿?
        0????¿¿??VBI????¿¿??
        1????¿¿??QTC??debug¿¿??
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: peri_vbi_clk_sel
        /*
        ??vbi??¿?¿?????¿¿???¿¿?
        0????¿¿??HDDEC??VBI???¿¿?
        1????¿¿??TVD??VBI???¿¿?
 */
        unsigned int peri_vbi_clk_sel : 1; /* [1] */
        // Comment of field: reserved_1
        /*
        ??????
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
        noc?????¿?
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
        ????
        ????DDR PHY??iotst_iotest?¿¿??¿?CK??DM¿???¿?IOTEST/TI/TE?¿?
        [0]:iotst_iotest[0],CK
        [1]:iotst_iotest[2]
        [6:2]:iotst_iotest[8:4]
        [15:7]:iotst_iotest[52:44]
 */
        unsigned int reserved_0 : 16; /* [15..0] */
        // Comment of field: reserved_1
        /*
        ????HDMI1.4 RX/HDMI2.0RX??????test pattern colorbar0?????¿?
 */
        unsigned int reserved_1 : 14; /* [29..16] */
        // Comment of field: reserved_2
        /*
        ??????
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
        ??????HDMI1.4 RX/HDMI2.0RX??????test pattern colorbar1??????
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        ??????
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
        ????HDMI1.4 RX/HDMI2.0RX??????test pattern colorbar2?????¿?
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        ??????
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
        ????HDMI1.4 RX/HDMI2.0RX??????test pattern colorbar3?????¿?
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        ??????
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
        ????HDMI1.4 RX/HDMI2.0RX??????test pattern colorbar4?????¿?
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        ??????
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
        ????HDMI1.4 RX/HDMI2.0RX??????test pattern colorbar5?????¿?
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        ??????
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
        ????HDMI1.4 RX/HDMI2.0RX??????test pattern colorbar6?????¿?
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        ??????
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
        ????HDMI1.4 RX/HDMI2.0RX??????test pattern colorbar7?????¿?
 */
        unsigned int reserved_0 : 30; /* [29..0] */
        // Comment of field: reserved_1
        /*
        ??????
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
        ????HDMI1.4 RX/HDMI2.0RX??????test pattern??
        [31:16] Reserved;
        [15] 0: sel hw value;1:overwrite with software value;
        [14] RGB or YCC;
        [13] 0??colorbar,1???????¿?;
        [12:0] h_resolution;??
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
        ????HDMI1.4 RX/HDMI2.0RX??????pattern??
        [31:1] reserved;
        [0] 0??normal pattern,1??test pattern??
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
        [15:14] reserved??
        [13] ??¿?¿fifo??full¿¿??
        [12] ??¿?¿fifo??empty¿¿??
        [11:6] ??¿?¿fifo??¿¿????????????¿??¿??
        [5:0] ??¿?¿fifo??¿¿??????¿????¿??¿??
 */
        unsigned int reserved_0 : 16; /* [15..0] */
        // Comment of field: reserved_1
        /*
        [31:24] Reserved;
        [23] ?????¿fifo??full¿¿??
        [22] ?????¿fifo??empty¿¿??
        [21] ?????¿fifo??¿¿???¿???¿?????¿?
        [20] reserved??
        [19] software reset??
        [18] 0??¿??hsync¿????¿???¿?1??¿??DE¿????¿???¿?
        [17] ????¿???¿fifo??¿???¿?¿?ã?¿??¿??¿??¿0???¿?
        [16] ??¿?¿?¿??adapter,0??disable,1??enable??
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
        ????SSP0 LDC????single dma????????burst dma????:
        0:burst dma??????
        1:single dma??????
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
} u_peri_ssp0_ldc_dma_req_sel;

/* Define the union U_PERI_SSP1_LDC_DMA_REQ_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        peri_ssp1_ldc_dma_req_sel
        ????SSP1 LDC????single dma????????burst dma????:
        0:burst dma??????
        1:single dma??????
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
} u_peri_ssp1_ldc_dma_req_sel;

/* Define the union U_PERI_QOS_CFG_09 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        QOS?????¿?
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        QOS?????¿?
 */
        unsigned int reserved_2 : 3; /* [6..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: vdh_arqos
        /*
        QOS?????¿?
 */
        unsigned int vdh_arqos : 3; /* [10..8] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [11] */
        // Comment of field: vdh_awqos
        /*
        QOS?????¿?
 */
        unsigned int vdh_awqos : 3; /* [14..12] */
        // Comment of field: reserved_5
        /*
        QOS?????¿?
 */
        unsigned int reserved_5 : 1; /* [15] */
        // Comment of field: peri_vdh_mmu_arqos
        /*
        QOS?????¿?
 */
        unsigned int peri_vdh_mmu_arqos : 3; /* [18..16] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 5; /* [23..19] */
        // Comment of field: reserved_7
        /*
        QOS?????¿?
 */
        unsigned int reserved_7 : 3; /* [26..24] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [27] */
        // Comment of field: jpgd_mmu_pref_qos
        /*
        QOS?????¿?
 */
        unsigned int jpgd_mmu_pref_qos : 3; /* [30..28] */
        // Comment of field: reserved_9
        /*
        ??????
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
        ??????
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
        QOS?????¿?
 */
        unsigned int reserved_2 : 3; /* [10..8] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [11] */
        // Comment of field: reserved_4
        /*
        QOS?????¿?
 */
        unsigned int reserved_4 : 3; /* [14..12] */
        // Comment of field: reserved_5
        /*
        ??????
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
        ??¿??¿??????¿??:
        1010?????¿??????¿??¿?¿
        0101????????¿??¿?¿
        ??????¿¿??????
 */
        unsigned int remapclear : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 4; /* [7..4] */
        // Comment of field: remapstat
        /*
        ??¿??¿????¿¿??
        0??¿¿??¿???¿?¿?¿??¿????????REMAP CLEAR??¿¿??
        1??¿¿??¿???¿?¿?¿??¿?¿REMAP??¿¿??
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
        ????SPI1????¿?¿?????¿??¿???¿¿?
        0??¿??SPI1??????¿???¿¿?
        1??¿??????????????????peri_spi1_clk_oen¿???¿¿?
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ????SPI1????¿???¿¿?
        0??????spi1_data_di4????io??spi1_di_in??4??¿¿????
        1??????spi1_data_di4????io??spi1_do_in??3??¿¿????
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        ????SPI1????¿?¿?????¿?¿?
        0??¿?¿?
        1????¿?¿?
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        SPI0????¿?¿?????¿??¿???¿¿?
        0??¿??SPI0??????¿???¿¿?
        1??¿??????????????????peri_spi0_clk_oen¿???¿¿?
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        SPI0????¿???¿¿?
        0??????spi0_data_di4????io??spi0_di_in??4??¿¿????
        1??????spi0_data_di4????io??spi0_do_in??3??¿¿????
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        SPI0????¿?¿?????¿?¿?
        0??¿?¿?
        1????¿?¿?
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        ??????
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
        ??????
        ¿???¿¿?
        0??comip??vpss?ã?
        1??comip??vdp?á?
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
        ????SDIO0¿?????¿¿??¿??¿?
        0??¿?????¿¿¿?¿??¿??
        1??¿?????¿¿¿?¿??¿??
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        ??????
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
        blk_on???¿¿???????¿
        0??????0
        1??????1
 */
        unsigned int blk_on_out : 1; /* [0] */
        // Comment of field: blk_on_oen
        /*
        blk_on???¿¿¿???
        0??????
        1??????
 */
        unsigned int blk_on_oen : 1; /* [1] */
        // Comment of field: pwr_en_out
        /*
        pwr_en???¿¿???????¿
        0??????0
        1??????1
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
        emmc_clk???¿¿¿???
        1??????
        0??????
 */
        unsigned int peri_emmc_clk_oen : 1; /* [6] */
        // Comment of field: peri_emmc_rstn_oen
        /*
        emmc_rstn???¿¿¿???
        1??????
        0??????
 */
        unsigned int peri_emmc_rstn_oen : 1; /* [7] */
        // Comment of field: reserved_0
        /*
        ssp?¿?¿??ssp????????spi_ldc??????
        0??ssp0_ldc
        1??spi0
 */
        unsigned int reserved_0 : 1; /* [8] */
        // Comment of field: reserved_1
        /*
        ssp?¿?¿??ssp????????spi_ldc??????
        0??ssp1_ldc
        1??spi1
 */
        unsigned int reserved_1 : 1; /* [9] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [10] */
        // Comment of field: reserved_3
        /*
        ??????USB2 USB3?¿??¿¿??ò???¿??.xlsx??
 */
        unsigned int reserved_3 : 4; /* [14..11] */
        // Comment of field: peri_sync_3d_sel
        /*
        reserved
 */
        unsigned int peri_sync_3d_sel : 1; /* [15] */
        // Comment of field: peri_ttl_clk_oen
        /*
        ??????????????¿????
        0??????????
        1????¿????
 */
        unsigned int peri_ttl_clk_oen : 1; /* [16] */
        // Comment of field: peri_ttl_de_oen
        /*
        ??????????????¿????
        0??????????
        1????¿????
 */
        unsigned int peri_ttl_de_oen : 1; /* [17] */
        // Comment of field: peri_ttl_hsync_oen
        /*
        ??????????????¿????
        0??????????
        1????¿????
 */
        unsigned int peri_ttl_hsync_oen : 1; /* [18] */
        // Comment of field: peri_ttl_vsync_oen
        /*
        ??????????????¿????
        0??????????
        1????¿????
 */
        unsigned int peri_ttl_vsync_oen : 1; /* [19] */
        // Comment of field: reserved_4
        /*
        ??????
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
        QAM LDO??¿?¿?
        0:??¿LDO
        1:?¿¿?¿
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        QAM LDO?¿???¿??¿??¿????
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
 */
        unsigned int reserved_1 : 4; /* [4..1] */
        // Comment of field: reserved_2
        /*
        QAM LDO????¿???¿?
        0:??¿??????????
        1:??¿?????¿???
 */
        unsigned int reserved_2 : 1; /* [5] */
        // Comment of field: reserved_3
        /*
        ??¿??!?????¿?????
        0000??????0.74V
        0001??????0.76V
        0010??????0.78V
        0011??????0.80V
        0100??????0.82V
        0101??????0.84V
        0110??????0.86V
        0111??????0.88V
        1000??????0.90V??¿?¿?
        1001??????0.92V
        1010??????0.94V
        1011??????0.96V
        1100??????0.98V
        1101??????1.00V
        1110??????1.02V
        1111??????1.04V
        20mV/step
 */
        unsigned int reserved_3 : 4; /* [9..6] */
        // Comment of field: reserved_4
        /*
        ??????
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
        ssp_ldc vsync??¿??
        0??vdp?¿???vsync
        1??lvds pwm2¿????vsync
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ssp_ldc vsync??¿??
        0??vdp?¿???vsync
        1??lvds pwm3¿????vsync
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        ??¿?????????¿?????¿??
        1??¿??SD_CPWR
        0??¿??SD_CPWR1
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        reserved
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
} u_peri_misc_contrl;

/* Define the union U_PCIE_PHY_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        pcie phy???¿?¿
 */
        unsigned int reserved_0 : 5; /* [4..0] */
        // Comment of field: reserved_1
        /*
        pcie phy????¿
 */
        unsigned int reserved_1 : 1; /* [5] */
        // Comment of field: reserved_2
        /*
        ????
 */
        unsigned int reserved_2 : 2; /* [7..6] */
        // Comment of field: reserved_3
        /*
        pcie_phy????¿????
 */
        unsigned int reserved_3 : 4; /* [11..8] */
        // Comment of field: reserved_4
        /*
        pcie phy????????
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
        ?¿?bypass 8b/10b????
 */
        unsigned int reserved_6 : 1; /* [18] */
        // Comment of field: reserved_7
        /*
        rx_standby¿¿
 */
        unsigned int reserved_7 : 1; /* [19] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [20] */
        // Comment of field: reserved_9
        /*
        PHY?¿?¿¿??¿¿????¿??
 */
        unsigned int reserved_9 : 2; /* [22..21] */
        // Comment of field: reserved_10
        /*
        PHY?¿?????l1ss?¿???¿¿????¿??
 */
        unsigned int reserved_10 : 1; /* [23] */
        // Comment of field: reserved_11
        /*
        PCIE Clk IO¿??4¿
        0: PHY
        1: CRG
 */
        unsigned int reserved_11 : 1; /* [24] */
        // Comment of field: reserved_12
        /*
        ????
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
        COMBO PHY UFS¿??:
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        UFS PAD reset????:
        0????¿??
        1??????¿??
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        COMBO PHY EMMC¿?¿?
        0????¿?¿?
        1??¿?¿?
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
        ????
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
        UFS Refclk IO pull up/dn select when bit4=1'b1??
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
        ????
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
        UFS Reset IO pull up/dn select when bit12=1'b1??
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
        ????
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
        tde?¿¿¿¿?¿¿
 */
        unsigned int tde_softrst_state : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
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
        tso_tu1??ts??¿???¿¿?
        0??¿??ts??ts2??
        1??¿??????demod??ts??ts3??
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        pvr dvb2??ts??¿???¿¿?
        0??¿??tsmix??ts??tso0??
        1??¿?¿?ts????
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        pvr dvb4??ts??¿???¿¿?
        0??¿??tsmix??ts??tso1??
        1??¿?¿?ts????
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        pvr dvb5??ts??¿???¿¿?
        0??¿??tsmix??ts??tso2??
        1??¿?¿?ts????
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        pvr dvb6??ts??¿???¿¿?
        0??¿??tsmix??ts??tso3??
        1??¿?¿?ts????
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        pvr dvb7??ts??¿???¿¿?
        0??¿??tsmix??ts??tso4??
        1??¿?¿?ts????
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        ???¿???ts??¿????
        0??¿??pvr??????ts????
        1??¿??tsmix??????ts????
 */
        unsigned int reserved_6 : 1; /* [6] */
        // Comment of field: reserved_7
        /*
        ????ts??¿???¿¿?
        0????0??ts??ts0??
        1????????ts????
 */
        unsigned int reserved_7 : 1; /* [7] */
        // Comment of field: reserved_8
        /*
        pvr dvb3??tsmix dvb1??ts??¿???¿¿?
        0??¿??ts2??ts3¿??????ts??tso_tu1??¿???¿?¿peri_ts_sel0??
        1??¿??ts??ts1??
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
        ¿??2port phy??phy2 dp??-???¿
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ¿??2port phy??phy2 dm??-???¿
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        ¿??2port phy??phy0 dp??-???¿
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        ¿??2port phy??phy0 dm??-???¿
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_3 : 1; /* [3] */
        // Comment of field: reserved_4
        /*
        ¿??2port phy??phy1 dp??-???¿
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_4 : 1; /* [4] */
        // Comment of field: reserved_5
        /*
        ¿??2port phy??phy1 dm??-???¿
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_5 : 1; /* [5] */
        // Comment of field: reserved_6
        /*
        usb2 phy debug?????¿¿?
 */
        unsigned int reserved_6 : 1; /* [6] */
        // Comment of field: reserved_7
        /*
        usb2 phy debug?????¿¿?
 */
        unsigned int reserved_7 : 2; /* [8..7] */
        // Comment of field: reserved_8
        /*
        usb2 phy debug?????¿¿?
 */
        unsigned int reserved_8 : 1; /* [9] */
        // Comment of field: reserved_9
        /*
        usb2 phy debug?????¿¿?
 */
        unsigned int reserved_9 : 1; /* [10] */
        // Comment of field: reserved_10
        /*
        usb2 phy debug?????¿¿?
 */
        unsigned int reserved_10 : 2; /* [12..11] */
        // Comment of field: reserved_11
        /*
        usb2 phy debug?¿¿?
 */
        unsigned int reserved_11 : 1; /* [13] */
        // Comment of field: reserved_12
        /*
        usb2 phy debug?????¿¿?
 */
        unsigned int reserved_12 : 1; /* [14] */
        // Comment of field: reserved_13
        /*
        usb2 phy debug?????¿¿?
 */
        unsigned int reserved_13 : 2; /* [16..15] */
        // Comment of field: reserved_14
        /*
        usb2 phy debug?????¿¿?
 */
        unsigned int reserved_14 : 1; /* [17] */
        // Comment of field: reserved_15
        /*
        usb3 phy debug?????¿?,¿??u3 phy?????¿?PWDN_0,PWDN_1??
        RX_EQ_TRAINING_I,TX_DETECT_RX_I,TX_ELECIDLE_I,TX_ONESZEROS_I,
        RX_POLARITY_I,RX_TERMINATION_I4¿??
        0??usb????????
        1??????????????
 */
        unsigned int reserved_15 : 1; /* [18] */
        // Comment of field: reserved_16
        /*
        usb3 phy debug?????¿¿?
 */
        unsigned int reserved_16 : 1; /* [19] */
        // Comment of field: reserved_17
        /*
        usb3 phy debug?????¿¿?
 */
        unsigned int reserved_17 : 1; /* [20] */
        // Comment of field: reserved_18
        /*
        usb3 phy debug?????¿¿?
 */
        unsigned int reserved_18 : 1; /* [21] */
        // Comment of field: reserved_19
        /*
        usb3 phy debug?????¿¿?
 */
        unsigned int reserved_19 : 1; /* [22] */
        // Comment of field: reserved_20
        /*
        usb3 phy debug?????¿¿?
 */
        unsigned int reserved_20 : 1; /* [23] */
        // Comment of field: reserved_21
        /*
        usb3 phy debug?????¿¿?
 */
        unsigned int reserved_21 : 1; /* [24] */
        // Comment of field: reserved_22
        /*
        usb3 phy debug?????¿¿?
 */
        unsigned int reserved_22 : 2; /* [26..25] */
        // Comment of field: reserved_23
        /*
        usb3 phy debug?????¿¿?
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
        ¿??USB2 Port3 dp??-???¿
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        ¿??USB2 Port3 dm??-???¿
        0????¿?¿?
        1??¿?¿?
 */
        unsigned int reserved_1 : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        usb2 phy port3 debug?????¿¿?
 */
        unsigned int reserved_2 : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        usb2 phy port3 debug?????¿¿?
 */
        unsigned int reserved_3 : 2; /* [4..3] */
        // Comment of field: reserved_4
        /*
        usb2 phy port3 debug?????¿¿?
 */
        unsigned int reserved_4 : 1; /* [5] */
        // Comment of field: reserved_5
        /*
        usb2 phy port3 debug?????¿?
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
        ??EDMA??32bit??¿¿??????34bit?u?¿????????¿???¿????
 */
        unsigned int reserved_4 : 2; /* [17..16] */
        // Comment of field: reserved_5
        /*
        ??EDMA??32bit??¿¿??????34bit¿??¿????????¿???¿????
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
        ??????
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
        RAM????¿
 */
        unsigned int himdtv_c_rfs_ema : 3; /* [2..0] */
        // Comment of field: himdtv_C_RFS_EMAW
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rfs_emaw : 2; /* [4..3] */
        // Comment of field: himdtv_C_RFS_WABL
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rfs_wabl : 1; /* [5] */
        // Comment of field: himdtv_C_RFS_WABLM
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rfs_wablm : 2; /* [7..6] */
        // Comment of field: himdtv_C_RAS_EMA
        /*
        RAM????¿
 */
        unsigned int himdtv_c_ras_ema : 3; /* [10..8] */
        // Comment of field: himdtv_C_RAS_EMAW
        /*
        RAM????¿
 */
        unsigned int himdtv_c_ras_emaw : 2; /* [12..11] */
        // Comment of field: himdtv_C_RAS_EMAS
        /*
        RAM????¿
 */
        unsigned int himdtv_c_ras_emas : 1; /* [13] */
        // Comment of field: himdtv_C_RAS_WABL
        /*
        RAM????¿
 */
        unsigned int himdtv_c_ras_wabl : 1; /* [14] */
        // Comment of field: himdtv_C_RAS_WABLM
        /*
        RAM????¿
 */
        unsigned int himdtv_c_ras_wablm : 2; /* [16..15] */
        // Comment of field: himdtv_C_RAS_STOV
        /*
        RAM????¿
 */
        unsigned int himdtv_c_ras_stov : 1; /* [17] */
        // Comment of field: reserved_0
        /*
        RAM????¿
 */
        unsigned int reserved_0 : 4; /* [21..18] */
        // Comment of field: himdtv_C_COLLDISN
        /*
        RAM????¿
 */
        unsigned int himdtv_c_colldisn : 1; /* [22] */
        // Comment of field: himdtv_C_RFT_EMAA
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rft_emaa : 3; /* [25..23] */
        // Comment of field: himdtv_C_RFT_EMAB
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rft_emab : 3; /* [28..26] */
        // Comment of field: himdtv_C_RFT_EMASA
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rft_emasa : 1; /* [29] */
        // Comment of field: reserved_1
        /*
        ??????
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
        RAM????¿
 */
        unsigned int reserved_0 : 3; /* [9..7] */
        // Comment of field: reserved_1
        /*
        RAM????¿
 */
        unsigned int reserved_1 : 3; /* [12..10] */
        // Comment of field: reserved_2
        /*
        RAM????¿
 */
        unsigned int reserved_2 : 1; /* [13] */
        // Comment of field: dsp_wabl
        /*
        RAM????¿
 */
        unsigned int dsp_wabl : 1; /* [14] */
        // Comment of field: dsp_emaw
        /*
        RAM????¿
 */
        unsigned int dsp_emaw : 2; /* [16..15] */
        // Comment of field: dsp_ema
        /*
        RAM????¿
 */
        unsigned int dsp_ema : 3; /* [19..17] */
        // Comment of field: dsp_wablm
        /*
        RAM????¿
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
        ??????
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
        RAM????¿
 */
        unsigned int usb_rfs_ema : 3; /* [2..0] */
        // Comment of field: usb_rfs_emaw
        /*
        RAM????¿
 */
        unsigned int usb_rfs_emaw : 2; /* [4..3] */
        // Comment of field: usb_rfs_wabl
        /*
        RAM????¿
 */
        unsigned int usb_rfs_wabl : 1; /* [5] */
        // Comment of field: usb_rfs_wablm
        /*
        RAM????¿
 */
        unsigned int usb_rfs_wablm : 2; /* [7..6] */
        // Comment of field: msp_ras_ema
        /*
        RAM????¿
 */
        unsigned int msp_ras_ema : 3; /* [10..8] */
        // Comment of field: msp_ras_emaw
        /*
        RAM????¿
 */
        unsigned int msp_ras_emaw : 2; /* [12..11] */
        // Comment of field: msp_ras_emas
        /*
        RAM????¿
 */
        unsigned int msp_ras_emas : 1; /* [13] */
        // Comment of field: msp_ras_wabl
        /*
        RAM????¿
 */
        unsigned int msp_ras_wabl : 1; /* [14] */
        // Comment of field: msp_ras_wablm
        /*
        RAM????¿
 */
        unsigned int msp_ras_wablm : 2; /* [16..15] */
        // Comment of field: msp_ras_stov
        /*
        RAM????¿
 */
        unsigned int msp_ras_stov : 1; /* [17] */
        // Comment of field: msp_ros_ema
        /*
        RAM????¿
 */
        unsigned int msp_ros_ema : 3; /* [20..18] */
        // Comment of field: himdtv_C_RAT_EMAP
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rat_emap : 1; /* [21] */
        // Comment of field: himdtv_C_RAT_EMAS
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rat_emas : 1; /* [22] */
        // Comment of field: himdtv_C_RAT_STOV
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rat_stov : 1; /* [23] */
        // Comment of field: himdtv_C_RAT_STOVAB
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rat_stovab : 1; /* [24] */
        // Comment of field: himdtv_C_RAT_EMAW
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rat_emaw : 2; /* [26..25] */
        // Comment of field: himdtv_C_RAT_EMA
        /*
        RAM????¿
 */
        unsigned int himdtv_c_rat_ema : 3; /* [29..27] */
        // Comment of field: reserved_0
        /*
        ??????
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
        usb2 phy3?¿??¿¿?
 */
        unsigned int reserved_0 : 8; /* [7..0] */
        // Comment of field: reserved_1
        /*
        usb2 phy2?¿??¿¿?
 */
        unsigned int reserved_1 : 8; /* [15..8] */
        // Comment of field: reserved_2
        /*
        usb2 phy1?¿??¿¿?
 */
        unsigned int reserved_2 : 8; /* [23..16] */
        // Comment of field: reserved_3
        /*
        usb2 ????phy?¿??¿¿?
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
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
 */
        unsigned int dsp_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: dtmb_arbit_ctrl
        /*
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
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
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
 */
        unsigned int cipher_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: aiao_arbit_ctrl
        /*
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
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
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
 */
        unsigned int edma_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: m0_64bit_axi_arbit_ctrl
        /*
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
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
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
 */
        unsigned int m0_h2x_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: qtc_arbit_ctrl
        /*
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
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
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
 */
        unsigned int pvr_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: tvd_arbit_ctrl
        /*
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
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
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
 */
        unsigned int vi_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: vi_tvd_dtmb_arbit_ctrl
        /*
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
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
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
 */
        unsigned int emmc_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: reserved_0
        /*
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
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
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
 */
        unsigned int ddrt_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: vdh_arbit_ctrl
        /*
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
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
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
 */
        unsigned int vpss_arbit_ctrl : 16; /* [15..0] */
        // Comment of field: jpgd_arbit_ctrl
        /*
        ?¿?¿ÿ????¿¿?
        bit[15]??¿?¿?¿õ?¿?¿??¿?
        bit[14:0]??¿?¿õ¿?¿???ò???
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
        peri_soc_fuse0[31]:DBX???????¿?
        peri_soc_fuse0[30]:Dolby DD+???????¿?
        peri_soc_fuse0[29]:DTS???????¿?
        peri_soc_fuse0[28]:reserved; Microvison????????,0¿??1??¿??
        peri_soc_fuse0[27]:Dolby DD???????¿?
        peri_soc_fuse0[26:22]: reserved.  ddr??c????ddr???¿¿??chip_id??
        peri_soc_fuse0[21]:4K?¿???,0¿??1??¿??
        peri_soc_fuse0[20]:reserved
        peri_soc_fuse0[19]:VP9 10bit????????,0¿??1??¿??
        peri_soc_fuse0[18]:H265 10bit????????,0¿??1??¿??
        peri_soc_fuse[017]:VP9 8bit????????,0¿??1??¿?¿???????¿????
        peri_soc_fuse0[16]:H265 8bit????????,0¿??1??¿?¿???????¿????
        peri_soc_fuse0[15]:MP2????????,0¿??1??¿?¿?
        peri_soc_fuse0[14]:AVS???????¿?0¿??1??¿?¿?
        peri_soc_fuse0[13]:MP4????????,0¿??1??¿?¿?
        peri_soc_fuse0[12]:H264???????¿?0¿??1??¿?¿?
        peri_soc_fuse0[11]: VP8????????,0¿??1??¿?¿?
        peri_soc_fuse0[10]:VP6????????,0¿??1??¿?¿?
        peri_soc_fuse0[9]: VC1????????,0¿??1??¿?¿?
        peri_soc_fuse0[8]: DIVX3????????,0¿??1??¿?¿?
        peri_soc_fuse0[7]: Real9????????,0¿??1??¿?¿?
        peri_soc_fuse0[6]:Real8???????¿?0¿??1??¿?¿?
        peri_soc_fuse0[5]: reserved
        peri_soc_fuse0[4]:ISDB-T????????,0¿??1??¿?¿?
        peri_soc_fuse0[3]: J83B???????¿?0¿??1??¿?¿?
        peri_soc_fuse0[2]: DVBC????????,0¿??1??¿?¿?
        peri_soc_fuse0[1]: DTMB????????,0¿??1??¿?¿?  ( otp [129] )
        peri_soc_fuse0[0]: ATSC????????,0¿??1??¿?¿?  ( otp [128] )
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
        peri_soc_fuse1[31]:uart0??????0¿??1??¿?¿?    ??otp[287]??
        peri_soc_fuse1[30]:uart1??????0¿??1??¿?¿?
        peri_soc_fuse1[29]:jtag??????0¿??1??¿?¿?
        peri_soc_fuse1[28]:bootrom?¿?¿??UART0??¿??0??¿??1¿?¿???bootrom??
        peri_soc_fuse1[27]:emmc_bootrom_freq??0¿25M??1¿400K   ??bootrom??
        peri_soc_fuse1[26]:emmc¿70ms????¿??????????
        peri_soc_fuse1[25:24]:reserved
        peri_soc_fuse1[23:11]??reserved  (????¿fephy???????¿?)
        peri_soc_fuse1[10]: soc_fuse_mc_info??0 cipher?¿?key?¿??¿¿? 1 cipher?¿?key?¿?????¿?¿?
        peri_soc_fuse1[9]:p2p??????0¿??1??¿?¿?
        peri_soc_fuse1[8]:mLVDS??????0¿??1??¿?¿?
        peri_soc_fuse1[7]:LVDS??????0¿??1??¿?¿?
        peri_soc_fuse1[6:0]:????¿¿????
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
        ????¿efuse
        ????otp[31:0]
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
        ????¿efuse
        ????otp[63:32]
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
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int cipher_port_pri_in : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: aiao_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int aiao_port_pri_in : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: dsp_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int dsp_port_pri_in : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: dtmb_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int dtmb_port_pri_in : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: edma_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int edma_port_pri_in : 3; /* [18..16] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [19] */
        // Comment of field: emmc_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int emmc_port_pri_in : 3; /* [22..20] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [23] */
        // Comment of field: m0_64bit_axi_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int m0_64bit_axi_port_pri_in : 3; /* [26..24] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [27] */
        // Comment of field: m0_h2x_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int m0_h2x_port_pri_in : 3; /* [30..28] */
        // Comment of field: reserved_7
        /*
        ??????
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
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int qtc_port_pri_in : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: pvr_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int pvr_port_pri_in : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: tvd_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int tvd_port_pri_in : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: vi_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int vi_port_pri_in : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: vi_tvd_dtmb_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int vi_tvd_dtmb_port_pri_in : 3; /* [18..16] */
        // Comment of field: reserved_4
        /*
        reserved
 */
        unsigned int reserved_4 : 1; /* [19] */
        // Comment of field: reserved_5
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int reserved_5 : 3; /* [22..20] */
        // Comment of field: reserved_6
        /*
        reserved
 */
        unsigned int reserved_6 : 1; /* [23] */
        // Comment of field: reserved_7
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int reserved_7 : 3; /* [26..24] */
        // Comment of field: reserved_8
        /*
        reserved
 */
        unsigned int reserved_8 : 1; /* [27] */
        // Comment of field: reserved_9
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int reserved_9 : 3; /* [30..28] */
        // Comment of field: reserved_10
        /*
        ??????
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
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int vpss_port_pri_in : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: jpgd_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int jpgd_port_pri_in : 3; /* [6..4] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: ddrt_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int ddrt_port_pri_in : 3; /* [10..8] */
        // Comment of field: reserved_2
        /*
        reserved
 */
        unsigned int reserved_2 : 1; /* [11] */
        // Comment of field: vdh_port_pri_in
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int vdh_port_pri_in : 3; /* [14..12] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [15] */
        // Comment of field: reserved_4
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int reserved_4 : 3; /* [18..16] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [19] */
        // Comment of field: reserved_6
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int reserved_6 : 3; /* [22..20] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [23] */
        // Comment of field: reserved_8
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int reserved_8 : 3; /* [26..24] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [27] */
        // Comment of field: reserved_10
        /*
        ?????????¿????ü¿???
        7???¿????¿?
        ~
        0???¿??¿?
 */
        unsigned int reserved_10 : 3; /* [30..28] */
        // Comment of field: reserved_11
        /*
        ??????
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
        ??????
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
        vbi_aif_sel debug?¿¿?
 */
        unsigned int peri_vbi_aif_sel_dbgen : 1; /* [0] */
        // Comment of field: peri_qamadc_aif_vdac_sel
        /*
        equator vdac¿???¿¿?????¿?á?
        2'b00??AIF??????cvbs???¿?
        2'b01??QAMADC_1CH??????ADC???¿?
        2'b00??reserved
        2'b01??reserved
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
        NOC???¿??????¿?
 */
        unsigned int reserved_0 : 3; /* [2..0] */
        // Comment of field: reserved_1
        /*
        reserved
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        NOC???¿??????¿?
 */
        unsigned int reserved_2 : 3; /* [6..4] */
        // Comment of field: reserved_3
        /*
        reserved
 */
        unsigned int reserved_3 : 1; /* [7] */
        // Comment of field: reserved_4
        /*
        NOC???¿??????¿?
 */
        unsigned int reserved_4 : 3; /* [10..8] */
        // Comment of field: reserved_5
        /*
        reserved
 */
        unsigned int reserved_5 : 1; /* [11] */
        // Comment of field: reserved_6
        /*
        NOC???¿??????¿?
 */
        unsigned int reserved_6 : 3; /* [14..12] */
        // Comment of field: reserved_7
        /*
        reserved
 */
        unsigned int reserved_7 : 1; /* [15] */
        // Comment of field: reserved_8
        /*
        NOC???¿??????¿?
 */
        unsigned int reserved_8 : 3; /* [18..16] */
        // Comment of field: reserved_9
        /*
        reserved
 */
        unsigned int reserved_9 : 1; /* [19] */
        // Comment of field: reserved_10
        /*
        NOC???¿??????¿?
 */
        unsigned int reserved_10 : 3; /* [22..20] */
        // Comment of field: reserved_11
        /*
        reserved
 */
        unsigned int reserved_11 : 1; /* [23] */
        // Comment of field: reserved_12
        /*
        NOC???¿??????¿?
 */
        unsigned int reserved_12 : 3; /* [26..24] */
        // Comment of field: reserved_13
        /*
        reserved
 */
        unsigned int reserved_13 : 1; /* [27] */
        // Comment of field: reserved_14
        /*
        NOC???¿??????¿?
 */
        unsigned int reserved_14 : 3; /* [30..28] */
        // Comment of field: reserved_15
        /*
        ??????
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
