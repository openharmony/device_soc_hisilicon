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
#include "hdmi_hal_scdc.h"
#include "drv_hdmi_intf.h"
#include "hdmi_osal.h"
#include "hdmi_product_define.h"
#include "hdmi_hal_phy.h"
#include "hdmi_hal_ddc.h"
#include "hdmi_hal_machine.h"
#include "hdmi_hal_intf.h"
#include "hdmi_reg_tx.h"

#define SCDC_HDMI20_VERSION       1
#define SCDC_UPDATE_TIME_INTERVAL 150
#define READ_SRC_VERSION_COUNT    2

typedef struct {
    td_bool src_scramble;
    td_bool sink_scramble;
    td_bool tmds_clk_ratio40x;
} scdc_scramble;

static scdc_infomation g_scdc_info[HDMI_ID_MAX] = {{{0}, {0}}};

static scdc_infomation *scdc_info_ptr_get(hdmi_device_id hdmi_id)
{
    if ((td_s32)hdmi_id < HDMI_ID_MAX) {
        return &g_scdc_info[hdmi_id];
    }
    return TD_NULL;
}

static td_s32 scdc_scramble_set(hdmi_device_id hdmi_id, const scdc_scramble *scramble)
{
    td_s32 ret;
    scdc_tmds_config ddc_config_byte = {0};
    ddc_cfg ddc_src_cfg = {0};

    hdmi_if_null_warn_return(scramble, TD_FAILURE);

    /* source scramble */
    hdmi_reg_enc_scr_on_set(scramble->src_scramble);
    /* sink scramble */
    ddc_config_byte.u8.screambling_enable = scramble->sink_scramble ? TD_TRUE : TD_FALSE;
    ddc_config_byte.u8.tmds_bit_clock_ratio = scramble->tmds_clk_ratio40x ? TD_TRUE : TD_FALSE;
    hal_hdmi_ddc_default_cfg_get(hdmi_id, &ddc_src_cfg);
    ddc_src_cfg.func_type  = DDC_FUNC_TYPE_SCDC;
    ddc_src_cfg.issue_mode = DDC_MODE_WRITE_MUTIL_ACK;
    ddc_src_cfg.offset     = SCDC_OFFSET_TMDS_CONFIG;
    ddc_src_cfg.data       = &ddc_config_byte.byte;
    ddc_src_cfg.data_size  = 1;
    ret = hal_hdmi_ddc_issue(hdmi_id, &ddc_src_cfg);

    return (ret == ddc_src_cfg.data_size) ? TD_SUCCESS : TD_FAILURE;
}

static td_s32 scdc_scramble_get(hdmi_device_id hdmi_id, scdc_scramble *scramble)
{
    td_s32 ret;
    scdc_scrambler_status ddc_sramble_byte = {0};
    scdc_tmds_config      ddc_config_byte  = {0};
    ddc_cfg               ddc_sink_cfg     = {0};
    scdc_infomation      *scdc_info        = TD_NULL;

    scdc_info = scdc_info_ptr_get(hdmi_id);

    hdmi_if_null_warn_return(scdc_info, TD_FAILURE);
    /* source scrambled */
    scramble->src_scramble = hdmi_reg_enc_scr_on_get() ? TD_TRUE : TD_FALSE;
    hal_hdmi_ddc_default_cfg_get(hdmi_id, &ddc_sink_cfg);
    /* sink scrambled status */
    ddc_sink_cfg.func_type  = DDC_FUNC_TYPE_SCDC;
    ddc_sink_cfg.issue_mode = DDC_MODE_READ_MUTIL_NO_ACK;
    ddc_sink_cfg.data_size  = 1;
    ddc_sink_cfg.offset     = SCDC_OFFSET_SCRAMBLER_STATUS;
    ddc_sramble_byte.byte   = 0;
    ddc_sink_cfg.data       = &ddc_sramble_byte.byte;
    ret = hal_hdmi_ddc_issue(hdmi_id, &ddc_sink_cfg);
    scramble->sink_scramble = ddc_sramble_byte.u8.screambling_status ? TD_TRUE : TD_FALSE;
    scdc_info->status.reg_update[scdc_info->status.reg_update_ptr].scramble_status.byte = ddc_sramble_byte.byte;

    /* sink tmds bit clock ratio & scramble cfg */
    ddc_sramble_byte.byte = 0;
    ddc_sink_cfg.data     = &ddc_config_byte.byte;
    ddc_sink_cfg.offset   = SCDC_OFFSET_TMDS_CONFIG;
    ret = hal_hdmi_ddc_issue(hdmi_id, &ddc_sink_cfg);
    scramble->tmds_clk_ratio40x = ddc_config_byte.u8.tmds_bit_clock_ratio ? TD_TRUE : TD_FALSE;
    scdc_info->status.reg_static.tmds_config0.byte = ddc_config_byte.byte;

    return (ret == ddc_sink_cfg.data_size) ? TD_SUCCESS : TD_FAILURE;
}

static td_void scdc_mach_poll_callback(td_void *data)
{
    scdc_infomation *scdc_info = (scdc_infomation *)data;
    hdmi_if_null_warn_return_void(scdc_info);
    hdmi_if_false_warn_return_void(scdc_info->status.init);

    return;
}

static td_void scdc_attr_param_check(scdc_attr *attr)
{
    if ((attr->scramble_interval < SCDC_DEFAULT_SCRAMBLE_INTERVAL) ||
        (attr->scramble_interval > SCDC_DEFAULT_SCRAMBLE_TIMEOUT)) {
        attr->scramble_interval = SCDC_DEFAULT_SCRAMBLE_INTERVAL;
    }
    if (attr->scramble_timeout < SCDC_DEFAULT_SCRAMBLE_TIMEOUT) {
        attr->scramble_timeout = SCDC_DEFAULT_SCRAMBLE_TIMEOUT;
    }
    if (attr->scramble_timeout > SCDC_MAX_SCRAMBLE_TIMEOUT) {
        attr->scramble_timeout = SCDC_MAX_SCRAMBLE_TIMEOUT;
    }
}

static td_void scdc_pre_scramble(hdmi_device_id hdmi_id, scdc_infomation *scdc_info)
{
    ddc_cfg cfg = {0};
    td_u8 ddc_byte = 0;

    /* read sink version */
    hal_hdmi_ddc_default_cfg_get(hdmi_id, &cfg);
    cfg.data_size  = 1;
    cfg.data       = &ddc_byte;
    cfg.func_type  = DDC_FUNC_TYPE_SCDC;
    cfg.offset     = SCDC_OFFSET_SINK_VERSION;
    cfg.issue_mode = DDC_MODE_READ_MUTIL_NO_ACK;
    hal_hdmi_ddc_issue(hdmi_id, &cfg);

    scdc_info->status.reg_static.sink_version = ddc_byte;
    if (ddc_byte != SCDC_HDMI20_VERSION) {
        scdc_info->status.err_cnt.sink_un_read_cnt++;
        hdmi_warn("scdc sink version is not %u\n", SCDC_HDMI20_VERSION);
    }
    /* RR_Enable disable */
    scdc_info->attr.sink_read_quest = TD_FALSE;
    scdc_info->status.reg_static.config0.byte = 0;
    scdc_info->status.reg_static.config0.u8.rr_enable = TD_FALSE;
    cfg.offset     = SCDC_OFFSET_CONFIG_0;
    cfg.issue_mode = DDC_MODE_WRITE_MUTIL_ACK;
    cfg.data       = &scdc_info->status.reg_static.config0.byte;
    hal_hdmi_ddc_issue(hdmi_id, &cfg);

    scdc_info->status.reg_static.rr_test_cfg.byte = 0;
    cfg.offset     = SCDC_OFFSET_TEST_CONFIG_0;
    cfg.issue_mode = DDC_MODE_WRITE_MUTIL_ACK;
    cfg.data       = &scdc_info->status.reg_static.rr_test_cfg.byte;
    hal_hdmi_ddc_issue(hdmi_id, &cfg);

    return;
}

static td_void scdc_err_record(scdc_infomation *info, scdc_error_type err)
{
    errno_t ret;
    scdc_error_record *err_cord = TD_NULL;

    err_cord = &info->status.err_record[info->status.err_record_ptr];
    err_cord->err_type = err;
    err_cord->time_stamp = hal_hdmi_mach_ms_get();
    ret = memcpy_s(&err_cord->err_attr, sizeof(err_cord->err_attr), &info->attr, sizeof(scdc_attr));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return;
    }
    switch (err_cord->err_type) {
        case SCDC_ERROR_SINK_UNREAD:
            info->status.err_cnt.sink_un_read_cnt++;
            break;
        case SCDC_ERROR_SCRAMBLE:
            info->status.err_cnt.sramble_fail_cnt++;
            break;
        case SCDC_ERROR_OE_DISABLE:
            info->status.err_cnt.oe_disable_fail_cnt++;
            break;
        case SCDC_ERROR_OE_ENABLE:
            info->status.err_cnt.oe_enable_fail_cnt++;
            break;
        default:
            break;
    }
    info->status.err_record_ptr = (info->status.err_record_ptr + 1) % SCDC_MAX_ERROR_RECORD_NUM;

    return;
}

static td_void scdc_oe_set(td_bool oe_en, td_u32 timeout, scdc_infomation *info)
{
    td_bool oe_enable = TD_FALSE;

#ifndef HDMI_FPGA_SUPPORT
#ifdef HDMI_PRODUCT_SS928V100
    hal_hdmi_phy_oe_set(info->status.hdmi_id, oe_en);
    osal_msleep(timeout);
    hal_hdmi_phy_oe_get(info->status.hdmi_id, &oe_enable);
#else
    hal_hdmi_phy_oe_set(oe_en);
    osal_msleep(timeout);
    hal_hdmi_phy_oe_get(&oe_enable);
#endif
#endif

    if (oe_enable != oe_en) {
        if (oe_en == TD_TRUE) {
            if (info->status.err_cnt.oe_enable_fail_cnt < SCDC_ERROR_MAX_NUM_OE_ENABLE) {
                scdc_err_record(info, SCDC_ERROR_OE_ENABLE);
            }
        } else {
            if (info->status.err_cnt.oe_disable_fail_cnt < SCDC_ERROR_MAX_NUM_OE_DISABLE) {
                scdc_err_record(info, SCDC_ERROR_OE_DISABLE);
            }
        }
        hdmi_err("scdc oe oe_enable=%u fail, expected %u\n", oe_enable, oe_en);
    }

    return;
}

td_s32 hal_hdmi_scdc_init(hdmi_device_id hdmi_id)
{
    td_s32 ret = TD_SUCCESS;
    scdc_infomation *scdc_info = scdc_info_ptr_get(hdmi_id);
    hdmi_mach_ctrl   mach_ctrl = {0};

    hdmi_if_null_warn_return(scdc_info, TD_FAILURE);

    if (!scdc_info->status.init) {
        mach_ctrl.data     = scdc_info;
        mach_ctrl.interval = SCDC_DEFAULT_POLL_INTERVAL;
        mach_ctrl.callback = scdc_mach_poll_callback;
        mach_ctrl.name     = "SCDC";
        (td_void)memset_s(scdc_info, sizeof(scdc_infomation), 0, sizeof(scdc_infomation));
        scdc_info->status.init          = TD_TRUE;
        scdc_info->status.hdmi_id       = hdmi_id;
        scdc_info->status.mach_interval = mach_ctrl.interval;
        ret = hal_hdmi_mach_register(&mach_ctrl, &scdc_info->status.mach_id);
    }

    return ret;
}

td_s32 hal_hdmi_scdc_deinit(hdmi_device_id hdmi_id)
{
    td_s32 ret;
    scdc_infomation *scdc_info = scdc_info_ptr_get(hdmi_id);

    hdmi_if_null_warn_return(scdc_info, TD_FAILURE);
    hdmi_if_false_warn_return(scdc_info->status.init, TD_FAILURE);
    ret = hal_hdmi_mach_unregister(scdc_info->status.mach_id);
    (td_void)memset_s(scdc_info, sizeof(scdc_infomation), 0, sizeof(scdc_infomation));

    return ret;
}

td_s32 hal_hdmi_scdc_attr_set(hdmi_device_id hdmi_id, scdc_attr *attr)
{
    td_s32 ret;
    td_u32 scdc_timestart;
    td_u32 scdc_time_cur;
    td_u32 timeout;
    scdc_scramble scramble = {0};
    scdc_infomation *scdc_info = scdc_info_ptr_get(hdmi_id);

    hdmi_if_null_warn_return(attr, TD_FAILURE);
    hdmi_if_null_warn_return(scdc_info, TD_FAILURE);
    hdmi_if_false_warn_return(scdc_info->status.init, TD_FAILURE);
    scdc_attr_param_check(attr);

    ret = memcpy_s(&scdc_info->attr, sizeof(scdc_info->attr), attr, sizeof(scdc_attr));
    hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);
    scdc_pre_scramble(hdmi_id, scdc_info);
    osal_msleep(SCDC_DEFAULT_SCRAMBLE_INTERVAL);
    /* scrameble cfg */
    timeout = scdc_info->attr.scramble_interval;
    scdc_timestart = hdmi_osal_get_time_in_ms();

    do {
        /* oe disable */
        scdc_oe_set(TD_FALSE, SCDC_OE_DISABLE_WAIT_TIME, scdc_info);
        /* sink & source scramble config */
        scramble.src_scramble      = scdc_info->attr.src_scramble;
        scramble.sink_scramble     = scdc_info->attr.sink_scramble;
        scramble.tmds_clk_ratio40x = scdc_info->attr.tmds_clk_ratio40x;
        scdc_scramble_set(hdmi_id, &scramble);
        /* oe enable */
#ifndef HDMI_FPGA_SUPPORT
        osal_msleep(timeout);
#endif
        scdc_oe_set(TD_TRUE, timeout, scdc_info);
        /* poll status */
        scdc_scramble_get(hdmi_id, &scramble);
        osal_msleep(SCDC_GET_SCRAMBLE_STATUS_TIME);
        /* compatibility try again */
        ret = TD_SUCCESS;
        if ((scramble.tmds_clk_ratio40x != scdc_info->attr.tmds_clk_ratio40x)) {
            scdc_err_record(scdc_info, SCDC_ERROR_SCRAMBLE);
            ret = TD_FAILURE;
        }
        timeout += scdc_info->attr.scramble_interval;
        scdc_time_cur = hdmi_osal_get_time_in_ms();
    } while ((ret != TD_SUCCESS) && (timeout <= scdc_info->attr.scramble_timeout) &&
             ((scdc_time_cur - scdc_timestart) < SCDC_WHILE_SCRAMBLE_TIMEOUT));

    hdmi_info("\ndelay_interval=%u, timeout=%u, src_scarmble=%u, sink_scramble=%u, tmds_clk_ratio40x=%u\n", timeout,
        (scdc_time_cur - scdc_timestart), scramble.src_scramble, scramble.sink_scramble, scramble.tmds_clk_ratio40x);

    if (ret != TD_SUCCESS) {
        hdmi_err("scdc scramble %s fail!\n", scdc_info->attr.sink_scramble ? "ON" : "OFF");
    } else {
        hdmi_info("scdc scramble %s success!\n", scdc_info->attr.sink_scramble ? "ON" : "OFF");
    }

    if ((ret != TD_SUCCESS) || scdc_info->attr.sink_scramble == TD_FALSE) {
        ret += hal_hdmi_mach_stop(scdc_info->status.mach_id);
        scdc_info->status.mach_enable = TD_FALSE;
    }

    return (ret != TD_SUCCESS) ? TD_FAILURE : TD_SUCCESS;
}

td_s32 hal_hdmi_scdc_attr_get(hdmi_device_id hdmi_id, scdc_attr *attr)
{
    errno_t ret;
    scdc_scramble scramble = {0};
    scdc_infomation *scdc_info = scdc_info_ptr_get(hdmi_id);

    hdmi_if_null_warn_return(attr, TD_FAILURE);
    hdmi_if_null_warn_return(scdc_info, TD_FAILURE);
    hdmi_if_false_warn_return(scdc_info->status.init, TD_FAILURE);

    scdc_scramble_get(hdmi_id, &scramble);
    scdc_info->attr.sink_scramble     = scramble.sink_scramble;
    scdc_info->attr.src_scramble      = scramble.src_scramble;
    scdc_info->attr.tmds_clk_ratio40x = scramble.tmds_clk_ratio40x;
    ret = memcpy_s(attr, sizeof(*attr), &scdc_info->attr, sizeof(scdc_attr));
    hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);

    return TD_SUCCESS;
}

