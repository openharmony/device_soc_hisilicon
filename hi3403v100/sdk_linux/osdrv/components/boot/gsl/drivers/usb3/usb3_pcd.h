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

#ifndef __USB3_PCD_H__
#define __USB3_PCD_H__

#include "usb3_drv.h"
usb3_pcd_ep_t *usb3_get_out_ep(usb3_pcd_t *pcd, uint32_t ep_num);
usb3_pcd_ep_t *usb3_get_in_ep(usb3_pcd_t *pcd, uint32_t ep_num);
usb3_pcd_ep_t *usb3_get_ep_by_addr(usb3_pcd_t *pcd, uint16_t index);
void usb3_ep_clear_stall(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep);
uint32_t usb3_get_device_speed(usb3_pcd_t *pcd);
void usb3_pcd_set_speed(usb3_pcd_t *pcd, int speed);
void usb3_ep_set_stall(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep);
void usb3_bulk_in_transfer(void *dev, char *status);
int usb3_bulk_out_transfer_cmd(void *dev);
int usb3_bulk_out_transfer_data(void *dev);
int usb3_bulk_out_transfer(void *dev);
int usb3_ep_complete_request(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep, uint32_t event);
int usb3_handle_ep_intr(usb3_pcd_t *pcd, int physep, uint32_t event);
void usb3_handle_dev_intr(usb3_pcd_t *pcd, uint32_t event);
void usb3_os_handle_ep0(usb3_pcd_t *pcd, uint32_t event);
void usb3_ep_start_transfer(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep);
bool usb3_driver_init(void);
void udc_disconnect(void);
#endif /* __USB3_PCD_H__ */
