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

#include "drv_vo_coef_org_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

const csc_coef g_csc_identity_limit = {
    /* csc coef */
    1024, 0, 0, 0, 1024, 0, 0, 0, 1024,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    16, 128, 128
};

const csc_coef g_csc_identity_full = {
    /* csc coef */
    1024, 0, 0, 0, 1024, 0, 0, 0, 1024,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    0, 128, 128
};

const csc_coef g_csc_yuv601full_to_yuv601limit = {
    /* csc coef */
    880, 0, 0, 0, 899, 0, 0, 0, 899,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    16, 128, 128
};

const csc_coef g_csc_yuv709limit_to_yuv601limit = {
    /* csc coef */
    1024, 102, 196, 0, 1014, -113, 0, -74, 1007,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    16, 128, 128
};

const csc_coef g_csc_yuv709full_to_yuv601limit = {
    /* csc coef */
    879, 89, 172, 0, 890, -100, 0, -65, 885,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    16, 128, 128
};

const csc_coef g_csc_yuv601limit_to_yuv709limit = {
    /* csc coef */
    1024, -118, -213, 0, 1043, 117, 0, 77, 1050,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    16, 128, 128
};

const csc_coef g_csc_yuv601full_to_yuv709limit = {
    /* csc coef */
    880, -103, -187, 0, 916, 102, 0, 67, 922,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    16, 128, 128
};

const csc_coef g_csc_yuv709full_to_yuv709limit = {
    /* csc coef */
    880, 0, 0, 0, 899, 0, 0, 0, 899,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    16, 128, 128
};

const csc_coef g_csc_yuv601limit_to_yuv601full = {
    /* csc coef */
    1192, 0, 0, 0, 1165, 0, 0, 0, 1165,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    0, 128, 128
};

const csc_coef g_csc_yuv709limit_to_yuv601full = {
    /* csc coef */
    1192, 117, 222, 0, 1154, -128, 0, -84, 1146,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    0, 128, 128
};

const csc_coef g_csc_yuv709full_to_yuv601full = {
    /* csc coef */
    1024, 102, 196, 0, 1014, -113, 0, -74, 1007,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    0, 128, 128
};

const csc_coef g_csc_yuv601limit_to_yuv709full = {
    /* csc coef */
    1192, -137, -248, 0, 1188, 133, 0, 87, 1194,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    0, 128, 128
};

const csc_coef g_csc_yuv601full_to_yuv709full = {
    /* csc coef */
    1024, -118, -213, 0, 1043, 117, 0, 77, 1050,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    0, 128, 128
};

const csc_coef g_csc_yuv709limit_to_yuv709full = {
    /* csc coef */
    1192, 0, 0, 0, 1165, 0, 0, 0, 1165,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    0, 128, 128
};

const csc_coef g_csc_yuv601limit_to_rgbfull = {
    /* csc coef */
    1192, 0, 1634, 1192, -400, -833, 1192, 2066, 0,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    0, 0, 0
};

const csc_coef g_csc_yuv601full_to_rgbfull = {
    /* csc coef */
    1024, 0, 1436, 1024, -352, -731, 1024, 1815, 0,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    0, 0, 0
};

const csc_coef g_csc_yuv709limit_to_rgbfull = {
    /* csc coef */
    1192, 0, 1836, 1192, -218, -547, 1192, 2166, 0,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    0, 0, 0
};

const csc_coef g_csc_yuv709full_to_rgbfull = {
    /* csc coef */
    1024, 0, 1613, 1024, -192, -479, 1024, 1900, 0,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    0, 0, 0
};

const csc_coef g_csc_yuv601limit_to_rgblimit = {
    /* csc coef */
    1024, 0, 1404, 1024, -344, -716, 1024, 1775, 0,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    16, 16, 16
};

const csc_coef g_csc_yuv601full_to_rgblimit = {
    /* csc coef */
    880, 0, 1233, 880, -302, -629, 880, 1599, 0,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    16, 16, 16
};

const csc_coef g_csc_yuv709limit_to_rgblimit = {
    /* csc coef */
    1024, 0, 1578, 1024, -187, -470, 1024, 1861, 0,
    /* csc input dc */
    -16, -128, -128,
    /* csc output dc */
    16, 16, 16
};

const csc_coef g_csc_yuv709full_to_rgblimit = {
    /* csc coef */
    880, 0, 1385, 880, -164, -413, 880, 1634, 0,
    /* csc input dc */
    0, -128, -128,
    /* csc output dc */
    16, 16, 16
};

const csc_coef g_csc_rgbfull_to_yuv601limit = {
    /* csc coef */
    264, 516, 100, -152, -298, 450, 450, -377, -73,
    /* csc input dc */
    0, 0, 0,
    /* csc output dc */
    16, 128, 128
};

const csc_coef g_csc_rgbfull_to_yuv601full = {
    /* csc coef */
    306, 601, 117, -173, -339, 512, 512, -429, -83,
    /* csc input dc */
    0, 0, 0,
    /* csc output dc */
    0, 128, 128
};

const csc_coef g_csc_rgbfull_to_yuv709limit = {
    /* csc coef */
    188, 629, 63, -103, -347, 450, 450, -409, -41,
    /* csc input dc */
    0, 0, 0,
    /* csc output dc */
    16, 128, 128
};

const csc_coef g_csc_rgbfull_to_yuv709full = {
    /* csc coef */
    218, 732, 74, -117, -395, 512, 512, -465, -47,
    /* csc input dc */
    0, 0, 0,
    /* csc output dc */
    0, 128, 128
};

/* sin table value, theta angle range[-30, 30], premultiplied by 1000 */
const td_s32 g_sin_table[61] = { /* 61 theta */
    -500, -485, -469, -454, -438, -422, -407, -391, -374, -358,
    -342, -325, -309, -292, -276, -259, -242, -225, -208, -191,
    -174, -156, -139, -122, -104, -87, -70, -52, -35, -17,
    0, 17, 35, 52, 70, 87, 104, 122, 139, 156,
    174, 191, 208, 225, 242, 259, 276, 292, 309, 325,
    342, 358, 374, 391, 407, 422, 438, 454, 469, 485,
    500
};

/* cos table value, theta angle range[-30, 30], premultiplied by 1000 */
const td_s32 g_cos_table[61] = { /* 61 theta */
    866, 875, 883, 891, 899, 906, 914, 921, 927, 934,
    940, 946, 951, 956, 961, 966, 970, 974, 978, 982,
    985, 988, 990, 993, 995, 996, 998, 999, 999, 1000,
    1000, 1000, 999, 999, 998, 996, 995, 993, 990, 988,
    985, 982, 978, 974, 970, 966, 961, 956, 951, 946,
    940, 934, 927, 921, 914, 906, 899, 891, 883, 875,
    866
};

const csc_coef *g_csc_coef[OT_VO_CSC_MATRIX_BUTT] = {
    &g_csc_identity_limit,
    &g_csc_yuv601full_to_yuv601limit,
    &g_csc_yuv709limit_to_yuv601limit,
    &g_csc_yuv709full_to_yuv601limit,
    &g_csc_yuv601limit_to_yuv709limit,
    &g_csc_yuv601full_to_yuv709limit,
    &g_csc_identity_limit,
    &g_csc_yuv709full_to_yuv709limit,
    &g_csc_yuv601limit_to_yuv601full,
    &g_csc_identity_full,
    &g_csc_yuv709limit_to_yuv601full,
    &g_csc_yuv709full_to_yuv601full,
    &g_csc_yuv601limit_to_yuv709full,
    &g_csc_yuv601full_to_yuv709full,
    &g_csc_yuv709limit_to_yuv709full,
    &g_csc_identity_full,
    &g_csc_yuv601limit_to_rgbfull,
    &g_csc_yuv601full_to_rgbfull,
    &g_csc_yuv709limit_to_rgbfull,
    &g_csc_yuv709full_to_rgbfull,
    &g_csc_yuv601limit_to_rgblimit,
    &g_csc_yuv601full_to_rgblimit,
    &g_csc_yuv709limit_to_rgblimit,
    &g_csc_yuv709full_to_rgblimit,
    &g_csc_rgbfull_to_yuv601limit,
    &g_csc_rgbfull_to_yuv601full,
    &g_csc_rgbfull_to_yuv709limit,
    &g_csc_rgbfull_to_yuv709full,
};

const csc_coef *vo_get_csc_coef(ot_vo_csc_matrix csc_matrix)
{
    if ((csc_matrix >= OT_VO_CSC_MATRIX_BT601LIMIT_TO_BT601LIMIT) &&
        (csc_matrix < OT_VO_CSC_MATRIX_BUTT)) {
        return g_csc_coef[csc_matrix];
    }

    return TD_NULL;
}

const td_s32 *vo_get_sin_table(td_void)
{
    return g_sin_table;
}

const td_s32 *vo_get_cos_table(td_void)
{
    return g_cos_table;
}
#endif /* #if vo_desc("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
