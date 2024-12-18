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
 * Description: dfx write interface
 * This file should be changed only infrequently and with great care.
 */

#ifndef DFX_WRITE_INTERFACE_H
#define DFX_WRITE_INTERFACE_H
#include <stdint.h>

/* dfx_data_transmit_interface.h */
typedef enum {
    DFX_DATA_TYPE_STRING,   /* 纯string数据 */
    DFX_DATA_TYPE_UAPI_LOG, /* UAPI LOG */
    DFX_DATA_DIAG_PKT,
    DFX_DATA_DIAG_PKT_CRITICAL,
} dfx_data_type_t; /* 数据类型 */

typedef struct dfx_write_data_interface {
    int32_t (*write)(void *fd, dfx_data_type_t data_type, uint8_t *data[], uint16_t len[], uint8_t cnt); /* 写数据接口 */
    void *fd;    /* 传入给write的fd */
} dfx_write_data_interface_t; /* 写数据抽象 */

#endif