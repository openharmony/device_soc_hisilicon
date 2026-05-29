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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART04_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART04_H_

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
#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_KLAD_RKP_REG_H_BODY_PART04_H_ */
