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
#include "sys.h"
#include <common.h>
#include <platform.h>
#include <lib.h>

int usb3_handle_event(usb3_device_t *dev);
void usb3_enable_device_interrupts(usb3_device_t *dev);
void usb3_dis_flush_eventbuf_intr(usb3_device_t *dev);
void usb3_init_eventbuf(usb3_device_t *dev, int size, uint32_t dma_addr);
static usb3_device_t *usb3_dev_ex = NULL;
static void usb_io_udelay(void);

uint32_t usb3_rd32(const volatile uint32_t *addr)
{
    return *(volatile uint32_t *)(addr);
}

void usb3_wr32(volatile uint32_t *addr, uint32_t val)
{
    *(volatile uint32_t *)(addr) = val;

    usb_io_udelay();
}

uint32_t usb3_is_host_mode(usb3_device_t *dev)
{
    return usb3_rd32(&dev->core_global_regs->gsts) & 0x01;
}

void pcd_epinit(usb3_pcd_t *pcd)
{
    usb3_pcd_ep_t *ep;

    /* Init EP0 */
    do {
        ep = &pcd->ep0;

        ep->pcd = pcd;
        ep->stopped = 1;
        ep->out_ep_reg = &pcd->out_ep_regs[0];
        ep->in_ep_reg = &pcd->in_ep_regs[0];
        ep->type = USB3_EP_TYPE_CONTROL;
        ep->maxpacket = 64; // ep0 max packet size is 64 bytes
    } while (0);

    /* Init EP1-OUT */
    do {
        ep = &pcd->out_ep;

        ep->pcd = pcd;
        ep->stopped = 1;
        ep->phys = USB3_BULK_OUT_EP << 1;
        ep->num = 1;
        ep->out_ep_reg = &pcd->out_ep_regs[USB3_BULK_OUT_EP];

        /* Bulk EP is activated */
        ep->type = USB3_EP_TYPE_BULK;
        ep->maxpacket = USB2_FS_MAX_PACKET_SIZE;
    } while (0);

    /* Init EP1-IN */
    do {
        ep = &pcd->in_ep;

        ep->pcd = pcd;
        ep->stopped = 1;
        ep->is_in = 1;
        ep->phys = (USB3_BULK_IN_EP << 1) | 1;
        ep->num = 1;
        ep->tx_fifo_num = USB3_BULK_IN_EP;
        ep->in_ep_reg = &pcd->in_ep_regs[USB3_BULK_IN_EP];

        /* Bulk EP is activated */
        ep->type = USB3_EP_TYPE_BULK;
        ep->maxpacket = USB2_FS_MAX_PACKET_SIZE;
    } while (0);

    pcd->ep0state = EP0_IDLE;
}

void usb3_set_address(const usb3_pcd_t *pcd, uint32_t addr)
{
    uint32_t dcfg;

    dcfg = usb3_rd32(&pcd->dev_global_regs->dcfg);
    dcfg &= ~USB3_DCFG_DEVADDR_BITS;
    dcfg |= addr << USB3_DCFG_DEVADDR_SHIFT;
    usb3_wr32(&pcd->dev_global_regs->dcfg, dcfg);
}

#define RAM_WIDTH 8
#define RAM_RX_DEPTH 4096
#define RAM_TX0_DEPTH 1024
#define RAM_TX1_DEPTH 2048
#define USB3_START_MAX_RETRIES 3
void usb3_set_tx_fifo_size(usb3_device_t *dev)
{
    usb3_core_global_regs_t *global_regs = dev->core_global_regs;
    uint32_t prev_start = 0;
    /* Set 1K for tx fifo0 */
    usb3_wr32(&global_regs->gtxfifosiz[0],
        ((RAM_TX0_DEPTH / RAM_WIDTH) << USB3_FIFOSZ_DEPTH_SHIFT) | (prev_start <<
                  USB3_FIFOSZ_STARTADDR_SHIFT));

    prev_start += RAM_TX0_DEPTH / RAM_WIDTH;
    /* Set 2K for tx fifo1 */
    usb3_wr32(&global_regs->gtxfifosiz[1],
        ((RAM_TX1_DEPTH / RAM_WIDTH) << USB3_FIFOSZ_DEPTH_SHIFT) | (prev_start <<
                  USB3_FIFOSZ_STARTADDR_SHIFT));
}

void usb3_set_rx_fifo_size(usb3_device_t *dev)
{
    usb3_core_global_regs_t *global_regs = dev->core_global_regs;
    /* Set 4K for rx fifo */
    usb3_wr32(&global_regs->grxfifosiz[0], ((RAM_RX_DEPTH / RAM_WIDTH) << USB3_FIFOSZ_DEPTH_SHIFT));
}

void usb3_resume_usb2_phy(const usb3_pcd_t *pcd)
{
    uint32_t usb2phycfg;

    usb2phycfg = usb3_rd32(&pcd->usb3_dev->core_global_regs->gusb2phycfg[0]);
    usb2phycfg |= USB3_USB2PHYCFG_SUS_PHY_BIT;
    usb3_wr32(&pcd->usb3_dev->core_global_regs->gusb2phycfg[0], usb2phycfg);
}

void usb3_resume_usb3_phy(const usb3_pcd_t *pcd)
{
    uint32_t pipectl;

    pipectl = usb3_rd32(&pcd->usb3_dev->core_global_regs->gusb3pipectl[0]);
    pipectl |= USB3_PIPECTL_SUS_PHY_BIT;
    usb3_wr32(&pcd->usb3_dev->core_global_regs->gusb3pipectl[0], pipectl);
}

void usb3_accept_u1(usb3_pcd_t *pcd)
{
    uint32_t dctl;

    dctl = usb3_rd32(&pcd->dev_global_regs->dctl);
    dctl |= USB3_DCTL_ACCEPT_U1_EN_BIT;
    usb3_wr32(&pcd->dev_global_regs->dctl, dctl);
}

void usb3_accept_u2(const usb3_pcd_t *pcd)
{
    return;
}

uint32_t usb3_u1_enabled(usb3_pcd_t *pcd)
{
    uint32_t dctl;

    dctl = usb3_rd32(&pcd->dev_global_regs->dctl);
    return !!(dctl & USB3_DCTL_INIT_U1_EN_BIT);
}

uint32_t usb3_u2_enabled(usb3_pcd_t *pcd)
{
    uint32_t dctl;

    dctl = usb3_rd32(&pcd->dev_global_regs->dctl);
    return !!(dctl & USB3_DCTL_INIT_U2_EN_BIT);
}

void usb3_dep_cstall(const usb3_pcd_t *pcd, usb3_dev_ep_regs_t *ep_reg)
{
    /* Start the command */
    usb3_wr32(&ep_reg->depcmd, USB3_EPCMD_CLR_STALL | USB3_EPCMD_ACT_BIT);

    /* Wait for command completion */
    handshake(pcd->usb3_dev, &ep_reg->depcmd, USB3_EPCMD_ACT_BIT, 0);
}

void usb3_dep_sstall(const usb3_pcd_t *pcd, usb3_dev_ep_regs_t *ep_reg)
{
    /* Start the command */
    usb3_wr32(&ep_reg->depcmd, USB3_EPCMD_SET_STALL | USB3_EPCMD_ACT_BIT);

    /* Wait for command completion */
    handshake(pcd->usb3_dev, &ep_reg->depcmd, USB3_EPCMD_ACT_BIT, 0);
}

uint32_t handshake(const usb3_device_t *dev, const volatile uint32_t *ptr, uint32_t mask, uint32_t done)
{
    uint32_t usec = 10000;
    uint32_t result;

    do {
        result = usb3_rd32(ptr);
        if ((result & mask) == done) {
            return 1;
        }

        udelay(100); // delay 100us
        usec -= 1;
    } while (usec > 0);

    return 0;
}

void usb3_fill_desc(usb3_dma_desc_t *desc, uint32_t dma_addr, uint32_t dma_len, uint32_t stream,
            uint32_t type,
            uint32_t ctrlbits, int own)
{
    dma_addr = map_to_dma_addr(dma_addr);

    desc->bptl = (uint32_t)(dma_addr & 0xffffffffU);
    desc->bpth = 0;
    desc->status = dma_len << USB3_DSCSTS_XFRCNT_SHIFT;

    /* Note: If type is 0, leave original control bits intact (for isoc) */
    if (type)
        desc->control = type << USB3_DSCCTL_TRBCTL_SHIFT;

    desc->control |= (stream << USB3_DSCCTL_STRMID_SOFN_SHIFT) | ctrlbits;

    /* Must do this last! */
    if (own) {
        desc->control |= USB3_DSCCTL_HWO_BIT;
    }
}

void usb3_dep_startnewcfg(const usb3_pcd_t *pcd, usb3_dev_ep_regs_t *ep_reg, uint32_t rsrcidx)
{
    /* Start the command */
    usb3_wr32(&ep_reg->depcmd,
        (rsrcidx << USB3_EPCMD_XFER_RSRC_IDX_SHIFT) | USB3_EPCMD_START_NEW_CFG | USB3_EPCMD_ACT_BIT);

    /* Wait for command completion */
    handshake(pcd->usb3_dev, &ep_reg->depcmd, USB3_EPCMD_ACT_BIT, 0);
}

void usb3_dep_cfg(const usb3_pcd_t *pcd, usb3_dev_ep_regs_t *ep_reg, uint32_t depcfg0, uint32_t depcfg1,
                  uint32_t depcfg2)
{
    /* Set param 2 */
    usb3_wr32(&ep_reg->depcmdpar2, depcfg2);

    /* Set param 1 */
    usb3_wr32(&ep_reg->depcmdpar1, depcfg1);

    /* Set param 0 */
    usb3_wr32(&ep_reg->depcmdpar0, depcfg0); /* bspmm 0x1018c808 0x200 */

    /* Start the command */
    usb3_wr32(&ep_reg->depcmd, USB3_EPCMD_SET_EP_CFG | USB3_EPCMD_ACT_BIT);

    /* Wait for command completion */
    handshake(pcd->usb3_dev, &ep_reg->depcmd, USB3_EPCMD_ACT_BIT, 0);
}

void usb3_dep_xfercfg(const usb3_pcd_t *pcd, usb3_dev_ep_regs_t *ep_reg, uint32_t depstrmcfg)
{
    /* Set param 0 */
    usb3_wr32(&ep_reg->depcmdpar0, depstrmcfg);

    /* Start the command */
    usb3_wr32(&ep_reg->depcmd, USB3_EPCMD_SET_XFER_CFG | USB3_EPCMD_ACT_BIT);

    /* Wait for command completion */
    handshake(pcd->usb3_dev, &ep_reg->depcmd, USB3_EPCMD_ACT_BIT, 0);
}

uint8_t usb3_dep_startxfer(const usb3_pcd_t *pcd, usb3_dev_ep_regs_t *ep_reg, uint32_t dma_addr,
    uint32_t stream_or_uf)
{
    uint32_t depcmd;

    dma_addr = map_to_dma_addr(dma_addr);

    /* Set param 1 */
    usb3_wr32(&ep_reg->depcmdpar1, dma_addr & 0xffffffffU);

    /* Set param 0 */
    usb3_wr32(&ep_reg->depcmdpar0, 0);

    usb3_wr32(&ep_reg->depcmd,
        (stream_or_uf << USB3_EPCMD_STR_NUM_OR_UF_SHIFT) | USB3_EPCMD_START_XFER | USB3_EPCMD_ACT_BIT);

    /* Wait for command completion */
    handshake(pcd->usb3_dev, &ep_reg->depcmd, USB3_EPCMD_ACT_BIT, 0);

    depcmd = usb3_rd32(&ep_reg->depcmd);

    return (depcmd >> USB3_EPCMD_XFER_RSRC_IDX_SHIFT) &
           (USB3_EPCMD_XFER_RSRC_IDX_BITS >> USB3_EPCMD_XFER_RSRC_IDX_SHIFT);
}

void usb3_dep_updatexfer(const usb3_pcd_t *pcd, usb3_dev_ep_regs_t *ep_reg, uint32_t tri)
{
    /* Start the command */
    usb3_wr32(&ep_reg->depcmd, (tri << USB3_EPCMD_XFER_RSRC_IDX_SHIFT) | USB3_EPCMD_UPDATE_XFER |
          USB3_EPCMD_ACT_BIT);

    /* Wait for command completion */
    handshake(pcd->usb3_dev, &ep_reg->depcmd, USB3_EPCMD_ACT_BIT, 0);
}

void usb3_enable_ep(usb3_pcd_t *pcd, const usb3_pcd_ep_t *ep)
{
    uint32_t ep_index_num;
    uint32_t dalepena;
    ep_index_num = ep->num * 2; // 2 = 1in + 1out

    if (ep->is_in) {
        ep_index_num += 1;
    }

    dalepena = usb3_rd32(&pcd->dev_global_regs->dalepena);
    /* If we have already enabled this EP, leave it alone
     * (shouldn't happen)
     */
    if (dalepena & (1 << ep_index_num)) {
        return;
    }

    dalepena |= 1 << ep_index_num;
    usb3_wr32(&pcd->dev_global_regs->dalepena, dalepena);
    return;
}

void usb3_dis_usb2_suspend(usb3_pcd_t *pcd)
{
    uint32_t usb2phycfg;
    if (pcd->speed == USB_SPEED_SUPER) {
        return;
    }

    usb2phycfg = usb3_rd32(&pcd->usb3_dev->core_global_regs->gusb2phycfg[0]);
    usb2phycfg &= ~USB3_USB2PHYCFG_SUS_PHY_BIT;
    usb2phycfg &= ~USB3_USB2PHYCFG_ENBL_SLP_M_BIT;
    usb3_wr32(&pcd->usb3_dev->core_global_regs->gusb2phycfg[0], usb2phycfg);
}

void usb3_ep0_activate(usb3_pcd_t *pcd)
{
    uint32_t diepcfg0;
    uint32_t doepcfg0;
    uint32_t diepcfg1;
    uint32_t doepcfg1;
    uint32_t diepcfg2 = 0;
    uint32_t doepcfg2 = 0;
    usb3_dev_ep_regs_t *ep_reg;

    diepcfg0 = USB3_EP_TYPE_CONTROL << USB3_EPCFG0_EPTYPE_SHIFT;
    diepcfg1 =
        USB3_EPCFG1_XFER_CMPL_BIT | USB3_EPCFG1_XFER_IN_PROG_BIT | USB3_EPCFG1_XFER_NRDY_BIT |
        USB3_EPCFG1_EP_DIR_BIT;

    doepcfg0 = USB3_EP_TYPE_CONTROL << USB3_EPCFG0_EPTYPE_SHIFT;
    doepcfg1 = USB3_EPCFG1_XFER_CMPL_BIT | USB3_EPCFG1_XFER_IN_PROG_BIT | USB3_EPCFG1_XFER_NRDY_BIT;

    /* Default to MPS of 512 (will reconfigure after ConnectDone event) */
    diepcfg0 |= (512 << USB3_EPCFG0_MPS_SHIFT); // 512 is maxpacketsize of bulk ep
    doepcfg0 |= (512 << USB3_EPCFG0_MPS_SHIFT); // 512 is maxpacketsize of bulk ep

    diepcfg0 |= (pcd->ep0.tx_fifo_num << USB3_EPCFG0_TXFNUM_SHIFT);

    /* Issue "DEPCFG" command to EP0-OUT */

    ep_reg = &pcd->out_ep_regs[0];
    usb3_dis_usb2_suspend(pcd); /* bspmm 0x1018c200 0x102400 */
    /* If core is version 1.09a or later */
    /* Must issue DEPSTRTNEWCFG command first */
    usb3_dep_startnewcfg(pcd, ep_reg, 0);

    usb3_dep_cfg(pcd, ep_reg, doepcfg0, doepcfg1, doepcfg2);

    /* Issue "DEPSTRMCFG" command to EP0-OUT */

    /* One stream */
    usb3_dep_xfercfg(pcd, ep_reg, 1);

    /* Issue "DEPCFG" command to EP0-IN */

    ep_reg = &pcd->in_ep_regs[0];
    usb3_dep_cfg(pcd, ep_reg, diepcfg0, diepcfg1, diepcfg2);

    /* Issue "DEPSTRMCFG" command to EP0-IN */

    /* One stream */
    usb3_dep_xfercfg(pcd, ep_reg, 1);

    pcd->ep0.active = 1;
}

void usb3_ep_activate(usb3_pcd_t *pcd, usb3_pcd_ep_t *ep)
{
    usb3_dev_ep_regs_t *ep_reg, *ep0_reg;
    uint32_t depcfg0;
    uint32_t depcfg1;
    uint32_t depcfg2 = 0;

    /*
     * Get the appropriate EP registers
     */
    if (ep->is_in) {
        ep_reg = ep->in_ep_reg;
    } else {
        ep_reg = ep->out_ep_reg;
    }

    /* If this is first EP enable (ie. start of a new configuration) */
    if (!pcd->eps_enabled) {
        pcd->eps_enabled = 1;

        /* NOTE: When setting a new configuration, we must issue a
         * "DEPCFG" command to physical EP1 (logical EP0-IN) first.
         * This resets the core's Tx FIFO mapping table
         */
        depcfg0 = USB3_EP_TYPE_CONTROL << USB3_EPCFG0_EPTYPE_SHIFT;
        depcfg0 |= USB3_CFG_ACTION_MODIFY << USB3_EPCFG0_CFG_ACTION_SHIFT;
        depcfg1 = USB3_EPCFG1_XFER_CMPL_BIT | USB3_EPCFG1_XFER_NRDY_BIT | USB3_EPCFG1_EP_DIR_BIT;

        switch (pcd->speed) {
            case USB_SPEED_HIGH:
            case USB_SPEED_FULL:
                depcfg0 |= 64 << USB3_EPCFG0_MPS_SHIFT; // 64 is maxpacketsize of ctrl ep
                break;

            case USB_SPEED_LOW:
                depcfg0 |= 8 << USB3_EPCFG0_MPS_SHIFT; // 8 is maxpacketsize of ctrl ep
                break;
            default:
                break;
        }

        ep0_reg = &pcd->in_ep_regs[0];
        usb3_dep_cfg(pcd, ep0_reg, depcfg0, depcfg1, 0);

        /* If core is version 1.09a or later */
        /* Must issue DEPSTRTNEWCFG command first */
        ep0_reg = &pcd->out_ep_regs[0];
        usb3_dep_startnewcfg(pcd, ep0_reg, 2); // resourceIdx is 2
    }

    /*
     * Issue "DEPCFG" command to EP
     */
    depcfg0 = ep->type << USB3_EPCFG0_EPTYPE_SHIFT;
    depcfg0 |= ep->maxpacket << USB3_EPCFG0_MPS_SHIFT;

    if (ep->is_in) {
        depcfg0 |= ep->tx_fifo_num << USB3_EPCFG0_TXFNUM_SHIFT;
    }

    depcfg0 |= ep->maxburst << USB3_EPCFG0_BRSTSIZ_SHIFT;

    depcfg1 = ep->num << USB3_EPCFG1_EP_NUM_SHIFT;

    if (ep->is_in) {
        depcfg1 |= USB3_EPCFG1_EP_DIR_BIT;
    }

    depcfg1 |= USB3_EPCFG1_XFER_CMPL_BIT;

    usb3_dep_cfg(pcd, ep_reg, depcfg0, depcfg1, depcfg2);

    /*
     * Issue "DEPSTRMCFG" command to EP
     */
    /* Setting 1 stream resource */
    usb3_dep_xfercfg(pcd, ep_reg, 1);

    /* Enable EP in DALEPENA reg */
    usb3_enable_ep(pcd, ep);

    ep->active = 1;
}

void usb3_ep0_out_start(usb3_pcd_t *pcd)
{
    usb3_dev_ep_regs_t *ep_reg;
    usb3_dma_desc_t *desc;
    uint32_t desc_dma;
    uint8_t tri;

    /* Get the SETUP packet DMA Descriptor (TRB) */
    desc = pcd->ep0_setup_desc;
    desc_dma = (uint32_t)(long)pcd->ep0_setup_desc;

    /* DMA Descriptor setup */
    usb3_fill_desc(desc, (uint32_t)(long)pcd->ep0_setup_pkt, pcd->ep0.maxpacket, 0,
        USB3_DSCCTL_TRBCTL_SETUP,
               USB3_DSCCTL_IOC_BIT | USB3_DSCCTL_ISP_BIT | USB3_DSCCTL_LST_BIT, 1);

    ep_reg = &pcd->out_ep_regs[0];

    /* Issue "DEPSTRTXFER" command to EP0-OUT */
    tri = usb3_dep_startxfer(pcd, ep_reg, desc_dma, 0);
    pcd->ep0.tri_out = tri;
}

void usb3_core_dev_init(usb3_device_t *dev)
{
    usb3_core_global_regs_t *global_regs = dev->core_global_regs;
    usb3_pcd_t *pcd = &dev->pcd;
    usb3_dev_global_regs_t *dev_global_regs = pcd->dev_global_regs;
    uint32_t temp_t;

    do {
        temp_t = usb3_rd32(&dev_global_regs->dctl);
        temp_t &= ~USB3_DCTL_RUN_STOP_BIT;
        temp_t |= USB3_DCTL_CSFT_RST_BIT;
        usb3_wr32(&dev_global_regs->dctl, temp_t);
        do {
            udelay(100); // delay 100us
            temp_t = usb3_rd32(&dev_global_regs->dctl);
        } while (temp_t &USB3_DCTL_CSFT_RST_BIT);

        /* Wait for at least 3 PHY clocks */
        mdelay(1);
    } while (0);

    pcd->link_state = 0;

    /* Set Turnaround Time = 9 (8-bit UTMI+ / ULPI) */
    temp_t = usb3_rd32(&global_regs->gusb2phycfg[0]);
    temp_t &= ~USB3_USB2PHYCFG_USB_TRD_TIM_BITS;
    temp_t |= 9 << USB3_USB2PHYCFG_USB_TRD_TIM_SHIFT; // Set Turnaround Time = 9
    usb3_wr32(&global_regs->gusb2phycfg[0], temp_t);

#ifdef USB3_SYNOPSYS_PHY /* SYNOPSYS PHY */
    /* Set LFPS filter */
    usb3_wr32(&global_regs->gusb3pipectl[0],
        USB3_PIPECTL_LFPS_FILTER_BIT | (1 << USB3_PIPECTL_TX_DEMPH_SHIFT));

    /* Special requirements for Austin */
    temp_t = usb3_rd32(&pcd->usb3_dev->core_global_regs->gusb3pipectl[0]);
    usb3_wr32(&global_regs->gusb3pipectl[0], temp_t);
#endif

    temp_t = 0x13802004;
    usb3_wr32(&global_regs->gctl, temp_t);

    usb_info("evnt buffer addr: 0x%x\n", dev->event_buf);

    usb3_init_eventbuf(dev, USB3_EVENT_BUF_SIZE, (uint32_t)(long)dev->event_buf);
    dev->event_ptr = dev->event_buf;

    /* Set speed to Super */
    temp_t = usb3_rd32(&pcd->dev_global_regs->dcfg);
    temp_t &= ~(USB3_DCFG_DEVSPD_BITS << USB3_DCFG_DEVSPD_SHIFT);
    temp_t |= USB3_SPEED_HS_PHY_30MHZ_OR_60MHZ << USB3_DCFG_DEVSPD_SHIFT;
    usb3_wr32(&pcd->dev_global_regs->dcfg, temp_t);

    /* If LPM enable was requested */
    temp_t = usb3_rd32(&pcd->dev_global_regs->dcfg);
    temp_t |= USB3_DCFG_LPM_CAP_BIT;
    usb3_wr32(&pcd->dev_global_regs->dcfg, temp_t); /* bspmm 0x1018c700 0x480804 */

    /* Set Nump */
    temp_t = usb3_rd32(&pcd->dev_global_regs->dcfg);
    temp_t &= ~USB3_DCFG_NUM_RCV_BUF_BITS;
    temp_t |= 16 << USB3_DCFG_NUM_RCV_BUF_SHIFT; // Nump == 16
    usb3_wr32(&pcd->dev_global_regs->dcfg, temp_t);

    usb3_set_address(pcd, 0);

    /* disable Phy suspend */
    usb3_resume_usb3_phy(pcd);
    usb3_resume_usb2_phy(pcd); /* bspmm 0x1018c200 0x102540 */
    /* Enable Global and Device interrupts */
    usb3_enable_device_interrupts(dev);

    /* Activate EP0 */
    usb3_ep0_activate(pcd);

    /* Start EP0 to receive SETUP packets */
    usb3_ep0_out_start(pcd);

    /* Enable EP0-OUT/IN in DALEPENA register */
    usb3_wr32(&pcd->dev_global_regs->dalepena, 3); // 3 == 2'b11

    /* Set Run/Stop bit */
    temp_t = usb3_rd32(&pcd->dev_global_regs->dctl);
    temp_t |= USB3_DCTL_RUN_STOP_BIT;
    usb3_wr32(&pcd->dev_global_regs->dctl, temp_t);
}

void usb3_pcd_init(usb3_device_t *dev)
{
    usb3_pcd_t *pcd = &dev->pcd;

    pcd->usb3_dev = dev;
    pcd->speed = USB_SPEED_UNKNOWN;

    /* Initialize EP structures */
    pcd_epinit(pcd);

    /* Initialize the Core (also enables interrupts and sets Run/Stop bit) */
    usb3_core_dev_init(dev);
}

void usb3_common_init(usb3_device_t *dev, const volatile uint8_t *base)
{
    usb3_pcd_t *pcd;

    dev->core_global_regs = (usb3_core_global_regs_t *)(base + USB3_CORE_GLOBAL_REG_OFFSET);

    pcd = &dev->pcd;

    pcd->dev_global_regs = (usb3_dev_global_regs_t *)(base + USB3_DEV_GLOBAL_REG_OFFSET);
    pcd->out_ep_regs = (usb3_dev_ep_regs_t *)(base + USB3_DEV_OUT_EP_REG_OFFSET);
    pcd->in_ep_regs = (usb3_dev_ep_regs_t *)(base + USB3_DEV_IN_EP_REG_OFFSET);
}

void usb3_init(usb3_device_t *dev)
{
    /* Init the PCD (also enables interrupts and sets Run/Stop bit) */
    usb3_pcd_init(dev);
}

/* Vbus IO register */
#define VBUS_IO_REG 0x10230038
#define VBUS_IO_DEFAULT_VAL 0x1200
#define USB_VBUS_ON (0x1 << 0)

/* USB PHY base register */
#define USB2_1_PHY_BASE_ADDR 0x10330000
#define USB2_1_PHY_TRIM_ADDR_OFFSET 0x8
#define USB2_1_PHY_TRIM_MASK (0x1f << 8)
#define usb2_1_phy_trim_val(a) (((a) << 8) & USB2_1_PHY_TRIM_MASK)
#define USB2_1_PHY_CURRENT_BIAS_ADDR_OFFSET 0x0
#define USB2_1_PHY_CURRENT_BIAS_MASK (0xf)
#define USB2_1_PHY_CURRENT_BIAS_1DOT25 (0x3)
#define USB2_1_PHY_DISCONNECT_ADDR_OFFSET (0x8)
#define USB2_1_PHY_DISCONNECT_MASK (0x00070000)
#define USB2_1_PHY_DISCONNECT_620MV ((0x6 << 16) & USB2_1_PHY_DISCONNECT_MASK)
#define USB2_1_PHY_DISCONNECT_640MV ((0x7 << 16) & USB2_1_PHY_DISCONNECT_MASK)
#define USB2_1_PHY_ATOP_TEST_ADDR_OFFSET (0xc)
#define USB2_1_PHY_ATOP_TEST_EOP_DOWN (0x1 << 31)
#define USB2_1_PHY_ATOP_TEST_BG (0x1 << 27)
#define USB2_1_PHY_ATOP_TEST_1V8 (0x1 << 26)
#define USB2_1_PHY_TX_REF_VOL_ADDR_OFFSET (0x10)
#define USB2_1_PHY_TX_REF_VOL_MASK (0x00000070)
#define USB2_1_PHY_TX_REF_VOL_410MV ((0x3 << 4) & USB2_1_PHY_TX_REF_VOL_MASK)
#define USB2_1_PHY_TX_REF_VOL_440MV ((0x6 << 4) & USB2_1_PHY_TX_REF_VOL_MASK)
#define USB2_1_PHY_PLLCK_ADDR_OFFSET 0x14
#define USB2_1_PHY_PLLCK_MASK 0x00000003
#define USB2_1_PHY_PLLCK_VAL ((0x3 << 0) & USB2_1_PHY_PLLCK_MASK)

/* USB Controller register ,base reg 0x10320000 */
#define DCFG 0xc700
#define GTXTHRCFG 0xc108
#define GRXTHRCFG 0xc10c
#define REG_GCTL 0xc110
#define REG_GUSB3PIPECTL0 0xc2c0
#define DEVSPD_MASK 0x07
#define devspd_val(a) (((a) << 0) & DEVSPD_MASK)
#define PCS_SSP_SOFT_RESET (0x1 << 31)

/* USB CRG register */
#define USB3_CRG_CTRL 0x3960
#define USB3_CRG_DEFAULT_VAL 0x00030001
#define USB3_CRG_PCLK_OCC_SEL (0x1 << 18)
#define USB3_CRG_PIPE_CKEN (0x1 << 12)
#define USB3_CRG_UTMI_CKEN (0x1 << 8)
#define USB3_CRG_SUSPEND_CKEN (0x1 << 6)
#define USB3_CRG_REF_CKEN (0x1 << 5)
#define USB3_CRG_BUS_CKEN (0x1 << 4)
#define USB3_CRG_SRST_REQ (0x1 << 0)
#define USB2_1_PHY_CRG_OFFSET 0x38E0
#define USB2_1_PHY_CRG_DEFAULT_VAL 0x00000057
#define USB2_1_PHY_CRG_APB_SREQ (0x1 << 2)
#define USB2_1_PHY_CRG_TREQ (0x1 << 1)
#define USB2_1_PHY_CRG_REQ (0x1 << 0)
#define USB3_1_PHY_CRG_OFFSET 0x3964
#define USB3_1_PHY_CRG_DEFAULT_VAL 0x00000013
#define USB3_1_PHY_CRG_TREQ (0x1 << 1)
#define USB3_1_PHY_CRG_REQ (0x1 << 0)

/* OTP USB base register */
#define OTP_USB2_PO_PHY_INFO_ADDR_OFFSET 0x0444
#define OTP_USB2_PHY1_TRIM_OFFSET (5)
#define OTP_USB2_PHY1_TRIM_MASK (0x1f)
#define OTP_USB2_PHY1_TRIM_MIN (0x9)
#define OTP_USB2_PHY1_TRIM_MAX (0x1d)
#define OTP_USB2_PO_INFO_ADDR_OFFSET 0x040C
#define OTP_USB2_EYE_VAL_MASK 0x0c0
#define otp_usb2_eye_val(a) (((a) << 6) & OTP_USB2_EYE_VAL_MASK)
#define OTP_USB2_DELAY_VAL_MASK 0x00000018
#define otp_usb2_delay_val(a) (((a) << 3) & OTP_USB2_DELAY_VAL_MASK)
#define OTP_USB2_RAM_CLK_SEL_MASK 0x02

int usb2_io_delay;
void usb_vbus_on(void)
{
    unsigned int reg;
    reg = readl(VBUS_IO_REG);
    reg |= USB_VBUS_ON;
    writel(reg, VBUS_IO_REG);
}

void usb2_io_delay_init(void)
{
    unsigned int reg;
    reg = readl(REG_BASE_OTP_READ_ONLY + OTP_USB2_PO_INFO_ADDR_OFFSET);
    usb2_io_delay = reg & OTP_USB2_DELAY_VAL_MASK;
}

static void usb_io_udelay(void)
{
    unsigned int val = 0;
    switch (usb2_io_delay) {
        case otp_usb2_delay_val(0):
            val = 200; // 200 delay
            break;
        case otp_usb2_delay_val(1):
            break;
        case otp_usb2_delay_val(2):
            val = 300; // 300 delay
            break;
        case otp_usb2_delay_val(3):
            val = 50; // 50 delay
            break;
        default:
            val = 200; // 200 delay
            break;
    }
    if (val) {
        udelay(val);
    }
}

unsigned int usb2_otg_ram_clk_sel(void)
{
    unsigned int reg;
    reg = readl(REG_BASE_OTP_READ_ONLY + OTP_USB2_PO_INFO_ADDR_OFFSET);
    return (reg & OTP_USB2_RAM_CLK_SEL_MASK) ? (1) : (0);
}

void xvpv100_phy_config(void)
{
    unsigned int reg;
    unsigned int otp_val;

    reg = readl(REG_BASE_OTP_READ_ONLY + OTP_USB2_PO_PHY_INFO_ADDR_OFFSET);
    otp_val = (reg >> OTP_USB2_PHY1_TRIM_OFFSET) & OTP_USB2_PHY1_TRIM_MASK;
    if ((otp_val >= OTP_USB2_PHY1_TRIM_MIN) && (otp_val <= OTP_USB2_PHY1_TRIM_MAX)) {
        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_TRIM_ADDR_OFFSET);
        reg &= ~USB2_1_PHY_TRIM_MASK;
        reg |= usb2_1_phy_trim_val(otp_val);
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_TRIM_ADDR_OFFSET);
    }

    /* config eye diagram */
    reg = readl(REG_BASE_OTP_READ_ONLY + OTP_USB2_PO_INFO_ADDR_OFFSET);
    otp_val = (reg & OTP_USB2_EYE_VAL_MASK);
    switch (otp_val) {
    case otp_usb2_eye_val(0):
        break; // do nothing

    case otp_usb2_eye_val(1):
        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_DISCONNECT_ADDR_OFFSET);
        reg &= ~USB2_1_PHY_DISCONNECT_MASK;
        reg |= USB2_1_PHY_DISCONNECT_620MV;
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_DISCONNECT_ADDR_OFFSET);

        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_CURRENT_BIAS_ADDR_OFFSET);
        reg &= ~USB2_1_PHY_CURRENT_BIAS_MASK;
        reg |= USB2_1_PHY_CURRENT_BIAS_1DOT25;
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_CURRENT_BIAS_ADDR_OFFSET);

        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_TX_REF_VOL_ADDR_OFFSET);
        reg &= ~USB2_1_PHY_TX_REF_VOL_MASK;
        reg |= USB2_1_PHY_TX_REF_VOL_410MV;
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_TX_REF_VOL_ADDR_OFFSET);

        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_ATOP_TEST_ADDR_OFFSET);
        reg |= USB2_1_PHY_ATOP_TEST_EOP_DOWN;
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_ATOP_TEST_ADDR_OFFSET);
        break;

    case otp_usb2_eye_val(2):
        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_DISCONNECT_ADDR_OFFSET);
        reg &= ~USB2_1_PHY_DISCONNECT_MASK;
        reg |= USB2_1_PHY_DISCONNECT_640MV;
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_DISCONNECT_ADDR_OFFSET);

        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_CURRENT_BIAS_ADDR_OFFSET);
        reg &= ~USB2_1_PHY_CURRENT_BIAS_MASK;
        reg |= USB2_1_PHY_CURRENT_BIAS_1DOT25;
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_CURRENT_BIAS_ADDR_OFFSET);

        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_TX_REF_VOL_ADDR_OFFSET);
        reg &= ~USB2_1_PHY_TX_REF_VOL_MASK;
        reg |= USB2_1_PHY_TX_REF_VOL_410MV;
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_TX_REF_VOL_ADDR_OFFSET);

        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_ATOP_TEST_ADDR_OFFSET);
        reg |= (USB2_1_PHY_ATOP_TEST_EOP_DOWN | USB2_1_PHY_ATOP_TEST_BG);
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_ATOP_TEST_ADDR_OFFSET);
        break;

    case otp_usb2_eye_val(3):
        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_DISCONNECT_ADDR_OFFSET);
        reg &= ~USB2_1_PHY_DISCONNECT_MASK;
        reg |= USB2_1_PHY_DISCONNECT_620MV;
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_DISCONNECT_ADDR_OFFSET);

        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_TX_REF_VOL_ADDR_OFFSET);
        reg &= ~USB2_1_PHY_TX_REF_VOL_MASK;
        reg |= USB2_1_PHY_TX_REF_VOL_440MV;
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_TX_REF_VOL_ADDR_OFFSET);

        reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_ATOP_TEST_ADDR_OFFSET);
        reg |= (USB2_1_PHY_ATOP_TEST_EOP_DOWN | USB2_1_PHY_ATOP_TEST_1V8);
        writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_ATOP_TEST_ADDR_OFFSET);
        break;

    default:
        break;
    }
    udelay(20); // delay 20us
}

void usb_xvpv100_phy_ctrl_init(void)
{
    unsigned int reg;

    /* step1 write default val */
    reg = USB3_CRG_DEFAULT_VAL;
    writel(reg, REG_BASE_CRG + USB3_CRG_CTRL);
    reg = USB2_1_PHY_CRG_DEFAULT_VAL;
    writel(reg, REG_BASE_CRG + USB2_1_PHY_CRG_OFFSET);
    reg = USB3_1_PHY_CRG_DEFAULT_VAL;
    writel(reg, REG_BASE_CRG + USB3_1_PHY_CRG_OFFSET);

    /* step2 config CTRL_CRG & PHY_CRG */
    reg = readl(REG_BASE_CRG + USB3_CRG_CTRL);
    reg |= (USB3_CRG_PCLK_OCC_SEL | USB3_CRG_PIPE_CKEN | USB3_CRG_UTMI_CKEN | USB3_CRG_SUSPEND_CKEN |
        USB3_CRG_REF_CKEN | USB3_CRG_BUS_CKEN);
    writel(reg, REG_BASE_CRG + USB3_CRG_CTRL);

    reg = readl(REG_BASE_CRG + USB2_1_PHY_CRG_OFFSET);
    reg &= ~(USB2_1_PHY_CRG_APB_SREQ);
    writel(reg, REG_BASE_CRG + USB2_1_PHY_CRG_OFFSET);
    udelay(100); // delay 100us

    /* setp3 trim setting & eye config */
    xvpv100_phy_config();

    /* step4 CRG power reset */
    reg = readl(REG_BASE_CRG + USB3_1_PHY_CRG_OFFSET);
    reg &= ~(USB3_1_PHY_CRG_TREQ | USB3_1_PHY_CRG_REQ);
    writel(reg, REG_BASE_CRG + USB3_1_PHY_CRG_OFFSET);

    reg = readl(REG_BASE_CRG + USB2_1_PHY_CRG_OFFSET);
    reg &= ~USB2_1_PHY_CRG_REQ;
    writel(reg, REG_BASE_CRG + USB2_1_PHY_CRG_OFFSET);

    reg = readl(USB2_1_PHY_BASE_ADDR + USB2_1_PHY_PLLCK_ADDR_OFFSET);
    reg &= ~USB2_1_PHY_PLLCK_MASK;
    reg |= USB2_1_PHY_PLLCK_VAL;
    writel(reg, USB2_1_PHY_BASE_ADDR + USB2_1_PHY_PLLCK_ADDR_OFFSET);
    udelay(2000); // delay 2000us

    reg = readl(REG_BASE_CRG + USB2_1_PHY_CRG_OFFSET);
    reg &= ~USB2_1_PHY_CRG_TREQ;
    writel(reg, REG_BASE_CRG + USB2_1_PHY_CRG_OFFSET);
    udelay(200); // delay 200us

    reg = readl(REG_BASE_CRG + USB3_CRG_CTRL);
    reg &= ~USB3_CRG_SRST_REQ;
    writel(reg, REG_BASE_CRG + USB3_CRG_CTRL);
    udelay(200); // delay 200us

    /* step5 Disable susper speed. just bootrom need this step */
    reg = readl(USB_P0_REG_BASE + REG_GUSB3PIPECTL0);
    reg |= PCS_SSP_SOFT_RESET;
    writel(reg, USB_P0_REG_BASE + REG_GUSB3PIPECTL0);
    udelay(200); // delay 200us

    reg = readl(USB_P0_REG_BASE + DCFG);
    reg &= ~DEVSPD_MASK; /* DCFG.DEVSPD config high speed */
    reg |= devspd_val(0);
    writel(reg, USB_P0_REG_BASE + DCFG);
    udelay(20); // delay 20us

    reg = readl(USB_P0_REG_BASE + REG_GUSB3PIPECTL0);
    reg &= ~PCS_SSP_SOFT_RESET;
    reg &= ~(1 << 17); // 17 bit: disable suspend
    writel(reg, USB_P0_REG_BASE + REG_GUSB3PIPECTL0);
    udelay(20); // delay 20us

    writel(0x23100000, USB_P0_REG_BASE + GTXTHRCFG); // 0x23100000 reg val
    writel(0x23100000, USB_P0_REG_BASE + GRXTHRCFG); // 0x23100000 reg val
    udelay(200); // delay 200us
}

void usb_poll(void)
{
    if (usb3_dev_ex != NULL) {
        usb3_handle_event(usb3_dev_ex);
    }
}

static uint8_t string_manu[] = {'V', 0, 'E', 0, 'N', 0, 'D', 0, 'O', 0, 'R', 0};
static uint8_t string_prod[] = {'U', 0, 'S', 0, 'B', 0, 'B', 0, 'u', 0, 'r', 0, 'n', 0};
bool usb3_driver_init(void)
{
    usb3_device_t *usb3_dev = NULL;
    struct usb_device_descriptor *usb3_dev_desc = NULL;
    unsigned long ts;
    int retries = 0;

    usb3_dev = gsl_malloc(sizeof(usb3_device_t));
    if (usb3_dev == NULL)
        return FALSE;

    (void)memset_s(usb3_dev, sizeof(usb3_device_t), 0, sizeof(usb3_device_t));
    usb3_dev_ex = usb3_dev;
    usb3_dev_desc = gsl_malloc(sizeof(struct usb_device_descriptor));
    if (usb3_dev_desc == NULL)
        goto free_dev;

    usb3_pcd_t *pcd = &usb3_dev->pcd;
    usb3_pcd_ep_t *ep = &pcd->in_ep;
    usb3_pcd_req_t *req = &ep->req;
    req->bufdma = (uint32_t *)gsl_malloc(BULK_EP_MAX_PACKET_SIZE);
    if (req->bufdma == NULL)
        goto free_desc;

    usb_info("size of usb3_dev %d\n", sizeof(*usb3_dev));
    usb3_dev->base = (volatile uint8_t *)(long)USB_P0_REG_BASE;
    usb3_dev->string_manu_len = sizeof(string_manu);
    usb3_dev->string_prod_len = sizeof(string_prod);
    usb3_dev->dev_desc = usb3_dev_desc;
    (void)memcpy_s(usb3_dev->string_manu, usb3_dev->string_manu_len, string_manu, usb3_dev->string_manu_len);
    (void)memcpy_s(usb3_dev->string_prod, usb3_dev->string_prod_len, string_prod, usb3_dev->string_prod_len);
    usb3_dev->pcd.ep0_setup_desc = (usb3_dma_desc_t *)((long)(usb3_dev->pcd.ep0_setup + 0xf) &
                       (uint32_t)(~0xf));
    usb3_dev->pcd.ep0_in_desc = (usb3_dma_desc_t *)((long)(usb3_dev->pcd.ep0_in + 0xf) & (uint32_t)(
                        ~0xf));
    usb3_dev->pcd.ep0_out_desc = (usb3_dma_desc_t *)((long)(usb3_dev->pcd.ep0_out + 0xf) & (uint32_t)(
                         ~0xf));
    usb3_dev->pcd.in_ep.ep_desc = (usb3_dma_desc_t *)((long)(usb3_dev->pcd.in_ep.epx_desc + 0xf) &
                      (uint32_t)(~0xf));
    usb3_dev->pcd.out_ep.ep_desc = (usb3_dma_desc_t *)((long)(usb3_dev->pcd.out_ep.epx_desc + 0xf) &
                       (uint32_t)(~0xf));

tryagain:
    /* USB_1_VBUS_IO_Config */
    usb_vbus_on();

    /* io delay init */
    usb2_io_delay_init();

    /* Release usb2.0 controller */
    usb_xvpv100_phy_ctrl_init();

    /* Get usb3.0 version number */
    usb3_dev->snpsid =
        usb3_rd32((volatile uint32_t *)(usb3_dev->base + USB3_CORE_REG_BASE +
                        USB3_CORE_GSNPSID_REG_OFFSET));

    /* Initialize usb3.0 core */
    usb3_common_init(usb3_dev, usb3_dev->base + USB3_CORE_REG_BASE);

    /* Initialize usb3.0 pcd */
    usb3_init(usb3_dev);

    usb_info("usb init done\n");

    usb3_handle_event(usb3_dev);
    ts = 2000 * timer_get_divider(); // 2000 count of timer reg
    timer_start();
    do {
        /* Polling the USB3.0 event interrupt */
        usb3_handle_event(usb3_dev);
        if ((usb3_dev->pcd).state == USB3_STATE_CONFIGURED)
            break;
    } while (timer_get_val() < ts);

    if (((usb3_dev->pcd).state == USB3_STATE_DEFAULT) && (retries < 2)) { // retry 2 times
        retries++;
        goto tryagain;
    }

    if ((usb3_dev->pcd).state == USB3_STATE_CONFIGURED)
        return TRUE;

free_dma:
    gsl_free(req->bufdma);

free_desc:
    gsl_free(usb3_dev->dev_desc);

free_dev:
    gsl_free(usb3_dev);
    usb3_dev_ex = NULL;

    return FALSE;
}

void udc_disconnect(void)
{
    unsigned int reg;

    reg = USB3_CRG_DEFAULT_VAL;
    writel(reg, REG_BASE_CRG + USB3_CRG_CTRL);
    reg = USB2_1_PHY_CRG_DEFAULT_VAL;
    writel(reg, REG_BASE_CRG + USB2_1_PHY_CRG_OFFSET);
    reg = USB3_1_PHY_CRG_DEFAULT_VAL;
    writel(reg, REG_BASE_CRG + USB3_1_PHY_CRG_OFFSET);

    writel(VBUS_IO_DEFAULT_VAL, VBUS_IO_REG);
}
