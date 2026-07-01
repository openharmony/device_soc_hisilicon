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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART04_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART04_H_

    ctrl_reg_attr->lvds_crop_start3.u32 = crop_start3.u32;
}

void mipi_rx_drv_set_lvds_crop_en(combo_dev_t devno, int enable)
{
    volatile lvds_ctrl_regs_t *ctrl_reg_attr;
    u_lvds_ctrl lvds_ctrl;

    ctrl_reg_attr = get_lvds_ctrl_regs(devno);
    if (ctrl_reg_attr == NULL) {
        return;
    }

    lvds_ctrl.u32 = ctrl_reg_attr->lvds_ctrl.u32;

    lvds_ctrl.bits.lvds_crop_en = enable;

    ctrl_reg_attr->lvds_ctrl.u32 = lvds_ctrl.u32;
}

static int mipi_rx_drv_set_lvds_wdr_normal(const lvds_vsync_attr_t *vsync_attr, u_lvds_wdr *lvds_wdr)
{
    if (vsync_attr->sync_type == LVDS_VSYNC_NORMAL) {
        /* SOF-EOF WDR, long exposure frame and short exposure frame has independent sync code */
        lvds_wdr->bits.lvds_wdr_mode = 0x0;
    } else if (vsync_attr->sync_type == LVDS_VSYNC_SHARE) {
        /* SOF-EOF WDR, long exposure frame and short exposure frame share the SOF and EOF */
        lvds_wdr->bits.lvds_wdr_mode = 0x2;
    } else {
        ot_err("not support vsync type: %d\n", vsync_attr->sync_type);
        return -1;
    }
    return 0;
}

static int mipi_rx_drv_set_lvds_wdr_dol(const lvds_vsync_attr_t *vsync_attr, const lvds_fid_attr_t *fid_attr,
    u_lvds_wdr *lvds_wdr, u_lvds_dolscd_hblk *scd_hblk)
{
    /* Sony DOL WDR */
    if (vsync_attr->sync_type == LVDS_VSYNC_NORMAL) {
        /*
         * SAV-EAV WDR, 4 sync code, fid embedded in 4th sync code
         * long exposure fame and short exposure frame has independent sync code
         */
        if (fid_attr->fid_type == LVDS_FID_IN_SAV) {
            lvds_wdr->bits.lvds_wdr_mode = 0x4;
        } else if (fid_attr->fid_type == LVDS_FID_IN_DATA) {
            /*
             * SAV-EAV WDR, 5 sync code(Line Information), fid in the fist DATA,
             * fid in data, line information
             */
            if (fid_attr->output_fil) {
                /* Frame Information Line is included in the image data */
                lvds_wdr->bits.lvds_wdr_mode = 0xd;
            } else {
                /* Frame Information Line is not included in the image data */
                lvds_wdr->bits.lvds_wdr_mode = 0x6;
            }
        } else {
            ot_err("not support fid type: %d\n", fid_attr->fid_type);
            return -1;
        }
    } else if (vsync_attr->sync_type == LVDS_VSYNC_HCONNECT) {
        /*
         * SAV-EAV H-Connection DOL, long exposure frame and short exposure frame
         * share the same SAV EAV, the H-Blank is assigned by the dol_hblank1 and dol_hblank2
         */
        if (fid_attr->fid_type == LVDS_FID_NONE) {
            lvds_wdr->bits.lvds_wdr_mode = 0x5;
        } else {
            ot_err("not support fid type: %d\n", fid_attr->fid_type);
            return -1;
        }
        scd_hblk->bits.dol_hblank1 = vsync_attr->hblank1;
        scd_hblk->bits.dol_hblank2 = vsync_attr->hblank2;
    } else {
        ot_err("not support vsync type: %d\n", vsync_attr->sync_type);
        return -1;
    }
    return 0;
}

/* magic num mean wdr mode, convert to register config */
static int mipi_rx_drv_set_lvds_wdr_num(lvds_wdr_mode_t wdr_mode, u_lvds_wdr *lvds_wdr)
{
    /* set the wdr frame number */
    switch (wdr_mode) {
        case OT_LVDS_WDR_MODE_NONE:
            lvds_wdr->bits.lvds_wdr_num = 0;
            break;
        case OT_LVDS_WDR_MODE_2F:
        case OT_LVDS_WDR_MODE_DOL_2F:
            lvds_wdr->bits.lvds_wdr_num = 1;
            break;

        case OT_LVDS_WDR_MODE_3F:
        case OT_LVDS_WDR_MODE_DOL_3F:
            lvds_wdr->bits.lvds_wdr_num = MIPI_RX_HAL_VALUE_2;
            break;

        case OT_LVDS_WDR_MODE_4F:
        case OT_LVDS_WDR_MODE_DOL_4F:
            lvds_wdr->bits.lvds_wdr_num = MIPI_RX_HAL_VALUE_3;
            break;

        default:
            ot_err("not support WDR_MODE: %d\n", wdr_mode);
            return -1;
    }
    return 0;
}

int mipi_rx_drv_set_lvds_wdr_mode(combo_dev_t devno, lvds_wdr_mode_t wdr_mode,
                                  const lvds_vsync_attr_t *vsync_attr, const lvds_fid_attr_t *fid_attr)
{
    int ret;
    volatile lvds_ctrl_regs_t *ctrl_reg_attr;
    u_lvds_wdr lvds_wdr;
    u_lvds_dolscd_hblk scd_hblk;

    ctrl_reg_attr = get_lvds_ctrl_regs(devno);

    lvds_wdr.u32 = ctrl_reg_attr->lvds_wdr.u32;
    scd_hblk.u32 = ctrl_reg_attr->lvds_dolscd_hblk.u32;

    if (wdr_mode == OT_LVDS_WDR_MODE_NONE) {
        lvds_wdr.bits.lvds_wdr_en = 0;
    } else {
        lvds_wdr.bits.lvds_wdr_en = 1;
    }

    ret = mipi_rx_drv_set_lvds_wdr_num(wdr_mode, &lvds_wdr);
    if (ret < 0) {
        return ret;
    }

    /* set wdr mode */
    if (OT_LVDS_WDR_MODE_2F <= wdr_mode && wdr_mode <= OT_LVDS_WDR_MODE_4F) {
        ret = mipi_rx_drv_set_lvds_wdr_normal(vsync_attr, &lvds_wdr);
        if (ret < 0) {
            return ret;
        }
    } else if (OT_LVDS_WDR_MODE_DOL_2F <= wdr_mode && wdr_mode <= OT_LVDS_WDR_MODE_DOL_4F) {
        ret = mipi_rx_drv_set_lvds_wdr_dol(vsync_attr, fid_attr, &lvds_wdr, &scd_hblk);
        if (ret < 0) {
            return ret;
        }
    }

    ctrl_reg_attr->lvds_wdr.u32 = lvds_wdr.u32;
    ctrl_reg_attr->lvds_dolscd_hblk.u32 = scd_hblk.u32;

    return 0;
}

void mipi_rx_drv_set_lvds_ctrl_mode(combo_dev_t devno, lvds_sync_mode_t sync_mode,
                                    data_type_t input_data_type,
                                    lvds_bit_endian_t data_endian,
                                    lvds_bit_endian_t sync_code_endian)
{
    volatile lvds_ctrl_regs_t *ctrl_reg_attr;
    u_lvds_ctrl lvds_ctrl;
    unsigned short raw_type;

    ctrl_reg_attr = get_lvds_ctrl_regs(devno);

    lvds_ctrl.u32 = ctrl_reg_attr->lvds_ctrl.u32;

    switch (input_data_type) {
        case DATA_TYPE_RAW_8BIT:
            raw_type = 0x1;
            break;

        case DATA_TYPE_RAW_10BIT:
            raw_type = 0x2;
            break;

        case DATA_TYPE_RAW_12BIT:
            raw_type = 0x3;
            break;

        case DATA_TYPE_RAW_14BIT:
            raw_type = 0x4;
            break;

        case DATA_TYPE_RAW_16BIT:
            raw_type = 0x5;
            break;

        default:
            return;
    }

    lvds_ctrl.bits.lvds_sync_mode = sync_mode;
    lvds_ctrl.bits.lvds_raw_type = raw_type;
    lvds_ctrl.bits.lvds_pix_big_endian = data_endian;
    lvds_ctrl.bits.lvds_code_big_endian = sync_code_endian;

    ctrl_reg_attr->lvds_ctrl.u32 = lvds_ctrl.u32;
}

void mipi_rx_drv_set_lvds_data_rate(combo_dev_t devno, mipi_data_rate_t data_rate)
{
    u_lvds_output_pix_num lvds_output_pixel_num;
    unsigned int lvds_double_pix_en = 0;
    volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

    if (data_rate == MIPI_DATA_RATE_X1) {
        lvds_double_pix_en = 0;
    } else if (data_rate == MIPI_DATA_RATE_X2) {
        lvds_double_pix_en = 0x1;
    } else {
        ot_err("unsupported  data_rate:%d  devno %d\n", data_rate, devno);
        return;
    }

    lvds_output_pixel_num.u32 = lvds_ctrl_regs->lvds_output_pix_num.u32;
    lvds_output_pixel_num.bits.lvds_double_pix_en = lvds_double_pix_en;
    lvds_ctrl_regs->lvds_output_pix_num.u32 = lvds_output_pixel_num.u32;
}

void mipi_rx_drv_set_dol_line_information(combo_dev_t devno, lvds_wdr_mode_t wdr_mode)
{
    volatile lvds_ctrl_regs_t *ctrl_reg_attr;

    ctrl_reg_attr = get_lvds_ctrl_regs(devno);

    if (wdr_mode >= OT_LVDS_WDR_MODE_DOL_2F) {
        ctrl_reg_attr->lvds_li_word0.bits.li_word0_0 = 0x0201;  /* LEF n frame */
        ctrl_reg_attr->lvds_li_word0.bits.li_word0_1 = 0x0211;  /* LEF n + 1 frame */

        ctrl_reg_attr->lvds_li_word1.bits.li_word1_0 = 0x0202;  /* SEF1 n frame */
        ctrl_reg_attr->lvds_li_word1.bits.li_word1_1 = 0x0212;  /* SEF1 n + 1 frame */
    }

    if (wdr_mode >= OT_LVDS_WDR_MODE_DOL_3F) {
        ctrl_reg_attr->lvds_li_word2.bits.li_word2_0 = 0x0204;  /* SEF2 n frame */
        ctrl_reg_attr->lvds_li_word2.bits.li_word2_1 = 0x0214;  /* SEF2 n + 1 frame */
    }

    if (wdr_mode >= OT_LVDS_WDR_MODE_DOL_4F) {
        ctrl_reg_attr->lvds_li_word3.bits.li_word3_0 = 0x0208;  /* SEF3 n frame */
        ctrl_reg_attr->lvds_li_word3.bits.li_word3_1 = 0x0218;  /* SEF3 n + 1 frame */
    }
}

/* magic nums means vc num and sync num */
static void set_lvds_sync_sof_eof(volatile lvds_sync_code_cfg_t *p_sync_code,
    const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM], int land_id)
{
    {
        u_lvds_lane_sof_01 lvds_sof_01;
        lvds_sof_01.u32 = p_sync_code->lvds_lane_sof_01.u32;
        lvds_sof_01.bits.lane_sof_0 = sync_code[land_id][0][0];
        lvds_sof_01.bits.lane_sof_1 = sync_code[land_id][1][0];
        p_sync_code->lvds_lane_sof_01.u32 = lvds_sof_01.u32;
    }
    {
        u_lvds_lane_sof_23 lvds_sof_23;
        lvds_sof_23.u32 = p_sync_code->lvds_lane_sof_23.u32;
        lvds_sof_23.bits.lane_sof_2 = sync_code[land_id][MIPI_RX_HAL_SYNC_CODE_INDEX_2][0];
        lvds_sof_23.bits.lane_sof_3 = sync_code[land_id][MIPI_RX_HAL_SYNC_CODE_INDEX_3][0];
        p_sync_code->lvds_lane_sof_23.u32 = lvds_sof_23.u32;
    }
    {
        u_lvds_lane_eof_01 lvds_eof_01;
        lvds_eof_01.u32 = p_sync_code->lvds_lane_eof_01.u32;
        lvds_eof_01.bits.lane_eof_0 = sync_code[land_id][0][1];
        lvds_eof_01.bits.lane_eof_1 = sync_code[land_id][1][1];
        p_sync_code->lvds_lane_eof_01.u32 = lvds_eof_01.u32;
    }
    {
        u_lvds_lane_eof_23 lvds_eof_23;
        lvds_eof_23.u32 = p_sync_code->lvds_lane_eof_23.u32;
        lvds_eof_23.bits.lane_eof_2 = sync_code[land_id][MIPI_RX_HAL_SYNC_CODE_INDEX_2][1];
        lvds_eof_23.bits.lane_eof_3 = sync_code[land_id][MIPI_RX_HAL_SYNC_CODE_INDEX_3][1];
        p_sync_code->lvds_lane_eof_23.u32 = lvds_eof_23.u32;
    }
}

/* magic nums means vc num and sync num */
static void set_lvds_sync_sol_eol(volatile lvds_sync_code_cfg_t *p_sync_code,
    const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM], int land_id)
{
    {
        u_lvds_lane_sol_01 lvds_sol_01;
        lvds_sol_01.u32 = p_sync_code->lvds_lane_sol_01.u32;
        lvds_sol_01.bits.lane_sol_0 = sync_code[land_id][0][MIPI_RX_HAL_SYNC_CODE_INDEX_2];
        lvds_sol_01.bits.lane_sol_1 = sync_code[land_id][1][MIPI_RX_HAL_SYNC_CODE_INDEX_2];
        p_sync_code->lvds_lane_sol_01.u32 = lvds_sol_01.u32;
    }
    {
        u_lvds_lane_sol_23 lvds_sol_23;
        lvds_sol_23.u32 = p_sync_code->lvds_lane_sol_23.u32;
        lvds_sol_23.bits.lane_sol_2 = sync_code[land_id][MIPI_RX_HAL_SYNC_CODE_INDEX_2][MIPI_RX_HAL_SYNC_CODE_INDEX_2];
        lvds_sol_23.bits.lane_sol_3 =
            sync_code[land_id][MIPI_RX_HAL_SYNC_CODE_INDEX_3][MIPI_RX_HAL_SYNC_CODE_INDEX_2];
        p_sync_code->lvds_lane_sol_23.u32 = lvds_sol_23.u32;
    }
    {
        u_lvds_lane_eol_01 lvds_eol_01;
        lvds_eol_01.u32 = p_sync_code->lvds_lane_eol_01.u32;
        lvds_eol_01.bits.lane_eol_0 = sync_code[land_id][0][MIPI_RX_HAL_SYNC_CODE_INDEX_3];
        lvds_eol_01.bits.lane_eol_1 = sync_code[land_id][1][MIPI_RX_HAL_SYNC_CODE_INDEX_3];
        p_sync_code->lvds_lane_eol_01.u32 = lvds_eol_01.u32;
    }
    {
        u_lvds_lane_eol_23 lvds_eol_23;
        lvds_eol_23.u32 = p_sync_code->lvds_lane_eol_23.u32;
        lvds_eol_23.bits.lane_eol_2 =
            sync_code[land_id][MIPI_RX_HAL_SYNC_CODE_INDEX_2][MIPI_RX_HAL_SYNC_CODE_INDEX_3];
        lvds_eol_23.bits.lane_eol_3 = sync_code[land_id][MIPI_RX_HAL_SYNC_CODE_INDEX_3][MIPI_RX_HAL_SYNC_CODE_INDEX_3];
        p_sync_code->lvds_lane_eol_23.u32 = lvds_eol_23.u32;
    }
}

static void set_lvds_sync_code(combo_dev_t devno, int n_frame, unsigned int lane_cnt,
    const short lane_id[LVDS_LANE_NUM], const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    unsigned int i;
    short lane_idx;
    volatile lvds_sync_code_cfg_t *p_sync_code = NULL;
    volatile lvds_ctrl_regs_t *ctrl_reg_attr = get_lvds_ctrl_regs(devno);

    for (i = 0; i < lane_cnt; i++) {
        lane_idx = lane_id[i];
        if (lane_idx != -1) {
            if (n_frame == TRUE) {
                p_sync_code = &ctrl_reg_attr->lvds_this_frame_sync_code[i];
            } else {
                p_sync_code = &ctrl_reg_attr->lvds_next_frame_sync_code[i];
            }

            set_lvds_sync_sof_eof(p_sync_code, sync_code, i);
            set_lvds_sync_sol_eol(p_sync_code, sync_code, i);
        }
    }
}

void mipi_rx_drv_set_lvds_sync_code(combo_dev_t devno, unsigned int lane_cnt, const short lane_id[LVDS_LANE_NUM],
                                    const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    set_lvds_sync_code(devno, TRUE, lane_cnt, lane_id, sync_code);
}

void mipi_rx_drv_set_lvds_nxt_sync_code(combo_dev_t devno, unsigned int lane_cnt, const short lane_id[LVDS_LANE_NUM],
                                        const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    set_lvds_sync_code(devno, FALSE, lane_cnt, lane_id, sync_code);
}

static void mipi_rx_drv_set_phy_sync_dct(int phy_id, int raw_type,
                                         lvds_bit_endian_t code_endian, unsigned int phy_lane_bitmap)
{
    u_phy_sync_dct_link phy_sync_dct_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_sync_dct_link.u32 = mipi_rx_phy_cfg->phy_sync_dct_link.u32;

    if (phy_lane_bitmap & 0x5) {
        phy_sync_dct_link.bits.cil_raw_type0 = raw_type;
        phy_sync_dct_link.bits.cil_code_big_endian0 = code_endian;
    }

    if (phy_lane_bitmap & 0xa) {
        phy_sync_dct_link.bits.cil_raw_type1 = raw_type;
        phy_sync_dct_link.bits.cil_code_big_endian1 = code_endian;
    }

    mipi_rx_phy_cfg->phy_sync_dct_link.u32 = phy_sync_dct_link.u32;
}

static short get_sensor_lane_index(short lane, const short lane_id[LVDS_LANE_NUM])
{
    int i;

    for (i = 0; i < LVDS_LANE_NUM; i++) {
        if (lane_id[i] == lane) {
            break;
        }
    }

    return i;
}

static void mipi_rx_drv_set_lvds_phy_sync_code(int phy_id,
                                               const short lane_id[LVDS_LANE_NUM],
                                               const unsigned short n_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM],
                                               const unsigned short nxt_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM],
                                               unsigned int phy_lane_bitmap)
{
    u_phy_sync_sof0_link phy_sync_sof0_link;
    u_phy_sync_sof1_link phy_sync_sof1_link;
    u_phy_sync_sof2_link phy_sync_sof2_link;
    u_phy_sync_sof3_link phy_sync_sof3_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
    short sensor_lane_idx;
    short lane;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_sync_sof0_link.u32 = mipi_rx_phy_cfg->phy_sync_sof0_link.u32;
    phy_sync_sof1_link.u32 = mipi_rx_phy_cfg->phy_sync_sof1_link.u32;
    phy_sync_sof2_link.u32 = mipi_rx_phy_cfg->phy_sync_sof2_link.u32;
    phy_sync_sof3_link.u32 = mipi_rx_phy_cfg->phy_sync_sof3_link.u32;

    if (phy_lane_bitmap & 0x1) {
        lane = 0 + MIPI_RX_PHY_LANE_NUM * phy_id;
        sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
        phy_sync_sof0_link.bits.cil_sof0_word4_0 = n_sync_code[sensor_lane_idx][0][0];
        phy_sync_sof0_link.bits.cil_sof1_word4_0 = nxt_sync_code[sensor_lane_idx][0][0];
    }

    if (phy_lane_bitmap & 0x2) {
        lane = 1 + MIPI_RX_PHY_LANE_NUM * phy_id;
        sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
        phy_sync_sof1_link.bits.cil_sof0_word4_1 = n_sync_code[sensor_lane_idx][0][0];
        phy_sync_sof1_link.bits.cil_sof1_word4_1 = nxt_sync_code[sensor_lane_idx][0][0];
    }

    if (phy_lane_bitmap & 0x4) {
        lane = 2 + MIPI_RX_PHY_LANE_NUM * phy_id; /* 2nd lane */
        sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
        phy_sync_sof2_link.bits.cil_sof0_word4_2 = n_sync_code[sensor_lane_idx][0][0];
        phy_sync_sof2_link.bits.cil_sof1_word4_2 = nxt_sync_code[sensor_lane_idx][0][0];
    }

    if (phy_lane_bitmap & 0x8) {
        lane = 3 + MIPI_RX_PHY_LANE_NUM * phy_id; /* 3rd lane */
        sensor_lane_idx = get_sensor_lane_index(lane, lane_id);


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART04_H_ */
