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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <securec.h>

#include "ss_mpi_cipher.h"
#include "ot_common_cipher.h"
#include "ss_mpi_sys.h"
#include "sample_utils.h"

#define DATA_LEN    16

static td_s32 _cipher_set_config_info(td_handle cipher, ot_cipher_alg alg)
{
    ot_cipher_ctrl cipher_ctrl = {0};

    cipher_ctrl.alg = alg;

    sample_chk_expr_return(ss_mpi_cipher_set_cfg(cipher, &cipher_ctrl), TD_SUCCESS);

    return TD_SUCCESS;
}

td_s32 run_sample_dma(td_void)
{
    td_s32 ret = TD_SUCCESS;
    td_handle handle = 0;
    td_phys_addr_t in_phys_addr;
    td_phys_addr_t out_phys_addr;
    td_u8 *in_addr_vir = TD_NULL;
    td_u8 *out_addr_vir = TD_NULL;
    ot_cipher_attr cipher_attr = {0};
    const td_u32 test_data_len = DATA_LEN;
    td_u8 data_src[DATA_LEN] = {0};

    /* 1. generate src data */
    sample_chk_expr_return(get_random_data(data_src, sizeof(data_src)), TD_SUCCESS);
    sample_chk_expr_return(ss_mpi_sys_mmz_alloc(&in_phys_addr, (td_void **)&in_addr_vir, NULL, NULL, test_data_len),
        TD_SUCCESS);
    sample_chk_expr_goto(ss_mpi_sys_mmz_alloc(&out_phys_addr, (td_void **)&out_addr_vir, NULL, NULL, test_data_len),
        TD_SUCCESS, __CIPHER_FREE__);

    (td_void)memset_s(in_addr_vir, test_data_len, 0, test_data_len);

    (td_void)memset_s(out_addr_vir, test_data_len, 0, test_data_len);

    /* cipher init */
    sample_chk_expr_goto(ss_mpi_cipher_init(), TD_SUCCESS, __CIPHER_FREE__);

    /* cipher create handle */
    cipher_attr.cipher_type = OT_CIPHER_TYPE_NORMAL;
    sample_chk_expr_goto(ss_mpi_cipher_create(&handle, &cipher_attr), TD_SUCCESS, __CIPHER_DEINIT__);

    /* cipher set config */
    sample_chk_expr_goto(_cipher_set_config_info(handle, OT_CIPHER_ALG_DMA), TD_SUCCESS, __CIPHER_EXIT__);

    sample_chk_expr_goto_with_ret(memcpy_s(in_addr_vir, test_data_len, data_src, test_data_len),
        EOK, ret, TD_FAILURE, __CIPHER_EXIT__);

    sample_chk_expr_goto(ss_mpi_cipher_encrypt(handle, in_phys_addr, out_phys_addr, test_data_len),
        TD_SUCCESS, __CIPHER_EXIT__);

    sample_chk_expr_goto_with_ret(memcmp(data_src, out_addr_vir, test_data_len),
        0, ret, TD_FAILURE, __CIPHER_EXIT__);

__CIPHER_EXIT__:
    ss_mpi_cipher_destroy(handle);
__CIPHER_DEINIT__:
    ss_mpi_cipher_deinit();

__CIPHER_FREE__:
    if (in_addr_vir != TD_NULL) {
        ss_mpi_sys_mmz_free(in_phys_addr, in_addr_vir);
    }
    if (out_addr_vir != TD_NULL) {
        ss_mpi_sys_mmz_free(out_phys_addr, out_addr_vir);
    }

    return ret;
}

td_s32 sample_dma(td_void)
{
    td_s32 ret;
    sample_log("************ test dma ************\n");
    ret = run_sample_dma();
    if (ret != TD_SUCCESS) {
        return ret;
    }
    sample_log("************ test dma success ************\n");
    return TD_SUCCESS;
}
