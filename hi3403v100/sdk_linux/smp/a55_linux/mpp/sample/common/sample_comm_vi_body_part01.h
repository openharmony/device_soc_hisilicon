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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART01_H_


#line 1 "sample_comm_vi.c"
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
#include <sys/prctl.h>

#include "sample_comm.h"
#include "ot_common.h"
#include "ot_mipi_rx.h"
#include "ss_mpi_vi.h"
#include "ss_mpi_isp.h"
#include "securec.h"

#define MIPI_DEV_NAME "/dev/ot_mipi_rx"

#define FPN_FILE_NAME_LENGTH 150
#define FPN_CALIB_TIMES 8
#define WIDTH_1920 1920
#define HEIGHT_1080 1080
#define WIDTH_3840 3840
#define HEIGHT_2160 2160
#define WIDTH_2688 2688
#define HEIGHT_1520 1520
#define SLEEP_TIME 1000
#define MIPI_NUM 3
#define OB_HEIGHT_END 32
#define OB_HEIGHT_START 0

typedef struct {
    sample_vi_user_frame_info *user_frame_info;
    ot_vi_pipe vi_pipe;
    td_u32 frame_cnt;
} sample_vi_send_frame_info;

static td_bool g_send_pipe_pthread = TD_FALSE;
static td_bool g_start_isp[OT_VI_MAX_PIPE_NUM] = {TD_FALSE};

static ext_data_type_t g_mipi_ext_data_type_12bit_8m_nowdr_attr = {
    .devno = 0,
    .num = MIPI_NUM,
    .ext_data_bit_width = {12, 12, 12},
    .ext_data_type = {0x37, 0x2c, 0x2c}
};

static ext_data_type_t g_mipi_ext_data_type_default_attr = {
    .devno = 0,
    .num = MIPI_NUM,
    .ext_data_bit_width = {12, 12, 12},
    .ext_data_type = {0x2c, 0x2c, 0x2c}
};

static combo_dev_attr_t g_mipi_4lane_chn0_sensor_hy_s0603_12bit_8m_nowdr_attr = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate  = MIPI_DATA_RATE_X1,
    .img_rect   = {0, 0, WIDTH_3840, HEIGHT_2160},
    .mipi_attr = {
        DATA_TYPE_RAW_12BIT,
        OT_MIPI_WDR_MODE_NONE,
        {0, 1, 2, 3, -1, -1, -1, -1}
    }
};

static combo_dev_attr_t g_mipi_4lane_chn0_sensor_hy_s0603_12bit_8m_nowdr_dev2_attr = {
    .devno = 2, /* dev2 */
    .input_mode = INPUT_MODE_MIPI,
    .data_rate  = MIPI_DATA_RATE_X1,
    .img_rect   = {0, 0, WIDTH_3840, HEIGHT_2160},
    .mipi_attr = {
        DATA_TYPE_RAW_12BIT,
        OT_MIPI_WDR_MODE_NONE,
        {4, 5, 6, 7, -1, -1, -1, -1}
    }
};

static combo_dev_attr_t g_mipi_4lane_chn0_sensor_hy_s0603_10bit_8m_wdr2to1_attr = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate = MIPI_DATA_RATE_X1,
    .img_rect = {0, 0, WIDTH_3840, HEIGHT_2160},
    .mipi_attr = {
        DATA_TYPE_RAW_10BIT,
        OT_MIPI_WDR_MODE_VC,
        {0, 1, 2, 3, -1, -1, -1, -1}
    }
};

static combo_dev_attr_t g_mipi_4lane_chn0_sensor_os08a20_12bit_8m_nowdr_attr = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate  = MIPI_DATA_RATE_X1,
    .img_rect   = {0, 0, WIDTH_3840, HEIGHT_2160},
    .mipi_attr = {
        DATA_TYPE_RAW_12BIT,
        OT_MIPI_WDR_MODE_NONE,
        {0, 1, 2, 3, -1, -1, -1, -1}
    }
};

static combo_dev_attr_t g_mipi_4lane_chn0_sensor_os08a20_12bit_8m_nowdr_dev2_attr = {
    .devno = 2, /* dev2 */
    .input_mode = INPUT_MODE_MIPI,
    .data_rate  = MIPI_DATA_RATE_X1,
    .img_rect   = {0, 0, WIDTH_3840, HEIGHT_2160},
    .mipi_attr = {
        DATA_TYPE_RAW_12BIT,
        OT_MIPI_WDR_MODE_NONE,
        {4, 5, 6, 7, -1, -1, -1, -1}
    }
};

static combo_dev_attr_t g_mipi_4lane_chn0_sensor_os08a20_12bit_8m_wdr2to1_attr = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate = MIPI_DATA_RATE_X1,
    .img_rect = {0, 0, WIDTH_3840, HEIGHT_2160},
    .mipi_attr = {
        DATA_TYPE_RAW_12BIT,
        OT_MIPI_WDR_MODE_VC,
        {0, 1, 2, 3, -1, -1, -1, -1}
    }
};

static combo_dev_attr_t g_mipi_4lane_chn0_sensor_sc450ai_10bit_4m_nowdr_attr = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate  = MIPI_DATA_RATE_X1,
    .img_rect   = {0, 0, WIDTH_2688, HEIGHT_1520},
    .mipi_attr  = {
        DATA_TYPE_RAW_10BIT,
        OT_MIPI_WDR_MODE_NONE,
        {0, 1, 2, 3, -1, -1, -1, -1}
    }
};

static combo_dev_attr_t g_mipi_4lane_chn0_sensor_sc450ai_10bit_4m_nowdr_dev2_attr = {
    .devno = 2,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate  = MIPI_DATA_RATE_X1,
    .img_rect   = {0, 0, WIDTH_2688, HEIGHT_1520},
    .mipi_attr  = {
        DATA_TYPE_RAW_10BIT,
        OT_MIPI_WDR_MODE_NONE,
        {4, 5, 6, 7, -1, -1, -1, -1}
    }
};

static combo_dev_attr_t g_mipi_4lane_chn0_sensor_sc450ai_10bit_4m_wdr2to1_attr = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate  = MIPI_DATA_RATE_X1,
    .img_rect   = {0, 0, WIDTH_2688, HEIGHT_1520},
    .mipi_attr  = {
        DATA_TYPE_RAW_10BIT,
        OT_MIPI_WDR_MODE_VC,
        {0, 1, 2, 3, -1, -1, -1, -1}
    }
};

static combo_dev_attr_t g_mipi_4lane_chn0_sensor_sc450ai_10bit_4m_wdr2to1_dev2_attr = {
    .devno = 0,
    .input_mode = INPUT_MODE_MIPI,
    .data_rate  = MIPI_DATA_RATE_X1,
    .img_rect   = {0, 0, WIDTH_2688, HEIGHT_1520},
    .mipi_attr  = {
        DATA_TYPE_RAW_10BIT,
        OT_MIPI_WDR_MODE_NONE,
        {4, 5, 6, 7, -1, -1, -1, -1}
    }
};

static td_void sample_comm_vi_get_mipi_attr(sample_sns_type sns_type, combo_dev_attr_t *combo_attr)
{
    td_u32 ob_height = OB_HEIGHT_START;
    switch (sns_type) {
        case HY_S0603_MIPI_8M_30FPS_12BIT:
            ob_height = OB_HEIGHT_END;
            (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                &g_mipi_4lane_chn0_sensor_hy_s0603_12bit_8m_nowdr_attr, sizeof(combo_dev_attr_t));
            break;
        case HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1:
            ob_height = OB_HEIGHT_END;
            (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                &g_mipi_4lane_chn0_sensor_hy_s0603_10bit_8m_wdr2to1_attr, sizeof(combo_dev_attr_t));
            break;
        case OV_OS08A20_MIPI_8M_30FPS_12BIT:
            ob_height = OB_HEIGHT_END;
            (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                &g_mipi_4lane_chn0_sensor_os08a20_12bit_8m_nowdr_attr, sizeof(combo_dev_attr_t));
            break;
        case OV_OS08A20_MIPI_8M_30FPS_12BIT_WDR2TO1:
            ob_height = OB_HEIGHT_END;
            (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                &g_mipi_4lane_chn0_sensor_os08a20_12bit_8m_wdr2to1_attr, sizeof(combo_dev_attr_t));
            break;
        case SC450AI_MIPI_4M_30FPS_10BIT_WDR2TO1:
            (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                &g_mipi_4lane_chn0_sensor_sc450ai_10bit_4m_wdr2to1_attr, sizeof(combo_dev_attr_t));
            break;
        default:
            (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                &g_mipi_4lane_chn0_sensor_hy_s0603_12bit_8m_nowdr_attr, sizeof(combo_dev_attr_t));
    }
    combo_attr->img_rect.height = combo_attr->img_rect.height + ob_height;
}

static td_void sample_comm_vi_get_mipi_ext_data_attr(sample_sns_type sns_type, ext_data_type_t *ext_data_attr)
{
    switch (sns_type) {
        case HY_S0603_MIPI_8M_30FPS_12BIT:
        case HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1:
        case OV_OS08A20_MIPI_8M_30FPS_12BIT:
        case SC450AI_MIPI_4M_30FPS_10BIT:
        case SC450AI_MIPI_4M_30FPS_10BIT_WDR2TO1:
            (td_void)memcpy_s(ext_data_attr, sizeof(ext_data_type_t),
                &g_mipi_ext_data_type_12bit_8m_nowdr_attr, sizeof(ext_data_type_t));
            break;
        case OV_OS08A20_MIPI_8M_30FPS_12BIT_WDR2TO1:
            (td_void)memcpy_s(ext_data_attr, sizeof(ext_data_type_t),
                &g_mipi_ext_data_type_default_attr, sizeof(ext_data_type_t));
            break;
        default:
            (td_void)memcpy_s(ext_data_attr, sizeof(ext_data_type_t),
                &g_mipi_ext_data_type_default_attr, sizeof(ext_data_type_t));
    }
}

static td_void sample_comm_vi_get_mipi_attr_by_dev_id(sample_sns_type sns_type, ot_vi_dev vi_dev,
                                                      combo_dev_attr_t *combo_attr)
{
    td_u32 ob_height = OB_HEIGHT_START;
    switch (sns_type) {
        case HY_S0603_MIPI_8M_30FPS_12BIT:
            ob_height = OB_HEIGHT_END;
            if (vi_dev == 0) {
                (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                    &g_mipi_4lane_chn0_sensor_hy_s0603_12bit_8m_nowdr_attr, sizeof(combo_dev_attr_t));
            } else if (vi_dev == 2) { /* dev2 */
                (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                    &g_mipi_4lane_chn0_sensor_hy_s0603_12bit_8m_nowdr_dev2_attr, sizeof(combo_dev_attr_t));
            }
            break;
        case OV_OS08A20_MIPI_8M_30FPS_12BIT:
            ob_height = OB_HEIGHT_END;
            if (vi_dev == 0) {
                (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                    &g_mipi_4lane_chn0_sensor_os08a20_12bit_8m_nowdr_attr, sizeof(combo_dev_attr_t));
            } else if (vi_dev == 2) { /* dev2 */
                (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                    &g_mipi_4lane_chn0_sensor_os08a20_12bit_8m_nowdr_dev2_attr, sizeof(combo_dev_attr_t));
            }
            break;
        case SC450AI_MIPI_4M_30FPS_10BIT:
            if (vi_dev == 0) {
                (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                    &g_mipi_4lane_chn0_sensor_sc450ai_10bit_4m_nowdr_attr, sizeof(combo_dev_attr_t));
            } else if (vi_dev == 2) { /* dev2 */
                (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                    &g_mipi_4lane_chn0_sensor_sc450ai_10bit_4m_nowdr_dev2_attr, sizeof(combo_dev_attr_t));
            }
            break;
        case SC450AI_MIPI_4M_30FPS_10BIT_WDR2TO1:
            if (vi_dev == 0) {
                (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                    &g_mipi_4lane_chn0_sensor_sc450ai_10bit_4m_wdr2to1_attr, sizeof(combo_dev_attr_t));
            } else if (vi_dev == 2) { /* dev2 */
                (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                    &g_mipi_4lane_chn0_sensor_sc450ai_10bit_4m_wdr2to1_dev2_attr, sizeof(combo_dev_attr_t));
            }
            break;

        default:
            (td_void)memcpy_s(combo_attr, sizeof(combo_dev_attr_t),
                &g_mipi_4lane_chn0_sensor_hy_s0603_12bit_8m_nowdr_attr, sizeof(combo_dev_attr_t));
    }
    combo_attr->img_rect.height = combo_attr->img_rect.height + ob_height;
}

static ot_vi_dev_attr g_mipi_raw_dev_attr = {
    .intf_mode = OT_VI_INTF_MODE_MIPI,

    /* Invalid argument */
    .work_mode = OT_VI_WORK_MODE_MULTIPLEX_1,

    /* mask component */
    .component_mask = {0xfff00000, 0x00000000},

    .scan_mode = OT_VI_SCAN_PROGRESSIVE,

    /* Invalid argument */
    .ad_chn_id = {-1, -1, -1, -1},

    /* data seq */
    .data_seq = OT_VI_DATA_SEQ_YVYU,

    /* sync param */
    .sync_cfg = {
        .vsync           = OT_VI_VSYNC_FIELD,
        .vsync_neg       = OT_VI_VSYNC_NEG_HIGH,
        .hsync           = OT_VI_HSYNC_VALID_SIG,
        .hsync_neg       = OT_VI_HSYNC_NEG_HIGH,
        .vsync_valid     = OT_VI_VSYNC_VALID_SIG,
        .vsync_valid_neg = OT_VI_VSYNC_VALID_NEG_HIGH,
        .timing_blank    = {
            /* hsync_hfb      hsync_act     hsync_hhb */
            0,                0,            0,
            /* vsync0_vhb     vsync0_act    vsync0_hhb */
            0,                0,            0,
            /* vsync1_vhb     vsync1_act    vsync1_hhb */
            0,                0,            0
        }
    },

    /* data type */
    .data_type = OT_VI_DATA_TYPE_RAW,

    /* data reverse */
    .data_reverse = TD_FALSE,

    /* input size */
    .in_size = {WIDTH_3840, HEIGHT_2160},

    /* data rate */
    .data_rate = OT_DATA_RATE_X1,
};

static td_void sample_comm_vi_get_dev_attr_by_intf_mode(ot_vi_intf_mode intf_mode, ot_vi_dev_attr *dev_attr)
{
    switch (intf_mode) {
        case OT_VI_INTF_MODE_MIPI:
            (td_void)memcpy_s(dev_attr, sizeof(ot_vi_dev_attr), &g_mipi_raw_dev_attr, sizeof(ot_vi_dev_attr));
            break;

        default:
            (td_void)memcpy_s(dev_attr, sizeof(ot_vi_dev_attr), &g_mipi_raw_dev_attr, sizeof(ot_vi_dev_attr));
            break;
    }
}

td_void sample_comm_vi_get_size_by_sns_type(sample_sns_type sns_type, ot_size *size)
{
    switch (sns_type) {
        case HY_S0603_MIPI_8M_30FPS_12BIT:
        case HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1:
            size->width = WIDTH_3840;
            size->height = HEIGHT_2160;
            break;
        case OV_OS08A20_MIPI_8M_30FPS_12BIT:
        case OV_OS08A20_MIPI_8M_30FPS_12BIT_WDR2TO1:
            size->width  = WIDTH_3840;
            size->height = HEIGHT_2160;
            break;
        case SC450AI_MIPI_4M_30FPS_10BIT:
        case SC450AI_MIPI_4M_30FPS_10BIT_WDR2TO1:
            size->width  = WIDTH_2688;
            size->height = HEIGHT_1520;
            break;

        default:
            size->width  = WIDTH_1920;
            size->height = HEIGHT_1080;
            break;
    }
}

td_u32 sample_comm_vi_get_obheight_by_sns_type(sample_sns_type sns_type)
{
    td_u32 ob_height = OB_HEIGHT_START;
    switch (sns_type) {
        case HY_S0603_MIPI_8M_30FPS_12BIT:
        case HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1:
            ob_height = OB_HEIGHT_END;
            break;
        case OV_OS08A20_MIPI_8M_30FPS_12BIT:
            ob_height = OB_HEIGHT_END;
            break;
        case OV_OS08A20_MIPI_8M_30FPS_12BIT_WDR2TO1:
            ob_height = OB_HEIGHT_END;
            break;
        case SC450AI_MIPI_4M_30FPS_10BIT:
        case SC450AI_MIPI_4M_30FPS_10BIT_WDR2TO1:
            ob_height = OB_HEIGHT_START;
            break;
        default:
            break;
    }

    return ob_height;
}

static td_u32 sample_comm_vi_get_pipe_num_by_sns_type(sample_sns_type sns_type)
{
    switch (sns_type) {
        case HY_S0603_MIPI_8M_30FPS_12BIT:
        case OV_OS08A20_MIPI_8M_30FPS_12BIT:
        case SC450AI_MIPI_4M_30FPS_10BIT:
            return 1;
        case HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1:
        case OV_OS08A20_MIPI_8M_30FPS_12BIT_WDR2TO1:
        case SC450AI_MIPI_4M_30FPS_10BIT_WDR2TO1:
            return 2; /* 2 pipe */

        default:
            return 1;
    }
}

static ot_wdr_mode sample_comm_vi_get_wdr_mode_by_sns_type(sample_sns_type sns_type)
{
    switch (sns_type) {
        case HY_S0603_MIPI_8M_30FPS_12BIT:
        case OV_OS08A20_MIPI_8M_30FPS_12BIT:
        case SC450AI_MIPI_4M_30FPS_10BIT:
            return OT_WDR_MODE_NONE;

        case HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1:
        case OV_OS08A20_MIPI_8M_30FPS_12BIT_WDR2TO1:
        case SC450AI_MIPI_4M_30FPS_10BIT_WDR2TO1:
            return OT_WDR_MODE_2To1_LINE;

        default:
            return OT_WDR_MODE_NONE;
    }
}

td_void sample_comm_vi_get_default_sns_info(sample_sns_type sns_type, sample_sns_info *sns_info)
{
    sns_info->sns_type    = sns_type;
    sns_info->sns_clk_src = 0;
    sns_info->sns_rst_src = 0;
    sns_info->bus_id      = 5; /* i2c5  */
}

td_void sample_comm_vi_get_default_mipi_info(sample_sns_type sns_type, sample_mipi_info *mipi_info)
{
    mipi_info->mipi_dev    = 0;
    mipi_info->divide_mode = LANE_DIVIDE_MODE_0;
    sample_comm_vi_get_mipi_attr(sns_type, &mipi_info->combo_dev_attr);
    sample_comm_vi_get_mipi_ext_data_attr(sns_type, &mipi_info->ext_data_type_attr);
}

/* used for two sensor: mipi lane 4 + 4 */
td_void sample_comm_vi_get_mipi_info_by_dev_id(sample_sns_type sns_type, ot_vi_dev vi_dev, sample_mipi_info *mipi_info)
{
    mipi_info->mipi_dev    = vi_dev;
    mipi_info->divide_mode = LANE_DIVIDE_MODE_1;
    sample_comm_vi_get_mipi_attr_by_dev_id(sns_type, vi_dev, &mipi_info->combo_dev_attr);
    sample_comm_vi_get_mipi_ext_data_attr(sns_type, &mipi_info->ext_data_type_attr);
    mipi_info->ext_data_type_attr.devno = vi_dev;
}

td_void sample_comm_vi_get_default_dev_info(sample_sns_type sns_type, sample_vi_dev_info *dev_info)
{
    ot_size size;
    td_u32 ob_height;

    dev_info->vi_dev = 0;
    sample_comm_vi_get_dev_attr_by_intf_mode(OT_VI_INTF_MODE_MIPI, &dev_info->dev_attr);
    sample_comm_vi_get_size_by_sns_type(sns_type, &size);
    ob_height = sample_comm_vi_get_obheight_by_sns_type(sns_type);
    dev_info->dev_attr.in_size.width  = size.width;
    dev_info->dev_attr.in_size.height = size.height + ob_height;
    dev_info->bas_attr.enable = TD_FALSE;
}

static td_void sample_comm_vi_get_default_bind_info(sample_sns_type sns_type, ot_vi_bind_pipe *bind_pipe)
{
    td_u32 i;

    bind_pipe->pipe_num = sample_comm_vi_get_pipe_num_by_sns_type(sns_type);
    for (i = 0; i < bind_pipe->pipe_num; i++) {
        bind_pipe->pipe_id[i] = i;
    }
}

static td_void sample_comm_vi_get_default_grp_info(sample_sns_type sns_type, sample_vi_grp_info *grp_info)
{
    td_u32 i;
    td_u32 pipe_num;
    ot_size size;

    sample_comm_vi_get_size_by_sns_type(sns_type, &size);
    grp_info->grp_num = 1;
    grp_info->fusion_grp[0] = 0;
    grp_info->fusion_grp_attr[0].wdr_mode = sample_comm_vi_get_wdr_mode_by_sns_type(sns_type);
    grp_info->fusion_grp_attr[0].cache_line = size.height;
    pipe_num = sample_comm_vi_get_pipe_num_by_sns_type(sns_type);
    for (i = 0; i < pipe_num; i++) {
        grp_info->fusion_grp_attr[0].pipe_id[i] = i;
    }
}

td_void sample_comm_vi_get_default_pipe_info(sample_sns_type sns_type, ot_vi_bind_pipe *bind_pipe,
                                             sample_vi_pipe_info pipe_info[])
{
    td_u32 i;
    ot_size size;

    sample_comm_vi_get_size_by_sns_type(sns_type, &size);

    for (i = 0; i < bind_pipe->pipe_num; i++) {
        /* pipe attr */
        pipe_info[i].pipe_attr.pipe_bypass_mode               = OT_VI_PIPE_BYPASS_NONE;
        pipe_info[i].pipe_attr.isp_bypass                     = TD_FALSE;
        pipe_info[i].pipe_attr.size.width                     = size.width;
        pipe_info[i].pipe_attr.size.height                    = size.height;
        pipe_info[i].pipe_attr.pixel_format                   = OT_PIXEL_FORMAT_RGB_BAYER_12BPP;
        pipe_info[i].pipe_attr.compress_mode                  = OT_COMPRESS_MODE_LINE;
        pipe_info[i].pipe_attr.bit_width                      = OT_DATA_BIT_WIDTH_8;
        pipe_info[i].pipe_attr.bit_align_mode                 = OT_VI_BIT_ALIGN_MODE_HIGH;
        pipe_info[i].pipe_attr.frame_rate_ctrl.src_frame_rate = -1;
        pipe_info[i].pipe_attr.frame_rate_ctrl.dst_frame_rate = -1;

        pipe_info[i].pipe_need_start = TD_TRUE;
        pipe_info[i].isp_need_run = TD_TRUE;

        /* pub attr */
        sample_comm_isp_get_pub_attr_by_sns(sns_type, &pipe_info[i].isp_info.isp_pub_attr);

        /* chn info */
        pipe_info[i].chn_num = 1;
        pipe_info[i].chn_info[0].vi_chn                                  = 0;
        pipe_info[i].chn_info[0].chn_attr.size.width                     = size.width;
        pipe_info[i].chn_info[0].chn_attr.size.height                    = size.height;
        pipe_info[i].chn_info[0].chn_attr.pixel_format                   = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
        pipe_info[i].chn_info[0].chn_attr.dynamic_range                  = OT_DYNAMIC_RANGE_SDR8;
        pipe_info[i].chn_info[0].chn_attr.video_format                   = OT_VIDEO_FORMAT_LINEAR;
        pipe_info[i].chn_info[0].chn_attr.compress_mode                  = OT_COMPRESS_MODE_NONE;
        pipe_info[i].chn_info[0].chn_attr.mirror_en                      = TD_FALSE;
        pipe_info[i].chn_info[0].chn_attr.flip_en                        = TD_FALSE;
        pipe_info[i].chn_info[0].chn_attr.depth                          = 0;
        pipe_info[i].chn_info[0].chn_attr.frame_rate_ctrl.src_frame_rate = -1;
        pipe_info[i].chn_info[0].chn_attr.frame_rate_ctrl.dst_frame_rate = -1;
    }
}

td_void sample_comm_vi_init_pipe_info(sample_sns_type sns_type, const ot_size *size, ot_vi_bind_pipe *bind_pipe,
    sample_vi_pipe_info pipe_info[])
{
    td_u32 i;

    for (i = 0; i < bind_pipe->pipe_num; i++) {
        /* pipe attr */
        pipe_info[i].pipe_attr.pipe_bypass_mode               = OT_VI_PIPE_BYPASS_NONE;
        pipe_info[i].pipe_attr.isp_bypass                     = TD_FALSE;
        pipe_info[i].pipe_attr.size.width                     = size->width;
        pipe_info[i].pipe_attr.size.height                    = size->height;
        pipe_info[i].pipe_attr.pixel_format                   = OT_PIXEL_FORMAT_RGB_BAYER_12BPP;
        pipe_info[i].pipe_attr.compress_mode                  = OT_COMPRESS_MODE_LINE;
        pipe_info[i].pipe_attr.bit_width                      = OT_DATA_BIT_WIDTH_8;
        pipe_info[i].pipe_attr.bit_align_mode                 = OT_VI_BIT_ALIGN_MODE_HIGH;
        pipe_info[i].pipe_attr.frame_rate_ctrl.src_frame_rate = -1;
        pipe_info[i].pipe_attr.frame_rate_ctrl.dst_frame_rate = -1;

        pipe_info[i].pipe_need_start = TD_TRUE;
        pipe_info[i].isp_need_run = TD_TRUE;

        /* pub attr */
        sample_comm_isp_get_pub_attr_by_sns(sns_type, &pipe_info[i].isp_info.isp_pub_attr);

        /* chn info */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VI_BODY_PART01_H_ */
