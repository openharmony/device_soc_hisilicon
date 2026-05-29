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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART24_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART24_H_

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
static const td_char* g_fmt_name[] = {
    "RGB565",
    "RGB888",
    "KRGB444",
    "KRGB555",
    "KRGB888",
    "ARGB4444",
    "ARGB1555",
    "ARGB8888",
    "ARGB8565",
    "RGBA4444",
    "RGBA5551",
    "RGBA5658",
    "RGBA8888",
    "BGR565",
    "BGR888",
    "ABGR4444",
    "ABGR1555",
    "ABGR8888",
    "ABGR8565",
    "KBGR444",
    "KBGR555",
    "KBGR888",
    "1BPP",
    "2BPP",
    "4BPP",
    "8BPP",
    "ACLUT44",
    "ACLUT88",
    "PUYVY",
    "PYUYV",
    "PYVYU",
    "YUV888",
    "AYUV8888",
    "YUVA8888",
    "BUTT"
};

static const td_char *g_layer_name[] = {"layer_0", "layer_1", "layer_2", "layer_3", "layer_4"};
static td_s32 gfbg_print_softcursor_proc(struct fb_info *info, osal_proc_entry_t *p, td_void *v)
{
    gfbg_par *par = (gfbg_par *)info->par;
    const td_char *layer_name = TD_NULL;
    ot_unused(v);

    if (par->layer_id >= sizeof(g_layer_name) / sizeof(td_char *)) {
        layer_name = "unknown layer";
    } else {
        layer_name = g_layer_name[par->layer_id];
    }

    osal_seq_printf(p, "layer_name                 \t: %s \n", layer_name);
    osal_seq_printf(p, "show_state               \t :%s\n", par->show ? "ON" : "OFF");
    osal_seq_printf(p, "referecce_count            \t :%d\n", atomic_read(&par->ref_count));
    osal_seq_printf(p, "position                   \t :(%d, %d)\n", par->display_info.pos.x_pos,
        par->display_info.pos.y_pos);
    osal_seq_printf(p, "color_format:               \t :%s\n", g_fmt_name[par->color_format]);
    osal_seq_printf(p, "alpha_en                 \t :%s\n", par->alpha.alpha_en ? "ON" : "OFF");
    osal_seq_printf(p, "alpha_channel_en           \t :%s\n", par->alpha.alpha_chn_en ? "ON" : "OFF");
    osal_seq_printf(p, "alpha0, alpha1             \t :%d, %d\n", par->alpha.alpha0, par->alpha.alpha1);
    osal_seq_printf(p, "alpha_global               \t :%d\n", par->alpha.global_alpha);
    osal_seq_printf(p, "colorkey_en                \t :%s\n", par->ckey.key_enable ? "ON" : "OFF");
    osal_seq_printf(p, "colorkey_value             \t :0x%x\n", par->ckey.key);
    osal_seq_printf(p, "cursor_hot_pos(x, y)       \t :(%d, %d)\n", par->cursor_info.cursor.hot_pos.x_pos,
        par->cursor_info.cursor.hot_pos.y_pos);
    return 0;
}

static td_void print_cursor_proc(osal_proc_entry_t *p, const gfbg_par *par)
{
    osal_seq_printf(p, "attach_cursor_id:         \t  :%d \n", par->cursor_info.attached_cursor_id);
    osal_seq_printf(p, "backup_cursor_addr         \t :0x%ld\n", (td_ulong)par->cursor_info.cursor.cursor.phys_addr);
    osal_seq_printf(p, "backup_cursor_format  \t :%s\n", g_fmt_name[par->cursor_info.cursor.cursor.format]);
    osal_seq_printf(p, "backup_cursor_stride       \t :%d\n", par->cursor_info.cursor.cursor.pitch);
    osal_seq_printf(p, "backup_cursor (width, h)       \t :(%d, %d)\n",
        par->cursor_info.cursor.cursor.width, par->cursor_info.cursor.cursor.height);
    osal_seq_printf(p, "cursor_rect in display buffer \t :(%d, %d, %d, %d)\n",
        par->cursor_info.rect_in_disp_buf.x, par->cursor_info.rect_in_disp_buf.y,
        par->cursor_info.rect_in_disp_buf.width, par->cursor_info.rect_in_disp_buf.height);
    osal_seq_printf(p, "cursor_pos in cursor image \t :(%d, %d)\n",
        par->cursor_info.pos_in_cursor.x_pos, par->cursor_info.pos_in_cursor.y_pos);
    return;
}

static td_void print_canvas_proc(osal_proc_entry_t *p, const gfbg_par *par)
{
    const gfbg_refresh_info *refresh_info = &par->refresh_info;
    const volatile gfbg_compress_info *compress_info = &par->compress_info;

    osal_seq_printf(p, "canavas_updated_addr       \t :0x%lx\n", (td_ulong)refresh_info->user_buffer.canvas.phys_addr +
        refresh_info->user_buffer.update_rect.y * refresh_info->user_buffer.update_rect.width +
        refresh_info->user_buffer.update_rect.x);
    osal_seq_printf(p, "canavas_updated (w, h)     \t :%d,%d \n", refresh_info->user_buffer.update_rect.width,
        refresh_info->user_buffer.update_rect.height);
    osal_seq_printf(p, "canvas_width               \t :%d\n", refresh_info->user_buffer.canvas.width);
    osal_seq_printf(p, "canvas_height              \t :%d\n", refresh_info->user_buffer.canvas.height);
    osal_seq_printf(p, "canvas_pitch               \t :%d\n", refresh_info->user_buffer.canvas.pitch);
    osal_seq_printf(p, "canvas_format              \t :%s\n", g_fmt_name[refresh_info->user_buffer.canvas.format]);
    osal_seq_printf(p, "is_compress                 \t :%s\n", compress_info->compress_open ? "YES" : "NO");
    osal_seq_printf(p, "is_ddr_dettect             \t :%s\n", (compress_info->compress_open &&
        compress_info->zone_nums && (refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE ||
        refresh_info->buf_mode == OT_FB_LAYER_BUF_BUTT)) ? "YES" : "NO");
    osal_seq_printf(p, "ddr_detect_zones           \t :%d\n", (compress_info->compress_open &&
        (refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE ||
        refresh_info->buf_mode == OT_FB_LAYER_BUF_BUTT)) ? compress_info->zone_nums : 0);
    osal_seq_printf(p, "premul_enable                 \t :%s\n", par->display_info.is_premul ? "ON" : "OFF");
    return;
}

static td_void print_display_proc(osal_proc_entry_t *p, gfbg_par *par)
{
    const td_char *buf_mode[] = {"triple", "double", "single", "triple( no frame discarded)", "unknown"};
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;

    osal_seq_printf(p, "display_buffer_mode(+usr_buf)\t :%s\n", buf_mode[refresh_info->buf_mode]);
    osal_seq_printf(p, "displaying_addr (register) \t :0x%lx\n", (td_ulong)refresh_info->screen_addr);
    osal_seq_printf(p, "display_buffer[0] addr     \t :0x%lx\n", (td_ulong)refresh_info->disp_buf_info.phys_addr[0]);
    osal_seq_printf(p, "display_buffer[1] addr     \t :0x%lx\n", (td_ulong)refresh_info->disp_buf_info.phys_addr[1]);
    osal_seq_printf(p, "is_premul_mode:            \t :%s\n", (display_info->is_premul == TD_TRUE) ? "YES" : "NO");
    osal_seq_printf(p, "display_rect                \t :(%d, %d)\n", display_info->display_width,
        display_info->display_height);
    osal_seq_printf(p, "screen_rect                 \t :(%d, %d)\n", display_info->screen_width,
        display_info->screen_height);
    osal_seq_printf(p, "device_max_resolution      \t :%d, %d\n", display_info->max_screen_width,
        display_info->max_screen_height);
    osal_seq_printf(p, "is_need_flip(2buf)           \t :%s\n",
        refresh_info->disp_buf_info.need_flip ? "YES" : "NO");
    osal_seq_printf(p, "buf_index_displaying(2buf)\t :%d\n", refresh_info->disp_buf_info.index_for_int);
    osal_seq_printf(p, "refresh_request_num(2buf)  \t :%d\n", refresh_info->refresh_num);
    osal_seq_printf(p, "switch_buf_num(2buf)       \t :%d\n", refresh_info->disp_buf_info.int_pic_num);
    osal_seq_printf(p, "union_rect (2buf)          \t :(%d,%d,%d,%d)\n",
        refresh_info->disp_buf_info.union_rect.x, refresh_info->disp_buf_info.union_rect.y,
        refresh_info->disp_buf_info.union_rect.width, refresh_info->disp_buf_info.union_rect.height);
    return;
}

static td_void print_common_proc(osal_proc_entry_t *p, struct fb_info *info, const td_char *layer_name)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    const td_char *antiflicer_level[] = {"NONE", "LOW", "MIDDLE", "HIGH", "AUTO", "ERROR"};
    const td_char *mirror_mode[] = {"NONE", "HORIZONTAL", "VERTICAL", "BOTH", "unknown"};
    const td_char *dynamic_range[] = {"SDR8", "SDR10", "HDR10", "HLG", "SLF", "unknown"};
    const td_char *anti_mode[] = {"NONE", "TDE", "VOU", "ERROR"};
    const td_char *rotation_mode[] = {"0", "90", "180", "270", "-"};
    td_bool enable = TD_FALSE;
    if (g_drv_ops.gfbg_drv_get_layer_enable != NULL) {
        (td_void)g_drv_ops.gfbg_drv_get_layer_enable(par->layer_id, &enable);
    }

    osal_seq_printf(p, "\n[GFBG] Version: ["OT_MPP_VERSION"], Build Time["__DATE__", "__TIME__"]\n");
    osal_seq_printf(p, "\n");
    osal_seq_printf(p, "layer_name                 \t :%s \n", layer_name);
    osal_seq_printf(p, "open_count                 \t :%d\n", atomic_read(&par->ref_count));
    osal_seq_printf(p, "show_state                 \t :%s\n", (par->show) ? "ON" : "OFF");
    osal_seq_printf(p, "graphic_enable             \t :%s\n", (enable == TD_TRUE) ? "ON" : "OFF");
    osal_seq_printf(p, "start_position             \t :(%d, %d)\n", display_info->pos.x_pos,
        display_info->pos.y_pos);
    osal_seq_printf(p, "xres, yres                 \t :(%d, %d)\n", gfbg_get_xres(info), gfbg_get_yres(info));
    osal_seq_printf(p, "xres_virtual, yres_virtual \t :(%d, %d)\n", gfbg_get_xres_virtual(info),
        gfbg_get_yres_virtual(info));
    osal_seq_printf(p, "xoffset, yoffset           \t :(%d, %d)\n", gfbg_get_xoffset(info),
        gfbg_get_yoffset(info));
    osal_seq_printf(p, "fix.line_length            \t :%d\n", gfbg_get_line_length(info));
    osal_seq_printf(p, "mem_size:                  \t :%d KB\n", gfbg_get_smem_len(info) / 1024); /* 1024 1K */
    osal_seq_printf(p, "layer_scale (hw):          \t :%s \n",
        g_drv_ops.capability[par->layer_id].is_vo_scale ? "YES" : "NO");
    osal_seq_printf(p, "color_format:               \t :%s\n", g_fmt_name[par->color_format]);
    osal_seq_printf(p, "alpha_en                    \t :%s\n", par->alpha.alpha_en ? "ON" : "OFF");
    osal_seq_printf(p, "alpha_channel_en           \t :%s\n", par->alpha.alpha_chn_en ? "ON" : "OFF");
    osal_seq_printf(p, "alpha0, alpha1             \t :%d, %d\n", par->alpha.alpha0, par->alpha.alpha1);
    osal_seq_printf(p, "alpha_global               \t :%d\n", par->alpha.global_alpha);
    osal_seq_printf(p, "colorkey_en                \t :%s\n", par->ckey.key_enable ? "ON" : "OFF");
    osal_seq_printf(p, "colorkey_value             \t :0x%x\n", par->ckey.key);
    osal_seq_printf(p, "mirror_mode:            \t :%s\n", mirror_mode[display_info->mirror_mode]);
    osal_seq_printf(p, "dynamic_range:            \t :%s\n", dynamic_range[display_info->dynamic_range]);
    osal_seq_printf(p, "deflicker_mode:            \t :%s\n", anti_mode[display_info->antiflicker_mode]);
    osal_seq_printf(p, "rotation_mode:             \t :%s\n", rotation_mode[display_info->rotate_mode]);
    osal_seq_printf(p, "deflicker_level:           \t :%s\n", antiflicer_level[display_info->antiflicker_level]);
    osal_seq_printf(p,  "gfbg_mode:                  \t :%s\n",
        (refresh_info->buf_mode == OT_FB_LAYER_BUF_BUTT) ? "STANDARD" : "EXTEND");
    return;
}

static td_s32 gfbg_print_layer_proc(struct fb_info *info, osal_proc_entry_t *p, td_void *v)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    const td_char *layer_name = TD_NULL;
    ot_unused(v);

    if (par->layer_id >= sizeof(g_layer_name) / sizeof(td_char *)) {
        layer_name = "unknown layer";
    } else {
        layer_name = g_layer_name[par->layer_id];
    }

    if (display_info->antiflicker_mode > GFBG_ANTIFLICKER_BUTT) {
        display_info->antiflicker_mode = GFBG_ANTIFLICKER_BUTT;
    }

    if (display_info->antiflicker_level > OT_FB_LAYER_ANTIFLICKER_BUTT) {
        display_info->antiflicker_level = OT_FB_LAYER_ANTIFLICKER_BUTT;
    }

    print_common_proc(p, info, layer_name);

    print_display_proc(p, par);

    print_canvas_proc(p, par);

    if (par->cursor_info.attached && is_soft_cursor()) {
        print_cursor_proc(p, par);
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_read_proc(osal_proc_entry_t *entry)
{
    struct fb_info* info = TD_NULL;
    gfbg_par *par = TD_NULL;
    if (entry == TD_NULL) {
        return TD_FAILURE;
    }
    info = (struct fb_info *)(entry->private);
    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    if (par == TD_NULL) {
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id) || !is_soft_cursor()) {
        return gfbg_print_layer_proc(info, entry, TD_NULL);
    } else {
        return gfbg_print_softcursor_proc(info, entry, TD_NULL);
    }

    return TD_SUCCESS;
}

static td_void parse_procmd_help(const gfbg_par *par, const td_char *cmd)
{
    if (strncmp("help", cmd, 4) == 0) { /* 4 cmp */
        osal_printk("help info:\n");
        osal_printk("echo cmd > proc file\n");
        osal_printk("gfbg support cmd:\n");
        osal_printk("show:show layer\n");
        osal_printk("hide:hide layer\n");
        osal_printk("For example, if you want to hide layer 0,you can input:\n");
        osal_printk("   echo hide > /proc/umap/gfbg0\n");
    } else {
        gfbg_error("layer_id %d doesn't support cmd:%s, use help cmd to show help info!\n",
            par->layer_id, cmd);
    }
    return;
}

static td_void gfbg_parse_proccmd(const osal_proc_entry_t *p, td_u32 layer_id, const td_char *cmd)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    td_s32 cnt;
    unsigned long lock_flag;
    ot_unused(p);

    gfbg_spin_lock_irqsave(&par->lock, lock_flag);
    cnt = atomic_read(&par->ref_count);

    if (strncmp("show", cmd, 4) == 0) { /* 4 cmp */
        if (cnt == 0) {
            gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
            gfbg_error("err:layer_id %d no open!\n", par->layer_id);
            return;
        }

        if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
            gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
            gfbg_error("cursor layer %d doesn't support this cmd!\n", par->layer_id);
            return;
        }

        if (!par->show) {
            par->modifying = TD_TRUE;
            par->show = TD_TRUE;
            par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_SHOW;
            par->modifying = TD_FALSE;
        }
    } else if (strncmp("hide", cmd, 4) == 0) { /* 4 cmp */
        if (cnt == 0) {
            gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
            gfbg_error("err:layer_id %d no open!\n", par->layer_id);
            return;
        }

        if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
            gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);
            gfbg_error("cursor layer %d doesn't support this cmd!\n", par->layer_id);
            return;
        }

        if (par->show) {
            par->modifying = TD_TRUE;
            par->show = TD_FALSE;
            par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_SHOW;
            par->modifying = TD_FALSE;
        }
    }

    gfbg_spin_unlock_irqrestore(&par->lock, lock_flag);

    parse_procmd_help(par, cmd);
    return;
}

static td_s32 gfbg_write_proc(osal_proc_entry_t *entry, const char *buf, int count, long long *ppos)
{
#ifndef __LITEOS__
#define TMP_BUF_LEN 32
    struct fb_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;
    char tmp_buf[TMP_BUF_LEN] = {0};
    int len;

    if (entry == TD_NULL) {
        return -ENOSYS;
    }

    if (count <= 0) {
        return -ENOSYS;
    }

    len = (count >= TMP_BUF_LEN) ? TMP_BUF_LEN : count;

    if (osal_copy_from_user(tmp_buf, buf, len)) {
        return TD_FAILURE;
    }
    tmp_buf[len - 1] = '\0';

    info = (struct fb_info *)(entry->private);
    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)(info->par);
    if (par == TD_NULL) {
        return TD_FAILURE;
    }

    gfbg_parse_proccmd(entry, par->layer_id, (td_char*)tmp_buf);
    if (memset_s((td_void *)tmp_buf, TMP_BUF_LEN, 0, count) != EOK) {
        gfbg_error("%s:%d:memset_s failed!\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
#endif
    ot_unused(ppos);
    return count;
}
#endif

/* for linux */
#ifndef __LITEOS__
#ifndef CONFIG_OT_VO_FB_SEPARATE
/* for CONFIG_OT_VO_FB_SEPARATE=n in linux */
#ifdef MODULE
module_init(gfbg_init);
module_exit(gfbg_cleanup);
MODULE_LICENSE("GPL");
#else
subsys_initcall(gfbg_init);
#endif
#endif

#endif


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART24_H_ */
