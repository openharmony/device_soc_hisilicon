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
 * Description: BT CPU UTILS Module Header
 * Author:
 * Create:
 */
#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include "non_os_reboot.h"
#ifdef SUPPORT_PARTITION_INFO
#include "partition.h"
#endif

/**
 * @defgroup connectivity_libs_cpu_utils CPU Utils
 * @ingroup  connectivity_libs
 * @{
 */
typedef void (*cpu_utils_reboot_cb)(int32_t);

/**
 * @brief  Resets the entire chip, and logging information
 * @param  core The core to request reboot
 * @param  cause The reboot reason
 */
void cpu_utils_reset_chip_with_log(cores_t core, reboot_cause_t cause);

/**
 * @brief  Reset the entire chip with reboot cause
 */
void cpu_utils_reset_chip_with_cause(reboot_cause_t cause);

/**
 * @brief  Registers the IPC action for the core reboot
 */
void cpu_utils_init(void);

/**
 * @brief  Return the core associated with a given core image
 * @param  cimage core image
 * @return physical core associated with core_images_e
 */
cores_t cpu_utils_core_images_to_cores(core_images_e cimage);

/**
 * @brief  set reboot cause & set reboot magic
 * @param  cause reboot cause
 */
void cpu_utils_set_system_status_by_cause(reboot_cause_t cause);

/**
 * @brief  Registers the call back for the Bt core reboot
 * @param  cb callback function
 */
void cpu_utils_set_mcu_callback(cpu_utils_reboot_cb cb);

#ifdef SUPPORT_PARTITION_INFO
/**
 * @brief  Return the partition id associated with a given core image
 * @param  cimage core image
 * @return partition id associated with core_images_e
 */
partition_ids_t cpu_utils_core_iamge_to_partition_id(core_images_e cimage);
#endif

/**
 * @}
 */
#endif
