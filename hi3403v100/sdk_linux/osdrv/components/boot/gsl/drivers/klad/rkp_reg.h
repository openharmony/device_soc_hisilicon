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

#ifndef __RKP_REG_H_
#define __RKP_REG_H_

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
	uint32_t    u32;
} rkp_klad_choose_tpp;

/* define the union u_rkp_module_id_0_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    reserved_0            : 1; /* [0]  */
		uint32_t    reserved_1            : 1; /* [1]  */
		uint32_t    tpp_decrypt         : 1; /* [2]  */
		uint32_t    tpp_encrypt         : 1; /* [3]  */
		uint32_t    tpp_content_key_scipher : 1; /* [4]  */
		uint32_t    tpp_content_key_mcipher : 1; /* [5]  */
		uint32_t    reserved_2            : 1; /* [6]  */
		uint32_t    tpp_content_key_tscipher : 1; /* [7]  */
		uint32_t    tpp_destination_sm4 : 1; /* [8]  */
		uint32_t    tpp_destination_tdes : 1; /* [9]  */
		uint32_t    tpp_destination_aes : 1; /* [10]  */
		uint32_t    tpp_destination_csa3 : 1; /* [11]  */
		uint32_t    tpp_destination_csa2 : 1; /* [12]  */
		uint32_t    tpp_destination_multi2 : 1; /* [13]  */
		uint32_t    tpp_destination_sm3_hmac : 1; /* [14]  */
		uint32_t    tpp_destination_sha2_hmac : 1; /* [15]  */
		uint32_t    reserved_3            : 8; /* [23..16]  */
		uint32_t    tpp_level_up        : 1; /* [24]  */
		uint32_t    tpp_stage           : 3; /* [27..25]  */
		uint32_t    reserved_4            : 4; /* [31..28]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_0_tpp;

/* define the union u_rkp_module_id_1_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_klad_aes        : 1; /* [0]  */
		uint32_t    tpp_klad_tdes       : 1; /* [1]  */
		uint32_t    tpp_klad_sm4        : 1; /* [2]  */
		uint32_t    reserved_0          : 5; /* [7..3]  */
		uint32_t    tpp_klad_hkl        : 1; /* [8]  */
		uint32_t    tpp_klad_skl        : 1; /* [9]  */
		uint32_t    reserved_1          : 6; /* [15..10]  */
		uint32_t    tpp_no_restriction  : 8; /* [23..16]  */
		uint32_t    tpp_tpp_gpp_access  : 4; /* [27..24]  */
		uint32_t    tpp_tpp_access      : 4; /* [31..28]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_1_tpp;

/* define the union u_rkp_module_id_2_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    reserved_0            : 16; /* [15..0]  */
		uint32_t    tpp_tee_tpp_gpp_access : 8; /* [23..16]  */
		uint32_t    tpp_remap           : 1; /* [24]  */
		uint32_t    tpp_flash_prot_en   : 1; /* [25]  */
		uint32_t    tpp_allowed_nonce   : 1; /* [26]  */
		uint32_t    tpp_c2_checksum_en  : 1; /* [27]  */
		uint32_t    tpp_cm_checksum_en  : 1; /* [28]  */
		uint32_t    tpp_hdcp_rk         : 1; /* [29]  */
		uint32_t    reserved_1          : 2; /* [31..30]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_2_tpp;

/* define the union u_rkp_unique_type_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_unique_type       : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_unique_type_tpp;

/* define the union u_rkp_fixed_rk_data_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_fixed_rk_data     : 16; /* [15..0]  */
		uint32_t    reserved_0            : 16; /* [31..16]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_data_tpp;

/* define the union u_rkp_fixed_rk_key_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_fixed_rk_key      : 16; /* [15..0]  */
		uint32_t    reserved_0            : 16; /* [31..16]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_key_tpp;

/* define the union u_rkp_fixed_rk_key_sel_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_fixed_rk_key_sel  : 2; /* [1..0]  */
		uint32_t    reserved_0            : 30; /* [31..2]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_key_sel_tpp;

/* define the union u_rkp_int_status_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_rkp_int           : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_int_status_tpp;

/* define the union u_rkp_raw_int_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_rkp_raw_int       : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_raw_int_tpp;

/* define the union u_rkp_int_enable_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_int_enable        : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_int_enable_tpp;

typedef union {
	/* define the struct bits */
	struct {
		uint32_t    cpux_error_type_0    : 1; /* [0]  */
		uint32_t    cpux_error_type_1    : 1; /* [1]  */
		uint32_t    cpux_error_type_2    : 1; /* [2]  */
		uint32_t    cpux_error_type_3    : 1; /* [3]  */
		uint32_t    cpux_error_type_4    : 1; /* [4]  */
		uint32_t    cpux_error_type_5    : 1; /* [5]  */
		uint32_t    cpux_error_type_6    : 1; /* [6]  */
		uint32_t    cpux_error_type_7    : 1; /* [7]  */
		uint32_t    cpux_error_type_8    : 1; /* [8]  */
		uint32_t    cpux_error_type_9    : 1; /* [9]  */
		uint32_t    cpux_error_type_10   : 1; /* [10]  */
		uint32_t    cpux_error_type_11   : 1; /* [11]  */
		uint32_t    cpux_error_type_12   : 1; /* [12]  */
		uint32_t    cpux_error_type_13   : 1; /* [13]  */
		uint32_t    cpux_error_type_14   : 1; /* [14]  */
		uint32_t    cpux_error_type_15   : 1; /* [15]  */
		uint32_t    cpux_error_type_16   : 1; /* [16]  */
		uint32_t    cpux_error_type_17   : 1; /* [17]  */
		uint32_t    cpux_error_type_18   : 1; /* [18]  */
		uint32_t    cpux_error_type_19   : 1; /* [19]  */
		uint32_t    cpux_error_type_20   : 1; /* [20]  */
		uint32_t    cpux_error_type_21   : 1; /* [21]  */
		uint32_t    reserved_0           : 2; /* [23..22]  */
		uint32_t    cpux_error_type_24   : 1; /* [24]  */
		uint32_t    cpux_error_type_25   : 1; /* [25]  */
		uint32_t    cpux_error_type_26   : 1; /* [26]  */
		uint32_t    reserved_1           : 5; /* [31..27]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_error_status_cpux;

/* define the union u_rkp_error_status_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_error_type_0    : 1; /* [0]  */
		uint32_t    tpp_error_type_1    : 1; /* [1]  */
		uint32_t    tpp_error_type_2    : 1; /* [2]  */
		uint32_t    tpp_error_type_3    : 1; /* [3]  */
		uint32_t    tpp_error_type_4    : 1; /* [4]  */
		uint32_t    tpp_error_type_5    : 1; /* [5]  */
		uint32_t    tpp_error_type_6    : 1; /* [6]  */
		uint32_t    tpp_error_type_7    : 1; /* [7]  */
		uint32_t    tpp_error_type_8    : 1; /* [8]  */
		uint32_t    tpp_error_type_9    : 1; /* [9]  */
		uint32_t    tpp_error_type_10   : 1; /* [10]  */
		uint32_t    tpp_error_type_11   : 1; /* [11]  */
		uint32_t    tpp_error_type_12   : 1; /* [12]  */
		uint32_t    tpp_error_type_13   : 1; /* [13]  */
		uint32_t    tpp_error_type_14   : 1; /* [14]  */
		uint32_t    tpp_error_type_15   : 1; /* [15]  */
		uint32_t    tpp_error_type_16   : 1; /* [16]  */
		uint32_t    tpp_error_type_17   : 1; /* [17]  */
		uint32_t    tpp_error_type_18   : 1; /* [18]  */
		uint32_t    tpp_error_type_19   : 1; /* [19]  */
		uint32_t    tpp_error_type_20   : 1; /* [20]  */
		uint32_t    tpp_error_type_21   : 1; /* [21]  */
		uint32_t    tpp_error_type_23_22 : 2; /* [23..22]  */
		uint32_t    tpp_error_type_24   : 1; /* [24]  */
		uint32_t    tpp_error_type_25   : 1; /* [25]  */
		uint32_t    tpp_error_type_26   : 1; /* [26]  */
		uint32_t    tpp_error_type_31_27 : 5; /* [31..27]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_error_status_tpp;

/* define the union u_rkp_module_id_disable_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_tpp_gpp_disable : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_disable_tpp;

/* define the union rkp_cas_kdf_static_disable_tpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tpp_cas_kdf_static_choose : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_cas_kdf_static_disable_tpp;

/* define the union u_rkp_calc_start_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_calc_start        : 1; /* [0]  */
		uint32_t    reserved_0            : 3; /* [3..1]  */
		uint32_t    tee_command_type      : 4; /* [7..4]  */
		uint32_t    reserved_1            : 24; /* [31..8]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_calc_start_tee;

/* define the union u_rkp_slot_choose_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_slot_choose       : 4; /* [3..0]  */
		uint32_t    reserved_0            : 28; /* [31..4]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_slot_choose_tee;

/* define the union u_rkp_klad_choose_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    reserved_0            : 4; /* [3..0]  */
		uint32_t    tee_klad_sel          : 4; /* [7..4]  */
		uint32_t    reserved_1            : 24; /* [31..8]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_klad_choose_tee;

/* define the union u_rkp_module_id_0_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    reserved_0            : 1; /* [0]  */
		uint32_t    reserved_1            : 1; /* [1]  */
		uint32_t    tee_decrypt           : 1; /* [2]  */
		uint32_t    tee_encrypt           : 1; /* [3]  */
		uint32_t    reserved_2            : 1; /* [4]  */
		uint32_t    tee_content_key_mcipher : 1; /* [5]  */
		uint32_t    reserved_3            : 1; /* [6]  */
		uint32_t    tee_content_key_tscipher : 1; /* [7]  */
		uint32_t    tee_destination_sm4   : 1; /* [8]  */
		uint32_t    tee_destination_tdes  : 1; /* [9]  */
		uint32_t    tee_destination_aes   : 1; /* [10]  */
		uint32_t    tee_destination_csa3  : 1; /* [11]  */
		uint32_t    tee_destination_csa2  : 1; /* [12]  */
		uint32_t    tee_destination_multi2 : 1; /* [13]  */
		uint32_t    tee_destination_sm3_hmac : 1; /* [14]  */
		uint32_t    tee_destination_sha2_hmac : 1; /* [15]  */
		uint32_t    reserved_4            : 8; /* [23..16]  */
		uint32_t    tee_level_up          : 1; /* [24]  */
		uint32_t    tee_stage             : 3; /* [27..25]  */
		uint32_t    reserved_5            : 4; /* [31..28]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_0_tee;

/* define the union u_rkp_module_id_1_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_klad_aes          : 1; /* [0]  */
		uint32_t    tee_klad_tdes         : 1; /* [1]  */
		uint32_t    tee_klad_sm4          : 1; /* [2]  */
		uint32_t    reserved_0            : 5; /* [7..3]  */
		uint32_t    tee_klad_hkl          : 1; /* [8]  */
		uint32_t    reserved_1            : 7; /* [15..9]  */
		uint32_t    tee_no_restriction    : 8; /* [23..16]  */
		uint32_t    reserved_2            : 8; /* [31..24]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_1_tee;

/* define the union u_rkp_module_id_2_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_tee_only          : 8; /* [7..0]  */
		uint32_t    reserved_0            : 8; /* [15..8]  */
		uint32_t    tee_tee_tpp_gpp_access : 8; /* [23..16]  */
		uint32_t    tee_remap             : 1; /* [24]  */
		uint32_t    tee_flash_prot_en     : 1; /* [25]  */
		uint32_t    tee_allowed_nonce     : 1; /* [26]  */
		uint32_t    tee_c2_checksum_en    : 1; /* [27]  */
		uint32_t    tee_cm_checksum_en    : 1; /* [28]  */
		uint32_t    tee_hdcp_rk           : 1; /* [29]  */
		uint32_t    reserved_1            : 2; /* [31..30]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_2_tee;

/* define the union u_rkp_unique_type_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_unique_type       : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_unique_type_tee;

/* define the union u_rkp_fixed_rk_data_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_fixed_rk_data     : 16; /* [15..0]  */
		uint32_t    reserved_0            : 16; /* [31..16]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_data_tee;

/* define the union u_rkp_fixed_rk_key_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_fixed_rk_key      : 12; /* [11..0]  */
		uint32_t    reserved_0            : 20; /* [31..12]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_key_tee;

/* define the union u_rkp_fixed_rk_key_sel_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_fixed_rk_key_sel  : 2; /* [1..0]  */
		uint32_t    reserved_0            : 30; /* [31..2]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_key_sel_tee;

/* define the union u_rkp_int_status_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_rkp_int           : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_int_status_tee;

/* define the union u_rkp_raw_int_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_rkp_raw_int       : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_raw_int_tee;

/* define the union u_rkp_int_enable_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_int_enable        : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_int_enable_tee;

/* define the union u_rkp_error_status_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_error_type_0      : 1; /* [0]  */
		uint32_t    tee_error_type_1      : 1; /* [1]  */
		uint32_t    tee_error_type_2      : 1; /* [2]  */
		uint32_t    tee_error_type_3      : 1; /* [3]  */
		uint32_t    tee_error_type_4      : 1; /* [4]  */
		uint32_t    tee_error_type_5      : 1; /* [5]  */
		uint32_t    tee_error_type_6      : 1; /* [6]  */
		uint32_t    tee_error_type_7      : 1; /* [7]  */
		uint32_t    tee_error_type_8      : 1; /* [8]  */
		uint32_t    tee_error_type_9      : 1; /* [9]  */
		uint32_t    tee_error_type_10     : 1; /* [10]  */
		uint32_t    tee_error_type_11     : 1; /* [11]  */
		uint32_t    tee_error_type_12     : 1; /* [12]  */
		uint32_t    tee_error_type_13     : 1; /* [13]  */
		uint32_t    tee_error_type_14     : 1; /* [14]  */
		uint32_t    tee_error_type_15     : 1; /* [15]  */
		uint32_t    tee_error_type_16     : 1; /* [16]  */
		uint32_t    tee_error_type_17     : 1; /* [17]  */
		uint32_t    tee_error_type_18     : 1; /* [18]  */
		uint32_t    tee_error_type_19     : 1; /* [19]  */
		uint32_t    tee_error_type_20     : 1; /* [20]  */
		uint32_t    tee_error_type_21     : 1; /* [21]  */
		uint32_t    tee_error_type_31_22  : 10; /* [31..22]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_error_status_tee;

/* define the union rkp_cas_kdf_static_disable_tee */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    tee_cas_kdf_static_choose : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_cas_kdf_static_disable_tee;

/* define the union u_rkp_calc_start_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    rkp_calc_start       : 1; /* [0]  */
		uint32_t    reserved_0            : 3; /* [3..1]  */
		uint32_t    rkp_rk_sel           : 3; /* [6..4] */
		uint32_t    reserved_1            : 1; /* [7] */
		uint32_t    rkp_remap		  : 1;  /* [8]  */
		uint32_t    reserved_2            : 23; /* [9..31]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_calc_start;

/* define the union u_rkp_slot_choose_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_slot_choose      : 4; /* [3..0]  */
		uint32_t    reserved_0            : 28; /* [31..4]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_slot_choose_hpp;

/* define the union u_rkp_klad_choose_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    reserved_0            : 4; /* [3..0]  */
		uint32_t    hpp_klad_sel         : 4; /* [7..4]  */
		uint32_t    reserved_1            : 24; /* [31..8]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_klad_choose_hpp;

/* define the union u_rkp_module_id_0_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    reserved_0            : 1; /* [0]  */
		uint32_t    reserved_1            : 1; /* [1]  */
		uint32_t    hpp_decrypt          : 1; /* [2]  */
		uint32_t    hpp_encrypt          : 1; /* [3]  */
		uint32_t    reserved_2            : 1; /* [4]  */
		uint32_t    hpp_content_key_mcipher : 1; /* [5]  */
		uint32_t    reserved_3            : 1; /* [6]  */
		uint32_t    hpp_content_key_tscipher : 1; /* [7]  */
		uint32_t    hpp_destination_sm4  : 1; /* [8]  */
		uint32_t    hpp_destination_tdes : 1; /* [9]  */
		uint32_t    hpp_destination_aes  : 1; /* [10]  */
		uint32_t    hpp_destination_csa3 : 1; /* [11]  */
		uint32_t    hpp_destination_csa2 : 1; /* [12]  */
		uint32_t    hpp_destination_multi2 : 1; /* [13]  */
		uint32_t    hpp_destination_sm3_hmac : 1; /* [14]  */
		uint32_t    hpp_destination_sha2_hmac : 1; /* [15]  */
		uint32_t    reserved_4            : 8; /* [23..16]  */
		uint32_t    hpp_level_up         : 1; /* [24]  */
		uint32_t    hpp_stage            : 3; /* [27..25]  */
		uint32_t    reserved_5            : 4; /* [31..28]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_0_hpp;

/* define the union u_rkp_module_id_1_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_klad_aes         : 1; /* [0]  */
		uint32_t    hpp_klad_tdes        : 1; /* [1]  */
		uint32_t    hpp_klad_sm4         : 1; /* [2]  */
		uint32_t    reserved_0            : 5; /* [7..3]  */
		uint32_t    hpp_klad_hkl         : 1; /* [8]  */
		uint32_t    reserved_1            : 7; /* [15..9]  */
		uint32_t    hpp_no_restriction   : 8; /* [23..16]  */
		uint32_t    hpp_tpp_gpp_access   : 4; /* [27..24]  */
		uint32_t    reserved_2            : 4; /* [31..28]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_1_hpp;

/* define the union u_rkp_module_id_2_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    reserved_0            : 8; /* [7..0]  */
		uint32_t    hpp_gpp_access       : 8; /* [15..8]  */
		uint32_t    hpp_tee_gpp_tpp_access : 8; /* [23..16]  */
		uint32_t    hpp_remap            : 1; /* [24]  */
		uint32_t    hpp_flash_prot_en    : 1; /* [25]  */
		uint32_t    hpp_allowed_nonce    : 1; /* [26]  */
		uint32_t    hpp_c2_checksum_en   : 1; /* [27]  */
		uint32_t    hpp_cm_checksum_en   : 1; /* [28]  */
		uint32_t    hpp_hdcp_rk          : 1; /* [29]  */
		uint32_t    reserved_1            : 2; /* [31..30]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_2_hpp;

/* define the union u_rkp_unique_type_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_unique_type      : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_unique_type_hpp;

/* define the union u_rkp_fixed_rk_data_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_fixed_rk_data    : 16; /* [15..0]  */
		uint32_t    reserved_0            : 16; /* [31..16]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_data_hpp;

/* define the union u_rkp_fixed_rk_key_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_fixed_rk_key     : 12; /* [11..0]  */
		uint32_t    reserved_0            : 20; /* [31..12]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_key_hpp;

/* define the union u_rkp_fixed_rk_key_sel_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_fixed_rk_key_sel : 2; /* [1..0]  */
		uint32_t    reserved_0            : 30; /* [31..2]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_key_sel_hpp;

/* define the union u_rkp_int_status_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_rkp_int          : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_int_status_hpp;

/* define the union u_rkp_raw_int_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_rkp_raw_int      : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_raw_int_hpp;

/* define the union u_rkp_int_enable_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_int_enable       : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_int_enable_hpp;

/* define the union u_rkp_error_status_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_error_type_0     : 1; /* [0]  */
		uint32_t    hpp_error_type_1     : 1; /* [1]  */
		uint32_t    hpp_error_type_2     : 1; /* [2]  */
		uint32_t    hpp_error_type_3     : 1; /* [3]  */
		uint32_t    reserved_0            : 1; /* [4]  */
		uint32_t    hpp_error_type_5     : 1; /* [5]  */
		uint32_t    hpp_error_type_6     : 1; /* [6]  */
		uint32_t    hpp_error_type_7     : 1; /* [7]  */
		uint32_t    hpp_error_type_8     : 1; /* [8]  */
		uint32_t    reserved_1            : 1; /* [9]  */
		uint32_t    hpp_error_type_10    : 1; /* [10]  */
		uint32_t    hpp_error_type_11    : 1; /* [11]  */
		uint32_t    hpp_error_type_12    : 1; /* [12]  */
		uint32_t    hpp_error_type_13    : 1; /* [13]  */
		uint32_t    hpp_error_type_14    : 1; /* [14]  */
		uint32_t    hpp_error_type_15    : 1; /* [15]  */
		uint32_t    hpp_error_type_16    : 1; /* [16]  */
		uint32_t    hpp_error_type_17    : 1; /* [17]  */
		uint32_t    hpp_error_type_18    : 1; /* [18]  */
		uint32_t    hpp_error_type_19    : 1; /* [19]  */
		uint32_t    hpp_error_type_20    : 1; /* [20]  */
		uint32_t    hpp_error_type_21    : 1; /* [21]  */
		uint32_t    hpp_error_type_25_22 : 4; /* [25..22]  */
		uint32_t    hpp_error_type_26    : 1; /* [26]  */
		uint32_t    hpp_error_type_31_22 : 5; /* [31..27]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_error_status_hpp;

/* define the union u_rkp_module_id_disable_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_tpp_gpp_disable  : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_disable_hpp;

/* define the union rkp_cas_kdf_static_disable_hpp */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    hpp_cas_kdf_static_choose : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_cas_kdf_static_disable_hpp;

/* define the union u_rkp_calc_start_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_calc_start        : 1; /* [0]  */
		uint32_t    reserved_0            : 3; /* [3..1]  */
		uint32_t    ree_command_type      : 4; /* [7..4]  */
		uint32_t    reserved_1            : 24; /* [31..8]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_calc_start_ree;

/* define the union u_rkp_slot_choose_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_slot_choose       : 4; /* [3..0]  */
		uint32_t    reserved_0            : 28; /* [31..4]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_slot_choose_ree;

/* define the union u_rkp_klad_choose_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    reserved_0            : 4; /* [3..0]  */
		uint32_t    ree_klad_sel          : 4; /* [7..4]  */
		uint32_t    reserved_1            : 24; /* [31..8]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_klad_choose_ree;

/* define the union u_rkp_module_id_0_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    reserved_0            : 2; /* [1..0]  */
		uint32_t    ree_decrypt           : 1; /* [2]  */
		uint32_t    ree_encrypt           : 1; /* [3]  */
		uint32_t    reserved_1            : 1; /* [4]  */
		uint32_t    ree_content_key_mcipher : 1; /* [5]  */
		uint32_t    reserved_2            : 1; /* [6]  */
		uint32_t    ree_content_key_tscipher : 1; /* [7]  */
		uint32_t    ree_destination_sm4   : 1; /* [8]  */
		uint32_t    ree_destination_tdes  : 1; /* [9]  */
		uint32_t    ree_destination_aes   : 1; /* [10]  */
		uint32_t    ree_destination_csa3  : 1; /* [11]  */
		uint32_t    ree_destination_csa2  : 1; /* [12]  */
		uint32_t    ree_destination_multi2 : 1; /* [13]  */
		uint32_t    ree_destination_sm3_hmac : 1; /* [14]  */
		uint32_t    ree_destination_sha2_hmac : 1; /* [15]  */
		uint32_t    reserved_3            : 8; /* [23..16]  */
		uint32_t    ree_enable_stage      : 1; /* [24]  */
		uint32_t    ree_stage             : 3; /* [27..25]  */
		uint32_t    reserved_4            : 4; /* [31..28]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_0_ree;

/* define the union u_rkp_module_id_1_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_klad_aes          : 1; /* [0]  */
		uint32_t    ree_klad_tdes         : 1; /* [1]  */
		uint32_t    ree_klad_sm4          : 1; /* [2]  */
		uint32_t    reserved_0            : 5; /* [7..3]  */
		uint32_t    ree_klad_hkl          : 1; /* [8]  */
		uint32_t    reserved_1            : 7; /* [15..9]  */
		uint32_t    ree_no_restriction    : 8; /* [23..16]  */
		uint32_t    reserved_2            : 8; /* [31..24]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_1_ree;

/* define the union u_rkp_module_id_2_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    reserved_0            : 24; /* [23..0]  */
		uint32_t    ree_remap             : 1; /* [24]  */
		uint32_t    ree_flash_prot_en     : 1; /* [25]  */
		uint32_t    ree_allowed_nonce     : 1; /* [26]  */
		uint32_t    ree_c2_checksum_en    : 1; /* [27]  */
		uint32_t    ree_cm_checksum_en    : 1; /* [28]  */
		uint32_t    ree_hdcp_rk           : 1; /* [29]  */
		uint32_t    reserved_1            : 2; /* [31..30]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_module_id_2_ree;

/* define the union u_rkp_unique_type_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_unique_type       : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_unique_type_ree;

/* define the union u_rkp_fixed_rk_data_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_fixed_rk_data     : 16; /* [15..0]  */
		uint32_t    reserved_0            : 16; /* [31..16]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_data_ree;

/* define the union u_rkp_fixed_rk_key_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_fixed_rk_key      : 8; /* [7..0]  */
		uint32_t    reserved_0            : 24; /* [31..8]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_key_ree;

/* define the union u_rkp_fixed_rk_key_sel_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_fixed_rk_key_sel  : 2; /* [1..0]  */
		uint32_t    reserved_0            : 30; /* [31..2]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_fixed_rk_key_sel_ree;

/* define the union u_rkp_int_status_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_rkp_int           : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_int_status_ree;

/* define the union u_rkp_raw_int_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_rkp_raw_int       : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_raw_int_ree;

/* define the union u_rkp_int_enable_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_int_enable        : 1; /* [0]  */
		uint32_t    reserved_0            : 31; /* [31..1]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_int_enable_ree;

/* define the union u_rkp_error_status_ree */
typedef union {
	/* define the struct bits */
	struct {
		uint32_t    ree_error_type_0      : 1; /* [0]  */
		uint32_t    ree_error_type_1      : 1; /* [1]  */
		uint32_t    ree_error_type_2      : 1; /* [2]  */
		uint32_t    ree_error_type_3      : 1; /* [3]  */
		uint32_t    ree_error_type_4      : 1; /* [4]  */
		uint32_t    ree_error_type_5      : 1; /* [5]  */
		uint32_t    ree_error_type_6      : 1; /* [6]  */
		uint32_t    ree_error_type_7      : 1; /* [7]  */
		uint32_t    ree_error_type_8      : 1; /* [8]  */
		uint32_t    ree_error_type_9      : 1; /* [9]  */
		uint32_t    ree_error_type_10     : 1; /* [10]  */
		uint32_t    ree_error_type_11     : 1; /* [11]  */
		uint32_t    ree_error_type_12     : 1; /* [12]  */
		uint32_t    ree_error_type_13     : 1; /* [13]  */
		uint32_t    ree_error_type_14     : 1; /* [14]  */
		uint32_t    ree_error_type_15     : 1; /* [15]  */
		uint32_t    ree_error_type_16     : 1; /* [16]  */
		uint32_t    ree_error_type_17     : 1; /* [17]  */
		uint32_t    ree_error_type_18     : 1; /* [18]  */
		uint32_t    ree_error_type_19     : 1; /* [19]  */
		uint32_t    ree_error_type_20     : 1; /* [20]  */
		uint32_t    ree_error_type_21     : 1; /* [21]  */
		uint32_t    ree_error_type_31_22  : 10; /* [31..22]  */
	} bits;

	/* define an unsigned member */
	uint32_t    u32;
} rkp_error_status_ree;


#endif /* __RKP_REG_H_ */
