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

#ifndef SECURITY_SUBSYS_CIPHER_V3_SRC_DRV_DRIVERS_CORE_DRV_SYMC_V300_H_BODY_PART02_H_
#define SECURITY_SUBSYS_CIPHER_V3_SRC_DRV_DRIVERS_CORE_DRV_SYMC_V300_H_BODY_PART02_H_

    /* Define the struct bits */
    struct {
        u32 sym_chn_node_rd_point : 8; /* [7..0] */
        u32 reserved_1 : 24;           /* [31..8] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} in_sym_chn_node_rd_point;

/* Define the union in_sym_chn_node_length */
typedef union {
    /* Define the struct bits */
    struct {
        u32 sym_chn_node_length : 8; /* [7..0] */
        u32 reserved_1 : 24;         /* [31..8] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} in_sym_chn_node_length;

/* Define the union out_sym_chn_node_length */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_chn_node_length : 8; /* [7..0] */
        u32 reserved_1 : 24;             /* [31..8] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chn_node_length;

/* Define the union dbg_in_sym_chn_rd_addr_h */
typedef union {
    /* Define the struct bits */
    struct {
        u32 dbg_sym_chn_rd_addr_h : 4; /* [3..0] */
        u32 reserved : 28;             /* [31..4] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} dbg_in_sym_chn_rd_addr_h;

/* Define the union dbg_out_sym_chn_rd_addr_h */
typedef union {
    /* Define the struct bits */
    struct {
        u32 dbg_sym_chn_rd_addr_h : 4; /* [3..0] */
        u32 reserved : 28;             /* [31..4] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} dbg_out_sym_chn_rd_addr_h;

/* Define the union hash_chann_int */
typedef union {
    /* Define the struct bits */
    struct {
        u32 hash_chann_int : 16; /* [15..0] */
        u32 reserved_0 : 16;     /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} hash_chann_int;

/* Define the union ista_in_hash_node_cnt */
typedef union {
    /* Define the struct bits */
    struct {
        u32 ista_in_hash_node_cnt : 16; /* [15..1] */
        u32 reserved_1 : 16;            /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} ista_in_hash_node_cnt;

/* Define the union sym_calc_ctrl_check_err */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    calc_ctrl_check_ds_error : 1; /* [0]  */
        unsigned int    calc_ctrl_check_ss_error : 1; /* [1]  */
        unsigned int    calc_ctrl_check_alg_error : 1; /* [2]  */
        unsigned int    calc_ctrl_check_alg_invld_error : 1; /* [3]  */
        unsigned int    calc_ctrl_check_dec_error : 1; /* [4]  */
        unsigned int    calc_ctrl_check_error : 1; /* [5]  */
        unsigned int    calc_ctrl_check_sc_error : 1; /* [6]  */
        unsigned int    calc_ctrl_check_key_size_error : 1; /* [7]  */
        unsigned int    calc_ctrl_check_hdcp_error : 1; /* [8]  */
        unsigned int    calc_ctrl_check_cenc_dec_error : 1; /* [9]  */
        unsigned int    calc_ctrl_check_cenc_alg_error : 1; /* [10]  */
        unsigned int    calc_ctrl_check_gcm_ccm_error : 1; /* [11]  */
        unsigned int    reserved_0            : 2; /* [13..12]  */
        unsigned int    calc_ctrl_check_iv_length_error : 1; /* [14]  */
        unsigned int    calc_ctrl_check_data_length_error : 1; /* [15]  */
        unsigned int    calc_ctrl_big_key_info_diff_error : 1; /* [16]  */
        unsigned int    reserved_1            : 1; /* [17]  */
        unsigned int    calc_ctrl_hdcp_key_size_error : 1; /* [18]  */
        unsigned int    calc_ctrl_hdcp_sm4_error : 1; /* [19]  */
        unsigned int    reserved_2            : 11; /* [30..20]  */
        unsigned int    calc_ctrl_des_keysame_error : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} sym_calc_ctrl_check_err;

/* Define the union sym_calc_ctrl_check_err_status */
typedef union {
    /* Define the struct bits */
    struct {
        u32 hpp_sym_error_code_clr : 1; /* [0] */
        u32 reserved : 15;              /* [15..1] */
        u32 hpp_error_sym_chan_id : 4;  /* [19..16] */
        u32 reserved1 : 12;             /* [31..20] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} sym_calc_ctrl_check_err_status;

/* Define the union spacc_axi_iraw */
typedef union {
    /* Define the struct bits */
    struct {
        u32 spacc_mmu_in_sym_sec_err_iraw : 1;    /* [0] */
        u32 spacc_mmu_in_hash_sec_err_iraw : 1;   /* [1] */
        u32 spacc_mmu_out_sym_sec_err_iraw : 1;   /* [2] */
        u32 reserved : 5;                         /* [7..3] */
        u32 spacc_mmu_in_sym_nosec_err_iraw : 1;  /* [8] */
        u32 spacc_mmu_in_hash_nosec_err_iraw : 1; /* [9] */
        u32 spacc_mmu_out_sym_nosec_err_iraw : 1; /* [10] */
        u32 reserved2 : 21;                       /* [31..11] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} spacc_axi_iraw;

/* Define the union in_sym_chn_node_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chn_node_level : 8;     /* [7..0] */
        unsigned int reserved_0 : 8;             /* [15..8] */
        unsigned int sym_chn_node_cnt : 8;       /* [23..16] */
        unsigned int reserved_1 : 7;             /* [30..24] */
        unsigned int sym_chn_node_cnt_clear : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} in_sym_chn_node_ctrl;

/* Define the union out_sym_chn_node_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out_sym_chn_node_level : 8; /* [7..0] */
        unsigned int reserved_0 : 24;            /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} out_sym_chn_node_ctrl;

/* Define the union spacc_sym_chn_clear_req */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chn_clear_req : 16; /* [15..0] */
        unsigned int reserved_0 : 16;        /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} spacc_sym_chn_clear_req;

/* Define the union spacc_int_raw_sym_clear_finish */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_raw_sym_clear_finish : 16; /* [15..0] */
        unsigned int reserved_0 : 16;               /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} spacc_int_raw_sym_clear_finish;

/* Define the union sym_chann_raw_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chann_raw_int : 16; /* [0] */
        unsigned int reserved_0 : 16;        /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} sym_chann_raw_int;

/* Define the union sym_chann_raw_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chann_int_en : 16; /* [0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} sym_chann_raw_int_en;

/* Define the union sym_chann_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chann_int : 16; /* [0] */
        unsigned int reserved_0 : 16;    /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} sym_chann_int;

/* Define the union sym_chan0_finish_raw_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chan0_finish_raw_int : 1; /* [0] */
        unsigned int reserved_0 : 31;              /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} sym_chan0_finish_raw_int;

/* Define the union sym_chan0_finish_raw_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chan0_finish_int_en : 1; /* [0] */
        unsigned int reserved_0 : 31;             /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} sym_chan0_finish_raw_int_en;

/* Define the union sym_chan0_finish_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chan0_finish_int : 1; /* [0] */
        unsigned int reserved_0 : 31;          /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} sym_chan0_finish_int;

/* Define the union sym_chann_error_raw_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chan_raw_err_int : 16; /* [0] */
        unsigned int reserved_0 : 16;           /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} sym_chann_error_raw_int;

/* Define the union sym_chann_error_raw_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chan_err_int_en : 16; /* [0] */
        unsigned int reserved_0 : 16;          /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} sym_chann_error_raw_int_en;

/* Define the union sym_chann_error_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chan_err_int : 16; /* [0] */
        unsigned int reserved_0 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} sym_chann_error_int;

/* Define the union in_sym_global_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_rr_weight : 4;         /* [3..0] */
        unsigned int reserved_0 : 4;            /* [7..4] */
        unsigned int sw_sym_rr_weight_lock : 1; /* [8] */
        unsigned int reserved_1 : 23;           /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} in_sym_global_ctrl;

/* Define the union spacc_calc_inc_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int spacc_gcm_incr_disable : 1; /* [0] */
        unsigned int spacc_ccm_incr_disable : 1; /* [1] */
        unsigned int reserved : 30;              /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} spacc_calc_inc_cfg;

/* Define the union U_SPACC_MMU_IN_SYM_CLEAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    sw_mmu_in_sym_clr     : 16; /* [15..0]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_mmu_in_sym_clear;

/* Define the union U_SPACC_MMU_IN_SYM_SEC_UNVLD_VA_TAB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    mmu_in_sym_sec_unvld_va_tab : 24; /* [23..0]  */
        unsigned int    reserved_0            : 3; /* [26..24]  */
        unsigned int    mmu_in_sym_sec_unvld_chn_id : 4; /* [30..27]  */
        unsigned int    mmu_in_sym_sec_unvld_va_vld : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_mmu_in_sym_sec_unvld_va_tab;

/* Define the union U_SPACC_MMU_IN_SYM_SEC_UNVLD_PA_TAB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    mmu_in_sym_sec_unvld_pa_tab : 24; /* [23..0]  */
        unsigned int    reserved_0            : 5; /* [28..24]  */
        unsigned int    mmu_in_sym_sec_unvld_tag_err : 1; /* [29]  */
        unsigned int    mmu_in_sym_sec_unvld_vld_err : 1; /* [30]  */
        unsigned int    mmu_in_sym_sec_unvld_pa_vld : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_mmu_in_sym_sec_unvld_pa_tab;

/* Define the union U_SPACC_MMU_IN_SYM_NOSEC_UNVLD_VA_TAB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    mmu_in_sym_nosec_unvld_va_tab : 24; /* [23..0]  */
        unsigned int    reserved_0            : 3; /* [26..24]  */
        unsigned int    mmu_in_sym_nosec_unvld_chn_id : 4; /* [30..27]  */
        unsigned int    mmu_in_sym_nosec_unvld_va_vld : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_mmu_in_sym_nosec_unvld_va_tab;

/* Define the union U_SPACC_MMU_IN_SYM_NOSEC_UNVLD_PA_TAB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    mmu_in_sym_nosec_unvld_pa_tab : 24; /* [23..0]  */
        unsigned int    reserved_0            : 6; /* [29..24]  */
        unsigned int    mmu_in_sym_nosec_unvld_vld_err : 1; /* [30]  */
        unsigned int    mmu_in_sym_nosec_unvld_pa_vld : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_mmu_in_sym_nosec_unvld_pa_tab;

/* Define the union U_SPACC_MMU_OUT_SYM_CLEAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    sw_mmu_out_sym_clr    : 16; /* [15..0]  */
        unsigned int    reserved_1            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_mmu_out_sym_clear;

/* Define the union U_SPACC_MMU_OUT_SYM_SEC_UNVLD_VA_TAB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    mmu_out_sym_sec_unvld_va_tab : 24; /* [23..0]  */
        unsigned int    reserved_0            : 3; /* [26..24]  */
        unsigned int    mmu_out_sym_sec_unvld_chn_id : 4; /* [30..27]  */
        unsigned int    mmu_out_sym_sec_unvld_va_vld : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_mmu_out_sym_sec_unvld_va_tab;

/* Define the union U_SPACC_MMU_OUT_SYM_SEC_UNVLD_PA_TAB */
typedef union {
    /* Define the struct bits */
    struct {


#endif /* SECURITY_SUBSYS_CIPHER_V3_SRC_DRV_DRIVERS_CORE_DRV_SYMC_V300_H_BODY_PART02_H_ */
