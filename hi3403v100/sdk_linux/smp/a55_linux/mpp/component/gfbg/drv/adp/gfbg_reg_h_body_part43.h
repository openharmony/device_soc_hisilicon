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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART43_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART43_H_

    volatile u_mac_rchn_sel0 mac_rchn_sel0;                                       /* 0x10020 */
    volatile unsigned int mac_rchn_sel1;                                          /* 0x10024 */
    volatile unsigned int reserved_163[2];                                        /* 2:0x10028~0x1002c */
    volatile u_mac_wchn_sel0 mac_wchn_sel0;                                       /* 0x10030 */
    volatile unsigned int reserved_164[3];                                        /* 3:0x10034~0x1003c */
    volatile u_mac_bus_err_clr mac_bus_err_clr;                                   /* 0x10040 */
    volatile u_mac_bus_err mac_bus_err;                                           /* 0x10044 */
    volatile unsigned int reserved_165[2];                                        /* 2:0x10048~0x1004c */
    volatile unsigned int mac_src0_status0;                                       /* 0x10050 */
    volatile unsigned int mac_src0_status1;                                       /* 0x10054 */
    volatile unsigned int mac_src1_status0;                                       /* 0x10058 */
    volatile unsigned int mac_src1_status1;                                       /* 0x1005c */
    volatile unsigned int mac_src2_status0;                                       /* 0x10060 */
    volatile unsigned int mac_src2_status1;                                       /* 0x10064 */
    volatile unsigned int reserved_166[2];                                        /* 2:0x10068~0x1006c */
    volatile u_mac_debug_ctrl mac_debug_ctrl;                                     /* 0x10070 */
    volatile u_mac_debug_clr mac_debug_clr;                                       /* 0x10074 */
    volatile unsigned int reserved_167[2];                                        /* 2:0x10078~0x1007c */
    volatile unsigned int mac0_debug_info;                                        /* 0x10080 */
    volatile unsigned int reserved_168[3];                                        /* 3:0x10084~0x1008c */
    volatile unsigned int mac0_rd_info;                                           /* 0x10090 */
    volatile unsigned int mac0_wr_info;                                           /* 0x10094 */
    volatile unsigned int mac1_rd_info;                                           /* 0x10098 */
    volatile unsigned int mac1_wr_info;                                           /* 0x1009c */
    volatile unsigned int mac2_rd_info;                                           /* 0x100a0 */
    volatile unsigned int mac2_wr_info;                                           /* 0x100a4 */
    volatile unsigned int reserved_169[2];                                        /* 2:0x100a8~0x100ac */
    volatile unsigned int mac0_det_latency0;                                      /* 0x100b0 */
    volatile unsigned int mac0_det_latency1;                                      /* 0x100b4 */
    volatile unsigned int mac0_det_latency2;                                      /* 0x100b8 */
    volatile unsigned int mac0_det_latency3;                                      /* 0x100bc */
    volatile unsigned int mac0_det_latency4;                                      /* 0x100c0 */
    volatile unsigned int mac0_det_latency5;                                      /* 0x100c4 */
    volatile unsigned int mac1_det_latency0;                                      /* 0x100c8 */
    volatile unsigned int mac1_det_latency1;                                      /* 0x100cc */
    volatile unsigned int mac1_det_latency2;                                      /* 0x100d0 */
    volatile unsigned int mac1_det_latency3;                                      /* 0x100d4 */
    volatile unsigned int mac1_det_latency4;                                      /* offset 0x100d0 + 0x08 */
    volatile unsigned int mac1_det_latency5;                                      /* 0x100dc */
    volatile unsigned int reserved_170[72];                                       /* 72:0x100e0~0x101fc */
    volatile u_vid_read_ctrl vid_read_ctrl;                                       /* 0x10200 */
    volatile u_vid_mac_ctrl vid_mac_ctrl;                                         /* 0x10204 */
    volatile unsigned int reserved_171[2];                                        /* 2:0x10208~0x1020c */
    volatile u_vid_out_ctrl vid_out_ctrl;                                         /* 0x10210 */
    volatile u_vid_mute_alpha vid_mute_alpha;                                     /* 0x10214 */
    volatile unsigned int reserved_172;                                           /* 0x10218 */
    volatile u_vid_mute_bk vid_mute_bk;                                           /* 0x1021c */
    volatile unsigned int reserved_173[8];                                        /* 8:0x10220~0x1023c */
    volatile u_vid_src_info vid_src_info;                                         /* 0x10240 */
    volatile u_vid_src_reso vid_src_reso;                                         /* 0x10244 */
    volatile u_vid_src_crop vid_src_crop;                                         /* 0x10248 */
    volatile u_vid_in_reso vid_in_reso;                                           /* 0x1024c */
    volatile unsigned int vid_addr_h;                                             /* 0x10250 */
    volatile unsigned int vid_addr_l;                                             /* 0x10254 */
    volatile unsigned int vid_caddr_h;                                            /* 0x10258 */
    volatile unsigned int vid_caddr_l;                                            /* 0x1025c */
    volatile unsigned int vid_naddr_h;                                            /* 0x10260 */
    volatile unsigned int vid_naddr_l;                                            /* 0x10264 */
    volatile unsigned int vid_ncaddr_h;                                           /* 0x10268 */
    volatile unsigned int vid_ncaddr_l;                                           /* 0x1026c */
    volatile u_vid_stride vid_stride;                                             /* 0x10270 */
    volatile u_vid_2bit_stride vid_2bit_stride;                                   /* 0x10274 */
    volatile u_vid_head_stride vid_head_stride;                                   /* 0x10278 */
    volatile unsigned int reserved_174;                                           /* 0x1027c */
    volatile u_vid_smmu_bypass vid_smmu_bypass;                                   /* 0x10280 */
    volatile unsigned int reserved_175[3];                                        /* 3:0x10284~0x1028c */
    volatile unsigned int vid_head_addr_h;                                        /* 0x10290 */
    volatile unsigned int vid_head_addr_l;                                        /* 0x10294 */
    volatile unsigned int vid_head_caddr_h;                                       /* 0x10298 */
    volatile unsigned int vid_head_caddr_l;                                       /* 0x1029c */
    volatile u_vid_testpat_cfg vid_testpat_cfg;                                   /* 0x102a0 */
    volatile u_vid_testpat_seed vid_testpat_seed;                                 /* 0x102a4 */
    volatile unsigned int vid_testpat_chksum_y;                                   /* 0x102a8 */
    volatile unsigned int vid_testpat_chksum_c;                                   /* 0x102ac */
    volatile unsigned int reserved_176[20];                                       /* 20:0x102b0~0x102fc */
    volatile unsigned int vid_l_cur_flow;                                         /* 0x10300 */
    volatile unsigned int vid_l_cur_sreq_time;                                    /* 0x10304 */
    volatile unsigned int vid_c_cur_flow;                                         /* 0x10308 */
    volatile unsigned int vid_c_cur_sreq_time;                                    /* 0x1030c */
    volatile unsigned int vid_l_last_flow;                                        /* 0x10310 */
    volatile unsigned int vid_l_last_sreq_time;                                   /* 0x10314 */
    volatile unsigned int vid_c_last_flow;                                        /* 0x10318 */
    volatile unsigned int vid_c_last_sreq_time;                                   /* 0x1031c */
    volatile unsigned int vid_l_busy_time;                                        /* 0x10320 */
    volatile unsigned int vid_l_neednordy_time;                                   /* 0x10324 */
    volatile unsigned int vid_l2_neednordy_time;                                  /* 0x10328 */
    volatile unsigned int vid_c_busy_time;                                        /* 0x1032c */
    volatile unsigned int vid_c_neednordy_time;                                   /* 0x10330 */
    volatile unsigned int vid_c2_neednordy_time;                                  /* 0x10334 */
    volatile unsigned int reserved_177[2];                                        /* 2:0x10338~0x1033c */
    volatile u_vid_dcmp_ctrl vid_dcmp_ctrl;                                       /* 0x10340 */
    volatile unsigned int vid_dcmp_l_fsize;                                       /* 0x10344 */
    volatile unsigned int reserved_178[14];                                       /* 14:0x10348~0x1037c */
    volatile u_vdp_v3r2_lineseg_dcmp_glb_info vdp_v3r2_lineseg_dcmp_glb_info;     /* 0x10380 */
    volatile u_vdp_v3r2_lineseg_dcmp_frame_size vdp_v3r2_lineseg_dcmp_frame_size; /* 0x10384 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_adpqp_thr0;                       /* 0x10388 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_adpqp_thr1;                       /* 0x1038c */
    volatile u_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr vdp_v3r2_lineseg_dcmp_smth_deltabits_thr; /* 0x10390 */
    volatile u_vdp_v3r2_lineseg_dcmp_error_sta vdp_v3r2_lineseg_dcmp_error_sta;                   /* 0x10394 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_extra;                                            /* 0x10398 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_dbg_reg;                                          /* 0x1039c */
    volatile unsigned int reserved_179[8];                                              /* 8:0x103a0~0x103bc */
    volatile u_vdp_v3r2_lineseg_dcmp_glb_info_c vdp_v3r2_lineseg_dcmp_glb_info_c;                 /* 0x103c0 */
    volatile u_vdp_v3r2_lineseg_dcmp_frame_size_c vdp_v3r2_lineseg_dcmp_frame_size_c;             /* 0x103c4 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_adpqp_thr0_c;                                     /* 0x103c8 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_adpqp_thr1_c;                                     /* 0x103cc */
    volatile u_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr_c vdp_v3r2_lineseg_dcmp_smth_deltabits_thr_c; /* 0x103d0 */
    volatile u_vdp_v3r2_lineseg_dcmp_error_sta_c vdp_v3r2_lineseg_dcmp_error_sta_c;                   /* 0x103d4 */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_extra_c;                                      /* extra register */
    volatile unsigned int vdp_v3r2_lineseg_dcmp_dbg_reg_c;                                            /* 0x103dc */
    volatile unsigned int reserved_180[648];                                        /* 648:0x103e0~0x10dfc */
    volatile u_gfx_read_ctrl gfx_read_ctrl;                                         /* 0x10e00 */
    volatile u_gfx_mac_ctrl gfx_mac_ctrl;                                           /* 0x10e04 */
    volatile u_gfx_out_ctrl gfx_out_ctrl;                                           /* 0x10e08 */
    volatile unsigned int reserved_181;                                             /* 0x10e0c */
    volatile u_gfx_mute_alpha gfx_mute_alpha;                                       /* 0x10e10 */
    volatile u_gfx_mute_bk gfx_mute_bk;                                             /* 0x10e14 */
    volatile unsigned int reserved_182[2];                                          /* 2:0x10e18~0x10e1c */
    volatile u_gfx_smmu_bypass gfx_smmu_bypass;                                     /* 0x10e20 */
    volatile unsigned int reserved_183;                                             /* 0x10e24 */
    volatile u_gfx_1555_alpha gfx_1555_alpha;                                       /* 0x10e28 */
    volatile unsigned int reserved_184[5];                                          /* 5:0x10e2c~0x10e3c */
    volatile u_gfx_src_info gfx_src_info;                                           /* 0x10e40 */
    volatile u_gfx_src_reso gfx_src_reso;                                           /* 0x10e44 */
    volatile u_gfx_src_crop gfx_src_crop;                                           /* 0x10e48 */
    volatile u_gfx_ireso gfx_ireso;                                                 /* 0x10e4c */
    volatile unsigned int gfx_addr_h;                                               /* 0x10e50 */
    volatile unsigned int gfx_addr_l;                                               /* 0x10e54 */
    volatile unsigned int gfx_naddr_h;                                              /* 0x10e58 */
    volatile unsigned int gfx_naddr_l;                                              /* 0x10e5c */
    volatile u_gfx_stride gfx_stride;                                               /* 0x10e60 */
    volatile unsigned int reserved_185[3];                                          /* 3:0x10e64~0x10e6c */
    volatile unsigned int gfx_dcmp_addr_h;                                          /* 0x10e70 */
    volatile unsigned int gfx_dcmp_addr_l;                                          /* 0x10e74 */
    volatile unsigned int gfx_dcmp_naddr_h;                                         /* 0x10e78 */
    volatile unsigned int gfx_dcmp_naddr_l;                                         /* 0x10e7c */
    volatile unsigned int reserved_186[32];                                         /* 32:0x10e80~0x10efc */
    volatile u_gfx_ckey_max gfx_ckey_max;                                           /* 0x10f00 */
    volatile u_gfx_ckey_min gfx_ckey_min;                                           /* 0x10f04 */
    volatile u_gfx_ckey_mask gfx_ckey_mask;                                         /* 0x10f08 */
    volatile unsigned int reserved_187;                                             /* 0x10f0c */
    volatile u_gfx_testpat_cfg gfx_testpat_cfg;                                     /* 0x10f10 */
    volatile u_gfx_testpat_seed gfx_testpat_seed;                                   /* 0x10f14 */
    volatile unsigned int reserved_188[2];                                          /* 2:0x10f18~0x10f1c */
    volatile unsigned int gfx_dcmp_framesize0;                                      /* 0x10f20 */
    volatile unsigned int gfx_dcmp_framesize1;                                      /* 0x10f24 */
    volatile unsigned int reserved_189[2];                                          /* 2:0x10f28~0x10f2c */
    volatile unsigned int gfx_cur_flow;                                             /* 0x10f30 */
    volatile unsigned int gfx_cur_sreq_time;                                        /* 0x10f34 */
    volatile unsigned int gfx_last_flow;                                            /* 0x10f38 */
    volatile unsigned int gfx_last_sreq_time;                                       /* 0x10f3c */
    volatile unsigned int gfx_busy_time;                                            /* 0x10f40 */
    volatile unsigned int gfx_ar_neednordy_time;                                    /* 0x10f44 */
    volatile unsigned int gfx_gb_neednordy_time;                                    /* 0x10f48 */
    volatile unsigned int reserved_190_1[1];                                        /* 0x10f4c */
    volatile u_gfx_ld_ctrl gfx_ld_ctrl;                                             /* 0x10f50 */
    volatile unsigned int gfx_tde_safe_dis;                                         /* 0x10f54 */
    volatile u_gfx_ld_smute_ctrl gfx_ld_smute_ctrl;                                 /* 0x10f58 */
    volatile u_gfx_ld_err_sta gfx_ld_err_sta;                                       /* 0x10f5c */
    volatile unsigned int gfx_ld_debug0;                                            /* 0x10f60 */
    volatile unsigned int gfx_ld_debug1;                                            /* 0x10f64 */
    volatile unsigned int gfx_ld_debug2;                                            /* 0x10f68 */
    volatile unsigned int gfx_ld_debug3;                                            /* 0x10f6c */
    volatile unsigned int gfx_ld_debug4;                                            /* 0x10f70 */
    volatile unsigned int gfx_ld_debug5;                                            /* 0x10f74 */
    volatile unsigned int reserved_190_2[2];                                        /* 2:0x10f78~0x10f7c */
#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info   vdp_v3r2_line_osd_dcmp_glb_info; /* 0x10f80 */
    volatile u_vdp_v3r2_line_osd_dcmp_frame_size   vdp_v3r2_line_osd_dcmp_frame_size; /* 0x10f84 */
    volatile u_vdp_v3r2_line_osd_dcmp_error_sta   vdp_v3r2_line_osd_dcmp_error_sta; /* 0x10f88 */
    volatile unsigned int reserved_191[541]; /* 0x10f8c~0x117fc 541 regs */
#endif
#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
    volatile u_gfx_dcmp_ctrl gfx_dcmp_ctrl;                                         /* 0x10f80 */
    volatile unsigned int reserved_191[3];                                          /* 3:0x10f84~0x10f8c */
    volatile u_gfx_dcmp_wrong_sta gfx_dcmp_wrong_sta;                               /* 0x10f90 */
    volatile unsigned int gfx_dcmp_debug_sta0;                                      /* 0x10f94 */
    volatile unsigned int reserved_192[538];                                        /* 538:0x10f98~0x117fc */
#endif
    volatile u_wbc_ctrl wbc_ctrl;                                                   /* 0x11800 */
    volatile u_wbc_mac_ctrl wbc_mac_ctrl;                                           /* 0x11804 */
    volatile unsigned int reserved_193[3];                                          /* 3:0x11808~0x11810 */
    volatile u_wbc_smmu_bypass wbc_smmu_bypass;                                     /* 0x11814 */
    volatile unsigned int reserved_194[2];                                          /* 2:0x11818~0x1181c */
    volatile u_wbc_lowdlyctrl wbc_lowdlyctrl;                                       /* 0x11820 */
    volatile unsigned int wbc_tunladdr_h;                                           /* 0x11824 */
    volatile unsigned int wbc_tunladdr_l;                                           /* 0x11828 */
    volatile u_wbc_lowdlysta wbc_lowdlysta;                                         /* 0x1182c */
    volatile unsigned int reserved_195[8];                                          /* 8:0x11830~0x1184c */
    volatile unsigned int wbc_yaddr_h;                                              /* 0x11850 */
    volatile unsigned int wbc_yaddr_l;                                              /* 0x11854 */
    volatile unsigned int wbc_caddr_h;                                              /* 0x11858 */
    volatile unsigned int wbc_caddr_l;                                              /* 0x1185c */
    volatile u_wbc_ystride wbc_ystride;                                             /* 0x11860 */
    volatile u_wbc_cstride wbc_cstride;                                             /* 0x11864 */
    volatile unsigned int reserved_196[2];                                          /* 2:0x11868~0x1186c */
    volatile unsigned int wbc_ynaddr_h;                                             /* 0x11870 */
    volatile unsigned int wbc_ynaddr_l;                                             /* 0x11874 */
    volatile unsigned int wbc_cnaddr_h;                                             /* 0x11878 */
    volatile unsigned int wbc_cnaddr_l;                                             /* 0x1187c */
    volatile u_wbc_ynstride wbc_ynstride;                                           /* 0x11880 */
    volatile u_wbc_cnstride wbc_cnstride;                                           /* 0x11884 */
    volatile unsigned int reserved_197[10];                                         /* 10:0x11888~0x118ac */
    volatile u_wbc_sta wbc_sta;                                                     /* 0x118b0 */
    volatile u_wbc_line_num wbc_line_num;                                           /* 0x118b4 */
    volatile u_wbc_cap_reso wbc_cap_reso;                                           /* 0x118b8 */
    volatile unsigned int wbc_cap_info;                                             /* 0x118bc */
    volatile unsigned int reserved_198[16];                                         /* 16:0x118c0~0x118fc */
    volatile u_vdp_v3r2_lineseg_cmp_glb_info vdp_v3r2_lineseg_cmp_glb_info;         /* 0x11900 */
    volatile u_vdp_v3r2_lineseg_cmp_frame_size vdp_v3r2_lineseg_cmp_frame_size;     /* 0x11904 */
    volatile u_vdp_v3r2_lineseg_cmp_rc_cfg0 vdp_v3r2_lineseg_cmp_rc_cfg0;           /* 0x11908 */
    volatile u_vdp_v3r2_lineseg_cmp_rc_cfg1 vdp_v3r2_lineseg_cmp_rc_cfg1;           /* 0x1190c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg2;                             /* 0x11910 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg3;                             /* 0x11914 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg4;                             /* 0x11918 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg5;                             /* 0x1191c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg6;                             /* 0x11920 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg7;                             /* 0x11924 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg8;                             /* 0x11928 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg9;                             /* 0x1192c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg10;                            /* 0x11930 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg11;                            /* 0x11934 */
    volatile u_vdp_v3r2_lineseg_cmp_rc_cfg12 vdp_v3r2_lineseg_cmp_rc_cfg12;         /* 0x11938 */
    volatile u_vdp_v3r2_lineseg_cmp_rc_cfg13 vdp_v3r2_lineseg_cmp_rc_cfg13;         /* 0x1193c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg14;                            /* 0x11940 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg15;                            /* 0x11944 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_adpqp_thr0;                          /* 0x11948 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_adpqp_thr1;                          /* 0x1194c */
    volatile u_vdp_v3r2_lineseg_cmp_rc_cfg16 vdp_v3r2_lineseg_cmp_rc_cfg16;         /* 0x11950 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_glb_cfg;                             /* 0x11954 */
    volatile u_vdp_v3r2_lineseg_cmp_glb_st vdp_v3r2_lineseg_cmp_glb_st;             /* 0x11958 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_dbg_reg;                             /* 0x1195c */
    volatile unsigned int reserved_199[8];                                          /* 8:0x11960~0x1197c */
    volatile u_vdp_v3r2_lineseg_cmp_glb_info_c vdp_v3r2_lineseg_cmp_glb_info_c;     /* 0x11980 */
    volatile u_vdp_v3r2_lineseg_cmp_frame_size_c vdp_v3r2_lineseg_cmp_frame_size_c; /* 0x11984 */
    volatile u_vdp_v3r2_lineseg_cmp_rc_cfg0_c vdp_v3r2_lineseg_cmp_rc_cfg0_c;       /* 0x11988 */
    volatile u_vdp_v3r2_lineseg_cmp_rc_cfg1_c vdp_v3r2_lineseg_cmp_rc_cfg1_c;       /* 0x1198c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg2_c;                           /* 0x11990 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg3_c;                           /* 0x11994 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg4_c;                           /* 0x11998 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg5_c;                           /* 0x1199c */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg6_c;                           /* 0x119a0 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg7_c;                           /* 0x119a4 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg8_c;                           /* 0x119a8 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg9_c;                           /* 0x119ac */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg10_c;                          /* 0x119b0 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg11_c;                          /* 0x119b4 */
    volatile u_vdp_v3r2_lineseg_cmp_rc_cfg12_c vdp_v3r2_lineseg_cmp_rc_cfg12_c;     /* 0x119b8 */
    volatile u_vdp_v3r2_lineseg_cmp_rc_cfg13_c vdp_v3r2_lineseg_cmp_rc_cfg13_c;     /* 0x119bc */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg14_c;                          /* 0x119c0 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_rc_cfg15_c;                          /* 0x119c4 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_adpqp_thr0_c;                        /* 0x119c8 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_adpqp_thr1_c;                        /* 0x119cc */
    volatile u_vdp_v3r2_lineseg_cmp_rc_cfg16_c vdp_v3r2_lineseg_cmp_rc_cfg16_c;     /* 0x119d0 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_glb_cfg_c;                           /* 0x119d4 */
    volatile u_vdp_v3r2_lineseg_cmp_glb_st_c vdp_v3r2_lineseg_cmp_glb_st_c;         /* offset 0x119d0 + 0x08 */
    volatile unsigned int vdp_v3r2_lineseg_cmp_dbg_reg_c;                           /* 0x119dc */
    volatile unsigned int reserved_200[264];                                        /* 264:0x119e0~0x11dfc */
    volatile u_wbc_cmp_ctrl wbc_cmp_ctrl;                                           /* 0x11e00 */
    volatile u_wbc_cmp_upd wbc_cmp_upd;                                             /* 0x11e04 */
    volatile u_wbc_cmp_height wbc_cmp_height;                                       /* 0x11e08 */
    volatile u_wbc_cmp_oreso wbc_cmp_oreso;                                         /* 0x11e0c */
    volatile unsigned int wbc_cmp_yaddr;                                            /* 0x11e10 */
    volatile unsigned int wbc_cmp_yaddr1;                                           /* 0x11e14 */
    volatile unsigned int wbc_cmp_caddr;                                            /* 0x11e18 */
    volatile unsigned int wbc_cmp_caddr1;                                           /* 0x11e1c */
    volatile unsigned int wbc_cmp_addr0_t0;                                         /* 0x11e20 */
    volatile unsigned int wbc_cmp_addr1_t0;                                         /* 0x11e24 */
    volatile unsigned int wbc_cmp_addr0_t1;                                         /* 0x11e28 */
    volatile unsigned int wbc_cmp_addr1_t1;                                         /* 0x11e2c */
    volatile unsigned int wbc_cmp_l_fsize;                                          /* 0x11e30 */
    volatile unsigned int wbc_cmp_c_fsize;                                          /* 0x11e34 */
    volatile unsigned int wbc_cmp_t0_fsize;                                         /* 0x11e38 */
    volatile unsigned int wbc_cmp_t1_fsize;                                         /* 0x11e3c */
    volatile unsigned int wbc_sety_fsize;                                           /* 0x11e40 */
    volatile unsigned int wbc_setc_fsize;                                           /* 0x11e44 */
    volatile unsigned int wbc_sett0_fsize;                                          /* 0x11e48 */
    volatile unsigned int wbc_sett1_fsize;                                          /* 0x11e4c */
    volatile u_wbc_od_state wbc_od_state;                                           /* 0x11e50 */
    volatile unsigned int reserved_201[43];                                         /* 43:0x11e54~0x11efc */
    volatile u_od_pic_osd_glb_info od_pic_osd_glb_info;                             /* 0x11f00 */
    volatile u_od_pic_osd_frame_size od_pic_osd_frame_size;                         /* 0x11f04 */
    volatile u_od_pic_osd_rc_cfg0 od_pic_osd_rc_cfg0;                               /* 0x11f08 */
    volatile u_od_pic_osd_rc_cfg1 od_pic_osd_rc_cfg1;                               /* 0x11f0c */
    volatile u_od_pic_osd_rc_cfg2 od_pic_osd_rc_cfg2;                               /* 0x11f10 */
    volatile u_od_pic_osd_rc_cfg3 od_pic_osd_rc_cfg3;                               /* 0x11f14 */
    volatile u_od_pic_osd_rc_cfg4 od_pic_osd_rc_cfg4;                               /* 0x11f18 */
    volatile u_od_pic_osd_rc_cfg5 od_pic_osd_rc_cfg5;                               /* 0x11f1c */
    volatile u_od_pic_osd_rc_cfg6 od_pic_osd_rc_cfg6;                               /* 0x11f20 */
    volatile u_od_pic_osd_rc_cfg7 od_pic_osd_rc_cfg7;                               /* 0x11f24 */
    volatile u_od_pic_osd_rc_cfg8 od_pic_osd_rc_cfg8;                               /* 0x11f28 */
    volatile u_od_pic_osd_rc_cfg9 od_pic_osd_rc_cfg9;                               /* 0x11f2c */
    volatile u_od_pic_osd_rc_cfg10 od_pic_osd_rc_cfg10;                             /* 0x11f30 */
    volatile u_od_pic_osd_rc_cfg11 od_pic_osd_rc_cfg11;                             /* 0x11f34 */
    volatile u_od_pic_osd_rc_cfg12 od_pic_osd_rc_cfg12;                             /* 0x11f38 */
    volatile u_od_pic_osd_rc_cfg13 od_pic_osd_rc_cfg13;                             /* 0x11f3c */
    volatile u_od_pic_osd_rc_cfg14 od_pic_osd_rc_cfg14;                             /* 0x11f40 */
    volatile u_od_pic_osd_rc_cfg15 od_pic_osd_rc_cfg15;                             /* 0x11f44 */
    volatile u_od_pic_osd_rc_cfg16 od_pic_osd_rc_cfg16;                             /* 0x11f48 */
    volatile u_od_pic_osd_rc_cfg17 od_pic_osd_rc_cfg17;                             /* 0x11f4c */
    volatile u_od_pic_osd_rc_cfg18 od_pic_osd_rc_cfg18;                             /* 0x11f50 */
    volatile u_od_pic_osd_rc_cfg19 od_pic_osd_rc_cfg19;                             /* 0x11f54 */
    volatile unsigned int reserved_202[2];                                          /* 2:0x11f58~0x11f5c */
    volatile u_od_pic_osd_stat_thr od_pic_osd_stat_thr;                             /* 0x11f60 */
    volatile u_od_pic_osd_pcmp od_pic_osd_pcmp;                                     /* 0x11f64 */
    volatile unsigned int reserved_203[6];                                          /* 6:0x11f68~0x11f7c */
    volatile u_od_pic_osd_bs_size od_pic_osd_bs_size;                               /* 0x11f80 */
    volatile u_od_pic_osd_worst_row od_pic_osd_worst_row;                           /* 0x11f84 */
    volatile u_od_pic_osd_best_row od_pic_osd_best_row;                             /* 0x11f88 */
    volatile u_od_pic_osd_stat_info od_pic_osd_stat_info;                           /* 0x11f8c */
    volatile unsigned int od_pic_osd_debug0;                                        /* 0x11f90 */
    volatile unsigned int od_pic_osd_debug1;                                        /* 0x11f94 */
    volatile unsigned int reserved_204[26];                                         /* 26:0x11f98~0x11ffc */
    volatile u_v0_mrg_ctrl v0_mrg_ctrl;                                             /* 0x12000 */
    volatile u_v0_mrg_disp_pos v0_mrg_disp_pos;                                     /* 0x12004 */
    volatile u_v0_mrg_disp_reso v0_mrg_disp_reso;                                   /* 0x12008 */
    volatile u_v0_mrg_src_reso v0_mrg_src_reso;                                     /* 0x1200c */
    volatile u_v0_mrg_src_offset v0_mrg_src_offset;                                 /* 0x12010 */
    volatile unsigned int v0_mrg_y_addr;                                            /* 0x12014 */
    volatile unsigned int v0_mrg_c_addr;                                            /* 0x12018 */
    volatile u_v0_mrg_stride v0_mrg_stride;                                         /* 0x1201c */
    volatile unsigned int v0_mrg_yh_addr;                                           /* 0x12020 */
    volatile unsigned int v0_mrg_ch_addr;                                           /* 0x12024 */
    volatile u_v0_mrg_hstride v0_mrg_hstride;                                       /* 0x12028 */
    volatile unsigned int reserved_205[5];                                          /* 5:0x1202c~0x1203c */
    volatile u_v0_mrg_read_ctrl v0_mrg_read_ctrl;                                   /* 0x12040 */
    volatile u_v0_mrg_read_en v0_mrg_read_en;                                       /* 0x12044 */
    volatile unsigned int reserved_206[750];                                        /* 750:0x12048~0x12bfc */
    volatile u_v1_mrg_ctrl v1_mrg_ctrl;                                             /* 0x12c00 */
    volatile u_v1_mrg_disp_pos v1_mrg_disp_pos;                                     /* 0x12c04 */
    volatile u_v1_mrg_disp_reso v1_mrg_disp_reso;                                   /* 0x12c08 */
    volatile u_v1_mrg_src_reso v1_mrg_src_reso;                                     /* 0x12c0c */
    volatile u_v1_mrg_src_offset v1_mrg_src_offset;                                 /* 0x12c10 */
    volatile unsigned int v1_mrg_y_addr;                                            /* 0x12c14 */
    volatile unsigned int v1_mrg_c_addr;                                            /* 0x12c18 */
    volatile u_v1_mrg_stride v1_mrg_stride;                                         /* 0x12c1c */
    volatile unsigned int v1_mrg_yh_addr;                                           /* 0x12c20 */
    volatile unsigned int v1_mrg_ch_addr;                                           /* 0x12c24 */
    volatile u_v1_mrg_hstride v1_mrg_hstride;                                       /* 0x12c28 */
    volatile unsigned int reserved_207[5];                                          /* 5:0x12c2c~0x12c3c */
    volatile u_v1_mrg_read_ctrl v1_mrg_read_ctrl;                                   /* 0x12c40 */
    volatile u_v1_mrg_read_en v1_mrg_read_en;                                       /* 0x12c44 */
    volatile unsigned int reserved_208_1[1262];                                     /* 1262:0x12c48~0x14ffc */
    volatile u_osb_ctrl1_box_0 osb_ctrl1_box_0;                                     /* 0x14000 */
    volatile u_osb_ctrl2_box_0 osb_ctrl2_box_0;                                     /* 0x14004 */
    volatile u_osb_ctrl3_box_0 osb_ctrl3_box_0;                                     /* 0x14008 */
    volatile unsigned int reserved_208_2[1021];                                     /* 1021:0x1400c~0x14ffc 1021 regs */
    volatile u_v1_csc_idc v1_csc_idc;                                               /* 0x15000 */
    volatile u_v1_csc_odc v1_csc_odc;                                               /* 0x15004 */
    volatile u_v1_csc_iodc v1_csc_iodc;                                             /* 0x15008 */
    volatile u_v1_csc_p0 v1_csc_p0;                                                 /* 0x1500c */
    volatile u_v1_csc_p1 v1_csc_p1;                                                 /* 0x15010 */
    volatile u_v1_csc_p2 v1_csc_p2;                                                 /* 0x15014 */
    volatile u_v1_csc_p3 v1_csc_p3;                                                 /* 0x15018 */
    volatile u_v1_csc_p4 v1_csc_p4;                                                 /* 0x1501c */
    volatile u_v1_csc1_idc v1_csc1_idc;                                             /* 0x15020 */
    volatile u_v1_csc1_odc v1_csc1_odc;                                             /* 0x15024 */
    volatile u_v1_csc1_iodc v1_csc1_iodc;                                           /* 0x15028 */
    volatile u_v1_csc1_p0 v1_csc1_p0;                                               /* 0x1502c */
    volatile u_v1_csc1_p1 v1_csc1_p1;                                               /* 0x15030 */
    volatile u_v1_csc1_p2 v1_csc1_p2;                                               /* 0x15034 */
    volatile u_v1_csc1_p3 v1_csc1_p3;                                               /* 0x15038 */
    volatile u_v1_csc1_p4 v1_csc1_p4;                                               /* 0x1503c */
    volatile unsigned int reserved_209[48];                                         /* 48:0x15040~0x150fc */
    volatile u_v2_csc_idc v2_csc_idc;                                               /* 0x15100 */
    volatile u_v2_csc_odc v2_csc_odc;                                               /* 0x15104 */
    volatile u_v2_csc_iodc v2_csc_iodc;                                             /* 0x15108 */
    volatile u_v2_csc_p0 v2_csc_p0;                                                 /* 0x1510c */
    volatile u_v2_csc_p1 v2_csc_p1;                                                 /* 0x15110 */
    volatile u_v2_csc_p2 v2_csc_p2;                                                 /* 0x15114 */
    volatile u_v2_csc_p3 v2_csc_p3;                                                 /* 0x15118 */
    volatile u_v2_csc_p4 v2_csc_p4;                                                 /* 0x1511c */
    volatile u_v2_csc1_idc v2_csc1_idc;                                             /* 0x15120 */
    volatile u_v2_csc1_odc v2_csc1_odc;                                             /* 0x15124 */
    volatile u_v2_csc1_iodc v2_csc1_iodc;                                           /* 0x15128 */
    volatile u_v2_csc1_p0 v2_csc1_p0;                                               /* 0x1512c */
    volatile u_v2_csc1_p1 v2_csc1_p1;                                               /* 0x15130 */
    volatile u_v2_csc1_p2 v2_csc1_p2;                                               /* 0x15134 */
    volatile u_v2_csc1_p3 v2_csc1_p3;                                               /* 0x15138 */
    volatile u_v2_csc1_p4 v2_csc1_p4;                                               /* 0x1513c */
    volatile unsigned int reserved_210[48];                                         /* 48:0x15140~0x151fc */
    volatile u_g1_csc_idc g1_csc_idc;                                               /* 0x15200 */
    volatile u_g1_csc_odc g1_csc_odc;                                               /* 0x15204 */
    volatile u_g1_csc_iodc g1_csc_iodc;                                             /* 0x15208 */
    volatile u_g1_csc_p0 g1_csc_p0;                                                 /* 0x1520c */
    volatile u_g1_csc_p1 g1_csc_p1;                                                 /* 0x15210 */
    volatile u_g1_csc_p2 g1_csc_p2;                                                 /* 0x15214 */
    volatile u_g1_csc_p3 g1_csc_p3;                                                 /* 0x15218 */
    volatile u_g1_csc_p4 g1_csc_p4;                                                 /* 0x1521c */
    volatile u_g1_csc1_idc g1_csc1_idc;                                             /* 0x15220 */
    volatile u_g1_csc1_odc g1_csc1_odc;                                             /* 0x15224 */
    volatile u_g1_csc1_iodc g1_csc1_iodc;                                           /* 0x15228 */
    volatile u_g1_csc1_p0 g1_csc1_p0;                                               /* 0x1522c */
    volatile u_g1_csc1_p1 g1_csc1_p1;                                               /* 0x15230 */
    volatile u_g1_csc1_p2 g1_csc1_p2;                                               /* 0x15234 */
    volatile u_g1_csc1_p3 g1_csc1_p3;                                               /* 0x15238 */
    volatile u_g1_csc1_p4 g1_csc1_p4;                                               /* 0x1523c */
    volatile unsigned int reserved_211[48];                                         /* 48:0x15240~0x152fc */
    volatile u_g3_csc_idc g3_csc_idc;                                               /* 0x15300 */
    volatile u_g3_csc_odc g3_csc_odc;                                               /* 0x15304 */
    volatile u_g3_csc_iodc g3_csc_iodc;                                             /* 0x15308 */
    volatile u_g3_csc_p0 g3_csc_p0;                                                 /* 0x1530c */
    volatile u_g3_csc_p1 g3_csc_p1;                                                 /* 0x15310 */
    volatile u_g3_csc_p2 g3_csc_p2;                                                 /* 0x15314 */
    volatile u_g3_csc_p3 g3_csc_p3;                                                 /* 0x15318 */
    volatile u_g3_csc_p4 g3_csc_p4;                                                 /* 0x1531c */
    volatile u_g3_csc1_idc g3_csc1_idc;                                             /* 0x15320 */
    volatile u_g3_csc1_odc g3_csc1_odc;                                             /* 0x15324 */
    volatile u_g3_csc1_iodc g3_csc1_iodc;                                           /* 0x15328 */
    volatile u_g3_csc1_p0 g3_csc1_p0;                                               /* 0x1532c */
    volatile u_g3_csc1_p1 g3_csc1_p1;                                               /* 0x15330 */
    volatile u_g3_csc1_p2 g3_csc1_p2;                                               /* 0x15334 */
    volatile u_g3_csc1_p3 g3_csc1_p3;                                               /* 0x15338 */
    volatile u_g3_csc1_p4 g3_csc1_p4;                                               /* 0x1533c */
    volatile unsigned int reserved_212[48];                                         /* 48:0x15340~0x153fc */
    volatile u_v0_cvfir_vinfo v0_cvfir_vinfo;                                       /* 0x15400 */
    volatile u_v0_cvfir_vsp v0_cvfir_vsp;                                           /* 0x15404 */
    volatile u_v0_cvfir_voffset v0_cvfir_voffset;                                   /* 0x15408 */
    volatile u_v0_cvfir_vboffset v0_cvfir_vboffset;                                 /* 0x1540c */
    volatile unsigned int reserved_213[8];                                          /* 8:0x15410~0x1542c */
    volatile u_v0_cvfir_vcoef0 v0_cvfir_vcoef0;                                     /* 0x15430 */
    volatile u_v0_cvfir_vcoef1 v0_cvfir_vcoef1;                                     /* 0x15434 */
    volatile u_v0_cvfir_vcoef2 v0_cvfir_vcoef2;                                     /* 0x15438 */
    volatile unsigned int reserved_214[721];                                        /* 721:0x1543c~0x15f7c */
    volatile u_gfx_osd_glb_info gfx_osd_glb_info;                                   /* 0x15f80 */
    volatile u_gfx_osd_frame_size gfx_osd_frame_size;                               /* 0x15f84 */
    volatile unsigned int reserved_215[2];                                          /* 2:0x15f88~0x15f8c */
    volatile u_gfx_osd_dbg_reg gfx_osd_dbg_reg;                                     /* 0x15f90 */
    volatile u_gfx_osd_dbg_reg1 gfx_osd_dbg_reg1;                                   /* 0x15f94 */
} vdp_regs_type;


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART43_H_ */
