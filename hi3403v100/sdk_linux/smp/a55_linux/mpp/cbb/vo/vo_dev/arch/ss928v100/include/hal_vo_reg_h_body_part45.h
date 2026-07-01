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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART45_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART45_H_

    volatile unsigned int v3_ot_pp_csc_ink_data2;                                    /* 0x42fc */
    volatile unsigned int reserved_54[64];                                          /* 0x4300~0x43fc 64 regs */
    volatile reg_v3_cvfir_vinfo v3_cvfir_vinfo;                                     /* 0x4400 */
    volatile reg_v3_cvfir_vsp v3_cvfir_vsp;                                         /* 0x4404 */
    volatile reg_v3_cvfir_voffset v3_cvfir_voffset;                                 /* 0x4408 */
    volatile reg_v3_cvfir_vboffset v3_cvfir_vboffset;                               /* 0x440c */
    volatile unsigned int reserved_55[8];                                           /* 0x4410~0x442c 8 regs */
    volatile reg_v3_cvfir_vcoef0 v3_cvfir_vcoef0;                                   /* 0x4430 */
    volatile reg_v3_cvfir_vcoef1 v3_cvfir_vcoef1;                                   /* 0x4434 */
    volatile reg_v3_cvfir_vcoef2 v3_cvfir_vcoef2;                                   /* 0x4438 */
    volatile unsigned int reserved_56[49];                                          /* 0x443c~0x44fc 49 regs */
    volatile reg_v3_hfir_ctrl v3_hfir_ctrl;                                         /* 0x4500 */
    volatile reg_v3_hfircoef01 v3_hfircoef01;                                       /* 0x4504 */
    volatile reg_v3_hfircoef23 v3_hfircoef23;                                       /* 0x4508 */
    volatile reg_v3_hfircoef45 v3_hfircoef45;                                       /* 0x450c */
    volatile reg_v3_hfircoef67 v3_hfircoef67;                                       /* 0x4510 */
    volatile unsigned int reserved_57[1211];                                        /* 0x4514~0x57fc 1211 regs */
    volatile unsigned int vp0_ctrl;                                                 /* 0x5800 */
    volatile reg_vp0_upd vp0_upd;                                                   /* 0x5804 */
    volatile reg_vp0_ireso vp0_ireso;                                               /* 0x5808 */
    volatile unsigned int reserved_58[29];                                          /* 0x580c~0x587c 29 regs */
    volatile reg_vp0_lbox_ctrl vp0_lbox_ctrl;                                       /* 0x5880 */
    volatile reg_vp0_galpha vp0_galpha;                                             /* 0x5884 */
    volatile reg_vp0_dfpos vp0_dfpos;                                               /* 0x5888 */
    volatile reg_vp0_dlpos vp0_dlpos;                                               /* 0x588c */
    volatile reg_vp0_vfpos vp0_vfpos;                                               /* 0x5890 */
    volatile reg_vp0_vlpos vp0_vlpos;                                               /* 0x5894 */
    volatile reg_vp0_bk vp0_bk;                                                     /* 0x5898 */
    volatile reg_vp0_alpha vp0_alpha;                                               /* 0x589c */
    volatile reg_vp0_mute_bk vp0_mute_bk;                                           /* 0x58a0 */
    volatile unsigned int reserved_59[1495];                                        /* 0x58a4~0x6ffc 1495 regs */
    volatile reg_g0_ctrl g0_ctrl;                                                   /* 0x7000 */
    volatile reg_g0_upd g0_upd;                                                     /* 0x7004 */
    volatile unsigned int g0_galpha_sum;                                            /* 0x7008 */
    volatile reg_g0_0reso_read g0_0reso_read;                                       /* 0x700c */
    volatile reg_g0_ireso g0_ireso;                                                 /* 0x7010 */
    volatile unsigned int reserved_60[27];                                          /* 0x7014~0x707c 27 regs */
    volatile reg_g0_dfpos g0_dfpos;                                                 /* 0x7080 */
    volatile reg_g0_dlpos g0_dlpos;                                                 /* 0x7084 */
    volatile reg_g0_vfpos g0_vfpos;                                                 /* 0x7088 */
    volatile reg_g0_vlpos g0_vlpos;                                                 /* 0x708c */
    volatile reg_g0_bk g0_bk;                                                       /* 0x7090 */
    volatile reg_g0_alpha g0_alpha;                                                 /* 0x7094 */
    volatile reg_g0_mute_bk g0_mute_bk;                                             /* 0x7098 */
    volatile reg_g0_lbox_ctrl g0_lbox_ctrl;                                         /* 0x709c */
    volatile unsigned int reserved_61[24];                                          /* 0x70a0~0x70fc 24 regs */
    volatile reg_g0_ot_pp_csc_ctrl g0_ot_pp_csc_ctrl;                                 /* 0x7100 */
    volatile reg_g0_ot_pp_csc_coef00 g0_ot_pp_csc_coef00;                             /* 0x7104 */
    volatile reg_g0_ot_pp_csc_coef01 g0_ot_pp_csc_coef01;                             /* 0x7108 */
    volatile reg_g0_ot_pp_csc_coef02 g0_ot_pp_csc_coef02;                             /* 0x710c */
    volatile reg_g0_ot_pp_csc_coef10 g0_ot_pp_csc_coef10;                             /* 0x7110 */
    volatile reg_g0_ot_pp_csc_coef11 g0_ot_pp_csc_coef11;                             /* 0x7114 */
    volatile reg_g0_ot_pp_csc_coef12 g0_ot_pp_csc_coef12;                             /* 0x7118 */
    volatile reg_g0_ot_pp_csc_coef20 g0_ot_pp_csc_coef20;                             /* 0x711c */
    volatile reg_g0_ot_pp_csc_coef21 g0_ot_pp_csc_coef21;                             /* 0x7120 */
    volatile reg_g0_ot_pp_csc_coef22 g0_ot_pp_csc_coef22;                             /* 0x7124 */
    volatile reg_g0_ot_pp_csc_scale g0_ot_pp_csc_scale;                               /* 0x7128 */
    volatile reg_g0_ot_pp_csc_idc0 g0_ot_pp_csc_idc0;                                 /* 0x712c */
    volatile reg_g0_ot_pp_csc_idc1 g0_ot_pp_csc_idc1;                                 /* 0x7130 */
    volatile reg_g0_ot_pp_csc_idc2 g0_ot_pp_csc_idc2;                                 /* 0x7134 */
    volatile reg_g0_ot_pp_csc_odc0 g0_ot_pp_csc_odc0;                                 /* 0x7138 */
    volatile reg_g0_ot_pp_csc_odc1 g0_ot_pp_csc_odc1;                                 /* 0x713c */
    volatile reg_g0_ot_pp_csc_odc2 g0_ot_pp_csc_odc2;                                 /* 0x7140 */
    volatile reg_g0_ot_pp_csc_min_y g0_ot_pp_csc_min_y;                               /* 0x7144 */
    volatile reg_g0_ot_pp_csc_min_c g0_ot_pp_csc_min_c;                               /* 0x7148 */
    volatile reg_g0_ot_pp_csc_max_y g0_ot_pp_csc_max_y;                               /* 0x714c */
    volatile reg_g0_ot_pp_csc_max_c g0_ot_pp_csc_max_c;                               /* 0x7150 */
    volatile reg_g0_ot_pp_csc2_coef00 g0_ot_pp_csc2_coef00;                           /* 0x7154 */
    volatile reg_g0_ot_pp_csc2_coef01 g0_ot_pp_csc2_coef01;                           /* 0x7158 */
    volatile reg_g0_ot_pp_csc2_coef02 g0_ot_pp_csc2_coef02;                           /* 0x715c */
    volatile reg_g0_ot_pp_csc2_coef10 g0_ot_pp_csc2_coef10;                           /* 0x7160 */
    volatile reg_g0_ot_pp_csc2_coef11 g0_ot_pp_csc2_coef11;                           /* 0x7164 */
    volatile reg_g0_ot_pp_csc2_coef12 g0_ot_pp_csc2_coef12;                           /* 0x7168 */
    volatile reg_g0_ot_pp_csc2_coef20 g0_ot_pp_csc2_coef20;                           /* 0x716c */
    volatile reg_g0_ot_pp_csc2_coef21 g0_ot_pp_csc2_coef21;                           /* 0x7170 */
    volatile reg_g0_ot_pp_csc2_coef22 g0_ot_pp_csc2_coef22;                           /* 0x7174 */
    volatile reg_g0_ot_pp_csc2_scale g0_ot_pp_csc2_scale;                             /* 0x7178 */
    volatile reg_g0_ot_pp_csc2_idc0 g0_ot_pp_csc2_idc0;                               /* 0x717c */
    volatile reg_g0_ot_pp_csc2_idc1 g0_ot_pp_csc2_idc1;                               /* 0x7180 */
    volatile reg_g0_ot_pp_csc2_idc2 g0_ot_pp_csc2_idc2;                               /* 0x7184 */
    volatile reg_g0_ot_pp_csc2_odc0 g0_ot_pp_csc2_odc0;                               /* 0x7188 */
    volatile reg_g0_ot_pp_csc2_odc1 g0_ot_pp_csc2_odc1;                               /* 0x718c */
    volatile reg_g0_ot_pp_csc2_odc2 g0_ot_pp_csc2_odc2;                               /* 0x7190 */
    volatile reg_g0_ot_pp_csc2_min_y g0_ot_pp_csc2_min_y;                             /* 0x7194 */
    volatile reg_g0_ot_pp_csc2_min_c g0_ot_pp_csc2_min_c;                             /* 0x7198 */
    volatile reg_g0_ot_pp_csc2_max_y g0_ot_pp_csc2_max_y;                             /* 0x719c */
    volatile reg_g0_ot_pp_csc2_max_c g0_ot_pp_csc2_max_c;                             /* 0x71a0 */
    volatile unsigned int reserved_62[19];                                          /* 0x71a4~0x71ec 19 regs */
    volatile reg_g0_ot_pp_csc_ink_ctrl g0_ot_pp_csc_ink_ctrl;                         /* 0x71f0 */
    volatile reg_g0_ot_pp_csc_ink_pos g0_ot_pp_csc_ink_pos;                           /* 0x71f4 */
    volatile unsigned int g0_ot_pp_csc_ink_data;                                     /* 0x71f8 */
    volatile unsigned int g0_ot_pp_csc_ink_data2;                                    /* 0x71fc */
    volatile reg_osb_mute_bk osb_mute_bk;                                           /* 0x7200 */
    volatile reg_osb_bk_alpha osb_bk_alpha;                                         /* 0x7204 */
    volatile reg_osb_coef_rd_en osb_coef_rd_en;                                     /* 0x7208 */
    volatile unsigned int osb_coef_rd_addr;                                         /* 0x720c */
    volatile unsigned int reserved_63[60];                                          /* 0x7210~0x72fc 60 regs */
    volatile reg_g0_zme_hinfo g0_zme_hinfo;                                         /* 0x7300 */
    volatile reg_g0_zme_hsp g0_zme_hsp;                                             /* 0x7304 */
    volatile reg_g0_zme_hloffset g0_zme_hloffset;                                   /* 0x7308 */
    volatile reg_g0_zme_hcoffset g0_zme_hcoffset;                                   /* 0x730c */
    volatile unsigned int reserved_64[5];                                           /* 0x7310~0x7320 5 regs */
    volatile reg_g0_zme_coef_ren g0_zme_coef_ren;                                   /* 0x7324 */
    volatile reg_g0_zme_coef_rdata g0_zme_coef_rdata;                               /* 0x7328 */
    volatile unsigned int reserved_65[21];                                          /* 0x732c~0x737c 21 regs */
    volatile reg_g0_zme_vinfo g0_zme_vinfo;                                         /* 0x7380 */
    volatile reg_g0_zme_vsp g0_zme_vsp;                                             /* 0x7384 */
    volatile reg_g0_zme_voffset g0_zme_voffset;                                     /* 0x7388 */
    volatile unsigned int reserved_66[285];                                         /* 0x738c~0x77fc 285 regs */
    volatile reg_g1_ctrl g1_ctrl;                                                   /* 0x7800 */
    volatile reg_g1_upd g1_upd;                                                     /* 0x7804 */
    volatile unsigned int g1_galpha_sum;                                            /* 0x7808 */
    volatile reg_g1_0reso_read g1_0reso_read;                                       /* 0x780c */
    volatile reg_g1_ireso g1_ireso;                                                 /* 0x7810 */
    volatile unsigned int reserved_67[27];                                          /* 0x7814~0x787c 27 regs */
    volatile reg_g1_dfpos g1_dfpos;                                                 /* 0x7880 */
    volatile reg_g1_dlpos g1_dlpos;                                                 /* 0x7884 */
    volatile reg_g1_vfpos g1_vfpos;                                                 /* 0x7888 */
    volatile reg_g1_vlpos g1_vlpos;                                                 /* 0x788c */
    volatile reg_g1_bk g1_bk;                                                       /* 0x7890 */
    volatile reg_g1_alpha g1_alpha;                                                 /* 0x7894 */
    volatile reg_g1_mute_bk g1_mute_bk;                                             /* 0x7898 */
    volatile reg_g1_lbox_ctrl g1_lbox_ctrl;                                         /* 0x789c */
    volatile unsigned int reserved_68[24];                                          /* 0x78a0~0x78fc 24 regs */
    volatile reg_g1_ot_pp_csc_ctrl g1_ot_pp_csc_ctrl;                                 /* 0x7900 */
    volatile reg_g1_ot_pp_csc_coef00 g1_ot_pp_csc_coef00;                             /* 0x7904 */
    volatile reg_g1_ot_pp_csc_coef01 g1_ot_pp_csc_coef01;                             /* 0x7908 */
    volatile reg_g1_ot_pp_csc_coef02 g1_ot_pp_csc_coef02;                             /* 0x790c */
    volatile reg_g1_ot_pp_csc_coef10 g1_ot_pp_csc_coef10;                             /* 0x7910 */
    volatile reg_g1_ot_pp_csc_coef11 g1_ot_pp_csc_coef11;                             /* 0x7914 */
    volatile reg_g1_ot_pp_csc_coef12 g1_ot_pp_csc_coef12;                             /* 0x7918 */
    volatile reg_g1_ot_pp_csc_coef20 g1_ot_pp_csc_coef20;                             /* 0x791c */
    volatile reg_g1_ot_pp_csc_coef21 g1_ot_pp_csc_coef21;                             /* 0x7920 */
    volatile reg_g1_ot_pp_csc_coef22 g1_ot_pp_csc_coef22;                             /* 0x7924 */
    volatile reg_g1_ot_pp_csc_scale g1_ot_pp_csc_scale;                               /* 0x7928 */
    volatile reg_g1_ot_pp_csc_idc0 g1_ot_pp_csc_idc0;                                 /* 0x792c */
    volatile reg_g1_ot_pp_csc_idc1 g1_ot_pp_csc_idc1;                                 /* 0x7930 */
    volatile reg_g1_ot_pp_csc_idc2 g1_ot_pp_csc_idc2;                                 /* 0x7934 */
    volatile reg_g1_ot_pp_csc_odc0 g1_ot_pp_csc_odc0;                                 /* 0x7938 */
    volatile reg_g1_ot_pp_csc_odc1 g1_ot_pp_csc_odc1;                                 /* 0x793c */
    volatile reg_g1_ot_pp_csc_odc2 g1_ot_pp_csc_odc2;                                 /* 0x7940 */
    volatile reg_g1_ot_pp_csc_min_y g1_ot_pp_csc_min_y;                               /* 0x7944 */
    volatile reg_g1_ot_pp_csc_min_c g1_ot_pp_csc_min_c;                               /* 0x7948 */
    volatile reg_g1_ot_pp_csc_max_y g1_ot_pp_csc_max_y;                               /* 0x794c */
    volatile reg_g1_ot_pp_csc_max_c g1_ot_pp_csc_max_c;                               /* 0x7950 */
    volatile reg_g1_ot_pp_csc2_coef00 g1_ot_pp_csc2_coef00;                           /* 0x7954 */
    volatile reg_g1_ot_pp_csc2_coef01 g1_ot_pp_csc2_coef01;                           /* 0x7958 */
    volatile reg_g1_ot_pp_csc2_coef02 g1_ot_pp_csc2_coef02;                           /* 0x795c */
    volatile reg_g1_ot_pp_csc2_coef10 g1_ot_pp_csc2_coef10;                           /* 0x7960 */
    volatile reg_g1_ot_pp_csc2_coef11 g1_ot_pp_csc2_coef11;                           /* 0x7964 */
    volatile reg_g1_ot_pp_csc2_coef12 g1_ot_pp_csc2_coef12;                           /* 0x7968 */
    volatile reg_g1_ot_pp_csc2_coef20 g1_ot_pp_csc2_coef20;                           /* 0x796c */
    volatile reg_g1_ot_pp_csc2_coef21 g1_ot_pp_csc2_coef21;                           /* 0x7970 */
    volatile reg_g1_ot_pp_csc2_coef22 g1_ot_pp_csc2_coef22;                           /* 0x7974 */
    volatile reg_g1_ot_pp_csc2_scale g1_ot_pp_csc2_scale;                             /* 0x7978 */
    volatile reg_g1_ot_pp_csc2_idc0 g1_ot_pp_csc2_idc0;                               /* 0x797c */
    volatile reg_g1_ot_pp_csc2_idc1 g1_ot_pp_csc2_idc1;                               /* 0x7980 */
    volatile reg_g1_ot_pp_csc2_idc2 g1_ot_pp_csc2_idc2;                               /* 0x7984 */
    volatile reg_g1_ot_pp_csc2_odc0 g1_ot_pp_csc2_odc0;                               /* 0x7988 */
    volatile reg_g1_ot_pp_csc2_odc1 g1_ot_pp_csc2_odc1;                               /* 0x798c */
    volatile reg_g1_ot_pp_csc2_odc2 g1_ot_pp_csc2_odc2;                               /* 0x7990 */
    volatile reg_g1_ot_pp_csc2_min_y g1_ot_pp_csc2_min_y;                             /* 0x7994 */
    volatile reg_g1_ot_pp_csc2_min_c g1_ot_pp_csc2_min_c;                             /* 0x7998 */
    volatile reg_g1_ot_pp_csc2_max_y g1_ot_pp_csc2_max_y;                             /* 0x799c */
    volatile reg_g1_ot_pp_csc2_max_c g1_ot_pp_csc2_max_c;                             /* 0x79a0 */
    volatile unsigned int reserved_69[19];                                          /* 0x79a4~0x79ec 19 regs */
    volatile reg_g1_ot_pp_csc_ink_ctrl g1_ot_pp_csc_ink_ctrl;                         /* 0x79f0 */
    volatile reg_g1_ot_pp_csc_ink_pos g1_ot_pp_csc_ink_pos;                           /* 0x79f4 */
    volatile unsigned int g1_ot_pp_csc_ink_data;                                     /* 0x79f8 */
    volatile unsigned int g1_ot_pp_csc_ink_data2;                                    /* 0x79fc */
    volatile reg_g1_osb_mute_bk g1_osb_mute_bk;                                     /* 0x7a00 */
    volatile reg_g1_osb_bk_alpha g1_osb_bk_alpha;                                   /* 0x7a04 */
    volatile reg_g1_osb_coef_rd_en g1_osb_coef_rd_en;                               /* 0x7a08 */
    volatile unsigned int g1_osb_coef_rd_addr;                                      /* 0x7a0c */
    volatile unsigned int reserved_70[60];                                          /* 0x7a10~0x7afc 60 regs */
    volatile reg_g1_zme_hinfo g1_zme_hinfo;                                         /* 0x7b00 */
    volatile reg_g1_zme_hsp g1_zme_hsp;                                             /* 0x7b04 */
    volatile reg_g1_zme_hloffset g1_zme_hloffset;                                   /* 0x7b08 */
    volatile reg_g1_zme_hcoffset g1_zme_hcoffset;                                   /* 0x7b0c */
    volatile unsigned int reserved_71[5];                                           /* 0x7b10~0x7b20 5 regs */
    volatile reg_g1_zme_coef_ren g1_zme_coef_ren;                                   /* 0x7b24 */
    volatile reg_g1_zme_coef_rdata g1_zme_coef_rdata;                               /* 0x7b28 */
    volatile unsigned int reserved_72[21];                                          /* 0x7b2c~0x7b7c 21 regs */
    volatile reg_g1_zme_vinfo g1_zme_vinfo;                                         /* 0x7b80 */
    volatile reg_g1_zme_vsp g1_zme_vsp;                                             /* 0x7b84 */
    volatile reg_g1_zme_voffset g1_zme_voffset;                                     /* 0x7b88 */
    volatile unsigned int reserved_73[285];                                         /* 0x7b8c~0x7ffc 285 regs */
    volatile reg_g2_ctrl g2_ctrl;                                                   /* 0x8000 */
    volatile reg_g2_upd g2_upd;                                                     /* 0x8004 */
    volatile unsigned int g2_galpha_sum;                                            /* 0x8008 */
    volatile reg_g2_0reso_read g2_0reso_read;                                       /* 0x800c */
    volatile reg_g2_ireso g2_ireso;                                                 /* 0x8010 */
    volatile unsigned int reserved_74[27];                                          /* 0x8014~0x807c 27 regs */
    volatile reg_g2_dfpos g2_dfpos;                                                 /* 0x8080 */
    volatile reg_g2_dlpos g2_dlpos;                                                 /* 0x8084 */
    volatile reg_g2_vfpos g2_vfpos;                                                 /* 0x8088 */
    volatile reg_g2_vlpos g2_vlpos;                                                 /* 0x808c */
    volatile reg_g2_bk g2_bk;                                                       /* 0x8090 */
    volatile reg_g2_alpha g2_alpha;                                                 /* 0x8094 */
    volatile reg_g2_mute_bk g2_mute_bk;                                             /* 0x8098 */
    volatile reg_g2_lbox_ctrl g2_lbox_ctrl;                                         /* 0x809c */
    volatile unsigned int reserved_75[24];                                          /* 0x80a0~0x80fc 24 regs */
    volatile reg_g2_ot_pp_csc_ctrl g2_ot_pp_csc_ctrl;                                 /* 0x8100 */
    volatile reg_g2_ot_pp_csc_coef00 g2_ot_pp_csc_coef00;                             /* 0x8104 */
    volatile reg_g2_ot_pp_csc_coef01 g2_ot_pp_csc_coef01;                             /* 0x8108 */
    volatile reg_g2_ot_pp_csc_coef02 g2_ot_pp_csc_coef02;                             /* 0x810c */
    volatile reg_g2_ot_pp_csc_coef10 g2_ot_pp_csc_coef10;                             /* 0x8110 */
    volatile reg_g2_ot_pp_csc_coef11 g2_ot_pp_csc_coef11;                             /* 0x8114 */
    volatile reg_g2_ot_pp_csc_coef12 g2_ot_pp_csc_coef12;                             /* 0x8118 */
    volatile reg_g2_ot_pp_csc_coef20 g2_ot_pp_csc_coef20;                             /* 0x811c */
    volatile reg_g2_ot_pp_csc_coef21 g2_ot_pp_csc_coef21;                             /* 0x8120 */
    volatile reg_g2_ot_pp_csc_coef22 g2_ot_pp_csc_coef22;                             /* 0x8124 */
    volatile reg_g2_ot_pp_csc_scale g2_ot_pp_csc_scale;                               /* 0x8128 */
    volatile reg_g2_ot_pp_csc_idc0 g2_ot_pp_csc_idc0;                                 /* 0x812c */
    volatile reg_g2_ot_pp_csc_idc1 g2_ot_pp_csc_idc1;                                 /* 0x8130 */
    volatile reg_g2_ot_pp_csc_idc2 g2_ot_pp_csc_idc2;                                 /* 0x8134 */
    volatile reg_g2_ot_pp_csc_odc0 g2_ot_pp_csc_odc0;                                 /* 0x8138 */
    volatile reg_g2_ot_pp_csc_odc1 g2_ot_pp_csc_odc1;                                 /* 0x813c */
    volatile reg_g2_ot_pp_csc_odc2 g2_ot_pp_csc_odc2;                                 /* 0x8140 */
    volatile reg_g2_ot_pp_csc_min_y g2_ot_pp_csc_min_y;                               /* 0x8144 */
    volatile reg_g2_ot_pp_csc_min_c g2_ot_pp_csc_min_c;                               /* 0x8148 */
    volatile reg_g2_ot_pp_csc_max_y g2_ot_pp_csc_max_y;                               /* 0x814c */
    volatile reg_g2_ot_pp_csc_max_c g2_ot_pp_csc_max_c;                               /* 0x8150 */
    volatile reg_g2_ot_pp_csc2_coef00 g2_ot_pp_csc2_coef00;                           /* 0x8154 */
    volatile reg_g2_ot_pp_csc2_coef01 g2_ot_pp_csc2_coef01;                           /* 0x8158 */
    volatile reg_g2_ot_pp_csc2_coef02 g2_ot_pp_csc2_coef02;                           /* 0x815c */
    volatile reg_g2_ot_pp_csc2_coef10 g2_ot_pp_csc2_coef10;                           /* 0x8160 */
    volatile reg_g2_ot_pp_csc2_coef11 g2_ot_pp_csc2_coef11;                           /* 0x8164 */
    volatile reg_g2_ot_pp_csc2_coef12 g2_ot_pp_csc2_coef12;                           /* 0x8168 */
    volatile reg_g2_ot_pp_csc2_coef20 g2_ot_pp_csc2_coef20;                           /* 0x816c */
    volatile reg_g2_ot_pp_csc2_coef21 g2_ot_pp_csc2_coef21;                           /* 0x8170 */
    volatile reg_g2_ot_pp_csc2_coef22 g2_ot_pp_csc2_coef22;                           /* 0x8174 */
    volatile reg_g2_ot_pp_csc2_scale g2_ot_pp_csc2_scale;                             /* 0x8178 */
    volatile reg_g2_ot_pp_csc2_idc0 g2_ot_pp_csc2_idc0;                               /* 0x817c */
    volatile reg_g2_ot_pp_csc2_idc1 g2_ot_pp_csc2_idc1;                               /* 0x8180 */
    volatile reg_g2_ot_pp_csc2_idc2 g2_ot_pp_csc2_idc2;                               /* 0x8184 */
    volatile reg_g2_ot_pp_csc2_odc0 g2_ot_pp_csc2_odc0;                               /* 0x8188 */
    volatile reg_g2_ot_pp_csc2_odc1 g2_ot_pp_csc2_odc1;                               /* 0x818c */
    volatile reg_g2_ot_pp_csc2_odc2 g2_ot_pp_csc2_odc2;                               /* 0x8190 */
    volatile reg_g2_ot_pp_csc2_min_y g2_ot_pp_csc2_min_y;                             /* 0x8194 */
    volatile reg_g2_ot_pp_csc2_min_c g2_ot_pp_csc2_min_c;                             /* 0x8198 */
    volatile reg_g2_ot_pp_csc2_max_y g2_ot_pp_csc2_max_y;                             /* 0x819c */
    volatile reg_g2_ot_pp_csc2_max_c g2_ot_pp_csc2_max_c;                             /* 0x81a0 */
    volatile unsigned int reserved_76[19];                                          /* 0x81a4~0x81ec 19 regs */
    volatile reg_g2_ot_pp_csc_ink_ctrl g2_ot_pp_csc_ink_ctrl;                         /* 0x81f0 */
    volatile reg_g2_ot_pp_csc_ink_pos g2_ot_pp_csc_ink_pos;                           /* 0x81f4 */
    volatile unsigned int g2_ot_pp_csc_ink_data;                                     /* 0x81f8 */
    volatile unsigned int g2_ot_pp_csc_ink_data2;                                    /* 0x81fc */
    volatile unsigned int reserved_77[384];                                         /* 0x8200~0x87fc 384 regs */
    volatile reg_g3_ctrl g3_ctrl;                                                   /* 0x8800 */
    volatile reg_g3_upd g3_upd;                                                     /* 0x8804 */
    volatile unsigned int g3_galpha_sum;                                            /* 0x8808 */
    volatile reg_g3_0reso_read g3_0reso_read;                                       /* 0x880c */
    volatile reg_g3_ireso g3_ireso;                                                 /* 0x8810 */
    volatile unsigned int reserved_78[27];                                          /* 0x8814~0x887c 27 regs */
    volatile reg_g3_dfpos g3_dfpos;                                                 /* 0x8880 */
    volatile reg_g3_dlpos g3_dlpos;                                                 /* 0x8884 */
    volatile reg_g3_vfpos g3_vfpos;                                                 /* 0x8888 */
    volatile reg_g3_vlpos g3_vlpos;                                                 /* 0x888c */
    volatile reg_g3_bk g3_bk;                                                       /* 0x8890 */
    volatile reg_g3_alpha g3_alpha;                                                 /* 0x8894 */
    volatile reg_g3_mute_bk g3_mute_bk;                                             /* 0x8898 */
    volatile reg_g3_lbox_ctrl g3_lbox_ctrl;                                         /* 0x889c */
    volatile unsigned int reserved_79[24];                                          /* 0x88a0~0x88fc 24 regs */
    volatile reg_g3_ot_pp_csc_ctrl g3_ot_pp_csc_ctrl;                                 /* 0x8900 */
    volatile reg_g3_ot_pp_csc_coef00 g3_ot_pp_csc_coef00;                             /* 0x8904 */
    volatile reg_g3_ot_pp_csc_coef01 g3_ot_pp_csc_coef01;                             /* 0x8908 */
    volatile reg_g3_ot_pp_csc_coef02 g3_ot_pp_csc_coef02;                             /* 0x890c */
    volatile reg_g3_ot_pp_csc_coef10 g3_ot_pp_csc_coef10;                             /* 0x8910 */
    volatile reg_g3_ot_pp_csc_coef11 g3_ot_pp_csc_coef11;                             /* 0x8914 */
    volatile reg_g3_ot_pp_csc_coef12 g3_ot_pp_csc_coef12;                             /* 0x8918 */
    volatile reg_g3_ot_pp_csc_coef20 g3_ot_pp_csc_coef20;                             /* 0x891c */
    volatile reg_g3_ot_pp_csc_coef21 g3_ot_pp_csc_coef21;                             /* 0x8920 */
    volatile reg_g3_ot_pp_csc_coef22 g3_ot_pp_csc_coef22;                             /* 0x8924 */
    volatile reg_g3_ot_pp_csc_scale g3_ot_pp_csc_scale;                               /* 0x8928 */
    volatile reg_g3_ot_pp_csc_idc0 g3_ot_pp_csc_idc0;                                 /* 0x892c */
    volatile reg_g3_ot_pp_csc_idc1 g3_ot_pp_csc_idc1;                                 /* 0x8930 */
    volatile reg_g3_ot_pp_csc_idc2 g3_ot_pp_csc_idc2;                                 /* 0x8934 */
    volatile reg_g3_ot_pp_csc_odc0 g3_ot_pp_csc_odc0;                                 /* 0x8938 */
    volatile reg_g3_ot_pp_csc_odc1 g3_ot_pp_csc_odc1;                                 /* 0x893c */
    volatile reg_g3_ot_pp_csc_odc2 g3_ot_pp_csc_odc2;                                 /* 0x8940 */
    volatile reg_g3_ot_pp_csc_min_y g3_ot_pp_csc_min_y;                               /* 0x8944 */
    volatile reg_g3_ot_pp_csc_min_c g3_ot_pp_csc_min_c;                               /* 0x8948 */
    volatile reg_g3_ot_pp_csc_max_y g3_ot_pp_csc_max_y;                               /* 0x894c */
    volatile reg_g3_ot_pp_csc_max_c g3_ot_pp_csc_max_c;                               /* 0x8950 */
    volatile reg_g3_ot_pp_csc2_coef00 g3_ot_pp_csc2_coef00;                           /* 0x8954 */
    volatile reg_g3_ot_pp_csc2_coef01 g3_ot_pp_csc2_coef01;                           /* 0x8958 */
    volatile reg_g3_ot_pp_csc2_coef02 g3_ot_pp_csc2_coef02;                           /* 0x895c */
    volatile reg_g3_ot_pp_csc2_coef10 g3_ot_pp_csc2_coef10;                           /* 0x8960 */
    volatile reg_g3_ot_pp_csc2_coef11 g3_ot_pp_csc2_coef11;                           /* 0x8964 */
    volatile reg_g3_ot_pp_csc2_coef12 g3_ot_pp_csc2_coef12;                           /* 0x8968 */
    volatile reg_g3_ot_pp_csc2_coef20 g3_ot_pp_csc2_coef20;                           /* 0x896c */
    volatile reg_g3_ot_pp_csc2_coef21 g3_ot_pp_csc2_coef21;                           /* 0x8970 */
    volatile reg_g3_ot_pp_csc2_coef22 g3_ot_pp_csc2_coef22;                           /* 0x8974 */
    volatile reg_g3_ot_pp_csc2_scale g3_ot_pp_csc2_scale;                             /* 0x8978 */
    volatile reg_g3_ot_pp_csc2_idc0 g3_ot_pp_csc2_idc0;                               /* 0x897c */
    volatile reg_g3_ot_pp_csc2_idc1 g3_ot_pp_csc2_idc1;                               /* 0x8980 */
    volatile reg_g3_ot_pp_csc2_idc2 g3_ot_pp_csc2_idc2;                               /* 0x8984 */
    volatile reg_g3_ot_pp_csc2_odc0 g3_ot_pp_csc2_odc0;                               /* 0x8988 */
    volatile reg_g3_ot_pp_csc2_odc1 g3_ot_pp_csc2_odc1;                               /* 0x898c */
    volatile reg_g3_ot_pp_csc2_odc2 g3_ot_pp_csc2_odc2;                               /* 0x8990 */
    volatile reg_g3_ot_pp_csc2_min_y g3_ot_pp_csc2_min_y;                             /* 0x8994 */
    volatile reg_g3_ot_pp_csc2_min_c g3_ot_pp_csc2_min_c;                             /* 0x8998 */
    volatile reg_g3_ot_pp_csc2_max_y g3_ot_pp_csc2_max_y;                             /* 0x899c */
    volatile reg_g3_ot_pp_csc2_max_c g3_ot_pp_csc2_max_c;                             /* 0x89a0 */
    volatile unsigned int reserved_80[19];                                          /* 0x89a4~0x89ec 19 regs */
    volatile reg_g3_ot_pp_csc_ink_ctrl g3_ot_pp_csc_ink_ctrl;                         /* 0x89f0 */
    volatile reg_g3_ot_pp_csc_ink_pos g3_ot_pp_csc_ink_pos;                           /* 0x89f4 */
    volatile unsigned int g3_ot_pp_csc_ink_data;                                     /* 0x89f8 */
    volatile unsigned int g3_ot_pp_csc_ink_data2;                                    /* 0x89fc */
    volatile reg_g3_osb_mute_bk g3_osb_mute_bk;                                     /* 0x8a00 */
    volatile reg_g3_osb_bk_alpha g3_osb_bk_alpha;                                   /* 0x8a04 */
    volatile reg_g3_osb_coef_rd_en g3_osb_coef_rd_en;                               /* 0x8a08 */
    volatile unsigned int g3_osb_coef_rd_addr;                                      /* 0x8a0c */
    volatile unsigned int reserved_81[380];                                         /* 0x8a10~0x8ffc 380 regs */
    volatile reg_g4_ctrl g4_ctrl;                                                   /* 0x9000 */
    volatile reg_g4_upd g4_upd;                                                     /* 0x9004 */
    volatile unsigned int g4_galpha_sum;                                            /* 0x9008 */
    volatile reg_g4_0reso_read g4_0reso_read;                                       /* 0x900c */
    volatile reg_g4_ireso g4_ireso;                                                 /* 0x9010 */
    volatile unsigned int reserved_82[27];                                          /* 0x9014~0x907c 27 regs */
    volatile reg_g4_dfpos g4_dfpos;                                                 /* 0x9080 */
    volatile reg_g4_dlpos g4_dlpos;                                                 /* 0x9084 */
    volatile reg_g4_vfpos g4_vfpos;                                                 /* 0x9088 */
    volatile reg_g4_vlpos g4_vlpos;                                                 /* 0x908c */
    volatile reg_g4_bk g4_bk;                                                       /* 0x9090 */
    volatile reg_g4_alpha g4_alpha;                                                 /* 0x9094 */
    volatile reg_g4_mute_bk g4_mute_bk;                                             /* 0x9098 */
    volatile reg_g4_lbox_ctrl g4_lbox_ctrl;                                         /* 0x909c */
    volatile unsigned int reserved_83[24];                                          /* 0x90a0~0x90fc 24 regs */
    volatile reg_g4_ot_pp_csc_ctrl g4_ot_pp_csc_ctrl;                                 /* 0x9100 */
    volatile reg_g4_ot_pp_csc_coef00 g4_ot_pp_csc_coef00;                             /* 0x9104 */
    volatile reg_g4_ot_pp_csc_coef01 g4_ot_pp_csc_coef01;                             /* 0x9108 */
    volatile reg_g4_ot_pp_csc_coef02 g4_ot_pp_csc_coef02;                             /* 0x910c */
    volatile reg_g4_ot_pp_csc_coef10 g4_ot_pp_csc_coef10;                             /* 0x9110 */
    volatile reg_g4_ot_pp_csc_coef11 g4_ot_pp_csc_coef11;                             /* 0x9114 */
    volatile reg_g4_ot_pp_csc_coef12 g4_ot_pp_csc_coef12;                             /* 0x9118 */
    volatile reg_g4_ot_pp_csc_coef20 g4_ot_pp_csc_coef20;                             /* 0x911c */
    volatile reg_g4_ot_pp_csc_coef21 g4_ot_pp_csc_coef21;                             /* 0x9120 */
    volatile reg_g4_ot_pp_csc_coef22 g4_ot_pp_csc_coef22;                             /* 0x9124 */
    volatile reg_g4_ot_pp_csc_scale g4_ot_pp_csc_scale;                               /* 0x9128 */
    volatile reg_g4_ot_pp_csc_idc0 g4_ot_pp_csc_idc0;                                 /* 0x912c */
    volatile reg_g4_ot_pp_csc_idc1 g4_ot_pp_csc_idc1;                                 /* 0x9130 */
    volatile reg_g4_ot_pp_csc_idc2 g4_ot_pp_csc_idc2;                                 /* 0x9134 */
    volatile reg_g4_ot_pp_csc_odc0 g4_ot_pp_csc_odc0;                                 /* 0x9138 */
    volatile reg_g4_ot_pp_csc_odc1 g4_ot_pp_csc_odc1;                                 /* 0x913c */
    volatile reg_g4_ot_pp_csc_odc2 g4_ot_pp_csc_odc2;                                 /* 0x9140 */
    volatile reg_g4_ot_pp_csc_min_y g4_ot_pp_csc_min_y;                               /* 0x9144 */
    volatile reg_g4_ot_pp_csc_min_c g4_ot_pp_csc_min_c;                               /* 0x9148 */
    volatile reg_g4_ot_pp_csc_max_y g4_ot_pp_csc_max_y;                               /* 0x914c */
    volatile reg_g4_ot_pp_csc_max_c g4_ot_pp_csc_max_c;                               /* 0x9150 */
    volatile reg_g4_ot_pp_csc2_coef00 g4_ot_pp_csc2_coef00;                           /* 0x9154 */
    volatile reg_g4_ot_pp_csc2_coef01 g4_ot_pp_csc2_coef01;                           /* 0x9158 */
    volatile reg_g4_ot_pp_csc2_coef02 g4_ot_pp_csc2_coef02;                           /* 0x915c */
    volatile reg_g4_ot_pp_csc2_coef10 g4_ot_pp_csc2_coef10;                           /* 0x9160 */
    volatile reg_g4_ot_pp_csc2_coef11 g4_ot_pp_csc2_coef11;                           /* 0x9164 */
    volatile reg_g4_ot_pp_csc2_coef12 g4_ot_pp_csc2_coef12;                           /* 0x9168 */
    volatile reg_g4_ot_pp_csc2_coef20 g4_ot_pp_csc2_coef20;                           /* 0x916c */
    volatile reg_g4_ot_pp_csc2_coef21 g4_ot_pp_csc2_coef21;                           /* 0x9170 */
    volatile reg_g4_ot_pp_csc2_coef22 g4_ot_pp_csc2_coef22;                           /* 0x9174 */
    volatile reg_g4_ot_pp_csc2_scale g4_ot_pp_csc2_scale;                             /* 0x9178 */
    volatile reg_g4_ot_pp_csc2_idc0 g4_ot_pp_csc2_idc0;                               /* 0x917c */
    volatile reg_g4_ot_pp_csc2_idc1 g4_ot_pp_csc2_idc1;                               /* 0x9180 */
    volatile reg_g4_ot_pp_csc2_idc2 g4_ot_pp_csc2_idc2;                               /* 0x9184 */
    volatile reg_g4_ot_pp_csc2_odc0 g4_ot_pp_csc2_odc0;                               /* 0x9188 */
    volatile reg_g4_ot_pp_csc2_odc1 g4_ot_pp_csc2_odc1;                               /* 0x918c */
    volatile reg_g4_ot_pp_csc2_odc2 g4_ot_pp_csc2_odc2;                               /* 0x9190 */
    volatile reg_g4_ot_pp_csc2_min_y g4_ot_pp_csc2_min_y;                             /* 0x9194 */
    volatile reg_g4_ot_pp_csc2_min_c g4_ot_pp_csc2_min_c;                             /* 0x9198 */
    volatile reg_g4_ot_pp_csc2_max_y g4_ot_pp_csc2_max_y;                             /* 0x919c */
    volatile reg_g4_ot_pp_csc2_max_c g4_ot_pp_csc2_max_c;                             /* 0x91a0 */
    volatile unsigned int reserved_84[19];                                          /* 0x91a4~0x91ec 19 regs */
    volatile reg_g4_ot_pp_csc_ink_ctrl g4_ot_pp_csc_ink_ctrl;                         /* 0x91f0 */
    volatile reg_g4_ot_pp_csc_ink_pos g4_ot_pp_csc_ink_pos;                           /* 0x91f4 */
    volatile unsigned int g4_ot_pp_csc_ink_data;                                     /* 0x91f8 */
    volatile unsigned int g4_ot_pp_csc_ink_data2;                                    /* 0x91fc */
    volatile reg_g4_osb_mute_bk g4_osb_mute_bk;                                     /* 0x9200 */
    volatile reg_g4_osb_bk_alpha g4_osb_bk_alpha;                                   /* 0x9204 */
    volatile reg_g4_osb_coef_rd_en g4_osb_coef_rd_en;                               /* 0x9208 */
    volatile unsigned int g4_osb_coef_rd_addr;                                      /* 0x920c */
    volatile unsigned int reserved_85[380];                                         /* 0x9210~0x97fc 380 regs */
    volatile unsigned int gp0_ctrl;                                                 /* 0x9800 */
    volatile reg_gp0_upd gp0_upd;                                                   /* 0x9804 */
    volatile reg_gp0_ireso gp0_ireso;                                               /* 0x9808 */
    volatile unsigned int reserved_86[29];                                          /* 0x980c~0x987c 29 regs */
    volatile reg_gp0_lbox_ctrl gp0_lbox_ctrl;                                       /* 0x9880 */
    volatile reg_gp0_galpha gp0_galpha;                                             /* 0x9884 */
    volatile unsigned int gp0_galpha_sum;                                           /* 0x9888 */
    volatile reg_gp0_dfpos gp0_dfpos;                                               /* 0x988c */
    volatile reg_gp0_dlpos gp0_dlpos;                                               /* 0x9890 */
    volatile reg_gp0_vfpos gp0_vfpos;                                               /* 0x9894 */
    volatile reg_gp0_vlpos gp0_vlpos;                                               /* 0x9898 */
    volatile reg_gp0_bk gp0_bk;                                                     /* 0x989c */
    volatile reg_gp0_alpha gp0_alpha;                                               /* 0x98a0 */
    volatile reg_gp0_mute_bk gp0_mute_bk;                                           /* 0x98a4 */
    volatile unsigned int reserved_87[22];                                          /* 0x98a8~0x98fc 22 regs */
    volatile reg_gp0_csc_idc gp0_csc_idc;                                           /* 0x9900 */
    volatile reg_gp0_csc_odc gp0_csc_odc;                                           /* 0x9904 */
    volatile reg_gp0_csc_iodc gp0_csc_iodc;                                         /* 0x9908 */
    volatile reg_gp0_csc_p0 gp0_csc_p0;                                             /* 0x990c */
    volatile reg_gp0_csc_p1 gp0_csc_p1;                                             /* 0x9910 */
    volatile reg_gp0_csc_p2 gp0_csc_p2;                                             /* 0x9914 */
    volatile reg_gp0_csc_p3 gp0_csc_p3;                                             /* 0x9918 */
    volatile reg_gp0_csc_p4 gp0_csc_p4;                                             /* 0x991c */
    volatile unsigned int reserved_88[1464];                                        /* 0x9920~0xaffc 1464 regs */
    volatile reg_wbc_g0_ctrl wbc_g0_ctrl;                                           /* 0xb000 */
    volatile reg_wbc_g0_upd wbc_g0_upd;                                             /* 0xb004 */
    volatile reg_wbc_g0_cmp wbc_g0_cmp;                                             /* 0xb008 */
    volatile unsigned int reserved_89;                                              /* 0xb00c */
    volatile unsigned int wbc_g0_ar_addr;                                           /* 0xb010 */
    volatile unsigned int wbc_g0_gb_addr;                                           /* 0xb014 */
    volatile reg_wbc_g0_stride wbc_g0_stride;                                       /* 0xb018 */
    volatile unsigned int wbc_g0_offset;                                            /* 0xb01c */
    volatile reg_wbc_g0_oreso wbc_g0_oreso;                                         /* 0xb020 */
    volatile reg_wbc_g0_fcrop wbc_g0_fcrop;                                         /* 0xb024 */
    volatile reg_wbc_g0_lcrop wbc_g0_lcrop;                                         /* 0xb028 */
    volatile unsigned int reserved_90[501];                                         /* 0xb02c~0xb7fc 501 regs */
    volatile reg_wbc_gp0_ctrl wbc_gp0_ctrl;                                         /* 0xb800 */
    volatile reg_wbc_gp0_upd wbc_gp0_upd;                                           /* 0xb804 */
    volatile unsigned int reserved_91[2];                                           /* 0xb808~0xb80c 2 regs */
    volatile unsigned int wbc_gp0_yaddr;                                            /* 0xb810 */
    volatile unsigned int wbc_gp0_caddr;                                            /* 0xb814 */
    volatile reg_wbc_gp0_stride wbc_gp0_stride;                                     /* 0xb818 */
    volatile unsigned int reserved_92;                                              /* 0xb81c */
    volatile reg_wbc_gp0_oreso wbc_gp0_oreso;                                       /* 0xb820 */
    volatile reg_wbc_gp0_fcrop wbc_gp0_fcrop;                                       /* 0xb824 */
    volatile reg_wbc_gp0_lcrop wbc_gp0_lcrop;                                       /* 0xb828 */
    volatile unsigned int reserved_93[53];                                          /* 0xb82c~0xb8fc 53 regs */
    volatile reg_wbc_gp0_dither_ctrl wbc_gp0_dither_ctrl;                           /* 0xb900 */
    volatile reg_wbc_gp0_dither_coef0 wbc_gp0_dither_coef0;                         /* 0xb904 */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART45_H_ */
