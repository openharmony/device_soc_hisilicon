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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART06_SUB02_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART06_SUB02_H_

#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
static td_void tde_hal_node_set_cmp_from_osd_mode(tde_hw_node *hw_node, td_u32 width)
{
    td_u32 mb_num_x = (width + 31) / 32; /* 31 and 32 is 32 bytes align */
    td_u32 osd_mode = hw_node->tde_od_pic_osd_glb_info.bits.osd_mode;
    td_u32 budget_bits_mb = hw_node->tde_od_pic_osd_rc_cfg0.bits.budget_bits_mb;
    td_u32 adj_sad_bits_thr = (width <= 320) ? 0 : ((width <= 720) ?  8 : 30); /* 320 720 8 30 alg data */
    td_u32 qp_inc1_bits_thr = (width <= 320) ? 0 : ((width <= 720) ? 20 : /* 320 720 20 alg data */
                               tde_node_max(20, tde_node_min(40, mb_num_x - 3))); /* 20 40 3 alg data */
    /* 720 150 80 120 200 3 alg data */
    td_u32 qp_dec1_bits_thr = (width <= 720) ? 150 : tde_node_max(80, tde_node_min(120, 200 - 3 * mb_num_x));
    /* 720 200 140 180 350 4 alg data */
    td_u32 qp_dec2_bits_thr = (width <= 720) ? 200 : tde_node_max(140, tde_node_min(180, 350 - 4 * mb_num_x));
    /* 720 5 8 3 4 alg data */
    td_u32 min_mb_bits = (width <= 720) ? (budget_bits_mb * 5 / 8) : (budget_bits_mb * 3 / 4);

    if ((hw_node->tde_od_pic_osd_glb_info.bits.osd_mode == 0) ||
        (hw_node->tde_od_pic_osd_glb_info.bits.osd_mode == 1)) {
        hw_node->tde_od_pic_osd_rc_cfg7.bits.adj_sad_bits_thr = adj_sad_bits_thr;
        hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_inc1_bits_thr = qp_inc1_bits_thr;
        hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec1_bits_thr = qp_dec1_bits_thr;
        hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec2_bits_thr = qp_dec2_bits_thr;
        hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec3_bits_thr = 255; /* 255 alg data */
        hw_node->tde_od_pic_osd_rc_cfg2.bits.max_qp = (osd_mode == 0) ? 5 : 4; /* 5 4 alg data */
        hw_node->tde_od_pic_osd_rc_cfg2.bits.rc_smth_ngain = (width < 1280) ? 3 : 4; /* 1280 3 4 alg data */
        hw_node->tde_od_pic_osd_rc_cfg2.bits.special_bits_gain = (width <= 720) ? 0 : 4; /* 720 4 alg data */
        hw_node->tde_od_pic_osd_rc_cfg7.bits.max_trow_bits = (width <= 720) ? 0 : 10; /* 720 10 alg data */
        hw_node->tde_od_pic_osd_rc_cfg0.bits.min_mb_bits = min_mb_bits;
    } else {
        hw_node->tde_od_pic_osd_rc_cfg7.bits.adj_sad_bits_thr = 10; /* 10 alg data */
        hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_inc1_bits_thr = 20; /* 20 alg data */
        hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec1_bits_thr = 60; /* 60 alg data */
        hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec2_bits_thr = 100; /* 100 alg data */
        hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec3_bits_thr = 130; /* 130 alg data */
        hw_node->tde_od_pic_osd_rc_cfg2.bits.max_qp = 1;
        hw_node->tde_od_pic_osd_rc_cfg2.bits.rc_smth_ngain = 3; /* 3 alg data */
        hw_node->tde_od_pic_osd_rc_cfg2.bits.special_bits_gain = 2; /* 2 alg data */
        hw_node->tde_od_pic_osd_rc_cfg7.bits.max_trow_bits = 10; /* 10 alg data */
        hw_node->tde_od_pic_osd_rc_cfg0.bits.min_mb_bits = (budget_bits_mb * 3) / 4; /* 3 4 alg data */
    }
}

static td_void tde_hal_node_set_cmp(tde_hw_node *hw_node, td_u32 width)
{
    td_u32 bit_dep_a;
    td_u32 bit_dep_rgb;
    td_u32 mb_org_bits;
    td_u32 mb_num_x;
    td_u32 ctl_delta;
    td_u32 osd_mode;
    td_u32 budget_bits_mb;
    td_u32 comp_ratio;
    osd_mode = hw_node->tde_od_pic_osd_glb_info.bits.osd_mode;
    if (osd_mode == 1) {
        comp_ratio = 2000; /* 2000 alg data */
    } else {
        comp_ratio = 1000; /* 1000 alg data */
    }
    mb_num_x = (width + 31) / 32; /* 31 32 alg data */
    /* 8 6 2 4 alg data */
    bit_dep_a = (osd_mode == 0) ? 8 : ((osd_mode == 1) ? 6 : ((osd_mode == 2) ? 1 : 4));
    /* 8 2 5 4 alg data */
    bit_dep_rgb = (osd_mode == 0) ? 8 : ((osd_mode == 1) ? 8 : ((osd_mode == 2) ? 5 : 4));
    mb_org_bits = 32 * (bit_dep_a + bit_dep_rgb * 3); /* 32 3 alg data */
    ctl_delta = (width <= 720) ? ((20 * 16 + 500) / mb_num_x) :  /* 720 20 16 500 alg data */
                ((((40 * 16 + 1000) / mb_num_x) > 20) ? ((40 * 16 + 1000) / mb_num_x) : 20); /* 40 16 1000 20 alg */

    ctl_delta = ((width % 32) != 0) ? (ctl_delta + 8) : ctl_delta; /* 32 8 alg data */

    if ((osd_mode != 0) && (osd_mode != 1)) {
        ctl_delta = 20; /* 20 alg data */
    }
    budget_bits_mb = ((mb_org_bits * 1000 / comp_ratio - ctl_delta) > 256) ? /* 1000 256 alg data */
                      (mb_org_bits * 1000 / comp_ratio - ctl_delta) : 256; /* 1000 256 alg data */
    budget_bits_mb = (budget_bits_mb > 1023) ?  1023 :  budget_bits_mb; /* 1023 alg data */
    hw_node->tde_od_pic_osd_rc_cfg0.bits.budget_bits_mb         = budget_bits_mb;
    hw_node->tde_od_pic_osd_rc_cfg1.bits.budget_bits_mb_cap     = 1023; /* 1023 alg data */
    hw_node->tde_od_pic_osd_rc_cfg6.bits.pix_diff_thr           = 50; /* 50 alg data */
    hw_node->tde_od_pic_osd_rc_cfg4.bits.smth_thr               = 3; /* 3 alg data */
    hw_node->tde_od_pic_osd_rc_cfg4.bits.still_thr              = 1;
    hw_node->tde_od_pic_osd_rc_cfg4.bits.big_grad_thr           = 16; /* 16 alg data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.smth_pix_num_thr       = 6; /* 6 alg data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.still_pix_num_thr      = 3; /* 3 alg data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.large_smth_pix_num_thr = 10; /* 10 alg data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.noise_pix_num_thr      = 24; /* 24 alg data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.smth_qp                = 2; /* 2 alg data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.sad_bits_ngain         = 10; /* 10 alg data */
    hw_node->tde_od_pic_osd_rc_cfg3.bits.min_sad_thr            = 12; /* 12 alg data */
    hw_node->tde_od_pic_osd_rc_cfg3.bits.max_sad_thr            = 64; /* 64 alg data */
    hw_node->tde_od_pic_osd_rc_cfg6.bits.noise_sad              = 32; /* 32 alg data */
    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr           = 70; /* 70 alg data */
    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr_cap       = 20; /* 20 alg data */
    tde_hal_node_set_cmp_from_osd_mode(hw_node, width);
}

#ifndef CONFIG_COMPRESS_ECONOMIZE_MEMERY
static td_void tde_hal_node_set_argb8888_320(tde_hw_node *hw_node)
{
    hw_node->tde_od_pic_osd_rc_cfg0.bits.budget_bits_mb = 994; /* 994 budget_bits_mb data */
    hw_node->tde_od_pic_osd_rc_cfg0.bits.min_mb_bits = 750; /* 750 min_mb_bits data */

    hw_node->tde_od_pic_osd_rc_cfg1.bits.budget_bits_mb_cap = 512; /* 512 budget_bits_mb_cap data */
    hw_node->tde_od_pic_osd_rc_cfg7.bits.max_trow_bits = 0;

    hw_node->tde_od_pic_osd_rc_cfg2.bits.max_qp = 5; /* 5 max_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.smth_qp = 2; /* 2 smth_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.sad_bits_ngain = 10; /* 10 sad_bits_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.rc_smth_ngain = 4; /* 4 rc_smth_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.special_bits_gain = 0;

    hw_node->tde_od_pic_osd_rc_cfg3.bits.max_sad_thr = 64; /* 64 max_sad_thr data */
    hw_node->tde_od_pic_osd_rc_cfg3.bits.min_sad_thr = 12; /* 12 min_sad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg4.bits.smth_thr = 3; /* 3 smth_thr data */
    hw_node->tde_od_pic_osd_rc_cfg4.bits.still_thr = 1;
    hw_node->tde_od_pic_osd_rc_cfg4.bits.big_grad_thr = 16; /* 16 big_grad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg5.bits.smth_pix_num_thr = 6; /* 6 smth_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.still_pix_num_thr = 3; /* 3 still_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.noise_pix_num_thr = 24; /* 24 noise_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.large_smth_pix_num_thr = 10; /* 10 large_smth_pix_num_thr data */

    hw_node->tde_od_pic_osd_rc_cfg6.bits.noise_sad = 32; /* 32 noise_sad data */
    hw_node->tde_od_pic_osd_rc_cfg6.bits.pix_diff_thr = 50; /* 50 pix_diff_thr data */

    hw_node->tde_od_pic_osd_rc_cfg7.bits.adj_sad_bits_thr = 0;

    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_inc1_bits_thr = 0;
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec1_bits_thr = 60; /* 60 qp_dec1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec2_bits_thr = 130; /* 130 qp_dec2_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec3_bits_thr = 250; /* 250 qp_dec3_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr = 70; /* 70 force_qp_thr data */
    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr_cap = 20; /* 20 force_qp_thr_cap data */
}

static td_void tde_hal_node_set_argb8888_720(tde_hw_node *hw_node)
{
    hw_node->tde_od_pic_osd_rc_cfg0.bits.budget_bits_mb = 492; /* 492 budget_bits_mb data */
    hw_node->tde_od_pic_osd_rc_cfg0.bits.min_mb_bits = 375; /* 375 min_mb_bits data */

    hw_node->tde_od_pic_osd_rc_cfg1.bits.budget_bits_mb_cap = 512; /* 512 budget_bits_mb_cap data */
    hw_node->tde_od_pic_osd_rc_cfg7.bits.max_trow_bits = 0;

    hw_node->tde_od_pic_osd_rc_cfg2.bits.max_qp = 5; /* 5 max_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.smth_qp = 2; /* 2 smth_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.sad_bits_ngain = 10; /* 10 sad_bits_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.rc_smth_ngain = 3; /* 3 rc_smth_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.special_bits_gain = 0;

    hw_node->tde_od_pic_osd_rc_cfg3.bits.max_sad_thr = 64; /* 64 max_sad_thr data */
    hw_node->tde_od_pic_osd_rc_cfg3.bits.min_sad_thr = 12; /* 12 min_sad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg4.bits.smth_thr = 3; /* 3 smth_thr data */
    hw_node->tde_od_pic_osd_rc_cfg4.bits.still_thr = 1;
    hw_node->tde_od_pic_osd_rc_cfg4.bits.big_grad_thr = 16; /* 16 big_grad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg5.bits.smth_pix_num_thr = 6; /* 6 smth_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.still_pix_num_thr = 3; /* 3 still_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.noise_pix_num_thr = 24; /* 24 noise_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.large_smth_pix_num_thr = 10; /* 10 large_smth_pix_num_thr data */

    hw_node->tde_od_pic_osd_rc_cfg6.bits.noise_sad = 32; /* 32 noise_sad data */
    hw_node->tde_od_pic_osd_rc_cfg6.bits.pix_diff_thr = 50; /* 50 pix_diff_thr data */

    hw_node->tde_od_pic_osd_rc_cfg7.bits.adj_sad_bits_thr = 8; /* 8 adj_sad_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_inc1_bits_thr = 20; /* 20 qp_inc1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec1_bits_thr = 60; /* 60 qp_dec1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec2_bits_thr = 130; /* 130 qp_dec2_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec3_bits_thr = 250; /* 250 qp_dec3_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr = 70; /* 70 force_qp_thr data */
    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr_cap = 20; /* 20 force_qp_thr_cap data */
}

static td_void tde_hal_node_set_argb8888_3840(tde_hw_node *hw_node)
{
    hw_node->tde_od_pic_osd_rc_cfg0.bits.budget_bits_mb = 492; /* 492 budget_bits_mb data */
    hw_node->tde_od_pic_osd_rc_cfg0.bits.min_mb_bits = 375; /* 375 min_mb_bits data */

    hw_node->tde_od_pic_osd_rc_cfg1.bits.budget_bits_mb_cap = 512; /* 512 budget_bits_mb_cap data */
    hw_node->tde_od_pic_osd_rc_cfg7.bits.max_trow_bits = 10; /* 10 max_trow_bits data */

    hw_node->tde_od_pic_osd_rc_cfg2.bits.max_qp = 5; /* 5 max_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.smth_qp = 2; /* 2 smth_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.sad_bits_ngain = 10; /* 10 sad_bits_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.rc_smth_ngain = 4; /* 4 rc_smth_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.special_bits_gain = 4; /* 4 special_bits_gain data */

    hw_node->tde_od_pic_osd_rc_cfg3.bits.max_sad_thr = 64; /* 64 max_sad_thr data */
    hw_node->tde_od_pic_osd_rc_cfg3.bits.min_sad_thr = 12; /* 12 min_sad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg4.bits.smth_thr = 3; /* 3 smth_thr data */
    hw_node->tde_od_pic_osd_rc_cfg4.bits.still_thr = 1;
    hw_node->tde_od_pic_osd_rc_cfg4.bits.big_grad_thr = 16; /* 16 big_grad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg5.bits.smth_pix_num_thr = 6; /* 6 smth_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.still_pix_num_thr = 3; /* 3 still_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.noise_pix_num_thr = 24; /* 24 noise_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.large_smth_pix_num_thr = 10; /* 10 large_smth_pix_num_thr data */

    hw_node->tde_od_pic_osd_rc_cfg6.bits.noise_sad = 32; /* 32 noise_sad data */
    hw_node->tde_od_pic_osd_rc_cfg6.bits.pix_diff_thr = 50; /* 50 pix_diff_thr data */

    hw_node->tde_od_pic_osd_rc_cfg7.bits.adj_sad_bits_thr = 30; /* 30 adj_sad_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_inc1_bits_thr = 40; /* 40 qp_inc1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec1_bits_thr = 60; /* 60 qp_dec1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec2_bits_thr = 100; /* 100 qp_dec2_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec3_bits_thr = 250; /* 250 qp_dec3_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr = 70; /* 70 force_qp_thr data */
    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr_cap = 20; /* 20 force_qp_thr_cap data */
}
#endif

static td_void tde_hal_node_set_rgb888_320(tde_hw_node *hw_node)
{
    hw_node->tde_od_pic_osd_rc_cfg0.bits.budget_bits_mb = 930; /* 930 budget_bits_mb data */
    hw_node->tde_od_pic_osd_rc_cfg0.bits.min_mb_bits = 702; /* 702 min_mb_bits data */

    hw_node->tde_od_pic_osd_rc_cfg1.bits.budget_bits_mb_cap = 512; /* 512 budget_bits_mb_cap data */
    hw_node->tde_od_pic_osd_rc_cfg7.bits.max_trow_bits = 0;

    hw_node->tde_od_pic_osd_rc_cfg2.bits.max_qp = 4; /* 4 max_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.smth_qp = 2; /* 2 smth_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.sad_bits_ngain = 10; /* 10 sad_bits_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.rc_smth_ngain = 4; /* 4 rc_smth_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.special_bits_gain = 0;

    hw_node->tde_od_pic_osd_rc_cfg3.bits.max_sad_thr = 64; /* 64 max_sad_thr data */
    hw_node->tde_od_pic_osd_rc_cfg3.bits.min_sad_thr = 12; /* 12 min_sad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg4.bits.smth_thr = 3; /* 3 smth_thr data */
    hw_node->tde_od_pic_osd_rc_cfg4.bits.still_thr = 1;
    hw_node->tde_od_pic_osd_rc_cfg4.bits.big_grad_thr = 16; /* 16 big_grad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg5.bits.smth_pix_num_thr = 6; /* 6 smth_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.still_pix_num_thr = 3; /* 3 still_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.noise_pix_num_thr = 24; /* 24 noise_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.large_smth_pix_num_thr = 10; /* 10 large_smth_pix_num_thr data */

    hw_node->tde_od_pic_osd_rc_cfg6.bits.noise_sad = 32; /* 32 noise_sad data */
    hw_node->tde_od_pic_osd_rc_cfg6.bits.pix_diff_thr = 50; /* 50 pix_diff_thr data */

    hw_node->tde_od_pic_osd_rc_cfg7.bits.adj_sad_bits_thr = 0;

    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_inc1_bits_thr = 0;
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec1_bits_thr = 60; /* 60 qp_dec1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec2_bits_thr = 130; /* 130 qp_dec2_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec3_bits_thr = 250; /* 250 qp_dec3_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr = 70; /* 70 force_qp_thr data */
    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr_cap = 20; /* 20 force_qp_thr_cap data */
}

static td_void tde_hal_node_set_rgb888_720(tde_hw_node *hw_node)
{
    hw_node->tde_od_pic_osd_rc_cfg0.bits.budget_bits_mb = 500; /* 500 budget_bits_mb data */
    hw_node->tde_od_pic_osd_rc_cfg0.bits.min_mb_bits = 375; /* 375 min_mb_bits data */

    hw_node->tde_od_pic_osd_rc_cfg1.bits.budget_bits_mb_cap = 512; /* 512 budget_bits_mb_cap data */
    hw_node->tde_od_pic_osd_rc_cfg7.bits.max_trow_bits = 0;

    hw_node->tde_od_pic_osd_rc_cfg2.bits.max_qp = 4; /* 4 max_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.smth_qp = 2; /* 2 smth_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.sad_bits_ngain = 10; /* 10 sad_bits_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.rc_smth_ngain = 3; /* 3 rc_smth_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.special_bits_gain = 0;

    hw_node->tde_od_pic_osd_rc_cfg3.bits.max_sad_thr = 64; /* 64 max_sad_thr data */
    hw_node->tde_od_pic_osd_rc_cfg3.bits.min_sad_thr = 12; /* 12 min_sad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg4.bits.smth_thr = 3; /* 3 smth_thr data */
    hw_node->tde_od_pic_osd_rc_cfg4.bits.still_thr = 1;
    hw_node->tde_od_pic_osd_rc_cfg4.bits.big_grad_thr = 16; /* 16 big_grad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg5.bits.smth_pix_num_thr = 6; /* 6 smth_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.still_pix_num_thr = 3; /* 3 still_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.noise_pix_num_thr = 24; /* 24 noise_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.large_smth_pix_num_thr = 10; /* 10 large_smth_pix_num_thr data */

    hw_node->tde_od_pic_osd_rc_cfg6.bits.noise_sad = 32; /* 32 noise_sad data */
    hw_node->tde_od_pic_osd_rc_cfg6.bits.pix_diff_thr = 50; /* 50 pix_diff_thr data */

    hw_node->tde_od_pic_osd_rc_cfg7.bits.adj_sad_bits_thr = 8; /* 8 adj_sad_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_inc1_bits_thr = 20; /* 20 qp_inc1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec1_bits_thr = 60; /* 60 qp_dec1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec2_bits_thr = 130; /* 130 qp_dec2_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec3_bits_thr = 250; /* 250 qp_dec3_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr = 70; /* 70 force_qp_thr data */
    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr_cap = 20; /* 20 force_qp_thr_cap data */
}

static td_void tde_hal_node_set_rgb888_3840(tde_hw_node *hw_node)
{
    hw_node->tde_od_pic_osd_rc_cfg0.bits.budget_bits_mb = 500; /* 500 budget_bits_mb data */
    hw_node->tde_od_pic_osd_rc_cfg0.bits.min_mb_bits = 375; /* 375 min_mb_bits data */

    hw_node->tde_od_pic_osd_rc_cfg1.bits.budget_bits_mb_cap = 512; /* 512 budget_bits_mb_cap data */
    hw_node->tde_od_pic_osd_rc_cfg7.bits.max_trow_bits = 10; /* 10 max_trow_bits data */

    hw_node->tde_od_pic_osd_rc_cfg2.bits.max_qp = 4; /* 4 max_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.smth_qp = 2; /* 2 smth_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.sad_bits_ngain = 10; /* 10 sad_bits_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.rc_smth_ngain = 4; /* 4 rc_smth_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.special_bits_gain = 4; /* 4 special_bits_gain data */

    hw_node->tde_od_pic_osd_rc_cfg3.bits.max_sad_thr = 64; /* 64 max_sad_thr data */
    hw_node->tde_od_pic_osd_rc_cfg3.bits.min_sad_thr = 12; /* 12 min_sad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg4.bits.smth_thr = 3; /* 3 smth_thr data */
    hw_node->tde_od_pic_osd_rc_cfg4.bits.still_thr = 1;
    hw_node->tde_od_pic_osd_rc_cfg4.bits.big_grad_thr = 16; /* 16 big_grad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg5.bits.smth_pix_num_thr = 6; /* 6 smth_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.still_pix_num_thr = 3; /* 3 still_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.noise_pix_num_thr = 24; /* 24 noise_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.large_smth_pix_num_thr = 10; /* 10 large_smth_pix_num_thr data */

    hw_node->tde_od_pic_osd_rc_cfg6.bits.noise_sad = 32; /* 32 noise_sad data */
    hw_node->tde_od_pic_osd_rc_cfg6.bits.pix_diff_thr = 50; /* 50 pix_diff_thr data */

    hw_node->tde_od_pic_osd_rc_cfg7.bits.adj_sad_bits_thr = 30; /* 30 adj_sad_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_inc1_bits_thr = 40; /* 40 qp_inc1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec1_bits_thr = 60; /* 60 qp_dec1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec2_bits_thr = 100; /* 100 qp_dec2_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec3_bits_thr = 250; /* 250 qp_dec3_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr = 70; /* 70 force_qp_thr data */
    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr_cap = 20; /* 20 force_qp_thr_cap data */
}

static td_void tde_hal_node_set_argb15555_or_argb4444_720(tde_hw_node *hw_node)
{
    hw_node->tde_od_pic_osd_rc_cfg0.bits.budget_bits_mb = 492; /* 492 budget_bits_mb data */
    hw_node->tde_od_pic_osd_rc_cfg0.bits.min_mb_bits = 375; /* 375 min_mb_bits data */

    hw_node->tde_od_pic_osd_rc_cfg1.bits.budget_bits_mb_cap = 512; /* 512 budget_bits_mb_cap data */
    hw_node->tde_od_pic_osd_rc_cfg7.bits.max_trow_bits = 10; /* 10 max_trow_bits data */

    hw_node->tde_od_pic_osd_rc_cfg2.bits.max_qp = 1;
    hw_node->tde_od_pic_osd_rc_cfg2.bits.smth_qp = 2; /* 2 smth_qp data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.sad_bits_ngain = 10; /* 10 sad_bits_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.rc_smth_ngain = 3; /* 3 rc_smth_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.special_bits_gain = 2; /* 2 special_bits_gain data */

    hw_node->tde_od_pic_osd_rc_cfg3.bits.max_sad_thr = 64; /* 64 max_sad_thr data */
    hw_node->tde_od_pic_osd_rc_cfg3.bits.min_sad_thr = 12; /* 12 min_sad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg4.bits.smth_thr = 3; /* 3 smth_thr data */
    hw_node->tde_od_pic_osd_rc_cfg4.bits.still_thr = 1;
    hw_node->tde_od_pic_osd_rc_cfg4.bits.big_grad_thr = 16; /* 16 big_grad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg5.bits.smth_pix_num_thr = 6; /* 6 smth_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.still_pix_num_thr = 3; /* 3 still_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.noise_pix_num_thr = 24; /* 24 noise_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.large_smth_pix_num_thr = 10; /* 10 large_smth_pix_num_thr data */

    hw_node->tde_od_pic_osd_rc_cfg6.bits.noise_sad = 32; /* 32 noise_sad data */
    hw_node->tde_od_pic_osd_rc_cfg6.bits.pix_diff_thr = 50; /* 50 pix_diff_thr data */

    hw_node->tde_od_pic_osd_rc_cfg7.bits.adj_sad_bits_thr = 10; /* 10 adj_sad_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_inc1_bits_thr = 20; /* 20 qp_inc1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec1_bits_thr = 60; /* 60 qp_dec1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec2_bits_thr = 100; /* 100 qp_dec2_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec3_bits_thr = 130; /* 130 qp_dec3_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr = 70; /* 70 force_qp_thr data */
    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr_cap = 20; /* 20 force_qp_thr_cap data */
}

static td_void tde_hal_node_set_argb15555_or_argb4444_3840(tde_hw_node *hw_node)
{
    hw_node->tde_od_pic_osd_rc_cfg0.bits.budget_bits_mb = 492; /* 492 budget_bits_mb data */
    hw_node->tde_od_pic_osd_rc_cfg0.bits.min_mb_bits = 375; /* 375 min_mb_bits data */

    hw_node->tde_od_pic_osd_rc_cfg1.bits.budget_bits_mb_cap = 512; /* 512 budget_bits_mb_cap data */
    hw_node->tde_od_pic_osd_rc_cfg7.bits.max_trow_bits = 10; /* 10 max_trow_bits data */

    hw_node->tde_od_pic_osd_rc_cfg2.bits.max_qp = 1;
    hw_node->tde_od_pic_osd_rc_cfg2.bits.smth_qp = 2; /* 2 smth_qp data */

    hw_node->tde_od_pic_osd_rc_cfg2.bits.sad_bits_ngain = 10; /* 10 sad_bits_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.rc_smth_ngain = 3; /* 3 rc_smth_ngain data */
    hw_node->tde_od_pic_osd_rc_cfg2.bits.special_bits_gain = 2; /* 2 special_bits_gain data */

    hw_node->tde_od_pic_osd_rc_cfg3.bits.max_sad_thr = 64; /* 64 max_sad_thr data */
    hw_node->tde_od_pic_osd_rc_cfg3.bits.min_sad_thr = 12; /* 12 min_sad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg4.bits.smth_thr = 3; /* 3 smth_thr data */
    hw_node->tde_od_pic_osd_rc_cfg4.bits.still_thr = 1;
    hw_node->tde_od_pic_osd_rc_cfg4.bits.big_grad_thr = 16; /* 16 big_grad_thr data */

    hw_node->tde_od_pic_osd_rc_cfg5.bits.smth_pix_num_thr = 6; /* 6 smth_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.still_pix_num_thr = 3; /* 3 still_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.noise_pix_num_thr = 24; /* 24 noise_pix_num_thr data */
    hw_node->tde_od_pic_osd_rc_cfg5.bits.large_smth_pix_num_thr = 10; /* 10 large_smth_pix_num_thr data */

    hw_node->tde_od_pic_osd_rc_cfg6.bits.noise_sad = 32; /* 32 noise_sad data */
    hw_node->tde_od_pic_osd_rc_cfg6.bits.pix_diff_thr = 50; /* 50 pix_diff_thr data */

    hw_node->tde_od_pic_osd_rc_cfg7.bits.adj_sad_bits_thr = 10; /* 10 adj_sad_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_inc1_bits_thr = 20; /* 20 qp_inc1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec1_bits_thr = 60; /* 60 qp_dec1_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec2_bits_thr = 100; /* 100 qp_dec2_bits_thr data */
    hw_node->tde_od_pic_osd_rc_cfg8.bits.qp_dec3_bits_thr = 130; /* 130 qp_dec3_bits_thr data */

    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr = 70; /* 70 force_qp_thr data */
    hw_node->tde_od_pic_osd_rc_cfg9.bits.force_qp_thr_cap = 20; /* 20 force_qp_thr_cap data */
}

td_void tde_hal_node_set_compress(tde_hw_node *hw_node)
{
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }

    hw_node->tde_od_pic_osd_glb_info.bits.is_lossless = 0;
    hw_node->tde_od_pic_osd_glb_info.bits.is_lossless_alpha = hw_node->tde_od_pic_osd_glb_info.bits.is_lossless;
    hw_node->tde_od_pic_osd_glb_info.bits.cmp_mode = 0;
    hw_node->tde_od_pic_osd_glb_info.bits.osd_mode =
        (hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_ARGB8888) ? 0 :
        ((hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_RGB888) ? 1 :
        ((hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_ARGB1555) ? 2 : 3)); /* 2 3 des_fmt value */
    hw_node->tde_od_pic_osd_glb_info.bits.partition_en = 0;
    hw_node->tde_od_pic_osd_glb_info.bits.part_num = 1;

    hw_node->tde_od_pic_osd_frame_size.bits.frame_width = hw_node->des_imgsize.bits.des_width;
    hw_node->tde_od_pic_osd_frame_size.bits.frame_height = hw_node->des_imgsize.bits.des_height;

    if (hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_ARGB8888) {
#ifndef CONFIG_COMPRESS_ECONOMIZE_MEMERY
        if (hw_node->des_imgsize.bits.des_width <= 320) { /* 320 set argb8888 */
            tde_hal_node_set_argb8888_320(hw_node);
        } else if (hw_node->des_imgsize.bits.des_width <= 720) { /* 720 set argb8888 */
            tde_hal_node_set_argb8888_720(hw_node);
        } else if (hw_node->des_imgsize.bits.des_width <= 3840) { /* 3840 set argb8888 */
            tde_hal_node_set_argb8888_3840(hw_node);
        }
#else
        tde_hal_node_set_cmp(hw_node, hw_node->des_imgsize.bits.des_width);
#endif
    } else if (hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_RGB888) {
        if (hw_node->des_imgsize.bits.des_width <= 320) { /* 320 set rgb888 */
            tde_hal_node_set_rgb888_320(hw_node);
        } else if (hw_node->des_imgsize.bits.des_width <= 720) { /* 720 set rgb888 */
            tde_hal_node_set_rgb888_720(hw_node);
        } else if (hw_node->des_imgsize.bits.des_width <= 3840) { /* 3840 set rgb888 */
            tde_hal_node_set_rgb888_3840(hw_node);
        }
    } else if ((hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_ARGB1555) ||
               (hw_node->des_ctrl.bits.des_fmt == TDE_DRV_COLOR_FMT_ARGB4444)) {
        if (hw_node->des_imgsize.bits.des_width <= 720) { /* 720 set argb15555 or argb4444 */
            tde_hal_node_set_argb15555_or_argb4444_720(hw_node);
        } else if (hw_node->des_imgsize.bits.des_width <= 3840) { /* 3840 set argb15555 or argb4444 */
            tde_hal_node_set_argb15555_or_argb4444_3840(hw_node);
        }
    }
}
#endif /* CONFIG_TDE_GFBG_COMPRESS_V1 */


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART06_SUB02_H_ */
