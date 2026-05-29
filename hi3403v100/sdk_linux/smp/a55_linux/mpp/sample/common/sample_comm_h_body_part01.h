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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_H_BODY_PART01_H_


#include <pthread.h>

#include "ot_common.h"
#include "ot_math.h"
#include "ot_buffer.h"
#include "ot_defines.h"
#include "securec.h"
#include "ot_mipi_rx.h"
#include "ot_mipi_tx.h"
#include "ot_common_sys.h"
#include "ot_common_vb.h"
#include "ot_common_isp.h"
#include "ot_common_vi.h"
#include "ot_common_vo.h"
#include "ot_common_venc.h"
#include "ot_common_vdec.h"
#include "ot_common_vpss.h"
#include "ot_common_region.h"
#include "ot_common_adec.h"
#include "ot_common_aenc.h"
#include "ot_common_aio.h"
#include "ot_common_hdmi.h"
#include "ot_common_vgs.h"

#include "ss_mpi_sys.h"
#include "ss_mpi_vb.h"
#include "ss_mpi_vi.h"
#include "ss_mpi_isp.h"
#include "ss_mpi_vo.h"
#include "ss_mpi_venc.h"
#include "ss_mpi_vdec.h"
#include "ss_mpi_vpss.h"
#include "ss_mpi_region.h"
#include "ss_mpi_audio.h"
#include "ss_mpi_hdmi.h"
#include "ss_mpi_vgs.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

/* macro define */
#define FILE_NAME_LEN 128
#define FILE_PATH_LEN 128

#define SAMPLE_PIXEL_FORMAT OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420

#define COLOR_RGB_RED      0xFF0000
#define COLOR_RGB_GREEN    0x00FF00
#define COLOR_RGB_BLUE     0x0000FF
#define COLOR_RGB_BLACK    0x000000
#define COLOR_RGB_YELLOW   0xFFFF00
#define COLOR_RGB_CYN      0x00ffff
#define COLOR_RGB_WHITE    0xffffff

#define SAMPLE_VO_DEV_DHD0 0                  /* VO's device HD0 */
#define SAMPLE_VO_DEV_DHD1 1                  /* VO's device HD1 */
#define SAMPLE_VO_DEV_DSD0 2                  /* VO's device SD0 */
#define SAMPLE_VO_DEV_UHD  SAMPLE_VO_DEV_DHD0 /* VO's ultra HD device:HD0 */
#define SAMPLE_VO_DEV_HD   SAMPLE_VO_DEV_DHD1 /* VO's HD device:HD1 */
#define SAMPLE_VO_LAYER_VHD0 0
#define SAMPLE_VO_LAYER_VHD1 1
#define SAMPLE_VO_LAYER_VHD2 2
#define SAMPLE_VO_LAYER_PIP  SAMPLE_VO_LAYER_VHD2
#define SAMPLE_RGN_HANDLE_NUM_MAX 16
#define SAMPLE_RGN_HANDLE_NUM_MIN 1

#define SAMPLE_AUDIO_EXTERN_AI_DEV 0
#define SAMPLE_AUDIO_EXTERN_AO_DEV 0
#define SAMPLE_AUDIO_INNER_AI_DEV 0
#define SAMPLE_AUDIO_INNER_AO_DEV 0
#define SAMPLE_AUDIO_INNER_HDMI_AO_DEV 1

#define SAMPLE_AUDIO_POINT_NUM_PER_FRAME 480

#define WDR_MAX_PIPE_NUM 4

#define CHN_NUM_PRE_DEV            4
#define SECOND_CHN_OFFSET_2MUX     2

#define D1_WIDTH            720
#define D1_HEIGHT_PAL       576
#define D1_HEIGHT_NTSC      480

#define _960H_WIDTH         960
#define _960H_HEIGHT_PAL    576
#define _960H_HEIGHT_NTSC   480

#define HD_WIDTH            1280
#define HD_HEIGHT           720

#define FHD_WIDTH           1920
#define FHD_HEIGHT          1080

#define _4K_WIDTH           3840
#define _4K_HEIGHT          2160

#define WIDTH_2688          2688
#define WIDTH_2592          2592
#define HEIGHT_1520         1520

#define AD_NVP6158 0
#define AD_TP2856 1

#define SAMPLE_AD_TYPE AD_TP2856

#define NVP6158_FILE "/dev/nc_vdec"
#define TP2856_FILE "/dev/tp2802dev"
#define TP2828_FILE "/dev/tp2823dev"

#define ACODEC_FILE "/dev/acodec"

#define ES8388_FILE "/dev/es8388"
#define ES8388_CHIP_ID 0

#define VO_LT8618SX 0
#define LT8618SX_DEV_NAME "/dev/lt8618sx"

#define VO_MIPI_SUPPORT 1
#define MIPI_TX_DEV_NAME "/dev/ot_mipi_tx"

#define SAMPLE_FRAME_BUF_RATIO_MAX 100
#define SAMPLE_FRAME_BUF_RATIO_MIN 70

#define minor_chn(vi_chn) ((vi_chn) + 1)

#define sample_pause() \
    do { \
        printf("---------------press enter key to exit!---------------\n"); \
        getchar(); \
    } while (0)

#define sample_print(fmt...) \
    do { \
        printf("[%s]-%d: ", __FUNCTION__, __LINE__); \
        printf(fmt); \
    } while (0)

#define check_null_ptr_return(ptr) \
    do { \
        if ((ptr) == TD_NULL) { \
            printf("func:%s,line:%d, NULL pointer\n", __FUNCTION__, __LINE__); \
            return TD_FAILURE; \
        } \
    } while (0)

#define check_chn_return(express, chn, name) \
    do { \
        td_s32 ret_ = (express); \
        if (ret_ != TD_SUCCESS) { \
            printf("\033[0;31m%s chn %d failed at %s: LINE: %d with %#x!\033[0;39m\n", \
                   (name), (chn), __FUNCTION__, __LINE__, ret_); \
            fflush(stdout); \
            return ret_; \
        } \
    } while (0)

#define check_return(express, name) \
    do { \
        td_s32 ret_ = (express); \
        if (ret_ != TD_SUCCESS) { \
            printf("\033[0;31m%s failed at %s: LINE: %d with %#x!\033[0;39m\n", \
                   (name), __FUNCTION__, __LINE__, ret_); \
            return ret_; \
        } \
    } while (0)

#define rgn_check_handle_num_return(handle_num) \
    do { \
        if (((handle_num) < SAMPLE_RGN_HANDLE_NUM_MIN) || ((handle_num) > SAMPLE_RGN_HANDLE_NUM_MAX)) { \
            sample_print("handle_num(%d) should be in [%d, %d].\n", \
                (handle_num), SAMPLE_RGN_HANDLE_NUM_MIN, SAMPLE_RGN_HANDLE_NUM_MAX); \
            return TD_FAILURE; \
        } \
    } while (0)

typedef enum {
    PIC_CIF,
    PIC_360P,    /* 640 * 360 */
    PIC_D1_PAL,  /* 720 * 576 */
    PIC_D1_NTSC, /* 720 * 480 */
    PIC_960H,      /* 960 * 576 */
    PIC_720P,    /* 1280 * 720 */
    PIC_1080P,   /* 1920 * 1080 */
    PIC_480P,
    PIC_576P,
    PIC_800X600,
    PIC_1024X768,
    PIC_1280X1024,
    PIC_1366X768,
    PIC_1440X900,
    PIC_1280X800,
    PIC_1600X1200,
    PIC_1680X1050,
    PIC_1920X1200,
    PIC_640X480,
    PIC_1920X2160,
    PIC_2560X1440,
    PIC_2560X1600,
    PIC_2592X1520,
    PIC_2592X1944,
    PIC_2688X1520,
    PIC_3840X2160,
    PIC_4096X2160,
    PIC_3000X3000,
    PIC_4000X3000,
    PIC_6080X2800,
    PIC_7680X4320,
    PIC_3840X8640,
    PIC_BUTT
} ot_pic_size;

typedef enum {
    HY_S0603_MIPI_8M_30FPS_12BIT,
    HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1,
    OV_OS08A20_MIPI_8M_30FPS_12BIT,
    OV_OS08A20_MIPI_8M_30FPS_12BIT_WDR2TO1,
    SC450AI_MIPI_4M_30FPS_10BIT,
    SC450AI_MIPI_4M_30FPS_10BIT_WDR2TO1,
    SNS_TYPE_BUTT,
} sample_sns_type;

typedef struct {
    sample_sns_type sns_type;
    td_u32          sns_clk_src;
    td_u32          sns_rst_src;
    td_u32          bus_id;
} sample_sns_info;

typedef struct {
    td_s32             mipi_dev;
    lane_divide_mode_t divide_mode;
    combo_dev_attr_t   combo_dev_attr;
    ext_data_type_t    ext_data_type_attr;
} sample_mipi_info;

typedef struct {
    ot_vi_dev      vi_dev;
    ot_vi_dev_attr dev_attr;
    ot_vi_bas_attr bas_attr;
} sample_vi_dev_info;

typedef struct {
    ot_isp_pub_attr isp_pub_attr;
} sample_isp_info;

typedef struct {
    td_u32                    grp_num;
    ot_vi_grp                 fusion_grp[OT_VI_MAX_WDR_FUSION_GRP_NUM];
    ot_vi_wdr_fusion_grp_attr fusion_grp_attr[OT_VI_MAX_WDR_FUSION_GRP_NUM];
} sample_vi_grp_info;

typedef struct {
    ot_vi_chn      vi_chn;
    ot_vi_chn_attr chn_attr;
} sample_vi_chn_info;

typedef struct {
    ot_vi_pipe_attr    pipe_attr;

    td_bool            pipe_need_start;
    td_bool            isp_need_run;
    sample_isp_info    isp_info;

    td_u32             chn_num;
    sample_vi_chn_info chn_info[OT_VI_MAX_PHYS_CHN_NUM];
} sample_vi_pipe_info;

typedef struct {
    sample_sns_info     sns_info;
    sample_mipi_info    mipi_info;
    sample_vi_dev_info  dev_info;
    ot_vi_bind_pipe     bind_pipe;
    sample_vi_grp_info  grp_info;
    sample_vi_pipe_info pipe_info[OT_VI_MAX_PHYS_PIPE_NUM];
} sample_vi_cfg;

typedef struct {
    ot_vb_blk           vb_blk;
    td_u32              blk_size;
    ot_video_frame_info frame_info;
} sample_vi_user_frame_info;

typedef struct {
    ot_size          size;
    ot_pixel_format  pixel_format;
    ot_video_format  video_format;
    ot_compress_mode compress_mode;
    ot_dynamic_range dynamic_range;
} sample_vi_get_frame_vb_cfg;

typedef struct {
    td_u32           threshold;
    td_u32           frame_num;
    ot_isp_fpn_type  fpn_type;
    ot_pixel_format  pixel_format;
    ot_compress_mode compress_mode;
} sample_vi_fpn_calibration_cfg;

typedef struct {
    ot_op_mode                op_mode;
    ot_isp_fpn_type           fpn_type;
    td_u32                    strength;
    ot_pixel_format           pixel_format;
    ot_compress_mode          compress_mode;
    sample_vi_user_frame_info user_frame_info;
} sample_vi_fpn_correction_cfg;

typedef struct {
    td_u32      offset;
} sample_scene_fpn_offset_cfg;

typedef enum {
    VI_USER_PIC_FRAME = 0,
    VI_USER_PIC_BGCOLOR,
} sample_vi_user_pic_type;

typedef enum {
    VO_MODE_1MUX = 0,
    VO_MODE_2MUX,
    VO_MODE_4MUX,
    VO_MODE_8MUX,
    VO_MODE_9MUX,
    VO_MODE_16MUX,
    VO_MODE_25MUX,
    VO_MODE_36MUX,
    VO_MODE_49MUX,
    VO_MODE_64MUX,
    VO_MODE_2X4,
    VO_MODE_BUTT
} sample_vo_mode;

typedef enum {
    SAMPLE_RC_CBR = 0,
    SAMPLE_RC_VBR,
    SAMPLE_RC_AVBR,
    SAMPLE_RC_CVBR,
    SAMPLE_RC_QVBR,
    SAMPLE_RC_QPMAP,
    SAMPLE_RC_FIXQP
} sample_rc;

/* structure define */
typedef struct {
    td_bool thread_start;
    ot_venc_chn venc_chn[OT_VENC_MAX_CHN_NUM];
    td_s32 cnt;
    td_bool save_heif;
} sample_venc_getstream_para;

typedef struct {
    td_bool thread_start;
    ot_venc_chn venc_chn[OT_VENC_MAX_CHN_NUM];
    td_s32  cnt;
    ot_vpss_grp vpss_grp;
    ot_vpss_chn      vpss_chn[OT_VENC_MAX_CHN_NUM];
} sample_venc_rateauto_para;

typedef struct {
    td_bool  thread_start;
    ot_vpss_grp vpss_grp;
    ot_venc_chn venc_chn[OT_VENC_MAX_CHN_NUM];
    ot_vpss_chn vpss_chn[OT_VPSS_MAX_PHYS_CHN_NUM];
    ot_size size[OT_VENC_MAX_CHN_NUM];
    td_s32 cnt;
} sample_venc_qpmap_sendframe_para;

typedef struct {
    td_bool thread_start;
    ot_vpss_grp vpss_grp;
    ot_venc_chn venc_chn[OT_VENC_MAX_CHN_NUM];
    ot_vpss_chn vpss_chn[OT_VPSS_MAX_PHYS_CHN_NUM];
    ot_size size[OT_VENC_MAX_CHN_NUM];
    ot_venc_jpeg_roi_attr roi_attr[OT_VENC_MAX_CHN_NUM];
    td_s32 cnt;
} sample_venc_roimap_frame_para;

typedef struct {
    ot_vo_intf_sync intf_sync;
    td_u32 width;
    td_u32 height;
    td_u32 frame_rate;
} sample_vo_sync_info;

typedef struct {
    sample_vo_mode mode;
    td_u32 wnd_num;
    td_u32 square;
    td_u32 row;
    td_u32 col;
} sample_vo_wnd_info;

typedef struct {
    ot_vo_wbc_src_type source_type;
    ot_dynamic_range dynamic_range;
    ot_compress_mode compress_mode;
    td_s32 depth;

    td_s32 vo_wbc;
    ot_vo_wbc_attr wbc_attr;
    ot_vo_wbc_src wbc_source;
    ot_vo_wbc_mode wbc_mode;
} sample_vo_wbc_cfg;

typedef struct {
    /* for layer */
    ot_vo_layer vo_layer;
    ot_vo_intf_sync intf_sync;
    ot_rect display_rect;
    ot_size image_size;
    ot_pixel_format pix_format;

    td_u32 dis_buf_len;
    ot_dynamic_range dst_dynamic_range;

    /* for chn */
    sample_vo_mode vo_mode;
} sample_comm_vo_layer_cfg;

typedef struct {
    /* for device */
    ot_vo_dev vo_dev;
    ot_vo_intf_type vo_intf_type;
    ot_vo_intf_sync intf_sync;
    ot_pic_size pic_size;
    td_u32 bg_color;

    /* for layer */
    ot_pixel_format pix_format;
    ot_rect disp_rect;
    ot_size image_size;
    ot_vo_partition_mode vo_part_mode;
    ot_compress_mode compress_mode;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_H_BODY_PART01_H_ */
