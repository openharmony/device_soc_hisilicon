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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HY_S0603_HY_S0603_CMOS_EX_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HY_S0603_HY_S0603_CMOS_EX_H_BODY_PART01_H_


#include "ot_common_awb.h"
#include "ot_common_isp.h"
#include "ot_common_sns.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

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
            {0x444, 0x444, 0x444, 0x444},
            {0x444, 0x444, 0x444, 0x444},
            {0x444, 0x444, 0x444, 0x444},
            {0x444, 0x444, 0x444, 0x444},
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
            {0x444, 0x444, 0x444, 0x444}, /* black level */
            {0x444, 0x444, 0x444, 0x444}, /* black level */
            {0x444, 0x444, 0x444, 0x444}, /* black level */
            {0x444, 0x444, 0x444, 0x444}, /* black level */
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
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
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

/* BAYER NR */
static ot_isp_noise_calibration g_cmos_noise_calibration = {
    {
        1621.0000000000, 0.0003682779, -0.0150903382, 0.0000000368, -0.0000816109, 0.0519085205,
        0.0003600083, -0.0016596481, 0.0000000619, -0.0000323338, 0.0041258741, 4900.0000000000, 0, 0, 0, 0,
    }
};

static const ot_isp_nr_attr g_cmos_bayer_nr = {
    1,      /* bEnable */
    0,      /* enOpType */
    1,      /* tnr_enable */
    0,      /* lsc_nr_enable */
    0,      /* lsc_ratio1 */
    1,      /* lsc_ratio2 */
    {
        85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
        85, 85, 85, 85, 85, 85, 90, 90, 90, 95, 95, 95, 100, 100, 100, 100, 100
    },   /* CoringRatio */

    /* snr_cfg */
    {
        {
            /* snr auto */
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, /* sfm_thresh */
            {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* sfm0_mode */
            {
                {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108}
            },
            {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13}, /* sfm0_ex_strength */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* sfm0_ext_detail_prot */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* sfm0_normal_edge_str */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* sfm1_detail_prot */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* sfm1_coarstr */
            {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}, /* finestr */
            {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}, /* coring_wgt */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} /* coring_mot_th */
        },

        {
            /* snr manual */
            255, /* sfm_thresh */
            1,   /* sfm0_mode */
            { 108, 108, 108, 108 }, /* snr_sfm0_coarstr */
            13, /* sfm0_ex_strength */
            16, /* sfm0_ext_detail_prot */
            0,  /* sfm0_normal_edge_str */
            16, /* sfm1_detail_prot */
            32, /* sfm1_coarstr */
            128, /* finestr */
            50, /* coring_wgt */
            0,  /* coring_mot_th */
        },
    },
    /* tnr_cfg */
    {
        {
            /* tnr auto */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* md_mode */
            {0,  0,  0,  16, 24, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* md_anti_flicker_str */
            {24, 24, 20, 20, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16},   /* md_static_ratio */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},   /* md_static_finestr */
            {0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 60, 60, 60, 60, 60, 60}, /* tnr_tss */
            {128, 128, 128, 128, 180, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, /* tnr_tfr */
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, /* tnr_tfs */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_md */
            {0, 0, 0, 0, 0, 16, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_slope */
            {0, 0, 0, 0, 0, 90, 90, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_dark_thresh */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* user_define_color_thresh */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, /* sfr_r */
            {32, 32, 32, 32, 32, 32, 32, 128, 128, 128, 80, 80, 80, 80, 80, 80}, /* sfr_g */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, /* sfr_b */
        },
        {
            0, /* md_mode */
            64, /* md_anti_flicker_str */
            32, /* md_static_ratio */
            32, /* md_static_finestr */
            0, /* tnr_tss */
            255, /* tnr_tfr */
            255, /* tnr_tfs */
            0, /* user_define_md */
            0, /* user_define_bright_thresh */
            90, /* user_define_dark_thresh */
            16, /* user_define_color_thresh */
            26, /* sfr_r */
            32, /* sfr_g */
            26, /* sfr_b */
        }
    },

    /* wdr_cfg */
    {
        {16, 16, 16, 16}, /* sfm0_wdr_frame_str */
        {16, 16, 16, 16}, /* sfm0_fusion_frame_str */
        16,
        16,
        {16, 16, 16, 16}, /* md_wdr_frame_str */
        {16, 16, 16, 16}, /* md_fusion_frame_str */
    }
};

static const ot_isp_ldci_attr g_cmos_ldci = {
    /* enable */
    1,
    /* gauss_lpf_sigma */
    36,
    /* ot_op_mode */
    0,

    /* ot_isp_ldci_manual_attr */
    {
        /* ot_isp_ldci_he_wgt_attr */
        {
            /* he_pos_wgt */
            {
                50, 80, 32
            },
            /* he_neg_wgt */
            {
                60, 80, 128
            }
        },

        /* blc_ctrl */
        20
    },

    /* ot_isp_ldci_auto_attr */
    {
        /* he_wgt[OT_ISP_AUTO_ISO_NUM] */
        {
            {{50, 80, 32}, {60, 80, 128}},
            {{45, 80, 16}, {45, 80, 128}},
            {{42, 70, 0}, {45, 80, 128}},
            {{40, 70, 0}, {45, 80, 0}},
            {{35, 70, 0}, {24, 72, 0}},
            {{32, 64, 0}, {12, 64, 0}},
            {{12, 24, 0}, {8, 54, 0}},
            {{8, 20, 0}, {6, 36, 0}},
            {{6, 12, 0}, {0, 8, 0}},
            {{0, 8, 0}, {0, 6, 0}},
            {{0, 6, 0}, {0, 2, 0}},
            {{0, 2, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}}
        },

        /* blc_ctrl */
        { 20, 20, 20, 20, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 }
    },

    /* tpr_incr_coef */
    8,
    /* tpr_decr_coef */
    8
};

static const ot_isp_ldci_attr g_cmos_ldci_wdr = {
    /* enable */
    1,
    /* gauss_lpf_sigma */
    36,
    /* ot_op_mode */
    0,

    /* ot_isp_ldci_manual_attr */
    {
        /* ot_isp_ldci_he_wgt_attr */
        {
            /* he_pos_wgt */
            {
                16, 80, 20
            },
            /* he_neg_wgt */
            {
                32, 80, 180
            }
        },

        /* blc_ctrl */
        20
    },

    /* ot_isp_ldci_auto_attr */
    {
        /* he_wgt[OT_ISP_AUTO_ISO_NUM] */
        {
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}}
        },

        /* blc_ctrl */
        { 20,  20,  20,   20, 20,  20,  20,  20,  20,  20,   20,  20,  20,  20,  20,  20 }
    },

    /* tpr_incr_coef */
    8,
    /* tpr_decr_coef */
    8
};

static const ot_isp_gamma_attr g_cmos_gamma = {

    1,
    {0,    13,   27,   40,   54,   68,   81,   95,   108,  121,  135,  150,  166,  180,  195,  210,  226,  242,  259,
     269,  281,  292,  303,  315,  327,  339,  352,  364,  376,  389,  402,  415,  425,  435,  446,  456,  467,  477,
     488,  499,  510,  521,  532,  543,  555,  566,  578,  591,  602,  613,  625,  636,  648,  659,  671,  683,  695,
     708,  721,  734,  748,  762,  777,  792,  809,  817,  826,  836,  845,  855,  865,  875,  885,  896,  906,  917,
     928,  939,  950,  961,  972,  983,  994,  1005, 1017, 1028, 1039, 1051, 1062, 1073, 1085, 1096, 1107, 1118, 1129,
     1140, 1151, 1161, 1172, 1182, 1193, 1203, 1213, 1223, 1234, 1244, 1254, 1264, 1274, 1284, 1295, 1305, 1315, 1325,
     1336, 1346, 1357, 1367, 1378, 1389, 1400, 1411, 1422, 1433, 1445, 1456, 1468, 1480, 1493, 1499, 1505, 1511, 1518,
     1524, 1531, 1537, 1544, 1551, 1558, 1564, 1571, 1578, 1585, 1592, 1599, 1607, 1614, 1621, 1628, 1635, 1643, 1650,
     1657, 1665, 1672, 1679, 1687, 1694, 1702, 1709, 1716, 1724, 1731, 1738, 1746, 1753, 1761, 1768, 1775, 1783, 1790,


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HY_S0603_HY_S0603_CMOS_EX_H_BODY_PART01_H_ */
