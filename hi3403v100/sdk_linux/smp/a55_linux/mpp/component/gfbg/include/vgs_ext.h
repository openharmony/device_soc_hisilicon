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

#ifndef VGS_EXT_H
#define VGS_EXT_H

#include "ot_errno.h"
#include "ot_common_video.h"
#include "ot_common_vgs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VGS_INVALD_HANDLE (-1UL) /* invalid job handle */

#define VGS_MAX_JOB_NUM 400 /* max job number */
#define VGS_MIN_JOB_NUM 20  /* min job number */

#define VGS_MAX_TASK_NUM 800 /* max task number */
#define VGS_MIN_TASK_NUM 20  /* min task number */

#define VGS_MAX_NODE_NUM 800 /* max node number */
#define VGS_MIN_NODE_NUM 20  /* min node number */

#define VGS_PRIVATE_DATA_LEN 6
#define VGS_MAX_LINE_NUM    4
#define MAX_VGS_COVER       OT_VGS_MAX_COVER_NUM
#define MAX_VGS_OSD         OT_VGS_MAX_OSD_NUM
#define MAX_VGS_MOSAIC      OT_VGS_MAX_MOSAIC_NUM
#define VGS_MAX_CORNER_RECT_NUM OT_VGS_MAX_CORNER_RECT_NUM

#define FD_GRID_SZ  4 /* grid size used in feature detection, in this version, the image is processed with 4x4 grid */
#define FD_CELL_NUM (FD_GRID_SZ * FD_GRID_SZ) /* blk number in total */

#define VGS_MAX_STITCH_NUM  OT_VGS_STITCH_NUM
#define VGS_MAX_MCF_PIPE_NUM 6

/* stitch_num and mcf_num */
#define VGS_MAX_INPUT_PIPE_NUM VGS_MAX_MCF_PIPE_NUM
#ifdef CONFIG_OT_VGS_MULTI_CHN_SUPPORT
#define VGS_MAX_CHN_NUM OT_VGS_MAX_CHN_NUM
#else
#define VGS_MAX_CHN_NUM 1
#endif

#define VGS_NOTIFY_BYPASS_VB_MASK 0x1

typedef enum {
    VGS_COLOR_REVERT_NONE = 0, /* Not revert */
    VGS_COLOR_REVERT_RGB,      /* Revert RGB */
    VGS_COLOR_REVERT_ALPHA,    /* Revert alpha */
    VGS_COLOR_REVERT_BOTH,     /* Revert RGB and alpha */
    VGS_COLOR_REVERT_BUTT
} vgs_color_revert_mode;

typedef struct {
    ot_rect src_rect;                           /* OSD color revert area */
    vgs_color_revert_mode color_revert_mode; /* OSD color revert mode */
} vgs_osd_revert;

/* The type of job */
typedef enum {
    VGS_JOB_TYPE_BYPASS = 0, /* BYPASS job,can only contain bypass task */
    VGS_JOB_TYPE_NORMAL = 1, /* normal job,can contain any task except bypass task and lumastat task */
    VGS_JOB_TYPE_BUTT
} vgs_job_type;

/* The completion status of task */
typedef enum {
    VGS_TASK_FNSH_STAT_OK = 1,       /* task has been completed correctly */
    VGS_TASK_FNSH_STAT_FAIL = 2,     /* task failed because of exception or not completed */
    VGS_TASK_FNSH_STAT_CANCELED = 3, /* task has been cancelled */
#ifdef CONFIG_OT_VGS_LOW_DELAY_SUPPORT
    VGS_TASK_FNSH_STAT_LOWDELAY = 4, /* low delay status */
#endif
    VGS_TASK_FNSH_STAT_BUTT
} vgs_task_fnsh_stat;

/* the priority of VGS task */
typedef enum {
    VGS_JOB_PRI_HIGH = 0,   /* high priority */
    VGS_JOB_PRI_NORMAL = 1, /* normal priority */
    VGS_JOB_PRI_LOW = 2,    /* low priority */
    VGS_JOB_PRI_BUTT
} vgs_job_pri;

/* The status after VGS cancel job */
typedef struct {
    td_u32 jobs_canceled; /* The count of cancelled job */
    td_u32 jobs_left;     /* The count of the rest job */
} vgs_cancel_stat;

/* The completion status of job */
typedef enum {
    VGS_JOB_FNSH_STAT_OK = 1,       /* job has been completed correctly */
    VGS_JOB_FNSH_STAT_FAIL = 2,     /* job failed because of exception or not completed */
    VGS_JOB_FNSH_STAT_CANCELED = 3, /* job has been cancelled */
    VGS_JOB_FNSH_STAT_BUTT
} vgs_job_fnsh_stat;

/*
 * The struct of vgs job.
 * After complete the job,VGS call the callback function to notify the caller with this struct as an parameter.
 */
typedef struct ot_vgs_job_data {
    td_u64 private_data[VGS_PRIVATE_DATA_LEN]; /* private data of job */
    vgs_job_fnsh_stat job_finish_stat;         /* output param:job finish status(ok or nok) */
    vgs_job_type job_type;
    void (*job_call_back)(ot_mod_id call_mod_id, td_s32 call_dev_id, td_s32 call_chn_id,
                          struct ot_vgs_job_data *job_data); /* callback */
} vgs_job_data;

/*
 * The struct of vgs task ,include input and output frame buffer,caller,and callback function .
 * After complete the task,VGS call the callback function to notify the caller with this struct as an parameter.
 */
typedef struct vgs_inner_task_data {
    ot_video_frame_info img_in;  /* input picture */
    union {
        ot_video_frame_info img_out;                    /* output picture */
#ifdef CONFIG_OT_VGS_MULTI_CHN_SUPPORT
        ot_video_frame_info multi_img_out[VGS_MAX_CHN_NUM];  /* output picture for vpss */
#endif
    };
    td_u64 private_data[VGS_PRIVATE_DATA_LEN]; /* task's private data */
    void (*call_back)(ot_mod_id call_mod_id, td_s32 call_dev_id, td_s32 call_chn_id,
                      const struct vgs_inner_task_data *task); /* callback */
    ot_mod_id call_mod_id;          /* module ID */
    td_s32 call_dev_id;             /* device ID */
    td_s32 call_chn_id;             /* channel ID */
    union {
        vgs_task_fnsh_stat finish_stat; /* output param:task finish status(ok or nok) */
#ifdef CONFIG_OT_VGS_MULTI_CHN_SUPPORT
        vgs_task_fnsh_stat multi_finish_stat[VGS_MAX_CHN_NUM]; /* task finish status for vpss */
#endif
    };
    td_u32 notify_flag; /* notify VGS to deal with special operation */
} vgs_task_data;

typedef struct vgs_inner_task_data_stitch {
    ot_video_frame_info img_in[VGS_MAX_STITCH_NUM]; /* input picture */
    ot_video_frame_info img_out;                    /* output picture */
    td_u64 private_data[VGS_PRIVATE_DATA_LEN];      /* task's private data */
    void (*call_back)(ot_mod_id call_mod_id, td_s32 call_dev_id, td_s32 call_chn_id,
                      const struct vgs_inner_task_data_stitch *task); /* callback */
    ot_mod_id call_mod_id;                            /* module ID */
    td_s32 call_dev_id;                               /* device ID */
    td_s32 call_chn_id;                               /* channel ID */
    vgs_task_fnsh_stat finish_stat; /* output param:task finish status(ok or nok) */
    td_u32 reserved;
} vgs_task_data_stitch;

#ifdef CONFIG_OT_VGS_MCF_SUPPORT
typedef struct vgs_inner_task_data_mcf {
    ot_video_frame_info img_in[VGS_MAX_MCF_PIPE_NUM];  /* input picture */
    ot_video_frame_info img_out[VGS_MAX_CHN_NUM]; /* output picture */
    td_u64 private_data[VGS_PRIVATE_DATA_LEN]; /* task's private data */
    void (*call_back)(ot_mod_id call_mod_id, td_s32 call_dev_id, td_s32 call_chn_id,
                      const struct vgs_inner_task_data_mcf *task); /* callback */
    ot_mod_id call_mod_id;          /* module ID */
    td_s32 call_dev_id;             /* device ID */
    td_s32 call_chn_id;             /* channel ID */
    vgs_task_fnsh_stat finish_stat[VGS_MAX_CHN_NUM]; /* output param:task finish status(ok or nok) */
    td_u32 reserved;
} vgs_task_data_mcf;
#endif

/* The information of OSD */
typedef struct {
    /* first address of bitmap */
    td_phys_addr_t phys_addr;

    ot_pixel_format pixel_format;

    td_u32 stride;

    /* Alpha bit should be extended by setting alpha0 and Alpha1, when pixel_format is PIXEL_FORMAT_RGB_1555 */
    td_bool alpha_ext1555; /* whether alpha bit should be extended */
    td_u8 alpha0;          /* alpha0 will be valid where alpha bit is 0 */
    td_u8 alpha1;          /* alpha1 will be valid where alpha bit is 1 */
} vgs_osd;

/* The struct of OSD operation */
typedef struct {
    td_bool osd_en;

    td_u8 global_alpha;
    vgs_osd osd;
    ot_rect osd_rect;

    td_bool revert_en;
    vgs_osd_revert osd_revert;

    td_u32 clut[OT_VGS_CLUT_LEN];
} vgs_osd_opt;

typedef struct {
    ot_rect rect; /* the rgns to get luma info */
    td_u64 *virt_addr_for_result;
    td_phys_addr_t phys_addr_for_result;
    td_u64 *virt_addr_for_user;
} vgs_lumastat_opt;

typedef struct {
    td_bool cover_en;
    ot_cover_type cover_type;
    union {
        ot_rect dst_rect;                    /* rectangle */
        ot_quad_cover quad; /* arbitrary quadrilateral */
    };
    td_u32 cover_color;
} vgs_cover_opt;

typedef struct {
    td_bool enable;
    ot_mosaic_blk_size blk_size;
    ot_rect rect;
} vgs_mosaic_opt;

typedef struct {
    td_u32 num;
    ot_vgs_line line[VGS_MAX_LINE_NUM];
} vgs_line_opt;

typedef struct {
    ot_rect dest_rect;
} vgs_crop_opt;

typedef struct {
    td_s32 x;
    td_s32 y;
    td_u32 width;
    td_u32 height;
    td_u32 hor_len;
    td_u32 ver_len;
    td_u32 thick;
} vgs_corner_rect;

typedef struct {
    td_u32             num;
    vgs_corner_rect     corner_rect[VGS_MAX_CORNER_RECT_NUM];
    ot_corner_rect_attr comm_attr;
} vgs_online_corner_rect_opt;

typedef struct {
    td_bool             enable;
    vgs_corner_rect      corner_rect;
    ot_corner_rect_attr comm_attr;
} vgs_corner_rect_opt;

typedef enum {
    VGS_SCALE_COEF_NORMAL = 0,  /* normal scale coefficient */
    VGS_SCALE_COEF_TAP2 = 1,    /* scale coefficient of 2 tap for IVE */
    VGS_SCALE_COEF_TAP4 = 2,    /* scale coefficient of 4 tap for IVE */
    VGS_SCALE_COEF_TAP6 = 3,    /* scale coefficient of 6 tap for IVE */
    VGS_SCALE_COEF_TAP8 = 4,    /* scale coefficient of 8 tap for IVE */
    VGS_SCALE_COEF_PYRAMID = 5, /* scale coefficient for DIS pyramid */
    VGS_SCALE_COEF_MCF = 6, /* scale coefficient for mcf */
    VGS_SCALE_COEF_BUTT
} vgs_scale_coef_mode;

#ifdef CONFIG_OT_VGS_FPD_SUPPORT
typedef struct {
    td_u32 proc_bit_depth;  /* 4bits; u; 4.0 */
    td_bool do_detection;    /* 1bit; bool */

    td_u16 grid_rows;  /* 3bits; u; 3.0 */
    td_u16 grid_cols;  /* 3bits; u; 3.0 */
    td_u16 grid_nums;  /* 5bits; u,; 5.0 */

    td_u8 quality_level;   /* 3bits; u; 3.0 */
    td_u16 min_eigen_val;  /* 16bits; u; 16.0 */
    td_u8 harris_k;        /* 4bits; u; 4.0 */
    td_u16 min_distance;  /* 5bits; u; 5.0 */

    td_u16 cell_row[FD_GRID_SZ + 1];  /* 9bits; u; 9.0 corner position of the cells */
    td_u16 cell_col[FD_GRID_SZ + 1];  /* 9bits; u; 9.0 */

    td_u16 pts_num_per_cell[FD_CELL_NUM];  /* 6bits; u; 6.0 maximum number of keypoints allowed in each block */
    td_u16 max_pts_num_in_use;               /* 9bits; u; 9.0 */
    td_bool set_pts_num_per_cell;              /* 1bit; bool */
} vgs_fpd_hw_cfg;
#endif

typedef struct {
    ot_rect video_rect;
    td_u32 bg_color;
} vgs_aspectratio_opt;

typedef struct {
    /* if enable crop */
    td_bool crop;
    vgs_crop_opt crop_opt;
    /* if enable cover */
    td_bool cover;
    vgs_cover_opt cover_opt[MAX_VGS_COVER];
    /* if enable osd */
    td_bool osd;
    vgs_osd_opt osd_opt[MAX_VGS_OSD];
#ifdef CONFIG_OT_VGS_MOSAIC_ONLINE_SUPPORT
    /* if enable mosaic */
    td_bool mosaic;
    vgs_mosaic_opt mosaic_opt[MAX_VGS_MOSAIC];
#endif

    /* if enable mirror */
    td_bool mirror;
    /* if enable flip */
    td_bool flip;
    /*
     * whether to force the horizontal direction filtering, it can be
     * set while input and output pic are same size at horizontal direction
     */
    td_bool force_h_filt;
    /*
     * whether to force the vertical direction filtering, it can be
     * set while input and output pic are same size at vertical direction
     */
    td_bool force_v_filt;
    /* whether decrease flicker */
    td_bool deflicker;
    vgs_scale_coef_mode vgs_scl_coef_mode;

    /* the operation is belong to fisheye */
    td_bool gdc;
    ot_rect gdc_rect;

#ifdef CONFIG_OT_VGS_FPD_SUPPORT
    /* if enable fpd */
    td_bool fpd;
    /* physical address of fpd */
    td_phys_addr_t fpd_phys_addr;
    vgs_fpd_hw_cfg fpd_opt;
#endif

#ifdef CONFIG_OT_VGS_CORNER_RECT_SUPPORT
    td_bool             corner;
    vgs_online_corner_rect_opt corner_opt;
#endif
#ifdef CONFIG_OT_VGS_LOW_DELAY_SUPPORT
    td_bool chn_low_delay_en;
    td_u32  chn_low_delay_line_cnt;
#endif
    /* if enable LBA */
    td_bool aspect_ratio;
    vgs_aspectratio_opt aspect_ratio_opt;
    td_bool border_en;
    ot_border border_opt;
} vgs_online_opt;

#ifdef CONFIG_OT_VGS_FPD_SUPPORT
typedef struct {
    vgs_scale_coef_mode vgs_scl_coef_mode;

    /* if enable fpd */
    td_bool fpd;
    /* physical address of fpd */
    td_phys_addr_t fpd_phys_addr;
    vgs_fpd_hw_cfg fpd_opt;
} vgs_fpd_opt;
#endif

typedef struct {
    td_u32 stitch_num;
} vgs_stitch_opt;

#ifdef CONFIG_OT_VGS_MULTI_CHN_SUPPORT
typedef struct {
    td_bool chn_enable[VGS_MAX_CHN_NUM];
    ot_crop_info crop_info[VGS_MAX_CHN_NUM];
    ot_size zme_out_size[VGS_MAX_CHN_NUM]; /* valid when enable chn crop */
    ot_low_delay_info low_delay_info[VGS_MAX_CHN_NUM];
    td_bool mirror[VGS_MAX_CHN_NUM];
    td_bool flip[VGS_MAX_CHN_NUM];
    vgs_online_opt online_opt;  /* multi chn online opt */
    td_void *mcf_data;

    /* gen mcf stt */
    td_bool mcf_gen_stt_en;
    td_bool mcf_is_big_image;
    td_phys_addr_t mcf_stt_phys_addr;
} vgs_multi_chn_opt;
#endif

typedef struct {
    ot_rect src_rect;
    ot_rect dest_rect;
} vgs_osd_quickcopy_opt;

#ifdef CONFIG_OT_VGS_GME_SUPPORT
typedef struct {
    td_void *reg_data;
} vgs_gme_opt;
#endif

typedef td_s32 fn_vgs_begin_job(ot_vgs_handle *handle, vgs_job_pri pri_level,
    const ot_mpp_chn *mpp_chn, const vgs_job_data *job_data);

typedef td_s32 fn_vgs_end_job(ot_vgs_handle handle, td_bool sort, const vgs_job_data *job_data);

typedef td_s32 fn_vgs_end_job_block(ot_vgs_handle handle);

typedef td_s32 fn_vgs_cancel_job(ot_vgs_handle handle);

typedef td_s32 fn_vgs_cancel_job_by_mod_dev(ot_mod_id call_mod_id, td_s32 call_dev_id, td_s32 call_chn_id,
                                            vgs_cancel_stat *cancel_stat);

typedef td_s32 fn_vgs_add_cover_task(ot_vgs_handle handle, const vgs_task_data *task, const vgs_cover_opt *cover_opt);

typedef td_s32 fn_vgs_add_mosaic_task(ot_vgs_handle handle, const vgs_task_data *task,
    const vgs_mosaic_opt *mosaic_opt);

typedef td_s32 fn_vgs_add_line_task(ot_vgs_handle handle, const vgs_task_data *task, const vgs_line_opt *line_opt);

typedef td_s32 fn_vgs_add_osd_task(ot_vgs_handle handle, const vgs_task_data *task, const vgs_osd_opt *osd_opt);

typedef td_s32 fn_vgs_add_online_task(ot_vgs_handle handle, const vgs_task_data *task,
    const vgs_online_opt *online_opt);

typedef td_s32 fn_vgs_add_2scale_task(ot_vgs_handle handle, vgs_task_data *task);

typedef td_s32 fn_vgs_add_get_luma_stat_task(ot_vgs_handle handle, const vgs_task_data *task,
    const vgs_lumastat_opt *luma_info_opt);

typedef td_s32 fn_vgs_add_quick_copy_task(ot_vgs_handle handle, const vgs_task_data *task);

typedef td_s32 fn_vgs_add_rotation_task(ot_vgs_handle handle, vgs_task_data *task, ot_rotation angle);

typedef td_s32 fn_vgs_add_bypass_task(ot_vgs_handle handle, const vgs_task_data *task);

#ifdef CONFIG_OT_VGS_GME_SUPPORT
typedef td_s32 fn_vgs_add_gme_task(ot_vgs_handle handle, const vgs_task_data *task, const vgs_gme_opt *gme_opt);
#endif

/* only for test */
typedef td_void fn_vgs_get_max_job_num(td_u32 *max_job_num);
typedef td_void fn_vgs_get_max_task_num(td_u32 *max_task_num);

typedef vgs_task_data *fn_vgs_get_free_task(td_void);
typedef td_void fn_vgs_put_free_task(const vgs_task_data *task_data);

#ifdef CONFIG_OT_VGS_STITCH_SUPPORT
typedef td_s32 fn_vgs_add_stitch_task(ot_vgs_handle handle, const vgs_task_data_stitch *task,
    const vgs_stitch_opt *stitch_opt);
typedef vgs_task_data_stitch *fn_vgs_get_free_stitch_task(td_void);
typedef td_void fn_vgs_put_free_stitch_task(const vgs_task_data_stitch *task_data_stitch);
#endif

#ifdef CONFIG_OT_VGS_CORNER_RECT_SUPPORT
typedef td_s32 fn_vgs_add_corner_rect_task(ot_vgs_handle handle, const vgs_task_data *task,
    const vgs_corner_rect_opt *corner_rect_opt);
#endif

#ifdef CONFIG_OT_VGS_MCF_SUPPORT
typedef td_s32 fn_vgs_add_mcf_task(ot_vgs_handle handle, const vgs_task_data_mcf *task,
    const vgs_multi_chn_opt *multi_chn_opt);
typedef vgs_task_data_mcf *fn_vgs_get_free_mcf_task(td_void);
typedef td_void fn_vgs_put_free_mcf_task(const vgs_task_data_mcf *task_data_mcf);
#endif

#ifdef CONFIG_OT_VGS_MULTI_CHN_SUPPORT
typedef td_s32 fn_vgs_add_multi_chn_task(ot_vgs_handle handle, const vgs_task_data *task,
    const vgs_multi_chn_opt *multi_chn_opt);
#endif

#ifdef CONFIG_OT_VGS_DISPATCH_VPSS_SUPPORT
typedef td_void fn_vgs_proc_vpss_callback(td_u32 vpss_id, td_u32 vgs_id, td_u32 job_id, vgs_task_fnsh_stat stat);
#endif

typedef struct {
    fn_vgs_begin_job *pfn_vgs_begin_job;
    fn_vgs_cancel_job *pfn_vgs_cancel_job;
    fn_vgs_cancel_job_by_mod_dev *pfn_vgs_cancel_job_by_mod_dev;
    fn_vgs_end_job *pfn_vgs_end_job;
    fn_vgs_add_cover_task *pfn_vgs_add_cover_task;
    fn_vgs_add_mosaic_task *pfn_vgs_add_mosaic_task;
    fn_vgs_add_line_task *pfn_vgs_add_line_task;
    fn_vgs_add_osd_task *pfn_vgs_add_osd_task;
    fn_vgs_add_bypass_task *pfn_vgs_add_bypass_task;
    fn_vgs_add_get_luma_stat_task *pfn_vgs_get_luma_stat_task;
    fn_vgs_add_online_task *pfn_vgs_add_online_task;
    /* for jpeg */
    fn_vgs_add_2scale_task *pfn_vgs_add_2scale_task;
    /* for rgn */
    fn_vgs_add_quick_copy_task *pfn_vgs_add_quick_copy_task;

    fn_vgs_add_rotation_task *pfn_vgs_add_rotation_task;
#ifdef CONFIG_OT_VGS_GME_SUPPORT
    fn_vgs_add_gme_task *pfn_vgs_add_gme_task;
#endif
    /* for ive */
    fn_vgs_end_job_block *pfn_vgs_end_job_block;

    /* only for test */
    fn_vgs_get_max_job_num *pfn_vgs_get_max_job_num;
    fn_vgs_get_max_task_num *pfn_vgs_get_max_task_num;

    fn_vgs_get_free_task *pfn_vgs_get_free_task;
    fn_vgs_put_free_task *pfn_vgs_put_free_task;

#ifdef CONFIG_OT_VGS_STITCH_SUPPORT
    fn_vgs_add_stitch_task *pfn_vgs_add_stitch_task;
    fn_vgs_get_free_stitch_task *pfn_vgs_get_free_stitch_task;
    fn_vgs_put_free_stitch_task *pfn_vgs_put_free_stitch_task;
#endif
#ifdef CONFIG_OT_VGS_CORNER_RECT_SUPPORT
    fn_vgs_add_corner_rect_task *pfn_vgs_add_corner_rect_task;
#endif
#ifdef CONFIG_OT_VGS_MCF_SUPPORT
    fn_vgs_add_mcf_task *pfn_vgs_add_mcf_task;
    fn_vgs_get_free_mcf_task *pfn_vgs_get_free_mcf_task;
    fn_vgs_put_free_mcf_task *pfn_vgs_put_free_mcf_task;
#endif
#ifdef CONFIG_OT_VGS_MULTI_CHN_SUPPORT
    fn_vgs_add_multi_chn_task *pfn_vgs_add_multi_chn_task;
#endif
#ifdef CONFIG_OT_VGS_DISPATCH_VPSS_SUPPORT
    fn_vgs_proc_vpss_callback *pfn_vgs_proc_vpss_callback;
#endif
} vgs_export_func;

const td_void *vgs_get_export_funcs(td_void);

#ifdef __cplusplus
}
#endif

#endif /* VGS_EXT_H */

