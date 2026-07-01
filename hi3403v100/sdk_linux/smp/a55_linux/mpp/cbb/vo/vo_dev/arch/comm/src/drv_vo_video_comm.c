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

#include "drv_vo_video_comm.h"
#include "drv_vo.h"
#include "drv_vo_video.h"
#include "hal_vo_layer_comm.h"
#include "hal_vo_video_comm.h"
#include "hal_vo_video.h"
#include "mm_ext.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("KERNEL_VO")
#if vo_desc("video coef")

td_s32 vo_drv_allocate_mem(td_u32 size, vo_mmz_buffer *mmz_buffer)
{
    td_s32 ret;
    cmpi_mmz_malloc_param malloc_param;
    td_void *mmz_name = TD_NULL;
    ot_mpp_chn chn;

    chn.mod_id = OT_ID_VO;
    chn.dev_id = 0;
    chn.chn_id = 0;
    if (call_sys_get_mmz_name(&chn, &mmz_name) != TD_SUCCESS) {
        vo_err_trace("vo_drv_allocate_mem get mmz name fail!\n");
        return OT_ERR_VO_NOT_READY;
    }

    malloc_param.mmz_name = mmz_name;
    malloc_param.buf_name = "vo_coef_buf";
    malloc_param.phys_addr = &mmz_buffer->start_phys_addr;
    malloc_param.virt_addr = &mmz_buffer->start_vir_addr;
    malloc_param.len = size;
    malloc_param.kernel_only = TD_TRUE;
    ret = cmpi_mmz_malloc_nocache(&malloc_param);
    if (ret != 0) {
        vo_err_trace("malloc vo_coef_buf failed\n");
        return OT_ERR_VO_NO_MEM;
    }

    (td_void)memset_s(mmz_buffer->start_vir_addr, size, 0, size);
    return TD_SUCCESS;
}

td_void vo_drv_delete_mem(vo_mmz_buffer *mmz_buffer)
{
    if (mmz_buffer != TD_NULL) {
        cmpi_mmz_free(mmz_buffer->start_phys_addr, mmz_buffer->start_vir_addr);
        mmz_buffer->start_phys_addr = 0;
        mmz_buffer->start_vir_addr = TD_NULL;
    }
}

td_void vo_drv_vhd_coef_para_up(vo_coef_buf buf_name)
{
    hal_para_set_para_up_vhd_chn(buf_name);
}

td_void vo_zme_coef_init(td_void)
{
    vo_coef_addr *coef_addr = vo_drv_get_coef_buf_addr();
    if (coef_addr->buf_base_addr.start_phys_addr == 0) {
        vo_drv_allocate_mem(VO_ALL_COEF_SIZE, &coef_addr->buf_base_addr);
        vo_drv_vhd_coef_buf_addr_distribute(coef_addr);
    }
}

#endif

#if vo_desc("video layer cfg")

ot_vo_layer vo_drv_get_hw_layer(ot_vo_layer layer)
{
#ifdef CONFIG_OT_VO_CASCADE
    ot_vo_dev dev;
    ot_vo_layer hw_layer;

    dev = vou_drv_get_video_layer_bind_dev(layer);
    hw_layer = (vo_drv_is_cas_dev(dev)) ? vo_drv_get_cas_phy_layer() : layer;
    return hw_layer;
#endif

    return layer;
}

td_void vou_drv_disable_all_area(ot_vo_layer layer)
{
    ot_vo_layer hw_layer = vo_drv_get_hw_layer(layer);
    hal_video_set_all_area_disable(hw_layer);
}

td_void vou_drv_layer_enable(ot_vo_layer layer, td_bool enable)
{
    ot_vo_layer hw_layer = vo_drv_get_hw_layer(layer);

    hal_video_hfir_set_ck_gt_en(hw_layer, enable);
    hal_layer_enable_layer(hw_layer, enable);
    hal_video_set_layer_ck_gt_en(hw_layer, enable);
}

td_void vou_drv_set_layer_data_fmt(ot_vo_layer layer, vou_layer_pixel_format data_fmt)
{
    ot_vo_layer hw_layer = vo_drv_get_hw_layer(layer);
    hal_disp_layer hal_layer;
    hal_disp_pixel_format disp_data_fmt;

    hal_layer = vou_drv_convert_layer(hw_layer);
    disp_data_fmt = vo_drv_convert_data_format(data_fmt);
    hal_layer_set_layer_data_fmt(hal_layer, disp_data_fmt);
}

#endif

#if vo_desc("video layer")

td_s32 vou_drv_get_video_layer_bind_dev(ot_vo_layer layer)
{
    vo_drv_layer *layer_ctx = vo_drv_get_layer_ctx(layer);
    return layer_ctx->bind_dev;
}

td_u32 vou_drv_get_layer_chn_num(ot_vo_layer layer)
{
    vo_drv_layer *drv_layer_ctx = vo_drv_get_layer_ctx(layer);
    return drv_layer_ctx->chn_num;
}

#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
