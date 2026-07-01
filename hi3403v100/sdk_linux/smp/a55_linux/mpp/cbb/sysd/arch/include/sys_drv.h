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

#ifndef SYS_DRV_H
#define SYS_DRV_H

#include "ot_osal.h"

#include "ot_common_sys.h"
#include "ot_inner_sys.h"
#include "ot_inner_sys_ipc.h"

#include "sys_ext.h"
#include "sys_dci_reg.h"
#include "mkp_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    td_phys_addr_t yh_coef_phy_addr;
    td_phys_addr_t yv_coef_phy_addr;
    td_phys_addr_t ch_coef_phy_addr;
    td_phys_addr_t cv_coef_phy_addr;
    td_void *yh_coef_vir_addr;
    td_void *yv_coef_vir_addr;
    td_void *ch_coef_vir_addr;
    td_void *cv_coef_vir_addr;
    td_s32 yh_avg;
    td_s32 yv_avg;
    td_s32 ch_avg;
    td_s32 cv_avg;
    td_u32 yhstep;
    td_u32 yvstep;
    td_u32 chstep;
    td_u32 cvstep;
    td_u32 yhrat;
    td_u32 yvrat;
    td_u32 chrat;
    td_u32 cvrat;
} scl_cfg;

typedef struct {
    td_u32 yhrat;
    td_u32 yvrat;
    td_u32 chrat;
    td_u32 cvrat;
} sys_scl_rate;

typedef enum {
    SCLCOEF_NORMAL = 0,
    SCLCOEF_IVE_TAP2 = 1,
    SCLCOEF_IVE_TAP4 = 2,
    SCLCOEF_IVE_TAP6 = 3,
    SCLCOEF_IVE_TAP8 = 4,
    SCLCOEF_DIS_LINEAR = 5,
    SCLCOEF_MCF = 6,
    SCLCOEF_BUTT
} sclcoef_mode;

typedef enum {
    MCF_LAYER_MAX = 0,
    MCF_LAYER_MEDIUM = 1,
    MCF_LAYER_MIN = 2,
    MCF_LAYER_BUTT
} mcf_layer;

typedef struct {
    td_bool online;
    td_bool deflicker;
    td_bool osd_mode;
    td_bool is_big_image_path;         /* only for mcf scale */
    mcf_layer mcf_pic_layer;           /* only for mcf scale */
    sclcoef_mode ive_sclcoef_mode;
} scale_coef_opt;

typedef struct {
    ot_mod_id mod_id;
    ot_pixel_format pixel_format;
    ot_compress_mode compress_mode;
    ot_data_bit_width bit_width;
    td_u32 bayer_mode;
    td_u32 width;
    td_u32 height;
    td_bool mirror;
    td_bool tile;  /* only compress by GDC is 1. */
    td_u32 tile_height;  /* GDC */
} sys_global_cmp_input; /* end : segment compress parameter data definition. */

typedef struct {
    td_phys_addr_t dci_coef_phy_addr;
    s_tmu_regs_type *dci_coef_vir_addr;
} sys_dci_addr;

td_s32 sys_drv_init(td_void);
td_s32 sys_drv_exit(td_void);

td_s32 sys_drv_set_align(td_u32 align);
td_s32 sys_drv_get_stride(td_u32 width, td_u32 *stride);

td_u32 sys_drv_get_chip_version(td_void);

td_s32 sys_drv_drv_ioctrl(const ot_mpp_chn *mpp_chn, td_u32 func_id, td_void *io_args);

td_s32 sys_drv_get_custom_code(td_u32 *custom_code);

td_s32 sys_drv_get_chip_id(td_u32 *chip_id);

td_void sys_drv_get_vb_cfg(const td_void *vb_base_info, td_void *vb_cal_config);
td_void sys_drv_get_share_vb_cfg(const td_void *vb_base_info, td_void *vb_cal_config, td_bool is_share_head);

td_void sys_drv_get_cmp_cfg(const td_void *video_frame, const td_void *y_cmp_cfg, const td_void *c_cmp_cfg,
    const td_void *cmp_mode_ex_param);

td_void sys_drv_vi_vpss_mode_init(td_void);
td_void sys_drv_vi_video_mode_init(td_void);

#ifdef CONFIG_OT_TZASC_SUPPORT
td_s32 sys_drv_get_security_version(sys_security_version *version);
#endif

td_s32 sys_drv_get_unique_id(ot_unique_id *unique_id);

td_u64 sys_drv_get_local_pts(td_void);

#ifdef CONFIG_OT_SYS_MUTEX_REG_SUPPORT
td_s32 sys_drv_get_mutex(td_u32 vaule);
td_void sys_drv_release_mutex(td_u32 vaule);
#endif

#ifdef CONFIG_OT_VI_PTS
td_u64 sys_drv_get_vi_local_pts(td_s32 dev_id);
#endif

#ifdef __cplusplus
}
#endif

#endif /* end of #ifndef SYS_DRV_H */
