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
#include "gfbg_graphics_drv.h"
#include "proc_ext.h"
#include "securec.h"
#include "mm_ext.h"
#include "mod_ext.h"
#include "sys_ext.h"
#include "ot_errno.h"
#include "ot_debug.h"
#include "gfbg_comm.h"
#include "gfbg_vou_graphics.h"

#define GFX_CSC_SCALE    0xa
#define GFX_CSC_CLIP_MIN 0x0
#define GFX_CSC_CLIP_MAX 0x3ff

#define GRAPHICS_LAYER_G0 0
#define GRAPHICS_LAYER_G1 1
#define GRAPHICS_LAYER_G2 2
#define GRAPHICS_LAYER_G3 3
#define GRAPHICS_LAYER_G4 4

#define GFX_MAX_CSC_TABLE 61

#define GFBG_DATEARR_RANGE 4

gfbg_coef_addr g_gfbg_coef_buf_addr;
gfbg_mmz_buffer g_clut_table_addr_g3;
#ifdef CONFIG_TDE_CLUT_RECT_V2
gfbg_mmz_buffer g_clut_table_addr_g4;
#endif
/* vou interrupt mask type */
typedef enum {
    GFBG_INTMSK_NONE = 0,
    GFBG_INTMSK_DHD0_VTTHD1 = 0x1,
    GFBG_INTMSK_DHD0_VTTHD2 = 0x2,
    GFBG_INTMSK_DHD0_VTTHD3 = 0x4,
    GFBG_INTMSK_DHD0_UFINT = 0x8,

    GFBG_INTMSK_DHD1_VTTHD1 = 0x10,
    GFBG_INTMSK_DHD1_VTTHD2 = 0x20,
    GFBG_INTMSK_DHD1_VTTHD3 = 0x40,
    GFBG_INTMSK_DHD1_UFINT = 0x80,

    GFBG_INTMSK_DSD_VTTHD1 = 0x100,
    GFBG_INTMSK_DSD_VTTHD2 = 0x200,
    GFBG_INTMSK_DSD_VTTHD3 = 0x400,
    GFBG_INTMSK_DSD_UFINT = 0x800,

    GFBG_INTMSK_B0_ERR = 0x1000,
    GFBG_INTMSK_B1_ERR = 0x2000,
    GFBG_INTMSK_B2_ERR = 0x4000,

    GFBG_INTMSK_WBC_DHDOVER = 0x8000,
    GFBG_INTREPORT_ALL = 0xffffffff
} gfbg_int_mask;

/* output 601 limit */
const csc_coef g_csc_yuv601limit_to_yuv601limit = {
    1024, 0, 0, 0, 1024, 0, 0, 0, 1024,
    -16, -128, -128,
    16, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv601full_to_yuv601limit = {
    880, 0, 0, 0, 899, 0, 0, 0, 899,
    0, -128, -128,
    16, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv709limit_to_yuv601limit = {
    1024, 102, 196, 0, 1014, -113, 0, -74, 1007,
    -16, -128, -128,
    16, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv709full_to_yuv601limit = {
    879, 89, 172, 0, 890, -100, 0, -65, 885,
    0, -128, -128,
    16, 128, 128,
    0, 0, 0
};

/* output 709 limit */
const csc_coef g_csc_yuv601limit_to_yuv709limit = {
    1024, -118, -213, 0, 1043, 117, 0, 77, 1050,
    -16, -128, -128,
    16, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv601full_to_yuv709limit = {
    880, -103, -187, 0, 916, 102, 0, 67, 922,
    0, -128, -128,
    16, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv709limit_to_yuv709limit = {
    1024, 0, 0, 0, 1024, 0, 0, 0, 1024,
    -16, -128, -128,
    16, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv709full_to_yuv709limit = {
    880, 0, 0, 0, 899, 0, 0, 0, 899,
    0, -128, -128,
    16, 128, 128,
    0, 0, 0
};

/* output 601 full */
const csc_coef g_csc_yuv601limit_to_yuv601full = {
    1192, 0, 0, 0, 1165, 0, 0, 0, 1165,
    -16, -128, -128,
    0, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv601full_to_yuv601full = {
    1024, 0, 0, 0, 1024, 0, 0, 0, 1024,
    0, -128, -128,
    0, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv709limit_to_yuv601full = {
    1192, 117, 222, 0, 1154, -128, 0, -84, 1146,
    -16, -128, -128,
    0, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv709full_to_yuv601full = {
    1024, 102, 196, 0, 1014, -113, 0, -74, 1007,
    0, -128, -128,
    0, 128, 128,
    0, 0, 0
};

/* output 709 full */
const csc_coef g_csc_yuv601limit_to_yuv709full = {
    1192, -137, -248, 0, 1188, 133, 0, 87, 1194,
    -16, -128, -128,
    0, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv601full_to_yuv709full = {
    1024, -118, -213, 0, 1043, 117, 0, 77, 1050,
    0, -128, -128,
    0, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv709limit_to_yuv709full = {
    1192, 0, 0, 0, 1165, 0, 0, 0, 1165,
    -16, -128, -128,
    0, 128, 128,
    0, 0, 0
};

const csc_coef g_csc_yuv709full_to_yuv709full = {
    1024, 0, 0, 0, 1024, 0, 0, 0, 1024,
    0, -128, -128,
    0, 128, 128,
    0, 0, 0
};

/* output rgb full 0~255 */
const csc_coef g_csc_yuv601limit_to_rgbfull = {
    1192, 0, 1634, 1192, -400, -833, 1192, 2066, 0,
    -16, -128, -128,
    0, 0, 0,
    0, 0, 0
};

const csc_coef g_csc_yuv601full_to_rgbfull = {
    1024, 0, 1436, 1024, -352, -731, 1024, 1815, 0,
    0, -128, -128,
    0, 0, 0,
    0, 0, 0
};

const csc_coef g_csc_yuv709limit_to_rgbfull = {
    1192, 0, 1836, 1192, -218, -547, 1192, 2166, 0,
    -16, -128, -128,
    0, 0, 0,
    0, 0, 0
};

const csc_coef g_csc_yuv709full_to_rgbfull = {
    1024, 0, 1613, 1024, -192, -479, 1024, 1900, 0,
    0, -128, -128,
    0, 0, 0,
    0, 0, 0
};

/* output rgb limit 16~235 */
const csc_coef g_csc_yuv601limit_to_rgblimit = {
    1024, 0, 1404, 1024, -344, -716, 1024, 1775, 0,
    -16, -128, -128,
    16, 16, 16,
    0, 0, 0
};

const csc_coef g_csc_yuv601full_to_rgblimit = {
    880, 0, 1233, 880, -302, -629, 880, 1599, 0,
    0, -128, -128,
    16, 16, 16,
    0, 0, 0
};

const csc_coef g_csc_yuv709limit_to_rgblimit = {
    1024, 0, 1578, 1024, -187, -470, 1024, 1861, 0,
    -16, -128, -128,
    16, 16, 16,
    0, 0, 0
};

const csc_coef g_csc_yuv709full_to_rgblimit = {
    880, 0, 1385, 880, -164, -413, 880, 1634, 0,
    0, -128, -128,
    16, 16, 16,
    0, 0, 0
};

/* input rgb full */
const csc_coef g_csc_rgbfull_to_yuv601limit = {
    264, 516, 100, -152, -298, 450, 450, -377, -73,
    0, 0, 0,
    16, 128, 128,
    0, 0, 0
};
const csc_coef g_csc_rgbfull_to_yuv601full = {
    306, 601, 117, -173, -339, 512, 512, -429, -83,
    0, 0, 0,
    0, 128, 128,
    0, 0, 0
};
const csc_coef g_csc_rgbfull_to_yuv709limit = {
    188, 629, 63, -103, -347, 450, 450, -409, -41,
    0, 0, 0,
    16, 128, 128,
    0, 0, 0
};
const csc_coef g_csc_rgbfull_to_yuv709full = {
    218, 732, 74, -117, -395, 512, 512, -465, -47,
    0, 0, 0,
    0, 128, 128,
    0, 0, 0
};

const int g_gfbg_sin_table[GFX_MAX_CSC_TABLE] = {
    -500, -485, -469, -454, -438, -422, -407, -391, -374, -358,
    -342, -325, -309, -292, -276, -259, -242, -225, -208, -191,
    -174, -156, -139, -122, -104, -87, -70, -52, -35, -17,
    0, 17, 35, 52, 70, 87, 104, 122, 139, 156,
    174, 191, 208, 225, 242, 259, 276, 292, 309, 325,
    342, 358, 374, 391, 407, 422, 438, 454, 469, 485,
    500
};

const int g_gfbg_cos_table[GFX_MAX_CSC_TABLE] = {
    866, 875, 883, 891, 899, 906, 914, 921, 927, 934,
    940, 946, 951, 956, 961, 966, 970, 974, 978, 982,
    985, 988, 990, 993, 995, 996, 998, 999, 999, 1000,
    1000, 1000, 999, 999, 998, 996, 995, 993, 990, 988,
    985, 982, 978, 974, 970, 966, 961, 956, 951, 946,
    940, 934, 927, 921, 914, 906, 899, 891, 883, 875,
    866
};

static td_s32 vo_drv_get_hal_csc_mode(ot_vo_csc_matrix csc_matrix, hal_csc_mode *csc_mode)
{
    td_u32 loop;
    td_u32 len;
    vo_drv_csc_matrix_mode csc_matrix_mode[] = {
        {VO_CSC_MATRIX_BT601LIMIT_TO_BT601LIMIT, HAL_CSC_MODE_BT601LIMIT_TO_BT601LIMIT},
        {VO_CSC_MATRIX_BT601FULL_TO_BT601LIMIT,  HAL_CSC_MODE_BT601FULL_TO_BT601LIMIT},
        {VO_CSC_MATRIX_BT709LIMIT_TO_BT601LIMIT, HAL_CSC_MODE_BT709LIMIT_TO_BT601LIMIT},
        {VO_CSC_MATRIX_BT709FULL_TO_BT601LIMIT,  HAL_CSC_MODE_BT709FULL_TO_BT601LIMIT},
        {VO_CSC_MATRIX_BT601LIMIT_TO_BT709LIMIT, HAL_CSC_MODE_BT601LIMIT_TO_BT709LIMIT},
        {VO_CSC_MATRIX_BT601FULL_TO_BT709LIMIT,  HAL_CSC_MODE_BT601FULL_TO_BT709LIMIT},
        {VO_CSC_MATRIX_BT709LIMIT_TO_BT709LIMIT, HAL_CSC_MODE_BT709LIMIT_TO_BT709LIMIT},
        {VO_CSC_MATRIX_BT709FULL_TO_BT709LIMIT,  HAL_CSC_MODE_BT709FULL_TO_BT709LIMIT},
        {VO_CSC_MATRIX_BT601LIMIT_TO_BT601FULL, HAL_CSC_MODE_BT601LIMIT_TO_BT601FULL},
        {VO_CSC_MATRIX_BT601FULL_TO_BT601FULL,  HAL_CSC_MODE_BT601FULL_TO_BT601FULL},
        {VO_CSC_MATRIX_BT709LIMIT_TO_BT601FULL, HAL_CSC_MODE_BT709LIMIT_TO_BT601FULL},
        {VO_CSC_MATRIX_BT709FULL_TO_BT601FULL,  HAL_CSC_MODE_BT709FULL_TO_BT601FULL},
        {VO_CSC_MATRIX_BT601LIMIT_TO_BT709FULL, HAL_CSC_MODE_BT601LIMIT_TO_BT709FULL},
        {VO_CSC_MATRIX_BT601FULL_TO_BT709FULL,  HAL_CSC_MODE_BT601FULL_TO_BT709FULL},
        {VO_CSC_MATRIX_BT709LIMIT_TO_BT709FULL, HAL_CSC_MODE_BT709LIMIT_TO_BT709FULL},
        {VO_CSC_MATRIX_BT709FULL_TO_BT709FULL,  HAL_CSC_MODE_BT709FULL_TO_BT709FULL},
        {VO_CSC_MATRIX_BT601LIMIT_TO_RGBFULL,   HAL_CSC_MODE_BT601LIMIT_TO_RGBFULL},
        {VO_CSC_MATRIX_BT601FULL_TO_RGBFULL,    HAL_CSC_MODE_BT601FULL_TO_RGBFULL},
        {VO_CSC_MATRIX_BT709LIMIT_TO_RGBFULL,    HAL_CSC_MODE_BT709LIMIT_TO_RGBFULL},
        {VO_CSC_MATRIX_BT709FULL_TO_RGBFULL,    HAL_CSC_MODE_BT709FULL_TO_RGBFULL},
        {VO_CSC_MATRIX_BT601LIMIT_TO_RGBLIMIT,  HAL_CSC_MODE_BT601LIMIT_TO_RGBLIMIT},
        {VO_CSC_MATRIX_BT601FULL_TO_RGBLIMIT,   HAL_CSC_MODE_BT601FULL_TO_RGBLIMIT},
        {VO_CSC_MATRIX_BT709LIMIT_TO_RGBLIMIT,  HAL_CSC_MODE_BT709LIMIT_TO_RGBLIMIT},
        {VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT,   HAL_CSC_MODE_BT709FULL_TO_RGBLIMIT},
        {VO_CSC_MATRIX_RGBFULL_TO_BT601LIMIT,   HAL_CSC_MODE_RGBFULL_TO_BT601LIMIT},
        {VO_CSC_MATRIX_RGBFULL_TO_BT601FULL,    HAL_CSC_MODE_RGBFULL_TO_BT601FULL},
        {VO_CSC_MATRIX_RGBFULL_TO_BT709LIMIT,   HAL_CSC_MODE_RGBFULL_TO_BT709LIMIT},
        {VO_CSC_MATRIX_RGBFULL_TO_BT709FULL,    HAL_CSC_MODE_RGBFULL_TO_BT709FULL},
        {VO_CSC_MATRIX_RGBFULL_TO_RGBFULL,      HAL_CSC_MODE_RGBFULL_TO_RGBFULL},
        {VO_CSC_MATRIX_RGBFULL_TO_RGBLIMIT,     HAL_CSC_MODE_RGBFULL_TO_RGBLIMIT}
    };
    len = sizeof(csc_matrix_mode) / sizeof(vo_drv_csc_matrix_mode);
    for (loop = 0; loop < len; loop++) {
        if (csc_matrix_mode[loop].csc_matrix == csc_matrix) {
            *csc_mode = csc_matrix_mode[loop].csc_mode;
            return TD_SUCCESS;
        }
    }
    return TD_FAILURE;
}
static td_s32 vo_drv_get_csc_matrix(hal_csc_mode csc_mode, const csc_coef **csc_tmp)
{
    td_u32 loop;
    td_u32 len;
    vo_drv_csc_mode_coef csc_mode_coef[] = {
        {HAL_CSC_MODE_BT601LIMIT_TO_BT601LIMIT,     &g_csc_yuv601limit_to_yuv601limit},
        {HAL_CSC_MODE_BT601FULL_TO_BT601LIMIT,      &g_csc_yuv601full_to_yuv601limit},
        {HAL_CSC_MODE_BT709LIMIT_TO_BT601LIMIT,     &g_csc_yuv709limit_to_yuv601limit},
        {HAL_CSC_MODE_BT709FULL_TO_BT601LIMIT,      &g_csc_yuv709full_to_yuv601limit},
        {HAL_CSC_MODE_BT601LIMIT_TO_BT601FULL,      &g_csc_yuv601limit_to_yuv601full},
        {HAL_CSC_MODE_BT601FULL_TO_BT601FULL,       &g_csc_yuv601full_to_yuv601full},
        {HAL_CSC_MODE_BT709LIMIT_TO_BT601FULL,      &g_csc_yuv709limit_to_yuv601full},
        {HAL_CSC_MODE_BT709FULL_TO_BT601FULL,       &g_csc_yuv709full_to_yuv601full},
        {HAL_CSC_MODE_BT601LIMIT_TO_BT709LIMIT,     &g_csc_yuv601limit_to_yuv709limit},
        {HAL_CSC_MODE_BT601FULL_TO_BT709LIMIT,      &g_csc_yuv601full_to_yuv709limit},
        {HAL_CSC_MODE_BT709LIMIT_TO_BT709LIMIT,     &g_csc_yuv709limit_to_yuv709limit},
        {HAL_CSC_MODE_BT709FULL_TO_BT709LIMIT,      &g_csc_yuv709full_to_yuv709limit},
        {HAL_CSC_MODE_BT601LIMIT_TO_BT709FULL,      &g_csc_yuv601limit_to_yuv709full},
        {HAL_CSC_MODE_BT601FULL_TO_BT709FULL,       &g_csc_yuv601full_to_yuv709full},
        {HAL_CSC_MODE_BT709LIMIT_TO_BT709FULL,      &g_csc_yuv709limit_to_yuv709full},
        {HAL_CSC_MODE_BT709FULL_TO_BT709FULL,       &g_csc_yuv709full_to_yuv709full},
        {HAL_CSC_MODE_BT601LIMIT_TO_RGBFULL,        &g_csc_yuv601limit_to_rgbfull},
        {HAL_CSC_MODE_BT601FULL_TO_RGBFULL,         &g_csc_yuv601full_to_rgbfull},
        {HAL_CSC_MODE_BT709LIMIT_TO_RGBFULL,        &g_csc_yuv709limit_to_rgbfull},
        {HAL_CSC_MODE_BT709FULL_TO_RGBFULL,         &g_csc_yuv709full_to_rgbfull},
        {HAL_CSC_MODE_BT601LIMIT_TO_RGBLIMIT,       &g_csc_yuv601limit_to_rgblimit},
        {HAL_CSC_MODE_BT601FULL_TO_RGBLIMIT,        &g_csc_yuv601full_to_rgblimit},
        {HAL_CSC_MODE_BT709LIMIT_TO_RGBLIMIT,       &g_csc_yuv709limit_to_rgblimit},
        {HAL_CSC_MODE_BT709FULL_TO_RGBLIMIT,        &g_csc_yuv709full_to_rgblimit},
        {HAL_CSC_MODE_RGBFULL_TO_BT601LIMIT,        &g_csc_rgbfull_to_yuv601limit},
        {HAL_CSC_MODE_RGBFULL_TO_BT601FULL,         &g_csc_rgbfull_to_yuv601full},
        {HAL_CSC_MODE_RGBFULL_TO_BT709LIMIT,        &g_csc_rgbfull_to_yuv709limit},
        {HAL_CSC_MODE_RGBFULL_TO_BT709FULL,         &g_csc_rgbfull_to_yuv709full},
    };
    len = sizeof(csc_mode_coef) / sizeof(vo_drv_csc_mode_coef);
    for (loop = 0; loop < len; loop++) {
        if (csc_mode_coef[loop].csc_mode == csc_mode) {
            *csc_tmp = csc_mode_coef[loop].coef;
            return TD_SUCCESS;
        }
    }
    return TD_FAILURE;
}
td_s32 fb_graphic_drv_get_layer_index(hal_disp_layer disp_layer, td_u32 *layer)
{
    if (layer == TD_NULL) {
        graphics_drv_error("NULL pointer!\n");
        return TD_FAILURE;
    }
    switch (disp_layer) {
        case HAL_DISP_LAYER_GFX0:
            *layer = GRAPHICS_LAYER_G0;
            break;
        case HAL_DISP_LAYER_GFX1:
            *layer = GRAPHICS_LAYER_G1;
            break;
        case HAL_DISP_LAYER_GFX2:
            *layer = GRAPHICS_LAYER_G2;
            break;
        case HAL_DISP_LAYER_GFX3:
            *layer = GRAPHICS_LAYER_G3;
            break;
        case HAL_DISP_LAYER_GFX4:
            *layer = GRAPHICS_LAYER_G4;
            break;
        default:
            graphics_drv_error("hal_disp_layer(%u) is invalid!\n", (td_u32)disp_layer);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_bool graphic_drv_set_gfx_key_mode(hal_disp_layer layer, td_u32 key_out)
{
    if (fb_hal_graphic_set_gfx_key_mode(layer, key_out) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_set_gfx_ext(hal_disp_layer layer, hal_gfx_bitextend mode)
{
    if (fb_hal_graphic_set_gfx_ext(layer, mode) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_set_gfx_palpha(hal_disp_layer layer, td_u32 alpha_en, td_u32 arange, td_u8 alpha0,
                                   td_u8 alpha1)
{
    ot_unused(arange);
    if (fb_hal_graphic_set_gfx_palpha(layer, alpha_en, TD_TRUE, alpha0, alpha1) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_layer_set_layer_galpha(hal_disp_layer layer, td_u8 alpha0)
{
    if (fb_hal_layer_set_layer_galpha(layer, alpha0) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_layer_set_csc_en(hal_disp_layer layer, td_bool csc_en)
{
    if (fb_hal_layer_set_csc_en(layer, csc_en) == TD_FALSE) {
        return TD_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0) {
        return TD_TRUE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_set_layer_addr(hal_disp_layer layer_id, td_phys_addr_t addr)
{
    td_u32 layer_index;

    if (fb_graphic_drv_get_layer_index(layer_id, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)layer_id);
        return TD_FALSE;
    }

    if (fb_hal_graphic_set_gfx_addr(layer_id, addr) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_set_gfx_stride(hal_disp_layer layer, td_u16 pitch)
{
    td_u32 layer_index = 0;
    td_u16 depth;

    if (fb_graphic_drv_get_layer_index(layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)layer);
        return TD_FALSE;
    }
    /* 4:Stride need be divided by 16 before setting the register */
    depth = pitch >> 4;
    if (fb_hal_graphic_set_gfx_stride(layer, depth) == TD_FALSE) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool graphic_drv_get_gfx_pre_mult(hal_disp_layer layer, td_u32 *enable)
{
    if (fb_hal_graphic_get_gfx_premult(layer, enable) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_set_gfx_pre_mult(hal_disp_layer layer, td_u32 enable)
{
    if (fb_hal_graphic_set_gfx_pre_mult(layer, enable) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt)
{
    td_u32 layer_index = 0;
    if (fb_graphic_drv_get_layer_index(layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)layer);
        return TD_FALSE;
    }

    if (fb_hal_layer_set_layer_data_fmt(layer, data_fmt) == TD_FALSE) {
        return TD_FALSE;
    }
    graphics_drv_info("Set Layer%d DataFmt: %d!\n", (td_u32)layer, data_fmt);
    return TD_TRUE;
}

td_bool graphic_drv_set_layer_in_rect(hal_disp_layer layer, const ot_fb_rect *rect)
{
    if (fb_hal_layer_set_layer_in_rect(layer, rect) == TD_FALSE) {
        return TD_FALSE;
    }

    if (fb_hal_video_set_layer_disp_rect(layer, rect) == TD_FALSE) {
        return TD_FALSE;
    }
    if (fb_hal_video_set_layer_video_rect(layer, rect) == TD_FALSE) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

/*
 * Name : graphic_drv_set_src_image_resolution
 * Desc : set the original resolution of the frame.
 */
td_bool graphic_drv_set_src_image_resolution(hal_disp_layer layer, const ot_fb_rect *rect)
{
    if (fb_hal_layer_set_src_resolution(layer, rect) == TD_FALSE) {
        return TD_FALSE;
    }

    if (fb_hal_layer_set_layer_in_rect(layer, rect) == TD_FALSE) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool graphic_drv_set_layer_out_rect(hal_disp_layer layer, const ot_fb_rect *rect)
{
    if (fb_hal_layer_set_layer_out_rect(layer, rect) == TD_FALSE) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool graphic_drv_set_color_key_value(hal_disp_layer layer, hal_gfx_key_max key_max, hal_gfx_key_min key_min)
{
    if (fb_hal_graphic_set_color_key_value(layer, key_max, key_min) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_set_color_key_mask(hal_disp_layer layer, hal_gfx_mask msk)
{
    if (fb_hal_graphic_set_color_key_mask(layer, msk) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_set_gfx_key_en(hal_disp_layer layer, td_u32 key_enable)
{
    if (fb_hal_graphic_set_gfx_key_en(layer, key_enable) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_set_reg_up(hal_disp_layer layer)
{
    if (fb_hal_layer_set_reg_up(layer) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_void graphic_drv_set_clut_reg(td_u32 layer_id, td_phys_addr_t clut_phy_addr)
{
    hal_clut_set_up_param(layer_id, clut_phy_addr);
    return;
}

td_void graphic_drv_set_clut_reg_up(td_u32 layer_id)
{
    hal_clut_set_up(layer_id);
    return;
}

td_bool graphic_drv_set_smart_rect_reg(td_u32 layer_id, td_phys_addr_t phys_addr)
{
    hal_smart_rect_set_up(layer_id, phys_addr);
    return TD_TRUE;
}

td_void graphic_drv_smart_rect_up_param(td_u32 layer_id)
{
    hal_smart_rect_up_param(layer_id);
}

td_void graphic_drv_smart_rect_disable(td_u32 layer_id)
{
    hal_smart_rect_disable(layer_id);
}

td_bool graphic_drv_get_layer_galpha(hal_disp_layer layer, td_u8 *alpha0)
{
    if (fb_hal_layer_get_layer_galpha(layer, alpha0) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_get_layer_data_fmt(hal_disp_layer layer, td_u32 *fmt)
{
    if (fb_hal_layer_get_layer_data_fmt(layer, fmt) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_get_gfx_addr(hal_disp_layer layer, td_phys_addr_t *gfx_addr)
{
    if (fb_hal_graphic_get_gfx_addr(layer, gfx_addr) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_get_gfx_stride(hal_disp_layer layer, td_u32 *gfx_stride)
{
    if (fb_hal_graphic_get_gfx_stride(layer, gfx_stride) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool fb_graphic_drv_get_scan_mode(ot_vo_dev vo_dev, td_bool *iop)
{
    if (fb_hal_disp_get_disp_iop(vo_dev, iop) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_enable_dcmp(hal_disp_layer layer, td_bool enable)
{
    td_u32 layer_index = 0;
    if (fb_graphic_drv_get_layer_index(layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)layer);
        return TD_FALSE;
    }
    if (fb_hal_graphic_set_gfx_dcmp_enable(layer, enable) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_get_dcmp_enable_state(hal_disp_layer layer, td_bool *enable)
{
    if (fb_hal_graphic_get_gfx_dcmp_enable_state(layer, enable) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
td_bool graphic_drv_set_dcmp_info(hal_disp_layer layer, const graphic_dcmp_info *dcmp_info)
{
    td_u32 layer_index;
    gfbg_dcmp_src_mode dcmp_src_mode = GFX_DCMP_SRC_MODE_ARGB8888;

    if (fb_graphic_drv_get_layer_index(layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)layer);
        return TD_FALSE;
    }

    if (fb_hal_graphic_set_gfx_dcmp_addr(layer, dcmp_info->ar_phy_addr) == TD_FALSE) {
        return TD_FALSE;
    }

    /* 4:divided by 16 before setting register */
    if (fb_hal_graphic_set_gfx_stride(layer, dcmp_info->stride >> 4) == TD_FALSE) {
        return TD_FALSE;
    }
    if (dcmp_info->pixel_fmt == HAL_INPUTFMT_ARGB_8888) {
        dcmp_src_mode = GFX_DCMP_SRC_MODE_ARGB8888;
    } else if (dcmp_info->pixel_fmt == HAL_INPUTFMT_ARGB_1555) {
        dcmp_src_mode = GFX_DCMP_SRC_MODE_ARGB1555;
    } else if (dcmp_info->pixel_fmt == HAL_INPUTFMT_ARGB_4444) {
        dcmp_src_mode = GFX_DCMP_SRC_MODE_ARGB4444;
    }

    fb_hal_fdr_gfx_set_source_mode(layer_index, dcmp_src_mode);
    fb_hal_fdr_gfx_set_cmp_mode(layer_index, 0);
    fb_hal_fdr_gfx_set_is_loss_lessa(layer_index, dcmp_info->is_lossless_a);
    fb_hal_fdr_gfx_set_is_loss_less(layer_index, dcmp_info->is_lossless);
    return TD_TRUE;
}
#endif

#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
td_bool graphic_drv_set_dcmp_info(hal_disp_layer layer, const graphic_dcmp_info *dcmp_info)
{
    td_u32 layer_index;
    gfbg_dcmp_src_mode dcmp_src_mode = (gfbg_dcmp_src_mode)GFX_DCMP_SRC_MODE_ARGB8888;
    td_u32 conv_en = 1;

    if (dcmp_info == TD_NULL) {
        return TD_FALSE;
    }

    if (fb_graphic_drv_get_layer_index(layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)layer);
        return TD_FALSE;
    }

    if (fb_hal_graphic_set_gfx_dcmp_addr(layer, dcmp_info->ar_phy_addr) == TD_FALSE) {
        return TD_FALSE;
    }

    /* 4:divided by 16 before setting register */
    if (fb_hal_graphic_set_gfx_stride(layer, dcmp_info->stride >> 4) == TD_FALSE) {
        return TD_FALSE;
    }

    if (dcmp_info->pixel_fmt == HAL_INPUTFMT_ARGB_8888) {
        dcmp_src_mode = (gfbg_dcmp_src_mode)GFX_DCMP_SRC_MODE_ARGB8888;
    } else if (dcmp_info->pixel_fmt == HAL_INPUTFMT_ARGB_1555) {
        dcmp_src_mode = (gfbg_dcmp_src_mode)GFX_DCMP_SRC_MODE_ARGB1555;
    } else if (dcmp_info->pixel_fmt == HAL_INPUTFMT_ARGB_4444) {
        dcmp_src_mode = (gfbg_dcmp_src_mode)GFX_DCMP_SRC_MODE_ARGB4444;
    } else if (dcmp_info->pixel_fmt == HAL_INPUTFMT_RGB_888) {
        dcmp_src_mode = (gfbg_dcmp_src_mode)GFX_DCMP_SRC_MODE_RGB888;
    }

    if (dcmp_src_mode > 1) {
        /* 1555 4444 don't need RGB2YUV */
        conv_en = 0;
    }

    fb_hal_v3r2_gfx_set_source_mode(layer_index, dcmp_src_mode);
    fb_hal_v3r2_gfx_set_is_loss_less(layer_index, dcmp_info->is_lossless);
    fb_hal_v3r2_gfx_set_is_conv_en(layer_index, conv_en);
    fb_hal_v3r2_gfx_set_cmp_mode(layer_index, 0);
    fb_hal_v3r2_gfx_set_is_ice_en(layer_index, 1);
    fb_hal_v3r2_gfx_set_frame_size(layer_index, dcmp_info->width, dcmp_info->height);

    return TD_TRUE;
}
#endif

td_bool graphic_drv_get_vt_thd_mode(ot_vo_dev vo_dev, td_bool *feild_update)
{
    if (fb_hal_disp_get_vt_thd_mode(vo_dev, feild_update) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_get_vcnt(ot_vo_dev vo_dev, td_u32 *vcnt)
{
    if (fb_hal_disp_get_int_state_vcnt(vo_dev, vcnt) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

/*
 * Name : graphic_drv_calc_csc_matrix
 * Desc : Calculate csc matrix.
 */
static td_s32 graphic_drv_calc_csc_matrix(const vo_csc *csc, hal_csc_mode csc_mode, csc_coef *coef)
{
    td_s32 luma;
    td_s32 contrast;
    td_s32 hue;
    td_s32 satu;

    const csc_coef *csc_tmp = TD_NULL;
    td_s32 ret;

    luma = (td_s32)csc->luma * 64 / 100 - 32; /* 64 100 32 alg data */
    contrast = ((td_s32)csc->contrast - 50) * 2 + 100; /* 50 2 100 alg data */
    hue = (td_s32)csc->hue * 60 / 100; /* 60 100 alg data */
    satu = ((td_s32)csc->satuature - 50) * 2 + 100; /* 50 2 100 alg data */

    ret = vo_drv_get_csc_matrix(csc_mode, &csc_tmp);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    coef->csc_in_dc0 = csc_tmp->csc_in_dc0;
    coef->csc_in_dc1 = csc_tmp->csc_in_dc1;
    coef->csc_in_dc2 = csc_tmp->csc_in_dc2;
    coef->csc_out_dc0 = csc_tmp->csc_out_dc0;
    coef->csc_out_dc1 = csc_tmp->csc_out_dc1;
    coef->csc_out_dc2 = csc_tmp->csc_out_dc2;
    /*
	 * C_ratio normally is 0~1.99, C_ratio=s32Contrast/100
     * S normally is 0~1.99,S=s32Satu/100
     * Hue -30~30, using the lut to get COS and SIN and then /1000
     */
    coef->csc_coef00 = (contrast * csc_tmp->csc_coef00) / 100; /* 100 alg data */
    coef->csc_coef01 = (contrast * csc_tmp->csc_coef01) / 100; /* 100 alg data */
    coef->csc_coef02 = (contrast * csc_tmp->csc_coef02) / 100; /* 100 alg data */
    if (hue >= GFX_MAX_CSC_TABLE) {
        graphics_drv_error("hue(%u) is invalid!\n", (td_u32)hue);
        return TD_FAILURE;
    }
    coef->csc_coef10 = (contrast * satu * ((csc_tmp->csc_coef10 * g_gfbg_cos_table[hue] +
                            csc_tmp->csc_coef20 * g_gfbg_sin_table[hue]) / 1000)) / 10000; /* 1000 10000 alg data */
    coef->csc_coef11 = (contrast * satu * ((csc_tmp->csc_coef11 * g_gfbg_cos_table[hue] +
                            csc_tmp->csc_coef21 * g_gfbg_sin_table[hue]) / 1000)) / 10000; /* 1000 10000 alg data */
    coef->csc_coef12 = (contrast * satu * ((csc_tmp->csc_coef12 * g_gfbg_cos_table[hue] +
                            csc_tmp->csc_coef22 * g_gfbg_sin_table[hue]) / 1000)) / 10000; /* 1000 10000 alg data */
    coef->csc_coef20 = (contrast * satu * ((csc_tmp->csc_coef20 * g_gfbg_cos_table[hue] -
                            csc_tmp->csc_coef10 * g_gfbg_sin_table[hue]) / 1000)) / 10000; /* 1000 10000 alg data */
    coef->csc_coef21 = (contrast * satu * ((csc_tmp->csc_coef21 * g_gfbg_cos_table[hue] -
                            csc_tmp->csc_coef11 * g_gfbg_sin_table[hue]) / 1000)) / 10000; /* 1000 10000 alg data */
    coef->csc_coef22 = (contrast * satu * ((csc_tmp->csc_coef22 * g_gfbg_cos_table[hue] -
                            csc_tmp->csc_coef12 * g_gfbg_sin_table[hue]) / 1000)) / 10000; /* 1000 10000 alg data */
    coef->csc_out_dc0 += luma;

    return TD_SUCCESS;
}

td_void graphic_drv_get_int_state_vcnt(ot_vo_dev vo_dev, td_u32 *vcnt)
{
    hal_disp_get_int_state_vcnt(vo_dev, vcnt);
    return;
}

td_bool graphic_drv_get_dev_enable(ot_vo_dev vo_dev, td_bool *intf_en)
{
    if (fb_hal_disp_get_intf_enable(vo_dev, intf_en) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_get_intf_sync(ot_vo_dev vo_dev, hal_disp_syncinfo *sync_info)
{
    if (fb_hal_disp_get_intf_sync(vo_dev, sync_info) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool graphic_drv_get_intf_mux_sel(ot_vo_dev vo_dev, vo_intf_type *intf_type)
{
    if (fb_hal_disp_get_intf_mux_sel(vo_dev, intf_type) == TD_FALSE) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

static td_s32 graphic_drv_allocate_mem(td_u32 size, gfbg_mmz_buffer *vdp_mmz_buffer, const char* name)
{
    td_s32 ret;
    cmpi_mmz_malloc_param malloc_param;

    td_void *mmz_name = TD_NULL;
    ot_mpp_chn chn;

    chn.mod_id = OT_ID_FB;
    chn.dev_id = 0;
    chn.chn_id = 0;

    if (func_entry(sys_export_func, OT_ID_SYS)->pfn_get_mmz_name(&chn, &mmz_name)) {
        graphics_drv_error("get mmz name fail!\n");
        return TD_FAILURE;
    }

    malloc_param.mmz_name = mmz_name;
    malloc_param.buf_name = name;
    malloc_param.phys_addr = &vdp_mmz_buffer->start_phy_addr;
    malloc_param.virt_addr = &vdp_mmz_buffer->start_vir_addr;
    malloc_param.len = size;
    malloc_param.kernel_only = TD_FALSE;
    ret = cmpi_mmz_malloc_nocache(&malloc_param);
    if (ret != 0) {
        graphics_drv_error("GFBG DDR CFG  failed\n");
        return TD_FAILURE;
    }

    vdp_mmz_buffer->size = size;
    (td_void)memset_s(vdp_mmz_buffer->start_vir_addr, size, 0, size);
    return TD_SUCCESS;
}

static td_void graphic_drv_delete_mem(gfbg_mmz_buffer *vdp_mmz_buffer)
{
    if (vdp_mmz_buffer != TD_NULL) {
        cmpi_mmz_free(vdp_mmz_buffer->start_phy_addr, vdp_mmz_buffer->start_vir_addr);
        vdp_mmz_buffer->start_phy_addr = 0;
        vdp_mmz_buffer->start_vir_addr = TD_NULL;
    }
}

static td_void graphic_drv_vhd_coef_buf_addr_distribute(gfbg_coef_addr *vdp_coef_buf_addr)
{
    vdp_coef_buf_addr->coef_vir_addr[GFBG_COEF_BUF_G0ZME] = vdp_coef_buf_addr->buf_base_addr.start_vir_addr;
    vdp_coef_buf_addr->coef_phy_addr[GFBG_COEF_BUF_G0ZME] = vdp_coef_buf_addr->buf_base_addr.start_phy_addr;

    fb_hal_para_set_para_addr_vhd_chn(vdp_coef_buf_addr->coef_phy_addr[GFBG_COEF_BUF_G0ZME]);
}

td_s32 graphic_zme_coef_init(td_void)
{
    td_s32 ret;
    char name[16] = "gfbg_coef"; /* 16 name length */
    /* gfbg only need zme coef size */
    ret = graphic_drv_allocate_mem(COEF_SIZE_G0ZME, &g_gfbg_coef_buf_addr.buf_base_addr, name);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    graphic_drv_vhd_coef_buf_addr_distribute(&g_gfbg_coef_buf_addr);
    return TD_SUCCESS;
}

/* only G3 can support clut */
static td_s32 graphic_clut_table_g3_init(td_void)
{
    td_s32 ret;
    char name_g3[19] = "gfbg_clut_table_g3"; /* 19 name length */

    ret = graphic_drv_allocate_mem(CLUT_TABLE_SIZE, &g_clut_table_addr_g3, name_g3);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}

#ifdef CONFIG_TDE_CLUT_RECT_V2
static td_s32 graphic_clut_table_g4_init(td_void)
{
    td_s32 ret;
    char name_g4[19] = "gfbg_clut_table_g4"; /* 19 name length */

    ret = graphic_drv_allocate_mem(CLUT_TABLE_SIZE, &g_clut_table_addr_g4, name_g4);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}
#endif
/*
 * only G3 can get bind
 */
static td_s32 graphic_drv_get_layer_bind_dev(ot_gfx_layer gfx_layer, ot_vo_dev *vo_dev)
{
    hal_disp_layer layer;
    vo_gfxlayer_context *gfx_layer_ctx = vou_get_gfx_layer_ctx();
    /* G3 index is 2 */
    if ((gfx_layer != GRAPHICS_LAYER_G3) && (gfx_layer != GRAPHICS_LAYER_G2) && (gfx_layer != GRAPHICS_LAYER_G4)) {
        graphics_drv_error("gfx_layer %d is illeagal\n", (td_s32)gfx_layer);
        return TD_FAILURE;
    }

    layer = gfx_layer + HAL_DISP_LAYER_GFX0;
    fb_hal_link_get_hc_link(layer, vo_dev);

    gfx_layer_ctx[gfx_layer].binded_dev = *vo_dev;
    gfx_layer_ctx[gfx_layer].binded = TD_TRUE;
    return TD_SUCCESS;
}

td_s32 fb_graphic_drv_init(td_void)
{
    td_s32 ret;
    td_s32 i;
    vo_gfxlayer_context *vo_gfx_ctx = vou_get_gfx_layer_ctx();
    ot_vo_dev vo_dev;

    for (i = GRAPHICS_LAYER_G0; i <= GRAPHICS_LAYER_G1; ++i) { /* 2 alg data */
        vo_gfx_ctx[i].layer_id = HAL_DISP_LAYER_GFX0 + i; /* HAL_DISP_LAYER_GFX0+1 */
        vo_gfx_ctx[i].binded_dev = i; /* 0 */
        vo_gfx_ctx[i].binded = TD_TRUE;
    }

    /* init G2~G4 */
    for (i = GRAPHICS_LAYER_G2; i <= GRAPHICS_LAYER_G4; i++) {
        vo_gfx_ctx[i].layer_id = HAL_DISP_LAYER_GFX0 + i;
        vo_gfx_ctx[i].binded = TD_TRUE;
        ret = graphic_drv_get_layer_bind_dev(i, &vo_dev);
        if (ret != TD_SUCCESS) {
            return ret;
        }
        vo_gfx_ctx[i].binded_dev = vo_dev;
    }

    for (i = 0; i < GFX_MAX; ++i) {
        vo_gfx_ctx[i].gfx_csc.csc_matrix = VO_CSC_MATRIX_RGBFULL_TO_BT601LIMIT;
        vo_gfx_ctx[i].gfx_csc.luma = 50; /* 50 alg data */
        vo_gfx_ctx[i].gfx_csc.contrast = 50; /* 50 alg data */
        vo_gfx_ctx[i].gfx_csc.hue = 50; /* 50 alg data */
        vo_gfx_ctx[i].gfx_csc.satuature = 50; /* 50 alg data */

        /* CSC extra coef */
        vo_gfx_ctx[i].coef_param.csc_scale2p = GFX_CSC_SCALE;
        vo_gfx_ctx[i].coef_param.csc_clip_min = GFX_CSC_CLIP_MIN;
        vo_gfx_ctx[i].coef_param.csc_clip_max = GFX_CSC_CLIP_MAX;
    }

    /* DDR detect coef */
    vo_gfx_ctx[GRAPHICS_LAYER_G0].start_section = 0;
    vo_gfx_ctx[GRAPHICS_LAYER_G0].zone_nums = 16; /* 16 alg data */
    vo_gfx_ctx[GRAPHICS_LAYER_G1].start_section = 16; /* 16 alg data */
    vo_gfx_ctx[GRAPHICS_LAYER_G1].zone_nums = 16; /* 16 alg data */

    return TD_SUCCESS;
}

#ifdef CONFIG_OT_VO_FB_SEPARATE
td_s32 graphic_drv_get_bind_dev(td_s32 layer_id)
{
    vo_gfxlayer_context *gfx_layer_ctx = vou_get_gfx_layer_ctx();
    return gfx_layer_ctx[layer_id].binded_dev;
}
#endif

td_s32 fb_graphic_drv_exit(td_void)
{
    return TD_SUCCESS;
}

static td_s32 graphic_drv_reg_and_lock_init(vo_gfxlayer_context *gfx_layer_ctx)
{
    td_s32 ret;
    td_s32 i;
    td_s32 j;

    ret = fb_hal_vou_init();
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* lock init */
    for (i = 0; i < GFBG_MAX_LAYER_NUM; ++i) {
        (td_void)memset_s(&gfx_layer_ctx[i], sizeof(vo_gfxlayer_context), 0, sizeof(vo_gfxlayer_context));
        if (gfx_spin_lock_init(&gfx_layer_ctx[i].spin_lock) != 0) {
            for (j = 0; j < i; j++) {
                gfx_spin_lock_deinit(&gfx_layer_ctx[j].spin_lock);
            }
            fb_hal_vou_exit();
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_void graphic_drv_reg_and_lock_deinit(vo_gfxlayer_context *gfx_layer_ctx)
{
    td_s32 i;
    /* lock deinit */
    for (i = 0; i < GFBG_MAX_LAYER_NUM; ++i) {
        gfx_spin_lock_deinit(&gfx_layer_ctx[i].spin_lock);
    }
    fb_hal_vou_exit();
    return;
}

td_s32 graphic_drv_resource_init(td_void)
{
    td_s32 ret;
    vo_gfxlayer_context *gfx_layer_ctx = vou_get_gfx_layer_ctx();

    ret = graphic_drv_reg_and_lock_init(gfx_layer_ctx);
    if (ret != TD_SUCCESS) {
        graphics_drv_error("GFBG LOCK INIT  failed\n");
        return TD_FAILURE;
    }

    /* mem alloc */
    ret = graphic_zme_coef_init();
    if (ret != TD_SUCCESS) {
        graphic_drv_reg_and_lock_deinit(gfx_layer_ctx);
        return ret;
    }

    ret = graphic_clut_table_g3_init();
    if (ret != TD_SUCCESS) {
        graphic_drv_delete_mem(&g_gfbg_coef_buf_addr.buf_base_addr);
        graphic_drv_reg_and_lock_deinit(gfx_layer_ctx);
        return ret;
    }
#ifdef CONFIG_TDE_CLUT_RECT_V2
    ret = graphic_clut_table_g4_init();
    if (ret != TD_SUCCESS) {
        graphic_drv_delete_mem(&g_gfbg_coef_buf_addr.buf_base_addr);
        graphic_drv_delete_mem(&g_clut_table_addr_g3);
        graphic_drv_reg_and_lock_deinit(gfx_layer_ctx);
        return ret;
    }
#endif
    return TD_SUCCESS;
}

td_s32 graphic_drv_resource_exit(td_void)
{
    td_s32 i;
    vo_gfxlayer_context *vo_gfx_ctx = vou_get_gfx_layer_ctx();

    /* lock deinit */
    for (i = 0; i < GFX_MAX; ++i) {
        gfx_spin_lock_deinit(&vo_gfx_ctx[i].spin_lock);
    }

    /* mem delete */
    graphic_drv_delete_mem(&g_gfbg_coef_buf_addr.buf_base_addr);
    graphic_drv_delete_mem(&g_clut_table_addr_g3);
#ifdef CONFIG_TDE_CLUT_RECT_V2
    graphic_drv_delete_mem(&g_clut_table_addr_g4);
#endif
    fb_hal_vou_exit();

    return TD_SUCCESS;
}

td_s32 graphic_drv_set_layer_enable(hal_disp_layer gfx_layer, td_bool enable)
{
    if (fb_hal_set_layer_enable(gfx_layer, enable) == TD_FALSE) {
        graphics_drv_error("graphics layer %d enable failed!\n", gfx_layer);
        return TD_FAILURE;
    }
    fb_hal_set_layer_ck_gt_en(gfx_layer, enable);
    return TD_SUCCESS;
}

td_s32 graphic_drv_get_layer_enable(hal_disp_layer gfx_layer, td_bool *enable)
{
    if (fb_hal_get_layer_enable(gfx_layer, enable) == TD_FALSE) {
        graphics_drv_error("graphics layer %d enable failed!\n", gfx_layer);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}


gfbg_mmz_buffer *graphic_drv_get_clut_table_g3(td_void)
{
    return &g_clut_table_addr_g3;
}

#ifdef CONFIG_TDE_CLUT_RECT_V2
gfbg_mmz_buffer *graphic_drv_get_clut_table_g4(td_void)
{
    return &g_clut_table_addr_g4;
}
#endif

td_s32 fb_graphic_drv_set_csc_coef(hal_disp_layer gfx_layer, const vo_csc *gfx_csc, const csc_coef_param *csc_param)
{
    csc_coef coef;
    hal_csc_mode csc_mode = HAL_CSC_MODE_BUTT;
    vo_gfxlayer_context *gfx_layer_ctx = vou_get_gfx_layer_ctx();
    td_s32 ret;
    const td_u32 dc_pre = 4;
    td_u32 layer_index;

    if (gfx_csc == TD_NULL || csc_param == TD_NULL) {
        return TD_FAILURE;
    }

    if (fb_graphic_drv_get_layer_index(gfx_layer, &layer_index) != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) is invalid!\n", (td_u32)gfx_layer);
        return TD_FALSE;
    }

    ret = vo_drv_get_hal_csc_mode(gfx_csc->csc_matrix, &csc_mode);
    if (ret != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) get CSC mode failed!\n", (td_u32)gfx_layer);
        return ret;
    }

    /* cal CSC coef and CSC dc coef */
    ret = graphic_drv_calc_csc_matrix(gfx_csc, csc_mode, &coef);
    if (ret != TD_SUCCESS) {
        graphics_drv_error("gfx_layer(%u) calculate CSC materix failed!\n", (td_u32)gfx_layer);
        return ret;
    }

    coef.new_csc_clip_max = GFX_CSC_CLIP_MAX;
    coef.new_csc_clip_min = GFX_CSC_CLIP_MIN;
    coef.new_csc_scale2p = GFX_CSC_SCALE;

    coef.csc_in_dc0 = (td_s32)dc_pre * coef.csc_in_dc0;
    coef.csc_in_dc1 = (td_s32)dc_pre * coef.csc_in_dc1;
    coef.csc_in_dc2 = (td_s32)dc_pre * coef.csc_in_dc2;

    coef.csc_out_dc0 = (td_s32)dc_pre * coef.csc_out_dc0;
    coef.csc_out_dc1 = (td_s32)dc_pre * coef.csc_out_dc1;
    coef.csc_out_dc2 = (td_s32)dc_pre * coef.csc_out_dc2;

    /* set CSC coef and CSC dc coef */
    fb_hal_layer_set_csc_coef(gfx_layer, &coef);

    /* copy to drv */
    ret = memcpy_s(&gfx_layer_ctx[layer_index].gfx_csc, sizeof(vo_csc), gfx_csc, sizeof(vo_csc));
    gfbg_unequal_eok_return(ret);
    ret = memcpy_s(&gfx_layer_ctx[layer_index].coef_param, sizeof(csc_coef_param), csc_param, sizeof(csc_coef_param));
    if (ret != EOK) {
        gfbg_error("secure function failure\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_void gf_func_set_g0zme_mode(td_u32 layer, gf_g0_zme_mode g0_zme_mode, const gf_zme_cfg *cfg)
{
    /* filed declare */
    const td_u32 hfir_order = 1;
    td_s32 lhfir_offset = 0;
    td_s32 chfir_offset = 0;
    td_s32 vtp_offset = 0;
    td_s32 vbtm_offset = 0;

    const td_ulong zme_hprec = ZME_HPREC;
    const td_ulong zme_vprec = ZME_VPREC;
    td_u32 hratio;
    td_u32 vratio;
    ot_unused(layer);
    if (cfg == TD_NULL) {
        return;
    }

    hratio = (cfg->in_width * zme_hprec) / cfg->out_width;
    vratio = (cfg->in_height * zme_vprec) / cfg->out_height;

    if (g0_zme_mode == VDP_G0_ZME_TYP) {
        /* typ mode */
        lhfir_offset = 0;
        chfir_offset = 0;
        vtp_offset = 0;
        vbtm_offset = (-1) * (td_s64)zme_vprec / 2; /* 2 alg data */
    }

    /* drv transfer */
    hal_g0_zme_set_ck_gt_en(cfg->ck_gt_en);
    hal_g0_zme_set_out_width(cfg->out_width);
    hal_g0_zme_set_hfir_en(cfg->hfir_en);
    hal_g0_zme_set_ahfir_mid_en(cfg->ahmid_en);
    hal_g0_zme_set_lhfir_mid_en(cfg->lhmid_en);
    hal_g0_zme_set_chfir_mid_en(cfg->lhmid_en);
    hal_g0_zme_set_lhfir_mode(cfg->lhfir_mode);
    hal_g0_zme_set_ahfir_mode(cfg->ahfir_mode);
    hal_g0_zme_set_hfir_order(hfir_order);
    hal_g0_zme_set_hratio(hratio);
    hal_g0_zme_set_lhfir_offset(lhfir_offset);
    hal_g0_zme_set_chfir_offset(chfir_offset);
    hal_g0_zme_set_out_pro(cfg->out_pro);
    hal_g0_zme_set_out_height(cfg->out_height);
    hal_g0_zme_set_vfir_en(cfg->vfir_en);
    hal_g0_zme_set_avfir_mid_en(cfg->avmid_en);
    hal_g0_zme_set_lvfir_mid_en(cfg->lvmid_en);
    hal_g0_zme_set_cvfir_mid_en(cfg->lvmid_en);
    hal_g0_zme_set_lvfir_mode(cfg->lvfir_mode);
    hal_g0_zme_set_vafir_mode(cfg->avfir_mode);
    hal_g0_zme_set_vratio(vratio);
    hal_g0_zme_set_vtp_offset(vtp_offset);
    hal_g0_zme_set_vbtm_offset(vbtm_offset);
}

#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
td_void gf_func_set_g1zme_mode(td_u32 layer, gf_g1_zme_mode g1_zme_mode, const gf_zme_cfg *cfg)
{
    /* filed declare */
    const td_u32 hfir_order = 1;
    td_s32 lhfir_offset = 0;
    td_s32 chfir_offset = 0;
    td_s32 vtp_offset = 0;
    td_s32 vbtm_offset = 0;

    const td_ulong zme_hprec = ZME_HPREC;
    const td_ulong zme_vprec = ZME_VPREC;
    td_u32 hratio;
    td_u32 vratio;
    ot_unused(layer);
    if (cfg == TD_NULL) {
        return;
    }

    hratio = (cfg->in_width * zme_hprec) / cfg->out_width;
    vratio = (cfg->in_height * zme_vprec) / cfg->out_height;

    if (g1_zme_mode == VDP_G1_ZME_TYP) {
        /* typ mode */
        lhfir_offset = 0;
        chfir_offset = 0;
        vtp_offset = 0;
        vbtm_offset = (-1) * (td_s64)zme_vprec / 2; /* 2 alg data */
    }

    /* drv transfer */
    hal_g1_zme_set_ck_gt_en(cfg->ck_gt_en);
    hal_g1_zme_set_out_width(cfg->out_width);
    hal_g1_zme_set_hfir_en(cfg->hfir_en);
    hal_g1_zme_set_ahfir_mid_en(cfg->ahmid_en);
    hal_g1_zme_set_lhfir_mid_en(cfg->lhmid_en);
    hal_g1_zme_set_chfir_mid_en(cfg->lhmid_en);
    hal_g1_zme_set_lhfir_mode(cfg->lhfir_mode);
    hal_g1_zme_set_ahfir_mode(cfg->ahfir_mode);
    hal_g1_zme_set_hfir_order(hfir_order);
    hal_g1_zme_set_hratio(hratio);
    hal_g1_zme_set_lhfir_offset(lhfir_offset);
    hal_g1_zme_set_chfir_offset(chfir_offset);
    hal_g1_zme_set_out_pro(cfg->out_pro);
    hal_g1_zme_set_out_height(cfg->out_height);
    hal_g1_zme_set_vfir_en(cfg->vfir_en);
    hal_g1_zme_set_avfir_mid_en(cfg->avmid_en);
    hal_g1_zme_set_lvfir_mid_en(cfg->lvmid_en);
    hal_g1_zme_set_cvfir_mid_en(cfg->lvmid_en);
    hal_g1_zme_set_lvfir_mode(cfg->lvfir_mode);
    hal_g1_zme_set_vafir_mode(cfg->avfir_mode);
    hal_g1_zme_set_vratio(vratio);
    hal_g1_zme_set_vtp_offset(vtp_offset);
    hal_g1_zme_set_vbtm_offset(vbtm_offset);
}
#endif

static td_u32 vo_drv_write_ddr(td_u8 *addr, gfbg_drv_u128 *data128)
{
    td_u32 ii;
    td_u32 data_arr[GFBG_DATEARR_RANGE] = {
        data128->data0, data128->data1,
        data128->data2, data128->data3
    };
    td_u8 *addr_tmp = 0;

    td_u32 data_tmp;

    for (ii = 0; ii < GFBG_DATEARR_RANGE; ii++) {
        addr_tmp = addr + ii * GFBG_DATEARR_RANGE;
        data_tmp = data_arr[ii];
        *(td_u32 *)addr_tmp = data_tmp;
    }

    return 0;
}

static td_s32 vo_drv_push128(gfbg_drv_u128 *data128, td_u32 coef_data, td_u32 bit_len)
{
    coef_data = coef_data & (0xFFFFFFFF >> (32 - bit_len)); /* 32 alg data */

    if (data128->depth < 32) {                  /* 32 alg data */
        if ((data128->depth + bit_len) <= 32) { /* 32 alg data */
            data128->data0 = (coef_data << data128->depth) | data128->data0;
        } else {
            data128->data0 = (coef_data << data128->depth) | data128->data0;
            data128->data1 = coef_data >> (32 - data128->depth % 32); /* 32 alg data */
        }
    } else if ((data128->depth >= 32) && (data128->depth < 64)) { /* 32 64 alg data */
        if ((data128->depth + bit_len) <= 64) {
            data128->data1 = (coef_data << (data128->depth % 32)) | data128->data1; /* 32 alg data */
        } else {
            data128->data1 = (coef_data << (data128->depth % 32)) | data128->data1; /* 32 alg data */
            data128->data2 = coef_data >> (32 - data128->depth % 32);               /* 32 alg data */
        }
    } else if ((data128->depth >= 64) && (data128->depth < 96)) {                   /* 96 64 alg data */
        if ((data128->depth + bit_len) <= 96) {                                     /* 96 alg data */
            data128->data2 = (coef_data << (data128->depth % 32)) | data128->data2; /* 32 alg data */
        } else {
            data128->data2 = (coef_data << (data128->depth % 32)) | data128->data2; /* 32 alg data */
            data128->data3 = coef_data >> (32 - data128->depth % 32);               /* 32 alg data */
        }
    } else if (data128->depth >= 96) {                                              /* 96 alg data */
        if ((data128->depth + bit_len) <= 128) {                                    /* 128 alg data */
            data128->data3 = (coef_data << (data128->depth % 32)) | data128->data3; /* 32 alg data */
        }
    }

    data128->depth = data128->depth + bit_len;

    if (data128->depth <= 128) { /* 128 alg data */
        return TD_SUCCESS;
    } else {
        return TD_FAILURE;
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

static td_void send_coef_coef_array(const gfbg_drv_coef_send_cfg *cfg, gfbg_drv_u128 *data128, td_u32 coef_cnt)
{
    td_s32 tmp_data = 0;
    td_u32 nn;
    for (nn = 0; nn < cfg->lut_num; nn++) {
        switch (cfg->data_type) {
            case DRV_COEF_DATA_TYPE_S16:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((td_s16 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            case DRV_COEF_DATA_TYPE_U16:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((td_u16 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            case DRV_COEF_DATA_TYPE_U32:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((td_u32 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            case DRV_COEF_DATA_TYPE_S32:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((td_s32 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            case DRV_COEF_DATA_TYPE_S8:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((td_s8 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            case DRV_COEF_DATA_TYPE_U8:
                if (coef_cnt < cfg->lut_length[nn]) {
                    tmp_data = (((td_u8 **)cfg->coef_array)[nn][coef_cnt]);
                }
                break;
            default:
                break;
        }

        if (vo_drv_push128(data128, tmp_data, cfg->coef_bit_length[nn]) != TD_SUCCESS) {
            gfbg_error("WARNING: data128's depth bigger than 128\n");
        }
    }
    return;
}

static td_u8 *vo_drv_send_coef(const gfbg_drv_coef_send_cfg *cfg)
{
    td_u32 kk;
    td_u32 mm;
    td_u8 *addr_base;
    td_u32 addr_offset = 0;
    td_u8 *addr;

    td_u32 cycle_num;

    gfbg_drv_u128 data128 = {0};
    td_u32 coef_cnt;

    td_u32 total_burst_num;
    td_u32 max_len;

    addr_base = cfg->coef_addr;

    /* data type conversion */
    addr = addr_base;

    cycle_num = cfg->cycle_num;

    /* send data */
    max_len = vo_drv_find_max(cfg->lut_length, cfg->lut_num);
    if (cfg->burst_num == 1 && (cfg->data_type < DRV_COEF_DATA_TYPE_BUTT && cfg->data_type >= 0)) {
        total_burst_num = (max_len + cycle_num - 1) / cycle_num;
            for (kk = 0; kk < total_burst_num; kk++) {
            (td_void)memset_s((td_void *)&data128, sizeof(data128), 0, sizeof(data128));

                for (mm = 0; mm < cycle_num; mm++) {
                    coef_cnt = kk * cycle_num + mm;
                send_coef_coef_array(cfg, &data128, coef_cnt);
            }
            addr = addr_base + addr_offset;
            addr_offset = addr_offset + 16; /* 16 alg data */
            vo_drv_write_ddr(addr, &data128);
        }
    }

    return (addr_base + addr_offset);
}

td_void gf_drv_set_g0zme_coef(td_s16 *coef_h, td_s16 *coef_v)
{
    gfbg_drv_coef_send_cfg coef_send;
    td_u8 *addr = 0;

    td_void *coef_array[1] = { coef_h };
    td_u32 lut_length[1] = { 64 };
    td_u32 coef_bit_length[1] = { 16 };

    addr = g_gfbg_coef_buf_addr.coef_vir_addr[GFBG_COEF_BUF_G0ZME];

    coef_send.coef_addr = addr;
    coef_send.lut_num = 1;
    coef_send.burst_num = 1;
    coef_send.cycle_num = 8; /* 8 alg data */
    coef_send.coef_array = coef_array;
    coef_send.lut_length = lut_length;
    coef_send.coef_bit_length = coef_bit_length;
    coef_send.data_type = DRV_COEF_DATA_TYPE_S16;

    addr = vo_drv_send_coef(&coef_send);
    coef_array[0] = coef_v;
    lut_length[0] = 128; /* 128 alg data */
    coef_bit_length[0] = 16; /* 16 alg data */

    coef_send.coef_addr = addr;
    coef_send.cycle_num = 8; /* 8 alg data */
    coef_send.coef_array = coef_array;
    coef_send.lut_length = lut_length;
    coef_send.coef_bit_length = coef_bit_length;
    coef_send.data_type = DRV_COEF_DATA_TYPE_S16;

    vo_drv_send_coef(&coef_send);
}

#define GFBG_COEF_V_RANGE     16
#define GFBG_COEF_H_RANGE     8
#define GFBG_COEF_RANGE       8
#define GFBG_COEF_V_NEW_RANGE 128
#define GFBG_COEF_H_NEW_RANGE 64

td_s16 g_coef_v[GFBG_COEF_V_RANGE][GFBG_COEF_RANGE] = {
    { 0, 0, 0, 0, 0,  63, 0,  0 },
    { 0, 0, 0, 0, 6,  51, 13, -6 },
    { 0, 0, 0, 0, 4,  51, 16, -7 },
    { 0, 0, 0, 0, 1,  50, 20, -7 },
    { 0, 0, 0, 0, -1, 49, 24, -8 },
    { 0, 0, 0, 0, -3, 47, 28, -8 },
    { 0, 0, 0, 0, -4, 45, 31, -8 },
    { 0, 0, 0, 0, -6, 42, 35, -7 },
    { 0, 0, 0, 0, -7, 39, 39, -7 },
    { 0, 0, 0, 0, -7, 35, 42, -6 },
    { 0, 0, 0, 0, -8, 31, 45, -4 },
    { 0, 0, 0, 0, -8, 28, 47, -3 },
    { 0, 0, 0, 0, -8, 24, 49, -1 },
    { 0, 0, 0, 0, -7, 20, 50, 1 },
    { 0, 0, 0, 0, -7, 16, 51, 4 },
    { 0, 0, 0, 0, -6, 13, 51, 6 }
};

td_s16 g_coef_h[GFBG_COEF_H_RANGE][GFBG_COEF_RANGE] = {
    { 0, 0, 0,  0,  63, 0,  0,  0 },
    { 0, 0, -4, 4,  52, 16, -6, 2 },
    { 0, 0, -2, -1, 48, 24, -7, 2 },
    { 0, 0, -1, -4, 44, 31, -7, 1 },
    { 0, 0, 1,  -7, 38, 38, -7, 1 },
    { 0, 0, 1,  -7, 31, 44, -4, -1 },
    { 0, 0, 2,  -7, 24, 48, -1, -2 },
    { 0, 0, 2,  -6, 16, 52, 4,  -4 }
};

td_s16 g_coef_h_new[GFBG_COEF_H_NEW_RANGE];
td_s16 g_coef_v_new[GFBG_COEF_V_NEW_RANGE];

td_void gf_vset_zme_coef(gf_rm_coef_mode coef_mode)
{
    td_u32 ii;

    if (coef_mode == GF_RM_COEF_MODE_TYP) {
        for (ii = 0; ii < 8; ii++) {                    /* 8 alg data */
            g_coef_h_new[ii * 8 + 0] = g_coef_h[ii][7]; /* 8 7 alg data */
            g_coef_h_new[ii * 8 + 1] = g_coef_h[ii][6]; /* 8 6 alg data */
            g_coef_h_new[ii * 8 + 2] = g_coef_h[ii][5]; /* 8 5 2 alg data */
            g_coef_h_new[ii * 8 + 3] = g_coef_h[ii][4]; /* 8 4 3 alg data */
            g_coef_h_new[ii * 8 + 4] = g_coef_h[ii][3]; /* 8 4 3 alg data */
            g_coef_h_new[ii * 8 + 5] = g_coef_h[ii][2]; /* 8 5 2 alg data */
            g_coef_h_new[ii * 8 + 6] = g_coef_h[ii][1]; /* 8 6 alg data */
            g_coef_h_new[ii * 8 + 7] = g_coef_h[ii][0]; /* 8 7 alg data */
        }

        for (ii = 0; ii < 16; ii++) {                   /* 16 alg data */
            g_coef_v_new[ii * 8 + 0] = g_coef_v[ii][7]; /* 8 7 alg data */
            g_coef_v_new[ii * 8 + 1] = g_coef_v[ii][6]; /* 8 6 alg data */
            g_coef_v_new[ii * 8 + 2] = g_coef_v[ii][5]; /* 8 5 2 alg data */
            g_coef_v_new[ii * 8 + 3] = g_coef_v[ii][4]; /* 8 4 3 alg data */
            g_coef_v_new[ii * 8 + 4] = g_coef_v[ii][3]; /* 8 4 3 alg data */
            g_coef_v_new[ii * 8 + 5] = g_coef_v[ii][2]; /* 8 5 2 alg data */
            g_coef_v_new[ii * 8 + 6] = g_coef_v[ii][1]; /* 8 6 alg data */
            g_coef_v_new[ii * 8 + 7] = g_coef_v[ii][0]; /* 8 7 alg data */
        }
    }

    /* send coef to DDR */
    gf_drv_set_g0zme_coef(g_coef_h_new, g_coef_v_new);
}

static td_void graphic_drv_cfg_zme_info(gf_zme_cfg *cfg, const gf_zme_cfg *zme_cfg)
{
    cfg->ck_gt_en = 1;
    cfg->out_pro = VDP_RMODE_PROGRESSIVE;

    cfg->in_width = zme_cfg->in_width;
    cfg->in_height = zme_cfg->in_height;
    cfg->out_width = zme_cfg->out_width;
    cfg->out_height = zme_cfg->out_height;
    cfg->lhmid_en = 1;
    cfg->ahmid_en = 1;
    cfg->lhfir_mode = 1;
    cfg->ahfir_mode = 1;
    cfg->lvmid_en = 1;
    cfg->avmid_en = 1;
    cfg->lvfir_mode = 1;
    cfg->avfir_mode = 1;
}

td_bool graphic_drv_enable_zme(td_u32 layer, const gf_zme_cfg *zme_cfg, td_bool enable_zme)
{
    gf_zme_cfg cfg;

    if ((zme_cfg == TD_NULL) || (zme_cfg->in_width == 0) || (zme_cfg->in_height == 0) ||
        (zme_cfg->out_width == 0) || (zme_cfg->out_height == 0)) {
        return TD_FALSE;
    }

    graphic_drv_cfg_zme_info(&cfg, zme_cfg);

    if (enable_zme) {
        cfg.hfir_en = 1;
        cfg.vfir_en = 1;

        if (layer == HAL_DISP_LAYER_GFX0) {
            gf_func_set_g0zme_mode(HAL_DISP_LAYER_GFX0, VDP_G0_ZME_TYP, &cfg);
        } else {
#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
            gf_func_set_g1zme_mode(HAL_DISP_LAYER_GFX1, VDP_G1_ZME_TYP, &cfg);
#endif
        }
        /* It will be reset when VO exit, so config again */
        graphic_drv_vhd_coef_buf_addr_distribute(&g_gfbg_coef_buf_addr);
        gf_vset_zme_coef(GF_RM_COEF_MODE_TYP);
#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
        fb_hal_para_set_para_up_vhd_chn(1);
#endif
#ifdef CONFIG_GFBG_G0_SUPPORT_ZME_CHN0
        fb_hal_para_set_para_up_vhd_chn(0);
#else
        fb_hal_para_set_para_up_vhd_chn(6); /* chn 6 */
#endif
    } else {
        cfg.hfir_en = 0;
        cfg.vfir_en = 0;

        if (layer == HAL_DISP_LAYER_GFX0) {
            gf_func_set_g0zme_mode(HAL_DISP_LAYER_GFX0, VDP_G0_ZME_TYP, &cfg);
        } else {
#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
            gf_func_set_g1zme_mode(HAL_DISP_LAYER_GFX1, VDP_G1_ZME_TYP, &cfg);
#endif
        }
    }

    return TD_TRUE;
}

td_void graphic_drv_int_clear(td_u32 int_clear, td_s32 irq)
{
    ot_unused(irq);
    /* vo double interrupt, read and mask use interrupt 1, but clear use interrupt 0 as video */
    fb_hal_disp_clear_int_status(int_clear);
    return;
}

#ifdef CONFIG_OT_VO_FB_SEPARATE
td_u32 graphic_drv_int_get_status(td_void)
{
    return fb_hal_disp_get_int_status(GFBG_INTREPORT_ALL);
}
#endif

td_s32 graphic_drv_get_interrupt_dev(td_u32 int_status, ot_vo_dev *vo_dev)
{
    if (vo_dev == TD_NULL) {
        gfbg_error("vo_dev pointer NULL\n");
        return TD_FAILURE;
    }
    if (int_status & GFBG_INTMSK_DHD0_VTTHD2) {
        graphics_drv_debug("Graphic: DHD0 INTERRUPT\n");
        graphic_drv_int_clear(GFBG_INTMSK_DHD0_VTTHD2, VOU1_IRQ_NR);
        *vo_dev = VO_DEV_DHD0;
    } else if (int_status & GFBG_INTMSK_DHD0_VTTHD3) {
        graphics_drv_debug("Graphic: DHD0 INTERRUPT\n");
        graphic_drv_int_clear(GFBG_INTMSK_DHD0_VTTHD3, VOU1_IRQ_NR);
        *vo_dev = VO_DEV_DHD0;
    } else if (int_status & GFBG_INTMSK_DHD1_VTTHD2) {
        graphics_drv_debug("Graphic: DHD1 INTERRUPT\n");
        graphic_drv_int_clear(GFBG_INTMSK_DHD1_VTTHD2, VOU1_IRQ_NR);
        *vo_dev = VO_DEV_DHD1;
    } else if (int_status & GFBG_INTMSK_DHD1_VTTHD3) {
        graphics_drv_debug("Graphic: DHD1 INTERRUPT\n");
        graphic_drv_int_clear(GFBG_INTMSK_DHD1_VTTHD3, VOU1_IRQ_NR);
        *vo_dev = VO_DEV_DHD1;
    } else if (int_status & GFBG_INTMSK_DSD_VTTHD2) {
        graphics_drv_debug("Graphic: DSD INTERRUPT\n");
        graphic_drv_int_clear(GFBG_INTMSK_DSD_VTTHD2, VOU1_IRQ_NR);
        *vo_dev = VO_DEV_DSD0;
    } else if (int_status & GFBG_INTMSK_DSD_VTTHD3) {
        graphics_drv_debug("Graphic: DSD INTERRUPT\n");
        graphic_drv_int_clear(GFBG_INTMSK_DSD_VTTHD3, VOU1_IRQ_NR);
        *vo_dev = VO_DEV_DSD0;
    } else {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_void graphics_drv_set_tde_sync(td_bool is_sync, td_bool is_hw_mute_clr_en, td_bool is_mute_en,
                                  td_u32 safe_dist)
{
    hal_gfx_set_bind_mode(is_sync);
    hal_gfx_set_hardware_mute_clr(is_hw_mute_clr_en);
    hal_gfx_set_tde_safe_dis(safe_dist);
    hal_gfx_set_ld_mute_en(is_mute_en);
    return;
}
