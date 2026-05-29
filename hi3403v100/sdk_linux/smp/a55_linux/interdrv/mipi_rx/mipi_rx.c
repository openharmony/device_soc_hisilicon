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

#include <linux/types.h>
#include "ot_osal.h"
#include "type.h"
#include "securec.h"
#include "ot_mipi_rx.h"
#include "ot_mipi_rx_mod_init.h"
#include "mipi_rx_hal.h"
#include "dev_ext.h"
#include "ot_common.h"

/* Constants for reported magic-number values. */
#define MIPI_RX_ARRAY_INDEX_2                  2
#define MIPI_RX_ARRAY_INDEX_3                  3
#define MIPI_RX_LANE_ID_INDEX_2                2
#define MIPI_RX_LANE_ID_INDEX_3                3
#define MIPI_RX_LANE_ID_INDEX_4                4
#define MIPI_RX_LANE_ID_INDEX_5                5
#define MIPI_RX_LANE_ID_INDEX_6                6
#define MIPI_RX_LANE_ID_INDEX_7                7

/* macro definition */
#define MIPI_RX_DEV_NAME  "ot_mipi_rx"
#define MIPI_RX_PROC_NAME "mipi_rx"

#define OT_MEDIA_DYNAMIC_MINOR UMAP_MIPI_RX_MINOR_BASE

#define COMBO_DEV_MAX_NUM  4
#define COMBO_MAX_LANE_NUM 8

#define COMBO_MIN_WIDTH  32
#define COMBO_MIN_HEIGHT 32

/* global variables definition */
typedef struct {
    lane_divide_mode_t lane_divide_mode;
    unsigned char hs_mode_cfged;
    combo_dev_attr_t combo_dev_attr[COMBO_DEV_MAX_NUM];
    unsigned char dev_valid[COMBO_DEV_MAX_NUM];
    unsigned char dev_cfged[COMBO_DEV_MAX_NUM];
    unsigned int lane_bitmap[COMBO_DEV_MAX_NUM];
} mipi_dev_ctx_t;

static osal_dev_t *g_mipi_rx_dev = NULL;

static osal_mutex_t g_mipi_mutex;

static mipi_dev_ctx_t g_mipi_dev_ctx;
static osal_spinlock_t g_mipi_ctx_spinlock;

void mipi_rx_set_irq_num(unsigned int irq_num)
{
    mipi_rx_drv_set_irq_num(irq_num);
}

void mipi_rx_set_regs(const void *regs)
{
    mipi_rx_drv_set_regs((mipi_rx_regs_type_t *)regs);
}

/* function definition */
static unsigned char mipi_is_hs_mode_cfged(void)
{
    unsigned char hs_mode_cfged;

    osal_spin_lock(&g_mipi_ctx_spinlock);
    hs_mode_cfged = g_mipi_dev_ctx.hs_mode_cfged;
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    return hs_mode_cfged;
}

static unsigned char mipi_is_dev_valid(combo_dev_t devno)
{
    unsigned char dev_valid;

    osal_spin_lock(&g_mipi_ctx_spinlock);
    dev_valid = g_mipi_dev_ctx.dev_valid[devno];
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    return dev_valid;
}

static unsigned char mipi_is_dev_cfged(combo_dev_t devno)
{
    unsigned char dev_cfged;

    osal_spin_lock(&g_mipi_ctx_spinlock);
    dev_cfged = g_mipi_dev_ctx.dev_cfged[devno];
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    return dev_cfged;
}

static int check_lane_id(combo_dev_t devno, input_mode_t input_mode, const short p_lane_id[])
{
    int lane_num;
    int all_lane_id_invalid_flag = 1;
    int i;
    int j;
    lane_divide_mode_t cur_lane_divide_mode;

    if (input_mode == INPUT_MODE_MIPI) {
        lane_num = MIPI_LANE_NUM;
    } else if (input_mode == INPUT_MODE_LVDS) {
        lane_num = LVDS_LANE_NUM;
    } else {
        return 0;
    }

    osal_spin_lock(&g_mipi_ctx_spinlock);
    cur_lane_divide_mode = g_mipi_dev_ctx.lane_divide_mode;
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    for (i = 0; i < lane_num; i++) {
        int temp_id = p_lane_id[i];
        int lane_valid;

        if (temp_id < -1 || temp_id >= COMBO_MAX_LANE_NUM) {
            ot_err("lane_id[%d] is invalid value %d.\n", i, temp_id);
            return -1;
        }

        if (temp_id == -1) {
            continue;
        }

        all_lane_id_invalid_flag = 0;

        for (j = i + 1; j < lane_num; j++) {
            if (temp_id == p_lane_id[j]) {
                ot_err("lane_id[%d] can't be same value %d as lane_id[%d]\n", i, temp_id, j);
                return -1;
            }
        }

        lane_valid = mipi_rx_drv_is_lane_valid(devno, temp_id, cur_lane_divide_mode);
        if (!lane_valid) {
            ot_err("lane_id[%d] %d is invalid in hs_mode %d\n", i, temp_id, cur_lane_divide_mode);
            return -1;
        }
    }

    if (all_lane_id_invalid_flag) {
        ot_err("all lane_id is invalid!\n");
        return -1;
    }

    return 0;
}

static unsigned int mipi_get_lane_bitmap(input_mode_t input_mode,
    const short *p_lane_id, unsigned int *p_total_lane_num)
{
    unsigned int lane_bitmap = 0;
    int lane_num;
    int total_lane_num;
    int i;

    if (input_mode == INPUT_MODE_MIPI) {
        lane_num = MIPI_LANE_NUM;
    } else {
        lane_num = LVDS_LANE_NUM;
    }

    total_lane_num = 0;

    for (i = 0; i < lane_num; i++) {
        short tmp_lane_id;
        tmp_lane_id = p_lane_id[i];

        if (tmp_lane_id != -1) {
            lane_bitmap = lane_bitmap | (1 << (unsigned short)(tmp_lane_id));
            total_lane_num++;
        }
    }

    *p_total_lane_num = total_lane_num;

    return lane_bitmap;
}

static void mipi_set_phy_config(combo_dev_t devno, input_mode_t input_mode, unsigned int lane_bitmap)
{
    unsigned int lane_mask;
    lane_divide_mode_t cur_lane_divide_mode;

    osal_spin_lock(&g_mipi_ctx_spinlock);
    cur_lane_divide_mode = g_mipi_dev_ctx.lane_divide_mode;
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    lane_mask = mipi_rx_drv_get_lane_mask(devno, cur_lane_divide_mode);

    mipi_rx_drv_set_phy_config(input_mode, lane_bitmap, lane_mask);
}

static int mipi_check_comb_dev_attr(const combo_dev_attr_t *p_attr)
{
    if (p_attr->devno >= COMBO_DEV_MAX_NUM) {
        ot_err("invalid combo_dev number(%d).\n", p_attr->devno);
        return -1;
    }

    if (p_attr->input_mode >= INPUT_MODE_BUTT) {
        ot_err("invalid input_mode(%d).\n", p_attr->input_mode);
        return -1;
    }

    if (p_attr->data_rate > MIPI_DATA_RATE_X2) {
        ot_err("invalid data_rate(%d).\n", p_attr->data_rate);
        return -1;
    }

    if (p_attr->img_rect.x < 0 || p_attr->img_rect.y < 0) {
        ot_err("crop x and y (%d, %d) must be great than 0\n", p_attr->img_rect.x, p_attr->img_rect.y);
        return -1;
    }

    if (p_attr->img_rect.width < COMBO_MIN_WIDTH || p_attr->img_rect.height < COMBO_MIN_HEIGHT) {
        ot_err("invalid img_size(%d, %d), can't be smaller than (%d, %d)\n",
               p_attr->img_rect.width, p_attr->img_rect.height, COMBO_MIN_WIDTH, COMBO_MIN_HEIGHT);
        return -1;
    }

    if (p_attr->img_rect.width % 2 != 0 || p_attr->img_rect.height % 2 != 0) { /* 2: size must be even value */
        ot_err("img_size(%d, %d) must be even value\n", p_attr->img_rect.width, p_attr->img_rect.height);
        return -1;
    }

    return 0;
}

static int check_lvds_wdr_mode(const lvds_dev_attr_t *p_attr)
{
    int ret = 0;

    switch (p_attr->wdr_mode) {
        case OT_LVDS_WDR_MODE_2F:
        case OT_LVDS_WDR_MODE_3F:
        case OT_LVDS_WDR_MODE_4F: {
            if (p_attr->vsync_attr.sync_type != LVDS_VSYNC_NORMAL && p_attr->vsync_attr.sync_type != LVDS_VSYNC_SHARE) {
                ot_err("invalid sync_type, must be LVDS_VSYNC_NORMAL or LVDS_VSYNC_SHARE\n");
                ret = -1;
            }
            break;
        }

        case OT_LVDS_WDR_MODE_DOL_2F:
        case OT_LVDS_WDR_MODE_DOL_3F:
        case OT_LVDS_WDR_MODE_DOL_4F: {
            if (p_attr->vsync_attr.sync_type == LVDS_VSYNC_NORMAL) {
                if (p_attr->fid_attr.fid_type != LVDS_FID_IN_SAV && p_attr->fid_attr.fid_type != LVDS_FID_IN_DATA) {
                    ot_err("invalid fid_type, must be LVDS_FID_IN_SAV or LVDS_FID_IN_DATA\n");
                    ret = -1;
                }
            } else if (p_attr->vsync_attr.sync_type == LVDS_VSYNC_HCONNECT) {
                if (p_attr->fid_attr.fid_type != LVDS_FID_NONE && p_attr->fid_attr.fid_type != LVDS_FID_IN_DATA) {
                    ot_err("invalid fid_type, must be LVDS_FID_NONE or LVDS_FID_IN_DATA\n");
                    ret = -1;
                }
            } else {
                ot_err("invalid sync_type, must be LVDS_VSYNC_NORMAL or LVDS_VSYNC_HCONNECT\n");
                ret = -1;
            }
            break;
        }

        default:
            break;
    }

    return ret;
}

static int check_lvds_dev_attr(combo_dev_t devno, const lvds_dev_attr_t *p_attr)
{
    int ret;

    if (p_attr->input_data_type > DATA_TYPE_RAW_16BIT) {
        ot_err("invalid data_type, must be in [%d, %d]\n", DATA_TYPE_RAW_8BIT, DATA_TYPE_RAW_16BIT);
        return -1;
    }

    if (p_attr->wdr_mode >= OT_LVDS_WDR_MODE_BUTT) {
        ot_err("invalid wdr_mode, must be in [%d, %d)\n", OT_LVDS_WDR_MODE_NONE, OT_LVDS_WDR_MODE_BUTT);
        return -1;
    }

    if (p_attr->sync_mode >= LVDS_SYNC_MODE_BUTT) {
        ot_err("invalid sync_mode, must be in [%d, %d)\n", LVDS_SYNC_MODE_SOF, LVDS_SYNC_MODE_BUTT);
        return -1;
    }

    if (p_attr->vsync_attr.sync_type >= LVDS_VSYNC_BUTT) {
        ot_err("invalid vsync_code, must be in [%d, %d)\n", LVDS_VSYNC_NORMAL, LVDS_VSYNC_BUTT);
        return -1;
    }

    if (p_attr->fid_attr.fid_type >= LVDS_FID_BUTT) {
        ot_err("invalid fid_type, must be in [%d, %d)\n", LVDS_FID_NONE, LVDS_FID_BUTT);
        return -1;
    }

    if (p_attr->fid_attr.output_fil != TRUE && p_attr->fid_attr.output_fil != FALSE) {
        ot_err("invalid output_fil, must be OT_TURE or FALSE\n");
        return -1;
    }

    if (p_attr->data_endian >= LVDS_ENDIAN_BUTT) {
        ot_err("invalid lvds_bit_endian, must be in [%d, %d)\n", LVDS_ENDIAN_LITTLE, LVDS_ENDIAN_BUTT);
        return -1;
    }

    if (p_attr->sync_code_endian >= LVDS_ENDIAN_BUTT) {
        ot_err("invalid lvds_bit_endian, must be in [%d, %d)\n", LVDS_ENDIAN_LITTLE, LVDS_ENDIAN_BUTT);
        return -1;
    }

    ret = check_lvds_wdr_mode(p_attr);
    if (ret < 0) {
        ot_err("check_lvds_wdr_mode failed!\n");
        return -1;
    }

    ret = check_lane_id(devno, INPUT_MODE_LVDS, p_attr->lane_id);
    if (ret < 0) {
        ot_err("check_lane_id failed!\n");
        return -1;
    }

    return 0;
}

static void mipi_get_lvds_nxt_sync_code(unsigned int lane_num, const lvds_dev_attr_t *attr,
    unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned int shift_bits;

    if (attr->input_data_type == DATA_TYPE_RAW_10BIT) {
        shift_bits = 8; /* 8 bits one pixel component */
    } else {
        shift_bits = 10; /* 10 bits one pixel component */
    }
    /* SONY DOL N frame and N+1 Frame has the different sync_code
     * N+1 Frame FSET is 1, FSET is the 10th bit
     */
    for (i = 0; i < lane_num; i++) {
        for (j = 0; j < WDR_VC_NUM; j++) {
            for (k = 0; k < SYNC_CODE_NUM; k++) {
                sync_code[i][j][k] = attr->sync_code[i][j][k] + (1 << shift_bits);
            }
        }
    }
}

static void mipi_set_lvds_sync_codes(combo_dev_t devno, const lvds_dev_attr_t *p_attr,
                                     unsigned int total_lane_num, unsigned int lane_bitmap)
{
    unsigned short(*nxt_sync_code)[WDR_VC_NUM][SYNC_CODE_NUM] = NULL;

    if (p_attr->wdr_mode >= OT_LVDS_WDR_MODE_DOL_2F && p_attr->wdr_mode <= OT_LVDS_WDR_MODE_DOL_4F) {
        /* Sony DOL Mode */
        nxt_sync_code = osal_vmalloc(sizeof(unsigned short) * total_lane_num * WDR_VC_NUM * SYNC_CODE_NUM);
        if (nxt_sync_code == NULL) {
            ot_err("alloc memory for nxt_sync_code failed\n");
            return;
        }

        mipi_get_lvds_nxt_sync_code(total_lane_num, p_attr, nxt_sync_code);

        /* Set Dony DOL Line Information */
        if (p_attr->fid_attr.fid_type == LVDS_FID_IN_DATA) {
            mipi_rx_drv_set_dol_line_information(devno, p_attr->wdr_mode);
        }

        /* LVDS_CTRL Sync code */
        mipi_rx_drv_set_lvds_sync_code(devno, total_lane_num, p_attr->lane_id, p_attr->sync_code);
        mipi_rx_drv_set_lvds_nxt_sync_code(devno, total_lane_num, p_attr->lane_id, nxt_sync_code);
        /* PHY Sync code detect setting */
        mipi_rx_drv_set_phy_sync_config(p_attr, lane_bitmap, nxt_sync_code);

        osal_vfree(nxt_sync_code);
    } else {
        /* LVDS_CTRL Sync code */
        mipi_rx_drv_set_lvds_sync_code(devno, total_lane_num, p_attr->lane_id, p_attr->sync_code);
        mipi_rx_drv_set_lvds_nxt_sync_code(devno, total_lane_num, p_attr->lane_id, p_attr->sync_code);
        /* PHY Sync code detect setting */
        mipi_rx_drv_set_phy_sync_config(p_attr, lane_bitmap, p_attr->sync_code);
    }
}

static int mipi_set_lvds_dev_attr(const combo_dev_attr_t *p_combo_dev_attr)
{
    int ret;
    unsigned int lane_bitmap;
    unsigned int lane_num;
    lvds_err_int_cnt_t *lvds_err_int_cnt = NULL;
    align_err_int_cnt_t *align_err_int_cnt = NULL;
    combo_dev_t devno = p_combo_dev_attr->devno;
    const lvds_dev_attr_t *p_lvds_attr = &p_combo_dev_attr->lvds_attr;

    if (mipi_is_hs_mode_cfged() != TRUE) {
        ot_err("mipi must set hs mode before set lvds attr!\n");
        return -1;
    }

    if (!mipi_is_dev_valid(devno)) {
        ot_err("invalid combo dev num after set hs mode!\n");
        return -1;
    }

    ret = check_lvds_dev_attr(devno, p_lvds_attr);
    if (ret < 0) {
        ot_err("check_lvds_dev_attr failed!\n");
        return -1;
    }

    lane_bitmap = mipi_get_lane_bitmap(INPUT_MODE_LVDS, p_lvds_attr->lane_id, &lane_num);
    g_mipi_dev_ctx.lane_bitmap[devno] = lane_bitmap;
    mipi_rx_drv_set_work_mode(devno, INPUT_MODE_LVDS);

    /* image crop */
    mipi_rx_drv_set_lvds_image_rect(devno, &p_combo_dev_attr->img_rect, lane_num);
    mipi_rx_drv_set_lvds_crop_en(devno, TRUE);

    /* data type & mode */
    ret = mipi_rx_drv_set_lvds_wdr_mode(devno, p_lvds_attr->wdr_mode, &p_lvds_attr->vsync_attr,
                                        &p_lvds_attr->fid_attr);
    if (ret < 0) {
        ot_err("set lvds wdr mode failed!\n");
        return -1;
    }

    mipi_rx_drv_set_lvds_ctrl_mode(devno, p_lvds_attr->sync_mode, p_lvds_attr->input_data_type,
                                   p_lvds_attr->data_endian, p_lvds_attr->sync_code_endian);

    /* data rate */
    mipi_rx_drv_set_lvds_data_rate(devno, p_combo_dev_attr->data_rate);

    /* phy lane config */
    mipi_rx_drv_set_link_lane_id(devno, INPUT_MODE_LVDS, p_lvds_attr->lane_id);
    mipi_rx_drv_set_mem_cken(devno, TRUE);
    mipi_rx_drv_set_clr_cken(devno, TRUE);

    mipi_set_phy_config(devno, INPUT_MODE_LVDS, lane_bitmap);

    /* sync codes */
    mipi_set_lvds_sync_codes(devno, p_lvds_attr, lane_num, lane_bitmap);

    /* interrupt mask */
    lvds_err_int_cnt = mipi_rx_drv_get_lvds_err_int_cnt(devno);
    align_err_int_cnt = mipi_rx_drv_get_align_err_int_cnt(devno);
    (void)memset_s(lvds_err_int_cnt, sizeof(lvds_err_int_cnt_t), 0, sizeof(lvds_err_int_cnt_t));
    (void)memset_s(align_err_int_cnt, sizeof(align_err_int_cnt_t), 0, sizeof(align_err_int_cnt_t));

    mipi_rx_drv_set_chn_int_mask(devno);
    mipi_rx_drv_set_lvds_ctrl_int_mask(devno, LVDS_CTRL_INT_MASK);
    mipi_rx_drv_set_align_int_mask(devno, ALIGN0_INT_MASK);

    return 0;
}

static int check_mipi_dev_attr(combo_dev_t devno, const mipi_dev_attr_t *p_attr)
{
    int ret;
    int i;

    if (p_attr->input_data_type >= DATA_TYPE_BUTT) {
        ot_err("invalid input_data_type, must be in [%d, %d)\n", DATA_TYPE_RAW_8BIT, DATA_TYPE_BUTT);
        return -1;
    }

    if (p_attr->wdr_mode >= OT_MIPI_WDR_MODE_BUTT) {
        ot_err("invalid wdr_mode, must be in [%d, %d)\n", OT_MIPI_WDR_MODE_NONE, OT_MIPI_WDR_MODE_BUTT);
        return -1;
    }

    if ((p_attr->wdr_mode != OT_MIPI_WDR_MODE_NONE) && (p_attr->input_data_type >= DATA_TYPE_YUV420_8BIT_NORMAL)) {
        ot_err("It do not support wdr mode when input_data_type is yuv format!\n");
        return -1;
    }

    if (p_attr->wdr_mode == OT_MIPI_WDR_MODE_DT) {
        for (i = 0; i < WDR_VC_NUM; i++) {
            /* data_type must be the CSI-2 reserve Type [0x38, 0x3f] */
            if (p_attr->data_type[i] < 0x38 || p_attr->data_type[i] > 0x3f) {
                ot_err("invalid data_type[%d]: %d, must be in [0x38, 0x3f]\n", i, p_attr->data_type[i]);
                return -1;
            }
        }
    }

    ret = check_lane_id(devno, INPUT_MODE_MIPI, p_attr->lane_id);
    if (ret < 0) {
        ot_err("check_lane_id failed!\n");
        return -1;
    }

    return 0;
}

static void mipi_set_dt_and_mode(combo_dev_t devno, const mipi_dev_attr_t *p_attr)
{
    data_type_t input_data_type;

    input_data_type = p_attr->input_data_type;

    mipi_rx_drv_set_mipi_yuv_dt(devno, input_data_type);

    if (p_attr->wdr_mode == OT_MIPI_WDR_MODE_DT) {
        mipi_rx_drv_set_mipi_wdr_user_dt(devno, input_data_type, p_attr->data_type);
    } else if (p_attr->wdr_mode == OT_MIPI_WDR_MODE_DOL) {
        mipi_rx_drv_set_mipi_dol_id(devno, input_data_type, NULL);
    }

    mipi_rx_drv_set_mipi_wdr_mode(devno, p_attr->wdr_mode);
}

static int mipi_set_mipi_dev_attr(const combo_dev_attr_t *p_combo_dev_attr)
{
    unsigned int lane_bitmap;
    unsigned int lane_num;
    int ret;
    combo_dev_t devno = p_combo_dev_attr->devno;
    mipi_err_int_cnt_t *mipi_err_int_cnt = NULL;
    align_err_int_cnt_t *align_err_int_cnt = NULL;
    const mipi_dev_attr_t *p_mipi_attr = &p_combo_dev_attr->mipi_attr;

    if (mipi_is_hs_mode_cfged() != TRUE) {
        ot_err("mipi must set hs mode before set mipi attr!\n");
        return -1;
    }

    if (!mipi_is_dev_valid(devno)) {
        ot_err("invalid combo dev num after set hs mode!\n");
        return -1;
    }

    ret = check_mipi_dev_attr(devno, p_mipi_attr);
    if (ret < 0) {
        ot_err("check_mipi_dev_attr failed!\n");
        return -1;
    }

    mipi_rx_drv_set_work_mode(devno, INPUT_MODE_MIPI);

    /* image crop */
    mipi_rx_drv_set_mipi_image_rect(devno, &p_combo_dev_attr->img_rect);
    mipi_rx_drv_set_mipi_crop_en(devno, TRUE);

    /* data type & mode */
    mipi_set_dt_and_mode(devno, p_mipi_attr);

    /* data rate */
    mipi_rx_drv_set_data_rate(devno, p_combo_dev_attr->data_rate);

    /* phy lane config */
    mipi_rx_drv_set_link_lane_id(devno, INPUT_MODE_MIPI, p_mipi_attr->lane_id);

    lane_bitmap = mipi_get_lane_bitmap(INPUT_MODE_MIPI, p_mipi_attr->lane_id, &lane_num);
    g_mipi_dev_ctx.lane_bitmap[devno] = lane_bitmap;

    mipi_rx_drv_set_mem_cken(devno, TRUE);

    mipi_set_phy_config(devno, INPUT_MODE_MIPI, lane_bitmap);

    /* interrupt mask */
    mipi_err_int_cnt = mipi_rx_drv_get_mipi_err_int_cnt(devno);
    align_err_int_cnt = mipi_rx_drv_get_align_err_int_cnt(devno);
    (void)memset_s(mipi_err_int_cnt, sizeof(mipi_err_int_cnt_t), 0, sizeof(mipi_err_int_cnt_t));
    (void)memset_s(align_err_int_cnt, sizeof(align_err_int_cnt_t), 0, sizeof(align_err_int_cnt_t));

    mipi_rx_drv_set_chn_int_mask(devno);
    mipi_rx_drv_set_mipi_ctrl_int_mask(devno, MIPI_CTRL_INT_MASK);
    mipi_rx_drv_set_mipi_csi_int_mask(devno, MIPI_CSI_INT_MASK);
    mipi_rx_drv_set_align_int_mask(devno, ALIGN0_INT_MASK);

    return 0;
}

static int mipi_set_combo_dev_attr(const combo_dev_attr_t *p_attr)
{
    int ret;

    ret = mipi_check_comb_dev_attr(p_attr);
    if (ret < 0) {
        ot_err("mipi check combo_dev attr failed!\n");
        return -1;
    }

    if (osal_mutex_lock_interruptible(&g_mipi_mutex)) {
        return -ERESTARTSYS;
    }

    switch (p_attr->input_mode) {
        case INPUT_MODE_LVDS:
        case INPUT_MODE_SUBLVDS:
        case INPUT_MODE_HISPI: {
            ret = mipi_set_lvds_dev_attr(p_attr);
            if (ret < 0) {
                ot_err("mipi set lvds attr failed!\n");
                ret = -1;
            }
            break;
        }

        case INPUT_MODE_MIPI: {
            ret = mipi_set_mipi_dev_attr(p_attr);
            if (ret < 0) {
                ot_err("mipi set mipi attr failed!\n");
                ret = -1;
            }
            break;
        }

        default: {
            ot_err("invalid input mode\n");
            ret = -1;
            break;
        }
    }

    osal_mutex_unlock(&g_mipi_mutex);

    return ret;
}

static int mipi_set_ext_data_type(const ext_data_type_t* data_type)
{
    unsigned int i;
    combo_dev_t devno;
    input_mode_t input_mode;
    data_type_t input_data_type;

    devno = data_type->devno;

    if (devno >= COMBO_DEV_MAX_NUM) {
        ot_err("invalid mipi dev number(%d).\n", devno);
        return TD_FAILURE;
    }

    if (osal_mutex_lock_interruptible(&g_mipi_mutex)) {
        return -ERESTARTSYS;
    }

    if (!mipi_is_dev_cfged(devno)) {
        osal_mutex_unlock(&g_mipi_mutex);
        ot_err("MIPI device %d has not beed configured\n", devno);
        return TD_FAILURE;
    }

    osal_spin_lock(&g_mipi_ctx_spinlock);
    input_mode = g_mipi_dev_ctx.combo_dev_attr[devno].input_mode;
    input_data_type = g_mipi_dev_ctx.combo_dev_attr[devno].mipi_attr.input_data_type;
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    if (input_mode != INPUT_MODE_MIPI) {
        osal_mutex_unlock(&g_mipi_mutex);
        ot_err("devno: %d, input mode: %d, not support set data type\n", devno, input_mode);
        return TD_FAILURE;
    }

    if (data_type->num > MAX_EXT_DATA_TYPE_NUM) {
        osal_mutex_unlock(&g_mipi_mutex);
        ot_err("invalid ext data type num(%d)\n", data_type->num);
        return TD_FAILURE;
    }

    for (i = 0; i < data_type->num; i++) {
        if (data_type->ext_data_bit_width[i] < MIPI_RX_MIN_EXT_DATA_TYPE_BIT_WIDTH ||
            data_type->ext_data_bit_width[i] > MIPI_RX_MAX_EXT_DATA_TYPE_BIT_WIDTH) {
            osal_mutex_unlock(&g_mipi_mutex);
            ot_err("invalid ext data bit width(%d)\n", data_type->ext_data_bit_width[i]);
            return TD_FAILURE;
        }

        if (data_type->ext_data_bit_width[i] % 2 != 0) { /* 2: bit width must be even value */
            ot_err("invalid ext data bit width(%d),must be even value\n", data_type->ext_data_bit_width[i]);
            osal_mutex_unlock(&g_mipi_mutex);
            return TD_FAILURE;
        }
    }

    mipi_rx_drv_set_ext_data_type(data_type, input_data_type);

    osal_mutex_unlock(&g_mipi_mutex);

    return TD_SUCCESS;
}

static int mipi_set_phy_cmvmode(combo_dev_t devno, phy_cmv_mode_t cmv_mode)
{
    input_mode_t input_mode;
    unsigned int lane_bit_map;

    if (devno >= COMBO_DEV_MAX_NUM) {
        ot_err("invalid mipi dev number(%d).\n", devno);
        return -1;
    }

    if (cmv_mode >= PHY_CMV_BUTT) {
        ot_err("invalid common mode voltage mode: %d, must be int [%d, %d)\n",
               cmv_mode, PHY_CMV_GE1200MV, PHY_CMV_BUTT);
        return -1;
    }

    if (osal_mutex_lock_interruptible(&g_mipi_mutex)) {
        return -ERESTARTSYS;
    }

    if (!mipi_is_dev_cfged(devno)) {
        osal_mutex_unlock(&g_mipi_mutex);
        ot_err("MIPI device %d has not beed configured\n", devno);
        return -1;
    }

    osal_spin_lock(&g_mipi_ctx_spinlock);
    input_mode = g_mipi_dev_ctx.combo_dev_attr[devno].input_mode;
    lane_bit_map = g_mipi_dev_ctx.lane_bitmap[devno];
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    if (input_mode != INPUT_MODE_MIPI &&
        input_mode != INPUT_MODE_SUBLVDS &&
        input_mode != INPUT_MODE_LVDS &&
        input_mode != INPUT_MODE_HISPI) {
        osal_mutex_unlock(&g_mipi_mutex);
        ot_err("devno: %d, input mode: %d, not support set common voltage mode\n", devno, input_mode);
        return -1;
    }

    mipi_rx_drv_set_phy_cmvmode(input_mode, cmv_mode, lane_bit_map);

    osal_mutex_unlock(&g_mipi_mutex);

    return 0;
}

static int mipi_reset_sensor(sns_rst_source_t sns_reset_source)
{
    if (sns_reset_source >= SNS_MAX_RST_SOURCE_NUM) {
        ot_err("invalid sns_reset_source(%d).\n", sns_reset_source);
        return -1;
    }

    sensor_drv_reset(sns_reset_source);

    return 0;
}

static int mipi_unreset_sensor(sns_rst_source_t sns_reset_source)
{
    if (sns_reset_source >= SNS_MAX_RST_SOURCE_NUM) {
        ot_err("invalid sns_reset_source(%d).\n", sns_reset_source);
        return -1;
    }

    sensor_drv_unreset(sns_reset_source);

    return 0;
}

static int mipi_reset_mipi_rx(combo_dev_t combo_dev)
{
    if (combo_dev >= MIPI_RX_MAX_DEV_NUM) {
        ot_err("invalid combo_dev num(%d).\n", combo_dev);
        return -1;
    }

    mipi_rx_drv_core_reset(combo_dev);

    return 0;
}

static int mipi_unreset_mipi_rx(combo_dev_t combo_dev)
{
    if (combo_dev >= MIPI_RX_MAX_DEV_NUM) {
        ot_err("invalid combo_dev num(%d).\n", combo_dev);
        return -1;
    }

    mipi_rx_drv_core_unreset(combo_dev);

    return 0;
}

/* magic numbers represents mipi_dev id numbers */
static void mipi_set_dev_valid(lane_divide_mode_t mode)
{
    switch (mode) {
        case LANE_DIVIDE_MODE_0:
            g_mipi_dev_ctx.dev_valid[0] = 1;
            break;
        case LANE_DIVIDE_MODE_1:
            g_mipi_dev_ctx.dev_valid[0] = 1;
            g_mipi_dev_ctx.dev_valid[MIPI_RX_ARRAY_INDEX_2] = 1;
            break;
        case LANE_DIVIDE_MODE_2:
            g_mipi_dev_ctx.dev_valid[0] = 1;
            g_mipi_dev_ctx.dev_valid[MIPI_RX_ARRAY_INDEX_2] = 1;
            g_mipi_dev_ctx.dev_valid[MIPI_RX_ARRAY_INDEX_3] = 1;
            break;
        case LANE_DIVIDE_MODE_3:
            g_mipi_dev_ctx.dev_valid[0] = 1;
            g_mipi_dev_ctx.dev_valid[1] = 1;
            g_mipi_dev_ctx.dev_valid[MIPI_RX_ARRAY_INDEX_2] = 1;
            g_mipi_dev_ctx.dev_valid[MIPI_RX_ARRAY_INDEX_3] = 1;
            break;

        default:
            break;
    }
}

static int mipi_set_hs_mode(lane_divide_mode_t lane_divide_mode)
{
    lane_divide_mode_t old_lane_divide_mode;
    if (lane_divide_mode >= LANE_DIVIDE_MODE_BUTT) {
        ot_err("invalid lane_divide_mode(%d), must be in [%d, %d)\n",
               lane_divide_mode, LANE_DIVIDE_MODE_0, LANE_DIVIDE_MODE_BUTT);
        return -1;
    }

    osal_spin_lock(&g_mipi_ctx_spinlock);
    old_lane_divide_mode = g_mipi_dev_ctx.lane_divide_mode;
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    if (lane_divide_mode == old_lane_divide_mode) {
        return 0;
    }

    mipi_rx_drv_set_hs_mode(lane_divide_mode);

    osal_spin_lock(&g_mipi_ctx_spinlock);
    g_mipi_dev_ctx.lane_divide_mode = lane_divide_mode;
    g_mipi_dev_ctx.hs_mode_cfged = TRUE;
    (void)memset_s(g_mipi_dev_ctx.dev_valid, sizeof(g_mipi_dev_ctx.dev_valid), 0, sizeof(g_mipi_dev_ctx.dev_valid));
    mipi_set_dev_valid(lane_divide_mode);
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    return 0;
}

static int mipi_enable_mipi_rx_clock(combo_dev_t combo_dev)
{
    if (combo_dev >= MIPI_RX_MAX_DEV_NUM) {
        ot_err("invalid combo_dev num(%d).\n", combo_dev);
        return -1;
    }

    mipi_rx_drv_enable_clock(combo_dev);

    return 0;
}

static int mipi_disable_mipi_rx_clock(combo_dev_t combo_dev)
{
    if (combo_dev >= MIPI_RX_MAX_DEV_NUM) {
        ot_err("invalid combo_dev num(%d).\n", combo_dev);
        return -1;
    }

    mipi_rx_drv_disable_clock(combo_dev);

    return 0;
}

static int mipi_enable_sensor_clock(sns_clk_source_t sns_clk_source)
{
    if (sns_clk_source >= SNS_MAX_CLK_SOURCE_NUM) {
        ot_err("invalid sns_clk_source(%d).\n", sns_clk_source);
        return -1;
    }

    sensor_drv_enable_clock(sns_clk_source);

    return 0;
}

static int mipi_disable_sensor_clock(sns_clk_source_t sns_clk_source)
{
    if (sns_clk_source >= SNS_MAX_CLK_SOURCE_NUM) {
        ot_err("invalid sns_clk_source(%d).\n", sns_clk_source);
        return -1;
    }

    sensor_drv_disable_clock(sns_clk_source);

    return 0;
}

static int mipi_ioctl_ret_check(int ret, const char *err_msg)
{
    if (ret < 0) {
        ot_err("%s", err_msg);
        return -1;
    }
    return ret;
}

static int mipi_ioctl_set_dev_attr(unsigned int *argp)
{
    int ret;
    combo_dev_t devno;
    combo_dev_attr_t *pstcombo_dev_attr = (combo_dev_attr_t *)argp;

    ret = mipi_set_combo_dev_attr(pstcombo_dev_attr);
    if (ret < 0) {
        ot_err("mipi set combo_dev attr failed!\n");
        return -1;
    }

    devno = pstcombo_dev_attr->devno;
    osal_spin_lock(&g_mipi_ctx_spinlock);
    g_mipi_dev_ctx.dev_cfged[devno] = TRUE;
    (void)memcpy_s(&g_mipi_dev_ctx.combo_dev_attr[devno], sizeof(combo_dev_attr_t),
                   pstcombo_dev_attr, sizeof(combo_dev_attr_t));
    osal_spin_unlock(&g_mipi_ctx_spinlock);
    return ret;
}

static int mipi_ioctl_reset_sensor(unsigned int cmd, unsigned int *argp)
{
    sns_rst_source_t sns_reset_source = *(sns_rst_source_t *)argp;

    if (cmd == OT_MIPI_RESET_SENSOR) {
        return mipi_ioctl_ret_check(mipi_reset_sensor(sns_reset_source), "mipi reset sensor failed!\n");
    }
    return mipi_ioctl_ret_check(mipi_unreset_sensor(sns_reset_source), "mipi unreset sensor failed!\n");
}

static int mipi_ioctl_reset_mipi(unsigned int cmd, unsigned int *argp)
{
    combo_dev_t combo_dev = *(combo_dev_t *)argp;

    if (cmd == OT_MIPI_RESET_MIPI) {
        return mipi_ioctl_ret_check(mipi_reset_mipi_rx(combo_dev), "mipi reset mipi_rx failed!\n");
    }
    return mipi_ioctl_ret_check(mipi_unreset_mipi_rx(combo_dev), "mipi unreset mipi_rx failed!\n");
}

static int mipi_ioctl_mipi_clock(unsigned int cmd, unsigned int *argp)
{
    combo_dev_t combo_dev = *(combo_dev_t *)argp;

    if (cmd == OT_MIPI_ENABLE_MIPI_CLOCK) {
        return mipi_ioctl_ret_check(mipi_enable_mipi_rx_clock(combo_dev), "mipi enable mipi_rx clock failed!\n");
    }
    return mipi_ioctl_ret_check(mipi_disable_mipi_rx_clock(combo_dev), "mipi disable mipi_rx clock failed!\n");
}

static int mipi_ioctl_sensor_clock(unsigned int cmd, unsigned int *argp)
{
    sns_clk_source_t sns_clk_source = *(sns_clk_source_t *)argp;

    if (cmd == OT_MIPI_ENABLE_SENSOR_CLOCK) {
        return mipi_ioctl_ret_check(mipi_enable_sensor_clock(sns_clk_source), "mipi enable sensor clock failed!\n");
    }
    return mipi_ioctl_ret_check(mipi_disable_sensor_clock(sns_clk_source), "mipi disable sensor clock failed!\n");
}

static long mipi_rx_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    unsigned int *argp = (unsigned int *)(uintptr_t)arg;
    int ret;

    mipi_rx_unused(private_data);

    switch (cmd) {
        case OT_MIPI_SET_DEV_ATTR:
            ret = mipi_ioctl_set_dev_attr(argp);
            break;

        case OT_MIPI_SET_EXT_DATA_TYPE: {
            ext_data_type_t* data_type;

            data_type = (ext_data_type_t*)argp;

            ret = mipi_set_ext_data_type(data_type);
            break;
        }

        case OT_MIPI_SET_PHY_CMVMODE: {
            phy_cmv_t *p_phy_cmv;

            p_phy_cmv = (phy_cmv_t *)argp;

            ret = mipi_set_phy_cmvmode(p_phy_cmv->devno, p_phy_cmv->cmv_mode);
            if (ret < 0) {
                ot_err("mipi set phy cmv mode failed!\n");
                ret = -1;
            }
            break;
        }

        case OT_MIPI_RESET_SENSOR:
        case OT_MIPI_UNRESET_SENSOR:
            ret = mipi_ioctl_reset_sensor(cmd, argp);
            break;

        case OT_MIPI_RESET_MIPI:
        case OT_MIPI_UNRESET_MIPI:
            ret = mipi_ioctl_reset_mipi(cmd, argp);
            break;

        case OT_MIPI_SET_HS_MODE: {
            lane_divide_mode_t lane_divide_mode = *(lane_divide_mode_t *)argp;
            ret = mipi_ioctl_ret_check(mipi_set_hs_mode(lane_divide_mode), "mipi set hs mode failed!\n");
            break;
        }

        case OT_MIPI_ENABLE_MIPI_CLOCK:
        case OT_MIPI_DISABLE_MIPI_CLOCK:
            ret = mipi_ioctl_mipi_clock(cmd, argp);
            break;

        case OT_MIPI_ENABLE_SENSOR_CLOCK:
        case OT_MIPI_DISABLE_SENSOR_CLOCK:
            ret = mipi_ioctl_sensor_clock(cmd, argp);
            break;

        default: {
            ot_err("invalid ot_mipi ioctl cmd\n");
            ret = -1;
            break;
        }
    }

    return ret;
}

#ifdef CONFIG_COMPAT
static long mipi_rx_compat_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    return mipi_rx_ioctl(cmd, arg, private_data);
}
#endif

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
static const char *mipi_get_intput_mode_name(input_mode_t input_mode)
{
    switch (input_mode) {
        case INPUT_MODE_SUBLVDS:
        case INPUT_MODE_LVDS:
        case INPUT_MODE_HISPI:
            return "lvds";

        case INPUT_MODE_MIPI:
            return "mipi";

        default:
            break;
    }

    return "n/a";
}

static const char *mipi_get_raw_data_type_name(data_type_t type)
{
    switch (type) {
        case DATA_TYPE_RAW_8BIT:
            return "raw8";

        case DATA_TYPE_RAW_10BIT:
            return "raw10";

        case DATA_TYPE_RAW_12BIT:
            return "raw12";

        case DATA_TYPE_RAW_14BIT:
            return "raw14";

        case DATA_TYPE_RAW_16BIT:
            return "raw16";

        case DATA_TYPE_YUV420_8BIT_NORMAL:
            return "yuv420_8bit_normal";

        case DATA_TYPE_YUV420_8BIT_LEGACY:
            return "yuv420_8bit_legacy";

        case DATA_TYPE_YUV422_8BIT:
            return "yuv422_8bit";

        case DATA_TYPE_YUV422_PACKED:
            return "yuv422_packed";

        default:
            break;
    }

    return "n/a";
}

static const char *mipi_get_data_rate_name(mipi_data_rate_t data_rate)
{
    switch (data_rate) {
        case MIPI_DATA_RATE_X1:
            return "X1";

        case MIPI_DATA_RATE_X2:
            return "X2";

        default:
            break;
    }

    return "n/a";
}

static const char *mipi_print_mipi_wdr_mode(mipi_wdr_mode_t wdr_mode)
{
    switch (wdr_mode) {
        case OT_MIPI_WDR_MODE_NONE:
            return "none";

        case OT_MIPI_WDR_MODE_VC:
            return "vc";

        case OT_MIPI_WDR_MODE_DT:
            return "dt";

        case OT_MIPI_WDR_MODE_DOL:
            return "dol";

        default:
            break;
    }

    return "n/a";
}

static const char *mipi_print_lvds_wdr_mode(lvds_wdr_mode_t wdr_mode)
{
    switch (wdr_mode) {
        case OT_LVDS_WDR_MODE_NONE:
            return "none";

        case OT_LVDS_WDR_MODE_2F:
            return "2f";

        case OT_LVDS_WDR_MODE_3F:
            return "3f";

        case OT_LVDS_WDR_MODE_4F:
            return "4f";

        case OT_LVDS_WDR_MODE_DOL_2F:
            return "dol_2f";

        case OT_LVDS_WDR_MODE_DOL_3F:
            return "dol_3f";

        case OT_LVDS_WDR_MODE_DOL_4F:
            return "dol_4f";

        default:
            break;
    }

    return "n/a";
}

static const char *mipi_print_lane_divide_mode(lane_divide_mode_t mode)
{
    switch (mode) {
        case LANE_DIVIDE_MODE_0:
            return "8";

        case LANE_DIVIDE_MODE_1:
            return "4+4";

        case LANE_DIVIDE_MODE_2:
            return "4+2+2";

        case LANE_DIVIDE_MODE_3:
            return "2+2+2+2";

        default:
            break;
    }

    return "n/a";
}

static void proc_show_mipi_device(osal_proc_entry_t *s)
{
    const char *wdr_mode = NULL;
    combo_dev_t devno;
    input_mode_t input_mode;
    data_type_t data_type;

    osal_seq_printf(s,
                    "\n-----mipi dev attr--------------------"
                    "---------------------------------------------------------------------------------\n");

    osal_seq_printf(s, "%8s" "%11s" "%11s" "%20s" "%10s" "%8s" "%8s" "%8s" "%8s" "\n",
                    "devno", "work_mode", "data_rate", "data_type", "wdr_mode", "img_x", "img_y", "img_w", "img_h");

    for (devno = 0; devno < MIPI_RX_MAX_DEV_NUM; devno++) {
        combo_dev_attr_t *pdev_attr = &g_mipi_dev_ctx.combo_dev_attr[devno];

        input_mode = pdev_attr->input_mode;

        if (!mipi_is_dev_cfged(devno)) {
            continue;
        }

        if (input_mode == INPUT_MODE_MIPI) {
            data_type = pdev_attr->mipi_attr.input_data_type;
            wdr_mode = mipi_print_mipi_wdr_mode(pdev_attr->mipi_attr.wdr_mode);
        } else {
            data_type = pdev_attr->lvds_attr.input_data_type;
            wdr_mode = mipi_print_lvds_wdr_mode(pdev_attr->lvds_attr.wdr_mode);
        }

        osal_seq_printf(s, "%8d" "%11s" "%11s" "%20s" "%10s" "%8d" "%8d" "%8d" "%8d" "\n",
                        devno,
                        mipi_get_intput_mode_name(input_mode),
                        mipi_get_data_rate_name(pdev_attr->data_rate),
                        mipi_get_raw_data_type_name(data_type),
                        wdr_mode,
                        pdev_attr->img_rect.x,
                        pdev_attr->img_rect.y,
                        pdev_attr->img_rect.width,
                        pdev_attr->img_rect.height);
    }
}

/* magic numbers represents the indexes of lane_id array */
static void proc_show_mipi_lane(osal_proc_entry_t *s)
{
    combo_dev_t devno;
    input_mode_t input_mode;

    osal_seq_printf(s,
                    "\n-----mipi lane info-------------------"
                    "----------------------------------------------------------------------------------\n");

    osal_seq_printf(s, "%8s" "%24s" "\n",
                    "devno", "lane_id");

    for (devno = 0; devno < MIPI_RX_MAX_DEV_NUM; devno++) {
        combo_dev_attr_t *pdev_attr = &g_mipi_dev_ctx.combo_dev_attr[devno];

        input_mode = pdev_attr->input_mode;

        if (!mipi_is_dev_cfged(devno)) {
            continue;
        }

        if (input_mode == INPUT_MODE_MIPI) {
            osal_seq_printf(s, "%8d" "%10d,%3d,%3d,%3d,%3d,%3d,%3d,%3d" "\n",
                            devno,
                            pdev_attr->mipi_attr.lane_id[0],
                            pdev_attr->mipi_attr.lane_id[1],
                            pdev_attr->mipi_attr.lane_id[MIPI_RX_LANE_ID_INDEX_2],
                            pdev_attr->mipi_attr.lane_id[MIPI_RX_LANE_ID_INDEX_3],
                            pdev_attr->mipi_attr.lane_id[MIPI_RX_LANE_ID_INDEX_4],
                            pdev_attr->mipi_attr.lane_id[MIPI_RX_LANE_ID_INDEX_5],
                            pdev_attr->mipi_attr.lane_id[MIPI_RX_LANE_ID_INDEX_6],
                            pdev_attr->mipi_attr.lane_id[MIPI_RX_LANE_ID_INDEX_7]);
        } else if (input_mode == INPUT_MODE_LVDS ||
                   input_mode == INPUT_MODE_SUBLVDS ||
                   input_mode == INPUT_MODE_HISPI) {
            osal_seq_printf(s, "%8d" "%10d,%3d,%3d,%3d,%3d,%3d,%3d,%3d" "\n",
                            devno,
                            pdev_attr->lvds_attr.lane_id[0],
                            pdev_attr->lvds_attr.lane_id[1],
                            pdev_attr->lvds_attr.lane_id[MIPI_RX_LANE_ID_INDEX_2],
                            pdev_attr->lvds_attr.lane_id[MIPI_RX_LANE_ID_INDEX_3],
                            pdev_attr->lvds_attr.lane_id[MIPI_RX_LANE_ID_INDEX_4],
                            pdev_attr->lvds_attr.lane_id[MIPI_RX_LANE_ID_INDEX_5],
                            pdev_attr->lvds_attr.lane_id[MIPI_RX_LANE_ID_INDEX_6],
                            pdev_attr->lvds_attr.lane_id[MIPI_RX_LANE_ID_INDEX_7]);
        }
    }
}

static void proc_show_mipi_phy_data(osal_proc_entry_t *s)
{
    int i;
    unsigned int freq;

    osal_seq_printf(s, "\n-----mipi phy data info------------------------------------------------------\n");
    osal_seq_printf(s, "%8s" "%15s" "%19s" "%24s" "%22s" "%16s" "\n",
                    "phy_id", "lane_id", "phy_data", "mipi_data", "lvds_data", "freq");

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        osal_seq_printf(s,
                        "%8d%8d,%2d,%2d,%2d    "
                        "0x%02x,0x%02x,0x%02x,0x%02x    "
                        "0x%02x,0x%02x,0x%02x,0x%02x    "
                        "0x%02x,0x%02x,0x%02x,0x%02x    ",
                        i,
                        4 * i, 4 * i + 1, 4 * i + 2, 4 * i + 3, /* Both PHY has 4 Lane, value: 0, 1, 2, 3 */
                        mipi_rx_drv_get_phy_data(i, 0),
                        mipi_rx_drv_get_phy_data(i, 1),
                        mipi_rx_drv_get_phy_data(i, 2),            /* Lane 2 in PHY_i */
                        mipi_rx_drv_get_phy_data(i, 3),            /* Lane 3 in PHY_i */
                        mipi_rx_drv_get_phy_mipi_link_data(i, 0),
                        mipi_rx_drv_get_phy_mipi_link_data(i, 1),
                        mipi_rx_drv_get_phy_mipi_link_data(i, 2),  /* Lane 2 in PHY_i */
                        mipi_rx_drv_get_phy_mipi_link_data(i, 3),  /* Lane 3 in PHY_i */
                        mipi_rx_drv_get_phy_lvds_link_data(i, 0),
                        mipi_rx_drv_get_phy_lvds_link_data(i, 1),
                        mipi_rx_drv_get_phy_lvds_link_data(i, 2),  /* Lane 2 in PHY_i */
                        mipi_rx_drv_get_phy_lvds_link_data(i, 3)); /* Lane 3 in PHY_i */
        freq = mipi_rx_drv_get_phy_freq(i);
        if (freq == 0) {
            osal_seq_printf(s, "%6s\n", "n/a");
        } else {
            osal_seq_printf(s, "%3dMHZ\n", freq);
        }
    }
}

static void proc_show_mipi_detect_info(osal_proc_entry_t *s, const combo_dev_t devno_array[], int mipi_cnt)
{
    img_size_t image_size;
    short vc_num;
    int devno_idx;

    osal_seq_printf(s, "\n-----mipi detect info----------------------------------------------------\n");
    osal_seq_printf(s, "%6s%3s%8s%8s\n", "devno", "vc", "width", "height");

    for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
        for (vc_num = 0; vc_num < WDR_VC_NUM; vc_num++) {
            mipi_rx_drv_get_mipi_imgsize_statis(devno_array[devno_idx], vc_num, &image_size);
            osal_seq_printf(s, "%6d%3d%8d%8d\n",
                            devno_array[devno_idx], vc_num, image_size.width, image_size.height);
        }
    }
}

static void proc_show_lvds_detect_info(osal_proc_entry_t *s, const combo_dev_t devno_array[], int mipi_cnt)
{
    img_size_t image_size;
    short vc_num;
    int devno_idx;

    osal_seq_printf(s, "\n-----lvds detect info----------------------------------------------------\n");
    osal_seq_printf(s, "%6s%3s%8s%8s\n", "devno", "vc", "width", "height");

    for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
        for (vc_num = 0; vc_num < WDR_VC_NUM; vc_num++) {
            mipi_rx_drv_get_lvds_imgsize_statis(devno_array[devno_idx], vc_num, &image_size);
            osal_seq_printf(s, "%6d%3d%8d%8d\n",
                            devno_array[devno_idx], vc_num, image_size.width, image_size.height);
        }
    }
}

static void proc_show_lvds_lane_detect_info(osal_proc_entry_t *s, const combo_dev_t devno_array[], int mipi_cnt)
{
    img_size_t image_size;
    short lane;
    int devno_idx;
    combo_dev_attr_t *pstcombo_dev_attr = NULL;

    osal_seq_printf(s, "\n-----lvds lane detect info----------------------------------------------------\n");
    osal_seq_printf(s, "%6s%6s%8s%8s\n", "devno", "lane", "width", "height");

    for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
        pstcombo_dev_attr = &g_mipi_dev_ctx.combo_dev_attr[devno_array[devno_idx]];

        for (lane = 0; lane < LVDS_LANE_NUM; lane++) {
            if (-1 != pstcombo_dev_attr->lvds_attr.lane_id[lane]) {
                mipi_rx_drv_get_lvds_lane_imgsize_statis(devno_array[devno_idx], lane, &image_size);
                osal_seq_printf(s, "%6d%6d%8d%8d\n",
                                devno_array[devno_idx],
                                pstcombo_dev_attr->lvds_attr.lane_id[lane],
                                image_size.width, image_size.height);
            }
        }
    }
}

static void proc_show_mipi_hs_mode(osal_proc_entry_t *s)
{
    lane_divide_mode_t lane_divide_mode;

    osal_spin_lock(&g_mipi_ctx_spinlock);
    lane_divide_mode = g_mipi_dev_ctx.lane_divide_mode;
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    osal_seq_printf(s,
                    "\n-----mipi lane divide mode-----------"
                    "----------------------------------------------------------------------------------\n");
    osal_seq_printf(s, "%6s"
                    "%20s"
                    "\n",
                    "mode", "lane divide");
    osal_seq_printf(s, "%6d"
                    "%20s"
                    "\n",
                    lane_divide_mode, mipi_print_lane_divide_mode(lane_divide_mode));
}

static void proc_show_phy_cil_int_err_cnt(osal_proc_entry_t *s)
{
    int phy_id;
    phy_err_int_cnt_t *phy_err_int_cnt = NULL;

    osal_seq_printf(s, "\n-----phy cil err int info---------------------------------------------\n");
    osal_seq_printf(s, "%8s%11s%10s%12s%12s%12s%12s%9s%8s%10s%10s%10s%10s\n",
                    "phy_id",
                    "clk2_tmout", "clk_tmout", "lane0_tmout", "lane1_tmout", "lane2_tmout", "lane3_tmout",
                    "clk2_esc", "clk_esc", "lane0_esc", "lane1_esc", " lane2_esc", "lane3_esc");

    for (phy_id = 0; phy_id < MIPI_RX_MAX_PHY_NUM; phy_id++) {
        phy_err_int_cnt = mipi_rx_drv_get_phy_err_int_cnt(phy_id);
        osal_seq_printf(s, "%8d%11d%10d%12d%12d%12d%12d%9d%8d%10d%10d%10d%10d\n",
                        phy_id,
                        phy_err_int_cnt->clk2_fsm_timeout_err_cnt,
                        phy_err_int_cnt->clk_fsm_timeout_err_cnt,
                        phy_err_int_cnt->d0_fsm_timeout_err_cnt,
                        phy_err_int_cnt->d1_fsm_timeout_err_cnt,
                        phy_err_int_cnt->d2_fsm_timeout_err_cnt,
                        phy_err_int_cnt->d3_fsm_timeout_err_cnt,

                        phy_err_int_cnt->clk2_fsm_escape_err_cnt,
                        phy_err_int_cnt->clk_fsm_escape_err_cnt,
                        phy_err_int_cnt->d0_fsm_escape_err_cnt,
                        phy_err_int_cnt->d1_fsm_escape_err_cnt,
                        phy_err_int_cnt->d2_fsm_escape_err_cnt,
                        phy_err_int_cnt->d3_fsm_escape_err_cnt);
    }
}

static void proc_show_mipi_err_info1(osal_proc_entry_t *s, const combo_dev_t devno_array[], int mipi_cnt)
{
    int devno_idx;
    combo_dev_t devno;
    mipi_err_int_cnt_t *mipi_err_int_cnt = NULL;

    osal_seq_printf(s, "\n-----mipi error int info1-----------------------------------------------------------\n");
    osal_seq_printf(s, "%8s%6s%9s%9s%9s%9s%16s%16s%16s%16s\n",
                    "devno",
                    "ecc2",
                    "vc0_crc", "vc1_crc", "vc2_crc", "vc3_crc",
                    "vc0_ecc_corrct", "vc1_ecc_corrct", "vc2_ecc_corrct", "vc3_ecc_corrct");

    for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
        devno = devno_array[devno_idx];
        mipi_err_int_cnt = mipi_rx_drv_get_mipi_err_int_cnt(devno);
        osal_seq_printf(s, "%8d%6d%9d%9d%9d%9d%16d%16d%16d%16d\n",
                        devno,
                        mipi_err_int_cnt->err_ecc_double_cnt,
                        mipi_err_int_cnt->vc0_err_crc_cnt,
                        mipi_err_int_cnt->vc1_err_crc_cnt,
                        mipi_err_int_cnt->vc2_err_crc_cnt,
                        mipi_err_int_cnt->vc3_err_crc_cnt,
                        mipi_err_int_cnt->vc0_err_ecc_corrected_cnt,
                        mipi_err_int_cnt->vc1_err_ecc_corrected_cnt,
                        mipi_err_int_cnt->vc2_err_ecc_corrected_cnt,
                        mipi_err_int_cnt->vc3_err_ecc_corrected_cnt);
    }
}

static void proc_show_mipi_err_info2(osal_proc_entry_t *s, const combo_dev_t devno_array[], int mipi_cnt)
{
    int devno_idx;
    combo_dev_t devno;
    mipi_err_int_cnt_t *mipi_err_int_cnt = NULL;

    osal_seq_printf(s, "\n-----mipi error int info2-----------------------------------------------------------\n");
    osal_seq_printf(s, "%8s%12s%12s%12s%12s%15s%15s%15s%15s\n",
                    "devno",
                    "vc0_frmnum", "vc1_frmnum", "vc2_frmnum", "vc3_frmnum",
                    "vc0_frmmmatch", "vc1_frmmmatch", "vc2_frmmmatch", "vc3_frmmmatch");

    for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
        devno = devno_array[devno_idx];
        mipi_err_int_cnt = mipi_rx_drv_get_mipi_err_int_cnt(devno);
        osal_seq_printf(s, "%8d%12d%12d%12d%12d%15d%15d%15d%15d\n",
                        devno,
                        mipi_err_int_cnt->vc0_err_frame_num_cnt,
                        mipi_err_int_cnt->vc1_err_frame_num_cnt,
                        mipi_err_int_cnt->vc2_err_frame_num_cnt,
                        mipi_err_int_cnt->vc3_err_frame_num_cnt,
                        mipi_err_int_cnt->vc0_err_frame_s_e_num_mismatch_cnt,
                        mipi_err_int_cnt->vc1_err_frame_s_e_num_mismatch_cnt,
                        mipi_err_int_cnt->vc2_err_frame_s_e_num_mismatch_cnt,
                        mipi_err_int_cnt->vc3_err_frame_s_e_num_mismatch_cnt);
    }
}

static void proc_show_mipi_err_info3(osal_proc_entry_t *s, const combo_dev_t devno_array[], int mipi_cnt)
{
    int devno_idx;
    combo_dev_t devno;
    mipi_err_int_cnt_t *mipi_err_int_cnt = NULL;

    osal_seq_printf(s, "\n-----mipi error int info3-----------------------------------------------------------\n");
    osal_seq_printf(s, "%8s%17s%16s%7s%16s%17s\n",
                    "devno",
                    "data_fifo_rderr", "cmd_fifo_rderr", "vsync", "cmd_fifo_wrerr", "data_fifo_wrerr");

    for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
        devno = devno_array[devno_idx];
        mipi_err_int_cnt = mipi_rx_drv_get_mipi_err_int_cnt(devno);
        osal_seq_printf(s, "%8d%17d%16d%7d%16d%17d\n",
                        devno,
                        mipi_err_int_cnt->data_fifo_rderr_cnt,
                        mipi_err_int_cnt->cmd_fifo_rderr_cnt,
                        mipi_err_int_cnt->vsync_cnt,
                        mipi_err_int_cnt->data_fifo_wrerr_cnt,
                        mipi_err_int_cnt->cmd_fifo_wrerr_cnt);
    }
}

static void proc_show_mipi_int_err(osal_proc_entry_t *s, const combo_dev_t devno_array[], int mipi_cnt)
{
    proc_show_mipi_err_info1(s, devno_array, mipi_cnt);
    proc_show_mipi_err_info2(s, devno_array, mipi_cnt);
    proc_show_mipi_err_info3(s, devno_array, mipi_cnt);
}

static void proc_show_lvds_int_err(osal_proc_entry_t *s, const combo_dev_t devno_array[], int lvds_cnt)
{
    combo_dev_t devno;
    int devno_idx;
    lvds_err_int_cnt_t *lvds_err_int_cnt = NULL;

    osal_seq_printf(s, "\n-----lvds error int info1-----------------------------------------------------------\n");
    osal_seq_printf(s, "%8s%7s%11s%11s%9s%10s\n", "devno", "vsync", "cmd_rderr", "cmd_wrerr", "pop_err", "stat_err");

    for (devno_idx = 0; devno_idx < lvds_cnt; devno_idx++) {
        devno = devno_array[devno_idx];
        lvds_err_int_cnt = mipi_rx_drv_get_lvds_err_int_cnt(devno);
        osal_seq_printf(s, "%8d%7d%11d%11d%9d%10d\n", devno,
                        lvds_err_int_cnt->lvds_vsync_cnt,
                        lvds_err_int_cnt->cmd_rd_err_cnt,
                        lvds_err_int_cnt->cmd_wr_err_cnt,
                        lvds_err_int_cnt->pop_err_cnt,
                        lvds_err_int_cnt->lvds_state_err_cnt);
    }

    osal_seq_printf(s, "\n-----lvds error int info2-----------------------------------------------------------\n");
    osal_seq_printf(s, "%8s%13s%13s%13s%13s%13s%13s\n",
                    "devno", "link0_wrerr", "link1_wrerr", "link2_wrerr", "link0_rderr", "link1_rderr", "link2_rderr");

    for (devno_idx = 0; devno_idx < lvds_cnt; devno_idx++) {
        devno = devno_array[devno_idx];
        lvds_err_int_cnt = mipi_rx_drv_get_lvds_err_int_cnt(devno);
        osal_seq_printf(s, "%8d%13d%13d%13d%13d%13d%13d\n", devno,
                        lvds_err_int_cnt->link0_wr_err_cnt,
                        lvds_err_int_cnt->link1_wr_err_cnt,
                        lvds_err_int_cnt->link2_wr_err_cnt,
                        lvds_err_int_cnt->link0_rd_err_cnt,
                        lvds_err_int_cnt->link1_rd_err_cnt,
                        lvds_err_int_cnt->link2_rd_err_cnt);
    }

    osal_seq_printf(s, "\n-----lvds error int info3-----------------------------------------------------------\n");
    osal_seq_printf(s, "%8s%11s%11s%11s%11s%11s%11s%11s%11s\n",
                    "devno", "lane0_err", "lane1_err", "lane2_err", "lane3_err",
                    "lane4_err", "lane5_err", "lane6_err", "lane7_err");

    for (devno_idx = 0; devno_idx < lvds_cnt; devno_idx++) {
        devno = devno_array[devno_idx];
        lvds_err_int_cnt = mipi_rx_drv_get_lvds_err_int_cnt(devno);
        osal_seq_printf(s, "%8d%11d%11d%11d%11d%11d%11d%11d%11d\n", devno,
                        lvds_err_int_cnt->lane0_sync_err_cnt,
                        lvds_err_int_cnt->lane1_sync_err_cnt,
                        lvds_err_int_cnt->lane2_sync_err_cnt,
                        lvds_err_int_cnt->lane3_sync_err_cnt,
                        lvds_err_int_cnt->lane4_sync_err_cnt,
                        lvds_err_int_cnt->lane5_sync_err_cnt,
                        lvds_err_int_cnt->lane6_sync_err_cnt,
                        lvds_err_int_cnt->lane7_sync_err_cnt);
    }
}

static void proc_show_align_int_err(osal_proc_entry_t *s)
{
    combo_dev_t devno;
    align_err_int_cnt_t *align_err_int_cnt = NULL;

    osal_seq_printf(s, "\n-----align error int info--------------------------------------\n");

    for (devno = 0; devno < MIPI_RX_MAX_DEV_NUM; devno++) {
        if (!mipi_is_dev_cfged(devno)) {
            continue;
        }
        align_err_int_cnt = mipi_rx_drv_get_align_err_int_cnt(devno);
        osal_seq_printf(s, "%8s%14s%11s%11s%11s%11s%11s%11s%11s%11s\n",
                        "devno", "fifo_fullerr",
                        "lane0_err", "lane1_err", "lane2_err", "lane3_err", "lane4_err", "lane5_err",
                        "lane6_err", "lane7_err");

        osal_seq_printf(s, "%8d%14d%11d%11d%11d%11d%11d%11d%11d%11d\n",
                        devno,
                        align_err_int_cnt->fifo_full_err_cnt,
                        align_err_int_cnt->lane0_align_err_cnt,
                        align_err_int_cnt->lane1_align_err_cnt,
                        align_err_int_cnt->lane2_align_err_cnt,
                        align_err_int_cnt->lane3_align_err_cnt,
                        align_err_int_cnt->lane4_align_err_cnt,
                        align_err_int_cnt->lane5_align_err_cnt,
                        align_err_int_cnt->lane6_align_err_cnt,
                        align_err_int_cnt->lane7_align_err_cnt);
    }
}

static int mipi_proc_show(osal_proc_entry_t *s, combo_dev_t devno_mipi[], int mipi_cnt,
                          combo_dev_t devno_lvds[], int lvds_cnt)
{
    if (mipi_cnt > 0 || lvds_cnt > 0) {
        proc_show_mipi_hs_mode(s);
        proc_show_mipi_device(s);
        proc_show_mipi_lane(s);
        proc_show_mipi_phy_data(s);
    }

    if (mipi_cnt > 0) {
        proc_show_mipi_detect_info(s, devno_mipi, mipi_cnt);
    }

    if (lvds_cnt > 0) {
        proc_show_lvds_detect_info(s, devno_lvds, lvds_cnt);
        proc_show_lvds_lane_detect_info(s, devno_lvds, lvds_cnt);
    }

    if (mipi_cnt > 0 || lvds_cnt > 0) {
        proc_show_phy_cil_int_err_cnt(s);
    }

    if (mipi_cnt > 0) {
        proc_show_mipi_int_err(s, devno_mipi, mipi_cnt);
    }

    if (lvds_cnt > 0) {
        proc_show_lvds_int_err(s, devno_lvds, lvds_cnt);
    }

    if (mipi_cnt > 0 || lvds_cnt > 0) {
        proc_show_align_int_err(s);
    }

    return 0;
}

static int mipi_rx_proc_show(osal_proc_entry_t *s)
{
    combo_dev_t devno;
    int mipi_cnt = 0;
    int lvds_cnt = 0;
    input_mode_t input_mode;
    combo_dev_attr_t *pdev_attr = NULL;
    combo_dev_t devno_mipi[MIPI_RX_MAX_DEV_NUM] = {0};
    combo_dev_t devno_lvds[MIPI_RX_MAX_DEV_NUM] = {0};

    for (devno = 0; devno < MIPI_RX_MAX_DEV_NUM; devno++) {
        if (!mipi_is_dev_cfged(devno)) {
            continue;
        }

        osal_spin_lock(&g_mipi_ctx_spinlock);
        pdev_attr = &g_mipi_dev_ctx.combo_dev_attr[devno];
        input_mode = pdev_attr->input_mode;
        osal_spin_unlock(&g_mipi_ctx_spinlock);

        if (input_mode == INPUT_MODE_MIPI) {
            devno_mipi[mipi_cnt] = devno;
            mipi_cnt++;
        } else if ((input_mode == INPUT_MODE_LVDS) || (input_mode == INPUT_MODE_SUBLVDS)
                   || (input_mode == INPUT_MODE_HISPI)) {
            devno_lvds[lvds_cnt] = devno;
            lvds_cnt++;
        }
    }

    osal_seq_printf(s, "\n[MIPI_RX] Version: [" OT_MPP_VERSION "], Build Time["__DATE__", "__TIME__"]\n\n");
    mipi_proc_show(s, devno_mipi, mipi_cnt, devno_lvds, lvds_cnt);

    return 0;
}
#endif

static int mipi_rx_init(void)
{
    int ret;

    ret = osal_mutex_init(&g_mipi_mutex);
    if (ret < 0) {
        ot_err("mutex init fail!\n");
        goto fail0;
    }

    ret = osal_spin_lock_init(&g_mipi_ctx_spinlock);
    if (ret < 0) {
        ot_err("spin lock init fail!\n");
        goto fail1;
    }

    osal_spin_lock(&g_mipi_ctx_spinlock);
    g_mipi_dev_ctx.lane_divide_mode = LANE_DIVIDE_MODE_BUTT;
    osal_spin_unlock(&g_mipi_ctx_spinlock);

    ret = mipi_rx_drv_init();
    if (ret < 0) {
        ot_err("mipi_rx_drv_init fail!\n");
        goto fail2;
    }

    return 0;

fail2:
    osal_spin_lock_destroy(&g_mipi_ctx_spinlock);
fail1:
    osal_mutex_destroy(&g_mipi_mutex);
fail0:
    return -1;
}

static void mipi_rx_exit(void)
{
    mipi_rx_drv_exit();

    osal_spin_lock_destroy(&g_mipi_ctx_spinlock);
    osal_mutex_destroy(&g_mipi_mutex);
}

static int mipi_rx_open(void *private_data)
{
    mipi_rx_unused(private_data);
    return 0;
}

static int mipi_rx_release(void *private_data)
{
    mipi_rx_unused(private_data);
    return 0;
}

static osal_fileops_t mipi_rx_fops = {
    .open = mipi_rx_open,
    .release = mipi_rx_release,
    .unlocked_ioctl = mipi_rx_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = mipi_rx_compat_ioctl,
#endif
};

int mipi_rx_mod_init(void)
{
    int ret;
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_proc_entry_t *mipi_rx_proc_entry = NULL;
#endif

    g_mipi_rx_dev = osal_createdev(MIPI_RX_DEV_NAME);
    if (g_mipi_rx_dev == NULL) {
        ot_err("create mipi_rx device failed!\n");
        goto fail0;
    }

    g_mipi_rx_dev->fops = &mipi_rx_fops;
    g_mipi_rx_dev->minor = OT_MEDIA_DYNAMIC_MINOR;

    ret = osal_registerdevice(g_mipi_rx_dev);
    if (ret < 0) {
        ot_err("register mipi_rx device failed!\n");
        goto fail1;
    }

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    mipi_rx_proc_entry = osal_create_proc_entry(MIPI_RX_PROC_NAME, NULL);
    if (mipi_rx_proc_entry == NULL) {
        ot_err("create mipi_rx proc(%s) failed!\n", MIPI_RX_PROC_NAME);
        goto fail2;
    }
    mipi_rx_proc_entry->read = mipi_rx_proc_show;
    mipi_rx_proc_entry->write = NULL;
#endif

    ret = mipi_rx_init();
    if (ret < 0) {
        ot_err("mipi_rx_init failed!\n");
        goto fail3;
    }

    osal_printk("load mipi_rx.ko ....OK!\n");
    return 0;

fail3:
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_remove_proc_entry(MIPI_RX_PROC_NAME, NULL);
fail2:
#endif
    osal_deregisterdevice(g_mipi_rx_dev);
fail1:
    osal_destroydev(g_mipi_rx_dev);
    g_mipi_rx_dev = NULL;
fail0:
    ot_err("load mipi_rx.ko ....failed!\n");
    return -1;
}

void mipi_rx_mod_exit(void)
{
    mipi_rx_exit();

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_remove_proc_entry(MIPI_RX_PROC_NAME, NULL);
#endif

    osal_deregisterdevice(g_mipi_rx_dev);

    osal_destroydev(g_mipi_rx_dev);
    g_mipi_rx_dev = NULL;

    osal_printk("unload mipi_rx.ko ....OK!\n");
}
