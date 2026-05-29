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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART02_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART02_H_

#ifndef __LITEOS__
static td_ulong gfbg_get_smem_start(const struct fb_info* info)
{
    return (td_ulong)info->fix.smem_start;
}
static td_s8 *gfbg_get_screen_base(const struct fb_info* info)
{
    return (td_s8*)info->screen_base;
}

static td_u32 gfbg_get_xres(const struct fb_info* info)
{
    return (td_u32)info->var.xres;
}

static td_u32 gfbg_get_yres(const struct fb_info* info)
{
    return (td_u32)info->var.yres;
}

static td_u32 gfbg_get_xres_virtual(const struct fb_info* info)
{
    return (td_u32)info->var.xres_virtual;
}

static td_u32 gfbg_get_yres_virtual(const struct fb_info* info)
{
    return (td_u32)info->var.yres_virtual;
}

static td_u32 gfbg_get_smem_len(const struct fb_info* info)
{
    return (td_u32)info->fix.smem_len;
}

static td_u32 gfbg_get_line_length(const struct fb_info* info)
{
    return (td_u32)info->fix.line_length;
}

/* when compress to uncompress,use for calculate xoffset,yoffset */
static td_u32 gfbg_get_line_length_ex(const struct fb_info* info)
{
    td_u32 uncompress_stride;
    uncompress_stride = (info->var.xres_virtual * info->var.bits_per_pixel / 8 + /* 8 one byte */
        GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    if (gfbg_check_memory_enough(info, uncompress_stride) == TD_TRUE) {
        return uncompress_stride;
    } else {
        return (td_u32)info->fix.line_length;
    }
}

static td_u32 gfbg_get_bits_per_pixel(const struct fb_info* info)
{
    return (td_u32)info->var.bits_per_pixel;
}

static td_u32 gfbg_get_yoffset(const struct fb_info* info)
{
    return (td_u32)info->var.yoffset;
}

static td_u32 gfbg_get_xoffset(const struct fb_info* info)
{
    return (td_u32)info->var.xoffset;
}

static struct fb_fix_screeninfo g_default_fix[GFBG_LAYER_TYPE_BUTT] = {
    {
        .id = "gfbg",                  /* String identifierString identifier */
        .type = FB_TYPE_PACKED_PIXELS, /* FB type */
        .visual = FB_VISUAL_TRUECOLOR,
        .xpanstep = 1,
        .ypanstep = 1,
        .ywrapstep = 0,
        .line_length = GFBG_4K_DEF_STRIDE,
        .accel = FB_ACCEL_NONE,
        .mmio_len = 0,
        .mmio_start = 0,
    },
    {
        .id          = "gfbg",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = GFBG_HD_DEF_STRIDE,
        .accel = FB_ACCEL_NONE,
        .mmio_len = 0,
        .mmio_start = 0,
    },
    {
        .id          = "gfbg",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = GFBG_SD_DEF_STRIDE,
        .accel       = FB_ACCEL_NONE,
        .mmio_len    =                     0,
        .mmio_start  =                     0,
    },
    {
        .id          = "gfbg",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = GFBG_SD_DEF_STRIDE,
        .accel       = FB_ACCEL_NONE,
        .mmio_len    =                     0,
        .mmio_start  =                     0,
    },
    {
        .id          = "gfbg",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = GFBG_HD_DEF_STRIDE,
        .accel       = FB_ACCEL_NONE,
        .mmio_len    =                     0,
        .mmio_start  =                     0,
    }
};

/* default variable information */
/*
 * Name : g_default_var
 * Desc : default variable info: g_default_var
 * See  : gfbg_open
 */
static struct fb_var_screeninfo g_default_var[GFBG_LAYER_TYPE_BUTT] = {
    /* for 4K layer */
    {
        .xres           = GFBG_4K_DEF_WIDTH,
        .yres           = GFBG_4K_DEF_HEIGHT,
        .xres_virtual   = GFBG_4K_DEF_WIDTH,
        .yres_virtual   = GFBG_4K_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = GFBG_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = {5, 5, 0},
        .blue           = {0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1, /* pixel clock in ps (pico seconds) */
        .left_margin    = 0, /* time from sync to picture   */
        .right_margin   = 0, /* time from picture to sync   */
        .upper_margin   = 0, /* time from sync to picture   */
        .lower_margin   = 0,
        .hsync_len      = 0, /* length of horizontal sync   */
        .vsync_len      = 0, /* length of vertical sync */
    },
    /* for HD layer */
    {
        .xres           = GFBG_HD_DEF_WIDTH,
        .yres           = GFBG_HD_DEF_HEIGHT,
        .xres_virtual   = GFBG_HD_DEF_WIDTH,
        .yres_virtual   = GFBG_HD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = GFBG_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = {5, 5, 0},
        .blue           = {0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1, /* pixel clock in ps (pico seconds) */
        .left_margin    = 0, /* time from sync to picture   */
        .right_margin   = 0, /* time from picture to sync   */
        .upper_margin   = 0, /* time from sync to picture   */
        .lower_margin   = 0,
        .hsync_len      = 0, /* length of horizontal sync   */
        .vsync_len      = 0, /* length of vertical sync */
    },
    /* for SD layer */
    {
        .xres           = GFBG_SD_DEF_WIDTH,
        .yres           = GFBG_SD_DEF_HEIGHT,
        .xres_virtual   = GFBG_SD_DEF_WIDTH,
        .yres_virtual   = GFBG_SD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = GFBG_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = {5, 5, 0},
        .blue           = {0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1, /* pixel clock in ps (pico seconds) */
        .left_margin    = 0, /* time from sync to picture   */
        .right_margin   = 0, /* time from picture to sync   */
        .upper_margin   = 0, /* time from sync to picture   */
        .lower_margin   = 0,
        .hsync_len      = 0, /* length of horizontal sync   */
        .vsync_len      = 0, /* length of vertical sync */
    },
    /* for AD layer */
    {
        .xres           = GFBG_AD_DEF_WIDTH,
        .yres           = GFBG_AD_DEF_HEIGHT,
        .xres_virtual   = GFBG_AD_DEF_WIDTH,
        .yres_virtual   = GFBG_AD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = GFBG_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = {5, 5, 0},
        .blue           = {0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1, /* pixel clock in ps (pico seconds) */
        .left_margin    = 0, /* time from sync to picture   */
        .right_margin   = 0, /* time from picture to sync   */
        .upper_margin   = 0, /* time from sync to picture   */
        .lower_margin   = 0,
        .hsync_len      = 0, /* length of horizontal sync   */
        .vsync_len      = 0, /* length of vertical sync */
    },
    /* for cursor layer */
    {
        .xres           = GFBG_CURSOR_DEF_WIDTH,
        .yres           = GFBG_CURSOR_DEF_HEIGHT,
        .xres_virtual   = GFBG_CURSOR_DEF_WIDTH,
        .yres_virtual   = GFBG_CURSOR_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = GFBG_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = {5, 5, 0},
        .blue           = {0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1, /* pixel clock in ps (pico seconds) */
        .left_margin    = 0, /* time from sync to picture   */
        .right_margin   = 0, /* time from picture to sync   */
        .upper_margin   = 0, /* time from sync to picture   */
        .lower_margin   = 0,
        .hsync_len      = 0, /* length of horizontal sync   */
        .vsync_len      = 0, /* length of vertical sync */
    }
};
#else
static td_ulong gfbg_get_smem_start(const struct gfbg_info* info)
{
    return (td_ulong)(uintptr_t)info->oinfo.fbmem;
}

static td_s8 *gfbg_get_screen_base(const struct gfbg_info* info)
{
    return (td_s8 *)info->oinfo.fbmem;
}

static td_u32 gfbg_get_xres(const struct gfbg_info* info)
{
    return (td_u32)info->vinfo.xres;
}

static td_u32 gfbg_get_yres(const struct gfbg_info* info)
{
    return (td_u32)info->vinfo.yres;
}

static td_u32 gfbg_get_xres_virtual(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.sarea.width;
}

static td_u32 gfbg_get_yres_virtual(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.sarea.height;
}

static td_u32 gfbg_get_smem_len(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.fblen;
}

static td_u32 gfbg_get_line_length(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.stride;
}

static td_u32 gfbg_get_bits_per_pixel(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.bpp;
}

static td_u32 gfbg_get_yoffset(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.sarea.y;
}

static td_u32 gfbg_get_xoffset(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.sarea.x;
}

struct gfbg_screeninfo {
    uint8_t format;         /* see FB_FMT_* */
    fb_coord_t xres;        /* Horizontal resolution in pixel columns */
    fb_coord_t yres;        /* Vertical resolution in pixel rows */
    struct fb_area_s sarea; /* Selected area within the overlay */
    fb_coord_t stride;      /* Length of a line in bytes */
    uint8_t bpp;            /* Bits per pixel */
    uint32_t accl;          /* Supported hardware acceleration */
    uint8_t nplanes;        /* Number of color planes supported */
    uint8_t noverlays;      /* Number of overlays supported */
};

static struct gfbg_screeninfo g_default_info[GFBG_LAYER_TYPE_BUTT] = {
    /* for 4K layer */
    {
        .format = OT_FB_FORMAT_ARGB1555,
        .xres = GFBG_4K_DEF_WIDTH,
        .yres = GFBG_4K_DEF_HEIGHT,
        .sarea = {0, 0, GFBG_4K_DEF_WIDTH, GFBG_4K_DEF_HEIGHT},
        .stride = GFBG_4K_DEF_STRIDE,
        .bpp = GFBG_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for HD layer */
    {
        .format = OT_FB_FORMAT_ARGB1555,
        .xres = GFBG_HD_DEF_WIDTH,
        .yres = GFBG_HD_DEF_HEIGHT,
        .sarea = {0, 0, GFBG_HD_DEF_WIDTH, GFBG_HD_DEF_HEIGHT},
        .stride = GFBG_HD_DEF_STRIDE,
        .bpp = GFBG_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for SD layer */
    {
        .format = OT_FB_FORMAT_ARGB1555,
        .xres = GFBG_SD_DEF_WIDTH,
        .yres = GFBG_SD_DEF_HEIGHT,
        .sarea = {0, 0, GFBG_SD_DEF_WIDTH, GFBG_SD_DEF_HEIGHT},
        .stride = GFBG_SD_DEF_STRIDE,
        .bpp = GFBG_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for AD layer */
    {
        .format = OT_FB_FORMAT_ARGB1555,
        .xres = GFBG_AD_DEF_WIDTH,
        .yres = GFBG_AD_DEF_HEIGHT,
        .sarea = {0, 0, GFBG_AD_DEF_WIDTH, GFBG_AD_DEF_HEIGHT},
        .stride = GFBG_AD_DEF_STRIDE,
        .bpp = GFBG_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for cursor layer */
    {
        .format = OT_FB_FORMAT_ARGB1555,
        .xres = GFBG_CURSOR_DEF_WIDTH,
        .yres = GFBG_CURSOR_DEF_HEIGHT,
        .sarea = {0, 0, GFBG_CURSOR_DEF_WIDTH, GFBG_CURSOR_DEF_HEIGHT},
        .stride = GFBG_CURSOR_DEF_STRIDE,
        .bpp = GFBG_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
};

static void gfbg_screeninfo_init(struct gfbg_info *info, struct gfbg_screeninfo *sinfo)
{
    info->vinfo.xres = sinfo->xres;
    info->vinfo.yres = sinfo->yres;
    info->oinfo.sarea = sinfo->sarea;
    info->oinfo.bpp = sinfo->bpp;
    info->oinfo.stride = sinfo->stride;
    info->oinfo.accl = sinfo->accl;
    info->vinfo.nplanes = sinfo->nplanes;
    info->vinfo.noverlays = sinfo->noverlays;
};
#endif

/*
 * Name : g_argb_bit_field
 * Desc : bit fields of each color format in GFBG_COLOR_FMT_E,
 *        the order must be the same as that of GFBG_COLOR_FMT_E
 */
static gfbg_argb_bitinfo g_argb_bit_field[] = {
    /* RGB565 */
    {
        .red    = {11, 5, 0},
        .green  = {5, 6, 0},
        .blue   = {0, 5, 0},
        .transp = {0, 0, 0},
    },
    /* RGB888 */
    {
        .red    = {16, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {0, 8, 0},
        .transp = {0, 0, 0},
    },
    /* KRGB444 */
    {
        .red    = {8, 4, 0},
        .green  = {4, 4, 0},
        .blue   = {0, 4, 0},
        .transp = {0, 0, 0},
    },
    /* KRGB555 */
    {
        .red    = {10, 5, 0},
        .green  = {5, 5, 0},
        .blue   = {0, 5, 0},
        .transp = {0, 0, 0},
    },
    /* KRGB888 */
    {
#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART02_H_ */
