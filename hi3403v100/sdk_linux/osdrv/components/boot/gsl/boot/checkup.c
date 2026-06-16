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
#include <securecutil.h>
#include "../drivers/cipher/cipher.h"
#include "../drivers/klad/ot_klad.h"
#include "../drivers/otp/otp.h"
#include "../drivers/tzasc/tzasc.h"
#include "../drivers/key_slot/key_ctrl.h"
#include "checkup.h"

static uintptr_t g_tee_img_sec_addr;

static uint32_t g_key_slot;
typedef struct {
    unsigned char *iv;
    unsigned char *prot_key_l1_enc;
    unsigned char *prot_key_l2_enc;
} iv_key_info_s;

static void decrypt_field(uintptr_t field_addr, size_t len,
    hpp_keyladder_sel keyladder, iv_key_info_s iv_key_info);

static uint32_t count_bit(uint32_t n)
{
    uint32_t num = 0;

    while (n > 0) {
        num += n & 1;
        n >>= 1;
    }

    return num;
}

uint32_t set_key_slot(uint32_t slot)
{
    g_key_slot = slot;

    if (kc_slot_is_lock(KC_SLOT_MC, g_key_slot))
        kc_slot_unlock(KC_SLOT_MC, g_key_slot);

    return kc_slot_auto_lock(KC_SLOT_MC, &g_key_slot);
}

uint32_t unset_key_slot(void)
{
    return kc_slot_unlock(KC_SLOT_MC, g_key_slot);
}

uint32_t get_gsl_code_area_len()
{
    gsl_key_area_s *gsl_key_area = NULL;

    gsl_key_area = (gsl_key_area_s *)(uintptr_t)GSL_KEY_AREA_ADDR;

    return (uint32_t)gsl_key_area->gsl_code_area_len;
}

uint32_t get_bootloader_unchecked_area_len()
{
    uint32_t gsl_code_area_len;
    bootloader_params_area_s *bootloader_params_area = NULL;

    gsl_code_area_len = get_gsl_code_area_len();
    bootloader_params_area = (bootloader_params_area_s *)(uintptr_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
                 BOOTLOADER_PARAMS_AREA_ADDR_OFFSET + gsl_code_area_len);

    return (uint32_t)bootloader_params_area->unchecked_area_len;
}

static uint32_t get_bootloader_checked_area_len()
{
    uint32_t gsl_code_area_len;
    bootloader_params_area_s *bootloader_params_area = NULL;

    gsl_code_area_len = get_gsl_code_area_len();
    bootloader_params_area = (bootloader_params_area_s *)(uintptr_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
                 BOOTLOADER_PARAMS_AREA_ADDR_OFFSET + gsl_code_area_len);

    return (uint32_t)bootloader_params_area->checked_area_len;
}

uint32_t get_boot_image_int_ddr_addr()
{
    return (uint32_t)DDR_DOWNLOAD_ADDR;
}

uint32_t get_ddr_decrypt_bootloader_addr()
{
    uint32_t gsl_code_area_len;
    bootloader_params_area_s *bootloader_params_area = NULL;

    gsl_code_area_len = get_gsl_code_area_len();
    bootloader_params_area = (bootloader_params_area_s *)(uintptr_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
                 BOOTLOADER_PARAMS_AREA_ADDR_OFFSET + gsl_code_area_len);

    /* to do: config high 32 bit address */
    return (uint32_t)bootloader_params_area->boot_entry_point;
}

uint32_t get_boot_image_total_len()
{
    uint32_t boot_image_total_len;
    uint32_t gsl_code_area_len;
    uint32_t unchecked_area_len;
    uint32_t checked_area_len;

    gsl_code_area_len = get_gsl_code_area_len();
    unchecked_area_len = get_bootloader_unchecked_area_len();
    checked_area_len = get_bootloader_checked_area_len();

    boot_image_total_len = (uint32_t)(BOOTLOADER_BOOT_CODE_ID_OFFSET + gsl_code_area_len +
                      unchecked_area_len + checked_area_len + RSA_4096_LEN + RSA_4096_LEN);

    return boot_image_total_len;
}

static int copy_boot_image_to_decrypt_addr()
{
    errno_t err;
    uint32_t boot_image_int_ddr_addr;
    uint32_t decrypt_bootloader_addr;
    uint32_t boot_image_total_len;

    boot_image_int_ddr_addr = get_boot_image_int_ddr_addr();
    decrypt_bootloader_addr = get_ddr_decrypt_bootloader_addr();
    boot_image_total_len = get_boot_image_total_len();

    err = memcpy_s((void *)(uintptr_t)decrypt_bootloader_addr, boot_image_total_len,
        (void *)(uintptr_t)boot_image_int_ddr_addr, boot_image_total_len);
    if (err != EOK)
        return TD_FAILURE;

    return TD_SUCCESS;
}

    static int check_bootloader_key_area_msid(const bootloader_key_area_s *bootloader_key_area)
{
    unsigned int oem_msid;

    oem_msid = get_oem_msid();
    if ((bootloader_key_area->oem_msid_ext & bootloader_key_area->oem_msid_ext_mask) ==
        (oem_msid & bootloader_key_area->oem_msid_ext_mask))
        return TD_SUCCESS;

    return TD_FAILURE;
}

    static int check_bootloader_key_area_version(const bootloader_key_area_s *bootloader_key_area)
{
    unsigned int oem_version;

    oem_version = get_oem_version();
    if ((count_bit(bootloader_key_area->oem_version_ext & bootloader_key_area->oem_version_ext_mask))
        >= (count_bit(oem_version & bootloader_key_area->oem_version_ext_mask)))
        return TD_SUCCESS;

    return TD_FAILURE;
}

static int check_bootloader_key_area_certificate_id(
    const bootloader_key_area_s *bootloader_key_area)
{
    if (bootloader_key_area->certificate_id == BOOTLOADER_KEY_AREA_CERTIFICATE_ID)
        return TD_SUCCESS;

    return TD_FAILURE;
}

static int check_bootloader_key_area_structure_version(
    const bootloader_key_area_s *bootloader_key_area)
{
    if (bootloader_key_area->structure_version == GSL_KEY_AREA_STRUCTURE_VERSION)
        return TD_SUCCESS;

    return TD_FAILURE;
}

    static int check_bootloader_params_area_msid(const bootloader_params_area_s *bootloader_params_area)
{
    unsigned int oem_msid;

    oem_msid = get_oem_msid();
    if ((bootloader_params_area->oem_msid_ext & bootloader_params_area->oem_msid_ext_mask) ==
        (oem_msid & bootloader_params_area->oem_msid_ext_mask))
        return TD_SUCCESS;

    return TD_FAILURE;
}

static int check_bootloader_params_area_version(
    const bootloader_params_area_s *bootloader_params_area)
{
    unsigned int oem_version;

    oem_version = get_oem_version();
    if ((count_bit(bootloader_params_area->oem_version_ext &
         bootloader_params_area->oem_version_ext_mask))
        >= (count_bit(oem_version & bootloader_params_area->oem_version_ext_mask)))
        return TD_SUCCESS;

    return TD_FAILURE;
}

static int check_bootloader_params_area_params_id(
    const bootloader_params_area_s *bootloader_params_area)
{
    if (bootloader_params_area->params_id == BOOTLOADER_PARAMS_AREA_PARAMS_ID)
        return TD_SUCCESS;

    return TD_FAILURE;
}

static int check_bootloader_params_area_structure_version(
    const bootloader_params_area_s *bootloader_params_area)
{
    if (bootloader_params_area->structure_version == BOOTLOADER_PARAMS_AREA_STRUCTURE_VERSION)
        return TD_SUCCESS;

    return TD_FAILURE;
}

static int check_bootloader_head_area_len(const bootloader_params_area_s *bootloader_params_area)
{
    if (bootloader_params_area->unchecked_area_len + bootloader_params_area->checked_area_len +
        RSA_4096_LEN + RSA_4096_LEN == bootloader_params_area->boot_area_len)

        return TD_SUCCESS;

    return TD_FAILURE;
}

static uint32_t check_bootloader_code_area_enc_flag(
    const bootloader_params_area_s *bootloader_params_area)
{
    if ((is_bload_dec_en_enable() == AUTH_FAILURE) &&
        (bootloader_params_area->boot_enc_flag == BOOTLOADER_BOOT_CODE_ENC_FLAG))
        return AUTH_FAILURE;

    return AUTH_SUCCESS;
}

    static int check_bootloader_code_area_msid(const bootloader_boot_msid_version_s *boot_msid_version)
{
    unsigned int oem_msid;

    oem_msid = get_oem_msid();
    if ((boot_msid_version->oem_msid_ext & boot_msid_version->oem_msid_ext_mask) ==
        (oem_msid & boot_msid_version->oem_msid_ext_mask))
        return TD_SUCCESS;

    return TD_FAILURE;
}

static int check_bootloader_code_area_version(
    const bootloader_boot_msid_version_s *boot_msid_version)
{
    unsigned int oem_version;

    oem_version = get_oem_version();
    if ((count_bit(boot_msid_version->oem_version_ext & boot_msid_version->oem_version_ext_mask))
        >= (count_bit(oem_version & boot_msid_version->oem_version_ext_mask)))
        return TD_SUCCESS;

    return TD_FAILURE;
}

static int check_bootloader_code_area_id(uint32_t boot_code_id)
{
    if (boot_code_id == BOOTLOADER_BOOT_CODE_ID)
        return TD_SUCCESS;

    return TD_FAILURE;
}

static int check_bootloader_code_area_structure_version(uint32_t boot_structure_version)
{
    if (boot_structure_version == BOOTLOADER_CODE_AREA_STRUCTURE_VERSION)
        return TD_SUCCESS;

    return TD_FAILURE;
}

static uint32_t is_scs_enable()
{
    uint32_t secure_boot_en;
    uint32_t scs_simulate_flag;
    uint32_t gsl_code_area_len;

    gsl_code_area_len = get_gsl_code_area_len();
    scs_simulate_flag = *(unsigned int *)(uintptr_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
                BOOTLOADER_UNCHECKED_AREA_FOR_VENDOR_OFFSET + gsl_code_area_len);

    secure_boot_en = is_secure_boot_en_enable();
    if ((secure_boot_en == AUTH_FAILURE) && (scs_simulate_flag == SCS_SIMULATE_AUTH_FLAG))
        return AUTH_FAILURE;

    return AUTH_SUCCESS;
}

static int confirm_sha256_value(uint32_t data_addr, uint32_t data_len,
                const uint8_t *data_sha_embed)
{
    uint8_t data_sha[SHA_256_LEN];
    uint8_t data_sha_b[SHA_256_LEN];
    volatile int ret;
    errno_t err;

    err = memset_s(data_sha, SHA_256_LEN, 0xA5, SHA_256_LEN);
    if (err != EOK)
        return TD_FAILURE;

    err = memset_s(data_sha_b, SHA_256_LEN, 0x5A, SHA_256_LEN);
    if (err != EOK)
        return TD_FAILURE;

    ret = calc_sha(data_addr, data_len, data_sha, SHA_256_LEN);
    if (ret != TD_SUCCESS)
        return TD_FAILURE;

    ret = memcmp(data_sha_embed, data_sha, SHA_256_LEN);
    if (ret != 0)
        return TD_FAILURE;

    ret = calc_sha(data_addr, data_len, data_sha_b, SHA_256_LEN);
    if (ret != TD_SUCCESS)
        return TD_FAILURE;

    ret = memcmp(data_sha_embed, data_sha_b, SHA_256_LEN);
    if (ret != 0)
        return TD_FAILURE;

    return TD_SUCCESS;
}

static int confirm_signature_value(const uint8_t *key_n, const uint8_t *key_e, const uint32_t data,
    const uint32_t data_len, const uint8_t *signature)
{
    int ret;
    errno_t err;
    uint8_t *key = NULL;

    key = (uint8_t *)gsl_malloc(RSA_TOTAL_LEN);
	if (key == NULL)
		return TD_FAILURE;

	err = memset_s(key, RSA_TOTAL_LEN, 0, RSA_TOTAL_LEN);
	if (err != EOK) {
        gsl_free(key);
        return TD_FAILURE;
	}

	err = memcpy_s(key, RSA_TOTAL_LEN, key_n, RSA_4096_LEN);
	if (err != EOK) {
        gsl_free(key);
        return TD_FAILURE;
	}

	err = memcpy_s(key + RSA_TOTAL_LEN - RSA_E_LEN, RSA_E_LEN, key_e, RSA_E_LEN);
	if (err != EOK) {
        gsl_free(key);
        return TD_FAILURE;
    }

	ret = secure_authenticate(key, data, data_len, signature);
	if (ret != TD_SUCCESS) {
        gsl_free(key);
        return TD_FAILURE;
	}

    gsl_free(key);

    return TD_SUCCESS;
}

int handle_bootloader_key_area()
{
    int ret;
    uint32_t gsl_code_area_len;
    bootloader_key_area_s *bootloader_key_area = NULL;
    root_public_key_s *oem_root_public_key = NULL;

    gsl_code_area_len = get_gsl_code_area_len();
    bootloader_key_area = (bootloader_key_area_s *)(uintptr_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
            BOOTLOADER_KEY_AREA_ADDR_OFFSET + gsl_code_area_len);

    oem_root_public_key = (root_public_key_s *)(uintptr_t)OEM_ROOT_PUBLIC_KEY_ADDR;
    ret = confirm_sha256_value((uintptr_t)oem_root_public_key, RSA_4096_LEN + RSA_E_LEN,
        oem_root_public_key->checksum_root_public_key);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('5', '1');
        failure_process();
    }

    ret = confirm_signature_value(oem_root_public_key->modulus,
        oem_root_public_key->exponent,
        (uintptr_t)bootloader_key_area,
        (uint32_t)sizeof(bootloader_key_area_s) - RSA_4096_LEN,
        (uint8_t *)bootloader_key_area->bootloader_key_sig);
	if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
		err_print('5', '2');
		failure_process();
	}

    ret = check_bootloader_key_area_msid(bootloader_key_area);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('5', '3');
        failure_process();
    }

    ret = check_bootloader_key_area_version(bootloader_key_area);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('5', '4');
        failure_process();
    }

    ret = check_bootloader_key_area_certificate_id(bootloader_key_area);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('5', '5');
        failure_process();
    }

    ret = check_bootloader_key_area_structure_version(bootloader_key_area);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('5', '6');
        failure_process();
    }

    return TD_SUCCESS;
}

int handle_bootloader_params_area()
{
    int ret;
    uint32_t gsl_code_area_len;
    bootloader_key_area_s *bootloader_key_area = NULL;
    bootloader_params_area_s *bootloader_params_area = NULL;

    gsl_code_area_len = get_gsl_code_area_len();
    bootloader_key_area = (bootloader_key_area_s *)(uintptr_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
            BOOTLOADER_KEY_AREA_ADDR_OFFSET + gsl_code_area_len);
    bootloader_params_area = (bootloader_params_area_s *)(uintptr_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
                 BOOTLOADER_PARAMS_AREA_ADDR_OFFSET + gsl_code_area_len);

    ret = confirm_signature_value(bootloader_key_area->modulus,
        bootloader_key_area->exponent,
        (uintptr_t)bootloader_params_area,
        (uint32_t)sizeof(bootloader_params_area_s) - RSA_4096_LEN,
        (uint8_t *)bootloader_params_area->bootloader_params_sig);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('6', '1');
        failure_process();
    }

    ret = check_bootloader_params_area_msid(bootloader_params_area);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('6', '2');
        failure_process();
    }

    ret = check_bootloader_params_area_version(bootloader_params_area);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('6', '3');
        failure_process();
    }

    ret = check_bootloader_params_area_params_id(bootloader_params_area);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('6', '4');
        failure_process();
    }

    ret = check_bootloader_params_area_structure_version(bootloader_params_area);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('6', '5');
        failure_process();
    }

    ret = check_bootloader_head_area_len(bootloader_params_area);
    if (ret != TD_SUCCESS) {
        err_print('6', '6');
        failure_process();
    }

    return TD_SUCCESS;
}

static int obtain_bootloader_code_area_info(bootloader_code_area_info_s *bootloader_info)
{
    int ret;
    uint32_t gsl_code_area_len;
    uint32_t unchecked_area_len;
    uint32_t decrypt_bootloader_addr;

    ret = copy_boot_image_to_decrypt_addr();
    if (ret != TD_SUCCESS)
        return TD_FAILURE;

    gsl_code_area_len = get_gsl_code_area_len();
    unchecked_area_len = get_bootloader_unchecked_area_len();
    bootloader_info->checked_area_len = get_bootloader_checked_area_len();
    decrypt_bootloader_addr = get_ddr_decrypt_bootloader_addr();

    bootloader_info->bootloader_key_area = (bootloader_key_area_s *)
             (uintptr_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
            BOOTLOADER_KEY_AREA_ADDR_OFFSET + gsl_code_area_len);
    bootloader_info->bootloader_params_area = (bootloader_params_area_s *)
            (uintptr_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
            BOOTLOADER_PARAMS_AREA_ADDR_OFFSET + gsl_code_area_len);
    bootloader_info->gsl_third_party_key = (gsl_third_party_key_s *)
            (uintptr_t)GSL_THIRD_PARTY_KEY_ADDR;
    bootloader_info->boot_area_addr = (uint32_t)(decrypt_bootloader_addr +
            BOOTLOADER_BOOT_CODE_ID_OFFSET + gsl_code_area_len + unchecked_area_len);
    bootloader_info->boot_code_addr = (uint32_t)(decrypt_bootloader_addr +
            BOOTLOADER_BOOT_CODE_OFFSET + gsl_code_area_len + unchecked_area_len);
    bootloader_info->boot_third_party_sig_addr = (uint32_t)(bootloader_info->boot_area_addr +
            bootloader_info->checked_area_len + RSA_4096_LEN);
    bootloader_info->boot_msid_version = (bootloader_boot_msid_version_s *)
            (uintptr_t)(decrypt_bootloader_addr + BOOTLOADER_OEM_MSID_EXT_OFFSET +
            gsl_code_area_len + unchecked_area_len + bootloader_info->checked_area_len);
    bootloader_info->boot_code_id_addr = bootloader_info->boot_area_addr;
    bootloader_info->boot_structure_version_addr = (uint32_t)(bootloader_info->boot_code_id_addr +
            BOOTLOADER_BOOT_CODE_ID_SIZE);

    return TD_SUCCESS;
}

static void bootloader_verify_third_party(const bootloader_code_area_info_s *bootloader_info)
{
    int ret;

    ret = confirm_signature_value(bootloader_info->gsl_third_party_key->modulus,
        bootloader_info->gsl_third_party_key->exponent,
        bootloader_info->boot_area_addr,
        (uint32_t)(bootloader_info->checked_area_len),
        (uint8_t *)(uintptr_t)(bootloader_info->boot_third_party_sig_addr));
    if ((ret != TD_SUCCESS) && (is_double_sign_en_enable() == AUTH_SUCCESS)) {
        err_print('9', '2');
        failure_process();
    }
}

static void bootloader_decrypt_if_needed(const bootloader_code_area_info_s *bootloader_info)
{
	iv_key_info_s iv_key_info = {0};

    if (AUTH_SUCCESS == check_bootloader_code_area_enc_flag(
        bootloader_info->bootloader_params_area)) {
        iv_key_info.iv = bootloader_info->bootloader_params_area->boot_iv;
        iv_key_info.prot_key_l1_enc =
			bootloader_info->bootloader_params_area->boot_protection_key_l1_enc;
        iv_key_info.prot_key_l2_enc =
			bootloader_info->bootloader_params_area->boot_protection_key_l2_enc;
        decrypt_field(
            (uintptr_t)bootloader_info->boot_code_addr,
            bootloader_info->checked_area_len - BOOTLOADER_BOOT_HEAD_SIZE,
            KLAD_BOOT,
            iv_key_info);
    }
}

static void bootloader_verify_oem(const bootloader_code_area_info_s *bootloader_info)
{
    int ret;

    ret = confirm_signature_value(bootloader_info->bootloader_key_area->modulus,
        bootloader_info->bootloader_key_area->exponent,
        bootloader_info->boot_area_addr,
        (uint32_t)(bootloader_info->checked_area_len),
        (uint8_t *)(uintptr_t)(bootloader_info->boot_area_addr + bootloader_info->checked_area_len));
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('9', '3');
        failure_process();
    }
}

static void bootloader_check_metadata(const bootloader_code_area_info_s *bootloader_info)
{
    int ret;

    ret = check_bootloader_code_area_msid(bootloader_info->boot_msid_version);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('9', '4');
        failure_process();
    }

    ret = check_bootloader_code_area_version(bootloader_info->boot_msid_version);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('9', '5');
        failure_process();
    }

    ret = check_bootloader_code_area_id(*(unsigned int *)(uintptr_t)bootloader_info->boot_code_id_addr);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('9', '6');
        failure_process();
    }

    ret = check_bootloader_code_area_structure_version(*(unsigned int *)
        (uintptr_t)bootloader_info->boot_structure_version_addr);
    if ((ret != TD_SUCCESS) && (is_scs_enable() == AUTH_SUCCESS)) {
        err_print('9', '7');
        failure_process();
    }
}

int handle_bootloader_code_area()
{
	int ret;
	bootloader_code_area_info_s bootloader_info = {0};

    ret = obtain_bootloader_code_area_info(&bootloader_info);
    if (ret != TD_SUCCESS) {
        err_print('9', '1');
        failure_process();
    }

    bootloader_verify_third_party(&bootloader_info);
    bootloader_decrypt_if_needed(&bootloader_info);
    bootloader_verify_oem(&bootloader_info);
    bootloader_check_metadata(&bootloader_info);
	return TD_SUCCESS;
}

static void decrypt_field(uintptr_t field_addr, size_t len, hpp_keyladder_sel keyladder,
    iv_key_info_s iv_key_info)
{
    int ret;
    errno_t err;
    uint32_t spacc_channel_number = 1;
    symc_alg decrypt_alg = SYMC_ALG_AES;
    spacc_decrypt_params decrypt_params = {0};
    hpp_klad klad = {
        .alg = HPP_ALG_AES,
        .cfg = {
            .is_odd = 0,
            .key_slot_num = g_key_slot
        }
    };

    err = memcpy_s(klad.session_key0, KLAD_KEY0_LEN, iv_key_info.prot_key_l1_enc, KLAD_KEY0_LEN);
    if (err != EOK) {
        err_print('f', '1');
        return;
    }

    err = memcpy_s(klad.session_key1, KLAD_KEY1_LEN, iv_key_info.prot_key_l2_enc, KLAD_KEY1_LEN);
    if (err != EOK) {
        err_print('f', '2');
        return;
    }

    ret = hpp_klad_process(keyladder, &klad);
    if (ret != TD_SUCCESS) {
        err_print('f', '3');
        return;
    }

    decrypt_params.chn = spacc_channel_number;
    decrypt_params.dst_addr = (uint32_t)field_addr;
    decrypt_params.src_addr = (uint32_t)field_addr;
    decrypt_params.length = (uint32_t)len;
    decrypt_params.iv_addr = (uint32_t)(uintptr_t)iv_key_info.iv;
    decrypt_params.iv_length = IV_LEN;
    decrypt_params.alg = decrypt_alg;
    decrypt_params.mode = SYMC_MODE_CBC;
    ret = drv_spacc_decrypt(decrypt_params);
    if (ret != TD_SUCCESS) {
        err_print('f', '3');
        return;
    }
    return;
}

static int check_tee_msid(const tee_key_area_s *tee_key_area)
{
    uint32_t tee_msid = get_tee_msid();
    uint32_t mask = tee_key_area->tee_msid_ext_mask;
    uint32_t tee_msid_ext = tee_key_area->tee_msid_ext;

    if ((tee_msid_ext & mask) == (tee_msid & mask))
        return TD_SUCCESS;

    return TD_FAILURE;
}

static int check_tee_sec_version(const tee_key_area_s *tee_key_area)
{
    uint32_t sec_ver_ext = tee_key_area->tee_sec_version_ext;
    uint32_t ext_mask = tee_key_area->tee_sec_version_ext_mask;

    uint32_t flash_sec_ver = (sec_ver_ext <= ext_mask) ? sec_ver_ext : ext_mask;
    uint32_t otp_sec_ver = get_tee_sec_version(ext_mask);
    if (flash_sec_ver >= otp_sec_ver)
        return TD_SUCCESS;

    return TD_FAILURE;
}

static size_t rgn_align(size_t size)
{
    size_t page_size = 1 << TZASC_ALGIN_BITS;
    size_t margin = size % page_size;

    if (margin != 0)
        size += (page_size - margin);

    return size;
}

// REE: TZASC Region 1~7 ...
// TEE: TZASC Region 8~15 ...
static void config_tzasc(uint8_t rgn, uint32_t attr, uint64_t addr, size_t size)
{
    tzasc_set_rgn_attr(rgn, attr);
    tzasc_set_rgn_map(rgn, addr, size);
    tzasc_rgn_enable(rgn);
}

void disable_sec_ddr_bypass()
{
    tzasc_bypass_disable();
}

void config_tee_img_non_sec_ddr()
{
    tee_key_area_s *area = (tee_key_area_s *)TEE_KEY_AREA_LOAD_ADDR;
    uint32_t tee_img_len = TEE_KEY_AREA_SIZE + area->atf_area_len +
            area->tee_code_area_len;
    uint32_t rgn_attr = TZASC_ATTR_SEC_R | TZASC_ATTR_SEC_W | TZASC_ATTR_NOSEC_R |
            TZASC_ATTR_NOSEC_W | TZASC_ATTR_SEC_INV;
    size_t rgn_size = rgn_align(tee_img_len);
    config_tzasc(TZASC_RGN_1, rgn_attr, TEE_KEY_AREA_LOAD_ADDR, rgn_size);
}

void config_key_area_sec_ddr()
{
    tee_key_area_s *area = (tee_key_area_s *)TEE_KEY_AREA_LOAD_ADDR;
    uint32_t rgn_attr = TZASC_ATTR_SEC_R | TZASC_ATTR_SEC_W | TZASC_ATTR_SEC_INV;
    size_t rgn_size = rgn_align(TEE_KEY_AREA_SIZE);
    g_tee_img_sec_addr = (uintptr_t)area->tee_ddr_address;
    config_tzasc(TZASC_RGN_8, rgn_attr, g_tee_img_sec_addr, rgn_size);
}

void config_tee_img_sec_ddr()
{
    tee_key_area_s *area = (tee_key_area_s *)g_tee_img_sec_addr;
    uint32_t rgn_attr = TZASC_ATTR_SEC_R | TZASC_ATTR_SEC_W | TZASC_ATTR_SEC_INV;
    uint32_t rgn_size = rgn_align(area->tee_reserved_ddr_size);
    config_tzasc(TZASC_RGN_8, rgn_attr, g_tee_img_sec_addr, rgn_size);
}

void config_non_sec_ddr()
{
    tee_key_area_s *area = (tee_key_area_s *)g_tee_img_sec_addr;
    uint32_t rgn_attr = TZASC_ATTR_SEC_R | TZASC_ATTR_SEC_W | TZASC_ATTR_NOSEC_R |
            TZASC_ATTR_NOSEC_W | TZASC_ATTR_SEC_INV;
    uintptr_t rgn1_base = DDR_BASE;
    size_t rgn1_size = area->tee_ddr_address - DDR_BASE;
    uintptr_t rgn2_base = area->tee_ddr_address + rgn_align(area->tee_reserved_ddr_size);
    size_t rgn2_size = DDR_END - rgn2_base;

    config_tzasc(TZASC_RGN_1, rgn_attr, rgn1_base, rgn1_size);
    config_tzasc(TZASC_RGN_2, rgn_attr, rgn2_base, rgn2_size);
}

int handle_tee_key_area()
{
    int ret;
    errno_t err;
    gsl_key_area_s *gsl_key_area = (gsl_key_area_s *)GSL_KEY_AREA_ADDR;
    tee_key_area_s *tee_key_area = (tee_key_area_s *)g_tee_img_sec_addr;

    err = memcpy_s((void *)tee_key_area, TEE_KEY_AREA_SIZE,
        (void *)TEE_KEY_AREA_LOAD_ADDR, TEE_KEY_AREA_SIZE);
    if (err != EOK) {
        err_print('a', '0');
        return TD_FAILURE;
    }

    ret = confirm_signature_value(gsl_key_area->modulus,
        gsl_key_area->exponent,
        (uint32_t)(uintptr_t)tee_key_area,
        (uint32_t)(sizeof(tee_key_area_s) - RSA_4096_LEN),
        (uint8_t *)tee_key_area->tee_key_sig);
    if (ret != TD_SUCCESS) {
        err_print('a', '1');
        return TD_FAILURE;
    }

    ret = check_tee_msid(tee_key_area);
    if (ret != TD_SUCCESS) {
        err_print('a', '2');
        return TD_FAILURE;
    }

    ret = check_tee_sec_version(tee_key_area);
    if (ret != TD_SUCCESS) {
        err_print('a', '3');
        return TD_FAILURE;
    }

    if (tee_key_area->certificate_id != TEE_KEY_AREA_CERTIFICATE_ID) {
        err_print('a', '4');
        return TD_FAILURE;
    }

    if (tee_key_area->structure_version != TEE_KEY_AREA_STRUCTURE_VERSION) {
        err_print('a', '5');
        return TD_FAILURE;
    }

    if (g_tee_img_sec_addr != tee_key_area->tee_ddr_address) {
        err_print('a', '6');
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

int handle_atf_area()
{
    int ret;
    tee_key_area_s *tee_key_area = (tee_key_area_s *)g_tee_img_sec_addr;
    uintptr_t atf_addr = TEE_DEC_ADDR + ATF_AREA_HEAD_SIZE;
    size_t area_len = tee_key_area->atf_area_len;
    size_t code_len = area_len - ATF_AREA_NON_CODE_SZIE;
    iv_key_info_s iv_key_info = {0};

    if (area_len < (RSA_4096_LEN + RSA_4096_LEN)) {
        err_print('b', '0');
        return TD_FAILURE;
    }

    if (memcpy_s((void *)TEE_DEC_ADDR, area_len, (void *)ATF_AREA_ADDR, area_len) != EOK) {
        err_print('b', '1');
        return TD_FAILURE;
    }

    ret = confirm_signature_value(((gsl_third_party_key_s *)GSL_THIRD_PARTY_KEY_ADDR)->modulus,
        ((gsl_third_party_key_s *)GSL_THIRD_PARTY_KEY_ADDR)->exponent,
        (uint32_t)TEE_DEC_ADDR, (uint32_t)(area_len - (RSA_4096_LEN + RSA_4096_LEN)),
        (uint8_t *)(TEE_DEC_ADDR + area_len - RSA_4096_LEN));
    if ((ret != TD_SUCCESS) && (is_double_sign_en_enable() == AUTH_SUCCESS)) {
        err_print('b', '2');
        return TD_FAILURE;
    }

    iv_key_info.iv = tee_key_area->atf_iv;
    iv_key_info.prot_key_l1_enc = tee_key_area->atf_protection_key_l1_enc;
    iv_key_info.prot_key_l2_enc = tee_key_area->atf_protection_key_l2_enc;

    decrypt_field(TEE_DEC_ADDR + TEE_CODE_AREA_HEAD_SIZE, code_len, KLAD_ATF, iv_key_info);

    ret = confirm_signature_value(tee_key_area->modulus, tee_key_area->exponent,
        (uint32_t)TEE_DEC_ADDR,
        (uint32_t)(area_len - (RSA_4096_LEN + RSA_4096_LEN)),
        (uint8_t *)(TEE_DEC_ADDR + area_len - (RSA_4096_LEN + RSA_4096_LEN)));
    if (ret != TD_SUCCESS) {
        err_print('b', '3');
        return TD_FAILURE;
    }

    if (*(uint32_t *)TEE_DEC_ADDR != ATF_CODE_ID) {
        err_print('b', '4');
        return TD_FAILURE;
    }

    if (*(uint32_t *)(TEE_DEC_ADDR + ATF_AREA_STRUCT_VER_OFFSET) !=
        ATF_AREA_STRUCTION_VERSION) {
        err_print('b', '5');
        return TD_FAILURE;
    }

    if (memcpy_s((void *)BL31_BASE, code_len, (void *)atf_addr, code_len) != EOK) {
        err_print('b', '6');
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static int tee_copy_code_area(uintptr_t area_addr, size_t area_len)
{
    if (area_len < (RSA_4096_LEN + RSA_4096_LEN)) {
        err_print('c', '0');
        return TD_FAILURE;
    }

    if (memcpy_s((void *)TEE_DEC_ADDR, area_len, (void *)area_addr, area_len) != EOK) {
        err_print('c', '1');
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static int tee_verify_third_party(size_t area_len)
{
    int ret;

    ret = confirm_signature_value(((gsl_third_party_key_s *)GSL_THIRD_PARTY_KEY_ADDR)->modulus,
        ((gsl_third_party_key_s *)GSL_THIRD_PARTY_KEY_ADDR)->exponent,
        (uint32_t)TEE_DEC_ADDR, (uint32_t)(area_len - (RSA_4096_LEN + RSA_4096_LEN)),
        (uint8_t *)(TEE_DEC_ADDR + area_len - RSA_4096_LEN));
    if ((ret != TD_SUCCESS) && (is_double_sign_en_enable() == AUTH_SUCCESS)) {
        err_print('c', '2');
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static void tee_decrypt_code_area(const tee_key_area_s *tee_key_area, size_t area_len)
{
    iv_key_info_s iv_key_info = {0};

    iv_key_info.iv = tee_key_area->tee_iv;
    iv_key_info.prot_key_l1_enc = tee_key_area->tee_protection_key_l1_enc;
    iv_key_info.prot_key_l2_enc = tee_key_area->tee_protection_key_l2_enc;
    decrypt_field(TEE_DEC_ADDR + TEE_CODE_AREA_HEAD_SIZE, area_len - TEE_CODE_AREA_NON_CODE_SZIE,
        KLAD_TEE, iv_key_info);
}

static int tee_verify_oem(const tee_key_area_s *tee_key_area, size_t area_len)
{
    int ret;

    ret = confirm_signature_value(tee_key_area->modulus, tee_key_area->exponent,
        (uint32_t)TEE_DEC_ADDR, (uint32_t)(area_len - (RSA_4096_LEN + RSA_4096_LEN)),
        (uint8_t *)(TEE_DEC_ADDR + area_len - (RSA_4096_LEN + RSA_4096_LEN)));
    if (ret != TD_SUCCESS) {
        err_print('c', '3');
        return TD_FAILURE;
    }

    if (*(uint32_t *)TEE_DEC_ADDR != TEE_CODE_AREA_ID) {
        err_print('c', '4');
        return TD_FAILURE;
    }

    if (*(uint32_t *)(TEE_DEC_ADDR + TEE_CODE_AREA_STRUCT_VER_OFFSET) !=
        TEE_CODE_AREA_STRUCTION_VERSION) {
        err_print('c', '5');
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static int tee_copy_optee(size_t optee_offset, size_t optee_len)
{
    if (memcpy_s((void *)BL32_BASE, optee_len, (void *)TEE_DEC_ADDR + optee_offset, optee_len) != EOK) {
        err_print('c', '6');
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

int handle_tee_code_area()
{
	tee_key_area_s *tee_key_area = (tee_key_area_s *)g_tee_img_sec_addr;
	uintptr_t area_addr = ATF_AREA_ADDR + tee_key_area->atf_area_len;
	size_t area_len = tee_key_area->tee_code_area_len;
	uintptr_t optee_offset = TEE_CODE_AREA_HEAD_SIZE + sizeof(struct optee_header);
	size_t optee_len = area_len - TEE_CODE_AREA_NON_CODE_SZIE - sizeof(struct optee_header);

    if (tee_copy_code_area(area_addr, area_len) != TD_SUCCESS) {
		return TD_FAILURE;
	}

    if (tee_verify_third_party(area_len) != TD_SUCCESS) {
		return TD_FAILURE;
	}

    tee_decrypt_code_area(tee_key_area, area_len);
    if (tee_verify_oem(tee_key_area, area_len) != TD_SUCCESS) {
		return TD_FAILURE;
	}

    if (tee_copy_optee(optee_offset, optee_len) != TD_SUCCESS) {
		return TD_FAILURE;
	}
	return TD_SUCCESS;
}
