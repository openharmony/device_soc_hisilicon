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

#include "drv_pke_v200.h"
#include "drv_pke.h"
#include "drv_trng.h"

#ifdef CHIP_PKE_VER_V200

/*************************** Structure Definition ****************************/
/** \addtogroup     pke */
/** @{ */ /** <!-- [pke] */

/* ! Define the time out */
#define PKE_TIME_OUT      6000

/* ! Define the start code */
#define PKE_START_CODE 0x05

/* ! Define the key type */
#define PKE_KEY_CFG_PRIVATE 0x05
#define PKE_KEY_CFG_PUBLIC  0x0a

/* ! Define the int code */
#define PKE_INT_NONE 0x0a
#define PKE_INT_DONE 0x05

#define PKE_MUL_DOT_SIZE_MIN 0x20

/* crc 16 */
#define CRC16_TABLE_SIZE 256
#define U16_MSB          0x8000
#define CRC16_POLYNOMIAL 0x1021
#define BYTE_MASK        0xFF

#define PKE_RAM_SECTION_SIZE_IN_BYTE  0x50
#define PKE_RAM_RANG_SIZE             0x400
#define PKE_MUL_RESULT_LEVEL          0x100

#define CHECK_HIGH_TWO_BIT  0xC0

const char *err_string[PKE_FAIL_FLAG_CNT] = {
    "The mod number is even when mod-inverse operation",
    "Alloc random number failed",
    "The result of mod add/multiply is infinity"
    "Point multiplication or point addition result is infinity point",
    "The input point is not on the elliptic curve",
    "Attacked by DFX",
    "Attacked by DFA",
    "Calling unit module failed",
};

/* ! Define the context of pke */
typedef struct {
    td_u32 done;             /* !<  calculation finish flag */
    td_u32 pke_sub_ver;      /* !<  pke support mask? */
    CRYPTO_QUEUE_HEAD queue; /* !<  queue list */
    td_u32 attacked;         /* !<  if be attacked, don't compute any more */
} pke_hard_context;

static pke_hard_context g_pke_info;

/* ! pke already initialize or not */
static td_u32 g_pke_init = TD_FALSE;

/* current cpu */
#define PKE_CPU_CUR (crypto_is_sec_cpu() ? PKE_LOCK_TEE : PKE_LOCK_REE)

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      pke */
/** @{ */ /** <!--[pke] */

static uint16_t crc_table(uint8_t byte)
{
    uint16_t remainder;
    uint16_t m;

    remainder = (uint16_t)byte << BYTE_BITS;
    for (m = BYTE_BITS; m > 0; m--) {
        if (remainder & U16_MSB) {
            remainder = (remainder << 1) ^ CRC16_POLYNOMIAL;
        } else {
            remainder = (remainder << 1);
        }
    }
    return remainder;
}

static uint16_t drv_pke_crc16_key(uint16_t initial, const uint8_t *key, uint32_t klen)
{
    uint32_t i;
    uint32_t j;
    uint16_t crc16;
    const uint8_t *p = NULL;

    crc16 = initial;

    for (i = klen; i >= PKE_LEN_BLOCK_IN_BYTE; i -= PKE_LEN_BLOCK_IN_BYTE) {
        p = &key[i - PKE_LEN_BLOCK_IN_BYTE];
        for (j = 0; j < PKE_LEN_BLOCK_IN_BYTE; j++) {
            crc16 = (crc16 << BYTE_BITS) ^ crc_table(((crc16 >> BYTE_BITS) ^ p[j]) & BYTE_MASK);
        }
    }

    return crc16;
}

/* *** crc = CRC16(crc, randnum) *** */
static uint16_t drv_pke_crc16_random(uint16_t initial, const pke_block *random)
{
    uint32_t i;
    uint16_t crc16 = initial;

    for (i = 0; i < PKE_LEN_BLOCK_IN_BYTE; i++) {
        crc16 = (crc16 << BYTE_BITS) ^ crc_table(((crc16 >> BYTE_BITS) ^ random->byte[i]) & BYTE_MASK);
    }

    return crc16;
}

/* *** crc = CRC16(crc, 0) *** */
static uint16_t drv_pke_crc16_padding(uint16_t initial, const pke_block *padding, uint32_t len)
{
    uint32_t i;
    uint16_t crc16;
    uint8_t byte;

    crc16 = initial;

    for (i = 0; i < len; i++) {
        byte = padding->byte[i & PKE_LEN_BLOCK_MASK];
        crc16 = (crc16 << BYTE_BITS) ^ crc_table(((crc16 >> BYTE_BITS) ^ byte) & BYTE_MASK);
    }

    return crc16;
}

static td_u32 drv_pke_done_try(void)
{
    pke_int_nomask_status int_raw;
    td_u32 mask;

    int_raw.u32 = pke_read(PKE_INT_NOMASK_STATUS);
    ot_cipher_dbg("pke int_raw 0x%x\n", int_raw.u32);

    mask = (int_raw.bits.alarm_int_nomask == PKE_INT_DONE) ||
        (int_raw.bits.finish_int_nomask == PKE_INT_DONE);

    /* Clean raw int */
    if (mask) {
        int_raw.bits.finish_int_nomask = 1;
        int_raw.bits.alarm_int_nomask = 1;
        pke_write(PKE_INT_NOMASK_STATUS, int_raw.u32);
    }

    return mask;
}

#ifdef CRYPTO_OS_INT_SUPPORT
static td_u32 drv_pke_done_notify(void)
{
    pke_int_status int_st;
    pke_int_nomask_status int_raw;

    int_st.u32 = pke_read(PKE_INT_STATUS);
    int_raw.u32 = 0x00;

    int_raw.bits.finish_int_nomask = 1;
    int_raw.bits.alarm_int_nomask = 1;

    /* Clean raw int */
    pke_write(PKE_INT_NOMASK_STATUS, int_raw.u32);

    return (int_st.bits.alarm_int == PKE_INT_DONE) || (int_st.bits.finish_int == PKE_INT_DONE);
}

/* ! pke interrupt process function */
static CRYPTO_IRQRETURN_T pke_interrupt_isr(td_s32 irq, td_void *dev_id)
{
    td_u32 mask;

    crypto_unused(irq);
    crypto_unused(dev_id);

    mask = drv_pke_done_notify();
    if (mask != 0x00) {
        g_pke_info.done = TD_TRUE;
        ot_cipher_dbg("pke wake up\n");
        crypto_queue_wait_up(&g_pke_info.queue);
    }

    return OSAL_IRQ_HANDLED;
}

/* ! pke register interrupt process function */
static td_s32 drv_pke_register_interrupt(void)
{
    td_s32 ret;
    td_u32 int_valid = 0;
    td_u32 int_num = 0;
    const char *name = TD_NULL;

    func_enter();

    module_get_attr(CRYPTO_MODULE_ID_PKE, &int_valid, &int_num, &name);

    /* request irq */
    ot_cipher_dbg("pke request irq, num %d, name %s\n", int_num, name);
    ret = crypto_request_irq(int_num, pke_interrupt_isr, name);
    if (ret != TD_SUCCESS) {
        ot_cipher_error("Irq request failure, irq = %d, name %s", int_num, name);
        print_errno(OT_ERR_CIPHER_REGISTER_IRQ);
        return OT_ERR_CIPHER_REGISTER_IRQ;
    }

    /* initialize queue list */
    crypto_queue_init(&g_pke_info.queue);

    func_exit();
    return TD_SUCCESS;
}

/* ! pke unregister interrupt process function */
static void drv_pke_unregister_interrupt(void)
{
    td_u32 int_valid = 0;
    td_u32 int_num = 0;
    const char *name = TD_NULL;

    func_enter();

    module_get_attr(CRYPTO_MODULE_ID_PKE, &int_valid, &int_num, &name);

    /* free irq */
    ot_cipher_dbg("pke free irq, num %d, name %s\n", int_num, name);
    crypto_free_irq(int_num, name);

    func_exit();

    return;
}
#endif

/* ! set interrupt */
static void pke_set_interrupt(void)
{
    pke_int_enable pke_int_en;

    pke_int_en.u32 = pke_read(PKE_INT_ENABLE);

    pke_int_en.bits.int_enable = 0;
    pke_int_en.bits.finish_int_enable = 0;
    pke_int_en.bits.alarm_int_enable = 0;

#ifdef CRYPTO_OS_INT_SUPPORT
    {
        pke_int_en.bits.int_enable = 1;
        pke_int_en.bits.finish_int_enable = 1;
        pke_int_en.bits.alarm_int_enable = 1;
    }
#endif

    pke_write(PKE_INT_ENABLE, pke_int_en.u32);
    ot_cipher_dbg("PKE_INT_EN: 0x%x\n", pke_int_en.u32);

    return;
}

td_s32 drv_pke_init(void)
{
    pke_power_on power;
    pke_noise_en noise;

    func_enter();

    ot_cipher_dbg("enable pke\n");

    if (g_pke_init) {
        return TD_SUCCESS;
    }

    module_enable(CRYPTO_MODULE_ID_PKE);

#ifdef CRYPTO_OS_INT_SUPPORT
    {
        td_s32 ret;

        ot_cipher_dbg("pke register interrupt function\n");
        ret = drv_pke_register_interrupt();
        if (ret != TD_SUCCESS) {
            ot_cipher_error("error, register interrupt failed\n");
            print_func_errno(drv_pke_register_interrupt, ret);
            module_disable(CRYPTO_MODULE_ID_PKE);
            return ret;
        }
    }
#endif

    g_pke_init = TD_TRUE;
    g_pke_info.done = TD_TRUE;
    g_pke_info.attacked = TD_FALSE;

    power.u32 = pke_read(PKE_POWER_EN);
    power.bits.power_en = PKE_SECURE_TRUE;
    pke_write(PKE_POWER_EN, power.u32);

    noise.u32 = pke_read(PKE_NOISE_EN);
    noise.bits.noise_en = PKE_SECURE_TRUE;
    pke_write(PKE_NOISE_EN, noise.u32);

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_pke_deinit(void)
{
    ot_cipher_dbg("disable pke\n");

    if (g_pke_init == TD_FALSE) {
        return TD_SUCCESS;
    }

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_pke_unregister_interrupt();
#endif

    g_pke_init = TD_FALSE;

    return TD_SUCCESS;
}

static td_s32 drv_pke_lock(void)
{
    pke_lock_ctrl lock_ctrl;
    pke_lock_status lock_status;
    td_u32 lock_code = PKE_LOCK_TEE;
    uint32_t i;

    if (crypto_is_sec_cpu() == TD_FALSE) {
        lock_code = PKE_LOCK_REE;
    }

    /* lock pke */
    for (i = 0; i < PKE_TIME_OUT; i++) {
        /* pke unlock ? */
        lock_status.u32 = pke_read(PKE_LOCK_STATUS);
        ot_cipher_dbg("-PKE_LOCK_STATUS[0x%x] 0x%x\n", PKE_LOCK_STATUS, lock_status.u32);
        if (lock_status.bits.pke_lock_stat != PKE_LOCK_UNLOCK) {
            crypto_msleep(1);
            continue;
        }

        /* try to lock pke */
        lock_ctrl.u32 = pke_read(PKE_LOCK_CTRL);
        lock_ctrl.bits.pke_lock_type = PKE_LOCK_TYPE_LOCK;
        lock_ctrl.bits.pke_lock = 1;
        pke_write(PKE_LOCK_CTRL, lock_ctrl.u32);

        /* check lock result */
        lock_status.u32 = pke_read(PKE_LOCK_STATUS);
        ot_cipher_dbg("+PKE_LOCK_STATUS[0x%x] 0x%x\n", PKE_LOCK_STATUS, lock_status.u32);
        if (lock_status.bits.pke_lock_stat == lock_code) {
            break;
        }
    }

    if (i >= PKE_TIME_OUT) {
        ot_cipher_error("pke lock timeout\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 drv_pke_unlock(void)
{
    pke_lock_ctrl lock_ctrl;
    pke_lock_status lock_status;

    /* try to unlock pke */
    lock_ctrl.u32 = pke_read(PKE_LOCK_CTRL);
    lock_ctrl.bits.pke_lock_type = PKE_LOCK_TYPE_UNLOCK;
    lock_ctrl.bits.pke_lock = 1;
    pke_write(PKE_LOCK_CTRL, lock_ctrl.u32);

    /* check unlock result */
    lock_status.u32 = pke_read(PKE_LOCK_STATUS);
    if ((lock_status.bits.pke_lock_stat != PKE_LOCK_UNLOCK) ||
        (lock_status.bits.pke_unlock_fail == 1)) {
        ot_cipher_error("pke unlock timeout\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}


td_s32 drv_pke_resume(void)
{
    td_s32 ret;

    func_enter();

    ret = drv_pke_lock();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_pke_lock, ret);
        return ret;
    }

    ot_cipher_dbg("pke interrupt configure\n");
    pke_set_interrupt();

    func_exit();
    return TD_SUCCESS;
}

void drv_pke_suspend(void)
{
    td_s32 ret;

    func_enter();

    ret = drv_pke_unlock();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_pke_unlock, ret);
        return;
    }

    func_exit();
    return;
}

static void drv_pke_set_ram(pke_ram_type type,
    td_u32 section, const td_u8 *ram, td_u32 klen, const pke_block *padding)
{
    td_u32 start;
    td_u32 end;
    td_u32 val;
    td_u32 i;

    start = PKE_MRAM + PKE_RAM_RANG_SIZE * type + section * PKE_RAM_SECTION_SIZE_IN_BYTE;
    end = start + PKE_RAM_SECTION_SIZE_IN_BYTE;

    for (i = klen; i >= WORD_WIDTH; i -= WORD_WIDTH) {
        val  = ram[i - 4] << 24;  /* offset 4: bit 31~24 */
        val |= ram[i - 3] << 16;  /* offset 3: bit 23~16 */
        val |= ram[i - 2] << 8;   /* offset 2: bit 15~8  */
        val |= ram[i - 1];        /* offset 1: bit 7~0   */
        pke_write(start, val);
        start += WORD_WIDTH;
    }

    /* padding */
    if (padding != TD_NULL) {
        i = 1;
        while (start < end) {
            val = crypto_cpu_to_be32(padding->word[i]);
            pke_write(start, val);
            start += WORD_WIDTH;
            i ^= 0x01;
        }
    }

    return;
}

static void drv_pke_get_ram(pke_ram_type type, td_u32 section, td_u8 *ram, td_u32 klen)
{
    td_u32 addr;
    td_u32 val;
    td_u32 i;
    td_u32 len = klen;

    addr = PKE_MRAM + PKE_RAM_RANG_SIZE * type + section * PKE_RAM_SECTION_SIZE_IN_BYTE;

    for (i = len; i >= WORD_WIDTH; i -= WORD_WIDTH) {
        val = pke_read(addr + i - WORD_WIDTH);
        ram[len - i + 0] = (val >> 24) & 0xFF; /* byte 0: bit 31~24 */
        ram[len - i + 1] = (val >> 16) & 0xFF; /* byte 1: bit 23~16 */
        ram[len - i + 2] = (val >> 8) & 0xFF;  /* byte 2: bit 15~8  */
        ram[len - i + 3] = (val) & 0xFF;       /* byte 3: bit 7~0   */
    }
    return;
}

static void drv_pke_set_mode(pke_mode mode, td_u32 len)
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
    ot_cipher_dbg("PKE_WORK_MODE 0x%x\n", work.u32);

    /* default: public key */
    pke_write(PKE_CFG_KEY_FLAG, PKE_KEY_CFG_PUBLIC);

    return;
}

static void drv_pke_start(void)
{
    pke_start start;

    func_enter();

    pke_write(PKE_CFG_KEY_FLAG, PKE_KEY_CFG_PUBLIC);

    start.u32 = 0x00;
    start.bits.pke_start = PKE_START_CODE;
    pke_write(PKE_START, start.u32);
    ot_cipher_dbg("PKE_START 0x%x\n", start.u32);

    func_exit();

    return;
}

static td_s32 drv_pke_wait_free(void)
{
    td_u32 i;
    pke_busy busy;

    func_enter();

    /* wait ready */
    for (i = 0; i < PKE_TIME_OUT; i++) {
        busy.u32 = pke_read(PKE_BUSY);
        if (!busy.bits.pke_busy) {
            break;
        }
        crypto_msleep(1);
    }

    if (i >= PKE_TIME_OUT) {
        ot_cipher_error("error, pke wait free timeout\n");
        print_errno(OT_ERR_CIPHER_TIMEOUT);
        return OT_ERR_CIPHER_TIMEOUT;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 drv_pke_error_code(void)
{
    pke_failure_flag code;
    pke_result_flag result;
    td_u32 i;

    func_enter();

    result.u32 = pke_read(PKE_RESULT_FLAG);
    if (result.bits.pke_result_flag != PKE_RESULT_FLAG_OK) {
        code.u32 = pke_read(PKE_FAILURE_FLAG);
        for (i = 0; i < PKE_FAIL_FLAG_CNT; i++) {
            if (code.bits.pke_failure_flag & (1 << i)) {
                ot_cipher_error("PKE operation failed: %s\n", err_string[i]);
            }
        }
        ot_cipher_error("Hardware Error Code: 0x%x\n", code.u32);
        g_pke_info.attacked = TD_TRUE;
        print_errno(OT_ERR_CIPHER_HARD_STATUS);
        return OT_ERR_CIPHER_HARD_STATUS;
    }

    func_exit();
    return TD_SUCCESS;
}

/* wait pke done */
static td_s32 drv_pke_wait_done(void)
{
    td_u32 int_valid = 0;
    td_u32 int_num = 0;
    td_u32 i;
    td_s32 ret;
    const char *name = TD_NULL;

    func_enter();

    module_get_attr(CRYPTO_MODULE_ID_PKE, &int_valid, &int_num, &name);

#ifdef CRYPTO_OS_INT_SUPPORT
    /* interrupt support, wait irq */
    if (int_valid) {
        /* wait interrupt */
        ret = crypto_queue_wait_timeout(&g_pke_info.queue, crypto_queue_callback_func, &g_pke_info.done, PKE_TIME_OUT);
        if (ret == 0x00) {
            ot_cipher_error("wait done timeout\n");
            print_func_errno(crypto_queue_wait_timeout, OT_ERR_CIPHER_TIMEOUT);
            ot_cipher_error("PKE_INT_ENABLE[0x%x]            0x%x\n", PKE_INT_ENABLE, pke_read(PKE_INT_ENABLE));
            ot_cipher_error("PKE_INT_STATUS[0x%x]            0x%x\n", PKE_INT_STATUS, pke_read(PKE_INT_STATUS));
            ot_cipher_error("pke PKE_INT_NOMASK_STATUS[0x%x] 0x%x\n",
                PKE_INT_NOMASK_STATUS, pke_read(PKE_INT_NOMASK_STATUS));

            return OT_ERR_CIPHER_TIMEOUT;
        }
    } else { /* interrupt unsupported, query the raw interrupt flag */
#endif
        for (i = 0; i < PKE_TIME_OUT; i++) {
            if (drv_pke_done_try()) {
                break;
            }
            if (i <= MS_TO_US) {
                crypto_udelay(1); /* short waiting for 1000 us */
            } else {
                crypto_msleep(1); /* long waiting for 5000 ms */
            }
        }

        if (i >= PKE_TIME_OUT) {
            ot_cipher_error("pke wait done timeout\n");
            print_func_errno(crypto_queue_wait_timeout, OT_ERR_CIPHER_TIMEOUT);
            return OT_ERR_CIPHER_TIMEOUT;
        }
#ifdef CRYPTO_OS_INT_SUPPORT
    }
#endif

    ret = drv_pke_error_code();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_pke_error_code, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

static void drv_pke_set_key(const td_u8 *inkey, td_u8 *outkey, td_u32 klen,
    const pke_block *random, td_u32 pad_len)
{
    td_u32 i;
    td_u16 crc16;

    if (random != TD_NULL) { /* private key */
        for (i = 0; i < klen; i++) {
            outkey[i] = inkey[i] ^ random->byte[i & PKE_LEN_BLOCK_MASK];
        }

        crc16 = drv_pke_crc16_random(0x0000, random);
        crc16 = drv_pke_crc16_key(crc16, outkey, klen);
        crc16 = drv_pke_crc16_padding(crc16, random, pad_len);

        ot_cipher_dbg("PKE_KEY_RANDOM_1 0x%x\n", random->word[1]);
        ot_cipher_dbg("PKE_KEY_RANDOM_2 0x%x\n", random->word[0]);
        ot_cipher_dbg("CRC16 0x%x\n", crc16);

        /* set Random */
        pke_write(PKE_KEY_RANDOM_1, crypto_cpu_to_be32(random->word[1]));
        pke_write(PKE_KEY_RANDOM_2, crypto_cpu_to_be32(random->word[0]));

        /* set src16 */
        pke_write(PKE_KEY_CRC, crc16);

        /* private key */
        pke_write(PKE_CFG_KEY_FLAG, PKE_KEY_CFG_PRIVATE);
    } else { /* not private key */
#ifdef CHIP_SM2_SUPPORT
        pke_write(PKE_CFG_KEY_FLAG, PKE_KEY_CFG_PUBLIC);

        if (memcpy_s(outkey, klen, inkey, klen) != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        }
#endif
    }

    /* select cpu key */
    pke_write(PKE_OTP_KEY_SEL, PKE_SECURE_FALSE);
}

#ifdef CHIP_SM2_SUPPORT
static void drv_pke_set_block(pke_ram_type type, td_u32 section,
    const pke_block *block, td_u32 power, td_u32 klen)
{
    td_u32 i;
    const td_u32 start = PKE_MRAM + PKE_RAM_RANG_SIZE * type + section * PKE_RAM_SECTION_SIZE_IN_BYTE;

    for (i = 0; i < klen; i += WORD_WIDTH) {
        pke_write(start + i, 0x00);
    }

    pke_write(start + power, block->word[1]);
    pke_write(start + power + WORD_WIDTH, block->word[0]);
}

/* c = a + b mod p */
td_s32 drv_pke_add_mod(const td_u8 *a, const td_u8 *b, const td_u8 *p, td_u8 *c, td_u32 klen)
{
    td_s32 ret;

    func_enter();

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);
    chk_ptr_err_return(a);
    chk_ptr_err_return(b);
    chk_ptr_err_return(p);
    chk_ptr_err_return(c);

    g_pke_info.done = TD_FALSE;

    ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 wait ready failed!\n");
        print_func_errno(drv_pke_wait_free, ret);
        return ret;
    }

    drv_pke_set_mode(PKE_MODE_ADD_MOD, klen);

    drv_pke_set_ram(PKE_RAM_TYPE_NRAM, 0, a, klen, TD_NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, 0, b, klen, TD_NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_RRAM, 0, p, klen, TD_NULL);

    drv_pke_start();

    ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 mod add failed\n");
        print_func_errno(drv_pke_wait_done, ret);
        return ret;
    }

    drv_pke_get_ram(PKE_RAM_TYPE_MRAM, 0, c, klen);

    func_exit();
    return TD_SUCCESS;
}

/* c = a - b mod p */
td_s32 drv_pke_sub_mod(const td_u8 *a, const td_u8 *b, const td_u8 *p, td_u8 *c, td_u32 klen)
{
    td_s32 ret;

    func_enter();

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);
    chk_ptr_err_return(a);
    chk_ptr_err_return(b);
    chk_ptr_err_return(p);
    chk_ptr_err_return(c);

    g_pke_info.done = TD_FALSE;

    ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 wait ready failed!\n");
        print_func_errno(drv_pke_wait_free, ret);
        return ret;
    }

    drv_pke_set_mode(PKE_MODE_SUB_MOD, klen);

    drv_pke_set_ram(PKE_RAM_TYPE_NRAM, 0, a, klen, TD_NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, 0, b, klen, TD_NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_RRAM, 0, p, klen, TD_NULL);

    drv_pke_start();

    ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 mod sub failed\n");
        print_func_errno(drv_pke_wait_done, ret);
        return ret;
    }

    drv_pke_get_ram(PKE_RAM_TYPE_MRAM, 0, c, klen);

    func_exit();
    return TD_SUCCESS;
}

/* c = a * b mod p */
td_s32 drv_pke_mul_mod(const td_u8 *a, const td_u8 *b, const td_u8 *p, td_u8 *c, td_u32 klen, td_u32 private)
{
    td_s32 ret;
    td_u8 *key = TD_NULL;
    pke_block random;

    func_enter();

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);
    chk_ptr_err_return(a);
    chk_ptr_err_return(b);
    chk_ptr_err_return(c);

    key = crypto_calloc(1, klen);
    if (key == TD_NULL) {
        print_func_errno(crypto_calloc, OT_ERR_CIPHER_FAILED_MEM);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    g_pke_info.done = TD_FALSE;

    ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 wait ready failed!\n");
        print_func_errno(drv_pke_wait_free, ret);
        crypto_free(key);
        return ret;
    }

    drv_pke_set_mode(PKE_MODE_MUL_MOD, klen);

    if (private == TD_TRUE) {
        random.word[0] = get_rand();
        random.word[1] = get_rand();
        drv_pke_set_key(b, key, klen, &random, 0x00);
    } else {
        random.word[0] = 0x00000000;
        random.word[1] = 0x00000000;
        drv_pke_set_key(b, key, klen, TD_NULL, 0x00);
    }

    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, 0, a, klen, TD_NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_NRAM, 0, p, klen, TD_NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, 0, key, klen, TD_NULL);

    drv_pke_start();

    ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 mod mul failed\n");
        print_func_errno(drv_pke_wait_done, ret);
        crypto_free(key);
        return ret;
    }

    drv_pke_get_ram(PKE_RAM_TYPE_RRAM, 0, c, klen);

    crypto_free(key);

    func_exit();
    return TD_SUCCESS;
}

/* c = a^-1 mod p */
td_s32 drv_pke_inv_mod(const td_u8 *a, const td_u8 *p, td_u8 *c, td_u32 klen)
{
    td_s32 ret;

    func_enter();

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);

    g_pke_info.done = TD_FALSE;

    ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 wait ready failed!\n");
        print_func_errno(drv_pke_wait_free, ret);
        return ret;
    }

    drv_pke_set_mode(PKE_MODE_MINV_MOD, klen);

    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, 0, a, klen, TD_NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, 0, p, klen, TD_NULL);

    drv_pke_start();

    ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 mod inv failed\n");
        print_func_errno(drv_pke_wait_done, ret);
        return ret;
    }

    drv_pke_get_ram(PKE_RAM_TYPE_RRAM, 0, c, klen);

    func_exit();
    return TD_SUCCESS;
}

/* c = a mod p */
td_s32 drv_pke_mod(const td_u8 *a, const td_u8 *p, td_u8 *c, td_u32 klen)
{
    td_s32 ret;

    func_enter();

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);
    chk_ptr_err_return(a);
    chk_ptr_err_return(p);
    chk_ptr_err_return(c);

    g_pke_info.done = TD_FALSE;

    ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 wait ready failed!\n");
        print_func_errno(drv_pke_wait_free, ret);
        return ret;
    }

    drv_pke_set_mode(PKE_MODE_MOD, klen);

    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, 0, a, klen, TD_NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, 0, p, klen, TD_NULL);

    drv_pke_start();

    ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 mod failed\n");
        print_func_errno(drv_pke_wait_done, ret);
        return ret;
    }

    drv_pke_get_ram(PKE_RAM_TYPE_RRAM, 0, c, klen);

    func_exit();
    return TD_SUCCESS;
}

/* c = a ^ mod p */
td_s32 drv_pke_mod_block(const pke_block *a, td_u32 power, const td_u8 *p, td_u32 plen, td_u8 *c)
{
    td_s32 ret;
    td_u32 klen;
    pke_block padding;

    func_enter();

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);
    chk_ptr_err_return(a);
    chk_ptr_err_return(p);
    chk_ptr_err_return(c);

    g_pke_info.done = TD_FALSE;

    ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 wait ready failed!\n");
        print_func_errno(drv_pke_wait_free, ret);
        return ret;
    }

    padding.word[0] = 0x00;
    padding.word[1] = 0x00;
    klen = crypto_max(power + PKE_LEN_BLOCK_IN_BYTE, plen);

    drv_pke_set_mode(PKE_MODE_MOD, klen);
    drv_pke_set_block(PKE_RAM_TYPE_MRAM, 0, a, power, klen);
    drv_pke_set_block(PKE_RAM_TYPE_KRAM, 0, &padding, 0, klen); /* clear KRAM */
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, 0, p, plen, TD_NULL);

    drv_pke_start();

    ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 mod failed\n");
        print_func_errno(drv_pke_wait_done, ret);
        return ret;
    }

    drv_pke_get_ram(PKE_RAM_TYPE_RRAM, 0, c, plen);

    func_exit();
    return TD_SUCCESS;
}

/* *** c = a * b *** */
td_s32 drv_pke_mul(const td_u8 *a, const td_u8 *b, td_u8 *c, td_u32 klen)
{
    td_s32 ret;

    func_enter();

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);
    chk_ptr_err_return(a);
    chk_ptr_err_return(b);
    chk_ptr_err_return(c);

    g_pke_info.done = TD_FALSE;

    ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 wait ready failed!\n");
        print_func_errno(drv_pke_wait_free, ret);
        return ret;
    }

    drv_pke_set_mode(PKE_MODE_MUL, klen);

    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, 0, a, klen, TD_NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_RRAM, 0, b, klen, TD_NULL);
    drv_pke_start();

    ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 mul failed\n");
        print_func_errno(drv_pke_wait_done, ret);
        return ret;
    }

    if (klen <= PKE_MUL_RESULT_LEVEL) {
        drv_pke_get_ram(PKE_RAM_TYPE_MRAM, 0, c, klen * 2); /* 2 klen for mul result */
    } else {
        drv_pke_get_ram(PKE_RAM_TYPE_MRAM, 0, c, PKE_MUL_RESULT_LEVEL);
        drv_pke_get_ram(PKE_RAM_TYPE_NRAM, 0, c + PKE_MUL_RESULT_LEVEL,
                        klen - PKE_MUL_RESULT_LEVEL);
    }

    func_exit();
    return TD_SUCCESS;
}

static td_void drv_pke_mul_dot_set_key(const td_u8 *k, td_u8 *key, td_u32 klen, td_u32 private,
    td_u32 pad_len, pke_block *random)
{
    if (private == TD_TRUE) {
        random->word[0] = get_rand();
        random->word[1] = get_rand();
        drv_pke_set_key(k, key, klen, random, pad_len);
    } else {
        random->word[0] = 0x00000000;
        random->word[1] = 0x00000000;
        drv_pke_set_key(k, key, klen, TD_NULL, pad_len);
    }
}

/* *** R = k . P(x,y) *** */
td_s32 drv_pke_mul_dot(const ecc_point_t *point_p, ecc_point_t *point_r,
    const td_u8 *k, td_u32 klen, td_u32 private, const ecc_param_t *ecp_id)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 *key = TD_NULL;
    td_u32 pad_len;
    td_u32 id;
    pke_block random;
    pke_block zero;

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);

    key = crypto_calloc(1, klen);
    if (key == TD_NULL) {
        print_func_errno(crypto_calloc, OT_ERR_CIPHER_FAILED_MEM);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    g_pke_info.done = TD_FALSE;

    chk_func_err_goto(drv_pke_wait_free());

    zero.word[0] = 0x00000000;
    zero.word[1] = 0x00000000;
    pad_len = PKE_RAM_SECTION_SIZE_IN_BYTE - klen;

    drv_pke_set_mode(PKE_MODE_MUL_DOT, klen);

    drv_pke_mul_dot_set_key(k, key, klen, private, pad_len, &random);

    id = 0;
    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, id++, point_p->x, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, id++, point_p->y, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, id++, ecp_id->g.x, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, id++, ecp_id->g.y, klen, &zero);

    id = 0;
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, id++, key, klen, &random);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, id++, ecp_id->n, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, id++, ecp_id->b, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, id++, ecp_id->a, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, id++, ecp_id->p, klen, &zero);

    drv_pke_start();

    chk_func_err_goto(drv_pke_wait_done());

    if (point_r->x != TD_NULL) {
        drv_pke_get_ram(PKE_RAM_TYPE_RRAM, 0, point_r->x, klen);
    }
    if (point_r->y != TD_NULL) {
        drv_pke_get_ram(PKE_RAM_TYPE_RRAM, 1, point_r->y, klen);
    }

exit__:
    crypto_free(key);
    return ret;
}

/* *** C = S(x,y) + R(x,y) *** */
td_s32 drv_pke_add_dot(const td_u8 *sx, const td_u8 *sy,
    const td_u8 *rx, const td_u8 *ry, td_u8 *cx, td_u8 *cy, td_u32 klen, const ecc_param_t *ecp_id)
{
    td_s32 ret;
    td_u32 id;
    pke_block zero;

    func_enter();

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);
    chk_ptr_err_return(sx);
    chk_ptr_err_return(sy);
    chk_ptr_err_return(rx);
    chk_ptr_err_return(ry);
    chk_ptr_err_return(cx);
    chk_ptr_err_return(cy);

    g_pke_info.done = TD_FALSE;

    ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 wait ready failed!\n");
        print_func_errno(drv_pke_wait_free, ret);
        return ret;
    }

    zero.word[0] = 0x00000000;
    zero.word[1] = 0x00000000;

    drv_pke_set_mode(PKE_MODE_ADD_DOT, klen);

    id = 0;
    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, id++, sx, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, id++, sy, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, id++, rx, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, id, ry, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, id++, ecp_id->a, klen, &zero);
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, id++, ecp_id->p, klen, &zero);

    drv_pke_start();

    ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
        ot_cipher_error("SM2 dot add failed\n");
        print_func_errno(drv_pke_wait_done, ret);
        return ret;
    }

    drv_pke_get_ram(PKE_RAM_TYPE_RRAM, 0, cx, klen);
    drv_pke_get_ram(PKE_RAM_TYPE_RRAM, 1, cy, klen);

    func_exit();
    return TD_SUCCESS;
}
#endif

td_s32 drv_pke_clean_ram(void)
{
    td_s32 ret;

    func_enter();

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);

    g_pke_info.done = TD_FALSE;

    ret = drv_pke_wait_free();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_pke_wait_free, ret);
        return ret;
    }

    drv_pke_set_mode(PKE_MODE_CLR_RAM, 0);
    drv_pke_start();

    ret = drv_pke_wait_done();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_pke_wait_done, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_u32 drv_rsa_get_klen(rsa_key_width width)
{
    td_u32 klen;

    switch (width) {
        case RSA_KEY_WIDTH_2048: {
            klen = RSA_KEY_LEN_2048;
            break;
        }
        case RSA_KEY_WIDTH_3072: {
            klen = RSA_KEY_LEN_3072;
            break;
        }
        case RSA_KEY_WIDTH_4096: {
            klen = RSA_KEY_LEN_4096;
            break;
        }
        default: {
            ot_cipher_error("error, nonsupport RSA width %d\n", width);
            klen = 0;
            break;
        }
    }

    return klen;
}

/* c=m^k mod n */
td_s32 drv_ifep_rsa_exp_mod(const td_u8 *n, const td_u8 *k,
    const td_u8 *in, td_u8 *out, rsa_key_width width)
{
    td_s32 ret;
    td_u8 *key = TD_NULL;
    pke_block random;
    td_u32 klen;

    chk_param_err_return((n == TD_NULL) || (k == TD_NULL) || (in == TD_NULL) || (out == TD_NULL));

    chk_init_err_return(g_pke_init);
    chk_param_err_return(g_pke_info.attacked);
    g_pke_info.done = TD_FALSE;

    klen = drv_rsa_get_klen(width);
    if (klen == 0) {
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    key = crypto_calloc(1, klen * 2); /* 2 * klen buffer for n and k */
    if (key == TD_NULL) {
        print_func_errno(crypto_calloc, OT_ERR_CIPHER_FAILED_MEM);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    ret = drv_pke_resume();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_pke_resume, ret);
        crypto_free(key);
        return ret;
    }

    chk_func_err_goto(drv_pke_wait_free());

    random.word[0] = get_rand();
    random.word[1] = get_rand();

    drv_pke_set_mode(PKE_MODE_EXP_MOD, klen);

    /* key xor random */
    if ((memcpy_s(key, klen * 2, k, klen) != EOK) || /* 2 * klen buffer for n and k */
        (memcpy_s(key + klen, klen, n, klen) != EOK)) {
        print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        ret = OT_ERR_CIPHER_FAILED_SEC_FUNC;
        goto exit__;
    }

    drv_pke_set_key(key, key, klen + klen, &random, 0);

    /* set data */
    drv_pke_set_ram(PKE_RAM_TYPE_MRAM, 0, in, klen, TD_NULL);
    drv_pke_set_ram(PKE_RAM_TYPE_NRAM, 0, key + klen, klen, TD_NULL); /* n */
    drv_pke_set_ram(PKE_RAM_TYPE_KRAM, 0, key, klen, TD_NULL);        /* k */

    /* start */
    drv_pke_start();
    chk_func_err_goto(drv_pke_wait_done());
    drv_pke_get_ram(PKE_RAM_TYPE_RRAM, 0, out, klen);

exit__:
    if (drv_pke_clean_ram() != TD_SUCCESS) {
        ot_cipher_error("clean ram failed\n");
    }

    drv_pke_suspend();
    crypto_free(key);
    return ret;
}

void drv_pke_get_capacity(pke_capacity *capacity)
{
    (void)memset_s(capacity, sizeof(pke_capacity), 0, sizeof(pke_capacity));

    capacity->sm2 = CRYPTO_CAPACITY_SUPPORT;
    capacity->ecc = CRYPTO_CAPACITY_SUPPORT;
    capacity->rsa = CRYPTO_CAPACITY_SUPPORT;

    return;
}

/** @} */ /** <!-- ==== API declaration end ==== */

#endif  // End of CHIP_PKE_VER_V100
