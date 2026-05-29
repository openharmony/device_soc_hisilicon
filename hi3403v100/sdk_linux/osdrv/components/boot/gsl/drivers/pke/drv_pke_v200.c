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
#include <types.h>
#include <common.h>
#include <platform.h>
#include <flash_map.h>
#include <lib.h>
#include <securecutil.h>
#include <sys_reg.h>
#include "drv_pke_v200.h"
#include "drv_pke.h"
#include "ot_drv_pke.h"

/*************************** Structure Definition ****************************/
/** \addtogroup     pke */
/** @{ */ /** <!-- [pke] */
#define REG_PKE_BASE           0x10118000
#define pke_read(offset)       reg_get(REG_PKE_BASE + (offset))
#define pke_write(offset, val) reg_set(REG_PKE_BASE + (offset), (val))

#define cpu_to_be32(v) (((v) >> 24) | (((v) >> 8) & 0xff00) | (((v) << 8) & 0xff0000) | ((v) << 24))

/* ! Define the time out */
#define PKE_TIME_OUT 30000

/* ! Define the start code */
#define PKE_START_CODE 0x05

/* ! Define the key type */
#define PKE_KEY_CFG_PRIVATE 0x05
#define PKE_KEY_CFG_PUBLIC  0x0a

/* ! Define the int code */
#define PKE_INT_NONE 0x0a
#define PKE_INT_DONE 0x05

/* crc 16 */
#define CRC16_TABLE_SIZE 256
#define U16_MSB          0x8000
#define CRC16_POLYNOMIAL 0x1021
#define BYTE_BITS        0x08
#define BYTE_MASK        0xFF

#define PKE_RAM_SECTION_SIZE_IN_BYTE 0x50
#define PKE_RAM_RANG_SIZE            0x400
#define PKE_MUL_RESULT_LEVEL         0x100

#define CHECK_HIGH_TWO_BIT 0xC0
#define PKE_RAM_SECTION_0 0
#define PKE_RAM_SECTION_1 1
#define PKE_RAM_SECTION_2 2
#define PKE_RAM_SECTION_3 3
#define PKE_RAM_SECTION_4 4

uint32_t g_pke_err_code = 0;

/**
 * \brief Function for fetching a random number from the hardware RNG
 *
 * \return 32 bit unsigned random number
 */
static uint32_t get_rng(void)
{
    unsigned int i = 0;
    unsigned int first_rng_value = 0x694B873C;
    unsigned int second_rng_value = 0x694B873C;
    trng_data_status status;

    do {
        status.u32 = reg_get(SEC_COM_TRNG_DATA_ST);
        if (status.bits.trng_fifo_data_cnt >= 2) { /* data count must be more than 2 */
            first_rng_value = reg_get(SEC_COM_TRNG_FIFO_DATA);
            second_rng_value = reg_get(SEC_COM_TRNG_FIFO_DATA);

            i++;

			if ((first_rng_value == 0) || (first_rng_value == 0xffffffff) ||
			    (first_rng_value == 0x694B873C)) {
				continue;
			}

			if ((second_rng_value == 0) || (second_rng_value == 0xffffffff) ||
			    (second_rng_value == 0x694B873C)) {
				continue;
			}

            if (first_rng_value == second_rng_value) {
                continue;
            }

            break;
        }

        __asm__ __volatile__("nop");

        i++;

        __asm__ __volatile__("nop");
    } while (i < 0x10000);

    if ((i == 0) || (i >= 0x10000)) {
        return 0;
    }

    if (first_rng_value == second_rng_value) {
        return 0;
    }

    return second_rng_value;
}

static uint16_t crc_table(uint8_t byte)
{
    uint16_t n_remainder;
    uint16_t m;

    n_remainder = (uint16_t)byte << BYTE_BITS;
    for (m = BYTE_BITS; m > 0; m--) {
        if (n_remainder & U16_MSB)
            n_remainder = (n_remainder << 1) ^ CRC16_POLYNOMIAL;
        else
            n_remainder = (n_remainder << 1);
    }
    return n_remainder;
}

static uint16_t drv_pke_crc16_key(uint16_t initial, const uint8_t *key, uint32_t klen)
{
    uint32_t i, j;
    uint16_t crc16;
    const uint8_t *p = NULL;

    crc16 = initial;

    for (i = klen; i >= PKE_LEN_BLOCK_IN_BYTE; i -= PKE_LEN_BLOCK_IN_BYTE) {
        p = &key[i - PKE_LEN_BLOCK_IN_BYTE];
        for (j = 0; j < PKE_LEN_BLOCK_IN_BYTE; j++)
            crc16 = (crc16 << BYTE_BITS) ^
                crc_table(((crc16 >> BYTE_BITS) ^ p[j]) & BYTE_MASK);
    }

    return crc16;
}

/* brief: crc = CRC16(crc, randnum) */
static uint16_t drv_pke_crc16_random(uint16_t initial, const pke_block *random)
{
    uint32_t i;
    uint16_t crc16 = initial;

    for (i = 0; i < PKE_LEN_BLOCK_IN_BYTE; i++)
        crc16 = (crc16 << BYTE_BITS) ^ crc_table(((crc16 >> BYTE_BITS) ^ random->byte[i]) & BYTE_MASK);

    return crc16;
}

/* brief: crc = CRC16(crc, 0) */
static uint16_t drv_pke_crc16_padding(uint16_t initial, const pke_block *padding, uint32_t len)
{
    uint32_t i;
    uint16_t crc16;
    uint8_t byte = 0;

    crc16 = initial;

    for (i = 0; i < len; i++) {
        byte = padding->byte[i & PKE_LEN_BLOCK_MASK];
        crc16 = (crc16 << BYTE_BITS) ^ crc_table(((crc16 >> BYTE_BITS) ^ byte) & BYTE_MASK);
    }

    return crc16;
}

static uint32_t drv_pke_done_try(void)
{
    pke_int_nomask_status int_raw;
    uint32_t mask;

    int_raw.u32 = pke_read(PKE_INT_NOMASK_STATUS);

    mask = (int_raw.bits.alarm_int_nomask == PKE_INT_DONE)
           || (int_raw.bits.finish_int_nomask == PKE_INT_DONE);

    /* Clean raw int */
    if (mask) {
        int_raw.bits.finish_int_nomask = 1;
        int_raw.bits.alarm_int_nomask = 1;
        pke_write(PKE_INT_NOMASK_STATUS, int_raw.u32);
    }

    return mask;
}

void drv_pke_reset(void)
{
    pke_noise_en noise;
    tee_crg_ctrl crg_ctrl;

    /* clock opened */
    crg_ctrl.u32 = reg_get(TEE_CRG_CTRL);
    crg_ctrl.bits.pke_cken = 1;
    reg_set(TEE_CRG_CTRL, crg_ctrl.u32);

    /* reset */
    crg_ctrl.u32 = reg_get(TEE_CRG_CTRL);
    crg_ctrl.bits.pke_srst_req = 1;
    reg_set(TEE_CRG_CTRL, crg_ctrl.u32);
    udelay(1); /* 1us */

    /* cancel reset */
    crg_ctrl.u32 = reg_get(TEE_CRG_CTRL);
    crg_ctrl.bits.pke_srst_req = 0;
    reg_set(TEE_CRG_CTRL, crg_ctrl.u32);
    udelay(1); /* 1us */

    /* enable noise */
    noise.u32 = pke_read(PKE_NOISE_EN);
    noise.bits.noise_en = PKE_SECURE_TRUE;
    pke_write(PKE_NOISE_EN, noise.u32);
}

int32_t drv_pke_init(void)
{
    pke_lock_ctrl lock_ctrl;
    pke_lock_status lock_status;
    uint32_t i;

    /* lock pke */
    for (i = 0; i < PKE_TIME_OUT; i++) {
        /* check lock result */
        lock_status.u32 = reg_get(PKE_LOCK_STATUS);
        if (lock_status.bits.pke_lock_stat == PKE_LOCK_TEE)
            break;
        /* try to lock pke */
        lock_ctrl.u32 = reg_get(PKE_LOCK_CTRL);
        lock_ctrl.bits.pke_lock_type = PKE_LOCK_TYPE_LOCK;
        lock_ctrl.bits.pke_lock = 1;
        reg_set(PKE_LOCK_CTRL, lock_ctrl.u32);
        udelay(10); /* 10us */
    }

    /* brief: serial_puts("pke lock failed\n"); */
    if (i >= PKE_TIME_OUT)
        return TD_FAILURE;

    drv_pke_reset();

    return TD_SUCCESS;
}

int32_t drv_pke_deinit(void)
{
    pke_lock_ctrl lock_ctrl;
    pke_lock_status lock_status;

    /* try to unlock pke */
    lock_ctrl.u32 = reg_get(PKE_LOCK_CTRL);
    lock_ctrl.bits.pke_lock_type = PKE_LOCK_TYPE_UNLOCK;
    lock_ctrl.bits.pke_lock = 1;
    reg_set(PKE_LOCK_CTRL, lock_ctrl.u32);

    /* check unlock result */
    lock_status.u32 = reg_get(PKE_LOCK_STATUS);
    if ((lock_status.bits.pke_lock_stat != PKE_LOCK_UNLOCK) ||
            (lock_status.bits.pke_unlock_fail == 1))
        return TD_FAILURE;

    return TD_SUCCESS;
}

static void drv_pke_set_ram(pke_ram_type type, uint32_t section,
    const uint8_t *ram, uint32_t klen,
    const pke_block *padding)
{
    uint32_t start;
    uint32_t end;
    uint32_t val;
    uint32_t i;

    start = PKE_MRAM + PKE_RAM_RANG_SIZE * type
        + section * PKE_RAM_SECTION_SIZE_IN_BYTE;
    end = start + PKE_RAM_SECTION_SIZE_IN_BYTE;

    for (i = klen; i >= WORD_WIDTH; i -= WORD_WIDTH) {
        val = 0;
        val = ram[i - 4] << 24;  /* i-4 index shift 24 bits */
        val |= ram[i - 3] << 16; /* i-3 index shift 16 bits */
        val |= ram[i - 2] << 8;  /* i-2 index shift 8 bits */
        val |= ram[i - 1];
        pke_write(start, val);
        start += WORD_WIDTH;
    }

    /* padding */
    if (padding != NULL) {
        i = 1;
        while (start < end) {
            val = cpu_to_be32(padding->word[i]);
            pke_write(start, val);
            start += WORD_WIDTH;
            i ^= 0x01;
        }
    }

    return;
}

static void drv_pke_get_ram(pke_ram_type type, uint32_t section, uint8_t *ram, uint32_t klen)
{
    uint32_t addr;
    uint32_t val;
    uint32_t len = klen;
    uint32_t i;
    uint32_t count = 0;

    addr = PKE_MRAM + PKE_RAM_RANG_SIZE * type
           + section * PKE_RAM_SECTION_SIZE_IN_BYTE;

	for (i = len; i >= WORD_WIDTH; i -= WORD_WIDTH) {
		val = pke_read(addr + i - WORD_WIDTH);
		ram[len - i + 0] = (val >> 24) & 0xFF; /* shift 24 bits */
		ram[len - i + 1] = (val >> 16) & 0xFF; /* shift 16 bits */
		ram[len - i + 2] = (val >> 8) & 0xFF;  /* offset 2, shift 8 bits */
		ram[len - i + 3] = (val) & 0xFF;       /* offset 3 */
		count++;
	}
    if (count != (len / WORD_WIDTH)) {
		return;
	}

    count = 0;
    for (i = len; i >= WORD_WIDTH; i -= WORD_WIDTH) {
        val = ram[len - i + 0] << 24;  /* shift 24 bits */
        val += ram[len - i + 1] << 16; /* shift 16 bits */
        val += ram[len - i + 2] << 8;  /* offset 2, shift 8 bits */
        val += ram[len - i + 3];       /* offset 3 */
        if (pke_read(addr + i - WORD_WIDTH) != val)
            call_reset();
        count++;
    }
}

static void drv_pke_set_mode(pke_mode mode, uint32_t len)
{
    pke_work_mode work;

    work.u32 = 0x00;

    work.bits.mode_sel = mode;

    if (mode == PKE_MODE_CLR_RAM) {
        work.bits.data_len = PKE_DATA_LEN_BYTE_512;
        work.bits.ram_sel = PKE_CLR_RAM_SEL_ALL;
    } else {
        work.bits.ram_sel = PKE_CLR_RAM_SEL_NONE;
        work.bits.data_len = len / PKE_LEN_BLOCK_IN_BYTE;
    }

    pke_write(PKE_WORK_MODE, work.u32);

    return;
}

static void drv_pke_start(void)
{
    pke_start start;

    start.u32 = 0x00;
    start.bits.pke_start = PKE_START_CODE;

    pke_write(PKE_START, start.u32);

    return;
}

static int32_t drv_pke_wait_free(void)
{
    uint32_t i;
    pke_busy busy;

    /* wait ready */
    for (i = 0; i < PKE_TIME_OUT; i++) {
        busy.u32 = pke_read(PKE_BUSY);
        if (!busy.bits.pke_busy)
            break;

        udelay(100); /* 100us */
    }

    if (i >= PKE_TIME_OUT)
        return TD_FAILURE;

    return TD_SUCCESS;
}

static int32_t drv_pke_error_code(void)
{
    pke_result_flag result;

    result.u32 = pke_read(PKE_RESULT_FLAG);
    if (result.bits.pke_result_flag != PKE_RESULT_FLAG_OK) {
        g_pke_err_code = pke_read(PKE_FAILURE_FLAG);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* wait pke done */
static int32_t drv_pke_wait_done(void)
{
    uint32_t i;
    int32_t ret;

    for (i = 0; i < PKE_TIME_OUT; i++) {
        if (drv_pke_done_try())
            break;

        udelay(100); /* 100us */
    }

    if (i >= PKE_TIME_OUT)
        return TD_FAILURE;

	ret = drv_pke_error_code();
    if (ret != TD_SUCCESS) {
		return ret;
	}

    return TD_SUCCESS;
}

static void drv_pke_set_key(const uint8_t *inkey, uint8_t *outkey, uint32_t klen,
    const pke_block *random, uint32_t pad_len)
{
    uint32_t i;
    uint16_t crc16;

    if (random != NULL) { /* with random mask */
        for (i = 0; i < klen; i++)
            outkey[i] = inkey[i] ^ random->byte[i & PKE_LEN_BLOCK_MASK];

        crc16 = drv_pke_crc16_random(0x0000, random);
        crc16 = drv_pke_crc16_key(crc16, outkey, klen);
        crc16 = drv_pke_crc16_padding(crc16, random, pad_len);

        /* set Random */
        pke_write(PKE_KEY_RANDOM_1, cpu_to_be32(random->word[1]));
        pke_write(PKE_KEY_RANDOM_2, cpu_to_be32(random->word[0]));

		/* set src16 */
		pke_write(PKE_KEY_CRC, crc16);
	} else { /* not private key */
        if (memcpy_s(outkey, SM2_LEN_IN_BYTE, inkey, klen) != EOK) {
			return;
		}
	}

    /* select cpu key */
    pke_write(PKE_OTP_KEY_SEL, PKE_SECURE_FALSE);

    return;
}

static int32_t drv_pke_clean_ram(void)
{
    int32_t ret;

	ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    drv_pke_set_mode(PKE_MODE_CLR_RAM, 0);
    drv_pke_start();

	ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}

/* c=m^e mod n */
int32_t drv_ifep_rsa_exp_mod(const unsigned char *n, const unsigned char *k,
    const unsigned char *in, uint8_t *out, uint32_t klen)
{
    int32_t ret;
    int32_t ret1;
    uint8_t *key;
    pke_block random;
    errno_t err;

    key = (uint8_t *)gsl_malloc(RSA_TOTAL_LEN);
    if (key == NULL)
        return TD_FAILURE;

    drv_pke_reset();

    err = memset_s(key, RSA_TOTAL_LEN, 0, klen * 2); /* 2: double len */
    if (err != EOK) {
        gsl_free(key);
        return TD_FAILURE;
    }

    ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        goto _exit;
    }

    random.word[0] = get_rng();
    random.word[1] = get_rng();

    /* with random mask */
    pke_write(PKE_CFG_KEY_FLAG, PKE_KEY_CFG_PRIVATE);

    drv_pke_set_mode(PKE_MODE_EXP_MOD, klen);

	/* key xor random */
	err = memcpy_s(key, RSA_TOTAL_LEN, k, klen);
	if (err != EOK) {
        gsl_free(key);
        return TD_FAILURE;
    }

	err = memcpy_s(key + klen, RSA_TOTAL_LEN - klen, n, klen);
	if (err != EOK) {
        gsl_free(key);
		return TD_FAILURE;
	}

    drv_pke_set_key(key, key, klen + klen, &random, 0);

    /* set data */
    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, 0, in, klen, NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_NRAM, 0, key + klen, klen, NULL); /* n */
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, 0, key, klen, NULL);        /* k */

    /* start */
    drv_pke_start();

	ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
		goto _exit;
	}

    drv_pke_get_ram(PKE_RAM_TYPE_RRAM, 0, out, klen);

_exit:
	gsl_free(key);
	ret1 = drv_pke_clean_ram();
	if ((ret == TD_SUCCESS) && (ret1 != TD_SUCCESS))
		ret = ret1;

    return ret;
}

