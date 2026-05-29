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
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "sample_comm.h"
#include "sample_ipc.h"
#include "securec.h"
#include "ot_common_dis.h"
#include "ss_mpi_vi.h"
#include "sdk_module_init.h"
static volatile sig_atomic_t g_sig_flag = 0;

#define X_ALIGN 16
#define Y_ALIGN 2
#define out_ratio_1(x) ((x) / 3)
#define out_ratio_2(x) ((x) * 2 / 3)
#define out_ratio_3(x) ((x) / 2)
#define check_digit(x) ((x) >= '0' && (x) <= '9')

#define VB_RAW_CNT_NONE     0
#define VB_LINEAR_RAW_CNT   5
#define VB_WDR_RAW_CNT      8
#define VB_MULTI_RAW_CNT    15
#define VB_YUV_ROUTE_CNT    10
#define VB_DOUBLE_YUV_CNT   15
#define VB_MULTI_YUV_CNT    30

static td_u16 g_lmf_coef[128] = {
    0, 15, 31, 47, 63, 79, 95, 111, 127, 143, 159, 175,
    191, 207, 223, 239, 255, 271, 286, 302, 318, 334, 350, 365, 381, 397, 412,
    428, 443, 459, 474, 490, 505, 520, 536, 551, 566, 581, 596, 611, 626, 641,
    656, 670, 685, 699, 713, 728, 742, 756, 769, 783, 797, 810, 823, 836, 848,
    861, 873, 885, 896, 908, 919, 929, 940, 950, 959, 969, 984, 998, 1013, 1027,
    1042, 1056, 1071, 1085, 1100, 1114, 1129, 1143, 1158, 1172, 1187, 1201, 1215,
    1230, 1244, 1259, 1273, 1288, 1302, 1317, 1331, 1346, 1360, 1375, 1389, 1404,
    1418, 1433, 1447, 1462, 1476, 1491, 1505, 1519, 1534, 1548, 1563, 1577, 1592,
    1606, 1621, 1635, 1650, 1664, 1679, 1693, 1708, 1722, 1737, 1751, 1766, 1780, 1795, 1809, 1823, 1838
};

static sample_vo_cfg g_vo_cfg = {
    .vo_dev            = SAMPLE_VO_DEV_UHD,
    .vo_intf_type      = OT_VO_INTF_HDMI,
    .intf_sync         = OT_VO_OUT_1080P60,
    .bg_color          = COLOR_RGB_BLACK,
    .pix_format        = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    .disp_rect         = {0, 0, 1920, 1080},
    .image_size        = {1920, 1080},
    .vo_part_mode      = OT_VO_PARTITION_MODE_SINGLE,
    .dis_buf_len       = 3, /* 3: def buf len for single */
    .dst_dynamic_range = OT_DYNAMIC_RANGE_SDR8,
    .vo_mode           = VO_MODE_1MUX,
    .compress_mode     = OT_COMPRESS_MODE_NONE,
};

static sample_comm_venc_chn_param g_venc_chn_param = {
    .frame_rate           = 30, /* 30 is a number */
    .stats_time           = 1,  /* 1 is a number */
    .gop                  = 30, /* 30 is a number */
    .venc_size            = {1920, 1080},
    .size                 = PIC_1080P,
    .profile              = 0,
    .is_rcn_ref_share_buf = TD_FALSE,
    .gop_attr             = {
        .gop_mode = OT_VENC_GOP_MODE_NORMAL_P,
        .normal_p = {2},
    },
    .type                 = OT_PT_H265,
    .rc_mode              = SAMPLE_RC_VBR,
};

static sample_vi_fpn_calibration_cfg g_calibration_cfg = {
    .threshold     = 4095, /* 4095 is a number */
    .frame_num     = 16,   /* 16 is a number */
    .fpn_type      = OT_ISP_FPN_TYPE_FRAME,
    .pixel_format  = OT_PIXEL_FORMAT_RGB_BAYER_16BPP,
    .compress_mode = OT_COMPRESS_MODE_NONE,
};

static sample_vi_fpn_correction_cfg g_correction_cfg = {
    .op_mode       = OT_OP_MODE_AUTO,
    .fpn_type      = OT_ISP_FPN_TYPE_FRAME,
    .strength      = 0,
    .pixel_format  = OT_PIXEL_FORMAT_RGB_BAYER_16BPP,
    .compress_mode = OT_COMPRESS_MODE_NONE,
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

static td_void sample_get_char(td_void)
{
    if (g_sig_flag == 1) {
        return;
    }

    sample_pause();
}

static td_void sample_vi_get_default_vb_config(ot_size *size, ot_vb_cfg *vb_cfg, ot_vi_video_mode video_mode,
    td_u32 yuv_cnt, td_u32 raw_cnt)
{
    ot_vb_calc_cfg calc_cfg;
    ot_pic_buf_attr buf_attr;

    (td_void)memset_s(vb_cfg, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg));
    vb_cfg->max_pool_cnt = 128; /* 128 blks */

    /* default YUV pool: SP420 + compress_seg */
    buf_attr.width         = size->width;
    buf_attr.height        = size->height;
    buf_attr.align         = OT_DEFAULT_ALIGN;
    buf_attr.bit_width     = OT_DATA_BIT_WIDTH_8;
    buf_attr.pixel_format  = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    buf_attr.compress_mode = OT_COMPRESS_MODE_SEG;
    ot_common_get_pic_buf_cfg(&buf_attr, &calc_cfg);

    vb_cfg->common_pool[0].blk_size = calc_cfg.vb_size;
    vb_cfg->common_pool[0].blk_cnt  = yuv_cnt;

    /* default raw pool: raw12bpp + compress_line */
    buf_attr.pixel_format  = OT_PIXEL_FORMAT_RGB_BAYER_12BPP;
    buf_attr.compress_mode = (video_mode == OT_VI_VIDEO_MODE_NORM ? OT_COMPRESS_MODE_LINE : OT_COMPRESS_MODE_NONE);
    ot_common_get_pic_buf_cfg(&buf_attr, &calc_cfg);
    vb_cfg->common_pool[1].blk_size = calc_cfg.vb_size;
    vb_cfg->common_pool[1].blk_cnt  = raw_cnt;
}

/* define SAMPLE_MEM_SHARE_ENABLE, when use tools to dump YUV/RAW. */
#ifdef SAMPLE_MEM_SHARE_ENABLE
td_void sample_vio_init_mem_share(td_void)
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
}
#endif

static td_s32 sample_vio_sys_init(ot_vi_vpss_mode_type mode_type, ot_vi_video_mode video_mode,
    td_u32 yuv_cnt, td_u32 raw_cnt)
{
    td_s32 ret;
    ot_size size;
    ot_vb_cfg vb_cfg;
    td_u32 supplement_config;
    sample_sns_type sns_type = SENSOR0_TYPE;

    sample_comm_vi_get_size_by_sns_type(sns_type, &size);
    sample_vi_get_default_vb_config(&size, &vb_cfg, video_mode, yuv_cnt, raw_cnt);

    supplement_config = OT_VB_SUPPLEMENT_BNR_MOT_MASK;
    ret = sample_comm_sys_init_with_vb_supplement(&vb_cfg, supplement_config);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

#ifdef SAMPLE_MEM_SHARE_ENABLE
    sample_vio_init_mem_share();
#endif

    ret = sample_comm_vi_set_vi_vpss_mode(mode_type, video_mode);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_vio_start_vpss(ot_vpss_grp grp, ot_size *in_size)
{
    td_s32 ret;
    ot_low_delay_info low_delay_info;
    ot_vpss_grp_attr grp_attr;
    ot_vpss_chn_attr chn_attr;
    td_bool chn_enable[OT_VPSS_MAX_PHYS_CHN_NUM] = {TD_TRUE, TD_FALSE, TD_FALSE, TD_FALSE};

    sample_comm_vpss_get_default_grp_attr(&grp_attr);
    grp_attr.max_width  = in_size->width;
    grp_attr.max_height = in_size->height;
    sample_comm_vpss_get_default_chn_attr(&chn_attr);
    chn_attr.width  = in_size->width;
    chn_attr.height = in_size->height;

    ret = sample_common_vpss_start(grp, chn_enable, &grp_attr, &chn_attr, OT_VPSS_MAX_PHYS_CHN_NUM);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    low_delay_info.enable     = TD_TRUE;
    low_delay_info.line_cnt   = 200; /* 200: lowdelay line */
    low_delay_info.one_buf_en = TD_FALSE;
    ret = ss_mpi_vpss_set_low_delay_attr(grp, 0, &low_delay_info);
    if (ret != TD_SUCCESS) {
        sample_common_vpss_stop(grp, chn_enable, OT_VPSS_MAX_PHYS_CHN_NUM);
        return ret;
    }

    return TD_SUCCESS;
}

static td_void sample_vio_stop_vpss(ot_vpss_grp grp)
{
    td_bool chn_enable[OT_VPSS_MAX_PHYS_CHN_NUM] = {TD_TRUE, TD_FALSE, TD_FALSE, TD_FALSE};

    sample_common_vpss_stop(grp, chn_enable, OT_VPSS_MAX_PHYS_CHN_NUM);
}

static td_s32 sample_vio_start_venc(ot_venc_chn venc_chn[], td_u32 chn_num, const ot_size *in_size)
{
    td_s32 i;
    td_s32 ret;

    g_venc_chn_param.venc_size.width  = in_size->width;
    g_venc_chn_param.venc_size.height = in_size->height;
    g_venc_chn_param.size = sample_comm_sys_get_pic_enum(in_size);

    for (i = 0; i < (td_s32)chn_num; i++) {
        ret = sample_comm_venc_start(venc_chn[i], &g_venc_chn_param);
        if (ret != TD_SUCCESS) {
            goto exit;
        }
    }

    ret = sample_comm_venc_start_get_stream(venc_chn, chn_num);
    if (ret != TD_SUCCESS) {
        goto exit;
    }

    return TD_SUCCESS;

exit:
    for (i = i - 1; i >= 0; i--) {
        sample_comm_venc_stop(venc_chn[i]);
    }
    return TD_FAILURE;
}

static td_void sample_vio_stop_venc(ot_venc_chn venc_chn[], td_u32 chn_num)
{
    td_u32 i;

    sample_comm_venc_stop_get_stream(chn_num);

    for (i = 0; i < chn_num; i++) {
        sample_comm_venc_stop(venc_chn[i]);
    }
}

static td_s32 sample_vio_start_vo(sample_vo_mode vo_mode)
{
    g_vo_cfg.vo_mode = vo_mode;

    return sample_comm_vo_start_vo(&g_vo_cfg);
}

static td_s32 sample_vio_start_vo_mipitx(const sample_vo_mipi_tx_cfg *vo_tx_cfg)
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


static td_void sample_vio_stop_vo(td_void)
{
    sample_comm_vo_stop_vo(&g_vo_cfg);
}

static td_s32 sample_vio_start_venc_and_vo(ot_vpss_grp vpss_grp[], td_u32 grp_num, const ot_size *in_size)
{
    td_u32 i;
    td_s32 ret;
    sample_vo_mode vo_mode = VO_MODE_1MUX;
    const ot_vpss_chn vpss_chn = 0;
    const ot_vo_layer vo_layer = 0;
    ot_vo_chn vo_chn[4] = {0, 1, 2, 3};     /* 4: max chn num, 0/1/2/3 chn id */
    ot_venc_chn venc_chn[4] = {0, 1, 2, 3}; /* 4: max chn num, 0/1/2/3 chn id */

    if (grp_num > 1) {
        vo_mode = VO_MODE_4MUX;
    }

    ret = sample_vio_start_vo(vo_mode);
    if (ret != TD_SUCCESS) {
        goto start_vo_failed;
    }

    ret = sample_vio_start_venc(venc_chn, grp_num, in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_failed;
    }

    for (i = 0; i < grp_num; i++) {
        sample_comm_vpss_bind_vo(vpss_grp[i], vpss_chn, vo_layer, vo_chn[i]);
        sample_comm_vpss_bind_venc(vpss_grp[i], vpss_chn, venc_chn[i]);
    }
    return TD_SUCCESS;

start_venc_failed:
    sample_vio_stop_vo();
start_vo_failed:
    return TD_FAILURE;
}

static td_s32 sample_vio_start_venc_and_vo_mipitx(ot_vpss_grp vpss_grp[], td_u32 grp_num, const ot_size *in_size)
{
    td_u32 i;
    td_s32 ret;
    sample_vo_mode vo_mode = VO_MODE_1MUX;
    const ot_vpss_chn vpss_chn = 0;
    const ot_vo_layer vo_layer = 0;
    ot_vo_chn vo_chn[4] = {0, 1, 2, 3};     /* 4: max chn num, 0/1/2/3 chn id */
    ot_venc_chn venc_chn[4] = {0, 1, 2, 3}; /* 4: max chn num, 0/1/2/3 chn id */

    if (grp_num > 1) {
        vo_mode = VO_MODE_4MUX;
    }

    ret = sample_vio_start_vo_mipitx(&g_vo_tx_cfg_800x1280_user);
    if (ret != TD_SUCCESS) {
        goto start_vo_failed;
    }

    ret = sample_vio_start_venc(venc_chn, grp_num, in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_failed;
    }

    for (i = 0; i < grp_num; i++) {
        sample_comm_vpss_bind_vo(vpss_grp[i], vpss_chn, vo_layer, vo_chn[i]);
        sample_comm_vpss_bind_venc(vpss_grp[i], vpss_chn, venc_chn[i]);
    }
    return TD_SUCCESS;

start_venc_failed:
    sample_vio_stop_vo();
start_vo_failed:
    return TD_FAILURE;
}

static td_void sample_vio_stop_venc_and_vo(ot_vpss_grp vpss_grp[], td_u32 grp_num)
{
    td_u32 i;
    const ot_vpss_chn vpss_chn = 0;
    const ot_vo_layer vo_layer = 0;
    ot_vo_chn vo_chn[4] = {0, 1, 2, 3};     /* 4: max chn num, 0/1/2/3 chn id */
    ot_venc_chn venc_chn[4] = {0, 1, 2, 3}; /* 4: max chn num, 0/1/2/3 chn id */

    for (i = 0; i < grp_num; i++) {
        sample_comm_vpss_un_bind_vo(vpss_grp[i], vpss_chn, vo_layer, vo_chn[i]);
        sample_comm_vpss_un_bind_venc(vpss_grp[i], vpss_chn, venc_chn[i]);
    }

    sample_vio_stop_venc(venc_chn, grp_num);
    sample_vio_stop_vo();
}

static td_void sample_vio_stop_venc_and_vo_mipitx(ot_vpss_grp vpss_grp[], td_u32 grp_num)
{
    td_u32 i;
    const ot_vpss_chn vpss_chn = 0;
    const ot_vo_layer vo_layer = 0;
    ot_vo_chn vo_chn[4] = {0, 1, 2, 3};     /* 4: max chn num, 0/1/2/3 chn id */
    ot_venc_chn venc_chn[4] = {0, 1, 2, 3}; /* 4: max chn num, 0/1/2/3 chn id */

    for (i = 0; i < grp_num; i++) {
        sample_comm_vpss_un_bind_vo(vpss_grp[i], vpss_chn, vo_layer, vo_chn[i]);
        sample_comm_vpss_un_bind_venc(vpss_grp[i], vpss_chn, venc_chn[i]);
    }

    sample_vio_stop_venc(venc_chn, grp_num);
    sample_comm_vo_stop_vo(&g_vo_tx_cfg_800x1280_user.vo_config);
}

static td_void sample_vio_print_vi_mode_list(td_bool is_wdr_mode)
{
    printf("vi vpss mode list: \n");
    printf("    (0) VI_ONLINE_VPSS_ONLINE,   Normal mode\n");
    printf("    (1) VI_ONLINE_VPSS_OFFLINE,  Normal mode\n");
    printf("    (2) VI_OFFLINE_VPSS_ONLINE,  Normal mode\n");
    printf("    (3) VI_OFFLINE_VPSS_OFFLINE, Normal mode\n");
    if (is_wdr_mode == TD_FALSE) {
        printf("    (4) VI_ONLINE_VPSS_ONLINE,   Advanced mode\n");
        printf("    (5) VI_ONLINE_VPSS_OFFLINE,  Advanced mode\n");
        printf("    (6) VI_OFFLINE_VPSS_ONLINE,  Advanced mode\n");
        printf("    (7) VI_OFFLINE_VPSS_OFFLINE, Advanced mode\n");
    }

    printf("please select mode: ");
}

static td_void sample_vio_get_vb_blk_num_by_char(td_char ch, td_u32 *yuv_cnt, td_u32 *raw_cnt, td_bool is_wdr)
{
    *yuv_cnt  = VB_YUV_ROUTE_CNT;
    switch (ch) {
        case '0':
        case '1':
            *raw_cnt = VB_RAW_CNT_NONE;
            break;
        case '2':
        case '3':
            *raw_cnt = (is_wdr ? VB_WDR_RAW_CNT : VB_LINEAR_RAW_CNT);
            break;
        case '4':
        case '5':
            *raw_cnt = VB_LINEAR_RAW_CNT;
            break;
        case '6':
        case '7':
            *raw_cnt = VB_WDR_RAW_CNT;
            break;
        default:
            *raw_cnt = VB_WDR_RAW_CNT;
            break;
    }
}

static td_void sample_vio_get_vi_vpss_mode_by_char(td_char ch,
    ot_vi_vpss_mode_type *mode_type, ot_vi_video_mode *video_mode)
{
    switch (ch) {
        case '0':
            *mode_type  = OT_VI_ONLINE_VPSS_ONLINE;
            *video_mode = OT_VI_VIDEO_MODE_NORM;
            break;
        case '1':
            *mode_type  = OT_VI_ONLINE_VPSS_OFFLINE;
            *video_mode = OT_VI_VIDEO_MODE_NORM;
            break;
        case '2':
            *mode_type  = OT_VI_OFFLINE_VPSS_ONLINE;
            *video_mode = OT_VI_VIDEO_MODE_NORM;
            break;
        case '3':
            *mode_type  = OT_VI_OFFLINE_VPSS_OFFLINE;
            *video_mode = OT_VI_VIDEO_MODE_NORM;
            break;
        case '4':
            *mode_type  = OT_VI_ONLINE_VPSS_ONLINE;
            *video_mode = OT_VI_VIDEO_MODE_ADVANCED;
            break;
        case '5':
            *mode_type  = OT_VI_ONLINE_VPSS_OFFLINE;
            *video_mode = OT_VI_VIDEO_MODE_ADVANCED;
            break;
        case '6':
            *mode_type  = OT_VI_OFFLINE_VPSS_ONLINE;
            *video_mode = OT_VI_VIDEO_MODE_ADVANCED;
            break;
        case '7':
            *mode_type  = OT_VI_OFFLINE_VPSS_OFFLINE;
            *video_mode = OT_VI_VIDEO_MODE_ADVANCED;
            break;
        default:
            *mode_type  = OT_VI_ONLINE_VPSS_ONLINE;
            *video_mode = OT_VI_VIDEO_MODE_NORM;
            break;
    }
}
static td_void sample_vio_get_vi_vpss_mode(ot_vi_vpss_mode_type *mode_type, ot_vi_video_mode *video_mode,
    td_u32 *yuv_cnt, td_u32 *raw_cnt, td_bool is_wdr_mode)
{
    td_char ch = '0';
    td_char end_ch;

    if (is_wdr_mode == TD_TRUE) {
        end_ch = '3';
    } else {
        end_ch = '7';
    }

    sample_vio_print_vi_mode_list(is_wdr_mode);

    while (((ch = getchar()) != EOF) && (g_sig_flag == 0)) {
        if ((ch >= '0') && (ch <= end_ch)) {
            getchar();
            break;
        } else {
            printf("\nInvalid param, please enter again!\n\n");
            sample_vio_print_vi_mode_list(is_wdr_mode);
        }
    }

    sample_vio_get_vi_vpss_mode_by_char(ch, mode_type, video_mode);
    sample_vio_get_vb_blk_num_by_char(ch, yuv_cnt, raw_cnt, is_wdr_mode);
}

static td_void sample_vi_get_two_sensor_vi_cfg(sample_sns_type sns_type, sample_vi_cfg *vi_cfg0, sample_vi_cfg *vi_cfg1)
{
    const ot_vi_dev vi_dev = 2; /* dev2 for sensor1 */
    const ot_vi_pipe vi_pipe = 1; /* dev2 bind pipe1 */

    sample_comm_vi_get_default_vi_cfg(sns_type, vi_cfg0);
    sample_comm_vi_get_default_vi_cfg(sns_type, vi_cfg1);

    vi_cfg0->mipi_info.divide_mode = LANE_DIVIDE_MODE_1;

    vi_cfg1->sns_info.bus_id = 5; /* i2c5 */
    vi_cfg1->sns_info.sns_clk_src = 1;
    vi_cfg1->sns_info.sns_rst_src = 1;

    sample_comm_vi_get_mipi_info_by_dev_id(sns_type, vi_dev, &vi_cfg1->mipi_info);
    vi_cfg1->dev_info.vi_dev = vi_dev;
    vi_cfg1->bind_pipe.pipe_id[0] = vi_pipe;
    vi_cfg1->grp_info.grp_num = 1;
    vi_cfg1->grp_info.fusion_grp[0] = 1;
    vi_cfg1->grp_info.fusion_grp_attr[0].pipe_id[0] = vi_pipe;
}

static td_s32 sample_vio_all_mode(td_void)
{
    td_s32 ret;
    td_u32 yuv_cnt;
    td_u32 raw_cnt;
    ot_vi_vpss_mode_type mode_type = OT_VI_ONLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type;
    ot_size in_size;

    sample_vio_get_vi_vpss_mode(&mode_type, &video_mode, &yuv_cnt, &raw_cnt, TD_FALSE);
    ret = sample_vio_sys_init(mode_type, video_mode, yuv_cnt, raw_cnt);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    sns_type = SENSOR0_TYPE;
    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_get_char();

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);

start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_s32 sample_vio_all_mode_mipitx(td_void)
{
    td_s32 ret;
    td_u32 yuv_cnt;
    td_u32 raw_cnt;
    ot_vi_vpss_mode_type mode_type = OT_VI_ONLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type;
    ot_size in_size;

    sample_vio_get_vi_vpss_mode(&mode_type, &video_mode, &yuv_cnt, &raw_cnt, TD_FALSE);
    ret = sample_vio_sys_init(mode_type, video_mode, yuv_cnt, raw_cnt);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    sns_type = SENSOR0_TYPE;
    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_vio_start_venc_and_vo_mipitx(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_get_char();

    sample_vio_stop_venc_and_vo_mipitx(vpss_grp, grp_num);

start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_s32 sample_vio_switch_first_route(sample_sns_type sns_type)
{
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg;
    ot_size in_size;
    td_s32 ret;

    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }
    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }
    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }
    sample_get_char();

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);
start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    if (ret == TD_SUCCESS) {
        sample_comm_vi_mode_switch_stop_vi(&vi_cfg);
    } else {
        sample_comm_vi_stop_vi(&vi_cfg);
    }
start_vi_failed:
    return ret;
}

static td_s32 sample_vio_switch_second_route(sample_sns_type sns_type)
{
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    ot_size in_size;
    sample_vi_cfg vi_cfg;
    td_s32 ret;

    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    ret = sample_comm_vi_mode_switch_start_vi(&vi_cfg, TD_FALSE, &in_size);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_get_char();

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);
start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);

    return ret;
}

static td_s32 sample_vio_switch_resolution_route(sample_sns_type sns_type)
{
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    ot_size in_size;
    sample_vi_cfg vi_cfg;
    td_s32 ret;

    in_size.width = 1920;  // 1920: target width
    in_size.height = 1080; // 1080: target height
    sample_comm_vi_init_vi_cfg(sns_type, &in_size, &vi_cfg);

    ret = sample_comm_vi_mode_switch_start_vi(&vi_cfg, TD_TRUE, &in_size);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_get_char();

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);
start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);

    return ret;
}

static td_s32 sample_vio_switch_mode(td_void)
{
    td_s32 ret;
    td_u32 yuv_cnt;
    td_u32 raw_cnt;
    ot_vi_vpss_mode_type mode_type = OT_VI_ONLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    sample_sns_type sns_type = SENSOR0_TYPE;

    sample_vio_get_vi_vpss_mode(&mode_type, &video_mode, &yuv_cnt, &raw_cnt, TD_TRUE);
    ret = sample_vio_sys_init(mode_type, video_mode, yuv_cnt, raw_cnt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = sample_vio_switch_first_route(sns_type);
    if (ret != TD_SUCCESS) {
        sample_comm_sys_exit();
        return ret;
    }

    if (sns_type == HY_S0603_MIPI_8M_30FPS_12BIT) {
        sns_type = HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1;
    } else {
        sns_type = HY_S0603_MIPI_8M_30FPS_12BIT;
    }

    ret = sample_vio_switch_second_route(sns_type);
    sample_comm_sys_exit();
    return ret;
}

static td_s32 sample_vio_switch_resolution(td_void)
{
    td_s32 ret;
    td_u32 yuv_cnt;
    td_u32 raw_cnt;
    ot_vi_vpss_mode_type mode_type = OT_VI_ONLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    sample_sns_type sns_type = SENSOR0_TYPE;

    sample_vio_get_vi_vpss_mode(&mode_type, &video_mode, &yuv_cnt, &raw_cnt, TD_TRUE);
    ret = sample_vio_sys_init(mode_type, video_mode, yuv_cnt, raw_cnt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = sample_vio_switch_first_route(sns_type);
    if (ret != TD_SUCCESS) {
        sample_comm_sys_exit();
        return ret;
    }

    ret = sample_vio_switch_resolution_route(sns_type);
    sample_comm_sys_exit();
    return ret;
}

static td_s32 sample_vio_wdr(td_void)
{
    td_s32 ret;
    td_u32 yuv_cnt;
    td_u32 raw_cnt;
    ot_vi_vpss_mode_type mode_type = OT_VI_ONLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type = SENSOR0_TYPE;
    ot_size in_size;

    sample_vio_get_vi_vpss_mode(&mode_type, &video_mode, &yuv_cnt, &raw_cnt, TD_TRUE);
    ret = sample_vio_sys_init(mode_type, video_mode, yuv_cnt, raw_cnt);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    if (sns_type == HY_S0603_MIPI_8M_30FPS_12BIT) {
        sns_type = HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1;
    }
    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_get_char();

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);

start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_s32 sample_vio_wdr_send_frame(td_void)
{
    td_s32 ret;
    ot_vi_vpss_mode_type mode_type = OT_VI_OFFLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type;
    ot_size in_size;

    ret = sample_vio_sys_init(mode_type, video_mode, VB_YUV_ROUTE_CNT, VB_WDR_RAW_CNT);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    sns_type = SENSOR0_TYPE;
    if (sns_type == HY_S0603_MIPI_8M_30FPS_12BIT) {
        sns_type = HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1;
    }
    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    vi_cfg.pipe_info[0].isp_need_run = TD_FALSE;
    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    ret = sample_comm_vi_send_wdr_frame(&vi_cfg.bind_pipe);

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);

start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_void sample_vio_restart_get_venc_stream(ot_venc_chn venc_chn[], td_u32 chn_num)
{
    td_u32 i;
    td_s32 ret;
    ot_venc_start_param start_param;

    for (i = 0; i < chn_num; i++) {
        start_param.recv_pic_num = -1;
        if ((ret = ss_mpi_venc_start_chn(venc_chn[i], &start_param)) != TD_SUCCESS) {
            sample_print("ss_mpi_venc_start_recv_pic failed with%#x! \n", ret);
            return;
        }
    }

    sample_comm_venc_start_get_stream(venc_chn, chn_num);
}

static td_void sample_vio_do_fpn_calibrate_and_correction(ot_vi_pipe vi_pipe)
{
    ot_venc_chn venc_chn[1] = {0};
    const td_u32 chn_num = 1;

    sample_comm_venc_stop_get_stream(chn_num);
    sample_comm_vi_fpn_calibrate(vi_pipe, &g_calibration_cfg);

    printf("please enter any key to enable fpn correction!\n");
    sample_get_char();

    sample_vio_restart_get_venc_stream(venc_chn, chn_num);
    sample_comm_vi_enable_fpn_correction(vi_pipe, &g_correction_cfg);
}

static td_s32 sample_vio_fpn(td_void)
{
    td_s32 ret;
    ot_vi_vpss_mode_type mode_type = OT_VI_OFFLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type;
    ot_size in_size;

    ret = sample_vio_sys_init(mode_type, video_mode, VB_YUV_ROUTE_CNT, VB_WDR_RAW_CNT);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    sns_type = SENSOR0_TYPE;
    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_vio_do_fpn_calibrate_and_correction(vi_pipe);

    sample_get_char();

    sample_comm_vi_disable_fpn_correction(vi_pipe, &g_correction_cfg);

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);

start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_void sample_vio_set_dis_en(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, td_bool enable)
{
    td_s32 ret;
    ot_dis_cfg dis_cfg = {0};
    ot_dis_attr dis_attr = {0};

    dis_cfg.motion_level  = OT_DIS_MOTION_LEVEL_NORM;
    dis_cfg.crop_ratio    = 80; /* 80 sample crop  ratio */
    dis_cfg.buf_num       = 10; /* 10 sample buf   num   */
    dis_cfg.frame_rate    = 30; /* 30 sample frame rate  */
    dis_cfg.camera_steady = TD_FALSE;
    dis_cfg.scale         = TD_TRUE;
    dis_cfg.pdt_type      = OT_DIS_PDT_TYPE_IPC;
    dis_cfg.mode          = OT_DIS_MODE_6_DOF_GME;
    ret = ss_mpi_vi_set_chn_dis_cfg(vi_pipe, vi_chn, &dis_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("set dis config failed.ret:0x%x !\n", ret);
    }

    dis_attr.enable               = enable;
    dis_attr.moving_subject_level = 0;
    dis_attr.rolling_shutter_coef = 0;
    dis_attr.timelag              = 1000;     /* 1000: timelag */
    dis_attr.still_crop           = TD_FALSE;
    dis_attr.hor_limit            = 512;      /* 512  sample hor_limit */
    dis_attr.ver_limit            = 512;      /* 512  sample ver_limit */
    dis_attr.gdc_bypass           = TD_FALSE;
    dis_attr.strength             = 1024;     /* 1024 sample strength  */
    ret = ss_mpi_vi_set_chn_dis_attr(vi_pipe, vi_chn, &dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
    }
}

static td_void sample_vio_set_ldc_en(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, td_bool enable)
{
    td_s32 ret;
    ot_ldc_attr ldc_attr;

    ldc_attr.enable                       = enable;
    ldc_attr.ldc_version                  = OT_LDC_V1;
    ldc_attr.ldc_v1_attr.aspect           = 0;
    ldc_attr.ldc_v1_attr.x_ratio          = 100; /* 100: x ratio */
    ldc_attr.ldc_v1_attr.y_ratio          = 100; /* 100: y ratio */
    ldc_attr.ldc_v1_attr.xy_ratio         = 100; /* 100: x y ratio */
    ldc_attr.ldc_v1_attr.center_x_offset  = 0;
    ldc_attr.ldc_v1_attr.center_y_offset  = 0;
    ldc_attr.ldc_v1_attr.distortion_ratio = 500; /* 500: distortion ratio */

    ret = ss_mpi_vi_set_chn_ldc_attr(vi_pipe, vi_chn, &ldc_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set ldc attr failed.ret:0x%x !\n", ret);
    }
}

static td_void sample_vio_switch_ldc_dis_en(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn)
{
    printf("please enter any key to enable ldc && dis!\n");
    sample_get_char();

    sample_vio_set_dis_en(vi_pipe, vi_chn, TD_TRUE);
    sample_vio_set_ldc_en(vi_pipe, vi_chn, TD_TRUE);

    printf("please enter any key to disable ldc && dis!\n");
    sample_get_char();

    sample_vio_set_ldc_en(vi_pipe, vi_chn, TD_FALSE);
    sample_vio_set_dis_en(vi_pipe, vi_chn, TD_FALSE);
}

static td_s32 sample_vio_ldc_dis(td_void)
{
    td_s32 ret;
    ot_vi_vpss_mode_type mode_type = OT_VI_ONLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type;
    ot_size in_size;

    ret = sample_vio_sys_init(mode_type, video_mode, VB_YUV_ROUTE_CNT, VB_LINEAR_RAW_CNT);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    sns_type = SENSOR0_TYPE;
    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_vio_switch_ldc_dis_en(vi_pipe, vi_chn);
    sample_get_char();

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);

start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_void sample_vio_get_fisheye_correction_attr(ot_fisheye_correction_attr *correction_attr, ot_size *out_size)
{
    correction_attr->enable                      = TD_TRUE;
    correction_attr->dst_size.width              = out_size->width;
    correction_attr->dst_size.height             = out_size->height;
    correction_attr->fisheye_attr.lmf_en         = TD_TRUE;
    correction_attr->fisheye_attr.bg_color_en    = TD_TRUE;
    correction_attr->fisheye_attr.bg_color       = COLOR_RGB_BLUE;
    correction_attr->fisheye_attr.hor_offset     = 0;
    correction_attr->fisheye_attr.ver_offset     = 0;
    correction_attr->fisheye_attr.trapezoid_coef = 0;
    correction_attr->fisheye_attr.fan_strength   = 0;
    correction_attr->fisheye_attr.mount_mode     = OT_FISHEYE_MOUNT_MODE_CEILING;
    correction_attr->fisheye_attr.rgn_num        = 2; /* 2: rgn_num */

    correction_attr->fisheye_attr.fisheye_rgn_attr[0].view_mode       = OT_FISHEYE_VIEW_MODE_360_PANORAMA;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].in_radius       = 0;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_radius      = 1200; /* 1200: out_radius */
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].pan             = 180;  /* 180: pan */
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].tilt            = 180;  /* 180: tilt */
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].hor_zoom        = 4095; /* 4095: hor_zoom */
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].ver_zoom        = 4095; /* 4095: ver_zoom */
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.x      = 0;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.y      = 0;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.width  = out_size->width;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.height
        = OT_ALIGN_DOWN(out_ratio_1(out_size->height), Y_ALIGN);

    correction_attr->fisheye_attr.fisheye_rgn_attr[1].view_mode       = OT_FISHEYE_VIEW_MODE_360_PANORAMA;
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].in_radius       = 0;
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_radius      = 1200; /* 1200: out_radius */
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].pan             = 180;  /* 180: pan */
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].tilt            = 180;  /* 180: tilt */
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].hor_zoom        = 4095; /* 4095: hor_zoom */
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].ver_zoom        = 4095; /* 4095: ver_zoom */
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.x      = 0;
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.y      =
        OT_ALIGN_DOWN(out_ratio_1(out_size->height), Y_ALIGN);
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.width  = out_size->width;
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.height =
        OT_ALIGN_DOWN(out_ratio_1(out_size->height), Y_ALIGN);
}

static td_void sample_vio_set_fisheye_param(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_size *out_size)
{
    td_s32 ret;
    ot_fisheye_correction_attr correction_attr;
    ot_fisheye_cfg fisheye_cfg;

    (td_void)memcpy_s(fisheye_cfg.lmf_coef, sizeof(g_lmf_coef), g_lmf_coef, sizeof(g_lmf_coef));
    ret = ss_mpi_vi_set_pipe_fisheye_cfg(vi_pipe, &fisheye_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("set fisheye cfg failed.ret:0x%x !\n", ret);
    }

    sample_vio_get_fisheye_correction_attr(&correction_attr, out_size);
    ret = ss_mpi_vi_set_chn_fisheye(vi_pipe, vi_chn, &correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set chn fisheye failed.ret:0x%x !\n", ret);
    }
}

static td_s32 sample_vio_start_vi_ext_chn(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vi_chn vi_ext_chn)
{
    td_s32 ret;
    ot_vi_ext_chn_attr ext_chn_attr;
    ot_size out_size;

    sample_comm_vi_get_size_by_sns_type(SENSOR0_TYPE, &out_size);

    ext_chn_attr.bind_chn                       = vi_chn;
    ext_chn_attr.src_type                       = OT_EXT_CHN_SRC_TYPE_BEFORE_FISHEYE;
    ext_chn_attr.size.width                     = out_size.width;
    ext_chn_attr.size.height                    = out_size.height;
    ext_chn_attr.pixel_format                   = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    ext_chn_attr.video_format                   = OT_VIDEO_FORMAT_LINEAR;
    ext_chn_attr.dynamic_range                  = OT_DYNAMIC_RANGE_SDR8;
    ext_chn_attr.compress_mode                  = OT_COMPRESS_MODE_NONE;
    ext_chn_attr.depth                          = 0;
    ext_chn_attr.frame_rate_ctrl.src_frame_rate = -1;
    ext_chn_attr.frame_rate_ctrl.dst_frame_rate = -1;
    ret = ss_mpi_vi_set_ext_chn_attr(vi_pipe, vi_ext_chn, &ext_chn_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set ext chn failed, ret: 0x%x\n", ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_vi_enable_chn(vi_pipe, vi_ext_chn);
    if (ret != TD_SUCCESS) {
        sample_print("enable ext chn failed, ret: 0x%x\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void sample_vio_stop_vi_ext_chn(ot_vi_pipe vi_pipe, ot_vi_chn vi_ext_chn)
{
    td_s32 ret;

    ret = ss_mpi_vi_disable_chn(vi_pipe, vi_ext_chn);
    if (ret != TD_SUCCESS) {
        sample_print("disable ext chn failed, ret: 0x%x\n", ret);
    }
}

static td_s32 sample_vio_start_vi_vpss_double_chn_route(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, sample_vi_cfg *vi_cfg,
                                                        ot_vpss_grp vpss_grp[], td_u32 grp_num)
{
    td_s32 ret;
    sample_sns_type sns_type;
    ot_size in_size;
    const ot_vi_chn vi_ext_chn = 4;
    const ot_vpss_chn vpss_chn = 0;

    sns_type = SENSOR0_TYPE;
    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, vi_cfg);
    ret = sample_comm_vi_start_vi(vi_cfg);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }

    ret = sample_vio_start_vi_ext_chn(vi_pipe, vi_chn, vi_ext_chn);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }

    ot_unused(grp_num);

    sample_comm_vi_bind_vpss(vi_pipe, vi_ext_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss0_failed;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[1], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[1], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss1_failed;
    }

    return TD_SUCCESS;

start_vpss1_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[1], vpss_chn);
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss0_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_ext_chn, vpss_grp[0], vpss_chn);
    sample_vio_stop_vi_ext_chn(vi_pipe, vi_ext_chn);
    sample_comm_vi_stop_vi(vi_cfg);
start_vi_failed:
    return ret;
}

static td_void sample_vio_stop_vi_vpss_double_chn_route(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, sample_vi_cfg *vi_cfg,
                                                        ot_vpss_grp vpss_grp[], td_u32 grp_num)
{
    const ot_vi_chn vi_ext_chn = 4;
    const ot_vpss_chn vpss_chn = 0;

    ot_unused(grp_num);

    sample_vio_stop_vpss(vpss_grp[1]);
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[1], vpss_chn);
    sample_vio_stop_vpss(vpss_grp[0]);
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_ext_chn, vpss_grp[0], vpss_chn);
    sample_vio_stop_vi_ext_chn(vi_pipe, vi_ext_chn);
    sample_comm_vi_stop_vi(vi_cfg);
}

static td_s32 sample_vio_fisheye(td_void)
{
    td_s32 ret;
    ot_vi_vpss_mode_type mode_type = OT_VI_ONLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[2] = {0, 1};
    const td_u32 grp_num = 2;
    sample_vi_cfg vi_cfg;
    ot_size out_size;

    ret = sample_vio_sys_init(mode_type, video_mode, VB_MULTI_YUV_CNT, VB_RAW_CNT_NONE);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    ret = sample_vio_start_vi_vpss_double_chn_route(vi_pipe, vi_chn, &vi_cfg, vpss_grp, grp_num);
    if (ret != TD_SUCCESS) {
        goto start_vi_vpss_failed;
    }

    sample_comm_vi_get_size_by_sns_type(SENSOR0_TYPE, &out_size);
    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &out_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_vio_set_fisheye_param(vi_pipe, vi_chn, &out_size);
    sample_get_char();

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);

start_venc_and_vo_failed:
    sample_vio_stop_vi_vpss_double_chn_route(vi_pipe, vi_chn, &vi_cfg, vpss_grp, grp_num);
start_vi_vpss_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_void sample_vio_switch_low_delay(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn)
{
    td_s32 ret;
    ot_low_delay_info low_delay_info;

    low_delay_info.enable = TD_TRUE;
    low_delay_info.line_cnt = 300; /* 300: low delay line cnt */
    low_delay_info.one_buf_en = TD_FALSE;

    printf("please enter any key to enable pipe low delay!\n");
    sample_get_char();

    ret = ss_mpi_vi_set_pipe_low_delay_attr(vi_pipe, &low_delay_info);
    if (ret != TD_SUCCESS) {
        sample_print("enable pipe low delay failed!\n");
    }

    printf("please enter any key to disable pipe low delay!\n");
    sample_get_char();

    low_delay_info.enable = TD_FALSE;
    ret = ss_mpi_vi_set_pipe_low_delay_attr(vi_pipe, &low_delay_info);
    if (ret != TD_SUCCESS) {
        sample_print("disable pipe low delay failed!\n");
    }

    printf("please enter any key to enable chn low delay!\n");
    sample_get_char();

    low_delay_info.enable = TD_TRUE;
    ret = ss_mpi_vi_set_chn_low_delay_attr(vi_pipe, vi_chn, &low_delay_info);
    if (ret != TD_SUCCESS) {
        sample_print("enable chn low delay failed!\n");
    }

    printf("please enter any key to disable chn low delay!\n");
    sample_get_char();

    low_delay_info.enable = TD_FALSE;
    ret = ss_mpi_vi_set_chn_low_delay_attr(vi_pipe, vi_chn, &low_delay_info);
    if (ret != TD_SUCCESS) {
        sample_print("disable chn low delay failed!\n");
    }
}

static td_s32 sample_vio_lowdelay(td_void)
{
    td_s32 ret;
    ot_vi_vpss_mode_type mode_type = OT_VI_OFFLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type;
    ot_size in_size;

    ret = sample_vio_sys_init(mode_type, video_mode, VB_YUV_ROUTE_CNT, VB_LINEAR_RAW_CNT);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    sns_type = SENSOR0_TYPE;
    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_vio_switch_low_delay(vi_pipe, vi_chn);
    sample_get_char();

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);

start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_s32 sample_vio_yuv_4mux(td_void)
{
    printf("Not support!\n");
    return TD_SUCCESS;
}

static td_void sample_switch_user_pic(ot_vi_pipe vi_pipe)
{
    td_s32 ret;
    sample_vi_user_pic_type user_pic_type;
    sample_vi_user_frame_info user_frame_info;

    for (user_pic_type = VI_USER_PIC_FRAME; user_pic_type <= VI_USER_PIC_BGCOLOR; user_pic_type++) {
        ret = sample_common_vi_load_user_pic(vi_pipe, user_pic_type, &user_frame_info);
        if (ret != TD_SUCCESS) {
            sample_print("load user pic failed!\n");
            return;
        }

        ret = ss_mpi_vi_set_user_pic(vi_pipe, &user_frame_info.frame_info);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vi_set_user_pic failed!\n");
        }

        printf("Enter any key to enable user pic!\n");
        sample_get_char();
        ret = ss_mpi_vi_enable_user_pic(vi_pipe);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vi_enable_user_pic failed!\n");
        }

        printf("Enter any key to disable user pic!\n");
        sample_get_char();
        ret = ss_mpi_vi_disable_user_pic(vi_pipe);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_vi_disable_user_pic failed!\n");
        }

        sleep(1);
        sample_common_vi_unload_user_pic(&user_frame_info);
    }
}

static td_s32 sample_vio_user_pic(td_void)
{
    td_s32 ret;
    ot_vi_vpss_mode_type mode_type = OT_VI_OFFLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type;
    ot_size in_size;

    ret = sample_vio_sys_init(mode_type, video_mode, VB_YUV_ROUTE_CNT, VB_LINEAR_RAW_CNT);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    sns_type = SENSOR0_TYPE;
    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != TD_SUCCESS) {
        goto start_vi_failed;
    }

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    ret = sample_vio_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_switch_user_pic(vi_pipe);
    sample_get_char();

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);

start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
start_vpss_failed:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_s32 sample_vio_start_multi_vi_vpss(sample_vi_cfg *vi_cfg, ot_vpss_grp *vpss_grp,
                                             td_s32 dev_num, td_s32 grp_num)
{
    td_s32 ret;
    td_s32 i;
    td_s32 j;
    ot_size in_size;
    sample_sns_type sns_type = SENSOR0_TYPE; /* default: use same sensors */

    if (dev_num != grp_num) {
        return TD_FAILURE;
    }

    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);

    for (i = 0; i < dev_num; i++) {
        ret = sample_comm_vi_start_vi(&vi_cfg[i]);
        if (ret != TD_SUCCESS) {
        goto start_vi_failed;
        }
    }

    for (i = 0; i < grp_num; i++) {
        sample_comm_vi_bind_vpss(i, 0, vpss_grp[i], 0);
    }

    for (i = 0; i < grp_num; i++) {
        ret = sample_vio_start_vpss(vpss_grp[i], &in_size);
        if (ret != TD_SUCCESS) {
            goto start_vpss_failed;
        }
    }

    return TD_SUCCESS;

start_vpss_failed:
    for (j = i - 1; j >= 0; j--) {
        sample_vio_stop_vpss(vpss_grp[j]);
    }

    for (i = 0; i < grp_num; i++) {
        sample_comm_vi_un_bind_vpss(i, 0, vpss_grp[i], 0);
    }

start_vi_failed:
    for (j = i - 1; j >= 0; j--) {
        sample_comm_vi_stop_vi(&vi_cfg[j]);
    }

    return TD_FAILURE;
}

/* Set pin_mux i2c2 & i2c5 & sensor0 & sensor1 & MIPI0 & MIPI1 before using this sample! */
static td_s32 sample_vio_two_sensor(td_void)
{
    td_s32 ret;
    ot_vi_vpss_mode_type mode_type = OT_VI_OFFLINE_VPSS_OFFLINE;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    ot_vi_pipe vi_pipe[2] = {0, 1}; /* 2 pipe */
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[2] = {0, 1}; /* 2 vpss grp */
    const td_u32 grp_num = 2; /* 2 vpss grp */
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg[2];
    sample_sns_type sns_type = SENSOR0_TYPE;
    ot_size in_size;

    ret = sample_vio_sys_init(mode_type, video_mode, VB_DOUBLE_YUV_CNT, VB_WDR_RAW_CNT);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    sample_vi_get_two_sensor_vi_cfg(sns_type, &vi_cfg[0], &vi_cfg[1]);
    ret = sample_vio_start_multi_vi_vpss(vi_cfg, vpss_grp, 2, 2);  /* start 2 route */
    if (ret != TD_SUCCESS) {
        goto start_vi_vpss_failed;
    }

    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    ret = sample_vio_start_venc_and_vo(vpss_grp, grp_num, &in_size);
    if (ret != TD_SUCCESS) {
        goto start_venc_and_vo_failed;
    }

    sample_get_char();

    sample_vio_stop_venc_and_vo(vpss_grp, grp_num);

start_venc_and_vo_failed:
    sample_vio_stop_vpss(vpss_grp[0]);
    sample_vio_stop_vpss(vpss_grp[1]);
    sample_comm_vi_un_bind_vpss(vi_pipe[0], vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_un_bind_vpss(vi_pipe[1], vi_chn, vpss_grp[1], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg[0]);
    sample_comm_vi_stop_vi(&vi_cfg[1]);
start_vi_vpss_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_void sample_vio_usage(char *prg_name)
{
    printf("usage : %s <index> \n", prg_name);
    printf("index:\n");
    printf("    (0) all mode route          :vi linear(Online/Offline) -> vpss(Online/Offline) -> venc && vo.\n");
    printf("    (1) wdr route               :vi wdr(Online) -> vpss(Offline) -> venc && vo.\n");
    printf("    (2) fpn calibrate & correct :vi fpn calibrate & correct -> vpss -> venc && vo.\n");
    printf("    (3) ldc & dis               :vi ldc & dis -> vpss -> venc && vo.\n");
    printf("    (4) fisheye                 :vi fisheye(phys_chn + ext_chn) -> vpss -> venc && vo.\n");
    printf("    (5) low delay               :vi(pipe & chn lowdelay) -> vpss(lowdelay) -> venc && vo.\n");
    printf("    (6) input yuv mux           :vi 4mux yuv input -> vpss -> venc && vo.\n");
    printf("    (7) user pic                :vi user pic (offline) -> vpss -> venc && vo.\n");
    printf("    (8) two sensor              :vi two sensor (offline) -> vpss -> venc && vo.\n");
    printf("    (9) switch mode             :vi linear switch to wdr -> vpss -> venc && vo.\n");
    printf("    (10) wdr send route         :vi wdr(offline) -> isp_runonce -> vpss(Offline) -> venc && vo.\n");
    printf("    (11) switch resolution      :vi 4K switch to FHD -> vpss -> venc && vo.\n");
    printf("    (12) all mode route mipitx  :"
        "vi linear(Online/Offline) -> vpss(Online/Offline) -> venc && vo mipirx.\n");
}

static td_void sample_vio_handle_sig(td_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        g_sig_flag = 1;
    }
}

static td_void sample_register_sig_handler(td_void (*sig_handle)(td_s32))
{
    struct sigaction sa;

    (td_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handle;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, TD_NULL);
    sigaction(SIGTERM, &sa, TD_NULL);
}

static td_s32 sample_vio_execute_case(td_u32 case_index)
{
    td_s32 ret;

    switch (case_index) {
        case 0: /* 0 all mode route */
            ret = sample_vio_all_mode();
            break;
        case 1: /* 1 wdr route */
            ret = sample_vio_wdr();
            break;
        case 2: /* 2 fpn calibrate and correct */
            ret = sample_vio_fpn();
            break;
        case 3: /* 3 ldc and dis */
            ret = sample_vio_ldc_dis();
            break;
        case 4: /* 4 fisheye */
            ret = sample_vio_fisheye();
            break;
        case 5: /* 5 low delay */
            ret = sample_vio_lowdelay();
            break;
        case 6: /* 6 yuv input mux */
            ret = sample_vio_yuv_4mux();
            break;
        case 7: /* 7 user pic */
            ret = sample_vio_user_pic();
            break;
        case 8: /* 8 two sensor */
            ret = sample_vio_two_sensor();
            break;
        case 9: /* 9 switch mode */
            ret = sample_vio_switch_mode();
            break;
        case 10: /* 10 wdr runonce route */
            ret = sample_vio_wdr_send_frame();
            break;
        case 11: /* 11 switch resolution */
            ret = sample_vio_switch_resolution();
            break;
        case 12: /* 12 all mode route -> MIPITX */
            ret = sample_vio_all_mode_mipitx();
            break;
        default:
            ret = TD_FAILURE;
            break;
    }

    return ret;
}


static td_s32 sample_vio_msg_proc_vb_pool_share(td_s32 pid)
{
    td_s32 ret;
    td_u32 i;
    td_bool isp_states[OT_VI_MAX_PIPE_NUM];
#ifndef SAMPLE_MEM_SHARE_ENABLE
    ot_vb_common_pools_id pools_id = {0};

    if (ss_mpi_vb_get_common_pool_id(&pools_id) != TD_SUCCESS) {
        sample_print("get common pool_id failed!\n");
        return TD_FAILURE;
    }

    for (i = 0; i < pools_id.pool_cnt; ++i) {
        if (ss_mpi_vb_pool_share(pools_id.pool[i], pid) != TD_SUCCESS) {
            sample_print("vb pool share failed!\n");
            return TD_FAILURE;
        }
    }
#endif
    ret = sample_comm_vi_get_isp_run_state(isp_states, OT_VI_MAX_PIPE_NUM);
    if (ret != TD_SUCCESS) {
        sample_print("get isp states fail\n");
        return TD_FAILURE;
    }

    for (i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        if (!isp_states[i]) {
            continue;
        }
        ret = ss_mpi_isp_mem_share(i, pid);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_isp_mem_share vi_pipe %d, pid %d fail\n", i, pid);
        }
    }

    return TD_SUCCESS;
}

static td_void sample_vio_msg_proc_vb_pool_unshare(td_s32 pid)
{
    td_s32 ret;
    td_u32 i;
    td_bool isp_states[OT_VI_MAX_PIPE_NUM];
#ifndef SAMPLE_MEM_SHARE_ENABLE
    ot_vb_common_pools_id pools_id = {0};
    if (ss_mpi_vb_get_common_pool_id(&pools_id) == TD_SUCCESS) {
        for (i = 0; i < pools_id.pool_cnt; ++i) {
            ret = ss_mpi_vb_pool_unshare(pools_id.pool[i], pid);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vb_pool_unshare vi_pipe %d, pid %d fail\n", pools_id.pool[i], pid);
            }
        }
    }
#endif
    ret = sample_comm_vi_get_isp_run_state(isp_states, OT_VI_MAX_PIPE_NUM);
    if (ret != TD_SUCCESS) {
        sample_print("get isp states fail\n");
        return;
    }

    for (i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        if (!isp_states[i]) {
            continue;
        }
        ret = ss_mpi_isp_mem_unshare(i, pid);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_isp_mem_unshare vi_pipe %d, pid %d fail\n", i, pid);
        }
    }
}

static td_s32 sample_vio_ipc_msg_proc(const sample_ipc_msg_req_buf *msg_req_buf,
    td_bool *is_need_fb, sample_ipc_msg_res_buf *msg_res_buf)
{
    td_s32 ret;

    if (msg_req_buf == TD_NULL || is_need_fb == TD_NULL) {
        return TD_FAILURE;
    }

    /* need feedback default */
    *is_need_fb = TD_TRUE;

    switch ((sample_msg_type)msg_req_buf->msg_type) {
        case SAMPLE_MSG_TYPE_VB_POOL_SHARE_REQ: {
            if (msg_res_buf == TD_NULL) {
                return TD_FAILURE;
            }
            ret = sample_vio_msg_proc_vb_pool_share(msg_req_buf->msg_data.pid);
            msg_res_buf->msg_type = SAMPLE_MSG_TYPE_VB_POOL_SHARE_RES;
            msg_res_buf->msg_data.is_req_success = (ret == TD_SUCCESS) ? TD_TRUE : TD_FALSE;
            break;
        }
        case SAMPLE_MSG_TYPE_VB_POOL_UNSHARE_REQ: {
            if (msg_res_buf == TD_NULL) {
                return TD_FAILURE;
            }
            sample_vio_msg_proc_vb_pool_unshare(msg_req_buf->msg_data.pid);
            msg_res_buf->msg_type = SAMPLE_MSG_TYPE_VB_POOL_UNSHARE_RES;
            msg_res_buf->msg_data.is_req_success = TD_TRUE;
            break;
        }
        default: {
            printf("unsupported msg type(%ld)!\n", msg_req_buf->msg_type);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

#ifdef __LITEOS__
td_s32 app_main(td_s32 argc, td_char *argv[])
#else
td_s32 main(td_s32 argc, td_char *argv[])
#endif
{
    td_s32 ret;
    td_u32 index;

    if (argc != 2) { /* 2:arg num */
        sample_vio_usage(argv[0]);
        return TD_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_vio_usage(argv[0]);
        return TD_FAILURE;
    }

    if (strlen(argv[1]) > 2 || strlen(argv[1]) <= 0 || !check_digit(argv[1][0]) || /* 2:arg len */
        (strlen(argv[1]) == 2 && (!check_digit(argv[1][1]) || argv[1][0] == '0'))) { /* 2:arg len */
        sample_vio_usage(argv[0]);
        return TD_FAILURE;
    }

#ifndef __LITEOS__
    sample_register_sig_handler(sample_vio_handle_sig);
#endif

    if (sample_ipc_server_init(sample_vio_ipc_msg_proc) != TD_SUCCESS) {
        printf("sample_ipc_server_init failed!!!\n");
    }

#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif

    index = atoi(argv[1]);
    ret = sample_vio_execute_case(index);
    if ((ret == TD_SUCCESS) && (g_sig_flag == 0)) {
        printf("\033[0;32mprogram exit normally!\033[0;39m\n");
    } else {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    sample_ipc_server_deinit();

#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif

#ifdef __LITEOS__
    return ret;
#else
    exit(ret);
#endif
}
