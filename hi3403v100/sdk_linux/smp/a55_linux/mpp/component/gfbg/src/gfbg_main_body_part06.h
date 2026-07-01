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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART06_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART06_H_

        g_drv_ops.gfbg_close_layer(par->layer_id);
        /* Clear memory detection area when closing graphics layer */
#ifdef MDDRDETECT
        g_drv_ops.gfbg_drv_clear_ddr_dectect_zone(par->compress_info.start_section, par->compress_info.zone_nums);
#endif
        par->compress_info.start_section = 0;
        par->compress_info.zone_nums = 0;
        par->compress_info.new_start_section = 0;
        par->compress_info.new_zone_nums = 0;
        par->compress_info.clear_zone = TD_TRUE;

        /* MMzFree in gfbg_freeccanbuf has a sleep function call, you need to release the spin lock first. */
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        gfbg_freeccanbuf(par);
        gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    }
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
}

static td_s32 gfbg_release(struct fb_info *info, td_s32 user)
{
    gfbg_par *par = TD_NULL;
    td_u32 mem_len;
    td_s8 *screen_base = TD_NULL;
    ot_unused(user);

    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    if (info->par == TD_NULL) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;

    if (atomic_dec_and_test(&par->ref_count)) {
        gfbg_release_info(par);

        screen_base = gfbg_get_screen_base(info);
        mem_len = gfbg_get_smem_len(info);
        if ((screen_base != TD_NULL) && (mem_len != 0)) {
            (td_void)memset_s(screen_base, mem_len, 0, mem_len);
        }
#ifdef CONFIG_OT_VO_FB_SEPARATE
        vou_graphics_deinit();
        par->layer_open = TD_FALSE;
#endif
    }

    return 0;
}

static td_s32 gfbg_draw_smart_rect(const struct fb_info *info, const ot_fb_smart_rect_param *param)
{
    gfbg_mmz_buffer osb_chn = {0};
    gfbg_par *par = (gfbg_par *)info->par;

    osb_chn.start_phy_addr = gfbg_get_smem_start(info);
    osb_chn.start_vir_addr = gfbg_get_screen_base(info);
    osb_chn.size = SMART_RECT_SIZE;
    if (g_drv_ops.gfbg_drv_set_smart_rect == TD_NULL || par == TD_NULL) {
        gfbg_error("gfbg_draw_smart_rect or par is NULL !\n");
        return TD_FAILURE;
    }
    if (g_drv_ops.gfbg_drv_set_smart_rect(par->layer_id, par->display_info.x_res,
        par->display_info.y_res, param, &osb_chn) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

#ifdef CURSOR
/* Soft mouse needs to filter out the hardware mouse interface */
static td_bool gfbg_cmdfilter(td_u32 cmd)
{
    td_bool filter = TD_FALSE;
    switch (cmd) {
        case FBIO_REFRESH:
        case FBIOGET_CANVAS_BUF:
        case FBIOPUT_LAYER_INFO:
        case FBIOGET_LAYER_INFO:
        case FBIOGET_ALPHA_GFBG:
        case FBIOPUT_ALPHA_GFBG:
        case FBIOGET_DEFLICKER_GFBG:
        case FBIOPUT_DEFLICKER_GFBG:
        case FBIOGET_COLORKEY_GFBG:
        case FBIOPUT_COLORKEY_GFBG:
        case FBIOPUT_SCREEN_SIZE:
        case FBIOGET_SCREEN_SIZE:
        case FBIOGET_SCREEN_ORIGIN_GFBG:
        case FBIOPUT_SCREEN_ORIGIN_GFBG:
        case FBIOGET_VER_BLANK_GFBG:
        case FBIOPUT_SHOW_GFBG:
        case FBIOGET_SHOW_GFBG:
        case FBIO_WAITFOR_FREFRESH_DONE:
        case FBIOPUT_COMPRESSION_GFBG:
        case FBIOGET_COMPRESSION_GFBG:
        case FBIOFLIP_SURFACE:

#ifdef MDDRDETECT
        case FBIOPUT_MDDRDETECT_GFBG:
        case FBIOGET_MDDRDETECT_GFBG:
#endif
            filter = TD_TRUE; /* These interfaces need to be filtered */
            break;
        default:
            break;
    }

    return filter;
}

static td_s32 gfbg_check_cusor_phyaddr(ot_fb_surface *cursor)
{
    ot_fb_surface *cursor_sur = cursor;
    td_u64 len;
    if (cursor == TD_NULL) {
        return TD_FAILURE;
    }

    len = cursor_sur->pitch * cursor_sur->height;
    return ot_mmz_check_phys_addr(cursor_sur->phys_addr, len);
}
#endif

static td_s32 gfbg_ioctl_check_param(const struct fb_info *info, td_u32 cmd, td_void *argp)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;

    if ((info == TD_NULL) || (info->par == TD_NULL)) {
        gfbg_error("NULL arg!\n");
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;
    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        gfbg_error("layer_id (%u) is invalid!should be in [%d, %d]\n", layer_id, 0, GFBG_MAX_LAYER_NUM - 1);
        return TD_FAILURE;
    }

    if ((argp == TD_NULL) && (cmd != FBIOGET_VER_BLANK_GFBG) && (cmd != FBIO_WAITFOR_FREFRESH_DONE) &&
        (cmd != FBIO_CREATE_LAYER) && (cmd != FBIO_DESTROY_LAYER)) {
        gfbg_error("NULL arg!\n");
        return TD_FAILURE;
    }

    if ((!g_drv_ops.capability[layer_id].is_layer_support) &&
        (!is_cursor_layer(layer_id) || (!is_soft_cursor()))) {
        gfbg_error("not support layer %u!\n", layer_id);
        return TD_FAILURE;
    }
#ifdef CURSOR
    /* Turn on the soft mouse function, and it is the mouse layer, you need to filter the hard interface. */
    if ((is_soft_cursor()) && (is_cursor_layer(layer_id))) {
        if (gfbg_cmdfilter(cmd)) {
            gfbg_error("soft cursor layer %u not support this function!\n", layer_id);
            return TD_FAILURE;
        }
    }
#endif

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_colorkey(struct fb_info *info, unsigned long arg)
{
    ot_fb_colorkey colorkey = {0};
    gfbg_colorkeyex colorkey_ex = {0};
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    par = (gfbg_par *)info->par;

    if ((!g_drv_ops.capability[par->layer_id].is_key_rgb) &&
        (!g_drv_ops.capability[par->layer_id].is_key_alpha)) {
        gfbg_error("Layer %u doesn't support colorkey!\n", par->layer_id);
        return TD_FAILURE;
    }

    gfbg_get_key(par, &colorkey_ex);

    colorkey.enable = colorkey_ex.key_enable;
    colorkey.value = colorkey_ex.key;

    return osal_copy_to_user(argp, &colorkey, sizeof(ot_fb_colorkey));
}

static td_s32 drv_gfbg_set_clut_ck(const struct fb_info *info, const ot_fb_colorkey *colorkey,
    gfbg_colorkeyex *colorkey_ex)
{
    if (colorkey->value >= (td_u32)(1 << gfbg_get_bits_per_pixel(info))) {
        gfbg_error("The value :%u is out of range the palette: %u!\n", colorkey->value,
            1 << gfbg_get_bits_per_pixel(info));
        return TD_FAILURE;
    }

    if ((info->cmap.blue == NULL) || (info->cmap.green == NULL) || (info->cmap.red == NULL)) {
        gfbg_error("error:cmap is NULL!\n");
        return TD_FAILURE;
    }

    colorkey_ex->blue_max = colorkey_ex->blue_min = info->cmap.blue[colorkey->value];
    colorkey_ex->green_max = colorkey_ex->green_min = info->cmap.green[colorkey->value];
    colorkey_ex->red_max = colorkey_ex->red_min = info->cmap.red[colorkey->value];
    colorkey_ex->red_mask = 0xff;
    colorkey_ex->green_mask = 0xff;
    colorkey_ex->blue_mask = 0xff;
    colorkey_ex->key_mode = 0;
    colorkey_ex->mask_enable = TD_TRUE;
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_colorkey(struct fb_info *info, unsigned long arg)
{
    ot_fb_colorkey colorkey;
    gfbg_colorkeyex colorkey_ex;
    unsigned long lock_flag;
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;

    if (osal_copy_from_user(&colorkey, argp, sizeof(ot_fb_colorkey))) {
        return -EFAULT;
    }

    if ((colorkey.enable != TD_TRUE) && (colorkey.enable != TD_FALSE)) {
        gfbg_error("enable(%d) should be TRUE or FALSE!\n", colorkey.enable);
        return TD_FAILURE;
    }

    if ((!g_drv_ops.capability[par->layer_id].is_key_rgb) && (!g_drv_ops.capability[par->layer_id].is_key_alpha)) {
        gfbg_error("Layer %u doesn't support colorkey!\n", par->layer_id);
        return TD_FAILURE;
    }

    if (colorkey.enable && display_info->is_premul) {
        gfbg_error("colorkey and premul couldn't take effect at the same time!\n");
        return TD_FAILURE;
    }

    colorkey_ex.key = colorkey.value;
    colorkey_ex.key_enable = colorkey.enable;

    if (gfbg_get_bits_per_pixel(info) <= 8) { /* 8 bits */
        if (drv_gfbg_set_clut_ck(info, &colorkey, &colorkey_ex) !=  TD_SUCCESS) {
            return TD_FAILURE;
        }
    } else {
        if (g_drv_ops.gfbg_drv_color_convert == TD_NULL) {
            return TD_FAILURE;
        }
#ifdef __LITEOS__
        g_drv_ops.gfbg_drv_color_convert(info, &colorkey_ex);
#else
        g_drv_ops.gfbg_drv_color_convert(&info->var, &colorkey_ex);
#endif
    }

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = TD_TRUE;
    gfbg_set_key(par, &colorkey_ex);
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_COLORKEY;
    par->modifying = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_layer_alpha(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_alpha alpha = {0};

    par = (gfbg_par *)info->par;
    gfbg_get_alpha(par, &alpha);
    return osal_copy_to_user(argp, &alpha, sizeof(ot_fb_alpha));
}

static td_s32 drv_gfbg_set_layer_alpha(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    unsigned long lock_flag;
    ot_fb_alpha alpha = {0};

    par = (gfbg_par *)info->par;
    if (osal_copy_from_user(&alpha, argp, sizeof(ot_fb_alpha))) {
        return -EFAULT;
    }

    if ((alpha.alpha_en != TD_TRUE) && (alpha.alpha_en != TD_FALSE)) {
        gfbg_error("alpha->alpha_en should be TRUE or FALSE!\n");
        return TD_FAILURE;
    }

    if ((alpha.alpha_chn_en != TD_TRUE) && (alpha.alpha_chn_en != TD_FALSE)) {
        gfbg_error("alpha->alpha_chn_en should be TRUE or FALSE!\n");
        return TD_FAILURE;
    }
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = TD_TRUE;
    gfbg_set_alpha(par, &alpha);
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_ALPHA;

    par->modifying = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_screen_origin_pos(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;

    ot_fb_point pos = {0};

    par = (gfbg_par *)info->par;
    gfbg_get_layerpos(par, &pos);

    return osal_copy_to_user(argp, &pos, sizeof(ot_fb_point));
}

static td_s32 drv_gfbg_set_screen_origin_pos(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_point expected_pos;
    td_u32 layer_id;
    unsigned long lock_flag;
    td_bool is_interlace;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;
    is_interlace = gfbg_is_interlace(par);

    if (is_cursor_layer(layer_id) && is_soft_cursor()) {
        gfbg_error("you shouldn't set soft cursor origin by this cmd, try FBIOPUT_CURSOR_POS\n");
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&expected_pos, argp, sizeof(ot_fb_point))) {
        return -EFAULT;
    }

    if (expected_pos.x_pos < 0 || expected_pos.y_pos < 0) {
        gfbg_error("It's not supported to set start pos of layer to negative!\n");
        return TD_FAILURE;
    }

    if (is_interlace && (expected_pos.y_pos % 2 != 0)) { /* 2 alg data */
        gfbg_error("y_pos should be even for interlace vodev!\n");
        return TD_FAILURE;
    }

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = TD_TRUE;

    /* Record the old location first */
    gfbg_set_layerpos(par, &expected_pos);
    par->modifying = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_deflicker(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    par = (gfbg_par *)info->par;
    ot_unused(arg);
    if (!g_drv_ops.capability[par->layer_id].hor_deflicker_level &&
        !g_drv_ops.capability[par->layer_id].ver_deflicker_level) {
        gfbg_error("deflicker is not supported!\n");
        return -EPERM;
    }

    /* There is currently no chip that needs to support this feature, so comment out first. */
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_deflicker(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    par = (gfbg_par *)info->par;
    ot_unused(arg);
    if (!g_drv_ops.capability[par->layer_id].hor_deflicker_level &&
        !g_drv_ops.capability[par->layer_id].ver_deflicker_level) {
        gfbg_error("deflicker is not supported!\n");
        return -EPERM;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_vblank(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    par = (gfbg_par *)info->par;
    ot_unused(arg);
    if (gfbg_wait_regconfig_work(par->layer_id)) {
        gfbg_error("It is not support VBL!\n");
        return -EPERM;
    }

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_show_layer(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    td_bool is_show = TD_FALSE;
    unsigned long lock_flag;
    td_u32 layer_id;

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;
    if (is_cursor_layer(layer_id) && is_soft_cursor()) {
        gfbg_error("you shouldn't show sot cursor by this cmd, try FBIOPUT_CURSOR_STATE!\n");
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&is_show, argp, sizeof(td_bool))) {
        return -EFAULT;
    }

    if ((is_show != TD_TRUE) && (is_show != TD_FALSE)) {
        gfbg_error("show(%d) should be TRUE or FALSE!\n", is_show);
        return TD_FAILURE;
    }

    if (is_show == gfbg_get_show(par)) {
        gfbg_info("The layer is show(%d) now!\n", par->show);


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART06_H_ */
