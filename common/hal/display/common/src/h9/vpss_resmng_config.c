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

#include "vpss_resmng.h"

int32_t ConfigVpssLowDelay(VPSS_GRP vpssGrp)
{
    ot_low_delay_info vpssLdyInfo;
    ret = ss_mpi_vpss_get_chn_low_delay(vpssGrp, 0, &vpssLdyInfo);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("ss_mpi_vpss_get_chn_low_delay failed with %#x\n", ret);
        return TD_FAILURE;
    }
    vpssLdyInfo.enable = TD_TRUE;
    vpssLdyInfo.line_cnt = 16; /* 16:VDEC_LOW_DELAY_MIN_LINE */
    ret = ss_mpi_vpss_set_chn_low_delay(vpssGrp, 0, &vpssLdyInfo);
    if (ret != TD_SUCCESS) {
        HDF_LOGI("ss_mpi_vpss_set_chn_low_delay failed with %#x\n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}