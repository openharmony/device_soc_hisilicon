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

#ifndef SAMPLE_HNR_MCF_COMMON_H
#define SAMPLE_HNR_MCF_COMMON_H
#include "ot_common.h"
#include "ot_math.h"
#include "ot_buffer.h"
#include "securec.h"
#include "ot_mipi_rx.h"
#include "ot_mipi_tx.h"
#include "ot_common_sys.h"
#include "ot_common_isp.h"
#include "ot_common_vpss.h"
#include "ot_common_aio.h"
#include "ot_common_hdmi.h"
#include "ss_mpi_sys.h"
#include "ss_mpi_vpss.h"
#include "ss_mpi_mcf.h"
#include "sample_comm.h"
#include "ss_mpi_mcf_vi.h"

typedef struct {
    td_char *input_file_name;
    ot_size input_img_size;
    td_char *output_file_name;
    ot_size output_img_size;
} sample_comm_mcf_scale_img_param;

#define safe_free(memory) do { \
    if ((memory) != TD_NULL) { \
        free(memory); \
        memory = TD_NULL; \
    } \
} while (0)

td_void sample_hnr_common_mcf_get_default_grp_attr(ot_mcf_grp_attr *grp_attr, ot_size *mono_size, ot_size *color_size);
td_s32 sample_hnr_common_vi_bind_mcf(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_mcf_grp mcf_grp, ot_mcf_pipe mcf_pipe);
td_s32 sample_hnr_common_vi_unbind_mcf(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_mcf_grp mcf_grp, ot_mcf_pipe mcf_pipe);

td_s32 sample_hnr_common_mcf_start(ot_mcf_grp grp, const ot_mcf_grp_attr *grp_attr, ot_crop_info *grp_crop_info,
                                   td_bool *mcf_chn_en, ot_size *chn_out_size);

td_s32 sample_hnr_common_mcf_stop_vpss(const ot_mcf_grp_attr *grp_attr);
td_s32 sample_hnr_common_mcf_stop(ot_mcf_grp grp, const td_bool *chn_enable, td_u32 chn_array_size);
td_s32 sample_hnr_common_mcf_start_vpss(const ot_mcf_grp_attr *mcf_grp_attr);
td_s32 sample_hnr_common_mcf_bind_vpss(ot_mcf_grp mcf_grp, ot_mcf_chn mcf_chn, ot_vpss_grp vpss_grp,
    ot_vpss_chn vpss_chn);
td_s32 sample_hnr_common_mcf_unbind_vpss(ot_mcf_grp mcf_grp, ot_mcf_chn mcf_chn, ot_vpss_grp vpss_grp,
    ot_vpss_chn vpss_chn);

/* functions for hnr */
td_s32 sample_hnr_mcf_check_size(ot_size mono_size, ot_size color_size);
td_void sample_hnr_mcf_start_mcf_vi_cfg(ot_mcf_grp mcf_grp, const ot_mcf_grp_attr *mcf_grp_attr);
td_void sample_hnr_mcf_stop_mcf_vi_cfg(ot_mcf_grp mcf_grp);

td_s32 sample_hnr_mcf_start_vpss(ot_vpss_grp grp, ot_size *in_size);
td_void sample_hnr_mcf_stop_vpss(ot_vpss_grp grp);
td_void sample_hnr_mcf_bind_vi(ot_mcf_grp mcf_grp, const ot_mcf_grp_attr *mcf_grp_attr, ot_vi_chn vi_chn);
td_void sample_hnr_mcf_unbind_vi(ot_mcf_grp mcf_grp, const ot_mcf_grp_attr *mcf_grp_attr, ot_vi_chn vi_chn);

#endif
