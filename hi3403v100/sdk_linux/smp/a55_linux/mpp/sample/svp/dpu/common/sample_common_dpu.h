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
#ifndef SAMPLE_COMMON_DPU_H
#define SAMPLE_COMMON_DPU_H
#include <stdio.h>
#include "ss_mpi_sys.h"

#include "ot_common_vb.h"
#include "ss_mpi_dpu_rect.h"
#include "ss_mpi_dpu_match.h"
#include "ot_common_dpu_rect.h"
#include "ot_common_dpu_match.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define SAMPLE_SVP_DPU_ALIGN_16              16
#define SAMPLE_SVP_DPU_FRAME_NUM             1
#define SAMPLE_SVP_DPU_DISPARITY_NUM         80

#define SAMPLE_SVP_DPU_IN_WIDTH              1280
#define SAMPLE_SVP_DPU_IN_HEIGHT             720
#define SAMPLE_SVP_DPU_DST_WIDTH             1050
#define SAMPLE_SVP_DPU_DST_HEIGHT            560
#define SAMPLE_SVP_DPU_CHN_NUM               2
#define SAMPLE_SVP_DPU_TIME_OUT              2000
#define SAMPLE_SVP_DPU_BLOCK                 (-1)
#define SAMPLE_SVP_DPU_NO_RATE_CTRL          (-1)
#define SAMPLE_SVP_DPU_MAX_POOL_CNT          128
#define SAMPLE_SVP_DPU_BLK_CNT_10            10
#define SAMPLE_SVP_DPU_BLK_CNT_8             8
#define SAMPLE_SVP_DPU_VI_CHN_CNT            2
#define SAMPLE_SVP_DPU_VI_PIPE_CNT           2
#define SAMPLE_SVP_DPU_VPSS_GRP_NUM          2
#define SAMPLE_SVP_DPU_PER_CHN_BLK_CNT       4
#define SAMPLE_SVP_DPU_VPSS_MAX_WIDTH        3840
#define SAMPLE_SVP_DPU_VPSS_MAX_HEIGHT       2160
#define SAMPLE_SVP_DPU_BG_COLOR              0xff
#define SAMPLE_SVP_DPU_COMMON_POOL_2         2
#define SAMPLE_SVP_DPU_COMMON_POOL_3         3

#define sample_svp_dpu_convert_addr_to_ptr(type, addr) ((type *)(td_uintptr_t)(addr))

#define sample_svp_dpu_mmz_free(phys, virt) \
do { \
    if (((phys) != 0) && ((virt) != 0)) { \
        ss_mpi_sys_mmz_free((td_phys_addr_t)(phys), (td_void*)(td_uintptr_t)(virt)); \
        (phys) = 0; \
        (virt) = 0; \
    } \
} while (0)

typedef struct {
    ot_dpu_rect_grp dpu_rect_grp;
    ot_dpu_rect_grp_attr dpu_rect_grp_attr;
    ot_dpu_rect_lut_id lut_id[OT_DPU_RECT_MAX_PIPE_NUM];
    ot_dpu_rect_mem_info dpu_rect_mem_info[OT_DPU_RECT_MAX_PIPE_NUM];
    ot_vb_pool pipe_vb_pool[OT_DPU_RECT_MAX_PIPE_NUM];
    FILE *pipe_file[OT_DPU_RECT_MAX_PIPE_NUM];
    ot_vb_blk vb_blk[OT_DPU_RECT_MAX_PIPE_NUM];

    ot_dpu_match_grp dpu_match_grp;
    ot_dpu_match_grp_attr dpu_match_grp_attr;
    ot_dpu_match_frame_info dpu_match_src_frame;
    ot_video_frame_info dpu_match_dst_frame;
    FILE *match_file;
} ot_sample_svp_dpu_cfg;

#define sample_svp_dpu_pause()                                                      \
do {                                                                    \
    printf("---------------press Enter key to exit!---------------\n"); \
    (void)getchar();                                                    \
} while (0)

#define sample_svp_dpu_printf(level_str, msg, ...) \
do { \
    fprintf(stderr, "[level]:%s,[func]:%s [line]:%d [info]:"msg, \
        level_str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
} while (0)
#define sample_svp_dpu_printf_red(level_str, msg, ...) \
do { \
    fprintf(stderr, "\033[0;31m [level]:%s,[func]:%s [line]:%d [info]:"msg"\033[0;39m\n", \
        level_str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
} while (0)

/* error conditions */
#define sample_svp_dpu_trace_err(msg, ...)     sample_svp_dpu_printf_red("Error", msg, ##__VA_ARGS__)
#define sample_svp_dpu_trace_info(msg, ...)    sample_svp_dpu_printf("Info", msg, ##__VA_ARGS__)
/* exps is true, goto */
#define sample_svp_dpu_check_exps_goto(exps, label, msg, ...)                  \
do {                                                                              \
    if ((exps)) {                                                                 \
        sample_svp_dpu_trace_err(msg, ## __VA_ARGS__);                                \
        goto label;                                                               \
    }                                                                             \
} while (0)

/* exps is true, return ret */
#define sample_svp_dpu_check_exps_return(exps, ret, msg, ...)                 \
do {                                                                             \
    if ((exps)) {                                                                \
        sample_svp_dpu_trace_err(msg, ##__VA_ARGS__);                                \
        return (ret);                                                            \
    }                                                                            \
} while (0)                                                                      \

td_u32 sample_common_svp_dpu_calc_stride(td_u32 width, td_u8 align);

/* function : get frame from file */
td_s32 sample_common_svp_dpu_rect_get_frame_from_file(FILE *fp, const ot_size *pic_size,
    td_u32 stride, ot_video_frame_info *frame_info, ot_vb_blk vb_blk);

td_s32 sample_common_svp_dpu_rect_load_lut(const td_char *file_name,
    ot_dpu_rect_mem_info *mem_info, ot_dpu_rect_lut_id *lut_id);

td_void sample_common_svp_dpu_rect_unload_lut(ot_dpu_rect_mem_info *mem_info,
    ot_dpu_rect_lut_id lut_id);

/* function : start dpu rect grp. */
td_s32 sample_common_svp_dpu_rect_start(ot_dpu_rect_grp dpu_rect_grp,
    ot_dpu_rect_grp_attr *grp_attr, ot_dpu_rect_chn_attr *chn_attr, td_u32 chn_num);

/* function : stop dpu rect grp */
td_s32 sample_common_svp_dpu_rect_stop(ot_dpu_rect_grp dpu_rect_grp, ot_dpu_rect_mode rect_mode);

/* function : write frame to file. */
td_s32 sample_common_svp_dpu_write_frame_to_file(FILE *fp, td_u32 ele_size,
    ot_video_frame_info *frame_info);

/* function : start dpu match grp. */
td_s32 sample_common_svp_dpu_match_start(ot_dpu_match_grp dpu_match_grp,
    ot_dpu_match_grp_attr *grp_attr, ot_dpu_match_chn_attr *chn_attr);
/* function : stop dpu match grp */
td_s32 sample_common_svp_dpu_match_stop(ot_dpu_match_grp dpu_match_grp);

td_s32 sample_common_svp_dpu_match_create_mem_info(ot_dpu_match_mem_info *mem_info,
    td_char *mmb, td_char *zone, td_u32 size);

td_s32 sample_common_svp_dpu_rect_bind_match(ot_dpu_rect_grp dpu_rect_grp,
    ot_dpu_match_grp dpu_match_grp);

td_s32 sample_common_svp_dpu_rect_unbind_match(ot_dpu_rect_grp dpu_rect_grp,
    ot_dpu_match_grp dpu_match_grp);

td_s32 sample_common_svp_dpu_vpss_bind_rect(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn,
    ot_dpu_rect_grp dpu_rect_grp, ot_dpu_rect_pipe dpu_rect_pipe);

td_s32 sample_common_svp_dpu_vpss_unbind_rect(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn,
    ot_dpu_rect_grp dpu_rect_grp, ot_dpu_rect_pipe dpu_rect_pipe);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* SAMPLE_COMMON_DPU_H */