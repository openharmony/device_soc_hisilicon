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

#ifndef __USB3_H__
#define __USB3_H__

#include <types.h>

#define USB_PROC_TIME_OUT 3000
#define USB_VENDOR_ID 0x1D6B
#define USB_PRODUCT_ID 0xD001

#define STRING_LANGUAGE 0
#define STRING_MANUFACTURER 1
#define STRING_PRODUCT 2

#define USB_SPEED_UNKNOWN 0x00
#define USB_SPEED_LOW 0x01
#define USB_SPEED_FULL 0x02
#define USB_SPEED_HIGH 0x03
#define USB_SPEED_VARIABLE 0x04
#define USB_SPEED_SUPER 0x05

#define UDESC_DEVICE 0x01
#define UDESC_CONFIG 0x02
#define UDESC_STRING 0x03
#define UDESC_INTERFACE 0x04
#define UDESC_ENDPOINT 0x05
#define UDESC_SS_USB_COMPANION 0x30
#define UDESC_DEVICE_QUALIFIER 0x06
#define UDESC_BOS 0x0f
#define UDESC_DEVICE_CAPABILITY 0x10

#define ut_get_dir(a) ((a) & 0x80)
#define UT_WRITE 0x00
#define UT_READ 0x80

#define ut_get_type(a) ((a) & 0x60)
#define UT_STANDARD 0x00
#define UT_CLASS 0x20
#define UT_VENDOR 0x40

#define ut_get_recipient(a) ((a) & 0x1f)
#define UT_DEVICE 0x00
#define UT_INTERFACE 0x01
#define UT_ENDPOINT 0x02
#define UT_OTHER 0x03

#define UR_GET_STATUS 0x00
#define UR_CLEAR_FEATURE 0x01
#define UR_SET_FEATURE 0x03
#define UR_SET_ADDRESS 0x05
#define UR_GET_DESCRIPTOR 0x06
#define UR_SET_DESCRIPTOR 0x07
#define UR_GET_CONFIG 0x08
#define UR_SET_CONFIG 0x09
#define UR_GET_INTERFACE 0x0a
#define UR_SET_INTERFACE 0x0b
#define UR_SYNCH_FRAME 0x0c
#define UR_SET_SEL 0x30
#define UR_SET_ISOC_DELAY 0x31

/* Feature numbers */
#define UF_ENDPOINT_HALT 0
#define UF_DEVICE_REMOTE_WAKEUP 1
#define UF_TEST_MODE 2
#define UF_DEVICE_B_HNP_ENABLE 3
#define UF_DEVICE_A_HNP_SUPPORT 4
#define UF_DEVICE_A_ALT_HNP_SUPPORT 5
#define UF_FUNCTION_SUSPEND 0
#define UF_U1_ENABLE 48
#define UF_U2_ENABLE 49
#define UF_LTM_ENABLE 50

/* OTG feature selectors */
#define UOTG_B_HNP_ENABLE 3
#define UOTG_A_HNP_SUPPORT 4
#define UOTG_A_ALT_HNP_SUPPORT 5
#define UOTG_NTF_HOST_REL 51
#define UOTG_B3_RSP_ENABLE 52

#define ue_get_dir(a) ((a) & 0x80)
#define ue_set_dir(a, d) ((a) | (((d) & 1) << 7))
#define UE_DIR_IN 0x80
#define UE_DIR_OUT 0x00
#define UE_ADDR 0x0f
#define ue_get_addr(a) ((a) & UE_ADDR)

/* * Maxpacket size for EP0, defined by USB3 spec */
#define USB3_MAX_EP0_SIZE 512

/* * Maxpacket size for any EP, defined by USB3 spec */
#define USB3_MAX_PACKET_SIZE 1024
#define USB2_HS_MAX_PACKET_SIZE 512
#define USB2_FS_MAX_PACKET_SIZE 64

#define USB3_BULK_IN_EP 1
#define USB3_BULK_OUT_EP 1

#define BULK_EP_MAX_PACKET_SIZE 512

typedef struct usb_device_request {
    uint8_t bm_request_type;
    uint8_t b_request;
    uint16_t w_value;
    uint16_t w_index;
    uint16_t w_length;
} usb_device_request_t;

#pragma pack(1)
/* * USB_DT_DEVICE: Device descriptor */
typedef struct usb_device_descriptor {
    uint8_t b_length;
    uint8_t b_descriptor_type;

    uint16_t bcd_usb; // bcdUSB
#define USB_CLASS_COMM 0x02
#define USB_CLASS_VENDOR_SPEC 0xFF
#define USB_SC_VENDOR_SPEC 0xFF
#define USB_PR_VENDOR_SPEC 0xFF
    uint8_t b_device_class;
    uint8_t b_device_sub_class;
    uint8_t b_device_protocol;
    uint8_t b_max_packet_size0;
    uint16_t id_vendor;
    uint16_t id_product;
    uint16_t bcd_device;
    uint8_t i_manufacturer;
    uint8_t i_product;
    uint8_t i_serial_number;
    uint8_t b_num_configurations;
} usb_device_descriptor_t;

/* USB_DT_CONFIG: Config descriptor */
typedef struct usb_config_descriptor {
    uint8_t b_length;
    uint8_t b_descriptor_type;

    uint16_t w_total_length;
    uint8_t b_num_interfaces;
#define CONFIG_VALUE 1
    uint8_t b_configuration_value;
    uint8_t i_configuration;
#define USB_CONFIG_ATT_ONE (1 << 7)
    uint8_t bm_attributes;
#define USB_CONFIG_VBUS_DRAW (0xFA)
    uint8_t b_max_power;
} usb_config_descriptor_t;

/* USB_DT_DEVICE_QUALIFIER: Device Qualifier descriptor */
typedef struct usb_qualifier_descriptor {
    uint8_t b_length;
    uint8_t b_descriptor_type;

    uint16_t bcd_usb; // bcdUSB
    uint8_t b_device_class;
    uint8_t b_device_sub_class;
    uint8_t b_device_protocol;
    uint8_t b_max_packet_size0;
    uint8_t b_num_configurations;
    uint8_t b_reserved;
} usb_qualifier_descriptor_t;

/* USB_DT_INTERFACE: Interface descriptor */
typedef struct usb_interface_descriptor {
    uint8_t b_length;
    uint8_t b_descriptor_type;

    uint8_t b_interface_number;
    uint8_t b_alternate_setting;
    uint8_t b_num_endpoints;
    uint8_t b_interface_class;
    uint8_t b_interface_sub_class;
    uint8_t b_interface_protocol;
    uint8_t i_interface;
} usb_interface_descriptor_t;

/* USB_DT_ENDPOINT: Endpoint descriptor */
typedef struct usb_endpoint_descriptor {
    uint8_t b_length;
    uint8_t b_descriptor_type;

    uint8_t b_endpoint_address;
    uint8_t bm_attributes;
#define USB_ENDPOINT_XFER_CONTROL 0x00
#define USB_ENDPOINT_XFER_ISOC 0x01
#define USB_ENDPOINT_XFER_BULK 0x02
#define USB_ENDPOINT_XFER_INT 0x03
    uint16_t w_max_packet_size;
    uint8_t b_interval;
} usb_endpoint_descriptor_t;

/* USB_DT_SS_ENDPOINT_COMP: SuperSpeed Endpoint Companion descriptor */
typedef struct usb_ss_ep_comp_descriptor {
    uint8_t b_length;
    uint8_t b_descriptor_type;

    uint8_t b_max_burst;
    uint8_t bm_attributes;
    uint16_t w_bytes_per_interval;
} usb_ss_ep_comp_descriptor_t;

/* WUSB BOS Descriptor (Binary device Object Store) */
typedef struct wusb_bos_desc {
    uint8_t b_length;
    uint8_t b_descriptor_type;
    uint16_t w_total_length;
    uint8_t b_num_device_caps;
} wusb_bos_desc_t;

#define USB_DEVICE_CAPABILITY_20_EXTENSION 0x02
typedef struct usb_dev_cap_20_ext_desc {
    uint8_t b_length;
    uint8_t b_descriptor_type;
    uint8_t b_dev_capability_type;
#define USB_20_EXT_LPM 0x02
    uint32_t bm_attributes;
} usb_dev_cap_20_ext_desc_t;

#define USB_DEVICE_CAPABILITY_SS_USB 0x03
typedef struct usb_dev_cap_ss_usb {
    uint8_t b_length;
    uint8_t b_descriptor_type;
    uint8_t b_dev_capability_type;
#define USB_DC_SS_USB_LTM_CAPABLE 0x02
    uint8_t bm_attributes;
#define USB_DC_SS_USB_SPEED_SUPPORT_LOW 0x01
#define USB_DC_SS_USB_SPEED_SUPPORT_FULL 0x02
#define USB_DC_SS_USB_SPEED_SUPPORT_HIGH 0x04
#define USB_DC_SS_USB_SPEED_SUPPORT_SS 0x08
    uint32_t w_speeds_supported;
    uint8_t b_functionality_support;
    uint8_t b_u1_dev_exit_lat;
    uint32_t w_u2_dev_exit_lat;
} usb_dev_cap_ss_usb_t;

#define USB_DEVICE_CAPABILITY_CONTAINER_ID 0x04
typedef struct usb_dev_cap_container_id {
    uint8_t b_length;
    uint8_t b_descriptor_type;
    uint8_t b_dev_capability_type;
    uint8_t b_reserved;
    uint8_t container_id[16];
} usb_dev_cap_container_id_t;
#pragma pack()

bool self_usb_check();

#endif /* __USB3_H__ */
