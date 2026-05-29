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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART01_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART01_H_


#line 1 "gfbg_main.c"
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
#include <linux/version.h>
#include "gfbg_main.h"
#include "ot_debug.h"
#include "mm_ext.h"
#include "securec.h"
#include "ot_drv_tde.h"
#include "gfbg_vou_drv.h"
#include "gfbg_vou_graphics.h"
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
#include "gfbg_proc.h"
#endif
#include "gfbg.h"
#include "gfbg_blit.h"
#include "gfbg_rotate.h"
#include "gfbg_comm.h"
#include "gfbg_init.h"
#include "sys_ext.h"
#include "proc_ext.h"
#include "osal_ioctl.h"

#define mkstr(exp) # exp
#define mkmarcotostr(exp) mkstr(exp)

#define gfbg_max_width(layer_id)  g_drv_ops.capability[layer_id].max_width
#define gfbg_max_height(layer_id) g_drv_ops.capability[layer_id].max_height
#define gfbg_min_width(layer_id)  g_drv_ops.capability[layer_id].min_width
#define gfbg_min_height(layer_id) g_drv_ops.capability[layer_id].min_height

#define gfbg_fb_info(layer_id) (gpgfbg_fb_infos[layer_id])

#define GFBG_CMAP_LEN 256
#define GFBG_ROTBUF_NAME_LEN 16

typedef td_s32(*drv_gfbg_ioctl_func)(struct fb_info *info, unsigned long arg);

typedef struct {
    td_u32 cmd;
    drv_gfbg_ioctl_func func;
}drv_gfbg_ioctl_func_item;

#define PAGE_SIZE_ALIGN_MAX ((~0ul - PAGE_SIZE) / 1024)

#define is_soft_cursor() (g_soft_cursor == TD_TRUE)

#ifdef __LITEOS__
#define in_atomic() (TD_FALSE)
#endif

#define GFBG_ALIGNMENT 0xf
#define GFBG_ALIGN     16

#define gfbg_debug_print_vcnt(dev, format, args...) \
    do { \
        td_u32 __vcnt = 0; \
        graphic_drv_get_int_state_vcnt(dev, &__vcnt); \
        gfbg_debug("dev %d cnt %d in "format"\n", dev, __vcnt, ##args); \
    } while (0)

/*
 * the interface to operate the chip
 * Collection of properties and methods,filled in gfbg_init
 */
static gfbg_drv_ops g_drv_ops;

static td_bool g_soft_cursor = TD_FALSE;
static td_bool g_display_on = TD_FALSE;
static td_bool g_gfbg_register = TD_TRUE;
#define VO_INVALID_DEV (-1)
#define VO_DEV_DHD0 0
#define VO_DEV_DHD1 1
#define VO_DEV_DSD0 2

#define GFBG_INTMSK_HD0_VTTHD1 0x1
#define GFBG_INTMSK_HD0_VTTHD2 0x2
#define GFBG_INTMSK_HD0_VTTHD3 0x4
#define GFBG_INTMSK_HD1_VTTHD1 0x10
#define GFBG_INTMSK_HD1_VTTHD2 0x20
#define GFBG_INTMSK_HD1_VTTHD3 0x40
#define GFBG_INTMSK_SD0_VTTHD1 0x100
#define GFBG_INTMSK_SD0_VTTHD2 0x200
#define GFBG_INTMSK_SD0_VTTHD3 0x400

#define DRV_GFBG_IOCTL_CMD_NUM_MAX 152
#define DRV_GFBG_IOCTL_FUNC_ITEM_NUM_MAX 46
#define DEV_NAME "gfbg"

static td_s32 gfbg_parse_cfg(td_void);
static td_s32 gfbg_overlay_probe(td_u32 layer_id);
static td_void gfbg_overlay_cleanup(td_u32 layer_id, td_bool unregister);
#ifdef __LITEOS__
typedef gfbg_info fb_info;
typedef fb_cmap_s fb_cmap;

static td_s32 gfbg_pan_display(struct fb_vtable_s *vtable, struct fb_overlayinfo_s *oinfo);
#else
static td_s32 gfbg_pan_display(struct fb_var_screeninfo *var, struct fb_info *info);
#endif
static ot_fb_color_format gfbg_getfmtbyargb(const struct fb_bitfield *red, const struct fb_bitfield *green,
                                            const struct fb_bitfield *blue, const struct fb_bitfield *transp,
                                            td_u32 color_depth);
static td_void gfbg_buf_freemem(td_phys_addr_t phyaddr);
static td_phys_addr_t gfbg_buf_allocmem(const td_char *buf_name, td_ulong layer_size, const td_char *mmz_name);

static td_s32 gfbg_wait_regconfig_work(td_u32 layer_id);

static td_s32 gfbg_freeccanbuf(gfbg_par *par);
static td_void gfbg_set_dispbufinfo(td_u32 layer_id);
static td_void gfbg_set_bufmode(td_u32 layer_id, ot_fb_layer_buf layer_buf_mode);
static inline td_void gfbg_get_bufmode(const gfbg_par *par, ot_fb_layer_buf *buf_mode);
static td_s32 gfbg_onrefresh(gfbg_par* par, td_void __user *argp);
static td_s32 gfbg_refresh_0buf(td_u32 layer_id, const ot_fb_buf *canvas_buf);
static td_s32 gfbg_refresh_1buf(td_u32 layer_id, const ot_fb_buf *canvas_buf);
static td_s32 gfbg_refresh_2buf(td_u32 layer_id, const ot_fb_buf *canvas_buf);
static td_s32 gfbg_refresh_2buf_immediate_display(td_u32 layer_id, const ot_fb_buf *canvas_buf);
static td_s32 gfbg_set_mirrormode(gfbg_par *par, ot_fb_mirror_mode mirror_mode);
static td_s32 gfbg_set_rotatemode(const struct fb_info *info, ot_fb_rotate_mode rotate_mode);

#ifdef CURSOR
static inline td_void gfbg_set_cursorinfo(gfbg_par *par, gfbg_cursor_info *cursor_info);
static inline td_void gfbg_get_cursorinfo(gfbg_par *par, gfbg_cursor_info *cursor_info);
static td_s32 ot_fb_cursor_attach(td_u32 cursor_id, td_u32 layer_id);
static td_s32 ot_fb_cursor_detach(td_u32 layer_id);
static td_s32 ot_fb_cursor_changepos(td_u32 cursor_id, ot_fb_point pos);
static td_s32 ot_fb_cursor_changestate(gfbg_par *cursor_par, td_bool show);
static td_s32 ot_fb_cursor_putinfo(gfbg_par *cursor_par, ot_fb_cursor* cursor);
#endif
static td_s32 gfbg_onputlayerinfo(struct fb_info *info, gfbg_par* par, const td_void __user *argp);
static td_void gfbg_get_layerinfo(const gfbg_par *par, ot_fb_layer_info *layer_info);
static inline td_void gfbg_get_antiflickerlevel(const gfbg_par *par, ot_fb_layer_antiflicker_level *antiflicker_level);
static td_void gfbg_set_antiflickerlevel(td_u32 layer_id, ot_fb_layer_antiflicker_level antiflicker_level);
static inline td_void gfbg_get_fmt(const gfbg_par *par, ot_fb_color_format *color_format);
static inline td_void gfbg_set_fmt(gfbg_par *par, ot_fb_color_format color_fmt);
static inline td_void gfbg_set_alpha(gfbg_par *par, const ot_fb_alpha *alpha);
static inline td_void gfbg_get_alpha(const gfbg_par *par, ot_fb_alpha *alpha);
static inline td_void gfbg_set_key(gfbg_par *par, const gfbg_colorkeyex *key);
static inline td_void gfbg_get_key(const gfbg_par *par, gfbg_colorkeyex *key);
static inline td_void gfbg_get_layerpos(const gfbg_par *par, ot_fb_point *pos);
static td_void gfbg_get_screensize(const gfbg_par *par, td_u32 *width, td_u32 *height);
static td_s32 gfbg_set_screensize(gfbg_par *par, const td_u32 *width, const td_u32 *height);
static td_void gfbg_get_maxscreensize(gfbg_par *par, td_u32 *width, td_u32 *height);
static td_void gfbg_get_dispsize(const gfbg_par *par, td_u32 *width, td_u32 *height);
static inline td_void gfbg_get_premul(const gfbg_par *par, td_bool *premul);
static inline td_bool gfbg_get_show(const gfbg_par *par);
static inline td_void gfbg_set_show(gfbg_par *par, td_bool show);
static td_void gfbg_set_layerpos(gfbg_par *par, const ot_fb_point *pos);
static td_s32 gfbg_tde_rotate_callback(const td_void *paraml, const td_void *paramr);
static td_void gfbg_vgs_rotate_callback(ot_mod_id call_mod_id, td_s32 call_dev_id, td_s32 call_chn_id,
    struct ot_vgs_job_data *job_data);
static td_s32 gfbg_tde_callback(const td_void *paraml, const td_void *paramr);
static td_s32 gfbg_interrupt_process(td_u32 layer_id);
static td_void gfbg_get_idledispbuf(const gfbg_par *par, td_phys_addr_t *phy_addr);
static td_void gfbg_get_workdispbuf(const gfbg_par *par, td_phys_addr_t *phy_addr);
static inline td_void gfbg_get_workcursorbuf(const gfbg_par *par, td_phys_addr_t *phy_addr);
#ifdef CURSOR
static td_void gfbg_get_idlecursorbuf(gfbg_par *par, td_phys_addr_t *phy_addr);
#endif
static inline td_bool gfbg_is_interlace(const gfbg_par *par);
static td_s32 gfbg_set_dynamic_range(gfbg_par *par, ot_fb_dynamic_range dynamic_range);

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
static td_s32 gfbg_read_proc(struct osal_proc_dir_entry *entry);
static td_s32 gfbg_write_proc(struct osal_proc_dir_entry *entry, const char *buf, int count, long long *);
#endif

static td_s32 drv_gfbg_get_colorkey(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_colorkey(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_layer_alpha(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_layer_alpha(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_screen_origin_pos(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_screen_origin_pos(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_deflicker(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_deflicker(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_vblank(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_show_layer(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_layer_show_state(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_capability(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_sursor_info(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_sursor_info(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_cursor_state(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_cursor_state(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_cursor_pos(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_cursor_pos(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_cursor_colorkey(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_cursor_colorkey(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_cursor_alpha(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_cursor_alpha(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_attch_cursor(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_detach_cursor(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_layer_info(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_layer_info(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_canvas_buffer(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_refresh_layer(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_wait_refresh_finish(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_mirror_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_mirror_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_rotate_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_rotate_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_screen_size(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_screen_size(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_flip_surface(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_compression_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_compression_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_mddrdetect(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_mddrdetect(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_dynamic_range(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_dynamic_range(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_create(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_release(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_draw_frame(struct fb_info *info, unsigned long arg);

static td_s32 g_drv_gfbg_ctl_num[DRV_GFBG_IOCTL_CMD_NUM_MAX] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  0,  0,  7,  8,
    9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 0,  0,  0,  0,
    25, 26, 0,  27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 0,  0,  41,
    42, 0,  0,  0,  0,  0,  0,  0,  0,  43, 44, 45
};

static drv_gfbg_ioctl_func_item g_drv_gfbg_ioctl_func[DRV_GFBG_IOCTL_FUNC_ITEM_NUM_MAX] = {
    {0, TD_NULL},
    {FBIOGET_COLORKEY_GFBG, drv_gfbg_get_colorkey},
    {FBIOPUT_COLORKEY_GFBG, drv_gfbg_set_colorkey},
    {FBIOGET_ALPHA_GFBG, drv_gfbg_get_layer_alpha},
    {FBIOPUT_ALPHA_GFBG, drv_gfbg_set_layer_alpha},
    {FBIOGET_SCREEN_ORIGIN_GFBG, drv_gfbg_get_screen_origin_pos},
    {FBIOPUT_SCREEN_ORIGIN_GFBG, drv_gfbg_set_screen_origin_pos},
    {FBIOGET_DEFLICKER_GFBG, drv_gfbg_get_deflicker},
    {FBIOPUT_DEFLICKER_GFBG, drv_gfbg_set_deflicker},
    {FBIOGET_VER_BLANK_GFBG, drv_gfbg_get_vblank},
    {FBIOPUT_SHOW_GFBG, drv_gfbg_show_layer},
    {FBIOGET_SHOW_GFBG, drv_gfbg_get_layer_show_state},
    {FBIOGET_CAPABILITY_GFBG, drv_gfbg_get_capability},
    {FBIOPUT_CURSOR_INFO, drv_gfbg_set_sursor_info},
    {FBIOGET_CURSOR_INFO, drv_gfbg_get_sursor_info},
    {FBIOPUT_CURSOR_STATE, drv_gfbg_set_cursor_state},
    {FBIOGET_CURSOR_STATE, drv_gfbg_get_cursor_state},
    {FBIOPUT_CURSOR_POS, drv_gfbg_set_cursor_pos},
    {FBIOGET_CURSOR_POS, drv_gfbg_get_cursor_pos},
    {FBIOPUT_CURSOR_COLORKEY, drv_gfbg_set_cursor_colorkey},
    {FBIOGET_CURSOR_COLORKEY, drv_gfbg_get_cursor_colorkey},
    {FBIOPUT_CURSOR_ALPHA, drv_gfbg_set_cursor_alpha},
    {FBIOGET_CURSOR_ALPHA, drv_gfbg_get_cursor_alpha},
    {FBIOPUT_CURSOR_ATTCH_CURSOR, drv_gfbg_set_attch_cursor},
    {FBIOPUT_CURSOR_DETACH_CURSOR, drv_gfbg_set_detach_cursor},
    {FBIOPUT_LAYER_INFO, drv_gfbg_set_layer_info},
    {FBIOGET_LAYER_INFO, drv_gfbg_get_layer_info},
    {FBIOGET_CANVAS_BUF, drv_gfbg_get_canvas_buffer},
    {FBIO_REFRESH, drv_gfbg_refresh_layer},
    {FBIO_WAITFOR_FREFRESH_DONE, drv_gfbg_wait_refresh_finish},
    {FBIOPUT_MIRROR_MODE, drv_gfbg_set_mirror_mode},
    {FBIOGET_MIRROR_MODE, drv_gfbg_get_mirror_mode},
    {FBIOPUT_ROTATE_MODE, drv_gfbg_set_rotate_mode},
    {FBIOGET_ROTATE_MODE, drv_gfbg_get_rotate_mode},
    {FBIOPUT_SCREEN_SIZE, drv_gfbg_set_screen_size},
    {FBIOGET_SCREEN_SIZE, drv_gfbg_get_screen_size},
    {FBIOFLIP_SURFACE, drv_gfbg_flip_surface},
    {FBIOPUT_COMPRESSION_GFBG, drv_gfbg_set_compression_mode},
    {FBIOGET_COMPRESSION_GFBG, drv_gfbg_get_compression_mode},
    {FBIOPUT_MDDRDETECT_GFBG, drv_gfbg_set_mddrdetect},
    {FBIOGET_MDDRDETECT_GFBG, drv_gfbg_get_mddrdetect},
    {FBIOPUT_DYNAMIC_RANGE_GFBG, drv_gfbg_set_dynamic_range},
    {FBIOGET_DYNAMIC_RANGE_GFBG, drv_gfbg_get_dynamic_range},
    {FBIO_CREATE_LAYER, drv_gfbg_create},
    {FBIO_DESTROY_LAYER, drv_gfbg_release},
    {FBIO_DRAW_SMART_RECT, drv_gfbg_draw_frame},
};

static td_bool gfbg_check_memory_enough(const struct fb_info *info, td_u32 stride)
{
    if ((stride * info->var.yres * 2) <= info->fix.smem_len) { /* 2 two buffer */
        return TD_TRUE;
    }
    return TD_FALSE;
}


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART01_H_ */
