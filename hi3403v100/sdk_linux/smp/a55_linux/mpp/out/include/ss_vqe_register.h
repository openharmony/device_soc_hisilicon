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

#ifndef __SS_VQE_REGISTER_H__
#define __SS_VQE_REGISTER_H__

#include "ot_vqe_register.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* record */
td_void *ss_vqe_record_get_handle(td_void);
/* HPF */
td_void *ss_vqe_hpf_get_handle(td_void);
/* AEC */
td_void *ss_vqe_aec_get_handle(td_void);
/* ANR */
td_void *ss_vqe_anr_get_handle(td_void);
/* RNR */
td_void *ss_vqe_rnr_get_handle(td_void);
/* HDR */
td_void *ss_vqe_hdr_get_handle(td_void);
/* DRC */
td_void *ss_vqe_drc_get_handle(td_void);
/* PEQ */
td_void *ss_vqe_peq_get_handle(td_void);
/* AGC */
td_void *ss_vqe_agc_get_handle(td_void);
/* EQ */
td_void *ss_vqe_eq_get_handle(td_void);
/* resample */
td_void *ss_vqe_resample_get_handle(td_void);
/* GAIN */
td_void *ss_vqe_gain_get_handle(td_void);
/* talkv2 */
td_void *ss_vqe_talkv2_get_handle(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
