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
#include "usb3_hw.h"
#include "usb3_drv.h"
#include "usb3_prot.h"
#include <lib.h>

#define BULK_EP_MAX_PACKET_SIZE   512
#define USB3_STRING_DESC_HEADER_LEN 2
#define USB3_STRING_LANG_DESC_LEN 0x04
#define USB3_STRING_LANG_ID_LOW 0x09
#define USB3_STRING_LANG_ID_HIGH 0x04

const struct usb_interface_descriptor g_intf_desc = {
    sizeof(struct usb_interface_descriptor), /* bLength */
    UDESC_INTERFACE,                         /* bDescriptorType */

    0,                     /* bInterfaceNumber */
    0,                     /* bAlternateSetting */
    2,                     /* bNumEndpoints */
    USB_CLASS_VENDOR_SPEC, /* bInterfaceClass */
    USB_SC_VENDOR_SPEC,    /* bInterfaceSubClass */
    USB_PR_VENDOR_SPEC,    /* bInterfaceProtocol */
    0,                     /* iInterface */
};

/* Two endpoint descriptors: bulk-in, bulk-out. */

const struct usb_endpoint_descriptor hs_bulk_in = {
    sizeof(struct usb_endpoint_descriptor), /* bLength */
    UDESC_ENDPOINT,                         /* bDescriptorType */

    UE_DIR_IN | USB3_BULK_IN_EP, /* bEndpointAddress */
    USB_ENDPOINT_XFER_BULK,      /* bmAttributes */
    0x200,                       /* wMaxPacketSize: 512 of high-speed */
    0,                           /* bInterval */
};

const struct usb_endpoint_descriptor hs_bulk_out = {
    sizeof(struct usb_endpoint_descriptor), /* bLength */
    UDESC_ENDPOINT,                         /* bDescriptorType */

    UE_DIR_OUT | USB3_BULK_OUT_EP, /* bEndpointAddress */
    USB_ENDPOINT_XFER_BULK,        /* bmAttributes */
    0x200,                         /* wMaxPacketSize: 512 of high-speed */
    1,                             /* bInterval */
};

usb3_pcd_ep_t *usb3_get_out_ep(usb3_pcd_t *pcd, uint32_t ep_num)
{
    if (ep_num == 0) {
        return &pcd->ep0;
    } else {
        return &pcd->out_ep;
    }
}

usb3_pcd_ep_t *usb3_get_in_ep(usb3_pcd_t *pcd, uint32_t ep_num)
{
    if (ep_num == 0) {
        return &pcd->ep0;
    } else {
        return &pcd->in_ep;
    }
}

usb3_pcd_ep_t *usb3_get_ep_by_addr(usb3_pcd_t *pcd, uint16_t index)
{
    uint32_t ep_num = ue_get_addr(index);

    if (ue_get_dir(index) == UE_DIR_IN) {
        return usb3_get_in_ep(pcd, ep_num);
    } else {
        return usb3_get_out_ep(pcd, ep_num);
    }
}

void usb3_ep_clear_stall(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep)
{
    usb3_dev_ep_regs_t *ep_reg;

    if (ep->is_in) {
        ep_reg = ep->in_ep_reg;
    } else {
        ep_reg = ep->out_ep_reg;
    }

    usb3_dep_cstall(pcd, ep_reg);
}

uint32_t usb3_get_device_speed(usb3_pcd_t *pcd)
{
    uint32_t dsts;
    uint32_t speed = USB_SPEED_UNKNOWN;

    dsts = usb3_rd32(&pcd->dev_global_regs->dsts);

    switch ((dsts >> USB3_DSTS_CONNSPD_SHIFT) & (USB3_DSTS_CONNSPD_BITS >> USB3_DSTS_CONNSPD_SHIFT)) {
        case USB3_SPEED_HS_PHY_30MHZ_OR_60MHZ:
            speed = USB_SPEED_HIGH;
            break;

        case USB3_SPEED_FS_PHY_30MHZ_OR_60MHZ:
        case USB3_SPEED_FS_PHY_48MHZ:
            speed = USB_SPEED_FULL;
            break;

        case USB3_SPEED_LS_PHY_6MHZ:
            speed = USB_SPEED_LOW;
            break;

        default:
            break;
    }

    return speed;
}

void usb3_pcd_set_speed(usb3_pcd_t *pcd, int speed)
{
    /* Set the MPS of EP0 based on the connection speed */
    switch (speed) {
        case USB_SPEED_HIGH:
            pcd->ep0.maxpacket = 64; // 64 is max packet size of ep0
            pcd->in_ep.maxpacket = USB2_HS_MAX_PACKET_SIZE;
            pcd->out_ep.maxpacket = USB2_HS_MAX_PACKET_SIZE;
            break;

        case USB_SPEED_FULL:
            pcd->ep0.maxpacket = 64; // 64 is max packet size of ep0
            pcd->in_ep.maxpacket = USB2_FS_MAX_PACKET_SIZE;
            pcd->out_ep.maxpacket = USB2_FS_MAX_PACKET_SIZE;
            break;

        case USB_SPEED_LOW:
            pcd->ep0.maxpacket = 8; // 8 is max packet size of ep0
            pcd->in_ep.maxpacket = 0;
            pcd->out_ep.maxpacket = 0;
            break;

        default:
            break;
    }
}

void usb3_ep_set_stall(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep)
{
    usb3_dev_ep_regs_t *ep_reg;

    if (ep->is_in) {
        ep_reg = ep->in_ep_reg;
    } else {
        ep_reg = ep->out_ep_reg;
    }

    usb3_dep_sstall(pcd, ep_reg);
}

static void ep0_do_stall(usb3_pcd_t *pcd)
{
    usb3_pcd_ep_t *ep0 = &pcd->ep0;

    /* Stall EP0 IN & OUT simultanelusly */
    ep0->is_in = 1;
    usb3_ep_set_stall(pcd, ep0);
    ep0->is_in = 0;
    usb3_ep_set_stall(pcd, ep0);

    /* Prepare for the next setup transfer */
    ep0->stopped = 1;
    pcd->ep0state = EP0_IDLE;
    usb3_ep0_out_start(pcd);
}

static void do_clear_halt(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep)
{
    usb3_ep_clear_stall(pcd, ep);

    if (ep->stopped) {
        ep->stopped = 0;
    }
}

void usb3_pcd_ep_enable(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep)
{
    /*
     * Activate the EP
     */
    ep->stopped = 0;
    ep->xfer_started = 0;

    /* Set initial data PID. */
    if (ep->type == USB3_EP_TYPE_BULK) {
        ep->data_pid_start = 0;
    }

    usb3_ep_activate(pcd, ep);
}

static void usb3_do_get_status(usb3_pcd_t *pcd)
{
    usb_device_request_t ctrl = pcd->ep0_setup_pkt->req;
    uint8_t *status = pcd->ep0_status_buf;
    usb3_pcd_ep_t *ep;

    if (ctrl.w_length != 2) { // 2 is length of ctrl
        ep0_do_stall(pcd);
        return;
    }

    switch (ut_get_recipient(ctrl.bm_request_type)) {
        case UT_DEVICE:
            *status = 0; /* Bus powered */
            *(status + 1) = 0;
            break;

        case UT_INTERFACE:
            *status = 0;
            *(status + 1) = 0;
            break;

        case UT_ENDPOINT:
            ep = usb3_get_ep_by_addr(pcd, ctrl.w_index);

            /* @todo check for EP stall */
            *status = ep->stopped;
            *(status + 1) = 0;
            break;

        default:
            ep0_do_stall(pcd);
            return;
    }

    pcd->ep0_req.bufdma = (uint32_t *)status;
    pcd->ep0_req.length = 2; // 2 is length of ctrl
    pcd->ep0_req.actual = 0;
    usb3_ep0_start_transfer(pcd, &pcd->ep0_req);
}

static int usb3_check_device_feature(usb3_pcd_t *pcd, usb_device_request_t ctrl, int val)
{
    switch (ctrl.w_value) {
        case UF_DEVICE_REMOTE_WAKEUP:
        case UF_TEST_MODE:
        case UF_DEVICE_B_HNP_ENABLE:
        case UOTG_NTF_HOST_REL:
        case UOTG_B3_RSP_ENABLE:
        case UF_DEVICE_A_HNP_SUPPORT:
        case UF_DEVICE_A_ALT_HNP_SUPPORT:
            break;

        case UF_U1_ENABLE:
        case UF_U2_ENABLE:
            if (pcd->speed != USB_SPEED_SUPER || pcd->state != USB3_STATE_CONFIGURED) {
                return -1;
            }
            break;

        case UF_LTM_ENABLE:
            if (pcd->speed != USB_SPEED_SUPER || pcd->state != USB3_STATE_CONFIGURED || ctrl.w_index != 0) {
                return -1;
            }
            pcd->ltm_enable = val;
            break;

        default:
            return -1;
    }

    return 0;
}

static int usb3_do_endpoint_feature(usb3_pcd_t *pcd, usb_device_request_t ctrl, int val)
{
    usb3_pcd_ep_t *ep = usb3_get_ep_by_addr(pcd, ctrl.w_index);

    if (ctrl.w_value != UF_ENDPOINT_HALT) {
        return -1;
    }
    if (val == 1) {
        ep->stopped = 1;
        usb3_ep_set_stall(pcd, ep);
    } else {
        do_clear_halt(pcd, ep);
    }
    return 0;
}

/* val==1 means set feature; val == 0 means clear feature*/
static void usb3_do_set_clear_feature(usb3_pcd_t *pcd, int val)
{
    usb_device_request_t ctrl = pcd->ep0_setup_pkt->req;

    switch (ut_get_recipient(ctrl.bm_request_type)) {
        case UT_DEVICE:
            if (usb3_check_device_feature(pcd, ctrl, val) != 0) {
                ep0_do_stall(pcd);
                return;
            }
            break;

        case UT_INTERFACE:
            /* if FUNCTION_SUSPEND ... */
            if (ctrl.w_value) {
                ep0_do_stall(pcd);
                return;
            }
            break;

        case UT_ENDPOINT:
            if (usb3_do_endpoint_feature(pcd, ctrl, val) != 0) {
                ep0_do_stall(pcd);
                return;
            }
            break;

        default:
            break;
    }

    pcd->ep0.is_in = 1;
    pcd->ep0state = EP0_IN_WAIT_NRDY;
}

static void usb3_do_set_address(usb3_pcd_t *pcd)
{
    usb_device_request_t ctrl = pcd->ep0_setup_pkt->req;

    if (ctrl.bm_request_type == UT_DEVICE) {
        usb3_set_address(pcd, ctrl.w_value);
        pcd->ep0.is_in = 1;
        pcd->ep0state = EP0_IN_WAIT_NRDY;
        if (ctrl.w_value) {
            pcd->state = USB3_STATE_ADDRESSED;
        } else {
            pcd->state = USB3_STATE_DEFAULT;
        }
    }
}

static void usb3_do_set_config(usb3_pcd_t *pcd)
{
    usb_device_request_t ctrl = pcd->ep0_setup_pkt->req;
    uint16_t wvalue = ctrl.w_value;
    usb3_pcd_ep_t *ep;

    if (ctrl.bm_request_type != (UT_WRITE | UT_STANDARD | UT_DEVICE)) {
        ep0_do_stall(pcd);
        return;
    }

    if (!wvalue || (wvalue == CONFIG_VALUE)) {
        pcd->new_config = wvalue;
        /* Set new configuration */
        if (wvalue) {
            /* Activate bulk in endpoint */
            ep = &pcd->in_ep;
            usb3_pcd_ep_enable(pcd, ep);

            /* Activate bulk out endpoint */
            ep = &pcd->out_ep;
            usb3_pcd_ep_enable(pcd, ep);

            /* Prepare for next bulk transfer */
            usb3_bulk_out_transfer((void *)pcd);

            pcd->state = USB3_STATE_CONFIGURED;
        } else {
            pcd->state = USB3_STATE_ADDRESSED;
        }

        pcd->ep0.is_in = 1;
        pcd->ep0state = EP0_IN_WAIT_NRDY;
    } else {
        ep0_do_stall(pcd);
    }
}

static void usb3_do_get_config(usb3_pcd_t *pcd)
{
    usb_device_request_t ctrl = pcd->ep0_setup_pkt->req;
    uint8_t *status = pcd->ep0_status_buf;

    if (ctrl.bm_request_type != (UT_READ | UT_STANDARD | UT_DEVICE)) {
        ep0_do_stall(pcd);
        return;
    }

    /* Notify host the current config value */
    *status = pcd->new_config;

    pcd->ep0_req.bufdma = (uint32_t *)status;
    pcd->ep0_req.length = 1;
    pcd->ep0_req.actual = 0;
    usb3_ep0_start_transfer(pcd, &pcd->ep0_req);
}

static uint16_t usb3_fill_device_desc(usb3_pcd_t *pcd, uint8_t *buf)
{
    usb3_device_t *usb3_dev = pcd->usb3_dev;
    struct usb_device_descriptor *dev = (struct usb_device_descriptor *)usb3_dev->dev_desc;
    uint16_t value = sizeof(struct usb_device_descriptor);

    dev->b_length = sizeof(struct usb_device_descriptor);
    dev->b_descriptor_type = UDESC_DEVICE;
    dev->b_device_class = 0;
    dev->b_device_sub_class = 0;
    dev->b_device_protocol = 0;
    if (pcd->speed == USB_SPEED_SUPER) {
        dev->bcd_usb = 0x300;
        dev->b_max_packet_size0 = 9; // NOTE! 2 ^ 9 = 512 for USB3
    } else if (pcd->speed == USB_SPEED_HIGH) {
        dev->bcd_usb = 0x0200;
        dev->b_max_packet_size0 = pcd->ep0.maxpacket;
    } else {
        dev->bcd_usb = 0x0110;
        dev->b_max_packet_size0 = pcd->ep0.maxpacket;
    }
    dev->id_vendor = USB_VENDOR_ID;
    dev->id_product = USB_PRODUCT_ID;
    dev->bcd_device = 0x0100;
    dev->i_manufacturer = STRING_MANUFACTURER;
    dev->i_product = STRING_PRODUCT;
    dev->i_serial_number = 0;
    dev->b_num_configurations = 1;
    (void)memcpy_s(buf, value, dev, value);
    return value;
}

static uint16_t usb3_fill_qualifier_desc(usb3_pcd_t *pcd, uint8_t *buf)
{
    usb3_device_t *usb3_dev = pcd->usb3_dev;
    struct usb_qualifier_descriptor *qual = (struct usb_qualifier_descriptor *)buf;
    struct usb_device_descriptor *dev = (struct usb_device_descriptor *)usb3_dev->dev_desc;

    qual->b_length = sizeof(*qual);
    qual->b_descriptor_type = UDESC_DEVICE_QUALIFIER;
    qual->bcd_usb = dev->bcd_usb;
    qual->b_device_class = dev->b_device_class;
    qual->b_device_sub_class = dev->b_device_sub_class;
    qual->b_device_protocol = dev->b_device_protocol;
    qual->b_max_packet_size0 = dev->b_max_packet_size0;
    qual->b_num_configurations = 1;
    qual->b_reserved = 0;
    return sizeof(usb_qualifier_descriptor_t);
}

static uint16_t usb3_fill_config_desc(usb3_pcd_t *pcd, uint8_t *buf)
{
    struct usb_config_descriptor *config = (struct usb_config_descriptor *)buf;

    config->b_length = sizeof(*config);
    config->b_descriptor_type = UDESC_CONFIG;
    config->b_num_interfaces = 1;
    config->b_configuration_value = CONFIG_VALUE;
    config->i_configuration = 0;
    config->bm_attributes = USB_CONFIG_ATT_ONE;
    config->b_max_power = (pcd->speed == USB_SPEED_SUPER) ?
        (USB_CONFIG_VBUS_DRAW / 0x8) : (USB_CONFIG_VBUS_DRAW / 0x2);

    buf += sizeof(*config);
    (void)memcpy_s(buf, sizeof(g_intf_desc), &g_intf_desc, sizeof(g_intf_desc));
    buf += sizeof(g_intf_desc);
    (void)memcpy_s(buf, sizeof(hs_bulk_in), &hs_bulk_in, sizeof(hs_bulk_in));
    buf += sizeof(hs_bulk_in);
    (void)memcpy_s(buf, sizeof(hs_bulk_out), &hs_bulk_out, sizeof(hs_bulk_out));
    config->w_total_length = sizeof(*config) + sizeof(g_intf_desc) + sizeof(hs_bulk_in) + sizeof(hs_bulk_out);
    return config->w_total_length;
}

static int usb3_fill_string_desc(usb3_pcd_t *pcd, uint8_t index, uint8_t *buf)
{
    usb3_device_t *usb3_dev = pcd->usb3_dev;

    switch (index) {
        case STRING_LANGUAGE:
            buf[0] = USB3_STRING_LANG_DESC_LEN;
            buf[1] = UDESC_STRING;
            buf[USB3_STRING_DESC_HEADER_LEN] = USB3_STRING_LANG_ID_LOW;
            buf[USB3_STRING_DESC_HEADER_LEN + 1] = USB3_STRING_LANG_ID_HIGH;
            return USB3_STRING_LANG_DESC_LEN;
        case STRING_MANUFACTURER:
            buf[0] = usb3_dev->string_manu_len + USB3_STRING_DESC_HEADER_LEN;
            buf[1] = UDESC_STRING;
            (void)memcpy_s((buf + USB3_STRING_DESC_HEADER_LEN), usb3_dev->string_manu_len,
                usb3_dev->string_manu, usb3_dev->string_manu_len);
            return usb3_dev->string_manu_len + USB3_STRING_DESC_HEADER_LEN;
        case STRING_PRODUCT:
            buf[0] = usb3_dev->string_prod_len + USB3_STRING_DESC_HEADER_LEN;
            buf[1] = UDESC_STRING;
            (void)memcpy_s((buf + USB3_STRING_DESC_HEADER_LEN), usb3_dev->string_prod_len,
                usb3_dev->string_prod, usb3_dev->string_prod_len);
            return usb3_dev->string_prod_len + USB3_STRING_DESC_HEADER_LEN;
        default:
            return -1;
    }
}

static void usb3_do_get_descriptor(usb3_pcd_t *pcd)
{
    usb_device_request_t ctrl = pcd->ep0_setup_pkt->req;
    uint8_t dt = ctrl.w_value >> 8; // value shift 8 bits
    uint8_t index = (uint8_t)ctrl.w_value;
    uint16_t len = ctrl.w_length;
    uint8_t *buf = pcd->ep0_status_buf;
    int value = 0;

    if (ctrl.bm_request_type != (UT_READ | UT_STANDARD | UT_DEVICE)) {
        ep0_do_stall(pcd);
        return;
    }

    switch (dt) {
        case UDESC_DEVICE:
            value = usb3_fill_device_desc(pcd, buf);
            break;
        case UDESC_DEVICE_QUALIFIER:
            value = usb3_fill_qualifier_desc(pcd, buf);
            break;
        case UDESC_CONFIG:
            value = usb3_fill_config_desc(pcd, buf);
            break;
        case UDESC_STRING:
            value = usb3_fill_string_desc(pcd, index, buf);
            if (value < 0) {
                ep0_do_stall(pcd);
                return;
            }
            break;
        default:
            ep0_do_stall(pcd);
            return;
    }

    pcd->ep0_req.bufdma = (uint32_t *)(pcd->ep0_status_buf);
    pcd->ep0_req.length = value < len ? value : len;
    pcd->ep0_req.actual = 0;
    usb3_ep0_start_transfer(pcd, &pcd->ep0_req);
}

static void usb3_prepare_ep0_setup(usb3_pcd_t *pcd, usb_device_request_t ctrl)
{
    usb3_pcd_ep_t *ep0 = &pcd->ep0;

    ep0->stopped = 0;
    ep0->three_stage = 1;
    if (ctrl.bm_request_type & UE_DIR_IN) {
        ep0->is_in = 1;
        pcd->ep0state = EP0_IN_DATA_PHASE;
    } else {
        ep0->is_in = 0;
        pcd->ep0state = EP0_OUT_DATA_PHASE;
    }

    if (ctrl.w_length == 0) {
        ep0->is_in = 1;
        pcd->ep0state = EP0_IN_WAIT_NRDY;
        ep0->three_stage = 0;
    }
}

static void usb3_do_set_sel(usb3_pcd_t *pcd)
{
    /* For now this is a no-op */
    pcd->ep0_req.bufdma = (uint32_t *)(pcd->ep0_status_buf);
    pcd->ep0_req.length = USB3_STATUS_BUF_SIZE;
    pcd->ep0_req.actual = 0;
    pcd->ep0.send_zlp = 0;
    usb3_ep0_start_transfer(pcd, &pcd->ep0_req);
}

void usb3_do_setup(usb3_pcd_t *pcd)
{
    usb_device_request_t ctrl = pcd->ep0_setup_pkt->req;

    usb3_prepare_ep0_setup(pcd, ctrl);

    if ((ut_get_type(ctrl.bm_request_type)) != UT_STANDARD) {
        ep0_do_stall(pcd);
        return;
    }

    switch (ctrl.b_request) {
        case UR_GET_STATUS:
            usb3_do_get_status(pcd);
            break;

        case UR_CLEAR_FEATURE:
            usb3_do_set_clear_feature(pcd, 0);
            break;

        case UR_SET_FEATURE:
            usb3_do_set_clear_feature(pcd, 1);
            break;

        case UR_SET_ADDRESS:
            usb3_do_set_address(pcd);
            break;

        case UR_SET_CONFIG:
            usb3_do_set_config(pcd);
            usb_info("usb enum done\n");

            pcd->ltm_enable = 0;
            break;

        case UR_GET_CONFIG:
            usb3_do_get_config(pcd);
            break;

        case UR_GET_DESCRIPTOR:
            usb3_do_get_descriptor(pcd);
            break;

        case UR_SET_SEL:
            usb3_do_set_sel(pcd);
            break;

        case UR_SET_ISOC_DELAY:
            /* For now this is a no-op */
            pcd->ep0.is_in = 1;
            pcd->ep0state = EP0_IN_WAIT_NRDY;
            break;

        default:
            ep0_do_stall(pcd);
            break;
    }
}

void usb3_os_get_trb(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep, usb3_pcd_req_t *req)
{
    /* If EP0, fill request with EP0 IN/OUT data TRB */
    if (ep == &pcd->ep0) {
        if (ep->is_in) {
            req->trb = pcd->ep0_in_desc;
            req->trbdma = (uint32_t)(long)pcd->ep0_in_desc;
        } else {
            req->trb = pcd->ep0_out_desc;
            req->trbdma = (uint32_t)(long)pcd->ep0_out_desc;
        }
        /* Else fill request with TRB from the non-EP0 allocation */
    } else {
        req->trb = ep->ep_desc;
        req->trbdma = (uint32_t)(long)ep->ep_desc;
    }
}

void usb3_ep0_start_transfer(usb3_pcd_t *pcd, usb3_pcd_req_t *req)
{
    usb3_pcd_ep_t *ep0 = &pcd->ep0;
    usb3_dev_ep_regs_t *ep_reg;
    usb3_dma_desc_t *desc;
    uint32_t desc_dma;
    uint32_t desc_type;
    uint32_t len;
    uint8_t tri;

    /* Get the DMA Descriptor (TRB) for this request */
    usb3_os_get_trb(pcd, ep0, req);
    desc = req->trb;
    desc_dma = req->trbdma;

    if (ep0->is_in) {
        /*
         * Start DMA on EP0-IN
         */
        ep_reg = ep0->in_ep_reg;

        /* DMA Descriptor (TRB) setup */
        len = req->length;

        if (pcd->ep0state == EP0_IN_STATUS_PHASE) {
            if (ep0->three_stage) {
                desc_type = USB3_DSCCTL_TRBCTL_STATUS_3;
            } else {
                desc_type = USB3_DSCCTL_TRBCTL_STATUS_2;
            }
        } else {
            desc_type = USB3_DSCCTL_TRBCTL_CTLDATA_1ST;
        }

        usb3_fill_desc(&(usb3_desc_param_t) {
            desc, (uint32_t)(long)req->bufdma, len, 0, desc_type,
            USB3_DSCCTL_IOC_BIT | USB3_DSCCTL_ISP_BIT | USB3_DSCCTL_LST_BIT, 1
        });
        /* Issue "DEPSTRTXFER" command to EP0-IN */
        tri = usb3_dep_startxfer(pcd, ep_reg, desc_dma, 0);
        ep0->tri_in = tri;
    } else {
        /*
         * Start DMA on EP0-OUT
         */
        ep_reg = ep0->out_ep_reg;

        /* DMA Descriptor (TRB) setup */
        len = (req->length + ep0->maxpacket - 1) & ~(ep0->maxpacket - 1);

        if (pcd->ep0state == EP0_OUT_STATUS_PHASE) {
            if (ep0->three_stage) {
                desc_type = USB3_DSCCTL_TRBCTL_STATUS_3;
            } else {
                desc_type = USB3_DSCCTL_TRBCTL_STATUS_2;
            }
        } else {
            desc_type = USB3_DSCCTL_TRBCTL_CTLDATA_1ST;
        }

        usb3_fill_desc(&(usb3_desc_param_t) {
            desc, (uint32_t)(long)req->bufdma, len, 0, desc_type,
            USB3_DSCCTL_IOC_BIT | USB3_DSCCTL_ISP_BIT | USB3_DSCCTL_LST_BIT, 1
        });
        /* Issue "DEPSTRTXFER" command to EP0-OUT */
        tri = usb3_dep_startxfer(pcd, ep_reg, desc_dma, 0);
        ep0->tri_out = tri;
    }
}

static void ep0_continue_transfer(usb3_pcd_t *pcd, usb3_pcd_req_t *req)
{
    usb3_pcd_ep_t *ep0 = &pcd->ep0;
    usb3_dev_ep_regs_t *ep_reg;
    usb3_dma_desc_t *desc;
    uint32_t desc_dma;
    uint8_t tri;

    /* It can be called to send a 0-length packet after the end of a transfer, so the code here
     * only supports that case.
     */

    if (ep0->is_in) {
        desc = pcd->ep0_in_desc;
        desc_dma = (uint32_t)(long)pcd->ep0_in_desc;
        ep_reg = ep0->in_ep_reg;

        /* DMA Descriptor Setup */
        usb3_fill_desc(&(usb3_desc_param_t) {
            desc, (uint32_t)(long)req->bufdma, 0, 0, USB3_DSCCTL_TRBCTL_NORMAL,
            USB3_DSCCTL_IOC_BIT | USB3_DSCCTL_ISP_BIT | USB3_DSCCTL_LST_BIT, 1
        });

        tri = usb3_dep_startxfer(pcd, ep_reg, desc_dma, 0);
        ep0->tri_in = tri;
    }
}

void usb3_ep_start_transfer(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep)
{
    usb3_pcd_req_t *req = &ep->req;
    usb3_dev_ep_regs_t *ep_reg;
    usb3_dma_desc_t *desc;
    uint32_t desc_dma;
    uint32_t len;
    uint8_t tri;

    /* Get the TRB for this request */
    usb3_os_get_trb(pcd, ep, req);

    ep->send_zlp = 0;
    desc = req->trb;
    desc_dma = req->trbdma;

    if (ep->is_in) {
        /* For IN, TRB length is just xfer length */
        len = req->length;
    } else {
        /* For OUT, TRB length must be multiple of maxpacket */
        /* Must be power of 2, use cheap AND */
        len = (req->length + ep->maxpacket - 1) & ~(ep->maxpacket - 1);

        req->length = len;
    }

    /* DMA Descriptor Setup */
    usb3_fill_desc(&(usb3_desc_param_t) {
        desc, (uint32_t)(long)req->bufdma, len, 0, USB3_DSCCTL_TRBCTL_NORMAL,
        USB3_DSCCTL_ISP_BIT | USB3_DSCCTL_IOC_BIT | USB3_DSCCTL_LST_BIT, 1
    });

    if (ep->is_in) {
        /*
         * Start DMA on EPn-IN
         */
        ep_reg = ep->in_ep_reg;
        /* Issue "DEPSTRTXFER" command to EP */
        tri = usb3_dep_startxfer(pcd, ep_reg, desc_dma, 0);
        ep->tri_in = tri;
    } else {
        /*
         * Start DMA on EPn-OUT
         */
        ep_reg = ep->out_ep_reg;
        if (ep->xfer_started) {
            /* Issue "DEPUPDTXFER" command to EP */
            usb3_dep_updatexfer(pcd, ep_reg, ep->tri_out);
        } else {
            /* Issue "DEPSTRTXFER" command to EP */
            tri = usb3_dep_startxfer(pcd, ep_reg, desc_dma, 0);
            ep->tri_out = tri;
            ep->xfer_started = 1;
        }
    }
}

void usb3_bulk_out_transfer(void *dev)
{
    usb3_pcd_t *pcd = (usb3_pcd_t *)dev;
    usb3_pcd_ep_t *ep = &pcd->out_ep;
    usb3_pcd_req_t *req = &ep->req;

    req->length = ep->maxpacket;
    req->bufdma = (uint32_t *)(pcd->ss_bulk_buf);
    req->complete = usb3_handle_protocol;

    usb3_ep_start_transfer(pcd, ep);
}

void usb3_bulk_out_continue_transfer(void *dev)
{
    usb3_pcd_t *pcd = (usb3_pcd_t *)dev;
    usb3_pcd_ep_t *ep = &pcd->out_ep;

    usb3_ep_start_transfer(pcd, ep);
}

void usb3_bulk_in_transfer(void *dev, const char *status)
{
    usb3_pcd_t *pcd = (usb3_pcd_t *)dev;
    usb3_pcd_ep_t *ep = &pcd->in_ep;
    usb3_pcd_req_t *req = &ep->req;
    int len;

    len = strlen(status) + 1;
    (void)memset_s(req->bufdma, BULK_EP_MAX_PACKET_SIZE, 0, BULK_EP_MAX_PACKET_SIZE);
    (void)memcpy_s(req->bufdma, len, status, len);
    req->length = len;

    req->complete = usb_tx_status_complete;

    usb3_ep_start_transfer(pcd, ep);
}

static void ep0_complete_request(usb3_pcd_t *pcd, usb3_pcd_req_t *req, usb3_dma_desc_t *desc,
    int status)
{
    usb3_pcd_ep_t *ep = &pcd->ep0;

    if (!req) {
        return;
    }

    if (pcd->ep0state == EP0_OUT_DATA_PHASE || pcd->ep0state == EP0_IN_DATA_PHASE) {
        if (ep->is_in) {
            if (usb3_get_xfercnt(desc) == 0) {
                pcd->ep0.is_in = 0;
                pcd->ep0state = EP0_OUT_WAIT_NRDY;
            }
        } else {
            pcd->ep0.is_in = 1;
            pcd->ep0state = EP0_IN_WAIT_NRDY;
        }
    }
}

static void setup_in_status_phase(usb3_pcd_t *pcd, void *buf)
{
    usb3_pcd_ep_t *ep0 = &pcd->ep0;

    if (pcd->ep0state == EP0_STALL) {
        return;
    }

    ep0->is_in = 1;
    pcd->ep0state = EP0_IN_STATUS_PHASE;

    pcd->ep0_req.bufdma = (uint32_t *)buf;
    pcd->ep0_req.length = 0;
    pcd->ep0_req.actual = 0;
    usb3_ep0_start_transfer(pcd, &pcd->ep0_req);
}

static void setup_out_status_phase(usb3_pcd_t *pcd, void *buf)
{
    usb3_pcd_ep_t *ep0 = &pcd->ep0;

    if (pcd->ep0state == EP0_STALL) {
        return;
    }

    ep0->is_in = 0;
    pcd->ep0state = EP0_OUT_STATUS_PHASE;

    pcd->ep0_req.bufdma = (uint32_t *)buf;
    pcd->ep0_req.length = 0;
    pcd->ep0_req.actual = 0;
    usb3_ep0_start_transfer(pcd, &pcd->ep0_req);
}

void usb3_handle_ep0_in_data(usb3_pcd_t *pcd, usb3_pcd_req_t *req, uint32_t event)
{
    usb3_pcd_ep_t *ep0 = &pcd->ep0;
    usb3_dma_desc_t *desc = NULL;
    uint32_t byte_count;
    uint32_t status;
    if (!req) {
        req = &pcd->ep0_req;
    }

    if (req == NULL) {
        return;
    }

    desc = pcd->ep0_in_desc;

    if (dwc_usb3_is_hwo(desc)) {
        return;
    }

    status = usb3_get_xfersts(desc);
    if (status & USB3_TRBRSP_SETUP_PEND) {
        /* Start of a new Control transfer */
        desc->status = 0;
    }

    byte_count = req->length - usb3_get_xfercnt(desc);
    req->actual += byte_count;
    req->bufdma += byte_count;

    if (req->actual < req->length) {
        /* IN CONTINUE, Stall EP0 */
        ep0_do_stall(pcd);
    } else if (ep0->send_zlp) {
        /* CONTINUE TRANSFER IN ZLP */
        ep0_continue_transfer(pcd, req);
        ep0->send_zlp = 0;
    } else {
        /* COMPLETE IN TRANSFER */
        ep0_complete_request(pcd, req, desc, 0);
    }
    return;
}

void usb3_handle_ep0_out_data(usb3_pcd_t *pcd, usb3_pcd_req_t *req, uint32_t event)
{
    usb3_pcd_ep_t *ep0 = &pcd->ep0;
    usb3_dma_desc_t *desc = NULL;
    uint32_t byte_count;
    uint32_t len;

    if (!req) {
        req = &pcd->ep0_req;
    }

    if (req == NULL) {
        return;
    }

    desc = pcd->ep0_out_desc;

    if (dwc_usb3_is_hwo(desc)) {
        return;
    }

    len = (req->length + ep0->maxpacket - 1) & ~(ep0->maxpacket - 1);
    byte_count = len - usb3_get_xfercnt(desc);
    req->actual += byte_count;
    req->bufdma += byte_count;

    if (ep0->send_zlp) {
        /* CONTINUE TRANSFER OUT ZLP */
        ep0_continue_transfer(pcd, req);
        ep0->send_zlp = 0;
    } else {
        /* COMPLETE OUT TRANSFER */
        ep0_complete_request(pcd, req, desc, 0);
    }
    return;
}

void usb3_handle_ep0(usb3_pcd_t *pcd, usb3_pcd_req_t *req, uint32_t event)
{
    usb3_pcd_ep_t *ep0 = &pcd->ep0;
    usb3_dma_desc_t *desc;

    switch (pcd->ep0state) {
        case EP0_IN_DATA_PHASE:
            usb3_handle_ep0_in_data(pcd, req, event);
            break;

        case EP0_OUT_DATA_PHASE:
            usb3_handle_ep0_out_data(pcd, req, event);
            break;

        case EP0_IN_WAIT_NRDY:
        case EP0_OUT_WAIT_NRDY:
            if (ep0->is_in) {
                setup_in_status_phase(pcd, pcd->ep0_setup_pkt);
            } else {
                setup_out_status_phase(pcd, pcd->ep0_setup_pkt);
            }

            break;

        case EP0_IN_STATUS_PHASE:
        case EP0_OUT_STATUS_PHASE:
            if (ep0->is_in) {
                desc = pcd->ep0_in_desc;
            } else {
                desc = pcd->ep0_out_desc;
            }
            ep0_complete_request(pcd, req, desc, 0);

            pcd->ep0state = EP0_IDLE;
            ep0->stopped = 1;
            ep0->is_in = 0; /* OUT for next SETUP */

            /* Prepare for more SETUP Packets */
            usb3_ep0_out_start(pcd);
            break;

        case EP0_STALL:
            break;

        case EP0_IDLE:
            break;

        default:
            break;
    }

    return;
}

void usb3_os_handle_ep0(usb3_pcd_t *pcd, uint32_t event)
{
    usb3_pcd_req_t *req = NULL;

    if (pcd->ep0state == EP0_IDLE) {
        usb3_do_setup(pcd);
    } else {
        usb3_handle_ep0(pcd, req, event);
    }
}

int usb3_request_done(usb3_pcd_t *pcd, const usb3_pcd_ep_t *ep, usb3_pcd_req_t *req, int status)
{
    int error = USB_NO_ERR;

    if (ep != &pcd->ep0) {
        req->trb = NULL;
    }

    if (req->complete) {
        req->complete(pcd);
    }

    req->actual = 0;

    return error;
}

int usb3_ep_complete_request(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep, uint32_t event)
{
    usb3_pcd_req_t *req = &ep->req;
    usb3_dma_desc_t *desc = req->trb;
    uint32_t byte_count;
    int error = USB_NO_ERR;

    ep->send_zlp = 0;

    if (!desc) {
        return USB_PROCESS_ERR;
    }

    if (dwc_usb3_is_hwo(desc)) {
        return USB_PROCESS_ERR;
    }

    if (ep->is_in) {
        /* IN endpoint */
        if (usb3_get_xfercnt(desc) == 0) {
            req->actual += req->length;
        }

        /* Reset IN tri */
        ep->tri_in = 0;
        /* Complete the IN request */
        error = usb3_request_done(pcd, ep, req, 0);
    } else { /* OUT endpoint */
        byte_count = req->length - usb3_get_xfercnt(desc);
        req->actual += byte_count;
        req->bufdma += byte_count;
        pcd->file_received += byte_count;
        /* Reset OUT tri */
        ep->tri_out = 0;

        /* OUT transfer complete or not */
        if ((byte_count < ep->maxpacket) || (pcd->file_capacity <= pcd->file_received - FRAME_HEAD_LEN)) {
            /* Complete the OUT request */
            error = usb3_request_done(pcd, ep, req, 0);
        } else {
            usb3_bulk_out_continue_transfer((void *)pcd);
        }
    }

    return error;
}
