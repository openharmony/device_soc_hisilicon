/*
 * Copyright (c) 2026 Nanjing Qinuo Information Technology Co., Ltd.
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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_OMNIVISION_OS08A20_OS08A20_CMOS_EX_H_BODY_PART06_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_OMNIVISION_OS08A20_OS08A20_CMOS_EX_H_BODY_PART06_H_

static const ot_isp_wdr_fs_attr g_cmos_wdr = {
    /* ot_isp_wdr_merge_mode */
    OT_ISP_MERGE_FUSION_MODE,

    /* ot_isp_wdr_combine_attr */
    {
        /* motion_comp */
        1,

        /* forcelong_coarse */
        1,

        /* short_thr */
        4032,

        /* long_thr */
        3008,

        /* force_long */
        1,

        /* force_long_low_threshold */
        500,

        /* force_long_hig_threshold */
        700,

        /* ot_isp_fswdr_mdt_attr */
        {
            /* short_expo_chk */
            0,

            /* short_check_thd */
            0x8,

            /* md_ref_flicker */
            1,

            /* mdt_still_thd */
            0x14,

            /* mdt_full_threshold */
            254,

            /* mdt_long_blend */
            0x0,

            /* ot_op_mode motion_comp */
            OT_OP_MODE_AUTO,

            /* ot_isp_fswdr_manual_attr */
            {
                /* md_thr_low_gain */
                64, 64
            },

            /* ot_isp_fswdr_auto_attr */
            {
                /* md_thr_low_gain[10][16] */
                {
                    { 45, 45, 45, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 45, 45, 45, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 24, 24, 24, 45, 64,  96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 24, 24, 24, 45, 64,  96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 45, 45, 45, 45, 64, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 45, 45, 45, 45, 64, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 64, 64, 64, 64, 128, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 128, 128, 128, 128, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 128, 128, 128, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 }
                },

                /* md_thr_hig_gain[10][16] */
                {
                    { 64, 64, 64, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 64, 64, 64, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 45, 45, 45, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 45, 45, 45, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 64, 64, 64, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 64, 64, 64, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 128, 128, 128, 128, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                },
            },
        },

        /* forcelong_coarse_low_threshold */
        3520,

        /* forcelong_coarse_high_threshold */
        4160,
    },

    /* ot_isp_fusion_attr */
    {
        { 15420, 12000, 12000, 12000 },
    },

};

static const ot_isp_dehaze_attr g_cmos_dehaze = {
    /* enable */
    0,
    /* user_lut_enable */
    0,
    /* dehaze_lut */
    {
        0, 9, 13, 17, 21, 25, 29, 33, 37, 42, 46, 50, 54, 58, 62, 67, 71, 75, 79, 83, 87, 91,
        95, 99, 102, 106, 110, 114, 117, 121, 124, 128, 131, 134, 137, 141, 144, 147, 150,
        153, 156, 159, 162, 165, 168, 170, 173, 176, 178, 181, 184, 186, 189, 191, 193, 196,
        198, 200, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 222, 224, 226, 228, 229,
        231, 232, 234, 235, 237, 238, 240, 241, 242, 243, 244, 246, 247, 248, 249, 249, 250,
        251, 252, 253, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 253, 253, 252, 251, 251, 250, 249,
        249, 248, 247, 247, 246, 245, 244, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235,
        233, 232, 231, 230, 229, 227, 226, 225, 223, 222, 221, 219, 218, 216, 215, 213, 212,
        210, 209, 208, 206, 205, 203, 201, 200, 198, 197, 195, 193, 192, 190, 188, 187, 185,
        183, 181, 180, 178, 176, 174, 172, 170, 169, 167, 165, 163, 161, 159, 157, 155, 153,
        151, 149, 147, 145, 143, 140, 138, 136, 134, 132, 130, 128, 125, 123, 121, 119, 116,
        114, 112, 110, 107, 105, 103, 100, 98, 96, 93, 91, 89, 86, 84, 81, 79, 77, 74, 72, 69,
        67, 64, 62, 59, 57, 54, 52, 49, 47, 44, 42, 39, 37, 34, 31, 29, 26, 24, 21, 18, 16, 13, 11,
        8, 5, 3, 0
    },
    /* op_type */
    0,
    /* auto_strength */
    {
        128
    },
    /* manual_strength */
    {
        128
    },
    /* tmprflt_incr_coef */
    8,
    /* tmprflt_decr_coef */
    64,
};

static const ot_isp_dehaze_attr g_cmos_dehaze_wdr = {
    /* enable */
    1,
    /* user_lut_enable */
    1,
    /* dehaze_lut */
    {
        0, 9, 13, 17, 21, 25, 29, 33, 37, 42, 46, 50, 54, 58, 62, 67, 71, 75, 79, 83, 87, 91,
        95, 99, 102, 106, 110, 114, 117, 121, 124, 128, 131, 134, 137, 141, 144, 147, 150,
        153, 156, 159, 162, 165, 168, 170, 173, 176, 178, 181, 184, 186, 189, 191, 193, 196,
        198, 200, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 222, 224, 226, 228, 229,
        231, 232, 234, 235, 237, 238, 240, 241, 242, 243, 244, 246, 247, 248, 249, 249, 250,
        251, 252, 253, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 253, 253, 252, 251, 251, 250, 249,
        249, 248, 247, 247, 246, 245, 244, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235,
        233, 232, 231, 230, 229, 227, 226, 225, 223, 222, 221, 219, 218, 216, 215, 213, 212,
        210, 209, 208, 206, 205, 203, 201, 200, 198, 197, 195, 193, 192, 190, 188, 187, 185,
        183, 181, 180, 178, 176, 174, 172, 170, 169, 167, 165, 163, 161, 159, 157, 155, 153,
        151, 149, 147, 145, 143, 140, 138, 136, 134, 132, 130, 128, 125, 123, 121, 119, 116,
        114, 112, 110, 107, 105, 103, 100, 98, 96, 93, 91, 89, 86, 84, 81, 79, 77, 74, 72, 69,
        67, 64, 62, 59, 57, 54, 52, 49, 47, 44, 42, 39, 37, 34, 31, 29, 26, 24, 21, 18, 16, 13, 11,
        8, 5, 3, 0
    },
    /* op_type */
    0,
    /* auto_strength */
    {
        100
    },
    /* manual_strength */
    {
        128
    },
    /* tmprflt_incr_coef */
    8,
    /* tmprflt_decr_coef */
    64,
};

static const ot_isp_drc_attr g_cmos_drc = {
    /* enable */
    0,
    /* curve_select */
    OT_ISP_DRC_CURVE_ASYMMETRY,
    /* purple_reduction_strength */
    35,
    /* bright_gain_limit */
    10,
    /* bright_gain_limit_step */
    8,
    /* dark_gain_limit_luma */
    0,
    /* dark_gain_limit_chroma */
    0,
    /* contrast_ctrl */
    8,
    /* rim_reduction_strength */
    32,
    /* rim_reduction_threshold */
    30,
    /* color_correction_lut[OT_ISP_DRC_CC_NODE_NUM] */
    {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
     1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024},
    /* tone_mapping_value[OT_ISP_DRC_TM_NODE_NUM] */
    {8,     8,     16,    24,    31,    39,    48,    56,    65,    75,    85,    95,    106,   118,   130,   143,
     156,   170,   185,   201,   218,   235,   254,   274,   294,   316,   339,   364,   390,   417,   446,   477,
     509,   543,   579,   617,   658,   701,   746,   794,   844,   898,   955,   1015,  1078,  1145,  1216,  1291,
     1370,  1454,  1543,  1637,  1736,  1841,  1952,  2069,  2194,  2325,  2465,  2612,  2767,  2932,  3106,  3290,
     3485,  3691,  3909,  4140,  4384,  4641,  4914,  5202,  5507,  5830,  6171,  6531,  6913,  7316,  7742,  8193,
     8669,  9173,  9705,  10268, 10863, 11492, 12145, 12808, 13483, 14171, 14872, 15587, 16319, 17069, 17840, 18635,
     19458, 19881, 20313, 20754, 21204, 21661, 22122, 22586, 23053, 23525, 24000, 24480, 24965, 25455, 25950, 26451,
     26959, 27473, 27995, 28524, 29062, 29609, 30165, 30732, 31309, 31899, 32501, 33116, 33746, 34391, 35043, 35706,
     36381, 37066, 37763, 38472, 39192, 39925, 40671, 41429, 42201, 42591, 42986, 43383, 43784, 44189, 44597, 45008,
     45424, 45842, 46265, 46691, 47121, 47555, 47993, 48434, 48880, 49329, 49783, 50241, 50703, 51169, 51639, 52113,
     52592, 53075, 53564, 54056, 54552, 55054, 55560, 56071, 56586, 56846, 57107, 57369, 57632, 57896, 58162, 58429,
     58697, 58967, 59238, 59510, 59783, 60057, 60333, 60611, 60889, 61169, 61451, 61733, 62017, 62303, 62589, 62877,
     63167, 63458, 63750, 64044, 64340, 64636, 64934, 65234},
    /* spatial_filter_coef */
    1,
    /* range_filter_coef */
    2,
    /* detail_adjust_coef */
    8,
    /* local_mixing_bright[OT_ISP_DRC_LMIX_NODE_NUM] */
    {15, 17, 18, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37, 39, 40, 42,
     43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 62, 63, 64},
    /* local_mixing_dark[OT_ISP_DRC_LMIX_NODE_NUM] */
    {15, 17, 18, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37, 39, 40, 42,
     43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 62, 63, 64},
    /* filter_coef_x[OT_ISP_DRC_FLTX_NODE_NUM] */
    {3, 3, 3},
    /* filter_low_threshold_x[OT_ISP_DRC_FLTX_NODE_NUM] */
    {2, 2, 2},
    /* filter_high_threshold_x[OT_ISP_DRC_FLTX_NODE_NUM] */
    {5, 5, 5},
    /* detail_adjust_coef_x */
    0,
    /* local_mixing_bright_x[OT_ISP_DRC_LMIX_NODE_NUM] */
    {0,  4,  8,  12, 16, 20, 25, 29, 33, 41, 49, 56, 64, 72, 80, 88, 96, 93,
     90, 87, 84, 80, 77, 74, 71, 68, 65, 62, 60, 57, 54, 51, 48},
    /* local_mixing_dark_x[OT_ISP_DRC_LMIX_NODE_NUM] */
    {0,  4,  8,  12, 16, 20, 25, 29, 33, 41, 49, 56, 64, 72, 80, 88, 96, 93,
     90, 87, 84, 80, 77, 74, 71, 68, 65, 62, 60, 57, 54, 51, 48},
    /* blend_luma_max */
    200,
    /* blend_luma_bright_min */
    0,
    /* blend_luma_bright_threshold */
    60,
    /* blend_luma_bright_slope */
    12,
    /* blend_luma_dark_min */
    0,
    /* blend_luma_dark_threshold */
    40,
    /* blend_luma_dark_slope */
    12,
    /* blend_detail_max */
    255,
    /* blend_detail_bright_min */
    0,
    /* blend_detail_bright_threshold */
    20,
    /* blend_detail_bright_slope */
    12,
    /* blend_detail_dark_min */
    0,
    /* blend_detail_dark_threshold */
    20,
    /* blend_detail_dark_slope */
    12,
    /* detail_adjust_coef_blend */
    0,
    /* low_saturation_color_ctrl */
    0,
    /* high_saturation_color_ctrl */
    15,
    /* color_correction_ctrl */
    1,
    /* op_type */
    OT_OP_MODE_AUTO,
    /* manual_attr */
    {
        /* manual_strength */
        1023,
    },
    /* auto_attr */
    {
        /* auto_strength */
        512,
        /* auto_strength_max */
        1023,
        /* auto_strength_min */
        512,
    },
    /* asymmetry_curve */
    {
        /* asymmetry */
        6,
        /* second_pole */
        200,
        /* stretch */
        40,
        /* compress */
        170
    }
};

static const ot_isp_drc_attr g_cmos_drc_wdr = {
    /* enable */
    1,
    /* curve_select */
    OT_ISP_DRC_CURVE_ASYMMETRY,
    /* purple_reduction_strength */
    35,
    /* bright_gain_limit */
    10,
    /* bright_gain_limit_step */
    8,
    /* dark_gain_limit_luma */
    0,
    /* dark_gain_limit_chroma */
    0,
    /* contrast_ctrl */
    8,
    /* rim_reduction_strength */
    32,
    /* rim_reduction_threshold */
    30,
    /* color_correction_lut[OT_ISP_DRC_CC_NODE_NUM] */
    {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
     1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024},
    /* tone_mapping_value[OT_ISP_DRC_TM_NODE_NUM] */
    {8,     8,     16,    24,    31,    39,    48,    56,    65,    75,    85,    95,    106,   118,   130,   143,
     156,   170,   185,   201,   218,   235,   254,   274,   294,   316,   339,   364,   390,   417,   446,   477,
     509,   543,   579,   617,   658,   701,   746,   794,   844,   898,   955,   1015,  1078,  1145,  1216,  1291,
     1370,  1454,  1543,  1637,  1736,  1841,  1952,  2069,  2194,  2325,  2465,  2612,  2767,  2932,  3106,  3290,
     3485,  3691,  3909,  4140,  4384,  4641,  4914,  5202,  5507,  5830,  6171,  6531,  6913,  7316,  7742,  8193,
     8669,  9173,  9705,  10268, 10863, 11492, 12145, 12808, 13483, 14171, 14872, 15587, 16319, 17069, 17840, 18635,
     19458, 19881, 20313, 20754, 21204, 21661, 22122, 22586, 23053, 23525, 24000, 24480, 24965, 25455, 25950, 26451,
     26959, 27473, 27995, 28524, 29062, 29609, 30165, 30732, 31309, 31899, 32501, 33116, 33746, 34391, 35043, 35706,
     36381, 37066, 37763, 38472, 39192, 39925, 40671, 41429, 42201, 42591, 42986, 43383, 43784, 44189, 44597, 45008,
     45424, 45842, 46265, 46691, 47121, 47555, 47993, 48434, 48880, 49329, 49783, 50241, 50703, 51169, 51639, 52113,
     52592, 53075, 53564, 54056, 54552, 55054, 55560, 56071, 56586, 56846, 57107, 57369, 57632, 57896, 58162, 58429,
     58697, 58967, 59238, 59510, 59783, 60057, 60333, 60611, 60889, 61169, 61451, 61733, 62017, 62303, 62589, 62877,
     63167, 63458, 63750, 64044, 64340, 64636, 64934, 65234},
    /* spatial_filter_coef */
    1,
    /* range_filter_coef */
    2,
    /* detail_adjust_coef */
    8,
    /* local_mixing_bright[OT_ISP_DRC_LMIX_NODE_NUM] */
    {15, 17, 18, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37, 39, 40, 42,
     43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 62, 63, 64},
    /* local_mixing_dark[OT_ISP_DRC_LMIX_NODE_NUM] */
    {15, 17, 18, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37, 39, 40, 42,
     43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 62, 63, 64},
    /* filter_coef_x[OT_ISP_DRC_FLTX_NODE_NUM] */
    {3, 3, 3},
    /* filter_low_threshold_x[OT_ISP_DRC_FLTX_NODE_NUM] */
    {2, 2, 2},
    /* filter_high_threshold_x[OT_ISP_DRC_FLTX_NODE_NUM] */
    {5, 5, 5},
    /* detail_adjust_coef_x */
    0,
    /* local_mixing_bright_x[OT_ISP_DRC_LMIX_NODE_NUM] */
    {0,  4,  8,  12, 16, 20, 25, 29, 33, 41, 49, 56, 64, 72, 80, 88, 96, 93,
     90, 87, 84, 80, 77, 74, 71, 68, 65, 62, 60, 57, 54, 51, 48},
    /* local_mixing_dark_x[OT_ISP_DRC_LMIX_NODE_NUM] */
    {0,  4,  8,  12, 16, 20, 25, 29, 33, 41, 49, 56, 64, 72, 80, 88, 96, 93,
     90, 87, 84, 80, 77, 74, 71, 68, 65, 62, 60, 57, 54, 51, 48},
    /* blend_luma_max */
    200,
    /* blend_luma_bright_min */
    0,
    /* blend_luma_bright_threshold */
    60,
    /* blend_luma_bright_slope */
    12,
    /* blend_luma_dark_min */
    0,
    /* blend_luma_dark_threshold */
    40,
    /* blend_luma_dark_slope */
    12,
    /* blend_detail_max */
    255,
    /* blend_detail_bright_min */
    0,
    /* blend_detail_bright_threshold */
    20,
    /* blend_detail_bright_slope */
    12,
    /* blend_detail_dark_min */
    0,
    /* blend_detail_dark_threshold */
    20,
    /* blend_detail_dark_slope */
    12,
    /* detail_adjust_coef_blend */
    0,
    /* low_saturation_color_ctrl */
    0,
    /* high_saturation_color_ctrl */
    15,
    /* color_correction_ctrl */
    1,
    /* op_type */
    OT_OP_MODE_AUTO,
    /* manual_attr */
    {
        /* manual_strength */
        1023,
    },
    /* auto_attr */
    {
        /* auto_strength */
        512,
        /* auto_strength_max */
        1023,
        /* auto_strength_min */
        512,
    },
    /* asymmetry_curve */
    {
        /* asymmetry */
        6,
        /* second_pole */
        200,
        /* stretch */
        40,
        /* compress */
        170
    }
};


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_OMNIVISION_OS08A20_OS08A20_CMOS_EX_H_BODY_PART06_H_ */
