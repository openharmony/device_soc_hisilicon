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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART44_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART44_H_

    volatile unsigned int cbm2_lay3_debug;                                          /* 0x3dc */
    volatile unsigned int cbm2_lay4_debug;                                          /* 0x3e0 */
    volatile unsigned int cbm2_lay0_last_debug;                                     /* 0x3e4 */
    volatile unsigned int cbm2_lay1_last_debug;                                     /* 0x3e8 */
    volatile unsigned int cbm2_lay2_last_debug;                                     /* 0x3ec */
    volatile unsigned int cbm2_lay3_last_debug;                                     /* 0x3f0 */
    volatile unsigned int cbm2_lay4_last_debug;                                     /* 0x3f4 */
    volatile unsigned int reserved_12[2];                                           /* 0x3f8~0x3fc 2 regs */
    volatile reg_cbm_bkg3 cbm_bkg3;                                                 /* 0x400 */
    volatile unsigned int reserved_13;                                              /* 0x404 */
    volatile reg_cbm_mix3 cbm_mix3;                                                 /* 0x408 */
    volatile unsigned int reserved_14[17];                                          /* 0x40c~0x44c 17 regs */
    volatile unsigned int cbm3_lay0_debug;                                          /* 0x450 */
    volatile unsigned int cbm3_lay1_debug;                                          /* 0x454 */
    volatile unsigned int cbm3_lay2_debug;                                          /* 0x458 */
    volatile unsigned int cbm3_lay3_debug;                                          /* 0x45c */
    volatile unsigned int cbm3_lay4_debug;                                          /* 0x460 */
    volatile unsigned int cbm3_lay0_last_debug;                                     /* 0x464 */
    volatile unsigned int cbm3_lay1_last_debug;                                     /* 0x468 */
    volatile unsigned int cbm3_lay2_last_debug;                                     /* 0x46c */
    volatile unsigned int cbm3_lay3_last_debug;                                     /* 0x470 */
    volatile unsigned int cbm3_lay4_last_debug;                                     /* 0x474 */
    volatile unsigned int reserved_15[98];                                          /* 0x478~0x5fc 98 regs */
    volatile reg_mixv0_bkg mixv0_bkg;                                               /* 0x600 */
    volatile unsigned int reserved_16;                                              /* 0x604 */
    volatile reg_mixv0_mix mixv0_mix;                                               /* 0x608 */
    volatile unsigned int reserved_17[189];                                         /* 0x60c~0x8fc 189 regs */
    volatile reg_mixg0_bkg mixg0_bkg;                                               /* 0x900 */
    volatile reg_mixg0_bkalpha mixg0_bkalpha;                                       /* 0x904 */
    volatile reg_mixg0_mix mixg0_mix;                                               /* 0x908 */
    volatile unsigned int reserved_18[189];                                         /* 0x90c~0xbfc 189 regs */
    volatile reg_link_ctrl link_ctrl;                                               /* 0xc00 */
    volatile unsigned int reserved_19[63];                                          /* 0xc04~0xcfc 63 regs */
    volatile reg_vpss_ctrl vpss_ctrl;                                               /* 0xd00 */
    volatile reg_vpss_miscellaneous vpss_miscellaneous;                             /* 0xd04 */
    volatile reg_vpss_ftconfig vpss_ftconfig;                                       /* 0xd08 */
    volatile unsigned int reserved_20[5];                                           /* 0xd0c~0xd1c 5 regs */
    volatile unsigned int vpss_version;                                             /* 0xd20 */
    volatile unsigned int vpss_debug0;                                              /* 0xd24 */
    volatile unsigned int vpss_debug1;                                              /* 0xd28 */
    volatile unsigned int vpss_debug2;                                              /* 0xd2c */
    volatile unsigned int vpss_debug3;                                              /* 0xd30 */
    volatile unsigned int vpss_debug4;                                              /* 0xd34 */
    volatile unsigned int vpss_debug5;                                              /* 0xd38 */
    volatile unsigned int vpss_debug6;                                              /* 0xd3c */
    volatile unsigned int reserved_21[48];                                          /* 0xd40~0xdfc 48 regs */
    volatile unsigned int para_haddr_vhd_chn00;                                     /* 0xe00 */
    volatile unsigned int para_addr_vhd_chn00;                                      /* 0xe04 */
    volatile unsigned int para_haddr_vhd_chn01;                                     /* 0xe08 */
    volatile unsigned int para_addr_vhd_chn01;                                      /* 0xe0c */
    volatile unsigned int para_haddr_vhd_chn02;                                     /* 0xe10 */
    volatile unsigned int para_addr_vhd_chn02;                                      /* 0xe14 */
    volatile unsigned int para_haddr_vhd_chn03;                                     /* 0xe18 */
    volatile unsigned int para_addr_vhd_chn03;                                      /* 0xe1c */
    volatile unsigned int para_haddr_vhd_chn04;                                     /* 0xe20 */
    volatile unsigned int para_addr_vhd_chn04;                                      /* 0xe24 */
    volatile unsigned int para_haddr_vhd_chn05;                                     /* 0xe28 */
    volatile unsigned int para_addr_vhd_chn05;                                      /* 0xe2c */
    volatile unsigned int para_haddr_vhd_chn06;                                     /* 0xe30 */
    volatile unsigned int para_addr_vhd_chn06;                                      /* 0xe34 */
    volatile unsigned int para_haddr_vhd_chn07;                                     /* 0xe38 */
    volatile unsigned int para_addr_vhd_chn07;                                      /* 0xe3c */
    volatile unsigned int para_haddr_vhd_chn08;                                     /* 0xe40 */
    volatile unsigned int para_addr_vhd_chn08;                                      /* 0xe44 */
    volatile unsigned int para_haddr_vhd_chn09;                                     /* 0xe48 */
    volatile unsigned int para_addr_vhd_chn09;                                      /* 0xe4c */
    volatile unsigned int para_haddr_vhd_chn10;                                     /* 0xe50 */
    volatile unsigned int para_addr_vhd_chn10;                                      /* 0xe54 */
    volatile unsigned int para_haddr_vhd_chn11;                                     /* 0xe58 */
    volatile unsigned int para_addr_vhd_chn11;                                      /* 0xe5c */
    volatile unsigned int para_haddr_vhd_chn12;                                     /* 0xe60 */
    volatile unsigned int para_addr_vhd_chn12;                                      /* 0xe64 */
    volatile unsigned int para_haddr_vhd_chn13;                                     /* 0xe68 */
    volatile unsigned int para_addr_vhd_chn13;                                      /* 0xe6c */
    volatile unsigned int para_haddr_vhd_chn14;                                     /* 0xe70 */
    volatile unsigned int para_addr_vhd_chn14;                                      /* 0xe74 */
    volatile unsigned int para_haddr_vhd_chn15;                                     /* 0xe78 */
    volatile unsigned int para_addr_vhd_chn15;                                      /* 0xe7c */
    volatile unsigned int para_haddr_vhd_chn16;                                     /* 0xe80 */
    volatile unsigned int para_addr_vhd_chn16;                                      /* 0xe84 */
    volatile unsigned int para_haddr_vhd_chn17;                                     /* 0xe88 */
    volatile unsigned int para_addr_vhd_chn17;                                      /* 0xe8c */
    volatile unsigned int para_haddr_vhd_chn18;                                     /* 0xe90 */
    volatile unsigned int para_addr_vhd_chn18;                                      /* 0xe94 */
    volatile unsigned int para_haddr_vhd_chn19;                                     /* 0xe98 */
    volatile unsigned int para_addr_vhd_chn19;                                      /* 0xe9c */
    volatile unsigned int para_haddr_vhd_chn20;                                     /* 0xea0 */
    volatile unsigned int para_addr_vhd_chn20;                                      /* 0xea4 */
    volatile unsigned int para_haddr_vhd_chn21;                                     /* 0xea8 */
    volatile unsigned int para_addr_vhd_chn21;                                      /* 0xeac */
    volatile unsigned int para_haddr_vhd_chn22;                                     /* 0xeb0 */
    volatile unsigned int para_addr_vhd_chn22;                                      /* 0xeb4 */
    volatile unsigned int para_haddr_vhd_chn23;                                     /* 0xeb8 */
    volatile unsigned int para_addr_vhd_chn23;                                      /* 0xebc */
    volatile unsigned int para_haddr_vhd_chn24;                                     /* 0xec0 */
    volatile unsigned int para_addr_vhd_chn24;                                      /* 0xec4 */
    volatile unsigned int para_haddr_vhd_chn25;                                     /* 0xec8 */
    volatile unsigned int para_addr_vhd_chn25;                                      /* 0xecc */
    volatile unsigned int para_haddr_vhd_chn26;                                     /* 0xed0 */
    volatile unsigned int para_addr_vhd_chn26;                                      /* 0xed4 */
    volatile unsigned int para_haddr_vhd_chn27;                                     /* 0xed8 */
    volatile unsigned int para_addr_vhd_chn27;                                      /* 0xedc */
    volatile unsigned int para_haddr_vhd_chn28;                                     /* 0xee0 */
    volatile unsigned int para_addr_vhd_chn28;                                      /* 0xee4 */
    volatile unsigned int para_haddr_vhd_chn29;                                     /* 0xee8 */
    volatile unsigned int para_addr_vhd_chn29;                                      /* 0xeec */
    volatile unsigned int para_haddr_vhd_chn30;                                     /* 0xef0 */
    volatile unsigned int para_addr_vhd_chn30;                                      /* 0xef4 */
    volatile unsigned int para_haddr_vhd_chn31;                                     /* 0xef8 */
    volatile unsigned int para_addr_vhd_chn31;                                      /* 0xefc */
    volatile reg_para_up_vhd para_up_vhd;                                           /* 0xf00 */
    volatile unsigned int para_haddr_vsd_chn00;                                     /* 0xf04 */
    volatile unsigned int para_addr_vsd_chn00;                                      /* 0xf08 */
    volatile unsigned int para_haddr_vsd_chn01;                                     /* 0xf0c */
    volatile unsigned int para_addr_vsd_chn01;                                      /* 0xf10 */
    volatile unsigned int para_haddr_vsd_chn02;                                     /* 0xf14 */
    volatile unsigned int para_addr_vsd_chn02;                                      /* 0xf18 */
    volatile unsigned int para_haddr_vsd_chn03;                                     /* 0xf1c */
    volatile unsigned int para_addr_vsd_chn03;                                      /* 0xf20 */
    volatile unsigned int para_haddr_vsd_chn04;                                     /* 0xf24 */
    volatile unsigned int para_addr_vsd_chn04;                                      /* 0xf28 */
    volatile unsigned int para_haddr_vsd_chn05;                                     /* 0xf2c */
    volatile unsigned int para_addr_vsd_chn05;                                      /* 0xf30 */
    volatile unsigned int para_haddr_vsd_chn06;                                     /* 0xf34 */
    volatile unsigned int para_addr_vsd_chn06;                                      /* 0xf38 */
    volatile unsigned int para_haddr_vsd_chn07;                                     /* 0xf3c */
    volatile unsigned int para_addr_vsd_chn07;                                      /* 0xf40 */
    volatile reg_para_up_vsd para_up_vsd;                                           /* 0xf44 */
    volatile reg_para_conflict_clr para_conflict_clr;                               /* 0xf48 */
    volatile reg_para_conflict_sta para_conflict_sta;                               /* 0xf4c */
    volatile unsigned int reserved_22[44];                                          /* 0xf50~0xffc 44 regs */
    volatile reg_v0_ctrl v0_ctrl;                                                   /* 0x1000 */
    volatile reg_v0_upd v0_upd;                                                     /* 0x1004 */
    volatile reg_v0_0reso_read v0_0reso_read;                                       /* 0x1008 */
    volatile unsigned int reserved_23;                                              /* 0x100c */
    volatile reg_v0_ireso v0_ireso;                                                 /* 0x1010 */
    volatile unsigned int reserved_24[27];                                          /* 0x1014~0x107c 27 regs */
    volatile reg_v0_dfpos v0_dfpos;                                                 /* 0x1080 */
    volatile reg_v0_dlpos v0_dlpos;                                                 /* 0x1084 */
    volatile reg_v0_vfpos v0_vfpos;                                                 /* 0x1088 */
    volatile reg_v0_vlpos v0_vlpos;                                                 /* 0x108c */
    volatile reg_v0_bk v0_bk;                                                       /* 0x1090 */
    volatile reg_v0_alpha v0_alpha;                                                 /* 0x1094 */
    volatile reg_v0_mute_bk v0_mute_bk;                                             /* 0x1098 */
    volatile unsigned int reserved_25;                                              /* 0x109c */
    volatile reg_v0_rimwidth v0_rimwidth;                                           /* 0x10a0 */
    volatile reg_v0_rimcol0 v0_rimcol0;                                             /* 0x10a4 */
    volatile reg_v0_rimcol1 v0_rimcol1;                                             /* 0x10a8 */
    volatile unsigned int reserved_26[85];                                          /* 0x10ac~0x11fc 85 regs */
    volatile reg_v0_ot_pp_csc_ctrl v0_ot_pp_csc_ctrl;                                 /* 0x1200 */
    volatile reg_v0_ot_pp_csc_coef00 v0_ot_pp_csc_coef00;                             /* 0x1204 */
    volatile reg_v0_ot_pp_csc_coef01 v0_ot_pp_csc_coef01;                             /* 0x1208 */
    volatile reg_v0_ot_pp_csc_coef02 v0_ot_pp_csc_coef02;                             /* 0x120c */
    volatile reg_v0_ot_pp_csc_coef10 v0_ot_pp_csc_coef10;                             /* 0x1210 */
    volatile reg_v0_ot_pp_csc_coef11 v0_ot_pp_csc_coef11;                             /* 0x1214 */
    volatile reg_v0_ot_pp_csc_coef12 v0_ot_pp_csc_coef12;                             /* 0x1218 */
    volatile reg_v0_ot_pp_csc_coef20 v0_ot_pp_csc_coef20;                             /* 0x121c */
    volatile reg_v0_ot_pp_csc_coef21 v0_ot_pp_csc_coef21;                             /* 0x1220 */
    volatile reg_v0_ot_pp_csc_coef22 v0_ot_pp_csc_coef22;                             /* 0x1224 */
    volatile reg_v0_ot_pp_csc_scale v0_ot_pp_csc_scale;                               /* 0x1228 */
    volatile reg_v0_ot_pp_csc_idc0 v0_ot_pp_csc_idc0;                                 /* 0x122c */
    volatile reg_v0_ot_pp_csc_idc1 v0_ot_pp_csc_idc1;                                 /* 0x1230 */
    volatile reg_v0_ot_pp_csc_idc2 v0_ot_pp_csc_idc2;                                 /* 0x1234 */
    volatile reg_v0_ot_pp_csc_odc0 v0_ot_pp_csc_odc0;                                 /* 0x1238 */
    volatile reg_v0_ot_pp_csc_odc1 v0_ot_pp_csc_odc1;                                 /* 0x123c */
    volatile reg_v0_ot_pp_csc_odc2 v0_ot_pp_csc_odc2;                                 /* 0x1240 */
    volatile reg_v0_ot_pp_csc_min_y v0_ot_pp_csc_min_y;                               /* 0x1244 */
    volatile reg_v0_ot_pp_csc_min_c v0_ot_pp_csc_min_c;                               /* 0x1248 */
    volatile reg_v0_ot_pp_csc_max_y v0_ot_pp_csc_max_y;                               /* 0x124c */
    volatile reg_v0_ot_pp_csc_max_c v0_ot_pp_csc_max_c;                               /* 0x1250 */
    volatile reg_v0_ot_pp_csc2_coef00 v0_ot_pp_csc2_coef00;                           /* 0x1254 */
    volatile reg_v0_ot_pp_csc2_coef01 v0_ot_pp_csc2_coef01;                           /* 0x1258 */
    volatile reg_v0_ot_pp_csc2_coef02 v0_ot_pp_csc2_coef02;                           /* 0x125c */
    volatile reg_v0_ot_pp_csc2_coef10 v0_ot_pp_csc2_coef10;                           /* 0x1260 */
    volatile reg_v0_ot_pp_csc2_coef11 v0_ot_pp_csc2_coef11;                           /* 0x1264 */
    volatile reg_v0_ot_pp_csc2_coef12 v0_ot_pp_csc2_coef12;                           /* 0x1268 */
    volatile reg_v0_ot_pp_csc2_coef20 v0_ot_pp_csc2_coef20;                           /* 0x126c */
    volatile reg_v0_ot_pp_csc2_coef21 v0_ot_pp_csc2_coef21;                           /* 0x1270 */
    volatile reg_v0_ot_pp_csc2_coef22 v0_ot_pp_csc2_coef22;                           /* 0x1274 */
    volatile reg_v0_ot_pp_csc2_scale v0_ot_pp_csc2_scale;                             /* 0x1278 */
    volatile reg_v0_ot_pp_csc2_idc0 v0_ot_pp_csc2_idc0;                               /* 0x127c */
    volatile reg_v0_ot_pp_csc2_idc1 v0_ot_pp_csc2_idc1;                               /* 0x1280 */
    volatile reg_v0_ot_pp_csc2_idc2 v0_ot_pp_csc2_idc2;                               /* 0x1284 */
    volatile reg_v0_ot_pp_csc2_odc0 v0_ot_pp_csc2_odc0;                               /* 0x1288 */
    volatile reg_v0_ot_pp_csc2_odc1 v0_ot_pp_csc2_odc1;                               /* 0x128c */
    volatile reg_v0_ot_pp_csc2_odc2 v0_ot_pp_csc2_odc2;                               /* 0x1290 */
    volatile reg_v0_ot_pp_csc2_min_y v0_ot_pp_csc2_min_y;                             /* 0x1294 */
    volatile reg_v0_ot_pp_csc2_min_c v0_ot_pp_csc2_min_c;                             /* 0x1298 */
    volatile reg_v0_ot_pp_csc2_max_y v0_ot_pp_csc2_max_y;                             /* 0x129c */
    volatile reg_v0_ot_pp_csc2_max_c v0_ot_pp_csc2_max_c;                             /* 0x12a0 */
    volatile unsigned int reserved_27[19];                                          /* 0x12a4~0x12ec 19 regs */
    volatile reg_v0_ot_pp_csc_ink_ctrl v0_ot_pp_csc_ink_ctrl;                         /* 0x12f0 */
    volatile reg_v0_ot_pp_csc_ink_pos v0_ot_pp_csc_ink_pos;                           /* 0x12f4 */
    volatile unsigned int v0_ot_pp_csc_ink_data;                                     /* 0x12f8 */
    volatile unsigned int v0_ot_pp_csc_ink_data2;                                    /* 0x12fc */
    volatile unsigned int reserved_28[64];                                          /* 0x1300~0x13fc 64 regs */
    volatile reg_v0_cvfir_vinfo v0_cvfir_vinfo;                                     /* 0x1400 */
    volatile reg_v0_cvfir_vsp v0_cvfir_vsp;                                         /* 0x1404 */
    volatile reg_v0_cvfir_voffset v0_cvfir_voffset;                                 /* 0x1408 */
    volatile reg_v0_cvfir_vboffset v0_cvfir_vboffset;                               /* 0x140c */
    volatile unsigned int reserved_29[8];                                           /* 0x1410~0x142c 8 regs */
    volatile reg_v0_cvfir_vcoef0 v0_cvfir_vcoef0;                                   /* 0x1430 */
    volatile reg_v0_cvfir_vcoef1 v0_cvfir_vcoef1;                                   /* 0x1434 */
    volatile reg_v0_cvfir_vcoef2 v0_cvfir_vcoef2;                                   /* 0x1438 */
    volatile unsigned int reserved_30[49];                                          /* 0x143c~0x14fc 49 regs */
    volatile reg_v0_hfir_ctrl v0_hfir_ctrl;                                         /* 0x1500 */
    volatile reg_v0_hfircoef01 v0_hfircoef01;                                       /* 0x1504 */
    volatile reg_v0_hfircoef23 v0_hfircoef23;                                       /* 0x1508 */
    volatile reg_v0_hfircoef45 v0_hfircoef45;                                       /* 0x150c */
    volatile reg_v0_hfircoef67 v0_hfircoef67;                                       /* 0x1510 */
    volatile unsigned int reserved_31[699];                                         /* 0x1514~0x1ffc 699 regs */
    volatile reg_v1_ctrl v1_ctrl;                                                   /* 0x2000 */
    volatile reg_v1_upd v1_upd;                                                     /* 0x2004 */
    volatile reg_v1_0reso_read v1_0reso_read;                                       /* 0x2008 */
    volatile unsigned int reserved_32;                                              /* 0x200c */
    volatile reg_v1_ireso v1_ireso;                                                 /* 0x2010 */
    volatile unsigned int reserved_33[27];                                          /* 0x2014~0x207c 27 regs */
    volatile reg_v1_dfpos v1_dfpos;                                                 /* 0x2080 */
    volatile reg_v1_dlpos v1_dlpos;                                                 /* 0x2084 */
    volatile reg_v1_vfpos v1_vfpos;                                                 /* 0x2088 */
    volatile reg_v1_vlpos v1_vlpos;                                                 /* 0x208c */
    volatile reg_v1_bk v1_bk;                                                       /* 0x2090 */
    volatile reg_v1_alpha v1_alpha;                                                 /* 0x2094 */
    volatile reg_v1_mute_bk v1_mute_bk;                                             /* 0x2098 */
    volatile unsigned int reserved_34;                                              /* 0x209c */
    volatile reg_v1_rimwidth v1_rimwidth;                                           /* 0x20a0 */
    volatile reg_v1_rimcol0 v1_rimcol0;                                             /* 0x20a4 */
    volatile reg_v1_rimcol1 v1_rimcol1;                                             /* 0x20a8 */
    volatile unsigned int reserved_35[85];                                          /* 0x20ac~0x21fc 85 regs */
    volatile reg_v1_ot_pp_csc_ctrl v1_ot_pp_csc_ctrl;                                 /* 0x2200 */
    volatile reg_v1_ot_pp_csc_coef00 v1_ot_pp_csc_coef00;                             /* 0x2204 */
    volatile reg_v1_ot_pp_csc_coef01 v1_ot_pp_csc_coef01;                             /* 0x2208 */
    volatile reg_v1_ot_pp_csc_coef02 v1_ot_pp_csc_coef02;                             /* 0x220c */
    volatile reg_v1_ot_pp_csc_coef10 v1_ot_pp_csc_coef10;                             /* 0x2210 */
    volatile reg_v1_ot_pp_csc_coef11 v1_ot_pp_csc_coef11;                             /* 0x2214 */
    volatile reg_v1_ot_pp_csc_coef12 v1_ot_pp_csc_coef12;                             /* 0x2218 */
    volatile reg_v1_ot_pp_csc_coef20 v1_ot_pp_csc_coef20;                             /* 0x221c */
    volatile reg_v1_ot_pp_csc_coef21 v1_ot_pp_csc_coef21;                             /* 0x2220 */
    volatile reg_v1_ot_pp_csc_coef22 v1_ot_pp_csc_coef22;                             /* 0x2224 */
    volatile reg_v1_ot_pp_csc_scale v1_ot_pp_csc_scale;                               /* 0x2228 */
    volatile reg_v1_ot_pp_csc_idc0 v1_ot_pp_csc_idc0;                                 /* 0x222c */
    volatile reg_v1_ot_pp_csc_idc1 v1_ot_pp_csc_idc1;                                 /* 0x2230 */
    volatile reg_v1_ot_pp_csc_idc2 v1_ot_pp_csc_idc2;                                 /* 0x2234 */
    volatile reg_v1_ot_pp_csc_odc0 v1_ot_pp_csc_odc0;                                 /* 0x2238 */
    volatile reg_v1_ot_pp_csc_odc1 v1_ot_pp_csc_odc1;                                 /* 0x223c */
    volatile reg_v1_ot_pp_csc_odc2 v1_ot_pp_csc_odc2;                                 /* 0x2240 */
    volatile reg_v1_ot_pp_csc_min_y v1_ot_pp_csc_min_y;                               /* 0x2244 */
    volatile reg_v1_ot_pp_csc_min_c v1_ot_pp_csc_min_c;                               /* 0x2248 */
    volatile reg_v1_ot_pp_csc_max_y v1_ot_pp_csc_max_y;                               /* 0x224c */
    volatile reg_v1_ot_pp_csc_max_c v1_ot_pp_csc_max_c;                               /* 0x2250 */
    volatile reg_v1_ot_pp_csc2_coef00 v1_ot_pp_csc2_coef00;                           /* 0x2254 */
    volatile reg_v1_ot_pp_csc2_coef01 v1_ot_pp_csc2_coef01;                           /* 0x2258 */
    volatile reg_v1_ot_pp_csc2_coef02 v1_ot_pp_csc2_coef02;                           /* 0x225c */
    volatile reg_v1_ot_pp_csc2_coef10 v1_ot_pp_csc2_coef10;                           /* 0x2260 */
    volatile reg_v1_ot_pp_csc2_coef11 v1_ot_pp_csc2_coef11;                           /* 0x2264 */
    volatile reg_v1_ot_pp_csc2_coef12 v1_ot_pp_csc2_coef12;                           /* 0x2268 */
    volatile reg_v1_ot_pp_csc2_coef20 v1_ot_pp_csc2_coef20;                           /* 0x226c */
    volatile reg_v1_ot_pp_csc2_coef21 v1_ot_pp_csc2_coef21;                           /* 0x2270 */
    volatile reg_v1_ot_pp_csc2_coef22 v1_ot_pp_csc2_coef22;                           /* 0x2274 */
    volatile reg_v1_ot_pp_csc2_scale v1_ot_pp_csc2_scale;                             /* 0x2278 */
    volatile reg_v1_ot_pp_csc2_idc0 v1_ot_pp_csc2_idc0;                               /* 0x227c */
    volatile reg_v1_ot_pp_csc2_idc1 v1_ot_pp_csc2_idc1;                               /* 0x2280 */
    volatile reg_v1_ot_pp_csc2_idc2 v1_ot_pp_csc2_idc2;                               /* 0x2284 */
    volatile reg_v1_ot_pp_csc2_odc0 v1_ot_pp_csc2_odc0;                               /* 0x2288 */
    volatile reg_v1_ot_pp_csc2_odc1 v1_ot_pp_csc2_odc1;                               /* 0x228c */
    volatile reg_v1_ot_pp_csc2_odc2 v1_ot_pp_csc2_odc2;                               /* 0x2290 */
    volatile reg_v1_ot_pp_csc2_min_y v1_ot_pp_csc2_min_y;                             /* 0x2294 */
    volatile reg_v1_ot_pp_csc2_min_c v1_ot_pp_csc2_min_c;                             /* 0x2298 */
    volatile reg_v1_ot_pp_csc2_max_y v1_ot_pp_csc2_max_y;                             /* 0x229c */
    volatile reg_v1_ot_pp_csc2_max_c v1_ot_pp_csc2_max_c;                             /* 0x22a0 */
    volatile unsigned int reserved_36[19];                                          /* 0x22a4~0x22ec 19 regs */
    volatile reg_v1_ot_pp_csc_ink_ctrl v1_ot_pp_csc_ink_ctrl;                         /* 0x22f0 */
    volatile reg_v1_ot_pp_csc_ink_pos v1_ot_pp_csc_ink_pos;                           /* 0x22f4 */
    volatile unsigned int v1_ot_pp_csc_ink_data;                                     /* 0x22f8 */
    volatile unsigned int v1_ot_pp_csc_ink_data2;                                    /* 0x22fc */
    volatile unsigned int reserved_37[64];                                          /* 0x2300~0x23fc 64 regs */
    volatile reg_v1_cvfir_vinfo v1_cvfir_vinfo;                                     /* 0x2400 */
    volatile reg_v1_cvfir_vsp v1_cvfir_vsp;                                         /* 0x2404 */
    volatile reg_v1_cvfir_voffset v1_cvfir_voffset;                                 /* 0x2408 */
    volatile reg_v1_cvfir_vboffset v1_cvfir_vboffset;                               /* 0x240c */
    volatile unsigned int reserved_38[8];                                           /* 0x2410~0x242c 8 regs */
    volatile reg_v1_cvfir_vcoef0 v1_cvfir_vcoef0;                                   /* 0x2430 */
    volatile reg_v1_cvfir_vcoef1 v1_cvfir_vcoef1;                                   /* 0x2434 */
    volatile reg_v1_cvfir_vcoef2 v1_cvfir_vcoef2;                                   /* 0x2438 */
    volatile unsigned int reserved_39[49];                                          /* 0x243c~0x24fc 49 regs */
    volatile reg_v1_hfir_ctrl v1_hfir_ctrl;                                         /* 0x2500 */
    volatile reg_v1_hfircoef01 v1_hfircoef01;                                       /* 0x2504 */
    volatile reg_v1_hfircoef23 v1_hfircoef23;                                       /* 0x2508 */
    volatile reg_v1_hfircoef45 v1_hfircoef45;                                       /* 0x250c */
    volatile reg_v1_hfircoef67 v1_hfircoef67;                                       /* 0x2510 */
    volatile unsigned int reserved_40[699];                                         /* 0x2514~0x2ffc 699 regs */
    volatile reg_v2_ctrl v2_ctrl;                                                   /* 0x3000 */
    volatile reg_v2_upd v2_upd;                                                     /* 0x3004 */
    volatile reg_v2_0reso_read v2_0reso_read;                                       /* 0x3008 */
    volatile unsigned int reserved_41;                                              /* 0x300c */
    volatile reg_v2_ireso v2_ireso;                                                 /* 0x3010 */
    volatile unsigned int reserved_42[27];                                          /* 0x3014~0x307c 27 regs */
    volatile reg_v2_dfpos v2_dfpos;                                                 /* 0x3080 */
    volatile reg_v2_dlpos v2_dlpos;                                                 /* 0x3084 */
    volatile reg_v2_vfpos v2_vfpos;                                                 /* 0x3088 */
    volatile reg_v2_vlpos v2_vlpos;                                                 /* 0x308c */
    volatile reg_v2_bk v2_bk;                                                       /* 0x3090 */
    volatile reg_v2_alpha v2_alpha;                                                 /* 0x3094 */
    volatile reg_v2_mute_bk v2_mute_bk;                                             /* 0x3098 */
    volatile unsigned int reserved_43[89];                                          /* 0x309c~0x31fc 89 regs */
    volatile reg_v2_ot_pp_csc_ctrl v2_ot_pp_csc_ctrl;                                 /* 0x3200 */
    volatile reg_v2_ot_pp_csc_coef00 v2_ot_pp_csc_coef00;                             /* 0x3204 */
    volatile reg_v2_ot_pp_csc_coef01 v2_ot_pp_csc_coef01;                             /* 0x3208 */
    volatile reg_v2_ot_pp_csc_coef02 v2_ot_pp_csc_coef02;                             /* 0x320c */
    volatile reg_v2_ot_pp_csc_coef10 v2_ot_pp_csc_coef10;                             /* 0x3210 */
    volatile reg_v2_ot_pp_csc_coef11 v2_ot_pp_csc_coef11;                             /* 0x3214 */
    volatile reg_v2_ot_pp_csc_coef12 v2_ot_pp_csc_coef12;                             /* 0x3218 */
    volatile reg_v2_ot_pp_csc_coef20 v2_ot_pp_csc_coef20;                             /* 0x321c */
    volatile reg_v2_ot_pp_csc_coef21 v2_ot_pp_csc_coef21;                             /* 0x3220 */
    volatile reg_v2_ot_pp_csc_coef22 v2_ot_pp_csc_coef22;                             /* 0x3224 */
    volatile reg_v2_ot_pp_csc_scale v2_ot_pp_csc_scale;                               /* 0x3228 */
    volatile reg_v2_ot_pp_csc_idc0 v2_ot_pp_csc_idc0;                                 /* 0x322c */
    volatile reg_v2_ot_pp_csc_idc1 v2_ot_pp_csc_idc1;                                 /* 0x3230 */
    volatile reg_v2_ot_pp_csc_idc2 v2_ot_pp_csc_idc2;                                 /* 0x3234 */
    volatile reg_v2_ot_pp_csc_odc0 v2_ot_pp_csc_odc0;                                 /* 0x3238 */
    volatile reg_v2_ot_pp_csc_odc1 v2_ot_pp_csc_odc1;                                 /* 0x323c */
    volatile reg_v2_ot_pp_csc_odc2 v2_ot_pp_csc_odc2;                                 /* 0x3240 */
    volatile reg_v2_ot_pp_csc_min_y v2_ot_pp_csc_min_y;                               /* 0x3244 */
    volatile reg_v2_ot_pp_csc_min_c v2_ot_pp_csc_min_c;                               /* 0x3248 */
    volatile reg_v2_ot_pp_csc_max_y v2_ot_pp_csc_max_y;                               /* 0x324c */
    volatile reg_v2_ot_pp_csc_max_c v2_ot_pp_csc_max_c;                               /* 0x3250 */
    volatile reg_v2_ot_pp_csc2_coef00 v2_ot_pp_csc2_coef00;                           /* 0x3254 */
    volatile reg_v2_ot_pp_csc2_coef01 v2_ot_pp_csc2_coef01;                           /* 0x3258 */
    volatile reg_v2_ot_pp_csc2_coef02 v2_ot_pp_csc2_coef02;                           /* 0x325c */
    volatile reg_v2_ot_pp_csc2_coef10 v2_ot_pp_csc2_coef10;                           /* 0x3260 */
    volatile reg_v2_ot_pp_csc2_coef11 v2_ot_pp_csc2_coef11;                           /* 0x3264 */
    volatile reg_v2_ot_pp_csc2_coef12 v2_ot_pp_csc2_coef12;                           /* 0x3268 */
    volatile reg_v2_ot_pp_csc2_coef20 v2_ot_pp_csc2_coef20;                           /* 0x326c */
    volatile reg_v2_ot_pp_csc2_coef21 v2_ot_pp_csc2_coef21;                           /* 0x3270 */
    volatile reg_v2_ot_pp_csc2_coef22 v2_ot_pp_csc2_coef22;                           /* 0x3274 */
    volatile reg_v2_ot_pp_csc2_scale v2_ot_pp_csc2_scale;                             /* 0x3278 */
    volatile reg_v2_ot_pp_csc2_idc0 v2_ot_pp_csc2_idc0;                               /* 0x327c */
    volatile reg_v2_ot_pp_csc2_idc1 v2_ot_pp_csc2_idc1;                               /* 0x3280 */
    volatile reg_v2_ot_pp_csc2_idc2 v2_ot_pp_csc2_idc2;                               /* 0x3284 */
    volatile reg_v2_ot_pp_csc2_odc0 v2_ot_pp_csc2_odc0;                               /* 0x3288 */
    volatile reg_v2_ot_pp_csc2_odc1 v2_ot_pp_csc2_odc1;                               /* 0x328c */
    volatile reg_v2_ot_pp_csc2_odc2 v2_ot_pp_csc2_odc2;                               /* 0x3290 */
    volatile reg_v2_ot_pp_csc2_min_y v2_ot_pp_csc2_min_y;                             /* 0x3294 */
    volatile reg_v2_ot_pp_csc2_min_c v2_ot_pp_csc2_min_c;                             /* 0x3298 */
    volatile reg_v2_ot_pp_csc2_max_y v2_ot_pp_csc2_max_y;                             /* 0x329c */
    volatile reg_v2_ot_pp_csc2_max_c v2_ot_pp_csc2_max_c;                             /* 0x32a0 */
    volatile unsigned int reserved_44[19];                                          /* 0x32a4~0x32ec 19 regs */
    volatile reg_v2_ot_pp_csc_ink_ctrl v2_ot_pp_csc_ink_ctrl;                         /* 0x32f0 */
    volatile reg_v2_ot_pp_csc_ink_pos v2_ot_pp_csc_ink_pos;                           /* 0x32f4 */
    volatile unsigned int v2_ot_pp_csc_ink_data;                                     /* 0x32f8 */
    volatile unsigned int v2_ot_pp_csc_ink_data2;                                    /* 0x32fc */
    volatile unsigned int reserved_45[64];                                          /* 0x3300~0x33fc 64 regs */
    volatile reg_v2_cvfir_vinfo v2_cvfir_vinfo;                                     /* 0x3400 */
    volatile reg_v2_cvfir_vsp v2_cvfir_vsp;                                         /* 0x3404 */
    volatile reg_v2_cvfir_voffset v2_cvfir_voffset;                                 /* 0x3408 */
    volatile reg_v2_cvfir_vboffset v2_cvfir_vboffset;                               /* 0x340c */
    volatile unsigned int reserved_46[8];                                           /* 0x3410~0x342c 8 regs */
    volatile reg_v2_cvfir_vcoef0 v2_cvfir_vcoef0;                                   /* 0x3430 */
    volatile reg_v2_cvfir_vcoef1 v2_cvfir_vcoef1;                                   /* 0x3434 */
    volatile reg_v2_cvfir_vcoef2 v2_cvfir_vcoef2;                                   /* 0x3438 */
    volatile unsigned int reserved_47[49];                                          /* 0x343c~0x34fc 49 regs */
    volatile reg_v2_hfir_ctrl v2_hfir_ctrl;                                         /* 0x3500 */
    volatile reg_v2_hfircoef01 v2_hfircoef01;                                       /* 0x3504 */
    volatile reg_v2_hfircoef23 v2_hfircoef23;                                       /* 0x3508 */
    volatile reg_v2_hfircoef45 v2_hfircoef45;                                       /* 0x350c */
    volatile reg_v2_hfircoef67 v2_hfircoef67;                                       /* 0x3510 */
    volatile unsigned int reserved_48[699];                                         /* 0x3514~0x3ffc 699 regs */
    volatile reg_v3_ctrl v3_ctrl;                                                   /* 0x4000 */
    volatile reg_v3_upd v3_upd;                                                     /* 0x4004 */
    volatile reg_v3_0reso_read v3_0reso_read;                                       /* 0x4008 */
    volatile unsigned int reserved_49;                                              /* 0x400c */
    volatile reg_v3_ireso v3_ireso;                                                 /* 0x4010 */
    volatile unsigned int reserved_50[27];                                          /* 0x4014~0x407c 27 regs */
    volatile reg_v3_dfpos v3_dfpos;                                                 /* 0x4080 */
    volatile reg_v3_dlpos v3_dlpos;                                                 /* 0x4084 */
    volatile reg_v3_vfpos v3_vfpos;                                                 /* 0x4088 */
    volatile reg_v3_vlpos v3_vlpos;                                                 /* 0x408c */
    volatile reg_v3_bk v3_bk;                                                       /* 0x4090 */
    volatile reg_v3_alpha v3_alpha;                                                 /* 0x4094 */
    volatile reg_v3_mute_bk v3_mute_bk;                                             /* 0x4098 */
    volatile unsigned int reserved_51;                                              /* 0x409c */
    volatile reg_v3_rimwidth v3_rimwidth;                                           /* 0x40a0 */
    volatile reg_v3_rimcol0 v3_rimcol0;                                             /* 0x40a4 */
    volatile reg_v3_rimcol1 v3_rimcol1;                                             /* 0x40a8 */
    volatile unsigned int reserved_52[85];                                          /* 0x40ac~0x41fc 85 regs */
    volatile reg_v3_ot_pp_csc_ctrl v3_ot_pp_csc_ctrl;                                 /* 0x4200 */
    volatile reg_v3_ot_pp_csc_coef00 v3_ot_pp_csc_coef00;                             /* 0x4204 */
    volatile reg_v3_ot_pp_csc_coef01 v3_ot_pp_csc_coef01;                             /* 0x4208 */
    volatile reg_v3_ot_pp_csc_coef02 v3_ot_pp_csc_coef02;                             /* 0x420c */
    volatile reg_v3_ot_pp_csc_coef10 v3_ot_pp_csc_coef10;                             /* 0x4210 */
    volatile reg_v3_ot_pp_csc_coef11 v3_ot_pp_csc_coef11;                             /* 0x4214 */
    volatile reg_v3_ot_pp_csc_coef12 v3_ot_pp_csc_coef12;                             /* 0x4218 */
    volatile reg_v3_ot_pp_csc_coef20 v3_ot_pp_csc_coef20;                             /* 0x421c */
    volatile reg_v3_ot_pp_csc_coef21 v3_ot_pp_csc_coef21;                             /* 0x4220 */
    volatile reg_v3_ot_pp_csc_coef22 v3_ot_pp_csc_coef22;                             /* 0x4224 */
    volatile reg_v3_ot_pp_csc_scale v3_ot_pp_csc_scale;                               /* 0x4228 */
    volatile reg_v3_ot_pp_csc_idc0 v3_ot_pp_csc_idc0;                                 /* 0x422c */
    volatile reg_v3_ot_pp_csc_idc1 v3_ot_pp_csc_idc1;                                 /* 0x4230 */
    volatile reg_v3_ot_pp_csc_idc2 v3_ot_pp_csc_idc2;                                 /* 0x4234 */
    volatile reg_v3_ot_pp_csc_odc0 v3_ot_pp_csc_odc0;                                 /* 0x4238 */
    volatile reg_v3_ot_pp_csc_odc1 v3_ot_pp_csc_odc1;                                 /* 0x423c */
    volatile reg_v3_ot_pp_csc_odc2 v3_ot_pp_csc_odc2;                                 /* 0x4240 */
    volatile reg_v3_ot_pp_csc_min_y v3_ot_pp_csc_min_y;                               /* 0x4244 */
    volatile reg_v3_ot_pp_csc_min_c v3_ot_pp_csc_min_c;                               /* 0x4248 */
    volatile reg_v3_ot_pp_csc_max_y v3_ot_pp_csc_max_y;                               /* 0x424c */
    volatile reg_v3_ot_pp_csc_max_c v3_ot_pp_csc_max_c;                               /* 0x4250 */
    volatile reg_v3_ot_pp_csc2_coef00 v3_ot_pp_csc2_coef00;                           /* 0x4254 */
    volatile reg_v3_ot_pp_csc2_coef01 v3_ot_pp_csc2_coef01;                           /* 0x4258 */
    volatile reg_v3_ot_pp_csc2_coef02 v3_ot_pp_csc2_coef02;                           /* 0x425c */
    volatile reg_v3_ot_pp_csc2_coef10 v3_ot_pp_csc2_coef10;                           /* 0x4260 */
    volatile reg_v3_ot_pp_csc2_coef11 v3_ot_pp_csc2_coef11;                           /* 0x4264 */
    volatile reg_v3_ot_pp_csc2_coef12 v3_ot_pp_csc2_coef12;                           /* 0x4268 */
    volatile reg_v3_ot_pp_csc2_coef20 v3_ot_pp_csc2_coef20;                           /* 0x426c */
    volatile reg_v3_ot_pp_csc2_coef21 v3_ot_pp_csc2_coef21;                           /* 0x4270 */
    volatile reg_v3_ot_pp_csc2_coef22 v3_ot_pp_csc2_coef22;                           /* 0x4274 */
    volatile reg_v3_ot_pp_csc2_scale v3_ot_pp_csc2_scale;                             /* 0x4278 */
    volatile reg_v3_ot_pp_csc2_idc0 v3_ot_pp_csc2_idc0;                               /* 0x427c */
    volatile reg_v3_ot_pp_csc2_idc1 v3_ot_pp_csc2_idc1;                               /* 0x4280 */
    volatile reg_v3_ot_pp_csc2_idc2 v3_ot_pp_csc2_idc2;                               /* 0x4284 */
    volatile reg_v3_ot_pp_csc2_odc0 v3_ot_pp_csc2_odc0;                               /* 0x4288 */
    volatile reg_v3_ot_pp_csc2_odc1 v3_ot_pp_csc2_odc1;                               /* 0x428c */
    volatile reg_v3_ot_pp_csc2_odc2 v3_ot_pp_csc2_odc2;                               /* 0x4290 */
    volatile reg_v3_ot_pp_csc2_min_y v3_ot_pp_csc2_min_y;                             /* 0x4294 */
    volatile reg_v3_ot_pp_csc2_min_c v3_ot_pp_csc2_min_c;                             /* 0x4298 */
    volatile reg_v3_ot_pp_csc2_max_y v3_ot_pp_csc2_max_y;                             /* 0x429c */
    volatile reg_v3_ot_pp_csc2_max_c v3_ot_pp_csc2_max_c;                             /* 0x42a0 */
    volatile unsigned int reserved_53[19];                                          /* 0x42a4~0x42ec 19 regs */
    volatile reg_v3_ot_pp_csc_ink_ctrl v3_ot_pp_csc_ink_ctrl;                         /* 0x42f0 */
    volatile reg_v3_ot_pp_csc_ink_pos v3_ot_pp_csc_ink_pos;                           /* 0x42f4 */
    volatile unsigned int v3_ot_pp_csc_ink_data;                                     /* 0x42f8 */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART44_H_ */
