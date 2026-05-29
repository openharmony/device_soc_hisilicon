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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART06_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART06_H_

#if (TDE_CAPABILITY & SLICE)
/* data_in :positive or negetive  ;  bit:complement width */
static td_u32 tde_true_value_to_complement(td_s32 data_in, td_u32 bit)
{
    td_u32 data_out;
    td_u32 data_in_tmp;

    if (data_in >> (bit - 1)) {
        data_in_tmp = 0 - data_in;
        data_out = ((1 << (bit - 1)) | (((~data_in_tmp) & ((1 << (bit - 1)) - 1)) + 1)) & ((1 << bit) - 1);
    } else {
        data_out = data_in;
    }

    return data_out;
}

/* data_in :positive or negetive  ;  bit:complement width */
static td_s32 tde_complement_to_true_value(td_u32 data_in, td_u32 bit)
{
    td_s32 data_out;
    td_u32 data_in_tmp;

    if (data_in >> (bit - 1)) {
        data_in_tmp = data_in & ((1 << (bit - 1)) - 1);
        data_out = (((~data_in_tmp) & ((1 << (bit - 1)) - 1)) + 1) * (-1);
    } else {
        data_out = data_in;
    }

    return data_out;
}

static td_void tde_osi_init_slice_data(tde_slice_data *slice_data, const tde_hw_node *child_node, td_u32 i)
{
    if (i == 0) {
        slice_data->fmt = child_node->src1_ctrl.bits.src1_fmt;
        slice_data->hor_scan_ord = child_node->src1_ctrl.bits.src1_h_scan_ord;
        slice_data->width = child_node->src1_imgsize.bits.src1_width + 1;

        slice_data->u32422v_pro = child_node->src1_ctrl.bits.src1_422v_pro;

        slice_data->hlmsc_en = child_node->src1_hsp.bits.hlmsc_en;
        slice_data->hchmsc_en = child_node->src1_hsp.bits.hchmsc_en;
        slice_data->hratio = child_node->src1_hsp.bits.hratio;
        slice_data->hor_loffset = child_node->src1_hloffset;
        slice_data->hor_coffset = child_node->src1_hcoffset;
        slice_data->zme_ow = child_node->src1_zmeoreso.bits.ow + 1;
        slice_data->zme_iw = child_node->src1_zmeireso.bits.iw + 1;
        slice_data->hpzme_en = child_node->src1_hpzme.bits.src1_hpzme_en;
        slice_data->hpzme_mode = child_node->src1_hpzme.bits.src1_hpzme_mode;
        slice_data->hpzme_width = child_node->src1_hpzme_size.bits.src1_hpzme_width + 1;
    } else {
        slice_data->fmt = child_node->src2_ctrl.bits.src2_fmt;
        slice_data->hor_scan_ord = child_node->src2_ctrl.bits.src2_h_scan_ord;
        slice_data->width = child_node->src2_imgsize.bits.src2_width + 1;
        slice_data->u32422v_pro = child_node->src2_ctrl.bits.src2_422v_pro;
        slice_data->hlmsc_en =  child_node->src2_hsp.bits.hlmsc_en;
        slice_data->hchmsc_en = child_node->src2_hsp.bits.hchmsc_en;
        slice_data->hratio = child_node->src2_hsp.bits.hratio;
        slice_data->hor_loffset =  child_node->src2_hloffset;
        slice_data->hor_coffset =  child_node->src2_hcoffset;
        slice_data->zme_ow = child_node->src2_zmeoreso.bits.ow + 1;
        slice_data->zme_iw = child_node->src2_zmeireso.bits.iw + 1;
        slice_data->hpzme_en =  child_node->src2_hpzme.bits.src2_hpzme_en;
        slice_data->hpzme_mode = child_node->src2_hpzme.bits.src2_hpzme_mode;
        slice_data->hpzme_width = child_node->src2_hpzme_size.bits.src2_hpzme_width + 1;
    }

    return;
}

static td_void tde_osi_calc_slice_s1(tde_slice_data *slice_data)
{
    slice_data->xdpos = (slice_data->hlmsc_en | slice_data->hchmsc_en) ? (slice_data->zme_ow - 1) :
        (slice_data->hpzme_en ? (slice_data->hpzme_width - 1) : (slice_data->width - 1));
    slice_data->xed_pos_blk = MIN2(slice_data->des_xed_pos_blk, slice_data->xdpos);
    /* step2 : out pos relative to s1 disp start */
    slice_data->xst_pos_cord = slice_data->des_xst_pos_blk;
    slice_data->xed_pos_cord = slice_data->xed_pos_blk;

    /* setp2 : calc s1 zme parameter */
    slice_data->xst_pos_cord_in = (slice_data->xst_pos_cord * slice_data->hratio) >> 20; /* 20 alg data */
    slice_data->xed_pos_cord_in = (slice_data->xed_pos_cord * slice_data->hratio) >> 20; /* 20 alg data */
    slice_data->hor_loffset_cfg_int_comp = slice_data->hor_loffset >> 20; /* 20 alg data */
    /* 8 alg data */
    slice_data->hor_loffset_cfg_int = tde_complement_to_true_value(slice_data->hor_loffset_cfg_int_comp, 8);
    slice_data->hor_loffset_cfg_fraction = slice_data->hor_loffset & 0xfffff;
    slice_data->hor_loffset_pix_fraction = (slice_data->xst_pos_cord * slice_data->hratio) & 0xfffff;
    slice_data->hor_loffset_fraction = (slice_data->hor_loffset_cfg_fraction +
                                        slice_data->hor_loffset_pix_fraction) & 0xfffff;

    slice_data->xst_pos_cord_in_offset = slice_data->xst_pos_cord_in + slice_data->hor_loffset_cfg_int + (
        ((slice_data->hor_loffset_cfg_fraction + slice_data->hor_loffset_pix_fraction) & 0xfff00000) != 0);
    slice_data->xed_pos_cord_in_offset = slice_data->xed_pos_cord_in + slice_data->hor_loffset_cfg_int + (
        ((slice_data->hor_loffset_cfg_fraction + slice_data->hor_loffset_pix_fraction) & 0xfff00000) != 0);
    slice_data->xst_pos_cord_in_tap_rgb = (slice_data->xst_pos_cord_in_offset < 0) ? 0 : (
        (slice_data->xst_pos_cord_in_offset >= (TDE_MAX_ZOOM_OUT_STEP / 2 - 1)) ?  /* 2 alg data */
        (slice_data->xst_pos_cord_in_offset - (TDE_MAX_ZOOM_OUT_STEP / 2 - 1)) : 0); /* 2 alg data */
    slice_data->xed_pos_cord_in_tap_rgb = (slice_data->xed_pos_cord_in_offset + TDE_MAX_ZOOM_OUT_STEP / 2) < 0 ?
        0 : (((slice_data->xed_pos_cord_in_offset + TDE_MAX_ZOOM_OUT_STEP / 2) >= /* 2 alg data */
        (slice_data->zme_iw - 1)) ? (slice_data->zme_iw - 1) : (slice_data->xed_pos_cord_in_offset +
        TDE_MAX_ZOOM_OUT_STEP / 2)); /* 2 alg data */
    slice_data->node_cfg_zme_iw_rgb = slice_data->xed_pos_cord_in_tap_rgb - slice_data->xst_pos_cord_in_tap_rgb + 1;
    slice_data->hor_loffset_int = (slice_data->xst_pos_cord_in_offset - slice_data->xst_pos_cord_in_tap_rgb);
    /* 8 alg data */
    slice_data->hor_loffset_int_complement = tde_true_value_to_complement(slice_data->hor_loffset_int, 8);
    slice_data->node_cfg_hor_loffset_rgb = (slice_data->hor_loffset_int_complement << 20) + /* 20 alg data */
                                            slice_data->hor_loffset_fraction;
    slice_data->xst_pos_cord_in_tap_luma = ((slice_data->xst_pos_cord_in_tap_rgb % 2) == 1) ? /* 2 alg data */
        (slice_data->xst_pos_cord_in_tap_rgb - 1) : slice_data->xst_pos_cord_in_tap_rgb;
    slice_data->xed_pos_cord_in_tap_luma = (((slice_data->xed_pos_cord_in_tap_rgb % 2) == 0) ? /* 2 alg data */
        (slice_data->xed_pos_cord_in_tap_rgb + 1) : slice_data->xed_pos_cord_in_tap_rgb) > (
        /* 2 alg data */
        slice_data->zme_iw - 1) ? (slice_data->zme_iw - 1) : (((slice_data->xed_pos_cord_in_tap_rgb % 2) == 0) ?
        (slice_data->xed_pos_cord_in_tap_rgb + 1) : slice_data->xed_pos_cord_in_tap_rgb);
    slice_data->xst_pos_cord_in_chroma = (slice_data->fmt == 50) ? (slice_data->xst_pos_cord * /* 50 alg data */
        (slice_data->hratio)) >> 20 : (slice_data->xst_pos_cord * (slice_data->hratio / 2)) >> 20; /* 20 alg data */
    slice_data->xed_pos_cord_in_chroma = (slice_data->fmt == 50) ? (slice_data->xed_pos_cord * /* 50 alg data */
        (slice_data->hratio)) >> 20 : (slice_data->xed_pos_cord * (slice_data->hratio / 2)) >> 20; /* 20 alg data */
    slice_data->hor_coffset_cfg_int_comp = slice_data->hor_coffset >> 20; /* 20 alg data */
    /* 8 alg data */
    slice_data->hor_coffset_cfg_int = tde_complement_to_true_value(slice_data->hor_coffset_cfg_int_comp, 8);
    slice_data->hor_coffset_cfg_fraction = slice_data->hor_coffset & 0xfffff;
    return;
}

static td_void tde_osi_calc_slice_sp(tde_slice_data *slice_data)
{
    /* 50 alg data */
    slice_data->hor_coffset_pix_fraction = ((slice_data->fmt == 50) ? (slice_data->xst_pos_cord *
        (slice_data->hratio)) : (slice_data->xst_pos_cord * (slice_data->hratio / 2))) & 0xfffff; /* 2 alg data */
    slice_data->hor_coffset_fraction = (slice_data->hor_coffset_cfg_fraction +
        slice_data->hor_coffset_pix_fraction) & 0xfffff;
    slice_data->xst_pos_cord_in_offset_chroma = slice_data->xst_pos_cord_in_chroma +
        slice_data->hor_coffset_cfg_int + (((slice_data->hor_coffset_cfg_fraction +
        slice_data->hor_coffset_pix_fraction) & 0xfff00000) != 0);
    slice_data->xed_pos_cord_in_offset_chroma = slice_data->xed_pos_cord_in_chroma +
        slice_data->hor_coffset_cfg_int + (((slice_data->hor_coffset_cfg_fraction +
        slice_data->hor_coffset_pix_fraction) & 0xfff00000) != 0);
    slice_data->xst_pos_cord_in_tap_chroma = (slice_data->xst_pos_cord_in_offset_chroma < 0) ? 0 :
        ((slice_data->xst_pos_cord_in_offset_chroma >= (TDE_MAX_ZOOM_OUT_STEP / 2 - 1)) ? /* 2 alg data */
        (slice_data->xst_pos_cord_in_offset_chroma - (TDE_MAX_ZOOM_OUT_STEP / 2 - 1)) : 0); /* 2 alg data */
    /* 2 alg data */
    slice_data->xed_pos_cord_in_tap_chroma = (slice_data->xed_pos_cord_in_offset_chroma + TDE_MAX_ZOOM_OUT_STEP / 2)
        < 0 ? 0 : (((slice_data->xed_pos_cord_in_offset_chroma + TDE_MAX_ZOOM_OUT_STEP / 2) >= /* 2 alg data */
        /* 2 50 alg data */
        ((slice_data->fmt == 50) ? ((slice_data->zme_iw - 1)) : ((slice_data->zme_iw - 1) / 2))) ? ((
        /* 2 50 alg data */
        slice_data->fmt == 50) ? ((slice_data->zme_iw - 1)) : ((slice_data->zme_iw - 1) / 2)) :
        (slice_data->xed_pos_cord_in_offset_chroma + TDE_MAX_ZOOM_OUT_STEP / 2)); /* 2 alg data */
    /* 50 alg data */
    slice_data->xst_pos_cord_in_tap_chroma_x2 = (slice_data->fmt == 50) ? slice_data->xst_pos_cord_in_tap_chroma :
                                                 (slice_data->xst_pos_cord_in_tap_chroma * 2); /* 2 alg data */
    /* 50 alg data */
    slice_data->xed_pos_cord_in_tap_chroma_x2 = (slice_data->fmt == 50) ? slice_data->xed_pos_cord_in_tap_chroma :
                                                 (slice_data->xed_pos_cord_in_tap_chroma * 2 + 1); /* 2 alg data */

    slice_data->xst_pos_cord_in_tap_sp = MIN2(slice_data->xst_pos_cord_in_tap_luma,
                                              slice_data->xst_pos_cord_in_tap_chroma_x2);
    slice_data->xed_pos_cord_in_tap_sp = MAX2(slice_data->xed_pos_cord_in_tap_luma,
                                              slice_data->xed_pos_cord_in_tap_chroma_x2);

    slice_data->node_cfg_zme_iw_sp = slice_data->xed_pos_cord_in_tap_sp - slice_data->xst_pos_cord_in_tap_sp + 1;
    slice_data->hor_loffset_int_sp = (slice_data->xst_pos_cord_in_offset - slice_data->xst_pos_cord_in_tap_sp);
    slice_data->hor_coffset_int_sp = (slice_data->xst_pos_cord_in_offset_chroma - (
        /* 50 2 alg data */
        (slice_data->fmt == 50) ? slice_data->xst_pos_cord_in_tap_sp : (slice_data->xst_pos_cord_in_tap_sp / 2)));
    /* 8 alg data */
    slice_data->hor_loffset_int_sp_complent = tde_true_value_to_complement(slice_data->hor_loffset_int_sp, 8);
    /* 8 alg data */
    slice_data->hor_coffset_int_sp_complent = tde_true_value_to_complement(slice_data->hor_coffset_int_sp, 8);
    slice_data->node_cfg_hor_loffset_sp = (slice_data->hor_loffset_int_sp_complent << 20) + /* 20 alg data */
                                           slice_data->hor_loffset_fraction;
    slice_data->node_cfg_hor_coffset_sp = (slice_data->hor_coffset_int_sp_complent << 20) + /* 20 alg data */
                                           slice_data->hor_coffset_fraction;
    return;
}

static td_void tde_osi_calc_slice_node(tde_slice_data *slice_data, tde_hw_node *child_node, td_u32 i)
{
    td_bool set_slice_size;
    /* 0 2 1 alg data */
    set_slice_size = ((slice_data->hpzme_en == 0) && (slice_data->slice_width % 2 == 1) &&
        ((slice_data->fmt == 48) || ((slice_data->fmt == 50) && !slice_data->u32422v_pro) || /* 48 50 alg data */
        (slice_data->fmt == 52)) && (slice_data->xed_pos_cord_in_tap_hpzme_hso == slice_data->width - 1) && /* 52 alg */
        (slice_data->width % 2 == 0)); /* 2 alg data */
    if (set_slice_size) {
        if (slice_data->hor_scan_ord) {
            slice_data->slice_wi = slice_data->slice_wi + 1;
            slice_data->slice_width = slice_data->slice_width + 1;
            slice_data->slice_hoffset = slice_data->slice_hoffset - 1;
        } else {
            slice_data->slice_wi = slice_data->slice_wi + 1;
            slice_data->slice_width = slice_data->slice_width + 1;
            slice_data->slice_hoffset = slice_data->slice_hoffset - 1;
            slice_data->slice_l_ofst = slice_data->slice_l_ofst + (
            slice_data->hor_loffset_int_beyond_complent << 20); /* 20 alg data */
            slice_data->slice_c_ofst = slice_data->slice_c_ofst + (
            slice_data->hor_loffset_int_beyond_complent << 20); /* 20 alg data */
        }
    }

    if (i == 0) {
        child_node->src1_zmeoreso.bits.ow = slice_data->slice_wo;
        child_node->src1_hloffset = slice_data->slice_l_ofst;
        child_node->src1_hcoffset = slice_data->slice_c_ofst;
        child_node->src1_pix_offset.bits.src1_hoffset_pix = slice_data->slice_hoffset;
        child_node->src1_imgsize.bits.src1_width = slice_data->slice_width - 1;
        child_node->src1_zmeireso.bits.iw = slice_data->slice_wi;
        child_node->src1_hpzme_size.bits.src1_hpzme_width = slice_data->slice_w_hpzme - 1;

        return;
    }

    child_node->src2_zmeoreso.bits.ow = slice_data->slice_wo;
    child_node->src2_hloffset = slice_data->slice_l_ofst;
    child_node->src2_hcoffset = slice_data->slice_c_ofst;
    child_node->src2_pix_offset.bits.src2_hoffset_pix = slice_data->slice_hoffset;
    child_node->src2_imgsize.bits.src2_width = slice_data->slice_width - 1;
    child_node->src2_zmeireso.bits.iw = slice_data->slice_wi;
    child_node->src2_hpzme_size.bits.src2_hpzme_width = slice_data->slice_w_hpzme - 1;
}

static td_void tde_osi_calc_slice_mux(tde_slice_data *slice_data, tde_hw_node *child_node, td_u32 i)
{
    td_bool cfg_zme_iw_sp;
    td_bool cfg_hor_loffset_sp;
    td_bool cfg_hor_coffset_sp;

    slice_data->slice_wo = slice_data->xed_pos_cord - slice_data->xst_pos_cord + 1;
    /* 32 48 50 alg data */
    cfg_zme_iw_sp = ((slice_data->fmt >= 32) && (slice_data->fmt != 48) && (!((slice_data->fmt == 50) &&
                     !slice_data->u32422v_pro)) && (slice_data->fmt != 52)); /* 52 alg data */
    slice_data->slice_wi = (cfg_zme_iw_sp) ? slice_data->node_cfg_zme_iw_sp : ((slice_data->hlmsc_en ||
                            slice_data->hchmsc_en) ? slice_data->node_cfg_zme_iw_rgb : slice_data->slice_wo);
    /* 32 48 50 alg data */
    cfg_hor_loffset_sp = ((slice_data->fmt >= 32) && (slice_data->fmt != 48) && (!((slice_data->fmt == 50) &&
                          !slice_data->u32422v_pro)) && (slice_data->fmt != 52)); /* 52 alg data */
    slice_data->slice_l_ofst = (cfg_hor_loffset_sp) ? slice_data->node_cfg_hor_loffset_sp :
                                slice_data->node_cfg_hor_loffset_rgb;

    cfg_hor_coffset_sp = cfg_hor_loffset_sp;
    slice_data->slice_c_ofst = (cfg_hor_coffset_sp) ? slice_data->node_cfg_hor_coffset_sp :
                                slice_data->node_cfg_hor_loffset_rgb;

    slice_data->slice_w_hpzme = slice_data->slice_wi;
    slice_data->slice_wo = slice_data->slice_wo - 1;
    slice_data->slice_wi = slice_data->slice_wi - 1;

    slice_data->xst_pos_cord_in_tap = (slice_data->hlmsc_en || slice_data->hchmsc_en) ?
        /* 32 48 50 alg data */
        (((slice_data->fmt >= 32) && (slice_data->fmt != 48) && (!((slice_data->fmt == 50) &&
        /* 52 alg data */
        !slice_data->u32422v_pro)) && (slice_data->fmt != 52)) ? slice_data->xst_pos_cord_in_tap_sp :
        slice_data->xst_pos_cord_in_tap_rgb) : slice_data->xst_pos_cord;

    slice_data->xed_pos_cord_in_tap = (slice_data->hlmsc_en || slice_data->hchmsc_en) ?
        /* 32 48 50 alg data */
        (((slice_data->fmt >= 32) && (slice_data->fmt != 48) && (!((slice_data->fmt == 50) &&
        /* 52 alg data */
        !slice_data->u32422v_pro)) && (slice_data->fmt != 52)) ? slice_data->xed_pos_cord_in_tap_sp :
        slice_data->xed_pos_cord_in_tap_rgb) : slice_data->xed_pos_cord;

    slice_data->xst_pos_cord_in_tap_hpzme = slice_data->hpzme_en ? (slice_data->xst_pos_cord_in_tap *
       (slice_data->hpzme_mode + 1)) : slice_data->xst_pos_cord_in_tap;
    slice_data->xed_pos_cord_in_tap_hpzme = slice_data->hpzme_en ? ((slice_data->xed_pos_cord_in_tap + 1) *
       (slice_data->hpzme_mode + 1) - 1) : slice_data->xed_pos_cord_in_tap;

    if (slice_data->hor_scan_ord) {
        slice_data->xst_pos_cord_in_tap_hpzme_hso = slice_data->width - 1 - slice_data->xed_pos_cord_in_tap_hpzme;
        slice_data->xed_pos_cord_in_tap_hpzme_hso = slice_data->width - 1 - slice_data->xst_pos_cord_in_tap_hpzme;
    } else {
        slice_data->xst_pos_cord_in_tap_hpzme_hso = slice_data->xst_pos_cord_in_tap_hpzme;
        slice_data->xed_pos_cord_in_tap_hpzme_hso = slice_data->xed_pos_cord_in_tap_hpzme;
    }

    slice_data->slice_width = (slice_data->xed_pos_cord_in_tap_hpzme_hso -
                               slice_data->xst_pos_cord_in_tap_hpzme_hso + 1);
    slice_data->slice_hoffset = slice_data->xst_pos_cord_in_tap_hpzme_hso;

    slice_data->hor_loffset_int_beyond = 1;
    /* 8 alg data */
    slice_data->hor_loffset_int_beyond_complent = tde_true_value_to_complement(slice_data->hor_loffset_int_beyond, 8);
}

static td_void tde_osi_finalize_slice_calculation(tde_slice_data *slice_data, tde_hw_node *child_node)
{
    slice_data->slice_dst_width = slice_data->des_xed_pos_blk - slice_data->des_xst_pos_blk + 1;
    slice_data->slice_dst_hoffset = (slice_data->des_h_scan_ord ? (slice_data->des_width - 1 -
        slice_data->des_xed_pos_blk) : slice_data->des_xst_pos_blk) + slice_data->des_hoffset_pix;
    slice_data->des_crop_en = (!((slice_data->des_crop_start_x > slice_data->des_xed_pos_blk) ||
        (slice_data->des_crop_end_x < slice_data->des_xst_pos_blk))) && slice_data->des_crop_en;
    slice_data->des_crop_start_x = slice_data->des_crop_en ? (MAX2(slice_data->des_xst_pos_blk,
        slice_data->des_crop_start_x) - slice_data->des_xst_pos_blk) : 0;
    slice_data->des_crop_end_x = slice_data->des_crop_en ? (MIN2(slice_data->des_xed_pos_blk,
        slice_data->des_crop_end_x) - slice_data->des_xst_pos_blk) : 0;

    child_node->des_imgsize.bits.des_width = slice_data->slice_dst_width - 1;
    child_node->des_pix_offset.bits.des_hoffset_pix = slice_data->slice_dst_hoffset;
    child_node->des_alpha.bits.des_crop_en = slice_data->des_crop_en;
    child_node->des_crop_pos_st.bits.des_crop_start_x = slice_data->des_crop_start_x;
    child_node->des_crop_pos_ed.bits.des_crop_end_x = slice_data->des_crop_end_x;

    return;
}

static td_s32 tde_osi_calc_slice(td_s32 handle, const tde_hw_node *node)
{
    tde_slice_data slice_data = { 0 };
    td_u32 n;
    td_u32 i;
    td_s32 ret;

    slice_data.des_width = node->des_imgsize.bits.des_width + 1;
    slice_data.des_hoffset_pix = 0;
    slice_data.des_h_scan_ord = node->des_ctrl.bits.des_h_scan_ord;

    slice_data.des_crop_en = node->des_alpha.bits.des_crop_en;
    slice_data.des_crop_start_x = node->des_crop_pos_st.bits.des_crop_start_x;
    slice_data.des_crop_end_x = node->des_crop_pos_ed.bits.des_crop_end_x;
    /* 256 alg data */
    slice_data.node_num = slice_data.des_width / 256 + ((slice_data.des_width % 256) != 0);

    for (n = 0; n < slice_data.node_num; n++) {
        tde_hw_node *child_node;
        td_void *buf = TD_NULL;
        buf = (td_void *)tde_malloc(sizeof(tde_hw_node) + TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE);
        if (buf == TD_NULL) {
            tde_error("malloc (%d) failed, wgetfreenum(%d)!\n", (sizeof(tde_hw_node)), wgetfreenum());
            return DRV_ERR_TDE_NO_MEM;
        }

        child_node = (tde_hw_node *)(buf + TDE_NODE_HEAD_BYTE);

        if (memcpy_s(child_node, sizeof(tde_hw_node), node, sizeof(tde_hw_node)) != EOK) {
            tde_hal_free_node_buf(child_node);
            return DRV_ERR_TDE_INVALID_PARA;
        }

        slice_data.des_xst_pos_blk = n * TDE_MAX_SLICE_WIDTH;
        slice_data.des_xed_pos_blk = (((n + 1) * TDE_MAX_SLICE_WIDTH) <= slice_data.des_width) ? (
            ((n + 1) * TDE_MAX_SLICE_WIDTH) - 1) : (slice_data.des_width - 1);

        for (i = 0; i < 2; i++) { /* 2 alg data */
            tde_osi_init_slice_data(&slice_data, child_node, i);

            tde_osi_calc_slice_s1(&slice_data);

            tde_osi_calc_slice_sp(&slice_data);

            tde_osi_calc_slice_mux(&slice_data, child_node, i);
        }

        tde_osi_finalize_slice_calculation(&slice_data, child_node);
        ret = tde_osi_set_node_finish(handle, child_node, 0, TDE_NODE_SUBM_ALONE);
        if (ret != TD_SUCCESS) {
            tde_hal_free_node_buf(child_node);
            return ret;
        }
    }
    return TD_SUCCESS;
}

#endif


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART06_H_ */
