/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART01_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART01_H_


/* define the union u_rkp_debug_info_0 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    crc_finish            : 1; /* [0]  */
        uint32_t    cas_jtag_deob_finish  : 1; /* [1]  */
        uint32_t    chip_jtag_kdf_finish  : 1; /* [2]  */
        uint32_t    soc_jtag_kdf_finish   : 1; /* [3]  */
        uint32_t    cas_tpp_jtag_kdf_finish : 1; /* [4]  */
        uint32_t    cas_uart_jtag_kdf_finish : 1; /* [5]  */
        uint32_t    reserved_0            : 10; /* [15..6]  */
        uint32_t    rk_deob_finish   : 1; /* [16]  */
        uint32_t    tpp_jtag_kdf_finish : 1; /* [17]  */
        uint32_t    hpp_jtag_kdf_finish  : 1; /* [18]  */
        uint32_t    tee_priv_jtag_finish : 1; /* [19]  */
        uint32_t    reserved_1            : 12; /* [31..20]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_debug_info_0;

/* define the union u_rkp_debug_info_1 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    vmask_busy_flag       : 1; /* [0]  */
        uint32_t    deob_busy_flag        : 1; /* [1]  */
        uint32_t    inte_busy_flag        : 1; /* [2]  */
        uint32_t    crc_busy_flag         : 1; /* [3]  */
        uint32_t    sck_busy_flag         : 1; /* [4]  */
        uint32_t    sckv_busy_flag        : 1; /* [5]  */
        uint32_t    seedv_busy_flag       : 1; /* [6]  */
        uint32_t    modkv_busy_flag       : 1; /* [7]  */
        uint32_t    eff_rk_busy_flag      : 1; /* [8]  */
        uint32_t    fixed_rk_busy_flag    : 1; /* [9]  */
        uint32_t    reserved_0            : 22; /* [31..10]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_debug_info_1;

/* define the union u_rkp_debug_info_3 */
typedef union {
	/* define the struct bits */
	struct {
        uint32_t    rkp_init_finish       : 1; /* [0]  */
        uint32_t    reserved_0            : 31; /* [31..1]  */
    } bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_debug_info_3;

/* define the union u_rkp_inte_info */
typedef union {
	/* define the struct bits */
	struct {
        uint32_t    reserved_0            : 8; /* [7..0]  */
        uint32_t    slot0_inte_once_vld   : 1; /* [8]  */
        uint32_t    slot1_inte_once_vld   : 1; /* [9]  */
        uint32_t    slot2_inte_once_vld   : 1; /* [10]  */
        uint32_t    slot3_inte_once_vld   : 1; /* [11]  */
        uint32_t    slot4_inte_once_vld   : 1; /* [12]  */
        uint32_t    slot5_inte_once_vld   : 1; /* [13]  */
        uint32_t    slot6_inte_once_vld   : 1; /* [14]  */
        uint32_t    slot7_inte_once_vld   : 1; /* [15]  */
        uint32_t    boot_inte_once_vld    : 1; /* [16]  */
        uint32_t    inte_once_vld    : 1; /* [17]  */
        uint32_t    stbm_inte_once_vld    : 1; /* [18]  */
        uint32_t    slot0_deob_inte_once_vld : 1; /* [19]  */
        uint32_t    slot1_deob_inte_once_vld : 1; /* [20]  */
        uint32_t    slot2_deob_inte_once_vld : 1; /* [21]  */
        uint32_t    slot3_deob_inte_once_vld : 1; /* [22]  */
        uint32_t    slot4_deob_inte_once_vld : 1; /* [23]  */
        uint32_t    slot5_deob_inte_once_vld : 1; /* [24]  */
        uint32_t    slot6_deob_inte_once_vld : 1; /* [25]  */
        uint32_t    slot7_deob_inte_once_vld : 1; /* [26]  */
        uint32_t    boot_deob_inte_once_vld : 1; /* [27]  */
        uint32_t    deob_inte_once_vld : 1; /* [28]  */
        uint32_t    reserved_1            : 3; /* [31..29]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_inte_info;

/* define the union u_rkp_crc_info */
typedef union {
	/* define the struct bits */
	struct {
        uint32_t    reserved_0            : 16; /* [15..0]  */
        uint32_t    boot_crc_once         : 1; /* [16]  */
        uint32_t    stbm_crc_once         : 1; /* [17]  */
        uint32_t    reserved_1            : 14; /* [31..18]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_crc_info;

/* define the union u_rkp_otp_rk_vld */
typedef union {
	/* define the struct bits */
	struct {
        uint32_t    slot0_vld             : 1; /* [0]  */
        uint32_t    slot1_vld             : 1; /* [1]  */
        uint32_t    slot2_vld             : 1; /* [2]  */
        uint32_t    slot3_vld             : 1; /* [3]  */
        uint32_t    slot4_vld             : 1; /* [4]  */
        uint32_t    slot5_vld             : 1; /* [5]  */
        uint32_t    slot6_vld             : 1; /* [6]  */
        uint32_t    slot7_vld             : 1; /* [7]  */
        uint32_t    boot_vld              : 1; /* [8]  */
        uint32_t    vld              : 1; /* [9]  */
        uint32_t    stbm_vld              : 1; /* [10]  */
        uint32_t    reserved_0            : 21; /* [31..11]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_otp_rk_vld;

/* define the union u_rkp_robust_alarm */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    rkp_import_signal_alarm : 1; /* [0]  */
        uint32_t    rkp_crc_alarm         : 1; /* [1]  */
        uint32_t    rkp_fsm_alarm         : 1; /* [2]  */
        uint32_t    rkp_algo_alarm        : 1; /* [3]  */
        uint32_t    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_robust_alarm;

/* define the union u_rkp_slotx_crc_rst */
typedef union {
	/* define the struct bits */
	struct {
        uint32_t    slotx_crc_ok          : 1; /* [0]  */
        uint32_t    slotx_rk_all_zero     : 1; /* [1]  */
        uint32_t    slotx_rk_all_one      : 1; /* [2]  */
        uint32_t    slotx_owner_id_all_zero : 1; /* [3]  */
        uint32_t    slotx_owner_id_all_one : 1; /* [4]  */
        uint32_t    slotx_owner_id_correct : 1; /* [5]  */
        uint32_t    reserved_0            : 26; /* [31..6]  */
    } bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_slotx_crc_rst;

/* define the union u_rkp_slot0_crc_rst */
typedef union {
	/* define the struct bits */
	struct {
        uint32_t    slot0_crc_ok          : 1; /* [0]  */
        uint32_t    slot0_rk_all_zero     : 1; /* [1]  */
        uint32_t    slot0_rk_all_one      : 1; /* [2]  */
        uint32_t    slot0_owner_id_all_zero : 1; /* [3]  */
        uint32_t    slot0_owner_id_all_one : 1; /* [4]  */
        uint32_t    slot0_owner_id_correct : 1; /* [5]  */
        uint32_t    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_slot0_crc_rst;

/* define the union u_rkp_slot1_crc_rst */
typedef union {
	/* define the struct bits */
	struct {
        uint32_t    slot1_crc_ok          : 1; /* [0]  */
        uint32_t    slot1_rk_all_zero     : 1; /* [1]  */
        uint32_t    slot1_rk_all_one      : 1; /* [2]  */
        uint32_t    slot1_owner_id_all_zero : 1; /* [3]  */
        uint32_t    slot1_owner_id_all_one : 1; /* [4]  */
        uint32_t    slot1_owner_id_correct : 1; /* [5]  */
        uint32_t    reserved_0            : 26; /* [31..6]  */
    } bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_slot1_crc_rst;

/* define the union u_rkp_slot2_crc_rst */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    slot2_crc_ok          : 1; /* [0]  */
        uint32_t    slot2_rk_all_zero     : 1; /* [1]  */
        uint32_t    slot2_rk_all_one      : 1; /* [2]  */
        uint32_t    slot2_owner_id_all_zero : 1; /* [3]  */
        uint32_t    slot2_owner_id_all_one : 1; /* [4]  */
        uint32_t    slot2_owner_id_correct : 1; /* [5]  */
        uint32_t    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_slot2_crc_rst;

/* define the union u_rkp_slot3_crc_rst */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    slot3_crc_ok          : 1; /* [0]  */
        uint32_t    slot3_rk_all_zero     : 1; /* [1]  */
        uint32_t    slot3_rk_all_one      : 1; /* [2]  */
        uint32_t    slot3_owner_id_all_zero : 1; /* [3]  */
        uint32_t    slot3_owner_id_all_one : 1; /* [4]  */
        uint32_t    slot3_owner_id_correct : 1; /* [5]  */
        uint32_t    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_slot3_crc_rst;

/* define the union u_rkp_slot4_crc_rst */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    slot4_crc_ok          : 1; /* [0]  */
        uint32_t    slot4_rk_all_zero     : 1; /* [1]  */
        uint32_t    slot4_rk_all_one      : 1; /* [2]  */
        uint32_t    slot4_owner_id_all_zero : 1; /* [3]  */
        uint32_t    slot4_owner_id_all_one : 1; /* [4]  */
        uint32_t    slot4_owner_id_correct : 1; /* [5]  */
        uint32_t    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_slot4_crc_rst;

/* define the union u_rkp_slot5_crc_rst */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    slot5_crc_ok          : 1; /* [0]  */
        uint32_t    slot5_rk_all_zero     : 1; /* [1]  */
        uint32_t    slot5_rk_all_one      : 1; /* [2]  */
        uint32_t    slot5_owner_id_all_zero : 1; /* [3]  */
        uint32_t    slot5_owner_id_all_one : 1; /* [4]  */
        uint32_t    slot5_owner_id_correct : 1; /* [5]  */
        uint32_t    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_slot5_crc_rst;

/* define the union u_rkp_slot6_crc_rst */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    slot6_crc_ok          : 1; /* [0]  */
        uint32_t    slot6_rk_all_zero     : 1; /* [1]  */
        uint32_t    slot6_rk_all_one      : 1; /* [2]  */
        uint32_t    slot6_owner_id_all_zero : 1; /* [3]  */
        uint32_t    slot6_owner_id_all_one : 1; /* [4]  */
        uint32_t    slot6_owner_id_correct : 1; /* [5]  */
        uint32_t    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_slot6_crc_rst;

/* define the union u_rkp_slot7_crc_rst */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    slot7_crc_ok          : 1; /* [0]  */
        uint32_t    slot7_rk_all_zero     : 1; /* [1]  */
        uint32_t    slot7_rk_all_one      : 1; /* [2]  */
        uint32_t    slot7_owner_id_all_zero : 1; /* [3]  */
        uint32_t    slot7_owner_id_all_one : 1; /* [4]  */
        uint32_t    slot7_owner_id_correct : 1; /* [5]  */
        uint32_t    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_slot7_crc_rst;

/* define the union u_rkp_boot_crc_rst */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    boot_crc_ok           : 1; /* [0]  */
        uint32_t    boot_rk_all_zero      : 1; /* [1]  */
        uint32_t    boot_rk_all_one       : 1; /* [2]  */
        uint32_t    boot_owner_id_all_zero : 1; /* [3]  */
        uint32_t    boot_owner_id_all_one : 1; /* [4]  */
        uint32_t    boot_owner_id_correct : 1; /* [5]  */
        uint32_t    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_boot_crc_rst;

/* define the union u_rkp_crc_rst */
typedef union {
	/* define the struct bits */
    struct {
        uint32_t    crc_ok           : 1; /* [0]  */
        uint32_t    rk_all_zero      : 1; /* [1]  */
        uint32_t    rk_all_one       : 1; /* [2]  */
        uint32_t    reserved_0            : 29; /* [31..3]  */
    } bits;

	/* define an unsigned member */
    uint32_t    u32;
} rkp_crc_rst;

/* define the union u_rkp_stbm_crc_rst */
typedef union {
	/* define the struct bits */
    struct {
        uint32_t    stbm_crc_ok           : 1; /* [0]  */
        uint32_t    stbm_rk_all_zero      : 1; /* [1]  */
        uint32_t    stbm_rk_all_one       : 1; /* [2]  */
        uint32_t    reserved_0            : 29; /* [31..3]  */
    } bits;

	/* define an unsigned member */
    uint32_t    u32;
} rkp_stbm_crc_rst;

/* define the union u_rkp_calc_crc_0 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    calc_vmask_crc        : 8; /* [7..0]  */
        uint32_t    calc_clear_rk_crc     : 8; /* [15..8]  */
        uint32_t    calc_fixed_rk_crc     : 8; /* [23..16]  */
        uint32_t    calc_sck_crc          : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_calc_crc_0;

/* define the union u_rkp_calc_crc_1 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    calc_sckv_crc         : 8; /* [7..0]  */
        uint32_t    calc_seedv_crc        : 8; /* [15..8]  */
        uint32_t    calc_modk_crc         : 8; /* [23..16]  */
        uint32_t    calc_eff_rk_crc       : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_calc_crc_1;

/* define the union u_rkp_low_power */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    rkp_low_power_enable  : 4; /* [3..0]  */
        uint32_t    rkp_low_power_enable_lock : 1; /* [4]  */
        uint32_t    reserved_0            : 27; /* [31..5]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_low_power;

/* define the union u_rkp_use_status */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    rkp_cur_right         : 8; /* [7..0]  */
        uint32_t    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_use_status;

/* define the union u_rkp_calc_start_tpp */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    tpp_calc_start      : 1; /* [0]  */
        uint32_t    reserved_0            : 3; /* [3..1]  */
        uint32_t    tpp_command_type    : 4; /* [7..4]  */
        uint32_t    reserved_1            : 8; /* [15..8]  */
        uint32_t    deob_type             : 2; /* [17..16]  */
        uint32_t    reserved_2            : 14; /* [31..18]  */
    } bits;

    /* define an unsigned member */
    uint32_t    u32;
} rkp_calc_start_tpp;

/* define the union u_rkp_slot_choose_tpp */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t    tpp_slot_choose     : 4; /* [3..0]  */
        uint32_t    reserved_0            : 28; /* [31..4]  */
    } bits;

	/* define an unsigned member */
    uint32_t    u32;
} rkp_slot_choose_tpp;

/* define the union u_rkp_klad_choose_tpp */
typedef union {
	/* define the struct bits */
    struct {
        uint32_t    tpp_klad_type_sel   : 1; /* [0]  */
        uint32_t    reserved_0            : 3; /* [3..1]  */
        uint32_t    tpp_klad_sel        : 4; /* [7..4]  */
        uint32_t    reserved_1            : 24; /* [31..8]  */
    } bits;

	/* define an unsigned member */


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART01_H_ */
