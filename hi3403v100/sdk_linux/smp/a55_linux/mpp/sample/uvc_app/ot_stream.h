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

#ifndef __STREAM_H__
#define __STREAM_H__

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "ot_camera.h"
#include "uvc.h"

// USB_Video_Payload_H_264_1.0 3.3
#define UVCX_PICTURE_TYPE_CONTROL 0x09

// H264 extension unit id, must same as to driver
#define UNIT_XU_H264 10

// OT_CAMERA
#define UNIT_XU_OT_CAMERA (0x11)

typedef struct stream_control_ops {
    int (*init)(void);
    int (*deinit)(void);
    int (*startup)(void);
    int (*shutdown)(void);
    int (*set_idr)(void);
    int (*set_property)(struct encoder_property *p);
} stream_control_ops_st;

typedef struct processing_unit_ops {
    uint16_t (*brightness_get)();
    uint16_t (*contrast_get)();
    uint16_t (*hue_get)();
    uint8_t (*power_line_frequency_get)();
    uint16_t (*saturation_get)();
    uint8_t (*white_balance_temperature_auto_get)();
    uint16_t (*white_balance_temperature_get)();

    void (*brightness_set)(uint16_t v);
    void (*contrast_set)(uint16_t v);
    void (*hue_set)(uint16_t v);
    void (*power_line_frequency_set)(uint8_t v);
    void (*saturation_set)(uint16_t v);
    void (*white_balance_temperature_auto_set)(uint8_t v);
    void (*white_balance_temperature_set)(uint16_t v);
} processing_unit_ops_st;

typedef struct input_terminal_ops {
    uint32_t (*exposure_ansolute_time_get)();
    uint8_t (*exposure_auto_mode_get)();
    void (*exposure_ansolute_time_set)(uint32_t v);
    void (*exposure_auto_mode_set)(uint8_t v);
} input_terminal_ops_st;

typedef struct extension_unit_ops {
    /* eu-h264 ops */
    /* eu-camera ops */
} extension_unit_ops_st;

typedef struct ot_stream {
    struct stream_control_ops *mpi_sc_ops;
    struct processing_unit_ops *mpi_pu_ops;
    struct input_terminal_ops *mpi_it_ops;
    struct extension_unit_ops *mpi_eu_ops;
    int streaming;
    int exposure_auto_stall;
    int brightness_stall;
} ot_stream;

/* media control functions */
extern int ot_stream_register_mpi_ops(struct stream_control_ops *sc_ops, struct processing_unit_ops *pu_ops,
    struct input_terminal_ops *it_ops, struct extension_unit_ops *eu_ops);

extern int ot_stream_set_enc_property(struct encoder_property *p);
extern int ot_stream_init(void);
extern int ot_stream_deinit(void);
extern int ot_stream_shutdown(void);
extern int ot_stream_startup(void);

extern void ot_stream_event_pu_control(struct ot_uvc_dev *dev, uint8_t req, uint8_t unit_id, uint8_t cs,
    struct uvc_request_data *resp);

extern void ot_stream_event_it_control(struct ot_uvc_dev *dev, uint8_t req, uint8_t unit_id, uint8_t cs,
    struct uvc_request_data *resp);

extern void ot_stream_event_eu_h264_control(struct ot_uvc_dev *dev, uint8_t req, uint8_t unit_id, uint8_t cs,
    struct uvc_request_data *resp);

extern void ot_stream_event_eu_camera_control(struct ot_uvc_dev *dev, uint8_t req, uint8_t unit_id, uint8_t cs,
    struct uvc_request_data *resp);

extern int ot_stream_set_enc_idr(void);

extern void ot_stream_event_eu_h264_data(struct ot_uvc_dev *dev, int unit_id, int control,
    struct uvc_request_data *data);

extern void ot_stream_event_pu_data(struct ot_uvc_dev *dev, int unit_id, int control, struct uvc_request_data *data);

extern void ot_stream_event_it_data(struct ot_uvc_dev *dev, int unit_id, int control, struct uvc_request_data *data);

extern void ot_stream_event_eu_camera_data(struct ot_uvc_dev *dev, int unit_id, int control,
    struct uvc_request_data *data);

#endif // __STREAM_H__
