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
#include <types.h>
#include <common.h>
#include <lib.h>
#include <platform.h>
#include <flash_map.h>

#define NUM_FF  0xff
#define NUM_F   0xf
#define WORD_BTI_LEN 32
#define OTP_SOC_TEE_DISABLE_FLAG  0x42
#define OTP_UBOOT_REDUNDANCE_BIT  11
#define OTP_SECURE_BOOT_DISABLE_FLAG  0x42
#define OTP_DOUBLE_SIGN_DISABLE_FLAG  0xa
#define CHIP_JTAG_SEL_ENABLE_FLAG  0x5
#define OTP_BOOT_INFO_LV_ENABLE_FLAG  0x3c
#define OTP_SCS_DBG_DISABLE_BIT  3

#define OTP_OP_BUSY_MASK                0x1
#define OTP_USER_OP_START_EN            0x1acce551
#define OTP_USER_REV_RD_MODE            0x6
#define OTP_USER_REV_RD_FINISH_MASK        (0x1 << 5)
#define OTP_USER_REV_TEE_SEC_VER_START    0x230
#define OTP_USER_REV_TEE_SEC_VER_END    0x233

typedef union {
    struct {
        uint32_t sm4_disable : 1;        /* [0] */
        uint32_t sm3_disable : 1;        /* [1] */
        uint32_t sw_sm2_disable : 1;        /* [2] */
        uint32_t trng_drbg_sm3_disable : 1;    /* [3] */
        uint32_t tee_owner_sel : 1;        /* [4] */
        uint32_t oem_rk_deob_en : 1;        /* [5] */
        uint32_t jtag_key_sel : 2;        /* [6:7] */
        uint32_t sec_ds_enable : 1;        /* [8] */
        uint32_t acpu_ds_enable : 1;        /* [9] */
        uint32_t rkp_aes_sm4_sel : 1;        /* [10] */
        uint32_t uboot_redundance : 1;        /* [11] */
        uint32_t otp_pcie_disable : 1;        /* [12] */
        uint32_t otp_pcie_ep_boot_disable : 1;    /* [13] */
        uint32_t bload_dec_en : 1;        /* [14] */
        uint32_t rsvd : 17;            /* [15:31] */
    } bits;

    uint32_t val;
} otp_user_lockable5_info;

uint32_t is_tee_enable(void)
{
    if ((NUM_FF & reg_get(OTP_USER_LOCKABLE0)) == OTP_SOC_TEE_DISABLE_FLAG)
        return AUTH_FAILURE;
    else
        return AUTH_SUCCESS;
}

uint32_t get_tee_owner(void)
{
    otp_user_lockable5_info otp_user_reg_val;

    otp_user_reg_val.val = reg_get(OTP_USER_LOCKABLE5);

    return otp_user_reg_val.bits.tee_owner_sel;
}

uint32_t is_backup_image_enable(void)
{
    if ((reg_get(OTP_USER_LOCKABLE5) >> OTP_UBOOT_REDUNDANCE_BIT) & 0x1)
        return AUTH_SUCCESS;
    else
        return AUTH_FAILURE;
}

uint32_t is_bload_dec_en_enable(void)
{
    otp_user_lockable5_info otp_user_reg_val;

    otp_user_reg_val.val = reg_get(OTP_USER_LOCKABLE5);

    if (otp_user_reg_val.bits.bload_dec_en)
        return AUTH_SUCCESS;
    else
        return AUTH_FAILURE;
}

uint32_t is_secure_boot_en_enable(void)
{
    if ((NUM_FF & reg_get(OTP_USER_LOCKABLE8)) == OTP_SECURE_BOOT_DISABLE_FLAG)
        return AUTH_FAILURE;
    else
        return AUTH_SUCCESS;
}

uint32_t is_double_sign_en_enable(void)
{
    if ((NUM_F & reg_get(OTP_USER_LOCKABLE9)) == OTP_DOUBLE_SIGN_DISABLE_FLAG)
        return AUTH_FAILURE;
    else
        return AUTH_SUCCESS;
}

uint32_t is_func_jtag_enable(void)
{
    if ((NUM_F & reg_get(DBC_APB_JTAG_DBG_INFO)) == CHIP_JTAG_SEL_ENABLE_FLAG)
        return AUTH_SUCCESS;
    else
        return AUTH_FAILURE;
}

uint32_t is_boot_info_lv_debug_enable(void)
{
    if ((NUM_FF & reg_get(OTP_PO_INFO_32)) == OTP_BOOT_INFO_LV_ENABLE_FLAG)
        return AUTH_SUCCESS;
    else
        return AUTH_FAILURE;
}

uint32_t is_scs_dbg_enable(void)
{
    if ((reg_get(OTP_USER_ONEWAYO) >> OTP_SCS_DBG_DISABLE_BIT) & 0x1)
        return AUTH_FAILURE;
    else
        return AUTH_SUCCESS;
}

uint32_t get_oem_msid(void)
{
    return reg_get(OTP_USER_LOCKABLE15);
}

uint32_t get_oem_version(void)
{
    return reg_get(OTP_USER_LOCKABLE10);
}

uint32_t get_tee_msid(void)
{
    return reg_get(OTP_USER_LOCKABLE17);
}

static void wait_otp_idle(uint32_t reg_addr)
{
    uint32_t val;
    uint32_t over_time = SECURE_BOOT_OVER_TIME * timer_get_divider();

    timer_start();
    do {
        if (timer_get_val() > over_time) {
            err_print('o', '1');
            break;
        }
        val = reg_get(reg_addr);
    } while (val & OTP_OP_BUSY_MASK);
}

static void wait_otp_user_rev_rd_finish(uint32_t reg_addr)
{
    uint32_t val;
    uint32_t over_time = SECURE_BOOT_OVER_TIME * timer_get_divider();

    timer_start();
    do {
        if (timer_get_val() > over_time) {
            err_print('o', '2');
            break;
        }
        val = reg_get(reg_addr);
    } while (!(val & OTP_USER_REV_RD_FINISH_MASK));
}

uint32_t  read_otp_user_rev(uint32_t addr)
{
    reg_set(OTP_USER_REV_ADDR_TEE, addr);
    reg_set(OTP_USER_WORK_MODE_TEE, OTP_USER_REV_RD_MODE);
    wait_otp_idle(OTP_USER_CTRL_STA_TEE);
    reg_set(OTP_USER_OP_START, OTP_USER_OP_START_EN);
    wait_otp_user_rev_rd_finish(OTP_USER_CTRL_STA_TEE);
    return reg_get(OTP_USER_REV_RDATA_TEE);
}

/*
 * This Funtion is used to get the OTP TEE security version.
 * The input mask_ext refer how many bits of TEE_SecVersion are used in OTP
 *
 * Explanation :
 *   In OTP, TEE_SecVersion is composed of 128 bits.
 *   The number of bit '1' of TEE_SecVersion represents the TEE security version.
 *   For example:
 *                           [31:0]   [63:32]  [64:95]  [96:128]
 *     TEE_SecVersion = 0x0000001F 00000000 00000000 00000000 -- security version = 5
 *     TEE_SecVersion = 0xFFFFFFFF 00000001 00000000 00000000 -- security version = 33
 */
uint32_t get_tee_sec_version(uint32_t mask_ext)
{
    uint32_t security_version = 0;
    uint32_t addr = OTP_USER_REV_TEE_SEC_VER_START;

    while ((addr <= OTP_USER_REV_TEE_SEC_VER_END) && (mask_ext > 0)) {
        uint32_t word = read_otp_user_rev(addr);
        int count = WORD_BTI_LEN;
        while ((mask_ext > 0) && (count > 0)) {
            security_version += word & 1;
            word >>= 1;
            mask_ext--;
            count --;
        }
        addr++;
    }
    return security_version;
}
