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
#ifndef SAMPLE_COMMON_SVP_H
#define SAMPLE_COMMON_SVP_H

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
#include "ss_mpi_sys.h"
#include "ot_common.h"
#include "ot_common_svp.h"
#include "sample_comm.h"
#ifndef CONFIG_USER_SPACE
#include "ss_mpi_dsp.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define OT_SVP_RECT_NUM                         64
#define OT_POINT_NUM                            4
#define OT_SVP_MAX_VPSS_CHN_NUM                 2
#define OT_SVP_TIMEOUT                          2000

typedef enum {
    SAMPLE_SVP_ERR_LEVEL_DEBUG   = 0x0,    /* debug-level                                  */
    SAMPLE_SVP_ERR_LEVEL_INFO    = 0x1,    /* informational                                */
    SAMPLE_SVP_ERR_LEVEL_NOTICE  = 0x2,    /* normal but significant condition             */
    SAMPLE_SVP_ERR_LEVEL_WARNING = 0x3,    /* warning conditions                           */
    SAMPLE_SVP_ERR_LEVEL_ERROR   = 0x4,    /* error conditions                             */
    SAMPLE_SVP_ERR_LEVEL_CRIT    = 0x5,    /* critical conditions                          */
    SAMPLE_SVP_ERR_LEVEL_ALERT   = 0x6,    /* action must be taken immediately             */
    SAMPLE_SVP_ERR_LEVEL_FATAL   = 0x7,    /* just for compatibility with previous version */

    SAMPLE_SVP_ERR_LEVEL_BUTT
} sample_svp_err_level;

typedef struct {
    ot_point point[OT_POINT_NUM];
} ot_sample_svp_rect;

typedef struct {
    td_u16 num;
    ot_sample_svp_rect rect[OT_SVP_RECT_NUM];
} ot_sample_svp_rect_info;

typedef struct {
    td_bool is_venc_open;
    td_bool is_vo_open;
} ot_sample_svp_switch;

typedef struct {
    ot_sample_svp_switch svp_switch;
    ot_size pic_size[OT_SVP_MAX_VPSS_CHN_NUM];
    ot_pic_size pic_type[OT_SVP_MAX_VPSS_CHN_NUM];
    ot_vb_pool vb_pool[OT_SVP_MAX_VPSS_CHN_NUM];
    td_u32 chn_num;
} ot_sample_svp_media_cfg;

#define sample_svp_printf(level_str, msg, ...) \
do { \
    fprintf(stderr, "[level]:%s,[func]:%s [line]:%d [info]:"msg, level_str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
} while (0)

#define sample_svp_printf_red(level_str, msg, ...) \
do { \
    fprintf(stderr, "\033[0;31m [level]:%s,[func]:%s [line]:%d [info]:"msg"\033[0;39m\n", level_str, __FUNCTION__, \
        __LINE__, ##__VA_ARGS__); \
} while (0)
/* system is unusable   */
#define sample_svp_trace_fatal(msg, ...)   sample_svp_printf_red("Fatal", msg, ##__VA_ARGS__)
/* action must be taken immediately */
#define sample_svp_trace_alert(msg, ...)   sample_svp_printf_red("Alert", msg, ##__VA_ARGS__)
/* critical conditions */
#define sample_svp_trace_critical(msg, ...)    sample_svp_printf_red("Critical", msg, ##__VA_ARGS__)
/* error conditions */
#define sample_svp_trace_err(msg, ...)     sample_svp_printf_red("Error", msg, ##__VA_ARGS__)
/* warning conditions */
#define sample_svp_trace_warning(msg, ...)    sample_svp_printf("Warning", msg, ##__VA_ARGS__)
/* normal but significant condition  */
#define sample_svp_trace_notic(msg, ...)  sample_svp_printf("Notice", msg, ##__VA_ARGS__)
/* informational */
#define sample_svp_trace_info(msg, ...)    sample_svp_printf("Info", msg, ##__VA_ARGS__)
/* debug-level messages  */
#define sample_svp_trace_debug(msg, ...)  sample_svp_printf("Debug", msg, ##__VA_ARGS__)

/* exps is true, goto */
#define sample_svp_check_exps_goto(exps, label, level, msg, ...)                  \
do {                                                                              \
    if ((exps)) {                                                                 \
        sample_svp_trace_err(msg, ## __VA_ARGS__);                                \
        goto label;                                                               \
    }                                                                             \
} while (0)
/* exps is true, return td_void */
#define sample_svp_check_exps_return_void(exps, level, msg, ...)                 \
do {                                                                             \
    if ((exps)) {                                                                \
        sample_svp_trace_err(msg, ##__VA_ARGS__);                                \
        return;                                                                  \
    }                                                                            \
} while (0)
/* exps is true, return ret */
#define sample_svp_check_exps_return(exps, ret, level, msg, ...)                 \
do {                                                                             \
    if ((exps)) {                                                                \
        sample_svp_trace_err(msg, ##__VA_ARGS__);                                \
        return (ret);                                                            \
    }                                                                            \
} while (0)                                                                      \
/* exps is true, trace */
#define sample_svp_check_exps_trace(exps, level, msg, ...)                       \
do {                                                                             \
    if ((exps)) {                                                                \
        sample_svp_trace_err(msg, ##__VA_ARGS__);                                \
    }                                                                            \
} while (0)

#define sample_svp_check_exps_continue(exps, level, msg, ...)                    \
do {                                                                             \
    if ((exps)) {                                                                \
        sample_svp_trace_err(msg, ##__VA_ARGS__);                                \
        continue;                                                                \
    }                                                                            \
} while (0)                                                                      \

/* exps is not success, trace */
#define sample_svp_check_failed_trace(exps, level, msg, ...)                     \
do {                                                                             \
    if ((exps) != TD_SUCCESS) {                                                  \
        sample_svp_trace_err(msg, ##__VA_ARGS__);                                \
    }                                                                            \
} while (0)

/* exps is not success, goto */
#define sample_svp_check_failed_goto(exps, label, level, msg, ...)                \
do {                                                                              \
    if ((exps) != TD_SUCCESS) {                                                   \
        sample_svp_trace_err(msg, ## __VA_ARGS__);                                \
        goto label;                                                               \
    }                                                                             \
} while (0)                                                                       \

/* exps is not sucecss, return */
#define sample_svp_check_failed_return(exps, ret, level, msg, ...)               \
do {                                                                             \
    if ((exps) != TD_SUCCESS) {                                                  \
        sample_svp_trace_err(msg, ##__VA_ARGS__);                                \
        return (ret);                                                            \
    }                                                                            \
} while (0)

/* exps is not equal to success, goto with level SAMPLE_SVP_ERR_LEVEL_ERROR */
#define sample_svp_check_failed_err_level_goto(exps, label, msg, ...)             \
do {                                                                              \
    if ((exps) != TD_SUCCESS) {                                                   \
        sample_svp_trace_err(msg, ## __VA_ARGS__);                                \
        goto label;                                                               \
    }                                                                             \
} while (0)

/* exps is not equal to success, return with level SAMPLE_SVP_ERR_LEVEL_ERROR */
#define sample_svp_check_failed_err_level_return(exps, ret, msg, ...)             \
do {                                                                              \
    if ((exps) != TD_SUCCESS) {                                                   \
        sample_svp_trace_err(msg, ## __VA_ARGS__);                                \
        return (ret);                                                             \
    }                                                                             \
} while (0)

/* exps is not equal success, trace with level SAMPLE_SVP_ERR_LEVEL_ERROR */
#define sample_svp_check_failed_err_level_trace(exps, msg, ...)                  \
do {                                                                             \
    if ((exps) != TD_SUCCESS) {                                                  \
        sample_svp_trace_err(msg, ##__VA_ARGS__);                                \
    }                                                                            \
} while (0)

#define sample_svp_pause()                                                      \
do {                                                                            \
    printf("---------------press Enter key to exit!---------------\n");         \
    (void)getchar();                                                            \
} while (0)

#define SAMPLE_SVP_VB_POOL_NUM     2
#define SAMPLE_SVP_ALIGN_16        16
#define SAMPLE_SVP_ALIGN_32        32
#define SAMPLE_SVP_D1_PAL_HEIGHT   576
#define SAMPLE_SVP_D1_PAL_WIDTH    704
#define sample_svp_convert_addr_to_ptr(type, addr) ((type *)(td_uintptr_t)(addr))
#define sample_svp_convert_ptr_to_addr(type, addr) ((type)(td_uintptr_t)(addr))

/* free mmz */
#define sample_svp_mmz_free(phys, virt)                                                 \
do {                                                                                    \
    if (((phys) != 0) && ((virt) != 0)) {                                               \
        ss_mpi_sys_mmz_free((td_phys_addr_t)(phys), (td_void*)(td_uintptr_t)(virt));    \
        (phys) = 0;                                                                     \
        (virt) = 0;                                                                     \
    }                                                                                   \
} while (0)

#define sample_svp_close_file(fp)   \
do {                                \
    if ((fp) != TD_NULL) {          \
        fclose((fp));               \
        (fp) = TD_NULL;             \
    }                               \
} while (0)

/* System init */
td_s32 sample_common_svp_check_sys_init(td_bool vi_en);

/* System exit */
td_void sample_common_svp_check_sys_exit(td_void);

/* Align */
td_u32 sample_common_svp_align(td_u32 size, td_u16 align);

/* Create mem info */
td_s32 sample_common_svp_create_mem_info(ot_svp_mem_info *mem_info, td_u32 size, td_u32 addr_offsset);

/* Destory mem info */
td_void sample_common_svp_destroy_mem_info(ot_svp_mem_info *mem_info, td_u32 addr_offsset);

/* Malloc memory */
td_s32 sample_common_svp_malloc_mem(td_char *mmb, td_char *zone, td_phys_addr_t *phys_addr,
    td_void **virt_addr, td_u32 size);

/* Malloc memory with cached */
td_s32 sample_common_svp_malloc_cached(td_char *mmb, td_char *zone, td_phys_addr_t *phys_addr,
    td_void **virt_addr, td_u32 size);

/* Fulsh cached */
td_s32 sample_common_svp_flush_cache(td_phys_addr_t phys_addr, td_void *virt_addr, td_u32 size);

/* function : Call vgs to fill rect */
td_s32 sample_common_svp_vgs_fill_rect(const ot_video_frame_info *frame_info,
    ot_sample_svp_rect_info *rect, td_u32 color);

/* function : Start Vi/Vpss/Venc/Vo */
td_s32 sample_common_svp_start_vi_vpss_venc_vo(sample_vi_cfg *vi_config,
    ot_sample_svp_switch *switch_ptr, ot_pic_size *ext_pic_size_type);

/* function : Stop Vi/Vpss/Venc/Vo */
td_void sample_common_svp_stop_vi_vpss_venc_vo(sample_vi_cfg *vi_config,
    ot_sample_svp_switch *switch_ptr);

#ifndef CONFIG_USER_SPACE
/*
 * Load bin
 */
td_s32 sample_comm_svp_load_core_binary(ot_svp_dsp_id core_id);

/*
 * UnLoad bin
 */
void sample_comm_svp_unload_core_binary(ot_svp_dsp_id core_id);
#endif

/*
 * Start Vdec/Vpss/Vo
 */
td_s32 sample_common_svp_create_vb_start_vdec_vpss_vo(sample_vdec_attr *vdec_attr,
    vdec_thread_param *vdec_send, pthread_t *vdec_thread, ot_sample_svp_media_cfg *media_cfg,
    sample_vo_cfg *vo_cfg);
/*
 * Stop Vdec/Vpss/Vo
 */
td_void sample_common_svp_destroy_vb_stop_vdec_vpss_vo(vdec_thread_param *vdec_send,
    pthread_t *vdec_thread, ot_sample_svp_media_cfg *media_cfg, sample_vo_cfg *vo_cfg);
/*
 * Send stream to venc/vo
 */
td_s32 sample_common_svp_venc_vo_send_stream(const ot_sample_svp_switch *vo_venc_switch,
    ot_venc_chn venc_chn, ot_vo_layer vo_layer, ot_vo_chn vo_chn, const ot_video_frame_info *frame);

td_s32 sample_common_svp_npu_start_vi_vpss_vo(sample_vi_cfg *vi_cfg, ot_sample_svp_media_cfg *media_cfg,
    sample_vo_cfg *vo_cfg);

td_void sample_common_svp_destroy_vb_stop_vi_vpss_vo(sample_vi_cfg *vi_cfg, ot_sample_svp_media_cfg *media_cfg,
    sample_vo_cfg *vo_cfg);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* SAMPLE_COMMON_SVP_H */
