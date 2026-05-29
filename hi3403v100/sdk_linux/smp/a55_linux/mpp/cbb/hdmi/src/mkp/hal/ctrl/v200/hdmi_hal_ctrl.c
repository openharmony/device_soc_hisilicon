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
#include "hdmi_hal_ctrl.h"
#include "hdmi_reg_tx.h"
#include "hdmi_reg_audio_path.h"
#include "hdmi_reg_video_path.h"
#include "hdmi_reg_aon.h"
#include "hdmi_reg_ctrl.h"
#include "hdmi_hal_ncts.h"
#include "hdmi_hal_ddc.h"
#include "hdmi_hal_intf.h"
#include "hdmi_reg_crg.h"
#include "hdmi_product_define.h"
#include "hdmi_hal_machine.h"
#include "drv_hdmi_common.h"

#define CTRL_CHANNEL0_Y          0x0
#define CTRL_CHANNEL0_Y422       0x3
#define CTRL_CHANNEL1_CB         0x1
#define CTRL_CHANNEL1_Y422       0x4
#define CTRL_CHANNEL2_CR         0x2
#define CTRL_CHANNEL2_Y422       0x3
#define CTRL_COLORMETRY_OUT_MASK 0xfc
#define CTRL_COLORMETRY_OUT_BIT  0   /* out colormetry offset in reg_csc_mode */
#define CTRL_COLORMETRY_IN_MASK  0xcf
#define CTRL_COLORMETRY_IN_BIT   0x4 /* in colormetry offset in reg_csc_mode */
#define CTRL_COLORMETRY_MASK     0x3
#define CTRL_RGB_OUT_BIT         0x3 /* out color space offset in reg_csc_mode */
#define CTRL_RGB_IN_BIT          0x7 /* in color space offset in reg_csc_mode */
#define CTRL_QUANTIZAION_OUT_BIT 0x2 /* out quantization offset in reg_csc_mode */
#define CTRL_QUANTIZAION_IN_BIT  0x6 /* in quantization offset in reg_csc_mode */
#define CTRL_SYCN_POL_V_BIT      0   /* vsync offset in reg_inver_sync */
#define CTRL_SYCN_POL_H_BIT      1   /* hsync offset in reg_inver_sync */
#define CTRL_SYCN_POL_DE_BIT     0x3 /* DE offset in reg_inver_sync */
#define CTRL_BLACK_Y_CB_CR       0x000000
#define CTRL_BLACK_DATA_YUV_CR   0x200 /* cr val for yuv black */
#define CTRL_BLACK_DATA_YUV_Y    0x40  /* y val for yuv black */
#define CTRL_BLACK_DATA_YUV_CB   0x200 /* cb val for yuv black */
#define CTRL_BLACK_DATA_RGB_R    0x40  /* r val for rgb black */
#define CTRL_BLACK_DATA_RGB_G    0x40  /* g val for rgb black */
#define CTRL_BLACK_DATA_RGB_B    0x40  /* b val for rgb black */
#define CTRL_AUDIO_INVALID_CFG   0xff
#define CTRL_AUDIO_INVALID_RATE  0xffffffff
#define CTRL_REAET_WAIT_TIME     5
#define HDMI_INFOFRAME_DATA_SIZE 31

typedef struct {
    td_bool ver_down_samp_bypass;
    td_bool ver_down_samp_enable;
    td_bool hor_down_samp_enable;
    td_bool demux_y420_enable;
    td_bool pxl_div_enable;
    td_bool hor_filter_enable;
} hdmi_video_down_samp;

typedef enum {
    HDMI_AUDIO_FIFO_MAP_SD0,
    HDMI_AUDIO_FIFO_MAP_SD1,
    HDMI_AUDIO_FIFO_MAP_SD2,
    HDMI_AUDIO_FIFO_MAP_SD3,
    HDMI_AUDIO_FIFO_MAP_BUTT
} hdmi_audio_fifo_map;

typedef enum {
    HDMI_AUDIO_I2S_SD_NONE,
    HDMI_AUDIO_I2S_SD_0,
    HDMI_AUDIO_I2S_SD_2,
    HDMI_AUDIO_I2S_SD_3 = 0x4,
    HDMI_AUDIO_I2S_SD_4 = 0x8,
    HDMI_AUDIO_I2S_SD_ALL = 0xf
} hdmi_audio_i2s_sd;

typedef enum {
    HDMI_AUDIO_HBRA_MASK_ALL,
    HDMI_AUDIO_HBRA_MASK_NONE = 0xf
} hdmi_i2s_hbra_mask;

typedef enum {
    HDMI_AUDIO_CLK_ACCURACY_LEVEL2,
    HDMI_AUDIO_CLK_ACCURACY_LEVEL1,
    HDMI_AUDIO_CLK_ACCURACY_LEVEL3,
    HDMI_AUDIO_CLK_ACCURACY_NOTMATCH
} hdmi_audio_clk_accuracy;

typedef struct {
    td_bool test;
    hdmi_i2s_hbra_mask hbra_mask;
} ctrl_fifo;

typedef struct {
    td_bool                 professional_app;
    td_bool                 compress;
    td_u32                  samp_rate;
    td_u32                  org_rate;
    td_u32                  out_bit_length;
    hdmi_audio_clk_accuracy clk_accuracy;
    hdmi_audio_fifo_map     fifo_map;
} ctrl_i2s_status;

typedef struct {
    td_bool channel_lr_swap;
    td_u32  in_bit_length;
    td_bool vbit_compress;
    td_bool data_dir_msb_first;
    td_bool justify_ws_right;
    td_bool polarity_ws_msb_left;
    td_bool sck_shift_one;
    td_bool hbra_enable;
} ctrl_i2s;

typedef struct {
    td_u32 bit_value;
    td_u32 bit_reg;
} hdmi_audio_samp_bit;

typedef struct {
    td_u32 rate_value;
    td_u32 rate_reg;
    td_u32 rate_org_reg;
} hdmi_i2s_samp_rate;

typedef struct {
    td_u32 rate_value;
    td_u32 rate_reg;
} hdmi_spdif_samp_rate;

typedef struct {
    td_bool in_rgb;
    td_bool out_rgb;
    td_bool csc_enable;
    td_bool y422_enable;
    td_bool y420_enable;
    td_bool dwsm_hori_enable;
    td_bool dwsm_vert_enable;
} ctrl_colorspace_en;

static hdmi_audio_samp_bit g_samp_bit[] = {
    /* bit reg */
    { 16, 0x2 },
    { 17, 0xc },
    { 18, 0x4 },
    { 19, 0x8 },
    { 20, 0xa },
    { 21, 0xd },
    { 22, 0x5 },
    { 23, 0x9 },
    { 24, 0xb }
};

static hdmi_i2s_samp_rate g_i2s_samp_rate[] = {
    /* sample rate value   reg  org_reg */
    { HDMI_SAMPLE_RATE_32K, 0x03, 0xc },
    { HDMI_SAMPLE_RATE_44K, 0x00, 0xf },
    { HDMI_SAMPLE_RATE_48K, 0x02, 0xd }
};

static hdmi_ctrl_info g_ctrl_info[HDMI_ID_MAX];

static hdmi_ctrl_info *ctrl_info_ptr_get(hdmi_device_id hdmi)
{
    if ((td_s32)hdmi < HDMI_ID_MAX) {
        return &g_ctrl_info[hdmi];
    }
    return TD_NULL;
}

static td_u32 ctrl_i2s_rate_reg_get(td_u32 rate_value)
{
    td_u32 i = 0;
    td_u32 ret = 0x2; /* 48Khz */
    hdmi_i2s_samp_rate *i2s_rate = TD_NULL;

    for (i2s_rate = &g_i2s_samp_rate[0]; i2s_rate && (i < hdmi_array_size(g_i2s_samp_rate)); i2s_rate++, i++) {
        if (i2s_rate->rate_value == rate_value) {
            ret = i2s_rate->rate_reg;
            break;
        }
    }

    if (i >= hdmi_array_size(g_i2s_samp_rate)) {
        hdmi_err("can't find aduio rate = %u, force 48k\n", rate_value);
    }

    return ret;
}

static td_u32 ctrl_i2s_rate_org_reg_get(td_u32 rate_value)
{
    td_u32 i = 0;
    td_u32 ret = 0xd; /* 48Khz */
    hdmi_i2s_samp_rate *i2s_rate = TD_NULL;

    for (i2s_rate = &g_i2s_samp_rate[0]; i2s_rate && (i < hdmi_array_size(g_i2s_samp_rate)); i2s_rate++, i++) {
        if (i2s_rate->rate_value == rate_value) {
            ret = i2s_rate->rate_org_reg;
            break;
        }
    }

    if (i >= hdmi_array_size(g_i2s_samp_rate)) {
        hdmi_err("can't find aduio rate = %u, force 48k\n", rate_value);
    }

    return ret;
}

static td_u32 ctrl_i2s_rate_value_get(td_u32 id)
{
    td_u32 i = 0;
    td_u32 rate_reg, rate_org_reg;
    hdmi_i2s_samp_rate *i2s_rate = TD_NULL;

    rate_reg = hdmi_reg_chst_byte3_fs_get(id);
    rate_org_reg = hdmi_reg_chst_byte4_org_fs_get(id);

    for (i2s_rate = &g_i2s_samp_rate[0]; i2s_rate && (i < hdmi_array_size(g_i2s_samp_rate)); i2s_rate++, i++) {
        if ((i2s_rate->rate_reg == rate_reg) && (i2s_rate->rate_org_reg == rate_org_reg)) {
            return i2s_rate->rate_value;
        }
    }

    return HDMI_SAMPLE_RATE_UNKNOWN;
}

static td_u32 ctrl_audio_bit_reg_get(td_u32 bit_value)
{
    td_u32 i = 0;
    td_u32 ret = 0x2; /* 16bit */
    hdmi_audio_samp_bit *audio_bit = TD_NULL;

    for (audio_bit = &g_samp_bit[0]; audio_bit && (i < hdmi_array_size(g_samp_bit)); audio_bit++, i++) {
        if (audio_bit->bit_value == bit_value) {
            ret = audio_bit->bit_reg;
            break;
        }
    }

    if (i >= hdmi_array_size(g_samp_bit)) {
        hdmi_err("can't find audio bit= %u, force 16\n", bit_value);
    }

    return ret;
}

static td_u32 ctrl_audio_bit_value_get(td_u32 id, td_bool spdif)
{
    td_u32 i = 0;
    td_u32 bit_reg;
    hdmi_audio_samp_bit *audio_bit = TD_NULL;

    if (spdif) {
        bit_reg = hdmi_reg_aud_length_get(id);
    } else {
        bit_reg = hdmi_reg_chst_byte4_length_get(id);
    }

    for (audio_bit = &g_samp_bit[0]; audio_bit && (i < hdmi_array_size(g_samp_bit)); audio_bit++, i++) {
        if (audio_bit->bit_reg == bit_reg) {
            return audio_bit->bit_value;
        }
    }

    return HDMI_AUDIO_BIT_DEPTH_UNKNOWN;
}

static td_bool ctrl_audio_mute_get(td_u32 id)
{
    if (hdmi_reg_aud_mute_en_get(id)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_void ctrl_audio_i2s_enable_set(td_u32 id, td_bool enable)
{
    hdmi_audio_i2s_sd audio_i2s_enable;
    audio_i2s_enable = enable ? HDMI_AUDIO_I2S_SD_ALL : HDMI_AUDIO_I2S_SD_NONE;
    hdmi_reg_aud_i2s_en_set(id, audio_i2s_enable);
    return;
}

static td_bool ctrl_audio_i2s_enable_get(td_u32 id)
{
    if (hdmi_reg_aud_i2s_en_get(id)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_void ctrl_audio_spdif_enable_set(td_u32 id, td_bool spdif_enable)
{
    hdmi_reg_aud_spdif_en_set(id, spdif_enable);
    return;
}

static td_bool ctrl_audio_spdif_enable_get(td_u32 id)
{
    if (hdmi_reg_aud_spdif_en_get(id)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_void ctrl_audio_path_enable_set(td_u32 id, td_bool audio_enable)
{
    hdmi_reg_aud_in_en_set(id, audio_enable);
    return;
}

static td_bool ctrl_audio_path_enable_get(td_u32 id)
{
    if (hdmi_reg_aud_in_en_get(id)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_void ctrl_audio_path_n_value_set(td_u32 id, td_u32 audio_n)
{
    hdmi_reg_acr_n_val_sw_set(id, audio_n);
    return;
}

static td_u32 ctrl_audio_path_n_value_get(td_u32 id)
{
    return hdmi_reg_acr_n_val_sw_get(id);
}

static td_void ctrl_audio_path_cts_value_set(td_u32 id, td_bool sw_cts)
{
    hdmi_reg_acr_cts_hw_sw_sel_set(id, sw_cts);
    return;
}

static td_u32 ctrl_audio_path_cts_value_get(td_u32 id)
{
    td_u32 cts_value;

    /* wait reg valid */
    osal_msleep(1);
    /* sw */
    if (hdmi_reg_acr_cts_hw_sw_sel_get(id)) {
        cts_value = hdmi_reg_acr_cts_val_sw_get(id);
    } else { /* hw */
        cts_value = hdmi_reg_acr_cts_val_hw_get(id);
    }

    return cts_value;
}

static td_void ctrl_audio_path_layout_set(td_u32 id, td_bool muti_layout)
{
    hdmi_reg_aud_layout_set(id, muti_layout);
    return;
}

static td_u32 ctrl_audio_path_layout_get(td_u32 id)
{
    return hdmi_reg_aud_layout_get(id);
}

static td_void ctrl_audio_hbra_enable_set(td_u32 id, td_bool hbra_enable)
{
    hdmi_reg_i2s_hbra_on_set(id, hbra_enable);
    return;
}

static td_bool ctrl_audio_hbra_enable_get(td_u32 id)
{
    if (hdmi_reg_i2s_hbra_on_get(id)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_void ctrl_audio_i2s_ctrl_set(td_u32 id, const ctrl_i2s *i2s_ctrl)
{
    hdmi_reg_i2s_1st_shift_set(id, i2s_ctrl->sck_shift_one);
    hdmi_reg_i2s_ws_polarity_set(id, i2s_ctrl->polarity_ws_msb_left);
    hdmi_reg_i2s_justify_set(id, i2s_ctrl->justify_ws_right);
    hdmi_reg_i2s_data_dir_set(id, i2s_ctrl->data_dir_msb_first);
    hdmi_reg_i2s_vbit_set(id, i2s_ctrl->vbit_compress);
    hdmi_reg_i2s_length_set(id, i2s_ctrl->in_bit_length);
    hdmi_reg_i2s_ch_swap_set(id, i2s_ctrl->channel_lr_swap);

    return;
}

static td_void ctrl_audio_i2s_status_set(td_u32 id, const ctrl_i2s_status *i2s_status)
{
    hdmi_reg_chst_byte0_aset(id, i2s_status->professional_app);
    hdmi_reg_chst_byte0_bset(id, i2s_status->compress);
    hdmi_reg_chst_byte3_fs_set(id, i2s_status->samp_rate);
    hdmi_reg_chst_byte3_clock_accuracy_set(id, i2s_status->clk_accuracy);
    hdmi_reg_chst_byte4_org_fs_set(id, i2s_status->org_rate);
    hdmi_reg_chst_byte4_length_set(id, i2s_status->out_bit_length);

    return;
}

static td_void ctrl_audio_fifo_ctrl_set(td_u32 id, const ctrl_fifo *fifo_ctrl)
{
    hdmi_reg_aud_fifo_test_set(id, fifo_ctrl->test);
    hdmi_reg_aud_fifo_hbr_mask_set(id, fifo_ctrl->hbra_mask);
    return;
}

static td_void ctrl_audio_reset(hdmi_device_id id)
{
    hdmi_reg_tx_afifo_srst_req_set(id, TD_TRUE);
    hdmi_reg_tx_acr_srst_req_set(id, TD_TRUE);
    hdmi_reg_tx_aud_srst_req_set(id, TD_TRUE);
    hdmi_reg_tx_afifo_srst_req_set(id, TD_FALSE);
    hdmi_reg_tx_acr_srst_req_set(id, TD_FALSE);
    hdmi_reg_tx_aud_srst_req_set(id, TD_FALSE);

    return;
}

static td_void ctrl_avmute_set(td_u32 id, const hdmi_avmute_cfg *avmute_cfg)
{
    hdmi_reg_cea_cp_en_set(id, TD_FALSE);
    /* set GCP set mute & clr mute */
    hdmi_reg_cp_set_avmute_set(id, avmute_cfg->mute_set);
    hdmi_reg_cp_clr_avmute_set(id, avmute_cfg->mute_clr);
    /* set GCP avmute funtcion enable, sent loop */
    hdmi_reg_cea_cp_rpt_cnt_set(id, avmute_cfg->rpt_cnt);
    hdmi_reg_cea_cp_rpt_en_set(id, avmute_cfg->mute_pkg_en);
    hdmi_reg_cea_cp_en_set(id, avmute_cfg->mute_pkg_en);

    return;
}

static td_s32 ctrl_avmute_get(td_u32 id, td_bool *enable)
{
    *enable = TD_FALSE;

    if (hdmi_reg_cp_set_avmute_get(id) || hdmi_reg_cea_cp_rpt_en_get(id)) {
        if (hdmi_reg_cp_set_avmute_get(id)) {
            *enable = TD_TRUE;
        }
    }

    return TD_SUCCESS;
}

static td_void ctrl_video_path_deep_clr_set(td_u32 id, td_bool enable_gcp, hdmi_deep_color deep_color_bit)
{
    hdmi_reg_tmds_pack_mode_set(id, deep_color_bit);
    hdmi_reg_dc_pkt_en_set(id, enable_gcp);
    return;
}

static td_s32 ctrl_video_path_polarity_set(td_u32 id, const hdmi_sync_pol_type *out_sync_pol)
{
    td_u32 sync_pol_cfg = 0;

    if (out_sync_pol->h_pol_inver) {
        hdmi_set_bit(sync_pol_cfg, CTRL_SYCN_POL_H_BIT);
    } else {
        hdmi_clr_bit(sync_pol_cfg, CTRL_SYCN_POL_H_BIT);
    }

    if (out_sync_pol->v_pol_inver) {
        hdmi_set_bit(sync_pol_cfg, CTRL_SYCN_POL_V_BIT);
    } else {
        hdmi_clr_bit(sync_pol_cfg, CTRL_SYCN_POL_V_BIT);
    }

    if (out_sync_pol->de_inver) {
        hdmi_set_bit(sync_pol_cfg, CTRL_SYCN_POL_DE_BIT);
    } else {
        hdmi_clr_bit(sync_pol_cfg, CTRL_SYCN_POL_DE_BIT);
    }

    hdmi_reg_inver_sync_set(id, sync_pol_cfg);
    hdmi_reg_syncmask_en_set(id, TD_FALSE);

    return TD_SUCCESS;
}

static td_void ctrl_video_path_dither_set(td_u32 id, td_bool enable, hdmi_video_dither dither_mode)
{
    hdmi_reg_dither_rnd_bypass_set(id, !enable);
    hdmi_reg_dither_mode_set(id, dither_mode);
    return;
}

static td_void ctrl_video_color_rgb_set(td_u32 id, td_bool rgb_in, td_bool rgb_out)
{
    td_u32 reg_value;

    reg_value = hdmi_reg_csc_mode_get(id);

    if (rgb_in) {
        hdmi_set_bit(reg_value, CTRL_RGB_IN_BIT);
    } else {
        hdmi_clr_bit(reg_value, CTRL_RGB_IN_BIT);
    }

    if (rgb_out) {
        hdmi_set_bit(reg_value, CTRL_RGB_OUT_BIT);
    } else {
        hdmi_clr_bit(reg_value, CTRL_RGB_OUT_BIT);
    }
    hdmi_reg_csc_mode_set(id, reg_value);

    return;
}

static td_void ctrl_video_color_csc_set(td_u32 id, td_bool csc_enable)
{
    hdmi_reg_csc_en_set(id, csc_enable);
    return;
}

static td_void ctrl_video_color_ycbcr422_set(td_u32 id, td_bool enable)
{
    if (enable) {
        hdmi_reg_vmux_y_sel_set(id, CTRL_CHANNEL0_Y422);
        hdmi_reg_vmux_cb_sel_set(id, CTRL_CHANNEL1_Y422);
        hdmi_reg_vmux_cr_sel_set(id, CTRL_CHANNEL2_Y422);
    } else {
        hdmi_reg_vmux_y_sel_set(id, CTRL_CHANNEL0_Y);
        hdmi_reg_vmux_cb_sel_set(id, CTRL_CHANNEL1_CB);
        hdmi_reg_vmux_cr_sel_set(id, CTRL_CHANNEL2_CR);
    }
    return;
}

static td_void ctrl_video_color_ycbcr420_set(td_u32 id, td_bool enable)
{
    hdmi_reg_demux_420_en_set(id, enable);
    hdmi_reg_pxl_div_en_set(id, enable);
    return;
}

static td_void ctrl_video_color_dwsm_vert_set(td_u32 id, td_bool dwsm_vert)
{
    hdmi_reg_dwsm_vert_bypass_set(id, !dwsm_vert);
    hdmi_reg_dwsm_vert_en_set(id, dwsm_vert);
    return;
}

static td_void ctrl_video_color_dwsm_hori_set(td_u32 id, td_bool dwsm_hori)
{
    hdmi_reg_hori_filter_en_set(id, dwsm_hori);
    hdmi_reg_dwsm_hori_en_set(id, dwsm_hori);
    return;
}

static td_void ctrl_video_path_colorimetry_set(td_u32 id,
    hdmi_hal_colorimetry in_colormetry, hdmi_hal_colorimetry out_colormetry)
{
    td_u32 csc_mode;

    csc_mode = hdmi_reg_csc_mode_get(id);

    out_colormetry = (td_u32)out_colormetry & CTRL_COLORMETRY_MASK;
    csc_mode &= CTRL_COLORMETRY_OUT_MASK;
    csc_mode |= (td_u32)out_colormetry << CTRL_COLORMETRY_OUT_BIT;

    in_colormetry = (td_u32)in_colormetry & CTRL_COLORMETRY_MASK;
    csc_mode &= CTRL_COLORMETRY_IN_MASK;
    csc_mode |= (td_u32)in_colormetry << CTRL_COLORMETRY_IN_BIT;

    hdmi_reg_csc_mode_set(id, csc_mode);

    return;
}

static td_void videopath_colorspace_param_get(hdmi_colorspace in, hdmi_colorspace out, ctrl_colorspace_en *en)
{
    if (in == HDMI_COLORSPACE_YCBCR444) {
        switch (out) {
            case HDMI_COLORSPACE_YCBCR422:
                en->y422_enable = TD_TRUE;
                en->dwsm_hori_enable = TD_TRUE;
                break;
            case HDMI_COLORSPACE_YCBCR420:
                en->dwsm_hori_enable = TD_TRUE;
                en->y420_enable = TD_TRUE;
                en->dwsm_vert_enable = TD_TRUE;
                break;
            case HDMI_COLORSPACE_RGB:
                en->out_rgb = TD_TRUE;
                en->csc_enable = TD_TRUE;
                break;
            default:
                break;
        }
    } else if (in == HDMI_COLORSPACE_RGB) {
        switch (out) {
            case HDMI_COLORSPACE_YCBCR444:
                en->in_rgb = TD_TRUE;
                en->csc_enable = TD_TRUE;
                break;
            case HDMI_COLORSPACE_YCBCR422:
                en->in_rgb = TD_TRUE;
                en->csc_enable = TD_TRUE;
                en->y422_enable = TD_TRUE;
                en->dwsm_hori_enable = TD_TRUE;
                break;
            case HDMI_COLORSPACE_YCBCR420:
                en->in_rgb = TD_TRUE;
                en->csc_enable = TD_TRUE;
                en->dwsm_hori_enable = TD_TRUE;
                en->y420_enable = TD_TRUE;
                en->dwsm_vert_enable = TD_TRUE;
                break;
            default:
                en->in_rgb = TD_TRUE;
                en->out_rgb = TD_TRUE;
                break;
        }
    } else {
        hdmi_warn("un-expected in_color_space=%u\n", in);
    }

    return;
}

static td_bool ctrl_video_path_color_space_set(td_u32 id,
    hdmi_colorspace in_color_space, hdmi_colorspace out_color_space)
{
    ctrl_colorspace_en colorspace_en = {0};

    videopath_colorspace_param_get(in_color_space, out_color_space, &colorspace_en);

    hdmi_info("ctrl csc set:\n"
              "\tin_rgb=%u, out_rgb=%u, csc_enable=%u \n"
              "\ty422_enable=%u, dwsm_hori_enable=%u \n"
              "\ty420_enable=%u, dwsm_vert_enable=%u \n",
              colorspace_en.in_rgb, colorspace_en.out_rgb, colorspace_en.csc_enable,
              colorspace_en.y422_enable, colorspace_en.dwsm_hori_enable,
              colorspace_en.y420_enable, colorspace_en.dwsm_vert_enable);

    ctrl_video_color_rgb_set(id, colorspace_en.in_rgb, colorspace_en.out_rgb);
    ctrl_video_color_ycbcr422_set(id, colorspace_en.y422_enable);
    ctrl_video_color_dwsm_hori_set(id, colorspace_en.dwsm_hori_enable);
    ctrl_video_color_ycbcr420_set(id, colorspace_en.y420_enable);
    ctrl_video_color_dwsm_vert_set(id, colorspace_en.dwsm_vert_enable);
    ctrl_video_color_csc_set(id, colorspace_en.csc_enable);

    return colorspace_en.csc_enable;
}

static td_void ctrl_video_path_quantization_set(td_u32 id,
    hdmi_quantization_range in_quantization, hdmi_quantization_range out_quantization)
{
    td_u32 csc_mode;

    csc_mode = hdmi_reg_csc_mode_get(id);

    if (in_quantization != HDMI_QUANTIZATION_RANGE_LIMITED) {
        hdmi_set_bit(csc_mode, CTRL_QUANTIZAION_IN_BIT);
    } else {
        hdmi_clr_bit(csc_mode, CTRL_QUANTIZAION_IN_BIT);
    }

    if (out_quantization != HDMI_QUANTIZATION_RANGE_LIMITED) {
        hdmi_set_bit(csc_mode, CTRL_QUANTIZAION_OUT_BIT);
    } else {
        hdmi_clr_bit(csc_mode, CTRL_QUANTIZAION_OUT_BIT);
    }

    hdmi_reg_csc_mode_set(id, csc_mode);
    hdmi_reg_csc_saturate_en_set(id, TD_TRUE);

    return;
}

static td_s32 ctrl_vendor_infoframe_en_set(td_u32 id, td_bool enable)
{
    hdmi_reg_cea_vsif_rpt_en_set(id, enable);
    hdmi_reg_cea_vsif_en_set(id, enable);
    return TD_SUCCESS;
}

static td_s32 ctrl_vendor_infoframe_data_set(td_u32 id, const td_u8 *data)
{
    hdmi_reg_vsif_header_set(id, data[VENDOR_OFFSET_TYPE], data[VENDOR_OFFSET_VERSION], data[VENDOR_OFFSET_LENGHT]);
    hdmi_reg_vsif_pkt0_low_set(id, data[VENDOR_OFFSET_CHECSUM],
        data[VENDOR_OFFSET_IEEE_LOWER], data[VENDOR_OFFSET_IEEE_UPPER], data[VENDOR_OFFSET_IEEE]);
    hdmi_reg_vsif_pkt0_high_set(id, data[VENDOR_OFFSET_VIDEO_FMT],
        data[VENDOR_OFFSET_VIC], data[VENDOR_OFFSET_3D_STRUCT]);
    hdmi_reg_vsif_pkt1_low_set(id, data[VENDOR_OFFSET_3D_EXT_DATA],
        data[VENDOR_OFFSET_PB7], data[VENDOR_OFFSET_PB8], data[VENDOR_OFFSET_PB9]);
    hdmi_reg_vsif_pkt1_high_set(id, data[VENDOR_OFFSET_PB10], data[VENDOR_OFFSET_PB11], data[VENDOR_OFFSET_PB12]);
    hdmi_reg_vsif_pkt2_low_set(id, data[VENDOR_OFFSET_PB13],
        data[VENDOR_OFFSET_PB14], data[VENDOR_OFFSET_PB15], data[VENDOR_OFFSET_PB16]);
    hdmi_reg_vsif_pkt2_high_set(id, data[VENDOR_OFFSET_PB17], data[VENDOR_OFFSET_PB18], data[VENDOR_OFFSET_PB19]);
    hdmi_reg_vsif_pkt3_low_set(id, data[VENDOR_OFFSET_PB20],
        data[VENDOR_OFFSET_PB21], data[VENDOR_OFFSET_PB22], data[VENDOR_OFFSET_PB23]);
    hdmi_reg_vsif_pkt3_high_set(id, data[VENDOR_OFFSET_PB24], data[VENDOR_OFFSET_PB25], data[VENDOR_OFFSET_PB26]);

    return TD_SUCCESS;
}

static td_s32 ctrl_avi_infoframe_en_set(td_u32 id, td_bool enable)
{
    hdmi_reg_cea_avi_rpt_en_set(id, enable);
    hdmi_reg_cea_avi_en_set(id, enable);

    return TD_SUCCESS;
}

static td_s32 ctrl_avi_infoframe_data_set(td_u32 id, const td_u8 *data)
{
    hdmi_reg_avi_pkt_header_hb_set(id, data[AVI_OFFSET_TYPE], data[AVI_OFFSET_VERSION], data[AVI_OFFSET_LENGTH]);
    hdmi_reg_avi_pkt0_low_set(id, data[AVI_OFFSET_CHECKSUM],
        data[AVI_OFFSET_PB1], data[AVI_OFFSET_PB2], data[AVI_OFFSET_PB3]);
    hdmi_reg_avi_pkt0_high_set(id, data[AVI_OFFSET_VIC], data[AVI_OFFSET_PB5], data[AVI_OFFSET_TOP_BAR_LOWER]);
    hdmi_reg_avi_pkt1_low_set(id, data[AVI_OFFSET_TOP_BAR_UPPER],
        data[AVI_OFFSET_BOTTOM_BAR_LOWER], data[AVI_OFFSET_BOTTOM_BAR_UPPER], data[AVI_OFFSET_LEFT_BAR_LOWER]);
    hdmi_reg_avi_pkt1_high_set(id, data[AVI_OFFSET_LEFT_BAR_UPPER],
        data[AVI_OFFSET_RIGHT_BAR_LOWER], data[AVI_OFFSET_RIGHT_BAR_UPPER]);
    hdmi_reg_avi_pkt2_low_set(id, data[AVI_OFFSET_PB14],
        data[AVI_OFFSET_PB15], data[AVI_OFFSET_PB16], data[AVI_OFFSET_PB17]);
    hdmi_reg_avi_pkt2_high_set(id, data[AVI_OFFSET_PB18], data[AVI_OFFSET_PB19], data[AVI_OFFSET_PB20]);
    hdmi_reg_avi_pkt3_low_set(id, data[AVI_OFFSET_PB21],
        data[AVI_OFFSET_PB22], data[AVI_OFFSET_PB23], data[AVI_OFFSET_PB24]);
    hdmi_reg_avi_pkt3_high_set(id, data[AVI_OFFSET_PB25], data[AVI_OFFSET_PB26], data[AVI_OFFSET_PB27]);

    return TD_SUCCESS;
}

static td_s32 ctrl_tmds_mode_set(td_u32 id, hdmi_tmds_mode tmds_mode)
{
    switch (tmds_mode) {
        case HDMI_TMDS_MODE_DVI:
            hdmi_reg_hdmi_mode_set(id, TD_FALSE);
            break;
        case HDMI_TMDS_MODE_HDMI_1_4:
            hdmi_reg_hdmi_mode_set(id, TD_TRUE);
            hdmi_reg_enc_hdmi2_on_set(id, TD_FALSE);
            break;
        case HDMI_TMDS_MODE_HDMI_2_0:
            hdmi_reg_hdmi_mode_set(id, TD_TRUE);
            hdmi_reg_enc_hdmi2_on_set(id, TD_TRUE);
            hdmi_reg_enc_bypass_set(id, TD_FALSE);
            break;
        default:
            hdmi_warn("un-known tmds mode:%u\n", tmds_mode);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 ctrl_video_path_deep_clr_get(td_u32 id, td_bool *enable_gcp, hdmi_deep_color *deep_color_bit)
{
    *enable_gcp = hdmi_reg_dc_pkt_en_get(id) ? TD_TRUE : TD_FALSE;
    *deep_color_bit = hdmi_reg_tmds_pack_mode_get(id);
    return TD_SUCCESS;
}

static td_s32 ctrl_video_path_polarity_get(td_u32 id, hdmi_sync_pol_type *out_sync_pol)
{
    td_u32 sync_pol_cfg;

    sync_pol_cfg = hdmi_reg_inver_sync_get(id);
    out_sync_pol->h_pol_inver = is_bit_set(sync_pol_cfg, CTRL_SYCN_POL_H_BIT) ? TD_TRUE : TD_FALSE;
    out_sync_pol->v_pol_inver = is_bit_set(sync_pol_cfg, CTRL_SYCN_POL_V_BIT) ? TD_TRUE : TD_FALSE;
    out_sync_pol->de_inver = is_bit_set(sync_pol_cfg, CTRL_SYCN_POL_DE_BIT) ? TD_TRUE : TD_FALSE;

    return TD_SUCCESS;
}

static td_s32 ctrl_video_path_dither_get(td_u32 id, td_bool *enable, hdmi_video_dither *dither_mode)
{
    *enable = hdmi_reg_dither_rnd_bypass_get(id) ? TD_FALSE : TD_TRUE;
    *dither_mode = hdmi_reg_dither_mode_get(id);
    return TD_SUCCESS;
}

static td_void ctrl_video_color_rgb_get(td_u32 id, td_bool *rgb_in, td_bool *rgb_out)
{
    td_u32 reg_value;

    reg_value = hdmi_reg_csc_mode_get(id);
    *rgb_in = is_bit_set(reg_value, CTRL_RGB_IN_BIT);
    *rgb_out = is_bit_set(reg_value, CTRL_RGB_OUT_BIT);

    return;
}

static td_bool ctrl_video_color_csc_get(td_u32 id)
{
    if (hdmi_reg_csc_en_get(id)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_bool ctrl_video_color_ycbcr422_get(td_u32 id)
{
    if ((hdmi_reg_vmux_y_sel_get(id) == CTRL_CHANNEL0_Y422) &&
        (hdmi_reg_vmux_cb_sel_get(id) == CTRL_CHANNEL1_Y422) &&
        (hdmi_reg_vmux_cr_sel_get(id) == CTRL_CHANNEL2_Y422)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_bool ctrl_video_color_ycbcr420_get(td_u32 id)
{
    if ((hdmi_reg_demux_420_en_get(id)) && (hdmi_reg_pxl_div_en_get(id))) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_bool ctrl_video_color_dwsm_vert_get(td_u32 id)
{
    if (hdmi_reg_dwsm_vert_en_get(id) && (!hdmi_reg_dwsm_vert_bypass_get(id))) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_bool ctrl_video_color_dwsm_hori_get(td_u32 id)
{
    if (hdmi_reg_hori_filter_en_get(id) && hdmi_reg_dwsm_hori_en_get(id)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_void ctrl_video_path_color_space_get(td_u32 id,
    hdmi_colorspace *in_color_space, hdmi_colorspace *out_color_space)
{
    td_bool in_rgb = TD_FALSE;
    td_bool out_rgb = TD_FALSE;

    ctrl_video_color_rgb_get(id, &in_rgb, &out_rgb);
    *in_color_space = in_rgb ? HDMI_COLORSPACE_RGB : HDMI_COLORSPACE_YCBCR444;
    *out_color_space = out_rgb ? HDMI_COLORSPACE_RGB : HDMI_COLORSPACE_YCBCR444;

    if (ctrl_video_color_ycbcr422_get(id) && ctrl_video_color_dwsm_hori_get(id)) {
        *out_color_space = HDMI_COLORSPACE_YCBCR422;
    }

    if (ctrl_video_color_ycbcr420_get(id) && ctrl_video_color_dwsm_vert_get(id)) {
        *out_color_space = HDMI_COLORSPACE_YCBCR420;
    }

    return;
}

static td_s32 ctrl_video_path_colormetry_get(td_u32 id,
    hdmi_hal_colorimetry *in_colormetry, hdmi_hal_colorimetry *out_colormetry)
{
    td_u32 csc_mode;

    csc_mode = hdmi_reg_csc_mode_get(id);
    *out_colormetry = (csc_mode & CTRL_COLORMETRY_OUT_MASK) >> CTRL_COLORMETRY_OUT_BIT;
    *in_colormetry = (csc_mode & CTRL_COLORMETRY_OUT_MASK) >> CTRL_COLORMETRY_IN_BIT;

    return TD_SUCCESS;
}

static td_s32 ctrl_video_path_quantization_get(td_u32 id,
    hdmi_quantization_range *in_quantization, hdmi_quantization_range *out_quantization)
{
    td_u32 csc_mode;

    csc_mode = hdmi_reg_csc_mode_get(id);
    *in_quantization =
        is_bit_set(csc_mode, CTRL_QUANTIZAION_IN_BIT) ? HDMI_QUANTIZATION_RANGE_FULL : HDMI_QUANTIZATION_RANGE_LIMITED;
    *out_quantization =
        is_bit_set(csc_mode, CTRL_QUANTIZAION_OUT_BIT) ? HDMI_QUANTIZATION_RANGE_FULL : HDMI_QUANTIZATION_RANGE_LIMITED;

    return TD_SUCCESS;
}

static td_s32 ctrl_tmds_mode_get(td_u32 id, hdmi_tmds_mode *tmds_mode)
{
    td_bool hdmi_mode, hdmi2x_enable;

    hdmi_mode = hdmi_reg_hdmi_mode_get(id);
    hdmi2x_enable = hdmi_reg_enc_hdmi2_on_get(id);

    if (!hdmi_mode) {
        *tmds_mode = HDMI_TMDS_MODE_DVI;
    } else if (hdmi_mode && (!hdmi2x_enable)) {
        *tmds_mode = HDMI_TMDS_MODE_HDMI_1_4;
    } else if (hdmi_mode && hdmi2x_enable) {
        *tmds_mode = HDMI_TMDS_MODE_HDMI_2_0;
    } else {
        hdmi_warn("un-config tmds mode!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 ctrl_vendor_infoframe_en_get(td_u32 id, td_bool *enable)
{
    *enable = hdmi_reg_cea_vsif_rpt_en_get(id) ? TD_TRUE : TD_FALSE;
    return TD_SUCCESS;
}

typedef struct {
    vsif_pkt_header    vsif_pkt_head;
    vsif_sub_pkt0_low  vsif_sub_pkt0_l;
    vsif_sub_pkt0_high vsif_sub_pkt0_h;
    vsif_sub_pkt1_low  vsif_sub_pkt1_l;
    vsif_sub_pkt1_high vsif_sub_pkt1_h;
    vsif_sub_pkt2_low  vsif_sub_pkt2_l;
    vsif_sub_pkt2_high vsif_sub_pkt2_h;
    vsif_sub_pkt3_low  vsif_sub_pkt3_l;
    vsif_sub_pkt3_high vsif_sub_pkt3_h;
} ctrl_vsif_regs;

static td_void ctrl_vsif_regs_get(td_u32 id, ctrl_vsif_regs *regs)
{
    hdmi_reg_vsif_pkt_header_get(id, &regs->vsif_pkt_head);
    hdmi_reg_vsif_pkt0_low_get(id, &regs->vsif_sub_pkt0_l);
    hdmi_reg_vsif_pkt0_high_get(id, &regs->vsif_sub_pkt0_h);
    hdmi_reg_vsif_pkt1_low_get(id, &regs->vsif_sub_pkt1_l);
    hdmi_reg_vsif_pkt1_high_get(id, &regs->vsif_sub_pkt1_h);
    hdmi_reg_vsif_pkt2_low_get(id, &regs->vsif_sub_pkt2_l);
    hdmi_reg_vsif_pkt2_high_get(id, &regs->vsif_sub_pkt2_h);
    hdmi_reg_vsif_pkt3_low_get(id, &regs->vsif_sub_pkt3_l);
    hdmi_reg_vsif_pkt3_high_get(id, &regs->vsif_sub_pkt3_h);
}

static td_void ctrl_vsif_data_fill(const ctrl_vsif_regs *regs, td_u8 *data)
{
    data[VENDOR_OFFSET_TYPE] = regs->vsif_pkt_head.bits.vsif_pkt_hb0;
    data[VENDOR_OFFSET_VERSION] = regs->vsif_pkt_head.bits.vsif_pkt_hb1;
    data[VENDOR_OFFSET_LENGHT] = regs->vsif_pkt_head.bits.vsif_pkt_hb2;
    data[VENDOR_OFFSET_CHECSUM] = regs->vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb0;
    data[VENDOR_OFFSET_IEEE_LOWER] = regs->vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb1;
    data[VENDOR_OFFSET_IEEE_UPPER] = regs->vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb2;
    data[VENDOR_OFFSET_IEEE] = regs->vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb3;
    data[VENDOR_OFFSET_VIDEO_FMT] = regs->vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb4;
    data[VENDOR_OFFSET_VIC] = regs->vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb5;
    data[VENDOR_OFFSET_3D_STRUCT] = regs->vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb6;
    data[VENDOR_OFFSET_3D_EXT_DATA] = regs->vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb0;
    data[VENDOR_OFFSET_PB7] = regs->vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb1;
    data[VENDOR_OFFSET_PB8] = regs->vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb2;
    data[VENDOR_OFFSET_PB9] = regs->vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb3;
    data[VENDOR_OFFSET_PB10] = regs->vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb4;
    data[VENDOR_OFFSET_PB11] = regs->vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb5;
    data[VENDOR_OFFSET_PB12] = regs->vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb6;
    data[VENDOR_OFFSET_PB13] = regs->vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb0;
    data[VENDOR_OFFSET_PB14] = regs->vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb1;
    data[VENDOR_OFFSET_PB15] = regs->vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb2;
    data[VENDOR_OFFSET_PB16] = regs->vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb3;
    data[VENDOR_OFFSET_PB17] = regs->vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb4;
    data[VENDOR_OFFSET_PB18] = regs->vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb5;
    data[VENDOR_OFFSET_PB19] = regs->vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb6;
    data[VENDOR_OFFSET_PB20] = regs->vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb0;
    data[VENDOR_OFFSET_PB21] = regs->vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb1;
    data[VENDOR_OFFSET_PB22] = regs->vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb2;
    data[VENDOR_OFFSET_PB23] = regs->vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb3;
    data[VENDOR_OFFSET_PB24] = regs->vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb4;
    data[VENDOR_OFFSET_PB25] = regs->vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb5;
    data[VENDOR_OFFSET_PB26] = regs->vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb6;
}

static td_s32 ctrl_vendor_infoframe_data_get(td_u32 id, td_u8 *data)
{
    ctrl_vsif_regs regs;

    ctrl_vsif_regs_get(id, &regs);
    ctrl_vsif_data_fill(&regs, data);
    return TD_SUCCESS;
}

static td_s32 ctrl_avi_infoframe_en_get(td_u32 id, td_bool *enable)
{
    *enable = hdmi_reg_cea_avi_en_get(id) ? TD_TRUE : TD_FALSE;
    return TD_SUCCESS;
}

static td_void avi_infoframe_head_get(td_u32 id, td_u8 *data)
{
    avi_pkt_header avi_pkt_head = {0};

    hdmi_reg_avi_pkt_header_hb_get(id, &avi_pkt_head);
    data[AVI_OFFSET_TYPE] = avi_pkt_head.bits.avi_pkt_hb0;
    data[AVI_OFFSET_VERSION] = avi_pkt_head.bits.avi_pkt_hb1;
    data[AVI_OFFSET_LENGTH] = avi_pkt_head.bits.avi_pkt_hb2;

    return;
}

static td_void avi_infoframe_sub_pkt_get(td_u32 id, td_u8 *data)
{
    avi_sub_pkt0_low  avi_sub_pkt0_l;
    avi_sub_pkt0_high avi_sub_pkt0_h;
    avi_sub_pkt1_low  avi_sub_pkt1_l;
    avi_sub_pkt1_high avi_sub_pkt1_h;
    avi_sub_pkt2_low  avi_sub_pkt2_l;
    avi_sub_pkt2_high avi_sub_pkt2_h;
    avi_sub_pkt3_low  avi_sub_pkt3_l;
    avi_sub_pkt3_high avi_sub_pkt3_h;

    hdmi_reg_avi_pkt0_low_get(id, &avi_sub_pkt0_l);
    hdmi_reg_avi_pkt0_high_get(id, &avi_sub_pkt0_h);
    hdmi_reg_avi_pkt1_low_get(id, &avi_sub_pkt1_l);
    hdmi_reg_avi_pkt1_high_get(id, &avi_sub_pkt1_h);
    hdmi_reg_avi_pkt2_low_get(id, &avi_sub_pkt2_l);
    hdmi_reg_avi_pkt2_high_get(id, &avi_sub_pkt2_h);
    hdmi_reg_avi_pkt3_low_get(id, &avi_sub_pkt3_l);
    hdmi_reg_avi_pkt3_high_get(id, &avi_sub_pkt3_h);

    data[AVI_OFFSET_CHECKSUM] = avi_sub_pkt0_l.bits.avi_sub_pkt0_pb0;
    data[AVI_OFFSET_PB1] = avi_sub_pkt0_l.bits.avi_sub_pkt0_pb1;
    data[AVI_OFFSET_PB2] = avi_sub_pkt0_l.bits.avi_sub_pkt0_pb2;
    data[AVI_OFFSET_PB3] = avi_sub_pkt0_l.bits.avi_sub_pkt0_pb3;
    data[AVI_OFFSET_VIC] = avi_sub_pkt0_h.bits.avi_sub_pkt0_pb4;
    data[AVI_OFFSET_PB5] = avi_sub_pkt0_h.bits.avi_sub_pkt0_pb5;
    data[AVI_OFFSET_TOP_BAR_LOWER] = avi_sub_pkt0_h.bits.avi_sub_pkt0_pb6;
    data[AVI_OFFSET_TOP_BAR_UPPER] = avi_sub_pkt1_l.bits.avi_sub_pkt1_pb0;
    data[AVI_OFFSET_BOTTOM_BAR_LOWER] = avi_sub_pkt1_l.bits.avi_sub_pkt1_pb1;
    data[AVI_OFFSET_BOTTOM_BAR_UPPER] = avi_sub_pkt1_l.bits.avi_sub_pkt1_pb2;
    data[AVI_OFFSET_LEFT_BAR_LOWER] = avi_sub_pkt1_l.bits.avi_sub_pkt1_pb3;
    data[AVI_OFFSET_LEFT_BAR_UPPER] = avi_sub_pkt1_h.bits.avi_sub_pkt1_pb4;
    data[AVI_OFFSET_RIGHT_BAR_LOWER] = avi_sub_pkt1_h.bits.avi_sub_pkt1_pb5;
    data[AVI_OFFSET_RIGHT_BAR_UPPER] = avi_sub_pkt1_h.bits.avi_sub_pkt1_pb6;
    data[AVI_OFFSET_PB14] = avi_sub_pkt2_l.bits.avi_sub_pkt2_pb0;
    data[AVI_OFFSET_PB15] = avi_sub_pkt2_l.bits.avi_sub_pkt2_pb1;
    data[AVI_OFFSET_PB16] = avi_sub_pkt2_l.bits.avi_sub_pkt2_pb2;
    data[AVI_OFFSET_PB17] = avi_sub_pkt2_l.bits.avi_sub_pkt2_pb3;
    data[AVI_OFFSET_PB18] = avi_sub_pkt2_h.bits.avi_sub_pkt2_pb4;
    data[AVI_OFFSET_PB19] = avi_sub_pkt2_h.bits.avi_sub_pkt2_pb5;
    data[AVI_OFFSET_PB20] = avi_sub_pkt2_h.bits.avi_sub_pkt2_pb6;
    data[AVI_OFFSET_PB21] = avi_sub_pkt3_l.bits.avi_sub_pkt3_pb0;
    data[AVI_OFFSET_PB22] = avi_sub_pkt3_l.bits.avi_sub_pkt3_pb1;
    data[AVI_OFFSET_PB23] = avi_sub_pkt3_l.bits.avi_sub_pkt3_pb2;
    data[AVI_OFFSET_PB24] = avi_sub_pkt3_l.bits.avi_sub_pkt3_pb3;
    data[AVI_OFFSET_PB25] = avi_sub_pkt3_h.bits.avi_sub_pkt3_pb4;
    data[AVI_OFFSET_PB26] = avi_sub_pkt3_h.bits.avi_sub_pkt3_pb5;
    data[AVI_OFFSET_PB27] = avi_sub_pkt3_h.bits.avi_sub_pkt3_pb6;

    return;
}

static td_s32 ctrl_avi_infoframe_data_get(td_u32 id, td_u8 *data)
{
    avi_infoframe_head_get(id, data);
    avi_infoframe_sub_pkt_get(id, data);

    return TD_SUCCESS;
}

static td_s32 ctrl_timming_decect_get(td_u32 id, ctrl_timming_detect *timing)
{
    timing->sync_sw_enable = TD_FALSE;
    hdmi_reg_sync_polarity_force_set(id, timing->sync_sw_enable);

    timing->vsync_polarity = hdmi_reg_vsync_polarity_get(id) ? TD_TRUE : TD_FALSE;
    timing->hsync_polarity = hdmi_reg_hsync_polarity_get(id) ? TD_TRUE : TD_FALSE;
    timing->progressive    = hdmi_reg_interlaced_get(id) ? TD_FALSE : TD_TRUE;
    timing->hsync_total    = hdmi_reg_hsync_total_cnt_get(id);
    timing->hactive_cnt    = hdmi_reg_hsync_active_cnt_get(id);
    timing->vsync_total    = hdmi_reg_vsync_total_cnt_get(id);
    timing->vactive_cnt    = hdmi_reg_vsync_active_cnt_get(id);

    /* clear format detect status */
    hdmi_reg_fdt_status_clear_set(id, TD_FALSE);
    hdmi_reg_fdt_status_clear_set(id, TD_TRUE);

    return TD_SUCCESS;
}

static td_void ctrl_video_mute_set(td_u32 id, td_bool enable)
{
    td_u32 data_value;
    hdmi_colorspace in_color_space = HDMI_COLORSPACE_RGB;
    hdmi_colorspace out_color_space = HDMI_COLORSPACE_RGB;

    ctrl_video_path_color_space_get(id, &in_color_space, &out_color_space);

    data_value = (in_color_space == HDMI_COLORSPACE_RGB) ? CTRL_BLACK_DATA_RGB_R : CTRL_BLACK_DATA_YUV_CR;
    hdmi_reg_solid_pattern_cr_set(id, data_value);

    data_value = (in_color_space == HDMI_COLORSPACE_RGB) ? CTRL_BLACK_DATA_RGB_G : CTRL_BLACK_DATA_YUV_Y;
    hdmi_reg_solid_pattern_y_set(id, data_value);

    data_value = (in_color_space == HDMI_COLORSPACE_RGB) ? CTRL_BLACK_DATA_RGB_B : CTRL_BLACK_DATA_YUV_CB;
    hdmi_reg_solid_pattern_cb_set(id, data_value);

    hdmi_reg_video_blank_en_set(id, enable);
    hdmi_reg_solid_pattern_en_set(id, enable);

    return;
}

static td_s32 ctrl_video_mute_get(td_u32 id, td_bool *enable)
{
    if (hdmi_reg_video_blank_en_get(id) && hdmi_reg_solid_pattern_en_get(id)) {
        *enable = TD_TRUE;
    } else {
        *enable = TD_FALSE;
    }

    return TD_SUCCESS;
}

static td_s32 ctrl_audio_infoframe_en_set(td_u32 id, td_bool enable)
{
    /* repeat enable */
    hdmi_reg_cea_aud_rpt_en_set(id, enable);
    /* info_frame enable */
    hdmi_reg_cea_aud_en_set(id, enable);
    return TD_SUCCESS;
}

static td_s32 ctrl_audio_infoframe_en_get(td_u32 id, td_bool *enable)
{
    *enable = hdmi_reg_cea_aud_en_get(id) ? TD_TRUE : TD_FALSE;
    return TD_SUCCESS;
}

static td_s32 ctrl_audio_infoframe_data_set(td_u32 id, const td_u8 *data)
{
    hdmi_reg_audio_pkt_header_set(id, data[AUDIO_OFFSET_TYPE], data[AUDIO_OFFSET_VERSION], data[AUDIO_OFFSET_LENGHT]);
    hdmi_reg_audio_pkt0_low_set(id, data[AUDIO_OFFSET_CHECKSUM],
        data[AUDIO_OFFSET_PB1], data[AUDIO_OFFSET_PB2], data[AUDIO_OFFSET_FORMAT]);
    hdmi_reg_audio_pkt0_high_set(id, data[AUDIO_OFFSET_CA], data[AUDIO_OFFSET_PB5], data[AUDIO_OFFSET_PB6]);
    hdmi_reg_audio_pkt1_low_set(id, data[AUDIO_OFFSET_PB7],
        data[AUDIO_OFFSET_PB8], data[AUDIO_OFFSET_PB9], data[AUDIO_OFFSET_PB10]);
    hdmi_reg_audio_pkt1_high_set(id, data[AUDIO_OFFSET_PB11], data[AUDIO_OFFSET_PB12], data[AUDIO_OFFSET_PB13]);
    hdmi_reg_audio_pkt2_low_set(id, data[AUDIO_OFFSET_PB14],
        data[AUDIO_OFFSET_PB15], data[AUDIO_OFFSET_PB16], data[AUDIO_OFFSET_PB17]);
    hdmi_reg_audio_pkt2_high_set(id, data[AUDIO_OFFSET_PB18], data[AUDIO_OFFSET_PB19], data[AUDIO_OFFSET_PB20]);
    hdmi_reg_audio_pkt3_low_set(id, data[AUDIO_OFFSET_PB21],
        data[AUDIO_OFFSET_PB22], data[AUDIO_OFFSET_PB23], data[AUDIO_OFFSET_PB24]);
    hdmi_reg_audio_pkt3_high_set(id, data[AUDIO_OFFSET_PB25], data[AUDIO_OFFSET_PB26], data[AUDIO_OFFSET_PB27]);

    return TD_SUCCESS;
}

typedef struct {
    aif_pkt_header    aif_pkt_head;
    aif_sub_pkt0_low  aif_sub_pkt0_l;
    aif_sub_pkt0_high aif_sub_pkt0_h;
    aif_sub_pkt1_low  aif_sub_pkt1_l;
    aif_sub_pkt1_high aif_sub_pkt1_h;
    aif_sub_pkt2_low  aif_sub_pkt2_l;
    aif_sub_pkt2_high aif_sub_pkt2_h;
    aif_sub_pkt3_low  aif_sub_pkt3_l;
    aif_sub_pkt3_high aif_sub_pkt3_h;
} ctrl_aif_regs;

static td_void ctrl_aif_regs_get(td_u32 id, ctrl_aif_regs *regs)
{
    hdmi_reg_aif_pkt_header_get(id, &regs->aif_pkt_head);
    hdmi_reg_aif_pkt0_low_get(id, &regs->aif_sub_pkt0_l);
    hdmi_reg_aif_pkt0_high_get(id, &regs->aif_sub_pkt0_h);
    hdmi_reg_aif_pkt1_low_get(id, &regs->aif_sub_pkt1_l);
    hdmi_reg_aif_pkt1_high_get(id, &regs->aif_sub_pkt1_h);
    hdmi_reg_aif_pkt2_low_get(id, &regs->aif_sub_pkt2_l);
    hdmi_reg_aif_pkt2_high_get(id, &regs->aif_sub_pkt2_h);
    hdmi_reg_aif_pkt3_low_get(id, &regs->aif_sub_pkt3_l);
    hdmi_reg_aif_pkt3_high_get(id, &regs->aif_sub_pkt3_h);
}

static td_void ctrl_aif_data_fill(const ctrl_aif_regs *regs, td_u8 *data)
{
    data[AUDIO_OFFSET_TYPE] = regs->aif_pkt_head.bits.aif_pkt_hb0;
    data[AUDIO_OFFSET_VERSION] = regs->aif_pkt_head.bits.aif_pkt_hb1;
    data[AUDIO_OFFSET_LENGHT] = regs->aif_pkt_head.bits.aif_pkt_hb2;
    data[AUDIO_OFFSET_CHECKSUM] = regs->aif_sub_pkt0_l.bits.aif_sub_pkt0_pb0;
    data[AUDIO_OFFSET_PB1] = regs->aif_sub_pkt0_l.bits.aif_sub_pkt0_pb1;
    data[AUDIO_OFFSET_PB2] = regs->aif_sub_pkt0_l.bits.aif_sub_pkt0_pb2;
    data[AUDIO_OFFSET_FORMAT] = regs->aif_sub_pkt0_l.bits.aif_sub_pkt0_pb3;
    data[AUDIO_OFFSET_CA] = regs->aif_sub_pkt0_h.bits.aif_sub_pkt0_pb4;
    data[AUDIO_OFFSET_PB5] = regs->aif_sub_pkt0_h.bits.aif_sub_pkt0_pb5;
    data[AUDIO_OFFSET_PB6] = regs->aif_sub_pkt0_h.bits.aif_sub_pkt0_pb6;
    data[AUDIO_OFFSET_PB7] = regs->aif_sub_pkt1_l.bits.aif_sub_pkt1_pb0;
    data[AUDIO_OFFSET_PB8] = regs->aif_sub_pkt1_l.bits.aif_sub_pkt1_pb1;
    data[AUDIO_OFFSET_PB9] = regs->aif_sub_pkt1_l.bits.aif_sub_pkt1_pb2;
    data[AUDIO_OFFSET_PB10] = regs->aif_sub_pkt1_l.bits.aif_sub_pkt1_pb3;
    data[AUDIO_OFFSET_PB11] = regs->aif_sub_pkt1_h.bits.aif_sub_pkt1_pb4;
    data[AUDIO_OFFSET_PB12] = regs->aif_sub_pkt1_h.bits.aif_sub_pkt1_pb5;
    data[AUDIO_OFFSET_PB13] = regs->aif_sub_pkt1_h.bits.aif_sub_pkt1_pb6;
    data[AUDIO_OFFSET_PB14] = regs->aif_sub_pkt2_l.bits.aif_sub_pkt2_pb0;
    data[AUDIO_OFFSET_PB15] = regs->aif_sub_pkt2_l.bits.aif_sub_pkt2_pb1;
    data[AUDIO_OFFSET_PB16] = regs->aif_sub_pkt2_l.bits.aif_sub_pkt2_pb2;
    data[AUDIO_OFFSET_PB17] = regs->aif_sub_pkt2_l.bits.aif_sub_pkt2_pb3;
    data[AUDIO_OFFSET_PB18] = regs->aif_sub_pkt2_h.bits.aif_sub_pkt2_pb4;
    data[AUDIO_OFFSET_PB19] = regs->aif_sub_pkt2_h.bits.aif_sub_pkt2_pb5;
    data[AUDIO_OFFSET_PB20] = regs->aif_sub_pkt2_h.bits.aif_sub_pkt2_pb6;
    data[AUDIO_OFFSET_PB21] = regs->aif_sub_pkt3_l.bits.aif_sub_pkt3_pb0;
    data[AUDIO_OFFSET_PB22] = regs->aif_sub_pkt3_l.bits.aif_sub_pkt3_pb1;
    data[AUDIO_OFFSET_PB23] = regs->aif_sub_pkt3_l.bits.aif_sub_pkt3_pb2;
    data[AUDIO_OFFSET_PB24] = regs->aif_sub_pkt3_l.bits.aif_sub_pkt3_pb3;
    data[AUDIO_OFFSET_PB25] = regs->aif_sub_pkt3_h.bits.aif_sub_pkt3_pb4;
    data[AUDIO_OFFSET_PB26] = regs->aif_sub_pkt3_h.bits.aif_sub_pkt3_pb5;
    data[AUDIO_OFFSET_PB27] = regs->aif_sub_pkt3_h.bits.aif_sub_pkt3_pb6;
}

static td_s32 ctrl_audio_infoframe_data_get(td_u32 id, td_u8 *data)
{
    ctrl_aif_regs regs;

    ctrl_aif_regs_get(id, &regs);
    ctrl_aif_data_fill(&regs, data);
    return TD_SUCCESS;
}

static td_s32 ctrl_gbd_infoframe_en_set(td_u32 id, td_bool enable)
{
    hdmi_reg_cea_gamut_rpt_en_set(id, enable);
    hdmi_reg_cea_gamut_en_set(id, enable);
    return TD_SUCCESS;
}

static td_s32 ctrl_gbd_infoframe_en_get(td_u32 id, td_bool *enable)
{
    *enable = hdmi_reg_cea_gamut_en_get(id) ? TD_TRUE : TD_FALSE;
    return TD_SUCCESS;
}

typedef struct {
    gamut_pkt_header    gamut_pkt_head;
    gamut_sub_pkt0_low  gamut_sub_pkt0_l;
    gamut_sub_pkt0_high gamut_sub_pkt0_h;
    gamut_sub_pkt1_low  gamut_sub_pkt1_l;
    gamut_sub_pkt1_high gamut_sub_pkt1_h;
    gamut_sub_pkt2_low  gamut_sub_pkt2_l;
    gamut_sub_pkt2_high gamut_sub_pkt2_h;
    gamut_sub_pkt3_low  gamut_sub_pkt3_l;
    gamut_sub_pkt3_high gamut_sub_pkt3_h;
} ctrl_gamut_regs;

static td_void ctrl_gamut_regs_get(td_u32 id, ctrl_gamut_regs *regs)
{
    hdmi_reg_gamut_pkt_header_get(id, &regs->gamut_pkt_head);
    hdmi_reg_gamut_pkt0_low_get(id, &regs->gamut_sub_pkt0_l);
    hdmi_reg_gamut_pkt0_high_get(id, &regs->gamut_sub_pkt0_h);
    hdmi_reg_gamut_pkt1_low_get(id, &regs->gamut_sub_pkt1_l);
    hdmi_reg_gamut_pkt1_high_get(id, &regs->gamut_sub_pkt1_h);
    hdmi_reg_gamut_pkt2_low_get(id, &regs->gamut_sub_pkt2_l);
    hdmi_reg_gamut_pkt2_high_get(id, &regs->gamut_sub_pkt2_h);
    hdmi_reg_gamut_pkt3_low_get(id, &regs->gamut_sub_pkt3_l);
    hdmi_reg_gamut_pkt3_high_get(id, &regs->gamut_sub_pkt3_h);
}

static td_void ctrl_gamut_data_fill(const ctrl_gamut_regs *regs, td_u8 *data)
{
    data[GDB_OFFSET_HB0] = regs->gamut_pkt_head.bits.gamut_pkt_hb0;
    data[GDB_OFFSET_HB1] = regs->gamut_pkt_head.bits.gamut_pkt_hb1;
    data[GDB_OFFSET_HB2] = regs->gamut_pkt_head.bits.gamut_pkt_hb2;
    data[GDB_OFFSET_CHECKSUM] = regs->gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb0;
    data[GDB_OFFSET_PB1] = regs->gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb1;
    data[GDB_OFFSET_PB2] = regs->gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb2;
    data[GDB_OFFSET_PB3] = regs->gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb3;
    data[GDB_OFFSET_PB4] = regs->gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb4;
    data[GDB_OFFSET_PB5] = regs->gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb5;
    data[GDB_OFFSET_PB6] = regs->gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb6;
    data[GDB_OFFSET_PB7] = regs->gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb0;
    data[GDB_OFFSET_PB8] = regs->gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb1;
    data[GDB_OFFSET_PB9] = regs->gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb2;
    data[GDB_OFFSET_PB10] = regs->gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb3;
    data[GDB_OFFSET_PB11] = regs->gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb4;
    data[GDB_OFFSET_PB12] = regs->gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb5;
    data[GDB_OFFSET_PB13] = regs->gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb6;
    data[GDB_OFFSET_PB14] = regs->gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb0;
    data[GDB_OFFSET_PB15] = regs->gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb1;
    data[GDB_OFFSET_PB16] = regs->gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb2;
    data[GDB_OFFSET_PB17] = regs->gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb3;
    data[GDB_OFFSET_PB18] = regs->gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb4;
    data[GDB_OFFSET_PB19] = regs->gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb5;
    data[GDB_OFFSET_PB20] = regs->gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb6;
    data[GDB_OFFSET_PB21] = regs->gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb0;
    data[GDB_OFFSET_PB22] = regs->gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb1;
    data[GDB_OFFSET_PB23] = regs->gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb2;
    data[GDB_OFFSET_PB24] = regs->gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb3;
    data[GDB_OFFSET_PB25] = regs->gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb4;
    data[GDB_OFFSET_PB26] = regs->gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb5;
    data[GDB_OFFSET_PB27] = regs->gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb6;
}

static td_s32 ctrl_gbd_infoframe_data_get(td_u32 id, td_u8 *data)
{
    ctrl_gamut_regs regs;

    ctrl_gamut_regs_get(id, &regs);
    ctrl_gamut_data_fill(&regs, data);
    return TD_SUCCESS;
}

static td_bool ctrl_hpd_get(td_u32 id)
{
    td_s32 hot_plug;
    td_bool hpd;

    hot_plug = hdmi_reg_hotplug_state_get(id);

    if (hdmi_reg_hpd_polarity_ctl_get(id) > 0) {
        hpd = (hot_plug > 0) ? TD_FALSE : TD_TRUE;
    } else {
        hpd = (hot_plug > 0) ? TD_TRUE : TD_FALSE;
    }

    return hpd;
}

static td_bool ctrl_rsen_get(td_u32 id)
{
    if (hdmi_reg_phy_rx_sense_get(id)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_void ctrl_hpd_intr_enable(td_u32 id, td_bool enable)
{
    hdmi_reg_aon_intr_stat0_set(id, TD_TRUE);
    hdmi_reg_aon_intr_mask0_set(id, enable);
    return;
}

static td_void ctrl_mach_ppd_callback(td_void *data)
{
    td_bool event;
    hdmi_ctrl_info *ctrl_info = (hdmi_ctrl_info *)data;

    hdmi_if_null_return_void(data);
    hdmi_if_null_return_void(ctrl_info->event_data);
    hdmi_if_null_return_void(ctrl_info->event_callback);

    if (hdmi_reg_aon_intr_stat0_get(ctrl_info->hdmi_id)) {
        /* clr intr */
        hdmi_reg_aon_intr_stat0_set(ctrl_info->hdmi_id, TD_TRUE);
        /* event */
        event = ctrl_hpd_get(ctrl_info->hdmi_id);
        if (event != ctrl_info->hpd) {
            ctrl_info->event_callback(ctrl_info->event_data,
                (event == TD_TRUE) ? HDMI_EVENT_HOTPLUG : HDMI_EVENT_HOTUNPLUG);
            ctrl_info->hpd = event;
        } else {
            hdmi_warn("HPD event not change: %u!\n", event);
        }
    }

    if (hdmi_reg_aon_intr_stat1_get(ctrl_info->hdmi_id)) {
        /* clr intr */
        hdmi_reg_aon_intr_stat1_set(ctrl_info->hdmi_id, TD_TRUE);
        /* event */
        event = ctrl_rsen_get(ctrl_info->hdmi_id);
        if (event != ctrl_info->rsen) {
            ctrl_info->event_callback(ctrl_info->event_data,
                (event == TD_TRUE) ? HDMI_EVENT_RSEN_CONNECT : HDMI_EVENT_RSEN_DISCONNECT);
            ctrl_info->rsen = event;
        } else {
            hdmi_warn("RSEN event not change: %u!\n", event);
        }
    }

    return;
}

static td_s32 ctrl_null_packet_set(td_u32 id, td_bool enable)
{
    hdmi_reg_null_pkt_en_set(id, enable);
    return TD_SUCCESS;
}

static td_void ctrl_video_dither_set(td_u32 id, hdmi_video_path *videopath)
{
    td_bool enable_gcp = TD_FALSE;
    td_bool enable_dither = TD_FALSE;
    hdmi_video_dither dither_mode = HDMI_VIDEO_DITHER_DISALBE;

    if (videopath->out_deep_color != HDMI_DEEP_COLOR_30BIT && videopath->out_deep_color != HDMI_DEEP_COLOR_36BIT) {
        videopath->out_deep_color = HDMI_DEEP_COLOR_24BIT;
    }

    /* dither */
    switch (videopath->out_deep_color) {
        case HDMI_DEEP_COLOR_24BIT:
            switch (videopath->in_deep_color) {
                case HDMI_DEEP_COLOR_30BIT:
                    enable_dither = TD_TRUE;
                    dither_mode = HDMI_VIDEO_DITHER_10_8;
                    break;
                case HDMI_DEEP_COLOR_36BIT:
                    enable_dither = TD_TRUE;
                    dither_mode = HDMI_VIDEO_DITHER_12_8;
                    break;
                default:
                    break;
            }
            break;
        case HDMI_DEEP_COLOR_30BIT:
            enable_gcp = TD_TRUE;
            if (videopath->in_deep_color == HDMI_DEEP_COLOR_36BIT) {
                enable_dither = TD_TRUE;
                dither_mode = HDMI_VIDEO_DITHER_12_10;
            }
            break;
        case HDMI_DEEP_COLOR_36BIT:
            enable_gcp = TD_TRUE;
            break;
        default:
            break;
    }

    if (videopath->timing == HDMI_VIDEO_TIMING_1440X480I_60000 ||
        videopath->timing == HDMI_VIDEO_TIMING_1440X576I_50000 ||
        videopath->out_color_space == HDMI_COLORSPACE_YCBCR422) {
        enable_dither = TD_FALSE;
        dither_mode = HDMI_VIDEO_DITHER_DISALBE;
    }

    ctrl_video_path_dither_set(id, enable_dither, dither_mode);
    ctrl_video_path_deep_clr_set(id, enable_gcp, videopath->out_deep_color);

    return;
}

td_s32 hal_hdmi_ctrl_init(hdmi_device_id hdmi, const hdmi_hal_init *hal_init)
{
    td_s32 ret = TD_SUCCESS;
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);
    hdmi_mach_ctrl mach_ctrl = {0};

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(hal_init, OT_ERR_HDMI_NULL_PTR);

    if (ctrl_info->init == TD_FALSE) {
        ret += hdmi_reg_crg_init(hdmi);
        hdmi_if_null_return(hal_init->base_addr, OT_ERR_HDMI_NULL_PTR);
        ret += hdmi_reg_aon_regs_init(hdmi, hal_init->base_addr);
        ret += hdmi_reg_audio_path_regs_init(hdmi, hal_init->base_addr);
        ret += hdmi_reg_tx_ctrl_regs_init(hdmi, hal_init->base_addr);
        ret += hdmi_reg_tx_hdmi_regs_init(hdmi, hal_init->base_addr);
        ret += hdmi_reg_video_path_regs_init(hdmi, hal_init->base_addr);

        drv_hdmi_prod_crg_gate_set(hdmi, TD_TRUE);
        ctrl_info->event_callback = hal_init->event_callback;
        ctrl_info->event_data = hal_init->event_data;
        ctrl_info->init       = TD_TRUE;
        ctrl_info->hdmi_id    = hdmi;
        mach_ctrl.data        = ctrl_info;
        mach_ctrl.callback    = ctrl_mach_ppd_callback;
        mach_ctrl.interval    = HDMI_MACH_DEFAULT_INTERVAL;
        mach_ctrl.name        = "HPD";
        ret += hal_hdmi_mach_register(hdmi, &ctrl_info->mach_id, &mach_ctrl);
        ret += hal_hdmi_mach_start(hdmi, ctrl_info->mach_id);

        ctrl_hpd_intr_enable(hdmi, TD_TRUE);
    }

    return (ret != TD_SUCCESS) ? TD_FAILURE : TD_SUCCESS;
}

td_s32 hal_hdmi_ctrl_deinit(hdmi_device_id hdmi)
{
    td_s32 ret;
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    ctrl_hpd_intr_enable(hdmi, TD_FALSE);
    ret = hal_hdmi_mach_stop(hdmi, ctrl_info->mach_id);
    ret += hal_hdmi_mach_unregister(hdmi, ctrl_info->mach_id);
    ret += hdmi_reg_audio_path_regs_deinit(hdmi);
    ret += hdmi_reg_aon_regs_deinit(hdmi);
    ret += hdmi_reg_tx_ctrl_regs_deinit(hdmi);
    ret += hdmi_reg_tx_hdmi_regs_deinit(hdmi);
    ret += hdmi_reg_video_path_regs_deinit(hdmi);

    (td_void)memset_s(ctrl_info, sizeof(hdmi_ctrl_info), 0, sizeof(hdmi_ctrl_info));

    return (ret != TD_SUCCESS) ? TD_FAILURE : TD_SUCCESS;
}

td_s32 hal_hdmi_ctrl_avmute_set(hdmi_device_id hdmi, const hdmi_avmute_cfg *avmute_cfg)
{
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(avmute_cfg, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    ctrl_avmute_set(hdmi, avmute_cfg);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_ctrl_avmute_get(hdmi_device_id hdmi, td_bool *enable)
{
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);
    hdmi_if_null_return(enable, OT_ERR_HDMI_NULL_PTR);

    return ctrl_avmute_get(hdmi, enable);
}

td_s32 hal_hdmi_ctrl_csc_set(hdmi_device_id hdmi, const hdmi_video_config *video_cfg)
{
    hdmi_if_null_return(video_cfg, OT_ERR_HDMI_NULL_PTR);

    ctrl_video_path_colorimetry_set(hdmi, video_cfg->conv_std, video_cfg->conv_std);
    ctrl_video_path_quantization_set(hdmi, video_cfg->out_csc_quantization, video_cfg->out_csc_quantization);
    ctrl_video_path_color_space_set(hdmi, video_cfg->in_color_space, video_cfg->out_color_space);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_ctrl_video_path_set(hdmi_device_id hdmi, hdmi_video_path *video_path)
{
    td_s32 ret;
    errno_t errno;
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(video_path, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    errno = memcpy_s(&ctrl_info->video_path, sizeof(ctrl_info->video_path), video_path, sizeof(hdmi_video_path));
    hdmi_unequal_eok_return(errno, OT_ERR_HDMI_INVALID_PARA);

    /* dither */
    ctrl_video_dither_set(hdmi, video_path);
    ret = ctrl_video_path_polarity_set(hdmi, &video_path->out_hv_sync_pol);
    ctrl_video_path_colorimetry_set(hdmi, video_path->in_colormetry, video_path->out_colormetry);
    ctrl_video_path_quantization_set(hdmi, video_path->in_quantization, video_path->out_quantization);
    ctrl_video_path_color_space_set(hdmi, video_path->in_color_space, video_path->out_color_space);

    return ret;
}

td_s32 hal_hdmi_ctrl_tmds_mode_set(hdmi_device_id hdmi, hdmi_tmds_mode tmds_mode)
{
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    return ctrl_tmds_mode_set(hdmi, tmds_mode);
}

td_s32 hal_hdmi_ctrl_infoframe_en_set(hdmi_device_id hdmi, hdmi_infoframe_id infoframe_id, td_bool enable)
{
    td_s32 ret;
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    switch (infoframe_id) {
        case HDMI_INFOFRAME_TYPE_NULL:
            ret = ctrl_null_packet_set(hdmi, enable);
            break;
        case HDMI_INFOFRAME_TYPE_VENDOR:
            ret = ctrl_vendor_infoframe_en_set(hdmi, enable);
            break;
        case HDMI_INFOFRAME_TYPE_AVI:
            ret = ctrl_avi_infoframe_en_set(hdmi, enable);
            break;
        case HDMI_INFOFRAME_TYPE_AUDIO:
            ret = ctrl_audio_infoframe_en_set(hdmi, enable);
            break;
        case HDMI_INFOFRAME_TYPE_GBD:
            ret = ctrl_gbd_infoframe_en_set(hdmi, enable);
            break;
        default:
            hdmi_warn("un-support infoframe type:%u!\n", infoframe_id);
            ret = TD_FAILURE;
            break;
    }

    return ret;
}

td_s32 hal_hdmi_ctrl_infoframe_data_set(hdmi_device_id hdmi, hdmi_infoframe_id infoframe_id,
    const td_u8 *data, td_u32 data_len)
{
    td_s32 ret;
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(data, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    if (data_len < HDMI_INFOFRAME_DATA_SIZE) {
        hdmi_warn("infoframe data size err, is %u!\n", data_len);
    }

    switch (infoframe_id) {
        case HDMI_INFOFRAME_TYPE_VENDOR:
            ret = ctrl_vendor_infoframe_data_set(hdmi, data);
            break;
        case HDMI_INFOFRAME_TYPE_AVI:
            ret = ctrl_avi_infoframe_data_set(hdmi, data);
            break;
        case HDMI_INFOFRAME_TYPE_AUDIO:
            ret = ctrl_audio_infoframe_data_set(hdmi, data);
            break;
        default:
            hdmi_warn("un-support infoframe type:%u!\n", infoframe_id);
            ret = TD_FAILURE;
            break;
    }

    return ret;
}

td_s32 hal_hdmi_ctrl_data_reset(hdmi_device_id hdmi, td_bool debug_mode, td_u32 delay_ms)
{
    static td_u32 delay;
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    hdmi_reg_tx_acr_srst_req_set(hdmi, TD_TRUE);
    hdmi_reg_tx_afifo_srst_req_set(hdmi, TD_TRUE);
    hdmi_reg_tx_aud_srst_req_set(hdmi, TD_TRUE);
    hdmi_reg_tx_hdmi_srst_req_set(hdmi, TD_TRUE);
    osal_udelay(CTRL_REAET_WAIT_TIME);
    hdmi_reg_tx_acr_srst_req_set(hdmi, TD_FALSE);
    hdmi_reg_tx_afifo_srst_req_set(hdmi, TD_FALSE);
    hdmi_reg_tx_aud_srst_req_set(hdmi, TD_FALSE);
    /* TOP rst: will also pack_fifo_ctrl reg_fifo_manu_rst */
    hdmi_reg_tx_hdmi_srst_req_set(hdmi, TD_FALSE);

    if (debug_mode) {
        delay = delay_ms;
    }

    if (delay) {
        osal_msleep(delay);
        hdmi_info("data_rst %u ms\n", delay);
    }

    return TD_SUCCESS;
}

td_s32 hal_hdmi_ctrl_video_mute_set(hdmi_device_id hdmi, td_bool enable)
{
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    ctrl_video_mute_set(hdmi, enable);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_ctrl_video_mute_get(hdmi_device_id hdmi, td_bool *enable)
{
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(enable, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    return ctrl_video_mute_get(hdmi, enable);
}

td_s32 hal_hdmi_ctrl_audio_path_enable_set(hdmi_device_id hdmi, td_bool enable)
{
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    ctrl_audio_path_enable_set(hdmi, enable);

    return TD_SUCCESS;
}

static td_void hal_hdmi_ctrl_audio_i2s_set(hdmi_device_id hdmi, const hdmi_audio_path *audio_path)
{
    td_bool muti_layout = TD_FALSE;
    ctrl_i2s_status i2s_status = {0};
    ctrl_i2s i2s_ctrl = {0};
    ctrl_fifo fifo_ctrl = {0};

    if (audio_path->channel_num == HDMI_AUDIO_FORMAT_8CH) {
        muti_layout = TD_TRUE;
    } else if (audio_path->channel_num >= HDMI_AUDIO_FORMAT_8CH) {
        hdmi_warn("error audio channel number:%u\n", audio_path->channel_num);
    }
    ctrl_audio_path_layout_set(hdmi, muti_layout);

    i2s_ctrl.in_bit_length = ctrl_audio_bit_reg_get(audio_path->sample_bit);
    ctrl_audio_i2s_ctrl_set(hdmi, &i2s_ctrl);

    i2s_status.clk_accuracy     = HDMI_AUDIO_CLK_ACCURACY_LEVEL2;
    i2s_status.samp_rate        = ctrl_i2s_rate_reg_get(audio_path->sample_rate);
    i2s_status.org_rate         = ctrl_i2s_rate_org_reg_get(audio_path->sample_rate);
    i2s_status.out_bit_length   = ctrl_audio_bit_reg_get(audio_path->sample_bit);
    ctrl_audio_i2s_status_set(hdmi, &i2s_status);

    fifo_ctrl.hbra_mask = HDMI_AUDIO_HBRA_MASK_NONE;
    ctrl_audio_fifo_ctrl_set(hdmi, &fifo_ctrl);
}

static td_void hal_hdmi_ctrl_audio_cts_n_set(hdmi_device_id hdmi, const hdmi_audio_path *audio_path)
{
    td_u32 n_value;
    td_u32 cts_value;

    ctrl_audio_path_cts_value_set(hdmi, TD_FALSE);
    n_value = hal_hdmi_n_value_get(audio_path->sample_rate, audio_path->pixel_clk);
    ctrl_audio_path_n_value_set(hdmi, n_value);

    cts_value = hal_hdmi_cts_value_get(audio_path->sample_rate, audio_path->pixel_clk);
    hdmi_info("\n fs=%u,bit=%u,intf=%u,tmds=%u\n"
              "reference N=%u,real N=%u,reference cts=%u,real_cts=%u\n",
              audio_path->sample_rate, audio_path->sample_bit, audio_path->sound_intf, audio_path->pixel_clk,
              n_value, ctrl_audio_path_n_value_get(hdmi), cts_value, ctrl_audio_path_cts_value_get(hdmi));
}

td_s32 hal_hdmi_ctrl_audio_path_set(hdmi_device_id hdmi, const hdmi_audio_path *audio_path)
{
    errno_t ret;
    td_bool         spdif_enable = TD_FALSE;
    td_bool         hbra_enable  = TD_FALSE;
    hdmi_ctrl_info *ctrl_info    = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(audio_path, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    ret = memcpy_s(&ctrl_info->audio_path, sizeof(ctrl_info->audio_path), audio_path, sizeof(hdmi_audio_path));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    ctrl_audio_path_enable_set(hdmi, TD_FALSE);
    ctrl_audio_reset(hdmi);

    hal_hdmi_ctrl_audio_i2s_set(hdmi, audio_path);
    hal_hdmi_ctrl_audio_cts_n_set(hdmi, audio_path);

    /* enable */
    ctrl_audio_i2s_enable_set(hdmi, TD_TRUE);
    ctrl_audio_spdif_enable_set(hdmi, spdif_enable);
    ctrl_audio_hbra_enable_set(hdmi, hbra_enable); /* when spdif, it must false */

    return TD_SUCCESS;
}

td_s32 hal_hdmi_ctrl_audio_path_get(hdmi_device_id hdmi, ctrl_audio_status *audio_stat)
{
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);
    hdmi_audio_path *audio_path = TD_NULL;

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);
    hdmi_if_null_return(audio_stat, OT_ERR_HDMI_NULL_PTR);

    audio_path = &ctrl_info->audio_path;
    hdmi_if_null_return(audio_path, OT_ERR_HDMI_NULL_PTR);

    if (ctrl_audio_spdif_enable_get(hdmi)) {
        audio_stat->sound_intf = HDMI_AUDIO_INTF_SPDIF;
    } else if (ctrl_audio_hbra_enable_get(hdmi)) {
        audio_stat->sound_intf = HDMI_AUDIO_INTF_HBRA;
    } else if (ctrl_audio_i2s_enable_get(hdmi)) {
        audio_stat->sound_intf = HDMI_AUDIO_INTF_I2S;
    } else {
        audio_stat->sound_intf = HDMI_AUDIO_INTF_BUTT;
    }

    audio_stat->enable_audio = ctrl_audio_path_enable_get(hdmi);
    audio_stat->audio_mute   = ctrl_audio_mute_get(hdmi);
    audio_stat->sample_bit   = ctrl_audio_bit_value_get(hdmi, TD_FALSE);
    audio_stat->sample_rate  = ctrl_i2s_rate_value_get(hdmi);
    audio_stat->channel_num  = ctrl_audio_path_layout_get(hdmi) ? HDMI_AUDIO_FORMAT_8CH : HDMI_AUDIO_FORMAT_2CH;
    audio_stat->ref_cts      = hal_hdmi_cts_value_get(audio_path->sample_rate, audio_path->pixel_clk);
    audio_stat->reg_cts      = ctrl_audio_path_cts_value_get(hdmi);
    audio_stat->ref_n        = hal_hdmi_n_value_get(audio_path->sample_rate, audio_path->pixel_clk);
    audio_stat->reg_n        = ctrl_audio_path_n_value_get(hdmi);

    return TD_SUCCESS;
}

td_void hal_hdmi_ctrl_video_path_get(hdmi_device_id hdmi, ctrl_video_status *video_stat)
{
    td_s32          ret;
    td_bool         enable_gcp    = TD_FALSE;
    td_bool         enable_dither = TD_FALSE;
    hdmi_ctrl_info *ctrl_info     = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return_void(video_stat);
    hdmi_if_null_return_void(ctrl_info);
    hdmi_if_false_return_void(ctrl_info->init);

    ret = ctrl_timming_decect_get(hdmi, &video_stat->timing);
    ret += ctrl_video_path_polarity_get(hdmi, &video_stat->out_hv_sync_pol);
    ctrl_video_path_color_space_get(hdmi, &video_stat->in_color_space, &video_stat->out_color_space);
    ret += ctrl_video_path_dither_get(hdmi, &enable_dither, &video_stat->dither_mode);
    ret += ctrl_video_path_colormetry_get(hdmi, &video_stat->in_colormetry, &video_stat->out_colormetry);
    ret += ctrl_video_path_quantization_get(hdmi, &video_stat->in_quantization, &video_stat->out_quantization);
    ret += ctrl_video_path_deep_clr_get(hdmi, &enable_gcp, &video_stat->out_deep_color);
    video_stat->csc_enable = ctrl_video_color_csc_get(hdmi);

    if (!enable_gcp) {
        video_stat->out_deep_color = HDMI_DEEP_COLOR_OFF;
    }

    switch (video_stat->dither_mode) {
        case HDMI_VIDEO_DITHER_12_10:
        case HDMI_VIDEO_DITHER_12_8:
            video_stat->in_deep_color = HDMI_DEEP_COLOR_36BIT;
            break;
        case HDMI_VIDEO_DITHER_10_8:
            video_stat->in_deep_color = HDMI_DEEP_COLOR_30BIT;
            break;
        default:
            video_stat->in_deep_color = video_stat->out_deep_color;
            break;
    }

    video_stat->y422_enable = ctrl_video_color_dwsm_hori_get(hdmi);
    video_stat->y420_enable = ctrl_video_color_ycbcr420_get(hdmi);

    return;
}

td_s32 hal_hdmi_ctrl_tmds_mode_get(hdmi_device_id hdmi, hdmi_tmds_mode *tmds_mode)
{
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(tmds_mode, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    return ctrl_tmds_mode_get(hdmi, tmds_mode);
}

td_s32 hal_hdmi_ctrl_infoframe_en_get(hdmi_device_id hdmi, hdmi_infoframe_id infoframe_id, td_bool *enable)
{
    td_s32 ret;
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(enable, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    switch (infoframe_id) {
        case HDMI_INFOFRAME_TYPE_VENDOR:
            ret = ctrl_vendor_infoframe_en_get(hdmi, enable);
            break;
        case HDMI_INFOFRAME_TYPE_AVI:
            ret = ctrl_avi_infoframe_en_get(hdmi, enable);
            break;
        case HDMI_INFOFRAME_TYPE_AUDIO:
            ret = ctrl_audio_infoframe_en_get(hdmi, enable);
            break;
        case HDMI_INFOFRAME_TYPE_GBD:
            ret = ctrl_gbd_infoframe_en_get(hdmi, enable);
            break;
        default:
            hdmi_warn("un-support infoframe type:%u!\n", infoframe_id);
            ret = TD_FAILURE;
            break;
    }

    return ret;
}

td_s32 hal_hdmi_ctrl_infoframe_data_get(hdmi_device_id hdmi, hdmi_infoframe_id infoframe_id, td_u8 *data)
{
    td_s32 ret;
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(data, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    switch (infoframe_id) {
        case HDMI_INFOFRAME_TYPE_VENDOR:
            ret = ctrl_vendor_infoframe_data_get(hdmi, data);
            break;
        case HDMI_INFOFRAME_TYPE_AVI:
            ret = ctrl_avi_infoframe_data_get(hdmi, data);
            break;
        case HDMI_INFOFRAME_TYPE_AUDIO:
            ret = ctrl_audio_infoframe_data_get(hdmi, data);
            break;
        case HDMI_INFOFRAME_TYPE_GBD:
            ret = ctrl_gbd_infoframe_data_get(hdmi, data);
            break;
        default:
            hdmi_warn("un-support infoframe type:%u!\n", infoframe_id);
            ret = TD_FAILURE;
            break;
    }

    return ret;
}

td_s32 hal_hdmi_ctrl_hpd_rsen_get(hdmi_device_id hdmi, hdmi_hpd_rsen *hpd_rsen)
{
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(hpd_rsen, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    hpd_rsen->rsen_on = ctrl_rsen_get(hdmi);
    hpd_rsen->hpd_on = ctrl_hpd_get(hdmi);

    return TD_SUCCESS;
}

td_void hal_hdmi_ctrl_dither_dbg_set(hdmi_device_id hdmi, hdmi_video_dither dither_mode)
{
    td_bool enable_dither;

    if (dither_mode >= HDMI_VIDEO_DITHER_DISALBE) {
        enable_dither = TD_FALSE;
        dither_mode = HDMI_VIDEO_DITHER_DISALBE;
    } else {
        enable_dither = TD_TRUE;
    }
    ctrl_video_path_dither_set(hdmi, enable_dither, dither_mode);

    return;
}

td_s32 hal_hdmi_ctrl_tmds_stable_get(hdmi_device_id hdmi, td_bool *stable)
{
    hdmi_ctrl_info *ctrl_info = ctrl_info_ptr_get(hdmi);

    hdmi_if_null_return(stable, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(ctrl_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(ctrl_info->init, TD_FAILURE);

    *stable = hdmi_reg_pclk2tclk_stable_get(hdmi) ? TD_TRUE : TD_FALSE;
    return TD_SUCCESS;
}

