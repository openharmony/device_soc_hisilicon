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
 * Description: log OAM ota producer interface
 * Author:
 * Create:
 */
#ifndef LOG_OAM_OTA_H
#define LOG_OAM_OTA_H

#include "log_oam_logger.h"

#pragma pack(push, 1)

#define OTA_DATA_MAX_SIZE 1200

#ifdef DEBUG_MODE
#define bt_ota_write(msgId, length, msg_buffer) log_oml_ota_write(OM_BT, msgId, length, msg_buffer)
#else
#define bt_ota_write(msgId, length, msg_buffer)
#endif

typedef enum {
    OM_OTA_OPEN,
    OM_OTA_CLOSED,
} om_ota_config_t;

typedef struct {
    om_msg_header_stru_t header;
    uint16_t msg_id;
    uint16_t data_len;
} om_ota_header_t;

typedef struct {
    om_ota_header_t st_ota_header;
    uint8_t data[OTA_DATA_MAX_SIZE];
    uint8_t msg_tail;
} om_ota_data_t;


typedef void(*log_oml_ota_write_handler)(uint8_t mode_id, uint16_t msg_id, uint16_t length, const uint8_t *msg_buffer);
void log_oml_ota_init(void);
void log_oml_ota_write(uint8_t mode_id, uint16_t msg_id, uint16_t length, const uint8_t *msg_buffer);
void log_oml_ota_switch(uint8_t on);
void log_oml_ota_set(om_ota_config_t value);
uint32_t log_oml_ota_get(void);
void log_oml_ota_write_deal(uint8_t mode_id, uint16_t msg_id, uint16_t length, const uint8_t *msg_buffer);
void log_oml_ota_write_register_callback(log_oml_ota_write_handler callback);
void log_oml_ota_write_unregister_callback(void);
void log_oml_ota_write_trigger_callback(uint8_t mode_id, uint16_t msg_id, uint16_t length, const uint8_t *msg_buffer);

#pragma pack(pop)

#endif
