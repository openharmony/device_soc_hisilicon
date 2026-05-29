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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VENC_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VENC_H_BODY_PART01_H_


#include "ot_common_rc.h"
#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_vb.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/* invalid channel ID */
#define OT_ERR_VENC_INVALID_CHN_ID OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_INVALID_CHN_ID)
/* at least one parameter is illagal ,eg, an illegal enumeration value  */
#define OT_ERR_VENC_ILLEGAL_PARAM OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_ILLEGAL_PARAM)
/* channel exists */
#define OT_ERR_VENC_EXIST         OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_EXIST)
/* channel exists */
#define OT_ERR_VENC_UNEXIST       OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_UNEXIST)
/* using a NULL pointer */
#define OT_ERR_VENC_NULL_PTR      OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define OT_ERR_VENC_NOT_CFG    OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_CFG)
/* operation is not supported by NOW */
#define OT_ERR_VENC_NOT_SUPPORT   OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change statuses attribute */
#define OT_ERR_VENC_NOT_PERM      OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_PERM)
/* failure caused by malloc memory */
#define OT_ERR_VENC_NO_MEM         OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_NO_MEM)
/* failure caused by malloc buffer */
#define OT_ERR_VENC_NO_BUF         OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_NO_BUF)
/* no data in buffer */
#define OT_ERR_VENC_BUF_EMPTY     OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_BUF_EMPTY)
/* no buffer for new data */
#define OT_ERR_VENC_BUF_FULL      OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded */
#define OT_ERR_VENC_SYS_NOT_READY  OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_READY)
/* system is busy */
#define OT_ERR_VENC_BUSY          OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_BUSY)
/* buf size not enough */
#define OT_ERR_VENC_SIZE_NOT_ENOUGH OT_DEFINE_ERR(OT_ID_VENC, OT_ERR_LEVEL_ERROR, OT_ERR_SIZE_NOT_ENOUGH)

#define OT_VENC_MAX_PACK_INFO_NUM 8
#define OT_VENC_MAX_MPF_NUM 2
#define OT_VENC_PRORES_MAX_ID_CHAR_NUM 4
#define OT_VENC_MAX_HIERARCHY_NUM 4
#define OT_VENC_SCALING_LIST_SIZE_16 16
#define OT_VENC_SCALING_LIST_SIZE_64 64
#define OT_VENC_JPEG_QT_COEF_NUM 64
#define OT_VENC_MJPEG_QT_COEF_NUM OT_VENC_JPEG_QT_COEF_NUM
#define OT_VENC_QP_HIST_NUM 52
#define OT_VENC_MAX_ROI_NUM 8
#define OT_VENC_MAX_JPEG_ROI_NUM 16
#define OT_VENC_MAX_SSE_NUM 8
#define OT_VENC_MD_THRESHOLD_NUM 3
#define OT_VENC_MD_LEVEL_NUM 4
#define OT_VENC_MAX_SVC_RECT_NUM  32
#define OT_VENC_MAX_MOSAIC_RECT_NUM 200
#define OT_VENC_MAX_FRAME_NUM 2
#define OT_VENC_MB_SIZE 16

typedef enum {
    OT_VENC_RECV_SRC0 = 0,
    OT_VENC_RECV_SRC1,
    OT_VENC_RECV_SRC2,
    OT_VENC_RECV_SRC3,
    OT_VENC_RECV_BUTT
} ot_venc_recv_src;

typedef enum {
    OT_VENC_H264_NALU_B_SLICE   = 0,
    OT_VENC_H264_NALU_P_SLICE   = 1,
    OT_VENC_H264_NALU_I_SLICE   = 2,
    OT_VENC_H264_NALU_IDR_SLICE = 5,
    OT_VENC_H264_NALU_SEI       = 6,
    OT_VENC_H264_NALU_SPS       = 7,
    OT_VENC_H264_NALU_PPS       = 8,
    OT_VENC_H264_NALU_BUTT
} ot_venc_h264_nalu_type;

typedef enum {
    OT_VENC_H265_NALU_B_SLICE   = 0,
    OT_VENC_H265_NALU_P_SLICE   = 1,
    OT_VENC_H265_NALU_I_SLICE   = 2,
    OT_VENC_H265_NALU_IDR_SLICE = 19,
    OT_VENC_H265_NALU_VPS       = 32,
    OT_VENC_H265_NALU_SPS       = 33,
    OT_VENC_H265_NALU_PPS       = 34,
    OT_VENC_H265_NALU_SEI       = 39,
    OT_VENC_H265_NALU_ENHANCE   = 64,
    OT_VENC_H265_NALU_BUTT
} ot_venc_h265_nalu_type;

typedef enum {
    OT_VENC_H264_REF_SLICE_FOR_1X = 1,
    OT_VENC_H264_REF_SLICE_FOR_2X = 2,
    OT_VENC_H264_REF_SLICE_FOR_4X = 5,
    OT_VENC_H264_REF_SLICE_FOR_BUTT
} ot_venc_h264_ref_slice_type;

typedef enum {
    OT_VENC_JPEG_PACK_ECS = 5,
    OT_VENC_JPEG_PACK_APP = 6,
    OT_VENC_JPEG_PACK_VDO = 7,
    OT_VENC_JPEG_PACK_PIC = 8,
    OT_VENC_JPEG_PACK_DCF = 9,
    OT_VENC_JPEG_PACK_DCF_PIC = 10,
    OT_VENC_JPEG_PACK_BUTT
} ot_venc_jpege_pack_type;

typedef enum {
    OT_VENC_PRORES_PACK_PIC = 1,
    OT_VENC_PRORES_PACK_BUTT
} ot_venc_prores_pack_type;

typedef enum {
    OT_VENC_MOSAIC_MODE_RECT = 0, /* user input mosaic rect */
    OT_VENC_MOSAIC_MODE_MAP,     /* user input mosaic map */
    OT_VENC_MOSAIC_MODE_BUTT
} ot_venc_mosaic_mode;

typedef union {
    ot_venc_h264_nalu_type h264_type;  /* AUTO:ot_payload_type:OT_PT_H264; */
    ot_venc_jpege_pack_type jpeg_type; /* AUTO:ot_payload_type:OT_PT_JPEG; */
    ot_venc_h265_nalu_type h265_type; /* AUTO:ot_payload_type:OT_PT_H265; */
    ot_venc_prores_pack_type prores_type; /* AUTO:ot_payload_type:OT_PT_PRORES; */
} ot_venc_data_type;

typedef struct {
    ot_venc_data_type pack_type;
    td_u32 pack_offset;
    td_u32 pack_len;
} ot_venc_pack_info;

typedef struct {
    td_phys_addr_t phys_addr;
    td_u8 ATTRIBUTE *addr;
    td_u32 ATTRIBUTE len;
    td_u64 pts;
    td_bool is_frame_end;
    ot_venc_data_type data_type;
    td_u32 offset;
    td_u32 data_num;
    ot_venc_pack_info pack_info[OT_VENC_MAX_PACK_INFO_NUM];
} ot_venc_pack;

/* Defines the frame type and reference attributes of the H.264 frame skipping reference streams */
typedef enum {
    OT_VENC_BASE_IDR_SLICE = 0,                     /* the Idr frame at Base layer */
    OT_VENC_BASE_P_SLICE_REF_TO_IDR,                /* the P frame at Base layer, referenced by other frames
                                                     * at Base layer and reference to Idr frame
                                                     */
    OT_VENC_BASE_P_SLICE_REF_BY_BASE,               /* the P frame at Base layer, referenced by other frames
                                                     * at Base layer
                                                     */
    OT_VENC_BASE_P_SLICE_REF_BY_ENHANCE,            /* the P frame at Base layer, referenced by other frames
                                                     * at Enhance layer
                                                     */
    OT_VENC_ENHANCE_P_SLICE_REF_BY_ENHANCE,         /* the P frame at Enhance layer, referenced by other frames
                                                     * at Enhance layer
                                                     */
    OT_VENC_ENHANCE_P_SLICE_NOT_FOR_REF,            /* the P frame at Enhance layer ,not referenced */
    OT_VENC_P_SLICE_BUTT
} ot_venc_ref_type;

typedef struct {
    td_u32 pic_bytes;
    td_u32 inter16x16_mb_num;
    td_u32 inter8x8_mb_num;
    td_u32 intra16_mb_num;
    td_u32 intra8_mb_num;
    td_u32 intra4_mb_num;
    ot_venc_ref_type ref_type;
    td_u32 update_attr_cnt;
    td_u32 start_qp;
    td_u32 mean_qp;
    td_bool is_p_skip;
} ot_venc_h264_stream_info;

typedef struct {
    td_u32 pic_bytes;
    td_u32 inter64x64_cu_num;
    td_u32 inter32x32_cu_num;
    td_u32 inter16x16_cu_num;
    td_u32 inter8x8_cu_num;
    td_u32 intra32x32_cu_num;
    td_u32 intra16x16_cu_num;
    td_u32 intra8x8_cu_num;
    td_u32 intra4x4_cu_num;
    ot_venc_ref_type ref_type;
    td_u32 update_attr_cnt;
    td_u32 start_qp;
    td_u32 mean_qp;
    td_bool is_p_skip;
} ot_venc_h265_stream_info;

typedef struct {
    td_bool enable;
    td_u64 sse_val;
} ot_venc_sse_info;

typedef struct {
    td_u32 residual_bits;
    td_u32 head_bits;
    td_u32 madi_val;
    td_u32 madp_val;
    td_double psnr_val;
    td_u32 sse_lcu_cnt;
    td_u64 sse_sum;
    ot_venc_sse_info sse_info[OT_VENC_MAX_SSE_NUM];
    td_u32 qp_hist[OT_VENC_QP_HIST_NUM];
    td_u32 move_scene16x16_num;
    td_u32 move_scene_bits;
    td_u32 deblur_near_bg_num;
    td_u32 deblur_far_bg_num;
} ot_venc_h264_adv_stream_info;

typedef struct {
    td_u32             residual_bits;
    td_u32             head_bits;
    td_u32             madi_val;
    td_u32             madp_val;
    td_double          psnr_val;
    td_u32             sse_lcu_cnt;
    td_u64             sse_sum;
    ot_venc_sse_info   sse_info[OT_VENC_MAX_SSE_NUM];
    td_u32             qp_hist[OT_VENC_QP_HIST_NUM];
    td_u32             move_scene32x32_num;
    td_u32             move_scene_bits;
    td_u32             deblur_near_bg_num;
    td_u32             deblur_far_bg_num;
} ot_venc_h265_adv_stream_info;

typedef struct {
    td_u32 pic_bytes;
    td_u32 update_attr_cnt;
} ot_venc_prores_stream_info;

typedef struct {
    td_u32 pic_bytes;
    td_u32 update_attr_cnt;
    td_u32 qfactor;
} ot_venc_jpeg_stream_info;

typedef struct {
    ot_venc_pack ATTRIBUTE *pack;
    td_u32 ATTRIBUTE pack_cnt;
    td_u32 seq;
    union {
        ot_venc_h264_stream_info   h264_info; /* AUTO:ot_payload_type:OT_PT_H264; */
        ot_venc_jpeg_stream_info   jpeg_info; /* AUTO:ot_payload_type:OT_PT_JPEG; */
        ot_venc_h265_stream_info   h265_info; /* AUTO:ot_payload_type:OT_PT_H265; */
        ot_venc_prores_stream_info prores_info; /* AUTO:ot_payload_type:OT_PT_PRORES; */
    };
    union {
        ot_venc_h264_adv_stream_info h264_adv_info; /* AUTO:ot_payload_type:OT_PT_H264; */
        ot_venc_h265_adv_stream_info h265_adv_info; /* AUTO:ot_payload_type:OT_PT_H265; */
    };
} ot_venc_stream;

typedef struct {
    ot_venc_ref_type ref_type;
    td_u32  pic_bytes;
    td_u32  pic_cnt;
    td_u32  start_qp;
    td_u32  mean_qp;
    td_bool is_p_skip;
    td_u32  residual_bits;
    td_u32  head_bits;
    td_u32  madi_val;
    td_u32  madp_val;
    td_u64  sse_sum;
    td_u32  sse_lcu_cnt;
    td_double psnr_val;
} ot_venc_stream_info;

typedef struct {
    td_u8 large_thumbnail_num;
    ot_size large_thumbnail_size[OT_VENC_MAX_MPF_NUM];
} ot_venc_mpf_cfg;

typedef enum {
    OT_VENC_PIC_RECV_SINGLE = 0,
    OT_VENC_PIC_RECV_MULTI,
    OT_VENC_PIC_RECV_BUTT
} ot_venc_pic_recv_mode;

typedef struct {
    td_bool dcf_en;
    ot_venc_mpf_cfg mpf_cfg;
    ot_venc_pic_recv_mode recv_mode;
} ot_venc_jpeg_attr;

typedef struct {
    td_bool rcn_ref_share_buf_en;
    td_u32 frame_buf_ratio;
} ot_venc_h264_attr;

typedef struct {
    td_bool rcn_ref_share_buf_en;
    td_u32 frame_buf_ratio;
} ot_venc_h265_attr;

typedef enum {
    OT_VENC_PRORES_FRAME_RATE_UNKNOWN = 0,
    OT_VENC_PRORES_FRAME_RATE_23_976,
    OT_VENC_PRORES_FRAME_RATE_24,
    OT_VENC_PRORES_FRAME_RATE_25,
    OT_VENC_PRORES_FRAME_RATE_29_97,
    OT_VENC_PRORES_FRAME_RATE_30,
    OT_VENC_PRORES_FRAME_RATE_50,
    OT_VENC_PRORES_FRAME_RATE_59_94,
    OT_VENC_PRORES_FRAME_RATE_60,
    OT_VENC_PRORES_FRAME_RATE_100,
    OT_VENC_PRORES_FRAME_RATE_119_88,
    OT_VENC_PRORES_FRAME_RATE_120,
    OT_VENC_PRORES_FRAME_RATE_BUTT
} ot_venc_prores_frame_rate;

typedef enum {
    OT_VENC_PRORES_ASPECT_RATIO_UNKNOWN = 0,
    OT_VENC_PRORES_ASPECT_RATIO_SQUARE,
    OT_VENC_PRORES_ASPECT_RATIO_4_3,
    OT_VENC_PRORES_ASPECT_RATIO_16_9,
    OT_VENC_PRORES_ASPECT_RATIO_BUTT
} ot_venc_prores_aspect_ratio;

typedef struct {
    td_char identifier[OT_VENC_PRORES_MAX_ID_CHAR_NUM];
    ot_venc_prores_frame_rate frame_rate_code;
    ot_venc_prores_aspect_ratio aspect_ratio;
} ot_venc_prores_attr;

typedef struct {
    ot_payload_type type;
    td_u32  max_pic_width;
    td_u32  max_pic_height;
    td_u32  buf_size;
    td_u32  profile;
    td_bool is_by_frame;
    td_u32  pic_width;
    td_u32  pic_height;
    union {
        ot_venc_h264_attr h264_attr; /* AUTO:ot_payload_type:OT_PT_H264; */
        ot_venc_h265_attr h265_attr; /* AUTO:ot_payload_type:OT_PT_H265; */
        ot_venc_jpeg_attr  jpeg_attr; /* AUTO:ot_payload_type:OT_PT_JPEG; */
        ot_venc_prores_attr prores_attr; /* AUTO:ot_payload_type:OT_PT_PRORES; */
    };
} ot_venc_attr;

typedef enum {
    OT_VENC_GOP_MODE_NORMAL_P    = 0,
    OT_VENC_GOP_MODE_DUAL_P      = 1,
    OT_VENC_GOP_MODE_SMART_P     = 2,
    OT_VENC_GOP_MODE_ADV_SMART_P = 3,
    OT_VENC_GOP_MODE_BIPRED_B    = 4,
    OT_VENC_GOP_MODE_LOW_DELAY_B = 5,
    OT_VENC_GOP_MODE_BUTT,
} ot_venc_gop_mode;

typedef struct {
    td_s32 ip_qp_delta;
} ot_venc_gop_normal_p;

typedef struct {
    td_u32 sp_interval;
    td_s32 sp_qp_delta;
    td_s32 ip_qp_delta;
} ot_venc_gop_dual_p;

typedef struct {
    td_u32 bg_interval;
    td_s32 bg_qp_delta;
    td_s32 vi_qp_delta;
    td_u32 vi_max_qp;
    td_u32 vi_min_qp;
} ot_venc_gop_smart_p;

typedef struct {
    td_u32 bg_interval;
    td_s32 bg_qp_delta;
    td_s32 vi_qp_delta;
} ot_venc_gop_adv_smart_p;

typedef struct {
    td_u32 b_frame_num;
    td_s32 b_qp_delta;
    td_s32 ip_qp_delta;
} ot_venc_gop_bipred_b;

typedef struct {
    ot_venc_gop_mode gop_mode;
    union {
        ot_venc_gop_normal_p    normal_p; /* AUTO:ot_venc_gop_mode:OT_VENC_GOP_MODE_NORMAL_P; */
        ot_venc_gop_dual_p      dual_p; /* AUTO:ot_venc_gop_mode:OT_VENC_GOP_MODE_DUAL_P; */
        ot_venc_gop_smart_p     smart_p; /* AUTO:ot_venc_gop_mode:OT_VENC_GOP_MODE_SMART_P; */
        ot_venc_gop_adv_smart_p adv_smart_p; /* AUTO:ot_venc_gop_mode:OT_VENC_GOP_MODE_ADV_SMART_P; */
        ot_venc_gop_bipred_b    bipred_b; /* AUTO:ot_venc_gop_mode:OT_VENC_GOP_MODE_BIPRED_B; */
    };
} ot_venc_gop_attr;

typedef struct {
    ot_venc_attr venc_attr;
    ot_venc_rc_attr rc_attr;
    ot_venc_gop_attr gop_attr;
} ot_venc_chn_attr;

typedef struct {
    td_s32 recv_pic_num;
} ot_venc_start_param;

typedef struct {
    td_u32 left_pics;
    td_u32 left_stream_bytes;
    td_u32 left_stream_frames;
    td_u32 cur_packs;
    td_u32 left_recv_pics;
    td_u32 left_enc_pics;
    td_bool is_jpeg_snap_end;
    td_u64 release_pic_pts;
    td_s64 cvbr_bytes_saving;
    ot_venc_stream_info stream_info;
    td_u32 discard_pics;
    td_u32 stream_buf_full_cnt;
} ot_venc_chn_status;

typedef struct {
    td_u32 constrained_intra_pred_flag;
} ot_venc_h264_intra_pred;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VENC_H_BODY_PART01_H_ */
