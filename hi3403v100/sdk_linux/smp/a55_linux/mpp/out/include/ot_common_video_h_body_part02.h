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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VIDEO_H_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VIDEO_H_BODY_PART02_H_

    OT_ROTATION_0   = 0,
    OT_ROTATION_90  = 1,
    OT_ROTATION_180 = 2,
    OT_ROTATION_270 = 3,
    OT_ROTATION_BUTT
} ot_rotation;

typedef enum {
    OT_ROTATION_VIEW_TYPE_ALL      = 0, /* View all source Image,no lose */
    OT_ROTATION_VIEW_TYPE_TYPICAL  = 1, /* View from rotation Image with source size,same lose */
    OT_ROTATION_VIEW_TYPE_INSIDE   = 2, /* View with no black section,all  in dest Image */
    OT_ROTATION_VIEW_TYPE_BUTT,
} ot_rotation_view_type;

typedef struct {
    ot_rotation_view_type view_type; /* RW;range: [0, 2];rotation mode */
    td_u32    angle; /* RW;range: [0,360];rotation angle:[0,360] */
    /* RW;range: [-511, 511];horizontal offset of the image distortion center relative to image center. */
    td_s32    center_x_offset;
    /* RW;range: [-511, 511];vertical offset of the image distortion center relative to image center. */
    td_s32    center_y_offset;
    ot_size   dst_size; /* RW; range: width:[480, 8192],height:[360, 8192]; dest size of any angle rotation */
} ot_free_rotation_attr;

typedef enum {
    OT_ROTATION_ANG_FIXED = 0,  /* Fixed angle. */
    OT_ROTATION_ANG_FREE,       /* Free angle. */
    OT_ROTATION_ANG_FREE_HP,       /* Free high-precision angle. */
    OT_ROTATION_ANG_BUTT
} ot_rotation_type;

typedef struct {
    td_bool             enable;         /* RW; range: [0, 1]; Rotate enable. */
    ot_rotation_type    rotation_type;  /* RW; Rotate type. */
    union {
        /*
         * RW; Fixed rotation attribute.
         * AUTO:ot_rotation_type:OT_ROTATION_ANG_FIXED;
         */
        ot_rotation rotation_fixed;
        /*
         * RW; Free rotation attribute.
         * AUTO:ot_rotation_type:OT_ROTATION_ANG_FREE;
         */
        ot_free_rotation_attr rotation_free;
    };
} ot_rotation_attr;

/* spread correction attribute */
typedef struct {
    /* RW; range: [0, 1];whether enable spread or not, when spread on,ldc distortion_ratio range should be [0, 500] */
    td_bool enable;
    td_u32  spread_coef; /* RW; range: [0, 18];strength coefficient of spread correction */
} ot_spread_attr;

typedef struct {
    td_bool enable; /* RW; Range: [0, 1]; Zoom enable. */
    ot_coord mode;  /* RW; Coordinate mode of zoom. */
    ot_rect rect;   /* RW; Zoom rectangular. */
} ot_zoom_attr;

typedef enum  {
    OT_FRAME_INTERRUPT_START,
    OT_FRAME_INTERRUPT_EARLY,
    OT_FRAME_INTERRUPT_EARLY_END,
    OT_FRAME_INTERRUPT_EARLY_EARLY,
    OT_FRAME_INTERRUPT_BUTT,
} ot_frame_interrupt_type;

typedef struct {
    ot_frame_interrupt_type interrupt_type;
    td_u32 early_line;
} ot_frame_interrupt_attr;

typedef enum {
    OT_DATA_RATE_X1 = 0, /* RW; output 1 pixel per clock */
    OT_DATA_RATE_X2,     /* RW; output 2 pixel per clock */

    OT_DATA_RATE_BUTT
} ot_data_rate;

typedef enum {
    OT_WDR_MODE_NONE = 0,
    OT_WDR_MODE_BUILT_IN,
    OT_WDR_MODE_QUDRA,

    OT_WDR_MODE_2To1_LINE,
    OT_WDR_MODE_2To1_FRAME,

    OT_WDR_MODE_3To1_LINE,
    OT_WDR_MODE_3To1_FRAME,

    OT_WDR_MODE_4To1_LINE,
    OT_WDR_MODE_4To1_FRAME,

    OT_WDR_MODE_BUTT,
} ot_wdr_mode;

typedef enum {
    OT_CORNER_RECT_TYPE_CORNER = 0,
    OT_CORNER_RECT_TYPE_FULL_LINE,
    OT_CORNER_RECT_TYPE_BUTT
}ot_corner_rect_type;

typedef struct {
    ot_rect      rect;
    td_u32       hor_len;
    td_u32       ver_len;
    td_u32       thick;
} ot_corner_rect;

typedef struct {
    ot_corner_rect_type    corner_rect_type;
    td_u32                 color;
} ot_corner_rect_attr;

typedef struct {
    td_u8 image_description[OT_DCF_DRSCRIPTION_LENGTH];
    /* shows manufacturer of digital cameras */
    td_u8 make[OT_DCF_DRSCRIPTION_LENGTH];
    /* shows model number of digital cameras */
    td_u8 model[OT_DCF_DRSCRIPTION_LENGTH];
    /* shows firmware (internal software of digital cameras) version number */
    td_u8 software[OT_DCF_DRSCRIPTION_LENGTH];
    /*
     * light source, actually this means white balance setting. '0' means unknown, '1' daylight,
     * '2' fluorescent, '3' tungsten, '10' flash, '17' standard light A, '18' standard light B,
     * '19' standard light C, '20' D55, '21' D65, '22' D75, '255' other.
     */
    td_u8 light_source;
    /* focal length of lens used to take image. unit is millimeter */
    td_u32 focal_length;
    /* indicates the type of scene. value '0x01' means that the image was directly photographed. */
    td_u8 scene_type;
    /*
     * indicates the use of special processing on image data, such as rendering geared to output.
     * 0 = normal process, 1 = custom process.
     */
    td_u8 custom_rendered;
    /* indicates the equivalent focal length assuming a 35mm film camera, in mm */
    td_u8 focal_length_in35mm_film;
    /* indicates the type of scene that was shot. 0 = standard, 1 = landscape, 2 = portrait, 3 = night scene. */
    td_u8 scene_capture_type;
    /*
     * indicates the degree of overall image gain adjustment. 0 = none, 1 = low gain up, 2 = high gain up,
     * 3 = low gain down, 4 = high gain down.
     */
    td_u8 gain_control;
    /*
     * indicates the direction of contrast processing applied by the camera when the image was shot.
     * 0 = normal, 1 = soft, 2 = hard.
     */
    td_u8 contrast;
    /*
     * indicates the direction of saturation processing applied by the camera when the image was shot.
     * 0 = normal, 1 = low saturation, 2 = high saturation.
     */
    td_u8 saturation;
    /*
     * indicates the direction of sharpness processing applied by the camera when the image was shot.
     * 0 = normal, 1 = soft, 2 = hard.
     */
    td_u8 sharpness;
    /*
     * exposure metering method. '0' means unknown, '1' average, '2' center weighted average, '3' spot,
     * '4' multi-spot, '5' multi-segment, '6' partial, '255' other.
     */
    td_u8 metering_mode;
} ot_isp_dcf_const_info;

typedef struct {
    /* CCD sensitivity equivalent to ag-hr film speedrate */
    td_u32 iso_speed_ratings;
    /* exposure time (reciprocal of shutter speed). */
    td_u32 exposure_time;
    /* exposure bias (compensation) value of taking picture */
    td_u32 exposure_bias_value;
    /*
     * exposure program that the camera used when image was taken. '1' means manual control,
     * '2' program normal, '3' aperture priority, '4' shutter priority, '5' program creative (slow program),
     * '6' program action(high-speed program), '7' portrait mode, '8' landscape mode.
     */
    td_u8 exposure_program;
    /* the actual F-number (F-stop) of lens when the image was taken */
    td_u32 f_number;
    /* maximum aperture value of lens. */
    td_u32 max_aperture_value;
    /*
     * indicates the exposure mode set when the image was shot.
     * 0 = auto exposure, 1 = manual exposure, 2 = auto bracket
     */
    td_u8 exposure_mode;
    /*
     * indicates the white balance mode set when the image was shot.
     * 0 = auto white balance, 1 = manual white balance.
     */
    td_u8 white_balance;
} ot_isp_dcf_update_info;

typedef struct {
    ot_isp_dcf_const_info  isp_dcf_const_info;
    ot_isp_dcf_update_info isp_dcf_update_info;
} ot_isp_dcf_info;

typedef struct {
    /* the date and time when the picture data was generated */
    td_u8           capture_time[OT_DCF_CAPTURE_TIME_LENGTH];
    /* whether the picture is captured when a flash lamp is on */
    td_bool         flash;
    /*
     * indicates the digital zoom ratio when the image was shot.
     * if the numerator of the recorded value is 0, this indicates that digital zoom was not used.
     */
    td_u32          digital_zoom_ratio;
    ot_isp_dcf_info isp_dcf_info;
} ot_jpeg_dcf;

/*
 * Defines the ISP FSWDR operating mode
 * 0 = Normal FSWDR mode
 * 1 = Long frame mode, only effective in LINE_WDR,
 * when running in this mode FSWDR module only output the long frame data
 */
typedef enum {
    OT_ISP_FSWDR_NORMAL_MODE          = 0x0,
    OT_ISP_FSWDR_LONG_FRAME_MODE      = 0x1,
    OT_ISP_FSWDR_AUTO_LONG_FRAME_MODE = 0x2, /* Auto long frame mode, only effective in LINE_WDR,  When running in this
                                             mode, normal WDR and long frame mode would auto switch */
    OT_ISP_FSWDR_MODE_BUTT
} ot_isp_fswdr_mode;

typedef struct {
    td_u32      iso;                    /* ISP internal ISO : again*dgain*is_pgain */
    td_u32      exposure_time[OT_ISP_WDR_MAX_FRAME_NUM]; /* exposure time (reciprocal of shutter speed), unit is us */
    td_u32      isp_dgain[OT_ISP_WDR_MAX_FRAME_NUM];
    td_u32      again[OT_ISP_WDR_MAX_FRAME_NUM];
    td_u32      dgain[OT_ISP_WDR_MAX_FRAME_NUM];
    ot_isp_fswdr_mode fs_wdr_mode;

    td_s32      fe_id[OT_ISP_WDR_MAX_FRAME_NUM];
    td_u32      ratio[3];
    td_u32      isp_nr_strength;
    td_u32      f_number;               /* the actual F-number (F-stop) of lens when the image was taken */
    td_u32      sensor_id;              /* which sensor is used */
    td_u32      sensor_mode;
    td_u32      hmax_times;             /* sensor hmax_times,unit is ns */
    td_u32      vmax;                   /* sensor vmax,unit is line */
    td_u32      vc_num;                 /* when dump wdr frame, which is long or short  exposure frame. */
    td_u32      wb_gain[OT_ISP_WB_GAIN_NUM]; /* white balance gain info */
    td_u16      ccm[OT_ISP_CAP_CCM_NUM];     /* RW; Range: [0x0, 0xFFFF]; Format:8.8; Manual CCM matrix. */
    td_u32      exposure_distance[OT_ISP_WDR_MAX_FRAME_NUM - 1]; /* wdr sensor exposure distance line number. */
    td_u32      inner_flag; /* isp inner flag, can be OR operation. */
} ot_isp_frame_info;

typedef struct {
    td_u32 color_temp;
    td_u16 ccm[9];
    td_u8  saturation;
} ot_isp_hdr_info;

typedef struct {
    ot_isp_hdr_info isp_hdr;
    td_u32 iso;
    td_u32 init_iso;
    td_u8 sns_wdr_mode;
} ot_isp_attach_info;

typedef struct {
    ot_color_gamut color_gamut;
} ot_isp_colorgammut_info;

typedef struct {
    td_u32 numerator;   /* represents the numerator of a fraction. */
    td_u32 denominator; /* the denominator. */
} ot_dng_rational;

/* defines the structure of DNG image dynamic information */
typedef struct {
    /* RO; range: [0x0, 0xFFFF]; black level. */
    td_u32 black_level[OT_ISP_BAYER_CHN];
    /*
     * specifies the selected white balance at time of capture,
     * encoded as the coordinates of a perfectly neutral color in linear reference space values.
     */
    ot_dng_rational as_shot_neutral[OT_CFACOLORPLANE];
    /* RO; describes the amount of noise in a raw image */
    td_double ad_noise_profile[OT_DNG_NP_SIZE];
} ot_dng_image_dynamic_info;

typedef struct {
    td_u32 iso;
    td_u32 isp_dgain;
    td_u32 exposure_time;
    td_u32 white_balance_gain[OT_ISP_WB_GAIN_NUM];
    td_u32 color_temperature;
    td_u16 cap_ccm[OT_ISP_CAP_CCM_NUM];
} ot_isp_config_info;

typedef struct {
    td_bool enable;         /* RW; low delay enable. */
    td_u32  line_cnt;       /* RW; range: [16, 8192]; low delay shoreline. */
    td_bool one_buf_en;     /* RW; one buffer for low delay enable. */
} ot_low_delay_info;

typedef enum {
    OT_SCHEDULE_NORMAL = 0,
    OT_SCHEDULE_QUICK,
    OT_SCHEDULE_BUTT
} ot_schedule_mode;

typedef enum {
    OT_SCALE_COEF_TYPE_NORMAL = 0,
    OT_SCALE_COEF_TYPE_BILINEAR,
    OT_SCALE_COEF_TYPE_BUTT
} ot_scale_coef_type;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_OUT_INCLUDE_OT_COMMON_VIDEO_H_BODY_PART02_H_ */
