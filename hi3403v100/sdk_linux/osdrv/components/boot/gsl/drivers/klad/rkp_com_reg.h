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

#ifndef _RKP_COM_REG_H__
#define _RKP_COM_REG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define RKP_BASE                    0x10111000

#define RKP_SEC_CFG                 0x074
#define RKP_SW_REG                    0x078
#define RKP_CALC_CTRL               0x07c
#define RKP_ERROR                   0x080

/* define the union u_rkp_module_id_0 */
typedef union {
    /* define an unsigned member */
    rkp_module_id_0_tpp tpp;
    rkp_module_id_0_tee tee;
    rkp_module_id_0_ree ree;
    rkp_module_id_0_hpp hpp;
    unsigned int u32;
} rkp_module_id_0;

/* define the union rkp_module_id_1 */
typedef union {
    /* define an unsigned member */
    rkp_module_id_1_tpp tpp;
    rkp_module_id_1_tee tee;
    rkp_module_id_1_ree ree;
    rkp_module_id_1_hpp hpp;
    unsigned int u32;
} rkp_module_id_1;

/* define the union rkp_module_id_2 */
typedef union {
    /* define an unsigned member */
    rkp_module_id_2_tpp tpp;
    rkp_module_id_2_tee tee;
    rkp_module_id_2_ree ree;
    rkp_module_id_2_hpp hpp;
    unsigned int u32;
} rkp_module_id_2;

typedef union {
    unsigned int u32;
} rkp_module_id_3;

/* define the union rkp_rk_info_s */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    ca_vendor_id             : 8; /* [7..0]  */
        unsigned int    csa2_check               : 1; /* [8]  */
        unsigned int    reserved_1               : 23; /* [31..9]  */
    } bits;
    /* define an unsigned member */
    unsigned int    u32;
} rkp_rk_info;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __drv_rkp_h__ */

