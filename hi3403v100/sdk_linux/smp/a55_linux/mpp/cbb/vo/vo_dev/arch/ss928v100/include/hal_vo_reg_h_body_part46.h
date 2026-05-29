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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART46_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART46_H_

    volatile reg_wbc_gp0_dither_coef1 wbc_gp0_dither_coef1;                         /* 0xb908 */
    volatile unsigned int reserved_94[17];                                          /* 0xb90c~0xb94c 17 regs */
    volatile reg_wbc_gp0_hpzme wbc_gp0_hpzme;                                       /* 0xb950 */
    volatile unsigned int reserved_95[43];                                          /* 0xb954~0xb9fc 43 regs */
    volatile reg_wbc_me_ctrl wbc_me_ctrl;                                           /* 0xba00 */
    volatile reg_wbc_me_upd wbc_me_upd;                                             /* 0xba04 */
    volatile reg_wbc_me_wlen_sel wbc_me_wlen_sel;                                   /* 0xba08 */
    volatile unsigned int reserved_96;                                              /* 0xba0c */
    volatile unsigned int wbc_me_yaddr;                                             /* 0xba10 */
    volatile unsigned int wbc_me_caddr;                                             /* 0xba14 */
    volatile reg_wbc_me_stride wbc_me_stride;                                       /* 0xba18 */
    volatile unsigned int reserved_97;                                              /* 0xba1c */
    volatile reg_wbc_me_oreso wbc_me_oreso;                                         /* 0xba20 */
    volatile unsigned int reserved_98[2];                                           /* 0xba24~0xba28 2 regs */
    volatile reg_wbc_me_smmu_bypass wbc_me_smmu_bypass;                             /* 0xba2c */
    volatile unsigned int reserved_99[4];                                           /* 0xba30~0xba3c 4 regs */
    volatile reg_wbc_me_paraup wbc_me_paraup;                                       /* 0xba40 */
    volatile unsigned int reserved_100[3];                                          /* 0xba44~0xba4c 3 regs */
    volatile unsigned int wbc_me_hlcoefad;                                          /* 0xba50 */
    volatile unsigned int wbc_me_hccoefad;                                          /* 0xba54 */
    volatile unsigned int wbc_me_vlcoefad;                                          /* 0xba58 */
    volatile unsigned int wbc_me_vccoefad;                                          /* 0xba5c */
    volatile unsigned int reserved_101[36];                                         /* 0xba60~0xbaec 36 regs */
    volatile unsigned int wbc_me_checksum_y;                                        /* 0xbaf0 */
    volatile unsigned int wbc_me_checksum_c;                                        /* 0xbaf4 */
    volatile unsigned int reserved_102[2];                                          /* 0xbaf8~0xbafc 2 regs */
    volatile reg_wbc_me_dither_ctrl wbc_me_dither_ctrl;                             /* 0xbb00 */
    volatile reg_wbc_me_dither_coef0 wbc_me_dither_coef0;                           /* 0xbb04 */
    volatile reg_wbc_me_dither_coef1 wbc_me_dither_coef1;                           /* 0xbb08 */
    volatile unsigned int reserved_103[109];                                        /* 0xbb0c~0xbcbc 109 regs */
    volatile reg_wbc_me_zme_hsp wbc_me_zme_hsp;                                     /* 0xbcc0 */
    volatile reg_wbc_me_zme_hloffset wbc_me_zme_hloffset;                           /* 0xbcc4 */
    volatile reg_wbc_me_zme_hcoffset wbc_me_zme_hcoffset;                           /* 0xbcc8 */
    volatile unsigned int reserved_104[3];                                          /* 0xbccc~0xbcd4 3 regs */
    volatile reg_wbc_me_zme_vsp wbc_me_zme_vsp;                                     /* 0xbcd8 */
    volatile reg_wbc_me_zme_vsr wbc_me_zme_vsr;                                     /* 0xbcdc */
    volatile reg_wbc_me_zme_voffset wbc_me_zme_voffset;                             /* 0xbce0 */
    volatile reg_wbc_me_zme_vboffset wbc_me_zme_vboffset;                           /* 0xbce4 */
    volatile unsigned int reserved_105[6];                                          /* 0xbce8~0xbcfc 6 regs */
    volatile reg_wbc_fi_ctrl wbc_fi_ctrl;                                           /* 0xbd00 */
    volatile reg_wbc_fi_upd wbc_fi_upd;                                             /* 0xbd04 */
    volatile reg_wbc_fi_wlen_sel wbc_fi_wlen_sel;                                   /* 0xbd08 */
    volatile unsigned int reserved_106;                                             /* 0xbd0c */
    volatile unsigned int wbc_fi_yaddr;                                             /* 0xbd10 */
    volatile unsigned int wbc_fi_caddr;                                             /* 0xbd14 */
    volatile reg_wbc_fi_stride wbc_fi_stride;                                       /* 0xbd18 */
    volatile unsigned int reserved_107;                                             /* 0xbd1c */
    volatile reg_wbc_fi_oreso wbc_fi_oreso;                                         /* 0xbd20 */
    volatile unsigned int reserved_108[2];                                          /* 0xbd24~0xbd28 2 regs */
    volatile reg_wbc_fi_smmu_bypass wbc_fi_smmu_bypass;                             /* 0xbd2c */
    volatile unsigned int reserved_109[5];                                          /* 0xbd30~0xbd40 5 regs */
    volatile reg_wbc_fi_frame_size wbc_fi_frame_size;                               /* 0xbd44 */
    volatile unsigned int wbc_fi_y_raddr;                                           /* 0xbd48 */
    volatile unsigned int wbc_fi_c_raddr;                                           /* 0xbd4c */
    volatile unsigned int reserved_110[40];                                         /* 0xbd50~0xbdec 40 regs */
    volatile unsigned int wbc_fi_checksum_y;                                        /* 0xbdf0 */
    volatile unsigned int wbc_fi_checksum_c;                                        /* 0xbdf4 */
    volatile unsigned int reserved_111[6];                                          /* 0xbdf8~0xbe0c 6 regs */
    volatile reg_wbc_fi_hcds wbc_fi_hcds;                                           /* 0xbe10 */
    volatile reg_wbc_fi_hcds_coef0 wbc_fi_hcds_coef0;                               /* 0xbe14 */
    volatile reg_wbc_fi_hcds_coef1 wbc_fi_hcds_coef1;                               /* 0xbe18 */
    volatile unsigned int reserved_112;                                             /* 0xbe1c */
    volatile reg_wbc_fi_cmp_mb wbc_fi_cmp_mb;                                       /* 0xbe20 */
    volatile reg_wbc_fi_cmp_max_min wbc_fi_cmp_max_min;                             /* 0xbe24 */
    volatile reg_wbc_fi_cmp_adj_thr wbc_fi_cmp_adj_thr;                             /* 0xbe28 */
    volatile reg_wbc_fi_cmp_big_grad wbc_fi_cmp_big_grad;                           /* 0xbe2c */
    volatile reg_wbc_fi_cmp_blk wbc_fi_cmp_blk;                                     /* 0xbe30 */
    volatile reg_wbc_fi_cmp_graphic_judge wbc_fi_cmp_graphic_judge;                 /* 0xbe34 */
    volatile reg_wbc_fi_cmp_rc wbc_fi_cmp_rc;                                       /* 0xbe38 */
    volatile reg_wbc_fi_cmp_frame_size wbc_fi_cmp_frame_size;                       /* 0xbe3c */
    volatile unsigned int reserved_113[48];                                         /* 0xbe40~0xbefc 48 regs */
    volatile reg_wbc_cmp_glb_info wbc_cmp_glb_info;                                 /* 0xbf00 */
    volatile reg_wbc_cmp_framesize wbc_cmp_framesize;                               /* 0xbf04 */
    volatile reg_wbc_cmp_rc_cfg0 wbc_cmp_rc_cfg0;                                   /* 0xbf08 */
    volatile reg_wbc_cmp_rc_cfg2 wbc_cmp_rc_cfg2;                                   /* 0xbf0c */
    volatile reg_wbc_cmp_rc_cfg3 wbc_cmp_rc_cfg3;                                   /* 0xbf10 */
    volatile reg_wbc_cmp_rc_cfg4 wbc_cmp_rc_cfg4;                                   /* 0xbf14 */
    volatile reg_wbc_cmp_rc_cfg5 wbc_cmp_rc_cfg5;                                   /* 0xbf18 */
    volatile reg_wbc_cmp_rc_cfg6 wbc_cmp_rc_cfg6;                                   /* 0xbf1c */
    volatile reg_wbc_cmp_rc_cfg7 wbc_cmp_rc_cfg7;                                   /* 0xbf20 */
    volatile reg_wbc_cmp_rc_cfg8 wbc_cmp_rc_cfg8;                                   /* 0xbf24 */
    volatile reg_wbc_cmp_rc_cfg10 wbc_cmp_rc_cfg10;                                 /* 0xbf28 */
    volatile reg_wbc_cmp_outsize0 wbc_cmp_outsize0;                                 /* 0xbf2c */
    volatile unsigned int wbc_cmp_dbg_reg0;                                         /* 0xbf30 */
    volatile reg_wbc_cmp_max_row wbc_cmp_max_row;                                   /* 0xbf34 */
    volatile reg_wbc_bmp_ctrl wbc_bmp_ctrl;                                         /* 0xbf38 */
    volatile reg_wbc_bmp_upd wbc_bmp_upd;                                           /* 0xbf3c */
    volatile unsigned int wbc_bmp_yaddr;                                            /* 0xbf40 */
    volatile unsigned int reserved_114[23];                                         /* 0xbf44~0xbf9c 23 regs */
    volatile reg_wbc_bmp_oreso wbc_bmp_oreso;                                       /* 0xbfa0 */
    volatile reg_wbc_bmp_sum wbc_bmp_sum;                                           /* 0xbfa4 */
    volatile unsigned int reserved_115[18];                                         /* 0xbfa8~0xbfec 18 regs */
    volatile unsigned int wbc_bmp_checksum_y;                                       /* 0xbff0 */
    volatile unsigned int wbc_bmp_checksum_c;                                       /* 0xbff4 */
    volatile unsigned int reserved_116[2];                                          /* 0xbff8~0xbffc 2 regs */
    volatile reg_wbc_dhd0_ctrl wbc_dhd0_ctrl;                                       /* 0xc000 */
    volatile reg_wbc_dhd0_upd wbc_dhd0_upd;                                         /* 0xc004 */
    volatile reg_wbc_dhd0_oreso wbc_dhd0_oreso;                                     /* 0xc008 */
    volatile unsigned int reserved_117[29];                                         /* 0xc00c~0xc07c 29 regs */
    volatile reg_wd_hpzme_ctrl wd_hpzme_ctrl;                                       /* 0xc080 */
    volatile reg_wd_hpzmecoef01 wd_hpzmecoef01;                                     /* 0xc084 */
    volatile reg_wd_hpzmecoef23 wd_hpzmecoef23;                                     /* 0xc088 */
    volatile reg_wd_hpzmecoef45 wd_hpzmecoef45;                                     /* 0xc08c */
    volatile reg_wd_hpzmecoef67 wd_hpzmecoef67;                                     /* 0xc090 */
    volatile unsigned int reserved_118[91];                                         /* 0xc094~0xc1fc 91 regs */
    volatile reg_wd_hcds_ctrl wd_hcds_ctrl;                                         /* 0xc200 */
    volatile reg_wd_hcdscoef01 wd_hcdscoef01;                                       /* 0xc204 */
    volatile reg_wd_hcdscoef23 wd_hcdscoef23;                                       /* 0xc208 */
    volatile reg_wd_hcdscoef45 wd_hcdscoef45;                                       /* 0xc20c */
    volatile reg_wd_hcdscoef67 wd_hcdscoef67;                                       /* 0xc210 */
    volatile unsigned int reserved_119[27];                                         /* 0xc214~0xc27c 27 regs */
    volatile reg_dither_ctrl dither_ctrl;                                           /* 0xc280 */
    volatile reg_dither_sed_y0 dither_sed_y0;                                       /* 0xc284 */
    volatile reg_dither_sed_u0 dither_sed_u0;                                       /* 0xc288 */
    volatile reg_dither_sed_v0 dither_sed_v0;                                       /* 0xc28c */
    volatile reg_dither_sed_w0 dither_sed_w0;                                       /* 0xc290 */
    volatile reg_dither_sed_y1 dither_sed_y1;                                       /* 0xc294 */
    volatile reg_dither_sed_u1 dither_sed_u1;                                       /* 0xc298 */
    volatile reg_dither_sed_v1 dither_sed_v1;                                       /* 0xc29c */
    volatile reg_dither_sed_w1 dither_sed_w1;                                       /* 0xc2a0 */
    volatile reg_dither_sed_y2 dither_sed_y2;                                       /* 0xc2a4 */
    volatile reg_dither_sed_u2 dither_sed_u2;                                       /* 0xc2a8 */
    volatile reg_dither_sed_v2 dither_sed_v2;                                       /* 0xc2ac */
    volatile reg_dither_sed_w2 dither_sed_w2;                                       /* 0xc2b0 */
    volatile reg_dither_sed_y3 dither_sed_y3;                                       /* 0xc2b4 */
    volatile reg_dither_sed_u3 dither_sed_u3;                                       /* 0xc2b8 */
    volatile reg_dither_sed_v3 dither_sed_v3;                                       /* 0xc2bc */
    volatile reg_dither_sed_w3 dither_sed_w3;                                       /* 0xc2c0 */
    volatile reg_dither_thr dither_thr;                                             /* 0xc2c4 */
    volatile unsigned int reserved_120[14];                                         /* 0xc2c8~0xc2fc 14 regs */
    volatile reg_wd_zme_hinfo wd_zme_hinfo;                                         /* 0xc300 */
    volatile reg_wd_zme_hsp wd_zme_hsp;                                             /* 0xc304 */
    volatile reg_wd_zme_hloffset wd_zme_hloffset;                                   /* 0xc308 */
    volatile reg_wd_zme_hcoffset wd_zme_hcoffset;                                   /* 0xc30c */
    volatile unsigned int reserved_121[5];                                          /* 0xc310~0xc320 5 regs */
    volatile reg_wd_zme_hcoef_ren wd_zme_hcoef_ren;                                 /* 0xc324 */
    volatile reg_wd_zme_hcoef_rdata wd_zme_hcoef_rdata;                             /* 0xc328 */
    volatile reg_wd_zme_hdraw wd_zme_hdraw;                                         /* 0xc32c */
    volatile reg_wd_zme_hratio wd_zme_hratio;                                       /* 0xc330 */
    volatile unsigned int reserved_122[51];                                         /* 0xc334~0xc3fc 51 regs */
    volatile reg_wd_zme_vinfo wd_zme_vinfo;                                         /* 0xc400 */
    volatile reg_wd_zme_vsp wd_zme_vsp;                                             /* 0xc404 */
    volatile reg_wd_zme_voffset wd_zme_voffset;                                     /* 0xc408 */
    volatile reg_wd_zme_vboffset wd_zme_vboffset;                                   /* 0xc40c */
    volatile unsigned int reserved_123[5];                                          /* 0xc410~0xc420 5 regs */
    volatile reg_wd_zme_vcoef_ren wd_zme_vcoef_ren;                                 /* 0xc424 */
    volatile reg_wd_zme_vcoef_rdata wd_zme_vcoef_rdata;                             /* 0xc428 */
    volatile reg_wd_zme_vdraw wd_zme_vdraw;                                         /* 0xc42c */
    volatile reg_wd_zme_vratio wd_zme_vratio;                                       /* 0xc430 */
    volatile unsigned int reserved_124[755];                                        /* 0xc434~0xcffc 755 regs */
    volatile reg_dhd0_ctrl dhd0_ctrl;                                               /* 0xd000 */
    volatile reg_dhd0_vsync1 dhd0_vsync1;                                           /* 0xd004 */
    volatile reg_dhd0_vsync2 dhd0_vsync2;                                           /* 0xd008 */
    volatile reg_dhd0_hsync1 dhd0_hsync1;                                           /* 0xd00c */
    volatile reg_dhd0_hsync2 dhd0_hsync2;                                           /* 0xd010 */
    volatile reg_dhd0_vplus1 dhd0_vplus1;                                           /* 0xd014 */
    volatile reg_dhd0_vplus2 dhd0_vplus2;                                           /* 0xd018 */
    volatile reg_dhd0_pwr dhd0_pwr;                                                 /* 0xd01c */
    volatile reg_dhd0_vtthd3 dhd0_vtthd3;                                           /* 0xd020 */
    volatile reg_dhd0_vtthd dhd0_vtthd;                                             /* 0xd024 */
    volatile reg_dhd0_parathd dhd0_parathd;                                         /* 0xd028 */
    volatile reg_dhd0_precharge_thd dhd0_precharge_thd;                             /* 0xd02c */
    volatile reg_dhd0_start_pos dhd0_start_pos;                                     /* 0xd030 */
    volatile reg_dhd0_start_pos1 dhd0_start_pos1;                                   /* 0xd034 */
    volatile reg_dhd0_paraup dhd0_paraup;                                           /* 0xd038 */
    volatile reg_dhd0_sync_inv dhd0_sync_inv;                                       /* 0xd03c */
    volatile reg_dhd0_clk_dv_ctrl dhd0_clk_dv_ctrl;                                 /* 0xd040 */
    volatile reg_dhd0_rgb_fix_ctrl dhd0_rgb_fix_ctrl;                               /* 0xd044 */
    volatile reg_dhd0_lockcfg dhd0_lockcfg;                                         /* 0xd048 */
    volatile unsigned int dhd0_cap_frm_cnt;                                         /* 0xd04c */
    volatile unsigned int dhd0_vdp_frm_cnt;                                         /* 0xd050 */
    volatile unsigned int dhd0_vsync_cap_vdp_cnt;                                   /* 0xd054 */
    volatile unsigned int dhd0_intf_chksum_y;                                       /* 0xd058 */
    volatile unsigned int dhd0_intf_chksum_u;                                       /* 0xd05c */
    volatile unsigned int dhd0_intf_chksum_v;                                       /* 0xd060 */
    volatile unsigned int dhd0_intf1_chksum_y;                                      /* 0xd064 */
    volatile unsigned int dhd0_intf1_chksum_u;                                      /* 0xd068 */
    volatile unsigned int dhd0_intf1_chksum_v;                                      /* 0xd06c */
    volatile reg_dhd0_intf_chksum_high1 dhd0_intf_chksum_high1;                     /* 0xd070 */
    volatile reg_dhd0_intf_chksum_high2 dhd0_intf_chksum_high2;                     /* 0xd074 */
    volatile unsigned int reserved_125[3];                                          /* 0xd078~0xd080 3 regs */
    volatile unsigned int dhd0_afifo_pre_thd;                                       /* 0xd084 */
    volatile reg_dhd0_state dhd0_state;                                             /* 0xd088 */
    volatile reg_dhd0_uf_state dhd0_uf_state;                                       /* 0xd08c */
    volatile reg_vo_mux vo_mux;                                                     /* 0xd090 */
    volatile reg_vo_mux_sync vo_mux_sync;                                           /* 0xd094 */
    volatile reg_vo_mux_data vo_mux_data;                                           /* 0xd098 */
    volatile unsigned int reserved_126;                                             /* 0xd09c */
    volatile reg_dhd0_vsync_te_state dhd0_vsync_te_state;                           /* 0xd0a0 */
    volatile reg_dhd0_vsync_te_state1 dhd0_vsync_te_state1;                         /* 0xd0a4 */
    volatile unsigned int reserved_127[6];                                          /* 0xd0a8~0xd0bc 6 regs */
    volatile reg_dhd0_ccdoimgmod dhd0_ccdoimgmod;                                   /* 0xd0c0 */
    volatile reg_dhd0_ccdoposmskh dhd0_ccdoposmskh;                                 /* 0xd0c4 */
    volatile reg_dhd0_ccdoposmskl dhd0_ccdoposmskl;                                 /* 0xd0c8 */
    volatile unsigned int reserved_128;                                             /* 0xd0cc */
    volatile reg_dhd0_dacdet1 dhd0_dacdet1;                                         /* 0xd0d0 */
    volatile reg_dhd0_dacdet2 dhd0_dacdet2;                                         /* 0xd0d4 */
    volatile unsigned int reserved_129[2];                                          /* 2 reserved regs */
    volatile reg_dhd0_ccd_info1 dhd0_ccd_info1;                                     /* 0xd0e0 */
    volatile reg_dhd0_ccd_info2 dhd0_ccd_info2;                                     /* 0xd0e4 */
    volatile reg_dhd0_ccd_info3 dhd0_ccd_info3;                                     /* 0xd0e8 */
    volatile unsigned int reserved_130[5];                                          /* 0xd0ec~0xd0fc 5 regs */
    volatile reg_intf_hdmi_ctrl intf_hdmi_ctrl;                                     /* 0xd100 */
    volatile reg_intf_hdmi_upd intf_hdmi_upd;                                       /* 0xd104 */
    volatile reg_intf_hdmi_sync_inv intf_hdmi_sync_inv;                             /* 0xd108 */
    volatile unsigned int reserved_131;                                             /* 0xd10c */
    volatile unsigned int hdmi_intf_chksum_y;                                       /* 0xd110 */
    volatile unsigned int hdmi_intf_chksum_u;                                       /* 0xd114 */
    volatile unsigned int hdmi_intf_chksum_v;                                       /* 0xd118 */
    volatile reg_hdmi_intf_chksum_high hdmi_intf_chksum_high;                       /* 0xd11c */
    volatile unsigned int hdmi_intf1_chksum_y;                                      /* 0xd120 */
    volatile unsigned int hdmi_intf1_chksum_u;                                      /* 0xd124 */
    volatile unsigned int hdmi_intf1_chksum_v;                                      /* 0xd128 */
    volatile reg_hdmi_intf1_chksum_high hdmi_intf1_chksum_high;                     /* 0xd12c */
    volatile unsigned int reserved_132[8];                                          /* 0xd130~0xd14c 8 regs */
    volatile reg_hdmi_hfir_coef0 hdmi_hfir_coef0;                                   /* 0xd150 */
    volatile reg_hdmi_hfir_coef1 hdmi_hfir_coef1;                                   /* 0xd154 */
    volatile reg_hdmi_hfir_coef2 hdmi_hfir_coef2;                                   /* 0xd158 */
    volatile reg_hdmi_hfir_coef3 hdmi_hfir_coef3;                                   /* 0xd15c */
    volatile reg_hdmi_csc_idc hdmi_csc_idc;                                         /* 0xd160 */
    volatile reg_hdmi_csc_odc hdmi_csc_odc;                                         /* 0xd164 */
    volatile reg_hdmi_csc_iodc hdmi_csc_iodc;                                       /* 0xd168 */
    volatile reg_hdmi_csc_p0 hdmi_csc_p0;                                           /* 0xd16c */
    volatile reg_hdmi_csc_p1 hdmi_csc_p1;                                           /* 0xd170 */
    volatile reg_hdmi_csc_p2 hdmi_csc_p2;                                           /* 0xd174 */
    volatile reg_hdmi_csc_p3 hdmi_csc_p3;                                           /* 0xd178 */
    volatile reg_hdmi_csc_p4 hdmi_csc_p4;                                           /* 0xd17c */
    volatile reg_intf_mipi_del_ctrl intf_mipi_del_ctrl;                             /* 0xd180 */
    volatile reg_intf_mipi_del_upd intf_mipi_del_upd;                               /* 0xd184 */
    volatile reg_intf_mipi_del_sync_inv intf_mipi_del_sync_inv;                     /* 0xd188 */
    volatile unsigned int reserved_133;                                             /* 0xd18c */
    volatile unsigned int mipi_del_intf_chksum_y;                                   /* 0xd190 */
    volatile unsigned int mipi_del_intf_chksum_u;                                   /* 0xd194 */
    volatile unsigned int mipi_del_intf_chksum_v;                                   /* 0xd198 */
    volatile reg_mipi_del_intf_chksum_high mipi_del_intf_chksum_high;               /* 0xd19c */
    volatile unsigned int mipi_del_intf1_chksum_y;                                  /* 0xd1a0 */
    volatile unsigned int mipi_del_intf1_chksum_u;                                  /* 0xd1a4 */
    volatile unsigned int mipi_del_intf1_chksum_v;                                  /* 0xd1a8 */
    volatile reg_mipi_del_intf1_chksum_high mipi_del_intf1_chksum_high;             /* 0xd1ac */
    volatile unsigned int reserved_134[8];                                          /* 0xd1b0~0xd1cc 8 regs */
    volatile reg_mipi_del_hfir_coef0 mipi_del_hfir_coef0;                           /* 0xd1d0 */
    volatile reg_mipi_del_hfir_coef1 mipi_del_hfir_coef1;                           /* 0xd1d4 */
    volatile reg_mipi_del_hfir_coef2 mipi_del_hfir_coef2;                           /* offset 0xd1d0 + 0x08 */
    volatile reg_mipi_del_hfir_coef3 mipi_del_hfir_coef3;                           /* 0xd1dc */
    volatile reg_mipi_del_csc_idc mipi_del_csc_idc;                                 /* 0xd1e0 */
    volatile reg_mipi_del_csc_odc mipi_del_csc_odc;                                 /* 0xd1e4 */
    volatile reg_mipi_del_csc_iodc mipi_del_csc_iodc;                               /* 0xd1e8 */
    volatile reg_mipi_del_csc_p0 mipi_del_csc_p0;                                   /* 0xd1ec */
    volatile reg_mipi_del_csc_p1 mipi_del_csc_p1;                                   /* 0xd1f0 */
    volatile reg_mipi_del_csc_p2 mipi_del_csc_p2;                                   /* 0xd1f4 */
    volatile reg_mipi_del_csc_p3 mipi_del_csc_p3;                                   /* 0xd1f8 */
    volatile reg_mipi_del_csc_p4 mipi_del_csc_p4;                                   /* 0xd1fc */
    volatile reg_intf_bt_ctrl intf_bt_ctrl;                                         /* 0xd200 */
    volatile reg_intf_bt_upd intf_bt_upd;                                           /* 0xd204 */
    volatile reg_intf_bt_sync_inv intf_bt_sync_inv;                                 /* 0xd208 */
    volatile unsigned int reserved_135;                                             /* 0xd20c */
    volatile reg_bt_clip0_l bt_clip0_l;                                             /* 0xd210 */
    volatile reg_bt_clip0_h bt_clip0_h;                                             /* 0xd214 */
    volatile unsigned int reserved_136[26];                                         /* 0xd218~0xd27c 26 regs */
    volatile reg_bt_dither_ctrl bt_dither_ctrl;                                     /* 0xd280 */
    volatile reg_bt_dither_sed_y0 bt_dither_sed_y0;                                 /* 0xd284 */
    volatile reg_bt_dither_sed_u0 bt_dither_sed_u0;                                 /* 0xd288 */
    volatile reg_bt_dither_sed_v0 bt_dither_sed_v0;                                 /* 0xd28c */
    volatile reg_bt_dither_sed_w0 bt_dither_sed_w0;                                 /* 0xd290 */
    volatile reg_bt_dither_sed_y1 bt_dither_sed_y1;                                 /* 0xd294 */
    volatile reg_bt_dither_sed_u1 bt_dither_sed_u1;                                 /* 0xd298 */
    volatile reg_bt_dither_sed_v1 bt_dither_sed_v1;                                 /* 0xd29c */
    volatile reg_bt_dither_sed_w1 bt_dither_sed_w1;                                 /* 0xd2a0 */
    volatile reg_bt_dither_sed_y2 bt_dither_sed_y2;                                 /* 0xd2a4 */
    volatile reg_bt_dither_sed_u2 bt_dither_sed_u2;                                 /* 0xd2a8 */
    volatile reg_bt_dither_sed_v2 bt_dither_sed_v2;                                 /* 0xd2ac */
    volatile reg_bt_dither_sed_w2 bt_dither_sed_w2;                                 /* 0xd2b0 */
    volatile reg_bt_dither_sed_y3 bt_dither_sed_y3;                                 /* 0xd2b4 */
    volatile reg_bt_dither_sed_u3 bt_dither_sed_u3;                                 /* 0xd2b8 */
    volatile reg_bt_dither_sed_v3 bt_dither_sed_v3;                                 /* 0xd2bc */
    volatile reg_bt_dither_sed_w3 bt_dither_sed_w3;                                 /* 0xd2c0 */
    volatile reg_bt_dither_thr bt_dither_thr;                                       /* 0xd2c4 */
    volatile unsigned int reserved_137[10];                                         /* 0xd2c8~0xd2ec 10 regs */
    volatile unsigned int bt_intf_chksum_y;                                         /* 0xd2f0 */
    volatile unsigned int bt_intf_chksum_u;                                         /* 0xd2f4 */
    volatile unsigned int bt_intf_chksum_v;                                         /* 0xd2f8 */
    volatile unsigned int reserved_138;                                             /* 0xd2fc */
    volatile reg_intf_lcd_ctrl intf_lcd_ctrl;                                       /* 0xd300 */
    volatile reg_intf_lcd_upd intf_lcd_upd;                                         /* 0xd304 */
    volatile reg_intf_lcd_sync_inv intf_lcd_sync_inv;                               /* 0xd308 */
    volatile unsigned int reserved_139[5];                                          /* 0xd30c~0xd31c 5 regs */
    volatile reg_lcd_csc_idc lcd_csc_idc;                                           /* 0xd320 */
    volatile reg_lcd_csc_odc lcd_csc_odc;                                           /* 0xd324 */
    volatile reg_lcd_csc_iodc lcd_csc_iodc;                                         /* 0xd328 */
    volatile reg_lcd_csc_p0 lcd_csc_p0;                                             /* 0xd32c */
    volatile reg_lcd_csc_p1 lcd_csc_p1;                                             /* 0xd330 */
    volatile reg_lcd_csc_p2 lcd_csc_p2;                                             /* 0xd334 */
    volatile reg_lcd_csc_p3 lcd_csc_p3;                                             /* 0xd338 */
    volatile reg_lcd_csc_p4 lcd_csc_p4;                                             /* 0xd33c */
    volatile unsigned int reserved_140[16];                                         /* 0xd340~0xd37c 16 regs */
    volatile reg_lcd_dither_ctrl lcd_dither_ctrl;                                   /* 0xd380 */
    volatile reg_lcd_dither_sed_y0 lcd_dither_sed_y0;                               /* 0xd384 */
    volatile reg_lcd_dither_sed_u0 lcd_dither_sed_u0;                               /* 0xd388 */
    volatile reg_lcd_dither_sed_v0 lcd_dither_sed_v0;                               /* 0xd38c */
    volatile reg_lcd_dither_sed_w0 lcd_dither_sed_w0;                               /* 0xd390 */
    volatile reg_lcd_dither_sed_y1 lcd_dither_sed_y1;                               /* 0xd394 */
    volatile reg_lcd_dither_sed_u1 lcd_dither_sed_u1;                               /* 0xd398 */
    volatile reg_lcd_dither_sed_v1 lcd_dither_sed_v1;                               /* 0xd39c */
    volatile reg_lcd_dither_sed_w1 lcd_dither_sed_w1;                               /* 0xd3a0 */
    volatile reg_lcd_dither_sed_y2 lcd_dither_sed_y2;                               /* 0xd3a4 */
    volatile reg_lcd_dither_sed_u2 lcd_dither_sed_u2;                               /* 0xd3a8 */
    volatile reg_lcd_dither_sed_v2 lcd_dither_sed_v2;                               /* 0xd3ac */
    volatile reg_lcd_dither_sed_w2 lcd_dither_sed_w2;                               /* 0xd3b0 */
    volatile reg_lcd_dither_sed_y3 lcd_dither_sed_y3;                               /* 0xd3b4 */
    volatile reg_lcd_dither_sed_u3 lcd_dither_sed_u3;                               /* 0xd3b8 */
    volatile reg_lcd_dither_sed_v3 lcd_dither_sed_v3;                               /* 0xd3bc */
    volatile reg_lcd_dither_sed_w3 lcd_dither_sed_w3;                               /* 0xd3c0 */
    volatile reg_lcd_dither_thr lcd_dither_thr;                                     /* 0xd3c4 */
    volatile unsigned int reserved_141[10];                                         /* 0xd3c8~0xd3ec 10 regs */
    volatile unsigned int lcd_intf_chksum_y;                                        /* 0xd3f0 */
    volatile unsigned int lcd_intf_chksum_u;                                        /* 0xd3f4 */
    volatile unsigned int lcd_intf_chksum_v;                                        /* 0xd3f8 */
    volatile unsigned int reserved_142;                                             /* 0xd3fc */
    volatile reg_intf_hdmi1_ctrl intf_hdmi1_ctrl;                                   /* 0xd400 */
    volatile reg_intf_hdmi1_upd intf_hdmi1_upd;                                     /* 0xd404 */
    volatile reg_intf_hdmi1_sync_inv intf_hdmi1_sync_inv;                           /* 0xd408 */
    volatile unsigned int reserved_143;                                             /* 0xd40c */
    volatile unsigned int hdmi1_intf_chksum_y;                                      /* 0xd410 */
    volatile unsigned int hdmi1_intf_chksum_u;                                      /* 0xd414 */
    volatile unsigned int hdmi1_intf_chksum_v;                                      /* 0xd418 */
    volatile reg_hdmi1_intf_chksum_high hdmi1_intf_chksum_high;                     /* 0xd41c */
    volatile unsigned int hdmi1_intf1_chksum_y;                                     /* 0xd420 */
    volatile unsigned int hdmi1_intf1_chksum_u;                                     /* 0xd424 */
    volatile unsigned int hdmi1_intf1_chksum_v;                                     /* 0xd428 */
    volatile reg_hdmi1_intf1_chksum_high hdmi1_intf1_chksum_high;                   /* 0xd42c */
    volatile unsigned int reserved_144[8];                                          /* 0xd430~0xd44c 8 regs */
    volatile reg_hdmi1_hfir_coef0 hdmi1_hfir_coef0;                                 /* 0xd450 */
    volatile reg_hdmi1_hfir_coef1 hdmi1_hfir_coef1;                                 /* 0xd454 */
    volatile reg_hdmi1_hfir_coef2 hdmi1_hfir_coef2;                                 /* 0xd458 */
    volatile reg_hdmi1_hfir_coef3 hdmi1_hfir_coef3;                                 /* 0xd45c */
    volatile unsigned int reserved_145[40];                                         /* 0xd460~0xd4fc 40 regs */
    volatile reg_intf_vga_ctrl intf_vga_ctrl;                                       /* 0xd500 */
    volatile reg_intf_vga_upd intf_vga_upd;                                         /* 0xd504 */
    volatile reg_intf_vga_sync_inv intf_vga_sync_inv;                               /* 0xd508 */
    volatile unsigned int reserved_146[5];                                          /* 0xd50c~0xd51c 5 regs */
    volatile reg_vga_csc_idc vga_csc_idc;                                           /* 0xd520 */
    volatile reg_vga_csc_odc vga_csc_odc;                                           /* 0xd524 */
    volatile reg_vga_csc_iodc vga_csc_iodc;                                         /* 0xd528 */
    volatile reg_vga_csc_p0 vga_csc_p0;                                             /* 0xd52c */
    volatile reg_vga_csc_p1 vga_csc_p1;                                             /* 0xd530 */
    volatile reg_vga_csc_p2 vga_csc_p2;                                             /* 0xd534 */
    volatile reg_vga_csc_p3 vga_csc_p3;                                             /* 0xd538 */
    volatile reg_vga_csc_p4 vga_csc_p4;                                             /* 0xd53c */
    volatile reg_vga_hspcfg0 vga_hspcfg0;                                           /* 0xd540 */
    volatile reg_vga_hspcfg1 vga_hspcfg1;                                           /* 0xd544 */
    volatile unsigned int reserved_147[3];                                          /* 0xd548~0xd550 3 regs */
    volatile reg_vga_hspcfg5 vga_hspcfg5;                                           /* 0xd554 */
    volatile reg_vga_hspcfg6 vga_hspcfg6;                                           /* 0xd558 */
    volatile reg_vga_hspcfg7 vga_hspcfg7;                                           /* 0xd55c */
    volatile reg_vga_hspcfg8 vga_hspcfg8;                                           /* 0xd560 */
    volatile unsigned int reserved_148[3];                                          /* 0xd564~0xd56c 3 regs */
    volatile reg_vga_hspcfg12 vga_hspcfg12;                                         /* 0xd570 */
    volatile reg_vga_hspcfg13 vga_hspcfg13;                                         /* 0xd574 */
    volatile reg_vga_hspcfg14 vga_hspcfg14;                                         /* 0xd578 */
    volatile reg_vga_hspcfg15 vga_hspcfg15;                                         /* 0xd57c */
    volatile unsigned int reserved_149[28];                                         /* 0xd580~0xd5ec 28 regs */
    volatile unsigned int vga_intf_chksum_y;                                        /* 0xd5f0 */
    volatile unsigned int vga_intf_chksum_u;                                        /* 0xd5f4 */
    volatile unsigned int vga_intf_chksum_v;                                        /* 0xd5f8 */
    volatile unsigned int reserved_150;                                             /* 0xd5fc */
    volatile reg_intf_date_ctrl intf_date_ctrl;                                     /* 0xd600 */
    volatile reg_intf_date_upd intf_date_upd;                                       /* 0xd604 */
    volatile reg_intf_date_sync_inv intf_date_sync_inv;                             /* 0xd608 */
    volatile unsigned int reserved_151;                                             /* 0xd60c */
    volatile reg_date_clip0_l date_clip0_l;                                         /* 0xd610 */
    volatile reg_date_clip0_h date_clip0_h;                                         /* 0xd614 */
    volatile unsigned int reserved_152[58];                                         /* 0xd618~0xd6fc 58 regs */
    volatile reg_intf0_dither_ctrl intf0_dither_ctrl;                               /* 0xd700 */
    volatile reg_intf0_dither_sed_y0 intf0_dither_sed_y0;                           /* 0xd704 */
    volatile reg_intf0_dither_sed_u0 intf0_dither_sed_u0;                           /* 0xd708 */
    volatile reg_intf0_dither_sed_v0 intf0_dither_sed_v0;                           /* 0xd70c */
    volatile reg_intf0_dither_sed_w0 intf0_dither_sed_w0;                           /* 0xd710 */
    volatile reg_intf0_dither_sed_y1 intf0_dither_sed_y1;                           /* 0xd714 */
    volatile reg_intf0_dither_sed_u1 intf0_dither_sed_u1;                           /* 0xd718 */
    volatile reg_intf0_dither_sed_v1 intf0_dither_sed_v1;                           /* 0xd71c */
    volatile reg_intf0_dither_sed_w1 intf0_dither_sed_w1;                           /* 0xd720 */
    volatile reg_intf0_dither_sed_y2 intf0_dither_sed_y2;                           /* 0xd724 */
    volatile reg_intf0_dither_sed_u2 intf0_dither_sed_u2;                           /* 0xd728 */
    volatile reg_intf0_dither_sed_v2 intf0_dither_sed_v2;                           /* 0xd72c */
    volatile reg_intf0_dither_sed_w2 intf0_dither_sed_w2;                           /* 0xd730 */
    volatile reg_intf0_dither_sed_y3 intf0_dither_sed_y3;                           /* 0xd734 */
    volatile reg_intf0_dither_sed_u3 intf0_dither_sed_u3;                           /* 0xd738 */
    volatile reg_intf0_dither_sed_v3 intf0_dither_sed_v3;                           /* 0xd73c */
    volatile reg_intf0_dither_sed_w3 intf0_dither_sed_w3;                           /* 0xd740 */
    volatile reg_intf0_dither_thr intf0_dither_thr;                                 /* 0xd744 */
    volatile unsigned int reserved_153[14];                                         /* 0xd748~0xd77c 14 regs */
    volatile reg_intf_mipi_ctrl intf_mipi_ctrl;                                     /* 0xd780 */
    volatile reg_intf_mipi_upd intf_mipi_upd;                                       /* 0xd784 */
    volatile reg_intf_mipi_sync_inv intf_mipi_sync_inv;                             /* 0xd788 */
    volatile unsigned int reserved_154;                                             /* 0xd78c */
    volatile unsigned int mipi_intf_chksum_y;                                       /* 0xd790 */
    volatile unsigned int mipi_intf_chksum_u;                                       /* 0xd794 */
    volatile unsigned int mipi_intf_chksum_v;                                       /* 0xd798 */
    volatile reg_mipi_intf_chksum_high mipi_intf_chksum_high;                       /* 0xd79c */
    volatile unsigned int mipi_intf1_chksum_y;                                      /* 0xd7a0 */
    volatile unsigned int mipi_intf1_chksum_u;                                      /* 0xd7a4 */
    volatile unsigned int mipi_intf1_chksum_v;                                      /* 0xd7a8 */
    volatile reg_mipi_intf1_chksum_high mipi_intf1_chksum_high;                     /* 0xd7ac */
    volatile unsigned int reserved_155[8];                                          /* 0xd7b0~0xd7cc 8 regs */
    volatile reg_mipi_hfir_coef0 mipi_hfir_coef0;                                   /* 0xd7d0 */
    volatile reg_mipi_hfir_coef1 mipi_hfir_coef1;                                   /* 0xd7d4 */
    volatile reg_mipi_hfir_coef2 mipi_hfir_coef2;                                   /* offset 0xd7d0 + 0x08 */
    volatile reg_mipi_hfir_coef3 mipi_hfir_coef3;                                   /* 0xd7dc */
    volatile reg_mipi_csc_idc mipi_csc_idc;                                         /* 0xd7e0 */
    volatile reg_mipi_csc_odc mipi_csc_odc;                                         /* 0xd7e4 */
    volatile reg_mipi_csc_iodc mipi_csc_iodc;                                       /* 0xd7e8 */
    volatile reg_mipi_csc_p0 mipi_csc_p0;                                           /* 0xd7ec */
    volatile reg_mipi_csc_p1 mipi_csc_p1;                                           /* 0xd7f0 */
    volatile reg_mipi_csc_p2 mipi_csc_p2;                                           /* 0xd7f4 */
    volatile reg_mipi_csc_p3 mipi_csc_p3;                                           /* 0xd7f8 */
    volatile reg_mipi_csc_p4 mipi_csc_p4;                                           /* 0xd7fc */
    volatile reg_mipi_dither_ctrl mipi_dither_ctrl;                                 /* 0xd800 */
    volatile reg_mipi_dither_sed_y0 mipi_dither_sed_y0;                             /* 0xd804 */
    volatile reg_mipi_dither_sed_u0 mipi_dither_sed_u0;                             /* 0xd808 */
    volatile reg_mipi_dither_sed_v0 mipi_dither_sed_v0;                             /* 0xd80c */
    volatile reg_mipi_dither_sed_w0 mipi_dither_sed_w0;                             /* 0xd810 */
    volatile reg_mipi_dither_sed_y1 mipi_dither_sed_y1;                             /* 0xd814 */
    volatile reg_mipi_dither_sed_u1 mipi_dither_sed_u1;                             /* 0xd818 */
    volatile reg_mipi_dither_sed_v1 mipi_dither_sed_v1;                             /* 0xd81c */
    volatile reg_mipi_dither_sed_w1 mipi_dither_sed_w1;                             /* 0xd820 */
    volatile reg_mipi_dither_sed_y2 mipi_dither_sed_y2;                             /* 0xd824 */
    volatile reg_mipi_dither_sed_u2 mipi_dither_sed_u2;                             /* 0xd828 */
    volatile reg_mipi_dither_sed_v2 mipi_dither_sed_v2;                             /* 0xd82c */
    volatile reg_mipi_dither_sed_w2 mipi_dither_sed_w2;                             /* 0xd830 */
    volatile reg_mipi_dither_sed_y3 mipi_dither_sed_y3;                             /* 0xd834 */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART46_H_ */
