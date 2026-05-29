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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART04_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART04_H_

    struct {
        td_u8 keybmax : 8; /* [7..0]  */
        td_u8 keygmax : 8; /* [15..8]  */
        td_u8 keyrmax : 8; /* [23..16]  */
        td_u8 keyamax : 8; /* [31..24]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_cbmkeymax;

typedef union {
    /* Define the struct bits */
    struct {
        td_u8 keybmask : 8; /* [7..0]  */
        td_u8 keygmask : 8; /* [15..8]  */
        td_u8 keyrmask : 8; /* [23..16]  */
        td_u8 keyamask : 8; /* [31..24]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_cbmkeymask;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 s1_galphaen : 1;      /* [0]  */
        td_u32 s1_palphaen : 1;      /* [1]  */
        td_u32 s1_premulten : 1;     /* [2]  */
        td_u32 s1_multiglobalen : 1; /* [3]  */
        td_u32 s1_blendmode : 4;     /* [7..4]  */
        td_u32 s1_galpha : 8;        /* [15..8]  */
        td_u32 reserved_0 : 15;      /* [30..16]  */
        td_u32 s1_coverblenden : 1;  /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_cbmpara;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 s1_xfpos : 16; /* [15..0]  */
        td_u32 s1_yfpos : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_cbmstpos;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 s2_galphaen : 1;      /* [0]  */
        td_u32 s2_palphaen : 1;      /* [1]  */
        td_u32 s2_premulten : 1;     /* [2]  */
        td_u32 s2_multiglobalen : 1; /* [3]  */
        td_u32 s2_blendmode : 4;     /* [7..4]  */
        td_u32 s2_galpha : 8;        /* [15..8]  */
        td_u32 reserved_0 : 15;      /* [30..16]  */
        td_u32 s2_coverblenden : 1;  /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_cbmpara;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 s2_rtt_en : 1;   /* [0]  */
        td_u32 s2_rtt_dir : 1;  /* [1]  */
        td_u32 s2_rtt_fmt : 2;  /* [2]  */
        td_u32 reserved_0 : 28; /* [31..3]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_rtt_ctrl;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 s2_xfpos : 16; /* [15..0]  */
        td_u32 s2_yfpos : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_cbmstpos;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_scl_lh : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_src1_zme_lhaddr;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_scl_lv : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_src1_zme_lvaddr;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_scl_ch : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_src1_zme_chaddr;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_scl_cv : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_src1_zme_cvaddr;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_scl_lh : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_src2_zme_lhaddr;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_scl_lv : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_src2_zme_lvaddr;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_scl_ch : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_src2_zme_chaddr;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_scl_cv : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_src2_zme_cvaddr;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 clut_addr : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_clut_addr;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved_0 : 20; /* [19..0]  */
        td_u32 awid_cfg0 : 4;   /* [23..20]  */
        td_u32 reserved_1 : 4;  /* [27..24]  */
        td_u32 arid_cfg0 : 4;   /* [31..28]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_axiid;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 nodeid : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_nodeid;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 eof_mask : 1;     /* [0]  */
        td_u32 timeout_mask : 1; /* [1]  */
        td_u32 bus_err_mask : 1; /* [2]  */
        td_u32 eof_end_mask : 1; /* [3]  */
        td_u32 reserved_0 : 28;  /* [31..4]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_intmask;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 p_next_hi : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_pnext_hi;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 p_next_low : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_pnext_low;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 start : 1;       /* [0]  */
        td_u32 reserved_0 : 31; /* [31..1]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_start;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 eof_state : 1;     /* [0]  */
        td_u32 timeout_state : 1; /* [1]  */
        td_u32 bus_err : 1;       /* [2]  */
        td_u32 eof_end_state : 1; /* [3]  */
        td_u32 reserved_0 : 28;   /* [31..4]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_intstate;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 eof_clr : 1;     /* [0]  */
        td_u32 timeout_clr : 1; /* [1]  */
        td_u32 bus_err_clr : 1; /* [2]  */
        td_u32 eof_end_clr : 1; /* [3]  */
        td_u32 reserved_0 : 28; /* [31..4]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_intclr;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 raw_eof : 1;     /* [0]  */
        td_u32 raw_timeout : 1; /* [1]  */
        td_u32 raw_bus_err : 1; /* [2]  */
        td_u32 raw_eof_end : 1; /* [3]  */
        td_u32 reserved_0 : 28; /* [31..4]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_rawint;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 pfcnt : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_pfcnt;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 routstanding : 4;  /* [3..0]  */
        td_u32 woutstanding : 4;  /* [7..4]  */
        td_u32 init_timer : 16;   /* [23..8]  */
        td_u32 ck_gt_en : 1;      /* [24]  */
        td_u32 ck_gt_en_calc : 1; /* [25]  */
        td_u32 split_128b_en : 1; /* [26]  */
        td_u32 split_256b_en : 1; /* [27]  */
        td_u32 split_1k_en : 1;   /* [28]  */
        td_u32 split_2k_en : 1;   /* [29]  */
        td_u32 reserved_0 : 2;    /* [31..30]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_miscellaneous;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 mac_ch_prio : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_maccfg;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 timeout : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_timeout;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 eof_cnt : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_eofcnt;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 emab : 3;         /* [2..0]  */
        td_u32 emaa : 3;         /* [5..3]  */
        td_u32 emasa : 1;        /* [6]  */
        td_u32 emaw : 2;         /* [8..7]  */
        td_u32 ema : 3;          /* [11..9]  */
        td_u32 rfsuhd_wtsel : 2; /* [13..12]  */
        td_u32 rfsuhd_rtsel : 2; /* [15..14]  */
        td_u32 rfs_wtsel : 2;    /* [17..16]  */
        td_u32 rfs_rtsel : 2;    /* [19..18]  */
        td_u32 rfts_wct : 2;     /* [21..20]  */
        td_u32 rfts_rct : 2;     /* [23..22]  */
        td_u32 rfts_kp : 3;      /* [26..24]  */
        td_u32 rftf_wct : 2;     /* [28..27]  */
        td_u32 rftf_rct : 2;     /* [30..29]  */
        td_u32 reserved : 1;     /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_memctrl;


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART04_H_ */
