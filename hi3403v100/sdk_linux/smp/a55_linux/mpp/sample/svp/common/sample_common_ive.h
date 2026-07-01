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

#ifndef SAMPLE_COMMON_IVE_H
#define SAMPLE_COMMON_IVE_H

#include "ot_common_svp.h"
#include "ot_common_ive.h"
#include "ss_mpi_ive.h"
#include "sample_comm.h"
#include "sample_common_svp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define OT_VIDEO_WIDTH                          352
#define OT_VIDEO_HEIGHT                         288
#define OT_IVE_ALIGN                            16
#define OT_IVE_CHAR_CALW                        8
#define OT_IVE_CHAR_CALH                        8
#define OT_IVE_CHAR_NUM                         (OT_IVE_CHAR_CALW * OT_IVE_CHAR_CALH)
#define OT_IVE_FILE_NAME_LEN                    256
#define OT_VPSS_CHN_NUM                         2

#define OT_MAX_LOOP_IMG_H                       3

#define OT_SAMPLE_IVE_IMAGE_CHN_ONE             1
#define OT_SAMPLE_IVE_IMAGE_CHN_TWO             2
#define OT_SAMPLE_IVE_IMAGE_CHN_THREE           3
#define OT_SAMPLE_IVE_IMAGE_CHN_FOUR            4
#define OT_SAMPLE_IVE_MAX_CCL_REGION_NUM        254
#define OT_SAMPLE_IVE_DIV_TWO                   2
#define OT_SAMPLE_POINT_IDX_ZERO                0
#define OT_SAMPLE_POINT_IDX_ONE                 1
#define OT_SAMPLE_POINT_IDX_TWO                 2
#define OT_SAMPLE_POINT_IDX_THREE               3

#define OT_SAMPLE_ADDR_IDX_ZERO                 0
#define OT_SAMPLE_ADDR_IDX_ONE                  1
#define OT_SAMPLE_ADDR_IDX_TWO                  2
#define OT_SAMPLE_IVE_MAX_POOL_CNT              128
#define OT_SAMPLE_IVE_DRAW_THICK                2

#define OT_SAMPLE_IVE_PIPE_IDX_ZERO             0
#define OT_SAMPLE_IVE_PIPE_IDX_ONE              1
#define OT_SAMPLE_IVE_PIPE_IDX_TWO              2
#define OT_SAMPLE_IVE_PIPE_IDX_THREE            3

#define OT_SAMPLE_IVE_1080P_WIDTH               1920
#define OT_SAMPLE_IVE_1080P_HEIGHT              1080

#define OT_SAMPLE_IVE_MASK_NUM                  25
#define OT_SAMPLE_IDX_ZERO                      0
#define OT_SAMPLE_IDX_ONE                       1
#define OT_SAMPLE_IDX_TWO                       2
#define OT_SAMPLE_IDX_THREE                     3
#define OT_SAMPLE_IDX_FOUR                      4
#define OT_SAMPLE_IDX_FIVE                      5

#define OT_SAMPLE_NUM_ZERO                      0
#define OT_SAMPLE_NUM_ONE                       1
#define OT_SAMPLE_NUM_TWO                       2
#define OT_SAMPLE_NUM_THREE                     3
#define OT_SAMPLE_NUM_FOUR                      4
#define OT_SAMPLE_NUM_FIVE                      5
#define OT_SAMPLE_NUM_SEVEN                     7
#define OT_SAMPLE_QUARTER_OF_1M                 256
#define OT_SAMPLE_1M                            1024
#define OT_SAMPLE_4M                            (4 * 1024)
#define OT_SAMPLE_MAX_SRC_FRAME_RATE            30
#define OT_SAMPLE_MAX_DST_FRAME_RATE            30

#define ot_sample_align_back(x, a) ((a) * (((x) / (a))))
#define ot_sample_ive_max(a, b) (((a) > (b)) ? (a) : (b))
#define ot_sample_ive_min(a, b) (((a) < (b)) ? (a) : (b))

typedef struct {
    td_s32 linear_num;
    td_s32 thresh_num;
    ot_point *linear_point;
} ot_sample_ive_linear_data;

typedef struct {
    td_u32 ele_size; /* element size */
    td_u32 loop_c;  /* loop times of c dimension */
    td_u32 loop_h[OT_SVP_IMG_ADDR_NUM]; /* loop times of h dimension */
} ot_sample_rw_image_loop_info;

typedef struct {
    ot_size src;
    ot_size dst;
} ot_sample_src_dst_size;

/*
 * function : Mpi check init
 */
td_s32 sample_common_ive_check_mpi_init(td_void);

/*
 * function : Mpi init
 */
td_s32 sample_common_ive_mpi_init(td_void);

/*
 * function : Mpi exit
 */
td_void sample_common_ive_mpi_exit(td_void);

/*
 * function :Read file
 */
td_s32 sample_common_ive_read_file(ot_svp_img *img, FILE *fp);

/*
 * function :Write file
 */
td_s32 sample_common_ive_write_file(ot_svp_img *img, FILE *fp);

/*
 * function :Calc stride
 */
td_u32 sample_common_ive_calc_stride(td_u32 width, td_u8 align);

/*
 * function : Copy blob to rect
 */
td_s32 sample_common_ive_blob_to_rect(ot_ive_ccblob *blob, ot_sample_svp_rect_info *rect,
    td_u16 rect_max_num, td_u16 area_thr_step, ot_sample_src_dst_size src_dst_size);

/*
 * function : Create ive image
 */
td_s32 sample_common_ive_create_image(ot_svp_img *img, ot_svp_img_type type,
    td_u32 width, td_u32 height);

/*
 * function : Create memory info
 */
td_s32 sample_common_ive_create_mem_info(ot_svp_mem_info *mem_info, td_u32 size);

/*
 * function : Create ive image by cached
 */
td_s32 sample_common_ive_create_image_by_cached(ot_svp_img *img,
    ot_svp_img_type type, td_u32 width, td_u32 height);

/*
 * function : Dma frame info to  ive image
 */
td_s32 sample_common_ive_dma_image(ot_video_frame_info *frame_info, ot_svp_dst_img *dst,
    td_bool is_instant);
#endif
