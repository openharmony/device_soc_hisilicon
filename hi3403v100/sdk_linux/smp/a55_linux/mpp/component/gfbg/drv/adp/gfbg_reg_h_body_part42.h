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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART42_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART42_H_

    volatile u_dither_sed_w1 dither_sed_w1;                                       /* 0xc2a0 */
    volatile u_dither_sed_y2 dither_sed_y2;                                       /* 0xc2a4 */
    volatile u_dither_sed_u2 dither_sed_u2;                                       /* 0xc2a8 */
    volatile u_dither_sed_v2 dither_sed_v2;                                       /* 0xc2ac */
    volatile u_dither_sed_w2 dither_sed_w2;                                       /* 0xc2b0 */
    volatile u_dither_sed_y3 dither_sed_y3;                                       /* 0xc2b4 */
    volatile u_dither_sed_u3 dither_sed_u3;                                       /* 0xc2b8 */
    volatile u_dither_sed_v3 dither_sed_v3;                                       /* 0xc2bc */
    volatile u_dither_sed_w3 dither_sed_w3;                                       /* 0xc2c0 */
    volatile u_dither_thr dither_thr;                                             /* 0xc2c4 */
    volatile unsigned int reserved_114[14];                                       /* 14:0xc2c8~0xc2fc */
    volatile u_wd_zme_hinfo wd_zme_hinfo;                                         /* 0xc300 */
    volatile u_wd_zme_hsp wd_zme_hsp;                                             /* 0xc304 */
    volatile u_wd_zme_hloffset wd_zme_hloffset;                                   /* 0xc308 */
    volatile u_wd_zme_hcoffset wd_zme_hcoffset;                                   /* 0xc30c */
    volatile unsigned int reserved_115[5];                                        /* 5:0xc310~0xc320 */
    volatile u_wd_zme_hcoef_ren wd_zme_hcoef_ren;                                 /* 0xc324 */
    volatile u_wd_zme_hcoef_rdata wd_zme_hcoef_rdata;                             /* 0xc328 */
    volatile u_wd_zme_hdraw wd_zme_hdraw;                                         /* 0xc32c */
    volatile u_wd_zme_hratio wd_zme_hratio;                                       /* 0xc330 */
    volatile unsigned int reserved_116[51];                                       /* 51:0xc334~0xc3fc */
    volatile u_wd_zme_vinfo wd_zme_vinfo;                                         /* 0xc400 */
    volatile u_wd_zme_vsp wd_zme_vsp;                                             /* 0xc404 */
    volatile u_wd_zme_voffset wd_zme_voffset;                                     /* 0xc408 */
    volatile u_wd_zme_vboffset wd_zme_vboffset;                                   /* 0xc40c */
    volatile unsigned int reserved_117[5];                                        /* 5:0xc410~0xc420 */
    volatile u_wd_zme_vcoef_ren wd_zme_vcoef_ren;                                 /* 0xc424 */
    volatile u_wd_zme_vcoef_rdata wd_zme_vcoef_rdata;                             /* 0xc428 */
    volatile u_wd_zme_vdraw wd_zme_vdraw;                                         /* 0xc42c */
    volatile u_wd_zme_vratio wd_zme_vratio;                                       /* 0xc430 */
    volatile unsigned int reserved_118[755];                                      /* 755:0xc434~0xcffc */
    volatile u_dhd0_ctrl dhd0_ctrl;                                               /* 0xd000 */
    volatile u_dhd0_vsync1 dhd0_vsync1;                                           /* 0xd004 */
    volatile u_dhd0_vsync2 dhd0_vsync2;                                           /* 0xd008 */
    volatile u_dhd0_hsync1 dhd0_hsync1;                                           /* 0xd00c */
    volatile u_dhd0_hsync2 dhd0_hsync2;                                           /* 0xd010 */
    volatile u_dhd0_vplus1 dhd0_vplus1;                                           /* 0xd014 */
    volatile u_dhd0_vplus2 dhd0_vplus2;                                           /* 0xd018 */
    volatile u_dhd0_pwr dhd0_pwr;                                                 /* 0xd01c */
    volatile u_dhd0_vtthd3 dhd0_vtthd3;                                           /* 0xd020 */
    volatile u_dhd0_vtthd dhd0_vtthd;                                             /* 0xd024 */
    volatile u_dhd0_parathd dhd0_parathd;                                         /* 0xd028 */
    volatile u_dhd0_precharge_thd dhd0_precharge_thd;                             /* 0xd02c */
    volatile u_dhd0_start_pos dhd0_start_pos;                                     /* 0xd030 */
    volatile u_dhd0_start_pos1 dhd0_start_pos1;                                   /* 0xd034 */
    volatile u_dhd0_paraup dhd0_paraup;                                           /* 0xd038 */
    volatile u_dhd0_sync_inv dhd0_sync_inv;                                       /* 0xd03c */
    volatile u_dhd0_clk_dv_ctrl dhd0_clk_dv_ctrl;                                 /* 0xd040 */
    volatile u_dhd0_rgb_fix_ctrl dhd0_rgb_fix_ctrl;                               /* 0xd044 */
    volatile u_dhd0_lockcfg dhd0_lockcfg;                                         /* 0xd048 */
    volatile unsigned int dhd0_cap_frm_cnt;                                       /* 0xd04c */
    volatile unsigned int dhd0_vdp_frm_cnt;                                       /* 0xd050 */
    volatile unsigned int dhd0_vsync_cap_vdp_cnt;                                 /* 0xd054 */
    volatile unsigned int dhd0_intf_chksum_y;                                     /* 0xd058 */
    volatile unsigned int dhd0_intf_chksum_u;                                     /* 0xd05c */
    volatile unsigned int dhd0_intf_chksum_v;                                     /* 0xd060 */
    volatile unsigned int dhd0_intf1_chksum_y;                                    /* 0xd064 */
    volatile unsigned int dhd0_intf1_chksum_u;                                    /* 0xd068 */
    volatile unsigned int dhd0_intf1_chksum_v;                                    /* 0xd06c */
    volatile u_dhd0_intf_chksum_high1 dhd0_intf_chksum_high1;                     /* 0xd070 */
    volatile u_dhd0_intf_chksum_high2 dhd0_intf_chksum_high2;                     /* 0xd074 */
    volatile unsigned int reserved_119[3];                                        /* 3:0xd078~0xd080 */
    volatile unsigned int dhd0_afifo_pre_thd;                                     /* 0xd084 */
    volatile u_dhd0_state dhd0_state;                                             /* 0xd088 */
    volatile u_dhd0_uf_state dhd0_uf_state;                                       /* 0xd08c */
    volatile u_vo_mux vo_mux;                                                     /* 0xd090 */
    volatile u_vo_mux_sync vo_mux_sync;                                           /* 0xd094 */
    volatile u_vo_mux_data vo_mux_data;                                           /* 0xd098 */
    volatile unsigned int reserved_120;                                           /* 0xd09c */
    volatile u_dhd0_vsync_te_state dhd0_vsync_te_state;                           /* 0xd0a0 */
    volatile u_dhd0_vsync_te_state1 dhd0_vsync_te_state1;                         /* 0xd0a4 */
    volatile unsigned int reserved_121[6];                                        /* 6:0xd0a8~0xd0bc */
    volatile u_dhd0_ccdoimgmod dhd0_ccdoimgmod;                                   /* 0xd0c0 */
    volatile u_dhd0_ccdoposmskh dhd0_ccdoposmskh;                                 /* 0xd0c4 */
    volatile u_dhd0_ccdoposmskl dhd0_ccdoposmskl;                                 /* 0xd0c8 */
    volatile unsigned int reserved_122;                                           /* 0xd0cc */
    volatile u_dhd0_dacdet1 dhd0_dacdet1;                                         /* 0xd0d0 */
    volatile u_dhd0_dacdet2 dhd0_dacdet2;                                         /* 0xd0d4 */
    volatile unsigned int reserved_123[2];                                        /* 2: reserved regs */
    volatile u_dhd0_ccd_info1 dhd0_ccd_info1;                                     /* 0xd0e0 */
    volatile u_dhd0_ccd_info2 dhd0_ccd_info2;                                     /* 0xd0e4 */
    volatile u_dhd0_ccd_info3 dhd0_ccd_info3;                                     /* 0xd0e8 */
    volatile unsigned int reserved_124[5];                                        /* 5:0xd0ec~0xd0fc */
    volatile u_intf_hdmi_ctrl intf_hdmi_ctrl;                                     /* 0xd100 */
    volatile u_intf_hdmi_upd intf_hdmi_upd;                                       /* 0xd104 */
    volatile u_intf_hdmi_sync_inv intf_hdmi_sync_inv;                             /* 0xd108 */
    volatile unsigned int reserved_125;                                           /* 0xd10c */
    volatile unsigned int hdmi_intf_chksum_y;                                     /* 0xd110 */
    volatile unsigned int hdmi_intf_chksum_u;                                     /* 0xd114 */
    volatile unsigned int hdmi_intf_chksum_v;                                     /* 0xd118 */
    volatile u_hdmi_intf_chksum_high hdmi_intf_chksum_high;                       /* 0xd11c */
    volatile unsigned int hdmi_intf1_chksum_y;                                    /* 0xd120 */
    volatile unsigned int hdmi_intf1_chksum_u;                                    /* 0xd124 */
    volatile unsigned int hdmi_intf1_chksum_v;                                    /* 0xd128 */
    volatile u_hdmi_intf1_chksum_high hdmi_intf1_chksum_high;                     /* 0xd12c */
    volatile unsigned int reserved_126[8];                                        /* 8:0xd130~0xd14c */
    volatile u_hdmi_hfir_coef0 hdmi_hfir_coef0;                                   /* 0xd150 */
    volatile u_hdmi_hfir_coef1 hdmi_hfir_coef1;                                   /* 0xd154 */
    volatile u_hdmi_hfir_coef2 hdmi_hfir_coef2;                                   /* 0xd158 */
    volatile u_hdmi_hfir_coef3 hdmi_hfir_coef3;                                   /* 0xd15c */
    volatile u_hdmi_csc_idc hdmi_csc_idc;                                         /* 0xd160 */
    volatile u_hdmi_csc_odc hdmi_csc_odc;                                         /* 0xd164 */
    volatile u_hdmi_csc_iodc hdmi_csc_iodc;                                       /* 0xd168 */
    volatile u_hdmi_csc_p0 hdmi_csc_p0;                                           /* 0xd16c */
    volatile u_hdmi_csc_p1 hdmi_csc_p1;                                           /* 0xd170 */
    volatile u_hdmi_csc_p2 hdmi_csc_p2;                                           /* 0xd174 */
    volatile u_hdmi_csc_p3 hdmi_csc_p3;                                           /* 0xd178 */
    volatile u_hdmi_csc_p4 hdmi_csc_p4;                                           /* 0xd17c */
    volatile u_intf_mipi_ctrl intf_mipi_ctrl;                                     /* 0xd180 */
    volatile u_intf_mipi_upd intf_mipi_upd;                                       /* 0xd184 */
    volatile u_intf_mipi_sync_inv intf_mipi_sync_inv;                             /* 0xd188 */
    volatile unsigned int reserved_127;                                           /* 0xd18c */
    volatile unsigned int mipi_intf_chksum_y;                                     /* 0xd190 */
    volatile unsigned int mipi_intf_chksum_u;                                     /* 0xd194 */
    volatile unsigned int mipi_intf_chksum_v;                                     /* 0xd198 */
    volatile u_mipi_intf_chksum_high mipi_intf_chksum_high;                       /* 0xd19c */
    volatile unsigned int mipi_intf1_chksum_y;                                    /* 0xd1a0 */
    volatile unsigned int mipi_intf1_chksum_u;                                    /* 0xd1a4 */
    volatile unsigned int mipi_intf1_chksum_v;                                    /* 0xd1a8 */
    volatile u_mipi_intf1_chksum_high mipi_intf1_chksum_high;                     /* 0xd1ac */
    volatile unsigned int reserved_128[8];                                        /* 8:0xd1b0~0xd1cc */
    volatile u_mipi_hfir_coef0 mipi_hfir_coef0;                                   /* 0xd1d0 */
    volatile u_mipi_hfir_coef1 mipi_hfir_coef1;                                   /* 0xd1d4 */
    volatile u_mipi_hfir_coef2 mipi_hfir_coef2;                                   /* offset 0xd1d0 + 0x08 */
    volatile u_mipi_hfir_coef3 mipi_hfir_coef3;                                   /* 0xd1dc */
    volatile unsigned int reserved_129[8];                                        /* 8:0xd1e0~0xd1fc */
    volatile u_intf_bt_ctrl intf_bt_ctrl;                                         /* 0xd200 */
    volatile u_intf_bt_upd intf_bt_upd;                                           /* 0xd204 */
    volatile u_intf_bt_sync_inv intf_bt_sync_inv;                                 /* 0xd208 */
    volatile unsigned int reserved_130;                                           /* 0xd20c */
    volatile u_bt_clip0_l bt_clip0_l;                                             /* 0xd210 */
    volatile u_bt_clip0_h bt_clip0_h;                                             /* 0xd214 */
    volatile unsigned int reserved_131[26];                                       /* 26:0xd218~0xd27c */
    volatile u_bt_dither_ctrl bt_dither_ctrl;                                     /* 0xd280 */
    volatile u_bt_dither_sed_y0 bt_dither_sed_y0;                                 /* 0xd284 */
    volatile u_bt_dither_sed_u0 bt_dither_sed_u0;                                 /* 0xd288 */
    volatile u_bt_dither_sed_v0 bt_dither_sed_v0;                                 /* 0xd28c */
    volatile u_bt_dither_sed_w0 bt_dither_sed_w0;                                 /* 0xd290 */
    volatile u_bt_dither_sed_y1 bt_dither_sed_y1;                                 /* 0xd294 */
    volatile u_bt_dither_sed_u1 bt_dither_sed_u1;                                 /* 0xd298 */
    volatile u_bt_dither_sed_v1 bt_dither_sed_v1;                                 /* 0xd29c */
    volatile u_bt_dither_sed_w1 bt_dither_sed_w1;                                 /* 0xd2a0 */
    volatile u_bt_dither_sed_y2 bt_dither_sed_y2;                                 /* 0xd2a4 */
    volatile u_bt_dither_sed_u2 bt_dither_sed_u2;                                 /* 0xd2a8 */
    volatile u_bt_dither_sed_v2 bt_dither_sed_v2;                                 /* 0xd2ac */
    volatile u_bt_dither_sed_w2 bt_dither_sed_w2;                                 /* 0xd2b0 */
    volatile u_bt_dither_sed_y3 bt_dither_sed_y3;                                 /* 0xd2b4 */
    volatile u_bt_dither_sed_u3 bt_dither_sed_u3;                                 /* 0xd2b8 */
    volatile u_bt_dither_sed_v3 bt_dither_sed_v3;                                 /* 0xd2bc */
    volatile u_bt_dither_sed_w3 bt_dither_sed_w3;                                 /* 0xd2c0 */
    volatile u_bt_dither_thr bt_dither_thr;                                       /* 0xd2c4 */
    volatile unsigned int reserved_132[10];                                       /* 10:0xd2c8~0xd2ec */
    volatile unsigned int bt_intf_chksum_y;                                       /* 0xd2f0 */
    volatile unsigned int bt_intf_chksum_u;                                       /* 0xd2f4 */
    volatile unsigned int bt_intf_chksum_v;                                       /* 0xd2f8 */
    volatile unsigned int reserved_133;                                           /* 0xd2fc */
    volatile u_intf_lcd_ctrl intf_lcd_ctrl;                                       /* 0xd300 */
    volatile u_intf_lcd_upd intf_lcd_upd;                                         /* 0xd304 */
    volatile u_intf_lcd_sync_inv intf_lcd_sync_inv;                               /* 0xd308 */
    volatile unsigned int reserved_134[29];                                       /* 29:0xd30c~0xd37c */
    volatile u_lcd_dither_ctrl lcd_dither_ctrl;                                   /* 0xd380 */
    volatile u_lcd_dither_sed_y0 lcd_dither_sed_y0;                               /* 0xd384 */
    volatile u_lcd_dither_sed_u0 lcd_dither_sed_u0;                               /* 0xd388 */
    volatile u_lcd_dither_sed_v0 lcd_dither_sed_v0;                               /* 0xd38c */
    volatile u_lcd_dither_sed_w0 lcd_dither_sed_w0;                               /* 0xd390 */
    volatile u_lcd_dither_sed_y1 lcd_dither_sed_y1;                               /* 0xd394 */
    volatile u_lcd_dither_sed_u1 lcd_dither_sed_u1;                               /* 0xd398 */
    volatile u_lcd_dither_sed_v1 lcd_dither_sed_v1;                               /* 0xd39c */
    volatile u_lcd_dither_sed_w1 lcd_dither_sed_w1;                               /* 0xd3a0 */
    volatile u_lcd_dither_sed_y2 lcd_dither_sed_y2;                               /* 0xd3a4 */
    volatile u_lcd_dither_sed_u2 lcd_dither_sed_u2;                               /* 0xd3a8 */
    volatile u_lcd_dither_sed_v2 lcd_dither_sed_v2;                               /* 0xd3ac */
    volatile u_lcd_dither_sed_w2 lcd_dither_sed_w2;                               /* 0xd3b0 */
    volatile u_lcd_dither_sed_y3 lcd_dither_sed_y3;                               /* 0xd3b4 */
    volatile u_lcd_dither_sed_u3 lcd_dither_sed_u3;                               /* 0xd3b8 */
    volatile u_lcd_dither_sed_v3 lcd_dither_sed_v3;                               /* 0xd3bc */
    volatile u_lcd_dither_sed_w3 lcd_dither_sed_w3;                               /* 0xd3c0 */
    volatile u_lcd_dither_thr lcd_dither_thr;                                     /* 0xd3c4 */
    volatile unsigned int reserved_135[10];                                       /* 10:0xd3c8~0xd3ec */
    volatile unsigned int lcd_intf_chksum_y;                                      /* 0xd3f0 */
    volatile unsigned int lcd_intf_chksum_u;                                      /* 0xd3f4 */
    volatile unsigned int lcd_intf_chksum_v;                                      /* 0xd3f8 */
    volatile unsigned int reserved_136;                                           /* 0xd3fc */
    volatile u_intf_hdmi1_ctrl intf_hdmi1_ctrl;                                   /* 0xd400 */
    volatile u_intf_hdmi1_upd intf_hdmi1_upd;                                     /* 0xd404 */
    volatile u_intf_hdmi1_sync_inv intf_hdmi1_sync_inv;                           /* 0xd408 */
    volatile unsigned int reserved_137;                                           /* 0xd40c */
    volatile unsigned int hdmi1_intf_chksum_y;                                    /* 0xd410 */
    volatile unsigned int hdmi1_intf_chksum_u;                                    /* 0xd414 */
    volatile unsigned int hdmi1_intf_chksum_v;                                    /* 0xd418 */
    volatile u_hdmi1_intf_chksum_high hdmi1_intf_chksum_high;                     /* 0xd41c */
    volatile unsigned int hdmi1_intf1_chksum_y;                                   /* 0xd420 */
    volatile unsigned int hdmi1_intf1_chksum_u;                                   /* 0xd424 */
    volatile unsigned int hdmi1_intf1_chksum_v;                                   /* 0xd428 */
    volatile u_hdmi1_intf1_chksum_high hdmi1_intf1_chksum_high;                   /* 0xd42c */
    volatile unsigned int reserved_138[8];                                        /* 8:0xd430~0xd44c */
    volatile u_hdmi1_hfir_coef0 hdmi1_hfir_coef0;                                 /* 0xd450 */
    volatile u_hdmi1_hfir_coef1 hdmi1_hfir_coef1;                                 /* 0xd454 */
    volatile u_hdmi1_hfir_coef2 hdmi1_hfir_coef2;                                 /* 0xd458 */
    volatile u_hdmi1_hfir_coef3 hdmi1_hfir_coef3;                                 /* 0xd45c */
    volatile unsigned int reserved_139[40];                                       /* 40:0xd460~0xd4fc */
    volatile u_intf_vga_ctrl intf_vga_ctrl;                                       /* 0xd500 */
    volatile u_intf_vga_upd intf_vga_upd;                                         /* 0xd504 */
    volatile u_intf_vga_sync_inv intf_vga_sync_inv;                               /* 0xd508 */
    volatile unsigned int reserved_140[5];                                        /* 5:0xd50c~0xd51c */
    volatile u_vga_csc_idc vga_csc_idc;                                           /* 0xd520 */
    volatile u_vga_csc_odc vga_csc_odc;                                           /* 0xd524 */
    volatile u_vga_csc_iodc vga_csc_iodc;                                         /* 0xd528 */
    volatile u_vga_csc_p0 vga_csc_p0;                                             /* 0xd52c */
    volatile u_vga_csc_p1 vga_csc_p1;                                             /* 0xd530 */
    volatile u_vga_csc_p2 vga_csc_p2;                                             /* 0xd534 */
    volatile u_vga_csc_p3 vga_csc_p3;                                             /* 0xd538 */
    volatile u_vga_csc_p4 vga_csc_p4;                                             /* 0xd53c */
    volatile u_vga_hspcfg0 vga_hspcfg0;                                           /* 0xd540 */
    volatile u_vga_hspcfg1 vga_hspcfg1;                                           /* 0xd544 */
    volatile unsigned int reserved_141[3];                                        /* 3:0xd548~0xd550 */
    volatile u_vga_hspcfg5 vga_hspcfg5;                                           /* 0xd554 */
    volatile u_vga_hspcfg6 vga_hspcfg6;                                           /* 0xd558 */
    volatile u_vga_hspcfg7 vga_hspcfg7;                                           /* 0xd55c */
    volatile u_vga_hspcfg8 vga_hspcfg8;                                           /* 0xd560 */
    volatile unsigned int reserved_142[3];                                        /* 3:0xd564~0xd56c */
    volatile u_vga_hspcfg12 vga_hspcfg12;                                         /* 0xd570 */
    volatile u_vga_hspcfg13 vga_hspcfg13;                                         /* 0xd574 */
    volatile u_vga_hspcfg14 vga_hspcfg14;                                         /* 0xd578 */
    volatile u_vga_hspcfg15 vga_hspcfg15;                                         /* 0xd57c */
    volatile unsigned int reserved_143[28];                                       /* 28:0xd580~0xd5ec */
    volatile unsigned int vga_intf_chksum_y;                                      /* 0xd5f0 */
    volatile unsigned int vga_intf_chksum_u;                                      /* 0xd5f4 */
    volatile unsigned int vga_intf_chksum_v;                                      /* 0xd5f8 */
    volatile unsigned int reserved_144;                                           /* 0xd5fc */
    volatile u_intf_date_ctrl intf_date_ctrl;                                     /* 0xd600 */
    volatile u_intf_date_upd intf_date_upd;                                       /* 0xd604 */
    volatile u_intf_date_sync_inv intf_date_sync_inv;                             /* 0xd608 */
    volatile unsigned int reserved_145;                                           /* 0xd60c */
    volatile u_date_clip0_l date_clip0_l;                                         /* 0xd610 */
    volatile u_date_clip0_h date_clip0_h;                                         /* 0xd614 */
    volatile unsigned int reserved_146[58];                                       /* 58:0xd618~0xd6fc */
    volatile u_intf0_dither_ctrl intf0_dither_ctrl;                               /* 0xd700 */
    volatile u_intf0_dither_sed_y0 intf0_dither_sed_y0;                           /* 0xd704 */
    volatile u_intf0_dither_sed_u0 intf0_dither_sed_u0;                           /* 0xd708 */
    volatile u_intf0_dither_sed_v0 intf0_dither_sed_v0;                           /* 0xd70c */
    volatile u_intf0_dither_sed_w0 intf0_dither_sed_w0;                           /* 0xd710 */
    volatile u_intf0_dither_sed_y1 intf0_dither_sed_y1;                           /* 0xd714 */
    volatile u_intf0_dither_sed_u1 intf0_dither_sed_u1;                           /* 0xd718 */
    volatile u_intf0_dither_sed_v1 intf0_dither_sed_v1;                           /* 0xd71c */
    volatile u_intf0_dither_sed_w1 intf0_dither_sed_w1;                           /* 0xd720 */
    volatile u_intf0_dither_sed_y2 intf0_dither_sed_y2;                           /* 0xd724 */
    volatile u_intf0_dither_sed_u2 intf0_dither_sed_u2;                           /* 0xd728 */
    volatile u_intf0_dither_sed_v2 intf0_dither_sed_v2;                           /* 0xd72c */
    volatile u_intf0_dither_sed_w2 intf0_dither_sed_w2;                           /* 0xd730 */
    volatile u_intf0_dither_sed_y3 intf0_dither_sed_y3;                           /* 0xd734 */
    volatile u_intf0_dither_sed_u3 intf0_dither_sed_u3;                           /* 0xd738 */
    volatile u_intf0_dither_sed_v3 intf0_dither_sed_v3;                           /* 0xd73c */
    volatile u_intf0_dither_sed_w3 intf0_dither_sed_w3;                           /* 0xd740 */
    volatile u_intf0_dither_thr intf0_dither_thr;                                 /* 0xd744 */
    volatile unsigned int reserved_147[558];                                      /* 558:0xd748~0xdffc */
    volatile u_dhd1_ctrl dhd1_ctrl;                                               /* 0xe000 */
    volatile u_dhd1_vsync1 dhd1_vsync1;                                           /* 0xe004 */
    volatile u_dhd1_vsync2 dhd1_vsync2;                                           /* 0xe008 */
    volatile u_dhd1_hsync1 dhd1_hsync1;                                           /* 0xe00c */
    volatile u_dhd1_hsync2 dhd1_hsync2;                                           /* 0xe010 */
    volatile u_dhd1_vplus1 dhd1_vplus1;                                           /* 0xe014 */
    volatile u_dhd1_vplus2 dhd1_vplus2;                                           /* 0xe018 */
    volatile u_dhd1_pwr dhd1_pwr;                                                 /* 0xe01c */
    volatile u_dhd1_vtthd3 dhd1_vtthd3;                                           /* 0xe020 */
    volatile u_dhd1_vtthd dhd1_vtthd;                                             /* 0xe024 */
    volatile u_dhd1_parathd dhd1_parathd;                                         /* 0xe028 */
    volatile u_dhd1_precharge_thd dhd1_precharge_thd;                             /* 0xe02c */
    volatile u_dhd1_start_pos dhd1_start_pos;                                     /* 0xe030 */
    volatile u_dhd1_start_pos1 dhd1_start_pos1;                                   /* 0xe034 */
    volatile u_dhd1_paraup dhd1_paraup;                                           /* 0xe038 */
    volatile u_dhd1_sync_inv dhd1_sync_inv;                                       /* 0xe03c */
    volatile u_dhd1_clk_dv_ctrl dhd1_clk_dv_ctrl;                                 /* 0xe040 */
    volatile u_dhd1_rgb_fix_ctrl dhd1_rgb_fix_ctrl;                               /* 0xe044 */
    volatile u_dhd1_lockcfg dhd1_lockcfg;                                         /* 0xe048 */
    volatile unsigned int dhd1_cap_frm_cnt;                                       /* 0xe04c */
    volatile unsigned int dhd1_vdp_frm_cnt;                                       /* 0xe050 */
    volatile unsigned int dhd1_vsync_cap_vdp_cnt;                                 /* 0xe054 */
    volatile unsigned int dhd1_intf_chksum_y;                                     /* 0xe058 */
    volatile unsigned int dhd1_intf_chksum_u;                                     /* 0xe05c */
    volatile unsigned int dhd1_intf_chksum_v;                                     /* 0xe060 */
    volatile unsigned int dhd1_intf1_chksum_y;                                    /* 0xe064 */
    volatile unsigned int dhd1_intf1_chksum_u;                                    /* 0xe068 */
    volatile unsigned int dhd1_intf1_chksum_v;                                    /* 0xe06c */
    volatile u_dhd1_intf_chksum_high1 dhd1_intf_chksum_high1;                     /* 0xe070 */
    volatile u_dhd1_intf_chksum_high2 dhd1_intf_chksum_high2;                     /* 0xe074 */
    volatile unsigned int reserved_148[3];                                        /* 3:0xe078~0xe080 */
    volatile unsigned int dhd1_afifo_pre_thd;                                     /* 0xe084 */
    volatile u_dhd1_state dhd1_state;                                             /* 0xe088 */
    volatile u_dhd1_uf_state dhd1_uf_state;                                       /* 0xe08c */
    volatile unsigned int reserved_149[4];                                        /* 4:0xe090~0xe09c */
    volatile u_dhd1_vsync_te_state dhd1_vsync_te_state;                           /* 0xe0a0 */
    volatile u_dhd1_vsync_te_state1 dhd1_vsync_te_state1;                         /* 0xe0a4 */
    volatile unsigned int reserved_150[406];                                      /* 406:0xe0a8~0xe6fc */
    volatile u_intf1_dither_ctrl intf1_dither_ctrl;                               /* 0xe700 */
    volatile u_intf1_dither_sed_y0 intf1_dither_sed_y0;                           /* 0xe704 */
    volatile u_intf1_dither_sed_u0 intf1_dither_sed_u0;                           /* 0xe708 */
    volatile u_intf1_dither_sed_v0 intf1_dither_sed_v0;                           /* 0xe70c */
    volatile u_intf1_dither_sed_w0 intf1_dither_sed_w0;                           /* 0xe710 */
    volatile u_intf1_dither_sed_y1 intf1_dither_sed_y1;                           /* 0xe714 */
    volatile u_intf1_dither_sed_u1 intf1_dither_sed_u1;                           /* 0xe718 */
    volatile u_intf1_dither_sed_v1 intf1_dither_sed_v1;                           /* 0xe71c */
    volatile u_intf1_dither_sed_w1 intf1_dither_sed_w1;                           /* 0xe720 */
    volatile u_intf1_dither_sed_y2 intf1_dither_sed_y2;                           /* 0xe724 */
    volatile u_intf1_dither_sed_u2 intf1_dither_sed_u2;                           /* 0xe728 */
    volatile u_intf1_dither_sed_v2 intf1_dither_sed_v2;                           /* 0xe72c */
    volatile u_intf1_dither_sed_w2 intf1_dither_sed_w2;                           /* 0xe730 */
    volatile u_intf1_dither_sed_y3 intf1_dither_sed_y3;                           /* 0xe734 */
    volatile u_intf1_dither_sed_u3 intf1_dither_sed_u3;                           /* 0xe738 */
    volatile u_intf1_dither_sed_v3 intf1_dither_sed_v3;                           /* 0xe73c */
    volatile u_intf1_dither_sed_w3 intf1_dither_sed_w3;                           /* 0xe740 */
    volatile u_intf1_dither_thr intf1_dither_thr;                                 /* 0xe744 */
    volatile unsigned int reserved_151[558];                                      /* 558:0xe748~0xeffc */
    volatile u_dhd2_ctrl dhd2_ctrl;                                               /* 0xf000 */
    volatile u_dhd2_vsync1 dhd2_vsync1;                                           /* 0xf004 */
    volatile u_dhd2_vsync2 dhd2_vsync2;                                           /* 0xf008 */
    volatile u_dhd2_hsync1 dhd2_hsync1;                                           /* 0xf00c */
    volatile u_dhd2_hsync2 dhd2_hsync2;                                           /* 0xf010 */
    volatile u_dhd2_vplus1 dhd2_vplus1;                                           /* 0xf014 */
    volatile u_dhd2_vplus2 dhd2_vplus2;                                           /* 0xf018 */
    volatile u_dhd2_pwr dhd2_pwr;                                                 /* 0xf01c */
    volatile u_dhd2_vtthd3 dhd2_vtthd3;                                           /* 0xf020 */
    volatile u_dhd2_vtthd dhd2_vtthd;                                             /* 0xf024 */
    volatile u_dhd2_parathd dhd2_parathd;                                         /* 0xf028 */
    volatile u_dhd2_precharge_thd dhd2_precharge_thd;                             /* 0xf02c */
    volatile u_dhd2_start_pos dhd2_start_pos;                                     /* 0xf030 */
    volatile u_dhd2_start_pos1 dhd2_start_pos1;                                   /* 0xf034 */
    volatile u_dhd2_paraup dhd2_paraup;                                           /* 0xf038 */
    volatile u_dhd2_sync_inv dhd2_sync_inv;                                       /* 0xf03c */
    volatile u_dhd2_clk_dv_ctrl dhd2_clk_dv_ctrl;                                 /* 0xf040 */
    volatile u_dhd2_rgb_fix_ctrl dhd2_rgb_fix_ctrl;                               /* 0xf044 */
    volatile u_dhd2_lockcfg dhd2_lockcfg;                                         /* 0xf048 */
    volatile unsigned int dhd2_cap_frm_cnt;                                       /* 0xf04c */
    volatile unsigned int dhd2_vdp_frm_cnt;                                       /* 0xf050 */
    volatile unsigned int dhd2_vsync_cap_vdp_cnt;                                 /* 0xf054 */
    volatile unsigned int dhd2_intf_chksum_y;                                     /* 0xf058 */
    volatile unsigned int dhd2_intf_chksum_u;                                     /* 0xf05c */
    volatile unsigned int dhd2_intf_chksum_v;                                     /* 0xf060 */
    volatile unsigned int dhd2_intf1_chksum_y;                                    /* 0xf064 */
    volatile unsigned int dhd2_intf1_chksum_u;                                    /* 0xf068 */
    volatile unsigned int dhd2_intf1_chksum_v;                                    /* 0xf06c */
    volatile u_dhd2_intf_chksum_high1 dhd2_intf_chksum_high1;                     /* 0xf070 */
    volatile u_dhd2_intf_chksum_high2 dhd2_intf_chksum_high2;                     /* 0xf074 */
    volatile unsigned int reserved_152[3];                                        /* 3:0xf078~0xf080 */
    volatile unsigned int dhd2_afifo_pre_thd;                                     /* 0xf084 */
    volatile u_dhd2_state dhd2_state;                                             /* 0xf088 */
    volatile u_dhd2_uf_state dhd2_uf_state;                                       /* 0xf08c */
    volatile unsigned int reserved_153[4];                                        /* 4:0xf090~0xf09c */
    volatile u_dhd2_vsync_te_state dhd2_vsync_te_state;                           /* 0xf0a0 */
    volatile u_dhd2_vsync_te_state1 dhd2_vsync_te_state1;                         /* 0xf0a4 */
    volatile unsigned int reserved_154[406];                                      /* 406:0xf0a8~0xf6fc */
    volatile u_intf2_dither_ctrl intf2_dither_ctrl;                               /* 0xf700 */
    volatile u_intf2_dither_sed_y0 intf2_dither_sed_y0;                           /* 0xf704 */
    volatile u_intf2_dither_sed_u0 intf2_dither_sed_u0;                           /* 0xf708 */
    volatile u_intf2_dither_sed_v0 intf2_dither_sed_v0;                           /* 0xf70c */
    volatile u_intf2_dither_sed_w0 intf2_dither_sed_w0;                           /* 0xf710 */
    volatile u_intf2_dither_sed_y1 intf2_dither_sed_y1;                           /* 0xf714 */
    volatile u_intf2_dither_sed_u1 intf2_dither_sed_u1;                           /* 0xf718 */
    volatile u_intf2_dither_sed_v1 intf2_dither_sed_v1;                           /* 0xf71c */
    volatile u_intf2_dither_sed_w1 intf2_dither_sed_w1;                           /* 0xf720 */
    volatile u_intf2_dither_sed_y2 intf2_dither_sed_y2;                           /* 0xf724 */
    volatile u_intf2_dither_sed_u2 intf2_dither_sed_u2;                           /* 0xf728 */
    volatile u_intf2_dither_sed_v2 intf2_dither_sed_v2;                           /* 0xf72c */
    volatile u_intf2_dither_sed_w2 intf2_dither_sed_w2;                           /* 0xf730 */
    volatile u_intf2_dither_sed_y3 intf2_dither_sed_y3;                           /* 0xf734 */
    volatile u_intf2_dither_sed_u3 intf2_dither_sed_u3;                           /* 0xf738 */
    volatile u_intf2_dither_sed_v3 intf2_dither_sed_v3;                           /* 0xf73c */
    volatile u_intf2_dither_sed_w3 intf2_dither_sed_w3;                           /* 0xf740 */
    volatile u_intf2_dither_thr intf2_dither_thr;                                 /* 0xf744 */
    volatile unsigned int reserved_155[46];                                       /* 46:0xf748~0xf7fc */
    volatile u_date_coeff0 date_coeff0;                                           /* 0xf800 */
    volatile u_date_coeff1 date_coeff1;                                           /* 0xf804 */
    volatile unsigned int date_coeff2;                                            /* 0xf808 */
    volatile u_date_coeff3 date_coeff3;                                           /* 0xf80c */
    volatile u_date_coeff4 date_coeff4;                                           /* 0xf810 */
    volatile u_date_coeff5 date_coeff5;                                           /* 0xf814 */
    volatile u_date_coeff6 date_coeff6;                                           /* 0xf818 */
    volatile u_date_coeff7 date_coeff7;                                           /* 0xf81c */
    volatile unsigned int date_coeff8;                                            /* 0xf820 */
    volatile unsigned int date_coeff9;                                            /* 0xf824 */
    volatile u_date_coeff10 date_coeff10;                                         /* 0xf828 */
    volatile u_date_coeff11 date_coeff11;                                         /* 0xf82c */
    volatile u_date_coeff12 date_coeff12;                                         /* 0xf830 */
    volatile u_date_coeff13 date_coeff13;                                         /* 0xf834 */
    volatile u_date_coeff14 date_coeff14;                                         /* 0xf838 */
    volatile u_date_coeff15 date_coeff15;                                         /* 0xf83c */
    volatile u_date_coeff16 date_coeff16;                                         /* 0xf840 */
    volatile unsigned int date_coeff17;                                           /* 0xf844 */
    volatile unsigned int date_coeff18;                                           /* 0xf848 */
    volatile u_date_coeff19 date_coeff19;                                         /* 0xf84c */
    volatile u_date_coeff20 date_coeff20;                                         /* 0xf850 */
    volatile u_date_coeff21 date_coeff21;                                         /* 0xf854 */
    volatile u_date_coeff22 date_coeff22;                                         /* 0xf858 */
    volatile u_date_coeff23 date_coeff23;                                         /* 0xf85c */
    volatile unsigned int date_coeff24;                                           /* 0xf860 */
    volatile u_date_coeff25 date_coeff25;                                         /* 0xf864 */
    volatile u_date_coeff26 date_coeff26;                                         /* 0xf868 */
    volatile u_date_coeff27 date_coeff27;                                         /* 0xf86c */
    volatile u_date_coeff28 date_coeff28;                                         /* 0xf870 */
    volatile u_date_coeff29 date_coeff29;                                         /* 0xf874 */
    volatile u_date_coeff30 date_coeff30;                                         /* 0xf878 */
    volatile unsigned int reserved_156;                                           /* 0xf87c */
    volatile u_date_isrmask date_isrmask;                                         /* 0xf880 */
    volatile u_date_isrstate date_isrstate;                                       /* 0xf884 */
    volatile u_date_isr date_isr;                                                 /* 0xf888 */
    volatile unsigned int reserved_157;                                           /* 0xf88c */
    volatile unsigned int date_version;                                           /* 0xf890 */
    volatile u_date_coeff37 date_coeff37;                                         /* 0xf894 */
    volatile u_date_coeff38 date_coeff38;                                         /* 0xf898 */
    volatile u_date_coeff39 date_coeff39;                                         /* 0xf89c */
    volatile u_date_coeff40 date_coeff40;                                         /* 0xf8a0 */
    volatile u_date_coeff41 date_coeff41;                                         /* 0xf8a4 */
    volatile u_date_coeff42 date_coeff42;                                         /* 0xf8a8 */
    volatile unsigned int reserved_158[5];                                        /* 5:0xf8ac~0xf8bc */
    volatile u_date_dacdet1 date_dacdet1;                                         /* 0xf8c0 */
    volatile u_date_dacdet2 date_dacdet2;                                         /* 0xf8c4 */
    volatile u_date_coeff50 date_coeff50;                                         /* 0xf8c8 */
    volatile u_date_coeff51 date_coeff51;                                         /* 0xf8cc */
    volatile u_date_coeff52 date_coeff52;                                         /* 0xf8d0 */
    volatile u_date_coeff53 date_coeff53;                                         /* 0xf8d4 */
    volatile u_date_coeff54 date_coeff54;                                         /* offset 0xf8d0 + 0x08 */
    volatile u_date_coeff55 date_coeff55;                                         /* 0xf8dc */
    volatile unsigned int reserved_159[456];                                      /* 456:0xf8e0~0xfffc */
    volatile u_mac_outstanding mac_outstanding;                                   /* 0x10000 */
    volatile u_mac_ctrl mac_ctrl;                                                 /* 0x10004 */
    volatile unsigned int reserved_160[2];                                        /* 2:0x10008~0x1000c */
    volatile u_mac_rchn_prio mac_rchn_prio;                                       /* 0x10010 */
    volatile unsigned int reserved_161;                                           /* 0x10014 */
    volatile u_mac_wchn_prio mac_wchn_prio;                                       /* 0x10018 */
    volatile unsigned int reserved_162;                                           /* 0x1001c */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART42_H_ */
