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
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "sample_comm.h"

#define VPSS_DEFAULT_WIDTH  3840
#define VPSS_DEFAULT_HEIGHT 2160

td_void sample_comm_vpss_get_default_grp_attr(ot_vpss_grp_attr *grp_attr)
{
    grp_attr->nr_en                     = TD_TRUE;
    grp_attr->ie_en                     = TD_FALSE;
    grp_attr->dci_en                    = TD_FALSE;
    grp_attr->buf_share_en              = TD_FALSE;
    grp_attr->mcf_en                    = TD_FALSE;
    grp_attr->max_width                 = VPSS_DEFAULT_WIDTH;
    grp_attr->max_height                = VPSS_DEFAULT_HEIGHT;
    grp_attr->max_dei_width             = 0;
    grp_attr->max_dei_height            = 0;
    grp_attr->dynamic_range             = OT_DYNAMIC_RANGE_SDR8;
    grp_attr->pixel_format              = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    grp_attr->dei_mode                  = OT_VPSS_DEI_MODE_OFF;
    grp_attr->buf_share_chn             = OT_VPSS_CHN0;
    grp_attr->nr_attr.nr_type           = OT_VPSS_NR_TYPE_VIDEO_NORM;
    grp_attr->nr_attr.compress_mode     = OT_COMPRESS_MODE_FRAME;
    grp_attr->nr_attr.nr_motion_mode    = OT_VPSS_NR_MOTION_MODE_NORM;
    grp_attr->frame_rate.src_frame_rate = -1;
    grp_attr->frame_rate.dst_frame_rate = -1;
}

td_void sample_comm_vpss_get_default_chn_attr(ot_vpss_chn_attr *chn_attr)
{
    chn_attr->mirror_en                 = TD_FALSE;
    chn_attr->flip_en                   = TD_FALSE;
    chn_attr->border_en                 = TD_FALSE;
    chn_attr->width                     = VPSS_DEFAULT_WIDTH;
    chn_attr->height                    = VPSS_DEFAULT_HEIGHT;
    chn_attr->depth                     = 0;
    chn_attr->chn_mode                  = OT_VPSS_CHN_MODE_USER;
    chn_attr->video_format              = OT_VIDEO_FORMAT_LINEAR;
    chn_attr->dynamic_range             = OT_DYNAMIC_RANGE_SDR8;
    chn_attr->pixel_format              = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    chn_attr->compress_mode             = OT_COMPRESS_MODE_SEG;
    chn_attr->aspect_ratio.mode         = OT_ASPECT_RATIO_NONE;
    chn_attr->frame_rate.src_frame_rate = -1;
    chn_attr->frame_rate.dst_frame_rate = -1;
}

static td_s32 sample_common_vpss_start_chn(ot_vpss_grp grp, const td_bool *chn_enable,
                                           const ot_vpss_chn_attr *chn_attr, td_u32 chn_array_size)
{
    ot_vpss_chn vpss_chn;
    td_s32 ret;
    td_s32 i;

    for (i = 0; i < (td_s32)chn_array_size; ++i) {
        if (chn_enable[i] == TD_TRUE) {
            vpss_chn = i;
            ret = ss_mpi_vpss_set_chn_attr(grp, vpss_chn, &chn_attr[vpss_chn]);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vpss_set_chn_attr failed with %#x\n", ret);
                goto disable_chn;
            }
            ret = ss_mpi_vpss_enable_chn(grp, vpss_chn);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vpss_enable_chn failed with %#x\n", ret);
                goto disable_chn;
            }
        }
    }
    return TD_SUCCESS;

disable_chn:
    for (i = i - 1; i >= 0; i--) {
        if (chn_enable[i] == TD_TRUE) {
            vpss_chn = i;
            ret = ss_mpi_vpss_disable_chn(grp, vpss_chn);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vpss_disable_chn failed with %#x!\n", ret);
            }
        }
    }
    return TD_FAILURE;
}

td_s32 sample_common_vpss_start(ot_vpss_grp grp, const td_bool *chn_enable,
    const ot_vpss_grp_attr *grp_attr, const ot_vpss_chn_attr *chn_attr, td_u32 chn_array_size)
{
    td_s32 ret;
    if (chn_array_size < OT_VPSS_MAX_PHYS_CHN_NUM) {
        sample_print("array size(%u) of chn_enable and chn_attr need > %u!\n",
            chn_array_size, OT_VPSS_MAX_PHYS_CHN_NUM);
        return TD_FAILURE;
    }
    ret = ss_mpi_vpss_create_grp(grp, grp_attr);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_vpss_create_grp(grp:%d) failed with %#x!\n", grp, ret);
        return TD_FAILURE;
    }
    ret = ss_mpi_vpss_start_grp(grp);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_vpss_start_grp failed with %#x\n", ret);
        goto destroy_grp;
    }

    ret = sample_common_vpss_start_chn(grp, chn_enable, chn_attr, OT_VPSS_MAX_PHYS_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto stop_grp;
    }

    return TD_SUCCESS;

stop_grp:
    ret = ss_mpi_vpss_stop_grp(grp);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_vpss_stop_grp failed with %#x!\n", ret);
    }
destroy_grp:
    ret = ss_mpi_vpss_destroy_grp(grp);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_vpss_destroy_grp failed with %#x!\n", ret);
    }
    return TD_FAILURE;
}

td_s32 sample_common_vpss_stop(ot_vpss_grp grp, const td_bool *chn_enable, td_u32 chn_array_size)
{
    td_s32 i;
    td_s32 ret;
    ot_vpss_chn vpss_chn;

    if (chn_array_size < OT_VPSS_MAX_PHYS_CHN_NUM) {
        sample_print("array size(%u) of chn_enable need > %u!\n", chn_array_size, OT_VPSS_MAX_PHYS_CHN_NUM);
        return TD_FAILURE;
    }

    for (i = 0; i < OT_VPSS_MAX_PHYS_CHN_NUM; ++i) {
        if (chn_enable[i] == TD_TRUE) {
            vpss_chn = i;
            ret = ss_mpi_vpss_disable_chn(grp, vpss_chn);
            if (ret != TD_SUCCESS) {
                sample_print("ss_mpi_vpss_disable_chn failed with %#x!\n", ret);
            }
        }
    }

    ret = ss_mpi_vpss_stop_grp(grp);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_vpss_stop_grp failed with %#x!\n", ret);
    }

    ret = ss_mpi_vpss_destroy_grp(grp);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_vpss_destroy_grp failed with %#x!\n", ret);
    }

    return TD_SUCCESS;
}
