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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/ioctl.h>

#include "ss_mpi_vgs.h"
#include "ss_mpi_gdc.h"
#include "ot_common_gdc.h"
#include "ot_common_vgs.h"
#include "sample_comm.h"
#include "sdk_module_init.h"

#define X_ALIGN 16
#define Y_ALIGN 2
#define ADDR_ALIGN 16
#define out_ratio_1(x) ((x) / 3)
#define out_ratio_2(x) ((x) * 2 / 3)
#define out_ratio_3(x) ((x) / 2)

volatile static sig_atomic_t g_fisheye_sample_sig_flag = 0;

ot_payload_type g_venc_type = OT_PT_H265;
sample_rc g_rc_mode = SAMPLE_RC_CBR;
ot_size g_size;
ot_pic_size g_pic_size = PIC_1080P;

td_u16 g_lmf_coef[128] = {
    0, 15, 31, 47, 63, 79, 95, 111, 127, 143, 159, 175,
    191, 207, 223, 239, 255, 271, 286, 302, 318, 334, 350, 365, 381, 397, 412,
    428, 443, 459, 474, 490, 505, 520, 536, 551, 566, 581, 596, 611, 626, 641,
    656, 670, 685, 699, 713, 728, 742, 756, 769, 783, 797, 810, 823, 836, 848,
    861, 873, 885, 896, 908, 919, 929, 940, 950, 959, 969, 984, 998, 1013, 1027,
    1042, 1056, 1071, 1085, 1100, 1114, 1129, 1143, 1158, 1172, 1187, 1201, 1215,
    1230, 1244, 1259, 1273, 1288, 1302, 1317, 1331, 1346, 1360, 1375, 1389, 1404,
    1418, 1433, 1447, 1462, 1476, 1491, 1505, 1519, 1534, 1548, 1563, 1577, 1592,
    1606, 1621, 1635, 1650, 1664, 1679, 1693, 1708, 1722, 1737, 1751, 1766, 1780, 1795, 1809, 1823, 1838
};

pthread_t g_thread_id;
td_bool g_set_fisheye_attr = TD_FALSE;

typedef struct {
    ot_vi_pipe vi_pipe;
    ot_vi_chn vi_chn;
} fisheye_set_attr_thread_info;

sample_vo_cfg g_vo_cfg;

/* function : show usage */
static td_void sample_fisheye_usage(char *prg_name)
{
    printf("Usage : %s <index> <vo intf> <venc type>\n", prg_name);
    printf("index:\n");
    printf("\t 0) fisheye 360 panorama 2 half with ceiling mount.\n");
    printf("\t 1) fisheye 360 panorama and 2 normal PTZ with desktop mount.\n");
    printf("\t 2) fisheye 180 panorama and 2 normal dynamic PTZ with wall mount.\n");
    printf("\t 3) fisheye source picture and 3 normal PTZ with wall mount.\n");
    printf("\t 4) nine_lattice preview(Only images larger than or equal to 8M are supported).\n");

    printf("vo intf:\n");
    printf("\t 0) vo HDMI output, default.\n");
    printf("\t 1) vo BT1120 output.\n");

    printf("venc type:\n");
    printf("\t 0) H265, default.\n");
    printf("\t 1) H264.\n");
    return;
}

static td_void sample_fisheye_stop_switch_mode_thread(td_void)
{
    if (g_set_fisheye_attr != TD_FALSE) {
        g_set_fisheye_attr = TD_FALSE;
        pthread_join(g_thread_id, TD_NULL);
    }
}

static td_void sample_fisheye_pause(td_void)
{
    if (g_fisheye_sample_sig_flag == 0) {
        sample_pause();
    }
}

static td_void sample_fisheye_getchar(td_void)
{
    if (g_fisheye_sample_sig_flag == 0) {
        getchar();
    }
}

/* function : to process abnormal case */
static td_void sample_fisheye_handle_sig(td_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        g_fisheye_sample_sig_flag = 1;
    }
}

static td_s32 sample_fisheye_start_venc(ot_venc_chn venc_chn)
{
    td_s32 ret;
    sample_comm_venc_chn_param venc_chnl_param = { 0 };

    ret = sample_comm_venc_get_gop_attr(OT_VENC_GOP_MODE_NORMAL_P, &venc_chnl_param.gop_attr);
    if (ret != TD_SUCCESS) {
        sample_print("sample fisheye get gop attr failed!\n");
        return ret;
    }

    venc_chnl_param.rc_mode = g_rc_mode;
    venc_chnl_param.type = g_venc_type;
    venc_chnl_param.profile = 0;
    venc_chnl_param.size = g_pic_size;
    ret = sample_comm_venc_start(venc_chn, &venc_chnl_param);
    if (ret != TD_SUCCESS) {
        sample_print("sample fisheye start venc failed with %#x!\n", ret);
        return ret;
    }
    return TD_SUCCESS;
}
/*
 * Function:    SAMPLE_VIO_FISHEYE_StartViVo
 * Description: online mode / offline mode. Embedded isp, phychn preview
 */
static td_s32 sample_fisheye_start_vi_vo_venc(sample_vi_cfg *vi_cfg, ot_vi_pipe vi_pipe, ot_vi_chn vi_chn,
    ot_venc_chn venc_chn, ot_vo_chn vo_chn)
{
    td_s32 ret;
    ot_vi_chn_attr chn_attr;
    ot_vo_layer vo_layer = g_vo_cfg.vo_dev;

    if (vi_cfg == TD_NULL) {
        sample_print("vi_cfg is NULL\n");
        return TD_FAILURE;
    }

    /* step 1: start vi dev & chn to capture */
    (td_void)memset_s(&chn_attr, sizeof(ot_vi_chn_attr), 0, sizeof(ot_vi_chn_attr));

    ret = sample_comm_vi_start_vi(vi_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("start vi failed!\n");
        goto exit;
    }

    /* step 2: start VENC */
    ret = sample_fisheye_start_venc(venc_chn);
    if (ret != TD_SUCCESS) {
        sample_print("sample fisheye start venc failed with %#x!\n", ret);
        goto stop_vi;
    }
    /* step 3: start VO */
    ret = sample_comm_vo_start_vo(&g_vo_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("SAMPLE_VIO start VO failed with %#x!\n", ret);
        goto stop_venc;
    }

    /* step 4: Bind Venc */
    ret = sample_comm_vi_bind_venc(vi_pipe, vi_chn, venc_chn);
    if (ret != TD_SUCCESS) {
        sample_print("SAMPLE_VIU_FISHEYE_BindVenc failed with %#x!\n", ret);
        goto stop_vo;
    }

    ret = sample_comm_vi_bind_vo(vi_pipe, vi_chn, vo_layer, vo_chn);
    if (ret != TD_SUCCESS) {
        sample_print("VIO sys bind failed with %#x!\n", ret);
        goto exit;
    }

    return TD_SUCCESS;

stop_vo:
    sample_comm_vo_stop_vo(&g_vo_cfg);
stop_venc:
    sample_comm_venc_stop(venc_chn);
stop_vi:
    sample_comm_vi_stop_vi(vi_cfg);
exit:
    return ret;
}

static td_void sample_fisheye_stop_vi_vo_venc(sample_vi_cfg *vi_cfg, ot_vi_pipe vi_pipe, ot_vi_chn vi_chn,
    ot_vo_chn vo_chn, ot_venc_chn venc_chn)
{
    ot_vo_layer vo_layer = g_vo_cfg.vo_dev;

    sample_comm_vi_un_bind_venc(vi_pipe, vi_chn, venc_chn);
    sample_comm_vi_un_bind_vo(vi_pipe, vi_chn, vo_layer, vo_chn);

    sample_comm_vo_stop_vo(&g_vo_cfg);

    sample_comm_vi_stop_vi(vi_cfg);
}

static td_s32 sample_fisheye_start_vi_vo(sample_vi_cfg *vi_cfg, sample_vo_cfg *vo_cfg)
{
    td_s32 ret;

    ret = sample_comm_vi_start_vi(vi_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("start vi failed!\n");
        return ret;
    }

    ret = sample_comm_vo_start_vo(vo_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("SAMPLE_VIO start VO failed with %#x!\n", ret);
        goto exit;
    }

    return ret;

exit:
    sample_comm_vi_stop_vi(vi_cfg);

    return ret;
}

static td_void sample_fisheye_stop_vi_vo(sample_vi_cfg *vi_cfg, sample_vo_cfg *vo_cfg)
{
    sample_comm_vo_stop_vo(vo_cfg);

    sample_comm_vi_stop_vi(vi_cfg);
}

static td_void *sample_proc_set_fisheye_attr_thread(td_void *arg)
{
    td_s32 i;
    ot_vi_pipe vi_pipe;
    ot_vi_chn vi_chn;
    td_s32 ret;
    ot_fisheye_correction_attr correction_attr;
    fisheye_set_attr_thread_info *thread_info = TD_NULL;

    if (arg == TD_NULL) {
        printf("arg is NULL\n");
        return TD_NULL;
    }

    prctl(PR_SET_NAME, "FISHEYE_Cruise", 0, 0, 0);

    (td_void)memset_s(&correction_attr, sizeof(correction_attr), 0, sizeof(correction_attr));

    thread_info = (fisheye_set_attr_thread_info *)arg;

    vi_pipe = thread_info->vi_pipe;
    vi_chn = thread_info->vi_chn;

    while (g_set_fisheye_attr == TD_TRUE) {
        ret = ss_mpi_vi_get_chn_fisheye(vi_pipe, vi_chn, &correction_attr);
        if (ret != TD_SUCCESS) {
            return TD_NULL;
        }

        for (i = 1; i < 3; i++) { /* 3:set 3 rgn attr */
            if (correction_attr.fisheye_attr.fisheye_rgn_attr[i].pan == 360) { /* 360:pan max value */
                correction_attr.fisheye_attr.fisheye_rgn_attr[i].pan = 0;
            } else {
                correction_attr.fisheye_attr.fisheye_rgn_attr[i].pan++;
            }

            if (correction_attr.fisheye_attr.fisheye_rgn_attr[i].tilt == 360) { /* 360:tilt max value */
                correction_attr.fisheye_attr.fisheye_rgn_attr[i].tilt = 0;
            } else {
                correction_attr.fisheye_attr.fisheye_rgn_attr[i].tilt++;
            }
        }

        ret = ss_mpi_vi_set_chn_fisheye(vi_pipe, vi_chn, &correction_attr);
        if (ret != TD_SUCCESS) {
            return TD_NULL;
        }

        sleep(1);
    }

    return TD_NULL;
}

static td_void sample_fisheye_start_set_fisheye_attr_thread(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn)
{
    fisheye_set_attr_thread_info fisheye_attr_thread_info;

    fisheye_attr_thread_info.vi_pipe = vi_pipe;
    fisheye_attr_thread_info.vi_chn = vi_chn;

    g_set_fisheye_attr = TD_TRUE;

    pthread_create(&g_thread_id, TD_NULL, sample_proc_set_fisheye_attr_thread, &fisheye_attr_thread_info);

    sleep(1);
}

static td_void sample_fisheye_stop_set_fisheye_attr_thread(td_void)
{
    if (g_set_fisheye_attr != TD_FALSE) {
        g_set_fisheye_attr = TD_FALSE;
        pthread_join(g_thread_id, TD_NULL);
    }
}

static td_void sample_fisheye_get_default_rgn_attr(ot_fisheye_rgn_attr *rgn_attr, ot_fisheye_view_mode view_mode)
{
    rgn_attr->view_mode = view_mode;
    rgn_attr->in_radius = 0;
    rgn_attr->out_radius = 1200; /* 1200:out radius value */
    rgn_attr->pan = 180; /* 180:pan value */
    rgn_attr->tilt = 180; /* 180:tilt value */
    rgn_attr->hor_zoom = 4095; /* 4095: default hor_zoom value */
    rgn_attr->ver_zoom = 4095; /* 4095: default ver_zoom value */
    rgn_attr->out_rect.x = 0;
    rgn_attr->out_rect.y = 0;
    rgn_attr->out_rect.width = OT_ALIGN_DOWN(out_ratio_1(g_size.width), X_ALIGN);
    rgn_attr->out_rect.height = OT_ALIGN_DOWN(out_ratio_1(g_size.height), Y_ALIGN);
}

static td_void sample_fisheye_get_default_attr(ot_fisheye_attr *fisheye_attr,
    td_u32 rgn_num, ot_fisheye_view_mode view_mode)
{
    td_u32 i;

    fisheye_attr->lmf_en = TD_FALSE;
    fisheye_attr->bg_color_en = TD_TRUE;
    fisheye_attr->bg_color = COLOR_RGB_BLUE;
    fisheye_attr->hor_offset = 0;
    fisheye_attr->ver_offset = 0;
    fisheye_attr->trapezoid_coef = 0;
    fisheye_attr->fan_strength = 0;
    fisheye_attr->mount_mode = OT_FISHEYE_MOUNT_MODE_WALL;
    fisheye_attr->rgn_num = rgn_num;
    for (i = 0; i < fisheye_attr->rgn_num && i < OT_FISHEYE_MAX_RGN_NUM; i++) {
        sample_fisheye_get_default_rgn_attr(&fisheye_attr->fisheye_rgn_attr[i], view_mode);
    }
}

static td_void sample_fisheye_get_out_rect_attr_1(ot_fisheye_rgn_attr *rgn_attr)
{
    rgn_attr[0].out_rect.x = 0;
    rgn_attr[0].out_rect.y = 0;
    rgn_attr[1].out_rect.x = OT_ALIGN_DOWN(out_ratio_1(g_size.width), X_ALIGN);
    rgn_attr[1].out_rect.y = 0;
    rgn_attr[2].out_rect.x = OT_ALIGN_DOWN(out_ratio_2(g_size.width), X_ALIGN);  /* 2:rgn num */
    rgn_attr[2].out_rect.y = 0;                                                  /* 2:rgn num */
    rgn_attr[3].out_rect.x = 0;                                                  /* 3:rgn num */
    rgn_attr[3].out_rect.y = OT_ALIGN_DOWN(out_ratio_1(g_size.height), Y_ALIGN); /* 3:rgn num */
}

static td_void sample_fisheye_get_out_rect_attr_2(ot_fisheye_rgn_attr *rgn_attr)
{
    rgn_attr[0].out_rect.x = OT_ALIGN_DOWN(out_ratio_1(g_size.width), X_ALIGN);
    rgn_attr[0].out_rect.y = OT_ALIGN_DOWN(out_ratio_1(g_size.height), Y_ALIGN);
    rgn_attr[1].out_rect.x = OT_ALIGN_DOWN(out_ratio_2(g_size.width), X_ALIGN);
    rgn_attr[1].out_rect.y = OT_ALIGN_DOWN(out_ratio_1(g_size.height), Y_ALIGN);
    rgn_attr[2].out_rect.x = 0;                                                  /* 2:rgn num */
    rgn_attr[2].out_rect.y = OT_ALIGN_DOWN(out_ratio_2(g_size.height), Y_ALIGN); /* 2:rgn num */
    rgn_attr[3].out_rect.x = OT_ALIGN_DOWN(out_ratio_1(g_size.width), X_ALIGN);  /* 3:rgn num */
    rgn_attr[3].out_rect.y = OT_ALIGN_DOWN(out_ratio_2(g_size.height), Y_ALIGN); /* 3:rgn num */
}

static td_void sample_fisheye_get_buff_attr(ot_pic_buf_attr *buf_attr)
{
    buf_attr->width = g_size.width;
    buf_attr->height = g_size.height;
    buf_attr->align = 0;
    buf_attr->bit_width = OT_DATA_BIT_WIDTH_8;
    buf_attr->pixel_format = SAMPLE_PIXEL_FORMAT;
    buf_attr->compress_mode = OT_COMPRESS_MODE_NONE;
}

static td_s32 sample_fisheye_set_vb(td_u64 *out_phys_addr,
    td_u8 **out_virt_addr, ot_vb_blk *vb_out_blk, td_u32 *buf_size)
{
    td_char *mmz_name = TD_NULL;
    ot_pic_buf_attr buf_attr;

    sample_fisheye_get_buff_attr(&buf_attr);
    *buf_size = ot_common_get_pic_buf_size(&buf_attr);

    *vb_out_blk = ss_mpi_vb_get_blk(OT_VB_INVALID_POOL_ID, *buf_size, mmz_name);
    if (*vb_out_blk == OT_VB_INVALID_HANDLE) {
        sample_print("Info:mpi_vb_get_blk(size:%u) fail\n", *buf_size);
        return TD_FAILURE;
    }

    *out_phys_addr = ss_mpi_vb_handle_to_phys_addr(*vb_out_blk);
    if (*out_phys_addr == 0) {
        sample_print("Info:mpi_vb_handle_to_phys_addr fail, u32OutPhyAddr:0x%llx\n", *out_phys_addr);
        ss_mpi_vb_release_blk(*vb_out_blk);
        return TD_FAILURE;
    }

    *out_virt_addr = (td_u8 *)ss_mpi_sys_mmap(*out_phys_addr, *buf_size);
    if (*out_virt_addr == TD_NULL) {
        sample_print("Info:mpi_sys_mmap fail!\n");
        ss_mpi_vb_release_blk(*vb_out_blk);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void sample_fisheye_set_task(ot_gdc_task_attr *task, const td_u64 *out_phys_addr,
    const td_u8 *out_virt_addr, const ot_vb_blk *vb_out_blk)
{
    td_u32 out_width;
    td_u32 out_height;
    td_u32 out_stride;

    out_width = g_size.width;
    out_height = g_size.height;
    out_stride = OT_ALIGN_UP(out_width, ADDR_ALIGN);

    (td_void)memcpy_s(&task->img_out, sizeof(ot_video_frame_info), &task->img_in, sizeof(ot_video_frame_info));

    task->img_out.pool_id = ss_mpi_vb_handle_to_pool_id(*vb_out_blk);
    task->img_out.video_frame.phys_addr[0] = *out_phys_addr;
    task->img_out.video_frame.phys_addr[1] = *out_phys_addr + out_stride * out_height;
    task->img_out.video_frame.virt_addr[0] = (td_void *)out_virt_addr;
    task->img_out.video_frame.virt_addr[1] = (td_void *)out_virt_addr + out_stride * out_height;
    task->img_out.video_frame.stride[0] = out_stride;
    task->img_out.video_frame.stride[1] = out_stride;
    task->img_out.video_frame.width = out_width;
    task->img_out.video_frame.height = out_height;
}

static void sample_fisheye_set_vgs_task(ot_vgs_task_attr *vgs_task, const td_u64 *out_phys_addr,
    const td_u8 *out_virt_addr, const ot_vb_blk *vb_out_blk)
{
    td_u32 out_width;
    td_u32 out_height;
    td_u32 out_stride;

    out_width = g_size.width;
    out_height = g_size.height;
    out_stride = OT_ALIGN_UP(g_size.width, ADDR_ALIGN);

    vgs_task->img_out.pool_id = ss_mpi_vb_handle_to_pool_id(*vb_out_blk);
    vgs_task->img_out.video_frame.width = OT_ALIGN_DOWN(out_ratio_1(out_width), X_ALIGN);
    vgs_task->img_out.video_frame.height = OT_ALIGN_DOWN(out_ratio_1(out_height), Y_ALIGN);
    vgs_task->img_out.video_frame.field = OT_VIDEO_FIELD_FRAME;
    vgs_task->img_out.video_frame.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vgs_task->img_out.video_frame.stride[0] = out_stride;
    vgs_task->img_out.video_frame.stride[1] = out_stride;

    vgs_task->img_out.video_frame.phys_addr[0] = *out_phys_addr + (td_u64)out_stride * out_ratio_2(out_height) +
        OT_ALIGN_DOWN(out_ratio_2(out_width), X_ALIGN);
    vgs_task->img_out.video_frame.phys_addr[1] = *out_phys_addr + (td_u64)out_stride * out_height +
        ((td_u64)out_stride * out_ratio_1(out_height)) + OT_ALIGN_DOWN(out_ratio_2(out_width), X_ALIGN);
    vgs_task->img_out.video_frame.virt_addr[0] = (td_void *)out_virt_addr + (td_u64)out_stride *
        out_ratio_2(out_height) + OT_ALIGN_DOWN(out_ratio_2(out_width), X_ALIGN);
    vgs_task->img_out.video_frame.virt_addr[1] = (td_void *)out_virt_addr + (td_u64)out_stride * out_height +
        ((td_u64)out_stride * out_ratio_1(out_height)) + OT_ALIGN_DOWN(out_ratio_2(out_width), X_ALIGN);
}

static td_s32 sample_fisheye_nine_lattice_add_gdc(const ot_gdc_task_attr *task, ot_gdc_handle handle)
{
    ot_fisheye_attr fisheye_attr;
    td_s32 ret;

    sample_fisheye_get_default_attr(&fisheye_attr, OT_FISHEYE_MAX_RGN_NUM, OT_FISHEYE_VIEW_MODE_180_PANORAMA);
    fisheye_attr.bg_color_en = TD_FALSE;

    sample_fisheye_get_out_rect_attr_1(fisheye_attr.fisheye_rgn_attr);

    ret = ss_mpi_gdc_add_correction_task(handle, task, &fisheye_attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    sample_fisheye_get_out_rect_attr_2(fisheye_attr.fisheye_rgn_attr);

    ret = ss_mpi_gdc_add_correction_task(handle, task, &fisheye_attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = ss_mpi_gdc_end_job(handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 sample_fisheye_nine_lattice_add_vgs(const ot_gdc_task_attr *task, const td_u64 *out_phys_addr,
    const td_u8 *out_virt_addr, const ot_vb_blk *vb_out_blk)
{
    ot_vgs_handle vgs_handle;
    ot_vgs_task_attr vgs_task;
    td_s32 ret;

    ret = ss_mpi_vgs_begin_job(&vgs_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memcpy_s(&vgs_task.img_in, sizeof(ot_video_frame_info), &task->img_in, sizeof(ot_video_frame_info));
    (td_void)memcpy_s(&vgs_task.img_out, sizeof(ot_video_frame_info), &task->img_in, sizeof(ot_video_frame_info));

    sample_fisheye_set_vgs_task(&vgs_task, out_phys_addr, out_virt_addr, vb_out_blk);
    ret = ss_mpi_vgs_add_scale_task(vgs_handle, &vgs_task, OT_VGS_SCALE_COEF_NORM);
    if (ret != TD_SUCCESS) {
        ss_mpi_vgs_cancel_job(vgs_handle);
        return ret;
    }

    ret = ss_mpi_vgs_end_job(vgs_handle);
    if (ret != TD_SUCCESS) {
        ss_mpi_vgs_cancel_job(vgs_handle);
        return ret;
    }

    return TD_SUCCESS;
}

static td_void sample_fisheye_nine_lattice_thread_set_vi_chn_attr(ot_vi_chn_attr *chn_attr, td_u32 *old_depth,
    td_u32 depth)
{
    td_s32 ret;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;

    ret = ss_mpi_vi_get_chn_attr(vi_pipe, vi_chn, chn_attr);
    if (ret != TD_SUCCESS) {
        sample_print("Err, ret:0x%x\n", ret);
    }

    *old_depth = chn_attr->depth;
    chn_attr->depth = depth;
    ret = ss_mpi_vi_set_chn_attr(vi_pipe, vi_chn, chn_attr);
    if (ret != TD_SUCCESS) {
        sample_print("Err, ret:0x%x\n", ret);
    }
}

static td_void sample_fisheye_nine_lattice_thread_set_vi_chn_attr_1(ot_vi_chn_attr *chn_attr, td_u32 old_depth)
{
    td_s32 ret;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;

    chn_attr->depth = old_depth;
    ret = ss_mpi_vi_set_chn_attr(vi_pipe, vi_chn, chn_attr);
    if (ret != TD_SUCCESS) {
        sample_print("Err, ret:0x%x\n", ret);
    }
}

static td_s32 sample_fisheye_nine_lattice_thread_frame(td_u32 *buf_size, ot_gdc_task_attr *task,
    td_u8 **out_virt_addr, ot_vb_blk *vb_out_blk)
{
    td_s32 ret;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    const td_s32 milli_sec = -1;
    td_u64 out_phys_addr;
    ot_gdc_handle handle;

    ret = ss_mpi_gdc_begin_job(&handle);
    if (ret != TD_SUCCESS) {
        sample_print("Err, ret:0x%x\n", ret);
        return TD_NULL;
    }
    ret = sample_fisheye_set_vb(&out_phys_addr, out_virt_addr, vb_out_blk, buf_size);
    if (ret != TD_SUCCESS) {
        ss_mpi_gdc_cancel_job(handle);
        return TD_NULL;
    }

    ret = ss_mpi_vi_get_chn_frame(vi_pipe, vi_chn, &task->img_in, milli_sec);
    if (ret != TD_SUCCESS) {
        sample_print("Err, ret:0x%x\n", ret);
        goto gdc_out;
    }

    sample_fisheye_set_task(task, &out_phys_addr, *out_virt_addr, vb_out_blk);
    ret = sample_fisheye_nine_lattice_add_gdc(task, handle);
    if (ret != TD_SUCCESS) {
        sample_print("Err, ret:0x%x\n", ret);
        goto vi_out;
    }
    ret = sample_fisheye_nine_lattice_add_vgs(task, &out_phys_addr, *out_virt_addr, vb_out_blk);
    if (ret != TD_SUCCESS) {
        sample_print("Err, ret:0x%x\n", ret);
        goto vi_out;
    }
    return TD_SUCCESS;
vi_out:
    ss_mpi_vi_release_chn_frame(vi_pipe, vi_chn, &task->img_in);
gdc_out:
    ss_mpi_gdc_cancel_job(handle);
    ss_mpi_sys_munmap(*out_virt_addr, *buf_size);
    ss_mpi_vb_release_blk(*vb_out_blk);
    return TD_FAILURE;
}

static td_s32 sample_fisheye_nine_lattice_thread_out(td_u32 buf_size, const ot_gdc_task_attr *task,
    const td_u8 *out_virt_addr, ot_vb_blk vb_out_blk)
{
    td_s32 ret;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;

    ret = ss_mpi_vi_release_chn_frame(vi_pipe, vi_chn, &task->img_in);
    if (ret != TD_SUCCESS) {
        printf("Info:mpi_vi_release_chn_frame fail, ret:0x%x\n", ret);
        ss_mpi_sys_munmap(out_virt_addr, buf_size);
        ss_mpi_vb_release_blk(vb_out_blk);
        return TD_FAILURE;
    }
    ret = ss_mpi_sys_munmap(out_virt_addr, buf_size);
    if (ret != TD_SUCCESS) {
        printf("Info:mpi_sys_munmap fail,ret:0x%x\n", ret);
        ss_mpi_vb_release_blk(vb_out_blk);
        return TD_FAILURE;
    }

    ret = ss_mpi_vb_release_blk(vb_out_blk);
    if (ret != TD_SUCCESS) {
        printf("Info:mpi_vb_release_blk fail,ret:0x%x\n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void *sample_fisheye_nine_lattice_thread(td_void *arg)
{
    td_s32 ret;
    ot_gdc_task_attr task;
    ot_vi_chn_attr chn_attr = { 0 };
    td_u32 buf_size = 0;
    td_u8 *out_virt_addr = TD_NULL;
    ot_vb_blk vb_out_blk = 0;
    td_u32 old_depth;
    const td_u32 depth = 2; /* 2:default depth */
    ot_vo_layer vo_layer = g_vo_cfg.vo_dev;
    const ot_vo_chn vo_chn = 0;
    const td_s32 milli_sec = -1;

    if (arg == TD_NULL) {
        sample_print("arg is NULL\n");
        return TD_NULL;
    }

    prctl(PR_SET_NAME, "FISHEYE_Frame", 0, 0, 0);

    sample_fisheye_nine_lattice_thread_set_vi_chn_attr(&chn_attr, &old_depth, depth);

    while (g_set_fisheye_attr == TD_TRUE) {
        ret = sample_fisheye_nine_lattice_thread_frame(&buf_size, &task, &out_virt_addr, &vb_out_blk);
        if (ret != TD_SUCCESS) {
            return TD_NULL;
        }

        ret = ss_mpi_vo_send_frame(vo_layer, vo_chn, &task.img_out, milli_sec);
        if (ret != TD_SUCCESS) {
            sample_print("mpi_vo_send_frame fail, ret:0x%x\n", ret);
        }

        ret = sample_fisheye_nine_lattice_thread_out(buf_size, &task, out_virt_addr, vb_out_blk);
        if (ret != TD_SUCCESS) {
            return TD_NULL;
        }
        usleep(20000); /* 20000:delay times */
    }

    sample_fisheye_nine_lattice_thread_set_vi_chn_attr_1(&chn_attr, old_depth);
    return TD_NULL;
}

#ifdef SAMPLE_MEM_SHARE_ENABLE
static td_void sample_init_mem_share(td_void)
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

static td_s32 sample_fisheye_sys_init()
{
    td_s32 ret;
    ot_pic_buf_attr buf_attr;
    td_u64 blk_size;
    ot_vb_cfg vb_cfg;

    /* step  1: mpp system init */
    (td_void)memset_s(&vb_cfg, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg));
    vb_cfg.max_pool_cnt = 128; /* 128:pool cnt num */

    /* comm video buffer */
    sample_fisheye_get_buff_attr(&buf_attr);
    blk_size = ot_common_get_pic_buf_size(&buf_attr);

    vb_cfg.common_pool[0].blk_size = blk_size;
    vb_cfg.common_pool[0].blk_cnt = 15; /* 15:blk cnt 0 num */

    /* vb for vi raw */
    buf_attr.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_422;
    blk_size = ot_common_get_pic_buf_size(&buf_attr);

    vb_cfg.common_pool[1].blk_size = blk_size;
    vb_cfg.common_pool[1].blk_cnt = 4; /* 4:blk cnt 1 num */

    ret = sample_comm_sys_init(&vb_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("system init failed with %d!\n", ret);
        sample_comm_sys_exit();
        return ret;
    }
#ifdef SAMPLE_MEM_SHARE_ENABLE
    sample_init_mem_share();
#endif
    return TD_SUCCESS;
}

static td_void sample_fisheye_get_360_correction_attr(ot_fisheye_correction_attr *correction_attr)
{
    td_u32 width = g_size.width;
    td_u32 height = g_size.height;

    correction_attr->enable = TD_TRUE;
    correction_attr->dst_size.width = width;
    correction_attr->dst_size.height = height;
    /* 2:rgn_num */
    sample_fisheye_get_default_attr(&correction_attr->fisheye_attr, 2, OT_FISHEYE_VIEW_MODE_360_PANORAMA);
    correction_attr->fisheye_attr.lmf_en = TD_TRUE;
    correction_attr->fisheye_attr.mount_mode = OT_FISHEYE_MOUNT_MODE_CEILING;

    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.x = 0;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.y = 0;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.width = width;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.height = OT_ALIGN_DOWN(out_ratio_1(height), Y_ALIGN);

    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.x = 0;
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.y = OT_ALIGN_DOWN(out_ratio_1(height), Y_ALIGN);
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.width = width;
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.height = OT_ALIGN_DOWN(out_ratio_1(height), Y_ALIGN);
}

static td_s32 sample_fisheye_360_pannorama_set_chn_fisheye(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn,
    ot_fisheye_correction_attr *correction_attr)
{
    td_s32 ret;

    ret = ss_mpi_vi_set_chn_fisheye(vi_pipe, vi_chn, correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set fisheye attr failed with ret:0x%x!\n", ret);
        return ret;
    }

    printf("\nplease press enter, disable fisheye\n\n");
    sample_fisheye_getchar();

    correction_attr->enable = TD_FALSE;
    ret = ss_mpi_vi_set_chn_fisheye(vi_pipe, vi_chn, correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set fisheye attr failed with ret:0x%x!\n", ret);
        return ret;
    }

    printf("\nplease press enter, enable fisheye\n");
    sample_fisheye_getchar();

    correction_attr->enable = TD_TRUE;
    ret = ss_mpi_vi_set_chn_fisheye(vi_pipe, vi_chn, correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set fisheye attr failed with ret:0x%x!\n", ret);
        return ret;
    }
    return ret;
}

/* function : vi/vpss: offline/online fisheye mode VI-VO. Embedded isp, phychn channel preview. */
static td_s32 sample_fisheye_360_panorama_celing_2_half(sample_vi_cfg *vi_cfg, ot_vi_pipe vi_pipe)
{
    const ot_vi_chn vi_chn = 0;
    ot_venc_chn venc_chn = 0;
    const ot_vo_chn vo_chn = 0;
    const td_s32 chn_num = 1;
    td_s32 ret;
    ot_fisheye_cfg fisheye_cfg;
    ot_fisheye_correction_attr correction_attr;

    ret = sample_fisheye_sys_init();
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /* step  1: start VI VO  VENC */
    ret = sample_fisheye_start_vi_vo_venc(vi_cfg, vi_pipe, vi_chn, venc_chn, vo_chn);
    if (ret != TD_SUCCESS) {
        sample_print("SAMPLE_VIO_FISHEYE_StartViVo failed witfh %d\n", ret);
        goto exit;
    }

    /* step   2: stream venc process -- get stream, then save it to file. */
    ret = sample_comm_venc_start_get_stream(&venc_chn, chn_num);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_venc_start_get_stream failed with %d\n", ret);
        goto stop_vi_vo_venc;
    }

    /* step  3: set fisheye Attr */
    (td_void) memcpy_s(fisheye_cfg.lmf_coef, sizeof(g_lmf_coef), g_lmf_coef, sizeof(g_lmf_coef));
    sample_fisheye_get_360_correction_attr(&correction_attr);

    printf("\nplease press enter, enable fisheye\n\n");
    sample_fisheye_getchar();
    ret = ss_mpi_vi_set_pipe_fisheye_cfg(vi_pipe, &fisheye_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("set fisheye config failed with ret:0x%x!\n", ret);
        goto stop_get_stream;
    }
    ret = sample_fisheye_360_pannorama_set_chn_fisheye(vi_pipe, vi_chn, &correction_attr);
    if (ret != TD_SUCCESS) {
        goto stop_get_stream;
    }
    
    sample_fisheye_pause();

stop_get_stream:
    sample_comm_venc_stop_get_stream(chn_num);
stop_vi_vo_venc:
    sample_fisheye_stop_vi_vo_venc(vi_cfg, vi_pipe, vi_chn, venc_chn, vo_chn);
exit:
    sample_comm_sys_exit();
    return ret;
}

static td_void sample_fisheye_get_360_and_2_normal_correction_attr(ot_fisheye_correction_attr *correction_attr)
{
    td_u32 width = g_size.width;
    td_u32 height = g_size.height;

    correction_attr->enable = TD_TRUE;
    correction_attr->dst_size.width = width;
    correction_attr->dst_size.height = height;

    /* 3:rgn num */
    sample_fisheye_get_default_attr(&correction_attr->fisheye_attr, 3, OT_FISHEYE_VIEW_MODE_NORM);

    correction_attr->fisheye_attr.mount_mode = OT_FISHEYE_MOUNT_MODE_DESKTOP;

    correction_attr->fisheye_attr.fisheye_rgn_attr[0].view_mode = OT_FISHEYE_VIEW_MODE_360_PANORAMA;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.width = width;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.height = OT_ALIGN_DOWN(out_ratio_1(height), Y_ALIGN);

    correction_attr->fisheye_attr.fisheye_rgn_attr[1].pan = 0;
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].tilt = 90; /* 90:tilt value */
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].hor_zoom = 2048; /* 2048:hor zoom value */
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.y = OT_ALIGN_DOWN(out_ratio_1(height), Y_ALIGN);
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.width = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.height = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);

    correction_attr->fisheye_attr.fisheye_rgn_attr[2].tilt = 270; /* 2:rgn num; 270:tilt value */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].hor_zoom = 2048; /* 2:rgn num; 2048:hor zoom */
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.x = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.y = OT_ALIGN_DOWN(out_ratio_1(height), Y_ALIGN);
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.width = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.height = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);
}
/* function : vi/vpss: offline/online fisheye mode VI-VO. Embedded isp, phychn channel preview. */
static td_s32 sample_fisheye_360_panorama_desktop_and_2_normal(sample_vi_cfg *vi_cfg, ot_vi_pipe vi_pipe)
{
    const ot_vi_chn vi_chn = 0;
    const ot_vo_chn vo_chn = 0;
    ot_venc_chn venc_chn = 0;
    const td_s32 chn_num = 1;
    td_s32 ret;
    ot_fisheye_correction_attr correction_attr;

    ret = sample_fisheye_sys_init();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* step  1: start VI VO  VENC */
    ret = sample_fisheye_start_vi_vo_venc(vi_cfg, vi_pipe, vi_chn, venc_chn, vo_chn);
    if (ret != TD_SUCCESS) {
        sample_print("SAMPLE_VIO_FISHEYE_StartViVo failed witfh %d\n", ret);
        goto exit;
    }

    /* step   2: stream venc process -- get stream, then save it to file. */
    ret = sample_comm_venc_start_get_stream(&venc_chn, chn_num);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_venc_start_get_stream failed witfh %d\n", ret);
        goto stop_vi_vo_venc;
    }

    /* step  3: set fisheye Attr */
    sample_fisheye_get_360_and_2_normal_correction_attr(&correction_attr);

    ret = sample_fisheye_360_pannorama_set_chn_fisheye(vi_pipe, vi_chn, &correction_attr);
    if (ret != TD_SUCCESS) {
        goto stop_get_stream;
    }

    sample_fisheye_pause();

stop_get_stream:
    sample_comm_venc_stop_get_stream(chn_num);

stop_vi_vo_venc:
    sample_fisheye_stop_vi_vo_venc(vi_cfg, vi_pipe, vi_chn, venc_chn, vo_chn);

exit:
    sample_comm_sys_exit();
    return ret;
}

static td_void sample_fisheye_get_180_and_2_normal_correction_attr(ot_fisheye_correction_attr *correction_attr)
{
    td_u32 width = g_size.width;
    td_u32 height = g_size.height;

    correction_attr->enable = TD_TRUE;
    correction_attr->dst_size.width = width;
    correction_attr->dst_size.height = height;

    /* 3:rgn num */
    sample_fisheye_get_default_attr(&correction_attr->fisheye_attr, 3, OT_FISHEYE_VIEW_MODE_NORM);
    correction_attr->fisheye_attr.trapezoid_coef = 10; /* 10:default value */
    correction_attr->fisheye_attr.fan_strength = 300; /* 300:default value */

    correction_attr->fisheye_attr.fisheye_rgn_attr[0].view_mode = OT_FISHEYE_VIEW_MODE_180_PANORAMA;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.width = width;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.height = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);

    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.y = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.width = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.height = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);

    correction_attr->fisheye_attr.fisheye_rgn_attr[2].pan = 200; /* 2:rgn num; 200:pan value */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].tilt = 200; /* 2:rgn num; 200:tilt value */
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.x = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.y = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.width = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.height = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);
}
/* function : vi/vpss: offline/online fisheye mode VI-VO. Embedded isp, phychn channel preview. */
static td_s32 sample_fisheye_180_panorama_wall_and_2_dynamic_normal(sample_vi_cfg *vi_cfg, ot_vi_pipe vi_pipe)
{
    const ot_vi_chn vi_chn = 0;
    ot_venc_chn venc_chn = 0;
    const ot_vo_chn vo_chn = 0;
    const td_s32 chn_num = 1;
    td_s32 ret;
    ot_fisheye_correction_attr correction_attr;

    ret = sample_fisheye_sys_init();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* step  1: start VI VO  VENC */
    ret = sample_fisheye_start_vi_vo_venc(vi_cfg, vi_pipe, vi_chn, venc_chn, vo_chn);
    if (ret != TD_SUCCESS) {
        sample_print("SAMPLE_VIO_FISHEYE_StartViVo failed witfh %d\n", ret);
        goto exit;
    }
    /* step   2: stream venc process -- get stream, then save it to file. */
    ret = sample_comm_venc_start_get_stream(&venc_chn, chn_num);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_venc_start_get_stream failed witfh %d\n", ret);
        goto stop_vi_vo_venc;
    }
    /* step  3: set fisheye Attr */
    sample_fisheye_get_180_and_2_normal_correction_attr(&correction_attr);

    ret = ss_mpi_vi_set_chn_fisheye(vi_pipe, vi_chn, &correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set fisheye attr failed with ret:0x%x!\n", ret);
        goto stop_get_stream;
    }
    /* create a pthread to change the fisheye attr */
    sample_fisheye_start_set_fisheye_attr_thread(vi_pipe, vi_chn);
    sample_fisheye_pause();
    sample_fisheye_stop_set_fisheye_attr_thread();

stop_get_stream:
    sample_comm_venc_stop_get_stream(chn_num);
stop_vi_vo_venc:
    sample_fisheye_stop_vi_vo_venc(vi_cfg, vi_pipe, vi_chn, venc_chn, vo_chn);
exit:
    sample_comm_sys_exit();
    return ret;
}

static td_void sample_fisheye_get_source_and_3_normal_correction_attr(ot_fisheye_correction_attr *correction_attr)
{
    td_u32 width = g_size.width;
    td_u32 height = g_size.height;

    correction_attr->enable = TD_TRUE;
    correction_attr->dst_size.width = width;
    correction_attr->dst_size.height = height;

    /* 4:rgn num */
    sample_fisheye_get_default_attr(&correction_attr->fisheye_attr, 4, OT_FISHEYE_VIEW_MODE_NORM);
    correction_attr->fisheye_attr.trapezoid_coef = 10; /* 10:trapezoid_coef strength */

    correction_attr->fisheye_attr.fisheye_rgn_attr[0].hor_zoom = 2048; /* 2048:hor zoom value */
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].view_mode = OT_FISHEYE_VIEW_MODE_NO_TRANS;
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.width =  OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    correction_attr->fisheye_attr.fisheye_rgn_attr[0].out_rect.height = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);

    correction_attr->fisheye_attr.fisheye_rgn_attr[1].tilt = 135; /* 135:tilt value */
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].hor_zoom = 2048; /* 2048:hor zoom value */
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.x = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.width = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    correction_attr->fisheye_attr.fisheye_rgn_attr[1].out_rect.height = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);

    correction_attr->fisheye_attr.fisheye_rgn_attr[2].pan = 135; /* 2:rgn num; 135:pan value */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].hor_zoom = 2048; /* 2:rgn num; 2048: hor zoom value */
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.y = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.width = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    /* 2:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[2].out_rect.height = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);

    correction_attr->fisheye_attr.fisheye_rgn_attr[3].pan = 215; /* 3:rgn num; 215:pan value */
    correction_attr->fisheye_attr.fisheye_rgn_attr[3].hor_zoom = 2048; /* 3:rgn num; 2048: hor zoom value */
    /* 3:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[3].out_rect.x = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    /* 3:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[3].out_rect.y = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);
    /* 3:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[3].out_rect.width = OT_ALIGN_DOWN(out_ratio_3(width), X_ALIGN);
    /* 3:rgn num */
    correction_attr->fisheye_attr.fisheye_rgn_attr[3].out_rect.height = OT_ALIGN_DOWN(out_ratio_3(height), Y_ALIGN);
}

static td_s32 sample_fisheye_source_and_3_normal_switch(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn,
    ot_fisheye_correction_attr *correction_attr)
{
    td_s32 ret;

    printf("\nplease press enter, disable fisheye\n\n");
    sample_fisheye_getchar();

    correction_attr->enable = TD_FALSE;
    ret = ss_mpi_vi_set_chn_fisheye(vi_pipe, vi_chn, correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set fisheye attr failed with ret:0x%x!\n", ret);
        return ret;
    }
    printf("\nplease press enter, enable fisheye\n");
    sample_fisheye_getchar();

    correction_attr->enable = TD_TRUE;
    ret = ss_mpi_vi_set_chn_fisheye(vi_pipe, vi_chn, correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set fisheye attr failed with ret:0x%x!\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

/* function : vi/vpss: offline/online fisheye mode VI-VO. Embedded isp, phychn channel preview. */
static td_s32 sample_fisheye_source_and_3_normal(sample_vi_cfg *vi_cfg, ot_vi_pipe vi_pipe)
{
    const ot_vi_chn vi_chn = 0;
    ot_venc_chn venc_chn = 0;
    const ot_vo_chn vo_chn = 0;
    const td_s32 chn_num = 1;
    td_s32 ret;
    ot_fisheye_correction_attr correction_attr;

    /* mpp system init */
    ret = sample_fisheye_sys_init();
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /* step  1: start VI VO  VENC */
    ret = sample_fisheye_start_vi_vo_venc(vi_cfg, vi_pipe, vi_chn, venc_chn, vo_chn);
    if (ret != TD_SUCCESS) {
        sample_print("SAMPLE_VIO_FISHEYE_StartViVo failed witfh %d\n", ret);
        goto exit;
    }
    /* step   2: stream venc process -- get stream, then save it to file. */
    ret = sample_comm_venc_start_get_stream(&venc_chn, chn_num);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_venc_start_get_stream failed witfh %d\n", ret);
        goto stop_vi_vo_venc;
    }
    /* step  3: set fisheye Attr */
    sample_fisheye_get_source_and_3_normal_correction_attr(&correction_attr);

    ret = ss_mpi_vi_set_chn_fisheye(vi_pipe, vi_chn, &correction_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set fisheye attr failed with ret:0x%x!\n", ret);
        goto stop_get_stream;
    }

    ret = sample_fisheye_source_and_3_normal_switch(vi_pipe, vi_chn, &correction_attr);
    if (ret != TD_SUCCESS) {
        goto stop_get_stream;
    }

    sample_fisheye_pause();

stop_get_stream:
    sample_comm_venc_stop_get_stream(chn_num);
stop_vi_vo_venc:
    sample_fisheye_stop_vi_vo_venc(vi_cfg, vi_pipe, vi_chn, venc_chn, vo_chn);
exit:
    sample_comm_sys_exit();
    return ret;
}

/* function : vi/: online fisheye mode VI-VO. Embedded isp, phychn channel preview. */
static td_s32 sample_fisheye_nine_lattice(sample_vi_cfg *vi_cfg)
{
    td_s32 ret;

    if (vi_cfg == TD_NULL) {
        sample_print("vi_cfg is NULL!\n");
        return TD_FAILURE;
    }

    /* step    1: mpp system init */
    ret = sample_fisheye_sys_init();
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /* step    2: start vi vo */
    sample_fisheye_start_vi_vo(vi_cfg, &g_vo_cfg);

    /* step    3: start a thread */
    g_set_fisheye_attr = TD_TRUE;
    pthread_create(&g_thread_id, TD_NULL, sample_fisheye_nine_lattice_thread, &g_size);

    sample_fisheye_pause();

    sample_fisheye_stop_switch_mode_thread();

    sample_fisheye_stop_vi_vo(vi_cfg, &g_vo_cfg);
    sample_comm_sys_exit();
    return ret;
}

static td_s32 sample_fisheye_start(char *argv[])
{
    td_s32 ret;
    sample_vi_cfg vi_cfg;
    const ot_vi_pipe vi_pipe = 0;

    sample_comm_vi_get_default_vi_cfg(SENSOR0_TYPE, &vi_cfg);
    /* step1:  Get  input size */
    sample_comm_vi_get_size_by_sns_type(vi_cfg.sns_info.sns_type, &g_size);

    switch (*argv[1]) {
        /* VI/VPSS - VO. Embedded isp, phychn channel preview. */
        case '0':
            ret = sample_fisheye_360_panorama_celing_2_half(&vi_cfg, vi_pipe);
            break;

        case '1':
            ret = sample_fisheye_360_panorama_desktop_and_2_normal(&vi_cfg, vi_pipe);
            break;

        case '2':
            ret = sample_fisheye_180_panorama_wall_and_2_dynamic_normal(&vi_cfg, vi_pipe);
            break;

        case '3':
            ret = sample_fisheye_source_and_3_normal(&vi_cfg, vi_pipe);
            break;

        case '4':
            ret = sample_fisheye_nine_lattice(&vi_cfg);
            break;

        default:
            sample_print("the index is invalid!\n");
            sample_fisheye_usage(argv[0]);
            return TD_FAILURE;
    }
    return ret;
}

static td_s32 sample_fisheye_parse_venc_type(int argc, char *argv[])
{
    if (argc <= 3) { /* 3:arg num */
        return TD_SUCCESS;
    }
    if ((strlen(argv[3]) != 1)) { /* 3 intf */
        return TD_FAILURE;
    }
    switch (*argv[3]) { /* 3:array index  */
        case '0':
            break;
        case '1':
            g_venc_type = OT_PT_H264;
            break;
        default:
            return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_fisheye_parse_vo_intf(int argc, char *argv[])
{
    sample_comm_vo_get_def_config(&g_vo_cfg);
    g_vo_cfg.vo_intf_type = OT_VO_INTF_HDMI;
    if (argc <= 2) { /* 2:arg num */
        return TD_SUCCESS;
    }
    if ((strlen(argv[2]) != 1)) { /* 2 intf */
        return TD_FAILURE;
    }
    switch (*argv[2]) { /* 2:array index */
        case '0':
            g_vo_cfg.vo_intf_type = OT_VO_INTF_HDMI;
            break;
        case '1':
            g_vo_cfg.vo_intf_type = OT_VO_INTF_BT1120;
            break;
        default:
            return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_fisheye_prepare_run(int argc, char *argv[])
{
#ifndef __LITEOS__
    sample_sys_signal(&sample_fisheye_handle_sig);
#endif

    if (sample_fisheye_parse_venc_type(argc, argv) != TD_SUCCESS) {
        return TD_FAILURE;
    }
#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif

    return sample_fisheye_parse_vo_intf(argc, argv);
}

static void sample_fisheye_finish_run(td_s32 ret)
{
    if (ret == TD_SUCCESS && g_fisheye_sample_sig_flag == 0) {
        sample_print("program exit normally!\n");
    } else {
        sample_print("program exit abnormally!\n");
    }

#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif
}

/*
 * function    : main()
 * Description : video fisheye preview sample
 */
#ifdef __LITEOS__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    td_s32 ret;
    
#ifdef DRM_OPTION_ENABLE
    sample_print("sample DONOT support DRM!\n");
    return 0;
#endif

    if ((argc < 2) || (argc > 4) || (strlen(argv[1]) != 1)) { /* 2,4:arg num */
        goto arg_error;
    }

    if (sample_fisheye_prepare_run(argc, argv) != TD_SUCCESS) {
        goto arg_error;
    }
    ret = sample_fisheye_start(argv);
    sample_fisheye_finish_run(ret);
    return ret;

arg_error:
    sample_fisheye_usage(argv[0]);
    return TD_FAILURE;
}