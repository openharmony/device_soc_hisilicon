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
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "ot_type.h"
#include "ss_mpi_cipher.h"
#include "ot_common_cipher.h"

#include "sample_utils.h"

#define GENERATE_TIMES  10

td_s32 sample_rng(td_void)
{
    sample_log("************ test rng ************\n");
    td_s32 ret;
    td_s32 index;
    td_u32 random_number = 0;

    /* 1. cipher init */
    sample_chk_expr_return(ss_mpi_cipher_init(), TD_SUCCESS);

    for (index = 0; index < GENERATE_TIMES; index++) {
        ret = ss_mpi_cipher_get_random_num(&random_number);
        if (ret == OT_ERR_CIPHER_NO_AVAILABLE_RNG) {
            /* "there is no ramdom number available now. try again! */
            index--;
            continue;
        }

        if (ret != TD_SUCCESS) {
            sample_err("ss_mpi_cipher_get_random_num failed!\n");
            goto __CIPHER_DEINIT__;
        }
    }
    sample_log("************ test tng success ************\n");
__CIPHER_DEINIT__:
    (td_void)ss_mpi_cipher_deinit();
    return ret;
}

