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
#include "sample_common_dpu.h"

#include <limits.h>
#include "securec.h"
#include "ss_mpi_sys.h"
#include "ss_mpi_vb.h"
#include "ot_common_dpu_rect.h"
#include "ss_mpi_dpu_rect.h"
#include "ss_mpi_dpu_match.h"

td_u32 sample_common_svp_dpu_calc_stride(td_u32 width, td_u8 align)
{
    sample_svp_dpu_check_exps_return(align == 0, 0, "align can't be zero!\n");
    return (width + (align - width % align) % align);
}

/* function : create dpu rect memory info */
static td_s32 sample_common_svp_dpu_rect_create_mem_info(ot_dpu_rect_mem_info *mem_info,
    td_char *mmb, td_char *zone, td_u32 size)
{
    td_s32 ret;

    mem_info->size = size;
    ret = ss_mpi_sys_mmz_alloc((td_phys_addr_t *)(&mem_info->phys_addr),
        (td_void **)&mem_info->virt_addr, mmb, zone, size);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, OT_ERR_DPU_RECT_ILLEGAL_PARAM,
        "Err(%#x), ss_mpi_sys_mmz_alloc failed!\n", ret);
    return ret;
}

static td_s32 sample_svp_dpu_get_lut_from_file(const td_char *file_name,
    ot_dpu_rect_mem_info *mem_info)
{
    td_s32 ret;
    td_u32 size;
    td_void *virt_addr = TD_NULL;
    FILE *fp = TD_NULL;
    td_char path[PATH_MAX] = {0};

    sample_svp_dpu_check_exps_return(((strlen(file_name) > PATH_MAX) || (realpath(file_name, path) == TD_NULL)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Error, file_name is invalid!\n");
    fp = fopen(file_name, "rb");
    sample_svp_dpu_check_exps_return(fp == TD_NULL,
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, open lut file failed!\n");
    ret = fseek(fp, 0L, SEEK_END);
    sample_svp_dpu_check_exps_goto(ret == -1, fail_0, "Error, fseek failed!\n");
    size = ftell(fp);
    sample_svp_dpu_check_exps_goto(size <= 0, fail_0, "Error, ftell failed!\n");
    ret = fseek(fp, 0L, SEEK_SET);
    sample_svp_dpu_check_exps_goto(ret == -1, fail_0, "Error, fseek failed!\n");

    ret = memset_s(mem_info, sizeof(ot_dpu_rect_mem_info), 0, sizeof(ot_dpu_rect_mem_info));
    sample_svp_dpu_check_exps_goto(ret != EOK, fail_0, "Err, memset_s mem_info failed!\n");
    ret = sample_common_svp_dpu_rect_create_mem_info(mem_info, "sampe_dpu_rect_lut", TD_NULL, size);
    sample_svp_dpu_check_exps_goto(ret != EOK, fail_0, "Err, dpu_rect_create_mem_info failed!\n");

    virt_addr = sample_svp_dpu_convert_addr_to_ptr(td_void, mem_info->virt_addr);
    ret = fread(virt_addr, size, 1, fp);
    sample_svp_dpu_check_exps_goto(ret != 1, fail_1, "Error,read lut file failed!\n");
    if (fp != TD_NULL) {
        fclose(fp);
    }

    return TD_SUCCESS;
fail_1:
    sample_svp_dpu_mmz_free(mem_info->phys_addr, mem_info->virt_addr);
fail_0:
    if (fp != TD_NULL) {
        fclose(fp);
    }
    return OT_ERR_DPU_RECT_ILLEGAL_PARAM;
}

td_s32 sample_common_svp_dpu_rect_load_lut(const td_char *file_name,
    ot_dpu_rect_mem_info *mem_info, ot_dpu_rect_lut_id *lut_id)
{
    td_s32 ret;
    td_char path[PATH_MAX] = {0};

    sample_svp_dpu_check_exps_return(((file_name == TD_NULL) || (mem_info == TD_NULL) || (lut_id == TD_NULL)),
        OT_ERR_DPU_RECT_NULL_PTR, "Error, model_file or nnie_model or lut_id is NULL!\n");
    sample_svp_dpu_check_exps_return(((strlen(file_name) > PATH_MAX) || (realpath(file_name, path) == TD_NULL)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Error, file_name is invalid!\n");

    ret = sample_svp_dpu_get_lut_from_file(file_name, mem_info);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS,
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, get_lut_from_file failed!\n");

    ret = ss_mpi_dpu_rect_load_lut(mem_info, lut_id);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail,
        "Error(%#x), ss_mpi_dpu_rect_load_lut failed!\n", ret);

    return ret;
fail:
    sample_svp_dpu_mmz_free(mem_info->phys_addr, mem_info->virt_addr);
    return ret;
}

td_void sample_common_svp_dpu_rect_unload_lut(ot_dpu_rect_mem_info *mem_info,
    ot_dpu_rect_lut_id lut_id)
{
    if (mem_info == TD_NULL) {
        sample_svp_dpu_trace_err("Error, mem_info can't be TD_NULL!\n");
        return;
    }
    (td_void)ss_mpi_dpu_rect_unload_lut(lut_id);
    sample_svp_dpu_mmz_free(mem_info->phys_addr, mem_info->virt_addr);
}

/* function : start dpu rect grp. */
td_s32 sample_common_svp_dpu_rect_start(ot_dpu_rect_grp dpu_rect_grp,
    ot_dpu_rect_grp_attr *grp_attr, ot_dpu_rect_chn_attr *chn_attr, td_u32 chn_num)
{
    ot_dpu_rect_chn dpu_rect_chn;
    td_s32 ret;
    td_s32 j;
    td_s32 dpu_rect_chn_num;

    sample_svp_dpu_check_exps_return(((grp_attr == TD_NULL) || (chn_attr == TD_NULL)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Error, grp_attr or chn_attr is TD_NULL!\n");
    sample_svp_dpu_check_exps_return(((dpu_rect_grp < 0) || (dpu_rect_grp >= OT_DPU_RECT_MAX_GRP_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, dpu_rect_grp(%d) must be in [0,%d)!\n",
        dpu_rect_grp, OT_DPU_RECT_MAX_GRP_NUM);
    sample_svp_dpu_check_exps_return(chn_num < OT_DPU_RECT_MAX_CHN_NUM, OT_ERR_DPU_RECT_ILLEGAL_PARAM,
        "Err, chn_num(%u) can' less than %u!\n", chn_num, OT_DPU_RECT_MAX_CHN_NUM);

    ret = ss_mpi_dpu_rect_create_grp(dpu_rect_grp, grp_attr);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
        "Err(%#x), ss_mpi_dpu_rect_create_grp failed!\n", ret);

    if (grp_attr->rect_mode == OT_DPU_RECT_MODE_SINGLE) {
        dpu_rect_chn_num = 1;
    } else  {
        dpu_rect_chn_num = OT_DPU_RECT_MAX_CHN_NUM;
    }

    for (j = 0; j < dpu_rect_chn_num; j++) {
        dpu_rect_chn = j;
        ret = ss_mpi_dpu_rect_set_chn_attr(dpu_rect_grp, dpu_rect_chn, &chn_attr[dpu_rect_chn]);
        sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
            "Err(%#x), ss_mpi_dpu_rect_set_chn_attr failed!\n", ret);

        ret = ss_mpi_dpu_rect_enable_chn(dpu_rect_grp, dpu_rect_chn);
        sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
            "Err(%#x), ss_mpi_dpu_rect_enable_chn failed!\n", ret);
    }

    ret = ss_mpi_dpu_rect_start_grp(dpu_rect_grp);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
        "Err(%#x), ss_mpi_dpu_rect_start_grp failed!\n", ret);

    return TD_SUCCESS;
}

/* function : stop dpu rect grp */
td_s32 sample_common_svp_dpu_rect_stop(ot_dpu_rect_grp dpu_rect_grp, ot_dpu_rect_mode rect_mode)
{
    td_s32 j;
    td_s32 ret;
    ot_dpu_rect_chn dpu_rect_chn;
    td_s32 dpu_rect_chn_num;

    sample_svp_dpu_check_exps_return(((dpu_rect_grp < 0) || (dpu_rect_grp >= OT_DPU_RECT_MAX_GRP_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, dpu_rect_grp(%d) must be in [0,%d)!\n",
        dpu_rect_grp, OT_DPU_RECT_MAX_GRP_NUM);
    sample_svp_dpu_check_exps_return(((rect_mode < OT_DPU_RECT_MODE_SINGLE) || (dpu_rect_grp >= OT_DPU_RECT_MODE_BUTT)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, rect_mode(%d) must be in [%d,%d)!\n",
        rect_mode, OT_DPU_RECT_MODE_SINGLE, OT_DPU_RECT_MODE_BUTT);

    ret = ss_mpi_dpu_rect_stop_grp(dpu_rect_grp);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
        "Err(%#x), ss_mpi_dpu_rect_stop_grp failed!\n", ret);

    if (rect_mode == OT_DPU_RECT_MODE_SINGLE) {
        dpu_rect_chn_num = 1;
    } else {
        dpu_rect_chn_num = OT_DPU_RECT_MAX_CHN_NUM;
    }

    for (j = 0; j < dpu_rect_chn_num; j++) {
        dpu_rect_chn = j;
        ret = ss_mpi_dpu_rect_disable_chn(dpu_rect_grp, dpu_rect_chn);
        sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
            "Err(%#x), ss_mpi_dpu_rect_disable_chn failed!\n", ret);
    }

    ret = ss_mpi_dpu_rect_destroy_grp(dpu_rect_grp);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
        "Err(%#x), ss_mpi_dpu_rect_disable_chn failed!\n", ret);

    return TD_SUCCESS;
}

static td_s32 sample_svp_dpu_rect_read_one_frame(FILE *fp, td_u8 *y,
    td_u32 width, td_u32 height, td_u32 stride)
{
    td_u8 *dst = TD_NULL;
    td_u32 row;
    td_s32 ret;

    dst = y;
    for (row = 0; row < height; row++) {
        ret = fread(dst, width, 1, fp);
        sample_svp_dpu_check_exps_return(ret != 1, OT_ERR_DPU_RECT_ILLEGAL_PARAM, "fread failed!\n");
        dst += stride;
    }

    return TD_SUCCESS;
}

/* function : get frame from file */
td_s32 sample_common_svp_dpu_rect_get_frame_from_file(FILE *fp, const ot_size *pic_size,
    td_u32 stride, ot_video_frame_info *frame_info, ot_vb_blk vb_blk)
{
    td_u32 size;
    td_phys_addr_t phys_addr;
    td_void *virt_addr = TD_NULL;
    td_s32 ret;

    sample_svp_dpu_check_exps_return(pic_size == TD_NULL, OT_ERR_DPU_RECT_NULL_PTR, "pic_size can't be null!\n");
    phys_addr = ss_mpi_vb_handle_to_phys_addr(vb_blk);
    sample_svp_dpu_check_exps_return(phys_addr == 0,
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "ss_mpi_vb_handle_to_phys_addr failed!\n");

    size = stride * pic_size->height;
    virt_addr = ss_mpi_sys_mmap(phys_addr, size);
    sample_svp_dpu_check_exps_return(virt_addr == TD_NULL,
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "ss_mpi_sys_mmap failed!\n");

    frame_info->pool_id = ss_mpi_vb_handle_to_pool_id(vb_blk);
    sample_svp_dpu_check_exps_goto(frame_info->pool_id == OT_VB_INVALID_POOL_ID,
        fail, "ss_mpi_vb_handle_to_pool_id failed!\n");

    frame_info->video_frame.phys_addr[0] = phys_addr;
    frame_info->video_frame.virt_addr[0] = virt_addr;
    frame_info->video_frame.width  = pic_size->width;
    frame_info->video_frame.height = pic_size->height;
    frame_info->video_frame.stride[0] = stride;
    frame_info->video_frame.compress_mode = OT_COMPRESS_MODE_NONE;
    frame_info->video_frame.video_format = OT_VIDEO_FORMAT_LINEAR;
    frame_info->video_frame.dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    frame_info->video_frame.pixel_format = OT_PIXEL_FORMAT_YUV_400;
    frame_info->video_frame.field = OT_VIDEO_FIELD_FRAME;

    ret = sample_svp_dpu_rect_read_one_frame(fp, (td_u8*)frame_info->video_frame.virt_addr[0],
        frame_info->video_frame.width, frame_info->video_frame.height, frame_info->video_frame.stride[0]);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail,
        "ss_mpi_vb_handle_to_pool_id failed!\n");

    (td_void)ss_mpi_sys_munmap(virt_addr, size);
    frame_info->video_frame.virt_addr[0] = TD_NULL;
    virt_addr = TD_NULL;

    return TD_SUCCESS;
fail:
    (td_void)ss_mpi_sys_munmap(virt_addr, size);
    return OT_ERR_DPU_RECT_ILLEGAL_PARAM;
}

/* function : write  image to binary file */
static td_s32 sample_svp_dpu_write_to_bin_file(FILE *fp, td_u8 *src,
    const ot_video_frame *frame_info, td_u32 ele_size)
{
    td_u32 i;
    td_s32 ret;

    for (i = 0; i < frame_info->height; i++) {
        ret = fwrite(src, frame_info->width * ele_size, 1, fp);
        sample_svp_dpu_check_exps_return(ret != 1, OT_ERR_DPU_RECT_ILLEGAL_PARAM, "fwrite file failed!\n");
        src += frame_info->stride[0];
    }

    (void)fflush(fp);
    return  TD_SUCCESS;
}

/* function : write frame to file. */
td_s32 sample_common_svp_dpu_write_frame_to_file(FILE *fp, td_u32 ele_size,
    ot_video_frame_info *frame_info)
{
    td_u32 stride;
    td_u32 height;
    td_void *virt_addr = TD_NULL;
    td_u32 size;
    td_s32 ret;

    height = frame_info->video_frame.height;
    stride = frame_info->video_frame.stride[0];
    size = stride * height;

    virt_addr = ss_mpi_sys_mmap(frame_info->video_frame.phys_addr[0], size);
    sample_svp_dpu_check_exps_return(virt_addr == TD_NULL,
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "ss_mpi_sys_mmap failed!\n");

    ret = sample_svp_dpu_write_to_bin_file(fp, (td_u8*)virt_addr, &frame_info->video_frame, ele_size);
    sample_svp_dpu_check_exps_goto(ret != TD_SUCCESS, fail, "write_to_bin_filp failed!\n");
fail:
    (td_void)ss_mpi_sys_munmap(virt_addr, size);
    virt_addr = TD_NULL;

    return ret;
}

/* function : create dpu match memory info */
td_s32 sample_common_svp_dpu_match_create_mem_info(ot_dpu_match_mem_info *mem_info,
    td_char *mmb, td_char *zone, td_u32 size)
{
    td_s32 ret;

    sample_svp_dpu_check_exps_return(mem_info == TD_NULL,
        OT_ERR_DPU_MATCH_ILLEGAL_PARAM, "mem_info can't be NULL\n");
    ret = memset_s(mem_info, sizeof(ot_dpu_rect_mem_info), 0, sizeof(ot_dpu_rect_mem_info));
    sample_svp_dpu_check_exps_return(ret != EOK,
        OT_ERR_DPU_MATCH_ILLEGAL_PARAM, "Err, memset_s mem_info failed!\n");

    mem_info->size = size;
    ret = ss_mpi_sys_mmz_alloc((td_phys_addr_t *)&mem_info->phys_addr,
        (td_void **)&mem_info->virt_addr, mmb, zone, size);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS,
        OT_ERR_DPU_MATCH_ILLEGAL_PARAM, "Err(%#x), ss_mpi_sys_mmz_alloc failed\n", ret);

    return TD_SUCCESS;
}

/* function : start dpu match grp. */
td_s32 sample_common_svp_dpu_match_start(ot_dpu_match_grp dpu_match_grp,
    ot_dpu_match_grp_attr *grp_attr, ot_dpu_match_chn_attr *chn_attr)
{
    ot_dpu_match_chn dpu_match_chn;
    td_s32 ret;
    td_s32 j;

    sample_svp_dpu_check_exps_return((grp_attr == TD_NULL) || (chn_attr == TD_NULL),
        OT_ERR_DPU_MATCH_NULL_PTR, "grp_attr or chn_attr can't be NULL\n");
    sample_svp_dpu_check_exps_return(((dpu_match_grp < 0) || (dpu_match_grp >= OT_DPU_MATCH_MAX_GRP_NUM)),
        OT_ERR_DPU_MATCH_ILLEGAL_PARAM, "Err, dpu_match_grp(%d) must be in [0,%d)!\n",
        dpu_match_grp, OT_DPU_MATCH_MAX_GRP_NUM);

    ret = ss_mpi_dpu_match_create_grp(dpu_match_grp, grp_attr);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
        "Err(%#x), ss_mpi_dpu_match_create_grp failed\n", ret);

    for (j = 0; j < OT_DPU_MATCH_MAX_CHN_NUM; j++) {
        dpu_match_chn = j;
        ret = ss_mpi_dpu_match_set_chn_attr(dpu_match_grp, dpu_match_chn, chn_attr);
        sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
            "Err(%#x), ss_mpi_dpu_match_set_chn_attr failed\n", ret);

        ret = ss_mpi_dpu_match_enable_chn(dpu_match_grp, dpu_match_chn);
        sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
            "Err(%#x), ss_mpi_dpu_match_enable_chn failed\n", ret);
    }

    ret = ss_mpi_dpu_match_start_grp(dpu_match_grp);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
        "Err(%#x), ss_mpi_dpu_match_start_grp failed\n", ret);

    return TD_SUCCESS;
}

/* function : stop dpu match grp */
td_s32 sample_common_svp_dpu_match_stop(ot_dpu_match_grp dpu_match_grp)
{
    td_s32 j;
    td_s32 ret;
    ot_dpu_match_chn dpu_match_chn;

    sample_svp_dpu_check_exps_return(((dpu_match_grp < 0) || (dpu_match_grp >= OT_DPU_MATCH_MAX_GRP_NUM)),
        OT_ERR_DPU_MATCH_ILLEGAL_PARAM, "Err, dpu_match_grp(%d) must be in [0,%d)!\n",
        dpu_match_grp, OT_DPU_MATCH_MAX_GRP_NUM);

    ret = ss_mpi_dpu_match_stop_grp(dpu_match_grp);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
        "Err(%#x), ss_mpi_dpu_match_stop_grp failed\n", ret);

    for (j = 0; j < OT_DPU_MATCH_MAX_CHN_NUM; j++) {
        dpu_match_chn = j;
        ret = ss_mpi_dpu_match_disable_chn(dpu_match_grp, dpu_match_chn);
        sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
            "Err(%#x), ss_mpi_dpu_match_disable_chn failed\n", ret);
    }

    ret = ss_mpi_dpu_match_destroy_grp(dpu_match_grp);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, ret,
        "Err(%#x), ss_mpi_dpu_match_destroy_grp failed\n", ret);

    return TD_SUCCESS;
}

td_s32 sample_common_svp_dpu_rect_bind_match(ot_dpu_rect_grp dpu_rect_grp,
    ot_dpu_match_grp dpu_match_grp)
{
    td_s32 ret;
    ot_mpp_chn src_chn;
    ot_mpp_chn dst_chn;

    sample_svp_dpu_check_exps_return(((dpu_rect_grp < 0) || (dpu_rect_grp >= OT_DPU_RECT_MAX_GRP_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, dpu_rect_grp(%d) must be in [0,%d)!\n",
        dpu_rect_grp, OT_DPU_MATCH_MAX_GRP_NUM);
    sample_svp_dpu_check_exps_return(((dpu_match_grp < 0) || (dpu_match_grp >= OT_DPU_MATCH_MAX_GRP_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, dpu_match_grp(%d) must be in [0,%d)!\n",
        dpu_match_grp, OT_DPU_MATCH_MAX_GRP_NUM);

    src_chn.mod_id = OT_ID_DPU_RECT;
    src_chn.dev_id = dpu_rect_grp;
    src_chn.chn_id  = 0;

    dst_chn.mod_id  = OT_ID_DPU_MATCH;
    dst_chn.dev_id = dpu_match_grp;
    dst_chn.chn_id = 0;

    ret = ss_mpi_sys_bind(&src_chn, &dst_chn);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, OT_ERR_DPU_RECT_ILLEGAL_PARAM,
        "Err(%#x), ss_mpi_sys_bind(RECT-MATCH) failede!\n", ret);

    return TD_SUCCESS;
}

td_s32 sample_common_svp_dpu_rect_unbind_match(ot_dpu_rect_grp dpu_rect_grp,
    ot_dpu_match_grp dpu_match_grp)
{
    td_s32 ret;
    ot_mpp_chn src_chn;
    ot_mpp_chn dst_chn;

    sample_svp_dpu_check_exps_return(((dpu_rect_grp < 0) || (dpu_rect_grp >= OT_DPU_RECT_MAX_GRP_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, dpu_rect_grp(%d) must be in [0,%d)!\n",
        dpu_rect_grp, OT_DPU_MATCH_MAX_GRP_NUM);
    sample_svp_dpu_check_exps_return(((dpu_match_grp < 0) || (dpu_match_grp >= OT_DPU_MATCH_MAX_GRP_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, dpu_match_grp(%d) must be in [0,%d)!\n",
        dpu_match_grp, OT_DPU_MATCH_MAX_GRP_NUM);

    src_chn.mod_id  = OT_ID_DPU_RECT;
    src_chn.dev_id = dpu_rect_grp;
    src_chn.chn_id = 0;

    dst_chn.mod_id  = OT_ID_DPU_MATCH;
    dst_chn.dev_id = dpu_match_grp;
    dst_chn.chn_id = 0;

    ret = ss_mpi_sys_unbind(&src_chn, &dst_chn);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, OT_ERR_DPU_RECT_ILLEGAL_PARAM,
        "Err(%#x), ss_mpi_sys_unbind(RECT-MATCH) failede!\n", ret);

    return TD_SUCCESS;
}

td_s32 sample_common_svp_dpu_vpss_bind_rect(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn,
    ot_dpu_rect_grp dpu_rect_grp, ot_dpu_rect_pipe dpu_rect_pipe)
{
    td_s32 ret;
    ot_mpp_chn src_chn;
    ot_mpp_chn dst_chn;

    sample_svp_dpu_check_exps_return(((vpss_grp < 0) || (vpss_grp >= OT_VPSS_MAX_GRP_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, vpss_grp(%d) must be in [0,%d)!\n",
        vpss_grp, OT_VPSS_MAX_GRP_NUM);
    sample_svp_dpu_check_exps_return(((vpss_chn < 0) || (vpss_chn >= OT_VPSS_MAX_CHN_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, vpss_chn(%d) must be in [0,%d)!\n",
        vpss_chn, OT_VPSS_MAX_CHN_NUM);

    sample_svp_dpu_check_exps_return(((dpu_rect_grp < 0) || (dpu_rect_grp >= OT_DPU_RECT_MAX_GRP_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, dpu_rect_grp(%d) must be in [0,%d)!\n",
        dpu_rect_grp, OT_DPU_MATCH_MAX_GRP_NUM);
    sample_svp_dpu_check_exps_return(((dpu_rect_pipe < 0) || (dpu_rect_pipe >= OT_DPU_RECT_MAX_PIPE_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, dpu_rect_pipe(%d) must be in [0,%d)!\n",
        dpu_rect_pipe, OT_DPU_RECT_MAX_PIPE_NUM);

    src_chn.mod_id   = OT_ID_VPSS;
    src_chn.dev_id  = vpss_grp;
    src_chn.chn_id  = vpss_chn;

    dst_chn.mod_id  = OT_ID_DPU_RECT;
    dst_chn.dev_id = dpu_rect_grp;
    dst_chn.chn_id = dpu_rect_pipe;

    ret = ss_mpi_sys_bind(&src_chn, &dst_chn);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, OT_ERR_DPU_RECT_ILLEGAL_PARAM,
        "Err(%#x), ss_mpi_sys_unbind(VPSS-RECT) failede!\n", ret);

    return TD_SUCCESS;
}

td_s32 sample_common_svp_dpu_vpss_unbind_rect(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn,
    ot_dpu_rect_grp dpu_rect_grp, ot_dpu_rect_pipe dpu_rect_pipe)
{
    td_s32 ret;
    ot_mpp_chn src_chn;
    ot_mpp_chn dst_chn;

    sample_svp_dpu_check_exps_return(((vpss_grp < 0) || (vpss_grp >= OT_VPSS_MAX_GRP_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, vpss_grp(%d) must be in [0,%d)!\n",
        vpss_grp, OT_VPSS_MAX_GRP_NUM);
    sample_svp_dpu_check_exps_return(((vpss_chn < 0) || (vpss_chn >= OT_VPSS_MAX_CHN_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, vpss_chn(%d) must be in [0,%d)!\n",
        vpss_chn, OT_VPSS_MAX_CHN_NUM);

    sample_svp_dpu_check_exps_return(((dpu_rect_grp < 0) || (dpu_rect_grp >= OT_DPU_RECT_MAX_GRP_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, dpu_rect_grp(%d) must be in [0,%d)!\n",
        dpu_rect_grp, OT_DPU_MATCH_MAX_GRP_NUM);
    sample_svp_dpu_check_exps_return(((dpu_rect_pipe < 0) || (dpu_rect_pipe >= OT_DPU_RECT_MAX_PIPE_NUM)),
        OT_ERR_DPU_RECT_ILLEGAL_PARAM, "Err, dpu_rect_pipe(%d) must be in [0,%d)!\n",
        dpu_rect_pipe, OT_DPU_RECT_MAX_PIPE_NUM);

    src_chn.mod_id  = OT_ID_VPSS;
    src_chn.dev_id = vpss_grp;
    src_chn.chn_id = vpss_chn;

    dst_chn.mod_id  = OT_ID_DPU_RECT;
    dst_chn.dev_id = dpu_rect_grp;
    dst_chn.chn_id = dpu_rect_grp;

    ret = ss_mpi_sys_unbind(&src_chn, &dst_chn);
    sample_svp_dpu_check_exps_return(ret != TD_SUCCESS, OT_ERR_DPU_RECT_ILLEGAL_PARAM,
        "Err(%#x), ss_mpi_sys_unbind(VPSS-RECT) failede!\n", ret);

    return TD_SUCCESS;
}