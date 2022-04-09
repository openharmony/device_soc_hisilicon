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

#ifndef __HI_REG_CRG_H__
#define __HI_REG_CRG_H__

/* Define the union U_PERI_CRG_PLL0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpll_frac
        /*
        CPLLС????Ƶϵ????
 */
        unsigned int cpll_frac : 24; /* [23..0] */
        // Comment of field: cpll_postdiv1
        /*
        CPLL??һ????????Ƶϵ????
 */
        unsigned int cpll_postdiv1 : 3; /* [26..24] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [27] */
        // Comment of field: cpll_postdiv2
        /*
        CPLL?ڶ?????????Ƶϵ????
 */
        unsigned int cpll_postdiv2 : 3; /* [30..28] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll0;

/* Define the union U_PERI_CRG_PLL1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpll_fbdiv
        /*
        CPLL??????Ƶϵ????
 */
        unsigned int cpll_fbdiv : 12; /* [11..0] */
        // Comment of field: cpll_refdiv
        /*
        CPLL?ο?ʱ?ӷ?Ƶϵ????
 */
        unsigned int cpll_refdiv : 6; /* [17..12] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [19..18] */
        // Comment of field: cpll_pd
        /*
        bit[20] pll_pd
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int cpll_pd : 1; /* [20] */
        // Comment of field: cpll_foutvcopd
        /*
        bit[21] pll_foutvcopd
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int cpll_foutvcopd : 1; /* [21] */
        // Comment of field: cpll_fout4phasepd
        /*
        bit[22] pll_fout4phasepd
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int cpll_fout4phasepd : 1; /* [22] */
        // Comment of field: cpll_postdivpd
        /*
        bit[23] pll_foutpostdivpd
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int cpll_postdivpd : 1; /* [23] */
        // Comment of field: cpll_dacpd
        /*
        bit[24] pll_dacpd
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int cpll_dacpd : 1; /* [24] */
        // Comment of field: cpll_dsmpd
        /*
        bit[25] pll_dsmpd
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
 */
        unsigned int cpll_dsmpd : 1; /* [25] */
        // Comment of field: cpll_bypass
        /*
        bit[26] pll_bypass
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
 */
        unsigned int cpll_bypass : 1; /* [26] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll1;

/* Define the union U_PERI_CRG_PLL2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dpll_frac
        /*
        DPLLС????Ƶϵ????
 */
        unsigned int dpll_frac : 24; /* [23..0] */
        // Comment of field: dpll_postdiv1
        /*
        DPLL??һ????????Ƶϵ????
 */
        unsigned int dpll_postdiv1 : 3; /* [26..24] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [27] */
        // Comment of field: dpll_postdiv2
        /*
        DPLL?ڶ?????????Ƶϵ????
 */
        unsigned int dpll_postdiv2 : 3; /* [30..28] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll2;

/* Define the union U_PERI_CRG_PLL3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dpll_fbdiv
        /*
        DPLL??????Ƶϵ????
 */
        unsigned int dpll_fbdiv : 12; /* [11..0] */
        // Comment of field: dpll_refdiv
        /*
        DPLL?ο?ʱ?ӷ?Ƶϵ????
 */
        unsigned int dpll_refdiv : 6; /* [17..12] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [19..18] */
        // Comment of field: dpll_pd
        /*
        bit[20] pll_pd
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int dpll_pd : 1; /* [20] */
        // Comment of field: dpll_foutvcopd
        /*
        bit[21] pll_foutvcopd
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int dpll_foutvcopd : 1; /* [21] */
        // Comment of field: dpll_fout4phasepd
        /*
        bit[22] pll_fout4phasepd
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int dpll_fout4phasepd : 1; /* [22] */
        // Comment of field: dpll_postdivpd
        /*
        bit[23] pll_foutpostdivpd
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int dpll_postdivpd : 1; /* [23] */
        // Comment of field: dpll_dacpd
        /*
        bit[24] pll_dacpd
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int dpll_dacpd : 1; /* [24] */
        // Comment of field: dpll_dsmpd
        /*
        bit[25] pll_dsmpd
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
 */
        unsigned int dpll_dsmpd : 1; /* [25] */
        // Comment of field: dpll_bypass
        /*
        bit[26] pll_bypass
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
 */
        unsigned int dpll_bypass : 1; /* [26] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll3;

/* Define the union U_PERI_CRG_PLL4 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: apll_frac
        /*
        APLLС????Ƶϵ????
 */
        unsigned int apll_frac : 24; /* [23..0] */
        // Comment of field: apll_postdiv1
        /*
        APLL??һ????????Ƶϵ????
 */
        unsigned int apll_postdiv1 : 3; /* [26..24] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [27] */
        // Comment of field: apll_postdiv2
        /*
        APLL?ڶ?????????Ƶϵ????
 */
        unsigned int apll_postdiv2 : 3; /* [30..28] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll4;

/* Define the union U_PERI_CRG_PLL5 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: apll_fbdiv
        /*
        APLL??????Ƶϵ????
 */
        unsigned int apll_fbdiv : 12; /* [11..0] */
        // Comment of field: apll_refdiv
        /*
        APLL?ο?ʱ?ӷ?Ƶϵ????
 */
        unsigned int apll_refdiv : 6; /* [17..12] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [19..18] */
        // Comment of field: apll_pd
        /*
        bit[20] pll_pd
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int apll_pd : 1; /* [20] */
        // Comment of field: apll_foutvcopd
        /*
        bit[21] pll_foutvcopd
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int apll_foutvcopd : 1; /* [21] */
        // Comment of field: apll_fout4phasepd
        /*
        bit[22] pll_fout4phasepd
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int apll_fout4phasepd : 1; /* [22] */
        // Comment of field: apll_postdivpd
        /*
        bit[23] pll_foutpostdivpd
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int apll_postdivpd : 1; /* [23] */
        // Comment of field: apll_dacpd
        /*
        bit[24] pll_dacpd
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int apll_dacpd : 1; /* [24] */
        // Comment of field: apll_dsmpd
        /*
        bit[25] pll_dsmpd
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
 */
        unsigned int apll_dsmpd : 1; /* [25] */
        // Comment of field: apll_bypass
        /*
        bit[26] pll_bypass
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
 */
        unsigned int apll_bypass : 1; /* [26] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll5;

/* Define the union U_PERI_CRG_PLL6 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vpll_frac
        /*
        VPLLС????Ƶϵ????
 */
        unsigned int vpll_frac : 24; /* [23..0] */
        // Comment of field: vpll_postdiv1
        /*
        VPLL??һ????????Ƶϵ????
 */
        unsigned int vpll_postdiv1 : 3; /* [26..24] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [27] */
        // Comment of field: vpll_postdiv2
        /*
        VPLL?ڶ?????????Ƶϵ????
 */
        unsigned int vpll_postdiv2 : 3; /* [30..28] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll6;

/* Define the union U_PERI_CRG_PLL7 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vpll_fbdiv
        /*
        VPLL??????Ƶϵ????
 */
        unsigned int vpll_fbdiv : 12; /* [11..0] */
        // Comment of field: vpll_refdiv
        /*
        VPLL?ο?ʱ?ӷ?Ƶϵ????
 */
        unsigned int vpll_refdiv : 6; /* [17..12] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [19..18] */
        // Comment of field: vpll_pd
        /*
        bit[20] pll_pd
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int vpll_pd : 1; /* [20] */
        // Comment of field: vpll_foutvcopd
        /*
        bit[21] pll_foutvcopd
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int vpll_foutvcopd : 1; /* [21] */
        // Comment of field: vpll_fout4phasepd
        /*
        bit[22] pll_fout4phasepd
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int vpll_fout4phasepd : 1; /* [22] */
        // Comment of field: vpll_postdivpd
        /*
        bit[23] pll_foutpostdivpd
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int vpll_postdivpd : 1; /* [23] */
        // Comment of field: vpll_dacpd
        /*
        bit[24] pll_dacpd
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int vpll_dacpd : 1; /* [24] */
        // Comment of field: vpll_dsmpd
        /*
        bit[25] pll_dsmpd
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
 */
        unsigned int vpll_dsmpd : 1; /* [25] */
        // Comment of field: vpll_bypass
        /*
        bit[26] pll_bypass
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
 */
        unsigned int vpll_bypass : 1; /* [26] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll7;

/* Define the union U_PERI_CRG_PLL8 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: epll_frac
        /*
        EPLLС????Ƶϵ????
 */
        unsigned int epll_frac : 24; /* [23..0] */
        // Comment of field: epll_postdiv1
        /*
        EPLL??һ????????Ƶϵ????
 */
        unsigned int epll_postdiv1 : 3; /* [26..24] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [27] */
        // Comment of field: epll_postdiv2
        /*
        EPLL?ڶ?????????Ƶϵ????
 */
        unsigned int epll_postdiv2 : 3; /* [30..28] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll8;

/* Define the union U_PERI_CRG_PLL9 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: epll_fbdiv
        /*
        EPLL??????Ƶϵ????
 */
        unsigned int epll_fbdiv : 12; /* [11..0] */
        // Comment of field: epll_refdiv
        /*
        EPLL?ο?ʱ?ӷ?Ƶϵ????
 */
        unsigned int epll_refdiv : 6; /* [17..12] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [19..18] */
        // Comment of field: epll_pd
        /*
        bit[20] pll_pd
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int epll_pd : 1; /* [20] */
        // Comment of field: epll_foutvcopd
        /*
        bit[21] pll_foutvcopd
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int epll_foutvcopd : 1; /* [21] */
        // Comment of field: epll_fout4phasepd
        /*
        bit[22] pll_fout4phasepd
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int epll_fout4phasepd : 1; /* [22] */
        // Comment of field: epll_postdivpd
        /*
        bit[23] pll_foutpostdivpd
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int epll_postdivpd : 1; /* [23] */
        // Comment of field: epll_dacpd
        /*
        bit[24] pll_dacpd
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int epll_dacpd : 1; /* [24] */
        // Comment of field: epll_dsmpd
        /*
        bit[25] pll_dsmpd
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
 */
        unsigned int epll_dsmpd : 1; /* [25] */
        // Comment of field: epll_bypass
        /*
        bit[26] pll_bypass
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
 */
        unsigned int epll_bypass : 1; /* [26] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll9;

/* Define the union U_PERI_CRG_PLL10 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tpll_frac
        /*
        TPLLС????Ƶϵ????
 */
        unsigned int tpll_frac : 24; /* [23..0] */
        // Comment of field: tpll_postdiv1
        /*
        TPLL??һ????????Ƶϵ????
 */
        unsigned int tpll_postdiv1 : 3; /* [26..24] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [27] */
        // Comment of field: tpll_postdiv2
        /*
        TPLL?ڶ?????????Ƶϵ????
 */
        unsigned int tpll_postdiv2 : 3; /* [30..28] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll10;

/* Define the union U_PERI_CRG_PLL11 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tpll_fbdiv
        /*
        TPLL??????Ƶϵ????
 */
        unsigned int tpll_fbdiv : 12; /* [11..0] */
        // Comment of field: tpll_refdiv
        /*
        TPLL?ο?ʱ?ӷ?Ƶϵ????
 */
        unsigned int tpll_refdiv : 6; /* [17..12] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [19..18] */
        // Comment of field: tpll_pd
        /*
        bit[20] pll_pd
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int tpll_pd : 1; /* [20] */
        // Comment of field: tpll_foutvcopd
        /*
        bit[21] pll_foutvcopd
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int tpll_foutvcopd : 1; /* [21] */
        // Comment of field: tpll_fout4phasepd
        /*
        bit[22] pll_fout4phasepd
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int tpll_fout4phasepd : 1; /* [22] */
        // Comment of field: tpll_postdivpd
        /*
        bit[23] pll_foutpostdivpd
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int tpll_postdivpd : 1; /* [23] */
        // Comment of field: tpll_dacpd
        /*
        bit[24] pll_dacpd
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int tpll_dacpd : 1; /* [24] */
        // Comment of field: tpll_dsmpd
        /*
        bit[25] pll_dsmpd
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
 */
        unsigned int tpll_dsmpd : 1; /* [25] */
        // Comment of field: tpll_bypass
        /*
        bit[26] pll_bypass
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
 */
        unsigned int tpll_bypass : 1; /* [26] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll11;

/* Define the union U_PERI_CRG_PLL12 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hpll_frac
        /*
        HPLLС????Ƶϵ????
 */
        unsigned int hpll_frac : 24; /* [23..0] */
        // Comment of field: hpll_postdiv1
        /*
        HPLL??һ????????Ƶϵ????
 */
        unsigned int hpll_postdiv1 : 3; /* [26..24] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [27] */
        // Comment of field: hpll_postdiv2
        /*
        HPLL?ڶ?????????Ƶϵ????
 */
        unsigned int hpll_postdiv2 : 3; /* [30..28] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll12;

/* Define the union U_PERI_CRG_PLL13 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hpll_fbdiv
        /*
        HPLL??????Ƶϵ????
 */
        unsigned int hpll_fbdiv : 12; /* [11..0] */
        // Comment of field: hpll_refdiv
        /*
        HPLL?ο?ʱ?ӷ?Ƶϵ????
 */
        unsigned int hpll_refdiv : 6; /* [17..12] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [19..18] */
        // Comment of field: hpll_pd
        /*
        bit[20] pll_pd
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int hpll_pd : 1; /* [20] */
        // Comment of field: hpll_foutvcopd
        /*
        bit[21] pll_foutvcopd
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int hpll_foutvcopd : 1; /* [21] */
        // Comment of field: hpll_fout4phasepd
        /*
        bit[22] pll_fout4phasepd
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int hpll_fout4phasepd : 1; /* [22] */
        // Comment of field: hpll_postdivpd
        /*
        bit[23] pll_foutpostdivpd
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int hpll_postdivpd : 1; /* [23] */
        // Comment of field: hpll_dacpd
        /*
        bit[24] pll_dacpd
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int hpll_dacpd : 1; /* [24] */
        // Comment of field: hpll_dsmpd
        /*
        bit[25] pll_dsmpd
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
 */
        unsigned int hpll_dsmpd : 1; /* [25] */
        // Comment of field: hpll_bypass
        /*
        bit[26] pll_bypass
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
 */
        unsigned int hpll_bypass : 1; /* [26] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll13;

/* Define the union U_PERI_CRG_PLL14 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mpll_frac
        /*
        MPLLС????Ƶϵ????
 */
        unsigned int mpll_frac : 24; /* [23..0] */
        // Comment of field: mpll_postdiv1
        /*
        MPLL??һ????????Ƶϵ????
 */
        unsigned int mpll_postdiv1 : 3; /* [26..24] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [27] */
        // Comment of field: mpll_postdiv2
        /*
        MPLL?ڶ?????????Ƶϵ????
 */
        unsigned int mpll_postdiv2 : 3; /* [30..28] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll14;

/* Define the union U_PERI_CRG_PLL15 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mpll_fbdiv
        /*
        MPLL??????Ƶϵ????
 */
        unsigned int mpll_fbdiv : 12; /* [11..0] */
        // Comment of field: mpll_refdiv
        /*
        MPLL?ο?ʱ?ӷ?Ƶϵ????
 */
        unsigned int mpll_refdiv : 6; /* [17..12] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [19..18] */
        // Comment of field: mpll_pd
        /*
        bit[20] pll_pd
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int mpll_pd : 1; /* [20] */
        // Comment of field: mpll_foutvcopd
        /*
        bit[21] pll_foutvcopd
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int mpll_foutvcopd : 1; /* [21] */
        // Comment of field: mpll_fout4phasepd
        /*
        bit[22] pll_fout4phasepd
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int mpll_fout4phasepd : 1; /* [22] */
        // Comment of field: mpll_postdivpd
        /*
        bit[23] pll_foutpostdivpd
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
 */
        unsigned int mpll_postdivpd : 1; /* [23] */
        // Comment of field: mpll_dacpd
        /*
        bit[24] pll_dacpd
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
 */
        unsigned int mpll_dacpd : 1; /* [24] */
        // Comment of field: mpll_dsmpd
        /*
        bit[25] pll_dsmpd
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
 */
        unsigned int mpll_dsmpd : 1; /* [25] */
        // Comment of field: mpll_bypass
        /*
        bit[26] pll_bypass
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
 */
        unsigned int mpll_bypass : 1; /* [26] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg_pll15;

/* Define the union U_PERI_CRG18 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpu_freq_sel_cfg_crg
        /*
        cpuʱ??Դͷѡ???????źţ?
        ??Ȼ??????3bit????ʵ????ֻ??[1:0]bit
        1??cpu_pll_cksel?Ĵ??????Ƶ?pllʱ????????
        0??24M??
 */
        unsigned int cpu_freq_sel_cfg_crg : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: cpu_freq_div_cfg_crg
        /*
        cpuʱ??Դͷ??Ƶ?????źţ?
        00??1??Ƶ??
        01??2??Ƶ??
        10??3??Ƶ??
        11??4??Ƶ??
 */
        unsigned int cpu_freq_div_cfg_crg : 2; /* [5..4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 4; /* [9..6] */
        // Comment of field: cpu_begin_cfg_bypass
        /*
        cpu?л???ʼbypass?źţ?
        0??״̬???????źţ?
        1???Ĵ????????źš?
 */
        unsigned int cpu_begin_cfg_bypass : 1; /* [10] */
        // Comment of field: cpu_sw_begin_cfg
        /*
        cpu?л???ʼ?źţ?
        0??δ??ʼ??
        1????ʼ?л???
 */
        unsigned int cpu_sw_begin_cfg : 1; /* [11] */
        unsigned int cpu_pll_cksel : 3;    /* [14..12] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 17; /* [31..15] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg18;

/* Define the union U_PERI_CRG19 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: timeout_plllock
        /*
        plllock timeoutʱ?䣬??λ??us??
 */
        unsigned int timeout_plllock : 12; /* [11..0] */
        // Comment of field: lock_bypass
        /*
        lock bypass?źţ?
        0??ʹ??PLL lock?źţ?
        1????ʹ??PLL lock?źš?
 */
        unsigned int lock_bypass : 1; /* [12] */
        // Comment of field: pll_cfg_bypass
        /*
        pll????bypass?źţ?
        0??״̬???????źţ?
        1???Ĵ????????źš?
 */
        unsigned int pll_cfg_bypass : 1; /* [13] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 18; /* [31..14] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg19;

/* Define the union U_PERI_CRG23 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: gpio_pwm15_pwd_cken
        /*
        gpio_pwm15_pwdʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpio_pwm15_pwd_cken : 1; /* [0] */
        // Comment of field: ssp1_cken
        /*
        ssp1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ssp1_cken : 1; /* [1] */
        // Comment of field: ssp0_cken
        /*
        ssp0 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ssp0_cken : 1; /* [2] */
        // Comment of field: pwm0_cken
        /*
        pwm0 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pwm0_cken : 1; /* [3] */
        // Comment of field: reserved_0
        /*
        gpio_pwm15_pwdʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_0 : 12; /* [15..4] */
        // Comment of field: otpc_srst_req
        /*
        otpc?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int otpc_srst_req : 1; /* [16] */
        // Comment of field: ssp1_srst_req
        /*
        ssp1 ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ssp1_srst_req : 1; /* [17] */
        // Comment of field: ssp0_srst_req
        /*
        ssp0 ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ssp0_srst_req : 1; /* [18] */
        // Comment of field: pwm0_srst_req
        /*
        pwm0 ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int pwm0_srst_req : 1; /* [19] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg23;

/* Define the union U_PERI_CRG26 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: uart0_cken
        /*
        uart0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int uart0_cken : 1; /* [0] */
        // Comment of field: uart0_srst_req
        /*
        uart0?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int uart0_srst_req : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [3..2] */
        // Comment of field: uart2_cken
        /*
        uart2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int uart2_cken : 1; /* [4] */
        // Comment of field: uart2_srst_req
        /*
        uart2?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int uart2_srst_req : 1; /* [5] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 2; /* [7..6] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg26;

/* Define the union U_PERI_CRG27 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: i2c0_cken
        /*
        i2c0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int i2c0_cken : 1; /* [0] */
        // Comment of field: i2c1_cken
        /*
        i2c1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int i2c1_cken : 1; /* [1] */
        // Comment of field: i2c2_cken
        /*
        i2c2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int i2c2_cken : 1; /* [2] */
        // Comment of field: i2c3_cken
        /*
        i2c3ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int i2c3_cken : 1;      /* [3] */
        unsigned int tuner_i2c_cken : 1; /* [4] */
        // Comment of field: i2c_dtmb_cken
        /*
        dtmb ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int i2c_dtmb_cken : 1; /* [5] */
        // Comment of field: i2c_adc_cken
        /*
        adc i2cʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int i2c_adc_cken : 1; /* [6] */
        // Comment of field: i2c_slv_cken
        /*
        i2c slvdʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int i2c_slv_cken : 1; /* [7] */
        // Comment of field: i2c7_cken
        /*
        i2c tunerʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int i2c7_cken : 1; /* [8] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 7; /* [15..9] */
        // Comment of field: i2c_slv_srst_req
        /*
        i2c slvd?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int i2c_slv_srst_req : 1; /* [16] */
        // Comment of field: i2c0_srst_req
        /*
        i2c0?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int i2c0_srst_req : 1; /* [17] */
        // Comment of field: i2c1_srst_req
        /*
        i2c1?���λ??????
        0??????λ??
        1????λ
 */
        unsigned int i2c1_srst_req : 1; /* [18] */
        // Comment of field: i2c2_srst_req
        /*
        i2c2?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int i2c2_srst_req : 1; /* [19] */
        // Comment of field: i2c3_srst_req
        /*
        i2c3?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int i2c3_srst_req : 1; /* [20] */
        // Comment of field: tuner_i2c_srst_req
        /*
        ????
 */
        unsigned int tuner_i2c_srst_req : 1; /* [21] */
        // Comment of field: i2c_dtmb_srst_req
        /*
        dtmb i2c?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int i2c_dtmb_srst_req : 1; /* [22] */
        // Comment of field: i2c_adc_srst_req
        /*
        ??????
 */
        unsigned int i2c_adc_srst_req : 1; /* [23] */
        // Comment of field: i2c7_srst_req
        /*
        i2c tuner?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int i2c7_srst_req : 1; /* [24] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [25] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 6; /* [31..26] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg27;

/* Define the union U_PERI_CRG28 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ssp3_cken
        /*
        ssp3ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ssp3_cken : 1; /* [0] */
        // Comment of field: ssp3_srst_req
        /*
        ssp3 ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ssp3_srst_req : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [3..2] */
        // Comment of field: ssp2_cken
        /*
        ssp2 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ssp2_cken : 1; /* [4] */
        // Comment of field: ssp2_srst_req
        /*
        ssp2 ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ssp2_srst_req : 1; /* [5] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 26; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg28;

/* Define the union U_PERI_CRG29 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: sci0_cken
        /*
        sci0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int sci0_cken : 1; /* [0] */
        // Comment of field: sci0_srst_req
        /*
        sci0?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int sci0_srst_req : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 6; /* [7..2] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg29;

/* Define the union U_PERI_CRG30 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vdh_cken
        /*
        ??????
 */
        unsigned int vdh_cken : 1; /* [0] */
        // Comment of field: vdhdsp_cken
        /*
        ??????
 */
        unsigned int vdhdsp_cken : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [3..2] */
        // Comment of field: vdh_all_srst_req
        /*
        ??????
 */
        unsigned int vdh_all_srst_req : 1; /* [4] */
        // Comment of field: vdh_scd_srst_req
        /*
        ??????
 */
        unsigned int vdh_scd_srst_req : 1; /* [5] */
        // Comment of field: vdh_mfd_srst_req
        /*
        ??????
 */
        unsigned int vdh_mfd_srst_req : 1; /* [6] */
        // Comment of field: vdh_clk_sel
        /*
        vdhʱ??ѡ????
        0??360M??
        1??432M??
        2??199M??
        3??mpll ?ɵ?ʱ?ӣ?
 */
        unsigned int vdh_clk_sel : 3; /* [9..7] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 2; /* [11..10] */
        // Comment of field: vdhclk_skipcfg
        /*
        vdh?????彵Ƶ??
        0x0?????????壻
        0x1???˵?һ?????壻
        0x2???˵?��?????壻
        ?��????ơ?
 */
        unsigned int vdhclk_skipcfg : 5; /* [16..12] */
        // Comment of field: vdhclk_loaden
        /*
        vdh?????彵Ƶʹ?��?
        0????ʹ?��?
        1??ʹ?��?
 */
        unsigned int vdhclk_loaden : 1; /* [17] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 2; /* [19..18] */
        // Comment of field: vdh_smmu_cken
        /*
        ??????
 */
        unsigned int vdh_smmu_cken : 1; /* [20] */
        // Comment of field: vdh_smmu_srst_req
        /*
        ??????
 */
        unsigned int vdh_smmu_srst_req : 1; /* [21] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 10; /* [31..22] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg30;

/* Define the union U_PERI_CRG31 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: jpgd_cken
        /*
        jpgdʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int jpgd_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: jpgd_srst_req
        /*
        jpgd?���λ????
        0??????λ??
        1????λ??
 */
        unsigned int jpgd_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: jpgd_smmu_cken
        /*
        jpgd smmuʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int jpgd_smmu_cken : 1; /* [8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [11..9] */
        // Comment of field: jpgd_smmu_srst_req
        /*
        jpgd smmu?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int jpgd_smmu_srst_req : 1; /* [12] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg31;

/* Define the union U_PERI_CRG33 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pgd_cken
        /*
        pgdʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pgd_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: pgd_srst_req
        /*
        pgd?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int pgd_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: pgd_smmu_cken
        /*
        pgd smmuʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pgd_smmu_cken : 1; /* [8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [11..9] */
        // Comment of field: pgd_smmu_srst_req
        /*
        pgd smmu?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int pgd_smmu_srst_req : 1; /* [12] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg33;

/* Define the union U_PERI_CRG34 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bpd_cken
        /*
        bpdʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int bpd_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: bpd_srst_req
        /*
        bpd?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int bpd_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 27; /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg34;

/* Define the union U_PERI_CRG35 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: venc_cken
        /*
        vencʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int venc_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [2..1] */
        // Comment of field: venc_srst_req
        /*
        venc?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int venc_srst_req : 1; /* [3] */
        // Comment of field: venc_smmu_cken
        /*
        venc_smmuʱ???ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int venc_smmu_cken : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: venc_smmu_srst_req
        /*
        venc_smmu ?���λ.
        0??????λ??
        1????λ??
 */
        unsigned int venc_smmu_srst_req : 1; /* [8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 23; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg35;

/* Define the union U_PERI_CRG37 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tde_apb_cken
        /*
        tdeʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int tde_apb_cken : 1; /* [0] */
        // Comment of field: tde_axi_cken
        /*
        ??????
 */
        unsigned int tde_axi_cken : 1; /* [1] */
        // Comment of field: tde_smmu_apb_cken
        /*
        tde?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int tde_smmu_apb_cken : 1; /* [2] */
        // Comment of field: tde_smmu_axi_cken
        /*
        ??????
 */
        unsigned int tde_smmu_axi_cken : 1; /* [3] */
        // Comment of field: tde_smmu_cken
        /*
        ??????
 */
        unsigned int tde_smmu_cken : 1; /* [4] */
        // Comment of field: tde_cken
        /*
        ??????
 */
        unsigned int tde_cken : 1;    /* [5] */
        unsigned int reserved_0 : 10; /* [15..6] */
        // Comment of field: tde_smmu_srst_req
        /*
        tde smmu ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int tde_smmu_srst_req : 1; /* [16] */
        // Comment of field: tde_srst_req
        /*
        tde smmu ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int tde_srst_req : 1; /* [17] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 6; /* [23..18] */
        // Comment of field: tdeclk_skipcfg
        /*
        tde?????彵Ƶ??
        0x0?????????壻
        0x1???˵?һ?????壻
        0x2???˵?��?????壻
        ?��????ơ?
 */
        unsigned int tdeclk_skipcfg : 5; /* [28..24] */
        // Comment of field: tdeclk_loaden
        /*
        tde?????彵Ƶʹ?��?
        0????ʹ?��?
        1??ʹ?��?
 */
        unsigned int tdeclk_loaden : 1; /* [29] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 2; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg37;

/* Define the union U_PERI_CRG38 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: fephy_cken
        /*
        fephy????ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int fephy_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 15; /* [15..1] */
        // Comment of field: fephy_srst_req
        /*
        fephy?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int fephy_srst_req : 1; /* [16] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 7;        /* [23..17] */
        unsigned int fephy_test_clk_en : 1; /* [24] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 7; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg38;

/* Define the union U_PERI_CRG48 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cipher_smmu_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 15; /* [15..1] */
        // Comment of field: ca_ci_srst_req
        /*
        ca cipher?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ca_ci_srst_req : 1; /* [16] */
        // Comment of field: otp_srst_req
        /*
        otp?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int otp_srst_req : 1;         /* [17] */
        unsigned int cipher_smmu_srst_req : 1; /* [18] */
        unsigned int cipher_srst_req : 1;      /* [19] */
        unsigned int reserved_1 : 4;           /* [23..20] */
        // Comment of field: ca_ci_clk_sel
        /*
        ca cipherʱ??ѡ????
        0??250MHz??
        1??300MHz??
 */
        unsigned int ca_ci_clk_sel : 1; /* [24] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 7; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg48;

/* Define the union U_PERI_CRG49 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: sha0_cken
        /*
        sha0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int sha0_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: sha0_srst_req
        /*
        sha0?���λ????
        0??????λ??
        1????λ??
 */
        unsigned int sha0_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: sha1_cken
        /*
        sha1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int sha1_cken : 1; /* [8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [11..9] */
        // Comment of field: sha1_srst_req
        /*
        sha1?���λ????
        0??????λ??
        1????λ??
 */
        unsigned int sha1_srst_req : 1; /* [12] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg49;

/* Define the union U_PERI_CRG50 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pmc_cken
        /*
        pmcʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pmc_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: pmc_srst_req
        /*
        pmc?���λ????
        0??????λ??
        1????λ??
 */
        unsigned int pmc_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 27; /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg50;

/* Define the union U_PERI_CRG53 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: gpu_cken
        /*
        gpuʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpu_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: gpu_srst_req
        /*
        gpuģ???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int gpu_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: gpu_pp0_cken
        /*
        pp0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpu_pp0_cken : 1; /* [8] */
        // Comment of field: gpu_pp1_cken
        /*
        pp1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpu_pp1_cken : 1; /* [9] */
        // Comment of field: gpu_pp2_cken
        /*
        pp2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpu_pp2_cken : 1; /* [10] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 21; /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg53;

/* Define the union U_PERI_CRG54 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vo_bus_cken
        /*
        vo????ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_bus_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: vo_hd0_cken
        /*
        vo hdʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_hd0_cken : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: vo_smmu_cken
        /*
        vo smmuʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_smmu_cken : 1; /* [8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [11..9] */
        // Comment of field: vo_smmu_srst_req
        /*
        vo smmu ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int vo_smmu_srst_req : 1; /* [12] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 17; /* [29..13] */
        // Comment of field: vdp_srst_req
        /*
        vou?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int vdp_srst_req : 1; /* [30] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg54;

/* Define the union U_PERI_CRG55 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vi_bus_cken
        /*
        vi????ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vi_bus_cken : 1; /* [0] */
        // Comment of field: vi_p0_cken
        /*
        vi?ӿ?ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vi_p0_cken : 1; /* [1] */
        // Comment of field: vi_p1_cken
        /*
        vi?ӿ?ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vi_p1_cken : 1; /* [2] */
        // Comment of field: vi_ppc0_cken
        /*
        vi????ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vi_ppc0_cken : 1; /* [3] */
        // Comment of field: vi_ppc0_mult2_cken
        /*
        vi????ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vi_ppc0_mult2_cken : 1; /* [4] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [7..5] */
        // Comment of field: vi_srst_req
        /*
        vi axi apb ppc0 ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int vi_srst_req : 1; /* [8] */
        // Comment of field: vi_p0_srst_req
        /*
        vi?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int vi_p0_srst_req : 1; /* [9] */
        // Comment of field: vi_p1_srst_req
        /*
        vi?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int vi_p1_srst_req : 1; /* [10] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [15..11] */
        // Comment of field: vi_smmu_cken
        /*
        vi smmu ????ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vi_smmu_cken : 1; /* [16] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [19..17] */
        // Comment of field: vi_smmu_srst_req
        /*
        vi smmu ?????���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int vi_smmu_srst_req : 1; /* [20] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 3; /* [23..21] */
        // Comment of field: vi_ppc0_cksel
        /*
        vi ppc ʱ?ӵ?ѡ???ź? ??
        0??308mhz ??
        1??180mhz ??
 */
        unsigned int vi_ppc0_cksel : 1; /* [24] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 3; /* [27..25] */
        // Comment of field: reserved_5
        /*
        ??????
 */
        unsigned int reserved_5 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg55;

/* Define the union U_PERI_CRG56 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: qtc_bus_cken
        /*
        qtc APB????ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int qtc_bus_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 15; /* [15..1] */
        // Comment of field: qtc_srst_req
        /*
        qtc?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int qtc_srst_req : 1; /* [16] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 7; /* [23..17] */
        // Comment of field: qtc_clk_pctrl
        /*
        qtc??λѡ????
        0:???ࣻ
        1:???ࡣ
 */
        unsigned int qtc_clk_pctrl : 1; /* [24] */
        // Comment of field: qtc_sif_aif_sel
        /*
        qtc?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int qtc_sif_aif_sel : 1; /* [25] */
        // Comment of field: qtc_vbi_clk_sel
        /*
        ??????
 */
        unsigned int qtc_vbi_clk_sel : 1; /* [26] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg56;

/* Define the union U_PERI_CRG58 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: qamadc_clk_sel
        /*
        qamadcʱ??ѡ????2ѡһ??Ĭ??ѡ 54Mhz
        0??core_aif ??ʱ?? 108/2=54MHz??
        1??30MHz??
        2??PAD????ʱ?ӣ??????á?
 */
        unsigned int qamadc_clk_sel : 2; /* [1..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 7; /* [8..2] */
        // Comment of field: qamctrl_clk_sel
        /*
        qamctrl ʱ??ѡ???? 2ѡһ??Ĭ??ѡcrg2??Ƶ
        0??41.5MHz??
        1??qam_adc phy????·ʱ?ӣ?
 */
        unsigned int qamctrl_clk_sel : 1; /* [9] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 4; /* [13..10] */
        // Comment of field: qamadc_pctrl
        /*
        qamadc??????ʱ??ѡ????
        0??QAMADCʱ??ͬ?ࣻ
        1??QAMADCʱ?ӷ??ࡣ
 */
        unsigned int qamadc_pctrl : 1; /* [14] */
        // Comment of field: qamadc_phy_pctrl
        /*
        qamadc phy ????·ʱ????????ʱ??ѡ????
        0??QAMADCʱ??ͬ?ࣻ
        1??QAMADCʱ?ӷ??ࡣ
 */
        unsigned int qamadc_phy_pctrl : 1; /* [15] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 16; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg58;

/* Define the union U_PERI_CRG62 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bootmem_cken
        /*
        bootmemʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int bootmem_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: bootmem_srst_req
        /*
        bootmem?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int bootmem_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 27; /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg62;

/* Define the union U_PERI_CRG63 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pvr_bus_cken
        /*
        pvr????ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pvr_bus_cken : 1; /* [0] */
        // Comment of field: pvr_dmx_cken
        /*
        pvrʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pvr_dmx_cken : 1; /* [1] */
        // Comment of field: pvr_27m_cken
        /*
        pvr 27Mʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pvr_27m_cken : 1; /* [2] */
        // Comment of field: pvr_tsi1_cken
        /*
        pvr tsi1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pvr_tsi1_cken : 1; /* [3] */
        // Comment of field: pvr_tsi2_cken
        /*
        pvr tsi2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pvr_tsi2_cken : 1; /* [4] */
        // Comment of field: pvr_tsi3_cken
        /*
        pvr tsi3ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pvr_tsi3_cken : 1; /* [5] */
        // Comment of field: pvr_ts0_cken
        /*
        pvr ts0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pvr_ts0_cken : 1; /* [6] */
        // Comment of field: pvr_tsout0_cken
        /*
        pvr tsout0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pvr_tsout0_cken : 1; /* [7] */
        // Comment of field: pvr_tsi2_pctrl
        /*
        pvr tsi2??λѡ?񣺣?S6V551??Ŀ???????䣩
        0:???ࣻ
        1:???ࡣ
 */
        unsigned int pvr_tsi2_pctrl : 1; /* [8] */
        // Comment of field: pvr_tsi3_pctrl
        /*
        pvr tsi3??λѡ?񣺣?S6V551??Ŀ???????䣩
        0:???ࣻ
        1:???ࡣ
 */
        unsigned int pvr_tsi3_pctrl : 1; /* [9] */
        // Comment of field: pvr_srst_req
        /*
        pvr?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int pvr_srst_req : 1; /* [10] */
        // Comment of field: pvr_tsi1_pctrl
        /*
        pvr tsi1??λѡ????
        0:???ࣻ
        1:???ࡣ
 */
        unsigned int pvr_tsi1_pctrl : 1; /* [11] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 12;    /* [23..12] */
        unsigned int pvr_tsi1_cksel : 1; /* [24] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 7; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg63;

/* Define the union U_PERI_CRG64 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pvr_tsout0_pctrl
        /*
        pvr tsout0??λѡ???? S6V551??Ŀ ?̶??䷴????
        0:???ࣻ
        1:???ࡣ
 */
        unsigned int pvr_tsout0_pctrl : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [2..1] */
        // Comment of field: pvr_dmx_clk_sel
        /*
        pvr dmxʱ??ѡ????
        0:200M??
        1:150M??
 */
        unsigned int pvr_dmx_clk_sel : 1; /* [3] */
        // Comment of field: sw_dmx_clk_div
        /*
        pvr?????彵Ƶ??
        0x0?????????壻
        0x1???˵?һ?????壻
        0x2???˵?��?????壻
        ?��????ơ?
 */
        unsigned int sw_dmx_clk_div : 5; /* [8..4] */
        // Comment of field: sw_dmxclk_loaden
        /*
        pvr?????彵Ƶʹ?��?
        0????ʹ?��?
        1??ʹ?��?
 */
        unsigned int sw_dmxclk_loaden : 1; /* [9] */
        // Comment of field: pvr_dmx_clkdiv_cfg
        /*
        pvr dmx clk??Ƶϵ??ѡ????
        0?????????ã?
        1??Ӳ?????á?
 */
        unsigned int pvr_dmx_clkdiv_cfg : 1; /* [10] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [11] */
        // Comment of field: pvr_ts0_clk_sel
        /*
        pvr ts0ʱ??ѡ????
        00??100M??
        01??150M??
        10:1200M??
        11:1500M??
 */
        unsigned int pvr_ts0_clk_sel : 2; /* [13..12] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 2; /* [15..14] */
        // Comment of field: pvr_ts0_clk_div
        /*
        pvr ts0ʱ?ӷ?Ƶ??
        0000??2??Ƶ??
        0001??4??Ƶ??
        0010??6??Ƶ??
        0011??8??Ƶ??
        0100??10??Ƶ??
        0101??12??Ƶ??
        0110??14??Ƶ??
        0111??16??Ƶ??
        1000??18??Ƶ??
        1001??20??Ƶ??
        1010??22??Ƶ??
        1011??24??Ƶ??
        1100??26??Ƶ??
        1101??28??Ƶ??
        1110??30??Ƶ??
        1111??32??Ƶ??
 */
        unsigned int pvr_ts0_clk_div : 4; /* [19..16] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 4; /* [23..20] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg64;

/* Define the union U_PERI_CRG70 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: aiao_cken
        /*
        aiaoʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int aiao_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: aiao_srst_req
        /*
        aiao?���λ????
        0??????λ??
        1????λ??
 */
        unsigned int aiao_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 7; /* [11..5] */
        // Comment of field: aiaoclk_skipcfg
        /*
        aiao?????彵Ƶ??
        0x0?????????壻
        0x1???˵?һ?????壻
        0x2???˵?��?????壻
        ?��????ơ?
 */
        unsigned int aiaoclk_skipcfg : 5; /* [16..12] */
        // Comment of field: aiaoclk_loaden
        /*
        aiao?????彵Ƶʹ?��?
        0????ʹ?��?
        1??ʹ?��?
 */
        unsigned int aiaoclk_loaden : 1; /* [17] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 2; /* [19..18] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg70;

/* Define the union U_PERI_CRG71 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: equator_dac0_cken
        /*
        ??????
 */
        unsigned int equator_dac0_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: equator_dac0_pctrl
        /*
        ??????
 */
        unsigned int equator_dac0_pctrl : 1; /* [4] */
        // Comment of field: equator_dac_srst_req
        /*
        ??????
 */
        unsigned int equator_dac_srst_req : 1; /* [5] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 2; /* [7..6] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg71;

/* Define the union U_PERI_CRG73 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: gpu_freq_sel_cfg_crg
        /*
        gpuʱ??ѡ???????źţ?
        000 ?? 24M ????ʱ?ӣ?
        001 ?? clk_mpll_pst??
        010 ?? 663mhz??
        011 ?? 540mhz??
        100 ?? 497mhz??
        101 ?? 398mhz??
        110 ?? 308mhz??
        111 ?? 216mhz??
 */
        unsigned int gpu_freq_sel_cfg_crg : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: gpu_freq_div_cfg_crg
        /*
        gpu??Ƶ?????źţ?
        00??1??Ƶ??
        01??2??Ƶ??
        10??3??Ƶ??
        11??4??Ƶ??
 */
        unsigned int gpu_freq_div_cfg_crg : 2; /* [5..4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [8..6] */
        // Comment of field: gpu_begin_cfg_bypass
        /*
        gpu?л???ʼbypass?źţ?
        0??״̬???????źţ?
        1???Ĵ????????źš?
 */
        unsigned int gpu_begin_cfg_bypass : 1; /* [9] */
        // Comment of field: gpu_sw_begin_cfg
        /*
        gpu?л???ʼ?źţ?
        0??δ??ʼ??
        1????ʼ?л???
 */
        unsigned int gpu_sw_begin_cfg : 1; /* [10] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 21; /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg73;

/* Define the union U_PERI_CRG74 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ddr_freq_sel_cfg_crg
        /*
        ddrʱ??ѡ???????źš?ֻʹ??0bit????
        0??24M??
        1??gpll_pstʱ?ӣ?Ĭ??400MHz????
 */
        unsigned int ddr_freq_sel_cfg_crg : 3; /* [2..0] */
        // Comment of field: dp_398m_cksel
        /*
        dp 398M????ʱ??ѡ????
        0:24M ????ʱ??
        1??dp 398M????ʱ??ѡ????
 */
        unsigned int dp_398m_cksel : 1; /* [3] */
        // Comment of field: ddr_freq_div_cfg_crg
        /*
        ddr??Ƶ?????źţ?????
        00??1??Ƶ??
        01??2??Ƶ??
        10??3??Ƶ??
        11??4??Ƶ??
 */
        unsigned int ddr_freq_div_cfg_crg : 2; /* [5..4] */
        // Comment of field: dp_432m_cksel
        /*
        dp 432M????ʱ??ѡ????
        0:24M ????ʱ??
        1??dp 432M????ʱ??ѡ????
 */
        unsigned int dp_432m_cksel : 1; /* [6] */
        // Comment of field: cfg_100m_cksel
        /*
        100M APB????ʱ??ѡ????
        0:24M ????ʱ??
        1??100M ????ʱ??ѡ????
 */
        unsigned int cfg_100m_cksel : 1; /* [7] */
        // Comment of field: dp_dsp_cksel
        /*
        dp dsphe edma????ʱ??ѡ????
        0:24M ????ʱ??
        1??dp 300M????ʱ??ѡ????
        2: dp 432M????ʱ??ѡ??
 */
        unsigned int dp_dsp_cksel : 2; /* [9..8] */
        // Comment of field: ddr_begin_cfg_bypass
        /*
        ddr?л???ʼbypass?źţ?
        0??״̬???????źţ?
        1???Ĵ????????źš?
 */
        unsigned int ddr_begin_cfg_bypass : 1; /* [10] */
        // Comment of field: ddr_sw_begin_cfg
        /*
        ddr?л?Ƶ?ʿ?ʼ?źţ?
        0??δ??ʼ??
        1????ʼ?л???
 */
        unsigned int ddr_sw_begin_cfg : 1; /* [11] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg74;

/* Define the union U_PERI_CRG75 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: apll_ssmod_ctrl
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
        unsigned int apll_ssmod_ctrl : 13; /* [12..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg75;

/* Define the union U_PERI_CRG76 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tpll_ssmod_ctrl
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
        unsigned int tpll_ssmod_ctrl : 13; /* [12..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg76;

/* Define the union U_PERI_CRG79 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vpll_ssmod_ctrl
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
        unsigned int vpll_ssmod_ctrl : 13; /* [12..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg79;

/* Define the union U_PERI_CRG80 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hpll_ssmod_ctrl
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
        unsigned int hpll_ssmod_ctrl : 13; /* [12..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg80;

/* Define the union U_PERI_CRG81 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: epll_ssmod_ctrl
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
        unsigned int epll_ssmod_ctrl : 13; /* [12..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg81;

/* Define the union U_PERI_CRG82 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpll_ssmod_ctrl
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
        unsigned int cpll_ssmod_ctrl : 13; /* [12..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg82;

/* Define the union U_PERI_CRG84 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pll_lock
        /*
        0??apll_lock??
        1??tpll_lock??
        2??vpll_lock??
        3??hpll_lock??
        4??epll_lock??
        5??mpll_lock??
        6??dpll_lock??
        7??gpll_lock??
 */
        unsigned int pll_lock : 8; /* [7..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg84;

/* Define the union U_PERI_CRG85 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpu_clk_sw_ok_crg
        /*
        cpu_pll?л?????ָʾ?źš?
 */
        unsigned int cpu_clk_sw_ok_crg : 1; /* [0] */
        // Comment of field: cpu_clk_mux
        /*
        cpuʱ???л?????ָʾ?źš?
 */
        unsigned int cpu_clk_mux : 3; /* [3..1] */
        // Comment of field: gpu_clk_sw_ok_crg
        /*
        gpu pll?л?????ָʾ?źš?
 */
        unsigned int gpu_clk_sw_ok_crg : 1; /* [4] */
        // Comment of field: gpu_clk_mux
        /*
        gpuʱ???л?????ָʾ?źš?
 */
        unsigned int gpu_clk_mux : 3; /* [7..5] */
        // Comment of field: ddr_clk_sw_ok_crg
        /*
        ddr pll?л?????ָʾ?źš?
 */
        unsigned int ddr_clk_sw_ok_crg : 1; /* [8] */
        // Comment of field: ddr_clk_mux
        /*
        ddrʱ????ѡ???źţ?Ĭ??ѡ????ʱ??
 */
        unsigned int ddr_clk_mux : 3; /* [11..9] */
        // Comment of field: reserved_0
        /*
        vdhʱ???л?????ָʾ?źš?
 */
        unsigned int reserved_0 : 2; /* [13..12] */
        // Comment of field: fmc_clk_seled
        /*
        sfcʱ???л?????ָʾ?źš?
 */
        unsigned int fmc_clk_seled : 2; /* [15..14] */
        // Comment of field: sdio0_clk_seled
        /*
        sdio0ʱ???л?????ָʾ?źš?
 */
        unsigned int sdio0_clk_seled : 1; /* [16] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [17] */
        // Comment of field: sdio1_clk_seled
        /*
        sdio1ʱ???л?????ָʾ?źš?
 */
        unsigned int sdio1_clk_seled : 2; /* [19..18] */
        // Comment of field: vdh_clk_seled
        /*
        vdhʱ???л?????ָʾ?źš?
 */
        unsigned int vdh_clk_seled : 3; /* [22..20] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 9; /* [31..23] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg85;

/* Define the union U_PERI_CRG86 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpu_pll_cfg0_sw
        /*
        pll_postdiv2 [30:28]
        PLL?ڶ?????????Ƶϵ????
        
        pll_postdiv1 [26:24]
        PLL??һ????????Ƶϵ????
        
        pll_frac [23:0]:
        PLLС????Ƶϵ????
 */
        unsigned int cpu_pll_cfg0_sw : 31; /* [30..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg86;

/* Define the union U_PERI_CRG87 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpu_pll_cfg1_sw
        /*
        pll_bypass [26]
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
        
        pll_dsmpd [25]
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
        
        pll_dacpd[24]
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
        
        pll_foutpostdivpd[23]
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
        
        pll_fout4phasepd[22]
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
        
        pll_foutvcopd[21]
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
        
        pll_pd[20]
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
        
        pll_refdiv[17:12]
        PLL?ο?ʱ?ӷ?Ƶϵ????
        
        pll_fbdiv[11:0]
        PLL??????Ƶϵ????
 */
        unsigned int cpu_pll_cfg1_sw : 28; /* [27..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg87;

/* Define the union U_PERI_CRG88 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ddr_pll_cfg0_sw
        /*
        pll_postdiv2 [30:28]
        PLL?ڶ?????????Ƶϵ????
        
        pll_postdiv1 [26:24]
        PLL??һ????????Ƶϵ????
        
        pll_frac [23:0]:
        PLLС????Ƶϵ????
 */
        unsigned int ddr_pll_cfg0_sw : 31; /* [30..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg88;

/* Define the union U_PERI_CRG89 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ddr_pll_cfg1_sw
        /*
        pll_bypass [26]
        PLLʱ?ӷ?Ƶ??·??bypass?????ơ?
        0??????·??
        1????·??
        
        pll_dsmpd [25]
        PLL С????Ƶ???ơ?
        0??С??ģʽ??
        1??????ģʽ??
        
        pll_dacpd[24]
        PLL ?????źſ??ơ?
        0??????????״̬??
        1??power down????״̬??
        
        pll_foutpostdivpd[23]
        PLL FOUTPOSTDIV????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
        
        pll_fout4phasepd[22]
        PLL FOUT0~4????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
        
        pll_foutvcopd[21]
        PLL FOUTVCO????Power Down???ơ?
        0??????ʱ????????
        1????????ʱ?ӡ?
        
        pll_pd[20]
        PLL Power Down???ơ?
        0??????????״̬??
        1??power down????״̬??
        
        pll_refdiv[17:12]
        PLL?ο?ʱ?ӷ?Ƶϵ????
        
        pll_fbdiv[11:0]
        PLL??????Ƶϵ????
 */
        unsigned int ddr_pll_cfg1_sw : 28; /* [27..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg89;

/* Define the union U_PERI_CRG91 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: usb2_phy0_cnt_out
        /*
        usb2 phy0 freeclk?????ض??źţ?
        0??freeclkδ??????
        1??freeclk????????
 */
        unsigned int usb2_phy0_cnt_out : 1; /* [0] */
        // Comment of field: usb2_phy1_cnt_out
        /*
        usb2 phy1 freeclk?????ض??źţ?
        0??freeclkδ??????
        1??freeclk????????
 */
        unsigned int usb2_phy1_cnt_out : 1; /* [1] */
        // Comment of field: usb2_phy2_cnt_out
        /*
        usb2 phy2 freeclk?????ض??źţ?
        0??freeclkδ??????
        1??freeclk????????
 */
        unsigned int usb2_phy2_cnt_out : 1; /* [2] */
        // Comment of field: usb2_phy3_cnt_out
        /*
        usb2 phy2 freeclk?????ض??źţ?
        0??freeclkδ??????
        1??freeclk????????
 */
        unsigned int usb2_phy3_cnt_out : 1; /* [3] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg91;

/* Define the union U_PERI_CRG93 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vdh_all_rst_ok
        /*
        ??????
 */
        unsigned int vdh_all_rst_ok : 1; /* [0] */
        // Comment of field: vdh_scd_rst_ok
        /*
        ??????
 */
        unsigned int vdh_scd_rst_ok : 1; /* [1] */
        // Comment of field: vdh_mfd_rst_ok
        /*
        ??????
 */
        unsigned int vdh_mfd_rst_ok : 1; /* [2] */
        // Comment of field: vdh_bpd_rst_ok
        /*
        ??????
 */
        unsigned int vdh_bpd_rst_ok : 1; /* [3] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg93;

/* Define the union U_PERI_CRG94 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dsp_wdg0_pwd_cken
        /*
        wdg0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int dsp_wdg0_pwd_cken : 1; /* [0] */
        // Comment of field: dsp_wdg1_pwd_cken
        /*
        wdg1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int dsp_wdg1_pwd_cken : 1; /* [1] */
        unsigned int wdg2_pwd_cken : 1;     /* [2] */
        unsigned int wdg3_pwd_cken : 1;     /* [3] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 12; /* [15..4] */
        // Comment of field: dsp_wdg0_pwd_srst_req
        /*
        wdg0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int dsp_wdg0_pwd_srst_req : 1; /* [16] */
        // Comment of field: dsp_wdg1_pwd_srst_req
        /*
        wdg1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int dsp_wdg1_pwd_srst_req : 1; /* [17] */
        unsigned int wdg2_pwd_srst_req : 1;     /* [18] */
        unsigned int wdg3_pwd_srst_req : 1;     /* [19] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg94;

/* Define the union U_PERI_CRG95 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pll_test_en
        /*
        pll testʹ???źţ?
        0????ʹ?��?
        1??ʹ?��?
 */
        unsigned int pll_test_en : 1; /* [0] */
        // Comment of field: test_clk_sel_0
        /*
        clk_test_out_0Դͷѡ???źţ?
        0??CPU????ʱ?ӵ?64??Ƶ??
        1??reserved;
        2??DPLL?��?ʱ?ӣ?FOUT4 8??Ƶ??
        3??APLL?��?ʱ?ӣ?FOUT4 8??Ƶ??
 */
        unsigned int test_clk_sel_0 : 2; /* [2..1] */
        // Comment of field: test_clk_sel_1
        /*
        clk_test_out_1Դͷѡ???źţ?
        0??GPU????ʱ?ӵ?64??Ƶ??
        1??VPLL?��?ʱ?ӣ?FOUT4 8?֣?
        2??EPLL?��?ʱ?ӣ?FOUT4 8??Ƶ??
        3??TPLL?��?ʱ?ӣ?FOUT4 8??Ƶ??
 */
        unsigned int test_clk_sel_1 : 2; /* [4..3] */
        // Comment of field: test_clk_sel_2
        /*
        clk_test_out_1Դͷѡ???źţ?
        0??DDR???߹???ʱ?ӵ?64??Ƶ??
        1??MPLL?��?ʱ?ӣ?FOUT4 8?֣?
        2??HPLL?��?ʱ?ӣ?FOUT4 8??Ƶ??
        3??reserved??
 */
        unsigned int test_clk_sel_2 : 2; /* [6..5] */
        // Comment of field: test_clk_sel_3
        /*
        ??????
 */
        unsigned int test_clk_sel_3 : 2; /* [8..7] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 23; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg95;

/* Define the union U_PERI_CRG96 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mem_adjust_cpu
        /*
        ??????
 */
        unsigned int mem_adjust_cpu : 27; /* [26..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg96;

/* Define the union U_PERI_CRG97 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mem_adjust_gpu
        /*
        colldisn[18]?ٶȵ??ڲ???
        colldisn_rft[17]?ٶȵ??ڲ???
        emasa_rft[16]?ٶȵ??ڲ?????
        ema_rfs[15:13]?ٶȵ??ڲ?????
        emaw_rfs[12:11]?ٶȵ??ڲ?????
        ema_ras[10:8]?ٶȵ??ڲ?????
        emaw_ras[7:6]?ٶȵ??ڲ?????
        emaa_rft[5:3]?ٶȵ??ڲ?????
        emab_rft[2:0]?ٶȵ??ڲ?????
 */
        unsigned int mem_adjust_gpu : 19; /* [18..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 13; /* [31..19] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg97;

/* Define the union U_PERI_CRG98 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: ci_srst_req
        /*
        ci?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ci_srst_req : 1; /* [1] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 2; /* [3..2] */
        // Comment of field: ci_cken
        /*
        ciʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ci_cken : 1; /* [4] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 27; /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg98;

/* Define the union U_PERI_CRG100 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: aif_cken
        /*
        AIFʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int aif_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [1] */
        // Comment of field: aif_srst_req
        /*
        AIF?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int aif_srst_req : 1; /* [2] */
        // Comment of field: dto_srst_req
        /*
        AIF DTO?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int dto_srst_req : 1; /* [3] */
        // Comment of field: aif_clk_sel
        /*
        clk_aif_com Դͷѡ???źţ?
        00:clk_108m_ini??
        01:clk_mpll_pst??
        10:clk_tpll_pst??
 */
        unsigned int aif_clk_sel : 2; /* [5..4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 26; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg100;

/* Define the union U_PERI_CRG101 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tvd_cken
        /*
        TVDʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int tvd_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [2..1] */
        // Comment of field: tvd_srst_req
        /*
        tvd?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int tvd_srst_req : 1; /* [3] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg101;

/* Define the union U_PERI_CRG102 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: clk_det_hddec_cken
        /*
        hddec detʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int clk_det_hddec_cken : 1; /* [0] */
        // Comment of field: clk_sog2_hddec_cken
        /*
        hddec sog2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int clk_sog2_hddec_cken : 1; /* [1] */
        // Comment of field: clk_pixel_hddec_cken
        /*
        hddec pixelʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int clk_pixel_hddec_cken : 1; /* [2] */
        // Comment of field: clk_apb_hddec_cken
        /*
        hddec apbʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int clk_apb_hddec_cken : 1; /* [3] */
        // Comment of field: hddec_apb_srst_req
        /*
        hddec apb?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hddec_apb_srst_req : 1; /* [4] */
        // Comment of field: hddec_det_srst_req
        /*
        hddec det?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hddec_det_srst_req : 1; /* [5] */
        // Comment of field: hddec_pixel_srst_req
        /*
        hddec pixel?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hddec_pixel_srst_req : 1; /* [6] */
        // Comment of field: reserved_0
        /*
        hddec pixel ʱ????????ѡ????
        0??pixel ????????
        1 ?? pixel ?ķ??ࡣ
 */
        unsigned int reserved_0 : 17; /* [23..7] */
        // Comment of field: hddec_pixel_cksel
        /*
        hddec pixel ʱ????????ѡ????
        0??pixel ????????
        1 ?? pixel ?ķ??ࡣ
 */
        unsigned int hddec_pixel_cksel : 1; /* [24] */
        // Comment of field: hddec_pixel_pctrl
        /*
        hddec pixel ʱ????????ѡ????
        0??pixel ????????
        1 ?? pixel ?ķ??ࡣ
 */
        unsigned int hddec_pixel_pctrl : 1; /* [25] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 6; /* [31..26] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg102;

/* Define the union U_PERI_CRG103 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: demod_cken
        /*
        demodʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int demod_cken : 1; /* [0] */
        // Comment of field: afe2_cken
        /*
        afe2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int afe2_cken : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 2; /* [3..2] */
        // Comment of field: demod_srst_req
        /*
        demod ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int demod_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: demod_bclk_div
        /*
        ??ʱ??MCLK??λʱ??BCLK??Ƶ??ϵ??
        0000??BCLK??MCLK??1??Ƶ??
        0001??BCLK??MCLK??3??Ƶ??
        0010??BCLK??MCLK??2??Ƶ??
        0011??BCLK??MCLK??4??Ƶ??
        0100??BCLK??MCLK??6??Ƶ??
        0101??BCLK??MCLK??8??Ƶ??
        0110??BCLK??MCLK??12??Ƶ??
        0111??BCLK??MCLK??16??Ƶ??
        1000??BCLK??MCLK??24??Ƶ??
        1001??BCLK??MCLK??32??Ƶ??
        1010??BCLK??MCLK??48??Ƶ??
        1011??BCLK??MCLK??64??Ƶ??
        ??????BCLK??MCLK??8??Ƶ??
 */
        unsigned int demod_bclk_div : 4; /* [11..8] */
        // Comment of field: demod_fsclk_div
        /*
        λʱ??BCLK??????ʱ??FS?ķ?Ƶ??ϵ??
        000??FS??BCLK??16??Ƶ??
        001??FS??BCLK??32??Ƶ??
        010??FS??BCLK??48??Ƶ??
        011??FS??BCLK??64??Ƶ??
        100??FS??BCLK??128??Ƶ??
        101??FS??BCLK??256??Ƶ??
        ??????FS??BCLK??8??Ƶ??
 */
        unsigned int demod_fsclk_div : 3; /* [14..12] */
        // Comment of field: audio_demod_pclk_srst_req
        /*
        audio demod pclk ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int audio_demod_pclk_srst_req : 1; /* [15] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [18..16] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 13; /* [31..19] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg103;

/* Define the union U_PERI_CRG106 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: lvds_pwm_cken
        /*
        LVDS PWMʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int lvds_pwm_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [1] */
        // Comment of field: lvds_pwm_srst_req
        /*
        LVDS PWM?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int lvds_pwm_srst_req : 1; /* [2] */
        // Comment of field: vbo_lvds_srst_req
        /*
        vbo_lvds_srst_req ??vbo??lvds??Ƶ???ĸ?λ?????źţ?
        0??????λ
        1????λ
 */
        unsigned int vbo_lvds_srst_req : 1; /* [3] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg106;

/* Define the union U_PERI_CRG108 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: timer23_pwd_cken
        /*
        timerʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int timer23_pwd_cken : 1; /* [0] */
        // Comment of field: timer45_pwd_cken
        /*
        timerʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int timer45_pwd_cken : 1; /* [1] */
        // Comment of field: timer67_pwd_cken
        /*
        timerʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int timer67_pwd_cken : 1; /* [2] */
        // Comment of field: timercd_pwd_cken
        /*
        timerʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int timercd_pwd_cken : 1; /* [3] */
        // Comment of field: timerab_pwd_cken
        /*
        timerʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int timerab_pwd_cken : 1; /* [4] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 11; /* [15..5] */
        // Comment of field: timer23_pwd_srst_req
        /*
        timer?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int timer23_pwd_srst_req : 1; /* [16] */
        // Comment of field: timer45_pwd_srst_req
        /*
        timer?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int timer45_pwd_srst_req : 1; /* [17] */
        // Comment of field: timer67_pwd_srst_req
        /*
        timer?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int timer67_pwd_srst_req : 1; /* [18] */
        // Comment of field: timercd_pwd_srst_req
        /*
        timer?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int timercd_pwd_srst_req : 1; /* [19] */
        // Comment of field: timerab_pwd_srst_req
        /*
        timer?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int timerab_pwd_srst_req : 1; /* [20] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 11; /* [31..21] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg108;

/* Define the union U_PERI_CRG110 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mpll_ssmod_ctrl
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
        unsigned int mpll_ssmod_ctrl : 13; /* [12..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg110;

/* Define the union U_PERI_CRG115 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ddr_dma_cken
        /*
        ??????
 */
        unsigned int ddr_dma_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: dma1_cken
        /*
        ??????
 */
        unsigned int dma1_cken : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: dma0_cken
        /*
        dma ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int dma0_cken : 1; /* [8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [11..9] */
        // Comment of field: dma_srst_req
        /*
        dma?���λ????
        0??????λ??
        1????λ??
 */
        unsigned int dma_srst_req : 1; /* [12] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg115;

/* Define the union U_PERI_CRG120 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hdmirx_1p4_div20sync_cken
        /*
        ??????
 */
        unsigned int hdmirx_1p4_div20sync_cken : 2; /* [1..0] */
        // Comment of field: hdmirx_1p4_dc_refck_cken
        /*
        ??????
 */
        unsigned int hdmirx_1p4_dc_refck_cken : 2; /* [3..2] */
        // Comment of field: hdmirx_1p4_clk1x_cken
        /*
        ??????
 */
        unsigned int hdmirx_1p4_clk1x_cken : 2; /* [5..4] */
        // Comment of field: hdmirx_1p4_dcclk_cken
        /*
        ??????
 */
        unsigned int hdmirx_1p4_dcclk_cken : 2; /* [7..6] */
        // Comment of field: reserve_for_eco_m0
        /*
        ??????
 */
        unsigned int reserve_for_eco_m0 : 8; /* [15..8] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [19..16] */
        // Comment of field: hdmirx_phyclk_test_en
        /*
        hdmirx_phyclk_test_en ??HDMIRX????ʱ??ʹ???źţ?
        0????ʹ??
        1??ʹ??
 */
        unsigned int hdmirx_phyclk_test_en : 1; /* [20] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 11; /* [31..21] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg120;

/* Define the union U_PERI_CRG122 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        tvģʽʱvo_hd ʱ??ѡ??,??HPLLԴʱ?ӵļ???Ƶ??
        000??1??Ƶ??
        001??2??Ƶ??
        010??3??Ƶ??
        011??4??Ƶ??
        100??5??Ƶ??
        101??6??Ƶ??
        110??7??Ƶ??
        111??8??Ƶ??
 */
        unsigned int reserved_0 : 7; /* [6..0] */
        // Comment of field: reserved_1
        /*
        vo_hd֧??stb/tvģʽ ʱ??ѡ????
        0:stb??
        1:tv??
 */
        unsigned int reserved_1 : 1; /* [7] */
        // Comment of field: reserve_for_eco_vdp_sub
        /*
        ??????
 */
        unsigned int reserve_for_eco_vdp_sub : 8; /* [15..8] */
        // Comment of field: reserve_for_eco_vdp_core
        /*
        ??????
 */
        unsigned int reserve_for_eco_vdp_core : 8; /* [23..16] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg122;

/* Define the union U_PERI_CRG123 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 12; /* [11..0] */
        // Comment of field: usb3_vcc_srst_req
        /*
        usb3ctrl vcc?���λ????
        0??????λ??
        1????λ??
 */
        unsigned int usb3_vcc_srst_req : 1; /* [12] */
        // Comment of field: usb3_phy_srst_req
        /*
        usb3phy por?���λ??????
        ??λ????Ч
        0??????λ??
        1????λ??
 */
        unsigned int usb3_phy_srst_req : 1; /* [13] */
        // Comment of field: usb3_bus_cken
        /*
        usb3ctrl????ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb3_bus_cken : 1; /* [14] */
        // Comment of field: usb3_ref_cken
        /*
        usb3ctrl refʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb3_ref_cken : 1; /* [15] */
        // Comment of field: usb3_suspend_cken
        /*
        usb3ctrl suspendʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb3_suspend_cken : 1; /* [16] */
        // Comment of field: usb3_utmi_cken
        /*
        usb3ctrl utmiʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb3_utmi_cken : 1; /* [17] */
        // Comment of field: usb3_pipe3_cken
        /*
        usb3ctrl pipe3ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb3_pipe3_cken : 1; /* [18] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 13; /* [31..19] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg123;

/* Define the union U_PERI_CRG124 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mpll_hdmi_cfg_bypass
        /*
        MPLL ?????ź?��Դѡ??
        0??from hdmi??
        1: from crg??
 */
        unsigned int mpll_hdmi_cfg_bypass : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg124;

/* Define the union U_PERI_CRG127 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vo_ppc_cken
        /*
        VDP VO PPCʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_ppc_cken : 1; /* [0] */
        // Comment of field: vo_cfg_cken
        /*
        VDP CFG????ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_cfg_cken : 1; /* [1] */
        // Comment of field: vid0_cken
        /*
        VDP VID0 CFG????ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vid0_cken : 1; /* [2] */
        // Comment of field: vid1_cken
        /*
        VDP VID0 CFG????ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vid1_cken : 1; /* [3] */
        // Comment of field: vo_wbchd_cken
        /*
        VDP VO WBCHD PPCʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_wbchd_cken : 1; /* [4] */
        // Comment of field: vo_wbcnr_cken
        /*
        VDP VO WBCNR PPCʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_wbcnr_cken : 1; /* [5] */
        // Comment of field: vo_wbcmad_cken
        /*
        VDP VO WBCMAD PPCʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_wbcmad_cken : 1; /* [6] */
        // Comment of field: vo_alpha_cken
        /*
        VDP VO ALPHA PPCʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_alpha_cken : 1; /* [7] */
        // Comment of field: vo_g0_cken
        /*
        VDP VO G0 PPCʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_g0_cken : 1; /* [8] */
        // Comment of field: vo_g1_cken
        /*
        VDP VO G1 PPCʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_g1_cken : 1; /* [9] */
        // Comment of field: vo_g3_cken
        /*
        VDP VO G3 PPCʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_g3_cken : 1; /* [10] */
        // Comment of field: vp0_cken
        /*
        VDP VP0ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vp0_cken : 1; /* [11] */
        // Comment of field: vo_ppc_cksel
        /*
        VDP VO PPCʱ??ѡ???źţ?
        0??300mhz??
        1??150mhz??
 */
        unsigned int vo_ppc_cksel : 1; /* [12] */
        // Comment of field: vbo_srst_req
        /*
        vbo_srst_req?���λ????
 */
        unsigned int vbo_srst_req : 1; /* [13] */
        // Comment of field: reserved_0
        /*
        p2p_srst_req?���λ??????
 */
        unsigned int reserved_0 : 1; /* [14] */
        // Comment of field: tcon_srst_req
        /*
        tcon_srst_req?���λ??????
 */
        unsigned int tcon_srst_req : 1; /* [15] */
        // Comment of field: disp_srst_req
        /*
        disp_srst_req?���λ??????
 */
        unsigned int disp_srst_req : 1; /* [16] */
        // Comment of field: vdp_cksel
        /*
        clk_vdp_ckselѡ???ź?
 */
        unsigned int vdp_cksel : 1; /* [17] */
        // Comment of field: vo_ppc_2_cken
        /*
        VDP VO PPC_2ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_ppc_2_cken : 1; /* [18] */
        // Comment of field: reserved_1
        /*
        VDP VBO PPCʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_1 : 1; /* [19] */
        // Comment of field: reserved_2
        /*
        VDP P2P TCON MLVDSʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_2 : 1; /* [20] */
        // Comment of field: vo_hd0_fix_cken
        /*
        VDP HD FIXʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vo_hd0_fix_cken : 1; /* [21] */
        // Comment of field: tcon_ppc_fix_cken
        /*
        VDP TCON PPC FIXʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int tcon_ppc_fix_cken : 1; /* [22] */
        // Comment of field: lvds_clk_en_sel
        /*
        lvds_clk_en?ź?ѡ??
        0????Ƶ????????
        1:1'b1
 */
        unsigned int lvds_clk_en_sel : 1; /* [23] */
        // Comment of field: vo_hd_srst_req
        /*
        vo_hd_srst_req?���λ??????
 */
        unsigned int vo_hd_srst_req : 1; /* [24] */
        // Comment of field: vo_hd_tcon_fix_cksel
        /*
        clk_vdp_hd_fixʱ??ѡ???źţ?
        0??clk_tpll_pst??
        1??clk_vo_hd??
 */
        unsigned int vo_hd_tcon_fix_cksel : 1; /* [25] */
        // Comment of field: tcon_ppc_cken
        /*
        tcon_ppcʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int tcon_ppc_cken : 1; /* [26] */
        // Comment of field: reserved_3
        /*
        p2p_packʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int reserved_3 : 1; /* [27] */
        // Comment of field: dphy_srst_req
        /*
        dphyʱ?ӵ??���λ????
        0??????λ??
        1????λ??
 */
        unsigned int dphy_srst_req : 1; /* [28] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 3; /* [31..29] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg127;

/* Define the union U_PERI_CRG128 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [3..0] */
        // Comment of field: pwm02_cken
        /*
        PWM 02????ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pwm02_cken : 1; /* [4] */
        // Comment of field: pwm02_srst_req
        /*
        PWM 02?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int pwm02_srst_req : 1; /* [5] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 2; /* [7..6] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg128;

/* Define the union U_PERI_CRG130 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hpll_hard_ajust_lock
        /*
        HPLL Ӳ??΢????·lockָʾ?źţ?
        0?? ûlock
        1?? lock״̬
 */
        unsigned int hpll_hard_ajust_lock : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg130;

/* Define the union U_PERI_CRG131 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hpll_hard_ajust_clk_divval
        /*
        HPLL ʱ??Ӳ??΢????·?Ĺ???ʱ?ӷ?Ƶ?ȡ?
        0??????Ƶ??
        1??????Ƶ??
        2??2??Ƶ??
        3??3??Ƶ??
        ????.
 */
        unsigned int hpll_hard_ajust_clk_divval : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [7..4] */
        // Comment of field: hpll_soft_ajust_clk_divval
        /*
        HPLL ʱ??????΢????·?Ĺ???ʱ?ӷ?Ƶ?ȡ?
        0??????Ƶ??
        1??????Ƶ??
        2??2??Ƶ??
        3??3??Ƶ??
        ????.
 */
        unsigned int hpll_soft_ajust_clk_divval : 4; /* [11..8] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 4; /* [15..12] */
        // Comment of field: hpll_freq_config_end
        /*
        HPLL ϵ??????????ָʾ?źš?΢???ڼ?Ҫ????Ϊ1
        0??δ????
        1??????FBDIV,FRAC??????
 */
        unsigned int hpll_freq_config_end : 1; /* [16] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [19..17] */
        // Comment of field: hpll_sample_old_config
        /*
        HPLL  ʱ??΢????·??ʼload???ɵ?????ϵ????
        0????load
        1??load??FBDIV,FRAC??????ϵ??
 */
        unsigned int hpll_sample_old_config : 1; /* [20] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 3; /* [23..21] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg131;

/* Define the union U_PERI_CRG132 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hpll_ajust_cken
        /*
        HPLL ʱ????????Ӳ??΢????·ʱ???ſ????üĴ???,Ĭ?ϴ򿪡?
        0???ر?ʱ?ӣ?
        1??????ʱ?ӡ?
 */
        unsigned int hpll_ajust_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: hpll_ajust_srst_req
        /*
        HPLL ʱ????????Ӳ??΢????·???���λ??????
        0????????λ??
        1????λ??
 */
        unsigned int hpll_ajust_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: hpll_freq_mode
        /*
        HPLL ʱ??????ϵ????ѡ????
        00?????üĴ???ֱ???͹?��??????ϵ??
        01??Ӳ??΢????·????֮????????ϵ??
        1X??????΢????·????֮????????ϵ??
 */
        unsigned int hpll_freq_mode : 2; /* [9..8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 2; /* [11..10] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg132;

/* Define the union U_PERI_CRG133 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hpll_step_int
        /*
        HPLL ʱ??Ӳ??΢????·?????????ֲ?????
 */
        unsigned int hpll_step_int : 12; /* [11..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg133;

/* Define the union U_PERI_CRG134 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hpll_step_frac
        /*
        HPLL ʱ??Ӳ??΢????·??С?????ֲ?????
 */
        unsigned int hpll_step_frac : 24; /* [23..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg134;

/* Define the union U_PERI_CRG135 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mpll_ref_cksel
        /*
        MPLL ?ο?ʱ??ѡ????Ĭ?Ͼ???ʱ?ӡ?
        0??24M????ʱ?ӣ?
        1??27M
 */
        unsigned int mpll_ref_cksel : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg135;

/* Define the union U_PERI_CRG136 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mem_adjust_l2
        /*
        l2_dataram_delay[19:0]?ٶȵ??ڲ?????ÿ5bits??Ӧһ??bank
 */
        unsigned int mem_adjust_l2 : 20; /* [19..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg136;

/* Define the union U_PERI_CRG137 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tpll_hard_ajust_lock
        /*
        TPLL Ӳ??΢????·lockָʾ?źţ?
        0?? ûlock
        1?? lock״̬
 */
        unsigned int tpll_hard_ajust_lock : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg137;

/* Define the union U_PERI_CRG138 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tpll_hard_ajust_clk_divval
        /*
        TPLL ʱ??Ӳ??΢????·?Ĺ???ʱ?ӷ?Ƶ?ȡ?
        0??????Ƶ??
        1??????Ƶ??
        2??2??Ƶ??
        3??3??Ƶ??
        ????.
 */
        unsigned int tpll_hard_ajust_clk_divval : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [7..4] */
        // Comment of field: tpll_soft_ajust_clk_divval
        /*
        TPLL ʱ??????΢????·?Ĺ???ʱ?ӷ?Ƶ?ȡ?
        0??????Ƶ??
        1??????Ƶ??
        2??2??Ƶ??
        3??3??Ƶ??
        ????.
 */
        unsigned int tpll_soft_ajust_clk_divval : 4; /* [11..8] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 4; /* [15..12] */
        // Comment of field: tpll_freq_config_end
        /*
        TPLL ϵ??????????ָʾ?źš?΢???ڼ?Ҫ????Ϊ1
        0??δ????
        1??????FBDIV,FRAC??????
 */
        unsigned int tpll_freq_config_end : 1; /* [16] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [19..17] */
        // Comment of field: tpll_sample_old_config
        /*
        TPLL  ʱ??΢????·??ʼload???ɵ?????ϵ????
        0????load
        1??load??FBDIV,FRAC??????ϵ??
 */
        unsigned int tpll_sample_old_config : 1; /* [20] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 3; /* [23..21] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg138;

/* Define the union U_PERI_CRG139 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tpll_ajust_cken
        /*
        TPLL ʱ????????Ӳ??΢????·ʱ???ſ????üĴ???,Ĭ?ϴ򿪡?
        0???ر?ʱ?ӣ?
        1??????ʱ?ӡ?
 */
        unsigned int tpll_ajust_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: tpll_ajust_srst_req
        /*
        TPLL ʱ????????Ӳ??΢????·???���λ??????
        0????????λ??
        1????λ??
 */
        unsigned int tpll_ajust_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: tpll_freq_mode
        /*
        TPLL ʱ??????ϵ????ѡ????
        00?????üĴ???ֱ???͹?��??????ϵ??
        01??Ӳ??΢????·????֮????????ϵ??
        1X??????΢????·????֮????????ϵ??
 */
        unsigned int tpll_freq_mode : 2; /* [9..8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 2; /* [11..10] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg139;

/* Define the union U_PERI_CRG140 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tpll_step_int
        /*
        TPLL ʱ??Ӳ??΢????·?????????ֲ?????
 */
        unsigned int tpll_step_int : 12; /* [11..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg140;

/* Define the union U_PERI_CRG141 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tpll_step_frac
        /*
        TPLL ʱ??Ӳ??΢????·??С?????ֲ?????
 */
        unsigned int tpll_step_frac : 24; /* [23..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg141;

/* Define the union U_PERI_CRG142 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dpll_hard_ajust_lock
        /*
        DPLL??Ӳ??΢????·lockָʾ?źţ?
        0?? ûlock
        1?? lock״̬
 */
        unsigned int dpll_hard_ajust_lock : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg142;

/* Define the union U_PERI_CRG143 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dpll_hard_ajust_clk_divval
        /*
        DPLL ʱ??Ӳ??΢????·?Ĺ???ʱ?ӷ?Ƶ?ȡ?
        0??????Ƶ??
        1??????Ƶ??
        2??2??Ƶ??
        3??3??Ƶ??
        ????.
 */
        unsigned int dpll_hard_ajust_clk_divval : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [7..4] */
        // Comment of field: dpll_soft_ajust_clk_divval
        /*
        DPLL ʱ??????΢????·?Ĺ???ʱ?ӷ?Ƶ?ȡ?
        0??????Ƶ??
        1??????Ƶ??
        2??2??Ƶ??
        3??3??Ƶ??
        ????.
 */
        unsigned int dpll_soft_ajust_clk_divval : 4; /* [11..8] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 4; /* [15..12] */
        // Comment of field: dpll_freq_config_end
        /*
        DPLL ϵ??????????ָʾ?źš?΢???ڼ?Ҫ????Ϊ1
        0??δ????
        1??????FBDIV,FRAC??????
 */
        unsigned int dpll_freq_config_end : 1; /* [16] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [19..17] */
        // Comment of field: dpll_sample_old_config
        /*
        DPLL  ʱ??΢????·??ʼload???ɵ?????ϵ????
        0????load
        1??load??FBDIV,FRAC??????ϵ??
 */
        unsigned int dpll_sample_old_config : 1; /* [20] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 3; /* [23..21] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg143;

/* Define the union U_PERI_CRG144 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dpll_ajust_cken
        /*
        DPLL ʱ????????Ӳ??΢????·ʱ???ſ????üĴ???,Ĭ?ϴ򿪡?
        0???ر?ʱ?ӣ?
        1??????ʱ?ӡ?
 */
        unsigned int dpll_ajust_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: dpll_ajust_srst_req
        /*
        DPLL ʱ????????Ӳ??΢????·???���λ??????
        0????????λ??
        1????λ??
 */
        unsigned int dpll_ajust_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: dpll_freq_mode
        /*
        DPLL ʱ??????ϵ????ѡ????
        00?????üĴ???ֱ???͹?��??????ϵ??
        01??Ӳ??΢????·????֮????????ϵ??
        1X??????΢????·????֮????????ϵ??
 */
        unsigned int dpll_freq_mode : 2; /* [9..8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 2; /* [11..10] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg144;

/* Define the union U_PERI_CRG145 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dpll_step_int
        /*
        DPLL ʱ??Ӳ??΢????·?????????ֲ?????
 */
        unsigned int dpll_step_int : 12; /* [11..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg145;

/* Define the union U_PERI_CRG146 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dpll_step_frac
        /*
        DPLL ʱ??Ӳ??΢????·??С?????ֲ?????
 */
        unsigned int dpll_step_frac : 24; /* [23..0] */
        unsigned int reserved_0 : 8;      /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg146;

/* Define the union U_PERI_CRG147 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpll_hard_ajust_lock
        /*
        cpll Ӳ??΢????·lockָʾ?źţ?
        0?? ûlock
        1?? lock״̬
 */
        unsigned int cpll_hard_ajust_lock : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg147;

/* Define the union U_PERI_CRG148 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpll_hard_ajust_clk_divval
        /*
        cpll ʱ??Ӳ??΢????·?Ĺ???ʱ?ӷ?Ƶ?ȡ?
        0??????Ƶ??
        1??????Ƶ??
        2??2??Ƶ??
        3??3??Ƶ??
        ????.
 */
        unsigned int cpll_hard_ajust_clk_divval : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [7..4] */
        // Comment of field: cpll_soft_ajust_clk_divval
        /*
        cpll ʱ??????΢????·?Ĺ???ʱ?ӷ?Ƶ?ȡ?
        0??????Ƶ??
        1??????Ƶ??
        2??2??Ƶ??
        3??3??Ƶ??
        ????.
 */
        unsigned int cpll_soft_ajust_clk_divval : 4; /* [11..8] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 4; /* [15..12] */
        // Comment of field: cpll_freq_config_end
        /*
        cpll ϵ??????????ָʾ?źš?΢???ڼ?Ҫ????Ϊ1
        0??δ????
        1??????FBDIV,FRAC??????
 */
        unsigned int cpll_freq_config_end : 1; /* [16] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [19..17] */
        // Comment of field: cpll_sample_old_config
        /*
        cpll  ʱ??΢????·??ʼload???ɵ?????ϵ????
        0????load
        1??load??FBDIV,FRAC??????ϵ??
 */
        unsigned int cpll_sample_old_config : 1; /* [20] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 3; /* [23..21] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg148;

/* Define the union U_PERI_CRG149 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpll_ajust_cken
        /*
        cpll ʱ????????Ӳ??΢????·ʱ???ſ????üĴ???,Ĭ?ϴ򿪡?
        0???ر?ʱ?ӣ?
        1??????ʱ?ӡ?
 */
        unsigned int cpll_ajust_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: cpll_ajust_srst_req
        /*
        cpll ʱ????????Ӳ??΢????·???���λ??????
        0????????λ??
        1????λ??
 */
        unsigned int cpll_ajust_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: cpll_freq_mode
        /*
        cpll ʱ??????ϵ????ѡ????
        00?????üĴ???ֱ???͹?��??????ϵ??
        01??Ӳ??΢????·????֮????????ϵ??
        1X??????΢????·????֮????????ϵ??
 */
        unsigned int cpll_freq_mode : 2; /* [9..8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 2; /* [11..10] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg149;

/* Define the union U_PERI_CRG150 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpll_step_int
        /*
        cpll ʱ??Ӳ??΢????·?????????ֲ?????
 */
        unsigned int cpll_step_int : 12; /* [11..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg150;

/* Define the union U_PERI_CRG151 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cpll_step_frac
        /*
        cpll ʱ??Ӳ??΢????·??С?????ֲ?????
 */
        unsigned int cpll_step_frac : 24; /* [23..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg151;

/* Define the union U_PERI_CRG153 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: equator_apb_srst_req
        /*
        ??????
 */
        unsigned int equator_apb_srst_req : 1; /* [0] */
        // Comment of field: rgb_srst_req
        /*
        ??????
 */
        unsigned int rgb_srst_req : 1; /* [1] */
        // Comment of field: cvbs_srst_req
        /*
        ??????
 */
        unsigned int cvbs_srst_req : 1; /* [2] */
        // Comment of field: clk_cvbs_pctrl
        /*
        ??????
 */
        unsigned int clk_cvbs_pctrl : 1; /* [3] */
        // Comment of field: clk_rgb_pctrl
        /*
        ??????
 */
        unsigned int clk_rgb_pctrl : 1; /* [4] */
        // Comment of field: rg_cvbs_clk_div2_sel
        /*
        ??????
 */
        unsigned int rg_cvbs_clk_div2_sel : 1; /* [5] */
        // Comment of field: rg_rgb_clk_div2_sel
        /*
        ??????
 */
        unsigned int rg_rgb_clk_div2_sel : 1; /* [6] */
        // Comment of field: rgb_cvbs_sel
        /*
        ??????
 */
        unsigned int rgb_cvbs_sel : 1; /* [7] */
        // Comment of field: dac_out_mux_0
        /*
        ??????
 */
        unsigned int dac_out_mux_0 : 2; /* [9..8] */
        // Comment of field: dac_srst_req_0
        /*
        ??????
 */
        unsigned int dac_srst_req_0 : 1; /* [10] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [11] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg153;

/* Define the union U_PERI_CRG156 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: eth_cken
        /*
        eth ????ʱ?ӵ?ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int eth_cken : 1; /* [0] */
        // Comment of field: eth_bus_cken
        /*
        eth ahb???ߵ?ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int eth_bus_cken : 1; /* [1] */
        // Comment of field: eth_cksel
        /*
        eth ????ʱ??Դͷѡ???źţ?
        0??100M??
        1??50M??
 */
        unsigned int eth_cksel : 1; /* [2] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [3] */
        // Comment of field: hrst_eth_s
        /*
        eth??λ????eth???߿???ʱ???��?λ
        0??????λ
        1????λ
 */
        unsigned int hrst_eth_s : 1; /* [4] */
        // Comment of field: eth_srst_req
        /*
        eth ?���λ??????
 */
        unsigned int eth_srst_req : 1; /* [5] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 2; /* [7..6] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg156;

/* Define the union U_PERI_CRG157 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 24; /* [23..0] */
        // Comment of field: fephy_cksel
        /*
        ??????
 */
        unsigned int fephy_cksel : 1; /* [24] */
        // Comment of field: fephy_ref_cksel
        /*
        ??????
 */
        unsigned int fephy_ref_cksel : 1; /* [25] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [26] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg157;

/* Define the union U_PERI_CRG158 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: a53_pclkdbg_cken
        /*
        ??????
 */
        unsigned int a53_pclkdbg_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 8; /* [8..1] */
        // Comment of field: cs_srst_req
        /*
        ??????
 */
        unsigned int cs_srst_req : 1; /* [9] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 2; /* [11..10] */
        // Comment of field: arm_por_srst_req
        /*
        ??????
 */
        unsigned int arm_por_srst_req : 4; /* [15..12] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 10; /* [25..16] */
        // Comment of field: arm_gic_srst_req
        /*
        ??????
 */
        unsigned int arm_gic_srst_req : 1; /* [26] */
        // Comment of field: cpu_clk_off_cken
        /*
        ??????
 */
        unsigned int cpu_clk_off_cken : 1; /* [27] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg158;

/* Define the union U_PERI_CRG159 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dpll_ssmod_ctrl
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
        unsigned int dpll_ssmod_ctrl : 13; /* [12..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg159;

/* Define the union U_PERI_CRG160 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: gpll_ssmod_ctrl
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
        unsigned int gpll_ssmod_ctrl : 13; /* [12..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg160;

/* Define the union U_PERI_CRG163 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: audio_codec_rst_n
        /*
        audio_codec_rst ??
        0????λ??
        1??????λ??
 */
        unsigned int audio_codec_rst_n : 1; /* [0] */
        unsigned int reserved_0 : 23;       /* [23..1] */
        unsigned int acodec_ana_cksel : 1;  /* [24] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 7; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg163;

/* Define the union U_PERI_CRG165 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dtmb_adc_cken
        /*
        hidtmb adc ʱ???ſ? ??
        0???ضϣ?
        1???򿪡?
 */
        unsigned int dtmb_adc_cken : 1; /* [0] */
        // Comment of field: dtmb_axi_cken
        /*
        hidtmb axi ʱ???ſ? ??
        0???ضϣ?
        1???򿪡?
 */
        unsigned int dtmb_axi_cken : 1;       /* [1] */
        unsigned int dtmb_dem_cken : 1;       /* [2] */
        unsigned int dtmb_dem_mult2_cken : 1; /* [3] */
        unsigned int dtmb_dem_mult4_cken : 1; /* [4] */
        unsigned int reserved_0 : 11;         /* [15..5] */
        unsigned int dtmb_srst_req : 1;       /* [16] */
        unsigned int reserved_1 : 7;          /* [23..17] */
        unsigned int dtmb_cksel : 1;          /* [24] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 7; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg165;

/* Define the union U_PERI_CRG166 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hdmirx_otp_pclk_cken
        /*
        hdmirx otp ʱ???ſ? ??
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_otp_pclk_cken : 1; /* [0] */
        // Comment of field: hdmirx_otp_pclk_srst_req
        /*
        hidtmb adc ʱ???ſ? ??
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_otp_pclk_srst_req : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg166;

/* Define the union U_PERI_CRG175 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vbi_cken
        /*
        vbiʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vbi_cken : 1; /* [0] */
        // Comment of field: vbi_srst_req
        /*
        vbi?���λ??????
        0??????λ??
        1????λ??
        
        VbiĬ???Ǹ?λ?ģ??Ѿ???????ȷ?Ϲ???
 */
        unsigned int vbi_srst_req : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg175;

/* Define the union U_PERI_CRG176 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ssp0_ldc_pclk_cken
        /*
        ssp0 ldc pclk ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ssp0_ldc_pclk_cken : 1; /* [0] */
        // Comment of field: ssp0_ldc_pclk_srst_req
        /*
        ssp0 ldc pclk ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ssp0_ldc_pclk_srst_req : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 6; /* [7..2] */
        // Comment of field: ssp0_ldc_cken
        /*
        ssp0 ldc ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ssp0_ldc_cken : 1; /* [8] */
        // Comment of field: ssp0_ldc_srst_req
        /*
        ssp0 ldc ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ssp0_ldc_srst_req : 1; /* [9] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 2; /* [11..10] */
        // Comment of field: ssp0_ldc_div_cksel
        /*
        ssp0 ldc ??Ƶ?????ź?,֧??1~256ż????Ƶ
        Ĭ??8??Ƶ??
        0??????Ƶ??
        1??2??Ƶ??
        7??8??Ƶ??
        ???????ơ?
 */
        unsigned int ssp0_ldc_div_cksel : 8; /* [19..12] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 4; /* [23..20] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg176;

/* Define the union U_PERI_CRG184 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ssp1_ldc_pclk_cken
        /*
        ssp1 ldc pclk ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ssp1_ldc_pclk_cken : 1; /* [0] */
        // Comment of field: ssp1_ldc_pclk_srst_req
        /*
        ssp1 ldc pclk ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ssp1_ldc_pclk_srst_req : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 6; /* [7..2] */
        // Comment of field: ssp1_ldc_cken
        /*
        ssp1 ldc ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ssp1_ldc_cken : 1; /* [8] */
        // Comment of field: ssp1_ldc_srst_req
        /*
        ssp1 ldc ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ssp1_ldc_srst_req : 1; /* [9] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 2; /* [11..10] */
        // Comment of field: ssp1_ldc_div_cksel
        /*
        ssp1 ldc ??Ƶ?????ź?,֧??1~256ż????Ƶ
        Ĭ??8??Ƶ??
        0??????Ƶ??
        1??2??Ƶ??
        7??8??Ƶ??
        ???????ơ?
 */
        unsigned int ssp1_ldc_div_cksel : 8; /* [19..12] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 4; /* [23..20] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg184;

/* Define the union U_PERI_CRG185 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserve_for_eco_m0_subsys_crg_in
        /*
        ????qtc????????????
        ??2??????cvbsһ·ʱ???????򣬴???Ŀ??????
        ??1??????rgbһ·ʱ???????򣬴???Ŀ??????
        ??0??????mclk_codecʱ???????򣬴???Ŀ??????
 */
        unsigned int reserve_for_eco_m0_subsys_crg_in : 20; /* [19..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg185;

/* Define the union U_PERI_CRG186 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserve_for_eco_m0_subsys_crg_out
        /*
        ??????
 */
        unsigned int reserve_for_eco_m0_subsys_crg_out : 20; /* [19..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg186;

/* Define the union U_PERI_CRG187 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserve_for_eco_rx2p0_pwd_in
        /*
        ??????
 */
        unsigned int reserve_for_eco_rx2p0_pwd_in : 8; /* [7..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg187;

/* Define the union U_PERI_CRG188 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserve_for_eco_rx2p0_pwd_out
        /*
        ??????
 */
        unsigned int reserve_for_eco_rx2p0_pwd_out : 8; /* [7..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg188;

/* Define the union U_PERI_CRG191 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: lvds_cken
        /*
        lvdsʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int lvds_cken : 1; /* [0] */
        // Comment of field: gpio_pwm_srst_req
        /*
        pwm_sync_srst_req ?���λ??????
 */
        unsigned int gpio_pwm_srst_req : 1; /* [1] */
        // Comment of field: gpio_pwm_cken
        /*
        pwm_sync_srst_req ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpio_pwm_cken : 1; /* [2] */
        // Comment of field: hpll_ref_cksel
        /*
        hpll?Ĳο?ʱ??ѡ???źš?
        0??????ʱ?ӣ?
        1??clk_75m_ini??
 */
        unsigned int hpll_ref_cksel : 1; /* [3] */
        // Comment of field: lvds_pctrl
        /*
        lvdsʱ?????????ɿ?
        0????????
        1????????
 */
        unsigned int lvds_pctrl : 1; /* [4] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [7..5] */
        // Comment of field: lvds_cksel
        /*
        lvds ??Ƶ?????źţ???????ֻ֧??1/2/4??Ƶ
        00??1??Ƶ??
        01??2??Ƶ??
        10??3??Ƶ??
        11??4??Ƶ??
 */
        unsigned int lvds_cksel : 2; /* [9..8] */
        // Comment of field: reserved_1
        /*
        p2p_pack ??Ƶ?????źţ???????֧??1-128??Ƶ
        0000000??1??Ƶ??
        0000001??2??Ƶ??
        0000010??3??Ƶ??
        0000011??4??Ƶ??
        ??
        ??
        ??
 */
        unsigned int reserved_1 : 7; /* [16..10] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 15; /* [31..17] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg191;

/* Define the union U_PERI_CRG192 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbo_ppc_cken : 1;  /* [0] */
        unsigned int lvds_ppc_cken : 1; /* [1] */
        // Comment of field: vbo_para_cken
        /*
        vbo paraʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vbo_para_cken : 1; /* [2] */
        // Comment of field: vbo_div_cken
        /*
        vbo divʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int vbo_div_cken : 1;        /* [3] */
        unsigned int tcon_div_cken : 1;       /* [4] */
        unsigned int tcon_div_mult2_cken : 1; /* [5] */
        unsigned int p2p_tcon_cken : 1;       /* [6] */
        unsigned int mlvds_cken : 1;          /* [7] */
        unsigned int p2p_pack_cken : 1;       /* [8] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 7;     /* [15..9] */
        unsigned int lvds_srst_req : 1;  /* [16] */
        unsigned int mlvds_srst_req : 1; /* [17] */
        unsigned int p2p_srst_req : 1;   /* [18] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [23..19] */
        // Comment of field: combo_pctrl
        /*
        clk_combo_dphy_readʱ?????????ɿأ?
        0????????
        1????????
 */
        unsigned int combo_pctrl : 1; /* [24] */
        // Comment of field: combo_aphy_ref_pll_cksel
        /*
        clk_combo_aphy_refʱ?ӵ?2??MUXѡ???źţ?
        0??clk_vo_hd0_ini
        1: clk_vo_hd0_fix_ini
        2??1???ſ?ѡ????��??ʱ?ӡ?
 */
        unsigned int combo_aphy_ref_pll_cksel : 2; /* [26..25] */
        // Comment of field: combo_dphy_cksel
        /*
        clk_combo_dphy_refʱ?ӵ?1??MUXѡ???źţ?
        0??clk_p2p_pack??
        1??clk_mlvds_ppc??
        2??clk_lvds_ppc??
        3??clk_vbo_para??
 */
        unsigned int combo_dphy_cksel : 2; /* [28..27] */
        // Comment of field: combo_aphy_ref_cksel
        /*
        clk_combo_aphy_refʱ??ѡ???źţ?
        0??clk_p2p_tcon??
        1??clk_mlvds_ppc??
        2??clk_lvds_ppc??
        3??clk_vbo_para??
 */
        unsigned int combo_aphy_ref_cksel : 2; /* [30..29] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg192;

/* Define the union U_PERI_CRG193 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int combo_apb_cken : 1;  /* [0] */
        unsigned int combo_gpio_cken : 1; /* [1] */
        unsigned int combo_xtal_cken : 1; /* [2] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 13;         /* [15..3] */
        unsigned int combo_dphy_srst_req : 1; /* [16] */
        unsigned int combo_gpio_srst_req : 1; /* [17] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 6;        /* [23..18] */
        unsigned int combo_gpio_divcfg : 7; /* [30..24] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg193;

/* Define the union U_PERI_CRG195 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vdp_apb_cken : 1;      /* [0] */
        unsigned int vdp_axi_cken : 1;      /* [1] */
        unsigned int vdp_cfg_cken : 1;      /* [2] */
        unsigned int vdp_ppc_cken : 1;      /* [3] */
        unsigned int vdp_ppc2_cken : 1;     /* [4] */
        unsigned int vdp_ppc_div2_cken : 1; /* [5] */
        unsigned int vdp_ppc_mult_cken : 1; /* [6] */
        unsigned int hwbuf_apb_cken : 1;    /* [7] */
        unsigned int vdp_smmu_apb_cken : 1; /* [8] */
        unsigned int vdp_smmu_axi_cken : 1; /* [9] */
        unsigned int reserved_0 : 6;        /* [15..10] */
        unsigned int vdp_smmu_srst_req : 1; /* [16] */
        unsigned int hwbuf_srst_req : 1;    /* [17] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 14; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg195;

/* Define the union U_PERI_CRG196 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vdp_disp_cfg_cken : 1;  /* [0] */
        unsigned int vdp_disp_apb_cken : 1;  /* [1] */
        unsigned int vdp_avcom_cken : 1;     /* [2] */
        unsigned int vdp_hdmi_cken : 1;      /* [3] */
        unsigned int vo_hd1_cken : 1;        /* [4] */
        unsigned int reserved_0 : 11;        /* [15..5] */
        unsigned int vdp_disp_srst_req : 1;  /* [16] */
        unsigned int vdp_avcom_srst_req : 1; /* [17] */
        unsigned int vo_hd1_srst_req : 1;    /* [18] */
        unsigned int vbo_para_srst_req : 1;  /* [19] */
        unsigned int vbo_div_srst_req : 1;   /* [20] */
        unsigned int reserved_1 : 3;         /* [23..21] */
        unsigned int vdp_hdmi_cksel : 1;     /* [24] */
        unsigned int vdp_4k_cksel : 1;       /* [25] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 6; /* [31..26] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg196;

/* Define the union U_PERI_CRG197 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 7;       /* [6..0] */
        unsigned int reserved_1 : 1;       /* [7] */
        unsigned int vo_hd1_divcfg : 7;    /* [14..8] */
        unsigned int reserved_2 : 1;       /* [15] */
        unsigned int vdp_avcom_divcfg : 7; /* [22..16] */
        unsigned int reserved_3 : 1;       /* [23] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg197;

/* Define the union U_PERI_CRG200 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int trng_cken : 1;      /* [0] */
        unsigned int efuse_cken : 1;     /* [1] */
        unsigned int reserved_0 : 14;    /* [15..2] */
        unsigned int trng_srst_req : 1;  /* [16] */
        unsigned int efuse_srst_req : 1; /* [17] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 14; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg200;

/* Define the union U_PERI_CRG201 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 24;           /* [23..0] */
        unsigned int acodec_refclk_cksel : 1;   /* [24] */
        unsigned int equator_aif_src_cksel : 1; /* [25] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 6; /* [31..26] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg201;

/* Define the union U_PERI_CRG256 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: hdmirx_2p0_ext_apll_cken
        /*
        ext apllʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ext_apll_cken : 1; /* [1] */
        // Comment of field: hdmirx_2p0_hdcp2x_cclk_cken
        /*
        hdcp2x cclkʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_hdcp2x_cclk_cken : 1; /* [2] */
        // Comment of field: hdmirx_2p0_hdcp2x_eclk_cken
        /*
        hdcp2x eclkʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_hdcp2x_eclk_cken : 1; /* [3] */
        // Comment of field: hdmirx_2p0_pwd_pclk_cken
        /*
        pclk ??cken?ź?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_pwd_pclk_cken : 1; /* [4] */
        // Comment of field: hdmirx_2p0_pwd_pclk_srst_req
        /*
        pclk ???���λ????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_pwd_pclk_srst_req : 1; /* [5] */
        // Comment of field: hdmirx_2p0_hdcp2x_cclk_srst_req
        /*
        hdcp2x cclk?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_hdcp2x_cclk_srst_req : 1; /* [6] */
        // Comment of field: hdmirx_2p0_hdcp2x_eclk_srst_req
        /*
        hdcp2x eclk?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_hdcp2x_eclk_srst_req : 1; /* [7] */
        // Comment of field: hdmirx_2p0_fiona_osc_cken
        /*
        fiona osc ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_fiona_osc_cken : 1; /* [8] */
        // Comment of field: hdmirx_2p0_fiona_osc_srst_req
        /*
        fiona osc ?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_fiona_osc_srst_req : 1; /* [9] */
        // Comment of field: hdmirx_2p0_dacr_refclk_cken
        /*
        dacr refclk ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_dacr_refclk_cken : 1; /* [10] */
        // Comment of field: hdmirx_2p0_600m_clk_sel
        /*
        dacr 600m refclk ʱ??ѡ????
        0??500m??epll??չƵ????
        1??600m??mpll??չƵ????
 */
        unsigned int hdmirx_2p0_600m_clk_sel : 1; /* [11] */
        // Comment of field: data_l0_cksel
        /*
        clk_data_l0ʱ??ѡ????
        00??hdmi_phy_port0??l0ʱ?ӣ?
        01??hdmi_phy_port1??l0ʱ?ӣ?
        10??hdmi_phy_port2??l0ʱ?ӣ?
        11??dft????ʱ?ӡ?
 */
        unsigned int data_l0_cksel : 2; /* [13..12] */
        // Comment of field: data_l1_cksel
        /*
        clk_data_l1ʱ??ѡ????
        00??hdmi_phy_port0??l0ʱ?ӣ?
        01??hdmi_phy_port1??l0ʱ?ӣ?
        10??hdmi_phy_port2??l0ʱ?ӣ?
        11??dft????ʱ?ӡ?
 */
        unsigned int data_l1_cksel : 2; /* [15..14] */
        // Comment of field: data_l2_cksel
        /*
        clk_data_l2ʱ??ѡ????
        00??hdmi_phy_port0??l0ʱ?ӣ?
        01??hdmi_phy_port1??l0ʱ?ӣ?
        10??hdmi_phy_port2??l0ʱ?ӣ?
        11??dft????ʱ?ӡ?
 */
        unsigned int data_l2_cksel : 2; /* [17..16] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 14; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg256;

/* Define the union U_PERI_CRG257 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hdmirx_2p0_dvi_cken_p0
        /*
        hdmirx_2p0_dviʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_dvi_cken_p0 : 1; /* [0] */
        // Comment of field: hdmirx_2p0_ref_lane0_cken_p0
        /*
        hdmirx_2p0_ref_lane0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ref_lane0_cken_p0 : 1; /* [1] */
        // Comment of field: hdmirx_2p0_ref_lane1_cken_p0
        /*
        hdmirx_2p0_ref_lane1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ref_lane1_cken_p0 : 1; /* [2] */
        // Comment of field: hdmirx_2p0_ref_lane2_cken_p0
        /*
        hdmirx_2p0_ref_lane2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ref_lane2_cken_p0 : 1; /* [3] */
        // Comment of field: hdmirx_2p0_fb_lane0_cken_p0
        /*
        hdmirx_2p0_fb_lane0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_fb_lane0_cken_p0 : 1; /* [4] */
        // Comment of field: hdmirx_2p0_fb_lane1_cken_p0
        /*
        hdmirx_2p0_fb_lane1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_fb_lane1_cken_p0 : 1; /* [5] */
        // Comment of field: hdmirx_2p0_fb_lane2_cken_p0
        /*
        hdmirx_2p0_fb_lane2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_fb_lane2_cken_p0 : 1; /* [6] */
        // Comment of field: dvi_from_phy_p0_cksel
        /*
        clk_dvi port0ʱ??ѡ????
        phy_p0??clk_dvi??
        1??dft ʱ?ӡ?
 */
        unsigned int dvi_from_phy_p0_cksel : 1; /* [7] */
        // Comment of field: dvi_from_phy_p1_cksel
        /*
        clk_dvi port1ʱ??ѡ????
        phy_p1??clk_dvi??
        1??dft ʱ?ӡ?
 */
        unsigned int dvi_from_phy_p1_cksel : 1; /* [8] */
        // Comment of field: dvi_from_phy_p2_cksel
        /*
        clk_dvi port2ʱ??ѡ????
        0??phy_p2??clk_dvi??
        1??dft ʱ?ӡ?
 */
        unsigned int dvi_from_phy_p2_cksel : 1; /* [9] */
        // Comment of field: ref_lane0_from_phy_p0_cksel
        /*
        clk_ref_lane0 port0ʱ??ѡ????
        0??phy_p0??lane0??
        1??dft ʱ?ӡ?
 */
        unsigned int ref_lane0_from_phy_p0_cksel : 1; /* [10] */
        // Comment of field: ref_lane0_from_phy_p1_cksel
        /*
        clk_ref_lane0 port1ʱ??ѡ????
        0??phy_p1??lane0??
        1??dft ʱ?ӡ?
 */
        unsigned int ref_lane0_from_phy_p1_cksel : 1; /* [11] */
        // Comment of field: ref_lane0_from_phy_p2_cksel
        /*
        clk_ref_lane0 port2ʱ??ѡ????
        0??phy_p2??lane0??
        1??dft ʱ?ӡ?
 */
        unsigned int ref_lane0_from_phy_p2_cksel : 1; /* [12] */
        // Comment of field: ref_lane1_from_phy_p0_cksel
        /*
        clk_ref_lane1 port0ʱ??ѡ????
        0??phy_p0??lane1??
        1??dft ʱ?ӡ?
 */
        unsigned int ref_lane1_from_phy_p0_cksel : 1; /* [13] */
        // Comment of field: ref_lane1_from_phy_p1_cksel
        /*
        clk_ref_lane1 port1ʱ??ѡ????
        0??phy_p1??lane1??
        1??dft ʱ?ӡ?
 */
        unsigned int ref_lane1_from_phy_p1_cksel : 1; /* [14] */
        // Comment of field: ref_lane1_from_phy_p2_cksel
        /*
        clk_ref_lane1 port2ʱ??ѡ????
        0??phy_p2??lane1??
        1??dft ʱ?ӡ?
 */
        unsigned int ref_lane1_from_phy_p2_cksel : 1; /* [15] */
        // Comment of field: ref_lane2_from_phy_p0_cksel
        /*
        clk_ref_lane2 port0ʱ??ѡ????
        0??phy_p0??lane2??
        1??dft ʱ?ӡ?
 */
        unsigned int ref_lane2_from_phy_p0_cksel : 1; /* [16] */
        // Comment of field: ref_lane2_from_phy_p1_cksel
        /*
        clk_ref_lane2 port1ʱ??ѡ????
        0??phy_p1??lane2??
        1??dft ʱ?ӡ?
 */
        unsigned int ref_lane2_from_phy_p1_cksel : 1; /* [17] */
        // Comment of field: ref_lane2_from_phy_p2_cksel
        /*
        clk_ref_lane2 port2ʱ??ѡ????
        0??phy_p2??lane2??
        1??dft ʱ?ӡ?
 */
        unsigned int ref_lane2_from_phy_p2_cksel : 1; /* [18] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 13; /* [31..19] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg257;

/* Define the union U_PERI_CRG258 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hdmirx_2p0_data_l0_cken
        /*
        hdmirx_2p0_data_l0_ckenʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l0_cken : 1; /* [0] */
        // Comment of field: hdmirx_2p0_data_l1_cken
        /*
        hdmirx_2p0_data_l1_ckenʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l1_cken : 1; /* [1] */
        // Comment of field: hdmirx_2p0_data_l2_cken
        /*
        hdmirx_2p0_data_l2_ckenʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l2_cken : 1; /* [2] */
        // Comment of field: hdmirx_2p0_hls_cken
        /*
        hdmirx_2p0_hls_ckenʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_hls_cken : 1; /* [3] */
        // Comment of field: hdmirx_2p0_ls_cken
        /*
        hdmirx_2p0_ls_ckenʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ls_cken : 1; /* [4] */
        // Comment of field: hdmirx_2p0_dp_clr_cken
        /*
        hdmirx_2p0_dp_clr_ckenʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_dp_clr_cken : 1; /* [5] */
        // Comment of field: hdmirx_2p0_hls_cksel
        /*
        clk hls ??Դͷʱ??ѡ????
        0??clk_data_l0??
        1??clk_data_l1??
        2??clk_data_l2.
 */
        unsigned int hdmirx_2p0_hls_cksel : 2; /* [7..6] */
        // Comment of field: hdmirx_2p0_data_srst_req
        /*
        hdmirx_2p0_data_srst_req?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_data_srst_req : 1; /* [8] */
        // Comment of field: hdmirx_2p0_dphy_xtal_cken
        /*
        dphy ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_dphy_xtal_cken : 1; /* [9] */
        // Comment of field: hdmirx_2p0_xtal_dphy_srst_req
        /*
        dphy ????ʱ?ӵ??���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_xtal_dphy_srst_req : 1; /* [10] */
        // Comment of field: fb_lane0_from_phy_p0_cksel
        /*
        clk_fb_lane0 port0ʱ??ѡ????
        0??phy_p0??lane0??
        1??dft ʱ?ӡ?
 */
        unsigned int fb_lane0_from_phy_p0_cksel : 1; /* [11] */
        // Comment of field: fb_lane0_from_phy_p1_cksel
        /*
        clk_fb_lane0 port1ʱ??ѡ????
        0??phy_p1??lane0??
        1??dft ʱ?ӡ?
 */
        unsigned int fb_lane0_from_phy_p1_cksel : 1; /* [12] */
        // Comment of field: fb_lane0_from_phy_p2_cksel
        /*
        clk_fb_lane0 port2ʱ??ѡ????
        0??phy_p2??lane0??
        1??dft ʱ?ӡ?
 */
        unsigned int fb_lane0_from_phy_p2_cksel : 1; /* [13] */
        // Comment of field: fb_lane1_from_phy_p0_cksel
        /*
        clk_fb_lane1 port0ʱ??ѡ????
        0??phy_p0??lane1??
        1??dft ʱ?ӡ?
 */
        unsigned int fb_lane1_from_phy_p0_cksel : 1; /* [14] */
        // Comment of field: fb_lane1_from_phy_p1_cksel
        /*
        clk_fb_lane1 port1ʱ??ѡ????
        0??phy_p1??lane1??
        1??dft ʱ?ӡ?
 */
        unsigned int fb_lane1_from_phy_p1_cksel : 1; /* [15] */
        // Comment of field: fb_lane1_from_phy_p2_cksel
        /*
        clk_fb_lane1 port2ʱ??ѡ????
        0??phy_p2??lane1??
        1??dft ʱ?ӡ?
 */
        unsigned int fb_lane1_from_phy_p2_cksel : 1; /* [16] */
        // Comment of field: fb_lane2_from_phy_p0_cksel
        /*
        clk_fb_lane2 port0ʱ??ѡ????
        0??phy_p0??lane2??
        1??dft ʱ?ӡ?
 */
        unsigned int fb_lane2_from_phy_p0_cksel : 1; /* [17] */
        // Comment of field: fb_lane2_from_phy_p1_cksel
        /*
        clk_fb_lane2 port1ʱ??ѡ????
        0??phy_p1??lane2??
        1??dft ʱ?ӡ?
 */
        unsigned int fb_lane2_from_phy_p1_cksel : 1; /* [18] */
        // Comment of field: fb_lane2_from_phy_p2_cksel
        /*
        clk_fb_lane2 port2ʱ??ѡ????
        0??phy_p2??lane2??
        1??dft ʱ?ӡ?
 */
        unsigned int fb_lane2_from_phy_p2_cksel : 1; /* [19] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg258;

/* Define the union U_PERI_CRG259 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hdmirx_2p0_rx_test_pclk_cksel
        /*
        i_rx_test_pclk?ķ?Ƶ?????źţ?
        00??1??Ƶ??
        01??2??Ƶ??
        10??3??Ƶ??
        11??4??Ƶ??
 */
        unsigned int hdmirx_2p0_rx_test_pclk_cksel : 2; /* [1..0] */
        // Comment of field: hdmirx_2p0_pwd_sheman_srst_req
        /*
        pwd??sheman ?���λ??????
 */
        unsigned int hdmirx_2p0_pwd_sheman_srst_req : 1; /* [2] */
        // Comment of field: hdmirx_2p0_rx_test_srst_req
        /*
        hdmirx_2p0_rx_test_srst_req?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_rx_test_srst_req : 1; /* [3] */
        // Comment of field: hdmirx_2p0_i_rx_test_pclk_cken
        /*
        hdmirx_2p0_i_rx_test_pclk_ckenʱ???ſأ?
        0???ضϣ?
        1???򿪡?
        ????һ???????õ?ʱ?ӣ??????Բ??ã???????ȷ??
        Ĭ?ϲ?????
 */
        unsigned int hdmirx_2p0_i_rx_test_pclk_cken : 1; /* [4] */
        // Comment of field: clk_ls_in_p0_cksel
        /*
        ??????
 */
        unsigned int clk_ls_in_p0_cksel : 1; /* [5] */
        // Comment of field: clk_ls_in_p1_cksel
        /*
        ??????
 */
        unsigned int clk_ls_in_p1_cksel : 1; /* [6] */
        // Comment of field: clk_ls_in_p2_cksel
        /*
        ??????
 */
        unsigned int clk_ls_in_p2_cksel : 1; /* [7] */
        // Comment of field: clk_dp_clr_in_p0_cksel
        /*
        ??????
 */
        unsigned int clk_dp_clr_in_p0_cksel : 1; /* [8] */
        // Comment of field: clk_dp_clr_in_p1_cksel
        /*
        ?��?TZASC??Ȩʧ?��??ص?CPU????slaverr??      0????ֹ?��Σ?Ĭ??ģʽ??                                   1??ʹ???��Σ??��?ģʽ??
 */
        unsigned int clk_dp_clr_in_p1_cksel : 1; /* [9] */
        // Comment of field: clk_dp_clr_in_p2_cksel
        /*
        ??????
 */
        unsigned int clk_dp_clr_in_p2_cksel : 1; /* [10] */
        // Comment of field: hdmirx_2p0_dp_clr_cksel
        /*
        clk_dp_clrʱ??ѡ????
        00??port0??clk_dp_clr??
        01??port1??clk_dp_clr??
        10??port2??clk_dp_clr??
        11??dft ʱ?ӣ?
 */
        unsigned int hdmirx_2p0_dp_clr_cksel : 2; /* [12..11] */
        // Comment of field: hdmirx_2p0_ls_cksel
        /*
        clk_lsʱ??ѡ????
        00??port0??clk_ls??
        01??port1??clk_ls??
        10??port2??clk_ls??
        11??dft ʱ?ӣ?
 */
        unsigned int hdmirx_2p0_ls_cksel : 2; /* [14..13] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 17; /* [31..15] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg259;

/* Define the union U_PERI_CRG260 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hdmirx_2p0_vid_cksel
        /*
        o_vid_clk ʱ??ѡ???źţ?
        0?? ????Ƶ??
        1?? 2??Ƶ??
 */
        unsigned int hdmirx_2p0_vid_cksel : 1; /* [0] */
        // Comment of field: hdmirx_2p0_dvi_cken_p1
        /*
        hdmirx_2p0_dviʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_dvi_cken_p1 : 1; /* [1] */
        // Comment of field: hdmirx_2p0_ref_lane0_cken_p1
        /*
        hdmirx_2p0_ref_lane0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ref_lane0_cken_p1 : 1; /* [2] */
        // Comment of field: hdmirx_2p0_ref_lane1_cken_p1
        /*
        hdmirx_2p0_ref_lane1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ref_lane1_cken_p1 : 1; /* [3] */
        // Comment of field: hdmirx_2p0_ref_lane2_cken_p1
        /*
        hdmirx_2p0_ref_lane2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ref_lane2_cken_p1 : 1; /* [4] */
        // Comment of field: hdmirx_2p0_fb_lane0_cken_p1
        /*
        hdmirx_2p0_fb_lane0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_fb_lane0_cken_p1 : 1; /* [5] */
        // Comment of field: hdmirx_2p0_fb_lane1_cken_p1
        /*
        hdmirx_2p0_fb_lane1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_fb_lane1_cken_p1 : 1; /* [6] */
        // Comment of field: hdmirx_2p0_fb_lane2_cken_p1
        /*
        hdmirx_2p0_fb_lane2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_fb_lane2_cken_p1 : 1; /* [7] */
        // Comment of field: hdmirx_2p0_dvi_cken_p2
        /*
        hdmirx_2p0_dviʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_dvi_cken_p2 : 1; /* [8] */
        // Comment of field: hdmirx_2p0_ref_lane0_cken_p2
        /*
        hdmirx_2p0_ref_lane0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ref_lane0_cken_p2 : 1; /* [9] */
        // Comment of field: hdmirx_2p0_ref_lane1_cken_p2
        /*
        hdmirx_2p0_ref_lane1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ref_lane1_cken_p2 : 1; /* [10] */
        // Comment of field: hdmirx_2p0_ref_lane2_cken_p2
        /*
        hdmirx_2p0_ref_lane2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_ref_lane2_cken_p2 : 1; /* [11] */
        // Comment of field: hdmirx_2p0_fb_lane0_cken_p2
        /*
        hdmirx_2p0_fb_lane0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_fb_lane0_cken_p2 : 1; /* [12] */
        // Comment of field: hdmirx_2p0_fb_lane1_cken_p2
        /*
        hdmirx_2p0_fb_lane1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_fb_lane1_cken_p2 : 1; /* [13] */
        // Comment of field: hdmirx_2p0_fb_lane2_cken_p2
        /*
        hdmirx_2p0_fb_lane2ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_fb_lane2_cken_p2 : 1; /* [14] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 17; /* [31..15] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg260;

/* Define the union U_PERI_CRG261 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: data_l0_p0_cksel
        /*
        data_l0_p0 ʱ??ѡ????
        0??clk_data_port0_line0??
        1??dft clk??
 */
        unsigned int data_l0_p0_cksel : 1; /* [0] */
        // Comment of field: data_l1_p0_cksel
        /*
        data_l1_p0 ʱ??ѡ????
        0??clk_data_port0_line1??
        1??dft clk??
 */
        unsigned int data_l1_p0_cksel : 1; /* [1] */
        // Comment of field: data_l2_p0_cksel
        /*
        data_l2_p0 ʱ??ѡ????
        0??clk_data_port0_line2??
        1??dft clk??
 */
        unsigned int data_l2_p0_cksel : 1; /* [2] */
        // Comment of field: data_l0_p1_cksel
        /*
        data_l0_p1 ʱ??ѡ????
        0??clk_data_port1_line0??
        1??dft clk??
 */
        unsigned int data_l0_p1_cksel : 1; /* [3] */
        // Comment of field: data_l1_p1_cksel
        /*
        data_l1_p1 ʱ??ѡ????
        0??clk_data_port1_line1??
        1??dft clk??
 */
        unsigned int data_l1_p1_cksel : 1; /* [4] */
        // Comment of field: data_l2_p1_cksel
        /*
        data_l2_p1 ʱ??ѡ????
        0??clk_data_port1_line2??
        1??dft clk??
 */
        unsigned int data_l2_p1_cksel : 1; /* [5] */
        // Comment of field: data_l0_p2_cksel
        /*
        data_l0_p2 ʱ??ѡ????
        0??clk_data_port2_line0??
        1??dft clk??
 */
        unsigned int data_l0_p2_cksel : 1; /* [6] */
        // Comment of field: data_l1_p2_cksel
        /*
        data_l1_p2 ʱ??ѡ????
        0??clk_data_port2_line1??
        1??dft clk??
 */
        unsigned int data_l1_p2_cksel : 1; /* [7] */
        // Comment of field: data_l2_p2_cksel
        /*
        data_l2_p2 ʱ??ѡ????
        0??clk_data_port2_line2??
        1??dft clk??
 */
        unsigned int data_l2_p2_cksel : 1; /* [8] */
        // Comment of field: hdmirx_2p0_data_l0_p0_cken
        /*
        data_l0_p0 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l0_p0_cken : 1; /* [9] */
        // Comment of field: hdmirx_2p0_data_l1_p0_cken
        /*
        data_l1_p0 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l1_p0_cken : 1; /* [10] */
        // Comment of field: hdmirx_2p0_data_l2_p0_cken
        /*
        data_l2_p0 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l2_p0_cken : 1; /* [11] */
        // Comment of field: hdmirx_2p0_data_l0_p1_cken
        /*
        data_l0_p1 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l0_p1_cken : 1; /* [12] */
        // Comment of field: hdmirx_2p0_data_l1_p1_cken
        /*
        data_l1_p1 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l1_p1_cken : 1; /* [13] */
        // Comment of field: hdmirx_2p0_data_l2_p1_cken
        /*
        data_l2_p1 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l2_p1_cken : 1; /* [14] */
        // Comment of field: hdmirx_2p0_data_l0_p2_cken
        /*
        data_l0_p2 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l0_p2_cken : 1; /* [15] */
        // Comment of field: hdmirx_2p0_data_l1_p2_cken
        /*
        data_l1_p2 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l1_p2_cken : 1; /* [16] */
        // Comment of field: hdmirx_2p0_data_l2_p2_cken
        /*
        data_l2_p2 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_data_l2_p2_cken : 1; /* [17] */
        // Comment of field: hdmirx_2p0_phase_l0_p0_cken
        /*
        phase_l0_p0 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_phase_l0_p0_cken : 1; /* [18] */
        // Comment of field: hdmirx_2p0_phase_l1_p0_cken
        /*
        phase_l1_p0 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_phase_l1_p0_cken : 1; /* [19] */
        // Comment of field: hdmirx_2p0_phase_l2_p0_cken
        /*
        phase_l2_p0 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_phase_l2_p0_cken : 1; /* [20] */
        // Comment of field: hdmirx_2p0_phase_l0_p1_cken
        /*
        phase_l0_p1 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_phase_l0_p1_cken : 1; /* [21] */
        // Comment of field: hdmirx_2p0_phase_l1_p1_cken
        /*
        phase_l1_p1 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_phase_l1_p1_cken : 1; /* [22] */
        // Comment of field: hdmirx_2p0_phase_l2_p1_cken
        /*
        phase_l2_p1 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_phase_l2_p1_cken : 1; /* [23] */
        // Comment of field: hdmirx_2p0_phase_l0_p2_cken
        /*
        phase_l0_p2 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_phase_l0_p2_cken : 1; /* [24] */
        // Comment of field: hdmirx_2p0_phase_l1_p2_cken
        /*
        phase_l1_p2 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_phase_l1_p2_cken : 1; /* [25] */
        // Comment of field: hdmirx_2p0_phase_l2_p2_cken
        /*
        phase_l2_p2 ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_phase_l2_p2_cken : 1; /* [26] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg261;

/* Define the union U_PERI_CRG262 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [0] */
        // Comment of field: hdmirx_2p0_l0_p0_srst_req
        /*
        clk_l0_p0???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_l0_p0_srst_req : 1; /* [1] */
        // Comment of field: hdmirx_2p0_l1_p0_srst_req
        /*
        clk_l1_p0???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_l1_p0_srst_req : 1; /* [2] */
        // Comment of field: hdmirx_2p0_l2_p0_srst_req
        /*
        clk_l2_p0???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_l2_p0_srst_req : 1; /* [3] */
        // Comment of field: hdmirx_2p0_l0_p1_srst_req
        /*
        clk_l0_p1???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_l0_p1_srst_req : 1; /* [4] */
        // Comment of field: hdmirx_2p0_l1_p1_srst_req
        /*
        clk_l1_p1???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_l1_p1_srst_req : 1; /* [5] */
        // Comment of field: hdmirx_2p0_l2_p1_srst_req
        /*
        clk_l2_p1???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_l2_p1_srst_req : 1; /* [6] */
        // Comment of field: hdmirx_2p0_l0_p2_srst_req
        /*
        clk_l0_p2???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_l0_p2_srst_req : 1; /* [7] */
        // Comment of field: hdmirx_2p0_l1_p2_srst_req
        /*
        clk_l1_p2???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_l1_p2_srst_req : 1; /* [8] */
        // Comment of field: hdmirx_2p0_l2_p2_srst_req
        /*
        clk_l2_p2???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_l2_p2_srst_req : 1; /* [9] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 22; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg262;

/* Define the union U_PERI_CRG263 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hls_p0_cksel
        /*
        clk_hls_p0 ʱ??ѡ????
        0??hls port0 ʱ?ӣ?
        1??dft ʱ?ӡ?
 */
        unsigned int hls_p0_cksel : 1; /* [0] */
        // Comment of field: hls_p1_cksel
        /*
        clk_hls_p1 ʱ??ѡ????
        0??hls port1 ʱ?ӣ?
        1??dft ʱ?ӡ?
 */
        unsigned int hls_p1_cksel : 1; /* [1] */
        // Comment of field: hls_p2_cksel
        /*
        clk_hls_p2 ʱ??ѡ????
        0??hls port2 ʱ?ӣ?
        1??dft ʱ?ӡ?
 */
        unsigned int hls_p2_cksel : 1; /* [2] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [6..3] */
        // Comment of field: hdmirx_2p0_hls_p0_cken
        /*
        clk_hls_p2 ʱ???ſأ?
        0???رգ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_hls_p0_cken : 1; /* [7] */
        // Comment of field: hdmirx_2p0_hls_p1_cken
        /*
        clk_hls_p2 ʱ???ſأ?
        0???رգ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_hls_p1_cken : 1; /* [8] */
        // Comment of field: hdmirx_2p0_hls_p2_cken
        /*
        clk_hls_p2 ʱ???ſأ?
        0???رգ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_hls_p2_cken : 1; /* [9] */
        // Comment of field: hdmirx_2p0_hls_p0_srst_req
        /*
        clk_hls_p0 ???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_hls_p0_srst_req : 1; /* [10] */
        // Comment of field: hdmirx_2p0_hls_p1_srst_req
        /*
        clk_hls_p1 ???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_hls_p1_srst_req : 1; /* [11] */
        // Comment of field: hdmirx_2p0_hls_p2_srst_req
        /*
        clk_hls_p2 ???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_hls_p2_srst_req : 1; /* [12] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg263;

/* Define the union U_PERI_CRG264 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 16;                  /* [15..0] */
        unsigned int hdmirx_test_pattern_srst_req : 1; /* [16] */
        unsigned int reserved_1 : 7;                   /* [23..17] */
        unsigned int hdmirx_2p0_phase_l0_cksel : 1;    /* [24] */
        unsigned int hdmirx_2p0_phase_l1_cksel : 1;    /* [25] */
        unsigned int hdmirx_2p0_phase_l2_cksel : 1;    /* [26] */
        // Comment of field: hdmirx_2p0_test_pclk_cksel
        /*
        HDMIRXPHY?????òο?ʱ?ӣ?
        0: TPLL POSTDIV????ʱ?ӣ?Ĭ??225M??
        1: 300M??
 */
        unsigned int hdmirx_2p0_test_pclk_cksel : 1; /* [27] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg264;

/* Define the union U_PERI_CRG265 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hdmirx_test_pattern_divcfg : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 25; /* [31..7] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg265;

/* Define the union U_PERI_CRG266 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hdmirx_2p1_pwd_link_cken : 1;      /* [0] */
        unsigned int hdmirx_2p1_pwd_link_cfg_cken : 1;  /* [1] */
        unsigned int hdmirx_2p1_pwd_media_cken : 1;     /* [2] */
        unsigned int hdmirx_2p1_pwd_media_cfg_cken : 1; /* [3] */
        unsigned int hdmirx_2p1_pwd_apb_cken : 1;       /* [4] */
        unsigned int reserved_0 : 11;                   /* [15..5] */
        unsigned int hdmirx_2p1_pwd_srst_req : 1;       /* [16] */
        unsigned int hdmirx_2p1_pwd_apb_srst_req : 1;   /* [17] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 14; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg266;

/* Define the union U_PERI_CRG267 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vpss_cfg_cken : 1;      /* [0] */
        unsigned int vpss_apb_cken : 1;      /* [1] */
        unsigned int vpss_axi_cken : 1;      /* [2] */
        unsigned int vpss_cken : 1;          /* [3] */
        unsigned int vpss_m2_cken : 1;       /* [4] */
        unsigned int vpss_smmu_apb_cken : 1; /* [5] */
        unsigned int vpss_smmu_axi_cken : 1; /* [6] */
        unsigned int reserved_0 : 9;         /* [15..7] */
        unsigned int vpss_srst_req : 1;      /* [16] */
        unsigned int vpss_smmu_srst_req : 1; /* [17] */
        unsigned int reserved_1 : 6;         /* [23..18] */
        unsigned int vpss_cksel : 1;         /* [24] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 7; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg267;

/* Define the union U_PERI_CRG270 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vo_hd_tv_cksel : 7; /* [6..0] */
        unsigned int reserved_0 : 1;     /* [7] */
        unsigned int tcon_divcfg : 7;    /* [14..8] */
        unsigned int reserved_1 : 1;     /* [15] */
        unsigned int vbo_para_cksel : 7; /* [22..16] */
        unsigned int reserved_2 : 1;     /* [23] */
        unsigned int vbo_div_cksel : 7;  /* [30..24] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg270;

/* Define the union U_PERI_CRG271 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tcon_div_divcfg : 7;      /* [6..0] */
        unsigned int reserved_0 : 1;           /* [7] */
        unsigned int p2p_tcon_mlvds_cksel : 7; /* [14..8] */
        unsigned int reserved_1 : 1;           /* [15] */
        unsigned int p2p_pack_cksel : 7;       /* [22..16] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 9; /* [31..23] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg271;

/* Define the union U_PERI_CRG290 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: usb2_vcc_srst_req
        /*
        USB XHCI??????VCC?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_vcc_srst_req : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        USB XHCI??????BUSʱ???ſء?
        0???ر?ʱ?ӣ?
        1??????ʱ?ӡ?
 */
        unsigned int reserved_0 : 1; /* [1] */
        // Comment of field: usb2_ref_cken
        /*
        USB XHCI??????REFʱ???ſء?
        0???ر?ʱ?ӣ?
        1??????ʱ?ӡ?
 */
        unsigned int usb2_ref_cken : 1; /* [2] */
        // Comment of field: reserved_1
        /*
        ???����?Ԥ??suspendʱ???ſأ?
 */
        unsigned int reserved_1 : 1; /* [3] */
        // Comment of field: reserved_2
        /*
        USB XHCI??????UTMI0ʱ???ſء?
        0???ر?ʱ?ӣ?
        1??????ʱ?ӡ?
 */
        unsigned int reserved_2 : 1; /* [4] */
        // Comment of field: reserved_3
        /*
        USB XHCI??????UTMI1ʱ???ſء?
        0???ر?ʱ?ӣ?
        1??????ʱ?ӡ?
 */
        unsigned int reserved_3 : 1; /* [5] */
        // Comment of field: reserved_4
        /*
        USB XHCI??????UTMI2ʱ???ſء?
        0???ر?ʱ?ӣ?
        1??????ʱ?ӡ?
 */
        unsigned int reserved_4 : 1; /* [6] */
        // Comment of field: reserved_5
        /*
        ???����?Ԥ??pipe0??pipe1??utmi3??
 */
        unsigned int reserved_5 : 3; /* [9..7] */
        // Comment of field: usb2_freeclk_cksel
        /*
        USB XHCI??????ѡ??freeclk????ʱ?ӡ?
        0??ѡ??UTMI0ʱ?ӣ?
        1??ѡ??FREE 60Mʱ?ӡ?
 */
        unsigned int usb2_freeclk_cksel : 1; /* [10] */
        // Comment of field: reserved_6
        /*
        ??????
 */
        unsigned int reserved_6 : 21; /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg290;

/* Define the union U_PERI_CRG291 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: usb2_bus_cken
        /*
        USB2 EHCI??????????ʱ???ſء?
        0???رգ?
        1???򿪡?
 */
        unsigned int usb2_bus_cken : 1; /* [0] */
        // Comment of field: usb2_ohci48m_cken
        /*
        USB2 EHCI??????48Mʱ???ſء?
        0???رգ?
        1???򿪡?
 */
        unsigned int usb2_ohci48m_cken : 1; /* [1] */
        // Comment of field: usb2_ohci12m_cken
        /*
        USB2 EHCI??????12Mʱ???ſء?
        0???رգ?
        1???򿪡?
 */
        unsigned int usb2_ohci12m_cken : 1; /* [2] */
        // Comment of field: usb2_hst_phy_cken
        /*
        USB2 EHCI??????RootHub 60Mʱ???ſء?
        0???رգ?
        1???򿪡?
 */
        unsigned int usb2_hst_phy_cken : 1; /* [3] */
        // Comment of field: usb2_utmi0_cken
        /*
        USB2 EHCI??????utmi0ʱ???ſء?
        0???رգ?
        1???򿪡?
 */
        unsigned int usb2_utmi0_cken : 1; /* [4] */
        // Comment of field: usb2_utmi1_cken
        /*
        USB2 EHCI??????utmi1ʱ???ſء?
        0???رգ?
        1???򿪡?
 */
        unsigned int usb2_utmi1_cken : 1; /* [5] */
        // Comment of field: usb2_utmi2_cken
        /*
        USB2 EHCI??????utmi2ʱ???ſء?
        0???رգ?
        1???򿪡?
 */
        unsigned int usb2_utmi2_cken : 1; /* [6] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [7] */
        // Comment of field: usb2_bus_srst_req
        /*
        USB2 EHCI????????AHB BUS?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_bus_srst_req : 1; /* [8] */
        // Comment of field: usb2_hst_phy_srst_req
        /*
        USB2 EHCI????????RootHub?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_hst_phy_srst_req : 1; /* [9] */
        // Comment of field: usb2_utmi0_srst_req
        /*
        USB2 EHCI????????utmi0?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_utmi0_srst_req : 1; /* [10] */
        // Comment of field: usb2_utmi1_srst_req
        /*
        USB2 EHCI????????utmi1?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_utmi1_srst_req : 1; /* [11] */
        // Comment of field: usb2_utmi2_srst_req
        /*
        USB2 EHCI????????utmi2?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_utmi2_srst_req : 1; /* [12] */
        // Comment of field: usb2_dev_srst_req
        /*
        ????
 */
        unsigned int usb2_dev_srst_req : 1; /* [13] */
        // Comment of field: usb2_cksel
        /*
        USB2 EHCI????????ʱ??Դͷѡ????
        0????PHY?ṩʱ?ӣ?
        1????CRG?ṩʱ?ӡ?
 */
        unsigned int usb2_cksel : 1; /* [14] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 17; /* [31..15] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg291;

/* Define the union U_PERI_CRG292 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: usb2_phy2_ref_xtal_cken
        /*
        usb2 phy2 refclk_xtal?ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb2_phy2_ref_xtal_cken : 1; /* [0] */
        // Comment of field: usb2_phy2_ref_pll_cken
        /*
        usb2 phy2 refclk_pll?ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb2_phy2_ref_pll_cken : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 14; /* [15..2] */
        // Comment of field: usb2_phy2_srst_req
        /*
        usb2 phy2 por?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_phy2_srst_req : 1; /* [16] */
        // Comment of field: usb2_phy2_srst_treq
        /*
        usb2 phy2 tpor?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_phy2_srst_treq : 1; /* [17] */
        // Comment of field: usb2_phy2_apb_srst_req
        /*
        usb2 phy2 apb?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_phy2_apb_srst_req : 1; /* [18] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 13; /* [31..19] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg292;

/* Define the union U_PERI_CRG293 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: usb2_phy1_ref_xtal_cken
        /*
        usb2 phy1 refclk_xtal?ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb2_phy1_ref_xtal_cken : 1; /* [0] */
        // Comment of field: usb2_phy1_ref_pll_cken
        /*
        usb2 phy1 refclk_pll?ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb2_phy1_ref_pll_cken : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 14; /* [15..2] */
        // Comment of field: usb2_phy1_srst_req
        /*
        usb2 phy1 por?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_phy1_srst_req : 1; /* [16] */
        // Comment of field: usb2_phy1_srst_treq
        /*
        usb2 phy1 tpor?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_phy1_srst_treq : 1; /* [17] */
        // Comment of field: usb2_phy1_apb_srst_req
        /*
        usb2 phy1 apb?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_phy1_apb_srst_req : 1; /* [18] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [19] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg293;

/* Define the union U_PERI_CRG294 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: usb2_phy0_ref_xtal_cken
        /*
        usb2 phy0 refclk_xtal?ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb2_phy0_ref_xtal_cken : 1; /* [0] */
        // Comment of field: usb2_phy0_ref_pll_cken
        /*
        usb2 phy0 refclk_pll?ſء?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int usb2_phy0_ref_pll_cken : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 14; /* [15..2] */
        // Comment of field: usb2_phy0_srst_req
        /*
        usb2 phy0 por?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_phy0_srst_req : 1; /* [16] */
        // Comment of field: usb2_phy0_srst_treq
        /*
        usb2 phy0 tpor?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_phy0_srst_treq : 1; /* [17] */
        // Comment of field: usb2_phy0_apb_srst_req
        /*
        usb2 phy0 apb?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int usb2_phy0_apb_srst_req : 1; /* [18] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 1; /* [19] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg294;

/* Define the union U_PERI_CRG300 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ddr_apb_cken
        /*
        ddrphy1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ddr_apb_cken : 1; /* [0] */
        // Comment of field: ddr_axi_cken
        /*
        ddrphy0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ddr_axi_cken : 1; /* [1] */
        // Comment of field: ddr_cfg_cken
        /*
        hipack0 pclkʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ddr_cfg_cken : 1; /* [2] */
        // Comment of field: ddr_dfi_cken
        /*
        ddrc pclkʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ddr_dfi_cken : 1; /* [3] */
        unsigned int reserved_0 : 12;  /* [15..4] */
        // Comment of field: ddr_apb_pack0_srst_req
        /*
        hipack0 prst?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ddr_apb_pack0_srst_req : 1; /* [16] */
        // Comment of field: ddr_apb_srst_req
        /*
        hipack0 prst?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int ddr_apb_srst_req : 1; /* [17] */
        // Comment of field: reserve_for_eco_ddr
        /*
        ??????
 */
        unsigned int reserve_for_eco_ddr : 9; /* [26..18] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg300;

/* Define the union U_PERI_CRG301 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ddrt_apb_cken
        /*
        ddrphy1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ddrt_apb_cken : 1; /* [0] */
        // Comment of field: ddrt_axi_cken
        /*
        ddrphy0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ddrt_axi_cken : 1;     /* [1] */
        unsigned int reserved_0 : 14;       /* [15..2] */
        unsigned int ddrt_apb_srst_req : 1; /* [16] */
        unsigned int ddrt_axi_srst_req : 1; /* [17] */
        unsigned int reserved_1 : 6;        /* [23..18] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg301;

/* Define the union U_PERI_CRG310 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dsp0_cken
        /*
        dsp0ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int dsp0_cken : 1;   /* [0] */
        unsigned int reserved_0 : 15; /* [15..1] */
        // Comment of field: dsp0_srst_req
        /*
        dsp0?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int dsp0_srst_req : 1; /* [16] */
        // Comment of field: dsp0_dbg_srst_req
        /*
        dsp0_dbg?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int dsp0_dbg_srst_req : 1; /* [17] */
        // Comment of field: dsp0_mmu_srst_req
        /*
        dsp0 mmu?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int dsp0_mmu_srst_req : 1; /* [18] */
        unsigned int reserved_1 : 5;        /* [23..19] */
        // Comment of field: dsp0_cksel
        /*
        dsp0ʱ??ѡ????
        0??600M??
        1??540M??
        2??432M;
        3: KPLL????ʱ?ӡ?
 */
        unsigned int dsp0_cksel : 2; /* [25..24] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 6; /* [31..26] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg310;

/* Define the union U_PERI_CRG311 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dsp1_cken
        /*
        dsp1ʱ???ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int dsp1_cken : 1;   /* [0] */
        unsigned int reserved_0 : 15; /* [15..1] */
        // Comment of field: dsp1_srst_req
        /*
        dsp1?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int dsp1_srst_req : 1; /* [16] */
        // Comment of field: dsp1_dbg_srst_req
        /*
        dsp1_dbg?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int dsp1_dbg_srst_req : 1; /* [17] */
        // Comment of field: dsp1_mmu_srst_req
        /*
        dsp1 mmu?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int dsp1_mmu_srst_req : 1; /* [18] */
        unsigned int reserved_1 : 5;        /* [23..19] */
        // Comment of field: dsp1_cksel
        /*
        dsp1ʱ??ѡ????
        0??600M??
        1??540M??
        2??432M;
        3: KPLL????ʱ?ӡ?
 */
        unsigned int dsp1_cksel : 2; /* [25..24] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 6; /* [31..26] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg311;

/* Define the union U_PERI_CRG320 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: apll_hard_ajust_lock
        /*
        APLL Ӳ??΢????·lockָʾ?źţ?
        0?? ûlock
        1?? lock״̬
 */
        unsigned int apll_hard_ajust_lock : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg320;

/* Define the union U_PERI_CRG321 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: apll_hard_ajust_clk_divval
        /*
        APLL ʱ??Ӳ??΢????·?Ĺ???ʱ?ӷ?Ƶ?ȡ?
        0??????Ƶ??
        1??????Ƶ??
        2??2??Ƶ??
        3??3??Ƶ??
        ????.
 */
        unsigned int apll_hard_ajust_clk_divval : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [7..4] */
        // Comment of field: apll_soft_ajust_clk_divval
        /*
        APLL ʱ??????΢????·?Ĺ???ʱ?ӷ?Ƶ?ȡ?
        0??????Ƶ??
        1??????Ƶ??
        2??2??Ƶ??
        3??3??Ƶ??
        ????.
 */
        unsigned int apll_soft_ajust_clk_divval : 4; /* [11..8] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 4; /* [15..12] */
        // Comment of field: apll_freq_config_end
        /*
        APLL ϵ??????????ָʾ?źš?΢???ڼ?Ҫ????Ϊ1
        0??δ????
        1??????FBDIV,FRAC??????
 */
        unsigned int apll_freq_config_end : 1; /* [16] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [19..17] */
        // Comment of field: apll_sample_old_config
        /*
        APLL  ʱ??΢????·??ʼload???ɵ?????ϵ????
        0????load
        1??load??FBDIV,FRAC??????ϵ??
 */
        unsigned int apll_sample_old_config : 1; /* [20] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 3; /* [23..21] */
        // Comment of field: reserved_4
        /*
        ??????
 */
        unsigned int reserved_4 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg321;

/* Define the union U_PERI_CRG322 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: apll_ajust_cken
        /*
        APLL ʱ????????Ӳ??΢????·ʱ???ſ????üĴ???,Ĭ?ϴ򿪡?
        0???ر?ʱ?ӣ?
        1??????ʱ?ӡ?
 */
        unsigned int apll_ajust_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 3; /* [3..1] */
        // Comment of field: apll_ajust_srst_req
        /*
        APLL ʱ????????Ӳ??΢????·???���λ??????
        0????????λ??
        1????λ??
 */
        unsigned int apll_ajust_srst_req : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 3; /* [7..5] */
        // Comment of field: apll_freq_mode
        /*
        APLL ʱ??????ϵ????ѡ????
        00?????üĴ???ֱ???͹?��??????ϵ??
        01??Ӳ??΢????·????֮????????ϵ??
        1X??????΢????·????֮????????ϵ??
 */
        unsigned int apll_freq_mode : 2; /* [9..8] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 2; /* [11..10] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg322;

/* Define the union U_PERI_CRG323 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: apll_step_int
        /*
        APLL ʱ??Ӳ??΢????·?????????ֲ?????
 */
        unsigned int apll_step_int : 12; /* [11..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg323;

/* Define the union U_PERI_CRG324 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: apll_step_frac
        /*
        APLL ʱ??Ӳ??΢????·??С?????ֲ?????
 */
        unsigned int apll_step_frac : 24; /* [23..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg324;

/* Define the union U_PERI_CRG330 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: emmc_mmc_cken
        /*
        eMMC ʱ???ſ????á?
        0???رգ?
        1???򿪡?
 */
        unsigned int emmc_mmc_cken : 1; /* [0] */
        // Comment of field: emmc_mmc_ahb_cken
        /*
        eMMC AHBʱ???ſ????á?
        0???رգ?
        1???򿪡?
 */
        unsigned int emmc_mmc_ahb_cken : 1; /* [1] */
        unsigned int reserved_0 : 14;       /* [15..2] */
        // Comment of field: emmc_mmc_srst_req
        /*
        eMMC ???���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int emmc_mmc_srst_req : 1; /* [16] */
        // Comment of field: emmc_mmc_rx_srst_req
        /*
        eMMC RX ?????���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int emmc_mmc_rx_srst_req : 1; /* [17] */
        // Comment of field: emmc_mmc_tx_srst_req
        /*
        eMMC TX ?????���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int emmc_mmc_tx_srst_req : 1; /* [18] */
        unsigned int reserved_1 : 5;           /* [23..19] */
        // Comment of field: emmc_clk_sel
        /*
        eMMC 4X ʱ??ѡ?񡣹��?ʱ??Ƶ??Ϊ??4??Ƶ??
        000??1.6MHz??
        001??reserved??
        010??reserved??
        011??EMMCר??PLLʱ?ӣ???չƵ??Ĭ??400MHz??
        100??200MHz????չƵʱ?ӣ?
        101??100MHz????չƵʱ?ӣ?
        110??reserved??
        111??reserved??
 */
        unsigned int emmc_clk_sel : 3; /* [26..24] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg330;

/* Define the union U_PERI_CRG331 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: emmc_p4_dll_stop
        /*
        eMMC p4 DLL stop?źš?
        0??ʹ??ʱ?Ӽ??⣻
        1????ֹʱ?Ӽ??⡣
 */
        unsigned int emmc_p4_dll_stop : 1; /* [0] */
        // Comment of field: emmc_mmc_dll_srst_req
        /*
        eMMC DRV DLL?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int emmc_mmc_dll_srst_req : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg331;

/* Define the union U_PERI_CRG332 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: emmc_drv_dll_ssel
        /*
        eMMC DRV DLL Slave line????ѡ????
        ????[7:6]??????
        6??h00??DLL slave????1??delay cell
        6??h01??DLL slave????1??delay cell
        6??h02??DLL slave????2??delay cell
        ????
        6??h3F??DLL slave????63??delay cell
 */
        unsigned int emmc_drv_dll_ssel : 8; /* [7..0] */
        // Comment of field: emmc_drv_dll_mode
        /*
        eMMC DRV DLL Slaveģʽѡ???źš?
        0??????ģʽ??
        1????drv_dll_sel????drv slave line??????
 */
        unsigned int emmc_drv_dll_mode : 1; /* [8] */
        // Comment of field: emmc_drv_dll_bypass
        /*
        eMMC DRV DLL Slave ??·ѡ???źš?
        0??????ģʽ??????ʱ??????????ʱ?????࣬??????????????SDIO?????????ؼĴ??????á㣻
        1????·??DRV DLL Slave??
 */
        unsigned int emmc_drv_dll_bypass : 1; /* [9] */
        // Comment of field: emmc_drv_dll_slave_en
        /*
        eMMC DRV DLL Slaveʹ???źš?
        0????ʹ??DLL Slave??
        1??ʹ??DLL Slave??
 */
        unsigned int emmc_drv_dll_slave_en : 1; /* [10] */
        // Comment of field: emmc_drv_dll_tune
        /*
        eMMC SAM DLL ????ʱ????λ???ƣ?
        0x0????У׼??
        0x1??????1??delay??
        0x2??????2??delay??
        0x3??????3??delay??
        ...
        0x7??????7??delay??
        0x8????У׼??
        0x9??????1??delay??
        0xA??????2??delay??
        0xB??????3??delay??
        ...
        0xF??????7??delay??
 */
        unsigned int emmc_drv_dll_tune : 4; /* [14..11] */
        // Comment of field: emmc_drv_clk_phase_sel
        /*
        eMMC DRV ʱ????λ???á?Ĭ??180C
        0x00??0C??
        0x01??11.25C??
        0x02??22.5C??
        ??
        0x1E??337.5C??
        0x1F??348.75C??
 */
        unsigned int emmc_drv_clk_phase_sel : 5; /* [19..15] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 12; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg332;

/* Define the union U_PERI_CRG333 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: emmc_sam_dll_ssel
        /*
        eMMC SAM DLL Slave line????ѡ????
        ????[7:6]??????
        6??h00??DLL slave????1??delay cell
        6??h01??DLL slave????1??delay cell
        6??h02??DLL slave????2??delay cell
        ????
        6??h3F??DLL slave????63??delay cell
 */
        unsigned int emmc_sam_dll_ssel : 8; /* [7..0] */
        // Comment of field: emmc_sam_dll_mode
        /*
        eMMC SAM DLL Slaveģʽѡ???źš?
        0??????ģʽ??
        1????sam_dll_ssel????sam slave line??????
 */
        unsigned int emmc_sam_dll_mode : 1; /* [8] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 1; /* [9] */
        // Comment of field: emmc_sam_dll_slave_en
        /*
        eMMC sam DLL Slaveʹ???źš?
        0????ʹ??DLL Slave??
        1??ʹ??DLL Slave??
 */
        unsigned int emmc_sam_dll_slave_en : 1; /* [10] */
        // Comment of field: emmc_sam_dll_tune
        /*
        eMMC  SAM DLL ????ʱ????λ???ƣ?
        0x0????У׼??
        0x1??????1??delay??
        0x2??????2??delay??
        0x3??????3??delay??
        ...
        0x7??????7??delay??
        0x8????У׼??
        0x9??????1??delay??
        0xA??????2??delay??
        0xB??????3??delay??
        ...
        0xF??????7??delay??
 */
        unsigned int emmc_sam_dll_tune : 4; /* [14..11] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 17; /* [31..15] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg333;

/* Define the union U_PERI_CRG334 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: emmc_ds_dll_ssel
        /*
        eMMC ds DLL Slave line????ѡ????
        ????[7]??????
        7??h00??DLL slave????1??delay cell
        7??h01??DLL slave????1??delay cell
        7??h02??DLL slave????2??delay cell
        ????
        7??h3F??DLL slave????63??delay cell
        ????
 */
        unsigned int emmc_ds_dll_ssel : 8; /* [7..0] */
        // Comment of field: emmc_ds_dll_mode
        /*
        eMMC ds DLL Slaveģʽѡ???źš?
        0??????ģʽ??
        1????ds_dll_ssel????ds slave line??????
 */
        unsigned int emmc_ds_dll_mode : 1; /* [8] */
        // Comment of field: emmc_ds_dll_bypass
        /*
        eMMC ds DLL Slave ??·ѡ???źš?
        0??????ģʽ??????ʱ??????????ʱ?????࣬??????????????SDIO?????????ؼĴ??????á㣻
        1????·??DRV DLL Slave??
 */
        unsigned int emmc_ds_dll_bypass : 1; /* [9] */
        // Comment of field: emmc_ds_dll_slave_en
        /*
        eMMC ds DLL Slaveʹ???źš?
        0????ʹ??DLL Slave??
        1??ʹ??DLL Slave??
 */
        unsigned int emmc_ds_dll_slave_en : 1; /* [10] */
        // Comment of field: emmc_ds_dll_tune
        /*
        eMMC ds DLL ????ʱ????λ???ƣ?
        0x0????У׼??
        0x1??????1??delay??
        0x2??????2??delay??
        0x3??????3??delay??
        ...
        0x7??????7??delay??
        0x8????У׼??
        0x9??????1??delay??
        0xA??????2??delay??
        0xB??????3??delay??
        ...
        0xF??????7??delay??
 */
        unsigned int emmc_ds_dll_tune : 4; /* [14..11] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 17; /* [31..15] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg334;

/* Define the union U_PERI_CRG335 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: emmc_sample_b_cclk_sel
        /*
        ???ؼ???ʱ????λ(??????sampleʱ??)ѡ???źţ?
        0x04: 45oC
        0x08: 90oC(?ϵ?Ĭ??ֵ)
        ????
        0x1C: 315oC
        ע?⣺[1:0] ????Ϊ2??b00??
 */
        unsigned int emmc_sample_b_cclk_sel : 5; /* [4..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 27; /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg335;

/* Define the union U_PERI_CRG336 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: emmc_p4_dll_mdly_tap
        /*
        eMMC P4 DLL mdly_tap?źš?
        DLL MASTER LINE tapֵ??
 */
        unsigned int emmc_p4_dll_mdly_tap : 8; /* [7..0] */
        // Comment of field: emmc_p4_dll_overflow
        /*
        eMMC P4 DLL overflow?źš?
        0??DLL MASTER unoverflow??
        1??DLL MASTER overflow??
 */
        unsigned int emmc_p4_dll_overflow : 1; /* [8] */
        // Comment of field: emmc_p4_dll_locked
        /*
        eMMC P4 DLL lock?źš?
        0??DLL MASTER unlock??
        1??DLL MASTER locked??
 */
        unsigned int emmc_p4_dll_locked : 1; /* [9] */
        // Comment of field: emmc_ds_dll_ready
        /*
        eMMC DS DLL ready?źš?
        0??DLL SLAVE not ready??
        1??DLL SLAVE ready??
 */
        unsigned int emmc_ds_dll_ready : 1; /* [10] */
        // Comment of field: emmc_drv_dll_ready
        /*
        eMMC drv DLL ready?źš?
        0??DLL SLAVE not ready??
        1??DLL SLAVE ready??
 */
        unsigned int emmc_drv_dll_ready : 1; /* [11] */
        // Comment of field: emmc_sam_dll_ready
        /*
        eMMC sam DLL ready?źš?
        0??DLL SLAVE not ready??
        1??DLL SLAVE ready??
 */
        unsigned int emmc_sam_dll_ready : 1; /* [12] */
        // Comment of field: reserve_for_eco_sdio_crg_out
        /*
        ??????
 */
        unsigned int reserve_for_eco_sdio_crg_out : 4; /* [16..13] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 15; /* [31..17] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg336;

/* Define the union U_PERI_CRG340 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ????
 */
        unsigned int reserved_0 : 16;        /* [15..0] */
        unsigned int apll_lock_srst_req : 1; /* [16] */
        unsigned int cpll_lock_srst_req : 1; /* [17] */
        unsigned int dpll_lock_srst_req : 1; /* [18] */
        unsigned int vpll_lock_srst_req : 1; /* [19] */
        unsigned int epll_lock_srst_req : 1; /* [20] */
        unsigned int tpll_lock_srst_req : 1; /* [21] */
        unsigned int mpll_lock_srst_req : 1; /* [22] */
        unsigned int hpll_lock_srst_req : 1; /* [23] */
        // Comment of field: pll_lock_int_clr
        /*
        PLLʧ???ж????㡣
 */
        unsigned int pll_lock_int_clr : 1; /* [24] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 7; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg340;

/* Define the union U_PERI_CRG341 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apll_lock_cnt : 4; /* [3..0] */
        unsigned int cpll_lock_cnt : 4; /* [7..4] */
        unsigned int dpll_lock_cnt : 4; /* [11..8] */
        unsigned int vpll_lock_cnt : 4; /* [15..12] */
        unsigned int epll_lock_cnt : 4; /* [19..16] */
        unsigned int tpll_lock_cnt : 4; /* [23..20] */
        unsigned int mpll_lock_cnt : 4; /* [27..24] */
        unsigned int reserved_0 : 4;    /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg341;

/* Define the union U_PERI_CRG342 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apll_overflow_alarm_cnt : 8; /* [7..0] */
        unsigned int cpll_overflow_alarm_cnt : 8; /* [15..8] */
        unsigned int dpll_overflow_alarm_cnt : 8; /* [23..16] */
        unsigned int reserved_0 : 8;              /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg342;

/* Define the union U_PERI_CRG343 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int epll_overflow_alarm_cnt : 8; /* [7..0] */
        unsigned int tpll_overflow_alarm_cnt : 8; /* [15..8] */
        unsigned int mpll_overflow_alarm_cnt : 8; /* [23..16] */
        unsigned int reseved : 8;                 /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg343;

/* Define the union U_PERI_CRG344 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hpll_overflow_alarm_cnt : 8; /* [7..0] */
        unsigned int vpll_overflow_alarm_cnt : 8; /* [15..8] */
        unsigned int hpll_lock_cnt : 4;           /* [19..16] */
        unsigned int reserved_0 : 12;             /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_crg344;

/* Define the union U_PERI_CRG_PLL300 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: fpll_ctrl0
        /*
        pll_postdiv2 [30:28] 
        PLL?ڶ?????????Ƶϵ????
        
        pll_postdiv1 [26:24]
        PLL??һ????????Ƶϵ????
        
        pll_frac [23:0]:
        PLLС????Ƶϵ????
 */
        unsigned int fpll_ctrl0 : 31; /* [30..0] */
        // Comment of field: reserved_0
        /*
        ????
 */
        unsigned int reserved_0 : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg_pll300;

/* Define the union U_PERI_CRG_PLL301 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: fpll_ctrl1
        /*
        bit[27] ???????á?
        
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
        unsigned int fpll_ctrl1 : 28; /* [27..0] */
        // Comment of field: reserved_0
        /*
        ????
 */
        unsigned int reserved_0 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg_pll301;

/* Define the union U_PERI_CRG302 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: fpll_ssmod_ctrl
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
        unsigned int fpll_ssmod_ctrl : 13; /* [12..0] */
        // Comment of field: reserved_0
        /*
        ????
 */
        unsigned int reserved_0 : 19; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg302;

/* Define the union U_PERI_CRG303 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: fpll_lock
        /*
        FPLL LOCKָʾ?ź?
        0??PLLδ????
        1??PLL??????
 */
        unsigned int fpll_lock : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ????
 */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg303;

/* Define the union U_PERI_CRG320 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: m8051_cken
        /*
        m8051ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int m8051_cken : 1; /* [0] */
        // Comment of field: pclk_edid_cken
        /*
        EDID????ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pclk_edid_cken : 1; /* [1] */
        // Comment of field: clk_edid_osc_cken
        /*
        EDID????ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int clk_edid_osc_cken : 1; /* [2] */
        // Comment of field: lsadc_cken
        /*
        LSADCʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int lsadc_cken : 1; /* [3] */
        // Comment of field: pwm_aon_cken
        /*
        PWMʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int pwm_aon_cken : 1; /* [4] */
        // Comment of field: timer01_aon_cken
        /*
        timer01ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int timer01_aon_cken : 1; /* [5] */
        // Comment of field: wdg_aon_cken
        /*
        ??????WDGʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int wdg_aon_cken : 1; /* [6] */
        // Comment of field: uart1_cken
        /*
        UART1ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int uart1_cken : 1; /* [7] */
        // Comment of field: uart3_cken
        /*
        UART3ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int uart3_cken : 1; /* [8] */
        // Comment of field: uart4_cken
        /*
        UART4ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int uart4_cken : 1; /* [9] */
        // Comment of field: ir_cken
        /*
        IRʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int ir_cken : 1;    /* [10] */
        unsigned int reserved_0 : 5; /* [15..11] */
        // Comment of field: m8051_srst_req
        /*
        m8051????ʱ???���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int m8051_srst_req : 1; /* [16] */
        // Comment of field: edid_psrst_req
        /*
        EDID????ʱ???���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int edid_psrst_req : 1; /* [17] */
        // Comment of field: edid_srst_req
        /*
        EDID????ʱ???���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int edid_srst_req : 1; /* [18] */
        // Comment of field: lsadc_srst_req
        /*
        LSADC?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int lsadc_srst_req : 1; /* [19] */
        // Comment of field: pwm_aon_srst_req
        /*
        PWM?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int pwm_aon_srst_req : 1; /* [20] */
        // Comment of field: timer01_aon_srst_req
        /*
        timer01?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int timer01_aon_srst_req : 1; /* [21] */
        // Comment of field: wdg_aon_srst_req
        /*
        ??????WDG?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int wdg_aon_srst_req : 1; /* [22] */
        // Comment of field: uart1_srst_req
        /*
        UART1?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int uart1_srst_req : 1; /* [23] */
        // Comment of field: uart3_srst_req
        /*
        UART3?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int uart3_srst_req : 1; /* [24] */
        // Comment of field: uart4_srst_req
        /*
        UART4?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int uart4_srst_req : 1; /* [25] */
        // Comment of field: ir_srst_req
        /*
        IR?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int ir_srst_req : 1; /* [26] */
        // Comment of field: uart4_cksel
        /*
        UART4ʱ??ѡ????
        0??100M??
        1??3M??
 */
        unsigned int uart4_cksel : 1; /* [27] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg320;

/* Define the union U_PERI_CRG321 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: gpio_pwm18_aon_cken
        /*
        GPIO_PWM18ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpio_pwm18_aon_cken : 1; /* [0] */
        // Comment of field: gpio_pwm19_aon_cken
        /*
        GPIO_PWM19ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpio_pwm19_aon_cken : 1; /* [1] */
        // Comment of field: gpio_pwm20_aon_cken
        /*
        GPIO_PWM20ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpio_pwm20_aon_cken : 1; /* [2] */
        // Comment of field: gpio_pwm21_aon_cken
        /*
        GPIO_PWM21ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpio_pwm21_aon_cken : 1; /* [3] */
        // Comment of field: gpio_pwm22_aon_cken
        /*
        GPIO_PWM22ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int gpio_pwm22_aon_cken : 1; /* [4] */
        unsigned int reserved_0 : 11;         /* [15..5] */
        // Comment of field: gpio_pwm18_aon_srst_req
        /*
        GPIO_PWM18?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int gpio_pwm18_aon_srst_req : 1; /* [16] */
        // Comment of field: gpio_pwm19_aon_srst_req
        /*
        GPIO_PWM19?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int gpio_pwm19_aon_srst_req : 1; /* [17] */
        // Comment of field: gpio_pwm20_aon_srst_req
        /*
        GPIO_PWM20?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int gpio_pwm20_aon_srst_req : 1; /* [18] */
        // Comment of field: gpio_pwm21_aon_srst_req
        /*
        GPIO_PWM21?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int gpio_pwm21_aon_srst_req : 1; /* [19] */
        // Comment of field: gpio_pwm22_aon_srst_req
        /*
        GPIO_PWM22?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int gpio_pwm22_aon_srst_req : 1; /* [20] */
        unsigned int reserved_1 : 3;              /* [23..21] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg321;

/* Define the union U_PERI_CRG322 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hdmirx_2p0_cec_cksel
        /*
        cec ʱ??ѡ????
        0??2M??
        1??2.02M??
        2.02MƵ?㱣?????á?
 */
        unsigned int hdmirx_2p0_cec_cksel : 1; /* [0] */
        // Comment of field: hdmirx_2p0_cbus_cksel
        /*
        cbusʱ??ѡ????
        0??24M??
        1??20M??
 */
        unsigned int hdmirx_2p0_cbus_cksel : 1; /* [1] */
        // Comment of field: hdmirx_2p0_edid_cksel
        /*
        edid ʱ??ѡ????
        0??2M??
        1??2.02M??
        2.02MƵ?㱣?????á?
 */
        unsigned int hdmirx_2p0_edid_cksel : 1; /* [2] */
        // Comment of field: hdmirx_2p0_sheman_cksel
        /*
        shemanʱ??ѡ????
        0??24M??
        1??27M??
 */
        unsigned int hdmirx_2p0_sheman_cksel : 1; /* [3] */
        // Comment of field: hdmirx_2p0_aphy_clk_p0_cksel
        /*
        aphy_p0ʱ??ѡ????
        0??ѡ??????ʱ??
        1??ѡ??hdmi_phy_p0??????ʱ??(AD_CLKO_DVI_DVI32_P0)
 */
        unsigned int hdmirx_2p0_aphy_clk_p0_cksel : 1; /* [4] */
        // Comment of field: hdmirx_2p0_aphy_clk_p1_cksel
        /*
        aphy_p1ʱ??ѡ????
        0??ѡ?????? ʱ??
        1??ѡ??hdmi_phy_p1??????ʱ??(AD_CLKO_DVI_DVI32_P1)
 */
        unsigned int hdmirx_2p0_aphy_clk_p1_cksel : 1; /* [5] */
        // Comment of field: hdmirx_2p0_aphy_clk_p2_cksel
        /*
        aphy_p2ʱ??ѡ????
        0??ѡ??????ʱ??
        1??ѡ??hdmi_phy_p2??????ʱ??(AD_CLKO_DVI_DVI32_P2)
 */
        unsigned int hdmirx_2p0_aphy_clk_p2_cksel : 1; /* [6] */
        // Comment of field: hdmirx_2p0_aphy_clk_p3_cksel
        /*
        aphy_p3ʱ??ѡ????
        0??ѡ??????ʱ??;
        1??ѡ??hdmi_phy_p3??????ʱ??(AD_CLKO_DVI_DVI32_P3)
 */
        unsigned int hdmirx_2p0_aphy_clk_p3_cksel : 1; /* [7] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 4; /* [11..8] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 20; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg322;

/* Define the union U_PERI_CRG323 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: hdmirx_2p0_cbus_cken
        /*
        cbusʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_cbus_cken : 1; /* [0] */
        // Comment of field: hdmirx_2p0_cec_cken
        /*
        cecʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_cec_cken : 1; /* [1] */
        // Comment of field: hdmirx_2p0_edid_cken
        /*
        edidʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_edid_cken : 1; /* [2] */
        // Comment of field: hdmirx_2p0_osc_cken
        /*
        oscʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_osc_cken : 1; /* [3] */
        // Comment of field: hdmirx_2p0_prif_cken
        /*
        prifʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_prif_cken : 1; /* [4] */
        // Comment of field: hdmirx_2p0_sheman_cken
        /*
        shemanʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_sheman_cken : 1; /* [5] */
        // Comment of field: hdmirx_2p0_aphy_clk_p0_cken
        /*
        aphy_p0ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_aphy_clk_p0_cken : 1; /* [6] */
        // Comment of field: hdmirx_2p0_aphy_clk_p1_cken
        /*
        aphy_p1ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_aphy_clk_p1_cken : 1; /* [7] */
        // Comment of field: hdmirx_2p0_aphy_clk_p2_cken
        /*
        aphy_p2ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_aphy_clk_p2_cken : 1; /* [8] */
        // Comment of field: hdmirx_2p0_aphy_clk_p3_cken
        /*
        aphy_p3ʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int hdmirx_2p0_aphy_clk_p3_cken : 1; /* [9] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 6; /* [15..10] */
        // Comment of field: hdmirx_2p0_cec_srst_req
        /*
        cec?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_cec_srst_req : 1; /* [16] */
        // Comment of field: hdmirx_2p0_osc_srst_req
        /*
        osc?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_osc_srst_req : 1; /* [17] */
        // Comment of field: hdmirx_2p0_prif_srst_req
        /*
        prif?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_prif_srst_req : 1; /* [18] */
        // Comment of field: hdmirx_2p0_sheman_srst_req
        /*
        sheman?���λ??????
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_sheman_srst_req : 1; /* [19] */
        // Comment of field: hdmirx_2p0_aphy_clk_p0_srst_req
        /*
        aphy_p0?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_aphy_clk_p0_srst_req : 1; /* [20] */
        // Comment of field: hdmirx_2p0_aphy_clk_p1_srst_req
        /*
        aphy_p1?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_aphy_clk_p1_srst_req : 1; /* [21] */
        // Comment of field: hdmirx_2p0_aphy_clk_p2_srst_req
        /*
        aphy_p2?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_aphy_clk_p2_srst_req : 1; /* [22] */
        // Comment of field: hdmirx_2p0_aphy_clk_p3_srst_req
        /*
        aphy_p3?���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int hdmirx_2p0_aphy_clk_p3_srst_req : 1; /* [23] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 4; /* [27..24] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg323;

/* Define the union U_PERI_CRG324 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: fephy_cken
        /*
        fephyʱ???ź??ſأ?
        0???ضϣ?
        1???򿪡?
 */
        unsigned int fephy_cken : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 15; /* [15..1] */
        // Comment of field: fephy_srst_req
        /*
        fephy???���λ?????źţ?
        0??????λ??
        1????λ??
 */
        unsigned int fephy_srst_req : 1; /* [16] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 7; /* [23..17] */
        // Comment of field: fephy_clk_sel
        /*
        clk_fephyʱ??��Դѡ??
        0???µ???25Mʱ??
        1: ??????25Mʱ??
 */
        unsigned int fephy_clk_sel : 1; /* [24] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 3; /* [27..25] */
        // Comment of field: reserved_3
        /*
        ??????
 */
        unsigned int reserved_3 : 4; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg324;

/* Define the union U_PERI_CRG325 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 16; /* [15..0] */
        // Comment of field: equator_apb_srst_req
        /*
        Equator_digital???���λ?????ź?
        0??????λ??
        1????λ??
 */
        unsigned int equator_apb_srst_req : 1; /* [16] */
        // Comment of field: reserved_1
        /*
        ??????
 */
        unsigned int reserved_1 : 7; /* [23..17] */
        // Comment of field: equator_cvbs_dff_sel
        /*
        equator_digital??clk_cvbs_dff??ʱ??ѡ??
        0: ѡ??ģ??IP??????clk_cvbsʱ??
        1??ѡ??clk_150m_occʱ??
 */
        unsigned int equator_cvbs_dff_sel : 1; /* [24] */
        // Comment of field: equator_cvbs_out_sel
        /*
        equator_digital??clk_cvbs_out??ʱ??ѡ??
        0: ѡ??ģ??IP??????clk_cvbs????2??Ƶ????ʱ??
        1??ѡ??clk_75m_occʱ??
 */
        unsigned int equator_cvbs_out_sel : 1; /* [25] */
        // Comment of field: equator_rgb_mux_sel
        /*
        Equator RGBʱ??ѡ???źţ?
        0: ģ??IP??????clk_rgbʱ??
        1??clk_150m_occʱ??
 */
        unsigned int equator_rgb_mux_sel : 1; /* [26] */
        // Comment of field: reserved_2
        /*
        ??????
 */
        unsigned int reserved_2 : 5; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg325;

/* Define the union U_PERI_CRG326 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mcu_bus_clk_sel
        /*
        MCU????????ʱ??ѡ??
        00??24M
        01??100M
        10??100K
        11??????
 */
        unsigned int mcu_bus_clk_sel : 2; /* [1..0] */
        // Comment of field: mcu_bus_clk_div
        /*
        MCU????????ʱ?ӷ?Ƶѡ??
        00??mcu_bus_clk 1??Ƶ??
        01??mcu_bus_clk 2??Ƶ??
        10??mcu_bus_clk 3??Ƶ??
        11??mcu_bus_clk 4??Ƶ??
 */
        unsigned int mcu_bus_clk_div : 2; /* [3..2] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg326;

/* Define the union U_PERI_CRG327 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mcu_bus_clk_seled
        /*
        MCU????????ʱ??״̬
        00??24M
        01??100M
        10??100K
        11??????
 */
        unsigned int mcu_bus_clk_seled : 2; /* [1..0] */
        // Comment of field: reserved_0
        /*
        ??????
 */
        unsigned int reserved_0 : 30; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_peri_stb_crg327;

/* ============================================================================== */
/* Define the global struct */
typedef struct {
    volatile u_peri_crg_pll0 peri_crg_pll0;   /* 0x0 */
    volatile u_peri_crg_pll1 peri_crg_pll1;   /* 0x4 */
    volatile u_peri_crg_pll2 peri_crg_pll2;   /* 0x8 */
    volatile u_peri_crg_pll3 peri_crg_pll3;   /* 0xc */
    volatile u_peri_crg_pll4 peri_crg_pll4;   /* 0x10 */
    volatile u_peri_crg_pll5 peri_crg_pll5;   /* 0x14 */
    volatile u_peri_crg_pll6 peri_crg_pll6;   /* 0x18 */
    volatile u_peri_crg_pll7 peri_crg_pll7;   /* 0x1c */
    volatile u_peri_crg_pll8 peri_crg_pll8;   /* 0x20 */
    volatile u_peri_crg_pll9 peri_crg_pll9;   /* 0x24 */
    volatile u_peri_crg_pll10 peri_crg_pll10; /* 0x28 */
    volatile u_peri_crg_pll11 peri_crg_pll11; /* 0x2c */
    volatile u_peri_crg_pll12 peri_crg_pll12; /* 0x30 */
    volatile u_peri_crg_pll13 peri_crg_pll13; /* 0x34 */
    volatile u_peri_crg_pll14 peri_crg_pll14; /* 0x38 */
    volatile u_peri_crg_pll15 peri_crg_pll15; /* 0x3c */
    volatile unsigned int reserved_0[2];      /* 2 is reserved reg size,0x40~0x44 */
    volatile u_peri_crg18 peri_crg18;         /* 0x48 */
    volatile u_peri_crg19 peri_crg19;         /* 0x4c */
    volatile unsigned int reserved_1[3];      /* 3 is reserved reg size,0x50~0x58 */
    volatile u_peri_crg23 peri_crg23;         /* 0x5c */
    volatile unsigned int reserved_2[2];      /* 2 is reserved reg size,0x60~0x64 */
    volatile u_peri_crg26 peri_crg26;         /* 0x68 */
    volatile u_peri_crg27 peri_crg27;         /* 0x6c */
    volatile u_peri_crg28 peri_crg28;         /* 0x70 */
    volatile u_peri_crg29 peri_crg29;         /* 0x74 */
    volatile u_peri_crg30 peri_crg30;         /* 0x78 */
    volatile u_peri_crg31 peri_crg31;         /* 0x7c */
    volatile unsigned int reserved_3;         /* 0x80 */
    volatile u_peri_crg33 peri_crg33;         /* 0x84 */
    volatile u_peri_crg34 peri_crg34;         /* 0x88 */
    volatile u_peri_crg35 peri_crg35;         /* 0x8c */
    volatile unsigned int reserved_4;         /* 0x90 */
    volatile u_peri_crg37 peri_crg37;         /* 0x94 */
    volatile u_peri_crg38 peri_crg38;         /* 0x98 */
    volatile unsigned int reserved_5[9];      /* 9 is reserved reg size,0x9c~0xbc */
    volatile u_peri_crg48 peri_crg48;         /* 0xc0 */
    volatile u_peri_crg49 peri_crg49;         /* 0xc4 */
    volatile u_peri_crg50 peri_crg50;         /* 0xc8 */
    volatile unsigned int reserved_6[2];      /* 2 is reserved reg size,0xcc~0xd0 */
    volatile u_peri_crg53 peri_crg53;         /* 0xd4 */
    volatile u_peri_crg54 peri_crg54;         /* 0xd8 */
    volatile u_peri_crg55 peri_crg55;         /* 0xdc */
    volatile u_peri_crg56 peri_crg56;         /* 0xe0 */
    volatile unsigned int reserved_7;         /* 0xe4 */
    volatile u_peri_crg58 peri_crg58;         /* 0xe8 */
    volatile unsigned int reserved_8[3];      /* 3 is reserved reg size,0xec~0xf4 */
    volatile u_peri_crg62 peri_crg62;         /* 0xf8 */
    volatile u_peri_crg63 peri_crg63;         /* 0xfc */
    volatile u_peri_crg64 peri_crg64;         /* 0x100 */
    volatile unsigned int reserved_9[5];      /* 5 is reserved reg size,0x104~0x114 */
    volatile u_peri_crg70 peri_crg70;         /* 0x118 */
    volatile u_peri_crg71 peri_crg71;         /* 0x11c */
    volatile unsigned int reserved_10;        /* 0x120 */
    volatile u_peri_crg73 peri_crg73;         /* 0x124 */
    volatile u_peri_crg74 peri_crg74;         /* 0x128 */
    volatile u_peri_crg75 peri_crg75;         /* 0x12c */
    volatile u_peri_crg76 peri_crg76;         /* 0x130 */
    volatile unsigned int reserved_11[2];     /* 2 is reserved reg size,0x134~0x138 */
    volatile u_peri_crg79 peri_crg79;         /* 0x13c */
    volatile u_peri_crg80 peri_crg80;         /* 0x140 */
    volatile u_peri_crg81 peri_crg81;         /* 0x144 */
    volatile u_peri_crg82 peri_crg82;         /* 0x148 */
    volatile unsigned int reserved_12;        /* 0x14c */
    volatile u_peri_crg84 peri_crg84;         /* 0x150 */
    volatile u_peri_crg85 peri_crg85;         /* 0x154 */
    volatile u_peri_crg86 peri_crg86;         /* 0x158 */
    volatile u_peri_crg87 peri_crg87;         /* 0x15c */
    volatile u_peri_crg88 peri_crg88;         /* 0x160 */
    volatile u_peri_crg89 peri_crg89;         /* 0x164 */
    volatile unsigned int reserved_13;        /* 0x168 */
    volatile u_peri_crg91 peri_crg91;         /* 0x16c */
    volatile unsigned int reserved_14;        /* 0x170 */
    volatile u_peri_crg93 peri_crg93;         /* 0x174 */
    volatile u_peri_crg94 peri_crg94;         /* 0x178 */
    volatile u_peri_crg95 peri_crg95;         /* 0x17c */
    volatile u_peri_crg96 peri_crg96;         /* 0x180 */
    volatile u_peri_crg97 peri_crg97;         /* 0x184 */
    volatile u_peri_crg98 peri_crg98;         /* 0x188 */
    volatile unsigned int reserved_15;        /* 0x18c */
    volatile u_peri_crg100 peri_crg100;       /* 0x190 */
    volatile u_peri_crg101 peri_crg101;       /* 0x194 */
    volatile u_peri_crg102 peri_crg102;       /* 0x198 */
    volatile u_peri_crg103 peri_crg103;       /* 0x19c */
    volatile unsigned int reserved_16[2];     /* 2 is reserved reg size,0x1a0~0x1a4 */
    volatile u_peri_crg106 peri_crg106;       /* 0x1a8 */
    volatile unsigned int reserved_17;        /* 0x1ac */
    volatile u_peri_crg108 peri_crg108;       /* 0x1b0 */
    volatile unsigned int reserved_18;        /* 0x1b4 */
    volatile u_peri_crg110 peri_crg110;       /* 0x1b8 */
    volatile unsigned int reserved_19[4];     /* 4 is reserved reg size,0x1bc~0x1c8 */
    volatile u_peri_crg115 peri_crg115;       /* 0x1cc */
    volatile unsigned int reserved_20[4];     /* 4 is reserved reg size,0x1d0~0x1dc */
    volatile u_peri_crg120 peri_crg120;       /* 0x1e0 */
    volatile unsigned int reserved_21;        /* 0x1e4 */
    volatile u_peri_crg122 peri_crg122;       /* 0x1e8 */
    volatile u_peri_crg123 peri_crg123;       /* 0x1ec */
    volatile u_peri_crg124 peri_crg124;       /* 0x1f0 */
    volatile unsigned int reserved_22[2];     /* 2 is reserved reg size,0x1f4~0x1f8 */
    volatile u_peri_crg127 peri_crg127;       /* 0x1fc */
    volatile u_peri_crg128 peri_crg128;       /* 0x200 */
    volatile unsigned int reserved_23;        /* 0x204 */
    volatile u_peri_crg130 peri_crg130;       /* 0x208 */
    volatile u_peri_crg131 peri_crg131;       /* 0x20c */
    volatile u_peri_crg132 peri_crg132;       /* 0x210 */
    volatile u_peri_crg133 peri_crg133;       /* 0x214 */
    volatile u_peri_crg134 peri_crg134;       /* 0x218 */
    volatile u_peri_crg135 peri_crg135;       /* 0x21c */
    volatile u_peri_crg136 peri_crg136;       /* 0x220 */
    volatile u_peri_crg137 peri_crg137;       /* 0x224 */
    volatile u_peri_crg138 peri_crg138;       /* 0x228 */
    volatile u_peri_crg139 peri_crg139;       /* 0x22c */
    volatile u_peri_crg140 peri_crg140;       /* 0x230 */
    volatile u_peri_crg141 peri_crg141;       /* 0x234 */
    volatile u_peri_crg142 peri_crg142;       /* 0x238 */
    volatile u_peri_crg143 peri_crg143;       /* 0x23c */
    volatile u_peri_crg144 peri_crg144;       /* 0x240 */
    volatile u_peri_crg145 peri_crg145;       /* 0x244 */
    volatile u_peri_crg146 peri_crg146;       /* 0x248 */
    volatile u_peri_crg147 peri_crg147;       /* 0x24c */
    volatile u_peri_crg148 peri_crg148;       /* 0x250 */
    volatile u_peri_crg149 peri_crg149;       /* 0x254 */
    volatile u_peri_crg150 peri_crg150;       /* 0x258 */
    volatile u_peri_crg151 peri_crg151;       /* 0x25c */
    volatile unsigned int reserved_24;        /* 0x260 */
    volatile u_peri_crg153 peri_crg153;       /* 0x264 */
    volatile unsigned int reserved_25[2];     /* 2 is reserved reg size,0x268~0x26c */
    volatile u_peri_crg156 peri_crg156;       /* 0x270 */
    volatile u_peri_crg157 peri_crg157;       /* 0x274 */
    volatile u_peri_crg158 peri_crg158;       /* 0x278 */
    volatile u_peri_crg159 peri_crg159;       /* 0x27c */
    volatile u_peri_crg160 peri_crg160;       /* 0x280 */
    volatile unsigned int reserved_26[2];     /* 2 is reserved reg size,0x284~0x288 */
    volatile u_peri_crg163 peri_crg163;       /* 0x28c */
    volatile unsigned int reserved_27;        /* 0x290 */
    volatile u_peri_crg165 peri_crg165;       /* 0x294 */
    volatile u_peri_crg166 peri_crg166;       /* 0x298 */
    volatile unsigned int reserved_28[8];     /* 8 is reserved reg size,0x29c~0x2b8 */
    volatile u_peri_crg175 peri_crg175;       /* 0x2bc */
    volatile u_peri_crg176 peri_crg176;       /* 0x2c0 */
    volatile unsigned int reserved_29[7];     /* 7 is reserved reg size,0x2c4~0x2dc */
    volatile u_peri_crg184 peri_crg184;       /* 0x2e0 */
    volatile u_peri_crg185 peri_crg185;       /* 0x2e4 */
    volatile u_peri_crg186 peri_crg186;       /* 0x2e8 */
    volatile u_peri_crg187 peri_crg187;       /* 0x2ec */
    volatile u_peri_crg188 peri_crg188;       /* 0x2f0 */
    volatile unsigned int reserved_30[2];     /* 2 is reserved reg size,0x2f4~0x2f8 */
    volatile u_peri_crg191 peri_crg191;       /* 0x2fc */
    volatile u_peri_crg192 peri_crg192;       /* 0x300 */
    volatile u_peri_crg193 peri_crg193;       /* 0x304 */
    volatile unsigned int reserved_31;        /* 0x308 */
    volatile u_peri_crg195 peri_crg195;       /* 0x30c */
    volatile u_peri_crg196 peri_crg196;       /* 0x310 */
    volatile u_peri_crg197 peri_crg197;       /* 0x314 */
    volatile unsigned int reserved_32[2];     /* 2 is reserved reg size,0x318~0x31c */
    volatile u_peri_crg200 peri_crg200;       /* 0x320 */
    volatile u_peri_crg201 peri_crg201;       /* 0x324 */
    volatile unsigned int reserved_33[54];    /* 54 is reserved reg size,0x328~0x3fc */
    volatile u_peri_crg256 peri_crg256;       /* 0x400 */
    volatile u_peri_crg257 peri_crg257;       /* 0x404 */
    volatile u_peri_crg258 peri_crg258;       /* 0x408 */
    volatile u_peri_crg259 peri_crg259;       /* 0x40c */
    volatile u_peri_crg260 peri_crg260;       /* 0x410 */
    volatile u_peri_crg261 peri_crg261;       /* 0x414 */
    volatile u_peri_crg262 peri_crg262;       /* 0x418 */
    volatile u_peri_crg263 peri_crg263;       /* 0x41c */
    volatile u_peri_crg264 peri_crg264;       /* 0x420 */
    volatile u_peri_crg265 peri_crg265;       /* 0x424 */
    volatile u_peri_crg266 peri_crg266;       /* 0x428 */
    volatile u_peri_crg267 peri_crg267;       /* 0x42c */
    volatile unsigned int reserved_34[2];     /* 2 is reserved reg size,0x430~0x434 */
    volatile u_peri_crg270 peri_crg270;       /* 0x438 */
    volatile u_peri_crg271 peri_crg271;       /* 0x43c */
    volatile unsigned int reserved_35[18];    /* 18 is reserved reg size,0x440~0x484 */
    volatile u_peri_crg290 peri_crg290;       /* 0x488 */
    volatile u_peri_crg291 peri_crg291;       /* 0x48c */
    volatile u_peri_crg292 peri_crg292;       /* 0x490 */
    volatile u_peri_crg293 peri_crg293;       /* 0x494 */
    volatile u_peri_crg294 peri_crg294;       /* 0x498 */
    volatile unsigned int reserved_36[5];     /* 5 is reserved reg size,0x49c~0x4ac */
    volatile u_peri_crg300 peri_crg300;       /* 0x4b0 */
    volatile u_peri_crg301 peri_crg301;       /* 0x4b4 */
    volatile unsigned int reserved_37[8];     /* 8 is reserved reg size,0x4b8~0x4d4 */
    volatile u_peri_crg310 peri_crg310;       /* 0x4d8 */
    volatile u_peri_crg311 peri_crg311;       /* 0x4dc */
    volatile unsigned int reserved_38[8];     /* 8 is reserved reg size,0x4e0~0x4fc */
    volatile u_peri_crg320 peri_crg320;       /* 0x500 */
    volatile u_peri_crg321 peri_crg321;       /* 0x504 */
    volatile u_peri_crg322 peri_crg322;       /* 0x508 */
    volatile u_peri_crg323 peri_crg323;       /* 0x50c */
    volatile u_peri_crg324 peri_crg324;       /* 0x510 */
    volatile unsigned int reserved_39[5];     /* 5 is reserved reg size,0x514~0x524 */
    volatile u_peri_crg330 peri_crg330;       /* 0x528 */
    volatile u_peri_crg331 peri_crg331;       /* 0x52c */
    volatile u_peri_crg332 peri_crg332;       /* 0x530 */
    volatile u_peri_crg333 peri_crg333;       /* 0x534 */
    volatile u_peri_crg334 peri_crg334;       /* 0x538 */
    volatile u_peri_crg335 peri_crg335;       /* 0x53c */
    volatile u_peri_crg336 peri_crg336;       /* 0x540 */
    volatile unsigned int reserved_40[3];     /* 3 is reserved reg size,0x544~0x54c */
    volatile u_peri_crg340 peri_crg340;       /* 0x550 */
    volatile u_peri_crg341 peri_crg341;       /* 0x554 */
    volatile u_peri_crg342 peri_crg342;       /* 0x558 */
    volatile u_peri_crg343 peri_crg343;       /* 0x55c */
    volatile u_peri_crg344 peri_crg344;       /* 0x560 */
} s_crg_regs_type;

/* Define the global struct */
typedef struct {
    volatile unsigned int reserved_0[300];              /* 300 is reserved reg size,0x0~0x4ac */
    volatile u_peri_stb_crg_pll300 peri_stb_crg_pll300; /* 0x4b0 */
    volatile u_peri_stb_crg_pll301 peri_stb_crg_pll301; /* 0x4b4 */
    volatile u_peri_stb_crg302 peri_stb_crg302;         /* 0x4b8 */
    volatile u_peri_stb_crg303 peri_stb_crg303;         /* 0x4bc */
    volatile unsigned int reserved_1[16];               /* 16 is reserved reg size,0x4c0~0x4fc */
    volatile u_peri_stb_crg320 peri_stb_crg320;         /* 0x500 */
    volatile u_peri_stb_crg321 peri_stb_crg321;         /* 0x504 */
    volatile u_peri_stb_crg322 peri_stb_crg322;         /* 0x508 */
    volatile u_peri_stb_crg323 peri_stb_crg323;         /* 0x50c */
    volatile u_peri_stb_crg324 peri_stb_crg324;         /* 0x510 */
    volatile u_peri_stb_crg325 peri_stb_crg325;         /* 0x514 */
    volatile u_peri_stb_crg326 peri_stb_crg326;         /* 0x518 */
    volatile u_peri_stb_crg327 peri_stb_crg327;         /* 0x51c */
} s_stb_crg_regs_type;

#endif /* __HI_REG_CRG_H__d */
