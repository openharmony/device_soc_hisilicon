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

#include "otp_data.h"

otp_data_item g_otp_data_item[] = {
    /* Key operation, support burn, verify */
    {"oem_root_public_key_sha256",  2, 256, TD_TRUE, OTP_ATTR_BURN_KEY | OTP_ATTR_VERIFY_KEY},
    {"tp_root_public_key_sha256",   4, 256, TD_TRUE, OTP_ATTR_BURN_KEY | OTP_ATTR_VERIFY_KEY},
    {"oem_root_symc_key0",          6, 128, TD_TRUE, OTP_ATTR_BURN_KEY | OTP_ATTR_VERIFY_KEY},
    {"oem_root_symc_key1",          7, 128, TD_TRUE, OTP_ATTR_BURN_KEY | OTP_ATTR_VERIFY_KEY},
    {"oem_root_symc_key2",          8, 128, TD_TRUE, OTP_ATTR_BURN_KEY | OTP_ATTR_VERIFY_KEY},
    {"oem_root_symc_key3",          9, 128, TD_TRUE, OTP_ATTR_BURN_KEY | OTP_ATTR_VERIFY_KEY},

    /* Specify flag operation, support write, not read */
    {"tee_owner_sel",            4, 1, TD_TRUE, OTP_ATTR_SPECIFY_FLAG},
    {"oem_rk_deob_en",           5, 1, TD_TRUE, OTP_ATTR_SPECIFY_FLAG},
    {"jtag_key_sel0",            6, 1, TD_TRUE, OTP_ATTR_SPECIFY_FLAG},
    {"jtag_key_sel1",            7, 1, TD_TRUE, OTP_ATTR_SPECIFY_FLAG},
    {"sec_ds_enable",            8, 1, TD_TRUE, OTP_ATTR_SPECIFY_FLAG},
    {"acpu_ds_enable",           9, 1, TD_TRUE, OTP_ATTR_SPECIFY_FLAG},
    {"uboot_redundance",         11, 1, TD_TRUE, OTP_ATTR_SPECIFY_FLAG},
    {"otp_pcie_disable",         12, 1, TD_TRUE, OTP_ATTR_SPECIFY_FLAG},
    {"otp_pcie_ep_boot_disable", 13, 1, TD_TRUE, OTP_ATTR_SPECIFY_FLAG},
    {"bload_dec_en",             14, 1, TD_TRUE, OTP_ATTR_SPECIFY_FLAG},

    /* User data flag operation, support write & read, can't lock */
    {"update_from_uart_disable",    0, 1, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"update_from_sdio_disable",    1, 1, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"update_from_usbdev_disable",  2, 1, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"scs_dbg_disable",             3, 1, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"oem_cw_crc_rd_disable",       8, 8, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"func_jtag_prt_mode",         16, 8, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"soc_jtag_prt_mode",          24, 8, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"uart0_disable",              32, 1, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"uart1_disable",              33, 1, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"uart2_disable",              34, 1, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"uart3_disable",              35, 1, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"uart4_disable",              36, 1, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"uart5_disable",              37, 1, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"oem_version",                64, 32, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},
    {"third_party_version",        96, 32, TD_TRUE, OTP_ATTR_DATA_FLAG_ONEWARY},

    /* User data flag operation, support write & read, can lock */
    {"soc_tee_enable",          512, 8, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"oem_root_symc_key0_flag", 544, 32, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"oem_root_symc_key1_flag", 576, 32, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"oem_root_symc_key2_flag", 608, 32, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"oem_root_symc_key3_flag", 640, 32, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"secure_boot_en",          672, 8, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"double_sign_en",          704, 4, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"gsl_dec_en",              768, 4, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"quick_boot",              800, 4, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"oem_msid",                832, 32, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"third_party_msid",        864, 32, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},
    {"tee_msid",                896, 32, TD_TRUE, OTP_ATTR_DATA_FLAG_LOCKABLE},

    /* offset: 1536b, 48word; bit_width: 16384b, 512word */
    {"ree_user_data",   1536, 16384, TD_TRUE, OTP_ATTR_REE_USER_DATA},

    /* offset: 17920b, 560word; bit_width: 128b, 4word, can't be locked */
    {"tee_sec_version", 17920, 128, TD_TRUE, OTP_ATTR_TEE_DATA_REE_READ},

    /* offset: 18048b, 564word; bit_width: 9088b, 284word */
    {"tee_user_data",   18048, 9088, TD_TRUE, OTP_ATTR_TEE_USER_DATA},
};

otp_data_item *otp_get_data_item(td_void)
{
    return g_otp_data_item;
}

td_u32 otp_get_data_item_num(td_void)
{
    return sizeof(g_otp_data_item) / sizeof(g_otp_data_item[0]);
}

