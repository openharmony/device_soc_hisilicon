/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART04_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART04_H_

#ifndef CONFIG_USE_SYS_CONFIG
td_void tde_hal_set_clock(td_bool enable)
{
    if (g_tde_clock_vir == TD_NULL) {
        tde_error("null pointer\n", __FUNCTION__, __LINE__);
        return;
    }
    if (enable) {
        /* enable clock */
        *g_tde_clock_vir |= 0x1010;

        /* cancel reset */
        *g_tde_clock_vir &= ~0x1;
    } else {
#ifdef TDE_LOWPOWER
        /* reset */
        *g_tde_clock_vir |= 0x1;

        /* disable clock */
        *g_tde_clock_vir &= ~0x10;
#endif
    }
}
#else
td_void tde_hal_set_clock(td_bool enable)
{
    td_bool tmp;
    ot_mpp_chn mpp_chn;
    mpp_chn.mod_id = OT_ID_TDE;
    mpp_chn.dev_id = 0;

    if (enable == TD_TRUE) {
        tmp = TD_TRUE;
        call_sys_drv_ioctrl(&mpp_chn, SYS_TDE_CLK_EN, &tmp);
        tmp = TD_FALSE;
        call_sys_drv_ioctrl(&mpp_chn, SYS_TDE_RESET_SEL, &tmp);
    } else {
#ifdef TDE_LOWPOWER
        tmp = TD_TRUE;
        call_sys_drv_ioctrl(&mpp_chn, SYS_TDE_RESET_SEL, &tmp);
        tmp = TD_FALSE;
        call_sys_drv_ioctrl(&mpp_chn, SYS_TDE_CLK_EN, &tmp);
#endif
    }
    return;
}
#endif

/*
 * Function:      tde_hal_node_init_nd
 * Description:   init the software node struct for tde
 * Input:         hw_node: the pointer of software node struct
 */
td_s32 tde_hal_node_init_nd(tde_hw_node **hw_node)
{
    td_void *buf = TD_NULL;

    if (hw_node == TD_NULL) {
        return TD_FAILURE;
    }
    buf = (td_void *)tde_malloc(sizeof(tde_hw_node) + TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE);
    if (buf == TD_NULL) {
        tde_error("malloc (%lu) failed, wgetfreenum(%u)!\n",
                  (unsigned long)(sizeof(tde_hw_node) + TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE), wgetfreenum());
        return DRV_ERR_TDE_NO_MEM;
    }

    *hw_node = (tde_hw_node *)((td_u8 *)buf + TDE_NODE_HEAD_BYTE);

    return TD_SUCCESS;
}

#ifdef CONFIG_GFX_MMU_SUPPORT
td_void tde_hal_free_tmp_buf(const tde_hw_node *hw_node)
{
    return;
}
#endif

/*
 * Function:      tde_hal_free_node_buf
 * Description:   Free TDE operate node buffer
 * Input:         hw_node:Node struct pointer.
 */
td_void tde_hal_free_node_buf(tde_hw_node *hw_node)
{
    td_void *buf = TD_NULL;

    if (hw_node == TD_NULL) {
        return;
    }
    buf = (td_void *)hw_node - TDE_NODE_HEAD_BYTE;
    if (buf == TD_NULL) {
        return;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    tde_hal_free_tmp_buf(hw_node);
#endif

    tde_free(buf);
    return;
}

/*
 * Function:      tde_hal_node_execute
 * Description:   start list of tde
 * Input:
 *                nodephy_addr: the start address of head node address
 *                update:the head node update set
 *                aq_use_buff: whether use temp buffer
 */
td_s32 tde_hal_node_execute(td_phys_addr_t nodephy_addr, td_u64 update, td_bool aq_use_buff)
{
    ot_unused(update);
    ot_unused(aq_use_buff);
    if (g_base_vir_addr == TD_NULL) {
        tde_error("null pointer\n");
        return TD_FAILURE;
    }
    /* tde is idle */
    if (tde_hal_ctl_is_idle_safely()) {
        tde_hal_set_clock(TD_TRUE);
        tde_hal_ctl_reset();
        tde_hal_init_queue();

        /* write the first node address */
        tde_write_reg(g_base_vir_addr, TDE_AQ_NADDR_LOW, get_low_addr(nodephy_addr));
        tde_write_reg(g_base_vir_addr, TDE_AQ_NADDR_HI, get_high_addr(nodephy_addr));
        osal_dmb();
        osal_udelay(100); /* 100 time out */
        /* start Aq list, CNcomment : Start the Aq */
        tde_write_reg(g_base_vir_addr, TDE_CTRL, 0x1);
    } else {
        return TD_FAILURE;
    }
#ifdef TDE_TIME_COUNT
    (td_void) osal_gettimeofday(&g_time_start);
    /* 1000000 us */
    printk("tde_hal_node_execute: start time : %ld!\n", (td_u32)g_time_start.tv_sec * 1000000 +
        g_time_start.tv_usec);
#endif
    return TD_SUCCESS;
}

/*
 * Function:      tde_hal_node_enable_complete_int
 * Description:   enable the finish interrupt of node
 * Input:         buf: buffer of node
 */
td_void tde_hal_node_enable_complete_int(td_void *buf)
{
    tde_hw_node *hw_node = (tde_hw_node *)buf;
    if (buf == TD_NULL) {
        tde_error("hw_node Buf is null !\n");
        return;
    }
    hw_node->tde_intmask.bits.eof_mask = 0;
    hw_node->tde_intmask.bits.timeout_mask = 0x1;
    hw_node->tde_intmask.bits.bus_err_mask = 0x1;
    hw_node->tde_intmask.bits.eof_end_mask = 0x1;
    return;
}

td_void tde_hal_next_node_addr(td_void *buf, td_phys_addr_t phy_addr)
{
    tde_hw_node *hw_node = (tde_hw_node *)buf;
    if (buf == TD_NULL) {
        tde_error("hw_node Buf is null !\n");
        return;
    }
    hw_node->tde_pnext_low.bits.p_next_low = get_low_addr(phy_addr);
    hw_node->tde_pnext_hi.bits.p_next_hi = get_high_addr(phy_addr);
    return;
}

#if (TDE_CAPABILITY & SYNC)
td_void tde_hal_node_enable_sync(td_void *buf)
{
    tde_hw_node *hw_node = (tde_hw_node *)buf;
    if (buf == TD_NULL) {
        tde_error("Buf is null !\n");
        return;
    }
    hw_node->des_ctrl.bits.des_bind_en = 0x1;
    hw_node->des_ctrl.bits.des_bind_mode = 0x0;
    hw_node->des_ctrl.bits.des_h_scan_ord = 0;
    hw_node->des_ctrl.bits.des_v_scan_ord = 0;
    hw_node->des_safe_dist.all = hw_node->des_ch0_stride.all;
    hw_node->des_safe_dist_inverse.all = hw_node->des_ch0_stride.all / 2; /* 2 half */
    return;
}
#endif

/*
 * Function:      tde_hal_node_set_src1
 * Description:   set the info for source of bitmap 1
 * Input:         hw_node: pointer of node
 *                drv_surface: bitmap info
 */
td_void tde_hal_node_set_src1(tde_hw_node *hw_node, const tde_surface_msg *drv_surface)
{
    td_u32 bpp;
    td_phys_addr_t phy_addr;
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }
    if (drv_surface == TD_NULL) {
        tde_error("DrvSurface is null !\n");
        return;
    }
    /* set the source bitmap attribute info, CNcomment: Configure attribute information source bitmap */
    hw_node->src1_ctrl.bits.src1_fmt = (td_u32)drv_surface->color_format;
    hw_node->src1_ctrl.bits.src1_alpha_range = 1 - (td_u32)drv_surface->alpha_max_is_255;
    hw_node->src1_ctrl.bits.src1_h_scan_ord = (td_u32)drv_surface->hor_scan;
    hw_node->src1_ctrl.bits.src1_v_scan_ord = (td_u32)drv_surface->ver_scan;
    /*
     * file zero of low area and top area use low area extend
     * CNcomment: Have been using low filling is 0, high use low expansion mode
     */
    hw_node->src1_ctrl.bits.src1_rgb_exp = 0;

    hw_node->src1_imgsize.bits.src1_width = drv_surface->width - 1;
    hw_node->src1_imgsize.bits.src1_height = drv_surface->height - 1;

    bpp = (td_u32)tde_hal_getbpp_by_fmt(drv_surface->color_format);

    phy_addr = drv_surface->phys_addr + (td_u64)(drv_surface->ypos) * (td_u64)(drv_surface->pitch) +
                  (((td_u64)(drv_surface->xpos) * (td_u64)(bpp)) >> 3); /* bpp 2^3 */

    hw_node->src1_ch0_addr_low.bits.src1_ch0_addr_low = get_low_addr(phy_addr);
    hw_node->src1_ch0_addr_high.bits.src1_ch0_addr_high = get_high_addr(phy_addr);

    if (drv_surface->color_format <= TDE_DRV_COLOR_FMT_RGB565) {
        hw_node->src1_ctrl.bits.src1_argb_order = drv_surface->rgb_order;
    }
    /*
     * target bitmapis same with source bitmap 1,so not need set.
     * config the node, CNcomment: Configure the cache node
     */
    hw_node->src1_ch0_stride.bits.src1_ch0_stride = drv_surface->pitch;
    return;
}

static td_void tde_hal_set_src2_ctrl(tde_hw_node *hw_node, const tde_surface_msg *drv_surface)
{
    hw_node->src2_ctrl.bits.src2_fmt = (td_u32)drv_surface->color_format;
    hw_node->src2_ctrl.bits.src2_alpha_range = 0;
    hw_node->src2_ctrl.bits.src2_h_scan_ord = (td_u32)drv_surface->hor_scan;
    hw_node->src2_ctrl.bits.src2_v_scan_ord = (td_u32)drv_surface->ver_scan;
}

static td_void tde_hal_set_src2_img_size(tde_hw_node *hw_node, const tde_surface_msg *drv_surface)
{
    hw_node->src2_imgsize.bits.src2_width = drv_surface->width - 1;
    hw_node->src2_imgsize.bits.src2_height = drv_surface->height - 1;
}

static td_void tde_hal_set_src2_addr(tde_hw_node *hw_node, const tde_surface_msg *drv_surface, td_phys_addr_t *phy_addr,
    td_phys_addr_t *cb_crphy_addr)
{
    td_u32 bpp = (td_u32)tde_hal_getbpp_by_fmt(drv_surface->color_format);
    if (drv_surface->color_format >= TDE_DRV_COLOR_FMT_YCBCR400MBP) {
        *phy_addr = drv_surface->phys_addr + (td_u64)(drv_surface->ypos) * (td_u64)(drv_surface->pitch) +
                    (((td_u64)(drv_surface->xpos) * 8) >> 3); /* 3 8 xpos alg data */

        hw_node->src2_ch0_addr_low.bits.src2_ch0_addr_low = get_low_addr(*phy_addr);
        hw_node->src2_ch0_addr_high.bits.src2_ch0_addr_high = get_high_addr(*phy_addr);
        switch (drv_surface->color_format) {
            case TDE_DRV_COLOR_FMT_YCBCR422MBH:
                *cb_crphy_addr = drv_surface->cbcr_phys_addr +
                                 (td_u64)(drv_surface->ypos) * (td_u64)(drv_surface->cb_cr_pitch) +
                                 ((td_u64)(drv_surface->xpos) / 2 * 2); /* 2 xpos alg data cb_crphy_addr */
                break;
            case TDE_DRV_COLOR_FMT_YCBCR422MBV:
                *cb_crphy_addr = drv_surface->cbcr_phys_addr +
                                 (td_u64)(drv_surface->ypos / 2) * (td_u64)(drv_surface->cb_cr_pitch) + /* 2 alg */
                                 (((td_u64)(drv_surface->xpos) * 16) >> 3); /* 16 3 alg data cb_crphy_addr */
                break;
            case TDE_DRV_COLOR_FMT_YCBCR420MB:
                *cb_crphy_addr = drv_surface->cbcr_phys_addr +
                                 (td_u64)(drv_surface->ypos / 2) * (td_u64)(drv_surface->cb_cr_pitch) + /* 2 alg */
                                 ((td_u64)(drv_surface->xpos) / 2 * 2); /* 2 xpos alg data cb_crphy_addr */
                break;
            case TDE_DRV_COLOR_FMT_YCBCR444MB:
                *cb_crphy_addr = drv_surface->cbcr_phys_addr +
                                 (td_u64)(drv_surface->ypos) * (td_u64)(drv_surface->cb_cr_pitch) +
                                 (((td_u64)(drv_surface->xpos) * 16) >> 3); /* 16 3 alg data */
                break;
            default:;
        }

        hw_node->src2_ch1_addr_low.bits.src2_ch1_addr_low = get_low_addr(*cb_crphy_addr);
        hw_node->src2_ch1_addr_high.bits.src2_ch1_addr_high = get_high_addr(*cb_crphy_addr);

        if (drv_surface->color_format == TDE_DRV_COLOR_FMT_YCBCR422MBV) {
            hw_node->src2_ctrl.bits.src2_422v_pro = 1;
        }
    } else {
        *phy_addr = drv_surface->phys_addr + (td_u64)(drv_surface->ypos) *
            /* alg data 2^3 */
            (td_u64)(drv_surface->pitch) + (((td_u64)(drv_surface->xpos) * (td_u64)(bpp)) >> 3);
        hw_node->src2_ch0_addr_low.bits.src2_ch0_addr_low = get_low_addr(*phy_addr);
        hw_node->src2_ch0_addr_high.bits.src2_ch0_addr_high = get_high_addr(*phy_addr);

        if (drv_surface->color_format <= TDE_DRV_COLOR_FMT_RGB565) {
            hw_node->src2_ctrl.bits.src2_argb_order = drv_surface->rgb_order;
        }
    }
}

/*
 * Function:      tde_hal_node_set_src2
 * Description:   set the source bitmap 2
 * Input:         hw_node: pointer of node
 *                drv_surface:  bitmap info
 */
td_void tde_hal_node_set_src2(tde_hw_node *hw_node, const tde_surface_msg *drv_surface)
{
    td_phys_addr_t phy_addr;
    td_phys_addr_t cb_crphy_addr;

    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }
    if (drv_surface == TD_NULL) {
        tde_error("DrvSurface is null !\n");
        return;
    }

    /* set attribute info for source bitmap, CNcomment:Configure attribute information source bitmap */
    tde_hal_set_src2_ctrl(hw_node, drv_surface);
    /*
     * file zero of low area and top area use low area extend
     * CNcomment: Have been using low filling is 0, high use low expansion mode
     */
    hw_node->src2_ctrl.bits.src2_rgb_exp = 0;

    tde_hal_set_src2_img_size(hw_node, drv_surface);

    phy_addr = drv_surface->phys_addr;
    cb_crphy_addr = drv_surface->cbcr_phys_addr;

    tde_hal_set_src2_addr(hw_node, drv_surface, &phy_addr, &cb_crphy_addr);
    if (drv_surface->color_format <= TDE_DRV_COLOR_FMT_A1) {
        hw_node->src2_alpha.bits.src2_alpha0 = 0x00;
        hw_node->src2_alpha.bits.src2_alpha1 = 0xff;
    }
    hw_node->src2_ch0_stride.bits.src2_ch0_stride = drv_surface->pitch;
    hw_node->src2_ch1_stride.bits.src2_ch1_stride = drv_surface->cb_cr_pitch;

    return;
}

#if (TDE_CAPABILITY & COMPRESS)
td_void tde_hal_node_set_src_to_decompress(tde_hw_node *hw_node, const tde_surface_msg *drv_surface)
{
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }
    tde_hal_node_set_src2(hw_node, drv_surface);

    hw_node->src2_ctrl.bits.src2_is_lossless = 0;
    hw_node->src2_ctrl.bits.src2_is_lossless_a = 0;
    hw_node->src2_ctrl.bits.src2_dcmp_en = 1;
    hw_node->src2_ctrl.bits.src2_top_pred_en = 1;

    return;
}
#endif


static td_void tde_hal_node_set_hw_node(tde_hw_node *hw_node, const tde_surface_msg *drv_surface,
    tde_drv_outalpha_from alpha_from)
{
    /* set bitmap attribute info, CNcomment:Configure attribute information source bitmap */
    hw_node->des_ctrl.bits.des_en = 1;
    hw_node->des_ctrl.bits.des_fmt = drv_surface->color_format;
    hw_node->des_ctrl.bits.des_alpha_range = 1 - (td_u32)drv_surface->alpha_max_is_255;
    hw_node->des_ctrl.bits.des_h_scan_ord = (td_u32)drv_surface->hor_scan;
    hw_node->des_ctrl.bits.des_v_scan_ord = (td_u32)drv_surface->ver_scan;
    hw_node->des_ctrl.bits.des_alpha_range = 1 - (td_u32)drv_surface->alpha_max_is_255;
    hw_node->des_ctrl.bits.des_rgb_round = g_rgb_truncation_mode;
    hw_node->cbmalupara.bits.alpha_from = (td_u32)alpha_from;
    hw_node->des_alpha.bits.des_alpha_thd = g_alpha_threshold_value;

    /* set bitmap size info, CNcomment: Configure the bitmap size information */
    hw_node->des_imgsize.bits.des_width = (td_u32)drv_surface->width - 1;
    hw_node->des_imgsize.bits.des_height = (td_u32)drv_surface->height - 1;
}

/*
 * Function:      tde_hal_node_set_tqt
 * Description:   set target bitmap info
 * Input:         hw_node: pointer of node
 *                drv_surface: bitmap info
 *                alpha_from: alpha from
 */
td_void tde_hal_node_set_tqt(tde_hw_node *hw_node, tde_surface_msg *drv_surface, tde_drv_outalpha_from alpha_from)
{
    td_u32 bpp;
    td_phys_addr_t phy_addr;
    td_phys_addr_t cb_crphy_addr;

    if (hw_node == TD_NULL || drv_surface == TD_NULL) {
        tde_error("null pointer !\n");
        return;
    }
    tde_hal_node_set_hw_node(hw_node, drv_surface, alpha_from);
    if (drv_surface->color_format == TDE_DRV_COLOR_FMT_AYCBCR8888) {
        drv_surface->color_format = TDE_DRV_COLOR_FMT_ARGB8888;
        hw_node->des_ctrl.bits.des_argb_order = 0x17;
    }

    if ((drv_surface->color_format <= TDE_DRV_COLOR_FMT_RGB565) ||
        (drv_surface->color_format == TDE_DRV_COLOR_FMT_AYCBCR8888)) {
        hw_node->des_ctrl.bits.des_argb_order = drv_surface->rgb_order;
        if (drv_surface->color_format == TDE_DRV_COLOR_FMT_AYCBCR8888) {
            drv_surface->color_format = TDE_DRV_COLOR_FMT_ARGB8888;
            hw_node->des_ctrl.bits.des_argb_order = 0x17;
        }
    }
    if (drv_surface->color_format == TDE_DRV_COLOR_FMT_RABG8888) {
        hw_node->des_ctrl.bits.des_argb_order = TDE_DRV_ORDER_RABG; /* RABG */
        hw_node->des_ctrl.bits.des_fmt = TDE_DRV_COLOR_FMT_ARGB8888;
    }


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART04_H_ */
