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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_OMNIVISION_OS08A20_OS08A20_CMOS_EX_H_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_OMNIVISION_OS08A20_OS08A20_CMOS_EX_H_BODY_PART02_H_

/* BAYER NR */
static ot_isp_noise_calibration g_cmos_noise_calibration = {
    {6400, 0.00031652, -0.04926185, 0.00000022, 0.00002624, 0.02665515,
     0.00029653, 0.00365509, 0.00000022, 0.00000076, 0.01076730, 0, 0, 0, 0, 0}
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
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* md_anti_flicker_str */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},   /* md_static_ratio */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},   /* md_static_finestr */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* tnr_tss */
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, /* tnr_tfr */
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, /* tnr_tfs */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_md */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_slope */
            {90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90}, /* user_define_dark_thresh */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* user_define_color_thresh */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, /* sfr_r */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* sfr_g */
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

static const ot_isp_nr_attr g_cmos_bayer_nr_wdr = {
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
            {255, 255, 255,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, /* sfm_thresh */
            {1,    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1}, /* sfm0_mode */
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
            {24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24}, /* sfm1_coarstr */
            {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}, /* finestr */
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, /* coring_wgt */
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
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* md_anti_flicker_str */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},   /* md_static_ratio */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},   /* md_static_finestr */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* tnr_tss */
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, /* tnr_tfr */
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, /* tnr_tfs */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_md */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_bright_thresh */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_dark_thresh */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* user_define_color_thresh */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, /* sfr_r */
            {32, 32, 32, 32, 32, 32, 32, 30, 30, 30, 30, 30, 30, 30, 30, 30}, /* sfr_g */
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
            0, /* user_define_dark_thresh */
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


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_OMNIVISION_OS08A20_OS08A20_CMOS_EX_H_BODY_PART02_H_ */
