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
#ifndef MPI_HDMI_H
#define MPI_HDMI_H

#include "ot_common_hdmi.h"
#include "drv_hdmi_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define hdmi_check_null_return(ptr, ret)           \
    do {                                           \
        if ((ptr) == TD_NULL) {                    \
            hdmi_err_trace("pointer is NULL!!\n"); \
            return (ret);                          \
        }                                          \
    } while (0)

td_s32 mpi_hdmi_com_init(void);

td_s32 mpi_hdmi_com_deinit(void);

td_s32 mpi_hdmi_com_open(ot_hdmi_id hdmi, const hdmi_open *open_param);

td_s32 mpi_hdmi_com_close(ot_hdmi_id hdmi);

td_s32 mpi_hdmi_com_get_status(ot_hdmi_id hdmi, ot_hdmi_status *status);

td_s32 mpi_hdmi_com_get_sink_capability(ot_hdmi_id hdmi, drv_hdmi_sink_capability *drv_cap);

td_s32 mpi_hdmi_com_set_attr(ot_hdmi_id hdmi, drv_hdmi_property *property);

td_s32 mpi_hdmi_com_get_attr(ot_hdmi_id hdmi, drv_hdmi_property *property);

td_s32 mpi_hdmi_com_start(ot_hdmi_id hdmi);

td_s32 mpi_hdmi_com_stop(ot_hdmi_id hdmi);

td_s32 mpi_hdmi_com_force_get_edid(ot_hdmi_id hdmi, td_u8 *edid, td_u32 *edid_len);

td_s32 mpi_hdmi_com_set_infoframe(ot_hdmi_id hdmi, drv_hdmi_infoframe *user_infoframe);

td_s32 mpi_hdmi_com_get_infoframe(ot_hdmi_id hdmi, drv_hdmi_infoframe *drv_infoframe);

td_s32 mpi_hdmi_com_reg_callback_func(ot_hdmi_id hdmi, const ot_hdmi_callback_func *callback_func);

td_s32 mpi_hdmi_com_unreg_callback_func(ot_hdmi_id hdmi, const ot_hdmi_callback_func *callback_func);

td_s32 mpi_hdmi_com_set_hw_spec(ot_hdmi_id hdmi, const ot_hdmi_hw_spec *hw_spec);

td_s32 mpi_hdmi_com_get_hw_spec(ot_hdmi_id hdmi, ot_hdmi_hw_spec *hw_spec);

td_s32 mpi_hdmi_com_set_avmute(ot_hdmi_id hdmi, td_bool enable_avmute);

td_s32 mpi_hdmi_com_mod_param_set(ot_hdmi_id hdmi, const ot_hdmi_mod_param *user_mod_param);

td_s32 mpi_hdmi_com_mod_param_get(ot_hdmi_id hdmi, ot_hdmi_mod_param *user_mod_param);

hdmi_deep_color deep_color_user2drv(ot_hdmi_deep_color user_deep_color);

ot_hdmi_deep_color deep_color_drv2user(hdmi_deep_color kernel_deep_color);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif

