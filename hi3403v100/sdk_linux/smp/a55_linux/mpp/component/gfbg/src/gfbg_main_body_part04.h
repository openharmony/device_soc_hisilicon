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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART04_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART04_H_

    .pfn_ver_checker = gfbg_cmpi_get_ver_magic,
    .export_funcs = TD_NULL,
    .data = TD_NULL,
};

static td_s32 gfbg_register(td_u32 index)
{
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    const td_char *entry_name[5] = { /* 5 max */
        "gfbg0",
        "gfbg1",
        "gfbg2",
        "gfbg3",
        "gfbg4"
    };
#endif
    td_s32 ret;
    ret = gfbg_overlay_probe(index);
    if (ret == TD_SUCCESS) {
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
        /* create a proc entry in 'gfbg' for the layer */
        gfbg_proc_add_module(entry_name[index], gfbg_read_proc, gfbg_write_proc, TD_NULL,
            g_layer[index].info);
#endif
        drv_gfbg_alloc_cmap(index);
    } else {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void gfbg_unregister(td_u32 index)
{
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    const td_char *entry_name[5] = { /* 5 max */
        "gfbg0",
        "gfbg1",
        "gfbg2",
        "gfbg3",
        "gfbg4"
    };
#endif
    td_s32 j;
    for (j = index - 1; j >= 0; j--) {
        if (g_drv_ops.capability[j].is_layer_support == TD_FALSE) {
            continue;
        }
        drv_gfbg_free_cmap(j);
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
        /* destroy a proc entry in 'gfbg' for the layer */
        gfbg_proc_remove_module(entry_name[j]);
#endif
        /* unregister the layer */
        gfbg_overlay_cleanup(j, TD_TRUE);
    }
        return;
}

static td_s32 gfbg_init_register(td_void)
{
    td_u32 i;
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    gfbg_proc_init();
#endif
    /* initialize fb file according the config */
    for (i = 0; i < g_drv_ops.layer_count; i++) {
        if (g_drv_ops.capability[i].is_layer_support == TD_FALSE) {
            continue;
        }

        if (gfbg_register(i) != TD_SUCCESS) {
            gfbg_unregister(i);
            osal_printk("ERROR: Load gfbg.ko ....FAILED!\n");
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_void gfbg_init_do_err1(td_void)
{
    td_u32 i;
    for (i = 0; i < g_drv_ops.layer_count; i++) {
        if (g_drv_ops.capability[i].is_layer_support == TD_FALSE) {
            continue;
        }
        drv_gfbg_free_cmap(i);
        gfbg_overlay_cleanup(i, TD_TRUE);
    }

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
        gfbg_proc_remove_all_module();
#endif
    osal_printk("ERROR: Load gfbg.ko ....FAILED!\n");
    return;
}

#if defined (CONFIG_OT_VO_FB_SEPARATE)
static td_void gfbg_init_do_err2(td_void)
{
    g_drv_ops.gfbg_drv_deinit();

    gfbg_drv_set_tde_callback(TD_NULL);
    gfbg_drv_set_tde_rotate_callback(TD_NULL);
    gfbg_drv_set_vgs_rotate_callback(TD_NULL);
    return;
}
#endif

static td_s32 gfbg_init_process(td_void)
{
    /* initial adoption layer */
    if (g_drv_ops.gfbg_drv_init() != TD_SUCCESS) {
        gfbg_error("drv init failed\n");
        return TD_FAILURE;
    }

    /* register rotation */
    gfbg_rotation_register();

    gfbg_drv_set_tde_callback(gfbg_tde_callback);
    gfbg_drv_set_tde_rotate_callback(gfbg_tde_rotate_callback);
    gfbg_drv_set_vgs_rotate_callback(gfbg_vgs_rotate_callback);

    return TD_SUCCESS;
}

#ifdef __LITEOS__
td_s32 gfbg_init(td_void* args)
{
    if (gfbg_get_module_para(args) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* get the chip operation method */
    gfbg_drv_get_ops(&g_drv_ops);

    if (cmpi_register_module(&g_gfbg_module)) {
        gfbg_error("cmpi_register_module for Gfbg failure!\n");
        return TD_FAILURE;
    }

    /* parse the \arg video && g_softcursor && display string */
    if (gfbg_parse_cfg() < 0) {
        gfbg_error("Usage: insmod gfbg.ko video=\"gfbg: vrami_size:xxx,vramj_size: xxx, ...\"\n");
        gfbg_error("i, j means layer id, xxx means layer size in kbytes!\n");
        gfbg_error("example: insmod gfbg.ko video=\"gfbg: vram0_size: 810, vram1_size: 810\"\n\n");
        return TD_FAILURE;
    }

    /* initialize fb file according the config */
    if (gfbg_init_register() != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* open vo and initial layer */
    if (gfbg_init_process() != TD_SUCCESS) {
        goto ERR1;
    }

    /* show version */
    gfbg_version();

    osal_printk("load gfbg.ko ....OK!\n");

    return TD_SUCCESS;

ERR1:
    /* do with gfbg init err1 */
    gfbg_init_do_err1();
    return TD_FAILURE;
}
#else
td_s32 gfbg_init(td_void)
{
    if (ckfn_sys_entry() == TD_FALSE) {
        gfbg_error("no sys ko!\n");
        return TD_FAILURE;
    }
#ifndef CONFIG_OT_VO_FB_SEPARATE
    set_global_name();
#endif
    /* get the chip operation method */
    gfbg_drv_get_ops(&g_drv_ops);

    if (cmpi_register_module(&g_gfbg_module)) {
        gfbg_error("cmpi_register_module for Gfbg failure!\n");
        g_gfbg_register = TD_FALSE;
        return TD_FAILURE;
    }

    /* parse the \arg video && g_softcursor && display string */
    if (gfbg_parse_cfg() < 0) {
        gfbg_error("Usage: insmod gfbg.ko video=\"gfbg: vrami_size:xxx, vramj_size:xxx, ...\"\n");
        gfbg_error("i, j means layer id, xxx means layer size in kbytes!\n");
        g_gfbg_register = TD_FALSE;
        osal_printk("ERROR: Load gfbg.ko ....FAILED!\n");
        return TD_FAILURE;
    }

    /* initialize fb file according the config */
    if (gfbg_init_register() != TD_SUCCESS) {
        g_gfbg_register = TD_FALSE;
        return TD_FAILURE;
    }

    /* open vo and initial layer */
    if (gfbg_init_process() != TD_SUCCESS) {
        goto err1;
    }

    /*
    * Interrupt registration interrupt is placed at the end.
    * Since the interrupt will be opened in the boot screen,
    * it will respond when loading ko, and the context is used in the interrupt.
    */
#ifdef CONFIG_OT_VO_FB_SEPARATE
    if (osal_request_irq(*(vou_get_gfbg_irq()), gfbg_interrupt_route, TD_NULL, "GFBG Int", gfbg_interrupt_route)) {
        gfbg_error("request_irq for Gfbg failure!\n");
        goto err2;
    }
#endif

    /* show version */
    gfbg_version();
#ifdef GFBG_HW_VSYNC_SUPPORT
    (void)gfbg_vsync_init();
    (void)gfbg_pts_init();
#endif

    osal_printk("load gfbg.ko ....OK!\n");

    return TD_SUCCESS;
#if defined (CONFIG_OT_VO_FB_SEPARATE)
err2:
    gfbg_init_do_err2();
#endif
err1:
    gfbg_init_do_err1();
    g_gfbg_register = TD_FALSE;
    return TD_FAILURE;
}
#endif

td_void gfbg_cleanup(td_void)
{
    td_u32 i;
    if (g_gfbg_register) {
#ifdef CONFIG_OT_VO_FB_SEPARATE
        osal_free_irq(*(vou_get_gfbg_irq()), gfbg_interrupt_route);
#endif
#ifdef GFBG_HW_VSYNC_SUPPORT
        gfbg_pts_exit();
        gfbg_vsync_exit();
#endif
        gfbg_drv_set_tde_callback(TD_NULL);
        gfbg_drv_set_tde_rotate_callback(TD_NULL);
        gfbg_drv_set_vgs_rotate_callback(TD_NULL);

        g_drv_ops.gfbg_drv_deinit();

        /* remove all entry under dir 'gfbg' */
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
        gfbg_proc_remove_all_module();

        /* remove dir 'gfbg' under proc */
#endif
        for (i = 0; i < g_drv_ops.layer_count; i++) {
            if (g_drv_ops.capability[i].is_layer_support == TD_FALSE) {
                continue;
            }
            drv_gfbg_free_cmap(i);
            gfbg_overlay_cleanup(i, TD_TRUE);
        }
    }
    cmpi_unregister_module(OT_ID_FB);
    osal_printk("unload gfbg.ko ....OK!\n");
    return;
}

#ifdef CONFIG_OT_VO_FB_SEPARATE
static td_s32 gfbg_drv_graphics_get_vo_dev(td_u32 int_status, ot_vo_dev *vo_dev)
{
    /*
     * GFBG_DRV_GraphicsClearINTStatus will clear all reported interrupts.
     * If devices' interrupt are reported at the same time,
     * Will cause only one device to be processed, the interruption of other devices is ignored,
     * so it cannot be cleared here.
     * All reported interrupts can only be cleared one by one in GFBG_DRV_GraphicsGetINTDev
     */
    if (g_drv_ops.gfbg_drv_graphics_get_int_dev(int_status, vo_dev) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (*vo_dev == VO_INVALID_DEV) {
        gfbg_error("unknown dev:%d \n", *vo_dev);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 gfbg_interrupt(td_void)
{
    td_u32 int_status = 0;
    ot_vo_dev vo_dev = VO_INVALID_DEV;
    td_s32 i;
    struct fb_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;
    td_u32 vtthd_flag = 0x0;
    td_u32 frm_start_flag = 0x0;
    gfbg_osd_data layer_data = {0};

    g_drv_ops.gfbg_drv_graphics_get_int(&int_status);

    if (gfbg_drv_graphics_get_vo_dev(int_status, &vo_dev) != TD_SUCCESS) {
        return OSAL_IRQ_HANDLED;
    }

    /* Handling all graphics layers on this device */
    for (i = 0; i < GFX_MAX; i++) {
        if (g_drv_ops.capability[i].is_layer_support == TD_FALSE) {
            continue;
        }
        info = g_layer[i].info;
        par = (gfbg_par *)(info->par);
        /* If the layer is not open, do not act. if the layer is bound to the device. */
        if ((par->layer_open == TD_FALSE) || (vo_dev != graphic_drv_get_bind_dev(i))) {
            continue;
        }

        if (vo_dev == VO_DEV_DHD0) {
            vtthd_flag    = GFBG_INTMSK_HD0_VTTHD2;
            frm_start_flag = GFBG_INTMSK_HD0_VTTHD3;
        } else if (vo_dev == VO_DEV_DHD1) {
            vtthd_flag    = GFBG_INTMSK_HD1_VTTHD2;
            frm_start_flag = GFBG_INTMSK_HD1_VTTHD3;
        } else if (vo_dev == VO_DEV_DSD0) {
            vtthd_flag    = GFBG_INTMSK_SD0_VTTHD2;
            frm_start_flag = GFBG_INTMSK_SD0_VTTHD3;
        }
        /*
         * 1. The frame start interrupt and the vertical timing interrupt appear
         * in the interrupt status register at the same time.
         * Frame start interrupts and vertical timing interrupts
         * are processed one by one and cannot be processed at the same time because
         * interrupts are cleared one by one, and the interrupt processing order
         * and the clearing order should be also consistent.
         * The first clear must be processed first, then the post-cleared
         * 2, as long as there is a vertical timing interrupt, it is necessary to drive the g_display
         */
        if (int_status & vtthd_flag) {  /* VO vertical timing interrupt */
            gfbg_debug_print_vcnt(vo_dev, "vtth2");
            par->refresh_info.do_refresh_job = TD_FALSE;
            gfbg_interrupt_process(i);
        } else if (int_status & frm_start_flag) {  /* VO frame start interrupt */
            if (g_drv_ops.gfbg_drv_get_osd_data(par->layer_id, &layer_data) != TD_SUCCESS) {
                gfbg_error("failed to get layer%d's osd data!\n", par->layer_id);
            }
            par->displaying_addr = layer_data.buffer_phy_addr;
            gfbg_debug_print_vcnt(vo_dev, "vtth3");
            par->refresh_info.do_refresh_job = TD_TRUE;
            wake_up(&(par->do_refresh_job));
        }
    }
    return OSAL_IRQ_HANDLED;
}
static int gfbg_interrupt_route(td_s32 irq, td_void *dev_id)
{
    ot_unused(irq);
    ot_unused(dev_id);
    return gfbg_interrupt();
}
#else
static td_s32 gfbg_interrupt_vo_callback(const td_void *paraml, ot_vo_dev vo_dev, const td_void *paramr)
{
    gfbg_par *par = TD_NULL;
    td_u32 vtthd_flag = 0;
    td_u32 frm_start_flag = 0;
    td_u32 int_status;
    td_u32 layer_id;

    if (paramr == TD_NULL || paraml == TD_NULL) {
        gfbg_error("NULL pointer !\n");
        return TD_FAILURE;
    }
    int_status = *(td_u32 *)paramr;
    layer_id = *(td_u32 *)paraml;
    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        gfbg_error("invalid layer_id %d !should be in [%d, %d]\n", layer_id, 0, GFBG_MAX_LAYER_NUM - 1);
        return TD_FAILURE;
    }
    par = (gfbg_par *)(g_layer[layer_id].info->par);

    if ((vo_dev < 0) || (vo_dev >= VO_DEV_BUTT)) {
        gfbg_error("invalid vo_dev is %d!should be in [%d, %d]\n", vo_dev, VO_DEV_DHD0, VO_DEV_DSD0);
        return TD_FAILURE;
    }

    if (vo_dev == VO_DEV_DHD0) {
        vtthd_flag    = GFBG_INTMSK_HD0_VTTHD1;
        frm_start_flag = GFBG_INTMSK_HD0_VTTHD3;
    } else if (vo_dev == VO_DEV_DHD1) {
        vtthd_flag    = GFBG_INTMSK_HD1_VTTHD1;
        frm_start_flag = GFBG_INTMSK_HD1_VTTHD3;
    } else if (vo_dev == VO_DEV_DSD0) {
        vtthd_flag    = GFBG_INTMSK_SD0_VTTHD1;
        frm_start_flag = GFBG_INTMSK_SD0_VTTHD3;
    }

    if ((int_status & vtthd_flag) && (int_status & frm_start_flag)) {
        gfbg_error("vtth and frm_start sccur in the same vo dev in one isr!\n");
    }

    if (int_status & vtthd_flag) {
        gfbg_debug_print_vcnt(vo_dev, "vtth1");
        par->refresh_info.do_refresh_job = TD_FALSE;
        gfbg_interrupt_process(*(td_u32 *)paraml);
    } else if (int_status & frm_start_flag) {
        gfbg_debug_print_vcnt(vo_dev, "vtth3");
        par->refresh_info.do_refresh_job = TD_TRUE;
        wake_up(&(par->do_refresh_job));
    }
    return TD_SUCCESS;
}
#endif

static td_s32 gfbg_open_check_param(const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_GFBG_SRC_GFBG_MAIN_BODY_PART04_H_ */
