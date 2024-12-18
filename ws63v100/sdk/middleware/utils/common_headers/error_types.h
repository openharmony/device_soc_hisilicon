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
 * Description:  Error handling for Functions
 */

#ifndef ERROR_TYPES_H
#define ERROR_TYPES_H

/**
 * @defgroup connectivity_libs_common_err ERROR Types
 * @ingroup  connectivity_libs_common
 * @{
 */
typedef enum {
    RET_TYPE_OK = 0,
    RET_TYPE_ERROR,
    RET_TYPE_ERROR_RPC,
    RET_TYPE_ERROR_IN_PARAMETERS,
    RET_TYPE_ILLEGAL_OPERATION,
    RET_TYPE_RETRY_LATER,
    RET_TYPE_RETRY_AFTER_DEEP_SLEEP,
    RET_TYPE_KEY_NOT_FOUND,
    RET_TYPE_FLOW_CONTROL,
    RET_TYPE_IN_PROGRESS,
    RET_TYPE_NO_NETWORK_SERVICE,
    RET_TYPE_INVALID_ENTRY,
    RET_TYPE_OUT_OF_RANGE,
    RET_TYPE_MEMORY_ERROR,
    RET_TYPE_DISABLED_OPERATION,
    RET_TYPE_NOT_POWER_ON,
    RET_TYPE_KV_FLASH_FULL,
    RET_TYPE_CID_ACTIVE,
    RET_TYPE_RADIO_TEST_STATE_ERROR,
    RET_TYPE_RADIOTEST_DISABLED,
    RET_TYPE_NONIP_UNCONFIG,
    RET_TYPE_NONIP_NOT_REGISTERED,
    RET_TYPE_SIM_PIN_REQUIRED,
    RET_TYPE_CID_NOT_DEFINED,
    RET_TYPE_CID_NOT_ACTIVE,
    RET_TYPE_CID_INVALID,
    RET_TYPE_DEACTIVE_LAST_ACTIVE_CID,
    RET_TYPE_IN_PROGRESS_BLOCK,
    RET_TYPE_ABORT_BY_USER,
    RET_TYPE_ABORT_BY_USER_FAILED,
    RET_TYPE_DATA_SEQUENCE_REPEAT_ERROR,
    RET_TYPE_NO_COMMAND_TO_ABORT,
    RET_TYPE_COMMAND_UNABORTABLE,
    RET_TYPE_COMMAND_INTERRUPTED,
    RET_TYPE_PROTOCOL_FAMILY_UNMATCHING,
    RET_TYPE_KEY_WRONG_SIZE,
    RET_TYPE_KEY_TOO_BIG,
    RET_TYPE_CONFIGURATION_CONFLICTS,
} ret_type_e;

/**
 * @}
 */
#endif
