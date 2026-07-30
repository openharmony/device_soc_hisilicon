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

#define __DDRT__
#ifdef DDR_DDRT_SPECIAL_CONFIG
/* Some special DDRT need read register repeatedly */
static unsigned int ddr_ddrt_read(unsigned int addr)
{
	int times = 0;
    unsigned int data0;
    unsigned int data1;
    unsigned int data2;

    do {
        data0 = reg_read(addr);
        data1 = reg_read(addr);
        data2 = reg_read(addr);
        times++;
    } while (((data0 != data1) || (data1 != data2)) && (times < DDRT_READ_TIMEOUT));

    if (times >= DDRT_READ_TIMEOUT) {
        ddr_fatal("DDRT wait timeout");
        ddr_training_stat(DDR_ERR_DDRT_TIME_OUT, 0, -1, -1);
    }

    return data0;
}

/* Some special DDRT need write twice register */
static void ddr_ddrt_write(unsigned int data, unsigned int addr)
{
    unsigned int tmp;
    tmp = reg_read(addr);
    reg_write(data, addr);
    reg_write(data, addr);
}
#endif /* DDR_DDRT_SPECIAL_CONFIG */

static unsigned int ddr_get_rank_size(const struct ddr_cfg_st *cfg)
{
	unsigned int base_dmc = cfg->cur_dmc;
	unsigned int rnkvol;
    unsigned int mem_bank;
    unsigned int mem_row;
    unsigned int mem_col;
    unsigned int mem_width;
	unsigned int size;

    mem_width = (reg_read(base_dmc + DDR_DMC_CFG_DDRMODE) >> DMC_MEM_WIDTH_BIT) & DMC_MEM_WIDTH_MASK;
    rnkvol = reg_read(base_dmc + ddr_dmc_cfg_rnkvol(0));
    mem_bank = (rnkvol >> DMC_RNKVOL_MEM_BANK_BIT) & DMC_RNKVOL_MEM_BANK_MASK;
    mem_row = (rnkvol >> DMC_RNKVOL_MEM_ROW_BIT) & DMC_RNKVOL_MEM_ROW_MASK;
    mem_col = rnkvol & DMC_RNKVOL_MEM_COL_MASK;

    /*
     * mem_bank
     * 0: 4 Bank(2 bank address)
     * 1: 8 Bank(3 bank address)
     * 2: 16 BANK(4 bank address)
     * 3: reserved
     * mem_row
     * 000: 11 bit
     * 001: 12 bit
     * 010: 13 bit
     * 011: 14 bit
     * 100: 15 bit
     * 101: 16 bit
     * 110: 17 bit
     * 111: 18 bit
     * mem_width
     * 000: 8 bit
     * 001: 9 bit
     * 010: 10 bit
     * 011: 11 bit
     * 100: 12 bit
     */
    size = 1UL << ((mem_bank + 2) + (mem_row + 11) + (mem_col + 8) + mem_width); /* 2:2 bank; 11:11 bit; 8:8 bit */
    ddr_debug("rank size[%x]", size);

    return size;
}

/* Init DDRT register before DDRT test */
void ddr_ddrt_init(const struct ddr_cfg_st *cfg, unsigned int mode)
{
    unsigned int mem_width;
    unsigned int mem_config;
    unsigned int offset = 0;

    if (cfg == NULL) {
		return;
	}

    if (cfg->rank_idx == 1) {
		offset = ddr_get_rank_size(cfg);
	}

    ddr_training_ddrt_prepare_func();

    mem_width = ((reg_read(cfg->cur_dmc + DDR_DMC_CFG_DDRMODE) >>
        DMC_MEM_WIDTH_BIT) & DMC_MEM_WIDTH_MASK);
    mem_config = ((mem_width - 1) << DDRT_DDR_MEM_WIDTH) |
        DDRT_DDR_COL_WIDTH | DDRT_DDR_ROW_WIDTH | DDRT_DDR_BANK_WIDTH;
    /* DDRT SDRAM config */
    ddrt_reg_write(mem_config, DDR_REG_BASE_DDRT + DDRT_MEM_CONFIG);
    /* DDR Address Base */
    ddrt_reg_write(ddrt_get_test_addr(DDRT_CFG_BASE_ADDR),
        DDR_REG_BASE_DDRT + DDRT_DDR_BASE_ADDR);
    /* DDRT test DDR using space */
    ddrt_reg_write(ddrt_get_test_addr(ddr_ddrt_get_test_addr() + offset),
        DDR_REG_BASE_DDRT + DDRT_ADDR);
    ddrt_reg_write(DDRT_CFG_SEED, DDR_REG_BASE_DDRT + DDRT_SEED);

    if (mode == DDR_DDRT_MODE_GATE) {
        /* Read or Write Once */
        ddrt_reg_write(DDRT_CFG_BURST_CFG_GATE,
            DDR_REG_BASE_DDRT + DDRT_BURST_CONFIG);
        ddrt_reg_write(0x0,  DDR_REG_BASE_DDRT + DDRT_BURST_NUM);
        ddrt_reg_write(0x0,  DDR_REG_BASE_DDRT + DDRT_ADDR_NUM);
        ddrt_reg_write(0x0,  DDR_REG_BASE_DDRT + DDRT_LOOP_NUM);
        ddrt_reg_write(DDRT_CFG_REVERSED,
            DDR_REG_BASE_DDRT + DDRT_REVERSED_DQ);
    } else {
        /* reversed data form register init table */
        /* 128bit BURST4 */
        ddrt_reg_write(DDRT_CFG_BURST_CFG_DATAEYE,
            DDR_REG_BASE_DDRT + DDRT_BURST_CONFIG);
        ddrt_reg_write(cfg->phy[cfg->phy_idx].dmc[cfg->dmc_idx].ddrt_pattern,
            DDR_REG_BASE_DDRT + DDRT_REVERSED_DQ);
        ddrt_reg_write(DDRT_CFG_BURST_NUM,
            DDR_REG_BASE_DDRT + DDRT_BURST_NUM);
        ddrt_reg_write(DDRT_CFG_ADDR_NUM,
            DDR_REG_BASE_DDRT + DDRT_ADDR_NUM);
        ddrt_reg_write(DDRT_CFG_LOOP_NUM,
            DDR_REG_BASE_DDRT + DDRT_LOOP_NUM);
    }

    ddr_debug("DDRT ADDR[%x = %x]", (DDR_REG_BASE_DDRT + DDRT_ADDR),
        reg_read(DDR_REG_BASE_DDRT + DDRT_ADDR));
}

static int ddr_ddrt_test_process(int byte, int dq)
{
    unsigned int err_ovfl;
    unsigned int err_cnt;
    unsigned int dq_num;
    unsigned int dq_tmp;

	if (dq != -1) { /* check for dq */
		dq_num = ((unsigned int)byte << DDR_BYTE_DQ) + dq;
		err_ovfl = ddrt_reg_read(DDR_REG_BASE_DDRT + DDRT_DQ_ERR_OVFL) & (1 << dq_num);
        if (err_ovfl) {
            return -1;
        }

        /* 3: dq0-dq3 */
        if (dq > 3) {
            dq_tmp = (unsigned int)(dq - 4) << DDR_BYTE_DQ; /* 4 dq: dq0-dq3,dq4-dq7 */
        } else {
            dq_tmp = (unsigned int)dq << DDR_BYTE_DQ;
        }

		err_cnt = ddrt_reg_read(DDR_REG_BASE_DDRT +
			ddrt_dq_err_cnt(((unsigned int)byte << 1) + ((unsigned int)dq >> 2))); /* shift left 2: 4 dq */
		err_cnt = err_cnt & (0xff << dq_tmp);
        if (err_cnt) {
            return -1;
        }
	} else if (byte != -1) { /* check for byte */
		err_ovfl = ddrt_reg_read(DDR_REG_BASE_DDRT +
			DDRT_DQ_ERR_OVFL) & (0xff << ((unsigned int)byte << DDR_BYTE_DQ));
        if (err_ovfl) {
            return -1;
		}

		err_cnt = ddrt_reg_read(DDR_REG_BASE_DDRT +
			ddrt_dq_err_cnt((unsigned int)byte << 1));
		err_cnt += ddrt_reg_read(DDR_REG_BASE_DDRT +
			ddrt_dq_err_cnt(((unsigned int)byte << 1) + 1));
        if (err_cnt) {
            return -1;
		}
	}

    return 0;
}

/*
 * @mask : DDRT option mask.
 * @byte : DDR byte index.
 * @dq   : DDR dq index.
 * DDRT test. Support read_only mode and write_read_compare mode.
 * Success return 0, fail return -1.
 */
int ddr_ddrt_test(unsigned int mask, int byte, int dq)
{
    int result;
    unsigned int regval;
    unsigned int times = 0;

    ddrt_reg_write(mask | DDRT_CFG_START, DDR_REG_BASE_DDRT + DDRT_OP);
    ddrt_reg_write(0, DDR_REG_BASE_DDRT + DDRT_STATUS);

    ddr_asm_dsb();

    do {
        regval = ddrt_reg_read(DDR_REG_BASE_DDRT + DDRT_STATUS);
        times++;
    } while ((!(regval & DDRT_TEST_DONE_MASK)) && (times < DDRT_WAIT_TIMEOUT));

    if (times >= DDRT_WAIT_TIMEOUT) {
        ddr_fatal("DDRT wait timeout");
        ddr_training_stat(DDR_ERR_DDRT_TIME_OUT, 0, -1, -1);
        return -1;
    }

	/* DDRT_READ_ONLY_MODE */
    if ((mask & DDRT_TEST_MODE_MASK) == DDRT_READ_ONLY_MODE) {
        return 0;   /* return when DDRT finish */
    }

	/* DDRT_WR_COMPRARE_MODE No error occurred, test pass. */
    if (regval & DDRT_TEST_PASS_MASK) {
        return 0;
    }

    result = ddr_ddrt_test_process(byte, dq);

    return result;
}

/* Check ddrt test result. Success return 0, fail return -1 */
int ddr_ddrt_check(const struct ddr_cfg_st *cfg)
{
    unsigned int byte_index_to_dmc = cfg->cur_byte;

	/* ddrt test the byte relate to dmc, make sure not overflow */
    if (cfg->cur_byte >= (cfg->dmc_idx << 1)) {
        byte_index_to_dmc = cfg->cur_byte - (cfg->dmc_idx << 1);
    }

    ddrt_reg_write(0, DDR_REG_BASE_DDRT + DDRT_REVERSED_DQ);
    if (ddr_ddrt_test(DDRT_WR_COMPRARE_MODE | DDRT_PATTERM_PRBS9, byte_index_to_dmc, cfg->cur_dq)) {
        return -1;
    }

    ddrt_reg_write(cfg->cur_pattern, DDR_REG_BASE_DDRT + DDRT_REVERSED_DQ);
    if (ddr_ddrt_test(DDRT_WR_COMPRARE_MODE | DDRT_PATTERM_PRBS11, byte_index_to_dmc, cfg->cur_dq)) {
		return -1;
    }

    return 0;
}
