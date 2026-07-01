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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART23_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART23_H_

#ifdef __LITEOS__
static td_void gfbg_overlay_cleanup(td_u32 layer_id, td_bool unregister)
{
    td_s32 ret;

    ret = fb_unregister(layer_id);
    if (ret < 0) {
        gfbg_error("gfbg overlay cleanup fail!\n");
    }

    return;
}
#else
static td_void gfbg_overlay_cleanup(td_u32 layer_id, td_bool unregister)
{
    struct fb_info* info = TD_NULL;

    /* get framebuffer info structure pointer */
    info = g_layer[layer_id].info;
    if (info != TD_NULL) {
        if (gfbg_get_screen_base(info)) {
            gfbg_buf_ummap(gfbg_get_screen_base(info));
        }

        if (gfbg_get_smem_start(info)) {
            gfbg_buf_freemem(gfbg_get_smem_start(info));
        }

        if (unregister) {
            unregister_framebuffer(info);
        }

        framebuffer_release(info);
        g_layer[layer_id].info = TD_NULL;
    }

    return;
}
#endif

#ifndef __LITEOS__
static td_s32 overlay_probe_alloc_mem(struct fb_info *info, struct fb_fix_screeninfo *fix,
                                      struct fb_var_screeninfo *var)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    td_char name[16]; /* 16 for length of name */

    par = (gfbg_par *)(info->par);
    layer_id = par->layer_id;

    if (g_layer[layer_id].layer_size != 0) {
        /* initialize the fix screen info */
        if (is_4k_layer(layer_id)) {
            *fix = g_default_fix[GFBG_LAYER_TYPE_4K];
            *var = g_default_var[GFBG_LAYER_TYPE_4K];
        } else if (is_hd_layer(layer_id)) {
            *fix = g_default_fix[GFBG_LAYER_TYPE_HD];
            *var = g_default_var[GFBG_LAYER_TYPE_HD];
        } else if (is_sd_layer(layer_id)) {
            *fix = g_default_fix[GFBG_LAYER_TYPE_SD];
            *var = g_default_var[GFBG_LAYER_TYPE_SD];
        } else if (is_ad_layer(layer_id)) {
            *fix = g_default_fix[GFBG_LAYER_TYPE_AD];
            *var = g_default_var[GFBG_LAYER_TYPE_AD];
        } else if (is_cursor_layer(layer_id)) {
            *fix = g_default_fix[GFBG_LAYER_TYPE_CURSOR];
            *var = g_default_var[GFBG_LAYER_TYPE_CURSOR];
        }
        if (snprintf_s(name, sizeof(name), 12, "gfbg_layer%01u", layer_id) < 0) { /* 12:for char length */
            gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
        }
        fix->smem_start = (td_ulong)gfbg_buf_allocmem(name,
            (g_layer[layer_id].layer_size + g_layer[layer_id].curosr_buf_size) * 1024, /* 1024 for 1k */
            gfbg_get_layer_mmz_names(layer_id));
        if (fix->smem_start == 0) {
            gfbg_error("%s:failed to malloc the video memory, size: %ld KBtyes!\n", name,
                (g_layer[layer_id].layer_size + g_layer[layer_id].curosr_buf_size));
            return TD_FAILURE;
        }
        /* u32LayerSize is KB */
        fix->smem_len = g_layer[layer_id].layer_size * 1024; /* 1024 for 1k */

        /* initialize the virtual address and clear memory */
        info->screen_base = gfbg_buf_map(fix->smem_start, (g_layer[layer_id].layer_size +
                                         g_layer[layer_id].curosr_buf_size) * 1024); /* 1024 for 1k */

        if (info->screen_base == TD_NULL) {
            gfbg_error("Failed to call map video memory,size:0x%x, start: 0x%lx\n",
                       fix->smem_len, fix->smem_start);
            return TD_FAILURE;
        }
        gfbg_drv_set_mmz_addr(fix->smem_start, fix->smem_len, layer_id);

        /* Initialize the memory to 0. Call TDE to do it. */
        gfbg_fill_data(info, 0, gfbg_get_smem_len(info));
    }
    return TD_SUCCESS;
}
#endif

/*
 * Function        : gfbg_overlay_probe
 * Description     : initialize the framebuffer for the overlay and set
 */
#ifdef __LITEOS__
static td_s32 __init gfbg_overlay_probe(td_u32 layer_id)
{
    td_s32 ret = TD_FAILURE;

    if ((ret = fb_register(layer_id, 0)) < 0) {
        gfbg_error("failed to register_framebuffer!\n");
        ret = -EINVAL;
        goto ERR;
    }

    return TD_SUCCESS;

ERR:
    gfbg_overlay_cleanup(layer_id, TD_FALSE);

    return ret;
}
#else
static td_s32 __init gfbg_overlay_probe(td_u32 layer_id)
{
    td_s32 ret = TD_FAILURE;
    struct fb_info *info = TD_NULL;
    struct fb_fix_screeninfo *fix = TD_NULL;
    struct fb_var_screeninfo *var = TD_NULL;
    gfbg_par *par = TD_NULL;

    /*
     * The size of the size represents the private data space of the device,
     * and the par of fb_info points to the private space, that is,
     * info->par already points to the memory space
     */
    info = framebuffer_alloc(sizeof(gfbg_par), TD_NULL);
    if (info == TD_NULL) {
        gfbg_error("failed to malloc the fb_info!\n");
        return -ENOMEM;
    }
    fix = &info->fix;
    var = &info->var;
    /* save the info pointer in global pointer array */
    g_layer[layer_id].info = info;
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
    info->flags = FBINFO_FLAG_DEFAULT | FBINFO_HWACCEL_YPAN | FBINFO_HWACCEL_XPAN;
#else
    info->flags = FB_MODE_IS_UNKNOWN | FBINFO_HWACCEL_YPAN | FBINFO_HWACCEL_XPAN;
#endif
    /* fbops members in fb_info point to g_ot_fb_ops, so open, release, ioctl, etc. can get fb_info. */
    info->fbops = &g_ot_fb_ops;

    par = (gfbg_par *)(info->par);
    (td_void)memset_s(par, sizeof(gfbg_par), 0, sizeof(gfbg_par));
    par->layer_id = layer_id;
    par->color_format = OT_FB_FORMAT_ARGB1555;

    if (snprintf_s(fix->id, sizeof(fix->id), 5, "ovl%01u", layer_id) < 0) { /* 5:for char length */
        gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
        ret = TD_FAILURE;
        goto ERR;
    }
    /* It's not need to alloc mem for cursor layer if use g_softcursor */
    ret = overlay_probe_alloc_mem(info, fix, var);
    if (ret != TD_SUCCESS) {
        goto ERR;
    }

    if ((ret = register_framebuffer(info)) < 0) {
        gfbg_error("failed to register_framebuffer!layerid = %d, s32Ret = %d\n", layer_id, ret);
        ret = -EINVAL;
        goto ERR;
    }

    gfbg_info("succeed in registering the fb%d: %s frame buffer device\n", info->node, fix->id);

    return TD_SUCCESS;

ERR:
    gfbg_overlay_cleanup(layer_id, TD_FALSE);

    return ret;
}
#endif

/*
 * Function        : gfbg_get_vram_size
 * Description     : parse the parameter string and get the size. if
                     the parameter is invalid, the size is default value.
 * Input           : const char* pstr  the string for the vram size
 * Return          : the video memory size
 */
static unsigned long gfbg_get_vram_size(const char* pstr)
{
    td_s32 str_is_valid = TD_TRUE;
    unsigned long vram_size = 0;
    unsigned long vram_size_temp;
    const char* ptr = pstr;

    if ((ptr == TD_NULL) || (*ptr == '\0')) {
        return 0;
    }

    /* check if the string is valid */
    while (*ptr != '\0') {
        if (*ptr == ',') {
            break;
        } else if ((!((*ptr) >= '0' && (*ptr) <= '9')) && (*ptr != 'X') && (*ptr != 'x') &&
                   ((*ptr > 'f' && *ptr <= 'z') || (*ptr > 'F' && *ptr <= 'Z'))) {
            str_is_valid = TD_FALSE;
            break;
        }

        ptr++;
    }

    if (str_is_valid) {
#ifdef __LITEOS__
        vram_size = strtoul(pstr, (char **)TD_NULL, 0);
#else
        vram_size = simple_strtoul(pstr, (char **)TD_NULL, 0);
#endif
        if (vram_size > PAGE_SIZE_ALIGN_MAX) {
            gfbg_error("vram_size(%lu)( > %lu) is overflow, it will be set to %u!\n", vram_size,
                PAGE_SIZE_ALIGN_MAX, 0);
            vram_size = 0;
        }

        vram_size_temp = vram_size;
        /* make the size PAGE_SIZE align */
        vram_size = ((vram_size * 1024 + PAGE_SIZE - 1) & PAGE_MASK) / 1024; /* 2^10 1024 */
        if (vram_size_temp != vram_size) {
            gfbg_error("vram_size(%lu) if not align in 4, it will be set to %lu!\n", vram_size_temp, vram_size);
        }
    }
    return vram_size;
}

static td_void parse_cfg_start(td_char **sc_str)
{
    /* judge the cursor if use soft or hard layer */
    if (!strcmp("off", g_softcursor)) {
        g_soft_cursor = TD_FALSE;
    } else {
        g_soft_cursor = TD_TRUE;
    }

    /* judge the display is need on */
    if (!strcmp("on", g_display)) {
        g_display_on = TD_TRUE;
    } else {
        g_display_on = TD_FALSE;
    }

    /* get the string before next varm */
    *sc_str = strstr(g_video, "vram");
    gfbg_info("video:%s\n", g_video);

    return;
}

static td_void parse_cfg_change_layer_size(const td_char *sc_str, td_u32 layer_id)
{
    td_ulong layer_size;
    layer_size = gfbg_get_vram_size(sc_str);

    if (g_drv_ops.capability[layer_id].is_layer_support) {
        if (is_cursor_layer(layer_id)) {
            if (!is_soft_cursor()) {
                g_layer[layer_id].layer_size = layer_size;
            }
        } else {
            g_layer[layer_id].layer_size = layer_size;
            if (is_soft_cursor() && layer_size) {
                g_layer[layer_id].curosr_buf_size = GFBG_CURSOR_DEF_VRAM;
            }
        }
    }
    return;
}

static td_s32 parse_cfg_change_layer_id(td_u32 *layer_id, const td_char *number, td_u32 length)
{
    if (length > 4) { /* 4 for array number max length */
        gfbg_error("length = %d; out of range!\n", length);
        return TD_FAILURE;
    }
#ifdef __LITEOS__
    *layer_id = strtoul(number, (char **)TD_NULL, 10); /* 10 for length */
#else
    *layer_id = simple_strtoul(number, (char **)TD_NULL, 10); /* 10 for length */
#endif
    if (*layer_id >= g_drv_ops.layer_count) {
        gfbg_error("Layer %d is in module_param---video out of range!\n", *layer_id);
        return TD_FAILURE;
    }

    if (!g_drv_ops.capability[*layer_id].is_layer_support) {
        gfbg_error("Layer %d unsupported, so module_param---video can't contain vram_size for it!\n", *layer_id);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/*
 * Name : gfbg_parse_cfg
 * Desc : Parse the parameters.
 * See  : gfbg_overlay_probe
 */
static td_s32 gfbg_parse_cfg(td_void)
{
    td_char *sc_str = TD_NULL;
    td_char number[4] = {0}; /* 4 cfg num */
    td_u32 i;
    td_u32 j;
    td_u32 layer_id;
    td_char ac_param[12] = {0}; /* 12 param length */
    td_char ac_temp[12] = {0}; /* 12 param length */
    td_bool is_param_valid = TD_FALSE;

    /*
     * 1.judge the cursor if use soft or hard layer
     * 2.judge the display is need on
     * 3.get the string before next varm
     */
    parse_cfg_start(&sc_str);

    /* parse cfg process */
    while (sc_str != TD_NULL) {
        /* parse the layer id and save it in a string */
        i = 0;

        /* if the number of graphics layers is 10 or more, the string is risky */
        for (j = 0; j < g_drv_ops.layer_count; j++) {
            if (snprintf_s(ac_param, sizeof(ac_param), 11, "vram%01u_size", j) < 0) { /* 11:for char length */
                gfbg_error("%s:%d:snprintf_s failure!\n", __FUNCTION__, __LINE__);
                return TD_FAILURE;
            }
            if (strncpy_s(ac_temp, sizeof(ac_temp), sc_str, 10) != EOK) { /* 10 cpy num */
                gfbg_error("%s:%d:strncpy_s failure!\n", __FUNCTION__, __LINE__);
                return TD_FAILURE;
            }
            if (!strcmp(ac_param, ac_temp)) {
                is_param_valid = TD_TRUE;
            }
        }
        if (!is_param_valid) {
            gfbg_error("insmod parameter is invalid!\n");
            return TD_FAILURE;
        }
        /* 4:skip "vram" */
        sc_str += 4;
        while (*sc_str != '_') {
            /* i>1 means layer id is bigger than 100, it's obviously out of range! */
            if (i > 1) {
                gfbg_error("layer id is out of range!\n");
                return -1;
            }

            number[i] = *sc_str;
            i++;
            sc_str++;
        }

        number[i] = '\0';

        /* change the layer id string into digital and assure it's legal */
        if (parse_cfg_change_layer_id(&layer_id, number, sizeof(number)) != TD_SUCCESS) {
            return TD_FAILURE;
        }

        sc_str += sizeof("size") + i;
        /* get the layer size string and change it to digital */
        parse_cfg_change_layer_size(sc_str, layer_id);

        /* get next layer string */
        sc_str = strstr(sc_str, "vram");
    }

    return TD_SUCCESS;
}


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART23_H_ */
