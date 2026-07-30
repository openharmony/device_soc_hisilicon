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

#include "usb3.h"
#include "usb3_drv.h"
#include "usb3_pcd.h"
#include "usb3_prot.h"
#include "onchiprom.h"
#include <lib.h>
#include <common.h>
#define USTART 0xFA
#define UHEAD 0xFE
#define UDATA 0xDA
#define UTAIL 0xED
#define UCMD 0XAB

const char u_ack[2] = {0xAA, '\0'};
const char u_nak[2] = {0x55, '\0'};
char tx_state[200];
static unsigned int rx_addr;
static unsigned int rx_length;
uint32_t usb_status = 0;
extern void invalidate_icache_all(void);

int usb3_bulk_out_transfer_cmd(void *dev)
{
    usb3_pcd_t *pcd = (usb3_pcd_t *)dev;
    usb3_pcd_ep_t *ep = &pcd->out_ep;
    usb3_pcd_req_t *req = &ep->req;

    (void)memset_s(pcd->ss_bulk_buf, USB3_BULK_BUF_SIZE, 0x0, USB3_BULK_BUF_SIZE);
    req->length = ep->maxpacket;
    req->bufdma = (uint32_t *)pcd->ss_bulk_buf;
    req->complete = usb3_handle_protocol;
    usb3_ep_start_transfer(pcd, ep);

    return USB_NO_ERR;
}

void usb_tx_status_complete(const void *dev)
{
    usb3_pcd_t *pcd = (usb3_pcd_t *)dev;
    usb3_pcd_ep_t *ep = &pcd->in_ep;
    usb3_pcd_req_t *req = &ep->req;

    req->actual = 0;
    req->length = 0;
}

#define UHEAD_BUF_LENGTH_MAX 0x9

void usb3_handle_protocol(void *dev)
{
    usb3_pcd_t *pcd = (usb3_pcd_t *)dev;
    char *buf = (char *)pcd->ss_bulk_buf;
    usb3_pcd_ep_t *ep = &pcd->out_ep;
    usb3_pcd_req_t *req = &ep->req;

    usb_status = 0;
    if (buf[0] == UHEAD) {
        char buf_tmp[9];

        (void)memcpy_s(buf_tmp, UHEAD_BUF_LENGTH_MAX, buf + 1, UHEAD_BUF_LENGTH_MAX - 1);
        rx_addr = 0;
        rx_addr |= (buf_tmp[0x4] << 24); // 24 bit
        rx_addr |= (buf_tmp[0x5] << 16); // 16 bit
        rx_addr |= (buf_tmp[0x6] << 8); // 8 bit
        rx_addr |= buf_tmp[0x7];
        rx_length = 0;
        rx_length |= (buf_tmp[0x0] << 24); // 24 bit
        rx_length |= (buf_tmp[0x1] << 16); // 16 bit
        rx_length |= (buf_tmp[0x2] << 8); // 8 bit
        rx_length |= buf_tmp[0x3];
        usb3_bulk_in_transfer(dev, (char *)u_ack);
        usb_status = USB_STATUS_OK;
    } else if (buf[0] == UTAIL) {
        if (rx_length > 0) {
            usb3_bulk_in_transfer(dev, (char *)u_nak);
            usb_status = USB_STATUS_ERROR;
        } else {
            usb3_bulk_in_transfer(dev, (char *)u_ack);
            usb_status = USB_STATUS_FIN;
        }
    } else if (buf[0] == UDATA) {
        (void)memcpy_s((void *)(long)rx_addr, req->actual - 1, buf + 1, req->actual - 1);
        rx_addr += (req->actual - 1);
        rx_length -= (req->actual - 1);
        usb_status = USB_STATUS_DATA;
    }

    (void)usb3_bulk_out_transfer_cmd(pcd);
}

void usb_deinit(void)
{
    udc_disconnect();
}

bool self_usb_check(void)
{
    bool ret = FALSE;
    ret = usb3_driver_init();
    if (ret == TRUE) {
        return TRUE;
    }
    usb_deinit();
    return FALSE;
}

int copy_from_usb(const void *dest, size_t count)
{
    uint32_t timerout = USB_PROC_TIME_OUT * timer_get_divider();
    uint32_t not_raw_burn = 0;

    while (1) {
        usb_status = 0;
        timer_start();

        while (usb_status == 0) {
            if (timer_get_val() >= timerout) {
                goto out;
            }

            usb_poll();
        }

        switch (usb_status) {
            case USB_STATUS_OK:
                if (rx_length != (uint32_t)count) {
                    not_raw_burn = 1;
                }
                rx_addr = (long)(uintptr_t)dest;
                break;

            case USB_STATUS_FIN:
                if (not_raw_burn) {
                    return TD_FAILURE;
                }
                return TD_SUCCESS;
                break;

            case USB_STATUS_DATA:
                break;

            case USB_STATUS_ERROR:
            default:
                goto out;

                break;
        }
    }

out:
    usb_deinit();
    return TD_FAILURE;
}
