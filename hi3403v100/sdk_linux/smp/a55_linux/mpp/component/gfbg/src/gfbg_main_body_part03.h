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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART03_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART03_H_

        .red    = {16, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {0, 8, 0},
        .transp = {0, 0, 0},
    },
    /* ARGB4444 */
    {
        .red    = {8, 4, 0},
        .green  = {4, 4, 0},
        .blue   = {0, 4, 0},
        .transp = {12, 4, 0},
    },
    /* ARGB1555 */
    {
        .red    = {10, 5, 0},
        .green  = {5, 5, 0},
        .blue   = {0, 5, 0},
        .transp = {15, 1, 0},
    },
    /* ARGB8888 */
    {
        .red    = {16, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {0, 8, 0},
        .transp = {24, 8, 0},
    },
    /* ARGB8565 */
    {
        .red    = {11, 5, 0},
        .green  = {5, 6, 0},
        .blue   = {0, 5, 0},
        .transp = {16, 8, 0},
    },
    /* RGBA4444 */
    {
        .red    = {12, 4, 0},
        .green  = {8, 4, 0},
        .blue   = {4, 4, 0},
        .transp = {0, 4, 0},
    },
    /* RGBA5551 */
    {
        .red    = {11, 5, 0},
        .green  = {6, 5, 0},
        .blue   = {1, 5, 0},
        .transp = {0, 1, 0},
    },
    /* RGBA5658 */
    {
        .red    = {19, 5, 0},
        .green  = {13, 6, 0},
        .blue   = {8, 5, 0},
        .transp = {0, 8, 0},
    },
    /* RGBA8888 */
    {
        .red    = {24, 8, 0},
        .green  = {16, 8, 0},
        .blue   = {8, 8, 0},
        .transp = {0, 8, 0},
    },
    /* BGR565 */
    {
        .red    = {0, 5, 0},
        .green  = {5, 6, 0},
        .blue   = {11, 5, 0},
        .transp = {0, 0, 0},
    },
    /* BGR888 */
    {
        .red    = {0, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {16, 8, 0},
        .transp = {0, 0, 0},
    },
    /* ABGR4444 */
    {
        .red    = {0, 4, 0},
        .green  = {4, 4, 0},
        .blue   = {8, 4, 0},
        .transp = {12, 4, 0},
    },
    /* ABGR1555 */
    {
        .red    = {0, 5, 0},
        .green  = {5, 5, 0},
        .blue   = {10, 5, 0},
        .transp = {15, 1, 0},
    },
    /* ABGR8888 */
    {
        .red    = {0, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {16, 8, 0},
        .transp = {24, 8, 0},
    },
    /* ABGR8565 */
    {
        .red    = {0, 5, 0},
        .green  = {5, 6, 0},
        .blue   = {11, 5, 0},
        .transp = {16, 8, 0},
    },
    /* KBGR444 16bpp */
    {
        .red    = {0, 4, 0},
        .green  = {4, 4, 0},
        .blue   = {8, 4, 0},
        .transp = {0, 0, 0},
    },
    /* KBGR555 16bpp */
    {
        .red    = {0, 5, 0},
        .green  = {5, 5, 0},
        .blue   = {10, 5, 0},
        .transp = {0, 0, 0},
    },
    /* KBGR888 32bpp */
    {
        .red    = {0, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {16, 8, 0},
        .transp = {0, 0, 0},
    },

    /* 1bpp */
    {
        .red = {0, 1, 0},
        .green = {0, 1, 0},
        .blue = {0, 1, 0},
        .transp = {0, 0, 0},
    },
    /* 2bpp */
    {
        .red = {0, 2, 0},
        .green = {0, 2, 0},
        .blue = {0, 2, 0},
        .transp = {0, 0, 0},
    },
    /* 4bpp */
    {
        .red = {0, 4, 0},
        .green = {0, 4, 0},
        .blue = {0, 4, 0},
        .transp = {0, 0, 0},
    },
    /* 8bpp */
    {
        .red = {0, 8, 0},
        .green = {0, 8, 0},
        .blue = {0, 8, 0},
        .transp = {0, 0, 0},
    },
    /* ACLUT44 */
    {
        .red = {4, 4, 0},
        .green = {4, 4, 0},
        .blue = {4, 4, 0},
        .transp = {0, 4, 0},
    },
    /* ACLUT88 */
    {
        .red = {8, 8, 0},
        .green = {8, 8, 0},
        .blue = {8, 8, 0},
        .transp = {0, 8, 0},
    }
};

static td_s32 tde_cal_scale_rect_gfbg(const drv_tde_rect* src_rect, const drv_tde_rect* dst_rect,
                                      const drv_tde_rect* rect_in_src, drv_tde_rect* rect_in_dst)
{
    rect_in_dst->pos_x = rect_in_src->pos_x * dst_rect->width / src_rect->width;
    rect_in_dst->pos_y = rect_in_src->pos_y * dst_rect->height / src_rect->height;
    rect_in_dst->height = dst_rect->height;
    rect_in_dst->width = dst_rect->width;

    return TD_SUCCESS;
}

static td_void gfbg_version(td_void);
#ifdef CURSOR
static td_void ot_fb_cursor_calcdispinfo(td_u32 cursor_id, gfbg_par *par, const ot_fb_point* cur_new_pos);
#endif
static td_s32 gfbg_refresh(td_u32 layer_id, const ot_fb_buf *canvas_buf, ot_fb_layer_buf buf_mode);
static td_void gfbg_select_antiflicker_mode(gfbg_par *par);

gfbg_layer g_layer[GFBG_MAX_LAYER_NUM];
char *g_video; /* 128 for vram length */

static char g_display[8] = "off";   /* 8:open vo or not */
static char g_softcursor[8] = "off";   /* 8:use soft cursor or not */

td_void set_video_name(char *temp_video)
{
    g_video = temp_video;
}

static td_s32 gfbg_drv_set_mmz_addr(td_phys_addr_t phys_addr, td_u32 phys_len, td_u32 layer_id)
{
    td_s32 ret;
    ot_tde_export_func *tde_export_func = func_entry(ot_tde_export_func, OT_ID_TDE);
    if ((tde_export_func == TD_NULL) || (tde_export_func->drv_tde_module_set_gfbg_mmz_addr == TD_NULL)) {
        gfbg_error("TDE tde_export_func is NULL!\n");
        return TD_FAILURE;
    }

    ret = tde_export_func->drv_tde_module_set_gfbg_mmz_addr(phys_addr, phys_len, layer_id);
    if (ret != TD_SUCCESS) {
        osal_printk("GFBG Warning: TDE set gfbg mmz addr failed\n");
        return ret;
    }

    return TD_SUCCESS;
}

/*
 * Name : gfbg_fill_data
 * Desc : fill buffer, it is faster than memset_s.
 * See  : Called by gfbg_overlay_probe
 */
static td_u32 gfbg_fill_data(const struct fb_info* info, td_u32 fill_data, size_t n)
{
    td_s32 ret;
    ot_fb_buf dst_img;
    td_s8 *rest = TD_NULL;
    const td_u32 can_pitch = GFBG_TDE_MEMSET_WIDTH * 4; /* 4 alg data */

    /*
     * Assume that the image width is 3840, the format is ARBG8888, and the image is filled by TDE.
     * Since the size of the image that TDE can handle is limited,
     * the width and height are limited to GFBG_TDE_MEMSET_WIDTH
     * and GFBG_TDE_MEMSET_HEIGHT, Exceeded parts are cleared using memset_s
     * Configure the target image, pitch, format, width and height, physical address (pointing to the memory start
     * address)
     */
    dst_img.canvas.phys_addr = gfbg_get_smem_start(info);
    dst_img.canvas.pitch = can_pitch;
    dst_img.canvas.format = OT_FB_FORMAT_ABGR8888;
    dst_img.canvas.width = GFBG_TDE_MEMSET_WIDTH;
    dst_img.canvas.height = n / can_pitch;

    if (dst_img.canvas.height > GFBG_TDE_MEMSET_HEIGHT) {
        dst_img.canvas.height = GFBG_TDE_MEMSET_HEIGHT;
    }

    if (dst_img.canvas.height != 0) {
        ret = gfbg_drv_fill(&dst_img, fill_data);
        if (ret != TD_SUCCESS) {
            gfbg_error("gfbg_drv_fill fail!\n");
            return TD_FAILURE;
        }
    }

    /* The memory not in the canvas is set by memset_s */
    if ((n - dst_img.canvas.height * can_pitch) != 0) {
        rest = gfbg_get_screen_base(info) +  can_pitch * (dst_img.canvas.height);
        (td_void)memset_s(rest, n - dst_img.canvas.height * can_pitch, fill_data,
            n - dst_img.canvas.height * can_pitch);
    }
    return TD_SUCCESS;
}

/*
* Name : gfbg_get_dcmp_framesize
* Desc : Read two numbers from the virtual address.
*/
static td_void gfbg_get_dcmp_framesize(td_u32 *ar_size, td_u32 *gb_size, const td_void *screen_base,
    td_u32 offset)
{
    *ar_size = *(td_u64*)screen_base;
    /* Screen_base cannot be converted to any type except HI_U8* */
    *gb_size = *(td_u64*)(screen_base + offset);
}

/*
 * Name : gfbg_set_dcmp_info
 * Desc : set the decompression info.
 */
static td_void gfbg_set_dcmp_info(struct fb_info *info)
{
    gfbg_par *par = (gfbg_par *)(info->par);
    td_u32 buf_size;
    gfbg_refresh_info *refresh_info = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    fb_vo_graphic_dcmp_info dcmp_info;
    ot_fb_color_format gfbg_color_fmt;

    if (par == TD_NULL) {
        return;
    }

    refresh_info  = &par->refresh_info;
    display_info = &par->display_info;
    buf_size = (gfbg_get_line_length(info) * gfbg_get_yres(info) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);

    if ((refresh_info->screen_addr == 0) || (refresh_info->gb_screen_addr == 0)) {
        /* close the dcmp */
        g_drv_ops.gfbg_drv_enable_dcmp(par->layer_id, TD_FALSE);
        gfbg_warning("Compression is opened, but compressed buffer phyaddr for refreshing is NULL(0)\n");
        return;
    }

    /*
     * Decompressing information from compressed information: compression channels AR, GB address, and its size size0,
     * size1
     */
    gfbg_color_fmt = refresh_info->user_buffer.canvas.format;
    switch (gfbg_color_fmt) {
        case OT_FB_FORMAT_ARGB4444:
            dcmp_info.pixel_fmt = FB_VO_INPUTFMT_ARGB_4444;
            break;
        case OT_FB_FORMAT_ARGB1555:
            dcmp_info.pixel_fmt = FB_VO_INPUTFMT_ARGB_1555;
            break;
        case OT_FB_FORMAT_ARGB8888:
            dcmp_info.pixel_fmt = FB_VO_INPUTFMT_ARGB_8888;
            break;
        default:
            gfbg_error("Pixel format(%d) is invalid!should be ARGB4444 or ARGB1555 or ARGB8888\n", gfbg_color_fmt);
            return;
    }
    dcmp_info.ar_phy_addr = gfbg_drv_get_dcmp_offset_addr(refresh_info->screen_addr);
    dcmp_info.gb_phy_addr = gfbg_drv_get_dcmp_offset_addr(refresh_info->gb_screen_addr);
    dcmp_info.width = display_info->display_width;
    dcmp_info.height = display_info->display_height;

    gfbg_get_dcmp_framesize(&dcmp_info.frame_size0, &dcmp_info.frame_size1,
        (td_void*)((td_u8*)(gfbg_get_screen_base(info)) +
        ((refresh_info->screen_addr - gfbg_get_smem_start(info)) / buf_size) * buf_size), buf_size / 2); /* 2 half */
    dcmp_info.is_lossless_a = TD_FALSE; /* Whether lossless,FALSE:Lossy,TRUE:lossless */
    dcmp_info.is_lossless = TD_FALSE;  /* Whether lossless,FALSE:Lossy,TRUE:lossless */

    /* Decompressing information to drv level */
    if (g_drv_ops.gfbg_drv_set_dcmp_info(par->layer_id, &dcmp_info) == TD_FALSE) {
        /* close dcmp */
        g_drv_ops.gfbg_drv_enable_dcmp(par->layer_id, TD_FALSE);
    }
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    /* for compress, we should replace the stride */
    info->fix.line_length = dcmp_info.stride;
#endif
}

#ifdef CONFIG_OT_VO_FB_SEPARATE
static int gfbg_interrupt_route(td_s32 irq, td_void *dev_id);
#endif

static td_void drv_gfbg_alloc_cmap(td_u32 layer_id)
{
    const td_u32 cmap_len = 256;
    struct fb_info *info = g_layer[layer_id].info;

    if (info == TD_NULL) {
        gfbg_error("fb_info is NULL !\n");
        return;
    }

    if (g_drv_ops.capability[layer_id].is_cmap != TD_TRUE) {
        return;
    }

    if (fb_alloc_cmap(&info->cmap, cmap_len, 1) < 0) {
        info->cmap.len = 0;
        gfbg_error("fb_alloc_cmap failed\n");
        return;
    }

    info->cmap.len = cmap_len;

    return;
}

static td_void drv_gfbg_free_cmap(td_u32 layer_id)
{
    struct fb_cmap *cmap = NULL;
    struct fb_info *info = g_layer[layer_id].info;

    if (info == TD_NULL) {
        return;
    }

    cmap = &info->cmap;
    if (cmap->len != 0) {
        fb_dealloc_cmap(cmap);
        cmap->len = 0;
    }

    return;
}
static td_s32 gfbg_cmpi_init(td_void *args)
{
    ot_unused(args);
    return 0;
}

static td_void gfbg_cmpi_exit(td_void)
{
    return;
}

static td_void gfbg_cmpi_notify(mod_notice_id notice)
{
    ot_unused(notice);
    return;
}

static td_void gfbg_cmpi_query_state(mod_state *state)
{
    if (state == TD_NULL) {
        return;
    }
    *state = MOD_STATE_FREE;
    return;
}

static td_u32 gfbg_cmpi_get_ver_magic(td_void)
{
    return VERSION_MAGIC;
}

static umap_module g_gfbg_module = {
    .mod_id = OT_ID_FB,
    .mod_name = "gfbg",
    .pfn_init = gfbg_cmpi_init,
    .pfn_exit = gfbg_cmpi_exit,
    .pfn_query_state = gfbg_cmpi_query_state,
    .pfn_notify = gfbg_cmpi_notify,


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART03_H_ */
