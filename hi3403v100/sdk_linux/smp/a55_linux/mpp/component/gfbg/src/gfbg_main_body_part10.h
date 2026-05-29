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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART10_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART10_H_

#ifdef __LITEOS__
static td_s32 gfbg_ioctl_liteos_standard_io(struct fb_info *info, td_u32 cmd, unsigned long arg, td_bool *is_continue)
{
    td_s32 ret;
    /* for  FBIOPUT_SCREENINFO_GFBG */
    struct gfbg_info *info_temp = (struct gfbg_info *)arg;
    struct gfbg_info info_copy;
    /* for FBIOPAN_DISPLAY_GFBG */
    struct fb_overlayinfo_s *oinfo = (struct fb_overlayinfo_s *)arg;

    switch (cmd) {
        case FBIOGET_SCREENINFO_GFBG:
            ret = memcpy_s((void *)arg, sizeof(struct gfbg_info), info, sizeof(struct gfbg_info));
            gfbg_unequal_eok_return(ret);
            break;
        case FBIOPUT_SCREENINFO_GFBG:
            ret = memcpy_s(&info_copy, sizeof(info_copy), info, sizeof(info_copy));
            gfbg_unequal_eok_return(ret);
            if (info->vtable.fb_set_par) {
                info->vinfo.xres = info_temp->vinfo.xres;
                info->vinfo.yres = info_temp->vinfo.yres;
                info->vinfo.format = info_temp->vinfo.format;
                info->oinfo.sarea.width = info_temp->oinfo.sarea.width;
                info->oinfo.sarea.height = info_temp->oinfo.sarea.height;
                info->oinfo.sarea.x = info_temp->oinfo.sarea.x;
                info->oinfo.sarea.y = info_temp->oinfo.sarea.y;
                info->oinfo.bpp = info_temp->oinfo.bpp;
                if (info->vtable.fb_set_par(&info->vtable) != TD_SUCCESS) {
                    gfbg_error("Put screeninfo error! ret=%d\n", ret);
                    ret = memcpy_s(info, sizeof(info_copy), &info_copy, sizeof(info_copy));
                    gfbg_unequal_eok_return(ret);
                    return TD_FAILURE;
                }
                gfbg_pan_display(&info->vtable, &info->oinfo);
            }
            break;
        case FBIOPAN_DISPLAY_GFBG:
            if (info->vtable.fb_pan_display) {
                if (info->vtable.fb_pan_display(&info->vtable, oinfo) != TD_SUCCESS) {
                    gfbg_error("Put screeninfo error! ret=%d\n", ret);
                    ret = memcpy_s(info, sizeof(info_copy), &info_copy, sizeof(info_copy));
                    gfbg_unequal_eok_return(ret);
                    return TD_FAILURE;
                }
                info->oinfo.sarea.x = oinfo->sarea.x;
                info->oinfo.sarea.y = oinfo->sarea.y;
            }
            break;
        default:
            *is_continue = TD_TRUE;
            break;
    }
    return TD_SUCCESS;
}
#endif

/*
 * Function        : gfbg_ioctl
 * Description     : set the colorkey or alpha for overlay
 * Return          : return 0 if succeed, otherwise return error code
 */
static td_s32 gfbg_ioctl(struct fb_info *info, td_u32 cmd, unsigned long arg)
{
    td_u8 gfbg_cmd = OSAL_IOC_NR(cmd);
    td_void __user *argp = (td_void __user *)(td_uintptr_t)arg;
#ifdef __LITEOS__
    td_bool is_continue = TD_FALSE;
#endif
    if (gfbg_ioctl_check_param(info, cmd, argp) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* liteos deal with FBIOGET_SCREENINFO_GFBG\FBIOPUT_SCREENINFO_GFBG\FBIOPAN_DISPLAY_GFBG */
#ifdef __LITEOS__
    if (gfbg_ioctl_liteos_standard_io(info, cmd, arg, &is_continue) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (is_continue != TD_TRUE) {
        return TD_SUCCESS;
    }
#endif
    if ((gfbg_cmd < 1) || (gfbg_cmd >= DRV_GFBG_IOCTL_CMD_NUM_MAX) || (g_drv_gfbg_ctl_num[gfbg_cmd] < 1) ||
        (g_drv_gfbg_ctl_num[gfbg_cmd] >= DRV_GFBG_IOCTL_FUNC_ITEM_NUM_MAX)) {
        return TD_FAILURE;
    }
    if (g_drv_gfbg_ioctl_func[g_drv_gfbg_ctl_num[gfbg_cmd]].func == TD_NULL) {
        return TD_FAILURE;
    }
    if (cmd != g_drv_gfbg_ioctl_func[g_drv_gfbg_ctl_num[gfbg_cmd]].cmd) {
        gfbg_error("the command:0x%x is unsupported!\n", cmd);
        return TD_FAILURE;
    }
    return g_drv_gfbg_ioctl_func[g_drv_gfbg_ctl_num[gfbg_cmd]].func(info, arg);
}

#ifdef CONFIG_COMPAT
static td_s32 gfbg_compat_ioctl(struct fb_info *info, unsigned cmd, unsigned long arg)
{
    return gfbg_ioctl(info, cmd, arg);
}
#endif

static td_void gfbg_version(td_void)
{
    /* 80length:Use "strings gfbg.ko | grep "GFBG_MAIN_VERSION"" to get the version */
    td_char gfbg_version[80] =
        "GFBG_MAIN_VERSION[" mkmarcotostr(GFBG_MAIN_VERSION) "] Build Time[" __DATE__ ", "__TIME__ "]";
    gfbg_info("%s\n", gfbg_version);
}

static td_s32 gfbg_bitfieldcmp(struct fb_bitfield x, struct fb_bitfield y)
{
    if ((x.offset == y.offset) && (x.length == y.length) && (x.msb_right == y.msb_right)) {
        return 0;
    } else {
        return -1;
    }
}

static td_u32 gfbg_getbppbyfmt(ot_fb_color_format color_fmt)
{
    switch (color_fmt) {
        case OT_FB_FORMAT_RGB565:
        case OT_FB_FORMAT_KRGB444:
        case OT_FB_FORMAT_KRGB555:
        case OT_FB_FORMAT_ARGB4444:
        case OT_FB_FORMAT_ARGB1555:
        case OT_FB_FORMAT_RGBA4444:
        case OT_FB_FORMAT_RGBA5551:
        case OT_FB_FORMAT_ACLUT88:
        case OT_FB_FORMAT_BGR565:
        case OT_FB_FORMAT_ABGR1555:
        case OT_FB_FORMAT_ABGR4444:
        case OT_FB_FORMAT_KBGR444:
        case OT_FB_FORMAT_KBGR555:
            return 16; /* 16 bit width */
        case OT_FB_FORMAT_RGB888:
        case OT_FB_FORMAT_ARGB8565:
        case OT_FB_FORMAT_RGBA5658:
        case OT_FB_FORMAT_ABGR8565:
        case OT_FB_FORMAT_BGR888:
            return 24; /* 24 bit width */
        case OT_FB_FORMAT_KRGB888:
        case OT_FB_FORMAT_ARGB8888:
        case OT_FB_FORMAT_RGBA8888:
        case OT_FB_FORMAT_ABGR8888:
        case OT_FB_FORMAT_KBGR888:
            return 32; /* 32 bit width */
        case OT_FB_FORMAT_1BPP:
            return 1;
        case OT_FB_FORMAT_2BPP:
            return 2; /* 2 bit width */
        case OT_FB_FORMAT_4BPP:
            return 4; /* 4 bit width */
        case OT_FB_FORMAT_8BPP:
        case OT_FB_FORMAT_ACLUT44:
            return 8; /* 8 bit width */
        default:
            return 0;
    }
}

static ot_fb_color_format gfbg_getfmtbyargb(const struct fb_bitfield *red, const struct fb_bitfield *green,
                                            const struct fb_bitfield *blue, const struct fb_bitfield *transp,
                                            td_u32 color_depth)
{
    td_u32 i;
    td_u32 bpp;

    if ((red == TD_NULL) || (green == TD_NULL) || (blue == TD_NULL) || (transp == TD_NULL)) {
        return  OT_FB_FORMAT_BUTT;
    }

    /* not support color palette low than 8bit */
    if (color_depth < 2) { /* 2 bit */
        return  OT_FB_FORMAT_BUTT;
    }

    if (color_depth == 2) { /* 2 bit */
        return OT_FB_FORMAT_2BPP;
    }

    if (color_depth == 4) { /* 4 bit */
        return OT_FB_FORMAT_4BPP;
    }

    if (color_depth == 8) { /* 8 bit */
        return OT_FB_FORMAT_8BPP;
    }

    /*
     * Find the pixel format (gfbg_argb_bitinfo) corresponding to the given red,
     * green, and blue bit field information and the number of bits per pixel (bpp)
     */
    for (i = 0; i < sizeof(g_argb_bit_field) / sizeof(gfbg_argb_bitinfo); i++) {
        if ((gfbg_bitfieldcmp(*red, g_argb_bit_field[i].red) == 0) &&
            (gfbg_bitfieldcmp(*green, g_argb_bit_field[i].green) == 0) &&
            (gfbg_bitfieldcmp(*blue, g_argb_bit_field[i].blue) == 0) &&
            (gfbg_bitfieldcmp(*transp, g_argb_bit_field[i].transp) == 0)) {
            bpp = gfbg_getbppbyfmt(i);
            if (bpp == color_depth) {
                return i;
            }
        }
    }
    i = OT_FB_FORMAT_BUTT;
    return i;
}

static td_s32 gfbg_check_mem_enough(const struct fb_info *info, td_u32 pitch, td_u32 height)
{
    td_u32 buffer_num = 0;
    td_u32 buffer_size;
    gfbg_par *par = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    if (info->par == TD_NULL) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    refresh_info = &par->refresh_info;

    switch (refresh_info->buf_mode) {
        case OT_FB_LAYER_BUF_DOUBLE:
        case OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE:
            buffer_num = 2; /* 2 buffer num */
            break;

        case OT_FB_LAYER_BUF_ONE:
            buffer_num = 1;
            break;

        default:
            return TD_SUCCESS;
    }
    /* The interface setting requires uBuffersize, the actual memory size info->fix.smem_len */
    buffer_size = buffer_num * pitch * height;
    if (gfbg_get_smem_len(info) >= buffer_size) {
        return TD_SUCCESS;
    }
    gfbg_error("memory is not enough!  now is %d u32Pitch %d u32Height %d expect %d\n", gfbg_get_smem_len(info), pitch,
               height, buffer_size);
    return TD_FAILURE;
}

/* Address check only for uncompressed data */
static td_s32 gfbg_check_phyaddr(const ot_fb_buf *canvas_buf)
{
    td_u64 len;
    td_u32 bpp;

    bpp = gfbg_getbppbyfmt(canvas_buf->canvas.format);
    if (bpp == 0) {
        gfbg_error("Unsupported PIXEL FORMAT!\n");
        return TD_FAILURE;
    }
    len = canvas_buf->update_rect.height * (canvas_buf->update_rect.width * bpp / 8);  /* 8 alg data */

    return ot_mmz_check_phys_addr(canvas_buf->canvas.phys_addr, (td_ulong)len);
}

#ifndef __LITEOS__
static td_s32 gfbg_check_fmt(const struct fb_var_screeninfo *var, const struct fb_info *info)
{
    ot_fb_color_format format;
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    format = gfbg_getfmtbyargb(&var->red, &var->green, &var->blue, &var->transp, var->bits_per_pixel);
    if (format == OT_FB_FORMAT_BUTT) {
        gfbg_error("Unknown format(offset, length) r:(%d, %d, %d) ,g:(%d, %d, %d), b(%d, %d, %d), \
            a(%d, %d, %d), bpp:%d!\n",
            var->red.offset, var->red.length, var->red.msb_right,
            var->green.offset, var->green.length, var->green.msb_right,
            var->blue.offset, var->blue.length, var->blue.msb_right,
            var->transp.offset, var->transp.length, var->transp.msb_right,
            var->bits_per_pixel);
        return -EINVAL;
    }

    if (!g_drv_ops.capability[layer_id].is_color_format[format]) {
        gfbg_error("Unsupported PIXEL FORMAT!\n");
        return -EINVAL;
    }

    return TD_SUCCESS;
}
#endif

/*
 * Name : gfbg_buf_map
 * Desc : Memory mapping, which generates virtual addresses based on physical address mappings.
 */
static td_void *gfbg_buf_map(td_phys_addr_t phys_addr, td_u32 size)
{
    return cmpi_remap_nocache(phys_addr, size);
}

static td_void gfbg_buf_ummap(td_void *vir_addr)
{
    cmpi_unmap(vir_addr);
    return;
}


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART10_H_ */
