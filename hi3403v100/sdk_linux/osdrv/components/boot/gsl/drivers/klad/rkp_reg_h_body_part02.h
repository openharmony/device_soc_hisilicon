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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART02_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART02_H_

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


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART02_H_ */
