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

#ifndef DRV_VO_COMM_H
#define DRV_VO_COMM_H
#include "mkp_vo.h"
#include "drv_vo_coef_comm.h"
#include "hal_vo_def.h"
#include "hal_vo_reg.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

typedef enum {
    /* for video surface interface */
    VOU_LAYER_PIXEL_FORMAT_SP_YCBCR_400 = 0x1,
    VOU_LAYER_PIXEL_FORMAT_SP_YCBCR_420 = 0x2,
    VOU_LAYER_PIXEL_FORMAT_SP_YCBCR_422 = 0x3,
    VOU_LAYER_PIXEL_FORMAT_BUTT
} vou_layer_pixel_format;

/* vou interrupt mask type */
typedef enum {
    VO_INTMSK_NONE = 0,
    VO_INTMSK_DHD0_VTTHD1 = 0x1,
    VO_INTMSK_DHD0_VTTHD2 = 0x2,
    VO_INTMSK_DHD0_VTTHD3 = 0x4,
    VO_INTMSK_DHD0_UFINT = 0x8,

    VO_INTMSK_DHD1_VTTHD1 = 0x10,
    VO_INTMSK_DHD1_VTTHD2 = 0x20,
    VO_INTMSK_DHD1_VTTHD3 = 0x40,
    VO_INTMSK_DHD1_UFINT = 0x80,

    VO_INTMSK_DSD_VTTHD1 = 0x100,
    VO_INTMSK_DSD_VTTHD2 = 0x200,
    VO_INTMSK_DSD_VTTHD3 = 0x400,
    VO_INTMSK_DSD_UFINT = 0x800,

    VO_INTMSK_B0_ERR = 0x1000,
    VO_INTMSK_B1_ERR = 0x2000,
    VO_INTMSK_B2_ERR = 0x4000,

    VO_INTMSK_WBC_DHDOVER = 0x8000,

    VO_INTMSK_VGA_VDAC = 0x70000,   /*  INT VDAC0/VDAC1/VDAC2 */
    VO_INTMSK_CVBS_VDAC = 0x80000,  /*  INT VDAC3 */

    VO_INTMSK_V0_TUNL_INT = 0x100000,
    VO_INTMSK_V1_TUNL_INT = 0x200000,

    VO_INTREPORT_ALL = 0xffffffff
} vo_int_mask;

typedef enum {
    VO_INT_MODE_FRAME = 0x0,
    VO_INT_MODE_FIELD = 0x1,
    VO_INT_MODE_BUTT
} vo_int_mode;

#if vo_desc("pub")
hal_disp_syncinfo *vo_drv_comm_get_sync_timing(ot_vo_intf_sync sync);
td_s32 vo_drv_check_cvbs_sync(ot_vo_dev dev, ot_vo_intf_type intf_type, ot_vo_intf_sync intf_sync);
hal_disp_pixel_format vo_drv_convert_data_format(vou_layer_pixel_format data_fmt);
td_void vo_drv_board_init(td_void);
td_void vo_drv_csc_trans_to_register(csc_coef *coef);
#endif

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KENEL_VO")

td_void vo_drv_board_deinit(td_void);

typedef struct vo_all_int_type {
    ot_vo_dev vtth_dev[OT_VO_MAX_PHYS_DEV_NUM];
    ot_vo_dev vga_dev;
    ot_vo_dev cvbs_dev;
} vo_int_type;

#if vo_desc("layer coef")
typedef struct {
    td_phys_addr_t start_phys_addr;
    td_void *start_vir_addr;
    td_u32 size;
} vo_mmz_buffer;

typedef struct {
    vo_mmz_buffer buf_base_addr;
    td_u32 u32size;

    td_u8 *coef_vir_addr[VO_COEF_BUF_BUTT];
    td_phys_addr_t coef_phys_addr[VO_COEF_BUF_BUTT];
} vo_coef_addr;

typedef struct {
    td_u32 data3;
    td_u32 data2;
    td_u32 data1;
    td_u32 data0;
    td_u32 depth;
} vo_drv_u128;

typedef enum {
    DRV_COEF_DATA_TYPE_U8 = 0,
    DRV_COEF_DATA_TYPE_S8,
    DRV_COEF_DATA_TYPE_U16,
    DRV_COEF_DATA_TYPE_S16,
    DRV_COEF_DATA_TYPE_U32,
    DRV_COEF_DATA_TYPE_S32,
    DRV_COEF_DATA_TYPE_BUTT
} vo_drv_coef_data_type;

typedef enum {
    VO_RM_COEF_MODE_TYP = 0x0,
} vo_rm_coef_mode;

typedef struct {
    vo_rm_coef_mode coef_mode;
    td_u32 phase;
    td_u32 tap;
    td_s16 (*typ_lut)[VO_MAX_ZME_TAP];
    td_s16 (*gen_lut)[VO_MAX_ZME_TAP];
    td_s16 *max_val;
    td_s16 *min_val;
} vo_zme_coef_gen_info;

#endif

#if vo_desc("pub")
td_void vo_drv_dev_info_de_init(td_void);
td_void vo_func_zme_exchange_lut(td_u32 phase,
                                 td_u32 tap,
                                 td_s16 old_lut[VO_MAX_ZME_PHASE][VO_MAX_ZME_TAP],
                                 td_s16 new_lut[VO_MAX_ZME_PHASE][VO_MAX_ZME_TAP]);
td_void vo_drv_zme_exchange_lut(td_u32 phase,
                                td_u32 tap,
                                const td_s16 *old_lut,
                                td_s16 new_lut[VO_MAX_ZME_PHASE][VO_MAX_ZME_TAP]);
vou_zoom_coef vo_drv_get_zoom_coef(td_u32 in, td_u32 out);
td_void vo_drv_set_reg(volatile reg_vdp_regs *reg);
#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of DRV_VO_COMM_H */
