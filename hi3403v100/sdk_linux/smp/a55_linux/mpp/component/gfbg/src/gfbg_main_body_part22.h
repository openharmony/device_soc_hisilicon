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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART22_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART22_H_

    cmp_reg.transp = 0xffff;
    red = cmap->red;
    green = cmap->green;
    blue = cmap->blue;
    transp = cmap->transp;

#ifdef __LITEOS__
    start = cmap->first;
#else
    start = cmap->start;
#endif

    for (i = 0; i < cmap->len; i++) {
        cmp_reg.red   = *red++;
        cmp_reg.green = *green++;
        cmp_reg.blue  = *blue++;
        if (transp != TD_NULL) {
            cmp_reg.transp = *transp++;
        }
        cmp_reg.regno = start;
        if (i < cmap->len - 1) {
            if (gfbg_dosetcolreg(&cmp_reg, info, TD_FALSE)) {
                break;
            }
        } else {   /* the last time update register */
            if (gfbg_dosetcolreg(&cmp_reg, info, TD_TRUE)) {
                break;
            }
        }
        start++;
    }
    return;
}

static td_s32 gfbg_setcmap(struct fb_cmap *cmap, struct fb_info *info)
{
    unsigned long lock_flag;
    gfbg_par *par = (gfbg_par *)info->par;
    if ((is_sd_layer(par->layer_id)) == TD_FALSE) {
        return -EINVAL;
    }

    if (g_drv_ops.capability[par->layer_id].is_cmap == TD_FALSE) {
        /* AE6D03519, delete this color map warning! */
        return -EPERM;
    }

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = TD_TRUE;
    gfbg_set_color_reg(cmap, info);
    par->param_modify_mask |= GFBG_LAYER_PARAMODIEY_CLUT_UP;
    par->modifying = TD_FALSE;
    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    return 0;
}

#ifdef CONFIG_FB_CFB_IMAGEBLIT
static td_void gfbg_imageblit(struct fb_info *p, const struct fb_image *image)
{
    cfb_imageblit(p, image);
}
#endif

#ifdef __LITEOS__
#define GFBG_MAX_LAYER_NUM  1
static struct gfbg_info *g_gfbg[GFBG_MAX_LAYER_NUM] = {TD_NULL};

static td_s32 copy_rgb_to_cmap(td_u8 *cmap_start, const struct fb_cmap_s *src, struct fb_cmap_s *dst)
{
    td_s32 offset;
    td_s32 copy_size;
    int cmap_size;

    cmap_size = dst->len * sizeof(u_short);
    offset = src->first - dst->first;
    copy_size = (src->len > (dst->len - offset)) ? (dst->len - offset) : src->len;

    if (copy_size <= 0) {
        return -EINVAL;
    }
    copy_size *= sizeof(u_short);

    ret = memcpy_s(cmap_start + offset, copy_size, src->red, copy_size);
    gfbg_unequal_eok_return(ret);
    cmap_start += cmap_size;
    ret = memcpy_s(cmap_start + offset, copy_size,  src->green, copy_size);
    gfbg_unequal_eok_return(ret);
    cmap_start += cmap_size;
    ret = memcpy_s(cmap_start + offset, copy_size, src->blue, copy_size);
    gfbg_unequal_eok_return(ret);
    cmap_start += cmap_size;
    if (src->transp && dst->transp) {
        ret = memcpy_s(cmap_start + offset, copy_size, src->transp, copy_size);
        gfbg_unequal_eok_return(ret);
    }
    return TD_SUCCESS;
}

static int fb_copy_cmap(const struct fb_cmap_s *src, struct fb_cmap_s *dst)
{
    int offset;
    int copy_size;
    td_u8 *cmap_start = TD_NULL;
    int cmap_size;
    int ret;

    if ((src == TD_NULL) || (dst == TD_NULL)) {
        return -EINVAL;
    }

    cmap_start = dst->red;
    if (cmap_start == TD_NULL)
        return -EINVAL;

    cmap_size = dst->len * sizeof(u_short);

    if (src->first > dst->first) {
        ret = copy_rgb_to_cmap(cmap_start, src, dst);
        if (ret != TD_SUCCESS) {
            return TD_FAILURE;
        }
    } else {
        offset = dst->first - src->first;
        copy_size = ((src->len - offset) > dst->len) ? dst->len : (src->len - offset);

        if (copy_size <= 0) {
            return -EINVAL;
        }
        copy_size *= sizeof(u_short);

        ret = memcpy_s(cmap_start, copy_size, src->red + offset, copy_size);
        gfbg_unequal_eok_return(ret);
        cmap_start += cmap_size;
        ret = memcpy_s(cmap_start, copy_size, src->green + offset, copy_size);
        gfbg_unequal_eok_return(ret);
        cmap_start += cmap_size;
        ret = memcpy_s(cmap_start, copy_size, src->blue + offset, copy_size);
        gfbg_unequal_eok_return(ret);
        cmap_start += cmap_size;
        if (src->transp && dst->transp) {
            ret = memcpy_s(cmap_start, copy_size, src->transp + offset, copy_size);
            gfbg_unequal_eok_return(ret);
        }
    }
    return 0;
}

int fb_set_cmap(struct fb_cmap_s *cmap, struct gfbg_info *info)
{
    int ret;

    if (cmap == TD_NULL) {
        return -EINVAL;
    }

    if ((cmap->first < 0) || (info == TD_NULL))
        return -EINVAL;

    ret = gfbg_setcmap(cmap, info);
    if (ret == 0)
        fb_copy_cmap(cmap, &info->cmap);

    return ret;
}

FAR struct fb_vtable_s *up_fbgetvplane(int display, int vplane)
{
    (void)vplane;
    if (g_gfbg[display] == TD_NULL)
        return TD_NULL;

    return &g_gfbg[display]->vtable;
}

static int gfbg_getvideoinfo(FAR struct fb_vtable_s *vtable, FAR struct fb_videoinfo_s *vinfo)
{
    struct gfbg_info *info = (struct gfbg_info *)vtable;
    td_s32 ret;

    if ((info == TD_NULL) || (vinfo == TD_NULL))
        return -EINVAL;

    ret = memcpy_s(vinfo, sizeof(struct fb_videoinfo_s), &info->vinfo, sizeof(struct fb_videoinfo_s));
    gfbg_unequal_eok_return(ret);
    return 0;
}

static int gfbg_getplaneinfo(FAR struct fb_vtable_s *vtable, int planeno, FAR struct fb_planeinfo_s *pinfo)
{
    struct gfbg_info *info = (struct gfbg_info *)vtable;

    if ((info == TD_NULL) || (pinfo == TD_NULL))
        return -EINVAL;

    pinfo->fbmem = info->oinfo.fbmem;
    pinfo->fblen = info->oinfo.fblen;
    pinfo->display = info->oinfo.overlay;
    pinfo->bpp = info->oinfo.bpp;
    pinfo->stride = info->oinfo.stride;

    return 0;
}

static int gfbg_getoverlayinfo(FAR struct fb_vtable_s *vtable, int overlayno, FAR struct fb_overlayinfo_s *oinfo)
{
    struct gfbg_info *info = (struct gfbg_info *)vtable;
    td_s32 ret;

    if ((info == TD_NULL) || (oinfo == TD_NULL))
        return -EINVAL;

    ret = memcpy_s(oinfo, sizeof(struct fb_overlayinfo_s), &info->oinfo, sizeof(struct fb_overlayinfo_s));
    gfbg_unequal_eok_return(ret);

    return 0;
}

static int gfbg_getcmap(FAR struct fb_vtable_s *vtable, FAR struct fb_cmap_s *cmap)
{
    struct gfbg_info *info = (struct gfbg_info *)vtable;

    if ((info == TD_NULL) || (cmap == TD_NULL))
        return -EINVAL;

    return fb_copy_cmap(&info->cmap, cmap);
}

static int gfbg_putcmap(FAR struct fb_vtable_s *vtable, FAR const struct fb_cmap_s *cmap)
{
    struct gfbg_info *info = (struct gfbg_info *)vtable;
    int cpopy_size;

    if ((info == TD_NULL) || (cmap == TD_NULL))
        return -EINVAL;

    if ((cpopy_size = cmap->len * sizeof(td_u16)) < 0)
        return -EINVAL;

    return fb_set_cmap(cmap, info);
}

static td_s32 fbinitialize_alloc_mem(struct gfbg_info *info)
{
    td_char name[16] = {'\0'}; /* 16 name max length */
    gfbg_par *par = TD_NULL;
    par = (gfbg_par *)(info->par);

    if (g_layer[par->layer_id].layer_size != 0) {
        /* initialize the fix screen info */
        if (is_4k_layer(par->layer_id)) {
            gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_4K]);
        } else if (is_hd_layer(par->layer_id)) {
            gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_HD]);
        } else if (is_sd_layer(par->layer_id)) {
            gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_SD]);
        } else if (is_ad_layer(par->layer_id)) {
            gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_AD]);
        } else if (is_cursor_layer(par->layer_id)) {
            gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_CURSOR]);
        }
        if (snprintf_s(name, sizeof(name), 12, "gfbg_layer%01u", par->layer_id) < 0) { /* 12:for char length */
            gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
        }

        info->oinfo.fbmem = (void *)gfbg_buf_allocmem(name, (g_layer[par->layer_id].layer_size +
            /* 1024 alg data */
            g_layer[par->layer_id].curosr_buf_size) * 1024, g_layer_mmz_names[par->layer_id]);
        if (info->oinfo.fbmem == TD_NULL) {
            gfbg_error("%s:failed to malloc the video memory, size: %ld KBtyes!\n", name,
                (g_layer[par->layer_id].layer_size + g_layer[par->layer_id].curosr_buf_size));
            gfbg_free(info);
            g_layer[par->layer_id].info = TD_NULL;
            return TD_FAILURE;
        }
        /* 1024:u32LayerSize is KB */
        info->oinfo.fblen = g_layer[par->layer_id].layer_size * 1024;

        /* initialize the virtual address and clear memory */
        info->oinfo.fbmem = gfbg_buf_map((td_u32)(uintptr_t)info->oinfo.fbmem,
                                         (g_layer[par->layer_id].layer_size +
                                         g_layer[par->layer_id].curosr_buf_size) * 1024); /* 1024 alg data */
        if (info->oinfo.fbmem == TD_NULL) {
            gfbg_warning("Failed to call map video memory,size:0x%x, start: 0x%lx\n", info->oinfo.fblen,
                         info->oinfo.fbmem);
        }

        (td_void)memset_s(info->oinfo.fbmem, info->oinfo.fblen, 0, info->oinfo.fblen);

        /* alloc color map */
        if (g_drv_ops.capability[par->layer_id].is_cmap) {
            /* now unsupported cmap function */
        }
    }
    return TD_SUCCESS;
}

static int up_fbinitialize(int display)
{
    td_s32 ret;
    struct gfbg_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;

    /* Creates a new frame buffer info structure. reserves gfbg_par for driver private data (info->par) */
    info = gfbg_alloc(sizeof(gfbg_par));
    if (info == TD_NULL) {
        gfbg_error("failed to malloc the gfbg_info!\n");
        return -ENOMEM;
    }

    /* save the info pointer in global pointer array */
    g_layer[display].info = info;

    info->oinfo.overlay = display;
    info->vtable.fb_open = gfbg_open;
    info->vtable.fb_release = gfbg_release;
    info->vtable.fb_set_par = gfbg_set_par;
    info->vtable.fb_pan_display = gfbg_pan_display;
    info->vtable.fb_ioctl = gfbg_ioctl;
    info->vtable.getvideoinfo = gfbg_getvideoinfo;
    info->vtable.getplaneinfo = gfbg_getplaneinfo;
    info->vtable.getoverlayinfo = gfbg_getoverlayinfo;
    info->vtable.getcmap = TD_NULL;
    info->vtable.putcmap = TD_NULL;

    par = (gfbg_par *)(info->par);
    (td_void)memset_s(par, sizeof(gfbg_par), 0, sizeof(gfbg_par));
    par->layer_id = display;
    par->color_format = OT_FB_FORMAT_ARGB1555;
    info->vinfo.format = OT_FB_FORMAT_ARGB1555;

    /* It's not need to alloc mem for cursor layer if use g_softcursor */
    ret = fbinitialize_alloc_mem(info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    g_gfbg[display] = info;

    return TD_SUCCESS;
}

static td_void up_fbuninitialize(td_s32 display)
{
    struct gfbg_info* info = TD_NULL;

    /* get framebuffer info structure pointer */
    info = g_layer[display].info;
    if (info != TD_NULL) {
        if (info->oinfo.fbmem) {
            gfbg_buf_ummap(info->oinfo.fbmem);
        }

        if (info->oinfo.fbmem) {
            gfbg_buf_freemem((td_u32)(uintptr_t)info->oinfo.fbmem);
        }

        gfbg_free(info);
        g_layer[display].info = TD_NULL;
        g_gfbg[display] = TD_NULL;
    }
}
#endif

static td_s32 gfbg_set_dynamic_range(gfbg_par *par, ot_fb_dynamic_range dynamic_range)
{
    if (dynamic_range >= OT_FB_DYNAMIC_RANGE_BUTT) {
        gfbg_error("The input dynamic range value is wrong!\n");
        return -EINVAL;
    }

    par->display_info.dynamic_range = dynamic_range;
    return TD_SUCCESS;
}

/*
 * Name : g_ot_fb_ops
 * Desc : fb struct
 * See  : gfbg_overlay_probe
 */
#ifndef __LITEOS__
static struct fb_ops g_ot_fb_ops = {
    .owner                 = THIS_MODULE,
    .fb_open               = gfbg_open,
    .fb_release           = gfbg_release,
    .fb_check_var       = gfbg_check_var,
    .fb_set_par           = gfbg_set_par,
    .fb_pan_display    = gfbg_pan_display,
    .fb_ioctl               = gfbg_ioctl,
    .fb_setcolreg        = gfbg_setcolreg,
    .fb_setcmap         = gfbg_setcmap,
#ifdef CONFIG_COMPAT
    .fb_compat_ioctl   = gfbg_compat_ioctl,
#endif
#ifdef CONFIG_FB_CFB_IMAGEBLIT
    .fb_imageblit      = gfbg_imageblit,
#endif

};
#endif

/*
 * Function        : gfbg_overlay_cleanup
 * Description     : releae the resource for certain framebuffer
 */


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART22_H_ */
