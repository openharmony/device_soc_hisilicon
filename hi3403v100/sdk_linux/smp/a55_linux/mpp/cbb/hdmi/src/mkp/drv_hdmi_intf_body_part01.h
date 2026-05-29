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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART01_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART01_H_


#line 1 "drv_hdmi_intf.c"
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

#include "drv_hdmi_intf.h"
#include <linux/kthread.h>
#include "hdmi_hal.h"
#include "hdmi_product_define.h"
#include "drv_hdmi_intf_k.h"
#include "drv_hdmi_edid.h"
#include "drv_hdmi_event.h"
#include "drv_hdmi_ioctl.h"
#include "drv_hdmi_debug.h"
#include "drv_hdmi_compatibility.h"
#include "dev_ext.h"
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
#include "drv_hdmi_proc.h"
#endif

#define hdmi_multiple_2p0(x)         ((x) *= 2)
#define hdmi_multiple_1p5(x)         ((x) = ((x) * 3) >> 1)
#define hdmi_multiple_1p25(x)        ((x) = ((x) * 5) >> 2)
#define hdmi_multiple_0p5(x)         ((x) = (x) >> 1)
#define MAX_DELAY_TIME_MS            10000
#define SCDC_ENABLE_TMDS_CHR_RATE    (600 * 1000000)
#define SCDC_DESABLE_TMDS_CHR_RATE   (300 * 1000000)
#define SCDC_SCRAMBLE_INTERVAL_RESET 20
#define SCDC_SCRAMBLE_TIMEOUT_RESET  200
#define DEV_HPD_ASSERT_WAIT_TIME     50
#define HDMI_THREAD_STATE_WAIT_TIME  90
#define HDMI_THREAD_DELAY            10
#define HDMI_DEV_FMT_DELAY           500
#define HDMI_DEV_MUTE_DELAY          120
#define HDMI_VIDEO_ATTR_CLK_FS       74250
#define ATTR_LOCK_WAIT_TIME          2
#define ATTR_LOCK_WAIT_TIMEOUT       5
#define HW_PARAM_ARRAY_COUNT         4
#define HDMI_READ_HPD_STATUS_DELAY   6
#define HDMI_READ_HPD_STATUS_DELAY_TIME 20
#define PROC_NAME_MAX                50
#define DEVFS_NAME_LEN               20
#define HDMI_VIC_4K24                4
#define EDID_UPDATA_CNT_MAX          5
#define THREAD_NAME_MAX              20
#define HDMI_14_MAX_TMDS_CLK (300 * 1000)
#define HDMI_TMDS_CLK_MIN            25000

static td_void hdmi_exit(td_void);
static td_s32 hdmi_init(td_void *args);
static td_u32 hdmi_get_ver_magic(td_void);
static td_void hdmi_notify(mod_notice_id notice);
static td_s32 hdmi_file_open(void *private_data);
static td_void hdmi_query_state(mod_state *state);
static td_s32 hdmi_file_close(void *private_data);
static long hdmi_file_ioctl(unsigned int cmd, unsigned long arg, void *private_data);

static td_char *g_hdmi_reg[HDMI_ID_MAX] = {TD_NULL};
static td_char *g_hdmi_phy[HDMI_ID_MAX] = {TD_NULL};
static osal_semaphore_t g_hdmi_mutex;
static osal_dev_t *g_hdmi_device = TD_NULL;
static osal_atomic_t g_hdmi_count = OSAL_ATOMIC_INIT(0);
static hdmi_device g_hdmi_ctrl[HDMI_ID_MAX];

static hdmi_export_func g_hdmi_export_funcs = {
    .pfn_stop = drv_hdmi_stop_export,
    .pfn_csc_param_set = drv_hdmi_csc_param_set_export,
    .pfn_video_param_set = drv_hdmi_video_param_set_export,
};

static umap_module g_module = {
    .export_funcs = &g_hdmi_export_funcs,
    .mod_id = OT_ID_HDMI,
    .mod_name = "hdmi",
    .pfn_init = hdmi_init,
    .pfn_exit = hdmi_exit,
    .pfn_query_state = hdmi_query_state,
    .pfn_notify = hdmi_notify,
    .pfn_ver_checker = hdmi_get_ver_magic,
    .data = TD_NULL,
};

static struct osal_fileops g_hdmi_file_ops = {
    .open = hdmi_file_open,
    .unlocked_ioctl = hdmi_file_ioctl,
    .release = hdmi_file_close,
#ifdef CONFIG_COMPAT
    .compat_ioctl = hdmi_file_ioctl,
#endif
};

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
static hdmi_proc_item g_hdmi_proc_item = {
    .read = drv_hdmi_proc_show,
#ifdef HDMI_DEBUG_SUPPORT
    .write = drv_hdmi_debug_source_write,
#else
    .write = TD_NULL,
#endif
};

static hdmi_proc_item g_vo_proc_item = {
    .read = drv_hdmi_vo_proc_show,
    .write = TD_NULL,
};

static hdmi_proc_item g_ao_proc_item = {
    .read = drv_hdmi_ao_proc_show,
    .write = TD_NULL,
};

static hdmi_proc_item g_sink_proc_item = {
    .read = drv_hdmi_sink_proc_show,
    .write = TD_NULL,
};
#endif

static td_void drv_hdmi_black_data_set(const hdmi_device *hdmi_dev, td_bool enable)
{
    hdmi_black_frame_info black_info = {0};

#if defined(HDMI_PRODUCT_SS626V100)
    /* SS626V100 8K30 yuv420 solution does not support black frames. */
    if (hdmi_dev->attr.vo_attr.video_timing == HDMI_VIDEO_TIMING_7680X4320P_30000) {
        return;
    }
#endif
    black_info.black_enable = enable;
    black_info.in_bit_depth = hdmi_dev->attr.vo_attr.in_bit_depth;
    black_info.in_color_space = hdmi_dev->attr.vo_attr.in_color_space;
    black_info.in_quantization = (black_info.in_color_space == HDMI_COLORSPACE_RGB) ?
        hdmi_dev->attr.vo_attr.rgb_quantization : hdmi_dev->attr.vo_attr.ycc_quantization;
    hal_call_void(hdmi_dev, hal_hdmi_black_data_set, &black_info);

    return;
}

static td_s32 hdmi_capability_inter_section(hdmi_sink_capability *dest_cap,
                                            const hdmi_tx_capability_data *tx_cap, td_bool auth_mode)
{
    dest_cap->support_hdmi = dest_cap->support_hdmi && tx_cap->tx_hdmi_14;
    dest_cap->support_hdmi_20 = dest_cap->support_hdmi_20 && tx_cap->tx_hdmi_20;
    dest_cap->support_scdc = dest_cap->support_scdc && tx_cap->tx_scdc;
    dest_cap->color_space.rgb444 = dest_cap->color_space.rgb444 && tx_cap->tx_rgb444;
    dest_cap->color_space.ycbcr444 = dest_cap->color_space.ycbcr444 && tx_cap->tx_ycbcr444;
    dest_cap->color_space.ycbcr422 = dest_cap->color_space.ycbcr422 && tx_cap->tx_ycbcr422;
    dest_cap->color_space.ycbcr420 = (dest_cap->color_space.ycbcr420 && tx_cap->tx_ycbcr420) || auth_mode;
    dest_cap->deep_color.deep_color30_bit = dest_cap->deep_color.deep_color30_bit && tx_cap->tx_deep_clr10_bit;
    dest_cap->deep_color.deep_color36_bit = dest_cap->deep_color.deep_color36_bit && tx_cap->tx_deep_clr12_bit;
    dest_cap->deep_color.deep_color48_bit = dest_cap->deep_color.deep_color48_bit && tx_cap->tx_deep_clr16_bit;
    dest_cap->deep_color_y420.deep_color30_bit = (dest_cap->deep_color_y420.deep_color30_bit &&
        (tx_cap->tx_ycbcr420 && tx_cap->tx_deep_clr10_bit)) || auth_mode;
    dest_cap->deep_color_y420.deep_color36_bit = (dest_cap->deep_color_y420.deep_color36_bit &&
        (tx_cap->tx_ycbcr420 && tx_cap->tx_deep_clr12_bit)) || auth_mode;
    dest_cap->deep_color_y420.deep_color48_bit = (dest_cap->deep_color_y420.deep_color48_bit &&
        (tx_cap->tx_ycbcr420 && tx_cap->tx_deep_clr16_bit)) || auth_mode;
    dest_cap->max_tmds_clock = (dest_cap->max_tmds_clock < tx_cap->tx_max_tmds_clk) ? dest_cap->max_tmds_clock :
        tx_cap->tx_max_tmds_clk;
    dest_cap->hdcp_support.hdcp14_support = dest_cap->hdcp_support.hdcp14_support && tx_cap->tx_hdcp_14;
    dest_cap->hdcp_support.hdcp22_support = dest_cap->hdcp_support.hdcp22_support && tx_cap->tx_hdcp_22;

    return TD_SUCCESS;
}

static td_void hdmi_phy_output_enable(const hdmi_device *hdmi_dev, td_bool phy_output_enable)
{
    hal_call_void(hdmi_dev, hal_hdmi_phy_output_enable_set, phy_output_enable);
    return;
}

#ifdef HDMI_SCDC_SUPPORT
static td_s32 hdmi_scdc_status_get(const hdmi_app_attr *app_attr, const hdmi_vo_attr *vo_attr,
                                   hdmi_scdc_status *scdc_status, hdmi_tmds_mode *tmds_mode)
{
    if (app_attr->enable_hdmi == TD_FALSE) {
        /* DVI mode */
        *tmds_mode = HDMI_TMDS_MODE_DVI;
        scdc_status->sink_scramble_on   = TD_FALSE;
        scdc_status->source_scramble_on = TD_FALSE;
        scdc_status->tmds_bit_clk_ratio = SCDC_TMDS_BIT_CLK_RATIO_10X;
        if ((vo_attr->hdmi_adapt_pix_clk > HDMI_EDID_MAX_HDMI14_TMDS_RATE) && (!app_attr->enable_clr_space_adapt)) {
            hdmi_err("tmds_clk=%u can't support in DVI mode. \n", vo_attr->hdmi_adapt_pix_clk);
            return TD_FAILURE;
        }
    } else if (vo_attr->hdmi_adapt_pix_clk > HDMI_EDID_MAX_HDMI14_TMDS_RATE) {
        *tmds_mode = HDMI_TMDS_MODE_HDMI_2_0;
        scdc_status->sink_scramble_on   = TD_TRUE;
        scdc_status->source_scramble_on = TD_TRUE;
        scdc_status->tmds_bit_clk_ratio = SCDC_TMDS_BIT_CLK_RATIO_40X;
    } else {
        *tmds_mode = HDMI_TMDS_MODE_HDMI_1_4;
        scdc_status->sink_scramble_on   = TD_FALSE;
        scdc_status->source_scramble_on = TD_FALSE;
        scdc_status->tmds_bit_clk_ratio = SCDC_TMDS_BIT_CLK_RATIO_10X;
    }

    return TD_SUCCESS;
}
#endif

static td_bool hdmi_ycbcr420_fmt_check(hdmi_device *hdmi_dev)
{
    td_u32 i;
    hdmi_video_code_vic vic;
    td_bool search_out = TD_FALSE;
    hdmi_vo_attr *vo_attr = TD_NULL;
    hdmi_sink_capability *sink_cap = TD_NULL;

    /* auth mode donot do cb_cr420_fmt_check and return TD_TRUE */
    if (hdmi_dev->attr.app_attr.auth_mode == TD_TRUE) {
        hdmi_info("auth_mode: %u \n", hdmi_dev->attr.app_attr.auth_mode);
        return TD_TRUE;
    }
    vo_attr = &hdmi_dev->attr.vo_attr;
    if (drv_hdmi_edid_capability_get(&hdmi_dev->edid_info, &sink_cap) == HDMI_EDID_DATA_INVALID) {
        hdmi_warn("get sink capability fail\n");
    }
    vic = drv_hdmi_vic_search(vo_attr->video_timing, vo_attr->picture_aspect, TD_FALSE);
    if (vic != 0) {
        for (i = 0; i < sink_cap->support_y420_vic_num && i < hdmi_array_size(sink_cap->support_y420_format); i++) {
            if (vic == sink_cap->support_y420_format[i]) {
                search_out = TD_TRUE;
                break;
            }
        }
        for (i = 0; (search_out == TD_FALSE) &&
             i < sink_cap->only_support_y420_vic_num &&
             i < hdmi_array_size(sink_cap->only_support_y420_format); i++) {
            if (vic == sink_cap->only_support_y420_format[i]) {
                search_out = TD_TRUE;
                break;
            }
        }
    }
    hdmi_info("is Y420 support vic=%u :%s\n", vic, search_out ? "YES" : "NO");

    return search_out;
}

static td_s32 hdmi_color_space_check(hdmi_device *hdmi_dev, const hdmi_app_attr *user_app)
{
    hdmi_tx_capability_data tx_cap = {0};
    td_bool support_clr_space = TD_FALSE;
    hdmi_sink_capability *sink_cap = TD_NULL;

    hal_call_void(hdmi_dev, hal_hdmi_tx_capability_get, &tx_cap);
    if (drv_hdmi_edid_capability_get(&hdmi_dev->edid_info, &sink_cap) == HDMI_EDID_DATA_INVALID) {
        hdmi_warn("get sink capability fail\n");
    }

    switch (user_app->out_color_space) {
        case HDMI_COLORSPACE_RGB:
            support_clr_space = TD_TRUE;
            if ((sink_cap->color_space.rgb444 && tx_cap.tx_rgb444) == TD_FALSE) {
                hdmi_warn("sink or source not support RGB!\n");
            }
            break;
        case HDMI_COLORSPACE_YCBCR422:
            support_clr_space = sink_cap->color_space.ycbcr422 && tx_cap.tx_ycbcr422;
            break;
        case HDMI_COLORSPACE_YCBCR444:
            support_clr_space = sink_cap->color_space.ycbcr444 && tx_cap.tx_ycbcr444;
            break;
        case HDMI_COLORSPACE_YCBCR420:
            support_clr_space = tx_cap.tx_ycbcr420 && hdmi_ycbcr420_fmt_check(hdmi_dev);
            break;
        default:
            hdmi_err("un-know color_space=%u!\n", user_app->out_color_space);
            return TD_FAILURE;
            break;
    }

    if (support_clr_space == TD_FALSE) {
        hdmi_err("fail, not support color space:%u\n", user_app->out_color_space);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void check_deep_color_10bit(const hdmi_sink_capability *sink_cap, const hdmi_app_attr *user_app,
    const hdmi_tx_capability_data *tx_cap, td_u32 *fmt_pix_clk, td_bool *support_deep_clr)
{
    td_u32 pix_clk;

    hdmi_if_null_return_void(sink_cap);

    pix_clk = *fmt_pix_clk;

    switch (user_app->out_color_space) {
        case HDMI_COLORSPACE_RGB:
            hdmi_multiple_1p25(pix_clk);
            *support_deep_clr = (sink_cap->deep_color.deep_color30_bit &&
                tx_cap->tx_deep_clr10_bit) ? TD_TRUE : TD_FALSE;
            break;
        case HDMI_COLORSPACE_YCBCR444:
            hdmi_multiple_1p25(pix_clk);
            *support_deep_clr = (sink_cap->deep_color.deep_color30_bit && tx_cap->tx_deep_clr10_bit &&
                sink_cap->deep_color.deep_color_y444) ? TD_TRUE : TD_FALSE;
            break;
        case HDMI_COLORSPACE_YCBCR420:
            hdmi_multiple_1p25(pix_clk);
            *support_deep_clr = (sink_cap->deep_color_y420.deep_color30_bit &&
                tx_cap->tx_deep_clr10_bit) ? TD_TRUE : TD_FALSE;
            break;
        default:
            /* Y422, ignore deepclr */
            *support_deep_clr = TD_TRUE;
            break;
    }
    *fmt_pix_clk = pix_clk;

    return;
}

static td_void check_deep_color_12bit(const hdmi_sink_capability *sink_cap, const hdmi_app_attr *user_app,
    const hdmi_tx_capability_data *tx_cap, td_u32 *fmt_pix_clk, td_bool *support_deep_clr)
{
    td_u32 pix_clk;

    hdmi_if_null_return_void(sink_cap);

    pix_clk = *fmt_pix_clk;

    switch (user_app->out_color_space) {
        case HDMI_COLORSPACE_RGB:
            hdmi_multiple_1p5(pix_clk);
            *support_deep_clr = (sink_cap->deep_color.deep_color36_bit &&
                tx_cap->tx_deep_clr12_bit) ? TD_TRUE : TD_FALSE;
            break;
        case HDMI_COLORSPACE_YCBCR444:
            hdmi_multiple_1p5(pix_clk);
            *support_deep_clr = (sink_cap->deep_color.deep_color36_bit && tx_cap->tx_deep_clr12_bit &&
                sink_cap->deep_color.deep_color_y444) ? TD_TRUE : TD_FALSE;
            break;
        case HDMI_COLORSPACE_YCBCR420:
            hdmi_multiple_1p5(pix_clk);
            *support_deep_clr = (sink_cap->deep_color_y420.deep_color36_bit &&
                tx_cap->tx_deep_clr12_bit) ? TD_TRUE : TD_FALSE;
            break;
        default:
            /* Y422, ignore deepclr */
            *support_deep_clr = TD_TRUE;
            break;
    }
    *fmt_pix_clk = pix_clk;

    return;
}

static td_u32 hdmi_current_id_get(const hdmi_device *hdmi_dev)
{
    ot_unused(hdmi_dev);
    return hdmi_get_current_id();
}

static td_void check_deep_color(const hdmi_sink_capability *sink_cap, const hdmi_app_attr *user_app,
    const hdmi_tx_capability_data *tx_cap, td_u32 *fmt_pix_clk, td_bool *support_deep_clr)
{
    switch (user_app->deep_color_mode) {
        case HDMI_DEEP_COLOR_30BIT:
            check_deep_color_10bit(sink_cap, user_app, tx_cap, fmt_pix_clk, support_deep_clr);
            break;
        case HDMI_DEEP_COLOR_36BIT:
            check_deep_color_12bit(sink_cap, user_app, tx_cap, fmt_pix_clk, support_deep_clr);
            break;
        default:
            *support_deep_clr = TD_TRUE;
            break;
    }

    return;
}

static td_s32 hdmi_deep_color_check(hdmi_device *hdmi_dev, hdmi_app_attr *user_app, td_u32 max_tmds_clk)
{
    td_u32 fmt_pix_clk;
    hdmi_vo_attr *vo_attr = TD_NULL;
    hdmi_sink_capability *sink_cap = TD_NULL;
    hdmi_tx_capability_data tx_cap = {0};
    td_bool adapt_to_y422;
    td_bool support_deep_clr = TD_FALSE;

    vo_attr = &hdmi_dev->attr.vo_attr;
    hal_call_void(hdmi_dev, hal_hdmi_tx_capability_get, &tx_cap);
    if (drv_hdmi_edid_capability_get(&hdmi_dev->edid_info, &sink_cap) == HDMI_EDID_DATA_INVALID) {
        hdmi_warn("get sink capability fail\n");
    }

    fmt_pix_clk = vo_attr->clk_fs;
    if (user_app->out_color_space == HDMI_COLORSPACE_YCBCR420) {
        hdmi_multiple_0p5(fmt_pix_clk);
    }
    check_deep_color(sink_cap, user_app, &tx_cap, &fmt_pix_clk, &support_deep_clr);

    if ((support_deep_clr == TD_TRUE) && (fmt_pix_clk < max_tmds_clk)) {
        vo_attr->hdmi_adapt_pix_clk = fmt_pix_clk;
        return TD_SUCCESS;
    } else {
        vo_attr->hdmi_adapt_pix_clk = vo_attr->clk_fs;
        if (user_app->out_color_space == HDMI_COLORSPACE_YCBCR420) {
            hdmi_multiple_0p5(vo_attr->hdmi_adapt_pix_clk);
        }

        adapt_to_y422 = ((user_app->out_color_space == HDMI_COLORSPACE_YCBCR444) &&
                         (user_app->enable_clr_space_adapt) &&
                         (user_app->deep_color_mode != HDMI_DEEP_COLOR_24BIT) &&
                         (user_app->deep_color_mode != HDMI_DEEP_COLOR_OFF) &&
                         (user_app->deep_color_mode != HDMI_DEEP_COLOR_48BIT) &&
                         (sink_cap->color_space.ycbcr422 && tx_cap.tx_ycbcr422));

        /* Y444 12/10bit */
        if (adapt_to_y422 == TD_TRUE) {
            hdmi_warn("fail, Y444 adapt to Y422!\n");
            user_app->out_color_space = HDMI_COLORSPACE_YCBCR422;


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART01_H_ */
