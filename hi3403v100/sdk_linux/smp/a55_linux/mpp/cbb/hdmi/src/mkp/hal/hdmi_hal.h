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
#ifndef HDMI_HAL_H
#define HDMI_HAL_H

#include "drv_hdmi_common.h"

typedef td_void *hdmi_handle;

typedef enum {
    HDMI_DEBUG_CMD_COLOR_BAR,
    HDMI_DEBUG_CMD_SW_RESET,
    HDMI_DEBUG_CMD_RGB2YUV,
    HDMI_DEBUG_CMD_YUV2RGB,
    HDMI_DEBUG_CMD_DITHER,
    HDMI_DEBUG_CMD_BYPASS,
    HDMI_DEBUG_CMD_DDC_FREQ,
    HDMI_DEBUG_CMD_PHY_DEFAULT_GET,
    HDMI_DEBUG_CMD_PHY_PARA_SET,
    HDMI_DEBUG_CMD_DUMP,
    HDMI_DEBUG_CMD_PROC_MACH,
    HDMI_DEBUG_CMD_PROC_SCDC,
    HDMI_DEBUG_CMD_PROC_HDCP14,
    HDMI_DEBUG_CMD_PROC_HDCP22,
    HDMI_DEBUG_CMD_PROC_DDC,
    HDMI_DEBUG_CMD_PROC_CECTX,
    HDMI_DEBUG_CMD_PROC_CECRX,
    HDMI_DEBUG_CMD_DBG_VIDEO_GET,
    HDMI_DEBUG_CMD_DBG_VIDEO_SET,
    HDMI_DEBUG_CMD_SSC,
    HDMI_DEBUG_CMD_FRL
} hdmi_debug_cmd;

typedef struct {
    td_u32 reg_addr;
    td_u32 reg_vaule;
} hdmi_hal_reg;

typedef struct {
    hdmi_handle             hdmi_hw;
    hdmi_handle             hdmi_dev;
    td_u32                  hdmi_id;
    hdmi_tx_capability_data tx_capability;
    hdmi_callback           callback;
    hdmi_video_config       video_cfg;
    hdmi_audio_config       audio_cfg;
    td_char                *base_addr;
    td_char                *phy_addr;
} hdmi_hal_context;

typedef struct {
    td_u32 disp_fmt;
    td_u32 pix_clk;
    hdmi_colorspace color_space;
    hdmi_deep_color deep_color;
} hdmi_hal_base_param;

typedef struct hdmi_hal_ {
    hdmi_hal_context hal_ctx;
    td_void (*hal_hdmi_hardware_init)(const struct hdmi_hal_ *hal);
    td_void (*hal_hdmi_tmds_mode_set)(const struct hdmi_hal_ *hal, hdmi_tmds_mode tmds_mode);
    td_void (*hal_hdmi_avmute_set)(const struct hdmi_hal_ *hal, td_bool av_mute);
    td_void (*hal_hdmi_infoframe_set)(const struct hdmi_hal_ *, hdmi_infoframe_id, const td_u8 *in_buffer, td_u32 len);
    td_void (*hal_hdmi_infoframe_enable_set)(const struct hdmi_hal_ *, hdmi_infoframe_id, td_bool);
    td_s32  (*hal_hdmi_video_path_set)(const struct hdmi_hal_ *hal, const hdmi_video_config *video_cfg);
    td_void (*hal_hdmi_phy_output_enable_set)(const struct hdmi_hal_ *hal, td_bool enable);
    td_void (*hal_hdmi_phy_power_enable_set)(const struct hdmi_hal_ *hal, td_bool enable);
    td_void (*hal_hdmi_tx_capability_get)(const struct hdmi_hal_ *hal, hdmi_tx_capability_data *tx_capability);
    td_void (*hal_hdmi_emi_status_get)(const struct hdmi_hal_ *hal, hdmi_emi_status *emi_status);
    td_void (*hal_hdmi_csc_param_set)(struct hdmi_hal_ *hal, const hdmi_video_config *video_cfg);
    td_void (*hal_hdmi_phy_set)(const struct hdmi_hal_ *hal, const hdmi_phy_cfg *phy_cfg);
    td_void (*hal_hdmi_ctrl_reset)(const struct hdmi_hal_ *hal);
    td_s32  (*hal_hdmi_phy_hw_spec_set)(const struct hdmi_hal_ *hal, td_u32 tmds_clk, const hdmi_hw_spec *hw_spec);
    td_s32  (*hal_hdmi_phy_hw_spec_get)(const struct hdmi_hal_ *hal, hdmi_hw_spec *hw_spec);
    td_void (*hal_hdmi_sequencer_handler_process)(const struct hdmi_hal_ *hal);
    td_void (*hal_hdmi_hardware_status_get)(const struct hdmi_hal_ *hal, hdmi_hardware_status *hw_status);
    td_void (*hal_hdmi_hot_plug_status_get)(const struct hdmi_hal_ *hal, td_bool *hot_plug);
    td_void (*hal_hdmi_hdp_intr_status_get)(const struct hdmi_hal_ *hal, td_bool *intr_status);
    td_s32  (*hal_hdmi_audio_path_set)(const struct hdmi_hal_ *hal, const hdmi_audio_config *audio_cfg);
    td_void (*hal_hdmi_audio_path_enable_set)(const struct hdmi_hal_ *hal, td_bool enable);
    td_s32  (*hal_hdmi_edid_raw_data_read)(const struct hdmi_hal_ *hal, td_u32 size, td_u8 out_buffer[]);
    td_void (*hal_hdmi_phy_output_enable_get)(const struct hdmi_hal_ *hal, td_bool *enable);
    td_void (*hal_hdmi_video_mute_set)(const struct hdmi_hal_ *hal, td_bool video_mute);
    td_void (*hal_hdmi_black_data_set)(const struct hdmi_hal_ *hal, const hdmi_black_frame_info *black_pram);
    td_void (*hal_hdmi_debug)(const struct hdmi_hal_ *hal, hdmi_debug_cmd debug_cmd, const td_void *data);
    td_u32 *(*hal_hdmi_base_addr_get)(const struct hdmi_hal_ *hal);
#ifdef HDMI_SCDC_SUPPORT
    td_void (*hal_hdmi_scdc_status_get)(const struct hdmi_hal_ *hal, hdmi_scdc_status *scdc_status);
    td_void (*hal_hdmi_scdc_status_set)(const struct hdmi_hal_ *hal, const hdmi_scdc_status *scdc_status);
    td_s32  (*hal_hdmi_scdc_process)(const struct hdmi_hal_ *hal, scdc_cmd scdc_cmd, td_void *data);
#endif
} hdmi_hal;

td_s32 hal_hdmi_open(const hdmi_hal_init *hal_init, hdmi_hal **hal_handle);

void hal_hdmi_close(hdmi_hal *hal);

#endif /* HDMI_HAL_H */

