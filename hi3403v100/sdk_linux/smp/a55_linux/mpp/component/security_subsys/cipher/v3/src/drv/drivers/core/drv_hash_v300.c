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

#include "drv_hash_v300.h"
#include "drv_hash.h"

#ifdef CHIP_HASH_VER_V300

/*************************** Structure Definition ****************************/
/** \addtogroup     hash */
/** @{ */ /** <!-- [hash] */
/* ! hash node depth */
#define HASH_MAX_DEPTH (255)

/* ! hash in entry list size */
#define HASH_NODE_SIZE (sizeof(struct hash_entry_in) * HASH_MAX_DEPTH)

/* ! hash in entry list size */
#define HASH_NODE_LIST_SIZE (HASH_NODE_SIZE * HASH_HARD_CHANNEL_CNT)

#define SPACC_CPU_CUR          (crypto_is_sec_cpu() ? SPACC_CPU_TEE : SPACC_CPU_REE)
#define spacc_lock_chn(chn)    (SPACC_CPU_CUR << chn)
#define spacc_release_chn(chn) (SPACC_CPU_CUR << chn)

#define SMMU_PAGE_BIT          12

/* ! Define the flag of node */
typedef enum {
    HASH_CTRL_NONE = 0x00,          /* !<  middle node */
    HASH_CTRL_HASH_IN_FIRST = 0x01, /* !<  first node */
    HASH_CTRL_HASH_IN_LAST = 0x02,  /* !<  last node */
    HASH_CTRL_COUNT,
} hash_ctrl_en;

/* ! spacc digest in entry struct which is defined by hardware, you can't change it */
struct hash_entry_in {
    td_u32 spacc_cmd : 2;       /* !<  reserve */
    td_u32 rev1 : 14;           /* !<  reserve */
    td_u32 hmac_last_block : 1; /* !<  hash control flag */
    td_u32 rev2 : 15;           /* !<  reserve */
    td_u32 hash_alg_length;     /* !<  hash message length */
    td_u32 hash_start_addr_l;   /* !<  hash message address */
    td_u32 hash_start_addr_h;   /* !<  hash message address */
};

/* ! Define the context of hash */
typedef struct {
    hash_mode hash_alg;             /* !<  hash mode */
    struct hash_entry_in *entry_in; /* ! spacc digest in entry struct */
    td_u32 id_in;                   /* !< current hash nodes to be used */
    td_u32 id_cnt;                  /* !< current hash nodes to be used */
    td_u32 done;                    /* !<  calculation finish flag */
    CRYPTO_QUEUE_HEAD queue;        /* !<  queue list */
} hash_hard_context;

/* ! hash already initialize or not */
static td_u32 g_hash_initialize = TD_FALSE;

/* ! dma memory of hash node list */
static crypto_mem g_hash_dma;

/* ! Channel of hash */
static channel_context g_hash_hard_channel[CRYPTO_HARD_CHANNEL_MAX];

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      hash */
/** @{ */ /** <!--[hash] */

#ifdef CRYPTO_OS_INT_SUPPORT
static td_u32 hash_done_notify(void)
{
    hash_chann_int last_status;
    hash_chann_raw_int last_raw;
    ista_in_hash_node_cnt level_status;
    iraw_in_hash_node_cnt level_raw;
    spacc_is hash_is;
    td_u32 mask;

    hash_is.u32 = symc_read(SPACC_IS);
    if (SPACC_CPU_CUR == SPACC_CPU_TEE) {
        mask = hash_is.bits.int_cal_ctrl_tee;
    } else {
        mask = hash_is.bits.int_cal_ctrl_ree;
    }
    if (mask == 0) {
        return 0; /* there is no last int or level int */
    }

    /* read last int for the valid channel then clear interception */
    last_status.u32 = 0;
    last_status.u32 = hash_read(HASH_CHANN_INT);
    last_status.bits.hash_chann_int &= HASH_HARD_CHANNEL_MASK;
    hash_write(HASH_CHANN_RAW_INT, last_status.u32);

    /* read level int for the valid channel then clear interception */
    level_status.u32 = 0;
    level_status.u32 = hash_read(ISTA_IN_HASH_NODE_CNT);
    level_status.bits.ista_in_hash_node_cnt &= HASH_HARD_CHANNEL_MASK;
    hash_write(IRAW_IN_HASH_NODE_CNT, level_status.u32);

    level_raw.u32 = hash_read(IRAW_IN_HASH_NODE_CNT);
    last_raw.u32 = hash_read(HASH_CHANN_RAW_INT);
    crypto_unused(level_raw);
    crypto_unused(last_raw);

    /* it's needn't to distinguish last and level int,
     * the isr function will distinguish them by the left of nodes.
     */
    mask = last_status.bits.hash_chann_int | level_status.bits.ista_in_hash_node_cnt;

    return mask; /* mask */
}

static td_u32 symc_done_test(void)
{
    out_sym_chan_int interruption;
    ista_in_sym_node_cnt status;

    interruption.u32 = hash_read(OUT_SYM_CHAN_LAST_NODE_INT);
    status.u32 = hash_read(ISTA_IN_SYM_NODE_CNT);

    /* just process the valid channel */
    interruption.bits.out_sym_chan_int &= CIPHER_HARD_CHANNEL_MASK;
    status.bits.ista_in_sym_node_cnt &= CIPHER_HARD_CHANNEL_MASK;

    return (interruption.bits.out_sym_chan_int | status.bits.ista_in_sym_node_cnt); /* mask */
}

/* ! hash interrupt process function */
static CRYPTO_IRQRETURN_T hash_interrupt_isr(td_s32 irq, td_void *dev_id)
{
    td_u32 mask;
    td_u32 i;
    hash_hard_context *ctx = TD_NULL;
    CRYPTO_IRQRETURN_T ret = OSAL_IRQ_HANDLED;

    crypto_unused(dev_id);
    crypto_unused(irq);

    mask = hash_done_notify();

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((mask >> i) & 0x01) {
            ctx = (hash_hard_context *)g_hash_hard_channel[i].ctx;
            ctx->done = TD_TRUE;
            ot_cipher_dbg("chn %d wake up\n", i);
            crypto_queue_wait_up(&ctx->queue);
        }
    }

    /* symc and hash use the sample interrupt number
     * so if symc has occur interrupt, we should return OSAL_IRQ_NONE
     * to tell system continue to process the symc interrupt.
 */
    if (symc_done_test() != 0) {
        ret = OSAL_IRQ_NONE;
    }

    return ret;
}

/* ! hash register interrupt process function */
static td_s32 drv_hash_register_interrupt(void)
{
    td_s32 ret;
    td_u32 int_valid = 0;
    td_u32 int_num = 0;
    td_u32 i;
    const char *name = TD_NULL;
    hash_hard_context *ctx = TD_NULL;

    func_enter();

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, &name);

    /* request irq */
    ot_cipher_dbg("hash request irq, num %d, name %s\n", int_num, name);
    ret = crypto_request_irq(int_num, hash_interrupt_isr, name);
    if (ret != TD_SUCCESS) {
        ot_cipher_error("Irq request failure, irq = %d", int_num);
        print_errno(OT_ERR_CIPHER_REGISTER_IRQ);
        return OT_ERR_CIPHER_REGISTER_IRQ;
    }

    /* initialize queue list */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        ctx = g_hash_hard_channel[i].ctx;
        crypto_queue_init(&ctx->queue);
    }

    func_exit();
    return TD_SUCCESS;
}

/* ! hash unregister interrupt process function */
static void drv_hash_unregister_interrupt(void)
{
    td_u32 int_valid = 0;
    td_u32 int_num = 0;
    td_u32 i;
    const char *name = TD_NULL;
    hash_hard_context *ctx = TD_NULL;

    func_enter();

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, &name);

    /* free irq */
    ot_cipher_dbg("hash free irq, num %d, name %s\n", int_num, name);
    crypto_free_irq(int_num, name);

    /* destroy wait queue */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        ctx = g_hash_hard_channel[i].ctx;
        crypto_queue_deinit(&ctx->queue);
    }

    func_exit();

    return;
}

/* ! set interrupt */
static void hash_set_interrupt(void)
{
    td_u32 int_valid = 0;
    td_u32 int_num = 0;
    const char *name = TD_NULL;
    hash_chann_raw_int_en hash_fin_int_en;
    hash_chann_raw_int hash_fin_int_raw;
    iena_in_hash_node_cnt hash_level_int_en;
    iraw_in_hash_node_cnt hash_level_int_raw;

    func_enter();

    module_get_attr(CRYPTO_MODULE_ID_HASH, &int_valid, &int_num, &name);

    if (int_valid == TD_FALSE) {
#ifndef CRYPTO_OS_INT_SUPPORT
        hash_fin_int_en.u32 = hash_read(HASH_CHANN_RAW_INT_EN);
        hash_fin_int_en.bits.hash_chann_int_en &= (~HASH_HARD_CHANNEL_MASK);
        hash_write(HASH_CHANN_RAW_INT_EN, hash_fin_int_en.u32);
        ot_cipher_dbg("HASH_CHANN_RAW_INT_EN[0x%x]: 0x%x\n", HASH_CHANN_RAW_INT_EN, hash_fin_int_en.u32);

        hash_level_int_en.u32 = hash_read(IENA_IN_HASH_NODE_CNT);
        hash_level_int_en.bits.iena_in_hash_node_cnt &= (~HASH_HARD_CHANNEL_MASK);
        hash_write(IENA_IN_HASH_NODE_CNT, hash_level_int_en.u32);
        ot_cipher_dbg("IENA_IN_HASH_NODE_CNT[0x%x]: 0x%x\n", IENA_IN_HASH_NODE_CNT, hash_level_int_en.u32);
#endif
    } else {
        /* clear interception
         * the history of interception may trigge the system to
         * call the irq function before initialization
         * when register interrupt, this will cause a system abort.
         */
        hash_fin_int_raw.u32 = hash_read(HASH_CHANN_RAW_INT);
        hash_fin_int_raw.bits.hash_chann_raw_int &= HASH_HARD_CHANNEL_MASK; /* clear valid channel */
        hash_write(HASH_CHANN_RAW_INT, hash_fin_int_raw.u32);
        ot_cipher_dbg("HASH_CHANN_RAW_INT[0x%x]: 0x%x\n", HASH_CHANN_RAW_INT, hash_fin_int_raw.u32);

        hash_level_int_raw.u32 = hash_read(IRAW_IN_HASH_NODE_CNT);
        hash_level_int_raw.bits.iraw_in_hash_node_cnt &= HASH_HARD_CHANNEL_MASK; /* clear valid channel */
        hash_write(IRAW_IN_HASH_NODE_CNT, hash_level_int_raw.u32);
        ot_cipher_dbg("IRAW_IN_HASH_NODE_CNT[0x%x]: 0x%x\n", IRAW_IN_HASH_NODE_CNT, hash_level_int_raw.u32);

        hash_fin_int_en.u32 = hash_read(HASH_CHANN_RAW_INT_EN);
        hash_fin_int_en.bits.hash_chann_int_en |= HASH_HARD_CHANNEL_MASK;
        hash_write(HASH_CHANN_RAW_INT_EN, hash_fin_int_en.u32);
        ot_cipher_dbg("HASH_INT_EN[0x%x]: 0x%x\n", HASH_CHANN_RAW_INT_EN, hash_fin_int_en.u32);

        hash_level_int_en.u32 = hash_read(IENA_IN_HASH_NODE_CNT);
        hash_level_int_en.bits.iena_in_hash_node_cnt |= HASH_HARD_CHANNEL_MASK;
        hash_write(IENA_IN_HASH_NODE_CNT, hash_level_int_en.u32);
        ot_cipher_dbg("HASH_CHANN_RAW_INT_EN[0x%x]: 0x%x\n", IENA_IN_HASH_NODE_CNT, hash_level_int_en.u32);
    }

    func_exit();

    return;
}
#endif

#ifndef CRYPTO_OS_INT_SUPPORT
static td_u32 hash_done_try(td_u32 chn_num)
{
    hash_chann_raw_int hash_fin_int_raw;
    iraw_in_hash_node_cnt level_raw;
    td_u32 chn_mask;

    hash_fin_int_raw.u32 = hash_read(HASH_CHANN_RAW_INT);
    hash_fin_int_raw.bits.hash_chann_raw_int &= 0x01 << chn_num;
    hash_write(HASH_CHANN_RAW_INT, hash_fin_int_raw.u32);

    level_raw.u32 = hash_read(IRAW_IN_HASH_NODE_CNT);
    level_raw.bits.iraw_in_hash_node_cnt &= 0x01 << chn_num;
    hash_write(IRAW_IN_HASH_NODE_CNT, level_raw.u32);

    chn_mask = hash_fin_int_raw.bits.hash_chann_raw_int | level_raw.bits.iraw_in_hash_node_cnt;

    return chn_mask;
}
#endif

static td_s32 hash_clear_channel(td_u32 chn_num)
{
    td_u32 j;
    td_u32 clear_int;

    ot_cipher_dbg("hash_clear_channel, chn %d!\n", chn_num);

    hash_write(SPACC_HASH_CHN_CLEAR_REQ, 0x01 << chn_num);
    for (j = 0; j < CRYPTO_TIME_OUT; j++) {
        clear_int = hash_read(SPACC_INT_RAW_HASH_CLEAR_FINISH);
        if ((0x01 << chn_num) & clear_int) {
            hash_write(SPACC_INT_RAW_HASH_CLEAR_FINISH, 0x01 << chn_num);
            break;
        }

        if (j <= MS_TO_US) {
            crypto_udelay(1); /* short waiting for 1000 us */
        } else {
            crypto_msleep(1); /* long waiting for 5000 ms */
        }
    }

    if (j >= CRYPTO_TIME_OUT) {
        ot_cipher_error("clear channel[%d] failed!\n", chn_num);
        return OT_ERR_CIPHER_TIMEOUT;
    }

    return TD_SUCCESS;
}

/* ! set hash entry */
static void hash_set_entry(td_u32 chn, compat_addr dma_addr, const td_void *cpu_addr)
{
    hash_hard_context *ctx = (hash_hard_context *)g_hash_hard_channel[chn].ctx;
    in_hash_chn_node_length hash_node_len;
    in_hash_chn_node_wr_point hash_wr_ptr;
    in_hash_chn_ctrl hash_chn_ctrl;

    /* set total num and start addr for hash in node */
    hash_node_len.u32 = hash_read(in_hash_chn_node_length(chn));
    hash_node_len.bits.hash_chn_node_length = HASH_MAX_DEPTH;
    hash_write(in_hash_chn_node_length(chn), hash_node_len.u32);
    hash_write(in_hash_chn_node_start_addr_l(chn), addr_l32(dma_addr));
    hash_write(in_hash_chn_node_start_addr_h(chn), addr_h32(dma_addr));
    ot_cipher_dbg("in_hash_chn_node_length[0x%x]: 0x%x\n", in_hash_chn_node_length(chn), hash_node_len.u32);
    ot_cipher_dbg("in_hash_chn_node_start_addr_l[0x%x]: 0x%x\n", in_hash_chn_node_start_addr_l(chn),
        addr_l32(dma_addr));
    ot_cipher_dbg("in_hash_chn_node_start_addr_h[0x%x]: 0x%x\n", in_hash_chn_node_start_addr_h(chn),
        addr_h32(dma_addr));

    hash_chn_ctrl.u32 = hash_read(in_hash_chn_ctrl(chn));
    hash_chn_ctrl.bits.hash_chn_en = 0x01; /* enable channel */
    hash_chn_ctrl.bits.hash_chn_ss = crypto_is_sec_cpu() ? 0x01 : 0x00; /* use sec or nsec table */
    hash_write(in_hash_chn_ctrl(chn), hash_chn_ctrl.u32);
    ot_cipher_dbg("in_hash_chn_ctrl[0x%x]: 0x%x\n", in_hash_chn_ctrl(chn), hash_chn_ctrl.u32);

    hash_wr_ptr.u32 = hash_read(in_hash_chn_node_wr_point(chn));
    ctx->entry_in = (struct hash_entry_in *)cpu_addr;
    ctx->id_in = hash_wr_ptr.bits.hash_chn_node_wr_point;
    ot_cipher_dbg("in_hash_chn_node_wr_point[0x%x]: 0x%x\n", in_hash_chn_node_wr_point(chn), hash_wr_ptr.u32);

    return;
}

/* ! set smmu */
static void hash_smmu_bypass(td_u32 chn, td_u32 enable)
{
#ifdef CRYPTO_SMMU_SUPPORT
    in_hash_chn_ctrl in_ctrl;

    func_enter();

    in_ctrl.u32 = hash_read(in_hash_chn_ctrl(chn));
    in_ctrl.bits.hash_chn_mmu_en = enable ? 1 : 0;
    hash_write(in_hash_chn_ctrl(chn), in_ctrl.u32);

    ot_cipher_dbg("in_hash_chn_ctrl[0x%x]  :0x%x\n", in_hash_chn_ctrl(chn), in_ctrl.u32);

    func_exit();
#else
    crypto_unused(chn);
    crypto_unused(enable);
#endif
}

/* ! smmu set base address */
static void drv_hash_smmu_base_addr(void)
{
#ifdef CRYPTO_SMMU_SUPPORT
    td_ulong err_raddr = 0;
    td_ulong err_waddr = 0;
    td_ulong table_addr = 0;

    /* get table base addr from system api */
    smmu_get_table_addr(&err_raddr, &err_waddr, &table_addr);

    /* without tee, ree can use sec or nsec smmu table
       depend on in_hash_chn_ctrl.hash_chn_ss.
       use sec table when in_hash_chn_ctrl.hash_chn_ss = 1,
       use nsec table when in_hash_chn_ctrl.hash_chn_ss = 0,
       there ree fix use nosec table
    */
    symc_write(SPACC_MMU_NOSEC_TLB, (td_u32)table_addr >> 4); /* bit[35:4]: mmu base address, bit[3:0] fixed 0 */
    symc_write(SPACC_MMU_IN_HASH_NOSEC_EADDR_H, (td_u32)(err_raddr >> 32)); /* high 32 bit of addr */
    symc_write(SPACC_MMU_IN_HASH_NOSEC_EADDR_L, (td_u32)(err_raddr));       /* low 32 bit of addr */
    ot_cipher_dbg("SPACC_MMU_NOSEC_TLB[0x%x]  : 0x%x\n", SPACC_MMU_NOSEC_TLB, (td_u32)table_addr);
#endif

    return;
}

/* ! set secure channel,
 *  non-secure CPU can't change the value of SEC_CHN_CFG,
 *  so non-secure CPU call this function will do nothing.
 */
static td_s32 drv_hash_enable_secure(void)
{
    td_s32 ret;
    td_u32 used;
    td_u32 chnn_who_used;
    td_u32 mask;

    func_enter();

    mask = crypto_is_sec_cpu() ? SPACC_CPU_TEE : SPACC_CPU_REE;
    used = hash_read(SPACC_HASH_CHN_LOCK);

    /* try to use this channel */
    chnn_who_used = chn_who_used_get(used, HASH_HARD_CHANNEL);
    if (chnn_who_used != SPACC_CPU_IDLE) {
        ot_cipher_error("chn[%d] aleardy be used by cpu 0x%x.\n", HASH_HARD_CHANNEL, chnn_who_used);
        return OT_ERR_CIPHER_BUSY;
    }

    chn_who_used_set(used, HASH_HARD_CHANNEL, mask);
    hash_write(SPACC_HASH_CHN_LOCK, used);

    /* check if the channal aleardy be useded by other cpu
     * if other cpu break-in when write the CIPHER_NON_SEC_CHN_WHO_USED
     * the value of CIPHER_NON_SEC_CHN_WHO_USED will be channged
     */
    used = hash_read(SPACC_HASH_CHN_LOCK);
    chnn_who_used = chn_who_used_get(used, HASH_HARD_CHANNEL);
    ot_cipher_dbg("SPACC_HASH_CHN_LOCK[0x%x] 0x%x\n", SPACC_HASH_CHN_LOCK, used);
    if (chnn_who_used != mask) {
        ot_cipher_error("chn[%d] aleardy be used by cpu 0x%x.\n", HASH_HARD_CHANNEL, chnn_who_used);
        return OT_ERR_CIPHER_BUSY;
    }

    ret = hash_clear_channel(HASH_HARD_CHANNEL);
    if (TD_SUCCESS != ret) {
        print_func_errno(hash_clear_channel, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 drv_hash_get_err_code(td_u32 chn_num)
{
    tee_hash_calc_ctrl_check_err code;
    tee_hash_calc_ctrl_check_err_status status;
    struct hash_entry_in *in = TD_NULL;
    hash_hard_context *ctx = TD_NULL;

    /* read error code */
    code.u32 = hash_read(hash_calc_ctrl_check_err(SPACC_CPU_CUR));
    status.u32 = hash_read(hash_calc_ctrl_check_err_status(SPACC_CPU_CUR));

    if (code.bits.tee_hash_check_alg_error) {
        ot_cipher_error("hash error: hash_check_alg_error, chn %d !!!\n", chn_num);
    }
    if (code.bits.tee_hash_check_alg_invld_error) {
        ot_cipher_error("hash error: hash_check_alg_invld_error, chn %d !!!\n", chn_num);
    }
    if (code.bits.tee_hash_check_tee_error) {
        ot_cipher_error("hash error: hash_check_tee_error, chn %d !!!\n", chn_num);
    }
    if (code.bits.tee_hash_check_sc_error) {
        ot_cipher_error("symc error: hash_check_sc_error, chn %d !!!\n", chn_num);
    }
    if (code.bits.tee_hash_check_hmac_lock_error) {
        ot_cipher_error("symc error: hash_check_hmac_lock_error, chn %d !!!\n", chn_num);
    }

    /* print the in out buffer address */
    if (code.u32) {
        ctx = (hash_hard_context *)g_hash_hard_channel[chn_num].ctx;
        /* get last in node info */
        if (ctx->id_in == 0x00) {
            in = &ctx->entry_in[HASH_MAX_DEPTH];
        } else {
            in = &ctx->entry_in[ctx->id_in - 1];
        }
        ot_cipher_dbg("chn %d, src addr 0x%x%x, size 0x%x\n",
            chn_num, in->hash_start_addr_h, in->hash_start_addr_l, in->hash_alg_length);

        /* clear error code */
        status.bits.tee_hash_error_code_clr = 1;
        hash_write(hash_calc_ctrl_check_err_status(SPACC_CPU_CUR), status.u32);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    crypto_unused(in);
    return TD_SUCCESS;
}

static td_s32 drv_hash_check_nsec_mmu(td_void)
{
    td_s32 ret = TD_SUCCESS;
    spacc_mmu_in_hash_nosec_unvld_va_tab va_tab;
    spacc_mmu_in_hash_nosec_unvld_pa_tab pa_tab;

    /* read error code */
    va_tab.u32 = hash_read(SPACC_MMU_IN_HASH_NOSEC_UNVLD_VA_TAB);
    pa_tab.u32 = hash_read(SPACC_MMU_IN_HASH_NOSEC_UNVLD_PA_TAB);

    if (va_tab.bits.mmu_in_hash_nosec_unvld_va_vld == 1) {
        ot_cipher_error("mmu_in_hash_nosec_unvld_va_vld:\n");
        ot_cipher_error("mmu_in_hash_nosec_unvld_chn_id %d !!!\n", va_tab.bits.mmu_in_hash_nosec_unvld_chn_id);
        ot_cipher_error("mmu_in_hash_nosec_unvld_va_tab 0x%x !!!\n",
            va_tab.bits.mmu_in_hash_nosec_unvld_va_tab << SMMU_PAGE_BIT);
        ot_cipher_error("SPACC_MMU_NOSEC_TLB[0x%x] 0x%x\n", SPACC_MMU_NOSEC_TLB, symc_read(SPACC_MMU_NOSEC_TLB));
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }
    if (pa_tab.bits.mmu_in_hash_nosec_unvld_pa_vld == 1) {
        ot_cipher_error("mmu_in_hash_nosec_unvld_pa_vld:\n");
        ot_cipher_error("mmu_in_hash_nosec_unvld_vld_err %d !!!\n", pa_tab.bits.mmu_in_hash_nosec_unvld_vld_err);
        ot_cipher_error("mmu_in_hash_nosec_unvld_pa_tab 0x%x !!!\n",
            pa_tab.bits.mmu_in_hash_nosec_unvld_pa_tab << SMMU_PAGE_BIT);
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }

    /* clear error code */
    symc_write(SPACC_MMU_IN_HASH_SEC_UNVLD_VA_TAB, va_tab.u32);
    symc_write(SPACC_MMU_IN_HASH_SEC_UNVLD_PA_TAB, pa_tab.u32);

    return ret;
}

static td_s32 drv_hash_check_sec_mmu(td_void)
{
    td_s32 ret = TD_SUCCESS;
    spacc_mmu_in_hash_sec_unvld_va_tab va_tab;
    spacc_mmu_in_hash_sec_unvld_pa_tab pa_tab;

    /* read error code */
    va_tab.u32 = hash_read(SPACC_MMU_IN_HASH_SEC_UNVLD_VA_TAB);
    pa_tab.u32 = hash_read(SPACC_MMU_IN_HASH_SEC_UNVLD_PA_TAB);

    if (va_tab.bits.mmu_in_hash_sec_unvld_va_vld == 1) {
        ot_cipher_error("mmu_in_hash_sec_unvld_va_vld:\n");
        ot_cipher_error("mmu_in_hash_sec_unvld_chn_id %d !!!\n", va_tab.bits.mmu_in_hash_sec_unvld_chn_id);
        ot_cipher_error("mmu_in_hash_sec_unvld_va_tab 0x%x !!!\n",
            va_tab.bits.mmu_in_hash_sec_unvld_va_tab << SMMU_PAGE_BIT);
        ot_cipher_error("SPACC_MMU_SEC_TLB[0x%x] 0x%x\n", SPACC_MMU_SEC_TLB, symc_read(SPACC_MMU_SEC_TLB));
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }
    if (pa_tab.bits.mmu_in_hash_sec_unvld_pa_vld == 1) {
        ot_cipher_error("mmu_in_hash_sec_unvld_pa_vld:\n");
        ot_cipher_error("mmu_in_hash_sec_unvld_vld_err %d !!!\n", pa_tab.bits.mmu_in_hash_sec_unvld_vld_err);
        ot_cipher_error("mmu_in_hash_sec_unvld_pa_tab 0x%x !!!\n",
            pa_tab.bits.mmu_in_hash_sec_unvld_pa_tab << SMMU_PAGE_BIT);
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }

    /* clear error code */
    symc_write(SPACC_MMU_IN_HASH_SEC_UNVLD_VA_TAB, va_tab.u32);
    symc_write(SPACC_MMU_IN_HASH_SEC_UNVLD_PA_TAB, pa_tab.u32);

    return ret;
}

static void hash_enrty_init(crypto_mem mem)
{
    td_u32 i;
    compat_addr mmz_addr;
    td_void *cpu_addr = TD_NULL;

    addr_u64(mmz_addr) = addr_u64(mem.dma_addr);
    cpu_addr = mem.dma_virt;

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((HASH_HARD_CHANNEL_MASK >> i) & 0x01) { /* valid channel */
            hash_set_entry(i, mmz_addr, cpu_addr);
            addr_u64(mmz_addr) += HASH_NODE_SIZE;          /* move to next channel */
            cpu_addr = (td_u8 *)cpu_addr + HASH_NODE_SIZE; /* move to next channel */
        }
    }

    return;
}

static td_void hash_entry_deinit(td_void)
{
    td_u32 used;

    used = hash_read(SPACC_HASH_CHN_LOCK);
    chn_who_used_clr(used, HASH_HARD_CHANNEL);
    hash_write(SPACC_HASH_CHN_LOCK, used);

    return;
}

static td_void drv_hash_low_power(td_void)
{
    spacc_calc_crg_cfg cfg;

    cfg.u32 = symc_read(SPACC_CALC_CRG_CFG);
    cfg.bits.spacc_sha1_low_power_enable  = HASH_LOW_POWER_ENABLE;
    cfg.bits.spacc_sha2_low_power_enable  = HASH_LOW_POWER_ENABLE;
    cfg.bits.spacc_sm3_low_power_enable   = HASH_LOW_POWER_ENABLE;
    symc_write(SPACC_CALC_CRG_CFG, cfg.u32);
}

td_s32 drv_hash_init(void)
{
    td_s32 ret;

    func_enter();

    if (g_hash_initialize == TD_TRUE) {
        func_exit();
        return TD_SUCCESS;
    }

    ret = crypto_channel_init(g_hash_hard_channel, CRYPTO_HARD_CHANNEL_MAX, sizeof(hash_hard_context));
    if (ret != TD_SUCCESS) {
        print_func_errno(crypto_channel_init, ret);
        return ret;
    }

    module_enable(CRYPTO_MODULE_ID_HASH);

    /* alloc memory for nodes list */
    ret = crypto_mem_create(&g_hash_dma, MEM_TYPE_MMZ, "cipher_sha_node", HASH_NODE_LIST_SIZE);
    if (ret != TD_SUCCESS) {
        print_func_errno(crypto_mem_create, ret);
        goto __error1;
    }
    ot_cipher_dbg("HASH DMA buffer, MMZ 0x%x, VIA %pK, size 0x%x\n",
        addr_l32(g_hash_dma.dma_addr), g_hash_dma.dma_virt, g_hash_dma.dma_size);

    /* hash secure channel configure */
    ret = drv_hash_enable_secure();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_hash_enable_secure, ret);
        crypto_mem_destroy(&g_hash_dma);
        goto __error1;
    }

    /* hash entry list configure */
    hash_enrty_init(g_hash_dma);

    /* hash SMMU configure */
    hash_smmu_bypass(HASH_HARD_CHANNEL, TD_TRUE);
    drv_hash_smmu_base_addr();
    drv_hash_low_power();

#ifdef CRYPTO_OS_INT_SUPPORT
    /* hash interrupt configure */
    hash_set_interrupt();

    ret = drv_hash_register_interrupt();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_hash_register_interrupt, ret);
        crypto_mem_destroy(&g_hash_dma);
        hash_entry_deinit();
        goto __error1;
    }
#endif

    g_hash_initialize = TD_TRUE;
    func_exit();
    return TD_SUCCESS;

__error1:
    module_disable(CRYPTO_MODULE_ID_HASH);
    crypto_channel_deinit(g_hash_hard_channel, CRYPTO_HARD_CHANNEL_MAX);

    return ret;
}

td_s32 drv_hash_deinit(void)
{
    func_enter();

    if (g_hash_initialize == TD_FALSE) {
        func_exit();
        return TD_SUCCESS;
    }

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_hash_unregister_interrupt();
#endif

    hash_entry_deinit();
    crypto_mem_destroy(&g_hash_dma);
    module_disable(CRYPTO_MODULE_ID_HASH);
    crypto_channel_deinit(g_hash_hard_channel, CRYPTO_HARD_CHANNEL_MAX);
    g_hash_initialize = TD_FALSE;

    func_exit();
    return TD_SUCCESS;
}

void drv_hash_resume(void)
{
    td_u32 i;

    func_enter();

    ot_cipher_dbg("enable hash\n");
    module_enable(CRYPTO_MODULE_ID_HASH);

    ot_cipher_dbg("hash entry list configure\n");
    hash_enrty_init(g_hash_dma);

#ifdef CRYPTO_OS_INT_SUPPORT
    ot_cipher_dbg("hash interrupt configure\n");
    hash_set_interrupt();
#endif

    ot_cipher_dbg("hash SMMU configure\n");
    for (i = 0; i < HASH_HARD_CHANNEL_CNT; i++) {
        /* check the valid channel */
        if (HASH_HARD_CHANNEL_MASK & (0x1 << i)) {
            hash_smmu_bypass(i, TD_TRUE);
        }
    }
    drv_hash_smmu_base_addr();

    ot_cipher_dbg("hash secure channel configure\n");
    if (drv_hash_enable_secure() != TD_SUCCESS) {
        ot_cipher_error("Failed call drv_hash_enable_secure.\n");
    }

    func_exit();

    return;
}

/* wait hash ready */
static td_s32 drv_hash_wait_ready(td_u32 chn_num)
{
#ifdef CRYPTO_OS_INT_SUPPORT
    hash_hard_context *ctx = TD_NULL;
#endif
    func_enter();

    chk_param_err_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

#ifdef CRYPTO_OS_INT_SUPPORT
    ctx = (hash_hard_context *)g_hash_hard_channel[chn_num].ctx;

    /* wait interrupt */
    if (crypto_queue_wait_timeout(&ctx->queue,
        crypto_queue_callback_func, &ctx->done, CRYPTO_TIME_OUT) == 0) {
        drv_hash_get_err_code(chn_num);
        print_func_errno(crypto_queue_wait_timeout, OT_ERR_CIPHER_TIMEOUT);
        return OT_ERR_CIPHER_TIMEOUT;
    }
#else
    td_u32 i;
    for (i = 0; i < CRYPTO_TIME_OUT; i++) {
        if (hash_done_try(chn_num)) {
            break;
        }
        if (i <= MS_TO_US) {
            crypto_udelay(1); /* short waiting for 1000 us */
        } else {
            crypto_msleep(1); /* long waiting for 5000 ms */
        }
    }
    if (i >= CRYPTO_TIME_OUT) {
        drv_hash_get_err_code(chn_num);
        print_errno(OT_ERR_CIPHER_TIMEOUT);
        return OT_ERR_CIPHER_TIMEOUT;
    }
#endif

    func_exit();
    return TD_SUCCESS;
}

static void hash_addbuf(td_u32 chn_num, compat_addr buf_phy, td_u32 buf_size)
{
    hash_hard_context *ctx = TD_NULL;
    td_u32 id;
    td_u32 size;
    td_void *addr = TD_NULL;

    ctx = (hash_hard_context *)g_hash_hard_channel[chn_num].ctx;

    /* clean in entry */
    id = ctx->id_in++;
    addr = &ctx->entry_in[id];
    size = sizeof(struct hash_entry_in);

    (td_void)memset_s(addr, size, 0, size);

    /* set addr and length */
    ctx->entry_in[id].spacc_cmd = HASH_CTRL_HASH_IN_FIRST | HASH_CTRL_HASH_IN_LAST;
    ctx->entry_in[id].hash_start_addr_l = addr_l32(buf_phy);
    ctx->entry_in[id].hash_start_addr_h = addr_h32(buf_phy) & 0xf;
    ctx->entry_in[id].hash_alg_length = buf_size;
    ctx->id_in %= HASH_MAX_DEPTH;
    ot_cipher_dbg("add digest in buf: id %d, addr 0x%x, len 0x%x\n", id, addr_l32(buf_phy), buf_size);

    return;
}

td_s32 drv_hash_cfg(td_u32 chn_num, hash_mode mode, const td_u32 state[HASH_RESULT_MAX_SIZE_IN_WORD])
{
    hash_hard_context *ctx = TD_NULL;
    in_hash_chn_key_ctrl hash_key_ctrl;
    td_u32 hash_mode_cfg_val[] = {0x0, 0x0, 0x1, 0x2, 0x3, 0x0}; /* null 224 256 384 512 null */
    td_u32 hash_alg_cfg_val[] = {0xa, 0xb, 0xb, 0xb, 0xb, 0xc};  /* null 224 256 384 512 null */
    td_u32 i;

    chk_param_err_return(g_hash_initialize != TD_TRUE);
    chk_param_err_return(((HASH_HARD_CHANNEL_MASK >> chn_num) & 0x01) == 0);

    ctx = (hash_hard_context *)g_hash_hard_channel[chn_num].ctx;
    ctx->hash_alg = mode;

    /* Control */
    hash_key_ctrl.u32 = hash_read(in_hash_chn_key_ctrl(chn_num));
    hash_key_ctrl.bits.hash_chn_alg_mode = hash_mode_cfg_val[mode];
    hash_key_ctrl.bits.hash_chn_alg_sel = hash_alg_cfg_val[mode];
    hash_write(in_hash_chn_key_ctrl(chn_num), hash_key_ctrl.u32);
    ot_cipher_dbg("in_hash_chn_key_ctrl: 0x%X\n", hash_key_ctrl.u32);

    /* Write last state */
    for (i = 0; i < HASH_RESULT_MAX_SIZE_IN_WORD; i++) {
        hash_write(chn_n_hash_state_val_addr(chn_num), i);
        hash_write(chn_n_hash_state_val(chn_num), state[i]);
    }

    return TD_SUCCESS;
}

static td_void drv_hash_check_resume(td_void)
{
    td_u32 base;

    func_enter();

#ifdef CRYPTO_SMMU_SUPPORT
    if (crypto_is_sec_cpu()) {
        base = symc_read(SPACC_MMU_SEC_TLB);
    } else {
        base = symc_read(SPACC_MMU_NOSEC_TLB);
    }
#else
    base = symc_read(in_hash_chn_node_start_addr_l(HASH_HARD_CHANNEL));
#endif
    if (base == 0) {
        /* smmu base address is zero means hardware be unexpected reset */
        ot_cipher_warn("hash module is not ready, try to resume it now...\n");
        drv_hash_resume();
    }

    func_exit();
    return;
}

td_s32 drv_hash_start(td_u32 chn_num, const crypto_mem *mem, td_u32 length)
{
    in_hash_chn_node_wr_point in_node_wr_ptr;
    spacc_mmu_in_hash_clear clear;
    hash_hard_context *ctx = TD_NULL;
    td_u32 ptr;

    func_enter();

    ctx = (hash_hard_context *)g_hash_hard_channel[chn_num].ctx;

    if (length == 0) {
        return TD_SUCCESS;
    }

    drv_hash_check_resume();

    /* channel clear mmu page (SW set this bit 1 and HW clean it to be 0) */
    clear.u32 = hash_read(SPACC_MMU_IN_HASH_CLEAR);
    clear.bits.sw_mmu_in_hash_clr = chn_num << 1;
    hash_write(SPACC_MMU_IN_HASH_CLEAR, clear.u32);

    ctx->done = TD_FALSE;
    /* set message addr and length */
    hash_addbuf(chn_num, mem->dma_addr, length);

    /* configure in-node, only compute one nodes */
    in_node_wr_ptr.u32 = hash_read(in_hash_chn_node_wr_point(chn_num));
    ptr = in_node_wr_ptr.bits.hash_chn_node_wr_point + 1;
    in_node_wr_ptr.bits.hash_chn_node_wr_point = ptr % HASH_MAX_DEPTH;

    /* The cache must be flushed when the calculation starts. */
    crypto_flush_dcache_all();

    /* Start */
    hash_write(in_hash_chn_node_wr_point(chn_num), in_node_wr_ptr.u32);
    ot_cipher_dbg("in_hash_chn_node_wr_point[0x%x]: 0x%X\n", in_hash_chn_node_wr_point(chn_num), in_node_wr_ptr.u32);

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_hash_wait_done(td_u32 chn_num, td_u32 *state, td_u32 len_in_word)
{
    td_u32 i;
    td_s32 ret;

    func_enter();

    if (len_in_word < HASH_RESULT_MAX_SIZE_IN_WORD) {
        ot_cipher_error("error, state len err, len_in_word=%d", len_in_word);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    ret = drv_hash_wait_ready(chn_num);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_hash_wait_ready, ret);
        return ret;
    }

    ret = drv_hash_check_nsec_mmu();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_hash_check_nsec_mmu, ret);
        return ret;
    }

    ret = drv_hash_check_sec_mmu();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_hash_check_sec_mmu, ret);
        return ret;
    }

    /* read hash result */
    for (i = 0; i < HASH_RESULT_MAX_SIZE_IN_WORD; i++) {
        hash_write(chn_n_hash_state_val_addr(chn_num), i);
        state[i] = hash_read(chn_n_hash_state_val(chn_num));
    }

    func_exit();
    return TD_SUCCESS;
}

void drv_hash_reset(td_u32 chn_num)
{
    crypto_unused(chn_num);
}

void drv_hash_get_capacity(hash_capacity *capacity)
{
    (td_void)memset_s(capacity, sizeof(hash_capacity), 0, sizeof(hash_capacity));

    capacity->sha256 = CRYPTO_CAPACITY_SUPPORT;
    capacity->sha384 = CRYPTO_CAPACITY_SUPPORT;
    capacity->sha512 = CRYPTO_CAPACITY_SUPPORT;
    capacity->sm3 = CRYPTO_CAPACITY_SUPPORT;

    return;
}

/** @} */ /** <!-- ==== API declaration end ==== */

#endif  // End of CHIP_HASH_VER_V200
