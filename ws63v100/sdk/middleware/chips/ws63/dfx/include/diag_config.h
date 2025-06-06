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
 * Description: diag usr config
 * This file should be changed only infrequently and with great care.
 */

#ifndef DIAG_CONFIG_H
#define DIAG_CONFIG_H

#include <stdint.h>
#include "dfx_resource_id.h"

#define DFX_MSG_MAX_SIZE            0x20 /* msg element size of msg queue */
#define DFX_MSG_ID_LEN              sizeof(uint32_t)
#define DFX_QUEUE_MAX_SIZE          64

#define CONFIG_DIAG_CMD_TBL_NUM         10      /* diag:Maximum number of cmd tbl cnt */
#define CONFIG_DIAG_IND_TBL_NUM         3       /* diag:Maximum number of ind tbl cnt */
#define CONFIG_STAT_CMD_LIST_NUM        10      /* diag:Maximum number of stat tbl cnt */
#define CONFIG_DIAG_RX_BUF_SIZE         0x200   /* diag:rx buf size */
#define CONFIG_DIAG_TRANSMIT_ITEM_CNT   3       /* diag:Maximum transfer item cnt */
#define CONFIG_DIAG_FILTER_GROUP_NUM    10      /* diag: Maximum number of diag file group */

#endif /* DIAG_CONFIG_H */
