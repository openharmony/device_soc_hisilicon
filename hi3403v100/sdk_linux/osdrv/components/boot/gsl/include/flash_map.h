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

#ifndef FLASH_MAP_H
#define FLASH_MAP_H

#define CFG_PARAM_SIZE 0x2b10

#define RSA_TOTAL_LEN   0x400
#define RSA_E_LEN    0x4
#define DEF_KEY_LEN    0x10
#define DEF_KEY1_LEN    0x10
#define DEF_KEY2_LEN    0x20
#define RSA_4096_LEN    0x200
#define SHA_256_LEN     0x20
#define IV_LEN       0x10
#define ROOT_PUBLIC_KEY_ENC_PART_SIZE 0x240

#define GSL_THIRD_PARTY_KEY_CERTIFICATE_ID 0x3C78961E
#define GSL_THIRD_PARTY_KEY_STRUCTURE_VERSION 0x00000000
#define GSL_KEY_AREA_CERTIFICATE_ID 0x3C78962D
#define GSL_KEY_AREA_STRUCTURE_VERSION 0x00000000
#define GSL_CODE_NOT_ENC_FLAG 0x3C7896E1
#define GSL_CODE_AREA_ID 0x3C78691E
#define GSL_CODE_AREA_STRUCTURE_VERSION 0x00000000
#define GSL_CODE_AREA_MAX_LEN  0x10000
#define GSL_CODE_AREA_VERSION_OFFSET  0x4

#define BOOTLOADER_KEY_AREA_CERTIFICATE_ID 0x3C78963C
#define BOOTLOADER_KEY_AREA_STRUCTURE_VERSION 0x00000000
#define BOOTLOADER_PARAMS_AREA_PARAMS_ID 0x3C78692D
#define BOOTLOADER_PARAMS_AREA_STRUCTURE_VERSION 0x00000000
#define BOOTLOADER_BOOT_CODE_ENC_FLAG 0x3C7896E1
#define BOOTLOADER_BOOT_CODE_ID 0x3C78693C
#define BOOTLOADER_BOOT_CODE_ID_SIZE 0x4
#define BOOTLOADER_CODE_AREA_STRUCTURE_VERSION 0x00000000
#define BOOTLOADER_UNCHECKED_AREA_FOR_VENDOR_OFFSET (0x20b0 + CFG_PARAM_SIZE)
#define SCS_SIMULATE_AUTH_FLAG 0x69875AB4
#define IMAGE_NAME_LEN     32

/* TEE_IMG_CODE_AREA refers ATF Area and TEE_CODE Area */
#define ATF_AREA_STRUCT_VER_OFFSET 0x4
#define ATF_AREA_NON_CODE_SZIE 0x420
#define ATF_AREA_HEAD_SIZE (0x4 + 0x4 + 0x18)

#define TEE_CODE_AREA_STRUCT_VER_OFFSET 0x4
#define TEE_CODE_AREA_NON_CODE_SZIE 0x420
#define TEE_CODE_AREA_HEAD_SIZE (0x4 + 0x4 + 0x18)

#define TEE_KEY_AREA_CERTIFICATE_ID 0x3C78964B
#define TEE_KEY_AREA_STRUCTURE_VERSION 0x00000000
#define ATF_CODE_ID 0x3C78694B
#define ATF_AREA_STRUCTION_VERSION 0x00000000
#define TEE_CODE_AREA_ID 0x3C78695A
#define TEE_CODE_AREA_STRUCTION_VERSION 0x00000000

typedef enum {
    VENDOR_ROOT_PUBLIC_KEY = 0x1,
    OEM_ROOT_PUBLIC_KEY = 0x2,
    THIRD_PARTY_ROOT_PUBLIC_KEY = 0x3
} key_type;

/* Vendor_Root_Public_Key\OEM_Root_Public_Key\Third_party_Root_Public_Key */
typedef struct {
    unsigned char modulus[0x200];
    unsigned char exponent[RSA_E_LEN];
    unsigned char checksum_root_public_key[0x20];
    unsigned char reserved_1[0x1c];
    unsigned char public_protection_key_l1_enc[0x10];
    unsigned char public_protection_key_l2_enc[0x20];
    unsigned char public_iv[IV_LEN];
    unsigned char reserved_2[0x180];
} root_public_key_s;

/* GSL third party Key Area */
typedef struct {
    unsigned int    certificate_id;
    unsigned int    structure_version;
    unsigned char   reserved_0[0x4];
    unsigned char    exponent[RSA_E_LEN];
    unsigned char    modulus[RSA_4096_LEN];
    unsigned int    third_party_msid_ext;
    unsigned int    third_party_msid_ext_mask;
    unsigned int    third_party_version_ext;
    unsigned int    third_party_version_ext_mask;
    unsigned char   reserved_1[0x1e0];
    unsigned char    gsl_third_party_sig[RSA_4096_LEN];
} gsl_third_party_key_s;

/* GSL key Area */
typedef struct {
    unsigned int    certificate_id;
    unsigned int    structure_version;
    unsigned int    gsl_code_area_len;
    unsigned char     exponent[RSA_E_LEN];
    unsigned char   modulus[0x200];
    unsigned int    oem_msid_ext;
    unsigned int    oem_msid_ext_mask;
    unsigned int    oem_version_ext;
    unsigned int    oem_version_ext_mask;
    unsigned char     gsl_protection_key_l1_enc[0x10];
    unsigned char     gsl_protection_key_l2_enc[0x20];
    unsigned char   gsl_iv[IV_LEN];
    unsigned int    gsl_code_enc_flag;
    unsigned char   reserved[0x19c];
    unsigned char   gsl_key_sig[RSA_4096_LEN];
} gsl_key_area_s;

/* Bootloader Key Area */
typedef struct {
    unsigned int     certificate_id;
    unsigned int    structure_version;
    unsigned char   reserved_1[0x4];
    unsigned char    exponent[RSA_E_LEN];
    unsigned char    modulus[RSA_4096_LEN];
    unsigned int    oem_msid_ext;
    unsigned int    oem_msid_ext_mask;
    unsigned int    oem_version_ext;
    unsigned int    oem_version_ext_mask;
    unsigned char   reserved_2[0x1e0];
    unsigned char    bootloader_key_sig[RSA_4096_LEN];
} bootloader_key_area_s;

/* Bootloader Params Area */
typedef struct {
    unsigned int     params_id;
    unsigned int    structure_version;
    uint64_t      boot_entry_point;
    unsigned int    unchecked_area_len;
    unsigned int    checked_area_len;
    unsigned int    boot_area_len;
    unsigned int    boot_enc_flag;
    unsigned char     boot_protection_key_l1_enc[0x10];
    unsigned char     boot_protection_key_l2_enc[0x20];
    unsigned char   boot_iv[IV_LEN];
    unsigned char   reserved[0x40];
    unsigned char   cfg_param[CFG_PARAM_SIZE];
    unsigned int    oem_msid_ext;
    unsigned int    oem_msid_ext_mask;
    unsigned int    oem_version_ext;
    unsigned int    oem_version_ext_mask;
    unsigned char    bootloader_params_sig[RSA_4096_LEN];
} bootloader_params_area_s;

/* Bootloader Boot Area msid version */
typedef struct {
    unsigned int    oem_msid_ext;
    unsigned int    oem_msid_ext_mask;
    unsigned int    oem_version_ext;
    unsigned int    oem_version_ext_mask;
} bootloader_boot_msid_version_s;

/* Bootloader Unchecked Area for Vendor */
typedef struct {
    unsigned int    scs_simulate_flag;
    unsigned char   reserved[0x3c];
} unchecked_area_for_s;

/* Boot Image BackUp */
typedef struct {
    int enable;
    unsigned int offset_times;
    unsigned int offset_addr;
} backup_image_params_s;

typedef struct aapcs64_params {
    uint64_t arg0;
    uint64_t arg1;
    uint64_t arg2;
    uint64_t arg3;
    uint64_t arg4;
    uint64_t arg5;
    uint64_t arg6;
    uint64_t arg7;
} aapcs64_params_t;

typedef struct param_header {
    uint8_t type;       /* type of the structure */
    uint8_t version;    /* version of this structure */
    uint16_t size;      /* size of this structure in bytes */
    uint32_t attr;      /* attributes: unused bits SBZ */
} param_header_t;

typedef struct entry_point_info {
    param_header_t h;
    uint64_t pc;
    uint32_t spsr;
    aapcs64_params_t args;
} entry_point_info_t;

typedef struct bl31_params {
    param_header_t h;
    uint64_t bl31_image_info;    /* atf_image_info_t *bl31_image_info */
    uint64_t bl32_ep_info;        /* entry_point_info_t *bl32_ep_info  */
    uint64_t bl32_image_info;    /* atf_image_info_t *bl32_image_info */
    uint64_t bl33_ep_info;        /* entry_point_info_t *bl33_ep_info  */
    uint64_t bl33_image_info;    /* atf_image_info_t *bl33_image_info */
} bl31_params_t;

struct optee_header {
    uint32_t magic;
    uint8_t version;
    uint8_t arch;
    uint16_t flags;
    uint32_t init_size;
    uint32_t init_load_addr_high;
    uint32_t init_load_addr_low;
    uint32_t init_mem_usage;
    uint32_t paged_size;
};

/* TEE Key Area */
typedef struct {
    uint32_t        certificate_id;
    uint32_t        structure_version;
    uint32_t        tee_code_area_len;
    uint8_t         exponent[RSA_E_LEN];
    uint8_t         modulus[0x200];
    uint32_t        tee_msid_ext;
    uint32_t        tee_msid_ext_mask;
    uint32_t        tee_sec_version_ext;
    uint32_t        tee_sec_version_ext_mask;
    uint8_t         atf_protection_key_l1_enc[0x10];
    uint8_t         atf_protection_key_l2_enc[0x20];
    uint8_t         tee_protection_key_l1_enc[0x10];
    uint8_t         tee_protection_key_l2_enc[0x20];
    uint8_t         atf_iv[IV_LEN];
    uint8_t         tee_iv[IV_LEN];
    uint32_t        tee_func_version;
    uint32_t        atf_area_len;
    uint64_t        tee_ddr_address;
    uint32_t        tee_reserved_ddr_size;
    uint8_t         reserved[0x14c];
    uint8_t         tee_key_sig[RSA_4096_LEN];
} tee_key_area_s;

typedef struct {
    uint32_t        boot_area_addr;
    uint32_t        boot_code_addr;
    uint32_t        boot_code_id_addr;
    uint32_t        boot_structure_version_addr;
    uint32_t        checked_area_len;
    uint32_t        boot_third_party_sig_addr;
    gsl_third_party_key_s           *gsl_third_party_key;
    bootloader_key_area_s           *bootloader_key_area;
    bootloader_params_area_s        *bootloader_params_area;
    bootloader_boot_msid_version_s  *boot_msid_version;
} bootloader_code_area_info_s;

#endif /* FLASH_MAP_H */
