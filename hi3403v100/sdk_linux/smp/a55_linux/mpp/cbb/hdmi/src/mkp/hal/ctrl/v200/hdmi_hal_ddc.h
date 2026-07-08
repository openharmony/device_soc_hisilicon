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
#ifndef HDMI_HAL_DDC_H
#define HDMI_HAL_DDC_H

#include "drv_hdmi_common.h"
#include "hdmi_reg_aon.h"
#include "hdmi_product_define.h"

#define DDC_MAX_RECORD_NUM              30
#define DDC_DEFAULT_TIMEOUT_ACCESS      100
#define DDC_DEFAULT_TIMEOUT_HPD         100
#define DDC_DEFAULT_TIMEOUT_IN_PROG     20
#define DDC_DEFAULT_TIMEOUT_SCL         1
#define DDC_DEFAULT_TIMEOUT_SDA         30
#define DDC_DEFAULT_TIMEOUT_ISSUE       20
#define DDC_DEFAULT_RETRY_TIMEOUT_ISSUE 60

typedef enum {
    DDC_FUNC_TYPE_EDID,
    DDC_FUNC_TYPE_HDCP,
    DDC_FUNC_TYPE_SCDC,
    DDC_FUNC_TYPE_BUTT
} ddc_func_type;

typedef enum {
    DDC_MASTER_MODE_PWD,
    DDC_MASTER_MODE_AON
} ddc_master_mode;

typedef enum {
    DDC_MODE_READ_SINGLE_NO_ACK,
    DDC_MODE_READ_SINGLE_ACK,
    DDC_MODE_READ_MUTIL_NO_ACK,
    DDC_MODE_READ_MUTIL_ACK,
    DDC_MODE_READ_SEGMENT_NO_ACK,
    DDC_MODE_READ_SEGMENT_ACK,
    DDC_MODE_WRITE_MUTIL_NO_ACK,
    DDC_MODE_WRITE_MUTIL_ACK,
    DDC_MODE_BUTT
} ddc_issue_mode;

typedef struct {
    td_bool        valid;
    ddc_mst_ctrl   ddc_ctrl;    /* 4040 */
    ddc_fifo_rdata ddc_rdata;   /* 4044 */
    ddc_fifo_wdata ddc_wdata;   /* 4048 */
    ddc_data_cnt   data_count;  /* 404C */
    ddc_slave_cfg  slave_cfg;   /* 4050 */
    ddc_mst_state  mst_state;   /* 4054 */
    ddc_mst_cmd    mst_cmd;     /* 4058 */
    ddc_man_ctrl   ddc_sw_ctrl; /* 405C */
    ddc_state_clr  state_clr;   /* 4060 */
} ddc_reg_status;

typedef union {
    struct {
        td_u32 access_wait_timeout : 1;
        td_u32 bus_low_scl         : 1;
        td_u32 bus_low_sda         : 1;
        td_u32 err_isseu_mode      : 1;
        td_u32 issue_timeout       : 1;
        td_u32 bus_no_ack          : 1;
        td_u32 in_prog_timeout     : 1;
        td_u32 hpd_no_exist        : 1;
        td_u32 clr_err_fail        : 1;
        td_u32 reserved            : 23;
    } u32;
    td_u32 word;
} ddc_err;

typedef struct {
    td_u8           segment;
    td_u8           offset;
    ddc_func_type   func_type;
    ddc_issue_mode  issue_mode;
    ddc_master_mode master_mode; /* AON MODE ONLY FOR DEBUG */
    td_u32          speed;
    td_u32          access_timeout;
    td_u32          hpd_timeout;
    td_u32          in_prog_timeout;
    td_u32          scl_timeout;
    td_u32          sda_timeout;
    td_u32          issue_timeout;
    td_s32          data_size;
    td_u8          *data;
} ddc_cfg;

typedef struct {
    td_bool init;      /* is the DDC init */
    td_u32  elem_ptr;  /* element pointer in the record array */
    td_u32  total_num; /* total record size, max value is DDC_MAX_RECORD_NUM */
} ddc_run;

typedef struct {
    td_u64         start_time;
    td_u64         time_len;
    ddc_err        un_err;
    ddc_reg_status reg_before;
    ddc_reg_status reg_after;
    ddc_cfg        cfg;
    td_s32         data_size;
    td_u8         *data;
} ddc_record_elem;

typedef struct {
    ddc_run run;
    ddc_record_elem  ddc_record[DDC_MAX_RECORD_NUM];
    osal_semaphore_t ddc_wr_mutex;
} ddc_info;

td_s32 hal_hdmi_ddc_init(hdmi_device_id hdmi);

td_s32 hal_hdmi_ddc_deinit(hdmi_device_id hdmi);

td_s32 hal_hdmi_ddc_issue(hdmi_device_id hdmi, const ddc_cfg *cfg);

td_void hal_hdmi_ddc_default_cfg_get(hdmi_device_id hdmi, ddc_cfg *cfg);

td_s32 hal_hdmi_ddc_edid_raw_get(hdmi_device_id hdmi, td_s32 size, td_u8 *data);

#endif /* HDMI_HAL_DDC_H */

