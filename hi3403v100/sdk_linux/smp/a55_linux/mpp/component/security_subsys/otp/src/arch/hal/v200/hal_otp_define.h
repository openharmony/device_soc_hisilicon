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

#ifndef HAL_OTP_DEFINE_H
#define HAL_OTP_DEFINE_H

/* Define the union otp_user_work_mode */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_user_work_mode    : 3; /* [2..0]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    user_reserved_lock_en : 1; /* [4]  */
        unsigned int    reserved_1            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_work_mode;

/* Define the union otp_user_key_index */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_user_key_index    : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_key_index;

/* Define the union otp_user_flag_value */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_flag_region_pgm_data : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_flag_value;

/* Define the union otp_user_flag_index */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_flag_region_index : 5; /* [4..0]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_flag_index;

/* Define the union otp_user_rev_addr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_user_reserved_addr : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_rev_addr;

/* Define the union otp_user_lock_sta_addr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_lock_st_addr      : 5; /* [4..0]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_lock_sta_addr;

/* Define the union otp_user_ctrl_sta */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_op_busy           : 1; /* [0]  */
        unsigned int    ree_addr_check_err    : 1; /* [1]  */
        unsigned int    otp_user_lock_err     : 1; /* [2]  */
        unsigned int    key_rd_back_check_err : 1; /* [3]  */
        unsigned int    key_crc_check_ok_flag : 1; /* [4]  */
        unsigned int    user_rev_rd_finish    : 1; /* [5]  */
        unsigned int    user_rev_pgm_finish   : 1; /* [6]  */
        unsigned int    flag_pgm_finish       : 1; /* [7]  */
        unsigned int    key_crc_check_finish  : 1; /* [8]  */
        unsigned int    key_pgm_finish        : 1; /* [9]  */
        unsigned int    key_load_finish       : 1; /* [10]  */
        unsigned int    rd_lock_sta_finish    : 1; /* [11]  */
        unsigned int    key_index_finish      : 4; /* [15..12]  */
        unsigned int    user_rev_addr_finish  : 10; /* [25..16]  */
        unsigned int    fms_state_error       : 1; /* [26]  */
        unsigned int    flag_index_finish     : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_ctrl_sta;

/* Define the union otp_user_oneway0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    update_from_uart_disable : 1; /* [0]  */
        unsigned int    update_from_sdio_disable : 1; /* [1]  */
        unsigned int    update_from_usbdev_disable : 1; /* [2]  */
        unsigned int    scs_dbg_disable       : 1; /* [3]  */
        unsigned int    reserveda0_0          : 4; /* [7..4]  */
        unsigned int    oem_cw_crc_rd_disable : 8; /* [15..8]  */
        unsigned int    func_jtag_prt_mode    : 8; /* [23..16]  */
        unsigned int    soc_jtag_prt_mode     : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_oneway0;

/* Define the union otp_user_oneway1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    uart_disable          : 6; /* [5..0]  */
        unsigned int    reserveda1_0          : 26; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_oneway1;

/* Define the union otp_user_lockable0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    soc_tee_enable        : 8; /* [7..0]  */
        unsigned int    reservedlk0           : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_lockable0;

/* Define the union otp_user_lockable5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    sm4_disable           : 1; /* [0]  */
        unsigned int    sm3_disable           : 1; /* [1]  */
        unsigned int    sw_sm2_disable        : 1; /* [2]  */
        unsigned int    trng_drbg_sm3_disable : 1; /* [3]  */
        unsigned int    tee_owner_sel         : 1; /* [4]  */
        unsigned int    oem_rk_deob_en        : 1; /* [5]  */
        unsigned int    jtag_key_sel          : 2; /* [7..6]  */
        unsigned int    sec_ds_enable         : 1; /* [8]  */
        unsigned int    acpu_ds_enable        : 1; /* [9]  */
        unsigned int    rkp_aes_sm4_sel       : 1; /* [10]  */
        unsigned int    uboot_redundance      : 1; /* [11]  */
        unsigned int    otp_pcie_disable      : 1; /* [12]  */
        unsigned int    otp_pcie_ep_boot_disable : 1; /* [13]  */
        unsigned int    bload_dec_en          : 1; /* [14]  */
        unsigned int    reserved_flag         : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_lockable5;

/* Define the union otp_user_lockable6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_init_rdy          : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_lockable6;

/* Define the union otp_user_lockable8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    secure_boot_en        : 8; /* [7..0]  */
        unsigned int    reservedlk5           : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_lockable8;

/* Define the union otp_user_lockable9 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    double_sign_en        : 4; /* [3..0]  */
        unsigned int    reservedlk6           : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_lockable9;

/* Define the union otp_user_lockable12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    scs_alg_sel           : 4; /* [3..0]  */
        unsigned int    reservedlk7           : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_lockable12;

/* Define the union otp_user_lockable13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    hsl_dec_en            : 4; /* [3..0]  */
        unsigned int    reservedlk8           : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_lockable13;

/* Define the union otp_user_lockable14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    quick_boot            : 4; /* [3..0]  */
        unsigned int    reservedlk9           : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_lockable14;

/* Define the union otp_user_work_mode_tee */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    tee_otp_user_work_mode : 3; /* [2..0]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    tee_user_reserved_lock_en : 1; /* [4]  */
        unsigned int    reserved_1            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_work_mode_tee;

/* Define the union otp_user_rev_addr_tee */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_user_reserved_addr_tee : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_rev_addr_tee;

/* Define the union otp_user_lock_sta_addr_tee */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_lock_st_addr_tee  : 5; /* [4..0]  */
        unsigned int    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_lock_sta_addr_tee;

/* Define the union otp_user_ctrl_sta_tee */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_op_busy_tee       : 1; /* [0]  */
        unsigned int    reserved_0            : 1; /* [1]  */
        unsigned int    otp_user_lock_err_tee : 1; /* [2]  */
        unsigned int    key_rd_back_check_err_tee : 1; /* [3]  */
        unsigned int    key_crc_check_ok_flag_tee : 1; /* [4]  */
        unsigned int    user_rev_rd_finish_tee : 1; /* [5]  */
        unsigned int    user_rev_pgm_finish_tee : 1; /* [6]  */
        unsigned int    flag_pgm_finish_tee   : 1; /* [7]  */
        unsigned int    key_crc_check_finish_tee : 1; /* [8]  */
        unsigned int    key_pgm_finish_tee    : 1; /* [9]  */
        unsigned int    key_load_finish_tee   : 1; /* [10]  */
        unsigned int    rd_lock_sta_finish_tee : 1; /* [11]  */
        unsigned int    key_index_finish_tee  : 4; /* [15..12]  */
        unsigned int    user_rev_addr_finish_tee : 10; /* [25..16]  */
        unsigned int    fms_state_error       : 1; /* [26]  */
        unsigned int    flag_index_finish_tee : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} otp_user_ctrl_sta_tee;

#endif /* HAL_OTP_DEFINE_H */
