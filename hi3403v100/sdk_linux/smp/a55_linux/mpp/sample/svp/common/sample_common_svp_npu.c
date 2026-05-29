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
#include "sample_common_svp_npu.h"
#include "ot_common.h"
#include "ot_common_svp.h"
#include "sample_comm.h"
#include "sample_common_svp.h"

/* acl init */
 td_s32 sample_common_svp_npu_acl_init(const td_char *acl_config_path, td_s32 dev_id)
{
    svp_acl_rt_run_mode run_mode;
    svp_acl_error ret;
    /* svp acl init */
    ret = svp_acl_init(acl_config_path);
    sample_svp_check_exps_return(ret != SVP_ACL_SUCCESS, TD_FAILURE, SAMPLE_SVP_ERR_LEVEL_ERROR, "acl init failed!\n");
    sample_svp_trace_info("svp acl init success!\n");
    /* open device */
    ret = svp_acl_rt_set_device(dev_id);
    sample_svp_check_exps_goto(ret != SVP_ACL_SUCCESS, end_0, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "svp acl open device %d failed!\n", dev_id);

    sample_svp_trace_info("open device %d success!\n", dev_id);
    /* get run mode */
    ret = svp_acl_rt_get_run_mode(&run_mode);
    sample_svp_check_exps_goto(ret != SVP_ACL_SUCCESS, end_1, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "acl get run mode failed!\n");
    sample_svp_check_exps_goto(run_mode != SVP_ACL_DEVICE, end_1, SAMPLE_SVP_ERR_LEVEL_ERROR,
        "run mode(%d) is not equal to %d!\n", run_mode, SVP_ACL_DEVICE);

    sample_svp_trace_info("get run mode success!\n");
    return TD_SUCCESS;
end_1:
    (td_void)svp_acl_rt_reset_device(dev_id);
end_0:
    (td_void)svp_acl_finalize();
    return TD_FAILURE;
}

/* acl deinit */
td_void sample_common_svp_npu_acl_deinit(td_s32 dev_id)
{
    svp_acl_error ret;

    ret = svp_acl_rt_reset_device(dev_id);
    if (ret != SVP_ACL_SUCCESS) {
        sample_svp_trace_err("reset device fail\n");
    }
    sample_svp_trace_info("end to reset device is %d\n", dev_id);

    ret = svp_acl_finalize();
    if (ret != SVP_ACL_SUCCESS) {
        sample_svp_trace_err("finalize acl fail\n");
    }
    sample_svp_trace_info("end to finalize acl\n");
}
