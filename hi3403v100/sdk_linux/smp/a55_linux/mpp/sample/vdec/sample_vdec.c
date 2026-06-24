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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include "sample_comm.h"
#include "sdk_module_init.h"
#ifdef CONFIG_OT_HEIF_SUPPORT
#include "heif_format.h"
#endif

#define PIC_SIZE   PIC_3840X2160
#define SAMPLE_STREAM_PATH "./source_file"
#define UHD_STREAN_WIDTH 3840
#define UHD_STREAM_HEIGHT 2160
#define FHD_STREAN_WIDTH 1920
#define FHD_STREAN_HEIGHT 1080
#define REF_NUM 2
#define DISPLAY_NUM 2
#define SAMPLE_VDEC_COMM_VB_CNT 4
#define SAMPLE_VDEC_VPSS_LOW_DELAY_LINE_CNT 16

static ot_payload_type g_cur_type = OT_PT_H265;

static vdec_display_cfg g_vdec_display_cfg = {
    .pic_size = PIC_3840X2160,
    .intf_sync = OT_VO_OUT_3840x2160_30,
    .intf_type = OT_VO_INTF_HDMI,
};

#define USLEEP_120000   120000
#define USLEEP_60000    60000
#define USLEEP_50       50

#define CMD_COUNT_800X1280 196
static mipi_tx_cmd_info g_cmd_info_800x1280[CMD_COUNT_800X1280] = {
    /* {devno work_mode lp_clk_en data_type cmd_size cmd}, usleep_value */
    {{0, 0, 0, 0x23, 0x00E0, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x93E1, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x65E2, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0xF8E3, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0380, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x01E0, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0000, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x3B01, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x740C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0017, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0xAF18, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0019, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x001A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0xAF1B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x001C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x2635, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0937, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0438, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0039, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x013A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x783C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0xFF3D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0xFF3E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x7F3F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0640, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0xA041, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x8142, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1443, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x2344, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x2845, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0255, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x6957, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0A59, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x2A5A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x175B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x7F5D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x6B5E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x5C5F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4F60, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4D61, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x3F62, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4263, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x2B64, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4465, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4366, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4367, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x6368, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x5269, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x5A6A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4F6B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4E6C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x206D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0F6E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x006F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x7F70, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x6B71, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x5C72, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4F73, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4D74, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x3F75, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4276, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x2B77, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4478, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4379, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x437A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x637B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x527C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x5A7D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4F7E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4E7F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x2080, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0F81, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0082, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x02E0, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0200, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0201, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0002, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0003, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1E04, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1E05, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F06, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F07, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F08, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1709, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x170A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x370B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x370C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x470D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x470E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x450F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4510, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4B11, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4B12, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4913, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4914, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F15, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0116, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0117, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0018, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0019, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1E1A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1E1B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F1C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F1D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F1E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x171F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1720, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x3721, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x3722, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4623, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4624, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4425, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4426, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4A27, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4A28, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4829, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x482A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F2B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x012C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x012D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x002E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x002F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F30, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F31, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1E32, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1E33, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F34, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1735, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1736, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x3737, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x3738, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0839, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x083A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0A3B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0A3C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x043D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x043E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x063F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0640, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F41, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0242, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0243, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0044, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0045, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F46, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F47, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1E48, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1E49, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F4A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x174B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x174C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x374D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x374E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x094F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0950, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0B51, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0B52, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0553, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0554, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0755, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0756, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1F57, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x4058, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x305B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x165C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x345D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x055E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x025F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0063, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x6A64, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x7367, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1D68, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0869, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x6A6A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x086B, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x006C, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x006D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x006E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x886F, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0xFF75, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0xDD77, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x3F78, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x1579, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x177A, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x147D, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x827E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x04E0, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0E00, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0xB302, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x6109, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x480E, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x00E0, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x02E6, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0CE7, NULL}, USLEEP_50},
    {{0, 0, 0, 0x23, 0x0011, NULL}, USLEEP_120000},
    {{0, 0, 0, 0x23, 0x0029, NULL}, USLEEP_60000}

};

/* VO: USER 800x1280_60, TX: USER 800x1280 */
static const sample_vo_mipi_tx_cfg g_vo_tx_cfg_800x1280_user = {
    .vo_config = {
        .vo_dev = SAMPLE_VO_DEV_UHD,
        .vo_intf_type = OT_VO_INTF_MIPI,
        .intf_sync = OT_VO_OUT_USER,
        .bg_color = COLOR_RGB_BLACK,
        .pix_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420,
        .disp_rect = {0, 0, 800, 1280},
        .image_size = {800, 1280},
        .vo_part_mode = OT_VO_PARTITION_MODE_SINGLE,
        .dis_buf_len = 3, /* 3: def buf len for single */
        .dst_dynamic_range = OT_DYNAMIC_RANGE_SDR8,
        .vo_mode = VO_MODE_1MUX,
        .compress_mode = OT_COMPRESS_MODE_NONE,

        .sync_info = {0, 1, 1, 1280, 24, 20, 800, 40, 40, 1, 1, 1, 1, 20, 4, 0, 0, 0},
        .user_sync = {
            .user_sync_attr = {
                .clk_src = OT_VO_CLK_SRC_PLL,
                .vo_pll = { /* if hdmi, set it by pixel clk and div mode */
                    .fb_div = 105, /* 105 fb div */
                    .frac = 0,
                    .ref_div = 1, /* 1 ref div */
                    .post_div1 = 6, /* 6 post div1 */
                    .post_div2 = 6, /* 6 post div2 */
                },
            },
            .pre_div = 1, /* if hdmi, set it by pixel clk */
            .dev_div = 1, /* if rgb, set it by serial mode */
            .clk_reverse_en = TD_FALSE,
        },
        .dev_frame_rate = 60,
    },
    .tx_config = {
        /* for combo dev config */
        .intf_sync = OT_MIPI_TX_OUT_USER,

        /* for screen cmd */
        .cmd_count = CMD_COUNT_800X1280,
        .cmd_info = g_cmd_info_800x1280,

        /* for user sync */
        .combo_dev_cfg = {
            .devno = 0,
            .lane_id = {0, 1, 2, 3},
            .out_mode = OUT_MODE_DSI_VIDEO,
            .out_format = OUT_FORMAT_RGB_24BIT,

            .video_mode =  BURST_MODE,
            .sync_info = {
                .hsa_pixels = 20, /* 20 pixel */
                .hbp_pixels = 20, /* 20 pixel */
                .hact_pixels = 800, /* 800 pixel */
                .hfp_pixels = 40, /* 40 pixel */
                .vsa_lines = 4, /* 4 line */
                .vbp_lines = 20, /* 20 line */
                .vact_lines = 1280, /* 1280 line */
                .vfp_lines = 20, /* 20 line */
            },
            .phy_data_rate = 999, /* 999 Mbps */
            .pixel_clk = 70000, /* 70000 KHz */
        },
    },
};

static ot_size g_disp_size;
static td_s32 g_sample_exit = 0;

#ifndef __LITEOS__
static td_void sample_vdec_handle_sig(td_s32 signo)
{
    if ((signo == SIGINT) || (signo == SIGTERM)) {
        g_sample_exit = 1;
    }
}
#endif

static td_s32 sample_getchar()
{
    int c;
    if (g_sample_exit == 1) {
        return 'e';
    }

    c = getchar();

    if (g_sample_exit == 1) {
        return 'e';
    }
    return c;
}

static td_void sample_vdec_usage(const char *s_prg_nm)
{
    printf("\n/************************************/\n");
    printf("usage : %s <index>\n", s_prg_nm);
    printf("index:\n");
    printf("\t0:  VDEC(H265 PLAYBACK)-VPSS-VO\n");
    printf("\t1:  VDEC(H264 PLAYBACK)-VPSS-VO\n");
    printf("\t2:  VDEC(JPEG PLAYBACK)-VPSS-VO\n");
    printf("\t3:  VDEC(H265 LOWDELAY PREVIEW)-VPSS-VO\n");
#ifdef SAMPLE_HEIC_SUPPORT
    printf("\t4:  VDEC(HEIC H265 PLAYBACK PREVIEW)-VPSS-VO\n");
#endif
    printf("\t5:  VDEC(H265 PLAYBACK)-VPSS-VO(MIPI-TX)\n");
    printf("/************************************/\n\n");
}

static td_u32 sample_vdec_get_chn_width()
{
    if (g_cur_type != OT_PT_H264 && g_cur_type != OT_PT_H265 &&
        g_cur_type != OT_PT_JPEG && g_cur_type != OT_PT_MJPEG) {
        sample_print("invalid type %d!\n", g_cur_type);
    }
    return UHD_STREAN_WIDTH;
}

static td_u32 sample_vdec_get_chn_height()
{
    if (g_cur_type != OT_PT_H264 && g_cur_type != OT_PT_H265 &&
        g_cur_type != OT_PT_JPEG && g_cur_type != OT_PT_MJPEG) {
        sample_print("invalid type %d!\n", g_cur_type);
    }
    return UHD_STREAM_HEIGHT;
}

static td_s32 sample_init_module_vb(sample_vdec_attr *sample_vdec, td_u32 vdec_chn_num, ot_payload_type type,
    td_u32 len)
{
    td_u32 i;
    td_s32 ret;
    for (i = 0; (i < vdec_chn_num) && (i < len); i++) {
        sample_vdec[i].type                           = type;
        sample_vdec[i].width                         = sample_vdec_get_chn_width();
        sample_vdec[i].height                        = sample_vdec_get_chn_height();
        sample_vdec[i].mode                           = sample_comm_vdec_get_lowdelay_en() ? OT_VDEC_SEND_MODE_COMPAT :
                                                        OT_VDEC_SEND_MODE_FRAME;
        sample_vdec[i].sample_vdec_video.dec_mode      = OT_VIDEO_DEC_MODE_IP;
        sample_vdec[i].sample_vdec_video.bit_width     = OT_DATA_BIT_WIDTH_8;
        if (type == OT_PT_JPEG) {
            sample_vdec[i].sample_vdec_video.ref_frame_num = 0;
        } else {
            sample_vdec[i].sample_vdec_video.ref_frame_num = REF_NUM;
        }
        sample_vdec[i].display_frame_num               = DISPLAY_NUM;
        sample_vdec[i].frame_buf_cnt = (type == OT_PT_JPEG) ? (sample_vdec[i].display_frame_num + 1) :
            (sample_vdec[i].sample_vdec_video.ref_frame_num + sample_vdec[i].display_frame_num + 1);
        if (type == OT_PT_JPEG) {
            sample_vdec[i].sample_vdec_picture.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
            sample_vdec[i].sample_vdec_picture.alpha      = 255; /* 255:pic alpha value */
        }
    }
    ret = sample_comm_vdec_init_vb_pool(vdec_chn_num, &sample_vdec[0], len);
    if (ret != TD_SUCCESS) {
        sample_print("init mod common vb fail for %#x!\n", ret);
        return ret;
    }
    return ret;
}

#ifdef SAMPLE_MEM_SHARE_ENABLE
static td_void sample_init_mem_share(td_void)
{
    td_u32 i;
    ot_vb_common_pools_id pools_id = {0};

    if (ss_mpi_vb_get_common_pool_id(&pools_id) != TD_SUCCESS) {
        sample_print("get common pool_id failed!\n");
        return;
    }
    for (i = 0; i < pools_id.pool_cnt; ++i) {
        ss_mpi_vb_pool_share_all(pools_id.pool[i]);
    }

    if (ss_mpi_vb_get_mod_common_pool_id(OT_VB_UID_VDEC, &pools_id) != TD_SUCCESS) {
        sample_print("get mod(vb_uid %d) common pool_id failed!\n", OT_VB_UID_VDEC);
        return;
    }
    for (i = 0; i < pools_id.pool_cnt; ++i) {
        ss_mpi_vb_pool_share_all(pools_id.pool[i]);
    }
}
#endif

static td_s32 sample_init_sys_and_vb(sample_vdec_attr *sample_vdec, td_u32 vdec_chn_num, ot_payload_type type,
    td_u32 len)
{
    ot_vb_cfg vb_cfg;
    ot_pic_buf_attr buf_attr = {0};
    td_s32 ret;

    ret = sample_comm_sys_get_pic_size(g_vdec_display_cfg.pic_size, &g_disp_size);
    if (ret != TD_SUCCESS) {
        sample_print("sys get pic size fail for %#x!\n", ret);
        return ret;
    }
    buf_attr.align = OT_DEFAULT_ALIGN;
    buf_attr.bit_width = OT_DATA_BIT_WIDTH_8;
    buf_attr.compress_mode = OT_COMPRESS_MODE_SEG;
    buf_attr.height = g_disp_size.height;
    buf_attr.width = g_disp_size.width;
    buf_attr.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;

    (td_void)memset_s(&vb_cfg, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg));
    vb_cfg.max_pool_cnt             = 1;
    vb_cfg.common_pool[0].blk_cnt  = SAMPLE_VDEC_COMM_VB_CNT * vdec_chn_num;
    vb_cfg.common_pool[0].blk_size = ot_common_get_pic_buf_size(&buf_attr);
    ret = sample_comm_sys_init(&vb_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("init sys fail for %#x!\n", ret);
        sample_comm_sys_exit();
        return ret;
    }
    ret = sample_init_module_vb(&sample_vdec[0], vdec_chn_num, type, len);
    if (ret != TD_SUCCESS) {
        sample_print("init mod vb fail for %#x!\n", ret);
        sample_comm_vdec_exit_vb_pool();
        sample_comm_sys_exit();
        return ret;
    }
#ifdef SAMPLE_MEM_SHARE_ENABLE
    sample_init_mem_share();
#endif
    return ret;
}

static td_s32 sample_vdec_bind_vpss(td_u32 vpss_grp_num)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    for (i = 0; i < vpss_grp_num; i++) {
        ret = sample_comm_vdec_bind_vpss(i, i);
        if (ret != TD_SUCCESS) {
            sample_print("vdec bind vpss fail for %#x!\n", ret);
            return ret;
        }
    }
    return ret;
}

static td_void sample_stop_vpss(ot_vpss_grp vpss_grp, td_bool *vpss_chn_enable, td_u32 chn_array_size)
{
    td_s32 i;
    for (i = vpss_grp; i >= 0; i--) {
        vpss_grp = i;
        sample_common_vpss_stop(vpss_grp, &vpss_chn_enable[0], chn_array_size);
    }
}

static td_s32 sample_vdec_unbind_vpss(td_u32 vpss_grp_num)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    for (i = 0; i < vpss_grp_num; i++) {
        ret = sample_comm_vdec_un_bind_vpss(i, i);
        if (ret != TD_SUCCESS) {
            sample_print("vdec unbind vpss fail for %#x!\n", ret);
        }
    }
    return ret;
}

static td_void sample_config_vpss_grp_attr(ot_vpss_grp_attr *vpss_grp_attr)
{
    vpss_grp_attr->max_width = sample_vdec_get_chn_width();
    vpss_grp_attr->max_height = sample_vdec_get_chn_height();
    vpss_grp_attr->frame_rate.src_frame_rate = -1;
    vpss_grp_attr->frame_rate.dst_frame_rate = -1;
    vpss_grp_attr->pixel_format  = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vpss_grp_attr->nr_en   = TD_FALSE;
    vpss_grp_attr->ie_en   = TD_FALSE;
    vpss_grp_attr->dci_en   = TD_FALSE;
    vpss_grp_attr->dei_mode = OT_VPSS_DEI_MODE_OFF;
    vpss_grp_attr->buf_share_en   = TD_FALSE;
}

static td_s32 sample_config_vpss_ldy_attr(td_u32 vpss_grp_num)
{
    td_u32 i;
    td_s32 ret;
    ot_low_delay_info vpss_ldy_info;
    if (!sample_comm_vdec_get_lowdelay_en()) {
        return TD_SUCCESS;
    }
    for (i = 0; i < vpss_grp_num; i++) {
        ret = ss_mpi_vpss_get_low_delay_attr(i, 0, &vpss_ldy_info);
        if (ret != TD_SUCCESS) {
            sample_print("vpss get low delay attr fail for %#x!\n", ret);
            return ret;
        }
        vpss_ldy_info.enable = TD_TRUE;
        vpss_ldy_info.line_cnt = SAMPLE_VDEC_VPSS_LOW_DELAY_LINE_CNT;
        ret = ss_mpi_vpss_set_low_delay_attr(i, 0, &vpss_ldy_info);
        if (ret != TD_SUCCESS) {
            sample_print("vpss set low delay attr fail for %#x!\n", ret);
            return ret;
        }
    }
    return TD_SUCCESS;
}

static td_s32 sample_start_vpss(ot_vpss_grp *vpss_grp, td_u32 vpss_grp_num, td_bool *vpss_chn_enable, td_u32 arr_len)
{
    td_u32 i;
    td_s32 ret;
    ot_vpss_chn_attr vpss_chn_attr[OT_VPSS_MAX_CHN_NUM];
    ot_vpss_grp_attr vpss_grp_attr = {0};
    sample_config_vpss_grp_attr(&vpss_grp_attr);
    (td_void)memset_s(vpss_chn_enable, arr_len * sizeof(td_bool), 0, arr_len * sizeof(td_bool));

    vpss_chn_enable[0] = TD_TRUE;
    vpss_chn_attr[0].width         = g_disp_size.width; /* 4:crop */
    vpss_chn_attr[0].height        = g_disp_size.height; /* 4:crop */
    vpss_chn_attr[0].compress_mode = OT_COMPRESS_MODE_SEG;
    vpss_chn_attr[0].chn_mode                  = OT_VPSS_CHN_MODE_USER;
    vpss_chn_attr[0].pixel_format              = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vpss_chn_attr[0].frame_rate.src_frame_rate = -1;
    vpss_chn_attr[0].frame_rate.dst_frame_rate = -1;
    vpss_chn_attr[0].depth                     = 0;
    vpss_chn_attr[0].mirror_en                 = TD_FALSE;
    vpss_chn_attr[0].flip_en                   = TD_FALSE;
    vpss_chn_attr[0].border_en                 = TD_FALSE;
    vpss_chn_attr[0].aspect_ratio.mode         = OT_ASPECT_RATIO_NONE;

    for (i = 0; i < vpss_grp_num; i++) {
        *vpss_grp = i;
        ret = sample_common_vpss_start(*vpss_grp, &vpss_chn_enable[0],
            &vpss_grp_attr, vpss_chn_attr, OT_VPSS_MAX_CHN_NUM);
        if (ret != TD_SUCCESS) {
            sample_print("start VPSS fail for %#x!\n", ret);
            sample_stop_vpss(*vpss_grp, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
            return ret;
        }
    }

    ret = sample_config_vpss_ldy_attr(vpss_grp_num);
    if (ret != TD_SUCCESS) {
        sample_stop_vpss(*vpss_grp, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
        return ret;
    }

    ret = sample_vdec_bind_vpss(vpss_grp_num);
    if (ret != TD_SUCCESS) {
        sample_vdec_unbind_vpss(vpss_grp_num);
        sample_stop_vpss(*vpss_grp, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
    }
    return ret;
}

static td_s32 sample_vpss_unbind_vo(td_u32 vpss_grp_num, sample_vo_cfg vo_config)
{
    td_u32 i;
    ot_vo_layer vo_layer = vo_config.vo_dev;
    td_s32 ret = TD_SUCCESS;
    for (i = 0; i < vpss_grp_num; i++) {
        ret = sample_comm_vpss_un_bind_vo(i, 0, vo_layer, i);
        if (ret != TD_SUCCESS) {
            sample_print("vpss unbind vo fail for %#x!\n", ret);
        }
    }
    return ret;
}

static td_s32 sample_vpss_bind_vo(sample_vo_cfg vo_config, td_u32 vpss_grp_num)
{
    td_u32 i;
    ot_vo_layer vo_layer;
    td_s32 ret = TD_SUCCESS;
    vo_layer = vo_config.vo_dev;
    for (i = 0; i < vpss_grp_num; i++) {
        ret = sample_comm_vpss_bind_vo(i, 0, vo_layer, i);
        if (ret != TD_SUCCESS) {
            sample_print("vpss bind vo fail for %#x!\n", ret);
            return ret;
        }
    }
    return ret;
}

#ifdef SAMPLE_MEM_SHARE_ENABLE
static td_void sample_init_vo_mem_share(ot_vo_layer vo_layer)
{
    td_s32 ret;
    ot_video_frame_info frame_info;
    td_s32 milli_sec = 5000;
    ot_sys_mem_info mem_info;

    ret = ss_mpi_vo_get_screen_frame(vo_layer, &frame_info, milli_sec);
    if (ret != TD_SUCCESS) {
        sample_print("layer %d ss_mpi_vo_get_screen_frame fail for %#x!\n", vo_layer, ret);
        return;
    }

    ret = ss_mpi_sys_get_mem_info_by_phys(frame_info.video_frame.phys_addr[0], &mem_info);
    if (ret != TD_SUCCESS) {
        ss_mpi_vo_release_screen_frame(vo_layer, &frame_info);
        sample_print("layer %d ss_mpi_sys_get_mem_info_by_phys fail for %#x!\n", vo_layer, ret);
        return;
    }

    ret = ss_mpi_sys_mem_share_all(mem_info.mem_handle);
    if (ret != TD_SUCCESS) {
        ss_mpi_vo_release_screen_frame(vo_layer, &frame_info);
        sample_print("layer %d ss_mpi_sys_get_mem_info_by_phys fail for %#x!\n", vo_layer, ret);
        return;
    }

    ss_mpi_vo_release_screen_frame(vo_layer, &frame_info);
}
#endif

static td_s32 sample_start_vo(sample_vo_cfg *vo_config, td_u32 vpss_grp_num)
{
    td_s32 ret;
    vo_config->vo_dev            = SAMPLE_VO_DEV_UHD;
    vo_config->vo_intf_type      = g_vdec_display_cfg.intf_type;
    vo_config->intf_sync         = OT_VO_OUT_1080P60;
    vo_config->pic_size          = PIC_1080P;
    vo_config->bg_color          = COLOR_RGB_BLUE;
    vo_config->dis_buf_len       = 3; /* 3:buf length */
    vo_config->dst_dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    vo_config->vo_mode           = VO_MODE_1MUX;
    vo_config->pix_format        = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vo_config->disp_rect.x       = 0;
    vo_config->disp_rect.y       = 0;
    vo_config->disp_rect.width = 1920;   /* 1920 */
    vo_config->disp_rect.height = 1080;  /* 1080 */
    vo_config->image_size.width = 1920;  /* 1920 */
    vo_config->image_size.height = 1080; /* 1080 */
    vo_config->vo_part_mode      = OT_VO_PARTITION_MODE_SINGLE;
    vo_config->compress_mode     = OT_COMPRESS_MODE_NONE;

    ret = sample_comm_vo_start_vo(vo_config);
    if (ret != TD_SUCCESS) {
        sample_print("start VO fail for %#x!\n", ret);
        sample_comm_vo_stop_vo(vo_config);
        return ret;
    }
#ifdef SAMPLE_MEM_SHARE_ENABLE
    sample_init_vo_mem_share(vo_config->vo_dev);
#endif
    ret = sample_vpss_bind_vo(*vo_config, vpss_grp_num);
    if (ret != TD_SUCCESS) {
        sample_vpss_unbind_vo(vpss_grp_num, *vo_config);
        sample_comm_vo_stop_vo(vo_config);
    }

    return ret;
}

static td_s32 start_vo_mipi_tx(const sample_vo_mipi_tx_cfg *vo_tx_cfg, td_u32 vpss_grp_num)
{
    td_s32 ret;
    const sample_vo_cfg *vo_config = &vo_tx_cfg->vo_config;
    const sample_mipi_tx_config *tx_config = &vo_tx_cfg->tx_config;

    ret = sample_comm_vo_start_vo(vo_config);
    if (ret != TD_SUCCESS) {
        sample_print("start vo failed with 0x%x!\n", ret);
        return ret;
    }
    printf("start vo dhd%d.\n", vo_config->vo_dev);

#ifdef SAMPLE_MEM_SHARE_ENABLE
    sample_init_vo_mem_share(vo_config->vo_dev);
#endif

    ret = sample_vpss_bind_vo(*vo_config, vpss_grp_num);
    if (ret != TD_SUCCESS) {
        sample_vpss_unbind_vo(vpss_grp_num, *vo_config);
        sample_comm_vo_stop_vo(vo_config);
    }

    if ((vo_config->vo_intf_type & OT_VO_INTF_MIPI) ||
        (vo_config->vo_intf_type & OT_VO_INTF_MIPI_SLAVE)) {
        ret = sample_comm_start_mipi_tx(tx_config);
        if (ret != TD_SUCCESS) {
            sample_print("start mipi tx failed with 0x%x!\n", ret);
            return ret;
        }
    }

    return TD_SUCCESS;
}

static td_void sample_vdec_cmd_ctrl(td_u32 chn_num, vdec_thread_param *vdec_send, pthread_t *vdec_thread,
    td_u32 send_arr_len, td_u32 thread_arr_len)
{
    td_u32 i;
    ot_vdec_chn_status status;
    td_s32 c;

    for (i = 0; (i < chn_num) && (i < send_arr_len) && (i < thread_arr_len); i++) {
        if (vdec_send[i].circle_send == TD_TRUE) {
            goto circle_send;
        }
    }

    sample_comm_vdec_cmd_not_circle_send(chn_num, vdec_send, vdec_thread, send_arr_len, thread_arr_len);
    return;

circle_send:
    c = 0;
    while (c != 'e') {
        printf("\n_sample_test:press 'e' to exit; 'q' to query!;\n");
        c = sample_getchar();
        if (c == 'e') {
            continue;
        } else if (c == 'q') {
            for (i = 0; (i < chn_num) && (i < send_arr_len) && (i < thread_arr_len); i++) {
                ss_mpi_vdec_query_status(vdec_send[i].chn_id, &status);
                sample_comm_vdec_print_chn_status(vdec_send[i].chn_id, status);
            }
        }
    }
    return;
}

#ifdef SAMPLE_HEIC_SUPPORT
static td_void sample_send_heic_to_vdec(const char *stream_name)
{
#ifdef CONFIG_OT_HEIF_SUPPORT
    heif_config config = { 0 };
    config.file_desc.file_type = HEIF_FILE_TYPE_URL;
    if (snprintf_s(config.file_desc.input.url, HEIF_MAX_URL_PATH_LEN, HEIF_MAX_URL_PATH_LEN -1, "%s/%s",
        SAMPLE_STREAM_PATH, stream_name) < 0) {
        return;
    }
    config.config_type = HEIF_CONFIG_DEMUXER;
    heif_handle handle = NULL;

    if (heif_create(&handle, &config) != TD_SUCCESS) {
        return;
    }

    heif_file_info file_info = { 0 };
    td_s32 ret = heif_get_file_info(handle, &file_info);
    if (ret != TD_SUCCESS) {
        printf("[%s:%d]HeifGetFileInfo failed ret:%d\n", __func__, __LINE__, ret);
        goto FAIL;
    }

    heif_image_item item = { 0 };
    td_u32 item_count = 1;
    ret = heif_get_master_image(handle, file_info.track[0].track_id, &item, &item_count);
    if (ret != TD_SUCCESS) {
        printf("[%s:%d] HeifGetExtendInfo fail\n", __FUNCTION__, __LINE__);
        goto FAIL;
    }

    ot_vdec_stream stream = { .pts = item.timestamp, .addr = item.data, .len = item.length, .end_of_frame = TD_TRUE,
        .end_of_stream = TD_TRUE, .need_display = 1};
    ss_mpi_vdec_send_stream(0, &stream, -1);

    ot_vdec_chn_status status;
    td_s32 c;
    c = 0;
    while (c != 'e') {
        printf("\n_sample_test:press 'e' to exit; 'q' to query!;\n");
        c = sample_getchar();
        if (c == 'e') {
            continue;
        } else if (c == 'q') {
            ss_mpi_vdec_query_status(0, &status);
            sample_comm_vdec_print_chn_status(0, status);
        }
    }

FAIL:
    ss_mpi_vdec_stop_recv_stream(0);
    heif_destroy(handle);
#endif
}
#endif

static td_void sample_send_stream_to_vdec(sample_vdec_attr *sample_vdec, td_u32 arr_len, td_u32 vdec_chn_num,
    const char *stream_name)
{
    td_u32 i;
    vdec_thread_param vdec_send[OT_VDEC_MAX_CHN_NUM];
    pthread_t   vdec_thread[OT_VDEC_MAX_CHN_NUM] = {0}; /* 2:thread */
    if (arr_len > OT_VDEC_MAX_CHN_NUM) {
        sample_print("array size(%u) of vdec_send need < %u!\n", arr_len, OT_VDEC_MAX_CHN_NUM);
        return;
    }
    for (i = 0; (i < vdec_chn_num) && (i < arr_len); i++) {
        if (snprintf_s(vdec_send[i].c_file_name, sizeof(vdec_send[i].c_file_name), sizeof(vdec_send[i].c_file_name) - 1,
            stream_name) < 0) {
            return;
        }
        if (snprintf_s(vdec_send[i].c_file_path, sizeof(vdec_send[i].c_file_path), sizeof(vdec_send[i].c_file_path) - 1,
            "%s", SAMPLE_STREAM_PATH) < 0) {
            return;
        }
        vdec_send[i].type          = sample_vdec[i].type;
        vdec_send[i].stream_mode   = sample_vdec[i].mode;
        vdec_send[i].chn_id        = i;
        vdec_send[i].interval_time = 1000; /* 1000: interval time */
        vdec_send[i].pts_init      = 0;
        vdec_send[i].pts_increase  = 0;
        vdec_send[i].e_thread_ctrl = THREAD_CTRL_START;
        vdec_send[i].circle_send   = TD_TRUE;
        vdec_send[i].milli_sec     = 0;
        vdec_send[i].min_buf_size  = (sample_vdec[i].width * sample_vdec[i].height * 3) >> 1; /* 3:yuv */
        vdec_send[i].fps           = 30; /* 30:frame rate */
    }
    sample_comm_vdec_start_send_stream(vdec_chn_num, &vdec_send[0], &vdec_thread[0],
        OT_VDEC_MAX_CHN_NUM, OT_VDEC_MAX_CHN_NUM);

    sample_vdec_cmd_ctrl(vdec_chn_num, &vdec_send[0], &vdec_thread[0],
        OT_VDEC_MAX_CHN_NUM, OT_VDEC_MAX_CHN_NUM);

    sample_comm_vdec_stop_send_stream(vdec_chn_num, &vdec_send[0], &vdec_thread[0],
        OT_VDEC_MAX_CHN_NUM, OT_VDEC_MAX_CHN_NUM);
}

static td_s32 sample_start_vdec(sample_vdec_attr *sample_vdec, td_u32 vdec_chn_num, td_u32 len)
{
    td_s32 ret;

    ret = sample_comm_vdec_start(vdec_chn_num, &sample_vdec[0], len);
    if (ret != TD_SUCCESS) {
        sample_print("start VDEC fail for %#x!\n", ret);
        sample_comm_vdec_stop(vdec_chn_num);
    }

    return ret;
}

static td_s32 sample_h265_vdec_vpss_vo(td_void)
{
    td_s32 ret;
    td_u32 vdec_chn_num;
    td_u32 vpss_grp_num;
    sample_vdec_attr sample_vdec[OT_VDEC_MAX_CHN_NUM];
    td_bool vpss_chn_enable[OT_VPSS_MAX_CHN_NUM];
    sample_vo_cfg vo_config;
    ot_vpss_grp vpss_grp;

    vdec_chn_num = 1;
    vpss_grp_num = vdec_chn_num;
    g_cur_type = OT_PT_H265;
    /************************************************
    step1:  init SYS, init common VB(for VPSS and VO), init module VB(for VDEC)
    *************************************************/
    ret = sample_init_sys_and_vb(&sample_vdec[0], vdec_chn_num, g_cur_type, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /************************************************
    step2:  init VDEC
    *************************************************/
    ret = sample_start_vdec(&sample_vdec[0], vdec_chn_num, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_sys;
    }
    /************************************************
    step3:  start VPSS
    *************************************************/
    ret = sample_start_vpss(&vpss_grp, vpss_grp_num, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_vdec;
    }
    /************************************************
    step4:  start VO
    *************************************************/
    ret = sample_start_vo(&vo_config, vpss_grp_num);
    if (ret != TD_SUCCESS) {
        goto stop_vpss;
    }

    /************************************************
    step5:  send stream to VDEC
    *************************************************/
    sample_send_stream_to_vdec(&sample_vdec[0], OT_VDEC_MAX_CHN_NUM, vdec_chn_num, "3840x2160_8bit.h265");

    ret = sample_vpss_unbind_vo(vpss_grp_num, vo_config);
    sample_comm_vo_stop_vo(&vo_config);
stop_vpss:
    ret = sample_vdec_unbind_vpss(vpss_grp_num);
    sample_stop_vpss(vpss_grp, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
stop_vdec:
    sample_comm_vdec_stop(vdec_chn_num);
    sample_comm_vdec_exit_vb_pool();
stop_sys:
    sample_comm_sys_exit();

    return ret;
}

#ifdef SAMPLE_HEIC_SUPPORT
static td_s32 sample_heic_h265_vdec_vpss_vo(td_void)
{
    td_s32 ret;
    td_u32 vdec_chn_num;
    td_u32 vpss_grp_num;
    sample_vdec_attr sample_vdec[OT_VDEC_MAX_CHN_NUM];
    td_bool vpss_chn_enable[OT_VPSS_MAX_CHN_NUM];
    sample_vo_cfg vo_config;
    ot_vpss_grp vpss_grp;

    vdec_chn_num = 1;
    vpss_grp_num = vdec_chn_num;
    g_cur_type = OT_PT_H265;
    /************************************************
    step1:  init SYS, init common VB(for VPSS and VO), init module VB(for VDEC)
    *************************************************/
    ret = sample_init_sys_and_vb(&sample_vdec[0], vdec_chn_num, g_cur_type, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /************************************************
    step2:  init VDEC
    *************************************************/
    ret = sample_start_vdec(&sample_vdec[0], vdec_chn_num, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_sys;
    }
    /************************************************
    step3:  start VPSS
    *************************************************/
    ret = sample_start_vpss(&vpss_grp, vpss_grp_num, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_vdec;
    }
    /************************************************
    step4:  start VO
    *************************************************/
    ret = sample_start_vo(&vo_config, vpss_grp_num);
    if (ret != TD_SUCCESS) {
        goto stop_vpss;
    }

    /************************************************
    step5:  send stream to VDEC
    *************************************************/
    sample_send_heic_to_vdec("3840x2160_8bit.heic");

    ret = sample_vpss_unbind_vo(vpss_grp_num, vo_config);
    sample_comm_vo_stop_vo(&vo_config);
stop_vpss:
    ret = sample_vdec_unbind_vpss(vpss_grp_num);
    sample_stop_vpss(vpss_grp, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
stop_vdec:
    sample_comm_vdec_stop(vdec_chn_num);
    sample_comm_vdec_exit_vb_pool();
stop_sys:
    sample_comm_sys_exit();

    return ret;
}
#endif

static td_s32 sample_h264_vdec_vpss_vo(td_void)
{
    td_s32 ret;
    td_u32 vdec_chn_num;
    td_u32 vpss_grp_num;
    ot_vpss_grp vpss_grp;
    sample_vdec_attr sample_vdec[OT_VDEC_MAX_CHN_NUM];
    sample_vo_cfg vo_config;
    td_bool vpss_chn_enable[OT_VPSS_MAX_CHN_NUM];

    vdec_chn_num = 1;
    vpss_grp_num = vdec_chn_num;
    g_cur_type = OT_PT_H264;
    /************************************************
    step1:  init SYS, init common VB(for VPSS and VO), init module VB(for VDEC)
    *************************************************/
    ret = sample_init_sys_and_vb(&sample_vdec[0], vdec_chn_num, g_cur_type, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /************************************************
    step2:  init VDEC
    *************************************************/
    ret = sample_start_vdec(&sample_vdec[0], vdec_chn_num, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_sys;
    }
    /************************************************
    step3:  start VPSS
    *************************************************/
    ret = sample_start_vpss(&vpss_grp, vpss_grp_num, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_vdec;
    }
    /************************************************
    step4:  start VO
    *************************************************/
    ret = sample_start_vo(&vo_config, vpss_grp_num);
    if (ret != TD_SUCCESS) {
        goto stop_vpss;
    }

    /************************************************
    step5:  send stream to VDEC
    *************************************************/
    sample_send_stream_to_vdec(&sample_vdec[0], OT_VDEC_MAX_CHN_NUM, vdec_chn_num, "3840x2160_8bit.h264");

    ret = sample_vpss_unbind_vo(vpss_grp_num, vo_config);
    sample_comm_vo_stop_vo(&vo_config);
stop_vpss:
    ret = sample_vdec_unbind_vpss(vpss_grp_num);
    sample_stop_vpss(vpss_grp, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
stop_vdec:
    sample_comm_vdec_stop(vdec_chn_num);
    sample_comm_vdec_exit_vb_pool();
stop_sys:
    sample_comm_sys_exit();

    return ret;
}

static td_s32 sample_jpeg_vdec_vpss_vo(td_void)
{
    td_s32 ret;
    td_u32 vdec_chn_num;
    td_u32 vpss_grp_num;
    ot_vpss_grp vpss_grp;
    sample_vdec_attr sample_vdec[OT_VDEC_MAX_CHN_NUM];
    sample_vo_cfg vo_config;
    td_bool vpss_chn_enable[OT_VPSS_MAX_CHN_NUM];

    vdec_chn_num = 1;
    vpss_grp_num = vdec_chn_num;
    g_cur_type = OT_PT_JPEG;
    /************************************************
    step1:  init SYS, init common VB(for VPSS and VO), init module VB(for VDEC)
    *************************************************/
    ret = sample_init_sys_and_vb(&sample_vdec[0], vdec_chn_num, g_cur_type, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /************************************************
    step2:  init VDEC
    *************************************************/
    ret = sample_start_vdec(&sample_vdec[0], vdec_chn_num, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_sys;
    }
    /************************************************
    step3:  start VPSS
    *************************************************/
    ret = sample_start_vpss(&vpss_grp, vpss_grp_num, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_vdec;
    }
    /************************************************
    step4:  start VO
    *************************************************/
    ret = sample_start_vo(&vo_config, vpss_grp_num);
    if (ret != TD_SUCCESS) {
        goto stop_vpss;
    }

    /************************************************
    step5:  send stream to VDEC
    *************************************************/
    sample_send_stream_to_vdec(&sample_vdec[0], OT_VDEC_MAX_CHN_NUM, vdec_chn_num, "3840x2160.jpg");

    ret = sample_vpss_unbind_vo(vpss_grp_num, vo_config);
    sample_comm_vo_stop_vo(&vo_config);
stop_vpss:
    ret = sample_vdec_unbind_vpss(vpss_grp_num);
    sample_stop_vpss(vpss_grp, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
stop_vdec:
    sample_comm_vdec_stop(vdec_chn_num);
    sample_comm_vdec_exit_vb_pool();
stop_sys:
    sample_comm_sys_exit();

    return ret;
}

static td_s32 sample_h265_lowdelay_vdec_vpss_vo(td_void)
{
    td_s32 ret;
    td_u32 vdec_chn_num;
    td_u32 vpss_grp_num;
    sample_vdec_attr sample_vdec[OT_VDEC_MAX_CHN_NUM];
    td_bool vpss_chn_enable[OT_VPSS_MAX_CHN_NUM];
    sample_vo_cfg vo_config;
    ot_vpss_grp vpss_grp;

    vdec_chn_num = 1;
    vpss_grp_num = vdec_chn_num;
    g_cur_type = OT_PT_H265;
    sample_comm_vdec_set_lowdelay_en(TD_TRUE);
    /************************************************
    step1:  init SYS, init common VB(for VPSS and VO), init module VB(for VDEC)
    *************************************************/
    ret = sample_init_sys_and_vb(&sample_vdec[0], vdec_chn_num, g_cur_type, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /************************************************
    step2:  init VDEC
    *************************************************/
    ret = sample_start_vdec(&sample_vdec[0], vdec_chn_num, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_sys;
    }
    /************************************************
    step3:  start VPSS
    *************************************************/
    ret = sample_start_vpss(&vpss_grp, vpss_grp_num, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_vdec;
    }
    /************************************************
    step4:  start VO
    *************************************************/
    ret = sample_start_vo(&vo_config, vpss_grp_num);
    if (ret != TD_SUCCESS) {
        goto stop_vpss;
    }

    /************************************************
    step5:  send stream to VDEC
    *************************************************/
    sample_send_stream_to_vdec(&sample_vdec[0], OT_VDEC_MAX_CHN_NUM, vdec_chn_num, "3840x2160_8bit.h265");

    ret = sample_vpss_unbind_vo(vpss_grp_num, vo_config);
    sample_comm_vo_stop_vo(&vo_config);
stop_vpss:
    ret = sample_vdec_unbind_vpss(vpss_grp_num);
    sample_stop_vpss(vpss_grp, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
stop_vdec:
    sample_comm_vdec_stop(vdec_chn_num);
    sample_comm_vdec_exit_vb_pool();
stop_sys:
    sample_comm_sys_exit();

    return ret;
}

static td_s32 sample_h265_vdec_vpss_vo_mipi_tx(td_void)
{
    td_s32 ret;
    td_u32 vdec_chn_num;
    td_u32 vpss_grp_num;
    sample_vdec_attr sample_vdec[OT_VDEC_MAX_CHN_NUM];
    td_bool vpss_chn_enable[OT_VPSS_MAX_CHN_NUM];
    ot_vpss_grp vpss_grp;

    vdec_chn_num = 1;
    vpss_grp_num = vdec_chn_num;
    g_cur_type = OT_PT_H265;
    /************************************************
    step1:  init SYS, init common VB(for VPSS and VO), init module VB(for VDEC)
    *************************************************/
    ret = sample_init_sys_and_vb(&sample_vdec[0], vdec_chn_num, g_cur_type, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /************************************************
    step2:  init VDEC
    *************************************************/
    ret = sample_start_vdec(&sample_vdec[0], vdec_chn_num, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_sys;
    }
    /************************************************
    step3:  start VPSS
    *************************************************/
    ret = sample_start_vpss(&vpss_grp, vpss_grp_num, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_vdec;
    }
    /************************************************
    step4:  start VO(MIPI-TX)
    *************************************************/
    ret = start_vo_mipi_tx(&g_vo_tx_cfg_800x1280_user, vpss_grp_num);
    if (ret != TD_SUCCESS) {
        goto stop_vpss;
    }

    /************************************************
    step5:  send stream to VDEC
    *************************************************/
    sample_send_stream_to_vdec(&sample_vdec[0], OT_VDEC_MAX_CHN_NUM, vdec_chn_num, "3840x2160_8bit.h265");

    ret = sample_vpss_unbind_vo(vpss_grp_num, g_vo_tx_cfg_800x1280_user.vo_config);
    sample_comm_stop_mipi_tx(g_vo_tx_cfg_800x1280_user.vo_config.vo_intf_type);
    sample_comm_vo_stop_vo(&g_vo_tx_cfg_800x1280_user.vo_config);
stop_vpss:
    ret = sample_vdec_unbind_vpss(vpss_grp_num);
    sample_stop_vpss(vpss_grp, &vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
stop_vdec:
    sample_comm_vdec_stop(vdec_chn_num);
    sample_comm_vdec_exit_vb_pool();
stop_sys:
    sample_comm_sys_exit();

    return ret;
}
static td_s32 sample_check_parameter(int argc, const char *argv0, const char *argv1)
{
    if ((argc != 2) || (strlen(argv1) != 1)) { /* 2:arg num */
        printf("\n invalid input!  for examples:\n");
        sample_vdec_usage(argv0);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_choose_case(const char argv1, const char *argv0)
{
    td_s32 ret;
    switch (argv1) {
        case '0': {
            ret = sample_h265_vdec_vpss_vo();
            break;
        }
        case '1': {
            ret = sample_h264_vdec_vpss_vo();
            break;
        }
        case '2': {
            ret = sample_jpeg_vdec_vpss_vo();
            break;
        }
        case '3': {
            ret = sample_h265_lowdelay_vdec_vpss_vo();
            break;
        }
#ifdef SAMPLE_HEIC_SUPPORT
        case '4': {
            ret = sample_heic_h265_vdec_vpss_vo();
            break;
        }
#endif
        case '5': {
            ret = sample_h265_vdec_vpss_vo_mipi_tx();
            break;
        }
        default: {
            sample_print("the index is invalid!\n");
            sample_vdec_usage(argv0);
            ret = TD_FAILURE;
            break;
        }
    }
    return ret;
}

/******************************************************************************
* function    : main()
* description : video vdec sample
******************************************************************************/
#ifdef __LITEOS__
    int app_main(int argc, char *argv[])
#else
    int main(int argc, char *argv[])
#endif
{
    td_s32 ret;

#ifdef DRM_OPTION_ENABLE
    sample_print("sample DONOT support DRM!\n");
    return 0;
#endif

    ret = sample_check_parameter(argc, argv[0], argv[1]);
    if (ret != TD_SUCCESS) {
        return ret;
    }

#ifndef __LITEOS__
    sample_sys_signal(sample_vdec_handle_sig);
#endif

#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif

    /******************************************
     choose the case
    ******************************************/
    ret = sample_choose_case(*argv[1], argv[0]);
    if (ret == TD_SUCCESS && g_sample_exit == 0) {
        sample_print("program exit normally!\n");
    } else {
        sample_print("program exit abnormally!\n");
    }

#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif
    return ret;
}
