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

#ifndef SYS_CMP_H
#define SYS_CMP_H

#include "ot_type.h"

#define RC_MANNUAL_CFG 0

#define CMP_RATIO_NUM 1000ULL
#define CMP_3DNR_FRAME_CH_ADD 16
#define CMP_3DNR_FRAME_BUF_ADD 1024
#define CMP_3DNR_FRAME_Y_HEAD_SIZE 32

#define MIN_CMP_RATIO_8BIT 1000
#define MIN_CMP_RATIO_10BIT 1250
#define MAX_CMP_RATIO 2000

/* 典型段压缩率，根据需要评估修改 */
#define VI_SEG_COMPRESS_RATIO_8BIT_LUMA 1300
#define VPSS_SEG_COMPRESS_RATIO_8BIT_LUMA 1300
#define SEG_COMPRESS_RATIO_8BIT_LUMA 1600
#define SEG_COMPRESS_RATIO_8BIT_CHROMA 2000
#define SEG_COMPRESS_RATIO_10BIT_LUMA 1667
#define SEG_COMPRESS_RATIO_10BIT_CHROMA 2000
#define SEG128_WIDTH_BIT_ALIGN 128
#define SEG_COMPRESS_MIN_WIDTH 256

typedef struct {
    td_s32 frame_width;  /* input frame width */
    td_s32 frame_height;  /* input frame height */
    td_s32 cmp_mode;  /* 0 -- cmp; 1 -- bypass; 2--esl_mode */
    td_s32 seg_en;    /* 1-- seg mode 0 -- line mode */
    td_s32 tile_en;
    td_s32 tile_height;
    td_s32 bit_depth; /* bit width   0-- 8bit  1-- 10bit */
    td_s32 pix_format; /* 0 -- 420 1 -- 422 */
    td_s32 esl_qp;
    td_s32 is_lossless;  /* 1: is lossless compress; 0: is lossy compress */
    td_s32 mirror_en;
    td_s32 chroma_en;
    td_s32 comp_ratio_int;  /* luma&chroma compression ratio, (=comp_ratio * 1000) */
    td_s32 rc_type_mode;  /* 0: default; 1: random; 2: max value; 3: min value; 4: max/min combination */
} cmp_seg_256x1_input;

typedef struct {
    td_u32 chroma_en;
    td_u32 is_lossless;
    td_u32 cmp_mode;
    td_u32 seg_en;
    td_u32 tile_en;
    td_u32 tile_height;
    td_u32 bit_depth;
    td_u32 esl_qp;
    td_u32 mirror_en;
    td_u32 frame_width;
    td_u32 frame_height;

    /* rc parameter */
    td_u32 big_grad_thr;
    td_u32 diff_thr;
    td_u32 noise_pix_num_thr;
    td_u32 qp_inc1_bits_thr;
    td_u32 qp_inc2_bits_thr;
    td_u32 qp_dec1_bits_thr;
    td_u32 qp_dec2_bits_thr;
    td_u32 buf_fullness_thr_reg0;
    td_u32 buf_fullness_thr_reg1;
    td_u32 buf_fullness_thr_reg2;
    td_u32 qp_rge_reg0;
    td_u32 qp_rge_reg1;
    td_u32 qp_rge_reg2;
    td_u32 bits_offset_reg0;
    td_u32 bits_offset_reg1;
    td_u32 bits_offset_reg2;
    td_u32 est_err_gain_map;
    td_u32 buffer_size;
    td_u32 buffer_init_bits;
    td_u32 smooth_status_thr;
    td_u32 budget_mb_bits_last;
    td_u32 min_mb_bits;
    td_u32 max_mb_qp;
    td_s32 budget_mb_bits;

    /* reserved parameter */
    td_u32 reserve_para0;
    td_u32 reserve_para1;

    /* add parameter */
    td_u8 smooth_deltabits_thr; /* used to determine the number of bits of smooth_delta */
    td_u32 adpqp_thr0;    /* adpqp_thr0,1,2,3 */
    td_u32 adpqp_thr1;    /* adpQp_thr4,5, and adpQP clip_thr */

    td_u32 seg_raw_en;
} cmp_seg_256x1_output;

typedef struct {
    td_s32 frame_width;  /* input frame width */
    td_s32 frame_height;  /* input frame height */
    td_s32 frame_bitdepth;  /* pixel bit depth, 1: 10 bit; 2: 12 bit; 3:14bit */
    td_s32 cmp_mode;  /* 0 -- cmp; 1 -- bypass */
    td_s32 is_lossless;  /* 1: is lossless compress; 0: is lossy compress */
    td_s32 bayer_mode;  /* bayer mode: 0--RGGB 1--GRBG 2--GBRG 3--BGGR */
    td_s32 stride_en;  /* 0 -- frame; 1 -- line */
    td_s32 align_mode;  /* 0-- 1burst align; 1-- 2burst align */
    td_s32 comp_ratio_int;  /* luma&chroma compression ratio, (=comp_ratio * 1000) */
    td_s32 rc_type_mode;  /* 0: default; 1: random; 2: max value; 3: min value; 4: max/min combination */
} cmp_bayer_input;

typedef struct {
    td_u32 stride_en;
    td_u32 stride;
    td_u32 align_mode;
    td_u32 is_lossless;
    td_u32 cmp_mode;
    td_u32 bayer_mode;
    td_u32 bit_depth;
    td_u32 frame_width;
    td_u32 frame_height;
    td_u32 rm_debug_en;

    /* rc parameter */
    td_u32 smth_thr;
    td_u32 still_thr;
    td_u32 big_grad_thr;
    td_u32 smth_pix_num_thr;
    td_u32 still_pix_num_thr;
    td_u32 noise_pix_num_thr;
    td_u32 qp_inc1_bits_thr;
    td_u32 qp_inc2_bits_thr;
    td_u32 buf_fullness_thr_reg0;
    td_u32 buf_fullness_thr_reg1;
    td_u32 buf_fullness_thr_reg2;
    td_u32 qp_rge_reg0;
    td_u32 qp_rge_reg1;
    td_u32 qp_rge_reg2;
    td_u32 bits_offset_reg0;
    td_u32 bits_offset_reg1;
    td_u32 bits_offset_reg2;
    td_u32 est_err_gain_map;
    td_u32 buffer_init_bits;
    td_u32 first_mb_adj_bits;
    td_u32 first_row_adj_bits;
    td_u32 first_col_adj_bits;
    td_u32 smooth_status_thr;
    td_u32 budget_mb_bits;
    td_u32 budget_mb_bits_last;
    td_u32 min_mb_bits;
    td_u32 max_mb_qp;
    td_u32 qp_force_en;
} cmp_bayer_output;

typedef struct {
    td_s32 bayer_mode;  /* bayer mode, 0:RGGB; 1: GRBG 2: GBRG; 3: BGGR */
    td_s32 cmp_mode;  /* 1: is raw frame compress mode; 0: is not ... */
    td_s32 is_lossless;  /* 1: is lossless compress; 0: is lossy compress */

    td_s32 frame_width;  /* input frame width */
    td_s32 frame_height;  /* input frame height */
    td_s32 frame_bitdepth;  /* pixel bit depth, 1: 10 bit; 2: 12 bit; 3: 14 bit */

    td_s32 stride_en;  /* bitstream store format in ddr, 0: frame; 1: stride */
    td_s32 stride;  /* bitstream stride per line(unit is 128bit) */
    td_s32 align_mode;  /* burst align mode, 0: 1burst align; 1: 2burst align */

    td_s32 bd_det_en;
    td_s32 buffer_init_bits;
    td_s32 buffer_fullness_thr;
    td_s32 budget_mb_bits;

    td_s32 nl2_num_thr;
    td_s32 nl1_num_thr;
    td_s32 noise_grad_thr;

    td_s32 rm_debug_en;  /* 1: rm debug enable     0: rm debug disable */
} dcmp_bayer_output;

typedef struct {
    td_s32    frame_width;           /* input frame width */
    td_s32    frame_height;          /* input frame height */
    td_s32    frame_bitdepth;        /* pixel bit depth,  0--8bit 1--10bit  2--12bit  3--14bit  4--16bit */
    td_s32    cmp_mode;              /* 0 -- cmp 1 -- bypass */
    td_s32    is_lossless;           /* 0--lossy 1--lossless */
    td_s32    bayer_mode;            /* # 0 --RGGB  1 --GRBG  2 --GBRG  3 --BGGR */
    td_s32    comp_ratio_int;        /* =comp_ratio * 1000 */
    td_s32    rc_type_mode;          /* 0: default  1: random  2: max value  3: min value   4: max/min combination */
} cmp_bnr_input;

typedef struct {
    td_u32   is_lossless;
    td_u32   cmp_mode;
    td_u32   bayer_mode;
    td_u32   bit_depth;
    td_u32   frame_width;
    td_u32   frame_height;
    td_u32   rm_debug_en; /* no reg */
    td_u32   bs_size;     /* no reg */

    /* rc parameter */
    td_u32 smth_thr;
    td_u32 still_thr;
    td_u32 big_grad_thr;
    td_u32 diff_thr;
    td_u32 smth_pix_num_thr;
    td_u32 still_pix_num_thr;
    td_u32 noise_pix_num_thr;
    td_u32 qp_inc1_bits_thr;
    td_u32 qp_inc2_bits_thr;
    td_u32 qp_dec1_bits_thr;
    td_u32 qp_dec2_bits_thr;
    td_u32 raw_bits_penalty;
    td_u32 buf_fullness_thr_reg0;
    td_u32 buf_fullness_thr_reg1;
    td_u32 buf_fullness_thr_reg2;
    td_u32 qp_rge_reg0;
    td_u32 qp_rge_reg1;
    td_u32 qp_rge_reg2;
    td_u32 bits_offset_reg0;
    td_u32 bits_offset_reg1;
    td_u32 bits_offset_reg2;
    td_u32 est_err_gain_map;
    td_u32 buffer_init_bits;
    td_u32 first_mb_adj_bits;
    td_u32 first_row_adj_bits;
    td_u32 first_col_adj_bits;
    td_u32 smooth_status_thr;
    td_u32 budget_mb_bits;
    td_u32 budget_mb_bits_last;
    td_u32 min_mb_bits;
    td_u32 max_mb_qp;
    td_u32 noise_grad_thr;
    td_u32 nl2_num_thr;
    td_u32 nl1_num_thr;

    td_u32 qp_force_en;
    td_u32 g_flt_en;
    td_u32 rb_flt_en;
} cmp_bnr_output;

typedef struct {
    td_u32 frame_width;         /* input frame width */
    td_u32 frame_height;        /* input frame height */
    td_u32 bit_depth;           /* pixel bit depth; 0: 8 bit, 1: 10 bit */
    td_u32 chroma_en;           /* 0--luma, 1--chroma */
    td_u32 stride_en;
    td_u32 align_mode;          /* 0-- 1burst align, 1-- 2burst align */
    td_u32 cmp_mode;            /* 1: is raw frame compress mode, 0: is not ... */
    td_u32 is_lossless;         /* 1: is lossless compress, 0: is lossy compress */
    td_u32 pix_format;          /* 0: 420, 1: 422 */
    td_u32 cmp_ratio_int;       /* compress ratio, (=comp_ratio * 1000) */
    td_u32 rc_type_mode;        /* 0--default, 1--rand, 2--MAX, 3--MIN, 4--MIN/MAX rand */
} cmp_3dnr_input;

typedef struct {
    td_u32 frame_width;
    td_u32 frame_height;
    td_u32 bit_depth;
    td_u32 chroma_en;
    td_u32 stride_en;
    td_u32 stride;
    td_u32 align_mode;
    td_u32 cmp_mode;
    td_u32 is_lossless;
    td_u32 pix_format;
    td_u32 adp_mode;

    /* rc parameter */
    td_u32 smth_thr;
    td_u32 still_thr;
    td_u32 big_grad_thr;
    td_u32 diff_thr;
    td_u32 smth_pix_num_thr;
    td_u32 still_pix_num_thr;
    td_u32 noise_pix_num_thr;
    td_u32 grph_group_num_thr;
    td_u32 qp_inc1_bits_thr;
    td_u32 qp_inc2_bits_thr;
    td_u32 raw_bits_penalty;
    td_u32 grph_bits_penalty;
    td_u32 buf_fullness_thr_reg0;
    td_u32 buf_fullness_thr_reg1;
    td_u32 buf_fullness_thr_reg2;
    td_u32 qp_rge_reg0;
    td_u32 qp_rge_reg1;
    td_u32 qp_rge_reg2;
    td_u32 bits_offset_reg0;
    td_u32 bits_offset_reg1;
    td_u32 bits_offset_reg2;
    td_u32 grph_loss_thr;
    td_u32 est_err_gain_map;
    td_u32 buffer_init_bits;
    td_u32 first_mb_adj_bits;
    td_u32 first_row_adj_bits;
    td_u32 first_col_adj_bits;
    td_u32 smooth_status_thr;
    td_u32 budget_mb_bits;
    td_u32 budget_mb_bits_last;
    td_u32 min_mb_bits;
    td_u32 max_mb_qp;
} cmp_3dnr_output;

typedef struct {
    td_s32 cmp_mode;  /* 1: is raw frame compress mode; 0: is not ... */
    td_s32 is_lossless;  /* 1: is lossless compress; 0: is lossy compress */
    td_s32 dcmp_merge_mode;  /* mode of decompressing separated Y/C; 1: dcmp Y; 2: dcmp C */
    td_s32 sample_mode;  /* 1: 422; 2: 420 */

    td_s32 frame_width;  /* input frame width */
    td_s32 frame_height;  /* input frame height */
    td_s32 dw_mode;  /* pixel bit depth */
} dcmp_3dnr_output;

typedef struct {
    td_s32 frame_width;
    td_s32 frame_height;
    td_s32 bitdepth;
    td_s32 cmp_mode;  /* 0 --compress; 1-- bypass raw; default:0 */
    td_s32 comp_ratio_int_y;  /* 压缩比*1000 */
    td_s32 comp_ratio_int_c;  /* 压缩比*1000 */
    td_s32 frm_start;  /* default:0 */
    td_s32 frm_len;  /* default:0 */
} cmp_tile16x8_input;

typedef struct {
    td_u32 frame_width;
    td_u32 frame_height;
    td_u32 bitdepth;
    td_u32 cmp_mode;  /* 0 --compress; 1-- bypass raw */
    td_u32 segment_cfg_tar_bit_y;
    td_u32 fix_k_y;
    td_u32 segment_cfg_tar_bit_c;
    td_u32 fix_k_c;
} cmp_tile16x8_output;

typedef struct {
    td_s32 frame_width;  /* input frame width */
    td_s32 frame_height;  /* input frame height */
    td_s32 is_lossless;  /* 1: is lossless compress; 0: is lossy compress */
    td_s32 is_lossless_a;  /* 1: is lossless alpha compress; 0:is lossy alpha compress */
    td_s32 comp_ratio_int;  /* line compress ratio, (=comp_ratio * 1000) */
    td_s32 comp_ratio_cap_int;  /* frame compress ratio, (=comp_ratio_cap * 1000) */
    td_s32 cmp_mode;  /* 1: is raw frame compress mode; 0: is not ... */
    /* 0表示ARGB8888，1表示ARGB1555，2表示ARGB4444，3表示RGB565，4表示RGB888，5表示YUV444，6表示RGBW */
    td_s32 source_mode;
    /* 1: pixel prediction with top linebuffer; 0: pixel prediction without top linebuffer. default:1 */
    td_s32 top_pred_en;
    td_s32 part_cmp_en;  /* 0: compress total frame; 1: compress partial frame */
    td_s32 pcmp_start_hpos;  /* start horizontal position of partial cmp */
    td_s32 pcmp_end_hpos;  /* end horizontal position of partial cmp */

    td_s32 rm_dbg_en;  /* RM debug enable control, 0: unable; 1: enable */
    td_s32 rc_type_mode;  /* 0: default; 1: random; 2: max value; 3: min value */
} cmp_muv1_input;

typedef struct {
    td_s32 cmp_mode;  /* 1: is raw frame compress mode; 0: is not ... */
    td_s32 is_lossless;  /* 1: is lossless compress; 0:is lossy compress */
    td_s32 is_lossless_a;  /* 1: is lossless alpha compress; 0:is lossy alpha compress */
    td_s32 source_mode;  /* 0: argb8888, 1: argb1555, 2: argb4444, 3: rgb565, 4: rgb888, 5: yuv444(8 bit), 6: rgbw */
    td_s32 top_pred_en;  /* 1: pixel prediction with top linebuffer; 0: pixel prediction without top linebuffer */
    td_s32 part_cmp_en;  /* 0: compress total frame; 1: compress partial frame */

    td_s32 frame_width;  /* input frame width */
    td_s32 frame_height;  /* input frame height */
    td_s32 pcmp_start_hpos;  /* start horizontal position of partial cmp */
    td_s32 pcmp_end_hpos;  /* end horizontal position of partial cmp */

    td_s32 rm_dbg_en;  /* 1: rm debug enable; 0: rm debug unable */

    /* register parameter of ratio control */
    td_s32 mb_bits;
    td_s32 mb_bits_cap;
    td_s32 pix_diff_thr;
    td_s32 smth_thr;
    td_s32 still_thr;
    td_s32 big_grad_thr;
    td_s32 smth_pix_num_thr;
    td_s32 still_pix_num_thr;
    td_s32 noise_pix_num_thr;
    td_s32 adj_sad_bits_thr;
    td_s32 qp_inc1_bits_thr;
    td_s32 qp_inc2_bits_thr;
    td_s32 qp_dec1_bits_thr;
    td_s32 qp_dec2_bits_thr;
    td_s32 max_qp;
    td_s32 sad_bits_gain;
    td_s32 rc_smth_ngain;
    td_s32 max_trow_bits;
    td_s32 min_sad_thr;
    td_s32 max_sad_thr;
    td_s32 noise_sad;
    td_s32 min_mb_bits;
    td_s32 est_err_gain;
    td_s32 max_est_err_level;
    td_s32 graphic_en;
    td_s32 qp_thr0;
    td_s32 qp_thr1;
    td_s32 qp_thr2;
    td_s32 grph_bias_bit_thr0;
    td_s32 grph_bias_bit_thr1;
    td_s32 grph_ideal_bit_thr;
    td_s32 vbv_buf_loss1_thr;
    td_s32 vbv_buf_loss2_thr;
    td_s32 forcerc_bits_diff_thr;
    td_s32 force_rc_en;
    td_s32 maxdiff_ctrl_en;
    td_s32 init_buf_bits_cap;
    td_s32 lfw_mb_len;
    td_s32 cmplx_sad_thr;
    td_s32 err_thr0;
    td_s32 err_thr1;
    td_s32 sim_num_thr;
    td_s32 sum_y_err_thr;
    td_s32 sum_c_err_thr;
    td_s32 cpmlx_sad_thr_y;
    td_s32 smpl_sad_thr_c;
    td_s32 smpl_sumsad_thr_y;
    td_s32 smpl_sumsad_thr_c;
    td_s32 future_sad_y_thr0;
    td_s32 future_sad_c_thr0;
    td_s32 future_sad_y_thr1;
    td_s32 future_sad_c_thr1;
    td_s32 cmplx_sumsad_thr_y;
    td_s32 cmplx_sumsad_thr_c;
    td_s32 buffer_up_lmt_thr;
    td_s32 buffer_low_lmt_thr;
    td_s32 buffer_adj_qp_thr0;
    td_s32 buffer_adj_qp_thr1;
    td_s32 buffer_adj_qp_thr2;
    td_s32 buffer_surplus_thr0;
    td_s32 buffer_surplus_thr1;
    td_s32 buffer_surplus_thr2;
    td_s32 offset_bits_thr;
} cmp_muv1_output;

typedef struct {
    td_s32 cmp_mode;  /* 1: is raw frame compress mode; 0: is not ... */
    td_s32 is_lossless;  /* 1: is lossless compress 0: is lossy compress */
    td_s32 is_lossless_a;  /* 1: is lossless alpha compress; 0:is lossy alpha compress */
    td_s32 source_mode;  /* 0: argb8888, 1: argb1555, 2: argb4444, 3: rgb565, 4: rgb888, 5: yuv444(8 bit) */
    td_s32 top_pred_en;  /* 1: pixel prediction with top linebuffer; 0: pixel prediction without top linebuffer */

    td_s32 frame_width;  /* input frame width */
    td_s32 frame_height;  /* input frame height */
} dcmp_muv1_out;

td_void cmp_generate_cfg_seg256x1(const cmp_seg_256x1_input *input, cmp_seg_256x1_output *cfg);
td_void cmp_generate_cfg_bayer(const cmp_bayer_input *para_input_ptr, cmp_bayer_output *cmp_parm);
td_void cmp_generate_cfg_bnr(const cmp_bnr_input *para_input_ptr, cmp_bnr_output *cmp_parm);
td_void cmp_generate_cfg_3dnr(const cmp_3dnr_input *para_input_ptr, cmp_3dnr_output *cmp_parm);
td_void cmp_generate_cfg_tile16x8(const cmp_tile16x8_input *para_input, cmp_tile16x8_output *rm_cfg);

#endif /* OT_SYS_H */
