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

#include "drv_vo_comm.h"
#include "drv_vo.h"
#include "hal_vo.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT")

hal_disp_syncinfo g_sync_timing[OT_VO_OUT_BUTT] = {
/*
 * |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-|
 * syncm,iop, itf, vact, vbb,  vfb,  hact,  hbb,  hfb, hmid,bvact,bvbb,bvfb, hpw, vpw,idv, ihs, ivs
 */
    { 0,   0,   0,  288,  22,    2,   720,  132,   12,    1,  288,  23,   2, 126,   3,  0, 0, 0 }, /* 576I(PAL)  */
    { 0,   0,   0,  240,  18,    4,   720,  119,   19,    1,  240,  19,   4, 124,   3,  0, 0, 0 }, /* 480I(NTSC) */
    { 0,   0,   0,  288,  22,    2,   960,  176,   16,    1,  288,  23,   2, 168,   3,  0, 0, 0 }, /* 960H(PAL) */
    { 0,   0,   0,  240,  18,    4,   960,  163,   21,    1,  240,  19,   4, 168,   3,  0, 0, 0 }, /* 960H(NTSC) */

    /* 640*480@60_hz CVT */
    { 1,   1,   2,  480,  35,   10,   640,  144,   16,    1,    1,   1,   1,  96,   2,  0, 1, 1 },
    { 1,   1,   1,  480,  36,    9,   720,  122,   16,    1,    1,   1,   1,  62,   6,  0, 0, 0 }, /* 480P@60_hz */
    { 1,   1,   1,  576,  44,    5,   720,  132,   12,    1,    1,   1,   1,  64,   5,  0, 0, 0 }, /* 576P@50_hz */
    /* 800*600@60_hz VGA@60_hz */
    { 1,   1,   2,  600,  27,    1,   800,  216,   40,    1,    1,   1,   1, 128,   4,  0, 0, 0 },
    { 1,   1,   2,  768,  35,    3,  1024,  296,   24,    1,    1,   1,   1, 136,   6,  0, 1, 1 }, /* 1024x768@60_hz */
    { 0,   1,   1,  720,  25,    5,  1280,  260,  440,    1,    1,   1,   1,  40,   5,  0, 0, 0 }, /* 720P@50_hz */
    { 0,   1,   1,  720,  25,    5,  1280,  260,  110,    1,    1,   1,   1,  40,   5,  0, 0, 0 }, /* 720P@60_hz */
    /* 1280*800@60_hz VGA@60_hz */
    { 1,   1,   2,  800,  28,    3,  1280,  328,   72,    1,    1,   1,   1, 128,   6,  0, 1, 0 },
    { 1,   1,   2, 1024,  41,    1,  1280,  360,   48,    1,    1,   1,   1, 112,   3,  0, 0, 0 }, /* 1280x1024@60_hz */
    { 1,   1,   2,  768,  27,    3,  1366,  356,   70,    1,    1,   1,   1, 143,   3,  0, 0, 0 }, /* 1366x768@60_hz */
    { 1,   1,   2, 1050,  36,    3,  1400,  376,   88,    1,    1,   1,   1, 144,   4,  0, 0, 0 }, /* 1400x1050@60_hz */
    { 1,   1,   2,  900,  31,    3,  1440,  384,   80,    1,    1,   1,   1, 152,   6,  0, 1, 0 }, /* 1440x900@60_hz */
    { 1,   1,   2, 1050,  36,    3,  1680,  456,  104,    1,    1,   1,   1, 176,   6,  0, 1, 0 }, /* 1680*1050@60_hz */

    { 0,   1,   1, 1080,  41,    4,  1920,  192,  638,    1,    1,   1,   1,  44,   5,  0, 0, 0 }, /* 1080P@24_hz */
    { 0,   1,   1, 1080,  41,    4,  1920,  192,  528,    1,    1,   1,   1,  44,   5,  0, 0, 0 }, /* 1080P@25_hz */
    { 0,   1,   1, 1080,  41,    4,  1920,  192,   88,    1,    1,   1,   1,  44,   5,  0, 0, 0 }, /* 1080P@30_hz */
    { 0,   0,   1,  540,  20,    2,  1920,  192,  528, 1128,  540,  21,   2,  44,   5,  0, 0, 0 }, /* 1080I@50_hz */
    { 0,   0,   1,  540,  20,    2,  1920,  192,   88,  908,  540,  21,   2,  44,   5,  0, 0, 0 }, /* 1080I@60_hz */
    { 0,   1,   1, 1080,  41,    4,  1920,  192,  528,    1,    1,   1,   1,  44,   5,  0, 0, 0 }, /* 1080P@50_hz */
    { 0,   1,   1, 1080,  41,    4,  1920,  192,   88,    1,    1,   1,   1,  44,   5,  0, 0, 0 }, /* 1080P@60_hz */

    { 1,   1,   2, 1200,  49,    1,  1600,  496,   64,    1,    1,   1,   1, 192,   3,  0, 0, 0 }, /* 1600*1200@60_hz */
    /* 1920*1200@60_hz CVT (reduced blanking) */
    { 1,   1,   2, 1200,  32,    3,  1920,  112,   48,    1,    1,   1,   1,  32,   6,  0, 0, 1 },
    { 0,   1,   1, 2160,  72,    8,  1920,  192,   88,    1,    1,   1,   1,  44,   5,  0, 0, 0 }, /* 1920*2160@30_hz */
    { 1,   1,   2, 1440,  39,    2,  2560,  112,   48,    1,    1,   1,   1,  32,   5,  0, 0, 0 }, /* 2560*1440@30_hz */
    { 1,   1,   2, 1440,  39,    2,  2560,  112,   48,    1,    1,   1,   1,  32,   5,  0, 0, 0 }, /* 2560*1440@60_hz */
    /* 2560*1600@60_hz CVT (reduced blanking) */
    { 0,   1,   2, 1600,  43,    3,  2560,  112,   48,    1,    1,   1,   1,  32,   6,  0, 0, 1 },
    { 0,   1,   1, 2160,  82,    8,  3840,  384, 1276,    1,    1,   1,   1,  88,  10,  0, 0, 0 }, /* 3840*2160@24_hz */
    { 0,   1,   1, 2160,  82,    8,  3840,  384, 1056,    1,    1,   1,   1,  88,  10,  0, 0, 0 }, /* 3840*2160@25_hz */
    { 0,   1,   1, 2160,  82,    8,  3840,  384,  176,    1,    1,   1,   1,  88,  10,  0, 0, 0 }, /* 3840*2160@30_hz */
    { 0,   1,   1, 2160,  82,    8,  3840,  384, 1056,    1,    1,   1,   1,  88,  10,  0, 0, 0 }, /* 3840*2160@50_hz */
    { 0,   1,   1, 2160,  82,    8,  3840,  384,  176,    1,    1,   1,   1,  88,  10,  0, 0, 0 }, /* 3840*2160@60_hz */

    { 0,   1,   1, 2160,  82,    8,  4096,  384, 1020,    1,    1,   1,   1,  88,  10,  0, 0, 0 }, /* 4096x2160@24 */
    { 0,   1,   1, 2160,  82,    8,  4096,  216,  968,    1,    1,   1,   1,  88,  10,  0, 0, 0 }, /* 4096x2160@25 */
    { 0,   1,   1, 2160,  82,    8,  4096,  216,   88,    1,    1,   1,   1,  88,  10,  0, 0, 0 }, /* 4096x2160@30 */
    { 0,   1,   1, 2160,  82,    8,  4096,  216,  968,    1,    1,   1,   1,  88,  10,  0, 0, 0 }, /* 4096x2160@50 */
    { 0,   1,   1, 2160,  82,    8,  4096,  216,   88,    1,    1,   1,   1,  88,  10,  0, 0, 0 }, /* 4096x2160@60 */
    { 0,   1,   1, 4320,  64,   16,  7680,  768,  552,    1,    1,   1,   1, 176,  20,  0, 0, 0 }, /* 7680x4320@30 */

    /* 240X320@50  6bit LCD */
    { 0,   1,   1,  320,  10,    4,   240,   30,   10,    1,    1,   1,   1,  10,   2,  0, 0, 0 },
    /* 320X240@50  6bit LCD */
    { 0,   1,   1,  240,   2,    2,   320,    5,   10,    1,    1,   1,   1,  10,   1,  0, 0, 0 },
    /* 240X320@60 16bit LCD */
    { 0,   1,   1,  320,   4,    8,   240,   20,   10,    1,    1,   1,   1,   2,   2,  0, 0, 0 },
    /* 320X240@60  8bit LCD */
    { 0,   1,   1,  240,  15,    9,   320,   65,    7,    1,  240,  14,   9,   1,   1,  0, 0, 0 },
    /* 800X600@60 24bit LCD */
    { 0,   1,   1,  600,  23,   12,   800,  210,   46,    1,    1,   1,   1,   2,   1,  0, 0, 0 },

    /* for MIPI DSI tx 720 x1280 at 60 hz */
    { 0,   1,   1, 1280,  24,    8,   720,  123,   99,    1,    1,   1,   1,  24,   4,  0, 0, 0 },
    /* for MIPI DSI tx 1080 x1920 at 60 hz */
    { 0,   1,   1, 1920,  36,   16,  1080,   28,  130,    1,    1,   1,   1,   8,  10,  0, 0, 0 },
    {} /* user sync info by user, empty configuration */
};

hal_disp_syncinfo *vo_drv_comm_get_sync_timing(ot_vo_intf_sync sync)
{
    return &g_sync_timing[sync];
}

td_s32 vo_drv_check_cvbs_sync(ot_vo_dev dev, ot_vo_intf_type intf_type, ot_vo_intf_sync intf_sync)
{
    if (intf_type & OT_VO_INTF_CVBS) {
        if ((intf_sync != OT_VO_OUT_PAL) && (intf_sync != OT_VO_OUT_NTSC)) {
            vo_err_trace("for CVBS interface, vo%d's intfsync %d illegal! only support PAL/NTSC\n", dev, intf_sync);
            return OT_ERR_VO_NOT_SUPPORT;
        }
    }

    return TD_SUCCESS;
}

hal_disp_pixel_format vo_drv_convert_data_format(vou_layer_pixel_format data_fmt)
{
    hal_disp_pixel_format pixel_format;

    if (data_fmt == VOU_LAYER_PIXEL_FORMAT_SP_YCBCR_420) {
        pixel_format = HAL_INPUTFMT_YCBCR_SEMIPLANAR_420;
    } else if (data_fmt == VOU_LAYER_PIXEL_FORMAT_SP_YCBCR_422) {
        pixel_format = HAL_INPUTFMT_YCBCR_SEMIPLANAR_422;
    } else {
        pixel_format = HAL_INPUTFMT_YCBCR_SEMIPLANAR_400;
    }

    return pixel_format;
}

td_void vo_drv_board_init(td_void)
{
    hal_vo_init();
}

td_void vo_drv_csc_trans_to_register(csc_coef *coef)
{
    const td_s32 dc_precision = 4; /* 4: reg precision is 10bit coef dc precision, need trans 10-8=2,2^2=4 */

    /* csc coef is 1024 precision, no need trans */
    coef->csc_in_dc0 = dc_precision * coef->csc_in_dc0;
    coef->csc_in_dc1 = dc_precision * coef->csc_in_dc1;
    coef->csc_in_dc2 = dc_precision * coef->csc_in_dc2;

    coef->csc_out_dc0 = dc_precision * coef->csc_out_dc0;
    coef->csc_out_dc1 = dc_precision * coef->csc_out_dc1;
    coef->csc_out_dc2 = dc_precision * coef->csc_out_dc2;
}

#endif /* #if vo_desc("UBOOT") */

#if vo_desc("KERNEL_VO")

td_void vo_drv_board_deinit(td_void)
{
    hal_vo_deinit();
}

#ifdef CONFIG_OT_VO_VGS
#ifdef CONFIG_OT_VO_CHN_ASPECT_RATIO

static td_void vou_drv_calc_auto_aspect_ratio(const ot_video_frame *src_video_frame, const ot_rect *out_rect,
                                              vgs_online_opt *online_opt)
{
    td_u32 src_width;
    td_u32 src_height;
    td_u32 dst_width;
    td_u32 dst_height;
    td_u32 src_common_divisor;
    td_u32 dst_common_divisor;
    td_bool aspect_ratio;

    src_width = src_video_frame->width;
    src_height = src_video_frame->height;
    dst_width = out_rect->width;
    dst_height = out_rect->height;

    src_common_divisor = src_width * dst_height;
    dst_common_divisor = dst_width * src_height;

    /* 左右加黑边 */
    if (src_common_divisor < dst_common_divisor) {
        aspect_ratio = TD_TRUE;
        online_opt->aspect_ratio_opt.video_rect.width = dst_height * src_width / src_height;
        online_opt->aspect_ratio_opt.video_rect.height = dst_height;
        online_opt->aspect_ratio_opt.video_rect.x =
            (dst_width - online_opt->aspect_ratio_opt.video_rect.width) / 2; /* 2: 取宽度一半 */
        online_opt->aspect_ratio_opt.video_rect.y = 0;
    } else if (src_common_divisor == dst_common_divisor) {
        /* 不加黑边 */
        aspect_ratio = TD_FALSE;
        online_opt->aspect_ratio_opt.video_rect.width = dst_width;
        online_opt->aspect_ratio_opt.video_rect.height = dst_height;
        online_opt->aspect_ratio_opt.video_rect.x = 0;
        online_opt->aspect_ratio_opt.video_rect.y = 0;
    } else {
        /* 上下加黑边 */
        aspect_ratio = TD_TRUE;
        online_opt->aspect_ratio_opt.video_rect.width = dst_width;
        online_opt->aspect_ratio_opt.video_rect.height = dst_width * src_height / src_width;
        online_opt->aspect_ratio_opt.video_rect.x = 0;
        online_opt->aspect_ratio_opt.video_rect.y =
            (dst_height - online_opt->aspect_ratio_opt.video_rect.height) / 2; /* 2: 取高度一半 */
    }

    online_opt->aspect_ratio = aspect_ratio;
}

#endif
#endif

td_void vo_drv_dev_info_de_init(td_void)
{
    vo_coef_addr *coef_addr = vo_drv_get_coef_buf_addr();
    vo_drv_delete_mem(&coef_addr->buf_base_addr);
    (td_void)memset_s(coef_addr->coef_vir_addr, sizeof(coef_addr->coef_vir_addr), 0,
        sizeof(coef_addr->coef_vir_addr));
    (td_void)memset_s(coef_addr->coef_phys_addr, sizeof(coef_addr->coef_phys_addr), 0,
        sizeof(coef_addr->coef_phys_addr));
}

/* 缩放相关系数是视频层缩放和wbc缩放共用,先使用wbc特性宏隔离下 */
#ifdef CONFIG_OT_VO_WBC

static td_void vo_drv_push128(vo_drv_u128 *data128, td_u32 coef_data, td_u32 bit_len)
{
    const td_u32 bits_per_bytes = 32;
    const td_u32 bits_per_line = 128;
    const td_u32 data1_start = 32;
    const td_u32 data2_start = 64;
    const td_u32 data3_start = 96;

    coef_data = coef_data & (0xFFFFFFFF >> (bits_per_bytes - bit_len));

    if (data128->depth < data1_start) {
        if ((data128->depth + bit_len) <= data1_start) {
            data128->data0 = (coef_data << data128->depth) | data128->data0;
        } else {
            data128->data0 = (coef_data << data128->depth) | data128->data0;
            data128->data1 = coef_data >> (bits_per_bytes - (data128->depth % bits_per_bytes));
        }
    } else if ((data128->depth >= data1_start) && (data128->depth < data2_start)) {
        if ((data128->depth + bit_len) <= data2_start) {
            data128->data1 = (coef_data << (data128->depth % bits_per_bytes)) | data128->data1;
        } else {
            data128->data1 = (coef_data << (data128->depth % bits_per_bytes)) | data128->data1;
            data128->data2 = coef_data >> (bits_per_bytes - (data128->depth % bits_per_bytes));
        }
    } else if ((data128->depth >= data2_start) && (data128->depth < data3_start)) {
        if ((data128->depth + bit_len) <= data3_start) {
            data128->data2 = (coef_data << (data128->depth % bits_per_bytes)) | data128->data2;
        } else {
            data128->data2 = (coef_data << (data128->depth % bits_per_bytes)) | data128->data2;
            data128->data3 = coef_data >> (bits_per_bytes - (data128->depth % bits_per_bytes));
        }
    } else if (data128->depth >= data3_start) {
        if ((data128->depth + bit_len) <= bits_per_line) {
            data128->data3 = (coef_data << (data128->depth % bits_per_bytes)) | data128->data3;
        }
    }

    data128->depth = data128->depth + bit_len;

    if (data128->depth <= bits_per_line) {
        return;
    }
}

static td_void vo_drv_write_ddr(td_u8 *addr, const vo_drv_u128 *data128)
{
    td_u32 ii;
    td_u32 data_arr[4] = { data128->data0, data128->data1, data128->data2, data128->data3 }; /* 4: 4 data */
    td_u8 *addr_tmp = TD_NULL;
    td_u32 u_data_temp;

    for (ii = 0; ii < 4; ii++) { /* 4: 4 data */
        addr_tmp = addr + ii * 4; /* 4: 4 bytes */
        u_data_temp = data_arr[ii];
        *(td_u32 *)addr_tmp = u_data_temp;
    }
}

static td_u32 vo_drv_find_max(const td_u32 *array, td_u32 num)
{
    td_u32 ii;
    td_u32 tmp_data = array[0];

    for (ii = 1; ii < num; ii++) {
        if (tmp_data < array[ii]) {
            tmp_data = array[ii];
        }
    }

    return tmp_data;
}

td_void vo_drv_zme_exchange_lut(td_u32 phase,
                                td_u32 tap,
                                const td_s16 *old_lut,
                                td_s16 new_lut[VO_MAX_ZME_PHASE][VO_MAX_ZME_TAP])
{
    td_u32 ii;
    td_u32 jj;

    for (ii = 0; ii < phase; ii++) {
        for (jj = 0; jj < tap; jj++) {
            new_lut[ii][jj] = *(old_lut + (ii * VO_MAX_ZME_TAP + tap - 1 - jj));
        }
    }
}

vou_zoom_coef vo_drv_get_zoom_coef(td_u32 in, td_u32 out)
{
    vou_zoom_coef zoom_coef;
    td_u32 r_int;
    td_u32 r_dec;

    if (in == 0) {
        return VOU_ZOOM_COEF_BUTT;
    }
    r_int = out / in;
    r_dec = ((out % in) * ZME_HPREC) / in; /* 注意数据溢出 out%in 4096之内 */

    /*
     * 自动配置系数
     * 放大，倍数>=2，不区分系数
     */
    if (r_int > 1) {
        zoom_coef = VOU_ZOOM_COEF_UP_1;
    } else if (r_int == 1) {
        zoom_coef = VOU_ZOOM_COEF_UP_1;
        /* 区分、细分系数 */
    } else if (r_dec >= (75 * ZME_HPREC / 100)) { /* 75, 100: 0.75 */
        zoom_coef = VOU_ZOOM_COEF_075;
    } else if (r_dec >= (666 * ZME_HPREC / 1000)) { /* 666, 1000: 0.666 */
        zoom_coef = VOU_ZOOM_COEF_0666;
    } else if (r_dec >= (50 * ZME_HPREC / 100)) { /* 50, 100: 0.50 */
        zoom_coef = VOU_ZOOM_COEF_05;
    } else if (r_dec >= (40 * ZME_HPREC / 100)) { /* 40, 100: 0.40 */
        zoom_coef = VOU_ZOOM_COEF_04;
    } else if (r_dec >= (375 * ZME_HPREC / 1000)) { /* 375, 1000: 0.375 */
        zoom_coef = VOU_ZOOM_COEF_0375;
    } else if (r_dec >= (33 * ZME_HPREC / 100)) { /* 33, 100: 0.33 */
        zoom_coef = VOU_ZOOM_COEF_033;
    } else {
        zoom_coef = VOU_ZOOM_COEF_0;
    }

    return zoom_coef;
}

#endif

td_void vo_func_zme_exchange_lut(td_u32 phase,
                                 td_u32 tap,
                                 td_s16 old_lut[VO_MAX_ZME_PHASE][VO_MAX_ZME_TAP],
                                 td_s16 new_lut[VO_MAX_ZME_PHASE][VO_MAX_ZME_TAP])
{
    td_u32 ii;
    td_u32 jj;

    for (ii = 0; ii < phase; ii++) {
        for (jj = 0; jj < tap; jj++) {
            new_lut[ii][jj] = old_lut[ii][tap - 1 - jj];
        }
    }
}

td_void vo_drv_set_reg(volatile reg_vdp_regs *reg)
{
    vo_hal_set_reg(reg);
}

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
