/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef MKP_SVP_NPU_H
#define MKP_SVP_NPU_H

#include "osal_ioctl.h"
#include "ot_type.h"
#include "ot_errno.h"
#include "ot_common.h"
#include "mkp_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* TRACE */
#define svp_npu_emerg_trace(fmt, ...) OT_EMERG_TRACE(OT_ID_SVP_NPU, "[Func]:%s [Line]:%d [Info]:" fmt, \
    __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define svp_npu_alert_trace(fmt, ...) OT_ALERT_TRACE(OT_ID_SVP_NPU, "[Func]:%s [Line]:%d [Info]:" fmt, \
    __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define svp_npu_crit_trace(fmt, ...) OT_CRIT_TRACE(OT_ID_SVP_NPU, "[Func]:%s [Line]:%d [Info]:" fmt, \
    __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define svp_npu_err_trace(fmt, ...) OT_ERR_TRACE(OT_ID_SVP_NPU, "[Func]:%s [Line]:%d [Info]:" fmt, \
    __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define svp_npu_warn_trace(fmt, ...) OT_WARN_TRACE(OT_ID_SVP_NPU, "[Func]:%s [Line]:%d [Info]:" fmt, \
    __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define svp_npu_notice_trace(fmt, ...) OT_NOTICE_TRACE(OT_ID_SVP_NPU, "[Func]:%s [Line]:%d [Info]:" fmt, \
    __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define svp_npu_info_trace(fmt, ...) OT_INFO_TRACE(OT_ID_SVP_NPU, "[Func]:%s [Line]:%d [Info]:" fmt, \
    __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define svp_npu_debug_trace(fmt, ...) OT_DEBUG_TRACE(OT_ID_SVP_NPU, "[Func]:%s [Line]:%d [Info]:" fmt, \
    __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define SVP_NPU_SUCCESS 0
#define SVP_NPU_AICPU_TIMEOUT 1000
#define SVP_NPU_ERROR_INVALID_PARAM  100000
#define SVP_NPU_ERROR_UNINITIALIZE  100001
#define SVP_NPU_ERROR_REPEAT_INITIALIZE  100002
#define SVP_NPU_ERROR_INVALID_FILE  100003
#define SVP_NPU_ERROR_WRITE_FILE  100004
#define SVP_NPU_ERROR_INVALID_FILE_SIZE  100005
#define SVP_NPU_ERROR_PARSE_FILE  100006
#define SVP_NPU_ERROR_FILE_MISSING_ATTR  100007
#define SVP_NPU_ERROR_FILE_ATTR_INVALID  100008
#define SVP_NPU_ERROR_INVALID_DUMP_CONFIG 100009
#define SVP_NPU_ERROR_INVALID_PROFILING_CONFIG  100010
#define SVP_NPU_ERROR_INVALID_MODEL_ID  100011
#define SVP_NPU_ERROR_DESERIALIZE_MODEL  100012
#define SVP_NPU_ERROR_PARSE_MODEL  100013
#define SVP_NPU_ERROR_READ_MODEL_FAILURE  100014
#define SVP_NPU_ERROR_MODEL_SIZE_INVALID  100015
#define SVP_NPU_ERROR_MODEL_MISSING_ATTR  100016
#define SVP_NPU_ERROR_MODEL_INPUT_NOT_MATCH  100017
#define SVP_NPU_ERROR_MODEL_OUTPUT_NOT_MATCH  100018
#define SVP_NPU_ERROR_MODEL_NOT_DYNAMIC  100019
#define SVP_NPU_ERROR_OP_TYPE_NOT_MATCH  100020
#define SVP_NPU_ERROR_OP_INPUT_NOT_MATCH  100021
#define SVP_NPU_ERROR_OP_OUTPUT_NOT_MATCH  100022
#define SVP_NPU_ERROR_OP_ATTR_NOT_MATCH  100023
#define SVP_NPU_ERROR_OP_NOT_FOUND  100024
#define SVP_NPU_ERROR_OP_LOAD_FAILED  100025
#define SVP_NPU_ERROR_UNSUPPORTED_DATA_TYPE  100026
#define SVP_NPU_ERROR_FORMAT_NOT_MATCH  100027
#define SVP_NPU_ERROR_BIN_SELECTOR_NOT_REGISTERED  100028
#define SVP_NPU_ERROR_KERNEL_NOT_FOUND 100029
#define SVP_NPU_ERROR_BIN_SELECTOR_ALREADY_REGISTERED 100030
#define SVP_NPU_ERROR_KERNEL_ALREADY_REGISTERED  100031
#define SVP_NPU_ERROR_INVALID_QUEUE_ID 100032
#define SVP_NPU_ERROR_REPEAT_SUBSCRIBE  100033
#define SVP_NPU_ERROR_STREAM_NOT_SUBSCRIBE  100034
#define SVP_NPU_ERROR_THREAD_NOT_SUBSCRIBE  100035
#define SVP_NPU_ERROR_WAIT_CALLBACK_TIMEOUT 100036
#define SVP_NPU_ERROR_REPEAT_FINALIZE  100037
#define SVP_NPU_ERROR_NOT_STATIC_AIPP  100038
#define SVP_NPU_ERROR_COMPILING_STUB_MODE  100039
#define SVP_NPU_ERROR_GROUP_NOT_SET  100040
#define SVP_NPU_ERROR_GROUP_NOT_CREATE  100041
#define SVP_NPU_ERROR_PROF_ALREADY_RUN  100042
#define SVP_NPU_ERROR_PROF_NOT_RUN  100043
#define SVP_NPU_ERROR_DUMP_ALREADY_RUN  100044
#define SVP_NPU_ERROR_DUMP_NOT_RUN  100045

#define SVP_NPU_ERROR_BAD_ALLOC  200000
#define SVP_NPU_ERROR_API_NOT_SUPPORT  200001
#define SVP_NPU_ERROR_INVALID_DEVICE  200002
#define SVP_NPU_ERROR_MEMORY_ADDRESS_UNALIGNED  200003
#define SVP_NPU_ERROR_RESOURCE_NOT_MATCH  200004
#define SVP_NPU_ERROR_INVALID_RESOURCE_HANDLE  200005
#define SVP_NPU_ERROR_FEATURE_UNSUPPORTED  200006
#define SVP_NPU_ERROR_PROF_MODULES_UNSUPPORTED 200007

#define SVP_NPU_ERROR_STORAGE_OVER_LIMIT  300000

#define SVP_NPU_ERROR_INTERNAL_ERROR  500000
#define SVP_NPU_ERROR_FAILURE  500001
#define SVP_NPU_ERROR_GE_FAILURE  500002
#define SVP_NPU_ERROR_RT_FAILURE  500003
#define SVP_NPU_ERROR_DRV_FAILURE  500004
#define SVP_NPU_ERROR_PROFILING_FAILURE  500005
#define SVP_NPU_ERROR_RT_REPORT_TIMEOUT  507012

#define svp_npu_convert_addr_to_ptr(type, addr) ((type *)(td_uintptr_t)(addr))

typedef enum {
    SVP_NPU_STATUS_FREE = 0x0,
    SVP_NPU_STATUS_BUSY = 0x1,
    SVP_NPU_STATUS_STOPPING = 0x2,
    SVP_NPU_STATUS_BUTT
} svp_npu_status;

typedef enum {
    SVP_NPU_PRIORITY_0 = 0x0,
    SVP_NPU_PRIORITY_1 = 0x1,
    SVP_NPU_PRIORITY_2 = 0x2,
    SVP_NPU_PRIORITY_3 = 0x3,
    SVP_NPU_PRIORITY_4 = 0x4,
    SVP_NPU_PRIORITY_5 = 0x5,
    SVP_NPU_PRIORITY_6 = 0x6,
    SVP_NPU_PRIORITY_7 = 0x7,
    SVP_NPU_PRIORITY_BUTT
} svp_npu_priority;


typedef struct {
    td_u32 device_id;
    void *base;
    unsigned int tcu_offset;
    unsigned int tbu_offset;
} svm_smmu_reset_arg;

/* IOC */
typedef enum {
    IOC_NR_SVP_NPU_ID = 0x0,
    IOC_NR_SVP_NPU_SEND_TASK,
    IOC_NR_SVP_NPU_SYNC,
    IOC_NR_SVP_NPU_REPORT,
    IOC_NR_SVP_NPU_GET_CALLBACK_TASK,
    IOC_NR_SVP_NPU_FINISH_CALLBACK_TASK,
    IOC_NR_SVP_NPU_GET_MODEL_RESOURCE,
    IOC_NR_SVP_NPU_GET_RUNTIME_RESOURCE,
    IOC_NR_SVP_NPU_SET_CHECK_INFO_MEM,
    IOC_NR_SVP_NPU_DUMP_CTRL,
    IOC_NR_SVP_NPU_PROC_AICPU_TASK_CTRL,

    IOC_NR_SVP_NPU_DEV_INFO_CTRL,

    IOC_NR_SVP_NPU_PROF_UPDATE,
    IOC_NR_SVP_NPU_PROF_START,
    IOC_NR_SVP_NPU_PROF_STOP,
    IOC_NR_SVP_NPU_PROF_POLL,
    IOC_NR_SVP_NPU_PROF_READ,

    IOC_NR_SVP_NPU_SMMU_RESET_CTRL,

    IOC_NR_SVP_NPU_BUTT
} ioc_nr_svp_npu;

#define SVP_NPU_SMMU_RESET_CTRL OSAL_IOWR(IOC_TYPE_SVP_NPU, IOC_NR_SVP_NPU_SMMU_RESET_CTRL, svm_smmu_reset_arg)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* MKP_SVP_NPU_H */
