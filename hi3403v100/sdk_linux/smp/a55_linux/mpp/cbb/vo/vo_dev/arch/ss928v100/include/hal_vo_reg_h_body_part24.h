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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART24_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART24_H_

    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_thr;

/* define the union reg_wd_zme_hinfo */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int out_width : 16;    /* [15..0]  */
        unsigned int hzme_ck_gt_en : 1; /* [16]  */
        unsigned int reserved_0 : 15;   /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_hinfo;

/* define the union reg_wd_zme_hsp */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 24; /* [23..0]  */
        unsigned int hfir_order : 1;  /* [24]  */
        unsigned int chfir_mode : 1;  /* [25]  */
        unsigned int lhfir_mode : 1;  /* [26]  */
        unsigned int non_lnr_en : 1;  /* [27]  */
        unsigned int chmid_en : 1;    /* [28]  */
        unsigned int lhmid_en : 1;    /* [29]  */
        unsigned int chfir_en : 1;    /* [30]  */
        unsigned int lhfir_en : 1;    /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_hsp;

/* define the union reg_wd_zme_hloffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lhfir_offset : 28; /* [27..0]  */
        unsigned int reserved_0 : 4;    /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_hloffset;

/* define the union reg_wd_zme_hcoffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int chfir_offset : 28; /* [27..0]  */
        unsigned int reserved_0 : 4;    /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_hcoffset;

/* define the union reg_wd_zme_hcoef_ren */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int apb_vhd_hf_cren : 1; /* [0]  */
        unsigned int apb_vhd_hf_lren : 1; /* [1]  */
        unsigned int reserved_0 : 30;     /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_hcoef_ren;

/* define the union reg_wd_zme_hcoef_rdata */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int apb_vhd_hcoef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;         /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_hcoef_rdata;

/* define the union reg_wd_zme_hdraw */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hdraw_mode : 2;  /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_hdraw;

/* define the union reg_wd_zme_hratio */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hratio : 27;    /* [26..0]  */
        unsigned int reserved_0 : 5; /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_hratio;

/* define the union reg_wd_zme_vinfo */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int out_height : 16;   /* [15..0]  */
        unsigned int out_fmt : 2;       /* [17..16]  */
        unsigned int out_pro : 1;       /* [18]  */
        unsigned int vzme_ck_gt_en : 1; /* [19]  */
        unsigned int reserved_0 : 12;   /* [31..20]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_vinfo;

/* define the union reg_wd_zme_vsp */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 16;  /* [15..0]  */
        unsigned int graphdet_en : 1;  /* [16]  */
        unsigned int reserved_1 : 8;   /* [24..17]  */
        unsigned int cvfir_mode : 1;   /* [25]  */
        unsigned int lvfir_mode : 1;   /* [26]  */
        unsigned int vfir_1tap_en : 1; /* [27]  */
        unsigned int cvmid_en : 1;     /* [28]  */
        unsigned int lvmid_en : 1;     /* [29]  */
        unsigned int cvfir_en : 1;     /* [30]  */
        unsigned int lvfir_en : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_vsp;

/* define the union reg_wd_zme_voffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vchroma_offset : 16; /* [15..0]  */
        unsigned int vluma_offset : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_voffset;

/* define the union reg_wd_zme_vboffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbchroma_offset : 16; /* [15..0]  */
        unsigned int vbluma_offset : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_vboffset;

/* define the union reg_wd_zme_vcoef_ren */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int apb_vhd_vf_cren : 1; /* [0]  */
        unsigned int apb_vhd_vf_lren : 1; /* [1]  */
        unsigned int reserved_0 : 30;     /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_vcoef_ren;

/* define the union reg_wd_zme_vcoef_rdata */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int apb_vhd_vcoef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;         /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_vcoef_rdata;

/* define the union reg_wd_zme_vdraw */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vdraw_mode : 2;  /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_vdraw;

/* define the union reg_wd_zme_vratio */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vratio : 19;     /* [18..0]  */
        unsigned int reserved_0 : 13; /* [31..19]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_zme_vratio;

/* define the union reg_dhd0_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;          /* [0]  */
        unsigned int disp_mode : 3;      /* [3..1]  */
        unsigned int iop : 1;            /* [4]  */
        unsigned int intf_ivs : 1;       /* [5]  */
        unsigned int intf_ihs : 1;       /* [6]  */
        unsigned int intf_idv : 1;       /* [7]  */
        unsigned int reserved_0 : 1;     /* [8]  */
        unsigned int hdmi420c_sel : 1;   /* [9]  */
        unsigned int hdmi420_en : 1;     /* [10]  */
        unsigned int uf_offline_en : 1;  /* [11]  */
        unsigned int reserved_1 : 2;     /* [13..12]  */
        unsigned int hdmi_mode : 1;      /* [14]  */
        unsigned int twochn_debug : 1;   /* [15]  */
        unsigned int twochn_en : 1;      /* [16]  */
        unsigned int reserved_2 : 1;     /* [17]  */
        unsigned int cbar_mode : 1;      /* [18]  */
        unsigned int sin_en : 1;         /* [19]  */
        unsigned int fpga_lmt_width : 7; /* [26..20]  */
        unsigned int fpga_lmt_en : 1;    /* [27]  */
        unsigned int p2i_en : 1;         /* [28]  */
        unsigned int cbar_sel : 1;       /* [29]  */
        unsigned int cbar_en : 1;        /* [30]  */
        unsigned int intf_en : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_ctrl;

/* define the union reg_dhd0_vsync1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vact : 16; /* [15..0]  */
        unsigned int vbb : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_vsync1;

/* define the union reg_dhd0_vsync2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vfb : 16;        /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_vsync2;

/* define the union reg_dhd0_hsync1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hact : 16; /* [15..0]  */
        unsigned int hbb : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_hsync1;

/* define the union reg_dhd0_hsync2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfb : 16;  /* [15..0]  */
        unsigned int hmid : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_hsync2;

/* define the union reg_dhd0_vplus1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int bvact : 16; /* [15..0]  */
        unsigned int bvbb : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_vplus1;

/* define the union reg_dhd0_vplus2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int bvfb : 16;       /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_vplus2;

/* define the union reg_dhd0_pwr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hpw : 16;        /* [15..0]  */
        unsigned int vpw : 8;         /* [23..16]  */
        unsigned int reserved_0 : 3;  /* [26..24]  */
        unsigned int multichn_en : 2; /* [28..27]  */
        unsigned int reserved_1 : 3;  /* [31..29]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_pwr;

/* define the union reg_dhd0_vtthd3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vtmgthd3 : 13;  /* [12..0]  */
        unsigned int reserved_0 : 2; /* [14..13]  */
        unsigned int thd3_mode : 1;  /* [15]  */
        unsigned int vtmgthd4 : 13;  /* [28..16]  */
        unsigned int reserved_1 : 2; /* [30..29]  */
        unsigned int thd4_mode : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_vtthd3;

/* define the union reg_dhd0_vtthd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vtmgthd1 : 13;  /* [12..0]  */
        unsigned int reserved_0 : 2; /* [14..13]  */
        unsigned int thd1_mode : 1;  /* [15]  */
        unsigned int vtmgthd2 : 13;  /* [28..16]  */
        unsigned int reserved_1 : 2; /* [30..29]  */
        unsigned int thd2_mode : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_vtthd;

/* define the union reg_dhd0_parathd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int para_thd : 8;    /* [7..0]  */
        unsigned int reserved_0 : 23; /* [30..8]  */
        unsigned int dfs_en : 1;      /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_parathd;

/* define the union reg_dhd0_precharge_thd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tcon_precharge_thd : 17; /* [16..0]  */
        unsigned int reserved_0 : 3;          /* [19..17]  */
        unsigned int vsync_te_mode : 1;       /* [20]  */
        unsigned int reserved_1 : 10;         /* [30..21]  */
        unsigned int dneed_en_mode : 1;       /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_precharge_thd;

/* define the union reg_dhd0_start_pos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int start_pos : 8;        /* [7..0]  */
        unsigned int timing_start_pos : 8; /* [15..8]  */
        unsigned int fi_start_pos : 4;     /* [19..16]  */
        unsigned int req_start_pos : 12;   /* [31..20]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_start_pos;

/* define the union reg_dhd0_start_pos1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int req_start_pos1 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;     /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_start_pos1;

/* define the union reg_dhd0_paraup */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0]  */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART24_H_ */
