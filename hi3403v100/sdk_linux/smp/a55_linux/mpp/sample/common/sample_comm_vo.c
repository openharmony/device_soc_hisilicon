/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "sample_comm.h"
#if VO_LT8618SX
#include "lt8618sx.h"
#endif

#if VO_MIPI_SUPPORT
#include "ot_mipi_tx.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

static td_bool g_vo_hdmi_rgb_mode = TD_FALSE;

static sample_vo_sync_info g_vo_sample_sync_info[OT_VO_OUT_BUTT] = {
    { OT_VO_OUT_PAL,        720, 576, 25 },
    { OT_VO_OUT_NTSC,       720, 480, 30 },
    { OT_VO_OUT_960H_PAL,   960, 576, 25 },
    { OT_VO_OUT_960H_NTSC,  960, 480, 30 },

    { OT_VO_OUT_640x480_60,     640,  480,  60 },
    { OT_VO_OUT_480P60,         720,  480,  60 },
    { OT_VO_OUT_576P50,         720,  576,  50 },
    { OT_VO_OUT_800x600_60,     800,  600,  60 },
    { OT_VO_OUT_1024x768_60,    1024, 768,  60 },
    { OT_VO_OUT_720P50,         1280, 720,  50 },
    { OT_VO_OUT_720P60,         1280, 720,  60 },
    { OT_VO_OUT_1280x800_60,    1280, 800,  60 },
    { OT_VO_OUT_1280x1024_60,   1280, 1024, 60 },
    { OT_VO_OUT_1366x768_60,    1366, 768,  60 },
    { OT_VO_OUT_1400x1050_60,   1400, 1050, 60 },
    { OT_VO_OUT_1440x900_60,    1440, 900,  60 },
    { OT_VO_OUT_1680x1050_60,   1680, 1050, 60 },

    { OT_VO_OUT_1080P24,    1920, 1080, 24 },
    { OT_VO_OUT_1080P25,    1920, 1080, 25 },
    { OT_VO_OUT_1080P30,    1920, 1080, 30 },
    { OT_VO_OUT_1080I50,    1920, 1080, 25 },
    { OT_VO_OUT_1080I60,    1920, 1080, 30 },
    { OT_VO_OUT_1080P50,    1920, 1080, 50 },
    { OT_VO_OUT_1080P60,    1920, 1080, 60 },

    { OT_VO_OUT_1600x1200_60,   1600, 1200, 60 },
    { OT_VO_OUT_1920x1200_60,   1920, 1200, 60 },
    { OT_VO_OUT_1920x2160_30,   1920, 2160, 30 },
    { OT_VO_OUT_2560x1440_30,   2560, 1440, 30 },
    { OT_VO_OUT_2560x1440_60,   2560, 1440, 60 },
    { OT_VO_OUT_2560x1600_60,   2560, 1600, 60 },

    { OT_VO_OUT_3840x2160_24, 3840, 2160, 24 },
    { OT_VO_OUT_3840x2160_25, 3840, 2160, 25 },
    { OT_VO_OUT_3840x2160_30, 3840, 2160, 30 },
    { OT_VO_OUT_3840x2160_50, 3840, 2160, 50 },
    { OT_VO_OUT_3840x2160_60, 3840, 2160, 60 },
    { OT_VO_OUT_4096x2160_24, 4096, 2160, 24 },
    { OT_VO_OUT_4096x2160_25, 4096, 2160, 25 },
    { OT_VO_OUT_4096x2160_30, 4096, 2160, 30 },
    { OT_VO_OUT_4096x2160_50, 4096, 2160, 50 },
    { OT_VO_OUT_4096x2160_60, 4096, 2160, 60 },
    { OT_VO_OUT_7680x4320_30, 7680, 4320, 30 },

    { OT_VO_OUT_240x320_50, 240, 320, 50 },
    { OT_VO_OUT_320x240_50, 320, 240, 50 },
    { OT_VO_OUT_240x320_60, 240, 320, 60 },
    { OT_VO_OUT_320x240_60, 320, 240, 60 },
    { OT_VO_OUT_800x600_50, 800, 600, 50 },

    { OT_VO_OUT_720x1280_60, 720, 1280, 60 },
    { OT_VO_OUT_1080x1920_60, 1080, 1920, 60 },
    { OT_VO_OUT_USER,  0,    0,    0 }
};

td_s32 sample_comm_vo_get_width_height(ot_vo_intf_sync intf_sync, td_u32 *width, td_u32 *height, td_u32 *frame_rate)
{
    check_null_ptr_return(width);
    check_null_ptr_return(height);
    check_null_ptr_return(frame_rate);

    if (intf_sync > OT_VO_OUT_USER) {
        sample_print("vo intf_sync %d not support!\n", intf_sync);
        return TD_FAILURE;
    }

    *width = g_vo_sample_sync_info[intf_sync].width;
    *height = g_vo_sample_sync_info[intf_sync].height;
    *frame_rate = g_vo_sample_sync_info[intf_sync].frame_rate;
    return TD_SUCCESS;
}

td_s32 sample_comm_vo_start_dev(ot_vo_dev vo_dev, const ot_vo_pub_attr *pub_attr,
    const ot_vo_user_sync_info *sync_info, td_u32 dev_frame_rate)
{
    td_s32 ret;
    check_null_ptr_return(pub_attr);
    if (pub_attr->intf_sync == OT_VO_OUT_USER) {
        check_null_ptr_return(sync_info);
    }

    ret = ss_mpi_vo_set_pub_attr(vo_dev, pub_attr);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    if (pub_attr->intf_sync == OT_VO_OUT_USER) {
        ret = ss_mpi_vo_set_user_sync_info(vo_dev, sync_info);
        if (ret != TD_SUCCESS) {
            sample_print("failed with %#x!\n", ret);
            return TD_FAILURE;
        }

        ret = ss_mpi_vo_set_dev_frame_rate(vo_dev, dev_frame_rate);
        if (ret != TD_SUCCESS) {
            sample_print("failed with %#x!\n", ret);
            return TD_FAILURE;
        }
    }

    ret = ss_mpi_vo_enable(vo_dev);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_vo_stop_dev(ot_vo_dev vo_dev)
{
    td_s32 ret;

    ret = ss_mpi_vo_disable(vo_dev);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_vo_start_layer(ot_vo_layer vo_layer, const ot_vo_video_layer_attr *layer_attr)
{
    td_s32 ret;

    check_null_ptr_return(layer_attr);

    ret = ss_mpi_vo_set_video_layer_attr(vo_layer, layer_attr);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_vo_enable_video_layer(vo_layer);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_vo_stop_layer(ot_vo_layer vo_layer)
{
    td_s32 ret;

    ret = ss_mpi_vo_disable_video_layer(vo_layer);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static sample_vo_wnd_info g_vo_sample_wnd_info[VO_MODE_BUTT] = {
    { VO_MODE_1MUX,   1,  1, 0, 0 },
    { VO_MODE_2MUX,   2,  2, 0, 0 },
    { VO_MODE_4MUX,   4,  2, 0, 0 },
    { VO_MODE_8MUX,   8,  3, 0, 0 },
    { VO_MODE_9MUX,   9,  3, 0, 0 },
    { VO_MODE_16MUX,  16, 4, 0, 0 },
    { VO_MODE_25MUX,  25, 5, 0, 0 },
    { VO_MODE_36MUX,  36, 6, 0, 0 },
    { VO_MODE_49MUX,  49, 7, 0, 0 },
    { VO_MODE_64MUX,  64, 8, 0, 0 },
    { VO_MODE_2X4,    8,  4, 4, 2 }
};

td_s32 sample_comm_vo_get_wnd_info(sample_vo_mode mode, sample_vo_wnd_info *wnd_info)
{
    check_null_ptr_return(wnd_info);

    if (mode >= VO_MODE_BUTT) {
        sample_print("vo mode %d not support!\n", mode);
        return TD_FAILURE;
    }

    wnd_info->mode = mode;
    wnd_info->wnd_num = g_vo_sample_wnd_info[mode].wnd_num;
    wnd_info->square = g_vo_sample_wnd_info[mode].square;
    wnd_info->row = g_vo_sample_wnd_info[mode].row;
    wnd_info->col = g_vo_sample_wnd_info[mode].col;
    return TD_SUCCESS;
}

td_s32 sample_comm_vo_get_chn_attr(sample_vo_wnd_info *wnd_info, ot_vo_video_layer_attr *layer_attr, td_s32 chn,
    ot_vo_chn_attr *chn_attr)
{
    td_u32 width;
    td_u32 height;

    check_null_ptr_return(wnd_info);
    check_null_ptr_return(layer_attr);
    check_null_ptr_return(chn_attr);

    width = layer_attr->img_size.width;
    height = layer_attr->img_size.height;
    if (wnd_info->mode != VO_MODE_2X4) {
        chn_attr->rect.x = OT_ALIGN_DOWN((width / wnd_info->square) * (chn % wnd_info->square), 2);  /* 2: 2 align */
        chn_attr->rect.y = OT_ALIGN_DOWN((height / wnd_info->square) * (chn / wnd_info->square), 2); /* 2: 2 align */
        chn_attr->rect.width = OT_ALIGN_DOWN(width / wnd_info->square, 2);                           /* 2: 2 align */
        chn_attr->rect.height = OT_ALIGN_DOWN(height / wnd_info->square, 2);                         /* 2: 2 align */
    } else {
        chn_attr->rect.x = OT_ALIGN_DOWN((width / wnd_info->col) * (chn % wnd_info->col), 2);  /* 2: 2 align */
        chn_attr->rect.y = OT_ALIGN_DOWN((height / wnd_info->row) * (chn / wnd_info->col), 2); /* 2: 2 align */
        chn_attr->rect.width = OT_ALIGN_DOWN(width / wnd_info->col, 2);                        /* 2: 2 align */
        chn_attr->rect.height = OT_ALIGN_DOWN(height / wnd_info->row, 2);                      /* 2: 2 align */
    }

    chn_attr->priority = 0;
    chn_attr->deflicker_en = TD_FALSE;
    return TD_SUCCESS;
}

td_s32 sample_comm_vo_start_chn(ot_vo_layer vo_layer, sample_vo_mode mode)
{
    td_s32 ret;
    td_s32 i;
    sample_vo_wnd_info wnd_info = { 0 };
    ot_vo_chn_attr chn_attr;
    ot_vo_video_layer_attr layer_attr;

    ret = sample_comm_vo_get_wnd_info(mode, &wnd_info);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return ret;
    }

    ret = ss_mpi_vo_get_video_layer_attr(vo_layer, &layer_attr);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    for (i = 0; i < (td_s32)wnd_info.wnd_num; i++) {
        ret = sample_comm_vo_get_chn_attr(&wnd_info, &layer_attr, i, &chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("failed with %#x!\n", ret);
            return TD_FAILURE;
        }

        ret = ss_mpi_vo_set_chn_attr(vo_layer, i, &chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("failed with %#x!\n", ret);
            return TD_FAILURE;
        }

        ret = ss_mpi_vo_enable_chn(vo_layer, i);
        if (ret != TD_SUCCESS) {
            sample_print("failed with %#x!\n", ret);
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_vo_stop_chn(ot_vo_layer vo_layer, sample_vo_mode mode)
{
    td_s32 ret;
    td_s32 i;
    sample_vo_wnd_info wnd_info = { 0 };

    ret = sample_comm_vo_get_wnd_info(mode, &wnd_info);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return ret;
    }

    for (i = 0; i < (td_s32)wnd_info.wnd_num; i++) {
        ret = ss_mpi_vo_disable_chn(vo_layer, i);
        if (ret != TD_SUCCESS) {
            sample_print("failed with %#x!\n", ret);
            return TD_FAILURE;
        }
    }

    return ret;
}

static td_void sample_private_vo_get_def_wbc_attr(ot_vo_wbc_attr *wbc_attr, ot_vo_wbc_src *source)
{
    wbc_attr->target_size.width = FHD_WIDTH;
    wbc_attr->target_size.height = FHD_HEIGHT;
    wbc_attr->pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    wbc_attr->frame_rate = 30; /* 30: 30fps */
    wbc_attr->compress_mode = OT_COMPRESS_MODE_NONE;
    wbc_attr->dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    source->src_type = OT_VO_WBC_SRC_DEV;
    source->src_id = SAMPLE_VO_DEV_DHD0;
}

static td_s32 sample_private_vo_start_wbc(ot_vo_wbc vo_wbc, ot_vo_wbc_attr *wbc_attr, ot_vo_wbc_src *source)
{
    td_s32 ret;

    ret = ss_mpi_vo_set_wbc_src(vo_wbc, source);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_vo_set_wbc_attr(vo_wbc, wbc_attr);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_vo_enable_wbc(vo_wbc);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_private_vo_stop_wbc(ot_vo_wbc vo_wbc)
{
    td_s32 ret;

    ret = ss_mpi_vo_disable_wbc(vo_wbc);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_vo_start_wbc(sample_vo_wbc_cfg *wbc_config)
{
    td_s32 ret;

    check_null_ptr_return(wbc_config);

    /* set dynamic range if changed */
    if ((td_s32)wbc_config->dynamic_range != -1) {
        wbc_config->wbc_attr.dynamic_range = wbc_config->dynamic_range;
    }

    /* set compress mode if changed */
    if ((td_s32)wbc_config->compress_mode != -1) {
        wbc_config->wbc_attr.compress_mode = wbc_config->compress_mode;
    }

    if (wbc_config->depth > -1) {
        ret = ss_mpi_vo_set_wbc_depth(wbc_config->vo_wbc, 5); /* 5: wbc depth */
        if (ret != TD_SUCCESS) {
            sample_print("failed with %#x!\n", ret);
            return ret;
        }
    }

    wbc_config->wbc_source.src_type = wbc_config->source_type;
    ret = sample_private_vo_start_wbc(wbc_config->vo_wbc, &wbc_config->wbc_attr, &wbc_config->wbc_source);
    return ret;
}

td_s32 sample_comm_vo_stop_wbc(sample_vo_wbc_cfg *wbc_config)
{
    check_null_ptr_return(wbc_config);
    return sample_private_vo_stop_wbc(wbc_config->vo_wbc);
}

td_s32 sample_comm_vo_get_def_wbc_config(sample_vo_wbc_cfg *wbc_config)
{
    check_null_ptr_return(wbc_config);

    wbc_config->vo_wbc = 0;
    wbc_config->source_type = OT_VO_WBC_SRC_DEV;
    wbc_config->wbc_source.src_type = OT_VO_WBC_SRC_DEV;
    wbc_config->wbc_source.src_id = 0;
    wbc_config->dynamic_range = OT_DYNAMIC_RANGE_BUTT;
    wbc_config->compress_mode = OT_COMPRESS_MODE_BUTT;
    wbc_config->depth = OT_INVALID_VALUE;

    sample_private_vo_get_def_wbc_attr(&wbc_config->wbc_attr, &wbc_config->wbc_source);

    return TD_SUCCESS;
}

#if VO_LT8618SX
static td_s32 g_vo_lt8618sx_fd = OT_INVALID_VALUE;

td_s32 sample_comm_vo_bt1120_start(ot_vo_dev vo_dev, ot_vo_pub_attr *pub_attr)
{
    td_s32 ret;
    td_u32 lt_mode;
    td_u32 clk_edge;
    ot_vo_bt_param bt_param = {0};

    check_null_ptr_return(pub_attr);
    lt_mode = pub_attr->intf_sync;
    if (!(pub_attr->intf_type & OT_VO_INTF_BT1120)) {
        return TD_FAILURE;
    }

    ret = ss_mpi_vo_get_bt_param(vo_dev, &bt_param);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
        return TD_FAILURE;
    }

    g_vo_lt8618sx_fd = open(LT8618SX_DEV_NAME, O_RDONLY);
    if (g_vo_lt8618sx_fd < 0) {
        sample_print("open file (%s) fail!\n", LT8618SX_DEV_NAME);
        return TD_FAILURE;
    }

    /* 0:single, 1:dual, set the clk edge firstly */
    clk_edge = (td_u32)bt_param.clk_edge;
    if (ioctl(g_vo_lt8618sx_fd, LT_CMD_SETCLK, &clk_edge) < 0) {
        sample_print("ioctl (%s) set clk fail!\n", LT8618SX_DEV_NAME);
    }

    if (ioctl(g_vo_lt8618sx_fd, LT_CMD_SETMODE, &lt_mode) < 0) {
        sample_print("ioctl (%s) set mode fail!\n", LT8618SX_DEV_NAME);
    }
    close(g_vo_lt8618sx_fd);
    g_vo_lt8618sx_fd = OT_INVALID_VALUE;
    return TD_SUCCESS;
}
#else
td_s32 sample_comm_vo_bt1120_start(ot_vo_dev vo_dev, ot_vo_pub_attr *pub_attr)
{
    ot_unused(vo_dev);
    ot_unused(pub_attr);
    return TD_SUCCESS;
}
#endif

static const ot_hdmi_video_format g_vo_hdmi_sync[OT_VO_OUT_BUTT] = {
    OT_HDMI_VIDEO_FORMAT_PAL,
    OT_HDMI_VIDEO_FORMAT_NTSC,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_861D_640X480_60,
    OT_HDMI_VIDEO_FORMAT_480P_60,
    OT_HDMI_VIDEO_FORMAT_576P_50,
    OT_HDMI_VIDEO_FORMAT_VESA_800X600_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1024X768_60,
    OT_HDMI_VIDEO_FORMAT_720P_50,
    OT_HDMI_VIDEO_FORMAT_720P_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1280X800_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1280X1024_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1366X768_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1400X1050_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1440X900_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1680X1050_60,
    OT_HDMI_VIDEO_FORMAT_1080P_24,
    OT_HDMI_VIDEO_FORMAT_1080P_25,
    OT_HDMI_VIDEO_FORMAT_1080P_30,
    OT_HDMI_VIDEO_FORMAT_1080i_50,
    OT_HDMI_VIDEO_FORMAT_1080i_60,
    OT_HDMI_VIDEO_FORMAT_1080P_50,
    OT_HDMI_VIDEO_FORMAT_1080P_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1600X1200_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1920X1200_60,
    OT_HDMI_VIDEO_FORMAT_1920x2160_30,
    OT_HDMI_VIDEO_FORMAT_2560x1440_30,
    OT_HDMI_VIDEO_FORMAT_2560x1440_60,
    OT_HDMI_VIDEO_FORMAT_2560x1600_60,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_24,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_25,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_30,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_50,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_60,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_24,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_25,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_30,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_50,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_60,
    OT_HDMI_VIDEO_FORMAT_7680X4320P_30,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
};

static td_void sample_comm_vo_hdmi_convert_sync(ot_vo_intf_sync intf_sync, ot_hdmi_video_format *video_fmt)
{
    *video_fmt = OT_HDMI_VIDEO_FORMAT_1080P_60;
    if ((intf_sync >= OT_VO_OUT_PAL) && (intf_sync < OT_VO_OUT_BUTT)) {
        *video_fmt = g_vo_hdmi_sync[intf_sync];
    }
}

td_s32 sample_comm_vo_hdmi_start(ot_vo_intf_sync intf_sync)
{
    ot_hdmi_attr attr;
    ot_hdmi_video_format video_fmt;
    ot_hdmi_id hdmi_id = OT_HDMI_ID_0;

    sample_comm_vo_hdmi_convert_sync(intf_sync, &video_fmt);

    check_return(ss_mpi_hdmi_init(), "ss_mpi_hdmi_init");
    check_return(ss_mpi_hdmi_open(hdmi_id), "ss_mpi_hdmi_open");
    check_return(ss_mpi_hdmi_get_attr(hdmi_id, &attr), "ss_mpi_hdmi_get_attr");
    attr.video_format = video_fmt;
    check_return(ss_mpi_hdmi_set_attr(hdmi_id, &attr), "ss_mpi_hdmi_set_attr");
    check_return(ss_mpi_hdmi_start(hdmi_id), "ss_mpi_hdmi_start");
    return TD_SUCCESS;
}

td_s32 sample_comm_vo_hdmi_stop(td_void)
{
    ot_hdmi_id hdmi_id = OT_HDMI_ID_0;

    check_return(ss_mpi_hdmi_stop(hdmi_id), "ss_mpi_hdmi_stop");
    check_return(ss_mpi_hdmi_close(hdmi_id), "ss_mpi_hdmi_close");
    check_return(ss_mpi_hdmi_deinit(), "ss_mpi_hdmi_deinit");
    return TD_SUCCESS;
}

/* dhd0: 1080P60, dhd1: 1080P60 */
static const ot_vo_sync_info g_sample_comm_vo_timing[OT_VO_MAX_PHYS_DEV_NUM] = {
    /*
     * |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-|
     * syncm, iop, itf,   vact, vbb,  vfb,  hact,  hbb,  hfb, hmid,bvact,bvbb, bvfb, hpw, vpw,idv, ihs, ivs
     */
    { 0, 1, 1, 1080, 41, 4, 1920, 192, 88,  1,    1,   1,  1,  44, 5, 0, 0, 0 }, /* 1080P@60_hz */
    { 0, 1, 1, 1080, 41, 4, 1920, 192, 88,  1,    1,   1,  1,  44, 5, 0, 0, 0 }, /* 1080P@60_hz */
};

const ot_vo_sync_info *vo_get_dev_timing(ot_vo_dev dev)
{
    if ((dev < 0) || (dev >= OT_VO_MAX_PHYS_DEV_NUM)) {
        return TD_NULL;
    }
    return &(g_sample_comm_vo_timing[dev]);
}

/* dhd0: 1080P60, dhd1: 1080P60 */
static const ot_vo_user_sync_info g_sample_comm_vo_sync_info[OT_VO_MAX_PHYS_DEV_NUM] = {
    {
        .user_sync_attr = {
            .clk_src = OT_VO_CLK_SRC_PLL,
            .vo_pll = { /* if hdmi, set it by pixel clk and div mode */
                .fb_div = 99, /* 99 fb div */
                .frac = 0,
                .ref_div = 2, /* 2 ref div */
                .post_div1 = 4, /* 4 post div1 */
                .post_div2 = 2, /* 2 post div2 */
            },
        },
        .pre_div = 1, /* if hdmi, set it by pixel clk */
        .dev_div = 1, /* if rgb, set it by serial mode */
        .clk_reverse_en = TD_FALSE,
    }, {
        .user_sync_attr = {
            .clk_src = OT_VO_CLK_SRC_PLL,
            .vo_pll = { /* if hdmi, set it by pixel clk and div mode */
                .fb_div = 99, /* 99 fb div */
                .frac = 0,
                .ref_div = 2, /* 2 ref div */
                .post_div1 = 4, /* 4 post div1 */
                .post_div2 = 2, /* 2 post div2 */
            },
        },
        .pre_div = 1, /* if hdmi, set it by pixel clk */
        .dev_div = 1, /* if rgb, set it by serial mode */
        .clk_reverse_en = TD_FALSE,
    }
};

const ot_vo_user_sync_info *vo_get_dev_user_sync_info(ot_vo_dev dev)
{
    if ((dev < 0) || (dev >= OT_VO_MAX_PHYS_DEV_NUM)) {
        return TD_NULL;
    }
    return &g_sample_comm_vo_sync_info[dev];
}

static td_void sample_comm_vo_get_def_sync_info_config(ot_vo_dev dev, ot_vo_sync_info *sync_info)
{
    (td_void)memcpy_s(sync_info, sizeof(ot_vo_sync_info), &g_sample_comm_vo_timing[dev], sizeof(ot_vo_sync_info));
}

static td_void sample_comm_vo_get_def_user_sync_config(ot_vo_dev dev, ot_vo_user_sync_info *user_sync)
{
    (td_void)memcpy_s(user_sync, sizeof(ot_vo_user_sync_info), &g_sample_comm_vo_sync_info[dev],
        sizeof(ot_vo_user_sync_info));
}

td_s32 sample_comm_vo_get_def_config(sample_vo_cfg *vo_config)
{
    ot_rect def_disp_rect = { 0, 0, 1920, 1080 };
    ot_size def_img_size = { 1920, 1080 };

    check_null_ptr_return(vo_config);

    vo_config->vo_dev = SAMPLE_VO_DEV_UHD;
    vo_config->vo_intf_type = OT_VO_INTF_HDMI;
    vo_config->intf_sync = OT_VO_OUT_1080P60;
    vo_config->bg_color = COLOR_RGB_BLACK;
    vo_config->pix_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vo_config->disp_rect = def_disp_rect;
    vo_config->image_size = def_img_size;
    vo_config->vo_part_mode = OT_VO_PARTITION_MODE_SINGLE;
    vo_config->dis_buf_len = 3; /* 3: def buf len for single */
    vo_config->dst_dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    vo_config->vo_mode = VO_MODE_1MUX;
    vo_config->compress_mode = OT_COMPRESS_MODE_NONE;

    sample_comm_vo_get_def_sync_info_config(vo_config->vo_dev, &vo_config->sync_info);
    sample_comm_vo_get_def_user_sync_config(vo_config->vo_dev, &vo_config->user_sync);

    return TD_SUCCESS;
}

static td_s32 sample_comm_vo_get_vo_layer_attr(const sample_vo_cfg *vo_config, ot_vo_video_layer_attr *layer_attr)
{
    td_s32 ret;
    ot_rect def_disp_rect = { 0, 0, 1920, 1080 };
    ot_size def_img_size = { 1920, 1080 };

    check_null_ptr_return(vo_config);
    check_null_ptr_return(layer_attr);

    ret = sample_comm_vo_get_width_height(vo_config->intf_sync, &layer_attr->display_rect.width,
        &layer_attr->display_rect.height, &layer_attr->display_frame_rate);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_get_width_height failed!\n");
        return ret;
    }
    if (vo_config->intf_sync == OT_VO_OUT_USER) {
        layer_attr->display_frame_rate = vo_config->dev_frame_rate;
    }

    layer_attr->cluster_mode_en = TD_FALSE;
    layer_attr->double_frame_en = TD_FALSE;
    layer_attr->pixel_format = vo_config->pix_format;
    layer_attr->display_rect.x = 0;
    layer_attr->display_rect.y = 0;

    /* set display rectangle if changed. */
    if (memcmp(&vo_config->disp_rect, &def_disp_rect, sizeof(ot_rect)) != 0) {
        layer_attr->display_rect.width = vo_config->disp_rect.width;
        layer_attr->display_rect.height = vo_config->disp_rect.height;
    }

    layer_attr->img_size.width = layer_attr->display_rect.width;
    layer_attr->img_size.height = layer_attr->display_rect.height;

    /* set image size if changed. */
    if (memcmp(&vo_config->image_size, &def_img_size, sizeof(ot_size)) != 0) {
        layer_attr->img_size.width = vo_config->image_size.width;
        layer_attr->img_size.height = vo_config->image_size.height;
    }

    layer_attr->dst_dynamic_range = vo_config->dst_dynamic_range;
    layer_attr->display_buf_len = vo_config->dis_buf_len;
    layer_attr->partition_mode = vo_config->vo_part_mode;
    layer_attr->compress_mode = vo_config->compress_mode;
    return TD_SUCCESS;
}

static td_void sample_comm_vo_set_intf_param(const sample_vo_cfg *vo_config)
{
    ot_vo_hdmi_param hdmi_param = { 0 };

    /* set interface param */
    if (g_vo_hdmi_rgb_mode == TD_TRUE) {
        ss_mpi_vo_get_hdmi_param(vo_config->vo_dev, &hdmi_param);
        hdmi_param.csc.csc_matrix = OT_VO_CSC_MATRIX_BT709FULL_TO_RGBFULL;
        ss_mpi_vo_set_hdmi_param(vo_config->vo_dev, &hdmi_param);
    }
}

td_void sample_comm_vo_set_hdmi_rgb_mode(td_bool enable)
{
    g_vo_hdmi_rgb_mode = !!enable;
}

td_s32 sample_comm_vo_start_vo(const sample_vo_cfg *vo_config)
{
    /* VO device vo_dev# information declaration. */
    td_s32 ret;
    ot_vo_dev vo_dev;
    ot_vo_layer vo_layer;
    ot_vo_pub_attr vo_pub_attr = { 0 };
    ot_vo_video_layer_attr layer_attr = { 0 };

    check_null_ptr_return(vo_config);

    vo_dev = vo_config->vo_dev;
    vo_layer = vo_config->vo_dev;

    /* set and start VO device vo_dev#. */
    vo_pub_attr.intf_type = vo_config->vo_intf_type;
    vo_pub_attr.intf_sync = vo_config->intf_sync;
    vo_pub_attr.bg_color = vo_config->bg_color;
    (td_void)memcpy_s(&vo_pub_attr.sync_info, sizeof(ot_vo_sync_info), &vo_config->sync_info, sizeof(ot_vo_sync_info));
    ret = sample_comm_vo_start_dev(vo_dev, &vo_pub_attr, &vo_config->user_sync, vo_config->dev_frame_rate);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_start_dev failed!\n");
        return ret;
    }

    ret = sample_comm_vo_get_vo_layer_attr(vo_config, &layer_attr);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_start get video layer failed!\n");
        (td_void)sample_comm_vo_stop_dev(vo_dev);
        return ret;
    }

    ret = sample_comm_vo_start_layer(vo_layer, &layer_attr);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_start set video layer failed!\n");
        (td_void)sample_comm_vo_stop_dev(vo_dev);
        return ret;
    }

    /* set interface param */
    sample_comm_vo_set_intf_param(vo_config);

    /* start vo channels. */
    ret = sample_comm_vo_start_chn(vo_layer, vo_config->vo_mode);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_start_chn failed!\n");
        (td_void)sample_comm_vo_stop_layer(vo_layer);
        (td_void)sample_comm_vo_stop_dev(vo_dev);
        return ret;
    }

    /* start hdmi device. note : do this after vo device started. */
    if (vo_config->vo_intf_type & OT_VO_INTF_HDMI) {
        (td_void)sample_comm_vo_hdmi_start(vo_config->intf_sync);
    }

    /* start bt1120 device. note : do this after vo device started. */
    if (vo_config->vo_intf_type & OT_VO_INTF_BT1120) {
        (td_void)sample_comm_vo_bt1120_start(vo_dev, &vo_pub_attr);
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_vo_start_pip(sample_vo_cfg *vo_config)
{
    /* VO device vo_dev# information declaration. */
    ot_vo_dev vo_dev;
    ot_vo_layer vo_layer;
    ot_vo_pub_attr vo_pub_attr = { 0 };
    ot_vo_video_layer_attr layer_attr = { 0 };
    td_s32 ret;

    check_null_ptr_return(vo_config);

    vo_dev = vo_config->vo_dev;
    vo_layer = SAMPLE_VO_LAYER_PIP;

    vo_pub_attr.intf_sync = vo_config->intf_sync;
    /* set and start layer vo_dev#. */
    ret = sample_comm_vo_get_width_height(vo_pub_attr.intf_sync, &layer_attr.display_rect.width,
        &layer_attr.display_rect.height, &layer_attr.display_frame_rate);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_get_width_height failed!\n");
        (td_void)sample_comm_vo_stop_dev(vo_dev);
        return ret;
    }

    layer_attr.cluster_mode_en = TD_FALSE;
    layer_attr.double_frame_en = TD_FALSE;
    layer_attr.pixel_format = vo_config->pix_format;
    layer_attr.display_rect.x = 0;
    layer_attr.display_rect.y = 0;
    /* set display rectangle if changed. */
    layer_attr.display_rect.width /= 2;  /* 2: 2x */
    layer_attr.display_rect.height /= 2; /* 2: 2x */
    layer_attr.img_size.width = layer_attr.display_rect.width;
    layer_attr.img_size.height = layer_attr.display_rect.height;
    /* set image size if changed. */
    layer_attr.dst_dynamic_range = vo_config->dst_dynamic_range;
    layer_attr.display_buf_len = vo_config->dis_buf_len;
    layer_attr.partition_mode = vo_config->vo_part_mode;

    ret = sample_comm_vo_start_layer(vo_layer, &layer_attr);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_start video layer failed!\n");
        (td_void)sample_comm_vo_stop_dev(vo_dev);
        return ret;
    }

    /* start vo channels. */
    ret = sample_comm_vo_start_chn(vo_layer, vo_config->vo_mode);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_start_chn failed!\n");
        (td_void)sample_comm_vo_stop_layer(vo_layer);
        (td_void)sample_comm_vo_stop_dev(vo_dev);
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_vo_stop_vo(const sample_vo_cfg *vo_config)
{
    td_s32 ret;
    ot_vo_dev vo_dev;
    ot_vo_layer vo_layer;
    sample_vo_mode vo_mode;

    check_null_ptr_return(vo_config);

    vo_dev = vo_config->vo_dev;
    vo_layer = vo_config->vo_dev;
    vo_mode = vo_config->vo_mode;

    if (OT_VO_INTF_HDMI & vo_config->vo_intf_type) {
        sample_comm_vo_hdmi_stop();
    }

    ret = sample_comm_vo_stop_chn(vo_layer, vo_mode);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_stop_vo stop chn failed!\n");
        return ret;
    }

    ret = sample_comm_vo_stop_layer(vo_layer);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_stop_vo stop layer failed!\n");
        return ret;
    }

    ret = sample_comm_vo_stop_dev(vo_dev);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_stop_vo stop dev failed!\n");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_vo_stop_pip(const sample_vo_cfg *vo_config)
{
    td_s32 ret;
    ot_vo_layer vo_layer;

    check_null_ptr_return(vo_config);

    vo_layer = SAMPLE_VO_LAYER_PIP;
    ret = sample_comm_vo_stop_chn(vo_layer, vo_config->vo_mode);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_stop_pip stop chn failed!\n");
        return ret;
    }

    ret = sample_comm_vo_stop_layer(vo_layer);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_stop_pip stop layer failed!\n");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_vo_get_def_layer_config(sample_comm_vo_layer_cfg *vo_layer_config)
{
    ot_rect def_disp_rect = { 0, 0, 1920, 1080 };
    ot_size def_img_size = { 1920, 1080 };

    check_null_ptr_return(vo_layer_config);

    vo_layer_config->vo_layer = SAMPLE_VO_LAYER_VHD2;
    vo_layer_config->intf_sync = OT_VO_OUT_1080P60;
    vo_layer_config->display_rect = def_disp_rect;
    vo_layer_config->image_size = def_img_size;
    vo_layer_config->pix_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vo_layer_config->dis_buf_len = 3; /* 3: display buf len */
    vo_layer_config->vo_mode = VO_MODE_1MUX;
    return TD_SUCCESS;
}

static td_s32 sample_comm_vo_get_layer_layer_attr(sample_comm_vo_layer_cfg *vo_layer_config,
    ot_vo_video_layer_attr *layer_attr)
{
    td_s32 ret;
    td_u32 frame_rate = 0;
    td_u32 width = 0;
    td_u32 height = 0;
    ot_rect def_disp_rect = { 0, 0, 1920, 1080 };
    ot_size def_img_size = { 1920, 1080 };

    check_null_ptr_return(vo_layer_config);
    check_null_ptr_return(layer_attr);

    /* start vo layer. */
    ret = sample_comm_vo_get_width_height(vo_layer_config->intf_sync, &width, &height, &frame_rate);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_get_width_height failed!\n");
        return TD_FAILURE;
    }

    layer_attr->display_rect.x = 0;
    layer_attr->display_rect.y = 0;
    layer_attr->display_rect.width = width;
    layer_attr->display_rect.height = height;
    layer_attr->display_frame_rate = frame_rate;
    layer_attr->double_frame_en = TD_FALSE;
    layer_attr->cluster_mode_en = TD_FALSE;
    layer_attr->pixel_format = vo_layer_config->pix_format;

    /* set display rectangle if changed. */
    if (memcmp(&vo_layer_config->display_rect, &def_disp_rect, sizeof(ot_rect)) != 0) {
        layer_attr->display_rect.width = vo_layer_config->display_rect.width;
        layer_attr->display_rect.height = vo_layer_config->display_rect.height;
    }
    layer_attr->img_size.width = layer_attr->display_rect.width;
    layer_attr->img_size.height = layer_attr->display_rect.height;

    /* set image size if changed. */
    if (memcmp(&vo_layer_config->image_size, &def_img_size, sizeof(ot_size)) != 0) {
        layer_attr->img_size.width = vo_layer_config->image_size.width;
        layer_attr->img_size.height = vo_layer_config->image_size.height;
    }

    layer_attr->dst_dynamic_range = vo_layer_config->dst_dynamic_range;
    layer_attr->display_buf_len = vo_layer_config->dis_buf_len;
    return TD_SUCCESS;
}

td_s32 sample_comm_vo_start_layer_chn(sample_comm_vo_layer_cfg *vo_layer_config)
{
    td_s32 ret;
    ot_vo_layer vo_layer;
    ot_vo_video_layer_attr layer_attr = { 0 };

    check_null_ptr_return(vo_layer_config);

    vo_layer = vo_layer_config->vo_layer;
    ret = sample_comm_vo_get_layer_layer_attr(vo_layer_config, &layer_attr);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_start video layer failed!\n");
        return ret;
    }

    ret = sample_comm_vo_start_layer(vo_layer, &layer_attr);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_start video layer failed!\n");
        return ret;
    }

    /* start vo channels. */
    ret = sample_comm_vo_start_chn(vo_layer, vo_layer_config->vo_mode);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_start_chn failed!\n");
        (td_void)sample_comm_vo_stop_layer(vo_layer);
        return ret;
    }
    return ret;
}

td_s32 sample_comm_vo_stop_layer_chn(sample_comm_vo_layer_cfg *vo_layer_config)
{
    td_s32 ret;
    ot_vo_layer vo_layer;

    check_null_ptr_return(vo_layer_config);

    vo_layer = vo_layer_config->vo_layer;
    ret = sample_comm_vo_stop_chn(vo_layer, vo_layer_config->vo_mode);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_stop_layer_chn stop chn failed!\n");
        return ret;
    }

    ret = sample_comm_vo_stop_layer(vo_layer);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_stop_layer_chn stop layer failed!\n");
        return ret;
    }

    return TD_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
