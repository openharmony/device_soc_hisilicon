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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART03_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART03_H_

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


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART03_H_ */
