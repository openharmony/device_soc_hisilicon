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

#ifndef SECURITY_SUBSYS_CIPHER_V3_SRC_DRV_DRIVERS_CORE_DRV_SYMC_V300_H_BODY_PART03_H_
#define SECURITY_SUBSYS_CIPHER_V3_SRC_DRV_DRIVERS_CORE_DRV_SYMC_V300_H_BODY_PART03_H_

        unsigned int    mmu_out_sym_sec_unvld_pa_tab : 24; /* [23..0]  */
        unsigned int    reserved_0            : 5; /* [28..24]  */
        unsigned int    mmu_out_sym_sec_unvld_tag_err : 1; /* [29]  */
        unsigned int    mmu_out_sym_sec_unvld_vld_err : 1; /* [30]  */
        unsigned int    mmu_out_sym_sec_unvld_pa_vld : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_mmu_out_sym_sec_unvld_pa_tab;

/* Define the union U_SPACC_MMU_OUT_SYM_NOSEC_UNVLD_VA_TAB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    mmu_out_sym_nosec_unvld_va_tab : 24; /* [23..0]  */
        unsigned int    reserved_0            : 3; /* [26..24]  */
        unsigned int    mmu_out_sym_nosec_unvld_chn_id : 4; /* [30..27]  */
        unsigned int    mmu_out_sym_nosec_unvld_va_vld : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_mmu_out_sym_nosec_unvld_va_tab;

/* Define the union U_SPACC_MMU_OUT_SYM_NOSEC_UNVLD_PA_TAB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    mmu_out_sym_nosec_unvld_pa_tab : 24; /* [23..0]  */
        unsigned int    reserved_0            : 6; /* [29..24]  */
        unsigned int    mmu_out_sym_nosec_unvld_vld_err : 1; /* [30]  */
        unsigned int    mmu_out_sym_nosec_unvld_pa_vld : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_mmu_out_sym_nosec_unvld_pa_tab;

/* Define the union U_SPACC_CALC_CRG_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    spacc_aes_low_power_enable  : 1; /* [0]  */
        unsigned int    spacc_tdes_low_power_enable : 1; /* [1]  */
        unsigned int    spacc_sm4_low_power_enable  : 1; /* [2]  */
        unsigned int    reserved0                   : 1; /* [3]  */
        unsigned int    spacc_sha1_low_power_enable : 1; /* [4]  */
        unsigned int    spacc_sha2_low_power_enable : 1; /* [5]  */
        unsigned int    spacc_sm3_low_power_enable  : 1; /* [6]  */
        unsigned int    reserved1                   : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} spacc_calc_crg_cfg;

/** @} */ /** <!-- ==== Structure Definition end ==== */


#endif /* SECURITY_SUBSYS_CIPHER_V3_SRC_DRV_DRIVERS_CORE_DRV_SYMC_V300_H_BODY_PART03_H_ */
