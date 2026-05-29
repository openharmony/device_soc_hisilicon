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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART02_H_

    mipi_ctrl_regs->mipi_crop_start_chn3.u32 = crop_start_chn3.u32;
    mipi_ctrl_regs->mipi_imgsize.u32 = mipi_imgsize.u32;
}

void mipi_rx_drv_set_mipi_crop_en(combo_dev_t devno, int enable)
{
    u_mipi_ctrl_mode_pixel mipi_ctrl_mode_pixel;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_ctrl_mode_pixel.u32 = mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32;
    mipi_ctrl_mode_pixel.bits.crop_en = enable;
    mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32 = mipi_ctrl_mode_pixel.u32;
}

static short mipi_rx_drv_get_data_type(data_type_t input_data_type)
{
    if (input_data_type == DATA_TYPE_RAW_8BIT) {
        return 0x2a;
    } else if (input_data_type == DATA_TYPE_RAW_10BIT) {
        return 0x2b;
    } else if (input_data_type == DATA_TYPE_RAW_12BIT) {
        return 0x2c;
    } else if (input_data_type == DATA_TYPE_RAW_14BIT) {
        return 0x2d;
    } else if (input_data_type == DATA_TYPE_RAW_16BIT) {
        return 0x2e;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
        return 0x18;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
        return 0x1a;
    } else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
        return 0x1e;
    } else if (input_data_type == DATA_TYPE_YUV422_PACKED) {
        return 0x1e;
    } else {
        return 0x0;
    }
}

static short mipi_rx_drv_get_data_bit_width(data_type_t input_data_type)
{
    if (input_data_type == DATA_TYPE_RAW_8BIT) {
        return 0x0;
    } else if (input_data_type == DATA_TYPE_RAW_10BIT) {
        return 0x1;
    } else if (input_data_type == DATA_TYPE_RAW_12BIT) {
        return 0x2;
    } else if (input_data_type == DATA_TYPE_RAW_14BIT) {
        return 0x3;
    } else if (input_data_type == DATA_TYPE_RAW_16BIT) {
        return 0x4;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
        return 0x0;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
        return 0x0;
    } else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
        return 0x0;
    } else if (input_data_type == DATA_TYPE_YUV422_PACKED) {
        return 0x4;
    } else {
        return 0x0;
    }
}

/* magic num mean bit width, convert to register condfig */
static short mipi_rx_drv_get_ext_data_bit_width(unsigned int ext_data_bit_width)
{
    if (ext_data_bit_width == 8) { /* 8 bits width */
        return 0x0;
    } else if (ext_data_bit_width == 10) { /* 10 bits width */
        return 0x1;
    } else if (ext_data_bit_width == 12) { /* 12 bits width */
        return 0x2;
    } else if (ext_data_bit_width == 14) { /* 14 bits width */
        return 0x3;
    } else if (ext_data_bit_width == 16) { /* 16 bits width */
        return 0x4;
    } else {
        return 0x0;
    }
}

/* magic num mean data type index, convert to register config */
static void mipi_rx_drv_set_mipi_user_dt(combo_dev_t devno, int index, short data_type, short bit_width)
{
    u_mipi_userdef_dt user_def_dt;
    u_mipi_user_def user_def;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    if (mipi_ctrl_regs != NULL) {
        user_def_dt.u32 = mipi_ctrl_regs->mipi_userdef_dt.u32;
        user_def.u32 = mipi_ctrl_regs->mipi_user_def.u32;

        if (index == 0) {
            user_def_dt.bits.user_def0_dt = bit_width;
            user_def.bits.user_def0 = data_type;
        } else if (index == 1) {
            user_def_dt.bits.user_def1_dt = bit_width;
            user_def.bits.user_def1 = data_type;
        } else if (index == MIPI_RX_HAL_VALUE_2) {
            user_def_dt.bits.user_def2_dt = bit_width;
            user_def.bits.user_def2 = data_type;
        } else if (index == MIPI_RX_HAL_VALUE_3) {
            user_def_dt.bits.user_def3_dt = bit_width;
            user_def.bits.user_def3 = data_type;
        }

        mipi_ctrl_regs->mipi_userdef_dt.u32 = user_def_dt.u32;
        mipi_ctrl_regs->mipi_user_def.u32 = user_def.u32;
    }
}

static void mipi_rx_drv_set_user_ef(combo_dev_t devno, data_type_t input_data_type)
{
    u_mipi_userdef_dt user_def_dt;
    u_mipi_user_def user_def;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);
    unsigned char bit_width;
    unsigned int temp_data_type;

    if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
        bit_width = 0;
        temp_data_type = 0x18;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
        bit_width = 0;
        temp_data_type = 0x1a;
    } else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
        bit_width = 0;
        temp_data_type = 0x1e;
    } else {
        bit_width = 4; /* 4 bits width */
        temp_data_type = 0x1e;
    }

    user_def_dt.bits.user_def0_dt = bit_width;
    user_def_dt.bits.user_def1_dt = bit_width;
    user_def_dt.bits.user_def2_dt = bit_width;
    user_def_dt.bits.user_def3_dt = bit_width;

    user_def.bits.user_def0 = temp_data_type;
    user_def.bits.user_def1 = temp_data_type;
    user_def.bits.user_def2 = temp_data_type;
    user_def.bits.user_def3 = temp_data_type;

    mipi_ctrl_regs->mipi_userdef_dt.u32 = user_def_dt.u32;
    mipi_ctrl_regs->mipi_user_def.u32 = user_def.u32;
}

void mipi_rx_drv_set_mipi_yuv_dt(combo_dev_t devno, data_type_t input_data_type)
{
    u_mipi_ctrl_mode_hs mipi_ctrl_mode_hs;
    u_mipi_ctrl_mode_pixel mipi_ctrl_mode_pixel;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    /* mipi_ctrl_mode_hs */
    mipi_ctrl_mode_hs.u32 = mipi_ctrl_regs->mipi_ctrl_mode_hs.u32;

    if (!is_data_type_yuv(input_data_type)) {
        mipi_ctrl_mode_hs.bits.user_def_en = 0;
        mipi_ctrl_regs->mipi_ctrl_mode_hs.u32 = mipi_ctrl_mode_hs.u32;
    } else {
        mipi_ctrl_mode_hs.bits.user_def_en = 1;
        mipi_ctrl_regs->mipi_ctrl_mode_hs.u32 = mipi_ctrl_mode_hs.u32;

        /* user_def && user_def_dt */
        mipi_rx_drv_set_user_ef(devno, input_data_type);
    }

    /* mipi_ctrl_mode_pixel */
    mipi_ctrl_mode_pixel.u32 = mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32;

    if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 1;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 0;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 1;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 0;
    } else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 1;
    } else { /* reset raw type */
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 0;
    }
    mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32 = mipi_ctrl_mode_pixel.u32;
}

void mipi_rx_drv_set_mipi_wdr_user_dt(combo_dev_t devno, data_type_t input_data_type, const short data_type[WDR_VC_NUM])
{
    u_mipi_userdef_dt user_def_dt;
    u_mipi_user_def user_def;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    user_def_dt.u32 = mipi_ctrl_regs->mipi_userdef_dt.u32;
    user_def.u32 = mipi_ctrl_regs->mipi_user_def.u32;

    user_def_dt.bits.user_def0_dt = input_data_type;
    user_def_dt.bits.user_def1_dt = input_data_type;

    user_def.bits.user_def0 = data_type[0];
    user_def.bits.user_def1 = data_type[1];

    mipi_ctrl_regs->mipi_userdef_dt.u32 = user_def_dt.u32;
    mipi_ctrl_regs->mipi_user_def.u32 = user_def.u32;
}

void mipi_rx_drv_set_mipi_dol_id(combo_dev_t devno, data_type_t input_data_type, const short dol_id[])
{
    u_mipi_dol_id_code0 dol_id0;
    u_mipi_dol_id_code1 dol_id1;
    u_mipi_dol_id_code2 dol_id2;
    short lef;
    short sef1;
    short sef2;
    short nxt_lef;
    short nxt_sef1;
    short nxt_sef2;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_rx_unused(input_data_type);
    mipi_rx_unused(dol_id);

    dol_id0.u32 = mipi_ctrl_regs->mipi_dol_id_code0.u32;
    dol_id1.u32 = mipi_ctrl_regs->mipi_dol_id_code1.u32;
    dol_id2.u32 = mipi_ctrl_regs->mipi_dol_id_code2.u32;

    lef = 0x241;
    sef1 = 0x242;
    sef2 = 0x244;

    nxt_lef = 0x251;
    nxt_sef1 = 0x252;
    nxt_sef2 = 0x254;

    dol_id0.bits.id_code_reg0 = lef;
    dol_id0.bits.id_code_reg1 = sef1;
    dol_id1.bits.id_code_reg2 = sef2;

    dol_id1.bits.id_code_reg3 = nxt_lef;
    dol_id2.bits.id_code_reg4 = nxt_sef1;
    dol_id2.bits.id_code_reg5 = nxt_sef2;

    mipi_ctrl_regs->mipi_dol_id_code0.u32 = dol_id0.u32;
    mipi_ctrl_regs->mipi_dol_id_code1.u32 = dol_id1.u32;
    mipi_ctrl_regs->mipi_dol_id_code2.u32 = dol_id2.u32;
}

void mipi_rx_drv_set_mipi_wdr_mode(combo_dev_t devno, mipi_wdr_mode_t wdr_mode)
{
    u_mipi_ctrl_mode_hs mode_hs;
    u_mipi_ctrl_mode_pixel mode_pixel;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mode_hs.u32 = mipi_ctrl_regs->mipi_ctrl_mode_hs.u32;
    mode_pixel.u32 = mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32;

    if (wdr_mode == OT_MIPI_WDR_MODE_NONE) {
        mode_pixel.bits.mipi_dol_mode = 0;
    }
    if (wdr_mode == OT_MIPI_WDR_MODE_VC) {
        mode_pixel.bits.mipi_dol_mode = 0;
    } else if (wdr_mode == OT_MIPI_WDR_MODE_DT) {
        mode_hs.bits.user_def_en = 1;
    } else if (wdr_mode == OT_MIPI_WDR_MODE_DOL) {
        mode_pixel.bits.mipi_dol_mode = 1;
    }

    mipi_ctrl_regs->mipi_ctrl_mode_hs.u32 = mode_hs.u32;
    mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32 = mode_pixel.u32;
}

static void mipi_rx_drv_enable_user_define_dt(combo_dev_t devno, int enable)
{
    u_mipi_ctrl_mode_hs mode_hs;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    if (mipi_ctrl_regs != NULL) {
        mode_hs.u32 = mipi_ctrl_regs->mipi_ctrl_mode_hs.u32;
        mode_hs.bits.user_def_en = enable;

        mipi_ctrl_regs->mipi_ctrl_mode_hs.u32 = mode_hs.u32;
    }
}

void mipi_rx_drv_set_ext_data_type(const ext_data_type_t* data_type, data_type_t input_data_type)
{
    unsigned int i;
    combo_dev_t devno;
    short input_dt;
    short bit_width;
    short ext_bit_width;

    devno = data_type->devno;
    input_dt = mipi_rx_drv_get_data_type(input_data_type);
    bit_width = mipi_rx_drv_get_data_bit_width(input_data_type);

    mipi_rx_drv_set_mipi_user_dt(devno, 0, input_dt, bit_width);

    for (i = 0; i < data_type->num; i++) {
        ext_bit_width = mipi_rx_drv_get_ext_data_bit_width(data_type->ext_data_bit_width[i]);
        mipi_rx_drv_set_mipi_user_dt(devno, i + 1, data_type->ext_data_type[i], ext_bit_width);
    }

    mipi_rx_drv_enable_user_define_dt(devno, 1);
}

unsigned int mipi_rx_drv_get_phy_data(int phy_id, int lane_id)
{
    volatile u_phy_data_link phy_data_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
    unsigned int lane_data = 0x0;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_data_link.u32 = mipi_rx_phy_cfg->phy_data_link.u32;

    if (lane_id == 0) {
        lane_data = phy_data_link.bits.phy_data0_mipi;
    } else if (lane_id == 1) {
        lane_data = phy_data_link.bits.phy_data1_mipi;
    } else if (lane_id == MIPI_RX_HAL_VALUE_2) {
        lane_data = phy_data_link.bits.phy_data2_mipi;
    } else if (lane_id == MIPI_RX_HAL_VALUE_3) {
        lane_data = phy_data_link.bits.phy_data3_mipi;
    }

    return lane_data;
}

unsigned int mipi_rx_drv_get_phy_mipi_link_data(int phy_id, int lane_id)
{
    volatile u_phy_data_mipi_link phy_data_mipi_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
    unsigned int lane_data = 0x0;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_data_mipi_link.u32 = mipi_rx_phy_cfg->phy_data_mipi_link.u32;

    if (lane_id == 0) {
        lane_data = phy_data_mipi_link.bits.phy_data0_mipi_hs;
    } else if (lane_id == 1) {
        lane_data = phy_data_mipi_link.bits.phy_data1_mipi_hs;
    } else if (lane_id == MIPI_RX_HAL_VALUE_2) {
        lane_data = phy_data_mipi_link.bits.phy_data2_mipi_hs;
    } else if (lane_id == MIPI_RX_HAL_VALUE_3) {
        lane_data = phy_data_mipi_link.bits.phy_data3_mipi_hs;
    }

    return lane_data;
}

unsigned int mipi_rx_drv_get_phy_freq(int phy_id)
{
    volatile u_freq_measure freq_measure;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
    unsigned int hs_cnt;
    unsigned int cil_cnt;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    freq_measure.u32 = mipi_rx_phy_cfg->freq_measure.u32;
    hs_cnt = freq_measure.bits.hs_cnt;
    cil_cnt = freq_measure.bits.cil_cnt;

    if (hs_cnt == 0 || cil_cnt == 0) {
        return 0;
    }

    return hs_cnt * MIPI_CIL_FREQ / cil_cnt; /* freq: MHZ >= 1MHZ */
}

unsigned int mipi_rx_drv_get_phy_lvds_link_data(int phy_id, int lane_id)
{
    volatile u_phy_data_lvds_link phy_data_lvds_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg = NULL;
    unsigned int lane_data = 0x0;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_data_lvds_link.u32 = mipi_rx_phy_cfg->phy_data_lvds_link.u32;

    if (lane_id == 0) {
        lane_data = phy_data_lvds_link.bits.phy_data0_lvds_hs;
    } else if (lane_id == 1) {
        lane_data = phy_data_lvds_link.bits.phy_data1_lvds_hs;
    } else if (lane_id == MIPI_RX_HAL_VALUE_2) {
        lane_data = phy_data_lvds_link.bits.phy_data2_lvds_hs;
    } else if (lane_id == MIPI_RX_HAL_VALUE_3) {
        lane_data = phy_data_lvds_link.bits.phy_data3_lvds_hs;
    }

    return lane_data;
}

void mipi_rx_drv_set_data_rate(combo_dev_t devno, mipi_data_rate_t data_rate)
{
    u_mipi_ctrl_mode_pixel mipi_ctrl_mode_pixel;
    unsigned int mipi_double_pix_en = 0;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    if (data_rate == MIPI_DATA_RATE_X1) {
        mipi_double_pix_en = 0;
    } else if (data_rate == MIPI_DATA_RATE_X2) {
        mipi_double_pix_en = 1;
    } else {
        ot_err("unsupported  data_rate:%d  devno %d\n", data_rate, devno);
        return;
    }

    mipi_ctrl_mode_pixel.u32 = mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32;
    mipi_ctrl_mode_pixel.bits.mipi_double_pix_en = mipi_double_pix_en;
    mipi_ctrl_mode_pixel.bits.sync_clear_en = 0x1;
    mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32 = mipi_ctrl_mode_pixel.u32;
}

static void mipi_rx_set_lane_id(combo_dev_t devno, int lane_idx, short lane_id)
{
    u_lane_id0_chn lane_id0_chn;
    u_lane_id1_chn lane_id1_chn;

    volatile global_ctrl_regs_t *global_ctrl_regs = get_global_ctrl_regs(devno);

    lane_id0_chn.u32 = global_ctrl_regs->lane_id0_chn.u32;
    lane_id1_chn.u32 = global_ctrl_regs->lane_id1_chn.u32;

    if (devno == 0) {
    } else if (devno == 1) {
        lane_id = lane_id / 2; /* 2: 1, 3 convert to 0, 1 */
    } else if (devno == 2) { /* dev2 */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_HAL_BODY_PART02_H_ */
