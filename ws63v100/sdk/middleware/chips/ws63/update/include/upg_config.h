/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: UPG config file for product.
 */
#ifndef UPG_PRODUCT_CONFIG_H
#define UPG_PRODUCT_CONFIG_H

#ifdef YES
#undef YES
#endif
#define YES           1

#ifdef  NO
#undef  NO
#endif
#define NO            0

#define UPG_CFG_DEBUG_ENABLED                   YES
#define UPG_CFG_DEBUG_PRINT_ENABLED             YES
#define UPG_CFG_DIRECT_FLASH_ACCESS             YES
#define UPG_CFG_PROCESS_NOTIFY_SUPPORT          YES
#ifndef UPG_CFG_VERIFICATION_SUPPORT
#define UPG_CFG_VERIFICATION_SUPPORT            YES
#endif
#define UPG_CFG_INTEGRITY_VERIFICATION_ONLY     NO
#define UPG_CFG_VERIFICATION_MODE_RSA           NO
#define UPG_CFG_VERIFICATION_MODE_ECC           YES
#define UPG_CFG_VERIFICATION_MODE_SM2_SM3       NO
#define UPG_CFG_VERIFICATION_MODE_SM3_ONLY      NO
#define UPG_CFG_VERIFICATION_MODE_SHA256_ONLY   NO
#define UPG_CFG_VERIFICATION_MODE_SW_HASH       NO
#define UPG_CFG_NV_UPGRADE_SUPPORT              NO
#define UPG_CFG_DIFF_UPGRADE_SUPPORT            NO
#define UPG_CFG_IMGAE_DECRYPT_SUPPORT           NO
#define UPG_CFG_PROGRESS_RECOVERY_SUPPORT       YES
#define UPG_CFG_ANTI_ROLLBACK_SUPPORT           NO
#define UPG_CFG_HASH_VERIFY_SUPPORT             NO
#define UPG_CFG_SIGNATURE_VERIFY_SUPPORT        NO
#define UPG_CFG_SUPPORT_FILE_SYSTEM             NO
#define UPG_CFG_MEMORY_DCACHE_ENABLED           NO
#define UPG_CFG_SUPPORT_IMAGE_ON_FILE_SYSTEM    NO
#define UPG_CFG_SUPPORT_RESOURCES_FILE          NO
#define UPG_CFG_SUPPORT_ERASE_WHOLE_IMAGE       YES

#include "upg_default_config.h"
#endif /* UPG_PRODUCT_CONFIG_H */
