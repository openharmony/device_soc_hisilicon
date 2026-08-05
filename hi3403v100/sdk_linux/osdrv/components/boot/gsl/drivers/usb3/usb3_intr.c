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

#include "usb3_hw.h"
#include "usb3_drv.h"
#include "usb3_pcd.h"
extern void usb3_handle_dev_intr(usb3_pcd_t *pcd, uint32_t event);
extern int usb3_handle_ep_intr(usb3_pcd_t *pcd, int physep, uint32_t event);

void ena_eventbuf_intr(const usb3_device_t *dev)
{
    uint32_t eventsiz;

    eventsiz = usb3_rd32(&dev->core_global_regs->geventbuf[0].geventsiz);
    eventsiz &= ~USB3_EVENTSIZ_INT_MSK_BIT;
    usb3_wr32(&dev->core_global_regs->geventbuf[0].geventsiz, eventsiz);
}

void dis_eventbuf_intr(const usb3_device_t *dev)
{
    uint32_t eventsiz;

    eventsiz = usb3_rd32(&dev->core_global_regs->geventbuf[0].geventsiz);
    eventsiz |= USB3_EVENTSIZ_INT_MSK_BIT;
    usb3_wr32(&dev->core_global_regs->geventbuf[0].geventsiz, eventsiz);
}

void usb3_dis_flush_eventbuf_intr(usb3_device_t *dev)
{
    uint32_t cnt;
    uint32_t gevntsize;

    dis_eventbuf_intr(dev);
    cnt = usb3_rd32(&dev->core_global_regs->geventbuf[0].geventcnt);
    usb3_wr32(&dev->core_global_regs->geventbuf[0].geventcnt, cnt);

    if (cnt != 0) {
        gevntsize = sizeof(dev->event_buf[0]) * USB3_EVENT_BUF_SIZE;
        usb_info("evnt count 0x%x, evnt buf size 0x%x\n", cnt, gevntsize);
        dev->event_ptr += cnt % gevntsize;
    }
}

int get_eventbuf_count(const usb3_device_t *dev)
{
    uint32_t cnt;

    cnt = usb3_rd32(&dev->core_global_regs->geventbuf[0].geventcnt);

    return cnt & USB3_EVENTCNT_CNT_BITS;
}

void update_eventbuf_count(const usb3_device_t *dev, int cnt)
{
    usb3_wr32(&dev->core_global_regs->geventbuf[0].geventcnt, cnt);
}

uint32_t get_eventbuf_event(usb3_device_t *dev, int size)
{
    uint32_t event;

    event = *dev->event_ptr++;

    if (dev->event_ptr >= dev->event_buf + size) {
        dev->event_ptr = dev->event_buf;
    }
    return event;
}

void usb3_init_eventbuf(const usb3_device_t *dev, int size, uint32_t dma_addr)
{
    dma_addr = map_to_dma_addr(dma_addr);

    usb3_wr32(&dev->core_global_regs->geventbuf[0].geventadr_low, dma_addr & 0xffffffff);

    usb3_wr32(&dev->core_global_regs->geventbuf[0].geventadr_high, 0);
    usb3_wr32(&dev->core_global_regs->geventbuf[0].geventsiz, size << 2); // bit 2

    usb3_wr32(&dev->core_global_regs->geventbuf[0].geventcnt, 0);
}

void usb3_enable_device_interrupts(usb3_device_t *dev)
{
    uint32_t eventsiz;

    /* Clear any pending interrupts */
    usb3_dis_flush_eventbuf_intr(dev);
    /* *
     * This routine enables the Event Buffer interrupt.
     */
    eventsiz = usb3_rd32(&dev->core_global_regs->geventbuf[0].geventsiz);
    eventsiz &= ~USB3_EVENTSIZ_INT_MSK_BIT;
    usb3_wr32(&dev->core_global_regs->geventbuf[0].geventsiz, eventsiz);
    /* Enable device interrupts */
    usb3_wr32(&dev->pcd.dev_global_regs->devten, USB3_DEVTEN_DISCONN_BIT | USB3_DEVTEN_CONNDONE_BIT |
          USB3_DEVTEN_USBRESET_BIT | USB3_DEVTEN_BER_REQ_EVT_BIT | USB3_DEVTEN_WKUP_BIT |
          USB3_DEVTEN_EOPF_BIT);
}

void usb3_handle_event(usb3_device_t *dev)
{
    usb3_pcd_t *pcd = &dev->pcd;
    uint32_t event;
    int count;
    int intr;
    int physep;
    int i;

    count = get_eventbuf_count(dev);
    if ((count & USB3_EVENTCNT_CNT_BITS) == USB3_EVENTCNT_CNT_BITS ||
        count >= USB3_EVENT_BUF_SIZE * 4) { // 4 bytes per event
        update_eventbuf_count(dev, count);
        count = 0;
    }

    if (count < 0) {
        count = 0;
    }

    for (i = 0; i < count; i += 4) { // 4 bytes per event
        event = get_eventbuf_event(dev, USB3_EVENT_BUF_SIZE);
        update_eventbuf_count(dev, 4); // 4 bytes per event
        if (event == 0) {
            /* Ignore null events */
            continue;
        }

        if (event & USB3_EVENT_NON_EP_BIT) {
            intr = event & USB3_EVENT_INTTYPE_BITS;
            if (intr == (USB3_EVENT_DEV_INT << USB3_EVENT_INTTYPE_SHIFT)) {
                usb3_handle_dev_intr(pcd, event);
            }
        } else {
            physep = (event >> USB3_DEPEVT_EPNUM_SHIFT) & (USB3_DEPEVT_EPNUM_BITS >> USB3_DEPEVT_EPNUM_SHIFT);
            (void)usb3_handle_ep_intr(pcd, physep, event);
        }
    }
}
