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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART16_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART16_H_

    /* fill src image info */
    tde_callback_src_init(par, &src_img);

    /* fill dst image info */
    tde_callback_dst_init(par, &dst_img, buf_mode);

    /* fill rot option info */
    rot_opt.rotate_mode = display_info->rotate_mode;
    /*
     * Note: After turning on the callback, the rotation job callback function may not be called.
     * Need to wait until the next tde task is called, the direct phenomenon is: if the last run
     * In the use case, the rotation angle of 90 or 270 is set and the tde rotation branch is entered.
     * Unloading ko and loading ko will result in core dump.
     */
    rot_opt.call_back = (buf_mode == OT_FB_LAYER_BUF_DOUBLE) ? TD_TRUE : TD_FALSE;
    /* In the interrupt, the blocking mode is not allowed, so the non-blocking mode is used. */
    rot_opt.block = TD_FALSE;
    rot_opt.param = &(par->layer_id);

    /* Fill is_refresh_screen.Not used now */
    is_refresh_screen = TD_TRUE;
    if (gfbg_get_rotation() != TD_NULL) {
        refresh_info->disp_buf_info.refresh_handle = gfbg_get_rotation()(&src_img, &dst_img, &rot_opt,
            is_refresh_screen);
    }
    return;
}

static td_void tde_callback_without_rotate(gfbg_par *par, ot_fb_layer_buf buf_mode, const td_void *paraml,
                                           const td_void *paramr)
{
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_tde_callback_param *param = (gfbg_tde_callback_param *)paraml;
    td_s32 tde_finish_handle = *(td_s32 *)paramr;

    if ((buf_mode == OT_FB_LAYER_BUF_DOUBLE) && (refresh_info->disp_buf_info.refresh_handle == tde_finish_handle)) {
        /* Notify VO, can be updated */
        gfbg_set_dispbufflip(par, TD_TRUE);
        par->refresh_info.disp_buf_info.compress = param->compress;
#ifdef CONFIG_HI_VO_FB_SEPARATE
        par->refresh_info.refresh_job_done = TD_TRUE;
        wake_up(&(par->refresh_job_done));
#endif
    }
    par->compress_info.update_finished = TD_TRUE;
    return;
}

static td_s32 gfbg_tde_callback(const td_void *paraml, const td_void *paramr)
{
    td_u32 layer_id = 0;
    td_s32 tde_finish_handle;
    struct fb_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    ot_fb_layer_buf buf_mode;
    unsigned long lock_flag;
    gfbg_tde_callback_param *param = (gfbg_tde_callback_param *)paraml;
    tde_finish_handle = *(td_s32 *)paramr;
    /* here get a wrong layer_id, make gfbg core dump here */
    if (param != TD_NULL) {
        layer_id = param->layer_id;
    } else {
        gfbg_error("paraml is NULL\n");
        return TD_FAILURE;
    }
    if (layer_id <= g_drv_ops.layer_count) {
        info = g_layer[layer_id].info;
    } else {
        kfree(param);
        param = TD_NULL;
        gfbg_error("layer_id = %d is invalid\n", layer_id);
        return TD_FAILURE;
    }
    par             = (gfbg_par *)(info->par);
    refresh_info     = &par->refresh_info;
    display_info     = &par->display_info;
    gfbg_info("tde callback blit handle:%x, end handle:%x\n", refresh_info->disp_buf_info.refresh_handle,
              tde_finish_handle);

    gfbg_get_bufmode(par, &buf_mode);
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);

    if ((OT_FB_ROTATE_90 == display_info->rotate_mode || OT_FB_ROTATE_270 == display_info->rotate_mode)) {
        /* gfbg callback with rotate */
        tde_callback_with_rotate(par, buf_mode);
    } else {
        /* gfbg callback without rotate */
        tde_callback_without_rotate(par, buf_mode, paraml, paramr);
    }

    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    if (param != TD_NULL) {
        kfree(param);
        param = TD_NULL;
    }
    return TD_SUCCESS;
}

static inline td_bool gfbg_is_interlace(const gfbg_par *par)
{
    gfbg_osd_data osd_data = {0};
    if (par != TD_NULL) {
        g_drv_ops.gfbg_drv_get_osd_data(par->layer_id, &osd_data);
    }
    return (osd_data.scan_mode == GFBG_SCANMODE_I);
}

static td_phys_addr_t gfbg_buf_allocmem(const td_char *buf_name, td_ulong layer_size, const td_char *mmz_name)
{
    td_phys_addr_t addr;

    if ((layer_size == 0) || (layer_size > 0x40000000)) {
        return 0;
    }

    addr = cmpi_mmz_malloc((td_char *)mmz_name, (td_char *)buf_name, layer_size, TD_FALSE);
    if ((addr == MMB_ADDR_INVALID) && (mmz_name != TD_NULL)) {
        addr = cmpi_mmz_malloc(TD_NULL, (td_char *)buf_name, layer_size, TD_FALSE);
    }

    if (addr == MMB_ADDR_INVALID) {
        gfbg_error("alloc mem failed!\n");
        return 0;
    }

    return addr;
}

static td_void gfbg_set_bufmode(td_u32 layer_id, ot_fb_layer_buf layer_buf_mode)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;

    /* in 0 buf mode ,maybe the stride or format will be changed! */
    if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && (refresh_info->buf_mode != layer_buf_mode)) {
        par->modifying = TD_TRUE;

        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_STRIDE;

        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_FMT;

        par->modifying = TD_FALSE;
    }

    refresh_info->buf_mode = layer_buf_mode;
}

/*
 * choose the module to do  flicker resiting, TDE or VOU ? the rule is as this ,the module should do flicker resisting
 * who has do scaling
 */
static td_void gfbg_select_antiflicker_mode(gfbg_par *par)
{
    gfbg_display_info *display_info = TD_NULL;

    display_info = &par->display_info;

    /* if the usr's configuration is no needed to do flicker resisting, so no needed to do it  */
    if (display_info->antiflicker_level == OT_FB_LAYER_ANTIFLICKER_NONE) {
        display_info->antiflicker_mode = GFBG_ANTIFLICKER_NONE;
    } else {
        /* current standard no needed to do flicker resisting */
        if (!display_info->need_antiflicker) {
            display_info->antiflicker_mode = GFBG_ANTIFLICKER_NONE;
        } else {
            /* VO has done scaling , so should do flicker resisting at the same time */
            if ((display_info->display_width != display_info->screen_width) ||
                (display_info->display_height != display_info->screen_height)) {
                display_info->antiflicker_mode = GFBG_ANTIFLICKER_VO;
            } else {
                display_info->antiflicker_mode = GFBG_ANTIFLICKER_TDE;
            }
        }
    }
}

static td_void gfbg_set_antiflickerlevel(td_u32 layer_id, ot_fb_layer_antiflicker_level antiflicker_level)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;

    display_info->antiflicker_level = antiflicker_level;
    gfbg_select_antiflicker_mode(par);

    return;
}


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART16_H_ */
