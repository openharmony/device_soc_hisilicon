/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART06_SUB03_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART06_SUB03_H_


#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
static td_void tde_hal_node_set_cmp_rc_base(tde_hw_node *hw_node)
{
    hw_node->tde_line_osd_cmp_rc_cfg1.bits.smth_thr = 8; /* 8 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg1.bits.still_thr = 1;
    hw_node->tde_line_osd_cmp_rc_cfg1.bits.big_grad_thr = 45; /* 45 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg1.bits.diff_thr = 30; /* 30 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg2.bits.smth_pix_num_thr = 6; /* 6 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg2.bits.still_pix_num_thr = 8; /* 8 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg2.bits.noise_pix_num_thr = 30; /* 30 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg2.bits.raw_bits_penalty = 10; /* 10 alg data */
}

static td_void tde_hal_node_set_cmp_cfg(tde_hw_node *hw_node, td_u32 max_mb_qp, td_u32 bit_depth_rgb)
{
    td_u32 qp_rge_reg0;
    td_u32 qp_rge_reg1;
    td_u32 qp_rge_reg2;

    tde_hal_node_set_cmp_rc_base(hw_node);

    if (max_mb_qp > (bit_depth_rgb / 2)) { /* 2 alg data */
        qp_rge_reg0 = (tde_node_clip(max_mb_qp - 4) << 28) | (0 << 24) | /* 4 28 24 alg data */
                      (tde_node_clip(max_mb_qp - 5) << 20) | (0 << 16) | /* 5 20 16 alg data */
                      (tde_node_clip(max_mb_qp - 6) << 12) | (0 << 8) | /* 6 12 8 alg data */
                      (tde_node_clip(max_mb_qp - 7) << 4) | 0; /* 7 4 alg data */
        qp_rge_reg1 = (tde_node_clip(max_mb_qp - 3) << 28) | (0 << 24) | /* 3 28 24 alg data */
                      (tde_node_clip(max_mb_qp - 4) << 20) | (0 << 16) | /* 4 20 16 alg data */
                      (tde_node_clip(max_mb_qp - 4) << 12) | (0 << 8) | /* 4 12 8 alg data */
                      (tde_node_clip(max_mb_qp - 4) << 4) | 0; /* 4 alg data */
        /* 28 4 24 20 16 alg data */
        qp_rge_reg2 = (max_mb_qp << 28) | (4 << 24) | (tde_node_clip(max_mb_qp - 1) << 20) | (4 << 16) |
            /* 2 12 8 4 alg data */
            (tde_node_clip(max_mb_qp - 2) << 12) | (2 << 8) | (tde_node_clip(max_mb_qp - 2) << 4) | 0;
    } else {
        qp_rge_reg0 = (tde_node_clip(max_mb_qp - 3) << 28) | (0 << 24) | /* 3 28 24 alg data */
                      (tde_node_clip(max_mb_qp - 3) << 20) | (0 << 16) | /* 3 20 16 alg data */
                      (tde_node_clip(max_mb_qp - 3) << 12) | (0 << 8) | /* 3 12 8 alg data */
                      (tde_node_clip(max_mb_qp - 3) << 4) | 0; /* 3 4 alg data */
        qp_rge_reg1 = (tde_node_clip(max_mb_qp - 2) << 28) | (1 << 24) | /* 2 28 24 alg data */
                      (tde_node_clip(max_mb_qp - 2) << 20) | (1 << 16) | /* 2 20 16 alg data */
                      (tde_node_clip(max_mb_qp - 2) << 12) | (1 << 8) | /* 2 12 8 alg data */
                      (tde_node_clip(max_mb_qp - 3) << 4) | 1; /* 3 4 alg data */
        qp_rge_reg2 = (max_mb_qp << 28) | (2 << 24) | (tde_node_clip(max_mb_qp - 1) << 20) | /* 28 2 24 20 alg data */
                      (2 << 16) | (tde_node_clip(max_mb_qp - 1) << 12) | (2 << 8) | /* 2 16 12 2 8 alg data */
                      (tde_node_clip(max_mb_qp - 1) << 4) | 2; /* 4 2 alg data */
    }

    hw_node->tde_line_osd_cmp_rc_cfg8.bits.qp_rge_reg0 = qp_rge_reg0;
    hw_node->tde_line_osd_cmp_rc_cfg9.bits.qp_rge_reg1 = qp_rge_reg1;
    hw_node->tde_line_osd_cmp_rc_cfg10.bits.qp_rge_reg2 = qp_rge_reg2;

    /* 71 24 56 16 35 8 18 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg5.bits.buf_fullness_thr_reg0 = (71 << 24) | (56 << 16) | (35 << 8) | 18;
    /* 116 24 108 16 98 8 85 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg6.bits.buf_fullness_thr_reg1 = (116 << 24) | (108 << 16) | (98 <<  8) | 85;
    /* 24 123 16 121 8 119 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg7.bits.buf_fullness_thr_reg2 = (0 << 24) | (123 << 16) | (121 << 8) | 119;
    /* 0xff 24 16 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg11.bits.bits_offset_reg0 = ((1 & 0xff) << 24) | ((1 & 0xff) << 16) |
                                                                ((1 & 0xff) << 8) | (1 & 0xff); /* 0xff 8 alg data */
    /* 0xff 24 16 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg12.bits.bits_offset_reg1 = ((0 & 0xff) << 24) | ((0 & 0xff) << 16) |
                                                                ((0 & 0xff) << 8) | (0 & 0xff); /* 0xff 8 alg data */
    /* -4 0xff 24 -3 16 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg13.bits.bits_offset_reg2 = ((-4 & 0xff) << 24) | ((-3 & 0xff) << 16) |
        ((-2 & 0xff) << 8) | (-1 & 0xff); /* -2 0xff 8 -1 alg data */
    /* 7 28 24 6 20 5 16 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg14.bits.est_err_gain_map = (7 << 28) | (7 << 24) | (6 << 20) | (5 << 16) |
        (4 << 12) | (4 << 8) | (3 << 4) | 3; /* 4 12 8 3 alg data */
}

static td_void tde_hal_node_set_cmp_qp_thr(tde_hw_node *hw_node, td_u32 osd_mode)
{
    /* 96 32 16 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg3.bits.qp_inc1_bits_thr = (unsigned int)(96 * (32 * ((osd_mode <= 1) ? 32 : 16)) /
        (32 * 8 * 3)); /* 32 8 3 alg data */
    /* 96 32 16 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg3.bits.qp_inc2_bits_thr = (unsigned int)(96 * (32 * ((osd_mode <= 1) ? 32 : 16)) /
        (32 * 8 * 3)); /* 32 8 3 alg data */
    /* 30 32 16 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg4.bits.qp_dec1_bits_thr = (unsigned int)(30 * (32 * ((osd_mode <= 1) ? 32 : 16)) /
        (32 * 8 * 3)); /* 32 8 3 alg data */
    /* 62 32 16 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg4.bits.qp_dec2_bits_thr = (unsigned int)(62 * (32 * ((osd_mode <= 1) ? 32 : 16)) /
        (32 * 8 * 3)); /* 32 8 3 alg data */
}

static td_void tde_hal_node_set_cmp_status(tde_hw_node *hw_node, td_u32 budget_mb_bits)
{
    hw_node->tde_line_osd_cmp_rc_cfg15.bits.smooth_status_thr = 9; /* 9 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg15.bits.min_mb_bits = budget_mb_bits * 3 / 4; /* 3 4 alg data */
    /* 255 2 alg data */
    hw_node->v4r2_line_osd_cmp_rc_cfg16.bits.first_mb_adj_bits = tde_node_min(255, budget_mb_bits >> 2);
    /* 255 4 alg data */
    hw_node->v4r2_line_osd_cmp_rc_cfg16.bits.first_row_adj_bits = tde_node_min(255, budget_mb_bits >> 4);
    /* 255 4 alg data */
    hw_node->v4r2_line_osd_cmp_rc_cfg16.bits.first_col_adj_bits = tde_node_min(255, budget_mb_bits >> 4);
    /* 6 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg17.bits.still_status_thr = 6;
    /* 64 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg17.bits.still_diff_thr = 64;
}

static td_void tde_hal_node_set_cmp(tde_hw_node *hw_node, td_u32 width)
{
    td_u32 osd_mode;
    td_u32 mb_ori_bits;
    td_u32 budget_mb_bits;
    td_u32 bit_depth_rgb;
    td_u32 bit_depth_a;
    td_u32 delta;
    td_u32 buffer_init_bits;
    td_u32 mb_num_x;
    td_u32 last_mb_width;
    td_u32 max_mb_qp;
    const td_u32 max_head_bits_raw = 1;
    td_u32 comp_ratio;
    osd_mode = hw_node->tde_line_osd_cmp_glb_info.bits.osd_mode;
    if (osd_mode == 1) {
        comp_ratio = 2000; /* 2000 alg data */
    } else {
        comp_ratio = 1000; /* 1000 alg data */
    }

    /* for calculate budget_mb_bits */
    bit_depth_a = (osd_mode <= 1) ? 8 : ((osd_mode == 2) ? 1 : 4); /* 8 2 4 alg data */
    bit_depth_rgb = (osd_mode <= 1) ? 8 : ((osd_mode == 2) ? 5 : 4); /* 8 2 5 4 alg data */
    mb_num_x = (width + 32 - 1) / 32; /* 32 alg data */
    /* 2 9 7946 7000 18 8082 alg data */
    buffer_init_bits = (osd_mode < 2) ? ((mb_num_x < 9) ? 7946 : 7000) : ((mb_num_x < 18) ? 8082 : 7000);
    delta = tde_node_max(1, (9216 - buffer_init_bits + mb_num_x - 1) / (mb_num_x)); /* 9216 alg data */
    mb_ori_bits = 32 * (bit_depth_rgb * 3 + bit_depth_a); /* 32 3 alg data */
    budget_mb_bits = mb_ori_bits * 1000 / comp_ratio - delta; /* 1000 alg data */
    budget_mb_bits = tde_node_min(tde_node_max(budget_mb_bits, 64), mb_ori_bits); /* 64 alg data */

    /* for calculate max_mb_qp */
    if (osd_mode == 2) { /* 2 alg data */
        /* 32 3 alg data */
        max_mb_qp = (bit_depth_rgb) - ((budget_mb_bits - bit_depth_a * 32 - max_head_bits_raw - 1) / (32 * 3)) + 1;
    } else {
        /* 5 2 alg data */
        max_mb_qp = (bit_depth_rgb) - ((budget_mb_bits - max_head_bits_raw - 1) >> (5 + 2)) + 1;
    }

    /* for calculate budget_mb_bits_last */
    last_mb_width = (width % 32) ? (width % 32) : 32; /* 32 alg data */

    /* config reg */
    hw_node->tde_line_osd_cmp_rc_cfg0.bits.budget_mb_bits = budget_mb_bits;
    hw_node->tde_line_osd_cmp_rc_cfg0.bits.max_mb_qp = max_mb_qp;
    /* 32 alg data */
    hw_node->tde_line_osd_cmp_rc_cfg0.bits.budget_mb_bits_last = (budget_mb_bits * last_mb_width) / 32;
    tde_hal_node_set_cmp_qp_thr(hw_node, osd_mode);
    tde_hal_node_set_cmp_cfg(hw_node, max_mb_qp, bit_depth_rgb);
    tde_hal_node_set_cmp_status(hw_node, budget_mb_bits);
}

td_void tde_hal_node_set_compress(tde_hw_node *hw_node)
{
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }
    hw_node->tde_line_osd_cmp_glb_info.bits.ice_en = 1;
    hw_node->tde_line_osd_cmp_glb_info.bits.cmp_mode = 0;
    hw_node->tde_line_osd_cmp_glb_info.bits.is_lossless = 0;

    hw_node->tde_line_osd_cmp_glb_info.bits.osd_mode = (hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_RGB888) ?
        0 : ((hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_ARGB8888) ?
        1 : ((hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_ARGB1555) ?
        2 : 3)); /* 2 3 des_fmt value */

    hw_node->tde_line_osd_cmp_glb_info.bits.conv_en = (hw_node->tde_line_osd_cmp_glb_info.bits.osd_mode > 1) ? 0 : 1;

    hw_node->tde_line_osd_cmp_frame_size.bits.frame_width = hw_node->des_imgsize.bits.des_width;
    hw_node->tde_line_osd_cmp_frame_size.bits.frame_height = hw_node->des_imgsize.bits.des_height;

    tde_hal_node_set_cmp(hw_node, hw_node->des_imgsize.bits.des_width);
    return;
}
#endif /* CONFIG_TDE_GFBG_COMPRESS_V2 */

td_void tde_hal_node_set_compress_tqt(tde_hw_node *hw_node, tde_surface_msg *drv_surface,
                                      tde_drv_outalpha_from alpha_from)
{
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }
    tde_hal_node_set_tqt(hw_node, drv_surface, alpha_from);
    hw_node->des_ctrl.bits.cmp_en = 1;
    return;
}


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART06_SUB03_H_ */
