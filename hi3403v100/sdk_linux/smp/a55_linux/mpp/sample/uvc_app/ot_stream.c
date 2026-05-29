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

#include <pthread.h>
#include "log.h"
#include "ot_stream.h"
#include "frame_cache.h"
#include "uvc.h"
#include "ot_ctrl.h"
#include "ot_camera.h"

#define OT_UVC_RESP_LENGTH_BYTE 1
#define OT_UVC_RESP_LENGTH_HALF_WORD 2
#define OT_UVC_RESP_LENGTH_WORD 4
#define SHIFT_8BIT 8
#define SHIFT_16BIT 16
#define SHIFT_24BIT 24

static struct ot_stream g_ot_stream = {
    .mpi_sc_ops = NULL,
    .mpi_pu_ops = NULL,
    .mpi_it_ops = NULL,
    .mpi_eu_ops = NULL,
    .streaming = 0,
    .exposure_auto_stall = 0,
    .brightness_stall = 0,
};

static ot_stream *get_ot_stream()
{
    return &g_ot_stream;
}

/* Histream Stream Control Operation Functions End */

static void _ot_stream_event_default_control(uint8_t req, struct uvc_request_data *resp)
{
    switch (req) {
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x04;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x01;
            break;
        default:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x06;
            break;
    }
}


/* ******************************************************
 * Histream Processing Unit Operation Functions
 * ***************************************************** */
void brightness_set_cur(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

void brightness_get_cur(struct ot_uvc_dev *dev, struct uvc_request_data *resp, uint8_t v)
{
    if (get_ot_stream()->brightness_stall) {
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x04;
        dev->request_error_code.length = 1;
        get_ot_stream()->brightness_stall = 0;
    } else {
        resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
        resp->data[0] = (v & 0xff);
        resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
    }
}

void brightness_get_min(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
    resp->data[0] = 0x00;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

void brightness_get_max(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
    resp->data[0] = 0x64;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

void brightness_get_def(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
    resp->data[0] = 0x32;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

void brightness_get_res(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->data[0] = 0x01;
    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

void brightness_get_info(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->data[0] = 0x03;
    resp->length = OT_UVC_RESP_LENGTH_BYTE;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

static void _ot_stream_pu_brightness_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 50;

    if (get_ot_stream()->streaming == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->brightness_get) {
            v = get_ot_stream()->mpi_pu_ops->brightness_get();
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            brightness_set_cur(dev, resp);
            break;
        case OT_UVC_GET_CUR:
            brightness_get_cur(dev, resp, v);
            break;
        case OT_UVC_GET_MIN:
            brightness_get_min(dev, resp);
            break;
        case OT_UVC_GET_MAX:
            brightness_get_max(dev, resp);
            break;
        case OT_UVC_GET_DEF:
            brightness_get_def(dev, resp);
            break;
        case OT_UVC_GET_RES:
            brightness_get_res(dev, resp);
            break;
        case OT_UVC_GET_INFO:
            brightness_get_info(dev, resp);
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void _ot_stream_pu_brightness_set(struct uvc_request_data *data)
{
    uint16_t v;

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);
    if (v > 0x64 || v < 0) {
        get_ot_stream()->brightness_stall = 1;
    }

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->brightness_set) {
        get_ot_stream()->mpi_pu_ops->brightness_set(v);
    }
}

void _ot_stream_pu_contrast_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 50; /* 50 is in range of 0~100  */

    if (get_ot_stream()->streaming == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->contrast_get) {
            v = get_ot_stream()->mpi_pu_ops->contrast_get();
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x64;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x02;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void _ot_stream_pu_contrast_set(struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->contrast_set) {
        get_ot_stream()->mpi_pu_ops->contrast_set(v);
    }
}

void _ot_stream_pu_hue_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 50; /* 50 is in range of 0~100  */

    if (get_ot_stream()->streaming == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->hue_get) {
            v = get_ot_stream()->mpi_pu_ops->hue_get();
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x64;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x02;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void _ot_stream_pu_hue_set(struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->hue_set) {
        get_ot_stream()->mpi_pu_ops->hue_set(v);
    }
}

void _ot_stream_pu_power_line_frequency_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0;

    if (get_ot_stream()->streaming == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->power_line_frequency_get) {
            v = get_ot_stream()->mpi_pu_ops->power_line_frequency_get();
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = v;
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x1;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x2;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x1;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void _ot_stream_pu_power_line_frequency_set(struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->power_line_frequency_set) {
        get_ot_stream()->mpi_pu_ops->power_line_frequency_set(v);
    }
}

void _ot_stream_pu_saturation_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 50; /* 50 is in range of 0~100  */

    if (get_ot_stream()->streaming == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->saturation_get) {
            v = get_ot_stream()->mpi_pu_ops->saturation_get();
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x64;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x02;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void _ot_stream_pu_saturation_set(struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->saturation_set) {
        get_ot_stream()->mpi_pu_ops->saturation_set(v);
    }
}

void _ot_stream_pu_white_balance_temperature_auto_ctrl(struct ot_uvc_dev *dev, uint8_t req,
    struct uvc_request_data *resp)
{
    uint8_t v = 0;

    if (get_ot_stream()->streaming == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_temperature_auto_get) {
            v = get_ot_stream()->mpi_pu_ops->white_balance_temperature_auto_get();
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = v;
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x1;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void _ot_stream_pu_white_balance_temperature_auto_set(struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_temperature_auto_set) {
        get_ot_stream()->mpi_pu_ops->white_balance_temperature_auto_set(v);
    }
}

void _ot_stream_pu_white_balance_temperature_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 15000; /* white balance temperature 15000K */

    if (get_ot_stream()->streaming == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_temperature_get) {
            v = get_ot_stream()->mpi_pu_ops->white_balance_temperature_get();
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 1500 & 0xff; /* 1500:min wbt,low 256 bit */
            resp->data[1] = 1500 / 256;  /* 1500:min wbt,high 256 bit */
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 15000 & 0xff; /* 15000:max wbt,low 256 bit */
            resp->data[1] = 15000 / 256;  /* 15000:max wbt,high 256 bit */
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x02;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void _ot_stream_pu_white_balance_temperature_set(struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_temperature_set) {
        get_ot_stream()->mpi_pu_ops->white_balance_temperature_set(v);
    }
}

void ot_stream_event_pu_control(struct ot_uvc_dev *dev, uint8_t req, uint8_t unit_id, uint8_t cs,
    struct uvc_request_data *resp)
{
    if (unit_id != 2) { /* 2 == id of pu */
        return;
    }

    switch (cs) {
        case OT_UVC_PU_BRIGHTNESS_CONTROL:
            _ot_stream_pu_brightness_ctrl(dev, req, resp);
            break;
        case OT_UVC_PU_HUE_CONTROL:
            _ot_stream_pu_hue_ctrl(dev, req, resp);
            break;
        case OT_UVC_PU_CONTRAST_CONTROL:
            _ot_stream_pu_contrast_ctrl(dev, req, resp);
            break;
        case OT_UVC_PU_SATURATION_CONTROL:
            _ot_stream_pu_saturation_ctrl(dev, req, resp);
            break;
        case OT_UVC_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL:
            break;
        case OT_UVC_PU_WHITE_BALANCE_COMPONENT_CONTROL:
            break;
        case OT_UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL:
            _ot_stream_pu_white_balance_temperature_ctrl(dev, req, resp);
            break;
        case OT_UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL:
            _ot_stream_pu_white_balance_temperature_auto_ctrl(dev, req, resp);
            break;
        case OT_UVC_PU_POWER_LINE_FREQUENCY_CONTROL:
            _ot_stream_pu_power_line_frequency_ctrl(dev, req, resp);
            break;
        default:
            _ot_stream_event_default_control(req, resp);
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
    }
}

void ot_stream_event_pu_data(struct ot_uvc_dev *dev, int unit_id, int control, struct uvc_request_data *data)
{
    if (unit_id != 2) { /* 2 == id of pu */
        return;
    }

    switch (control) {
        case OT_UVC_PU_BRIGHTNESS_CONTROL:
            return _ot_stream_pu_brightness_set(data);
        case OT_UVC_PU_CONTRAST_CONTROL:
            return _ot_stream_pu_contrast_set(data);
        case OT_UVC_PU_HUE_CONTROL:
            return _ot_stream_pu_hue_set(data);
        case OT_UVC_PU_SATURATION_CONTROL:
            return _ot_stream_pu_saturation_set(data);
        case OT_UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL:
            return _ot_stream_pu_white_balance_temperature_auto_set(data);
        case OT_UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL:
            return _ot_stream_pu_white_balance_temperature_set(data);
        case OT_UVC_PU_POWER_LINE_FREQUENCY_CONTROL:
            return _ot_stream_pu_power_line_frequency_set(data);
        default:
            break;
    }
}
/* Histream Processing Unit Operation Functions End */
void exposure_auto_set_cur(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->data[0] = 0x0;
    resp->length = OT_UVC_RESP_LENGTH_BYTE;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

void exposure_auto_get_cur(struct ot_uvc_dev *dev, struct uvc_request_data *resp, uint8_t v)
{
    if (get_ot_stream()->exposure_auto_stall) {
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x04;
        dev->request_error_code.length = 1;
        get_ot_stream()->exposure_auto_stall = 0;
    } else {
        resp->data[0] = v;
        resp->length = OT_UVC_RESP_LENGTH_BYTE;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
    }
}

void exposure_auto_get_res(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->data[0] = 0x05;
    resp->length = OT_UVC_RESP_LENGTH_BYTE;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

void exposure_auto_get_def(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->data[0] = 0x04;
    resp->length = OT_UVC_RESP_LENGTH_BYTE;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

void exposure_auto_get_info(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->data[0] = 0x3;
    resp->length = OT_UVC_RESP_LENGTH_BYTE;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}
/* ******************************************************
 * Histream Input Terminal Operation Functions
 * ***************************************************** */
static void _ot_stream_it_exposure_auto_mode_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0x04;

    if (get_ot_stream()->streaming == 1) {
        if (get_ot_stream()->mpi_it_ops && get_ot_stream()->mpi_it_ops->exposure_auto_mode_get) {
            v = get_ot_stream()->mpi_it_ops->exposure_auto_mode_get();
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            exposure_auto_set_cur(dev, resp);
            break;
        case OT_UVC_GET_CUR:
            exposure_auto_get_cur(dev, resp, v);
            break;
        case OT_UVC_GET_RES:
            exposure_auto_get_res(dev, resp);
            break;
        case OT_UVC_GET_DEF:
            exposure_auto_get_def(dev, resp);
            break;
        case OT_UVC_GET_INFO:
            exposure_auto_get_info(dev, resp);
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void _ot_stream_it_exposure_auto_mode_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint8_t v = 0x04;

    if (get_ot_stream()->streaming != 1) {
        return;
    }

    v = data->data[0];
    switch (v) {
        case 0x01:
        case 0x02:
        case 0x04:
        case 0x08:
            get_ot_stream()->exposure_auto_stall = 0;
            break;
        default:
            get_ot_stream()->exposure_auto_stall = 1;
            return;
    }
    if (get_ot_stream()->mpi_it_ops && get_ot_stream()->mpi_it_ops->exposure_auto_mode_set) {
        get_ot_stream()->mpi_it_ops->exposure_auto_mode_set(v);
    }
}

void exposure_time_set_cur(struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_WORD;
    resp->data[0] = 0x0;
}

void exposure_time_get_cur(struct uvc_request_data *resp, uint32_t v, struct ot_uvc_dev *dev)
{
    resp->length = OT_UVC_RESP_LENGTH_WORD;
    resp->data[0] = (v & 0xff);
    resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
    resp->data[2] = ((v >> SHIFT_16BIT) & 0xff); /* [2]:data 16~23bit */
    resp->data[3] = ((v >> SHIFT_24BIT) & 0xff); /* [3]:data 24~31bit */
    resp->length = -EL2HLT;
    dev->request_error_code.length = 1;
    dev->request_error_code.data[0] = 0x06;
}

void exposure_time_get_max(struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_WORD;
    resp->data[0] = (2000) & 0xff;               /* [0]:data 0~7bit      2000:exposure time val */
    resp->data[1] = (2000 >> SHIFT_8BIT) & 0xff; /* [1]:data 8~15bit     2000:exposure time val */
    resp->data[2] = 0x0;                         /* [2]:data 16~23bit */
    resp->data[3] = 0x0;                         /* [3]:data 24~31bit */
}

void exposure_time_get_min(struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_WORD;
    resp->data[0] = 0x0a; /* [0]:data 0~7bit */
    resp->data[1] = 0x0;  /* [1]:data 8~15bit */
    resp->data[2] = 0x0;  /* [2]:data 16~23bit */
    resp->data[3] = 0x0;  /* [3]:data 24~31bit */
}

void exposure_time_get_res(struct uvc_request_data *resp)
{
    resp->data[0] = 0x0a;
    resp->length = OT_UVC_RESP_LENGTH_WORD;
}

void exposure_time_get_def(struct uvc_request_data *resp)
{
    resp->data[0] = 0x64;
    resp->length = OT_UVC_RESP_LENGTH_WORD;
}

void exposure_time_get_info(struct uvc_request_data *resp)
{
    resp->data[0] = 0xf;
    resp->length = OT_UVC_RESP_LENGTH_BYTE;
}

static void _ot_stream_it_exposure_ansolute_time_ctrl(struct ot_uvc_dev *dev, uint8_t req,
    struct uvc_request_data *resp)
{
    uint32_t v = 2000; /* 2000 us */

    if (get_ot_stream()->streaming == 1) {
        if (get_ot_stream()->mpi_it_ops && get_ot_stream()->mpi_it_ops->exposure_ansolute_time_get) {
            v = get_ot_stream()->mpi_it_ops->exposure_ansolute_time_get();
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            exposure_time_set_cur(resp);
            break;
        case OT_UVC_GET_CUR:
            exposure_time_get_cur(resp, v, dev);
            break;
        case OT_UVC_GET_MAX:
            exposure_time_get_max(resp);
            break;
        case OT_UVC_GET_MIN:
            exposure_time_get_min(resp);
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x04;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_RES:
            exposure_time_get_res(resp);
            break;
        case OT_UVC_GET_DEF:
            exposure_time_get_def(resp);
            break;
        case OT_UVC_GET_INFO:
            exposure_time_get_info(resp);
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.length = 1;
            dev->request_error_code.data[0] = 0x07;
            break;
    }
}

static void _ot_stream_it_exposure_absolute_time_set(struct uvc_request_data *data)
{
    uint32_t v;

    if (get_ot_stream()->streaming != 1) {
        return;
    }

    v = 100 * (data->data[0] + (data->data[1] << SHIFT_8BIT) + /* 100 ms */
        (data->data[2] << SHIFT_16BIT) +                       /* [2]: data 16~23bit */
        (data->data[3] << SHIFT_24BIT));                       /* [3]: data 24~31bit */

    if (get_ot_stream()->mpi_it_ops && get_ot_stream()->mpi_it_ops->exposure_ansolute_time_set) {
        get_ot_stream()->mpi_it_ops->exposure_ansolute_time_set(v);
    }
}

void ot_stream_event_it_control(struct ot_uvc_dev *dev, uint8_t req, uint8_t unit_id, uint8_t cs,
    struct uvc_request_data *resp)
{
    if (unit_id != 1) {
        return;
    }

    if (get_ot_stream()->exposure_auto_stall) {
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x04;
        dev->request_error_code.length = 1;
        get_ot_stream()->exposure_auto_stall = 0;
        return;
    }

    switch (cs) {
        case OT_UVC_CT_AE_MODE_CONTROL:
            _ot_stream_it_exposure_auto_mode_ctrl(dev, req, resp);
            break;
        case OT_UVC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL:
            _ot_stream_it_exposure_ansolute_time_ctrl(dev, req, resp);
            break;
        case OT_UVC_CT_EXPOSURE_TIME_RELATIVE_CONTROL:
        case OT_UVC_CT_AE_PRIORITY_CONTROL:
        default:
            _ot_stream_event_default_control(req, resp);
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
    }
}

void ot_stream_event_it_data(struct ot_uvc_dev *dev, int unit_id, int control, struct uvc_request_data *data)
{
    if (unit_id != 1) {
        return;
    }

    switch (control) {
        case OT_UVC_CT_AE_MODE_CONTROL:
            _ot_stream_it_exposure_auto_mode_set(dev, data);
            break;
        case OT_UVC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL:
            _ot_stream_it_exposure_absolute_time_set(data);
            break;
        default:
            break;
    }
}

/* Histream Input Terminal Operation Functions End */


/* ******************************************************
 * 	Histream Extension Unit H264 Operation Functions
 * ***************************************************** */
static void _ot_stream_eu_h264_picture_type_control(uint8_t req, struct uvc_request_data *resp)
{
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x04;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            break;
    }
}

/* Histream Extension Unit H264 Operation Functions End */
void ot_stream_event_eu_h264_control(struct ot_uvc_dev *dev, uint8_t req, uint8_t unit_id, uint8_t cs,
    struct uvc_request_data *resp)
{
    if (unit_id != 10) { /* 10 == id of xu */
        return;
    }

    switch (cs) {
        case UVCX_PICTURE_TYPE_CONTROL:
            _ot_stream_eu_h264_picture_type_control(req, resp);
            break;
        default:
            _ot_stream_event_default_control(req, resp);
            break;
    }
}

void ot_stream_event_eu_h264_data(struct ot_uvc_dev *dev, int unit_id, int control, struct uvc_request_data *data)
{
    if (unit_id != 10) { /* 10 == id of xu */
        return;
    }

    switch (control) {
        case UVCX_PICTURE_TYPE_CONTROL:
            break;
        default:
            break;
    }
}

/* ******************************************************
 * Histream Extension Unit Camera Operation Functions
 * ***************************************************** */
static void _ot_stream_eu_camera_iframe_control(uint8_t req, struct uvc_request_data *resp)
{
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x04;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            break;
    }
}

static void _ot_stream_eu_camera_stream_control(uint8_t req, struct uvc_request_data *resp)
{
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x04;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            break;
    }
}

static void _ot_stream_eu_camera_reset_control(uint8_t req, struct uvc_request_data *resp)
{
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x04;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            break;
    }
}

static void _ot_stream_eu_camera_update_system(uint8_t req, struct uvc_request_data *resp)
{
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x04;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            break;
    }
}

void ot_stream_event_eu_camera_control(struct ot_uvc_dev *dev, uint8_t req, uint8_t unit_id, uint8_t cs,
    struct uvc_request_data *resp)
{
    if (unit_id != UNIT_XU_OT_CAMERA) {
        return;
    }

    switch (cs) {
        case OT_XUID_SET_RESET:
            _ot_stream_eu_camera_reset_control(req, resp);
            break;

        case OT_XUID_SET_STREAM:
            _ot_stream_eu_camera_stream_control(req, resp);
            break;

        case OT_XUID_SET_IFRAME:
            _ot_stream_eu_camera_iframe_control(req, resp);
            break;

        case OT_XUID_UPDATE_SYSTEM:
            _ot_stream_eu_camera_update_system(req, resp);
            break;

        default:
            _ot_stream_event_default_control(req, resp);
            break;
    }
}

void ot_stream_event_eu_camera_data(struct ot_uvc_dev *dev, int unit_id, int control, struct uvc_request_data *data)
{
    if (unit_id != UNIT_XU_OT_CAMERA) {
        return;
    }

    switch (control) {
        case OT_XUID_SET_RESET:
            break;
        default:
            break;
    }
}
/* Histream Extension Unit Camera Operation Functions End */


/* ******************************************************
 * Histream Stream Control Operation Functions
 * ***************************************************** */
int ot_stream_init(void)
{
    if (get_ot_stream()->mpi_sc_ops && get_ot_stream()->mpi_sc_ops->init) {
        return get_ot_stream()->mpi_sc_ops->init();
    }

    return 0;
}

int ot_stream_deinit(void)
{
    if (get_ot_stream()->mpi_sc_ops && get_ot_stream()->mpi_sc_ops->deinit) {
        return get_ot_stream()->mpi_sc_ops->deinit();
    }

    return 0;
}

int ot_stream_startup(void)
{
    int ret = 0;

    if (get_ot_stream()->mpi_sc_ops && get_ot_stream()->mpi_sc_ops->startup) {
        ret = get_ot_stream()->mpi_sc_ops->startup();
    }

    if (ret == 0) {
        get_ot_stream()->streaming = 1;
    }
    return ret;
}

int ot_stream_shutdown(void)
{
    int ret = 0;

    if (get_ot_stream()->mpi_sc_ops && get_ot_stream()->mpi_sc_ops->shutdown) {
        ret = get_ot_stream()->mpi_sc_ops->shutdown();
    }

    if (ret == 0) {
        get_ot_stream()->streaming = 0;
    }
    return ret;
}

int ot_stream_set_enc_property(struct encoder_property *p)
{
    if (get_ot_stream()->mpi_sc_ops && get_ot_stream()->mpi_sc_ops->set_property) {
        return get_ot_stream()->mpi_sc_ops->set_property(p);
    }

    return 0;
}

int ot_stream_set_enc_idr(void)
{
    if (get_ot_stream()->mpi_sc_ops && get_ot_stream()->mpi_sc_ops->set_idr) {
        return get_ot_stream()->mpi_sc_ops->set_idr();
    }

    return 0;
}

int ot_stream_register_mpi_ops(struct stream_control_ops *sc_ops, struct processing_unit_ops *pu_ops,
    struct input_terminal_ops *it_ops, struct extension_unit_ops *eu_ops)
{
    if (sc_ops != NULL) {
        get_ot_stream()->mpi_sc_ops = sc_ops;
    }

    if (pu_ops != NULL) {
        get_ot_stream()->mpi_pu_ops = pu_ops;
    }

    if (it_ops != NULL) {
        get_ot_stream()->mpi_it_ops = it_ops;
    }

    if (eu_ops != NULL) {
        get_ot_stream()->mpi_eu_ops = eu_ops;
    }

    return 0;
}

void release_ot_stream(ot_stream *stream) {}
