/**
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
 *
 * Description: SLE Glp Manager module.
 */

/**
 * @defgroup sle_glp_manager glp manager API
 * @ingroup  SLE
 * @{
 */

#ifndef SLE_GLP_MANAGER_H
#define SLE_GLP_MANAGER_H

#include <stdint.h>
#include "errcode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t status;
    uint16_t con_hdl;
    int32_t  cfo;
} sle_cs_glp_report_t;

typedef void (*sle_cs_glp_report_callback)(uint16_t conn_id,
    sle_cs_glp_report_t *report);

typedef struct {
    sle_cs_glp_report_callback cs_glp_report_cb;
} sle_cs_glp_callbacks_t;

errcode_t sle_glp_register_callbacks(sle_cs_glp_callbacks_t *func);

#ifdef __cplusplus
}
#endif
#endif /* SLE_GLP_MANAGER_H */
/**
 * @}
 */
