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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_OMNIVISION_OS08A20_OS08A20_CMOS_EX_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_OMNIVISION_OS08A20_OS08A20_CMOS_EX_H_BODY_PART01_H_


/* Piris attr */
static ot_isp_piris_attr g_piris = {
    0,   // bStepFNOTableChange
    1,   // bZeroIsMax
    94,  // u16TotalStep
    62,  // u16StepCount
    /* Step-F number mapping table. Must be from small to large. F1.0 is 1024 and F32.0 is 1 */
    { 30,  35,  40,  45,  50,  56,  61,  67,  73,  79,  85,  92,  98,  105, 112, 120, 127, 135, 143, 150, 158,
      166, 174, 183, 191, 200, 208, 217, 225, 234, 243, 252, 261, 270, 279, 289, 298, 307, 316, 325, 335, 344,
      353, 362, 372, 381, 390, 399, 408, 417, 426, 435, 444, 453, 462, 470, 478, 486, 493, 500, 506, 512 },
    OT_ISP_IRIS_F_NO_1_4,  // enMaxIrisFNOTarget
    OT_ISP_IRIS_F_NO_5_6,   // enMinIrisFNOTarget
    1,
    512,
    32
};

static const ot_isp_cmos_black_level g_cmos_blc = {
    TD_FALSE, /* user_black_level_en */
    {
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
    },   /* user_black_level */
    OT_ISP_BLACK_LEVEL_MODE_AUTO, /* ot_isp_black_level_mode */
    {
        {
            {0x400, 0x400, 0x400, 0x400},
            {0x400, 0x400, 0x400, 0x400},
            {0x400, 0x400, 0x400, 0x400},
            {0x400, 0x400, 0x400, 0x400},
        },
    }, /* manual_attr */
    {
        OT_ISP_DYNAMIC_PATTERN_RGB, /* ot_isp_black_level_dynamic_pattern */
        {0, 0, 3840, 20}, /* ob_area */
        0x0,    /* low_threshold */
        0xbb8,  /* high_threshold */
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},    /* dynablc offset */
        0x0,    /* dynablc tolerance */
        0x0,    /* filter_strength */
        0x0,    /* separate_mode */
        {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024},
        0,
        /* calibration black level */
    }, /* dynamic_attr */
    {
        0,                            /* update mode 0 */
        {
            {0x400, 0x400, 0x400, 0x400}, /* black level */
            {0x400, 0x400, 0x400, 0x400}, /* black level */
            {0x400, 0x400, 0x400, 0x400}, /* black level */
            {0x400, 0x400, 0x400, 0x400}, /* black level */
        },
    },
    {
        {3840, 2180},
    },
};

static const ot_isp_cmos_black_level g_cmos_blc_wdr = {
    TD_FALSE, /* user_black_level_en */
    {
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
    },   /* user_black_level */
    OT_ISP_BLACK_LEVEL_MODE_AUTO, /* ot_isp_black_level_mode */
    {
        {
            {0x400, 0x400, 0x400, 0x400},
            {0x400, 0x400, 0x400, 0x400},
            {0x400, 0x400, 0x400, 0x400},
            {0x400, 0x400, 0x400, 0x400},
        },
    }, /* manual_attr */
    {
        OT_ISP_DYNAMIC_PATTERN_RGB, /* ot_isp_black_level_dynamic_pattern */
        {0, 0, 3840, 20}, /* ob_area */
        0x0,    /* low_threshold */
        0xbb8,  /* high_threshold */
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},    /* dynablc offset */
        0x0,    /* dynablc tolerance */
        0x0,    /* filter_strength */
        0x0,    /* separate_mode */
        {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024},
        0,
        /* calibration black level */
    }, /* dynamic_attr */
    {
        0,                            /* update mode 0 */
        {
            {0x400, 0x400, 0x400, 0x400}, /* black level */
            {0x400, 0x400, 0x400, 0x400}, /* black level */
            {0x400, 0x400, 0x400, 0x400}, /* black level */
            {0x400, 0x400, 0x400, 0x400}, /* black level */
        },
    },
    {
        {3840, 2180},
    },
};

static const ot_isp_dp_dynamic_auto_attr g_cmos_dpc = {
    { 0, 0, 0, 152, 220, 220, 220, 220, 220, 220, 152, 152, 152, 152, 152, 152 }, /* strength[16] */
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50, 50, 50, 50, 50 },                     /* blend_ratio[16] */
};

static const ot_isp_dp_dynamic_auto_attr g_cmos_dpc_wdr = {
    { 230, 230, 230, 235, 235, 235, 240, 240, 240, 250, 250, 250, 250, 250, 250, 250 }, /* strength[16] */
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },                                 /* blend_ratio[16] */
};

#ifdef CONFIG_OT_ISP_CR_SUPPORT
static const ot_isp_cr_attr g_cmos_ge = {
    1,   /* en */
    9,   /* slope */
    9,   /* sensi_slope */
    300, /* sensi_threshold */
    { 128, 128, 128, 128, 129, 129, 129, 129, 130, 130, 130, 130, 131, 131, 131, 131 }, /* strength */
    { 300, 300, 300, 300, 310, 310, 310, 310, 320, 320, 320, 320, 330, 330, 330, 330 }, /* threshold */
    { 1024, 1024, 1024, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048 } /* np_offset */
};

static const ot_isp_cr_attr g_cmos_ge_wdr = {
    0,   /* en */
    9,   /* slope */
    9,   /* sensi_slope */
    300, /* sensi_threshold */
    { 128, 128, 128, 128, 129, 129, 129, 129, 130, 130, 130, 130, 131, 131, 131, 131 }, /* strength */
    { 300, 300, 300, 300, 310, 310, 310, 310, 320, 320, 320, 320, 330, 330, 330, 330 }, /* threshold */
    { 1024, 1024, 1024, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048 } /* np_offset */
};
#endif

static const ot_isp_demosaic_attr g_cmos_demosaic = {
    1, /* en */
    OT_OP_MODE_AUTO, /* op_type */
    {
        48, /* nddm_strength */
        64, /* nddm_mf_detail_strength */
        0, /* nddm_hf_detail_strength */
        2, /* detail_smooth_range */
        0, /* color_noise_f_threshold */
        8, /* color_noise_f_strength */
        1, /* color_noise_y_threshold */
        10, /* color_noise_y_strength */
    },
    {
        /* nddm_strength */
        { 48, 48, 48, 54, 54, 54, 48, 40, 40, 48, 48, 48, 48, 48, 48, 48 },
        /* nddm_mf_detail_strength */
        { 82, 74, 68, 60, 54, 50, 44, 40, 40, 40, 40, 32, 16, 16, 16, 16 },
        /* nddm_hf_detail_strength */
        { 0, 0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },
        /* detail_smooth_range */
        { 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3, 5, 7, 7, 7, 7 },
        /* color_noise_f_threshold */
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* color_noise_f_strength */
        { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },
        /* color_noise_y_threshold */
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        /* color_noise_y_strength */
        { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
    }
};

static const ot_isp_demosaic_attr g_cmos_demosaic_wdr = {
    1, /* en */
    OT_OP_MODE_AUTO, /* op_type */
    {
        48, /* nddm_strength */
        64, /* nddm_mf_detail_strength */
        0, /* nddm_hf_detail_strength */
        2, /* detail_smooth_range */
        0, /* color_noise_f_threshold */
        8, /* color_noise_f_strength */
        1, /* color_noise_y_threshold */
        10, /* color_noise_y_strength */
    },
    {
        /* nddm_strength */
        { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
        /* nddm_mf_detail_strength */
        { 60, 60, 58, 55, 50, 45, 40, 30, 20, 16, 16, 16, 16, 16, 16, 16 },
        /* nddm_hf_detail_strength */
        { 0, 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 5, 5, 5, 5, 5 },
        /* detail_smooth_range */
        { 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 6, 7, 7, 7, 7, 7 },
        /* color_noise_f_threshold */
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* color_noise_f_strength */
        { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },
        /* color_noise_y_threshold */
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        /* color_noise_y_strength */
        { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
    }
};

static const ot_isp_anti_false_color_attr g_cmos_anti_false_color = {
    1,                  /* en */
    OT_OP_MODE_AUTO,    /* op_type */
    { 8, 8 },           /* manual_attr */
    {
        { 20, 15, 12, 12, 10, 8, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0 }, /* anti_false_color_threshold */
        { 15, 15, 12, 10,  8, 8, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0 }, /* anti_false_color_strength */
    }
};

static const ot_isp_anti_false_color_attr g_cmos_anti_false_color_wdr = {
    1,                  /* en */
    OT_OP_MODE_AUTO,    /* op_type */
    { 8, 8 },           /* manual_attr */
    {
        { 25, 20, 15, 12, 10, 10, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0 }, /* anti_false_color_threshold */
        { 20, 18, 15, 12, 10,  8, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0 }, /* anti_false_color_strength */
    }
};


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_OMNIVISION_OS08A20_OS08A20_CMOS_EX_H_BODY_PART01_H_ */
