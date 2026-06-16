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

#include <platform.h>
#include <types.h>
#include <common.h>
#include <lib.h>
#include "securecutil.h"
#include "uart.h"

/* Constants for reported magic-number values. */
#define GSL_UART_VALUE_2                       2
#define GSL_UART_VALUE_4                       4

#define IN
#define OUT

#define XSTART  0xFE         /* START frame head byte */
#define XFILE   0xFE        /* FILE frame head byte */
#define XDATA   0xDA        /* DATA frame head byte */
#define XEOT    0xED        /* EOT frame head byte */

#define ACK     0xAA        /* UE response byte ACK */
#define NAK     0x55        /* UE response byte NAK */

#define LOAD_RAM    0x01    /* file transfer type: RAM init file */
#define LOAD_USB    0x02    /* fiel transfer type: USB load file */

#define XUSER_DATA_LENGTH   1024    /* Data length of data frame */

#define FRAME_HEAD          0        /* The head of frame */
#define FRAME_LENGTH_FILE   13        /* The length of file frame(start from 0) */
#define FRAME_LENGTH_DATA   (XUSER_DATA_LENGTH + 4)    /* The length of standard data frame(start from 0) */
#define FRAME_LENGTH_EOT    4        /* The length of standard EOT frame(start from 0) */

#define PROC_BYTE_HEAD          0    /* The offset of frame head byte */
#define PROC_BYTE_SEQ           1    /* The offset of seq byte */
#define PROC_BYTE_SEQF          2    /* The offset of inverse seq byte */
#define PROC_BYTE_FILE_TYPE     3    /* The offset of file type byte */
#define PROC_BYTE_FILE_LENG0    4    /* The offset of file length byte */
#define PROC_BYTE_FILE_LENG1    5    /* The offset of file length byte */
#define PROC_BYTE_FILE_LENG2    6    /* The offset of file length byte */
#define PROC_BYTE_FILE_LENG3    7    /* The offset of file length byte */
#define PROC_BYTE_FILE_ADDR0    8    /* The offset of file addr byte */
#define PROC_BYTE_FILE_ADDR1    9    /* The offset of file addr byte */
#define PROC_BYTE_FILE_ADDR2    10    /* The offset of file addr byte */
#define PROC_BYTE_FILE_ADDR3    11    /* The offset of file addr byte */

#define OCR_STATUS_OK       0x01    /* protocol receive status: no error */
#define OCR_STATUS_ERROR    0x02    /* protocol receive status: frame error */
#define OCR_STATUS_SKIP     0x03    /* protocol receive status: data error, need to clear */
#define OCR_STATUS_FIN      0x04    /* The file has been download completely */

#define CRC16_LEN        2
#define BIT_CNT_4    4
#define BIT_CNT_8    8

#ifndef CFG_EDA_VERIFY
#define PROC_TIME_OUT       10  /* timeout threshold  ms */
#else
#define PROC_TIME_OUT       1   /* for eda */
#endif

typedef struct {
    uint32_t  file_type;        /* File type(RAM/USB)*/
    uint32_t  file_address;    /* The write addr for downloaded file */
    uint32_t  file_length;    /* The length for downloaded file */
    uint32_t  total_frame;    /* The total frame number for the file */
    uint32_t  count_frame;    /* Current frame number been transfered */
    uint8_t   exp_frame_idx;    /* The next frame index to expect */

    void (*SubFunction)(void);    /* The function pointer for the downloaded file*/
} data_handle_t;

unsigned int uart_inited;

typedef struct {
    uint16_t frame_crc;
    uint16_t calc_crc;
    uint16_t len;
    uint8_t  seq;
    uint8_t *write_addr;
} frame_info;

static uint16_t cal_crc_perbyte(IN uint8_t byte, IN uint16_t crc)
{
    uint8_t  da;
    const uint16_t crc_ta[16] = {0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
                     0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
                    };

    da = ((uint8_t)(crc >> BIT_CNT_8)) >> BIT_CNT_4;
    crc <<= BIT_CNT_4;
    crc ^= (uint16_t)crc_ta[da ^ (byte >> BIT_CNT_4)];
    da = ((uint8_t)(crc >> BIT_CNT_8)) >> BIT_CNT_4;
    crc <<= BIT_CNT_4;
    crc ^= (uint16_t)crc_ta[da ^ (byte & 0x0f)];

    return (crc);
}

static void uart_mux_pad_ctrl_set()
{
    reg_set(REG_AHB_IO_CFG_BASE + UART0_RXD_IOCFG_OFST, 0x1101);
    reg_set(REG_AHB_IO_CFG_BASE + UART0_TXD_IOCFG_OFST, 0x1011);
}

void uart_init()
{
    uart_mux_pad_ctrl_set();
    serial_init();
    uart_inited = 1;
}

void uart_deinit()
{
    serial_deinit();
    uart_inited = 0;
}

void uart_reset()
{
    /* clear the FIFO of uart controller */
    while (serial_tstc()) {
        (void)serial_getc();
    }
}

static bool calc_check_crc(frame_info *frame, uint32_t pos, uint8_t cr)
{
    if (pos <= frame->len - CRC16_LEN) {
        /* calculated crc */
        frame->calc_crc = cal_crc_perbyte(cr, frame->calc_crc);
    } else {
        /* received crc */
        frame->frame_crc |= (uint16_t)(cr << (BIT_CNT_8 * (frame->len - pos)));
        if (pos == frame->len) {
            /* compare calcuated crc and received crc in the last */
            return (frame->calc_crc == frame->frame_crc);
        }
    }

    return TRUE;
}

static void parse_file_frame_done(data_handle_t *data_handle)
{
    data_handle->total_frame = (data_handle->file_length - 1) / XUSER_DATA_LENGTH + GSL_UART_VALUE_2;
    data_handle->SubFunction = (void *)(uintptr_t)data_handle->file_address;
    data_handle->count_frame = 1;
    data_handle->exp_frame_idx = 1;
}

static void parse_file_frame_len_or_addr(data_handle_t *data_handle, uint32_t pos, uint8_t cr)
{
    if (pos <= PROC_BYTE_FILE_LENG3) {
        if (pos == PROC_BYTE_FILE_LENG0) {
            data_handle->file_length = 0;
        }
        data_handle->file_length |= cr << (BIT_CNT_8 * ((PROC_BYTE_FILE_LENG3) - pos));
        return;
    }

    if (pos == PROC_BYTE_FILE_ADDR0) {
        data_handle->file_address = 0;
    }
    data_handle->file_address |= cr << (BIT_CNT_8 * ((PROC_BYTE_FILE_ADDR3) - pos));
}

static int parse_file_frame(data_handle_t *data_handle, frame_info *frame,
    uint32_t pos, uint8_t cr)
{
    switch (pos) {
     case PROC_BYTE_HEAD:
        frame->len = FRAME_LENGTH_FILE;
        break;

     case PROC_BYTE_SEQ:
        frame->seq = cr;
        if (frame->seq != 0) {
            return -1;
        }
        break;

     case PROC_BYTE_SEQF:
        if (cr != (uint8_t) ~frame->seq) {
            return -1;
        }
        break;

     case PROC_BYTE_FILE_TYPE:
        if ((cr == LOAD_RAM) || (cr == LOAD_USB)) {
            data_handle->file_type = cr;
        } else {
            return -1;
        }
        break;

     case PROC_BYTE_FILE_LENG0:
     case PROC_BYTE_FILE_LENG1:
     case PROC_BYTE_FILE_LENG2:
     case PROC_BYTE_FILE_LENG3:
     case PROC_BYTE_FILE_ADDR0:
     case PROC_BYTE_FILE_ADDR1:
     case PROC_BYTE_FILE_ADDR2:
     case PROC_BYTE_FILE_ADDR3:
        parse_file_frame_len_or_addr(data_handle, pos, cr);
        break;

     default:
        break;
    }

    if (!calc_check_crc(frame, pos, cr)) {
        return -1;
    }

    if (pos == frame->len) {
        parse_file_frame_done(data_handle);
    }
    return 0;
}

static uint16_t calc_frame_len(data_handle_t *data_handle)
{
    /* the length of the last data frame is
       (file length - number of received data frame * length of data frame) +
       length of the package of the frame */
    /* the length of the rest is FRAME_LENGTH_DATA */
    return (data_handle->count_frame == (data_handle->total_frame - 1)) ?
           GSL_UART_VALUE_4 + (data_handle->file_length - (data_handle->count_frame - 1) * XUSER_DATA_LENGTH) :
           FRAME_LENGTH_DATA;
}

static bool parse_data_frame(data_handle_t *data_handle, frame_info *frame, uint32_t pos,
    uint8_t cr)
{
    switch (pos) {
     case PROC_BYTE_HEAD:
        frame->len = calc_frame_len(data_handle);
        break;

     case PROC_BYTE_SEQ:
        frame->seq = cr;
        if (frame->seq != data_handle->exp_frame_idx) {
            /* trace back */
            if ((data_handle->count_frame > 1) &&
                (frame->seq == data_handle->exp_frame_idx - 1)) {
                data_handle->count_frame--;
                data_handle->exp_frame_idx--;
                frame->len = calc_frame_len(data_handle);
            } else {
                return -1;
            }
        }

        /* obtain the address which frame should be located in */
        frame->write_addr = (uint8_t *)(uintptr_t)(data_handle->file_address +
                    (data_handle->count_frame - 1) * XUSER_DATA_LENGTH);
        break;

     case PROC_BYTE_SEQF:
        if (cr != (uint8_t) ~(frame->seq)) {
            return -1;
        }
        break;

     default:
        /* locate frame */
        if (pos <= (frame->len - CRC16_LEN))
            *(frame->write_addr++) = cr;
        break;
    }

    if (!calc_check_crc(frame, pos, cr)) {
        return -1;
    }

    if (pos == frame->len) {
        /* increase the index of frame */
        data_handle->count_frame++;
        data_handle->exp_frame_idx++;
    }
    return 0;
}

static bool parse_eot_frame(data_handle_t *data_handle, frame_info *frame, uint32_t pos, uint8_t cr)
{
    switch (pos) {
     case PROC_BYTE_HEAD:
        frame->len = FRAME_LENGTH_EOT;
        break;

     case PROC_BYTE_SEQ:
        frame->seq = cr;
        if (frame->seq != data_handle->exp_frame_idx) {
            return -1;
        }
        break;

     case PROC_BYTE_SEQF:
        if (cr != (uint8_t) ~frame->seq) {
            return -1;
        }
        break;

     default:
        break;
    }

    if (!calc_check_crc(frame, pos, cr)) {
        return -1;
    }

    /* count_frame of EOT frame should equal to total_frame */
    if ((pos == frame->len) && (data_handle->count_frame != data_handle->total_frame)) {
        return -1;
    }

    return 0;
}

static int uart_wait_data(int status)
{
    uint32_t timer_out = PROC_TIME_OUT * timer_get_divider();

    /* initialize timer controller */
    timer_start();

    /* does uart controller have any data? */
    while (!serial_tstc()) {
        /* time out. */
        if (timer_get_val() >= timer_out) {
            if (status == OCR_STATUS_ERROR) {
                return OCR_STATUS_ERROR;
            } else {
                return OCR_STATUS_SKIP;
            }
        }
    }

    return OCR_STATUS_OK;
}

uint8_t uart_hand_head_frame(IN data_handle_t *data_handle, uint8_t  *frame_type,
    uint32_t pos, int *status)
{
    errno_t err;
    uint8_t  cr;

    cr = (uint8_t)serial_getc();
    if (pos != FRAME_HEAD)
        return cr;

    if (data_handle == NULL || frame_type == NULL) {
        return 0;
    }

    *frame_type = cr;

    /* check the first byte of the whole process. */
    if ((data_handle->count_frame == 0) && (*frame_type != XFILE) && (*frame_type != XSTART)) {
        if (status == NULL) {
            return 0;
        }
        *status = OCR_STATUS_SKIP;
        return cr;
    }

    if ((*frame_type != XFILE) && (*frame_type != XSTART))
        return cr;

    err = memset_s(data_handle, sizeof(data_handle_t),
            0, sizeof(data_handle_t));
    if (err != EOK) {
        return 0;
    }

    return cr;
}

static bool uart_handle_skip_status(const data_handle_t *data_handle, int *status, uint32_t *pos)
{
    if ((*status != OCR_STATUS_SKIP) && (*status != OCR_STATUS_ERROR)) {
        return FALSE;
    }

    if (data_handle->count_frame == 0) {
        *status = OCR_STATUS_OK;
        *pos = 0;
    } else {
        (*pos)++;
    }
    return TRUE;
}

typedef struct {
    data_handle_t *data_handle;
    frame_info *frame;
    uint8_t frame_type;
    uint32_t pos;
    uint8_t cr;
    int *status;
} uart_dispatch_ctx;

static void uart_dispatch_frame(const uart_dispatch_ctx *ctx)
{
    switch (ctx->frame_type) {
    case XFILE:
        if (parse_file_frame(ctx->data_handle, ctx->frame, ctx->pos, ctx->cr)) {
            *(ctx->status) = OCR_STATUS_ERROR;
        }
        break;
    case XDATA:
        if (parse_data_frame(ctx->data_handle, ctx->frame, ctx->pos, ctx->cr)) {
            *(ctx->status) = OCR_STATUS_ERROR;
        }
        break;
    case XEOT:
        if (parse_eot_frame(ctx->data_handle, ctx->frame, ctx->pos, ctx->cr)) {
            *(ctx->status) = OCR_STATUS_ERROR;
        } else if (ctx->pos == ctx->frame->len) {
            *(ctx->status) = OCR_STATUS_FIN;
        }
        break;
    default:
        *(ctx->status) = OCR_STATUS_SKIP;
    }
}

static int uart_proc_loop(IN data_handle_t *data_handle)
{
    uint8_t cr;
    uint8_t frame_type = 0;
    uint32_t pos = FRAME_HEAD;

    int status = OCR_STATUS_OK;
    frame_info frame;
    uart_dispatch_ctx dispatch_ctx;

    if (memset_s(&frame, sizeof(frame_info), 0, sizeof(frame_info)) != EOK) {
        return -1;
    }

    while (status != OCR_STATUS_FIN) {
        if (uart_wait_data(status) != OCR_STATUS_OK) {
            return -1;
        }

        cr = uart_hand_head_frame(data_handle, &frame_type, pos, &status);
        if (uart_handle_skip_status(data_handle, &status, &pos)) {
            continue;
        }

        dispatch_ctx.data_handle = data_handle;
        dispatch_ctx.frame = &frame;
        dispatch_ctx.frame_type = frame_type;
        dispatch_ctx.pos = pos;
        dispatch_ctx.cr = cr;
        dispatch_ctx.status = &status;
        uart_dispatch_frame(&dispatch_ctx);

        if (pos == frame.len) {
            return status;
        }

        /* increase the index within the frame */
        pos++;
    }

    return status;
}

int copy_from_uart(const void *dest, size_t count)
{
    data_handle_t data_handle;
    int32_t status;
    int32_t error = 0;
    int32_t count_err = 0;
    int32_t receiving = 1;
    errno_t err;

    err = memset_s(&data_handle, sizeof(data_handle_t), 0, sizeof(data_handle_t));
    if (err != EOK) {
        return ERROR;
    }

    while (receiving) {
        status = uart_proc_loop(&data_handle);

        switch (status) {
        case OCR_STATUS_OK:
            serial_putc((uint8_t)ACK);
            if (data_handle.file_length != (uint32_t)count)
                count_err = 1;

            if (data_handle.count_frame == 1)
                data_handle.file_address = (uint32_t)(uintptr_t)dest;
            break;

        case OCR_STATUS_FIN:
            receiving = 0;
            serial_putc((uint8_t)ACK);
            if (count_err)
                return ERROR;
            return OK;

        case OCR_STATUS_ERROR:
            serial_putc((uint8_t)NAK);
            break;

        default:
            ++error;

            if (error > 0xff)
                return ERROR;

            break;
        }
    }

    return ERROR;
}
