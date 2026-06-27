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

#include "drv_symc_v300.h"
#include "drv_symc.h"

#ifdef CHIP_SYMC_VER_V300

/* Length of padding buffer */
#define SYMC_PAD_BUFFER_LEN             128

/* Length of aes ccm/gcm key */
#define AES_CCM_GCM_KEY_LEN             16

/* Total Length of padding buffer */
#define SYMC_PAD_BUFFER_TOTAL_LEN       (SYMC_PAD_BUFFER_LEN * CIPHER_HARD_CHANNEL_CNT)

#define SYMC_CFG_SECURE     0x05
#define SYMC_CFG_NON_SECURE 0x0a

#define SMMU_PAGE_BIT       12
#define VALID_SIGNAL        1

/* current cpu */
#define SPACC_CPU_CUR (crypto_is_sec_cpu() ? SPACC_CPU_TEE : SPACC_CPU_REE)

/* spacc symc int entry struct which is defined by hardware, you can't change it */
typedef struct {
    td_u32 sym_first_node : 1;         /* !<  Indicates whether the node is the first node */
    td_u32 sym_last_node : 1;          /* !<  Indicates whether the node is the last node */
    td_u32 rev1 : 7;                   /* !<  reserve */
    td_u32 odd_even : 1;               /* !<  Indicates whether the key is an odd key or even key */
    td_u32 rev2 : 6;                   /* !<  reserve */
    td_u32 sym_gcm_first : 1;          /* !<  Indicates whether the node is the first node of first list,only for GCM */
    td_u32 sym_ccm_last : 1;           /* !<  Indicates whether the node is the last node of last list ,only for CCM */
    td_u32 aes_ccm_gcm_in_flag : 2;    /* !<  Indicates GCM/CCM flag: CCM[2'b0:N, 2'b1:A, 2'b10:P] ;
                                              GCM[2'b0:A, 2'b1:P, 2'b10:LEN] */
    td_u32 rev3 : 4;                   /* !<  reserve */
    td_u32 sym_gcm_and_ctr_iv_len : 5; /* !<  GCM: Indicates iv length; CTR:Bit width of the current counter,
                                              The length of the IV is 128 bits. */
    td_u32 rev4 : 3;                   /* !<  reserve */
    td_u32 sym_alg_length;             /* !<  syma data length */
    td_u32 sym_start_addr;             /* !<  syma start low addr */
    td_u32 sym_start_high;             /* !<  syma start high addr */
    td_u32 iv[AES_IV_SIZE / WORD_WIDTH]; /* !<  symc IV */
    td_u32 cenc_enable : 1;            /* !<  cenc enable */
    td_u32 rev5 : 7;                   /* !<  reserve */
    td_u32 cenc_head : 1;              /* !<  Indicates whether the head data */
    td_u32 rev6 : 23;                  /* !<  reserve */
    td_u32 e_num : 8;                  /* !<  Number of blocks in ciphertext in a pattern. */
    td_u32 rev7 : 24;                  /* !<  reserve */
    td_u32 c_num : 8;                  /* !<  Number of blocks in plain text in a pattern. */
    td_u32 rev8 : 24;                  /* !<  reserve */
    td_u32 cenc_offset : 13;           /* !<  Length of the non-complete loop in the head of node */
    td_u32 rev9 : 19;                  /* !<  reserve */
    td_u32 rev10;                      /* !<  reserve */
    td_u32 rev11;                      /* !<  reserve */
    td_u32 rev12;                      /* !<  reserve */
    td_u32 rev13;                      /* !<  reserve */
} symc_entry_in;

/* spacc symc out entry struct which is defined by hardware, you can't change it */
typedef struct {
    td_u32 rev1;           /* !<  reserve */
    td_u32 sym_alg_length; /* !<  syma data length */
    td_u32 sym_start_addr; /* !<  syma start low addr */
    td_u32 sym_start_high; /* !<  syma start high addr */
} symc_entry_out;

/* Define the context of cipher */
typedef struct {
    td_u32 open;                        /* open or close */
    td_u32 who_used;                    /* !<  TEE(sec) or REE(nosec) or HPP used */
    symc_entry_in *entry_in;            /* in node list */
    symc_entry_out *entry_out;          /* out node list */
    compat_addr dma_entry;              /* dma addr of node */
    compat_addr dma_pad;                /* dma addr of padding buffer, for CCM/GCM */
    td_u8 *via_pad;                     /* via addr of padding buffer, for CCM/GCM */
    td_u32 offset_pad;                  /* offset of padding buffer, for CCM/GCM */
    td_u32 iv[SYMC_IV_MAX_SIZE_IN_WORD];    /* iv data from user. */

    /* iv usage flag, should be OT_CIPHER_IV_CHG_ONE_PACK
     * or OT_CIPHER_IV_CHG_ALL_PACK.
     */
    td_u32 iv_flag;                     /* iv flag */
    td_u32 iv_len;                      /* iv length */
    symc_alg alg;                       /* The alg of Symmetric cipher */
    symc_mode mode;                     /* mode */
    td_u32 key_src;                     /* key source */
    td_u32 id_in;                       /* current in nodes index */
    td_u32 id_out;                      /* current out nodes index */
    td_u32 cnt_in;                      /* total count in nodes to be computed */
    td_u32 cnt_out;                     /* total count out nodes to be computed */
    td_u32 done;                        /* calculation finish flag. */
    CRYPTO_QUEUE_HEAD queue;            /* queue list */
    callback_symc_isr callback;         /* isr callback function */
    callback_symc_destroy destroy;      /* destroy callback function */
    td_void *ctx;                       /* params for isr callback function */
} symc_hard_context;

/* spacc symc_chn_who_used struct which is defined by hardware, you can't change it */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32    non_sec_chn_who_used : 16; /* [15..0]  */
        td_u32    sec_chn1_who_used     : 2; /* [17..16] */
        td_u32    reserved              : 22; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    td_u32        u32;
} symc_chn_who_used;

/* ! Channel of cipher */
static symc_hard_context *g_hard_context = TD_NULL;

/* ! dma memory of cipher node list */
static crypto_mem g_symc_dma;

/* dma padding memory of cipher */
static crypto_mem g_padding_dma;

/* ! symc already initialize or not */
static td_u32 g_symc_initialize = TD_FALSE;

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Code *****************************/
/** \addtogroup      cipher drivers */
/** @{ */ /** <!-- [cipher] */

#ifdef CRYPTO_OS_INT_SUPPORT
static td_u32 drv_symc_done_notify(void)
{
    out_sym_chan_int last_status;
    out_sym_chan_raw_int last_raw;
    out_sym_chan_raw_level_int level_status;
    out_sym_chan_raw_level_int level_raw;
    sym_chann_int tag_status;
    sym_chann_raw_int tag_raw;
    spacc_is symc_is;
    td_u32 mask;

    symc_is.u32 = symc_read(SPACC_IS);
    ot_cipher_dbg("SPACC_IS[0x%x]: 0x%x\n", SPACC_IS, symc_is.u32);
    if (SPACC_CPU_CUR == SPACC_CPU_TEE) {
        mask = symc_is.bits.int_out_ctrl_tee | symc_is.bits.int_cal_ctrl_tee;
    } else {
        mask = symc_is.bits.int_out_ctrl_ree | symc_is.bits.int_cal_ctrl_ree;
    }
    if (mask == 0) {
        return 0; /* there is no last int or level int */
    }

    /* read last int for the valid channel then clear interception */
    last_raw.u32 = 0;
    last_status.u32 = symc_read(OUT_SYM_CHAN_LAST_NODE_INT);
    last_status.bits.out_sym_chan_int &= CIPHER_HARD_CHANNEL_MASK;
    last_raw.bits.out_sym_chan_raw_int = last_status.bits.out_sym_chan_int;
    symc_write(OUT_SYM_CHAN_RAW_LAST_NODE_INT, last_raw.u32);
    ot_cipher_dbg("OUT_SYM_CHAN_LAST_NODE_INT[0x%x]: 0x%x\n", OUT_SYM_CHAN_LAST_NODE_INT, last_status.u32);

    /* read level int for the valid channel then clear interception */
    level_raw.u32 = 0;
    level_status.u32 = symc_read(OUT_SYM_CHAN_LEVEL_INT);
    level_status.bits.out_sym_level_chan_raw_int &= CIPHER_HARD_CHANNEL_MASK;
    level_raw.bits.out_sym_level_chan_raw_int = level_status.bits.out_sym_level_chan_raw_int;
    symc_write(OUT_SYM_CHAN_RAW_LEVEL_INT, level_raw.u32);
    ot_cipher_dbg("OUT_SYM_CHAN_LEVEL_INT[0x%x]: 0x%x\n", OUT_SYM_CHAN_LEVEL_INT, level_status.u32);

    /* read tag int for the valid channel then clear interception */
    tag_raw.u32 = 0;
    tag_status.u32 = symc_read(SYM_CHANN_INT);
    tag_status.bits.sym_chann_int &= CIPHER_HARD_CHANNEL_MASK;
    tag_raw.bits.sym_chann_raw_int = tag_status.bits.sym_chann_int;
    symc_write(SYM_CHANN_RAW_INT, tag_raw.u32);
    ot_cipher_dbg("SYM_CHANN_RAW_INT[0x%x]: 0x%x for ccm/gcm tag\n", SYM_CHANN_RAW_INT, tag_status.u32);

    /* it's needn't to distinguish last and level int,
     * the isr function will distinguish them by the left of nodes.
     */
    mask = last_status.bits.out_sym_chan_int |
        level_status.bits.out_sym_level_chan_raw_int | tag_status.bits.sym_chann_int;

    return mask;
}

static td_u32 drv_hash_done_test(void)
{
    hash_chann_int interruption;
    ista_in_hash_node_cnt status;
    td_u32 chn_mask;

    interruption.u32 = symc_read(HASH_CHANN_INT);
    status.u32 = symc_read(ISTA_IN_HASH_NODE_CNT);

    /* just process the valid channel */
    interruption.bits.hash_chann_int &= HASH_HARD_CHANNEL_MASK;
    status.bits.ista_in_hash_node_cnt &= HASH_HARD_CHANNEL_MASK;
    chn_mask = interruption.bits.hash_chann_int | status.bits.ista_in_hash_node_cnt;

    return chn_mask;
}

/* ! symc interrupt process function */
static CRYPTO_IRQRETURN_T drv_symc_interrupt_isr(td_s32 irq, td_void *dev_id)
{
    td_u32 mask;
    td_u32 i;
    td_s32 finish;
    symc_hard_context *ctx = TD_NULL;
    CRYPTO_IRQRETURN_T ret = OSAL_IRQ_HANDLED;

    crypto_unused(irq);
    crypto_unused(dev_id);

    /* get channel context */
    mask = drv_symc_done_notify();
    /* without channel 0 */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if (((mask >> i) & 0x01) == 0) {
            continue;
        }

        /* process irq for channel i */
        ctx = &g_hard_context[i];
        if (ctx->callback != TD_NULL) {
            finish = ctx->callback(ctx->ctx);
            if (finish == TD_FALSE) {
                ot_cipher_dbg("continue to compute chn %d\n", i);
                continue;
            }
        }

        if (ctx->done == TD_FALSE) {
            ctx->done = TD_TRUE;
            ot_cipher_dbg("chn %d wake up\n", i);
            crypto_queue_wait_up(&ctx->queue);
            continue;
        }
    }

    /* symc and hash use the sample interrupt number
     * so if hash has occur interrupt, we should return OSAL_IRQ_NONE
     * to tell system continue to process the hash interrupt.
     */
    if (drv_hash_done_test() != 0) {
        ret = OSAL_IRQ_NONE;
    }

    return ret;
}

/* ! symc register interrupt process function */
static td_s32 drv_symc_register_interrupt(void)
{
    td_s32 ret;
    td_u32 int_valid = 0;
    td_u32 int_num = 0;
    td_u32 i;
    const char *name = TD_NULL;

    func_enter();

    module_get_attr(CRYPTO_MODULE_ID_SYMC, &int_valid, &int_num, &name);

    /* request irq */
    ot_cipher_dbg("symc request irq, num %d, name %s\n", int_num, name);
    ret = crypto_request_irq(int_num, drv_symc_interrupt_isr, name);
    if (ret != TD_SUCCESS) {
        ot_cipher_error("Irq request failure, irq = %d, name %s", int_num, name);
        print_errno(OT_ERR_CIPHER_REGISTER_IRQ);
        return ret;
    }

    /* initialize queue list */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        crypto_queue_init(&g_hard_context[i].queue);
    }

    func_exit();
    return TD_SUCCESS;
}

/* ! symc unregister interrupt process function */
static void drv_symc_unregister_interrupt(void)
{
    td_u32 int_valid = 0;
    td_u32 int_num = 0;
    const char *name = TD_NULL;
    td_u32 i;

    func_enter();

    module_get_attr(CRYPTO_MODULE_ID_SYMC, &int_valid, &int_num, &name);

    /* free irq */
    ot_cipher_dbg("symc free irq, num %d, name %s\n", int_num, name);
    crypto_free_irq(int_num, name);

    /* destroy wait queue */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        crypto_queue_deinit(&g_hard_context[i].queue);
    }

    func_exit();

    return;
}

/* ! set interrupt */
static void drv_symc_set_interrupt(td_u32 chn_num)
{
    out_sym_chan_raw_level_int_en out_level_int_en;
    out_sym_chan_raw_level_int out_level_int_raw;
    out_sym_chan_raw_int_en out_sym_int_en;
    out_sym_chan_raw_int out_sym_raw;
    sym_chann_raw_int_en tag_int_en;
    sym_chann_raw_int tag_raw;
    td_u32 mask;

    func_enter();

    mask = 1 << chn_num;

    /* clear interception
     * the history of interception may trigge the system to
     * call the irq function before initialization
     * when register interrupt, this will cause a system abort.
     */
    out_level_int_raw.u32 = symc_read(OUT_SYM_CHAN_RAW_LEVEL_INT);
    out_level_int_raw.bits.out_sym_level_chan_raw_int &= mask;
    symc_write(OUT_SYM_CHAN_RAW_LEVEL_INT, out_level_int_raw.u32);

    out_sym_raw.u32 = symc_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT);
    out_sym_raw.bits.out_sym_chan_raw_int &= mask;
    symc_write(OUT_SYM_CHAN_RAW_LAST_NODE_INT, out_sym_raw.u32);

    tag_raw.u32 = symc_read(SYM_CHANN_RAW_INT);
    tag_raw.bits.sym_chann_raw_int &= mask;
    symc_write(SYM_CHANN_RAW_INT, tag_raw.u32);

    /* enable interrupt */
    out_sym_int_en.u32 = symc_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN);
    out_sym_int_en.bits.out_sym_chan_int_en |= mask;
    symc_write(OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN, out_sym_int_en.u32);
    ot_cipher_dbg("OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN[0x%x]: 0x%x\n",
        OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN, out_sym_int_en.u32);

    /* now don't use level interrupt, may be use it in the future. */
    out_level_int_en.u32 = symc_read(OUT_SYM_CHAN_RAW_LEVEL_INT_EN);
    out_level_int_en.bits.out_sym_level_chan_int_en = 0;
    symc_write(OUT_SYM_CHAN_RAW_LEVEL_INT_EN, out_level_int_en.u32);
    ot_cipher_dbg("OUT_SYM_CHAN_RAW_LEVEL_INT_EN[0x%x]: 0x%x\n",
        OUT_SYM_CHAN_RAW_LEVEL_INT_EN, out_level_int_en.u32);

    tag_int_en.u32 = symc_read(SYM_CHANN_RAW_INT_EN);
    tag_int_en.bits.sym_chann_int_en |= mask;
    symc_write(SYM_CHANN_RAW_INT_EN, tag_int_en.u32);
    ot_cipher_dbg("SYM_CHANN_RAW_INT_EN[0x%x]: 0x%x\n", SYM_CHANN_RAW_INT_EN, tag_int_en.u32);

    func_exit();

    return;
}

/* ! set interrupt */
static void drv_symc_interrupt_init(td_void)
{
    spacc_ie cipher_int_en;

    func_enter();

    /* Enable interrupt */
    cipher_int_en.u32 = symc_read(SPACC_IE);
    cipher_int_en.bits.spacc_ie_ree = 1;
    cipher_int_en.bits.spacc_ie_tee = 1;
    symc_write(SPACC_IE, cipher_int_en.u32);
    ot_cipher_dbg("SPACC_IE[0x%x]: 0x%x\n", SPACC_IE, cipher_int_en.u32);

    func_exit();

    return;
}
#endif

#ifndef CRYPTO_OS_INT_SUPPORT
static td_s32 drv_symc_done_try(td_u32 chn_num)
{
    out_sym_chan_raw_int last_raw;
    out_sym_chan_raw_level_int level_raw;
    sym_chann_raw_int tag_raw;
    td_u32 mask;

    chk_param_err_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    last_raw.u32 = symc_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT);
    last_raw.bits.out_sym_chan_raw_int &= 0x01 << (chn_num); /* check interception */
    symc_write(OUT_SYM_CHAN_RAW_LAST_NODE_INT, last_raw.u32);

    level_raw.u32 = symc_read(OUT_SYM_CHAN_RAW_LEVEL_INT);
    level_raw.bits.out_sym_level_chan_raw_int &= 0x01 << (chn_num); /* check interception */
    symc_write(OUT_SYM_CHAN_RAW_LEVEL_INT, level_raw.u32);

    tag_raw.u32 = symc_read(SYM_CHANN_RAW_INT);
    tag_raw.bits.sym_chann_raw_int &= 0x01 << (chn_num);
    symc_write(SYM_CHANN_RAW_INT, tag_raw.u32);

    mask = last_raw.bits.out_sym_chan_raw_int | tag_raw.bits.sym_chann_raw_int;
    if (mask == 0) {
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}
#endif

static td_void drv_symc_set_dma(td_u32 chn_num, td_u32 dma_enable)
{
    in_sym_out_ctrl cipher_dma_ctrl;

    cipher_dma_ctrl.u32 = symc_read(in_sym_out_ctrl(chn_num));
    cipher_dma_ctrl.bits.sym_dma_copy = dma_enable;
    symc_write(in_sym_out_ctrl(chn_num), cipher_dma_ctrl.u32);
    ot_cipher_dbg("in_sym_out_ctrl[0x%x]  :0x%x\n", in_sym_out_ctrl(chn_num), cipher_dma_ctrl.u32);

    return;
}

/* ! set smmu */
static td_void drv_symc_smmu_bypass(td_u32 chn)
{
    in_sym_chn_ctrl in_ctrl;
    out_sym_chn_ctrl out_ctrl;

    func_enter();

    in_ctrl.u32 = symc_read(in_sym_chn_ctrl(chn));
    out_ctrl.u32 = symc_read(out_sym_chn_ctrl(chn));

#ifdef CRYPTO_SMMU_SUPPORT
    in_ctrl.bits.sym_chn_mmu_en      = 0x01;
    out_ctrl.bits.out_sym_chn_mmu_en = 0x01;
#else
    in_ctrl.bits.sym_chn_mmu_en      = 0x00;
    out_ctrl.bits.out_sym_chn_mmu_en = 0x00;
#endif

    symc_write(in_sym_chn_ctrl(chn), in_ctrl.u32);
    ot_cipher_dbg("in_sym_chn_ctrl[0x%x]  :0x%x\n", in_sym_chn_ctrl(chn), in_ctrl.u32);

    symc_write(out_sym_chn_ctrl(chn), out_ctrl.u32);
    ot_cipher_dbg("out_sym_chn_ctrl[0x%x] :0x%x\n", out_sym_chn_ctrl(chn), out_ctrl.u32);

    func_exit();
}

/* ! smmu set base address */
static void drv_symc_smmu_base_addr(void)
{
#ifdef CRYPTO_SMMU_SUPPORT
    td_ulong err_raddr = 0;
    td_ulong err_waddr = 0;
    td_ulong table_addr = 0;

    func_enter();

    /* get table base addr from system api */
    smmu_get_table_addr(&err_raddr, &err_waddr, &table_addr);
    if (crypto_is_sec_cpu()) {
        symc_write(SPACC_MMU_SEC_TLB, (td_u32)table_addr >> 4);             /* bit[35:4]: mmu base address */
        symc_write(SPACC_MMU_IN_SYM_SEC_EADDR_H, (td_u32)(err_raddr >> 32));  /* high 32 bit of addr */
        symc_write(SPACC_MMU_IN_SYM_SEC_EADDR_L, (td_u32)(err_raddr));        /* low 32 bit of addr */
        symc_write(SPACC_MMU_OUT_SYM_SEC_EADDR_H, (td_u32)(err_waddr >> 32)); /* high 32 bit of addr */
        symc_write(SPACC_MMU_OUT_SYM_SEC_EADDR_L, (td_u32)(err_waddr));       /* low 32 bit of addr */
        ot_cipher_dbg("SPACC_MMU_SEC_TLB[0x%x]  : 0x%llx\n", SPACC_MMU_SEC_TLB, table_addr);
    } else {
        symc_write(SPACC_MMU_NOSEC_TLB, (td_u32)table_addr >> 4);                /* bit[35:4]: mmu base address */
        symc_write(SPACC_MMU_IN_SYM_NOSEC_EADDR_H, (td_u32)(err_raddr >> 32));  /* high 32 bit of addr */
        symc_write(SPACC_MMU_IN_SYM_NOSEC_EADDR_L, (td_u32)(err_raddr));        /* low 32 bit of addr */
        symc_write(SPACC_MMU_OUT_SYM_NOSEC_EADDR_H, (td_u32)(err_waddr >> 32)); /* high 32 bit of addr */
        symc_write(SPACC_MMU_OUT_SYM_NOSEC_EADDR_L, (td_u32)(err_waddr));       /* low 32 bit of addr */
        ot_cipher_dbg("SPACC_MMU_NOSEC_TLB[0x%x]  : 0x%llx\n", SPACC_MMU_NOSEC_TLB, table_addr);
    }
#endif

    func_exit();
    return;
}

/* ! set symc entry */
static td_s32 drv_symc_clear_entry(td_u32 chn_num)
{
    td_u32 i;
    td_u32 timeout = CRYPTO_TIME_OUT;
    td_u32 mask;
    spacc_sym_chn_clear_req symc_chn_clear;
    spacc_int_raw_sym_clear_finish symc_chn_clear_finish;
    spacc_calc_inc_cfg calc_inc_cfg;

    func_enter();

    mask = 0x1 << chn_num;
    symc_chn_clear.u32 = symc_read(SPACC_SYM_CHN_CLEAR_REQ);
    symc_chn_clear.bits.sym_chn_clear_req |= mask;
    symc_write(SPACC_SYM_CHN_CLEAR_REQ, symc_chn_clear.u32);

    calc_inc_cfg.u32 = symc_read(SPACC_CALC_INC_CFG);
    calc_inc_cfg.bits.spacc_gcm_incr_disable = 1;
    calc_inc_cfg.bits.spacc_ccm_incr_disable = 1;
    symc_write(SPACC_CALC_INC_CFG, calc_inc_cfg.u32);

    for (i = 0; i < timeout; i++) {
        symc_chn_clear_finish.u32 = symc_read(SPACC_INT_RAW_SYM_CLEAR_FINISH);
        symc_chn_clear_finish.bits.int_raw_sym_clear_finish &= mask;
        if (symc_chn_clear_finish.bits.int_raw_sym_clear_finish != 0) {
            symc_write(SPACC_INT_RAW_SYM_CLEAR_FINISH, symc_chn_clear.u32);
            break;
        }
        if (i <= MS_TO_US) {
            crypto_udelay(1); /* short waiting for 1000 us */
        } else {
            crypto_msleep(1); /* long waiting for 5000 ms */
        }
    }
    if (timeout <= i) {
        ot_cipher_error("symc wait clear channel timeout, chn=%d\n", chn_num);
        print_errno(OT_ERR_CIPHER_TIMEOUT);
        return OT_ERR_CIPHER_TIMEOUT;
    }

    func_exit();
    return TD_SUCCESS;
}

/* ! set symc entry */
static td_s32 drv_symc_recover_entry(td_u32 chn)
{
    in_sym_chn_node_wr_point cipher_in_cfg;
    out_sym_chn_node_wr_point cipher_out_cfg;
    in_sym_chn_node_length cipher_in_depth;
    out_sym_chn_node_length cipher_out_depth;
    in_sym_chn_key_ctrl cipher_key_ctrl;
    symc_hard_context *ctx = &g_hard_context[chn];
    compat_addr out_addr;
    td_u32 entry;
    td_s32 ret;

    /* Disable DMA_COPY */
    drv_symc_set_dma(chn, 0x0);
    ret = drv_symc_clear_entry(chn);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_clear_entry, ret);
        return ret;
    }

    /* set total num and start addr for cipher in node
     * On ree, the chn may be seized by tee,
     * so we must check it, that is check we can write the reg of chn or not.
     */
    symc_write(in_sym_chn_node_start_addr_l(chn), addr_l32(ctx->dma_entry));
    symc_write(in_sym_chn_node_start_addr_h(chn), addr_h32(ctx->dma_entry));
    entry = symc_read(in_sym_chn_node_start_addr_l(chn));
    if (entry != addr_l32(ctx->dma_entry)) {
        ot_cipher_error("the chn %d - entry: 0x%x, dma_entry: 0x%x.\n", chn, entry, addr_l32(ctx->dma_entry));
        return OT_ERR_CIPHER_UNAVAILABLE;
    }
    cipher_in_cfg.u32 = symc_read(in_sym_chn_node_wr_point(chn));
    ctx->id_in = cipher_in_cfg.bits.sym_chn_node_wr_point;
    ot_cipher_dbg("symc chn %d recover, id in 0x%x, IN_NODE_START_ADDR 0x%x%08x, VIA %pK\n",
        chn, ctx->id_in, addr_h32(ctx->dma_entry), addr_l32(ctx->dma_entry), ctx->entry_in);

    /* set total num and start addr for cipher out node */
    cipher_out_cfg.u32 = symc_read(out_sym_chn_node_wr_point(chn));
    addr_u64(out_addr) = addr_u64(ctx->dma_entry) + SYMC_IN_NODE_SIZE;
    symc_write(out_sym_chn_node_start_addr_l(chn), addr_l32(out_addr));
    symc_write(out_sym_chn_node_start_addr_h(chn), addr_h32(out_addr));
    ctx->id_out = cipher_out_cfg.bits.out_sym_chn_node_wr_point;
    ot_cipher_dbg("symc chn %d recover, id out 0x%x, OUT_NODE_START_ADDR 0x%x%08x, VIA %pK\n",
        chn, ctx->id_out, addr_h32(out_addr), addr_l32(out_addr), ctx->entry_out);

    cipher_in_depth.u32 = symc_read(in_sym_chn_node_length(chn));
    cipher_in_depth.bits.sym_chn_node_length = SYMC_MAX_LIST_NUM;
    symc_write(in_sym_chn_node_length(chn), cipher_in_depth.u32);

    cipher_out_depth.u32 = symc_read(out_sym_chn_node_length(chn));
    cipher_out_depth.bits.out_sym_chn_node_length = SYMC_MAX_LIST_NUM;
    symc_write(out_sym_chn_node_length(chn), cipher_out_depth.u32);

    /* default key seclect */
    cipher_key_ctrl.u32 = symc_read(in_sym_chn_key_ctrl(chn));
    cipher_key_ctrl.bits.sym_key_chn_id = chn;
    symc_write(in_sym_chn_key_ctrl(chn), cipher_key_ctrl.u32);

    drv_symc_smmu_bypass(chn);

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_symc_set_interrupt(chn);
#endif
    return TD_SUCCESS;
}

/* ! set symc entry */
static void drv_symc_set_entry(td_u32 chn, compat_addr dma_addr, const td_void *cpu_addr)
{
    symc_hard_context *ctx = &g_hard_context[chn];

    func_enter();

    /* set total num and start addr for cipher in node */
    ot_cipher_dbg("CHAN[%-2d] IN Node: PHY 0x%x, VIA %pK\n", chn, addr_l32(dma_addr), cpu_addr);
    ctx->entry_in = (symc_entry_in *)cpu_addr;
    ctx->cnt_in = 0;

    addr_u64(dma_addr) += SYMC_IN_NODE_SIZE;
    cpu_addr = (u8 *)cpu_addr + SYMC_IN_NODE_SIZE;
    /* set total num and start addr for cipher out node */
    ot_cipher_dbg("CHAN[%-2d] OUT Node: PHY 0x%x, VIA %pK\n", chn, addr_l32(dma_addr), cpu_addr);
    ctx->entry_out = (symc_entry_out *)cpu_addr;
    ctx->cnt_out = 0;

    func_exit();
}

/* ! set symc pad buffer */
static void drv_symc_set_pad_buffer(td_u32 chn, compat_addr dma_addr, const td_void *cpu_addr)
{
    symc_hard_context *ctx = &g_hard_context[chn];

    addr_u64(ctx->dma_pad) = addr_u64(dma_addr);
    ctx->via_pad = (td_u8 *)cpu_addr;
    ctx->offset_pad = 0x00;
    ot_cipher_dbg("CHAN[%-2d] PAD: PHY 0x%x, VIA %pK\n", chn, addr_l32(dma_addr), cpu_addr);
}

/* ! set secure channel,
 *  non-secure CPU can't change the value of SEC_CHN_CFG,
 *  so non-secure CPU call this function will do nothing.
 */
static void drv_symc_enable_secure(td_u32 chn, td_u32 ss, td_u32 ds)
{
    in_sym_chn_ctrl chn_ctrl;

    /* key seclect */
    chn_ctrl.u32 = symc_read(in_sym_chn_ctrl(chn));
    chn_ctrl.bits.sym_chn_ss = ss;
    chn_ctrl.bits.sym_chn_ds = ds;
    symc_write(in_sym_chn_ctrl(chn), chn_ctrl.u32);
    ot_cipher_dbg("in_sym_chn_ctrl[0x%x]  :0x%x\n", in_sym_chn_ctrl(chn), chn_ctrl.u32);

    return;
}

static void drv_symc_print_last_node(td_u32 chn_num)
{
    symc_entry_in *in = TD_NULL;
    symc_entry_out *out = TD_NULL;
    symc_hard_context *ctx = TD_NULL;

    func_enter();

    ctx = &g_hard_context[chn_num];

    /* get last in node info */
    in = &ctx->entry_in[ctx->id_in == 0x00 ? SYMC_MAX_LIST_NUM : ctx->id_in - 1];

    /* get last out node info */
    out = &ctx->entry_out[ctx->id_out == 0x00 ? SYMC_MAX_LIST_NUM : ctx->id_out - 1];

    ot_cipher_error("chn %d, src ctrl 0x%x, addr 0x%x, size 0x%x, dest addr 0x%x, size 0x%x\n", chn_num,
        *(td_u32 *)in, in->sym_start_addr, in->sym_alg_length, out->sym_start_addr, out->sym_alg_length);

    crypto_unused(in);
    crypto_unused(out);

    func_exit();
    return;
}

static void drv_symc_print_status(td_u32 chn_num)
{
    out_sym_chan_raw_int out_chan_raw_int;
    out_sym_chan_raw_int_en out_chan_raw_int_en;
    out_sym_chan_int out_chan_int;
    in_sym_chn_ctrl in_chn_ctrl;
    spacc_is status;
    td_u32 chn_lock;

    ot_cipher_error("in_sym_chn_node_start_addr_l  : 0x%x\n", symc_read(in_sym_chn_node_start_addr_l(chn_num)));
    ot_cipher_error("in_sym_chn_node_start_addr_h  : 0x%x\n", symc_read(in_sym_chn_node_start_addr_h(chn_num)));
    ot_cipher_error("in_sym_chn_node_length        : 0x%x\n", symc_read(in_sym_chn_node_length(chn_num)));
    ot_cipher_error("in_sym_chn_node_wr_point      : 0x%x\n", symc_read(in_sym_chn_node_wr_point(chn_num)));
    ot_cipher_error("in_sym_chn_node_rd_point      : 0x%x\n", symc_read(in_sym_chn_node_rd_point(chn_num)));
    ot_cipher_error("IN_SYM_CHN_RD_ADDR_H          : 0x%x\n", symc_read(dbg_in_sym_chn_rd_addr_h(chn_num)));
    ot_cipher_error("IN_SYM_CHN_RD_ADDR_L          : 0x%x\n", symc_read(dbg_in_sym_chn_rd_addr_l(chn_num)));
    ot_cipher_error("IN_SYM_CHN_DATA_LEN           : 0x%x\n", symc_read(dbg_in_sym_chn_data_len(chn_num)));
    ot_cipher_error("in_sym_chn_key_ctrl           : 0x%x\n", symc_read(in_sym_chn_key_ctrl(chn_num)));
    ot_cipher_error("sym_dma_copy                  : 0x%x\n", symc_read(in_sym_out_ctrl(chn_num)));

    ot_cipher_error("out_sym_chn_node_start_addr_l : 0x%x\n", symc_read(out_sym_chn_node_start_addr_l(chn_num)));
    ot_cipher_error("out_sym_chn_node_start_addr_h : 0x%x\n", symc_read(out_sym_chn_node_start_addr_h(chn_num)));
    ot_cipher_error("out_sym_chn_node_length       : 0x%x\n", symc_read(out_sym_chn_node_length(chn_num)));
    ot_cipher_error("out_sym_chn_node_wr_point     : 0x%x\n", symc_read(in_sym_chn_node_wr_point(chn_num)));
    ot_cipher_error("out_sym_chn_node_rd_point     : 0x%x\n", symc_read(in_sym_chn_node_rd_point(chn_num)));
    ot_cipher_error("out_sym_chn_node_ctrl         : 0x%x\n", symc_read(out_sym_chn_node_ctrl(chn_num)));
    ot_cipher_error("dbg_out_sym_chn_rd_addr_l     : 0x%x\n", symc_read(dbg_out_sym_chn_rd_addr_l(chn_num)));
    ot_cipher_error("dbg_out_sym_chn_rd_addr_h     : 0x%x\n", symc_read(dbg_out_sym_chn_rd_addr_h(chn_num)));
    ot_cipher_error("dbg_out_sym_chn_node_left_buf_len : 0x%x\n",
        symc_read(dbg_out_sym_chn_node_left_buf_len(chn_num)));

    in_chn_ctrl.u32 = symc_read(in_sym_chn_ctrl(chn_num));
    ot_cipher_error("chn enable %d, mmu enable %d, ss 0x%x, ds 0x%x\n", in_chn_ctrl.bits.sym_chn_en,
        in_chn_ctrl.bits.sym_chn_mmu_en, in_chn_ctrl.bits.sym_chn_ss, in_chn_ctrl.bits.sym_chn_ds);

    status.u32 = symc_read(SPACC_IS);
    out_chan_raw_int.u32 = symc_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT);
    out_chan_int.u32 = symc_read(OUT_SYM_CHAN_LAST_NODE_INT);
    out_chan_raw_int_en.u32 = symc_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN);
    chn_lock = symc_read(SPACC_SYM_CHN_LOCK);
    ot_cipher_error("\nsym_chn_lock 0x%x, chn %d, spacc_int_status 0x%x, chn_obuf_en 0x%x, status 0x%x, raw 0x%x\n",
        (chn_lock >> (chn_num * 0x2)) & 0x03, chn_num, status.u32,
        (out_chan_raw_int_en.bits.out_sym_chan_int_en >> ((chn_num > 0) ? (chn_num - 1) : chn_num)) & 0x01,
        (out_chan_int.bits.out_sym_chan_int >> ((chn_num > 0) ? (chn_num - 1) : chn_num)) & 0x01,
        (out_chan_raw_int.bits.out_sym_chan_raw_int >> ((chn_num > 0) ? (chn_num - 1) : chn_num)) & 0x01);

    ot_cipher_error("\nThe cause of time out may be:\n"
        "\t1. SMMU address invalid\n"
        "\t2. interrupt number or name incorrect\n"
        "\t3. CPU type mismatching, request CPU and channel: %s\n", crypto_is_sec_cpu() ? "secure" : "non-secure");

    /* avoid compile error when ot_cipher_error be defined to empty */
    crypto_unused(status);
    crypto_unused(chn_lock);
    crypto_unused(out_chan_raw_int);
    crypto_unused(out_chan_raw_int_en);
    crypto_unused(out_chan_int);
}

static void drv_symc_print_smmu_error(td_u32 chn_num)
{
#ifdef CRYPTO_SMMU_SUPPORT
    spacc_axi_iraw smmu_raw_int;
    spacc_axi_iraw smmu_raw_set;

    func_enter();

    smmu_raw_int.u32 = symc_read(SPACC_AXI_IRAW);
    smmu_raw_set.u32 = 0;

    if (smmu_raw_int.bits.spacc_mmu_in_sym_sec_err_iraw) {
        smmu_raw_set.bits.spacc_mmu_in_sym_sec_err_iraw = 1;
        ot_cipher_error("symc error: spacc_mmu_in_sym_sec_err_iraw, chn %d !!!\n", chn_num);
    }
    if (smmu_raw_int.bits.spacc_mmu_out_sym_sec_err_iraw) {
        smmu_raw_set.bits.spacc_mmu_out_sym_sec_err_iraw = 1;
        ot_cipher_error("symc error: spacc_mmu_out_sym_sec_err_iraw, chn %d !!!\n", chn_num);
    }
    if (smmu_raw_int.bits.spacc_mmu_in_sym_nosec_err_iraw) {
        smmu_raw_set.bits.spacc_mmu_in_sym_nosec_err_iraw = 1;
        ot_cipher_error("symc error: spacc_mmu_in_sym_nosec_err_iraw, chn %d !!!\n", chn_num);
    }
    if (smmu_raw_int.bits.spacc_mmu_out_sym_sec_err_iraw) {
        smmu_raw_set.bits.spacc_mmu_out_sym_sec_err_iraw = 1;
        ot_cipher_error("symc error: spacc_mmu_out_sym_nosec_err_iraw, chn %d !!!\n", chn_num);
    }

    /* clear error int */
    symc_write(SPACC_AXI_IRAW, smmu_raw_set.u32);

    func_exit();
#else
    crypto_unused(chn_num);
#endif

    return;
}

#define SYMC_PRINT_CTRL_CHECK_ERR(code, field, msg) \
    do { \
        if ((code).bits.field) { \
            ot_cipher_error(msg); \
        } \
    } while (0)

static void drv_symc_print_ctrl_check_err(sym_calc_ctrl_check_err code)
{
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_ds_error,
        "symc error: in ctrl input ds doesn't match key info ds/dns.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_ss_error,
        "symc error: in ctrl input ss doesn't match key info ss/sns.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_alg_error,
        "symc error: in ctr input algorithm doesn't match key info status.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_alg_invld_error,
        "symc error: in ctrl input algorithm mode is invalid.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_dec_error,
        "symc error: in ctrl input dec doesn't match key info status.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_error,
        "symc error: key info is hpp dedicated, but channel lock doesn't hpp.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_sc_error,
        "symc error: key info is secure, but channel lock is ree.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_key_size_error,
        "symc error: in ctrl input key size doesn't match key info key size.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_hdcp_error,
        "symc error: When HDCP calculation, hw key isn't HDCP key.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_cenc_dec_error,
        "symc error: When cenc enable ,input data is used for encrypt.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_cenc_alg_error,
        "symc error: When cenc enable, input algorithm isn't AES/SM4 CBC/CTR.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_gcm_ccm_error,
        "symc error: When algorithm mode is AES GCM/CCM ,sym_ccm_gcm_in_fla is illegal.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_iv_length_error,
        "symc error: When mode is GCM/CTR, in ctrl iv length beyond 16byte or 0 byte is valid.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_check_data_length_error,
        "symc error: IN CTRL input data length beyond 16 byte or 0 byte is valid.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_big_key_info_diff_error,
        "symc error: When key size is 192/256bit, but key info doesn't match.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_hdcp_key_size_error,
        "symc error: when channel 0 HDCP is enable, Hdcp key is static key but key size isn't 128bit.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_hdcp_sm4_error,
        "symc error: when channel 0 HDCP is enable, SW select SM4 algorithm but OTP sm4_disable is 1.\n");
    SYMC_PRINT_CTRL_CHECK_ERR(code, calc_ctrl_des_keysame_error, "symc error: des core's tdes key same error.\n");
}

#undef SYMC_PRINT_CTRL_CHECK_ERR

static td_s32 drv_symc_handle_ctrl_check_err(td_u32 chn_num,
    sym_calc_ctrl_check_err code, sym_calc_ctrl_check_err_status status)
{
    if (!code.u32) {
        return TD_SUCCESS;
    }

    ot_cipher_error("symc error chn %d !!!\n", status.bits.hpp_error_sym_chan_id);
    ot_cipher_error("sym_calc_ctrl_check_err[0x%x]: 0x%x\n", sym_calc_ctrl_check_err(SPACC_CPU_CUR), code.u32);

    /* clear error code */
    status.bits.hpp_sym_error_code_clr = 1;
    symc_write(sym_calc_ctrl_check_err_status(SPACC_CPU_CUR), status.u32);

    /* print the in out buffer address */
    drv_symc_print_smmu_error(chn_num);
    drv_symc_print_last_node(chn_num);
    drv_symc_print_status(chn_num);

    return OT_ERR_CIPHER_HARD_STATUS;
}

static td_s32 drv_symc_get_err_code(td_u32 chn_num)
{
    sym_calc_ctrl_check_err code;
    sym_calc_ctrl_check_err_status status;

    func_enter();

    /* read error code */
    code.u32 = symc_read(sym_calc_ctrl_check_err(SPACC_CPU_CUR));
    status.u32 = symc_read(sym_calc_ctrl_check_err_status(SPACC_CPU_CUR));
    drv_symc_print_ctrl_check_err(code);
    if (drv_symc_handle_ctrl_check_err(chn_num, code, status) != TD_SUCCESS) {
        return OT_ERR_CIPHER_HARD_STATUS;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 drv_symc_check_nosec_mmu(td_void)
{
    td_s32 ret = TD_SUCCESS;
    spacc_mmu_in_sym_nosec_unvld_va_tab in_va_tab;
    spacc_mmu_in_sym_nosec_unvld_pa_tab in_pa_tab;
    spacc_mmu_out_sym_nosec_unvld_va_tab out_va_tab;
    spacc_mmu_out_sym_nosec_unvld_pa_tab out_pa_tab;

    /* read error code */
    in_va_tab.u32 = symc_read(SPACC_MMU_IN_SYM_NOSEC_UNVLD_VA_TAB);
    in_pa_tab.u32 = symc_read(SPACC_MMU_IN_SYM_NOSEC_UNVLD_PA_TAB);
    if (in_va_tab.bits.mmu_in_sym_nosec_unvld_va_vld == VALID_SIGNAL) {
        ot_cipher_error("SPACC_MMU_IN_SYM_NOSEC_UNVLD_VA_TAB[0x%x]: 0x%x\n",
            SPACC_MMU_IN_SYM_NOSEC_UNVLD_PA_TAB, in_va_tab.u32);
        ot_cipher_error("mmu_in_sym_nosec_unvld_chn_id %d !!!\n", in_va_tab.bits.mmu_in_sym_nosec_unvld_chn_id);
        ot_cipher_error("mmu_in_sym_nosec_unvld_va_tab 0x%x !!!\n",
            in_va_tab.bits.mmu_in_sym_nosec_unvld_va_tab << SMMU_PAGE_BIT);
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }
    if (in_pa_tab.bits.mmu_in_sym_nosec_unvld_pa_vld == VALID_SIGNAL) {
        ot_cipher_error("SPACC_MMU_IN_SYM_NOSEC_UNVLD_PA_TAB[0x%x]: 0x%x\n",
            SPACC_MMU_IN_SYM_NOSEC_UNVLD_PA_TAB, in_pa_tab.u32);
        ot_cipher_error("mmu_in_sym_nosec_unvld_vld_err %d !!!\n", in_pa_tab.bits.mmu_in_sym_nosec_unvld_vld_err);
        ot_cipher_error("mmu_in_sym_nosec_unvld_pa_tab 0x%x !!!\n",
            in_pa_tab.bits.mmu_in_sym_nosec_unvld_pa_tab << SMMU_PAGE_BIT);
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }

    out_va_tab.u32 = symc_read(SPACC_MMU_OUT_SYM_NOSEC_UNVLD_VA_TAB);
    out_pa_tab.u32 = symc_read(SPACC_MMU_OUT_SYM_NOSEC_UNVLD_PA_TAB);
    if (out_va_tab.bits.mmu_out_sym_nosec_unvld_va_vld == VALID_SIGNAL) {
        ot_cipher_error("SPACC_MMU_OUT_SYM_NOSEC_UNVLD_VA_TAB[0x%x]: 0x%x\n",
            SPACC_MMU_OUT_SYM_NOSEC_UNVLD_VA_TAB, out_va_tab.u32);
        ot_cipher_error("mmu_out_sym_nosec_unvld_chn_id %d !!!\n", out_va_tab.bits.mmu_out_sym_nosec_unvld_chn_id);
        ot_cipher_error("mmu_out_sym_nosec_unvld_va_tab 0x%x !!!\n",
            out_va_tab.bits.mmu_out_sym_nosec_unvld_va_tab << SMMU_PAGE_BIT);
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }
    if (out_pa_tab.bits.mmu_out_sym_nosec_unvld_pa_vld == VALID_SIGNAL) {
        ot_cipher_error("SPACC_MMU_OUT_SYM_NOSEC_UNVLD_PA_TAB[0x%x]: 0x%x\n",
            SPACC_MMU_OUT_SYM_NOSEC_UNVLD_PA_TAB, out_pa_tab.u32);
        ot_cipher_error("mmu_in_sym_nosec_unvld_vld_err %d !!!\n", out_pa_tab.bits.mmu_out_sym_nosec_unvld_vld_err);
        ot_cipher_error("mmu_in_sym_nosec_unvld_pa_tab 0x%x !!!\n",
            out_pa_tab.bits.mmu_out_sym_nosec_unvld_pa_tab << SMMU_PAGE_BIT);
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }

    /* clear error code */
    symc_write(SPACC_MMU_IN_SYM_NOSEC_UNVLD_VA_TAB,  in_va_tab.u32);
    symc_write(SPACC_MMU_IN_SYM_NOSEC_UNVLD_PA_TAB,  in_pa_tab.u32);
    symc_write(SPACC_MMU_OUT_SYM_NOSEC_UNVLD_VA_TAB, out_va_tab.u32);
    symc_write(SPACC_MMU_OUT_SYM_NOSEC_UNVLD_PA_TAB, out_pa_tab.u32);

    return ret;
}

static td_s32 drv_symc_check_sec_mmu(td_void)
{
    td_s32 ret = TD_SUCCESS;
    spacc_mmu_in_sym_sec_unvld_va_tab in_va_tab;
    spacc_mmu_in_sym_sec_unvld_pa_tab in_pa_tab;
    spacc_mmu_out_sym_sec_unvld_va_tab out_va_tab;
    spacc_mmu_out_sym_sec_unvld_pa_tab out_pa_tab;

    /* read error code */
    in_va_tab.u32 = symc_read(SPACC_MMU_IN_SYM_SEC_UNVLD_VA_TAB);
    in_pa_tab.u32 = symc_read(SPACC_MMU_IN_SYM_SEC_UNVLD_PA_TAB);
    if (in_va_tab.bits.mmu_in_sym_sec_unvld_va_vld == VALID_SIGNAL) {
        ot_cipher_error("SPACC_MMU_IN_SYM_SEC_UNVLD_VA_TAB[0x%x]: 0x%x\n",
            SPACC_MMU_IN_SYM_SEC_UNVLD_PA_TAB, in_va_tab.u32);
        ot_cipher_error("mmu_in_sym_sec_unvld_chn_id %d !!!\n", in_va_tab.bits.mmu_in_sym_sec_unvld_chn_id);
        ot_cipher_error("mmu_in_sym_sec_unvld_va_tab 0x%x !!!\n",
            in_va_tab.bits.mmu_in_sym_sec_unvld_va_tab << SMMU_PAGE_BIT);
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }
    if (in_pa_tab.bits.mmu_in_sym_sec_unvld_pa_vld == VALID_SIGNAL) {
        ot_cipher_error("SPACC_MMU_IN_SYM_SEC_UNVLD_PA_TAB[0x%x]: 0x%x\n",
            SPACC_MMU_IN_SYM_SEC_UNVLD_PA_TAB, in_pa_tab.u32);
        ot_cipher_error("mmu_in_sym_sec_unvld_vld_err %d !!!\n", in_pa_tab.bits.mmu_in_sym_sec_unvld_vld_err);
        ot_cipher_error("mmu_in_sym_sec_unvld_tag_err %d !!!\n", in_pa_tab.bits.mmu_in_sym_sec_unvld_tag_err);
        ot_cipher_error("mmu_in_sym_sec_unvld_pa_tab 0x%x !!!\n",
            in_pa_tab.bits.mmu_in_sym_sec_unvld_pa_tab << SMMU_PAGE_BIT);
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }

    out_va_tab.u32 = symc_read(SPACC_MMU_OUT_SYM_SEC_UNVLD_VA_TAB);
    out_pa_tab.u32 = symc_read(SPACC_MMU_OUT_SYM_SEC_UNVLD_PA_TAB);
    if (out_va_tab.bits.mmu_out_sym_sec_unvld_va_vld == VALID_SIGNAL) {
        ot_cipher_error("SPACC_MMU_OUT_SYM_SEC_UNVLD_VA_TAB[0x%x]: 0x%x\n",
            SPACC_MMU_OUT_SYM_SEC_UNVLD_VA_TAB, out_va_tab.u32);
        ot_cipher_error("mmu_out_sym_sec_unvld_chn_id %d !!!\n", out_va_tab.bits.mmu_out_sym_sec_unvld_chn_id);
        ot_cipher_error("mmu_out_sym_sec_unvld_va_tab 0x%x !!!\n",
            out_va_tab.bits.mmu_out_sym_sec_unvld_va_tab << SMMU_PAGE_BIT);
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }
    if (out_pa_tab.bits.mmu_out_sym_sec_unvld_pa_vld == VALID_SIGNAL) {
        ot_cipher_error("SPACC_MMU_OUT_SYM_SEC_UNVLD_PA_TAB[0x%x]: 0x%x\n",
            SPACC_MMU_OUT_SYM_SEC_UNVLD_PA_TAB, out_pa_tab.u32);
        ot_cipher_error("mmu_in_sym_sec_unvld_vld_err  %d !!!\n", out_pa_tab.bits.mmu_out_sym_sec_unvld_vld_err);
        ot_cipher_error("mmu_out_sym_sec_unvld_tag_err %d !!!\n", out_pa_tab.bits.mmu_out_sym_sec_unvld_tag_err);
        ot_cipher_error("mmu_in_sym_sec_unvld_pa_tab 0x%x !!!\n",
            out_pa_tab.bits.mmu_out_sym_sec_unvld_pa_tab << SMMU_PAGE_BIT);
        ret = OT_ERR_CIPHER_INVALID_ADDR;
    }

    /* clear error code */
    symc_write(SPACC_MMU_IN_SYM_SEC_UNVLD_VA_TAB, in_va_tab.u32);
    symc_write(SPACC_MMU_IN_SYM_SEC_UNVLD_PA_TAB, in_pa_tab.u32);
    symc_write(SPACC_MMU_OUT_SYM_SEC_UNVLD_VA_TAB, out_va_tab.u32);
    symc_write(SPACC_MMU_OUT_SYM_SEC_UNVLD_PA_TAB, out_pa_tab.u32);

    return ret;
}

static void drv_symc_entry_init(crypto_mem mem, crypto_mem padding)
{
    td_u32 i;
    td_void *cpu_addr = TD_NULL;
    compat_addr dma_pad;
    td_u8 *via_pad = TD_NULL;

    func_enter();

    /* set in node and out node dma buffer */
    ot_cipher_dbg("symc entry list configure\n");
    cpu_addr = mem.dma_virt;

    /* skip the in node and out node dma buffer */
    addr_u64(dma_pad) = addr_u64(padding.dma_addr);
    via_pad = (td_u8 *)padding.dma_virt;

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if ((CIPHER_HARD_CHANNEL_MASK >> i) & 0x01) { /* valid channel */
            /* in node and out node */
            drv_symc_set_entry(i, mem.dma_addr, cpu_addr);
            addr_u64(g_hard_context[i].dma_entry) = addr_u64(mem.dma_addr);

            addr_u64(mem.dma_addr) += SYMC_NODE_SIZE;      /* move to next channel */
            cpu_addr = (td_u8 *)cpu_addr + SYMC_NODE_SIZE; /* move to next channel */

            /* padding */
            drv_symc_set_pad_buffer(i, dma_pad, via_pad);
            addr_u64(dma_pad) += SYMC_PAD_BUFFER_LEN;
            via_pad += SYMC_PAD_BUFFER_LEN;
        }
    }

    func_exit();
    return;
}

static td_s32 drv_symc_mem_init(void)
{
    td_s32 ret;

    func_enter();

    /** dma buffer struct
      * ((in_node || out_node) * chn_num) || (pad_buffer * chn_num)
      */
    ot_cipher_dbg("alloc memory for nodes list\n");
    ret = crypto_mem_create(&g_symc_dma, MEM_TYPE_MMZ, "cipher_node", SYMC_NODE_LIST_SIZE);
    if (ret != TD_SUCCESS) {
        ot_cipher_error("error, malloc ddr for symc nodes list failed\n");
        print_func_errno(crypto_mem_create, ret);
        return ret;
    }

    ot_cipher_dbg("alloc memory for padding buffer\n");
#ifdef OT_SMMU_SUPPORT
    ret = crypto_mem_create(&g_padding_dma, MEM_TYPE_SMMU, "cipher_padding", SYMC_PAD_BUFFER_TOTAL_LEN);
#else
    ret = crypto_mem_create(&g_padding_dma, MEM_TYPE_MMZ, "cipher_padding", SYMC_PAD_BUFFER_TOTAL_LEN);
#endif
    if (ret != TD_SUCCESS) {
        ot_cipher_error("error, malloc ddr for symc padding buffer failed\n");
        print_func_errno(crypto_mem_create, ret);
        (td_void)crypto_mem_destroy(&g_symc_dma);
        return ret;
    }
    ot_cipher_dbg("SYMC DMA buffer, MMZ 0x%x, VIA %pK, size 0x%x\n",
        addr_l32(g_symc_dma.dma_addr), g_symc_dma.dma_virt, g_symc_dma.dma_size);

    drv_symc_entry_init(g_symc_dma, g_padding_dma);

    func_exit();
    return TD_SUCCESS;
}

static td_s32 drv_symc_mem_deinit(void)
{
    td_s32 ret;

    ret = crypto_mem_destroy(&g_symc_dma);
    if (ret != TD_SUCCESS) {
        print_func_errno(crypto_mem_destroy, ret);
        return ret;
    }
    ret = crypto_mem_destroy(&g_padding_dma);
    if (ret != TD_SUCCESS) {
        print_func_errno(crypto_mem_destroy, ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 drv_symc_chn_resume(td_u32 chn_num)
{
    td_s32 ret;
    td_u32 base;

    func_enter();

#ifdef CRYPTO_SMMU_SUPPORT
    if (crypto_is_sec_cpu()) {
        base = symc_read(SPACC_MMU_SEC_TLB);
    } else {
        base = symc_read(SPACC_MMU_NOSEC_TLB);
    }
#else
    base = symc_read(in_sym_chn_node_start_addr_l(chn_num));
#endif
    if (base == 0) {
        /* smmu base address is zero means hardware be unexpected reset */
        ot_cipher_warn("cipher module is not ready, try to resume it now...\n");
        ret = drv_symc_resume();
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_symc_resume, ret);
            return ret;
        }
    }

    crypto_unused(chn_num);

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_symc_alloc_chn(td_u32 *chn_num, ot_cipher_type type)
{
    td_s32 ret;
    td_u32 i;
    td_u32 used;
    td_u32 chnn_who_used;
    td_u32 my_cpu;

    crypto_unused(type);

    my_cpu = (crypto_is_sec_cpu() == TD_TRUE) ? SPACC_CPU_TEE : SPACC_CPU_REE;

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if (g_hard_context[i].open == TD_TRUE) {
            continue;
        }

        if (((0x1 << i) & CIPHER_HARD_CHANNEL_MASK) == 0x00) {
            continue;
        }

        used = symc_read(SPACC_SYM_CHN_LOCK);
        chnn_who_used = chn_who_used_get(used, i);
        if (chnn_who_used == SPACC_CPU_IDLE) {
            /* try to use this channel */
            chn_who_used_set(used, i, my_cpu);
            symc_write(SPACC_SYM_CHN_LOCK, used);

            /* check if the channal aleardy be useded by other cpu
             * if other cpu break-in when write the CIPHER_NON_SEC_CHN_WHO_USED
             * the value of CIPHER_NON_SEC_CHN_WHO_USED will be channged
             */
            used = symc_read(SPACC_SYM_CHN_LOCK);
            chnn_who_used = chn_who_used_get(used, i);
            if (chnn_who_used != my_cpu) {
                /* chn aleardy be used by tee */
                continue;
            }

            /* alloc channel */
            g_hard_context[i].open = TD_TRUE;
            *chn_num = i;
            ot_cipher_dbg("+++ alloc symc chn %d +++\n", i);
            break;
        }
    }

    if (i >= CRYPTO_HARD_CHANNEL_MAX) {
        print_errno(OT_ERR_CIPHER_BUSY);
        return OT_ERR_CIPHER_BUSY;
    }

    ret = drv_symc_recover_entry(i);
    if (ret != TD_SUCCESS) {
        chn_who_used_clr(used, i);
        symc_write(SPACC_SYM_CHN_LOCK, used);
        print_func_errno(drv_symc_recover_entry, ret);
        return ret;
    }

    /* hardware may be unexpected reset by other module or platform,
     * such as unexpected reset by fastboot after load tee image,
     * in this case, the hardware configuration will be reset,
     * here try to re-config the hardware.
     */
    ret = drv_symc_chn_resume(*chn_num);
    if (ret != TD_SUCCESS) {
        chn_who_used_clr(used, i);
        symc_write(SPACC_SYM_CHN_LOCK, used);
        print_func_errno(drv_symc_chn_resume, ret);
        return ret;
    }

    return TD_SUCCESS;
}

void drv_symc_free_chn(td_u32 chn_num)
{
    td_u32 used;

    func_enter();

    if (g_hard_context[chn_num].open == TD_FALSE) {
        func_exit();
        return;
    }

    /* set channel to idle */
    used = symc_read(SPACC_SYM_CHN_LOCK);
    chn_who_used_clr(used, chn_num);
    symc_write(SPACC_SYM_CHN_LOCK, used);

    drv_symc_enable_secure(chn_num, SYMC_CFG_NON_SECURE, SYMC_CFG_NON_SECURE);

    if (g_hard_context[chn_num].destroy != TD_NULL) {
        g_hard_context[chn_num].destroy();
        g_hard_context[chn_num].destroy = TD_NULL;
    }

    /* free channel */
    g_hard_context[chn_num].open = TD_FALSE;

    ot_cipher_dbg("free symc chn %d\n", chn_num);

    func_exit();
    return;
}

static td_void drv_symc_low_power(td_void)
{
    spacc_calc_crg_cfg cfg;

    cfg.u32 = symc_read(SPACC_CALC_CRG_CFG);
    cfg.bits.spacc_aes_low_power_enable  = SYMC_LOW_POWER_ENABLE;
    cfg.bits.spacc_tdes_low_power_enable = SYMC_LOW_POWER_ENABLE;
    cfg.bits.spacc_sm4_low_power_enable  = SYMC_LOW_POWER_ENABLE;
    symc_write(SPACC_CALC_CRG_CFG, cfg.u32);
}

td_s32 drv_symc_init(void)
{
    td_s32 ret;
    td_u32 i;

    func_enter();

    if (g_symc_initialize == TD_TRUE) {
        func_exit();
        return TD_SUCCESS;
    }

    g_hard_context = crypto_calloc(sizeof(symc_hard_context), CRYPTO_HARD_CHANNEL_MAX);
    if (g_hard_context == TD_NULL) {
        print_func_errno(crypto_calloc, OT_ERR_CIPHER_FAILED_MEM);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    (td_void)memset_s(&g_symc_dma, sizeof(g_symc_dma), 0, sizeof(g_symc_dma));

    /* enable symc */
    module_disable(CRYPTO_MODULE_ID_SYMC);
    module_enable(CRYPTO_MODULE_ID_SYMC);

    ret = drv_symc_mem_init();
    if (ret != TD_SUCCESS) {
        goto __error;
    }

    drv_symc_smmu_base_addr();
    drv_symc_low_power();

#ifdef CRYPTO_OS_INT_SUPPORT
    /* symc interrupt configure */
    drv_symc_interrupt_init();

    /* symc register interrupt function */
    ret = drv_symc_register_interrupt();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_register_interrupt, ret);
        drv_symc_mem_deinit();
        goto __error;
    }
#endif

    /* set all channel as non-secure channel,
     *  may be set it to secure channel when alloc handle.
     */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        /* check the valid channel */
        if (CIPHER_HARD_CHANNEL_MASK & (0x1 << i)) {
            drv_symc_enable_secure(i, SYMC_CFG_NON_SECURE, SYMC_CFG_NON_SECURE);
        }
    }

    g_symc_initialize = TD_TRUE;
    symc_write(SPACC_SYM_CHN_LOCK, 0x00);

    func_exit();
    return TD_SUCCESS;

__error:
    module_disable(CRYPTO_MODULE_ID_SYMC);
    crypto_free(g_hard_context);

    func_exit();
    return ret;
}

td_s32 drv_symc_resume(void)
{
    td_u32 i;
    td_s32 ret;
    td_u32 ss = SYMC_CFG_NON_SECURE;
    td_u32 ds = SYMC_CFG_NON_SECURE;
    td_u32 my_cpu = SPACC_CPU_REE;
    td_u32 used;

    func_enter();

    ot_cipher_dbg("enable symc\n");
    module_enable(CRYPTO_MODULE_ID_SYMC);
    module_disable(CRYPTO_MODULE_ID_SM4);

    if (crypto_is_sec_cpu() == TD_TRUE) {
        ss = SYMC_CFG_SECURE;
        ds = SYMC_CFG_SECURE;
        my_cpu = SPACC_CPU_TEE;
    }

    for (i = 1; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if (g_hard_context[i].open) {
            used = symc_read(SPACC_SYM_CHN_LOCK);
            drv_symc_enable_secure(i, ss, ds);
            chn_who_used_set(used, i, my_cpu);
            symc_write(SPACC_SYM_CHN_LOCK, used);
            ret = drv_symc_recover_entry(i);
            if (ret != TD_SUCCESS) {
                print_func_errno(drv_symc_recover_entry, ret);
                return ret;
            }
        }
    }

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_symc_interrupt_init();
#endif

    drv_symc_entry_init(g_symc_dma, g_padding_dma);
    drv_symc_smmu_base_addr();

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_symc_deinit(void)
{
    td_s32 ret;

    func_enter();

    chk_param_err_return(g_symc_initialize != TD_TRUE);

    ret = drv_symc_mem_deinit();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_mem_deinit, ret);
        return ret;
    }
    module_disable(CRYPTO_MODULE_ID_SYMC);

#ifdef CRYPTO_OS_INT_SUPPORT
    drv_symc_unregister_interrupt();
#endif

    crypto_free(g_hard_context);
    g_symc_initialize = TD_FALSE;

    func_exit();
    return TD_SUCCESS;
}

void drv_symc_add_buf_usage(td_u32 chn_num, td_u32 in, symc_node_usage usage)
{
    symc_hard_context *ctx = TD_NULL;
    td_u32 id;

    ctx = &g_hard_context[chn_num];

    if (in == TD_FALSE) {
        /* out node info doesn't need last node flag */
        return;
    }

    /* get last node */
    id = (ctx->id_in == 0) ? (SYMC_MAX_LIST_NUM - 1) : (ctx->id_in - 1);
    if (usage == SYMC_NODE_USAGE_LAST) {
        ctx->entry_in[id].sym_last_node = 0x1;
    }

    ctx->entry_in[id].aes_ccm_gcm_in_flag |=
        ((td_u32)usage & (SYMC_NODE_USAGE_IN_CCM_A | SYMC_NODE_USAGE_IN_CCM_P)) >> 3; /* 3: bit 1-0 */

    if (usage == SYMC_NODE_USAGE_CCM_LAST) {
        ctx->entry_in[id].sym_ccm_last = 0x1;
    }

    if (usage == SYMC_NODE_USAGE_GCM_FIRST) {
        ctx->entry_in[id].sym_gcm_first = 0x1;
    }

    ot_cipher_dbg("chn %d, add symc in ctrl: id %d, sym_last_node 0x%x\n", chn_num, id,
        ctx->entry_in[id].sym_last_node);

    return;
}

td_void drv_symc_set_iv(td_u32 chn_num, const td_u32 *iv, td_u32 ivlen, td_u32 flag)
{
    td_u32 i;
    symc_hard_context *ctx = TD_NULL;

    func_enter();

    ctx = &g_hard_context[chn_num];

    /* copy iv data into channel context. */
    for (i = 0; i < SYMC_IV_MAX_SIZE_IN_WORD; i++) {
        ctx->iv[i] = iv[i];
    }
    ctx->iv_flag = flag;
    ctx->iv_len = ivlen;

    func_exit();
    return;
}

td_void drv_symc_get_iv(td_u32 chn_num, td_u32 iv[SYMC_IV_MAX_SIZE_IN_WORD], td_u32 iv_len)
{
    td_u32 i;

    func_enter();

    for (i = 0; i < SYMC_IV_MAX_SIZE_IN_WORD; i++) {
        iv[i] = symc_read(chn_n_cipher_ivout(chn_num) + i * WORD_WIDTH);
        ot_cipher_dbg("IV[%u]: 0x%x\n", i, iv[i]);
    }

    func_exit();
    return;
}

td_void drv_symc_set_key(td_u32 chn_num, const td_u32 *key, td_u32 key_len, td_u32 odd)
{
    /* all of key is come from klad, need not to config */
    crypto_unused(chn_num);
    crypto_unused(key);
    crypto_unused(odd);
    crypto_unused(key_len);

    return;
}

void drv_symc_set_key_source(td_u32 chn_num, td_u32 keyslot)
{
    in_sym_chn_key_ctrl cipher_key_ctrl;

    func_enter();

    /* key seclect */
    cipher_key_ctrl.u32 = symc_read(in_sym_chn_key_ctrl(chn_num));
    cipher_key_ctrl.bits.sym_key_chn_id = keyslot;
    symc_write(in_sym_chn_key_ctrl(chn_num), cipher_key_ctrl.u32);

    func_exit();
    return;
}

td_s32 drv_symc_add_inbuf(td_u32 chn_num, compat_addr buf_phy, td_u32 buf_size, symc_node_usage usage)
{
    symc_hard_context *ctx = TD_NULL;
    td_u32 id;
    td_u32 size;
    td_u32 i;
    td_void *addr = TD_NULL;

    func_enter();

    chk_param_err_return(g_symc_initialize != TD_TRUE);
    chk_param_err_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    ctx = &g_hard_context[chn_num];

    id = ctx->id_in;
    addr = &ctx->entry_in[id];
    size = sizeof(symc_entry_in);
    (td_void)memset_s(addr, size, 0, size);

    ctx->entry_in[id].sym_start_addr = addr_l32(buf_phy);
    ctx->entry_in[id].sym_start_high = addr_h32(buf_phy);
    ctx->entry_in[id].sym_alg_length = buf_size;
    ctx->entry_in[id].sym_first_node = (td_u32)usage & SYMC_NODE_USAGE_FIRST;
    ctx->entry_in[id].sym_last_node  = ((td_u32)usage & SYMC_NODE_USAGE_LAST) >> 1;
    ctx->entry_in[id].odd_even       = ((td_u32)usage & SYMC_NODE_USAGE_ODD_KEY) >> 6; /* bit 6 -> bit 0 */
    ctx->entry_in[id].aes_ccm_gcm_in_flag = ((td_u32)usage &
        (SYMC_NODE_USAGE_IN_GCM_A | SYMC_NODE_USAGE_IN_GCM_P | SYMC_NODE_USAGE_IN_GCM_LEN |
        SYMC_NODE_USAGE_IN_CCM_N | SYMC_NODE_USAGE_IN_CCM_A | SYMC_NODE_USAGE_IN_CCM_P)) >> 0x3; /* 3: bit 1..0 */
    ctx->entry_in[id].sym_ccm_last = ((td_u32)usage & SYMC_NODE_USAGE_CCM_LAST) >> 5;    /* bit 5 -> bit 0 */
    ctx->entry_in[id].sym_gcm_first = ((td_u32)usage & SYMC_NODE_USAGE_GCM_FIRST) >> 16; /* bit 16 -> bit 0 */
    ctx->entry_in[id].sym_gcm_and_ctr_iv_len = ctx->iv_len;

    /* set IV to every node, but the hardware only update the IV
     * from node when first flag is 1
     */
    for (i = 0; i < SYMC_IV_MAX_SIZE_IN_WORD; i++) {
        ctx->entry_in[id].iv[i] = ctx->iv[i];
        ot_cipher_dbg("IV[%d]: 0x%x\n", i, ctx->iv[i]);
    }

    if (ctx->iv_flag == OT_CIPHER_IV_CHG_ONE_PACK) {
        /* update iv for first node */
        ctx->iv_flag = OT_CIPHER_IV_CHG_NONE;

        /* don't update iv any more */
        ctx->entry_in[id].sym_first_node = 1;
    } else if (ctx->iv_flag == OT_CIPHER_IV_CHG_ALL_PACK) {
        /* update iv for all node */
        ctx->entry_in[id].sym_first_node = 1;
        ctx->entry_in[id].sym_last_node = 1;
    }

    /* move to next node */
    ctx->id_in++;
    ctx->id_in %= SYMC_MAX_LIST_NUM;

    /* total count of computed nodes add 1 */
    ctx->cnt_in++;

    ot_cipher_dbg(
        "chn %d, add symc in buf[0x%llx]: id %d, cnt %d, addr 0x%llx, len 0x%x, first_node 0x%x, usage 0x%x\n",
        chn_num, addr_u64(ctx->dma_entry), id, ctx->cnt_in, addr_u64(buf_phy),
        buf_size, ctx->entry_in[id].sym_first_node, usage);

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_symc_add_outbuf(td_u32 chn_num, compat_addr buf_phy, td_u32 buf_size, symc_node_usage usage)
{
    symc_hard_context *ctx = TD_NULL;
    td_u32 id;
    td_u32 size;
    td_void *addr = TD_NULL;

    func_enter();

    crypto_unused(usage);

    ctx = &g_hard_context[chn_num];

    id = ctx->id_out;
    addr = &ctx->entry_out[id];
    size = sizeof(symc_entry_out);
    (td_void)memset_s(addr, size, 0, size);

    ctx->entry_out[id].sym_start_addr = addr_l32(buf_phy);
    ctx->entry_out[id].sym_start_high = addr_h32(buf_phy);
    ctx->entry_out[id].sym_alg_length = buf_size;

    /* move to next node */
    ctx->id_out++;
    ctx->id_out %= SYMC_MAX_LIST_NUM;

    ot_cipher_dbg("chn %d, add symc out buf[0x%llx]: id %d, addr 0x%llx, len 0x%x, ctrl 0x%x\n",
        chn_num, addr_u64(ctx->dma_entry) + SYMC_IN_NODE_SIZE, id, addr_u64(buf_phy), buf_size, usage);

    /* total count of computed nodes add 1 */
    ctx->cnt_out++;

    func_exit();
    return TD_SUCCESS;
}

#ifdef CHIP_AES_CCM_GCM_SUPPORT
td_s32 drv_aead_ccm_add_n(td_u32 chn_num, const td_u8 *n, td_u32 n_size)
{
    td_s32 ret;
    symc_hard_context *ctx = TD_NULL;
    symc_node_usage usage;
    compat_addr dma_pad;
    td_u8 *via_pad = TD_NULL;

    func_enter();

    ctx = &g_hard_context[chn_num];

    ot_cipher_dbg("PAD buffer, PHY: 0x%x, VIA %pK\n", addr_l32(ctx->dma_pad), ctx->via_pad);

    addr_u64(dma_pad) = addr_u64(ctx->dma_pad) + ctx->offset_pad;
    via_pad = ctx->via_pad + ctx->offset_pad;
    usage = SYMC_NODE_USAGE_IN_CCM_N | SYMC_NODE_USAGE_LAST;
    ret = memcpy_s(via_pad, SYMC_CCM_N_LEN, n, n_size);
    if (ret != TD_SUCCESS) {
        print_func_errno(memcpy_s, ret);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }
    ctx->offset_pad += SYMC_CCM_N_LEN;

    ret = drv_symc_add_inbuf(chn_num, dma_pad, SYMC_CCM_N_LEN, usage);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_add_inbuf, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_u32 drv_aead_ccm_a_head(td_u8 *buf, td_u32 alen)
{
    td_u32 idx = 0;

    /*
     * Formatting of the Associated Data in B1, the length of A denotes as a.
     *
     * The value a is encoded according to the following three cases:
     * If 0 < a < 2^16 - 2^8, then a  is encoded as a[0..15], i.e., two octets.
     * If 2^16 - 2^8 <= a < 2^32, then a  is encoded as 0xff || 0xfe || a[0..31], i.e., six octets.
     * If 2^32 <= a < 2^64, then  a is encoded as 0xff || 0xff || a[0..63], i.e., ten octets.
     * For example, if a=2^16, the encoding of a  is
     * 11111111 11111110 00000000 00000001 00000000 00000000.
     */
    if (alen < SYMC_CCM_A_SMALL_LEN) {
        buf[idx++] = (td_u8)(alen >> SHIFT_8BITS);
        buf[idx++] = (td_u8)(alen);
    } else {
        buf[idx++] = 0xFF;
        buf[idx++] = 0xFE;
        buf[idx++] = (td_u8)(alen >> SHIFT_24BITS);
        buf[idx++] = (td_u8)(alen >> SHIFT_16BITS);
        buf[idx++] = (td_u8)(alen >> SHIFT_8BITS);
        buf[idx++] = (td_u8)alen;
    }

    return idx;
}

td_s32 drv_aead_ccm_add_a(td_u32 chn_num, compat_addr buf_phy, td_u32 buf_size)
{
    symc_hard_context *ctx = TD_NULL;
    compat_addr dma_pad;
    td_u8 *via_pad = TD_NULL;
    td_s32 ret;
    td_u32 count;

    func_enter();

    ctx = &g_hard_context[chn_num];
    chk_param_err_return((ctx->offset_pad + SYMC_CCM_A_HEAD_LEN) > SYMC_PAD_BUFFER_LEN);

    /* return success when alen is zero. */
    if (buf_size == 0x00) {
        return TD_SUCCESS;
    }

    addr_u64(dma_pad) = addr_u64(ctx->dma_pad) + ctx->offset_pad;
    via_pad = ctx->via_pad + ctx->offset_pad;
    (td_void)memset_s(via_pad, AES_BLOCK_SIZE * MUL_VAL_2, 0, AES_BLOCK_SIZE * MUL_VAL_2);

    /* add ccm a head */
    count = drv_aead_ccm_a_head(via_pad, buf_size);
    ret = drv_symc_add_inbuf(chn_num, dma_pad, count, SYMC_NODE_USAGE_IN_CCM_A | SYMC_NODE_USAGE_FIRST);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_add_inbuf, ret);
        return ret;
    }

    /* move buffer addr */
    ctx->offset_pad += count;
    addr_u64(dma_pad) += count;
    via_pad += count;

    /*  add the phy of A into node list. */
    ret = drv_symc_add_inbuf(chn_num, buf_phy, buf_size, SYMC_NODE_USAGE_IN_CCM_A);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_add_inbuf, ret);
        return ret;
    }

    /* if idx + Alen do not aligned with 16, padding 0 to the tail. */
    count = (buf_size + count) % AES_BLOCK_SIZE;
    if (count > 0) {
        /* add the padding phy of A into node list. */
        ret = drv_symc_add_inbuf(chn_num, dma_pad, AES_BLOCK_SIZE - count, SYMC_NODE_USAGE_IN_CCM_A);
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_symc_add_inbuf, ret);
            return ret;
        }
        ctx->offset_pad += AES_BLOCK_SIZE - count;
    }

    /* add ccm a last flag */
    drv_symc_add_buf_usage(chn_num, TD_TRUE, SYMC_NODE_USAGE_LAST);

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_aead_gcm_add_a(td_u32 chn_num, compat_addr buf_phy, td_u32 buf_size)
{
    symc_hard_context *ctx = TD_NULL;
    compat_addr dma_pad;
    td_u8 *via_pad = TD_NULL;
    td_s32 ret;
    td_u32 count = 0;

    func_enter();

    ctx = &g_hard_context[chn_num];
    chk_param_err_return((ctx->offset_pad + AES_BLOCK_SIZE) > SYMC_PAD_BUFFER_LEN);

    /* return success when alen is zero. */
    if (buf_size == 0x00) {
        func_exit();
        return TD_SUCCESS;
    }

    addr_u64(dma_pad) = addr_u64(ctx->dma_pad) + ctx->offset_pad;
    via_pad = ctx->via_pad + ctx->offset_pad;
    (td_void)memset_s(via_pad, AES_BLOCK_SIZE, 0, AES_BLOCK_SIZE);

    /* Add phy of A into node list. */
    ret = drv_symc_add_inbuf(chn_num, buf_phy, buf_size,
        SYMC_NODE_USAGE_IN_GCM_A | SYMC_NODE_USAGE_FIRST | SYMC_NODE_USAGE_GCM_FIRST);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_add_inbuf, ret);
        return ret;
    }

    /* if Alen do not aligned with 16, padding 0 to the tail. */
    count = (buf_size + count) % AES_BLOCK_SIZE;
    if (count > 0) {
        /* add the padding phy of A into node list. */
        ret = drv_symc_add_inbuf(chn_num, dma_pad, AES_BLOCK_SIZE - count, SYMC_NODE_USAGE_IN_GCM_A);
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_symc_add_inbuf, ret);
            return ret;
        }
        ctx->offset_pad += AES_BLOCK_SIZE - count;
    }

    /* add gcm a last flag */
    drv_symc_add_buf_usage(chn_num, TD_TRUE, SYMC_NODE_USAGE_LAST);

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_aead_gcm_add_clen(td_u32 chn_num, const td_u8 *clen, td_u32 clen_len)
{
    td_s32 ret;
    symc_hard_context *ctx = TD_NULL;
    symc_node_usage usage;
    compat_addr dma_pad;
    td_u8 *via_pad = TD_NULL;

    func_enter();

    ctx = &g_hard_context[chn_num];
    chk_param_err_return((ctx->offset_pad + SYMC_CCM_N_LEN) > SYMC_PAD_BUFFER_LEN);

    /* add Clen */
    addr_u64(dma_pad) = addr_u64(ctx->dma_pad) + ctx->offset_pad;
    via_pad = ctx->via_pad + ctx->offset_pad;

    usage = SYMC_NODE_USAGE_IN_GCM_LEN | SYMC_NODE_USAGE_LAST | SYMC_NODE_USAGE_FIRST;

    if (memcpy_s(via_pad, SYMC_GCM_CLEN_LEN, clen, clen_len) != EOK) {
        print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }

    ctx->offset_pad += SYMC_GCM_CLEN_LEN;

    ret = drv_symc_add_inbuf(chn_num, dma_pad, SYMC_GCM_CLEN_LEN, usage);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_add_inbuf, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_aead_get_tag(td_u32 chn_num, td_u32 *tag, td_u32 tag_buf_len)
{
    td_u32 i;

    func_enter();

    chk_param_err_return(g_symc_initialize != TD_TRUE);
    chk_param_err_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);
    chk_param_err_return(tag_buf_len > AEAD_TAG_SIZE);

    for (i = 0; i < AEAD_TAG_SIZE_IN_WORD; i++) {
        tag[i] = symc_read(chn_n_cipher_ccm_gcm_tag(chn_num, i));
        ot_cipher_dbg("chn_n_cipher_ccm_gcm_tag[0x%x]: 0x%x\n", chn_n_cipher_ccm_gcm_tag(chn_num, i), tag[i]);
    }

    func_exit();
    return TD_SUCCESS;
}
#endif

td_s32 drv_symc_node_check(symc_alg alg, symc_mode mode,
    td_u32 klen, td_u32 block_size, const symc_multi_pack *pack)
{
    td_u32 i;
    td_u32 total = 0;

    func_enter();

    chk_param_err_return(block_size == 0);

    for (i = 0; i < pack->num; i++) {
        /* Used the odd key must accord with conditions as follows: */
        if ((td_u32)pack->usage[i] & SYMC_NODE_USAGE_ODD_KEY) {
            /* 1. Only support aes ecb/cbc/cfb/ofb/ctr */
            if ((alg != SYMC_ALG_AES) || ((mode != SYMC_MODE_ECB) && (mode != SYMC_MODE_CBC) &&
                (mode != SYMC_MODE_CFB) && (mode != SYMC_MODE_OFB) && (mode != SYMC_MODE_CTR))) {
                print_errno(OT_ERR_CIPHER_INVALID_PARAM);
                return OT_ERR_CIPHER_INVALID_PARAM;
            }

            /* 2. Only support aes128 */
            chk_param_err_return(klen != AES_CCM_GCM_KEY_LEN);

            /* 3. each node length must be a multiple of 64 */
            chk_param_err_return((pack->len[i] % (AES_BLOCK_SIZE * WORD_WIDTH)) != 0);
        }

        /* each node length can't be zero, and check overflow. */
        chk_param_err_return((pack->len[i] == 0) || (pack->len[i] > addr_l32(pack->in[i]) + pack->len[i]));
        total += pack->len[i];
    }

    if ((alg != SYMC_ALG_NULL_CIPHER) && ((mode == SYMC_MODE_ECB) ||
        (mode == SYMC_MODE_CBC) || (mode == SYMC_MODE_CFB) || (mode == SYMC_MODE_OFB))) {
        /* The length of data depend on alg and mode, which limit to hardware
         * for ecb/cbc/ofb/cfb, the total data length must aligned with block size.
         * for ctr/ccm/gcm, support any data length.
         */
        if ((total % block_size) != 0) {
            print_errno(OT_ERR_CIPHER_INVALID_LEN);
            return OT_ERR_CIPHER_INVALID_LEN;
        }
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_symc_cfg(const cryp_symc_context *symc_ctx, td_u32 decrypt, symc_klen klen)
{
    symc_hard_context *ctx = TD_NULL;
    in_sym_chn_key_ctrl cipher_ctrl;
    td_u32 symc_alg_cfg_val[]  = {0x0, TDES, AES, SM4, 0x0, 0x0};  /* DES TDES AES SM4 SM1 DMA */
    td_u32 symc_mode_cfg_val[] = {ECB, CBC,  CFB, OFB, CTR, CCM, GCM, 0x0};  /* ECB CBC CFB OFB CTR CCM GCM CTS */
    td_u32 symc_klen_cfg_val[][KEY_CNT] = {
        {KEY_128BIT, KEY_192BIT, KEY_256BIT, 0},  /* 0-AES: 128, 192, 256 */
        {KEY_192BIT, 0, 0, KEY_128BIT},   /* 1-TDES: 3key 2key */
        {KEY_128BIT, 0, 0, 0}             /* 2-SM4: 128 */
    };

    func_enter();
    chk_ptr_err_return(symc_ctx);
    chk_param_err_return(g_symc_initialize != TD_TRUE);
    chk_param_err_return(symc_ctx->hard_chn >= CRYPTO_HARD_CHANNEL_MAX);
    chk_param_err_return(symc_ctx->alg >= SYMC_ALG_COUNT);
    chk_param_err_return(symc_ctx->mode > SYMC_MODE_GCM);
    chk_param_err_return(klen >= SYMC_KEY_LEN_COUNT);

    ctx = &g_hard_context[symc_ctx->hard_chn];

    /* record alg */
    ctx->alg = symc_ctx->alg;

    ot_cipher_dbg("symc configure, chn %u, alg %d, mode %d, dec %u, hard %u\n",
        symc_ctx->hard_chn, symc_ctx->alg, symc_ctx->mode, decrypt, symc_ctx->hard_key);

    if (symc_ctx->alg == SYMC_ALG_NULL_CIPHER) {
        drv_symc_set_dma(symc_ctx->hard_chn, 0x1);
    } else {
        drv_symc_set_dma(symc_ctx->hard_chn, 0x0);
        cipher_ctrl.u32 = symc_read(in_sym_chn_key_ctrl(symc_ctx->hard_chn));
        if (symc_ctx->alg == SYMC_ALG_AES) {
            cipher_ctrl.bits.sym_alg_key_len = symc_klen_cfg_val[0][klen]; /* 0 - aes */
        } else if (symc_ctx->alg == SYMC_ALG_TDES) {
            cipher_ctrl.bits.sym_alg_key_len = symc_klen_cfg_val[1][klen]; /* 1 - tdes */
        } else if (symc_ctx->alg == SYMC_ALG_SM4) {
            cipher_ctrl.bits.sym_alg_key_len = symc_klen_cfg_val[2][klen]; /* 2 - sm4 */
        } else {
            cipher_ctrl.bits.sym_alg_key_len = 0x1;
        }
        cipher_ctrl.bits.sym_alg_data_width = symc_ctx->width;
        cipher_ctrl.bits.sym_alg_decrypt = decrypt;
        cipher_ctrl.bits.sym_alg_sel = symc_alg_cfg_val[symc_ctx->alg];
        cipher_ctrl.bits.sym_alg_mode = symc_mode_cfg_val[symc_ctx->mode];
        ctx->mode = symc_ctx->mode;
        symc_write(in_sym_chn_key_ctrl(symc_ctx->hard_chn), cipher_ctrl.u32);
        ot_cipher_dbg("in_sym_chn_key_ctrl[0x%x]: 0x%x\n", in_sym_chn_key_ctrl(symc_ctx->hard_chn), cipher_ctrl.u32);
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_symc_set_isr_callback(td_u32 chn_num, callback_symc_isr callback, td_void *ctx)
{
    symc_hard_context *ot_ctx = TD_NULL;

    func_enter();

    chk_param_err_return(g_symc_initialize != TD_TRUE);
    chk_param_err_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    ot_ctx = &g_hard_context[chn_num];

    ot_ctx->callback = callback;
    ot_ctx->ctx = ctx;

    func_exit();
    return TD_SUCCESS;
}

static td_void drv_symc_enable_chn(td_u32 chn_num)
{
    in_sym_chn_ctrl in_ctrl;

    /* enable channel */
    in_ctrl.u32 = symc_read(in_sym_chn_ctrl(chn_num));
    in_ctrl.bits.sym_chn_en = 1;
    in_ctrl.bits.sym_chn_ss = SYMC_CFG_NON_SECURE;
    in_ctrl.bits.sym_chn_ds = SYMC_CFG_NON_SECURE;
    symc_write(in_sym_chn_ctrl(chn_num), in_ctrl.u32);
    ot_cipher_dbg("in_sym_chn_ctrl[0x%x]  :0x%x\n", in_sym_chn_ctrl(chn_num), in_ctrl.u32);
}

static td_void drv_symc_cfg_smmu(td_u32 chn_num)
{
    spacc_mmu_in_sym_clear mmu_in_clear;
    spacc_mmu_out_sym_clear mmu_out_clear;

    /* channel clear in mmu page (SW set this bit 1 and HW clean it to be 0) */
    mmu_in_clear.u32 = symc_read(SPACC_MMU_IN_SYM_CLEAR);
    mmu_in_clear.bits.sw_mmu_in_sym_clr = 1 << chn_num;
    symc_write(SPACC_MMU_IN_SYM_CLEAR, mmu_in_clear.u32);
    ot_cipher_dbg("SPACC_MMU_IN_SYM_CLEAR: 0x%x\n", mmu_in_clear.u32);

    /* channel clear out mmu page (SW set this bit 1 and HW clean it to be 0) */
    mmu_out_clear.u32 = symc_read(SPACC_MMU_OUT_SYM_CLEAR);
    mmu_out_clear.bits.sw_mmu_out_sym_clr = 1 << chn_num;
    symc_write(SPACC_MMU_OUT_SYM_CLEAR, mmu_out_clear.u32);
    ot_cipher_dbg("SPACC_MMU_OUT_SYM_CLEAR: 0x%x\n", mmu_out_clear.u32);
}

td_s32 drv_symc_start(td_u32 chn_num)
{
    symc_hard_context *ctx = TD_NULL;
    in_sym_chn_node_wr_point in_node_wr_ptr;
    out_sym_chn_node_wr_point out_node_wr_ptr;
    in_sym_chn_node_rd_point in_node_rd_ptr;
    out_sym_chn_node_rd_point out_node_rd_ptr;
    out_sym_chn_node_ctrl cipher_node_ctrl;
    td_u32 timeout_us = 1000;       /* 1000: timeout 1ms */
    td_u32 ptr;

    func_enter();

    chk_param_err_return(g_symc_initialize != TD_TRUE);
    chk_param_err_return(chn_num >= CRYPTO_HARD_CHANNEL_MAX);

    ctx = &g_hard_context[chn_num];

    ctx->done = TD_FALSE;

    drv_symc_enable_chn(chn_num);
    drv_symc_cfg_smmu(chn_num);

    /* configure out nodes */
    while (timeout_us > 0) {
        timeout_us--;
        out_node_wr_ptr.u32 = symc_read(out_sym_chn_node_wr_point(chn_num));
        out_node_rd_ptr.u32 = symc_read(out_sym_chn_node_rd_point(chn_num));
        if (out_node_wr_ptr.bits.out_sym_chn_node_wr_point == out_node_rd_ptr.bits.out_sym_chn_node_rd_point) {
            break;
        }
        osal_udelay(1);
    }

    ptr = out_node_wr_ptr.bits.out_sym_chn_node_wr_point + ctx->cnt_out;
    out_node_wr_ptr.bits.out_sym_chn_node_wr_point = ptr % SYMC_MAX_LIST_NUM;

    cipher_node_ctrl.u32 = symc_read(out_sym_chn_node_ctrl(chn_num));
    cipher_node_ctrl.bits.out_sym_chn_node_level = ctx->cnt_out;
    symc_write(out_sym_chn_node_ctrl(chn_num), cipher_node_ctrl.u32);
    ot_cipher_dbg("out_sym_chn_node_ctrl: \t0x%x\n", cipher_node_ctrl.u32);

    /* *** out_node_cfg.bits.cipher_out_node_mpackage_int_level = ctx->cnt_out *** */
    symc_write(out_sym_chn_node_wr_point(chn_num), out_node_wr_ptr.u32);
    ot_cipher_dbg("out_sym_chn_node_wr_point: 0x%x\n", out_node_wr_ptr.u32);

    /* configure in nodes */
    in_node_wr_ptr.u32 = symc_read(in_sym_chn_node_wr_point(chn_num));
    in_node_rd_ptr.u32 = symc_read(in_sym_chn_node_rd_point(chn_num));
    if (in_node_wr_ptr.bits.sym_chn_node_wr_point != in_node_rd_ptr.bits.sym_chn_node_rd_point) {
        ot_cipher_error("Error, in_chn %d is busy, Wr_ptr 0x%x, Rd_ptr 0x%x.\n",
            chn_num, in_node_wr_ptr.u32, in_node_rd_ptr.u32);
    }

    ptr = in_node_wr_ptr.bits.sym_chn_node_wr_point + ctx->cnt_in;
    in_node_wr_ptr.bits.sym_chn_node_wr_point = ptr % SYMC_MAX_LIST_NUM;

    /* The cache must be flushed when the calculation starts. */
    crypto_flush_dcache_all();

    symc_write(in_sym_chn_node_wr_point(chn_num), in_node_wr_ptr.u32);
    ot_cipher_dbg("in_sym_chn_node_wr_point: 0x%x\n", in_node_wr_ptr.u32);

    /* all the nodes are processed, retset the cnount to zero */
    ctx->cnt_in = 0;
    ctx->cnt_out = 0;
    ctx->offset_pad = 0;

    func_exit();
    return TD_SUCCESS;
}

td_s32 drv_symc_wait_done(td_u32 chn_num, td_u32 timeout)
{
    td_s32 ret;
    symc_hard_context *ctx __attribute__((unused));

#ifdef CRYPTO_OS_INT_SUPPORT
    ctx = &g_hard_context[chn_num];
    ret = crypto_queue_wait_timeout(&ctx->queue, crypto_queue_callback_func, &ctx->done, timeout);
    if (ret == 0) {
        print_func_errno(crypto_queue_wait_timeout, OT_ERR_CIPHER_TIMEOUT);
        drv_symc_get_err_code(chn_num);
        return OT_ERR_CIPHER_TIMEOUT;
    }
#else /* !CRYPTO_OS_INT_SUPPORT */
    td_u32 i;

    for (i = 0; i < timeout; i++) {
        ret = drv_symc_done_try(chn_num);
        if (ret == TD_SUCCESS) {
            break;
        }
        if (i <= MS_TO_US) {
            crypto_udelay(1); /* short waiting for 1000 us */
        } else {
            crypto_msleep(1); /* long waiting for 5000 ms */
        }
    }

    if (timeout <= i) {
        print_errno(OT_ERR_CIPHER_TIMEOUT);
        drv_symc_get_err_code(chn_num);
        return OT_ERR_CIPHER_TIMEOUT;
    }
#endif

    ret = drv_symc_check_sec_mmu();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_check_sec_mmu, ret);
        drv_symc_get_err_code(chn_num);
        return ret;
    }

    ret = drv_symc_check_nosec_mmu();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_check_nosec_mmu, ret);
        drv_symc_get_err_code(chn_num);
    }

    return ret;
}

/* proc function begin */
#ifdef OT_PROC_SUPPORT
static td_void drv_symc_proc_status_set_open(symc_chn_status *status, td_u32 chnnel_id)
{
    if (g_hard_context[chnnel_id].open == TD_TRUE) {
        status[chnnel_id].open = "open ";
    } else {
        status[chnnel_id].open = "close";
    }
}

static td_void drv_symc_proc_status_set_alg(symc_chn_status *status,
    td_u32 chnnel_id, td_u32 alg)
{
    switch (alg) {
        case AES:
            status[chnnel_id].alg = "AES ";
            break;
#ifdef CHIP_SM4_SUPPORT
        case SM4:
            status[chnnel_id].alg = "SM4 ";
            break;
#endif
        default:
            status[chnnel_id].alg = "AES ";
            break;
    }
}

static td_void drv_symc_proc_status_set_mode(symc_chn_status *status,
    td_u32 chnnel_id, td_u32 work_mode)
{
    switch (work_mode) {
        case ECB:
            status[chnnel_id].mode = "ECB ";
            break;
        case CBC:
            status[chnnel_id].mode = "CBC ";
            break;
        case CFB:
            status[chnnel_id].mode = "CFB ";
            break;
        case OFB:
            status[chnnel_id].mode = "OFB ";
            break;
        case CTR:
            status[chnnel_id].mode = "CTR ";
            break;
        case CCM:
            status[chnnel_id].mode = "CCM ";
            break;
        case GCM:
            status[chnnel_id].mode = "GCM ";
            break;
        default:
            status[chnnel_id].mode = "ECB ";
            break;
    }
}

static td_void drv_symc_proc_status_set_key_len(symc_chn_status *status,
    td_u32 chnnel_id, td_u32 alg, td_u32 key_len)
{
    if (alg == AES) {
        switch (key_len) {
            case KEY_128BIT:
                status[chnnel_id].klen = AES_KEY_128BIT;
                break;
            case KEY_192BIT:
                status[chnnel_id].klen = AES_KEY_192BIT;
                break;
            case KEY_256BIT:
                status[chnnel_id].klen = AES_KEY_256BIT;
                break;
            default:
                status[chnnel_id].klen = 0;
                break;
        }
    } else if (alg == SM4) {
#ifdef CHIP_SM4_SUPPORT
        status[chnnel_id].klen = SM4_KEY_SIZE;
#endif
    } else {
        status[chnnel_id].klen = 0;
    }
}

static td_void drv_symc_get_ivout(symc_chn_status *status, td_u32 chn)
{
    td_u32 i;
    td_u32 val;

    /* out iv */
    for (i = 0; i < AES_IV_SIZE * 2; i += 8) { /* iv string have 32 bytes, a word have 8bytes */
        val = symc_read(chn_n_cipher_ivout(chn) + i);
        hex2str(status[chn].iv + i, (td_u8)(val & 0xFF));
        hex2str(status[chn].iv + i + 2, (td_u8)((val >> 8) & 0xFF));  /* byte2~3: bit 15..8  */
        hex2str(status[chn].iv + i + 4, (td_u8)((val >> 16) & 0xFF)); /* byte4~5: bit 23..16 */
        hex2str(status[chn].iv + i + 6, (td_u8)((val >> 24) & 0xFF)); /* byte6~7: bit 31..24 */
    }
}

static td_void drv_symc_get_in_node(symc_chn_status *status, td_u32 chn)
{
    in_sym_chn_node_wr_point in_node_wr_ptr;
    in_sym_chn_node_rd_point in_node_rd_ptr;
    in_sym_chn_node_length in_node_depth;

    /* in node */
    in_node_wr_ptr.u32 = symc_read(in_sym_chn_node_wr_point(chn));
    in_node_rd_ptr.u32 = symc_read(in_sym_chn_node_rd_point(chn));
    in_node_depth.u32 = symc_read(in_sym_chn_node_length(chn));
    status[chn].in_node_head = symc_read(in_sym_chn_node_start_addr_l(chn));
    status[chn].in_node_depth = in_node_depth.bits.sym_chn_node_length;
    status[chn].in_node_rptr = in_node_rd_ptr.bits.sym_chn_node_rd_point;
    status[chn].in_node_wptr = in_node_wr_ptr.bits.sym_chn_node_wr_point;
    status[chn].inaddr = symc_read(dbg_in_sym_chn_rd_addr_l(chn));
}

static td_void drv_symc_get_out_node(symc_chn_status *status, td_u32 chn)
{
    out_sym_chn_node_wr_point out_node_wr_ptr;
    out_sym_chn_node_rd_point out_node_rd_ptr;
    out_sym_chn_node_length out_node_depth;

    /* out node */
    out_node_wr_ptr.u32 = symc_read(out_sym_chn_node_wr_point(chn));
    out_node_rd_ptr.u32 = symc_read(out_sym_chn_node_rd_point(chn));
    out_node_depth.u32 = symc_read(out_sym_chn_node_length(chn));
    status[chn].out_node_head = symc_read(out_sym_chn_node_start_addr_l(chn));
    status[chn].out_node_depth = out_node_depth.bits.out_sym_chn_node_length;
    status[chn].out_node_rptr = out_node_rd_ptr.bits.out_sym_chn_node_rd_point;
    status[chn].out_node_wptr = out_node_wr_ptr.bits.out_sym_chn_node_wr_point;
    status[chn].outaddr = symc_read(dbg_out_sym_chn_rd_addr_l(chn));
}

static td_void drv_symc_get_smmu(symc_chn_status *status, td_u32 chn)
{
    in_sym_chn_ctrl in_chn_ctrl;
    out_sym_chn_ctrl out_chn_ctrl;

    /* SMMU Enable */
    in_chn_ctrl.u32 = symc_read(in_sym_chn_ctrl(chn));
    out_chn_ctrl.u32 = symc_read(out_sym_chn_ctrl(chn));
    status[chn].smmu_enable = in_chn_ctrl.bits.sym_chn_mmu_en;
    status[chn].smmu_enable &= out_chn_ctrl.bits.out_sym_chn_mmu_en;
}

static td_void drv_symc_get_key_ctrl(symc_chn_status *status, td_u32 chn)
{
    in_sym_chn_key_ctrl ctrl;

    ctrl.u32 = symc_read(in_sym_chn_key_ctrl(chn));
    status[chn].decrypt = ctrl.bits.sym_alg_decrypt;
    status[chn].ksrc = ctrl.bits.sym_key_chn_id;
    drv_symc_proc_status_set_alg(status, chn, ctrl.bits.sym_alg_sel);
    drv_symc_proc_status_set_mode(status, chn, ctrl.bits.sym_alg_mode);
    drv_symc_proc_status_set_key_len(status, chn,
        ctrl.bits.sym_alg_sel, ctrl.bits.sym_alg_key_len);
}

static td_void drv_symc_get_int_sta(symc_chn_status *status, td_u32 chn)
{
    out_sym_chan_raw_int out_int_raw;
    out_sym_chan_raw_int_en out_int_en;
    out_sym_chan_int out_int_status;

    out_int_en.u32 = symc_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN);
    out_int_raw.u32 = symc_read(OUT_SYM_CHAN_RAW_LAST_NODE_INT);
    out_int_status.u32 = symc_read(OUT_SYM_CHAN_LAST_NODE_INT);

    status[chn].int_raw = (out_int_raw.bits.out_sym_chan_raw_int >> chn) & 0x1;
    status[chn].int_en = (out_int_en.bits.out_sym_chan_int_en >> chn) & 0x1;
    status[chn].int_status = (out_int_status.bits.out_sym_chan_int >> chn) & 0x1;
}

static void drv_symc_set_module_status(symc_module_status *module)
{
    spacc_ie int_en;
    td_u32 info = 0;
    td_u32 int_valid = 0;
    td_u32 int_num = 0;
    const char *name = TD_NULL;

    module_get_attr(CRYPTO_MODULE_ID_SYMC, &int_valid, &int_num, &name);
    module_get_info(CRYPTO_MODULE_ID_SYMC, &info, MODULE_INFO_RESET_VALID);
    int_en.u32 = symc_read(SPACC_IE);

    module->allow_reset = info;
    module->sec_cpu = crypto_is_sec_cpu();
    module->int_num = int_num;
    module->name = name;
    module->smmu_base = symc_read(module->sec_cpu ? SPACC_MMU_SEC_TLB : SPACC_MMU_NOSEC_TLB);
    module->err_code = symc_read(sym_calc_ctrl_check_err(SPACC_CPU_CUR));
    module->int_en = int_en.bits.spacc_ie_tee;
}

static void drv_symc_set_chn_status(symc_chn_status *status, td_u32 status_len)
{
    td_u32 i;
    td_u32 sec_chn_cfg;

    if ((status == TD_NULL) || (status_len != sizeof(symc_chn_status) * CRYPTO_HARD_CHANNEL_MAX)) {
        return;
    }

    sec_chn_cfg = symc_read(SPACC_SYM_CHN_LOCK);

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if (((0x1 << i) & CIPHER_HARD_CHANNEL_MASK) == 0x00) {
            continue;
        }

        drv_symc_proc_status_set_open(status, i);

        /* secure channel */
        status[i].secure = chn_who_used_get(sec_chn_cfg, i);

        drv_symc_get_key_ctrl(status, i);

        drv_symc_get_ivout(status, i);

        drv_symc_get_int_sta(status, i);

        drv_symc_get_in_node(status, i);

        drv_symc_get_out_node(status, i);

        drv_symc_get_smmu(status, i);
    }
}

void drv_symc_proc_status(symc_chn_status *status, td_u32 status_len, symc_module_status *module)
{
    drv_symc_set_module_status(module);

    drv_symc_set_chn_status(status, status_len);

    return;
}
#endif
/* proc function end */

void drv_symc_get_capacity(symc_capacity *capacity)
{
    func_enter();

    /* the mode depend on alg, which limit to hardware
     * aes support ecb/cbc/cfb/ofb/ctr/ccm/gcm
     * sm4 support ecb/cbc/ctr
     */
    (td_void)memset_s(capacity, sizeof(symc_capacity), 0, sizeof(symc_capacity));

    /* AES */
    capacity->aes_ecb = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_cbc = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_ofb = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_cfb = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_ctr = CRYPTO_CAPACITY_SUPPORT;

#ifdef CHIP_AES_CCM_GCM_SUPPORT
    capacity->aes_ccm = CRYPTO_CAPACITY_SUPPORT;
    capacity->aes_gcm = CRYPTO_CAPACITY_SUPPORT;
#endif

    /* TDES */
    capacity->tdes_ecb = CRYPTO_CAPACITY_SUPPORT;
    capacity->tdes_cbc = CRYPTO_CAPACITY_SUPPORT;
    capacity->tdes_ofb = CRYPTO_CAPACITY_SUPPORT;
    capacity->tdes_cfb = CRYPTO_CAPACITY_SUPPORT;

    /* SM4 */
#ifdef CHIP_SM4_SUPPORT
    capacity->sm4_ecb = CRYPTO_CAPACITY_SUPPORT;
    capacity->sm4_cbc = CRYPTO_CAPACITY_SUPPORT;
    capacity->sm4_ctr = CRYPTO_CAPACITY_SUPPORT;
#endif

    /* DMA */
    capacity->dma = CRYPTO_CAPACITY_SUPPORT;

    return;
}

/** @} */ /** <!-- ==== API Code end ==== */

#endif  // End of CHIP_SYMC_VER_V300
