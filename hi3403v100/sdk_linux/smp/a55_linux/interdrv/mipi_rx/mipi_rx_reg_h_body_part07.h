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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART07_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART07_H_

    u_phy_sync_sol1_link phy_sync_sol1_link;              /* 0x64 */
    u_phy_sync_sol2_link phy_sync_sol2_link;              /* 0x68 */
    u_phy_sync_sol3_link phy_sync_sol3_link;              /* 0x6c */
    unsigned int reserved_3[36];                          /* 0x70~0xfc */
    u_cil_timeout_link cil_timeout_link;                  /* 0x100 */
    u_cil_fsm0_link cil_fsm0_link;                        /* 0x104 */
    u_cil_fsm_st0_link cil_fsm_st0_link;                  /* 0x108 */
    u_cil_fsm_st1_link cil_fsm_st1_link;                  /* 0x10c */
    u_phy_st0_link phy_st0_link;                          /* 0x110 */
    u_phy_st1_link phy_st1_link;                          /* 0x114 */
    u_phy_st2_link phy_st2_link;                          /* 0x118 */
    u_phy_st3_link phy_st3_link;                          /* 0x11c */
    u_phy_st4_link phy_st4_link;                          /* 0x120 */
    u_phy_st5_link phy_st5_link;                          /* 0x124 */
    u_phy_st_so_link phy_st_so_link;                      /* 0x128 */
    u_cil_fsm1_link cil_fsm1_link;                        /* 0x12c */
    u_skew_cal_lepgth0_link skew_cal_lepgth0_link;        /* 0x130 */
    u_skew_cal_lepgth1_link skew_cal_lepgth1_link;        /* 0x134 */
    unsigned int reserved_4[6];                           /* 0x138~0x14c */
    u_freq_measure freq_measure;                          /* 0x150 */
    unsigned int reserved_5[39];                          /* 0x154~0x1ec */
    u_mipi_cil_int_raw_link mipi_cil_int_raw_link;        /* 0x1f0 */
    u_mipi_cil_int_link mipi_cil_int_link;                /* 0x1f4 */
    u_mipi_cil_int_msk_link mipi_cil_int_msk_link;        /* 0x1f8 */
    unsigned int reserved_6;                              /* 0x1fc */
} mipi_rx_phy_cfg_t;

typedef struct {
    u_hs_mode_select hs_mode_select;                           /* 0x800 */
    u_d_p_swap d_p_swap;                                       /* 0x804 */
    u_phy_en phy_en;                                           /* 0x808 */
    u_lane_en lane_en;                                         /* 0x80c */
    u_phy_cil_ctrl phy_cil_ctrl;                               /* 0x810 */
    u_phy_lp_select phy_lp_select;                             /* 0x814 */
    u_phycfg_mode phycfg_mode;                                 /* 0x818 */
    u_phycfg_en phycfg_en;                                     /* 0x81c */
    u_chn0_mem_ctrl chn0_mem_ctrl;                             /* 0x820 */
    u_chn0_clr_en chn0_clr_en;                                 /* 0x824 */
    u_chn1_mem_ctrl chn1_mem_ctrl;                             /* 0x828 */
    u_chn1_clr_en chn1_clr_en;                                 /* 0x82c */
    u_chn2_mem_ctrl chn2_mem_ctrl;                             /* 0x830 */
    u_chn2_clr_en chn2_clr_en;                                 /* 0x834 */
    u_chn3_mem_ctrl chn3_mem_ctrl;                             /* 0x838 */
    u_chn3_clr_en chn3_clr_en;                                 /* 0x83c */
    unsigned int reserved_0[16];                               /* 0x840~0x87c */
    u_mipi_test_phy mipi_test_phy;                             /* 0x880 */
    unsigned int reserved_1[31];                               /* 0x884~0x8fc */
    u_colorbar_ctrl_link0 colorbar_ctrl_link0;                 /* 0x900 */
    u_colorbar_init_link0 colorbar_init_link0;                 /* 0x904 */
    u_colorbar_v_blk_link0 colorbar_v_blk_link0;               /* 0x908 */
    u_colorbar_h_blk_link0 colorbar_h_blk_link0;               /* 0x90c */
    u_colorbar_img_size_link0 colorbar_img_size_link0;         /* 0x910 */
    u_colorbar_sync_code0_link0 colorbar_sync_code0_link0;     /* 0x914 */
    u_colorbar_sync_code1_link0 colorbar_sync_code1_link0;     /* 0x918 */
    unsigned int reserved_2[57];                               /* 0x91c~0x9fc */
    u_phy0_test_out phy0_test_out;                             /* 0xa00 */
    u_phy1_test_out phy1_test_out;                             /* 0xa04 */
    u_phy2_test_out phy2_test_out;                             /* 0xa08 */
    u_phy3_test_out phy3_test_out;                             /* 0xa0c */
    unsigned int reserved_3[60];                               /* 0xa10~0xafc */
    u_lpdt_ph_addr lpdt_ph_addr;                               /* 0xb00 */
    u_lpdt_check_res_addr lpdt_check_res_addr;                 /* 0xb04 */
    unsigned int reserved_4[314];                              /* 0xb08~0xfec */
    u_mipi_int_raw mipi_int_raw;                               /* 0xff0 */
    u_mipi_int_st mipi_int_st;                                 /* 0xff4 */
    u_mipi_int_msk mipi_int_msk;                               /* 0xff8 */
    unsigned int reserved_5;                                   /* 0xffc */
} mipi_rx_sys_regs_t;

typedef struct {
    u_mipi_interlaced mipi_interlaced;                   /* 0x1000 */
    unsigned int reserved_1[3];                          /* 0x1004~0x100c */
    u_mipi_frame_num0 mipi_frame_num0;                   /* 0x1010 */
    u_mipi_frame_num1 mipi_frame_num1;                   /* 0x1014 */
    unsigned int reserved_2[2];                          /* 0x1018~0x101c */
    u_mipi_crc_intr_raw mipi_crc_intr_raw;               /* 0x1020 */
    u_mipi_crc_intr_st mipi_crc_intr_st;                 /* 0x1024 */
    u_mipi_crc_intr_msk mipi_crc_intr_msk;               /* 0x1028 */
    unsigned int reserved_3[53];                         /* 0x102c~0x10fc */
    u_mipi_userdef_dt mipi_userdef_dt;                   /* 0x1100 */
    u_mipi_user_def mipi_user_def;                       /* 0x1104 */
    u_mipi_ctrl_mode_hs mipi_ctrl_mode_hs;               /* 0x1108 */
    u_mipi_vhend_delay mipi_vhend_delay;                 /* 0x110c */
    unsigned int reserved_4[60];                         /* 0x1110~0x11fc */
    u_mipi_dol_id_code0 mipi_dol_id_code0;               /* 0x1200 */
    u_mipi_dol_id_code1 mipi_dol_id_code1;               /* 0x1204 */
    u_mipi_dol_id_code2 mipi_dol_id_code2;               /* 0x1208 */
    unsigned int reserved_5;                             /* 0x120c */
    u_mipi_crop_start_chn0 mipi_crop_start_chn0;         /* 0x1210 */
    u_mipi_crop_start_chn1 mipi_crop_start_chn1;         /* 0x1214 */
    u_mipi_crop_start_chn2 mipi_crop_start_chn2;         /* 0x1218 */
    u_mipi_crop_start_chn3 mipi_crop_start_chn3;         /* 0x121c */
    unsigned int reserved_6;                             /* 0x1220 */
    u_mipi_imgsize mipi_imgsize;                         /* 0x1224 */
    unsigned int reserved_7[2];                          /* 0x1228~0x122c */
    u_mipi_ctrl_mode_pixel mipi_ctrl_mode_pixel;         /* 0x1230 */
    unsigned int reserved_8[3];                          /* 0x1234~0x123c */
    u_mipi_dummy_pix_reg mipi_dummy_pix_reg;             /* 0x1240 */
    unsigned int reserved_9[3];                          /* 0x1244~0x124c */
    u_mipi_imgsize0_statis mipi_imgsize0_statis;         /* 0x1250 */
    u_mipi_imgsize1_statis mipi_imgsize1_statis;         /* 0x1254 */
    u_mipi_imgsize2_statis mipi_imgsize2_statis;         /* 0x1258 */
    u_mipi_imgsize3_statis mipi_imgsize3_statis;         /* 0x125c */
    unsigned int reserved_10[36];                        /* 0x1260~0x12ec */
    u_mipi_ctrl_int_raw mipi_ctrl_int_raw;               /* 0x12f0 */
    u_mipi_ctrl_int mipi_ctrl_int;                       /* 0x12f4 */
    u_mipi_ctrl_int_msk mipi_ctrl_int_msk;               /* 0x12f8 */
    unsigned int reserved_11;                            /* 0x12fc */
} mipi_ctrl_regs_t;

typedef struct {
    u_lvds_lane_sof_01 lvds_lane_sof_01;                 /* 0x1320 */
    u_lvds_lane_sof_23 lvds_lane_sof_23;                 /* 0x1324 */
    u_lvds_lane_eof_01 lvds_lane_eof_01;                 /* 0x1328 */
    u_lvds_lane_eof_23 lvds_lane_eof_23;                 /* 0x132c */
    u_lvds_lane_sol_01 lvds_lane_sol_01;                 /* 0x1330 */
    u_lvds_lane_sol_23 lvds_lane_sol_23;                 /* 0x1334 */
    u_lvds_lane_eol_01 lvds_lane_eol_01;                 /* 0x1338 */
    u_lvds_lane_eol_23 lvds_lane_eol_23;                 /* 0x133c */
} lvds_sync_code_cfg_t;

typedef struct {
    u_lvds_wdr lvds_wdr;                                          /* 0x1300 */
    u_lvds_dolscd_hblk lvds_dolscd_hblk;                          /* 0x1304 */
    u_lvds_ctrl lvds_ctrl;                                        /* 0x1308 */
    u_lvds_imgsize lvds_imgsize;                                  /* 0x130c */
    u_lvds_crop_start0 lvds_crop_start0;                          /* 0x1310 */
    u_lvds_crop_start1 lvds_crop_start1;                          /* 0x1314 */
    u_lvds_crop_start2 lvds_crop_start2;                          /* 0x1318 */
    u_lvds_crop_start3 lvds_crop_start3;                          /* 0x131c */
    lvds_sync_code_cfg_t lvds_this_frame_sync_code[16];           /* 0x1320~0x151c */
    lvds_sync_code_cfg_t lvds_next_frame_sync_code[16];           /* 0x1520~0x171c */
    u_lvds_li_word0 lvds_li_word0;                                /* 0x1720 */
    u_lvds_li_word1 lvds_li_word1;                                /* 0x1724 */
    u_lvds_li_word2 lvds_li_word2;                                /* 0x1728 */
    u_lvds_li_word3 lvds_li_word3;                                /* 0x172c */
    u_lvds_sync_bord0 lvds_sync_bord0;                            /* 0x1730 */
    u_lvds_sync_bord1 lvds_sync_bord1;                            /* 0x1734 */
    u_lvds_sync_bord2 lvds_sync_bord2;                            /* 0x1738 */
    u_lvds_sync_bord3 lvds_sync_bord3;                            /* 0x173c */
    u_lvds_lane_imgsize_statis lvds_lane_imgsize_statis[16];      /* 0x1740~0x177c */
    u_lvds_imgsize0_statis lvds_imgsize0_statis;                  /* 0x1780 */
    u_lvds_imgsize1_statis lvds_imgsize1_statis;                  /* 0x1784 */
    u_lvds_imgsize2_statis lvds_imgsize2_statis;                  /* 0x1788 */
    u_lvds_imgsize3_statis lvds_imgsize3_statis;                  /* 0x178c */
    u_lvds_lane_orders0 lvds_lane_orders0;                        /* 0x1790 */
    u_lvds_lane_orders1 lvds_lane_orders1;                        /* 0x1794 */
    u_lvds_lane_orders2 lvds_lane_orders2;                        /* 0x1798 */
    u_lvds_lane_orders3 lvds_lane_orders3;                        /* 0x179c */
    u_lvds_output_pix_num lvds_output_pix_num;                    /* 0x17a0 */
    u_lvds_f1_en lvds_f1_en;                              /* 0x17a4 */
    unsigned int reserved_4[18];                                  /* 0x17a8~0x17ec */
    u_lvds_ctrl_int_raw lvds_ctrl_int_raw;                        /* 0x17f0 */
    u_lvds_ctrl_int lvds_ctrl_int;                                /* 0x17f4 */
    u_lvds_ctrl_int_msk lvds_ctrl_int_msk;                        /* 0x17f8 */
    unsigned int reserved_5;                                      /* 0x17fc */
} lvds_ctrl_regs_t;

typedef struct {
    u_lane_id0_chn lane_id0_chn;                         /* 0x1800 */
    u_lane_id1_chn lane_id1_chn;                         /* 0x1804 */
    u_lane_id2_chn lane_id2_chn;                         /* 0x1808 */
    u_lane_id3_chn lane_id3_chn;                         /* 0x180c */
    unsigned int reserved_1[56];                         /* 0x1810~0x18ec */
    u_align_int_raw align_int_raw;                       /* 0x18f0 */
    u_align_int align_int;                               /* 0x18f4 */
    u_align_int_msk align_int_msk;                       /* 0x18f8 */
    unsigned int reserved_2[445];                        /* 0x18fc~0x1fec */
    u_chn_int_raw chn_int_raw;                           /* 0x1ff0 */
    u_chn_int chn_int;                                   /* 0x1ff4 */
    u_chn_int_mask chn_int_mask;                         /* 0x1ff8 */
    unsigned int reserved_3;                             /* 0x1ffc */
} global_ctrl_regs_t;

typedef struct {
    mipi_ctrl_regs_t       mipi_ctrl_regs;   /* 0x1000 ~ 0x12fc */
    lvds_ctrl_regs_t       lvds_ctrl_regs;   /* 0x1300 ~ 0x17fc */
    global_ctrl_regs_t     global_ctrl_regs; /* 0x1800 ~ 0x1ffc */
} mipi_rx_ctrl_regs_t;

/* Define the global struct */
typedef struct {
    mipi_rx_phy_cfg_t      mipi_rx_phy_cfg[2];      /* 0x0 ~ 0x1fc, 0x200 ~ 0x3fc */
    volatile unsigned int  reserved_0[256];         /* 0x400 ~ 0x7fc */
    mipi_rx_sys_regs_t     mipi_rx_sys_regs;        /* 0x800 ~ 0xffc */
    mipi_rx_ctrl_regs_t    mipi_rx_ctrl_regs[4];    /* 0x1000 ~ 0x1ffc */
} mipi_rx_regs_type_t;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART07_H_ */
