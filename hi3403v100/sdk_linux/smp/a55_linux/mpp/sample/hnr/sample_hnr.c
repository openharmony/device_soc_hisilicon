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

#include <signal.h>
#include "sample_comm.h"
#include "ot_sns_ctrl.h"
#include "ot_common_hnr.h"
#include "ss_mpi_hnr.h"
#include "ss_mpi_ae.h"
#include "sdk_module_init.h"
#include "sample_ipc.h"

ot_size g_mcf_chn_out_size[OT_MCF_MAX_PHYS_CHN_NUM] = { 0 };
td_bool g_mcf_chn_enable[OT_MCF_MAX_PHYS_CHN_NUM] = {TD_TRUE, TD_FALSE, TD_FALSE};
#define MCF_VENC_WIDHT_3840 3840
#define MCF_VNEC_HEIGHT_2160 2160
#define MCF_BIND_VPSS_GRP 2

#define VO_WIDTH 1920
#define VO_HEIGTH 1080

#define VENC_WIDTH 3840
#define VENC_HEIGTH 2160

#define VB_HNR_CNT                 20
#define VB_HNR_SNAP_CNT            20
#define VB_HNR_SNAP_ADVANCED_CNT   20
#define VB_HNR_BLEND_CNT          20
#define VB_MCF_YUV_CNT   20

#define VB_COMMON_LINE_CNT         20
#define VB_COMMON_WDR_CNT          20

volatile static sig_atomic_t g_sig_flag = 0;
static ot_vb_pool g_vb_pool = OT_VB_INVALID_POOL_ID;

typedef struct {
    td_bool is_wdr_mode;
    ot_hnr_ref_mode ref_mode;
    td_bool normal_blend;
    ot_vi_video_mode video_mode;
} sample_hnr_param;

typedef struct {
    ot_vi_pipe video_pipe;
    ot_vi_pipe snap_pipe;
    pthread_t  thread_id;
    td_bool    start;
} sample_hnr_snap_thread_info;

static sample_hnr_snap_thread_info g_snap_thread_info;

static sample_vo_cfg g_vo_cfg = {
    .vo_dev            = SAMPLE_VO_DEV_UHD,
    .vo_intf_type      = OT_VO_INTF_HDMI,
    .intf_sync         = OT_VO_OUT_1080P30,
    .bg_color          = COLOR_RGB_BLACK,
    .pix_format        = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    .disp_rect         = {0, 0, VO_WIDTH, VO_HEIGTH},
    .image_size        = {VO_WIDTH, VO_HEIGTH},
    .vo_part_mode      = OT_VO_PARTITION_MODE_SINGLE,
    .dis_buf_len       = 3, /* 3: def buf len for single */
    .dst_dynamic_range = OT_DYNAMIC_RANGE_SDR8,
    .vo_mode           = VO_MODE_1MUX,
    .compress_mode     = OT_COMPRESS_MODE_NONE,
};

static sample_comm_venc_chn_param g_venc_chn_param = {
    .frame_rate           = 30, /* 30 is a number */
    .stats_time           = 1,  /* 1 is a number */
    .gop                  = 30, /* 30 is a number */
    .venc_size            = {VENC_WIDTH, VENC_HEIGTH},
    .size                 = PIC_3840X2160,
    .profile              = 0,
    .is_rcn_ref_share_buf = TD_FALSE,
    .gop_attr             = {
        .gop_mode = OT_VENC_GOP_MODE_NORMAL_P,
        .normal_p = {2},
    },
    .type                 = OT_PT_H265,
    .rc_mode              = SAMPLE_RC_CBR,
};

static td_s32 g_hnr_cfg_id[OT_HNR_MAX_CFG_NUM] = {-1};
static td_void sample_get_char(td_char *s)
{
    if (g_sig_flag == 1) {
        return;
    }

    printf("---------------press any key to %s!---------------\n", s);
    (td_void)getchar();
}

static td_void sample_hnr_usage(char *prg_name)
{
    printf("usage : %s <index> \n", prg_name);
    printf("index:\n");
    printf("    (0) hnr line mode : base mode, has reference frame\n");
    printf("    (1) hnr wdr mode  : wdr and long frame mode\n");
    printf("    (2) hnr line and none ref mode : no reference frame\n");
    printf("    (3) hnr line mode + normal blend : base mode and normal blend on\n");
    printf("    (4) hnr snap advanced mode: advanced mode of no reference frame \n");
    printf("    (5) hnr line switch to wdr: line mode and hnr normal blend switch to wdr mode\n");
}

static td_void sample_hnr_get_default_vb_config(const ot_size *size, ot_vb_cfg *vb_cfg, td_u32 vb_cnt)
{
    ot_vb_calc_cfg calc_cfg;
    ot_pic_buf_attr buf_attr;

    (td_void)memset_s(vb_cfg, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg));
    vb_cfg->max_pool_cnt = 128; /* 128 blks */

    buf_attr.width         = size->width;
    buf_attr.height        = size->height;
    buf_attr.align         = OT_DEFAULT_ALIGN;
    buf_attr.bit_width     = OT_DATA_BIT_WIDTH_8;
    buf_attr.pixel_format  = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_422;
    buf_attr.compress_mode = OT_COMPRESS_MODE_NONE;
    ot_common_get_pic_buf_cfg(&buf_attr, &calc_cfg);

    vb_cfg->common_pool[0].blk_size = calc_cfg.vb_size;
    vb_cfg->common_pool[0].blk_cnt  = vb_cnt;
}

#ifdef SAMPLE_MEM_SHARE_ENABLE
static td_void sample_hnr_init_mem_share(td_void)
{
    td_u32 i;
    ot_vb_common_pools_id pools_id = {0};

    if (ss_mpi_vb_get_common_pool_id(&pools_id) != TD_SUCCESS) {
        sample_print("get common pool_id failed!\n");
        return;
    }
    for (i = 0; i < pools_id.pool_cnt; ++i) {
        ss_mpi_vb_pool_share_all(pools_id.pool[i]);
    }
}
#endif

static td_s32 sample_hnr_sys_init(ot_size *in_size, ot_vi_video_mode video_mode,  ot_vi_vpss_mode_type mode_type,
    td_u32 vb_cnt)
{
    td_s32 ret;
    ot_vb_cfg vb_cfg;
    td_u32 supplement_config;

    sample_hnr_get_default_vb_config(in_size, &vb_cfg, vb_cnt);

    supplement_config = OT_VB_SUPPLEMENT_BNR_MOT_MASK;
    ret = sample_comm_sys_init_with_vb_supplement(&vb_cfg, supplement_config);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

#ifdef SAMPLE_MEM_SHARE_ENABLE
    sample_hnr_init_mem_share();
#endif

    ret = sample_comm_vi_set_vi_vpss_mode(mode_type, video_mode);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_char *hnr_common_get_param_file(ot_hnr_ref_mode ref_mode)
{
    td_char *param_file = TD_NULL;

    if (ref_mode == OT_HNR_REF_MODE_NORM) {
        param_file = "./cfg/normal_hnr.bin";
    } else {
        param_file = "./cfg/snap_hnr.bin";
    }

    printf("load config file %s\n", param_file);
    return param_file;
}

static td_s32 hnr_common_load_mem(ot_hnr_mem_info *mem, ot_hnr_ref_mode ref_mode)
{
    td_s32 ret;
    td_char *param_file = TD_NULL;
    FILE *fp = TD_NULL;

    param_file = hnr_common_get_param_file(ref_mode);
    /* Get model file size */
    fp = fopen(param_file, "rb");
    if (fp == TD_NULL) {
        sample_print("open file %s err!\n", param_file);
        return TD_FAILURE;
    }

    ret = fseek(fp, 0L, SEEK_END);
    if (ret != TD_SUCCESS) {
        sample_print("fseek end failed!\n");
        goto fail_0;
    }

    mem->size = ftell(fp);
    if (mem->size <= 0) {
        sample_print("ftell failed!\n");
        goto fail_0;
    }

    ret = fseek(fp, 0L, SEEK_SET);
    if (ret != TD_SUCCESS) {
        sample_print("fseek set failed!\n");
        goto fail_0;
    }

    /* malloc model file mem */
    ret = ss_mpi_sys_mmz_alloc(&(mem->phys_addr), &(mem->virt_addr), "hnr_cfg", TD_NULL, mem->size);
    if (ret != TD_SUCCESS) {
        sample_print("malloc mmz failed!\n");
        goto fail_0;
    }

    ret = fread(mem->virt_addr, mem->size, 1, fp);
    if (ret != 1) {
        sample_print("read model file failed!\n");
        goto fail_1;
    }

    ret = fclose(fp);
    if (ret != 0) {
        sample_print("close file error\n");
    }
    return TD_SUCCESS;

fail_1:
    ss_mpi_sys_mmz_free(mem->phys_addr, mem->virt_addr);
    mem->phys_addr = 0;
    mem->virt_addr = TD_NULL;
fail_0:
    if (fp != TD_NULL) {
        fclose(fp);
    }
    return TD_FAILURE;
}

static td_void hnr_common_unload_mem(ot_hnr_mem_info *param_mem)
{
    if ((param_mem->phys_addr != 0) && (param_mem->virt_addr != TD_NULL)) {
        (td_void)ss_mpi_sys_mmz_free(param_mem->phys_addr, param_mem->virt_addr);
    }
}

static td_s32 sample_hnr_set_attr(ot_vi_pipe vi_pipe, sample_hnr_param *hnr_param)
{
    td_s32 ret;
    ot_hnr_attr hnr_attr;

    ret = ss_mpi_hnr_get_attr(vi_pipe, &hnr_attr);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_hnr_get_attr error\n");
        return ret;
    }

    hnr_attr.enable = TD_TRUE;
    hnr_attr.bnr_bypass =  TD_FALSE;
    hnr_attr.normal_blend = hnr_param->normal_blend;
    hnr_attr.op_type = OT_OP_MODE_MANUAL;
    hnr_attr.manual_attr.param.sfs = 31; /* sfs: 31 */
    hnr_attr.manual_attr.param.tfs = 31; /* tfs: 31 */
    ret = ss_mpi_hnr_set_attr(vi_pipe, &hnr_attr);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_hnr_set_attr error\n");
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 sample_hnr_load_cfg(ot_size in_size, ot_hnr_cfg *hnr_cfg, ot_hnr_ref_mode ref_mode)
{
    td_s32 ret;

    ret = hnr_common_load_mem((ot_hnr_mem_info *)&(hnr_cfg->mem_info), ref_mode);
    if (ret != TD_SUCCESS) {
        sample_print("hnr_common_load_mem error\n");
        return ret;
    }

    if (ref_mode != OT_HNR_REF_MODE_NORM) {
        hnr_cfg->image_size.width  = in_size.width;
        hnr_cfg->image_size.height = in_size.height;
        hnr_cfg->is_wdr_mode = TD_FALSE;
        ret = ss_mpi_hnr_load_cfg(hnr_cfg, &g_hnr_cfg_id[0]);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_hnr_load_cfg error(%#x)\n", ret);
            goto unload_mem;
        }
        return ret;
    }

    hnr_cfg->image_size.width  = in_size.width;
    hnr_cfg->image_size.height = in_size.height;
    hnr_cfg->is_wdr_mode = TD_FALSE;
    ret = ss_mpi_hnr_load_cfg(hnr_cfg, &g_hnr_cfg_id[0]);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_hnr_load_cfg error(%#x)\n", ret);
        goto unload_mem;
    }

    hnr_cfg->is_wdr_mode = TD_TRUE;
    ret = ss_mpi_hnr_load_cfg(hnr_cfg, &g_hnr_cfg_id[1]);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_hnr_load_cfg error(%#x)\n", ret);
        goto unload_mem;
    }
    return ret;

unload_mem:
    hnr_common_unload_mem((ot_hnr_mem_info *)&(hnr_cfg->mem_info));
    return ret;
}

static td_void sample_hnr_unload_cfg(ot_hnr_cfg *hnr_cfg, ot_hnr_ref_mode ref_mode)
{
    if (ss_mpi_hnr_unload_cfg(g_hnr_cfg_id[0]) != TD_SUCCESS) {
        sample_print("ss_mpi_hnr_unload_cfg error\n");
    }
    if (ref_mode == OT_HNR_REF_MODE_NORM) {
        if (ss_mpi_hnr_unload_cfg(g_hnr_cfg_id[1]) != TD_SUCCESS) {
            sample_print("ss_mpi_hnr_unload_cfg error\n");
        }
    }
    hnr_common_unload_mem((ot_hnr_mem_info *)&(hnr_cfg->mem_info));
}

static td_s32 sample_hnr_start(ot_vi_pipe vi_pipe, ot_size in_size, ot_hnr_cfg *hnr_cfg,
    sample_hnr_param *hnr_param)
{
    td_s32 ret = ss_mpi_hnr_init();
    ot_hnr_alg_cfg attr;

    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_hnr_init error(%#x)\n", ret);
        return ret;
    }

    ret = sample_hnr_load_cfg(in_size, hnr_cfg, hnr_param->ref_mode);
    if (ret != TD_SUCCESS) {
        goto task_exit;
    }

    attr.ref_mode = hnr_param->ref_mode; /* ref mode NORM: need reference frame; NONE: no reference frame */
    ret = ss_mpi_hnr_set_alg_cfg(vi_pipe, &attr);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_hnr_set_alg_cfg error(%#x)\n", ret);
        goto unload_cfg;
    }

    sample_get_char("enable hnr");

    ret = ss_mpi_hnr_enable(vi_pipe);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_hnr_enable error(%#x)\n", ret);
        goto unload_cfg;
    }

    ret = sample_hnr_set_attr(vi_pipe, hnr_param);
    if (ret != TD_SUCCESS) {
        sample_print("sample_hnr_set_attr error(%#x)\n", ret);
        goto hnr_disable;
    }

    return ret;

hnr_disable:
    ss_mpi_hnr_disable(vi_pipe);
unload_cfg:
    sample_hnr_unload_cfg(hnr_cfg, hnr_param->ref_mode);
task_exit:
    ss_mpi_hnr_exit();

    return ret;
}

static td_s32 sample_hnr_stop(ot_vi_pipe vi_pipe, ot_hnr_cfg *hnr_cfg, sample_hnr_param *hnr_param)
{
    td_s32 return_value = TD_SUCCESS;
    td_s32 ret;

    ret = ss_mpi_hnr_disable(vi_pipe);
    if (ret != TD_SUCCESS) {
        return_value = TD_FAILURE;
        sample_print("ss_mpi_hnr_disable false error(%#x)\n", ret);
    }

    ret = ss_mpi_hnr_unload_cfg(g_hnr_cfg_id[0]);
    if (ret != TD_SUCCESS) {
        return_value = TD_FAILURE;
        sample_print("ss_mpi_hnr_unload_cfg error(%#x)\n", ret);
    }

    if (hnr_param->ref_mode == OT_HNR_REF_MODE_NORM) {
        ret = ss_mpi_hnr_unload_cfg(g_hnr_cfg_id[1]);
        if (ret != TD_SUCCESS) {
            return_value = TD_FAILURE;
            sample_print("ss_mpi_hnr_unload_cfg error(%#x)\n", ret);
        }
    }
    hnr_common_unload_mem((ot_hnr_mem_info *)&(hnr_cfg->mem_info));

    ss_mpi_hnr_exit();
    return return_value;
}

static td_s32 sample_hnr_start_vpss(ot_vpss_grp grp, ot_size *in_size)
{
    td_s32 ret;
    ot_low_delay_info low_delay_info;
    ot_vpss_grp_attr grp_attr;
    ot_vpss_chn_attr chn_attr;
    td_bool chn_enable[OT_VPSS_MAX_PHYS_CHN_NUM] = {TD_TRUE, TD_FALSE, TD_FALSE, TD_FALSE};

    sample_comm_vpss_get_default_grp_attr(&grp_attr);
    grp_attr.max_width  = in_size->width;
    grp_attr.max_height = in_size->height;
    sample_comm_vpss_get_default_chn_attr(&chn_attr);
    chn_attr.width  = in_size->width;
    chn_attr.height = in_size->height;

    ret = sample_common_vpss_start(grp, chn_enable, &grp_attr, &chn_attr, OT_VPSS_MAX_PHYS_CHN_NUM);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    low_delay_info.enable     = TD_TRUE;
    low_delay_info.line_cnt   = 200; /* 200: lowdelay line */
    low_delay_info.one_buf_en = TD_FALSE;
    ret = ss_mpi_vpss_set_low_delay_attr(grp, 0, &low_delay_info);
    if (ret != TD_SUCCESS) {
        sample_common_vpss_stop(grp, chn_enable, OT_VPSS_MAX_PHYS_CHN_NUM);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 sample_hnr_start_vo(sample_vo_mode vo_mode)
{
    g_vo_cfg.vo_mode = vo_mode;

    return sample_comm_vo_start_vo(&g_vo_cfg);
}

static td_void sample_hnr_stop_vo(td_void)
{
    sample_comm_vo_stop_vo(&g_vo_cfg);
}

static td_s32 sample_hnr_start_venc(ot_venc_chn venc_chn[], td_u32 venc_chn_len, td_u32 chn_num)
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < (td_s32)chn_num && i < venc_chn_len; i++) {
        ret = sample_comm_venc_start(venc_chn[i], &g_venc_chn_param);
        if (ret != TD_SUCCESS) {
            goto exit;
        }
    }

    ret = sample_comm_venc_start_get_stream(venc_chn, chn_num);
    if (ret != TD_SUCCESS) {
        goto exit;
    }

    return TD_SUCCESS;

exit:
    for (i = i - 1; i >= 0; i--) {
        sample_comm_venc_stop(venc_chn[i]);
    }
    return TD_FAILURE;
}

static td_s32 sample_hnr_start_venc_and_vo(ot_vpss_grp vpss_grp[], td_u32 grp_num, ot_size *in_size)
{
    td_u32 i;
    td_s32 ret;
    sample_vo_mode vo_mode = VO_MODE_1MUX;
    const ot_vpss_chn vpss_chn = 0;
    const ot_vo_layer vo_layer = 0;
    ot_vo_chn vo_chn[4] = {0, 1, 2, 3};     /* 4: max chn num, 0/1/2/3 chn id */
    ot_venc_chn venc_chn[4] = {0, 1, 2, 3}; /* 4: max chn num, 0/1/2/3 chn id */

    if (grp_num > 1) {
        vo_mode = VO_MODE_4MUX;
    }

    ret = sample_hnr_start_vo(vo_mode);
    if (ret != TD_SUCCESS) {
        goto start_vo_failed;
    }

    g_venc_chn_param.venc_size.width  = in_size->width;
    g_venc_chn_param.venc_size.height = in_size->height;

    if (SENSOR0_TYPE == HY_S0603_MIPI_8M_30FPS_12BIT) {
        g_venc_chn_param.size = PIC_3840X2160;
    } else if (SENSOR0_TYPE == OV_OS08A20_MIPI_8M_30FPS_12BIT) {
        g_venc_chn_param.size = PIC_3840X2160;
    } else {
        g_venc_chn_param.size = PIC_1080P;
    }

    ret = sample_hnr_start_venc(venc_chn, sizeof(venc_chn) / sizeof(ot_venc_chn), grp_num);
    if (ret != TD_SUCCESS) {
        goto start_venc_failed;
    }

    for (i = 0; i < grp_num; i++) {
        sample_comm_vpss_bind_vo(vpss_grp[i], vpss_chn, vo_layer, vo_chn[i]);
        sample_comm_vpss_bind_venc(vpss_grp[i], vpss_chn, venc_chn[i]);
    }
    return TD_SUCCESS;

start_venc_failed:
    sample_hnr_stop_vo();
start_vo_failed:
    return TD_FAILURE;
}

static td_void sample_hnr_stop_vpss(ot_vpss_grp grp)
{
    td_bool chn_enable[OT_VPSS_MAX_PHYS_CHN_NUM] = {TD_TRUE, TD_FALSE, TD_FALSE, TD_FALSE};

    sample_common_vpss_stop(grp, chn_enable, OT_VPSS_MAX_PHYS_CHN_NUM);
}

static td_void sample_hnr_stop_venc(ot_venc_chn venc_chn[], td_u32 venc_chn_len, td_u32 chn_num)
{
    td_u32 i;

    sample_comm_venc_stop_get_stream(chn_num);

    for (i = 0; i < chn_num && i < venc_chn_len; i++) {
        sample_comm_venc_stop(venc_chn[i]);
    }
}

static td_void sample_hnr_stop_venc_and_vo(ot_vpss_grp vpss_grp[], td_u32 grp_num)
{
    td_u32 i;
    const ot_vpss_chn vpss_chn = 0;
    const ot_vo_layer vo_layer = 0;
    ot_vo_chn vo_chn[4] = {0, 1, 2, 3};     /* 4: max chn num, 0/1/2/3 chn id */
    ot_venc_chn venc_chn[4] = {0, 1, 2, 3}; /* 4: max chn num, 0/1/2/3 chn id */

    for (i = 0; i < grp_num; i++) {
        sample_comm_vpss_un_bind_vo(vpss_grp[i], vpss_chn, vo_layer, vo_chn[i]);
        sample_comm_vpss_un_bind_venc(vpss_grp[i], vpss_chn, venc_chn[i]);
    }

    sample_hnr_stop_venc(venc_chn, sizeof(venc_chn) / sizeof(ot_venc_chn), grp_num);
    sample_hnr_stop_vo();
}

static td_void sample_hnr_sensor_cfg(ot_vi_pipe vi_pipe, sample_sns_type sns_type)
{
    return;
}

static td_s32 sample_hnr_comm_start_vi(ot_vi_pipe vi_pipe, const sample_vi_cfg *vi_cfg, ot_size *in_size,
    td_u32 vb_cnt)
{
    td_s32 ret;
    td_s32 blk_size;
    ot_vb_pool vb_pool;
    ot_vb_pool_cfg vb_pool_cfg = {0};

    blk_size = ot_hnr_get_pic_buf_size(in_size->width, in_size->height);
    vb_pool_cfg.blk_size = blk_size;
    vb_pool_cfg.blk_cnt = vb_cnt;
    vb_pool_cfg.remap_mode = OT_VB_REMAP_MODE_NONE;
    vb_pool = ss_mpi_vb_create_pool(&vb_pool_cfg);
    if (vb_pool == OT_VB_INVALID_POOL_ID) {
        sample_print("create user pool failed!\n");
        return TD_FAILURE;
    }

    ret = sample_comm_vi_start_vi(vi_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("start vi failed.\n");
        return ret;
    }

    ret = ss_mpi_vi_pipe_set_vb_src(vi_pipe, OT_VB_SRC_USER);
    if (ret != TD_SUCCESS) {
        sample_print("vi pipe set vb src failed.\n");
        return ret;
    }

    ret = ss_mpi_vi_pipe_attach_vb_pool(vi_pipe, vb_pool);
    if (ret != TD_SUCCESS) {
        sample_print("vi pipe attach vb pool failed.\n");
        return ret;
    }

    g_vb_pool = vb_pool;

    return TD_SUCCESS;
}

static td_s32 sample_hnr_start_vi(ot_vi_pipe vi_pipe, const sample_vi_cfg *vi_cfg, ot_size *in_size,
    sample_hnr_param *hnr_param, td_u32 vb_cnt)
{
    td_s32 ret;
    ot_vi_vpss_mode_type mode_type;
    td_u32 common_vb_num = hnr_param->is_wdr_mode ? VB_COMMON_WDR_CNT : VB_COMMON_LINE_CNT;

    if (hnr_param->ref_mode == OT_HNR_REF_MODE_NORM) {
        mode_type = (hnr_param->video_mode == OT_VI_VIDEO_MODE_ADVANCED) ?
            OT_VI_ONLINE_VPSS_OFFLINE : OT_VI_OFFLINE_VPSS_ONLINE;
    } else {
        mode_type = OT_VI_OFFLINE_VPSS_ONLINE;
    }

    if (sample_hnr_sys_init(in_size, hnr_param->video_mode, mode_type, common_vb_num) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = sample_hnr_comm_start_vi(vi_pipe, vi_cfg, in_size, vb_cnt);

    return ret;
}

static td_s32 sample_hnr_switch_start_vi(ot_vi_pipe vi_pipe, const sample_vi_cfg *vi_cfg, ot_size *in_size)
{
    td_s32 ret;
    td_s32 blk_size;
    ot_vb_pool vb_pool;
    ot_vb_pool_cfg vb_pool_cfg = {0};

    blk_size = ot_hnr_get_pic_buf_size(in_size->width, in_size->height);
    vb_pool_cfg.blk_size = blk_size;
    vb_pool_cfg.blk_cnt = VB_HNR_BLEND_CNT;
    vb_pool_cfg.remap_mode = OT_VB_REMAP_MODE_NONE;
    vb_pool = ss_mpi_vb_create_pool(&vb_pool_cfg);
    if (vb_pool == OT_VB_INVALID_POOL_ID) {
        sample_print("create user pool failed!\n");
        return TD_FAILURE;
    }

    ret = sample_comm_vi_mode_switch_start_vi(vi_cfg, TD_FALSE, (const ot_size *)&in_size);
    if (ret != TD_SUCCESS) {
        sample_print("start vi failed.\n");
        return ret;
    }

    ret = ss_mpi_vi_pipe_set_vb_src(vi_pipe, OT_VB_SRC_USER);
    if (ret != TD_SUCCESS) {
        sample_print("vi pipe set vb src failed.\n");
        return ret;
    }

    ret = ss_mpi_vi_pipe_attach_vb_pool(vi_pipe, vb_pool);
    if (ret != TD_SUCCESS) {
        sample_print("vi pipe attach vb pool failed.\n");
        return ret;
    }

    g_vb_pool = vb_pool;

    return TD_SUCCESS;
}

static td_void sample_hnr_stop_vi(ot_vi_pipe vi_pipe, const sample_vi_cfg *vi_cfg)
{
    sample_comm_vi_stop_vi(vi_cfg);

    if (g_vb_pool != OT_VB_INVALID_POOL_ID) {
        ss_mpi_vb_destroy_pool(g_vb_pool);
        g_vb_pool = OT_VB_INVALID_POOL_ID;
    }
}

static td_s32 sample_hnr_set_long_frame_mode(ot_vi_pipe vi_pipe, td_bool is_wdr_mode)
{
    td_s32 ret;
    ot_isp_exposure_attr exp_attr;
    ot_isp_wdr_fs_attr fswdr_attr;

    if (is_wdr_mode == TD_FALSE) {
        return TD_SUCCESS;
    }

    sample_get_char("set long frame mode");

    ret = ss_mpi_isp_get_exposure_attr(vi_pipe, &exp_attr);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_isp_get_exposure_attr error[0x%x]\n", ret);
        return TD_FAILURE;
    }

    exp_attr.auto_attr.fswdr_mode = OT_ISP_FSWDR_LONG_FRAME_MODE;
    ret = ss_mpi_isp_set_exposure_attr(vi_pipe, &exp_attr);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_isp_set_exposure_attr error[0x%x]\n", ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_isp_get_fswdr_attr(vi_pipe, &fswdr_attr);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_isp_get_fswdr_attr error[0x%x]\n", ret);
        return TD_FAILURE;
    }

    fswdr_attr.wdr_merge_mode = OT_ISP_MERGE_WDR_MODE;
    ret = ss_mpi_isp_set_fswdr_attr(vi_pipe, &fswdr_attr);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_isp_set_fswdr_attr error[0x%x]\n", ret);
        return TD_FAILURE;
    }

    return ret;
}

static td_void sample_hnr_get_default_cfg(sample_sns_type sns_type, ot_vi_pipe vi_pipe, ot_vi_video_mode video_mode,
    ot_size *size, sample_vi_cfg *vi_cfg)
{
    sample_comm_vi_get_size_by_sns_type(sns_type, size);
    sample_comm_vi_get_default_vi_cfg(sns_type, vi_cfg);

    if (video_mode == OT_VI_VIDEO_MODE_NORM) {
        vi_cfg->pipe_info[vi_pipe].pipe_attr.compress_mode = OT_COMPRESS_MODE_NONE;
    }
}

static td_s32 sample_hnr_set_blc(ot_vi_pipe vi_pipe, sample_sns_type sns_type)
{
    td_s32 i;
    td_s32 j;
    td_s32 ret;
    ot_isp_black_level_attr black_level_attr;

    ret = ss_mpi_isp_get_black_level_attr(vi_pipe, &black_level_attr);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_isp_get_black_level_attr failed.\n");
        return TD_FAILURE;
    }

    black_level_attr.user_black_level_en = TD_TRUE;

    for (i = 0; i < OT_ISP_WDR_MAX_FRAME_NUM; i++) {
        for (j = 0; j < OT_ISP_BAYER_CHN_NUM; j++) {
            black_level_attr.user_black_level[i][j] = 1200; /* user_black_level of hnr default as 1200 */
        }
    }

    if (sns_type == HY_S0603_MIPI_8M_30FPS_12BIT) {
        black_level_attr.black_level_mode = OT_ISP_BLACK_LEVEL_MODE_DYNAMIC;
    }

    ret = ss_mpi_isp_set_black_level_attr(vi_pipe, &black_level_attr);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_isp_set_black_level_attr failed.\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_hnr_switch_first_route_line(sample_sns_type sns_type, ot_hnr_cfg *hnr_cfg)
{
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    sample_vi_cfg vi_cfg;
    ot_size in_size;
    td_s32 ret = TD_FAILURE;
    sample_hnr_param hnr_param = {TD_FALSE, OT_HNR_REF_MODE_NORM, TD_TRUE, OT_VI_VIDEO_MODE_NORM};
    ot_vi_vpss_mode_type mode_type = OT_VI_OFFLINE_VPSS_ONLINE;

    sample_hnr_get_default_cfg(sns_type, vi_pipe, hnr_param.video_mode, &in_size, &vi_cfg);

    if (sample_hnr_sys_init(&in_size, hnr_param.video_mode, mode_type, VB_COMMON_WDR_CNT) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (sample_hnr_comm_start_vi(vi_pipe, &vi_cfg, &in_size, VB_HNR_BLEND_CNT) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    sample_hnr_sensor_cfg(vi_pipe, sns_type);

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);

    if (sample_hnr_start_vpss(vpss_grp[0], &in_size) != TD_SUCCESS) {
        goto stop_vi;
    }

    if (sample_hnr_start_venc_and_vo(vpss_grp, grp_num, &in_size) != TD_SUCCESS) {
        goto stop_vpss;
    }

    if (sample_hnr_set_blc(vi_pipe, sns_type) != TD_SUCCESS) {
        goto stop_venc_and_vo;
    }

    if (sample_hnr_start(vi_pipe, in_size, hnr_cfg, &hnr_param) != TD_SUCCESS) {
        goto stop_venc_and_vo;
    }

    sample_get_char("exit line mode");

    ret = ss_mpi_hnr_disable(vi_pipe);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_hnr_disable false error(%#x)\n", ret);
    }

stop_venc_and_vo:
    sample_hnr_stop_venc_and_vo(vpss_grp, grp_num);
stop_vpss:
    sample_hnr_stop_vpss(vpss_grp[0]);
stop_vi:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], vpss_chn);
    if (ret == TD_SUCCESS) {
        sample_comm_vi_mode_switch_stop_vi(&vi_cfg);
    } else {
        sample_comm_vi_stop_vi(&vi_cfg);
    }

    return ret;
}

static td_s32 sample_hnr_switch_second_route_wdr(sample_sns_type sns_type, ot_hnr_cfg *hnr_cfg)
{
    const ot_vi_pipe vi_pipe = 1;
    const ot_vi_pipe master_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    const td_u32 grp_num = 1;
    const ot_vpss_chn vpss_chn = 0;
    ot_size in_size;
    sample_vi_cfg vi_cfg;
    td_s32 ret = TD_FAILURE;
    sample_hnr_param hnr_param = {TD_TRUE, OT_HNR_REF_MODE_NORM, TD_FALSE, OT_VI_VIDEO_MODE_NORM};

    sample_comm_vi_get_size_by_sns_type(sns_type, &in_size);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    sample_hnr_get_default_cfg(sns_type, vi_pipe, hnr_param.video_mode, &in_size, &vi_cfg);

    if (sample_hnr_switch_start_vi(vi_pipe, &vi_cfg, &in_size) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    sample_hnr_sensor_cfg(vi_pipe, sns_type);
    sample_comm_vi_bind_vpss(master_pipe, vi_chn, vpss_grp[0], vpss_chn);

    if (sample_hnr_start_vpss(vpss_grp[0], &in_size) != TD_SUCCESS) {
        goto stop_vi;
    }

    if (sample_hnr_start_venc_and_vo(vpss_grp, grp_num, &in_size) != TD_SUCCESS) {
        goto stop_vpss;
    }

    if (sample_hnr_set_blc(master_pipe, sns_type) != TD_SUCCESS) {
        goto stop_venc_and_vo;
    }

    sample_get_char("enable hnr");

    ret = ss_mpi_hnr_enable(vi_pipe);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_hnr_enable false error(%#x)\n", ret);
        goto exit;
    }

    ret = sample_hnr_set_attr(vi_pipe, &hnr_param);
    if (ret != TD_SUCCESS) {
        sample_print("sample_hnr_set_attr error(%#x)\n", ret);
    }

    sample_get_char("exit wdr mode");

exit:
    sample_hnr_stop(vi_pipe, hnr_cfg, &hnr_param);
stop_venc_and_vo:
    sample_hnr_stop_venc_and_vo(vpss_grp, grp_num);
stop_vpss:
    sample_hnr_stop_vpss(vpss_grp[0]);
stop_vi:
    sample_comm_vi_un_bind_vpss(master_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_stop_vi(&vi_cfg);

    return ret;
}

static td_s32 sample_hnr_line_switch_to_wdr(td_void)
{
    td_s32 ret;
    sample_vi_cfg vi_cfg;
    ot_size in_size;
    const ot_vi_pipe vi_pipe = 0;
    ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;
    sample_sns_type sns_type = SENSOR0_TYPE;
    ot_hnr_cfg hnr_cfg;
    ot_vi_vpss_mode_type mode_type = OT_VI_OFFLINE_VPSS_ONLINE;

    sample_hnr_get_default_cfg(sns_type, vi_pipe, video_mode, &in_size, &vi_cfg);

    if (sample_hnr_sys_init(&in_size, video_mode, mode_type, VB_COMMON_WDR_CNT) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = sample_hnr_switch_first_route_line(sns_type, &hnr_cfg);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    printf("switch to wdr mode\n");

    if (sns_type == HY_S0603_MIPI_8M_30FPS_12BIT) {
        sns_type = HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1;
    } else {
        sns_type = HY_S0603_MIPI_8M_30FPS_12BIT;
    }

    ret = sample_hnr_switch_second_route_wdr(sns_type, &hnr_cfg);
    sample_comm_sys_exit();

    return ret;
}

static td_s32 sample_hnr_check_support(ot_vi_pipe vi_pipe, sample_hnr_param *hnr_param)
{
    if (hnr_param->is_wdr_mode == TD_TRUE) {
        if (hnr_param->video_mode != OT_VI_VIDEO_MODE_NORM) {
            sample_print("not support OT_VI_VIDEO_MODE_ADVANCED in wdr mode, must be OT_VI_VIDEO_MODE_NORM \n");
            return TD_FAILURE;
        }

        if (hnr_param->normal_blend != TD_FALSE) {
            sample_print("normal_blend must be false in wdr mode\n");
            return TD_FAILURE;
        }

        if (vi_pipe >= OT_VI_MAX_PHYS_PIPE_NUM) {
            sample_print("vi_pipe must be phy pipe in wdr mode\n");
            return TD_FAILURE;
        }
    }

    /* bnr_bypass is invalid in OT_VI_VIDEO_MODE_ADVANCED */
    /* bnr_bypass must be false when attr->enable is false and in OT_VI_VIDEO_MODE_NORM */
    /* normal_blend is valid when bnr_bypass is false in OT_VI_VIDEO_MODE_NORM */
    /* vi must be offline when hnr in OT_VI_VIDEO_MODE_NORM */

    return TD_SUCCESS;
}

static td_u32 sample_hnr_get_vb_cnt(sample_hnr_param *hnr_param)
{
    td_u32 vb_cnt = 0;
    if (hnr_param->ref_mode == OT_HNR_REF_MODE_NORM) {
        vb_cnt = hnr_param->normal_blend ? VB_HNR_BLEND_CNT : VB_HNR_CNT;
    } else if (hnr_param->ref_mode == OT_HNR_REF_MODE_NONE) {
        vb_cnt = VB_HNR_SNAP_CNT;
    } else if (hnr_param->ref_mode == OT_HNR_REF_MODE_NONE_ADVANCED) {
        vb_cnt = VB_HNR_SNAP_ADVANCED_CNT;
    }

    return vb_cnt;
}

static td_s32 sample_hnr(sample_hnr_param *hnr_param)
{
    td_s32 ret = TD_FAILURE;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type = hnr_param->is_wdr_mode ? HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1 : SENSOR0_TYPE;
    const td_u32 vb_cnt = sample_hnr_get_vb_cnt(hnr_param);
    const ot_vi_pipe vi_pipe = hnr_param->is_wdr_mode ? 1 : 0; /* 1-long frame of wdr */
    const ot_vi_pipe master_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    ot_size in_size;
    ot_hnr_cfg hnr_cfg;

    sample_hnr_get_default_cfg(sns_type, vi_pipe, hnr_param->video_mode, &in_size, &vi_cfg);

    if (sample_hnr_check_support(vi_pipe, hnr_param) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (sample_hnr_start_vi(vi_pipe, &vi_cfg, &in_size, hnr_param, vb_cnt) != TD_SUCCESS) {
        goto sys_exit;
    }

    sample_hnr_sensor_cfg(vi_pipe, sns_type);

    sample_comm_vi_bind_vpss(master_pipe, vi_chn, vpss_grp[0], 0);
    if (sample_hnr_start_vpss(vpss_grp[0], &in_size) != TD_SUCCESS) {
        goto stop_vi;
    }

    if (sample_hnr_start_venc_and_vo(vpss_grp, sizeof(vpss_grp) / sizeof(vpss_grp[0]), &in_size) != TD_SUCCESS) {
        goto stop_vpss;
    }

    if (sample_hnr_set_blc(master_pipe, sns_type) != TD_SUCCESS) {
        goto stop_venc_and_vo;
    }
     
    if (sample_hnr_start(vi_pipe, in_size, &hnr_cfg, hnr_param) != TD_SUCCESS) {
        goto stop_venc_and_vo;
    }

    if (sample_hnr_set_long_frame_mode(master_pipe, hnr_param->is_wdr_mode) != TD_SUCCESS) {
        goto stop_hnr;
    }

    sample_get_char("exit");
    ret = TD_SUCCESS;

stop_hnr:
    sample_hnr_stop(vi_pipe, &hnr_cfg, hnr_param);
stop_venc_and_vo:
    sample_hnr_stop_venc_and_vo(vpss_grp, sizeof(vpss_grp) / sizeof(vpss_grp[0]));
stop_vpss:
    sample_hnr_stop_vpss(vpss_grp[0]);
stop_vi:
    sample_comm_vi_un_bind_vpss(master_pipe, vi_chn, vpss_grp[0], 0);
    sample_hnr_stop_vi(vi_pipe, &vi_cfg);
sys_exit:
    sample_comm_sys_exit();
    return ret;
}

static td_void *sample_hnr_snap_thread(td_void *param)
{
    td_s32 ret;
    const td_s32 milli_sec = 1000; /* 1000: wait 1000ms */
    ot_video_frame_info get_frame_info;
    ot_vi_frame_dump_attr dump_attr;
    sample_hnr_snap_thread_info *thread_info = (sample_hnr_snap_thread_info *)param;
    const ot_video_frame_info *send_frame_info[1];

    dump_attr.enable = TD_TRUE;
    dump_attr.depth = 3; /* 3: dump depth is 3 */
    ret = ss_mpi_vi_set_pipe_frame_dump_attr(thread_info->video_pipe, &dump_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set pipe frame dump attr failed! ret:0x%x\n", ret);
        return TD_NULL;
    }

    ret = ss_mpi_vi_set_pipe_frame_source(thread_info->snap_pipe, OT_VI_PIPE_FRAME_SOURCE_USER);
    if (ret != TD_SUCCESS) {
        sample_print("vi set pipe frame source failed!\n");
        return TD_NULL;
    }

    while (thread_info->start == TD_TRUE) {
        ret = ss_mpi_vi_get_pipe_frame(thread_info->video_pipe, &get_frame_info, milli_sec);
        if (ret != TD_SUCCESS) {
            break;
        }

        ret = ss_mpi_isp_run_once(thread_info->snap_pipe);
        if (ret != TD_SUCCESS) {
            sample_print("isp run once failed!\n");
            goto exit0;
        }

        send_frame_info[0] = &get_frame_info;
        ret = ss_mpi_vi_send_pipe_raw(thread_info->snap_pipe, send_frame_info, 1, milli_sec);
        if (ret != TD_SUCCESS) {
            sample_print("vi send pipe bas dump frame failed!\n");
        }

exit0:
        ret = ss_mpi_vi_release_pipe_frame(thread_info->video_pipe, &get_frame_info);
        if (ret != TD_SUCCESS) {
            sample_print("release pipe frame failed!\n");
            return TD_NULL;
        }
    }

    return TD_NULL;
}

static td_s32 sample_hnr_snap_create_thread(ot_vi_pipe video_pipe, ot_vi_pipe capture_pipe)
{
    td_s32 ret;

    g_snap_thread_info.video_pipe   = video_pipe;
    g_snap_thread_info.snap_pipe = capture_pipe;
    ret = pthread_create(&g_snap_thread_info.thread_id, TD_NULL, sample_hnr_snap_thread, &g_snap_thread_info);
    if (ret != 0) {
        sample_print("create capture thread failed!\n");
        return TD_FAILURE;
    }
    g_snap_thread_info.start = TD_TRUE;

    return TD_SUCCESS;
}

static td_void sample_hnr_snap_set_vi_cfg(sample_sns_type sns_type, ot_vi_pipe vi_pipe, ot_vi_video_mode video_mode,
    ot_size *size, sample_vi_cfg *vi_cfg)
{
    sample_hnr_get_default_cfg(sns_type, vi_pipe, video_mode, size, vi_cfg);
    vi_cfg->pipe_info[vi_pipe].pipe_attr.compress_mode = OT_COMPRESS_MODE_NONE;
    vi_cfg->pipe_info[vi_pipe].isp_need_run = TD_FALSE;
}

static td_void sample_hnr_snap_destroy_thread(td_void)
{
    if (g_snap_thread_info.start == TD_TRUE) {
        g_snap_thread_info.start = TD_FALSE;
        pthread_join(g_snap_thread_info.thread_id, NULL);
    }
}

static td_s32 sample_hnr_snap(sample_hnr_param *hnr_param)
{
    td_s32 ret = TD_FAILURE;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type =  SENSOR0_TYPE;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    ot_vpss_grp vpss_grp[1] = {0};
    ot_size in_size;
    ot_hnr_cfg hnr_cfg;
    const td_u32 vb_cnt = sample_hnr_get_vb_cnt(hnr_param);

    sample_hnr_snap_set_vi_cfg(sns_type, vi_pipe, hnr_param->video_mode, &in_size, &vi_cfg);

    if (sample_hnr_check_support(vi_pipe, hnr_param) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (sample_hnr_start_vi(vi_pipe, &vi_cfg, &in_size, hnr_param, vb_cnt) != TD_SUCCESS) {
        goto sys_exit;
    }

    sample_hnr_sensor_cfg(vi_pipe, sns_type);

    sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], 0);
    if (sample_hnr_start_vpss(vpss_grp[0], &in_size) != TD_SUCCESS) {
        goto stop_vi;
    }

    if (sample_hnr_start_venc_and_vo(vpss_grp, sizeof(vpss_grp) / sizeof(vpss_grp[0]), &in_size) != TD_SUCCESS) {
        goto stop_vpss;
    }

    if (sample_hnr_set_blc(vi_pipe, sns_type) != TD_SUCCESS) {
        goto stop_venc_and_vo;
    }

    if (sample_hnr_start(vi_pipe, in_size, &hnr_cfg, hnr_param) != TD_SUCCESS) {
        goto stop_venc_and_vo;
    }

    if (sample_hnr_snap_create_thread(vi_pipe, vi_pipe) != TD_SUCCESS) {
        goto stop_hnr;
    }

    sample_get_char("exit");

    sample_hnr_snap_destroy_thread();

    ret = TD_SUCCESS;

stop_hnr:
    sample_hnr_stop(vi_pipe, &hnr_cfg, hnr_param);
stop_venc_and_vo:
    sample_hnr_stop_venc_and_vo(vpss_grp, sizeof(vpss_grp) / sizeof(vpss_grp[0]));
stop_vpss:
    sample_hnr_stop_vpss(vpss_grp[0]);
stop_vi:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp[0], 0);
    sample_hnr_stop_vi(vi_pipe, &vi_cfg);
sys_exit:
    sample_comm_sys_exit();
    return ret;
}

static td_void sample_hnr_handle_sig(td_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        g_sig_flag = 1;
    }
}

static td_void sample_register_sig_handler(td_void (*sig_handle)(td_s32))
{
    struct sigaction sa;

    (td_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handle;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, TD_NULL);
    sigaction(SIGTERM, &sa, TD_NULL);
}

static td_s32 sample_hnr_execute_case(td_u32 case_index)
{
    td_s32 ret = TD_FAILURE;
    sample_hnr_param hnr_param;

    switch (case_index) {
        case 0: /* 0 hnr line mode */
            hnr_param.is_wdr_mode = TD_FALSE;
            hnr_param.ref_mode = OT_HNR_REF_MODE_NORM;
            hnr_param.normal_blend = TD_FALSE;
            hnr_param.video_mode = OT_VI_VIDEO_MODE_ADVANCED;
            ret = sample_hnr(&hnr_param);
            break;
        case 1: /* 1 hnr wdr mode */
            hnr_param.is_wdr_mode = TD_TRUE;
            hnr_param.ref_mode = OT_HNR_REF_MODE_NORM;
            hnr_param.normal_blend = TD_FALSE;
            hnr_param.video_mode = OT_VI_VIDEO_MODE_NORM;
            ret = sample_hnr(&hnr_param);
            break;
        case 2: /* 2 hnr line mode, snap */
            hnr_param.is_wdr_mode = TD_FALSE;
            hnr_param.ref_mode = OT_HNR_REF_MODE_NONE;
            hnr_param.normal_blend = TD_FALSE;
            hnr_param.video_mode = OT_VI_VIDEO_MODE_ADVANCED;
            ret = sample_hnr(&hnr_param);
            break;
        case 3: /* 3 hnr line mode, normal blend */
            hnr_param.is_wdr_mode = TD_FALSE;
            hnr_param.ref_mode = OT_HNR_REF_MODE_NORM;
            hnr_param.normal_blend = TD_TRUE;
            hnr_param.video_mode = OT_VI_VIDEO_MODE_NORM;
            ret = sample_hnr(&hnr_param);
            break;
        case 4: /* 4 hnr snap advanced mode, advanced mode of no reference frame */
            hnr_param.is_wdr_mode = TD_FALSE;
            hnr_param.ref_mode = OT_HNR_REF_MODE_NONE_ADVANCED;
            hnr_param.normal_blend = TD_FALSE;
            hnr_param.video_mode = OT_VI_VIDEO_MODE_ADVANCED;
            ret = sample_hnr_snap(&hnr_param);
            break;
        case 5: /* 5 hnr line switch to wdr */
            ret = sample_hnr_line_switch_to_wdr();
            break;

        default:
            break;
    }
    return ret;
}

static td_s32 sample_hnr_msg_proc_vb_pool_share(td_s32 pid)
{
    td_s32 ret;
    td_u32 i;
    td_bool isp_states[OT_VI_MAX_PIPE_NUM];
#ifndef SAMPLE_MEM_SHARE_ENABLE
    ot_vb_common_pools_id pools_id = {0};

    if (ss_mpi_vb_get_common_pool_id(&pools_id) != TD_SUCCESS) {
        sample_print("get common pool_id failed!\n");
        return TD_FAILURE;
    }

    for (i = 0; i < pools_id.pool_cnt; ++i) {
        if (ss_mpi_vb_pool_share(pools_id.pool[i], pid) != TD_SUCCESS) {
            sample_print("vb pool share failed!\n");
            return TD_FAILURE;
        }
    }
#endif
    ret = sample_comm_vi_get_isp_run_state(isp_states, OT_VI_MAX_PIPE_NUM);
    if (ret != TD_SUCCESS) {
        sample_print("get isp states fail\n");
        return TD_FAILURE;
    }

    for (i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        if (!isp_states[i]) {
            continue;
        }
        ret = ss_mpi_isp_mem_share(i, pid);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_isp_mem_share vi_pipe %d, pid %d fail\n", i, pid);
        }
    }

    return TD_SUCCESS;
}

static td_void sample_hnr_msg_proc_vb_pool_unshare(td_s32 pid)
{
    td_s32 ret;
    td_u32 i;
    td_bool isp_states[OT_VI_MAX_PIPE_NUM];
#ifndef SAMPLE_MEM_SHARE_ENABLE
    ot_vb_common_pools_id pools_id = {0};
    if (ss_mpi_vb_get_common_pool_id(&pools_id) == TD_SUCCESS) {
        for (i = 0; i < pools_id.pool_cnt; ++i) {
            ret = ss_mpi_vb_pool_unshare(pools_id.pool[i], pid);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vb_pool_unshare pool %d, pid %d fail\n", pools_id.pool[i], pid);
            }
        }
    }
#endif
    ret = sample_comm_vi_get_isp_run_state(isp_states, OT_VI_MAX_PIPE_NUM);
    if (ret != TD_SUCCESS) {
        sample_print("get isp states fail\n");
        return;
    }

    for (i = 0; i < OT_VI_MAX_PIPE_NUM; i++) {
        if (!isp_states[i]) {
            continue;
        }
        ret = ss_mpi_isp_mem_unshare(i, pid);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_isp_mem_unshare vi_pipe %d, pid %d fail\n", i, pid);
        }
    }
}

static td_s32 sample_hnr_ipc_msg_proc(const sample_ipc_msg_req_buf *msg_req_buf,
    td_bool *is_need_fb, sample_ipc_msg_res_buf *msg_res_buf)
{
    td_s32 ret;

    if (msg_req_buf == TD_NULL || is_need_fb == TD_NULL) {
        return TD_FAILURE;
    }

    /* need feedback default */
    *is_need_fb = TD_TRUE;

    switch ((sample_msg_type)msg_req_buf->msg_type) {
        case SAMPLE_MSG_TYPE_VB_POOL_SHARE_REQ: {
            if (msg_res_buf == TD_NULL) {
                return TD_FAILURE;
            }
            ret = sample_hnr_msg_proc_vb_pool_share(msg_req_buf->msg_data.pid);
            msg_res_buf->msg_type = SAMPLE_MSG_TYPE_VB_POOL_SHARE_RES;
            msg_res_buf->msg_data.is_req_success = (ret == TD_SUCCESS) ? TD_TRUE : TD_FALSE;
            break;
        }
        case SAMPLE_MSG_TYPE_VB_POOL_UNSHARE_REQ: {
            if (msg_res_buf == TD_NULL) {
                return TD_FAILURE;
            }
            sample_hnr_msg_proc_vb_pool_unshare(msg_req_buf->msg_data.pid);
            msg_res_buf->msg_type = SAMPLE_MSG_TYPE_VB_POOL_UNSHARE_RES;
            msg_res_buf->msg_data.is_req_success = TD_TRUE;
            break;
        }
        default: {
            printf("unsupported msg type(%ld)!\n", msg_req_buf->msg_type);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

#ifdef __LITEOS__
td_s32 app_main(td_s32 argc, td_char *argv[])
#else
td_s32 main(td_s32 argc, td_char *argv[])
#endif
{
    td_s32 ret;
    td_u32 index;

    if (argc != 2) { /* 2:arg num */
        sample_hnr_usage(argv[0]);
        return TD_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_hnr_usage(argv[0]);
        return TD_FAILURE;
    }

    if ((strlen(argv[1]) != 1) ||
        (argv[1][0] < '0' || argv[1][0] > '5')) { /* only support case 0/1/2/3/4/5 */
        sample_hnr_usage(argv[0]);
        return TD_FAILURE;
    }

#ifndef __LITEOS__
    sample_register_sig_handler(sample_hnr_handle_sig);
#endif
    if (sample_ipc_server_init(sample_hnr_ipc_msg_proc) != TD_SUCCESS) {
        printf("sample_ipc_server_init failed!!!\n");
    }
#ifdef CONFIG_USER_SPACE
    sample_print("sdk init\n");
    SDK_init();
#endif
    index = atoi(argv[1]);
    ret = sample_hnr_execute_case(index);
    if ((ret == TD_SUCCESS) && (g_sig_flag == 0)) {
        sample_print("\033[0;32mprogram exit normally!\033[0;39m\n");
    } else {
        sample_print("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    sample_ipc_server_deinit();

#ifdef CONFIG_USER_SPACE
    sample_print("sdk exit\n");
    SDK_exit();
#endif
#ifdef __LITEOS__
    return ret;
#else
    exit(ret);
#endif
}

