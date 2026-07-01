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

#include "drv_vo_layer_comm.h"
#include "drv_vo.h"
#include "drv_vo_coef_org_comm.h"
#include "hal_vo_layer_comm.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")
#if vo_desc("layer csc")

td_s32 vo_drv_get_csc_matrix(ot_vo_csc_matrix csc_matrix, const csc_coef **csc_tmp)
{
    *csc_tmp = vo_get_csc_coef(csc_matrix);
    if (*csc_tmp == TD_NULL) {
        return OT_ERR_VO_NULL_PTR;
    }
    return TD_SUCCESS;
}

td_void vo_drv_calculate_yuv2rgb(const hal_csc_value *csc_value, const csc_coef *csc_tmp, csc_coef *coef)
{
    td_s32 luma;
    td_s32 contrast;
    td_s32 hue;
    td_s32 satu;
    const td_s32 csc_value_times = 100;
    const td_s32 table_times = 1000;
    td_s32 square_cv_times = csc_value_times * csc_value_times;
    const td_s32 *cos_table = vo_get_cos_table();
    const td_s32 *sin_table = vo_get_sin_table();

    luma = csc_value->luma;
    contrast = csc_value->cont;
    hue = csc_value->hue;
    satu = csc_value->satu;

    /* yuv->rgb */
    coef->csc_coef00 = (contrast * csc_tmp->csc_coef00) / csc_value_times;
    coef->csc_coef01 = (contrast * satu * ((csc_tmp->csc_coef01 * cos_table[hue] - csc_tmp->csc_coef02 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef02 = (contrast * satu * ((csc_tmp->csc_coef01 * sin_table[hue] + csc_tmp->csc_coef02 *
                                            cos_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef10 = (contrast * csc_tmp->csc_coef10) / csc_value_times;
    coef->csc_coef11 = (contrast * satu * ((csc_tmp->csc_coef11 * cos_table[hue] - csc_tmp->csc_coef12 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef12 = (contrast * satu * ((csc_tmp->csc_coef11 * sin_table[hue] + csc_tmp->csc_coef12 *
                                            cos_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef20 = (contrast * csc_tmp->csc_coef20) / csc_value_times;
    coef->csc_coef21 = (contrast * satu * ((csc_tmp->csc_coef21 * cos_table[hue] - csc_tmp->csc_coef22 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef22 = (contrast * satu * ((csc_tmp->csc_coef21 * sin_table[hue] + csc_tmp->csc_coef22 *
                                            cos_table[hue]) / table_times)) / square_cv_times;
    coef->csc_in_dc0 += ((contrast != 0) ? (luma * 100 / contrast) : (luma * 100));  /* 100 : trans coef */
}

td_void vo_drv_calculate_rgb2yuv(const hal_csc_value *csc_value, const csc_coef *csc_tmp, csc_coef *coef)
{
    td_s32 luma;
    td_s32 contrast;
    td_s32 hue;
    td_s32 satu;
    const td_s32 csc_value_times = 100;
    const td_s32 table_times = 1000;
    td_s32 square_cv_times = csc_value_times * csc_value_times;
    const td_s32 *cos_table = vo_get_cos_table();
    const td_s32 *sin_table = vo_get_sin_table();

    luma = csc_value->luma;
    contrast = csc_value->cont;
    hue = csc_value->hue;
    satu = csc_value->satu;

    /* rgb->yuv or yuv->yuv */
    coef->csc_coef00 = (contrast * csc_tmp->csc_coef00) / csc_value_times;
    coef->csc_coef01 = (contrast * csc_tmp->csc_coef01) / csc_value_times;
    coef->csc_coef02 = (contrast * csc_tmp->csc_coef02) / csc_value_times;
    coef->csc_coef10 = (contrast * satu * ((csc_tmp->csc_coef10 * cos_table[hue] + csc_tmp->csc_coef20 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef11 = (contrast * satu * ((csc_tmp->csc_coef11 * cos_table[hue] + csc_tmp->csc_coef21 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef12 = (contrast * satu * ((csc_tmp->csc_coef12 * cos_table[hue] + csc_tmp->csc_coef22 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef20 = (contrast * satu * ((csc_tmp->csc_coef20 * cos_table[hue] - csc_tmp->csc_coef10 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef21 = (contrast * satu * ((csc_tmp->csc_coef21 * cos_table[hue] - csc_tmp->csc_coef11 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef22 = (contrast * satu * ((csc_tmp->csc_coef22 * cos_table[hue] - csc_tmp->csc_coef12 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_out_dc0 += luma;
}

td_void vou_drv_calc_csc_matrix(const ot_vo_csc *csc, ot_vo_csc_matrix csc_matrix, csc_coef *coef)
{
    td_s32 ret;
    const csc_coef *csc_tmp = TD_NULL;
    hal_csc_value csc_value;

    if (csc->ex_csc_en == TD_FALSE) {
        csc_value.luma = (td_s32)csc->luma * 64 / 100 - 32; /* 64: -32~32 100: trans coef  */
    } else {
        csc_value.luma = (td_s32)csc->luma * 256 / 100 - 128; /* 256: -128~128 128 100  */
    }

    csc_value.cont = ((td_s32)csc->contrast - 50) * 2 + 100; /* 50 2 100 trans coef */
    csc_value.hue = (td_s32)csc->hue * 60 / 100;             /* 60 100 trans coef */
    csc_value.satu = ((td_s32)csc->saturation - 50) * 2 + 100; /* 50 2 100 trans coef */

    ret = vo_drv_get_csc_matrix(csc_matrix, &csc_tmp);
    if (ret != TD_SUCCESS) {
        return;
    }

    coef->csc_in_dc0 = csc_tmp->csc_in_dc0;
    coef->csc_in_dc1 = csc_tmp->csc_in_dc1;
    coef->csc_in_dc2 = csc_tmp->csc_in_dc2;
    coef->csc_out_dc0 = csc_tmp->csc_out_dc0;
    coef->csc_out_dc1 = csc_tmp->csc_out_dc1;
    coef->csc_out_dc2 = csc_tmp->csc_out_dc2;

    /*
     * c_ratio的调节范围一般是0～1.99, c_ratio=contrast/100
     * S的调节范围一般为0~1.99, S=satu/100
     * 色调调节参数的范围一般为-30°~30°, 通过查表法求得COS和SIN值并/1000
     */
    if ((csc_matrix >= OT_VO_CSC_MATRIX_BT601LIMIT_TO_RGBFULL) &&
        (csc_matrix <= OT_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT)) {
        vo_drv_calculate_yuv2rgb(&csc_value, csc_tmp, coef);
    } else {
        vo_drv_calculate_rgb2yuv(&csc_value, csc_tmp, coef);
    }
}

td_void vo_drv_layer_get_csc_param(ot_vo_layer layer, csc_coef_param *csc_param)
{
    vo_drv_layer *drv_layer_ctx = vo_drv_get_layer_ctx(layer);
    (td_void)memcpy_s(csc_param, sizeof(csc_coef_param), &(drv_layer_ctx->csc_param),
        sizeof(csc_coef_param));
}

#endif
#endif /* #if vo_desc("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
