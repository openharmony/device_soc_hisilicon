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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART09_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART09_H_


    /* check surface is value or not */
    if (flip_surface_check_param(info, &surface_ex) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* refresh and color convert */
    if (flip_surface_pan_display(info, &surface_ex, &colorkey_ex) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = TD_TRUE;
    gfbg_set_alpha(par, &surface_ex.alpha);
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_ALPHA;
    if (g_drv_ops.capability[par->layer_id].is_key_rgb || g_drv_ops.capability[par->layer_id].is_key_alpha) {
        gfbg_set_key(par, &colorkey_ex);
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_COLORKEY;
    }
    par->modifying = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    return TD_SUCCESS;
}

static td_s32 set_compression_process(const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    ot_fb_layer_buf buf_mode;
    unsigned long lock_flag;
    td_u32 buf_size;
    td_u32 cmp_stride;
    gfbg_stride_attr attr = {0};

    par = (gfbg_par *)info->par;
    compress_info = &par->compress_info;
    attr.format = par->color_format;
    attr.width = par->display_info.display_width;
    cmp_stride = 0;
    if (par->color_format == OT_FB_FORMAT_ARGB1555 || par->color_format == OT_FB_FORMAT_ARGB4444) {
        gfbg_recalculate_stride(&cmp_stride, NULL, &attr);
        buf_size = ((cmp_stride * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
        if (gfbg_get_smem_len(info) < (buf_size * 2)) { /* 2 for cal */
            gfbg_error("Memory is too small. ARGB1555 or ARGB4444 compression needs to allocate more memory.\n");
            gfbg_error("Memory should be greater than or equal to %d byte\n", (buf_size * 2)); /* 2 for cal */
            return TD_FAILURE;
        }
    }

    if (par->color_format != OT_FB_FORMAT_ARGB8888 && par->color_format != OT_FB_FORMAT_ARGB1555 &&
        par->color_format != OT_FB_FORMAT_ARGB4444) {
        gfbg_error("compression only support pixel format (ARGB8888,ARGB1555,ARGB4444)\n");
        return TD_FAILURE;
    }
    if (par->display_info.mirror_mode != OT_FB_MIRROR_NONE || par->display_info.rotate_mode != OT_FB_ROTATE_NONE) {
        gfbg_error("Can't do compression when mirror or rotate!\n");
        return TD_FAILURE;
    }
    gfbg_get_bufmode(par, &buf_mode);

    /* FB uses frame decompression, can not be displayed while refreshing, so only supports double buf mode */
    if ((buf_mode != OT_FB_LAYER_BUF_DOUBLE) && (buf_mode != OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        gfbg_error("only FB_LAYER_BUF_DOUBLE or FB_LAYER_BUF_DOUBLE_IMMEDIATE mode support compress!\n");
        return TD_FAILURE;
    }

    /* Record the entire image as the area to be compressed */
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    compress_info->compress_rect.x = 0;
    compress_info->compress_rect.y = 0;
    compress_info->compress_rect.width = par->display_info.display_width;
    compress_info->compress_rect.height = par->display_info.display_height;
    compress_info->update_finished = TD_TRUE;
    compress_info->delay = TD_TRUE;
    compress_info->compress_open = TD_TRUE;
    compress_info->is_losslessa = TD_FALSE;
    compress_info->is_lossless = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    return TD_SUCCESS;
}

static td_s32 set_compression_start(const struct fb_info *info, td_bool is_compress)
{
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    unsigned long lock_flag;
    par = (gfbg_par *)info->par;
    compress_info = &par->compress_info;
    if (is_compress != TD_TRUE && is_compress != TD_FALSE) {
        gfbg_error("compress(%d) should be TRUE or FALSE!\n", is_compress);
        return TD_FAILURE;
    }
    if ((compress_info->is_economize_memory) == TD_TRUE && (is_compress != TD_TRUE)) {
        gfbg_error("memory not enough, should be start compress!\n");
        return TD_FAILURE;
    }
    if (is_compress != compress_info->compress_open) {
        if (is_compress) {
            /*
             * FB uses frame decompression, can not be displayed while refreshing
             * so only supports double buf mode
             * Record the entire image as the area to be compressed
             */
            if (set_compression_process(info) != TD_SUCCESS) {
                return TD_FAILURE;
            }
        } else {
            gfbg_spin_lock_irqsave(&par->lock, lock_flag);
            par->param_modify_mask |= GFBG_LAYER_PARAMODIEY_COMPRESS;
            compress_info->compress_open = TD_FALSE;
            compress_info->is_losslessa = TD_FALSE;
            compress_info->is_lossless = TD_FALSE;
            gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
            /* Waiting for an interrupt before proceeding to the next two operations */
            gfbg_wait_regconfig_work(par->layer_id);
            /* Reset compression information */
            compress_info->update_rect.width = 0;
            compress_info->update_rect.height = 0;
        }
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_compression_mode(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    td_bool is_compress = TD_FALSE;
    par = (gfbg_par *)info->par;
    if (!g_drv_ops.capability[par->layer_id].is_decompress) {
        gfbg_error("Layer %d doesn't support compression operation!\n", par->layer_id);
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&is_compress, argp, sizeof(td_bool))) {
        return -EFAULT;
    }

    /* set compress config */
    if (set_compression_start(info, is_compress) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_compression_mode(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    td_bool is_compress = TD_FALSE;
    par = (gfbg_par *)info->par;
    compress_info = &par->compress_info;

    if (!g_drv_ops.capability[par->layer_id].is_decompress) {
        gfbg_warning("Layer %d doesn't support get compression!\n", par->layer_id);
    } else {
        is_compress = compress_info->compress_open;
    }
    return osal_copy_to_user(argp, &is_compress, sizeof(td_bool));
}

static td_s32 drv_gfbg_set_mddrdetect(struct fb_info *info, unsigned long arg)
{
#ifdef MDDRDETECT
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    ot_fb_ddr_zone ddr_zone_para = {0};
    unsigned long lock_flag;

    par = (gfbg_par *)info->par;
    compress_info = &par->compress_info;
    if ((g_drv_ops.gfbg_drv_is_layer_support_ddr_detect == TD_NULL) ||
        (g_drv_ops.gfbg_drv_check_ddr_dectect_zone == TD_NULL)) {
        gfbg_error("ptr is NULL\n");
        return TD_FAILURE
    }
    if (g_drv_ops.gfbg_drv_is_layer_support_ddr_detect(par->layer_id) == TD_FALSE) {
        gfbg_error("Layer %d doesn't support DDR detect PUT.\n", par->layer_id);
        return TD_FAILURE;
    }

    if (!g_drv_ops.capability[par->layer_id].is_decompress) {
        gfbg_error("Layer %d doesn't support DDR detect!\n", par->layer_id);
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&ddr_zone_para, argp, sizeof(ot_fb_ddr_zone))) {
        return -EFAULT;
    }

    /* check detect zone */
    if (g_drv_ops.gfbg_drv_check_ddr_dectect_zone(par->layer_id, ddr_zone_para.start_section,
        ddr_zone_para.zone_nums, compress_info->start_section, compress_info->zone_nums) == TD_FAILURE) {
        gfbg_error("Layer %d DDR detect zone ERR!\n", par->layer_id);
        return TD_FAILURE;
    }

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);

    /* If you repeat the settings, return directly */
    if (ddr_zone_para.start_section == compress_info->start_section &&
        ddr_zone_para.zone_nums == compress_info->zone_nums) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

        return TD_SUCCESS;
    }

    compress_info->new_start_section = ddr_zone_para.start_section;
    compress_info->new_zone_nums = ddr_zone_para.zone_nums;
    compress_info->clear_zone = (ddr_zone_para.zone_nums == 0) ? TD_TRUE : TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    return TD_SUCCESS;
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the set mddrdetect operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_get_mddrdetect(struct fb_info *info, unsigned long arg)
{
#ifdef MDDRDETECT
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    ot_fb_ddr_zone ddr_zone_para = {0};
    unsigned long lock_flag;
    par = (gfbg_par *)info->par;
    compress_info = &par->compress_info;
    if (g_drv_ops.gfbg_drv_is_layer_support_ddr_detect == TD_NULL) {
        gfbg_error("ptr is NULL\n");
        return TD_FAILURE
    }
    if (g_drv_ops.gfbg_drv_is_layer_support_ddr_detect(par->layer_id) == TD_FALSE) {
        gfbg_error("Layer %d doesn't support DDR detect GET.\n", par->layer_id);
        return TD_FAILURE;
    }
    if (!g_drv_ops.capability[par->layer_id].is_decompress) {
        gfbg_error("Layer %d doesn't support DDR detect!\n", par->layer_id);
        return TD_FAILURE;
    }

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    ddr_zone_para.start_section = compress_info->new_start_section;
    ddr_zone_para.zone_nums = compress_info->new_zone_nums;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    return osal_copy_to_user(argp, &ddr_zone_para, sizeof(ot_fb_ddr_zone));
#else
    ot_unused(info);
    ot_unused(arg);
    gfbg_error("the get mddrdetect operation is unsupported!\n");
    return TD_FAILURE;
#endif
}

static td_s32 drv_gfbg_set_dynamic_range(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_dynamic_range dynamic_range = OT_FB_DYNAMIC_RANGE_BUTT;
    par = (gfbg_par *)info->par;
    if (g_drv_ops.capability[par->layer_id].is_ghdr == TD_FALSE) {
        gfbg_error("Layer %d# doesn't support HDR function.\n", par->layer_id);
        return TD_FAILURE;
    }
    if (osal_copy_from_user(&dynamic_range, argp, sizeof(ot_fb_dynamic_range))) {
        return -EFAULT;
    }
    if (gfbg_set_dynamic_range(par, dynamic_range) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_dynamic_range(struct fb_info *info, unsigned long arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    gfbg_par *par = TD_NULL;
    ot_fb_dynamic_range dynamic_range;
    gfbg_display_info *display_info = TD_NULL;
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    if (g_drv_ops.capability[par->layer_id].is_ghdr == TD_FALSE) {
        gfbg_error("Layer %d# doesn't support HDR function.\n", par->layer_id);
        return TD_FAILURE;
    }

    dynamic_range = display_info->dynamic_range;
    if (osal_copy_to_user(argp, &dynamic_range, sizeof(ot_fb_dynamic_range))) {
        return -EFAULT;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_create(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    unsigned long lock_flag;
    ot_unused(arg);
    par = (gfbg_par *)info->par;
    if (par == TD_NULL) {
        gfbg_error("par is NULL failed!\n");
        return TD_FAILURE;
    }
    layer_id = par->layer_id;

    /* when unbind/bind  */
    if (g_drv_ops.gfbg_open_layer(layer_id) != TD_SUCCESS) {
        gfbg_error("Open graphic layer %u# failed!\n", layer_id);
        return TD_FAILURE;
    }

    if (g_drv_ops.gfbg_drv_set_layer_enable(layer_id, TD_TRUE) != TD_SUCCESS) {
        gfbg_error("enable graphic layer %u# failed!\n", layer_id);
        return TD_FAILURE;
    }
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->show = TD_TRUE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_release(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    unsigned long lock_flag;
    ot_unused(arg);
    par = (gfbg_par *)info->par;
    if (par == TD_NULL) {
        gfbg_error("par is NULL failed!\n");
        return TD_FAILURE;
    }
    layer_id = par->layer_id;

    if (g_drv_ops.gfbg_drv_set_layer_enable(layer_id, TD_FALSE) != TD_SUCCESS) {
        gfbg_error("disable graphic layer %u# failed!\n", layer_id);
        return TD_FAILURE;
    }
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->show = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_draw_frame(struct fb_info *info, td_ulong arg)
{
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
    td_void __user *frame = NULL;
    gfbg_par *par = NULL;
    ot_fb_smart_rect_param frame_param = {0};
    td_ulong lock_flag;
    td_char *vir = TD_NULL;
    par = (gfbg_par *)info->par;
    if (g_drv_ops.capability[par->layer_id].is_osb == TD_FALSE) {
        gfbg_error("Layer %d# doesn't support draw smart rect function.\n", par->layer_id);
        return TD_FAILURE;
    }
    if (osal_copy_from_user(&frame_param, argp, sizeof(ot_fb_smart_rect_param))) {
        return -EFAULT;
    }
    if (frame_param.num > 128) { /* frame max is 128 */
        gfbg_error("actual %d,support draw smart rect max 128.\n", frame_param.num);
        return TD_FAILURE;
    }
    if (frame_param.rect_start == NULL) {
        return TD_FAILURE;
    }
    vir = kmalloc(sizeof(ot_fb_smart_rect) * 128, GFP_KERNEL); /* frame max is 128 */
    if (vir == TD_NULL) {
        return TD_FAILURE;
    }
    frame = (td_void __user *)frame_param.rect_start;
    if (osal_copy_from_user(vir, frame, (td_ulong)(sizeof(ot_fb_smart_rect) * frame_param.num))) {
        kfree(vir);
        return -EFAULT;
    }
    frame_param.rect_start = (ot_fb_smart_rect *)vir;
    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = TD_TRUE;
    if (gfbg_draw_smart_rect(info, &frame_param) != TD_SUCCESS) {
        gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
        kfree(vir);
        return TD_FAILURE;
    }
    par->param_modify_mask |= GFBG_LAYER_PARAMODIEY_SMART_RECT;
    par->modifying = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
    kfree(vir);
    return TD_SUCCESS;
}


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART09_H_ */
