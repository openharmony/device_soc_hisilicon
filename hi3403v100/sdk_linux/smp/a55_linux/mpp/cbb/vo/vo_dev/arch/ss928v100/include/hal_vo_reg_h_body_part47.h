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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART47_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART47_H_

    volatile reg_mipi_dither_sed_u3 mipi_dither_sed_u3;                             /* 0xd838 */
    volatile reg_mipi_dither_sed_v3 mipi_dither_sed_v3;                             /* 0xd83c */
    volatile reg_mipi_dither_sed_w3 mipi_dither_sed_w3;                             /* 0xd840 */
    volatile reg_mipi_dither_thr mipi_dither_thr;                                   /* 0xd844 */
    volatile unsigned int reserved_156[494];                                        /* 0xd848~0xdffc 494 regs */
    volatile reg_dhd1_ctrl dhd1_ctrl;                                               /* 0xe000 */
    volatile reg_dhd1_vsync1 dhd1_vsync1;                                           /* 0xe004 */
    volatile reg_dhd1_vsync2 dhd1_vsync2;                                           /* 0xe008 */
    volatile reg_dhd1_hsync1 dhd1_hsync1;                                           /* 0xe00c */
    volatile reg_dhd1_hsync2 dhd1_hsync2;                                           /* 0xe010 */
    volatile reg_dhd1_vplus1 dhd1_vplus1;                                           /* 0xe014 */
    volatile reg_dhd1_vplus2 dhd1_vplus2;                                           /* 0xe018 */
    volatile reg_dhd1_pwr dhd1_pwr;                                                 /* 0xe01c */
    volatile reg_dhd1_vtthd3 dhd1_vtthd3;                                           /* 0xe020 */
    volatile reg_dhd1_vtthd dhd1_vtthd;                                             /* 0xe024 */
    volatile reg_dhd1_parathd dhd1_parathd;                                         /* 0xe028 */
    volatile reg_dhd1_precharge_thd dhd1_precharge_thd;                             /* 0xe02c */
    volatile reg_dhd1_start_pos dhd1_start_pos;                                     /* 0xe030 */
    volatile reg_dhd1_start_pos1 dhd1_start_pos1;                                   /* 0xe034 */
    volatile reg_dhd1_paraup dhd1_paraup;                                           /* 0xe038 */
    volatile reg_dhd1_sync_inv dhd1_sync_inv;                                       /* 0xe03c */
    volatile reg_dhd1_clk_dv_ctrl dhd1_clk_dv_ctrl;                                 /* 0xe040 */
    volatile reg_dhd1_rgb_fix_ctrl dhd1_rgb_fix_ctrl;                               /* 0xe044 */
    volatile reg_dhd1_lockcfg dhd1_lockcfg;                                         /* 0xe048 */
    volatile unsigned int dhd1_cap_frm_cnt;                                         /* 0xe04c */
    volatile unsigned int dhd1_vdp_frm_cnt;                                         /* 0xe050 */
    volatile unsigned int dhd1_vsync_cap_vdp_cnt;                                   /* 0xe054 */
    volatile unsigned int dhd1_intf_chksum_y;                                       /* 0xe058 */
    volatile unsigned int dhd1_intf_chksum_u;                                       /* 0xe05c */
    volatile unsigned int dhd1_intf_chksum_v;                                       /* 0xe060 */
    volatile unsigned int dhd1_intf1_chksum_y;                                      /* 0xe064 */
    volatile unsigned int dhd1_intf1_chksum_u;                                      /* 0xe068 */
    volatile unsigned int dhd1_intf1_chksum_v;                                      /* 0xe06c */
    volatile reg_dhd1_intf_chksum_high1 dhd1_intf_chksum_high1;                     /* 0xe070 */
    volatile reg_dhd1_intf_chksum_high2 dhd1_intf_chksum_high2;                     /* 0xe074 */
    volatile unsigned int reserved_157[3];                                          /* 0xe078~0xe080 3 regs */
    volatile unsigned int dhd1_afifo_pre_thd;                                       /* 0xe084 */
    volatile reg_dhd1_state dhd1_state;                                             /* 0xe088 */
    volatile reg_dhd1_uf_state dhd1_uf_state;                                       /* 0xe08c */
    volatile unsigned int reserved_158[4];                                          /* 0xe090~0xe09c 4 regs */
    volatile reg_dhd1_vsync_te_state dhd1_vsync_te_state;                           /* 0xe0a0 */
    volatile reg_dhd1_vsync_te_state1 dhd1_vsync_te_state1;                         /* 0xe0a4 */
    volatile unsigned int reserved_159[406];                                        /* 0xe0a8~0xe6fc 406 regs */
    volatile reg_intf1_dither_ctrl intf1_dither_ctrl;                               /* 0xe700 */
    volatile reg_intf1_dither_sed_y0 intf1_dither_sed_y0;                           /* 0xe704 */
    volatile reg_intf1_dither_sed_u0 intf1_dither_sed_u0;                           /* 0xe708 */
    volatile reg_intf1_dither_sed_v0 intf1_dither_sed_v0;                           /* 0xe70c */
    volatile reg_intf1_dither_sed_w0 intf1_dither_sed_w0;                           /* 0xe710 */
    volatile reg_intf1_dither_sed_y1 intf1_dither_sed_y1;                           /* 0xe714 */
    volatile reg_intf1_dither_sed_u1 intf1_dither_sed_u1;                           /* 0xe718 */
    volatile reg_intf1_dither_sed_v1 intf1_dither_sed_v1;                           /* 0xe71c */
    volatile reg_intf1_dither_sed_w1 intf1_dither_sed_w1;                           /* 0xe720 */
    volatile reg_intf1_dither_sed_y2 intf1_dither_sed_y2;                           /* 0xe724 */
    volatile reg_intf1_dither_sed_u2 intf1_dither_sed_u2;                           /* 0xe728 */
    volatile reg_intf1_dither_sed_v2 intf1_dither_sed_v2;                           /* 0xe72c */
    volatile reg_intf1_dither_sed_w2 intf1_dither_sed_w2;                           /* 0xe730 */
    volatile reg_intf1_dither_sed_y3 intf1_dither_sed_y3;                           /* 0xe734 */
    volatile reg_intf1_dither_sed_u3 intf1_dither_sed_u3;                           /* 0xe738 */
    volatile reg_intf1_dither_sed_v3 intf1_dither_sed_v3;                           /* 0xe73c */
    volatile reg_intf1_dither_sed_w3 intf1_dither_sed_w3;                           /* 0xe740 */
    volatile reg_intf1_dither_thr intf1_dither_thr;                                 /* 0xe744 */
    volatile unsigned int reserved_160[558];                                        /* 0xe748~0xeffc 558 regs */
    volatile reg_dhd2_ctrl dhd2_ctrl;                                               /* 0xf000 */
    volatile reg_dhd2_vsync1 dhd2_vsync1;                                           /* 0xf004 */
    volatile reg_dhd2_vsync2 dhd2_vsync2;                                           /* 0xf008 */
    volatile reg_dhd2_hsync1 dhd2_hsync1;                                           /* 0xf00c */
    volatile reg_dhd2_hsync2 dhd2_hsync2;                                           /* 0xf010 */
    volatile reg_dhd2_vplus1 dhd2_vplus1;                                           /* 0xf014 */
    volatile reg_dhd2_vplus2 dhd2_vplus2;                                           /* 0xf018 */
    volatile reg_dhd2_pwr dhd2_pwr;                                                 /* 0xf01c */
    volatile reg_dhd2_vtthd3 dhd2_vtthd3;                                           /* 0xf020 */
    volatile reg_dhd2_vtthd dhd2_vtthd;                                             /* 0xf024 */
    volatile reg_dhd2_parathd dhd2_parathd;                                         /* 0xf028 */
    volatile reg_dhd2_precharge_thd dhd2_precharge_thd;                             /* 0xf02c */
    volatile reg_dhd2_start_pos dhd2_start_pos;                                     /* 0xf030 */
    volatile reg_dhd2_start_pos1 dhd2_start_pos1;                                   /* 0xf034 */
    volatile reg_dhd2_paraup dhd2_paraup;                                           /* 0xf038 */
    volatile reg_dhd2_sync_inv dhd2_sync_inv;                                       /* 0xf03c */
    volatile reg_dhd2_clk_dv_ctrl dhd2_clk_dv_ctrl;                                 /* 0xf040 */
    volatile reg_dhd2_rgb_fix_ctrl dhd2_rgb_fix_ctrl;                               /* 0xf044 */
    volatile reg_dhd2_lockcfg dhd2_lockcfg;                                         /* 0xf048 */
    volatile unsigned int dhd2_cap_frm_cnt;                                         /* 0xf04c */
    volatile unsigned int dhd2_vdp_frm_cnt;                                         /* 0xf050 */
    volatile unsigned int dhd2_vsync_cap_vdp_cnt;                                   /* 0xf054 */
    volatile unsigned int dhd2_intf_chksum_y;                                       /* 0xf058 */
    volatile unsigned int dhd2_intf_chksum_u;                                       /* 0xf05c */
    volatile unsigned int dhd2_intf_chksum_v;                                       /* 0xf060 */
    volatile unsigned int dhd2_intf1_chksum_y;                                      /* 0xf064 */
    volatile unsigned int dhd2_intf1_chksum_u;                                      /* 0xf068 */
    volatile unsigned int dhd2_intf1_chksum_v;                                      /* 0xf06c */
    volatile reg_dhd2_intf_chksum_high1 dhd2_intf_chksum_high1;                     /* 0xf070 */
    volatile reg_dhd2_intf_chksum_high2 dhd2_intf_chksum_high2;                     /* 0xf074 */
    volatile unsigned int reserved_161[3];                                          /* 0xf078~0xf080 3 regs */
    volatile unsigned int dhd2_afifo_pre_thd;                                       /* 0xf084 */
    volatile reg_dhd2_state dhd2_state;                                             /* 0xf088 */
    volatile reg_dhd2_uf_state dhd2_uf_state;                                       /* 0xf08c */
    volatile unsigned int reserved_162[4];                                          /* 0xf090~0xf09c 4 regs */
    volatile reg_dhd2_vsync_te_state dhd2_vsync_te_state;                           /* 0xf0a0 */
    volatile reg_dhd2_vsync_te_state1 dhd2_vsync_te_state1;                         /* 0xf0a4 */
    volatile unsigned int reserved_163[406];                                        /* 0xf0a8~0xf6fc 406 regs */
    volatile reg_intf2_dither_ctrl intf2_dither_ctrl;                               /* 0xf700 */
    volatile reg_intf2_dither_sed_y0 intf2_dither_sed_y0;                           /* 0xf704 */
    volatile reg_intf2_dither_sed_u0 intf2_dither_sed_u0;                           /* 0xf708 */
    volatile reg_intf2_dither_sed_v0 intf2_dither_sed_v0;                           /* 0xf70c */
    volatile reg_intf2_dither_sed_w0 intf2_dither_sed_w0;                           /* 0xf710 */
    volatile reg_intf2_dither_sed_y1 intf2_dither_sed_y1;                           /* 0xf714 */
    volatile reg_intf2_dither_sed_u1 intf2_dither_sed_u1;                           /* 0xf718 */
    volatile reg_intf2_dither_sed_v1 intf2_dither_sed_v1;                           /* 0xf71c */
    volatile reg_intf2_dither_sed_w1 intf2_dither_sed_w1;                           /* 0xf720 */
    volatile reg_intf2_dither_sed_y2 intf2_dither_sed_y2;                           /* 0xf724 */
    volatile reg_intf2_dither_sed_u2 intf2_dither_sed_u2;                           /* 0xf728 */
    volatile reg_intf2_dither_sed_v2 intf2_dither_sed_v2;                           /* 0xf72c */
    volatile reg_intf2_dither_sed_w2 intf2_dither_sed_w2;                           /* 0xf730 */
    volatile reg_intf2_dither_sed_y3 intf2_dither_sed_y3;                           /* 0xf734 */
    volatile reg_intf2_dither_sed_u3 intf2_dither_sed_u3;                           /* 0xf738 */
    volatile reg_intf2_dither_sed_v3 intf2_dither_sed_v3;                           /* 0xf73c */
    volatile reg_intf2_dither_sed_w3 intf2_dither_sed_w3;                           /* 0xf740 */
    volatile reg_intf2_dither_thr intf2_dither_thr;                                 /* 0xf744 */
    volatile unsigned int reserved_164[46];                                         /* 0xf748~0xf7fc 46 regs */
    volatile reg_date_coeff0 date_coeff0;                                           /* 0xf800 */
    volatile reg_date_coeff1 date_coeff1;                                           /* 0xf804 */
    volatile unsigned int date_coeff2;                                              /* 0xf808 */
    volatile reg_date_coeff3 date_coeff3;                                           /* 0xf80c */
    volatile reg_date_coeff4 date_coeff4;                                           /* 0xf810 */
    volatile reg_date_coeff5 date_coeff5;                                           /* 0xf814 */
    volatile reg_date_coeff6 date_coeff6;                                           /* 0xf818 */
    volatile reg_date_coeff7 date_coeff7;                                           /* 0xf81c */
    volatile unsigned int date_coeff8;                                              /* 0xf820 */
    volatile unsigned int date_coeff9;                                              /* 0xf824 */
    volatile reg_date_coeff10 date_coeff10;                                         /* 0xf828 */
    volatile reg_date_coeff11 date_coeff11;                                         /* 0xf82c */
    volatile reg_date_coeff12 date_coeff12;                                         /* 0xf830 */
    volatile reg_date_coeff13 date_coeff13;                                         /* 0xf834 */
    volatile reg_date_coeff14 date_coeff14;                                         /* 0xf838 */
    volatile reg_date_coeff15 date_coeff15;                                         /* 0xf83c */
    volatile reg_date_coeff16 date_coeff16;                                         /* 0xf840 */
    volatile unsigned int date_coeff17;                                             /* 0xf844 */
    volatile unsigned int date_coeff18;                                             /* 0xf848 */
    volatile reg_date_coeff19 date_coeff19;                                         /* 0xf84c */
    volatile reg_date_coeff20 date_coeff20;                                         /* 0xf850 */
    volatile reg_date_coeff21 date_coeff21;                                         /* 0xf854 */
    volatile reg_date_coeff22 date_coeff22;                                         /* 0xf858 */
    volatile reg_date_coeff23 date_coeff23;                                         /* 0xf85c */
    volatile unsigned int date_coeff24;                                             /* 0xf860 */
    volatile reg_date_coeff25 date_coeff25;                                         /* 0xf864 */
    volatile reg_date_coeff26 date_coeff26;                                         /* 0xf868 */
    volatile reg_date_coeff27 date_coeff27;                                         /* 0xf86c */
    volatile reg_date_coeff28 date_coeff28;                                         /* 0xf870 */
    volatile reg_date_coeff29 date_coeff29;                                         /* 0xf874 */
    volatile reg_date_coeff30 date_coeff30;                                         /* 0xf878 */
    volatile unsigned int reserved_165;                                             /* 0xf87c */
    volatile reg_date_isrmask date_isrmask;                                         /* 0xf880 */
    volatile reg_date_isrstate date_isrstate;                                       /* 0xf884 */
    volatile reg_date_isr date_isr;                                                 /* 0xf888 */
    volatile unsigned int reserved_166;                                             /* 0xf88c */
    volatile unsigned int date_version;                                             /* 0xf890 */
    volatile reg_date_coeff37 date_coeff37;                                         /* 0xf894 */
    volatile reg_date_coeff38 date_coeff38;                                         /* 0xf898 */
    volatile reg_date_coeff39 date_coeff39;                                         /* 0xf89c */
    volatile reg_date_coeff40 date_coeff40;                                         /* 0xf8a0 */
    volatile reg_date_coeff41 date_coeff41;                                         /* 0xf8a4 */
    volatile reg_date_coeff42 date_coeff42;                                         /* 0xf8a8 */
    volatile unsigned int reserved_167[5];                                          /* 0xf8ac~0xf8bc 5 regs */
    volatile reg_date_dacdet1 date_dacdet1;                                         /* 0xf8c0 */
    volatile reg_date_dacdet2 date_dacdet2;                                         /* 0xf8c4 */
    volatile reg_date_coeff50 date_coeff50;                                         /* 0xf8c8 */
    volatile reg_date_coeff51 date_coeff51;                                         /* 0xf8cc */
    volatile reg_date_coeff52 date_coeff52;                                         /* 0xf8d0 */
    volatile reg_date_coeff53 date_coeff53;                                         /* 0xf8d4 */
    volatile reg_date_coeff54 date_coeff54;                                         /* offset 0xf8d0 + 0x08 */
    volatile reg_date_coeff55 date_coeff55;                                         /* 0xf8dc */
    volatile unsigned int reserved_168_1;                                           /* 0xf8e0 */
    volatile reg_date_coeff57 date_coeff57;                                         /* 0xf8e4 */
    volatile unsigned int reserved_168_2[454];                                      /* 0xf8e8~0xfffc 454 regs */
    volatile reg_mac_outstanding mac_outstanding;                                   /* 0x10000 */
    volatile reg_mac_ctrl mac_ctrl;                                                 /* 0x10004 */
    volatile unsigned int reserved_169[2];                                          /* 0x10008~0x1000c 2 regs */
    volatile reg_mac_rchn_prio mac_rchn_prio;                                       /* 0x10010 */
    volatile unsigned int reserved_170;                                             /* 0x10014 */
    volatile reg_mac_wchn_prio mac_wchn_prio;                                       /* 0x10018 */
    volatile unsigned int reserved_171;                                             /* 0x1001c */
    volatile reg_mac_rchn_sel0 mac_rchn_sel0;                                       /* 0x10020 */
    volatile unsigned int mac_rchn_sel1;                                            /* 0x10024 */
    volatile unsigned int reserved_172[2];                                          /* 0x10028~0x1002c 2 regs */
    volatile reg_mac_wchn_sel0 mac_wchn_sel0;                                       /* 0x10030 */
    volatile unsigned int reserved_173[3];                                          /* 0x10034~0x1003c 3 regs */
    volatile reg_mac_bus_err_clr mac_bus_err_clr;                                   /* 0x10040 */
    volatile reg_mac_bus_err mac_bus_err;                                           /* 0x10044 */
    volatile unsigned int reserved_174[2];                                          /* 0x10048~0x1004c 2 regs */
    volatile unsigned int mac_src0_status0;                                         /* 0x10050 */
    volatile unsigned int mac_src0_status1;                                         /* 0x10054 */
    volatile unsigned int mac_src1_status0;                                         /* 0x10058 */
    volatile unsigned int mac_src1_status1;                                         /* 0x1005c */
    volatile unsigned int mac_src2_status0;                                         /* 0x10060 */
    volatile unsigned int mac_src2_status1;                                         /* 0x10064 */
    volatile unsigned int reserved_175[2];                                          /* 0x10068~0x1006c 2 regs */
    volatile reg_mac_debug_ctrl mac_debug_ctrl;                                     /* 0x10070 */
    volatile reg_mac_debug_clr mac_debug_clr;                                       /* 0x10074 */
    volatile unsigned int reserved_176[2];                                          /* 0x10078~0x1007c 2 regs */
    volatile unsigned int mac0_debug_info;                                          /* 0x10080 */
    volatile unsigned int reserved_177[3];                                          /* 0x10084~0x1008c 3 regs */
    volatile unsigned int mac0_rd_info;                                             /* 0x10090 */
    volatile unsigned int mac0_wr_info;                                             /* 0x10094 */
    volatile unsigned int mac1_rd_info;                                             /* 0x10098 */
    volatile unsigned int mac1_wr_info;                                             /* 0x1009c */
    volatile unsigned int mac2_rd_info;                                             /* 0x100a0 */
    volatile unsigned int mac2_wr_info;                                             /* 0x100a4 */
    volatile unsigned int reserved_178[2];                                          /* 0x100a8~0x100ac 2 regs */
    volatile unsigned int mac0_det_latency0;                                        /* 0x100b0 */
    volatile unsigned int mac0_det_latency1;                                        /* 0x100b4 */
    volatile unsigned int mac0_det_latency2;                                        /* 0x100b8 */
    volatile unsigned int mac0_det_latency3;                                        /* 0x100bc */
    volatile unsigned int mac0_det_latency4;                                        /* 0x100c0 */
    volatile unsigned int mac0_det_latency5;                                        /* 0x100c4 */
    volatile unsigned int mac1_det_latency0;                                        /* 0x100c8 */
    volatile unsigned int mac1_det_latency1;                                        /* 0x100cc */
    volatile unsigned int mac1_det_latency2;                                        /* 0x100d0 */
    volatile unsigned int mac1_det_latency3;                                        /* 0x100d4 */
    volatile unsigned int mac1_det_latency4;                                        /* offset 0x100d0 + 0x08 */
    volatile unsigned int mac1_det_latency5;                                        /* 0x100dc */
    volatile unsigned int reserved_179[8];                                          /* 0x100e0~0x100fc 8 regs */
    volatile reg_mac_axi_press0_ctrl0 mac_axi_press0_ctrl0;                         /* 0x10100 */
    volatile reg_mac_axi_press0_ctrl1 mac_axi_press0_ctrl1;                         /* 0x10104 */
    volatile reg_mac_axi_press0_ctrl2 mac_axi_press0_ctrl2;                         /* 0x10108 */
    volatile reg_mac_axi_press0_ctrl3 mac_axi_press0_ctrl3;                         /* 0x1010c */
    volatile reg_mac_axi_press0_ctrl4 mac_axi_press0_ctrl4;                         /* 0x10110 */
    volatile unsigned int reserved_180[3];                                          /* 0x10114~0x1011c 3 regs */
    volatile reg_mac_axi_press0_ctrl5 mac_axi_press0_ctrl5;                         /* 0x10120 */
    volatile unsigned int reserved_181[23];                                         /* 0x10124~0x1017c 23 regs */
    volatile reg_mac_axi_press1_ctrl0 mac_axi_press1_ctrl0;                         /* 0x10180 */
    volatile reg_mac_axi_press1_ctrl1 mac_axi_press1_ctrl1;                         /* 0x10184 */
    volatile reg_mac_axi_press1_ctrl2 mac_axi_press1_ctrl2;                         /* 0x10188 */
    volatile reg_mac_axi_press1_ctrl3 mac_axi_press1_ctrl3;                         /* 0x1018c */
    volatile reg_mac_axi_press1_ctrl4 mac_axi_press1_ctrl4;                         /* 0x10190 */
    volatile unsigned int reserved_182[3];                                          /* 0x10194~0x1019c 3 regs */
    volatile reg_mac_axi_press1_ctrl5 mac_axi_press1_ctrl5;                         /* 0x101a0 */
    volatile unsigned int reserved_183[23];                                         /* 0x101a4~0x101fc 23 regs */
    volatile reg_vid_read_ctrl vid_read_ctrl;                                       /* 0x10200 */
    volatile reg_vid_mac_ctrl vid_mac_ctrl;                                         /* 0x10204 */
    volatile unsigned int reserved_184[2];                                          /* 0x10208~0x1020c 2 regs */
    volatile reg_vid_out_ctrl vid_out_ctrl;                                         /* 0x10210 */
    volatile reg_vid_mute_alpha vid_mute_alpha;                                     /* 0x10214 */
    volatile unsigned int reserved_185;                                             /* 0x10218 */
    volatile reg_vid_mute_bk vid_mute_bk;                                           /* 0x1021c */
    volatile unsigned int reserved_186[8];                                          /* 0x10220~0x1023c 8 regs */
    volatile reg_vid_src_info vid_src_info;                                         /* 0x10240 */
    volatile reg_vid_src_reso vid_src_reso;                                         /* 0x10244 */
    volatile reg_vid_src_crop vid_src_crop;                                         /* 0x10248 */
    volatile reg_vid_in_reso vid_in_reso;                                           /* 0x1024c */
    volatile unsigned int vid_addr_h;                                               /* 0x10250 */
    volatile unsigned int vid_addr_l;                                               /* 0x10254 */
    volatile unsigned int vid_caddr_h;                                              /* 0x10258 */
    volatile unsigned int vid_caddr_l;                                              /* 0x1025c */
    volatile unsigned int vid_naddr_h;                                              /* 0x10260 */
    volatile unsigned int vid_naddr_l;                                              /* 0x10264 */
    volatile unsigned int vid_ncaddr_h;                                             /* 0x10268 */
    volatile unsigned int vid_ncaddr_l;                                             /* 0x1026c */
    volatile reg_vid_stride vid_stride;                                             /* 0x10270 */
    volatile reg_vid_2bit_stride vid_2bit_stride;                                   /* 0x10274 */
    volatile reg_vid_head_stride vid_head_stride;                                   /* 0x10278 */
    volatile unsigned int reserved_187;                                             /* 0x1027c */
    volatile reg_vid_smmu_bypass vid_smmu_bypass;                                   /* 0x10280 */
    volatile unsigned int reserved_188[3];                                          /* 0x10284~0x1028c 3 regs */
    volatile unsigned int vid_head_addr_h;                                          /* 0x10290 */
    volatile unsigned int vid_head_addr_l;                                          /* 0x10294 */
    volatile unsigned int vid_head_caddr_h;                                         /* 0x10298 */
    volatile unsigned int vid_head_caddr_l;                                         /* 0x1029c */
    volatile reg_vid_testpat_cfg vid_testpat_cfg;                                   /* 0x102a0 */
    volatile reg_vid_testpat_seed vid_testpat_seed;                                 /* 0x102a4 */
    volatile unsigned int vid_testpat_chksum_y;                                     /* 0x102a8 */
    volatile unsigned int vid_testpat_chksum_c;                                     /* 0x102ac */
    volatile unsigned int vid_work_addr_y;                                          /* 0x102b0 */
    volatile unsigned int reserved_189[3];                                          /* 0x102b4~0x102bc 3 regs */
    volatile reg_vid_tunl_ctrl vid_tunl_ctrl;                                       /* 0x102c0 */
    volatile reg_vid_tunl_crop vid_tunl_crop;                                       /* 0x102c4 */
    volatile unsigned int reserved_190[2];                                          /* 0x102c8~0x102cc 2 regs */
    volatile unsigned int vid_tunl_addr_h;                                          /* 0x102d0 */
    volatile unsigned int vid_tunl_addr_l;                                          /* 0x102d4 */
    volatile unsigned int reserved_191[2];                                          /* 2 reserved regs */
    volatile reg_vid_tunl_errsta vid_tunl_errsta;                                   /* 0x102e0 */
    volatile reg_vid_tunl_debug vid_tunl_debug;                                     /* 0x102e4 */
    volatile unsigned int reserved_192[6];                                          /* 0x102e8~0x102fc 6 regs */
    volatile unsigned int vid_l_cur_flow;                                           /* 0x10300 */
    volatile unsigned int vid_l_cur_sreq_time;                                      /* 0x10304 */
    volatile unsigned int vid_c_cur_flow;                                           /* 0x10308 */
    volatile unsigned int vid_c_cur_sreq_time;                                      /* 0x1030c */
    volatile unsigned int vid_l_last_flow;                                          /* 0x10310 */
    volatile unsigned int vid_l_last_sreq_time;                                     /* 0x10314 */
    volatile unsigned int vid_c_last_flow;                                          /* 0x10318 */
    volatile unsigned int vid_c_last_sreq_time;                                     /* 0x1031c */
    volatile unsigned int vid_l_busy_time;                                          /* 0x10320 */
    volatile unsigned int vid_l_neednordy_time;                                     /* 0x10324 */
    volatile unsigned int vid_l2_neednordy_time;                                    /* 0x10328 */
    volatile unsigned int vid_c_busy_time;                                          /* 0x1032c */
    volatile unsigned int vid_c_neednordy_time;                                     /* 0x10330 */
    volatile unsigned int vid_c2_neednordy_time;                                    /* 0x10334 */
    volatile unsigned int reserved_193[2];                                          /* 0x10338~0x1033c 2 regs */
    volatile reg_vid_dcmp_ctrl vid_dcmp_ctrl;                                       /* 0x10340 */
    volatile unsigned int vid_dcmp_l_fsize;                                         /* 0x10344 */
    volatile unsigned int reserved_194[14];                                         /* 0x10348~0x1037c 14 regs */
    volatile reg_vdp_v3r2_lineseg_dcmp_glb_info vdp_v3r2_lineseg_dcmp_glb_info;     /* 0x10380 */
    volatile reg_vdp_v3r2_lineseg_dcmp_frame_size vdp_v3r2_lineseg_dcmp_frame_size; /* 0x10384 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_adpqp_thr0;                         /* 0x10388 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_adpqp_thr1;                         /* 0x1038c */
    volatile reg_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr vdp_v3r2_lineseg_dcmp_smth_deltabits_thr; /* 0x10390 */
    volatile reg_vdp_v3r2_lineseg_dcmp_error_sta vdp_v3r2_lineseg_dcmp_error_sta;                   /* 0x10394 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_extra;                                              /* 0x10398 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_dbg_reg;                                            /* 0x1039c */
    volatile unsigned int reserved_195[8];                                              /* 0x103a0~0x103bc 8 regs */
    volatile reg_vdp_v3r2_lineseg_dcmp_glb_info_c vdp_v3r2_lineseg_dcmp_glb_info_c;     /* 0x103c0 */
    volatile reg_vdp_v3r2_lineseg_dcmp_frame_size_c vdp_v3r2_lineseg_dcmp_frame_size_c; /* 0x103c4 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_adpqp_thr0_c;                           /* 0x103c8 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_adpqp_thr1_c;                           /* 0x103cc */
    volatile reg_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr_c vdp_v3r2_lineseg_dcmp_smth_deltabits_thr_c; /* 0x103d0 */
    volatile reg_vdp_v3r2_lineseg_dcmp_error_sta_c vdp_v3r2_lineseg_dcmp_error_sta_c;                   /* 0x103d4 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_extra_c;                                        /* extra register */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_dbg_reg_c;                                              /* 0x103dc */
    volatile unsigned int reserved_196[648];                                        /* 0x103e0~0x10dfc 648 regs */
    volatile reg_gfx_read_ctrl gfx_read_ctrl;                                                 /* 0x10e00 */
    volatile reg_gfx_mac_ctrl gfx_mac_ctrl;                                                   /* 0x10e04 */
    volatile reg_gfx_out_ctrl gfx_out_ctrl;                                                   /* 0x10e08 */
    volatile unsigned int reserved_197;                                                       /* 0x10e0c */
    volatile reg_gfx_mute_alpha gfx_mute_alpha;                                               /* 0x10e10 */
    volatile reg_gfx_mute_bk gfx_mute_bk;                                                     /* 0x10e14 2 regs */
    volatile unsigned int reserved_198[2];                                                    /* 0x10e18~0x10e1c */
    volatile reg_gfx_smmu_bypass gfx_smmu_bypass;                                             /* 0x10e20 */
    volatile unsigned int reserved_199;                                                       /* 0x10e24 */
    volatile reg_gfx_1555_alpha gfx_1555_alpha;                                               /* 0x10e28 */
    volatile unsigned int reserved_200[5];                                          /* 0x10e2c~0x10e3c 5 regs */
    volatile reg_gfx_src_info gfx_src_info;                                                   /* 0x10e40 */
    volatile reg_gfx_src_reso gfx_src_reso;                                                   /* 0x10e44 */
    volatile reg_gfx_src_crop gfx_src_crop;                                                   /* 0x10e48 */
    volatile reg_gfx_ireso gfx_ireso;                                                         /* 0x10e4c */
    volatile unsigned int gfx_addr_h;                                                         /* 0x10e50 */
    volatile unsigned int gfx_addr_l;                                                         /* 0x10e54 */
    volatile unsigned int gfx_naddr_h;                                                        /* 0x10e58 */
    volatile unsigned int gfx_naddr_l;                                                        /* 0x10e5c */
    volatile reg_gfx_stride gfx_stride;                                                       /* 0x10e60 */
    volatile unsigned int reserved_201[3];                                          /* 0x10e64~0x10e6c 3 regs */
    volatile unsigned int gfx_dcmp_addr_h;                                                    /* 0x10e70 */
    volatile unsigned int gfx_dcmp_addr_l;                                                    /* 0x10e74 */
    volatile unsigned int gfx_dcmp_naddr_h;                                                   /* 0x10e78 */
    volatile unsigned int gfx_dcmp_naddr_l;                                                   /* 0x10e7c */
    volatile unsigned int reserved_202[28];                                         /* 0x10e80~0x10eec 28 regs */
    volatile unsigned int gfx_work_addr;                                                      /* 0x10ef0 */
    volatile unsigned int reserved_203[3];                                          /* 0x10ef4~0x10efc 3 regs */
    volatile reg_gfx_ckey_max gfx_ckey_max;                                                   /* 0x10f00 */
    volatile reg_gfx_ckey_min gfx_ckey_min;                                                   /* 0x10f04 */
    volatile reg_gfx_ckey_mask gfx_ckey_mask;                                                 /* 0x10f08 */
    volatile unsigned int reserved_204;                                                       /* 0x10f0c */
    volatile reg_gfx_testpat_cfg gfx_testpat_cfg;                                             /* 0x10f10 */
    volatile reg_gfx_testpat_seed gfx_testpat_seed;                                           /* 0x10f14 */
    volatile unsigned int reserved_205[2];                                          /* 0x10f18~0x10f1c 2 regs */
    volatile unsigned int gfx_dcmp_framesize0;                                                /* 0x10f20 */
    volatile unsigned int gfx_dcmp_framesize1;                                                /* 0x10f24 */
    volatile unsigned int reserved_206[2];                                          /* 0x10f28~0x10f2c 2 regs */
    volatile unsigned int gfx_cur_flow;                                                       /* 0x10f30 */
    volatile unsigned int gfx_cur_sreq_time;                                                  /* 0x10f34 */
    volatile unsigned int gfx_last_flow;                                                      /* 0x10f38 */
    volatile unsigned int gfx_last_sreq_time;                                                 /* 0x10f3c */
    volatile unsigned int gfx_busy_time;                                                      /* 0x10f40 */
    volatile unsigned int gfx_ar_neednordy_time;                                              /* 0x10f44 */
    volatile unsigned int gfx_gb_neednordy_time;                                              /* 0x10f48 */
    volatile unsigned int reserved_207;                                                       /* 0x10f4c */
    volatile reg_gfx_ld_ctrl gfx_ld_ctrl;                                                     /* 0x10f50 */
    volatile unsigned int gfx_tde_safe_dis;                                                   /* 0x10f54 */
    volatile reg_gfx_ld_smute_ctrl gfx_ld_smute_ctrl;                                         /* 0x10f58 */
    volatile reg_gfx_ld_err_sta gfx_ld_err_sta;                                               /* 0x10f5c */
    volatile unsigned int gfx_ld_debug0;                                                      /* 0x10f60 */
    volatile unsigned int gfx_ld_debug1;                                                      /* 0x10f64 */
    volatile unsigned int gfx_ld_debug2;                                                      /* 0x10f68 */
    volatile unsigned int gfx_ld_debug3;                                                      /* 0x10f6c */
    volatile unsigned int gfx_ld_debug4;                                                      /* 0x10f70 */
    volatile unsigned int gfx_ld_debug5;                                                      /* 0x10f74 */
    volatile unsigned int reserved_208[2];                                          /* 0x10f78~0x10f7c 2 regs */
    volatile reg_vdp_v3r2_line_osd_dcmp_glb_info vdp_v3r2_line_osd_dcmp_glb_info;     /* 0x10f80 */
    volatile reg_vdp_v3r2_line_osd_dcmp_frame_size vdp_v3r2_line_osd_dcmp_frame_size; /* 0x10f84 */
    volatile reg_vdp_v3r2_line_osd_dcmp_error_sta vdp_v3r2_line_osd_dcmp_error_sta;   /* 0x10f88 */
    volatile unsigned int reserved_209[541];                                        /* 0x10f8c~0x117fc 541 regs */
    volatile reg_wbc_ctrl wbc_ctrl;                                                           /* 0x11800 */
    volatile reg_wbc_mac_ctrl wbc_mac_ctrl;                                                   /* 0x11804 */
    volatile unsigned int reserved_210[3];                                          /* 0x11808~0x11810 3 regs */
    volatile reg_wbc_smmu_bypass wbc_smmu_bypass;                                             /* 0x11814 */
    volatile unsigned int reserved_211[2];                                          /* 0x11818~0x1181c 2 regs */
    volatile reg_wbc_lowdlyctrl wbc_lowdlyctrl;                                               /* 0x11820 */
    volatile unsigned int wbc_tunladdr_h;                                                     /* 0x11824 */
    volatile unsigned int wbc_tunladdr_l;                                                     /* 0x11828 */
    volatile reg_wbc_lowdlysta wbc_lowdlysta;                                                 /* 0x1182c */
    volatile unsigned int reserved_212[8];                                          /* 0x11830~0x1184c 8 regs */
    volatile unsigned int wbc_yaddr_h;                                                        /* 0x11850 */
    volatile unsigned int wbc_yaddr_l;                                                        /* 0x11854 */
    volatile unsigned int wbc_caddr_h;                                                        /* 0x11858 */
    volatile unsigned int wbc_caddr_l;                                                        /* 0x1185c */
    volatile reg_wbc_ystride wbc_ystride;                                                     /* 0x11860 */
    volatile reg_wbc_cstride wbc_cstride;                                                     /* 0x11864 */
    volatile unsigned int reserved_213[2];                                          /* 0x11868~0x1186c 2 regs */
    volatile unsigned int wbc_ynaddr_h;                                                       /* 0x11870 */
    volatile unsigned int wbc_ynaddr_l;                                                       /* 0x11874 */
    volatile unsigned int wbc_cnaddr_h;                                                       /* 0x11878 */
    volatile unsigned int wbc_cnaddr_l;                                                       /* 0x1187c */
    volatile reg_wbc_ynstride wbc_ynstride;                                                   /* 0x11880 */
    volatile reg_wbc_cnstride wbc_cnstride;                                                   /* 0x11884 */
    volatile unsigned int reserved_214[10];                                         /* 0x11888~0x118ac 10 regs */
    volatile reg_wbc_sta wbc_sta;                                                             /* 0x118b0 */
    volatile reg_wbc_line_num wbc_line_num;                                                   /* 0x118b4 */
    volatile reg_wbc_cap_reso wbc_cap_reso;                                                   /* 0x118b8 */
    volatile unsigned int wbc_cap_info;                                                       /* 0x118bc */
    volatile unsigned int reserved_215[16];                                         /* 0x118c0~0x118fc 16 regs */
    volatile reg_vdp_v3r2_lineseg_cmp_glb_info vdp_v3r2_lineseg_cmp_glb_info;                 /* 0x11900 */
    volatile reg_vdp_v3r2_lineseg_cmp_frame_size vdp_v3r2_lineseg_cmp_frame_size;             /* 0x11904 */
    volatile reg_vdp_v3r2_lineseg_cmp_rc_cfg0 vdp_v3r2_lineseg_cmp_rc_cfg0;                   /* 0x11908 */
    volatile reg_vdp_v3r2_lineseg_cmp_rc_cfg1 vdp_v3r2_lineseg_cmp_rc_cfg1;                   /* 0x1190c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg2;                                       /* 0x11910 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg3;                                       /* 0x11914 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg4;                                       /* 0x11918 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg5;                                       /* 0x1191c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg6;                                       /* 0x11920 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg7;                                       /* 0x11924 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg8;                                       /* 0x11928 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg9;                                       /* 0x1192c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg10;                                      /* 0x11930 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg11;                                      /* 0x11934 */
    volatile reg_vdp_v3r2_lineseg_cmp_rc_cfg12 vdp_v3r2_lineseg_cmp_rc_cfg12;                 /* 0x11938 */
    volatile reg_vdp_v3r2_lineseg_cmp_rc_cfg13 vdp_v3r2_lineseg_cmp_rc_cfg13;                 /* 0x1193c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg14;                                      /* 0x11940 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg15;                                      /* 0x11944 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_adpqp_thr0;                                    /* 0x11948 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_adpqp_thr1;                                    /* 0x1194c */
    volatile reg_vdp_v3r2_lineseg_cmp_rc_cfg16 vdp_v3r2_lineseg_cmp_rc_cfg16;                 /* 0x11950 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_glb_cfg;                                       /* 0x11954 */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART47_H_ */
