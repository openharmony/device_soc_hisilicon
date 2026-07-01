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

#ifndef DISPLAY_ADAPT_VO_H
#define DISPLAY_ADAPT_VO_H

#include "ss_mpi_vo.h"
#include "ot_defines.h"
#include "ot_common_video.h"
#include "display_adapt_vo_include.h"

#define HI_ID_VO            OT_ID_VO
#define HI_ID_VPSS          OT_ID_VPSS

typedef ot_mpp_chn MPP_CHN_S;

#define VO_MAX_CHN_NUM      OT_VO_MAX_CHN_NUM
#define VO_MAX_LAYER_NUM    OT_VO_MAX_LAYER_NUM

typedef ot_vo_pub_attr VO_PUB_ATTR_S;

#define VO_CSC_MATRIX_IDENTITY        OT_VO_CSC_MATRIX_BT601LIMIT_TO_BT601LIMIT

typedef ot_vo_csc VO_CSC_S;

#define VO_OUTPUT_PAL           OT_VO_OUT_PAL
#define VO_OUTPUT_NTSC          OT_VO_OUT_NTSC
#define VO_OUTPUT_960H_PAL      OT_VO_OUT_960H_PAL
#define VO_OUTPUT_960H_NTSC     OT_VO_OUT_960H_NTSC

#define VO_OUTPUT_640X480_60    OT_VO_OUT_640x480_60
#define VO_OUTPUT_480P60        OT_VO_OUT_480P60
#define VO_OUTPUT_576P50        OT_VO_OUT_576P50
#define VO_OUTPUT_800X600_60    OT_VO_OUT_800x600_60
#define VO_OUTPUT_1024X768_60   OT_VO_OUT_1024x768_60
#define VO_OUTPUT_720P50        OT_VO_OUT_720P50
#define VO_OUTPUT_720P60        OT_VO_OUT_720P60
#define VO_OUTPUT_1280X800_60   OT_VO_OUT_1280x800_60
#define VO_OUTPUT_1280X1024_60  OT_VO_OUT_1280x1024_60
#define VO_OUTPUT_1366X768_60   OT_VO_OUT_1366x768_60
#define VO_OUTPUT_1440X900_60   OT_VO_OUT_1440x900_60
#define VO_OUTPUT_1680X1050_60  OT_VO_OUT_1680x1050_60

#define VO_OUTPUT_1080P24 OT_VO_OUT_1080P24
#define VO_OUTPUT_1080P25 OT_VO_OUT_1080P25
#define VO_OUTPUT_1080P30 OT_VO_OUT_1080P30
#define VO_OUTPUT_1080I50 OT_VO_OUT_1080I50
#define VO_OUTPUT_1080I60 OT_VO_OUT_1080I60
#define VO_OUTPUT_1080P50 OT_VO_OUT_1080P50
#define VO_OUTPUT_1080P60 OT_VO_OUT_1080P60

#define VO_OUTPUT_1600X1200_60 OT_VO_OUT_1600x1200_60
#define VO_OUTPUT_1920X1200_60 OT_VO_OUT_1920x1200_60
#define VO_OUTPUT_1920X2160_30 OT_VO_OUT_1920x2160_30
#define VO_OUTPUT_2560X1440_30 OT_VO_OUT_2560x1440_30
#define VO_OUTPUT_2560X1440_60 OT_VO_OUT_2560x1440_60
#define VO_OUTPUT_2560X1600_60 OT_VO_OUT_2560x1600_60

#define VO_OUTPUT_3840X2160_24 OT_VO_OUT_3840x2160_24
#define VO_OUTPUT_3840X2160_25 OT_VO_OUT_3840x2160_25
#define VO_OUTPUT_3840X2160_30 OT_VO_OUT_3840x2160_30
#define VO_OUTPUT_3840X2160_50 OT_VO_OUT_3840x2160_50
#define VO_OUTPUT_3840X2160_60 OT_VO_OUT_3840x2160_60
#define VO_OUTPUT_4096X2160_24 OT_VO_OUT_4096x2160_24
#define VO_OUTPUT_4096X2160_25 OT_VO_OUT_4096x2160_25
#define VO_OUTPUT_4096X2160_30 OT_VO_OUT_4096x2160_30
#define VO_OUTPUT_4096X2160_50 OT_VO_OUT_4096x2160_50
#define VO_OUTPUT_4096X2160_60 OT_VO_OUT_4096x2160_60
#define VO_OUTPUT_7680X4320_30 OT_VO_OUT_7680x4320_30

#define VO_OUTPUT_240X320_50 OT_VO_OUT_240x320_50
#define VO_OUTPUT_320X240_50 OT_VO_OUT_320x240_50
#define VO_OUTPUT_240X320_60 OT_VO_OUT_240x320_60
#define VO_OUTPUT_320X240_60 OT_VO_OUT_320x240_60
#define VO_OUTPUT_800X600_50 OT_VO_OUT_800x600_50

#define VO_OUTPUT_720X1280_60   OT_VO_OUT_720x1280_60
#define VO_OUTPUT_1080X1920_60  OT_VO_OUT_1080x1920_60
#define VO_OUTPUT_USER          OT_VO_OUT_USER
typedef ot_vo_intf_sync VO_INTF_SYNC_E;

typedef ot_vo_sync_info VO_SYNC_INFO_S;

typedef ot_vo_mod_param VO_MOD_PARAM_S;

#define VO_CLK_SOURCE_PLL       OT_VO_CLK_SRC_PLL
#define VO_CLK_SOURCE_LCDMCLK   OT_VO_CLK_SRC_LCDMCLK
#define VO_CLK_SOURCE_BUTT      OT_VO_CLK_SRC_BUTT

typedef ot_vo_pll VO_USER_INTFSYNC_PLL_S;

typedef ot_vo_user_sync_attr VO_USER_INTFSYNC_ATTR_S;

typedef ot_vo_user_sync_info VO_USER_INTFSYNC_INFO_S;

#define VO_INTF_CVBS        OT_VO_INTF_CVBS
#define VO_INTF_VGA         OT_VO_INTF_VGA
#define VO_INTF_BT1120      OT_VO_INTF_BT1120
#define VO_INTF_HDMI        OT_VO_INTF_HDMI
#define VO_INTF_RGB_6BIT    OT_VO_INTF_RGB_6BIT
#define VO_INTF_RGB_8BIT    OT_VO_INTF_RGB_8BIT
#define VO_INTF_RGB_16BIT   OT_VO_INTF_RGB_16BIT
#define VO_INTF_RGB_18BIT   OT_VO_INTF_RGB_18BIT
#define VO_INTF_RGB_24BIT   OT_VO_INTF_RGB_24BIT
#define VO_INTF_MIPI        OT_VO_INTF_MIPI
#define VO_INTF_MIPI_SLAVE  OT_VO_INTF_MIPI_SLAVE
#define VO_INTF_HDMI1       OT_VO_INTF_HDMI1

// adapt vo variable type
#define VO_LAYER            ot_vo_layer
#define VO_CHN              ot_vo_chn

typedef ot_rect RECT_S;

typedef ot_vo_chn_attr      VO_CHN_ATTR_S;

typedef ot_vo_video_layer_attr VO_VIDEO_LAYER_ATTR_S;

typedef ot_vo_layer_param VO_LAYER_PARAM_S;
typedef ot_vo_chn_param VO_CHN_PARAM_S;

#define VO_CSC_MATRIX_BT709_TO_RGB_PC OT_VO_CSC_MATRIX_BT709FULL_TO_BT709FULL
typedef ot_vo_csc_matrix VO_CSC_MATRIX_E;

#endif  // DISPLAY_ADAPT_VO_H