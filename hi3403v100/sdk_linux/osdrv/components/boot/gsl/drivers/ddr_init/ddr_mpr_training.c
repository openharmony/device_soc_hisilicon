/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ddr_training_impl.h"

#define DDR_MPR_TRAINING
#ifdef DDR_MPR_TRAINING_CONFIG
/* Switch MPR function */
static void ddr_mpr_switch(unsigned int base_dmc, int val)
{
    unsigned int sfc_cmd;
    if (val == DDR_TRUE)
        sfc_cmd = (DMC_CMD_MRS_MR3 << DMC_SFC_CMD_MRS_BIT) | DMC_CMD_TYPE_LMR;
    else
        sfc_cmd = DMC_CMD_TYPE_LMR;

    ddr_dmc_sfc_cmd(base_dmc, sfc_cmd, 0x0, DMC_BANK_MR3);

    /* clear */
    if (val == DDR_FALSE) {
        reg_write(0x0, base_dmc + DDR_DMC_SFCBANK);
        reg_write(0x0, base_dmc + DDR_DMC_SFCREQ);
    }
}

/* Judge MPR data */
static int ddr_mpr_judge(unsigned int data1, unsigned int data2,
    unsigned int data3, unsigned int data4, unsigned int dq_index)
{
    /* check byte */
    if (dq_index == -1) {
        if (data1 == DDR_MPR_BYTE_MASK && data2 == 0x0 &&
            data3 == DDR_MPR_BYTE_MASK && data4 == 0x0)
            return 0;
        else
            return -1;
    } else {
        /* check DQ */
        data1 = (data1 >> dq_index) & DDR_MPR_BIT_MASK;
        data2 = (data2 >> dq_index) & DDR_MPR_BIT_MASK;
        data3 = (data3 >> dq_index) & DDR_MPR_BIT_MASK;
        data4 = (data4 >> dq_index) & DDR_MPR_BIT_MASK;
        if (data1 == DDR_MPR_BIT_MASK && data2 == 0x0 &&
            data3 == DDR_MPR_BIT_MASK && data4 == 0x0)
            return 0;
        else
            return -1;
    }
}

/* Extract MPR read data to judge */
static int ddr_mpr_extract(struct ddr_cfg_st *cfg,
    unsigned int offset0, unsigned int offset1,
    unsigned int offset2, unsigned int offset3)
{
    unsigned int data1;
    unsigned int data2;
    unsigned int data3;
    unsigned int data4;
	unsigned int base_dmc = cfg->cur_dmc;
	unsigned int byte_index = cfg->cur_byte;

    data1 = reg_read(base_dmc + offset0);  /* SFC read data [127:96] or [255:224] */
    data2 = reg_read(base_dmc + offset1);  /* SFC read data [95:64] or [223:192] */
    data3 = reg_read(base_dmc + offset2);  /* SFC read data [63:32] or [191:160] */
    data4 = reg_read(base_dmc + offset3);  /* SFC read data [31:0] or [159:128] */

    ddr_info("byte[%x] data[%x=%x][%x=%x][%x=%x][%x=%x]",
        byte_index,
        base_dmc + offset0, data1, base_dmc + offset1, data2,
        base_dmc + offset2, data3, base_dmc + offset3, data4);

    if (get_byte_num(cfg) == DDR_PHY_BYTE_MAX) {
        /* four byte: data1[0xFFFFFFFF] data2[0x00000000]
        data3[0xFFFFFFFF] data4[0x00000000] */
        data1 = (data1 >> (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) & DDR_MPR_BYTE_MASK;
        data2 = (data2 >> (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) & DDR_MPR_BYTE_MASK;
        data3 = (data3 >> (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) & DDR_MPR_BYTE_MASK;
        data4 = (data4 >> (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) & DDR_MPR_BYTE_MASK;
    } else {
        /* two byte: data1[0xFFFF0000] data2[0xFFFF0000]
        data3[0xFFFF0000] data4[0xFFFF0000] */
        data1 = (data1 >> DDR_MPR_BYTE_BIT >> (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) &
            DDR_MPR_BYTE_MASK;
        data2 = (data2 >> (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) & DDR_MPR_BYTE_MASK;
        data3 = (data3 >> DDR_MPR_BYTE_BIT >> (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) &
            DDR_MPR_BYTE_MASK;
        data4 = (data4 >> (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) & DDR_MPR_BYTE_MASK;
        if (ddr_mpr_judge(data1, data2, data3, data4, cfg->cur_dq))
            return -1;

        /* two byte need to swap data and check again */
        data1 = (reg_read(base_dmc + DDR_DMC_SFC_RDATA1) >>
            DDR_MPR_BYTE_BIT >> (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) &
            DDR_MPR_BYTE_MASK;
        data2 = (reg_read(base_dmc + DDR_DMC_SFC_RDATA0) >>
            (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) & DDR_MPR_BYTE_MASK;
        data3 = (reg_read(base_dmc + DDR_DMC_SFC_RDATA3) >>
            DDR_MPR_BYTE_BIT >> (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) &
            DDR_MPR_BYTE_MASK;
        data4 = (reg_read(base_dmc + DDR_DMC_SFC_RDATA2) >>
            (byte_index << DDR_MPR_BYTE_SHIFT_BIT)) & DDR_MPR_BYTE_MASK;
    }

    return ddr_mpr_judge(data1, data2, data3, data4, cfg->cur_dq);
}

/* Find RDQ via MPR */
static int ddr_mpr_find_rdq(struct ddr_cfg_st *cfg)
{
	struct training_data training;
	unsigned int dq_num;
	unsigned int win_num;
    unsigned int def_dq;
    unsigned int best_dq;
    unsigned int byte_index;
    unsigned int dq_index;

    ddrtr_set_data(&training, 0, sizeof(struct training_data));
    /* find rdq via mpr */
    cfg->dq_check_type = DDR_CHECK_TYPE_MPR;

    /* find rdq */
    for (byte_index = 0;
        byte_index < get_byte_num(cfg); byte_index++) {
        for (dq_index = 0; dq_index < DDR_PHY_BIT_NUM; dq_index++) {
            dq_num = (byte_index << DDR_BYTE_DQ) + dq_index;
            def_dq = ddr_phy_get_dq_bdl(cfg);
            ddr_dataeye_find_dq(cfg, &training);
            win_num = training.ddr_bit_best[dq_num] >> DDR_DATAEYE_RESULT_BIT;
            best_dq = training.ddr_bit_best[dq_num] & DDR_DATAEYE_RESULT_MASK;
            if (win_num > 0) {
                ddr_phy_set_dq_bdl(cfg, best_dq);
            } else {
                /* In normal case, not reach here */
                /* restore default value */
                ddr_phy_set_dq_bdl(cfg, def_dq);

                ddr_fatal("PHY[%x] Byte[%x] DQ[%x] MPR fail",
                    cfg->cur_phy, byte_index, dq_index);
                ddr_training_stat(DDR_ERR_MPR, cfg->cur_phy,
                    byte_index, dq_index);
                return -1;
            }
        }
    }

    return 0;
}

/* Find RDQS via MPR */
static int ddr_mpr_find_rdqs(struct ddr_cfg_st *cfg)
{
	unsigned int rdqs_start = 0;
	unsigned int rdqs_end   = PHY_RDQS_BDL_MASK;
	unsigned int rdqs_mid;
    unsigned int val;
    unsigned int delay;
	unsigned int count = 0;
	int found = DDR_FALSE;

    /* set rdq to middle value */
    reg_write(PHY_DQ_MIDDLE_VAL << PHY_BDL_DQ_BIT,
        cfg->cur_phy + ddr_phy_dxnrdqnbdl0(cfg->rank_idx, cfg->cur_byte));
    reg_write(PHY_DQ_MIDDLE_VAL << PHY_BDL_DQ_BIT,
        cfg->cur_phy + ddr_phy_dxnrdqnbdl1(cfg->rank_idx, cfg->cur_byte));

    /* clear rdqs */
    delay = reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte)) >> PHY_RDQS_BDL_BIT;
    rdqs_mid = delay;   /* if not found, restore default value */
    delay = delay & (~PHY_RDQS_BDL_MASK);

    /* find rdqs */
    for (val = 0; val <= PHY_RDQS_BDL_MASK; val++) {
        reg_write(delay | (val << PHY_RDQS_BDL_BIT),
            cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte));
        ddr_phy_cfg_update(cfg->cur_phy);
        /* check ok */
        if (!ddr_mpr_check(cfg)) {
            if (found == DDR_TRUE)
                continue;

            rdqs_start = val; /* found start value */
            count++;
            if (count == DDR_MPR_RDQS_FIND_TIMES)
                found = DDR_TRUE;
        } else {
            if (found == DDR_TRUE) {
                rdqs_end = val;  /* found end value */
                break;
            }
        }
    }

    if (found == DDR_TRUE) {
        rdqs_mid = ((rdqs_end - rdqs_start) >> 1) + rdqs_start;
        ddr_info("PHY[%x] Byte[%x] rdqs_middle[%x]",
            cfg->cur_phy, cfg->cur_byte, rdqs_mid);
        ddr_info("rdqs_start[%x] rdqs_end[%x]", rdqs_start, rdqs_end);
    } else {
        ddr_fatal("PHY[%x] Byte[%x] not find RDQS, restore",
            cfg->cur_phy, cfg->cur_byte);
        ddr_training_stat(DDR_ERR_MPR, cfg->cur_phy, cfg->cur_byte, -1);
    }

    reg_write(delay | (rdqs_mid << PHY_RDQS_BDL_BIT),
        cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte));
    ddr_phy_cfg_update(cfg->cur_phy);

    return ((found == DDR_TRUE) ? 0 : -1);
}

/* Multi Purpose Register(MPR) */
static int ddr_mpr_training(struct ddr_cfg_st *cfg)
{
    int i;
    int result = 0;
    unsigned int byte_num = get_byte_num(cfg);
    unsigned int mr0;
    unsigned int sfc_cmd;
    unsigned int base_dmc = cfg->cur_dmc;
    unsigned int base_phy = cfg->cur_phy;

    ddr_debug("DDR MPR training");

    /* set DDR bust */
    if (byte_num == DDR_PHY_BYTE_MAX) {
        mr0 = (reg_read(base_phy + DDR_PHY_MODEREG01) &
            DMC_MRS_MASK) & (~DMC_MR0_BL_MASK);
        sfc_cmd = ((mr0 | DMC_MR0_BL_BUST4) << DMC_SFC_CMD_MRS_BIT) |
            DMC_CMD_TYPE_LMR;
        ddr_dmc_sfc_cmd(base_dmc, sfc_cmd, 0x0, 0x0);
    }

    /* precharge all */
    ddr_dmc_sfc_cmd(base_dmc, DMC_CMD_TYPE_PRECHARGE_ALL, 0x0, 0x0);

    /* enable MPR */
    ddr_mpr_switch(base_dmc, DDR_TRUE);

    /* find rdqs */
    for (i = 0; i < byte_num; i++)
        result += ddr_mpr_find_rdqs(cfg);

    /* find rdq */
    if (!result)
        result = ddr_mpr_find_rdq(cfg);

    /* disable MPR */
    ddr_mpr_switch(base_dmc, DDR_FALSE);

    /* restore DDR bust */
    if (byte_num == DDR_PHY_BYTE_MAX) {
        mr0 = (reg_read(base_phy + DDR_PHY_MODEREG01) & DMC_MRS_MASK);
        sfc_cmd = (mr0 << DMC_SFC_CMD_MRS_BIT) | DMC_CMD_TYPE_LMR;
        ddr_dmc_sfc_cmd(base_dmc, sfc_cmd, 0x0, 0x0);
    }

    return result;
}

int ddr_mpr_training_func(struct ddr_cfg_st *cfg)
{
    struct tr_relate_reg relate_reg;
    int result;

    /* MPR training disable */
    if (ddr_training_check_bypass(cfg, DDR_BYPASS_MPR_MASK) != DDR_FALSE)
        return 0;

    ddr_training_save_reg(cfg, &relate_reg, DDR_BYPASS_MPR_MASK);
    result = ddr_mpr_training(cfg);
    ddr_training_restore_reg(cfg, &relate_reg);

    return result;
}
#else
int ddr_mpr_training_func(struct ddr_cfg_st *cfg)
{
    ddr_warning("Not support DDR MPR training");
    return 0;
}
#endif /* DDR_MPR_TRAINING_CONFIG */
