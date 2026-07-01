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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART48_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART48_H_

    volatile reg_vdp_v3r2_lineseg_cmp_glb_st vdp_v3r2_lineseg_cmp_glb_st;                     /* 0x11958 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_dbg_reg;                                       /* 0x1195c */
    volatile unsigned int reserved_216[8];                                          /* 0x11960~0x1197c 8 regs */
    volatile reg_vdp_v3r2_lineseg_cmp_glb_info_c vdp_v3r2_lineseg_cmp_glb_info_c;             /* 0x11980 */
    volatile reg_vdp_v3r2_lineseg_cmp_frame_size_c vdp_v3r2_lineseg_cmp_frame_size_c;         /* 0x11984 */
    volatile reg_vdp_v3r2_lineseg_cmp_rc_cfg0_c vdp_v3r2_lineseg_cmp_rc_cfg0_c;               /* 0x11988 */
    volatile reg_vdp_v3r2_lineseg_cmp_rc_cfg1_c vdp_v3r2_lineseg_cmp_rc_cfg1_c;               /* 0x1198c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg2_c;                                     /* 0x11990 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg3_c;                                     /* 0x11994 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg4_c;                                     /* 0x11998 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg5_c;                                     /* 0x1199c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg6_c;                                     /* 0x119a0 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg7_c;                                     /* 0x119a4 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg8_c;                                     /* 0x119a8 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg9_c;                                     /* 0x119ac */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg10_c;                                    /* 0x119b0 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg11_c;                                    /* 0x119b4 */
    volatile reg_vdp_v3r2_lineseg_cmp_rc_cfg12_c vdp_v3r2_lineseg_cmp_rc_cfg12_c;             /* 0x119b8 */
    volatile reg_vdp_v3r2_lineseg_cmp_rc_cfg13_c vdp_v3r2_lineseg_cmp_rc_cfg13_c;             /* 0x119bc */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg14_c;                                    /* 0x119c0 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg15_c;                                    /* 0x119c4 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_adpqp_thr0_c;                                  /* 0x119c8 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_adpqp_thr1_c;                                  /* 0x119cc */
    volatile reg_vdp_v3r2_lineseg_cmp_rc_cfg16_c vdp_v3r2_lineseg_cmp_rc_cfg16_c;             /* 0x119d0 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_glb_cfg_c;                                     /* 0x119d4 */
    volatile reg_vdp_v3r2_lineseg_cmp_glb_st_c vdp_v3r2_lineseg_cmp_glb_st_c;                 /* status register */
    volatile unsigned int vdp_v3r2_lineseg_cmp_dbg_reg_c;                                     /* 0x119dc */
    volatile unsigned int reserved_217[264];                                        /* 0x119e0~0x11dfc 264 regs */
    volatile reg_wbc_cmp_ctrl wbc_cmp_ctrl;                                                   /* 0x11e00 */
    volatile reg_wbc_cmp_upd wbc_cmp_upd;                                                     /* 0x11e04 */
    volatile reg_wbc_cmp_height wbc_cmp_height;                                               /* 0x11e08 */
    volatile reg_wbc_cmp_oreso wbc_cmp_oreso;                                                 /* 0x11e0c */
    volatile unsigned int wbc_cmp_yaddr;                                                      /* 0x11e10 */
    volatile unsigned int wbc_cmp_yaddr1;                                                     /* 0x11e14 */
    volatile unsigned int wbc_cmp_caddr;                                                      /* 0x11e18 */
    volatile unsigned int wbc_cmp_caddr1;                                                     /* 0x11e1c */
    volatile unsigned int wbc_cmp_addr0_t0;                                                   /* 0x11e20 */
    volatile unsigned int wbc_cmp_addr1_t0;                                                   /* 0x11e24 */
    volatile unsigned int wbc_cmp_addr0_t1;                                                   /* 0x11e28 */
    volatile unsigned int wbc_cmp_addr1_t1;                                                   /* 0x11e2c */
    volatile unsigned int wbc_cmp_l_fsize;                                                    /* 0x11e30 */
    volatile unsigned int wbc_cmp_c_fsize;                                                    /* 0x11e34 */
    volatile unsigned int wbc_cmp_t0_fsize;                                                   /* 0x11e38 */
    volatile unsigned int wbc_cmp_t1_fsize;                                                   /* 0x11e3c */
    volatile unsigned int wbc_sety_fsize;                                                     /* 0x11e40 */
    volatile unsigned int wbc_setc_fsize;                                                     /* 0x11e44 */
    volatile unsigned int wbc_sett0_fsize;                                                    /* 0x11e48 */
    volatile unsigned int wbc_sett1_fsize;                                                    /* 0x11e4c */
    volatile reg_wbc_od_state wbc_od_state;                                                   /* 0x11e50 */
    volatile unsigned int reserved_218[43];                                         /* 0x11e54~0x11efc 43 regs */
    volatile reg_od_pic_osd_glb_info od_pic_osd_glb_info;                                     /* 0x11f00 */
    volatile reg_od_pic_osd_frame_size od_pic_osd_frame_size;                                 /* 0x11f04 */
    volatile reg_od_pic_osd_rc_cfg0 od_pic_osd_rc_cfg0;                                       /* 0x11f08 */
    volatile reg_od_pic_osd_rc_cfg1 od_pic_osd_rc_cfg1;                                       /* 0x11f0c */
    volatile reg_od_pic_osd_rc_cfg2 od_pic_osd_rc_cfg2;                                       /* 0x11f10 */
    volatile reg_od_pic_osd_rc_cfg3 od_pic_osd_rc_cfg3;                                       /* 0x11f14 */
    volatile reg_od_pic_osd_rc_cfg4 od_pic_osd_rc_cfg4;                                       /* 0x11f18 */
    volatile reg_od_pic_osd_rc_cfg5 od_pic_osd_rc_cfg5;                                       /* 0x11f1c */
    volatile reg_od_pic_osd_rc_cfg6 od_pic_osd_rc_cfg6;                                       /* 0x11f20 */
    volatile reg_od_pic_osd_rc_cfg7 od_pic_osd_rc_cfg7;                                       /* 0x11f24 */
    volatile reg_od_pic_osd_rc_cfg8 od_pic_osd_rc_cfg8;                                       /* 0x11f28 */
    volatile reg_od_pic_osd_rc_cfg9 od_pic_osd_rc_cfg9;                                       /* 0x11f2c */
    volatile reg_od_pic_osd_rc_cfg10 od_pic_osd_rc_cfg10;                                     /* 0x11f30 */
    volatile reg_od_pic_osd_rc_cfg11 od_pic_osd_rc_cfg11;                                     /* 0x11f34 */
    volatile reg_od_pic_osd_rc_cfg12 od_pic_osd_rc_cfg12;                                     /* 0x11f38 */
    volatile reg_od_pic_osd_rc_cfg13 od_pic_osd_rc_cfg13;                                     /* 0x11f3c */
    volatile reg_od_pic_osd_rc_cfg14 od_pic_osd_rc_cfg14;                                     /* 0x11f40 */
    volatile reg_od_pic_osd_rc_cfg15 od_pic_osd_rc_cfg15;                                     /* 0x11f44 */
    volatile reg_od_pic_osd_rc_cfg16 od_pic_osd_rc_cfg16;                                     /* 0x11f48 */
    volatile reg_od_pic_osd_rc_cfg17 od_pic_osd_rc_cfg17;                                     /* 0x11f4c */
    volatile reg_od_pic_osd_rc_cfg18 od_pic_osd_rc_cfg18;                                     /* 0x11f50 */
    volatile reg_od_pic_osd_rc_cfg19 od_pic_osd_rc_cfg19;                                     /* 0x11f54 */
    volatile unsigned int reserved_219[2];                                          /* 0x11f58~0x11f5c 2 regs */
    volatile reg_od_pic_osd_stat_thr od_pic_osd_stat_thr;                                     /* 0x11f60 */
    volatile reg_od_pic_osd_pcmp od_pic_osd_pcmp;                                             /* 0x11f64 */
    volatile unsigned int reserved_220[6];                                          /* 0x11f68~0x11f7c 6 regs */
    volatile reg_od_pic_osd_bs_size od_pic_osd_bs_size;                                       /* 0x11f80 */
    volatile reg_od_pic_osd_worst_row od_pic_osd_worst_row;                                   /* 0x11f84 */
    volatile reg_od_pic_osd_best_row od_pic_osd_best_row;                                     /* 0x11f88 */
    volatile reg_od_pic_osd_stat_info od_pic_osd_stat_info;                                   /* 0x11f8c */
    volatile unsigned int od_pic_osd_debug0;                                                  /* 0x11f90 */
    volatile unsigned int od_pic_osd_debug1;                                                  /* 0x11f94 */
    volatile unsigned int reserved_221[26];                                         /* 0x11f98~0x11ffc 26 regs */
    volatile reg_v0_mrg_ctrl v0_mrg_ctrl;                                                     /* 0x12000 */
    volatile reg_v0_mrg_disp_pos v0_mrg_disp_pos;                                             /* 0x12004 */
    volatile reg_v0_mrg_disp_reso v0_mrg_disp_reso;                                           /* 0x12008 */
    volatile reg_v0_mrg_src_reso v0_mrg_src_reso;                                             /* 0x1200c */
    volatile reg_v0_mrg_src_offset v0_mrg_src_offset;                                         /* 0x12010 */
    volatile unsigned int v0_mrg_y_addr;                                                      /* 0x12014 */
    volatile unsigned int v0_mrg_c_addr;                                                      /* 0x12018 */
    volatile reg_v0_mrg_stride v0_mrg_stride;                                                 /* 0x1201c */
    volatile unsigned int v0_mrg_yh_addr;                                                     /* 0x12020 */
    volatile unsigned int v0_mrg_ch_addr;                                                     /* 0x12024 */
    volatile reg_v0_mrg_hstride v0_mrg_hstride;                                               /* 0x12028 */
    volatile unsigned int reserved_222[5];                                          /* 0x1202c~0x1203c 5 regs */
    volatile reg_v0_mrg_read_ctrl v0_mrg_read_ctrl;                                           /* 0x12040 */
    volatile reg_v0_mrg_read_en v0_mrg_read_en;                                               /* 0x12044 */
    volatile unsigned int reserved_223[750];                                        /* 0x12048~0x12bfc 750 regs */
    volatile reg_v1_mrg_ctrl v1_mrg_ctrl;                                                     /* 0x12c00 */
    volatile reg_v1_mrg_disp_pos v1_mrg_disp_pos;                                             /* 0x12c04 */
    volatile reg_v1_mrg_disp_reso v1_mrg_disp_reso;                                           /* 0x12c08 */
    volatile reg_v1_mrg_src_reso v1_mrg_src_reso;                                             /* 0x12c0c */
    volatile reg_v1_mrg_src_offset v1_mrg_src_offset;                                         /* 0x12c10 */
    volatile unsigned int v1_mrg_y_addr;                                                      /* 0x12c14 */
    volatile unsigned int v1_mrg_c_addr;                                                      /* 0x12c18 */
    volatile reg_v1_mrg_stride v1_mrg_stride;                                                 /* 0x12c1c */
    volatile unsigned int v1_mrg_yh_addr;                                                     /* 0x12c20 */
    volatile unsigned int v1_mrg_ch_addr;                                                     /* 0x12c24 */
    volatile reg_v1_mrg_hstride v1_mrg_hstride;                                               /* 0x12c28 */
    volatile unsigned int reserved_224[5];                                          /* 0x12c2c~0x12c3c 5 regs */
    volatile reg_v1_mrg_read_ctrl v1_mrg_read_ctrl;                                           /* 0x12c40 */
    volatile reg_v1_mrg_read_en v1_mrg_read_en;                                               /* 0x12c44 */
    volatile unsigned int reserved_225[1262];                                       /* 0x12c48~0x13ffc 1262 regs */
    volatile reg_g1_osb_ctrl1_box_0 g1_osb_ctrl1_box_0;                                       /* 0x14000 */
    volatile reg_g1_osb_ctrl2_box_0 g1_osb_ctrl2_box_0;                                       /* 0x14004 */
    volatile reg_g1_osb_ctrl3_box_0 g1_osb_ctrl3_box_0;                                       /* 0x14008 */
    volatile unsigned int reserved_226[509];                                        /* 0x1400c~0x147fc 509 regs */
    volatile reg_g3_osb_ctrl1_box_0 g3_osb_ctrl1_box_0;                                       /* 0x14800 */
    volatile reg_g3_osb_ctrl2_box_0 g3_osb_ctrl2_box_0;                                       /* 0x14804 */
    volatile reg_g3_osb_ctrl3_box_0 g3_osb_ctrl3_box_0;                                       /* 0x14808 */
    volatile unsigned int reserved_227[509];                                        /* 0x1480c~0x14ffc 509 regs */
    volatile reg_g4_osb_ctrl1_box_0 g4_osb_ctrl1_box_0;                                       /* 0x15000 */
    volatile reg_g4_osb_ctrl2_box_0 g4_osb_ctrl2_box_0;                                       /* 0x15004 */
    volatile reg_g4_osb_ctrl3_box_0 g4_osb_ctrl3_box_0;                                       /* 0x15008 */
    volatile unsigned int reserved_228[1021];                                       /* 0x1500c~0x15ffc 1021 regs */
    volatile reg_v1_csc_idc v1_csc_idc;                                                       /* 0x16000 */
    volatile reg_v1_csc_odc v1_csc_odc;                                                       /* 0x16004 */
    volatile reg_v1_csc_iodc v1_csc_iodc;                                                     /* 0x16008 */
    volatile reg_v1_csc_p0 v1_csc_p0;                                                         /* 0x1600c */
    volatile reg_v1_csc_p1 v1_csc_p1;                                                         /* 0x16010 */
    volatile reg_v1_csc_p2 v1_csc_p2;                                                         /* 0x16014 */
    volatile reg_v1_csc_p3 v1_csc_p3;                                                         /* 0x16018 */
    volatile reg_v1_csc_p4 v1_csc_p4;                                                         /* 0x1601c */
    volatile reg_v1_csc1_idc v1_csc1_idc;                                                     /* 0x16020 */
    volatile reg_v1_csc1_odc v1_csc1_odc;                                                     /* 0x16024 */
    volatile reg_v1_csc1_iodc v1_csc1_iodc;                                                   /* 0x16028 */
    volatile reg_v1_csc1_p0 v1_csc1_p0;                                                       /* 0x1602c */
    volatile reg_v1_csc1_p1 v1_csc1_p1;                                                       /* 0x16030 */
    volatile reg_v1_csc1_p2 v1_csc1_p2;                                                       /* 0x16034 */
    volatile reg_v1_csc1_p3 v1_csc1_p3;                                                       /* 0x16038 */
    volatile reg_v1_csc1_p4 v1_csc1_p4;                                                       /* 0x1603c */
    volatile unsigned int reserved_229[48];                                         /* 0x16040~0x160fc 48 regs */
    volatile reg_v2_csc_idc v2_csc_idc;                                                       /* 0x16100 */
    volatile reg_v2_csc_odc v2_csc_odc;                                                       /* 0x16104 */
    volatile reg_v2_csc_iodc v2_csc_iodc;                                                     /* 0x16108 */
    volatile reg_v2_csc_p0 v2_csc_p0;                                                         /* 0x1610c */
    volatile reg_v2_csc_p1 v2_csc_p1;                                                         /* 0x16110 */
    volatile reg_v2_csc_p2 v2_csc_p2;                                                         /* 0x16114 */
    volatile reg_v2_csc_p3 v2_csc_p3;                                                         /* 0x16118 */
    volatile reg_v2_csc_p4 v2_csc_p4;                                                         /* 0x1611c */
    volatile reg_v2_csc1_idc v2_csc1_idc;                                                     /* 0x16120 */
    volatile reg_v2_csc1_odc v2_csc1_odc;                                                     /* 0x16124 */
    volatile reg_v2_csc1_iodc v2_csc1_iodc;                                                   /* 0x16128 */
    volatile reg_v2_csc1_p0 v2_csc1_p0;                                                       /* 0x1612c */
    volatile reg_v2_csc1_p1 v2_csc1_p1;                                                       /* 0x16130 */
    volatile reg_v2_csc1_p2 v2_csc1_p2;                                                       /* 0x16134 */
    volatile reg_v2_csc1_p3 v2_csc1_p3;                                                       /* 0x16138 */
    volatile reg_v2_csc1_p4 v2_csc1_p4;                                                       /* 0x1613c */
    volatile unsigned int reserved_230[48];                                         /* 0x16140~0x161fc 48 regs */
    volatile reg_g1_csc_idc g1_csc_idc;                                                       /* 0x16200 */
    volatile reg_g1_csc_odc g1_csc_odc;                                                       /* 0x16204 */
    volatile reg_g1_csc_iodc g1_csc_iodc;                                                     /* 0x16208 */
    volatile reg_g1_csc_p0 g1_csc_p0;                                                         /* 0x1620c */
    volatile reg_g1_csc_p1 g1_csc_p1;                                                         /* 0x16210 */
    volatile reg_g1_csc_p2 g1_csc_p2;                                                         /* 0x16214 */
    volatile reg_g1_csc_p3 g1_csc_p3;                                                         /* 0x16218 */
    volatile reg_g1_csc_p4 g1_csc_p4;                                                         /* 0x1621c */
    volatile reg_g1_csc1_idc g1_csc1_idc;                                                     /* 0x16220 */
    volatile reg_g1_csc1_odc g1_csc1_odc;                                                     /* 0x16224 */
    volatile reg_g1_csc1_iodc g1_csc1_iodc;                                                   /* 0x16228 */
    volatile reg_g1_csc1_p0 g1_csc1_p0;                                                       /* 0x1622c */
    volatile reg_g1_csc1_p1 g1_csc1_p1;                                                       /* 0x16230 */
    volatile reg_g1_csc1_p2 g1_csc1_p2;                                                       /* 0x16234 */
    volatile reg_g1_csc1_p3 g1_csc1_p3;                                                       /* 0x16238 */
    volatile reg_g1_csc1_p4 g1_csc1_p4;                                                       /* 0x1623c */
    volatile unsigned int reserved_231[48];                                         /* 0x16240~0x162fc 48 regs */
    volatile reg_g3_csc_idc g3_csc_idc;                                                       /* 0x16300 */
    volatile reg_g3_csc_odc g3_csc_odc;                                                       /* 0x16304 */
    volatile reg_g3_csc_iodc g3_csc_iodc;                                                     /* 0x16308 */
    volatile reg_g3_csc_p0 g3_csc_p0;                                                         /* 0x1630c */
    volatile reg_g3_csc_p1 g3_csc_p1;                                                         /* 0x16310 */
    volatile reg_g3_csc_p2 g3_csc_p2;                                                         /* 0x16314 */
    volatile reg_g3_csc_p3 g3_csc_p3;                                                         /* 0x16318 */
    volatile reg_g3_csc_p4 g3_csc_p4;                                                         /* 0x1631c */
    volatile reg_g3_csc1_idc g3_csc1_idc;                                                     /* 0x16320 */
    volatile reg_g3_csc1_odc g3_csc1_odc;                                                     /* 0x16324 */
    volatile reg_g3_csc1_iodc g3_csc1_iodc;                                                   /* 0x16328 */
    volatile reg_g3_csc1_p0 g3_csc1_p0;                                                       /* 0x1632c */
    volatile reg_g3_csc1_p1 g3_csc1_p1;                                                       /* 0x16330 */
    volatile reg_g3_csc1_p2 g3_csc1_p2;                                                       /* 0x16334 */
    volatile reg_g3_csc1_p3 g3_csc1_p3;                                                       /* 0x16338 */
    volatile reg_g3_csc1_p4 g3_csc1_p4;                                                       /* 0x1633c */
    volatile unsigned int reserved_232[176];                                        /* 0x16340~0x165fc 176 regs */
    volatile reg_v0_zme_hinfo v0_zme_hinfo;                                                   /* 0x16600 */
    volatile reg_v0_zme_hsp v0_zme_hsp;                                                       /* 0x16604 */
    volatile reg_v0_zme_hloffset v0_zme_hloffset;                                             /* 0x16608 */
    volatile reg_v0_zme_hcoffset v0_zme_hcoffset;                                             /* 0x1660c */
    volatile reg_v0_zme_hzone0delta v0_zme_hzone0delta;                                       /* 0x16610 */
    volatile reg_v0_zme_hzone2delta v0_zme_hzone2delta;                                       /* 0x16614 */
    volatile reg_v0_zme_hzoneend v0_zme_hzoneend;                                             /* 0x16618 */
    volatile reg_v0_zme_hl_shootctrl v0_zme_hl_shootctrl;                                     /* 0x1661c */
    volatile reg_v0_zme_hc_shootctrl v0_zme_hc_shootctrl;                                     /* 0x16620 */
    volatile reg_v0_zme_hcoef_ren v0_zme_hcoef_ren;                                           /* 0x16624 */
    volatile reg_v0_zme_hcoef_rdata v0_zme_hcoef_rdata;                                       /* 0x16628 */
    volatile unsigned int reserved_233[53];                                         /* 0x1662c~0x166fc 53 regs */
    volatile reg_v0_zme_vinfo v0_zme_vinfo;                                                   /* 0x16700 */
    volatile reg_v0_zme_vsp v0_zme_vsp;                                                       /* 0x16704 */
    volatile reg_v0_zme_voffset v0_zme_voffset;                                               /* 0x16708 */
    volatile reg_v0_zme_vboffset v0_zme_vboffset;                                             /* 0x1670c */
    volatile unsigned int reserved_234[3];                                          /* 0x16710~0x16718 3 regs */
    volatile reg_v0_zme_vl_shootctrl v0_zme_vl_shootctrl;                                     /* 0x1671c */
    volatile reg_v0_zme_vc_shootctrl v0_zme_vc_shootctrl;                                     /* 0x16720 */
    volatile reg_v0_zme_vcoef_ren v0_zme_vcoef_ren;                                           /* 0x16724 */
    volatile reg_v0_zme_vcoef_rdata v0_zme_vcoef_rdata;                                       /* 0x16728 */
    volatile unsigned int reserved_235[533];                                        /* 0x1672c~0x16f7c 533 regs */
    volatile reg_gfx_osd_glb_info gfx_osd_glb_info;                                           /* 0x16f80 */
    volatile reg_gfx_osd_frame_size gfx_osd_frame_size;                                       /* 0x16f84 */
    volatile unsigned int reserved_236[2];                                          /* 0x16f88~0x16f8c 2 regs */
    volatile reg_gfx_osd_dbg_reg gfx_osd_dbg_reg;                                             /* 0x16f90 */
    volatile reg_gfx_osd_dbg_reg1 gfx_osd_dbg_reg1;                                           /* 0x16f94 */
} reg_vdp_regs;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART48_H_ */
