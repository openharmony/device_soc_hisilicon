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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART07_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART07_H_

        return TD_SUCCESS;
    }
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = TD_TRUE;

    gfbg_set_show(par, is_show);
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_SHOW;

    par->modifying = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_layer_show_state(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    td_bool is_show;
    par = (gfbg_par *)info->par;
    is_show = par->show;
    return osal_copy_to_user(argp, &is_show, sizeof(td_bool));
}

static td_s32 drv_gfbg_get_capability(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_capability capability = {0};

    par = (gfbg_par *)info->par;
    capability = g_drv_ops.capability[par->layer_id];
    return osal_copy_to_user(argp, (td_void *)&capability, sizeof(ot_fb_capability));
}

static td_s32 drv_gfbg_set_sursor_info(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_cursor cursor_info;

    par = (gfbg_par *)info->par;
    if (par == TD_NULL) {
        gfbg_error("par is NULL!\n");
        return TD_FAILURE;
    }
    (td_void)memset_s(&cursor_info, sizeof(ot_fb_cursor), 0, sizeof(ot_fb_cursor));
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't detach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&cursor_info, argp, sizeof(ot_fb_cursor))) {
        return -EFAULT;
    }

    if (gfbg_check_cusor_phyaddr(&cursor_info.cursor) == TD_FAILURE) {
        gfbg_error("cursor mmz phy addr 0x%lx invalid.\n", cursor_info.cursor.phys_addr);
        return TD_FAILURE;
    }

    if (ot_fb_cursor_putinfo(par, &cursor_info) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_get_sursor_info(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    gfbg_cursor_info cursor_info = {0};
    ot_fb_cursor cursor = {0};

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't detach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }
    gfbg_get_cursorinfo(par, &cursor_info);
    cursor = cursor_info.cursor;
    return osal_copy_to_user(argp, &cursor, sizeof(ot_fb_cursor));
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_set_cursor_state(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    td_bool is_show;

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't detach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&is_show, argp, sizeof(td_bool))) {
        return -EFAULT;
    }

    if (ot_fb_cursor_changestate(par, is_show) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_get_cursor_state(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    td_bool is_show;

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't detach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }

    is_show = gfbg_get_show(par);
    return osal_copy_to_user(argp, &is_show, sizeof(td_bool));
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_set_cursor_pos(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_point pos = {0};

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't detach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&pos, argp, sizeof(ot_fb_point))) {
        return -EFAULT;
    }

    if (ot_fb_cursor_changepos(par->layer_id, pos) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_get_cursor_pos(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_point pos = {0};

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't detach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }

    gfbg_get_layerpos(par, &pos);

    return osal_copy_to_user(argp, &pos, sizeof(ot_fb_point));
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_set_cursor_colorkey(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_colorkey colorkey;
    gfbg_colorkeyex colorkey_ex;
    ot_fb_color_format color_format;

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't detach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }
    if (osal_copy_from_user(&colorkey, argp, sizeof(ot_fb_colorkey))) {
        return -EFAULT;
    }

    colorkey_ex.key = colorkey.value;
    colorkey_ex.key_enable = colorkey.enable;
    gfbg_get_fmt(par, &color_format);
    if (g_drv_ops.gfbg_drv_color_convert == TD_NULL) {
        gfbg_error("ptr is NULL!\n");
        return TD_FAILURE;
    }
#ifdef __LITEOS__
    g_drv_ops.gfbg_drv_color_convert(info, &colorkey_ex);
#else
    g_drv_ops.gfbg_drv_color_convert(&info->var, &colorkey_ex);
#endif

    gfbg_set_key(par, &colorkey_ex);

    return TD_SUCCESS;
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_get_cursor_colorkey(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_colorkey colorkey = {0};
    gfbg_colorkeyex colorkey_ex = {0};

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't detach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }
    gfbg_get_key(par, &colorkey_ex);

    colorkey.enable = colorkey_ex.key_enable;
    colorkey.value = colorkey_ex.key;
    return osal_copy_to_user(argp, &(colorkey), sizeof(ot_fb_colorkey));
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_set_cursor_alpha(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_alpha alpha = {0};

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't set hard cursor alpha by this cmd!\n");
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&alpha, argp, sizeof(ot_fb_alpha))) {
        return -EFAULT;
    }

    if ((alpha.alpha_en != TD_TRUE) && (alpha.alpha_en != TD_FALSE)) {
        gfbg_error("pstAlphaInfo->alpha_en(%d) should be TRUE or FALSE!\n", alpha.alpha_en);
        return TD_FAILURE;
    }

    if ((alpha.alpha_chn_en != TD_TRUE) && (alpha.alpha_chn_en != TD_FALSE)) {
        gfbg_error("pstAlphaInfo->alpha_chn_en(%d) should be TRUE or FALSE!\n", alpha.alpha_chn_en);
        return TD_FAILURE;
    }

    gfbg_set_alpha(par, &alpha);

    return TD_SUCCESS;
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_get_cursor_alpha(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_alpha alpha = {0};

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't detach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }
    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }
    gfbg_get_alpha(par, &alpha);

    return osal_copy_to_user(argp, &alpha, sizeof(ot_fb_alpha));
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_set_attch_cursor(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    td_u32 layer_id = 0;

    par = (gfbg_par *)info->par;
    if (!is_soft_cursor()) {
        gfbg_error("you shouldn't attach hard cursor by this cmd!\n");
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        gfbg_error("layer %u is not cursor layer!\n", par->layer_id);
        return TD_FAILURE;
    }
    if (osal_copy_from_user(&layer_id, argp, sizeof(td_u32))) {
        return -EFAULT;
    }
    if ((layer_id >= g_drv_ops.layer_count) || (g_drv_ops.capability[layer_id].is_layer_support == TD_FALSE)) {
        gfbg_error("LayerId %u should be in [0, %u) or it is not supported!\n", layer_id, g_drv_ops.layer_count);
        return -EFAULT;
    }

    if (is_cursor_layer(layer_id)) {
        gfbg_error("Don't allow attach cursor to itself or other cursor layer!\n");
        return TD_FAILURE;
    }

    return ot_fb_cursor_attach(par->layer_id, layer_id);
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the soft cursor operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_set_detach_cursor(struct fb_info *info, unsigned long arg)
{
#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART07_H_ */
