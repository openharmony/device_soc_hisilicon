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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART04_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART04_H_

    td_bool emi_en;
    hdmi_trace_len trace_len;
} hdmi_mode_param;

typedef struct {
    td_u32 pixel_clk;
    td_u32 tmds_clk; /* in khz */
    td_bool emi_enable;
    hdmi_deep_color deep_color; /* deep color(color depth) */
    hdmi_phy_mode_cfg mode_cfg; /* tmds/frl/tx ffe */
    hdmi_trace_len trace_len;
    hdmi_colorspace color_space;
    hdmi_work_mode rate; /* lane and rate */
    hdmi_txfff_mode aen_tx_ffe[AEN_TX_FFE_LEN]; /* tx ffe */
} hdmi_phy_cfg;

hdmi_video_code_vic drv_hdmi_vic_search(hdmi_video_timing, hdmi_picture_aspect, td_bool);

td_void hdmi_reg_write(volatile td_u32 *reg_addr, td_u32 value);

td_u32 hdmi_reg_read(volatile td_u32 *reg_addr);

td_u32 drv_hdmi_vic_to_index(td_u32 vic);

hdmi_video_timing drv_hdmi_video_timing_get(hdmi_video_code_vic vic, hdmi_picture_aspect aspect);

hdmi_video_timing drv_hdmi_vsif_video_timing_get(hdmi_vsif_vic vic);

hdmi_video_4k_def *drv_hdmi_video_codes_4k_get(td_u32 cnt);

hdmi_video_def *drv_hdmi_comm_format_param_get(hdmi_video_code_vic vic);


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART04_H_ */
