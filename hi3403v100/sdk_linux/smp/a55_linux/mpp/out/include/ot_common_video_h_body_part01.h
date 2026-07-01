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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VIDEO_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VIDEO_H_BODY_PART01_H_


#include "ot_type.h"
#include "ot_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define OT_ISP_BAYER_CHN           4
#define OT_DCF_DRSCRIPTION_LENGTH  32
#define OT_DCF_CAPTURE_TIME_LENGTH 20
#define OT_CFACOLORPLANE           3
#define OT_DNG_NP_SIZE             6
#define OT_ISP_WB_GAIN_NUM         4
/* 3*3=9 matrix */
#define OT_ISP_CAP_CCM_NUM         9
#define OT_SRC_LENS_COEF_NUM       9
#define OT_DST_LENS_COEF_NUM       14
#define OT_ISP_WDR_MAX_FRAME_NUM   4

typedef enum  {
    OT_OP_MODE_AUTO   = 0,
    OT_OP_MODE_MANUAL = 1,
    OT_OP_MODE_BUTT
} ot_op_mode;

typedef enum  {
    OT_VB_SRC_COMMON  = 0,
    OT_VB_SRC_MOD  = 1,
    OT_VB_SRC_PRIVATE = 2,
    OT_VB_SRC_USER    = 3,
    OT_VB_SRC_BUTT
} ot_vb_src;

typedef enum  {
    OT_ASPECT_RATIO_NONE   = 0,        /* full screen */
    OT_ASPECT_RATIO_AUTO   = 1,        /* ratio no change, 1:1 */
    OT_ASPECT_RATIO_MANUAL = 2,        /* ratio manual set */
    OT_ASPECT_RATIO_BUTT
} ot_aspect_ratio_type;

typedef enum  {
    OT_VIDEO_FIELD_TOP         = 1,    /* even field */
    OT_VIDEO_FIELD_BOTTOM      = 2,    /* odd field */
    OT_VIDEO_FIELD_INTERLACED  = 3,    /* two interlaced fields */
    OT_VIDEO_FIELD_FRAME       = 4,    /* frame */

    OT_VIDEO_FIELD_BUTT
} ot_video_field;

typedef enum  {
    OT_VIDEO_FORMAT_LINEAR = 0,       /* nature video line */
    OT_VIDEO_FORMAT_TILE_64x16,       /* tile cell: 64pixel x 16line */
    OT_VIDEO_FORMAT_TILE_16x8,        /* tile cell: 16pixel x 8line */
    OT_VIDEO_FORMAT_BUTT
} ot_video_format;

typedef enum  {
    OT_COMPRESS_MODE_NONE = 0,      /* no compress */
    OT_COMPRESS_MODE_SEG,           /* compress unit is 256x1 bytes as a segment. */
    OT_COMPRESS_MODE_SEG_COMPACT,   /* compact compress unit is 256x1 bytes as a segment. */
    OT_COMPRESS_MODE_TILE,          /* compress unit is a tile. */
    OT_COMPRESS_MODE_LINE,          /* compress unit is the whole line. */
    OT_COMPRESS_MODE_FRAME,         /* compress unit is the whole frame. YUV for VPSS(3DNR) */

    OT_COMPRESS_MODE_BUTT
} ot_compress_mode;

typedef enum  {
    OT_VIDEO_DISPLAY_MODE_PREVIEW  = 0,
    OT_VIDEO_DISPLAY_MODE_PLAYBACK = 1,

    OT_VIDEO_DISPLAY_MODE_BUTT
} ot_video_display_mode;

typedef enum  {
    OT_FRAME_FLAG_SNAP_FLASH  = 0x1 << 0,
    OT_FRAME_FLAG_SNAP_CUR    = 0x1 << 1,
    OT_FRAME_FLAG_SNAP_REF    = 0x1 << 2,
    OT_FRAME_FLAG_SNAP_END    = 0x1 << 3,
    OT_FRAME_FLAG_MIRROR      = 0x1 << 4,
    OT_FRAME_FLAG_FLIP        = 0x1 << 5,
    OT_FRAME_FLAG_DGAIN_BYPASS = 0x1 << 6,
    OT_FRAME_FLAG_BUTT
} ot_frame_flag;

typedef enum  {
    OT_ISP_INNER_FLAG_DGAIN_FORWARD = 0x1 << 0,
    OT_ISP_INNER_FLAG_BUTT
} ot_isp_inner_flag;

typedef enum  {
    OT_COLOR_GAMUT_BT601 = 0,
    OT_COLOR_GAMUT_BT709,
    OT_COLOR_GAMUT_BT2020,
    OT_COLOR_GAMUT_USER,
    OT_COLOR_GAMUT_BUTT
} ot_color_gamut;

/* we ONLY define picture format used, all unused will be deleted! */
typedef enum {
    OT_PIXEL_FORMAT_RGB_444 = 0,
    OT_PIXEL_FORMAT_RGB_555,
    OT_PIXEL_FORMAT_RGB_565,
    OT_PIXEL_FORMAT_RGB_888,

    OT_PIXEL_FORMAT_BGR_444,
    OT_PIXEL_FORMAT_BGR_555,
    OT_PIXEL_FORMAT_BGR_565,
    OT_PIXEL_FORMAT_BGR_888,

    OT_PIXEL_FORMAT_ARGB_1555,
    OT_PIXEL_FORMAT_ARGB_4444,
    OT_PIXEL_FORMAT_ARGB_8565,
    OT_PIXEL_FORMAT_ARGB_8888,
    OT_PIXEL_FORMAT_ARGB_2BPP,
    OT_PIXEL_FORMAT_ARGB_CLUT2,
    OT_PIXEL_FORMAT_ARGB_CLUT4,

    OT_PIXEL_FORMAT_ABGR_1555,
    OT_PIXEL_FORMAT_ABGR_4444,
    OT_PIXEL_FORMAT_ABGR_8565,
    OT_PIXEL_FORMAT_ABGR_8888,

    OT_PIXEL_FORMAT_RGB_BAYER_8BPP,
    OT_PIXEL_FORMAT_RGB_BAYER_10BPP,
    OT_PIXEL_FORMAT_RGB_BAYER_12BPP,
    OT_PIXEL_FORMAT_RGB_BAYER_14BPP,
    OT_PIXEL_FORMAT_RGB_BAYER_16BPP,

    OT_PIXEL_FORMAT_YVU_PLANAR_422,
    OT_PIXEL_FORMAT_YVU_PLANAR_420,
    OT_PIXEL_FORMAT_YVU_PLANAR_444,

    OT_PIXEL_FORMAT_YVU_SEMIPLANAR_422,
    OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    OT_PIXEL_FORMAT_YVU_SEMIPLANAR_444,

    OT_PIXEL_FORMAT_YUV_SEMIPLANAR_422,
    OT_PIXEL_FORMAT_YUV_SEMIPLANAR_420,
    OT_PIXEL_FORMAT_YUV_SEMIPLANAR_444,

    OT_PIXEL_FORMAT_YUYV_PACKAGE_422,
    OT_PIXEL_FORMAT_YVYU_PACKAGE_422,
    OT_PIXEL_FORMAT_UYVY_PACKAGE_422,
    OT_PIXEL_FORMAT_VYUY_PACKAGE_422,
    OT_PIXEL_FORMAT_YYUV_PACKAGE_422,
    OT_PIXEL_FORMAT_YYVU_PACKAGE_422,
    OT_PIXEL_FORMAT_UVYY_PACKAGE_422,
    OT_PIXEL_FORMAT_VUYY_PACKAGE_422,
    OT_PIXEL_FORMAT_VY1UY0_PACKAGE_422,

    OT_PIXEL_FORMAT_YUV_400,
    OT_PIXEL_FORMAT_UV_420,

    /* SVP data format */
    OT_PIXEL_FORMAT_BGR_888_PLANAR,
    OT_PIXEL_FORMAT_HSV_888_PACKAGE,
    OT_PIXEL_FORMAT_HSV_888_PLANAR,
    OT_PIXEL_FORMAT_LAB_888_PACKAGE,
    OT_PIXEL_FORMAT_LAB_888_PLANAR,
    OT_PIXEL_FORMAT_S8C1,
    OT_PIXEL_FORMAT_S8C2_PACKAGE,
    OT_PIXEL_FORMAT_S8C2_PLANAR,
    OT_PIXEL_FORMAT_S8C3_PLANAR,
    OT_PIXEL_FORMAT_S16C1,
    OT_PIXEL_FORMAT_U8C1,
    OT_PIXEL_FORMAT_U16C1,
    OT_PIXEL_FORMAT_S32C1,
    OT_PIXEL_FORMAT_U32C1,
    OT_PIXEL_FORMAT_U64C1,
    OT_PIXEL_FORMAT_S64C1,

    OT_PIXEL_FORMAT_BUTT
} ot_pixel_format;

typedef enum  {
    OT_DYNAMIC_RANGE_SDR8 = 0,
    OT_DYNAMIC_RANGE_SDR10,
    OT_DYNAMIC_RANGE_HDR10,
    OT_DYNAMIC_RANGE_HLG,
    OT_DYNAMIC_RANGE_SLF,
    OT_DYNAMIC_RANGE_XDR,
    OT_DYNAMIC_RANGE_BUTT
} ot_dynamic_range;

typedef enum  {
    OT_DATA_BIT_WIDTH_8 = 0,
    OT_DATA_BIT_WIDTH_10,
    OT_DATA_BIT_WIDTH_12,
    OT_DATA_BIT_WIDTH_14,
    OT_DATA_BIT_WIDTH_16,
    OT_DATA_BIT_WIDTH_BUTT
} ot_data_bit_width;

typedef struct {
    td_u32 top_width;
    td_u32 bottom_width;
    td_u32 left_width;
    td_u32 right_width;
    td_u32 color;
} ot_border;

typedef struct {
    td_s32 x;
    td_s32 y;
} ot_point;

typedef struct {
    td_u32 width;
    td_u32 height;
} ot_size;

typedef struct {
    td_s32 x;
    td_s32 y;
    td_u32 width;
    td_u32 height;
} ot_rect;

typedef enum {
    OT_COORD_ABS = 0,                          /* Absolute coordinate. */
    OT_COORD_RATIO,                            /* Ratio coordinate. */
    OT_COORD_BUTT
} ot_coord;

typedef enum {
    OT_COVER_RECT = 0,                         /* Rectangle cover. */
    OT_COVER_QUAD,                             /* Quadrangle cover. */
    OT_COVER_BUTT
} ot_cover_type;

typedef struct {
    td_bool        is_solid;                   /* RW; Solid or hollow cover. */
    td_u32         thick;                      /* RW; Thick of the hollow quadrangle. */
    ot_point       point[OT_QUAD_POINT_NUM];   /* RW; Four points of the quadrangle. */
} ot_quad_cover;

typedef struct {
    ot_cover_type      type;   /* RW; Cover type. */
    union {
        ot_rect        rect;   /* RW; The rectangle area. AUTO:ot_cover_type:OT_COVER_RECT; */
        ot_quad_cover  quad;   /* RW; The quadrangle area.  AUTO:ot_cover_type:OT_COVER_QUAD; */
    };
    td_u32             color;  /* RW; Range: [0,0xFFFFFF]; Color of cover. */
} ot_cover;

typedef enum  {
    OT_MOSAIC_BLK_SIZE_4 = 0, /* block size 4 * 4 of MOSAIC */
    OT_MOSAIC_BLK_SIZE_8,     /* block size 8 * 8 of MOSAIC */
    OT_MOSAIC_BLK_SIZE_16,    /* block size 16 * 16 of MOSAIC */
    OT_MOSAIC_BLK_SIZE_32,    /* block size 32 * 32 of MOSAIC */
    OT_MOSAIC_BLK_SIZE_64,    /* block size 64 * 64 of MOSAIC */
    OT_MOSAIC_BLK_SIZE_128,   /* block size 128 * 128 of MOSAIC */
    OT_MOSAIC_BLK_SIZE_BUTT
} ot_mosaic_blk_size;

typedef struct {
    ot_mosaic_blk_size blk_size;
    ot_rect            rect;   /* RW; The rectangle area. */
} ot_mosaic;

typedef struct {
    td_bool enable;
    ot_rect  rect;
} ot_crop_info;

typedef struct {
    td_s32  src_frame_rate;        /* RW; source frame rate */
    td_s32  dst_frame_rate;        /* RW; dest frame rate */
} ot_frame_rate_ctrl;

typedef struct {
    ot_aspect_ratio_type mode;          /* aspect ratio mode: none/auto/manual */
    td_u32         bg_color;      /* background color, RGB 888 */
    ot_rect         video_rect;     /* valid in ASPECT_RATIO_MANUAL mode */
} ot_aspect_ratio;

typedef struct {
    td_s16              offset_top;           /* top offset of show area */
    td_s16              offset_left;          /* left offset of show area */
    td_s16              offset_bottom;        /* bottom offset of show area */
    td_s16              offset_right;         /* right offset of show area */

    td_u32              max_luma;             /* for HDR */
    td_u32              min_luma;             /* for HDR */
    td_u64              private_data[OT_MAX_PRIVATE_DATA_NUM];
} ot_video_supplement_misc;

typedef struct {
    td_phys_addr_t   misc_info_phys_addr;        /* default allocated buffer */
    td_phys_addr_t   jpeg_dcf_phys_addr;
    td_phys_addr_t   isp_info_phys_addr;
    td_phys_addr_t   low_delay_phys_addr;
    td_phys_addr_t   bnr_rnt_phys_addr;
    td_phys_addr_t   motion_data_phys_addr;
    td_phys_addr_t   frame_dng_phys_addr;

    td_void* ATTRIBUTE misc_info_virt_addr;       /* misc info */
    td_void* ATTRIBUTE jpeg_dcf_virt_addr;        /* jpeg_dcf, used in JPEG DCF */
    td_void* ATTRIBUTE isp_info_virt_addr;        /* isp_frame_info, used in ISP debug, when get raw and send raw */
    td_void* ATTRIBUTE low_delay_virt_addr;       /* used in low delay */
    td_void* ATTRIBUTE bnr_mot_virt_addr;         /* used for 3dnr from bnr mot */
    td_void* ATTRIBUTE motion_data_virt_addr;     /* vpss 3dnr use: gme motion data, filter motion data, gyro data. */
    td_void* ATTRIBUTE frame_dng_virt_addr;
} ot_video_supplement;

typedef struct {
    td_u32              width;
    td_u32              height;
    ot_video_field      field;
    ot_pixel_format     pixel_format;
    ot_video_format     video_format;
    ot_compress_mode    compress_mode;
    ot_dynamic_range    dynamic_range;
    ot_color_gamut      color_gamut;

    td_u32              header_stride[OT_MAX_COLOR_COMPONENT];
    td_u32              stride[OT_MAX_COLOR_COMPONENT];

    td_phys_addr_t      header_phys_addr[OT_MAX_COLOR_COMPONENT];
    td_phys_addr_t      phys_addr[OT_MAX_COLOR_COMPONENT];
    td_void* ATTRIBUTE  header_virt_addr[OT_MAX_COLOR_COMPONENT];
    td_void* ATTRIBUTE  virt_addr[OT_MAX_COLOR_COMPONENT];

    td_u32              time_ref;
    td_u64              pts;

    td_u64              user_data[OT_MAX_USER_DATA_NUM];
    td_u32              frame_flag;     /* frame_flag, can be OR operation. */
    ot_video_supplement supplement;
} ot_video_frame;

typedef struct {
    ot_video_frame video_frame;
    td_u32        pool_id;
    ot_mod_id      mod_id;
} ot_video_frame_info;

typedef struct {
    td_u32 vb_size;

    td_u32 head_stride;
    td_u32 head_size;
    td_u32 head_y_size;

    td_u32 main_stride;
    td_u32 main_size;
    td_u32 main_y_size;
} ot_vb_calc_cfg;

typedef struct {
    ot_pixel_format pixel_format;  /* bitmap's pixel format */
    td_u32 width;               /* bitmap's width */
    td_u32 height;              /* bitmap's height */
    td_void* ATTRIBUTE data;      /* address of bitmap's data */
} ot_bmp;

typedef struct {
    td_bool    aspect;      /* RW;range: [0, 1];whether aspect ration  is keep */
    td_s32     x_ratio;     /* RW; range: [0, 100]; field angle ration of  horizontal,valid when aspect=0. */
    td_s32     y_ratio;     /* RW; range: [0, 100]; field angle ration of  vertical,valid when aspect=0. */
    td_s32     xy_ratio;    /* RW; range: [0, 100]; field angle ration of  all,valid when aspect=1. */
     /* RW; range: [-511, 511]; horizontal offset of the image distortion center relative to image center. */
    td_s32 center_x_offset;
     /* RW; range: [-511, 511]; vertical offset of the image distortion center relative to image center. */
    td_s32 center_y_offset;
    /* RW; range: [-300, 500]; LDC distortion ratio.when spread on,distortion_ratio range should be [0, 500] */
    td_s32 distortion_ratio;
} ot_ldc_v1_attr;

typedef struct {
    /* RW; range: [6400, 117341700]; focal length in horizontal direction, with 2 decimal numbers */
    td_s32 focal_len_x;
    /* RW; range: [6400, 117341700]; focal length in vertical direction, with 2 decimal numbers */
    td_s32 focal_len_y;
    /* RW; range: [35*width, 65*width]; coordinate of image center, with 2 decimal numbers */
    td_s32 coord_shift_x;
    /* RW; range: [35*height, 65*height]; Y coordinate of image center, with 2 decimal numbers */
    td_s32 coord_shift_y;
    /* RW; lens distortion coefficients of the source image, with 5 decimal numbers */
    td_s32 src_calibration_ratio[OT_SRC_LENS_COEF_NUM];
    /* RW; lens distortion coefficients, with 5 decimal numbers */
    td_s32 dst_calibration_ratio[OT_DST_LENS_COEF_NUM];
    /* RW; range: [0, 1048576]; max undistorted distance before 3rd polynomial drop, with 16bits decimal */
    td_s32 max_du;
} ot_ldc_v2_attr;

typedef struct {
    td_bool    aspect;      /* RW;range: [0, 1];whether aspect ration  is keep */
    td_s32     x_ratio;     /* RW; range: [0, 100]; field angle ration of  horizontal,valid when aspect=0. */
    td_s32     y_ratio;     /* RW; range: [0, 100]; field angle ration of  vertical,valid when aspect=0. */
    td_s32     xy_ratio;    /* RW; range: [0, 100]; field angle ration of  all,valid when aspect=1. */
    /* RW; range: [6400, 3276700]; focal length in horizontal direction, with 2 decimal numbers */
    td_s32 focal_len_x;
    /* RW; range: [6400, 3276700]; focal length in vertical direction, with 2 decimal numbers */
    td_s32 focal_len_y;
    /* RW; range: [35*width, 65*width]; coordinate of image center, with 2 decimal numbers */
    td_s32 coord_shift_x;
    /* RW; range: [35*height, 65*height]; Y coordinate of image center, with 2 decimal numbers */
    td_s32 coord_shift_y;
    /* RW; lens distortion coefficients of the source image, with 5 decimal numbers */
    td_s32 src_calibration_ratio[OT_SRC_LENS_COEF_NUM];
    /* RW; lens distortion coefficients of the source image (for interpolation), with 5 decimal numbers */
    td_s32 src_calibration_ratio_next[OT_SRC_LENS_COEF_NUM];
    td_u32 coef_intp_ratio; /* RW; range: [0, 32768]; interpolation ratio between two coefficient sets, 15bits */
} ot_ldc_v3_attr;

typedef enum {
    OT_LDC_V1 = 0,
    OT_LDC_V2 = 1,
    OT_LDC_V3 = 2,

    OT_LDC_VERSION_BUTT
} ot_ldc_version;

typedef struct {
    td_bool enable;                 /* RW; Range [0,1];Whether LDC is enable */
    ot_ldc_version ldc_version;     /* RW; Version of LDC */
    union {
        ot_ldc_v1_attr ldc_v1_attr; /* RW; Attribute of LDC v1. AUTO:ot_ldc_version:OT_LDC_V1; */
        ot_ldc_v2_attr ldc_v2_attr; /* RW; Attribute of LDC v2. AUTO:ot_ldc_version:OT_LDC_V2; */
        ot_ldc_v3_attr ldc_v3_attr; /* RW; Attribute of LDC v3. AUTO:ot_ldc_version:OT_LDC_V3; */
    };
} ot_ldc_attr;

/* Angle of rotation */
typedef enum  {


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VIDEO_H_BODY_PART01_H_ */
