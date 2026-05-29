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

#ifndef _SS_MPI_MAU_H_
#define _SS_MPI_MAU_H_

#include "ot_common_mau.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Prototype    : ss_mpi_svp_mau_matrix_mul
 * Description  : matrix product operator
 * Parameters   : ot_svp_mau_handle            *handle        Returned handle ID of a task
 * const ot_svp_mau_src_double_matrix  *src           Input left and right matrix
 * const ot_svp_mau_src_double_matrix  *src_idx       Input left or right matrix's row vector idx
 * const ot_svp_mau_ctrl        *ctrl          Ctrl prameters
 * const ot_svp_mau_op_result   *dst           Output matrix product operator result
 * Return Value : TD_SUCCESS: Success; Error codes: Failure.
 */
td_s32 ss_mpi_svp_mau_matrix_mul(ot_svp_mau_handle *handle, const ot_svp_mau_src_double_matrix *src,
    const ot_svp_mau_src_double_matrix *src_idx, const ot_svp_mau_ctrl *ctrl, const ot_svp_dst_blob *dst);

/*
 * Prototype    : ss_mpi_svp_mau_cos_dist
 * Description  : cosine distance operator
 * Parameters   : ot_svp_mau_handle            *handle        Returned handle ID of a task
 * const ot_svp_mau_src_double_matrix  *src           Input left and right matrix,
 * the data in each row vector must be normalized;
 * const ot_svp_mau_src_double_matrix  *src_idx       Input left or right matrix's row vector idx
 * const ot_svp_mau_ctrl        *ctrl          Ctrl prameters
 * const ot_svp_mau_dist_result *dst           Output cosine distance operator result
 * Return Value : TD_SUCCESS: Success; Error codes: Failure.
 */
td_s32 ss_mpi_svp_mau_cos_dist(ot_svp_mau_handle *handle, const ot_svp_mau_src_double_matrix *src,
    const ot_svp_mau_src_double_matrix *src_idx, const ot_svp_mau_ctrl *ctrl, const ot_svp_mau_dist_result *dst);

/*
 * Prototype    : ss_mpi_svp_mau_euclid_dist
 * Description  : euclidean distance operator
 * Parameters   : ot_svp_mau_handle            *handle        Returned handle ID of a task
 * const ot_svp_mau_src_double_matrix  *src           Input left and right matrix
 * const ot_svp_mau_src_double_matrix  *src_idx       Input left or right matrix's row vector idx
 * const ot_svp_mau_ctrl        *ctrl          Ctrl prameters
 * const ot_svp_mau_op_result   *dst           Output euclidean distance operator result
 * Return Value : TD_SUCCESS: Success; Error codes: Failure.
 */
td_s32 ss_mpi_svp_mau_euclid_dist(ot_svp_mau_handle *handle, const ot_svp_mau_src_double_matrix *src,
    const ot_svp_mau_src_double_matrix *src_idx, const ot_svp_mau_ctrl *ctrl, const ot_svp_mau_dist_result *dst);

/*
 * Prototype    : ss_mpi_svp_mau_query
 * Description  : This API is used to query the status of a function run on mau.
 * In block mode, the system waits until the function that is being queried is called.
 * In non-block mode, the current status is queried and no action is taken.
 * Parameters   : ot_svp_mau_id        mau_id       MAU core id
 * ot_svp_mau_handle    handle       Handle of a called function. It is entered by users.
 * td_bool              *is_finish      Returned status
 * td_bool               is_block       Flag indicating the block mode or non-block mode
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 */
td_s32 ss_mpi_svp_mau_query(ot_svp_mau_id mau_id, ot_svp_mau_handle handle, td_bool *is_finish, td_bool is_block);

/*
 * Prototype    : ss_mpi_svp_mau_add_mem_info
 * Description  : This API is used to record mem info in mem info array.
 * If the mem info has been recorded, when user send the mem the data in which
 * will be checked in kernel    (e.g. the mem which stores row vector idx),
 * the phyaddr of this mem will not be mapped again,
 * this will improve the execution efficiency
 * Parameters   :  const ot_svp_mau_mem_info *mem_info mem info
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 */
td_s32 ss_mpi_svp_mau_add_mem_info(const ot_svp_mem_info *mem_info);

/*
 * Prototype    : ss_mpi_svp_mau_rm_mem_info
 * Description  : This API is used to remove mem info from mem info array and umap viraddr.
 * If user no longer uses the recorded mem info, the recorded mem info must be removed
 * Parameters   : const ot_svp_mau_mem_info *mem_info mem info
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 */
td_s32 ss_mpi_svp_mau_rm_mem_info(const ot_svp_mem_info *mem_info);
#ifdef __cplusplus
}
#endif

#endif