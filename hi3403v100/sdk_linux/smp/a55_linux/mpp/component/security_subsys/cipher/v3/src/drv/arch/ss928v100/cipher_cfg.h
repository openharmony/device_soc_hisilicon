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

#ifndef CIPHER_CFG_H
#define CIPHER_CFG_H

/* support read IRQ number from DTS */
#define IRQ_DTS_SUPPORT

/* secure cpu, SMP, liteos of secure AMP, linux of non-secure AMP use secure config.
 * #define CRYPTO_SEC_CPU
 */
/* the loader cpu may be running under non-secure CPU or secure CPU,
 * need to switch the hard info when ree running in non-secure CPU
 */
#define CRYPTO_SWITCH_CPU

/* the total cipher hard channel which we can used */
#define CIPHER_HARD_CHANNEL_CNT         0x0F

/* mask which cipher channel we can used, bit0 means channel 0 */
#define CIPHER_HARD_CHANNEL_MASK        0xFFFE

/* the total hash hard channel which we can used */
#define HASH_HARD_CHANNEL_CNT           0x01

/* mask which cipher channel we can used, bit0 means channel 0 */
#define HASH_HARD_CHANNEL_MASK          0x04
#define HASH_HARD_CHANNEL               0x02

/* support smmu
 * #define CRYPTO_SMMU_SUPPORT
 */
/* support interrupt
 * #define CRYPTO_OS_INT_SUPPORT
*/
/* the hardware version */
#define CHIP_SYMC_VER_V300
#define CHIP_HASH_VER_V300
#define CHIP_TRNG_VER_V200
#define CHIP_PKE_VER_V200

/* support hard SM2
 * #define CHIP_SM2_SUPPORT
 */
/* support hard SM3
 * #define CHIP_SM3_SUPPORT
 */
/* support hard SM4
 * #define CHIP_SM4_SUPPORT
 */
/* support hard CCM/GCM */
#define CHIP_AES_CCM_GCM_SUPPORT

/*
 * #define SOFT_SM3_SUPPORT
 *
 * #define SOFT_SHA1_SUPPORT
 *
 * #define SOFT_SHA256_SUPPORT
 *
 * #define SOFT_SHA512_SUPPORT
 *
 * #define SOFT_RSA_SUPPORT
 */
/* module unsupported, we need set the table */
#define BASE_TABLE_NULL    {        \
    .reset_valid = 0,               \
    .clk_valid = 0,                 \
    .phy_valid = 0,                 \
    .crg_valid = 0,                 \
    .ver_valid = 0,                 \
    .int_valid = 0,                 \
}

/* define initial value of struct sys_arch_hardware_info for cipher */
#define SEC_HARD_INFO_CIPHER {      \
    .name = "int_spacc_tee",        \
    .reset_valid = 0,               \
    .clk_valid = 0,                 \
    .phy_valid = 1,                 \
    .crg_valid = 1,                 \
    .ver_valid = 1,                 \
    .int_valid = 1,                 \
    .int_num = 42,                  \
    .version_reg = 0x308,           \
    .version_val = 0x0,             \
    .reg_addr_phy = 0x10100000,     \
    .reg_addr_size = 0x10000,       \
    .crg_addr_phy = 0x10115304,     \
    .reset_bit = 8,                 \
    .clk_bit = 12,                  \
}

/* define initial value of struct sys_arch_hardware_info for hash */
#define SEC_HARD_INFO_HASH {        \
    .name = "int_spacc_tee",        \
    .reset_valid = 0,               \
    .clk_valid = 0,                 \
    .phy_valid = 1,                 \
    .crg_valid = 0,                 \
    .ver_valid = 0,                 \
    .int_valid = 1,                 \
    .int_num = 42,                  \
    .reg_addr_phy = 0x10100000,     \
    .reg_addr_size = 0x10000,       \
}

/* define initial value of struct sys_arch_hardware_info for rng */
#define SEC_HARD_INFO_TRNG {        \
    .name = "trng",                 \
    .reset_valid = 0,               \
    .clk_valid = 0,                 \
    .phy_valid = 1,                 \
    .crg_valid = 0,                 \
    .ver_valid = 0,                 \
    .int_valid = 0,                 \
    .reg_addr_phy = 0x10114200,     \
    .reg_addr_size = 0x100,         \
}

/* define initial value of struct sys_arch_hardware_info for rsa/sm2/ecc */
#define SEC_HARD_INFO_PKE {         \
    .name = "int_pke_tee",          \
    .reset_valid = 1,               \
    .clk_valid = 1,                 \
    .phy_valid = 1,                 \
    .crg_valid = 1,                 \
    .ver_valid = 0,                 \
    .int_valid = 1,                 \
    .reg_addr_phy = 0x10118000,     \
    .reg_addr_size = 0x2000,        \
    .crg_addr_phy = 0x10115304,     \
    .reset_bit = 0,                 \
    .clk_bit = 4,                   \
    .int_num = 44,                  \
    .version_reg = 0x88,            \
    .version_val = 0x00000009,      \
}

#define SEC_HARD_INFO_SMMU          BASE_TABLE_NULL
#define SEC_HARD_INFO_SIC_RSA       BASE_TABLE_NULL
#define SEC_HARD_INFO_CIPHER_KEY    BASE_TABLE_NULL
#define SEC_HARD_INFO_SM4           BASE_TABLE_NULL
#define SEC_HARD_INFO_IFEP_RSA      BASE_TABLE_NULL

/* define initial value of struct sys_arch_hardware_info for cipher */
#define NSEC_HARD_INFO_CIPHER {     \
    .name = "int_spacc_ree",        \
    .reset_valid = 0,               \
    .clk_valid = 0,                 \
    .phy_valid = 1,                 \
    .crg_valid = 0,                 \
    .ver_valid = 1,                 \
    .int_valid = 1,                 \
    .int_num = 41,                  \
    .version_reg = 0x308,           \
    .version_val = 0x0,             \
    .reg_addr_phy = 0x10100000,     \
    .reg_addr_size = 0x10000,       \
    .crg_addr_phy = 0x10115304,     \
    .reset_bit = 8,                 \
    .clk_bit = 12,                  \
}

/* define initial value of struct sys_arch_hardware_info for hash */
#define NSEC_HARD_INFO_HASH {       \
    .name = "int_spacc_ree",        \
    .reset_valid = 0,               \
    .clk_valid = 0,                 \
    .phy_valid = 1,                 \
    .crg_valid = 0,                 \
    .ver_valid = 1,                 \
    .int_valid = 1,                 \
    .int_num = 41,                  \
    .version_reg = 0x308,           \
    .version_val = 0x0,             \
    .reg_addr_phy = 0x10100000,     \
    .reg_addr_size = 0x10000,       \
    .crg_addr_phy = 0x10115304,     \
    .reset_bit = 8,                 \
    .clk_bit = 12,                  \
}

/* define initial value of struct sys_arch_hardware_info for rng */
#define NSEC_HARD_INFO_TRNG {       \
    .name = "trng",                 \
    .reset_valid = 0,               \
    .clk_valid = 0,                 \
    .phy_valid = 1,                 \
    .crg_valid = 0,                 \
    .ver_valid = 0,                 \
    .int_valid = 0,                 \
    .reg_addr_phy = 0x10114200,     \
    .reg_addr_size = 0x100,         \
}

/* define initial value of struct sys_arch_hardware_info for rsa/ecc/sm2 */
#define NSEC_HARD_INFO_PKE {        \
    .name = "int_pke_ree",          \
    .reset_valid = 1,               \
    .clk_valid = 1,                 \
    .phy_valid = 1,                 \
    .crg_valid = 1,                 \
    .ver_valid = 0,                 \
    .int_valid = 1,                 \
    .reg_addr_phy = 0x10118000,     \
    .reg_addr_size = 0x2000,        \
    .crg_addr_phy = 0x10115304,     \
    .reset_bit = 0,                 \
    .clk_bit = 4,                   \
    .int_num = 43,                  \
    .version_reg = 0x88,            \
    .version_val = 0x00000009,      \
}

#define NSEC_HARD_INFO_CIPHER_KEY       BASE_TABLE_NULL
#define NSEC_HARD_INFO_SIC_RSA          BASE_TABLE_NULL
#define NSEC_HARD_INFO_SMMU             BASE_TABLE_NULL
#define NSEC_HARD_INFO_SM4              BASE_TABLE_NULL
#define NSEC_HARD_INFO_IFEP_RSA         BASE_TABLE_NULL

#define KC_REG_BASE_ADDR_PHY    0x10112000
#define KC_REG_BASE_ADDR_SIZE   0x1000

#endif /* CIPHER_CFG_H */

