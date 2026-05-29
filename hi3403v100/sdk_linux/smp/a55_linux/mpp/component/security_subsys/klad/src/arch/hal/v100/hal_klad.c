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

#include "hal_klad.h"
#include "klad_cfg.h"
#include "klad_reg_base.h"
#include "klad_reg_define.h"
#include "ot_debug_klad.h"
#include "hal_rkp.h"
#include "drv_osal_init.h"
#include "drv_lib.h"
#include "securec.h"

#define HKL_OP_TIMEOUT              6000
#define WORD_WIDTH                  4
#define HKL_DATA_IN_8BYTES          8
#define HKL_DATA_IN_16BYTES         16
#define HKL_DATA_IN_24BYTES         24
#define HKL_DATA_IN_32BYTES         32

typedef enum {
    HKL_COM_LOCK_STAT_UNLOCK    = 0x00,
    HKL_COM_LOCK_STAT_TEE       = 0xa5,
    HKL_COM_LOCK_STAT_REE       = 0xaa,
} hkl_com_lock_stat;

typedef enum {
    HKL_COM_ALG_AES = 0x01,
    HKL_COM_ALG_SM4 = 0x02,
} hkl_com_alg;

typedef enum {
    HKL_COM_KEY_SIZE_128BIT = 0x00,
    HKL_COM_KEY_SIZE_256BIT = 0x01,
} hkl_com_key_size;

typedef struct {
} hkl_com_key;

typedef enum {
    HKL_KEY_TYPE_EVEN = 0x00,
    HKL_KEY_TYPE_ODD  = 0x01,
} hkl_key_type;

typedef enum {
    HKL_DSC_CODE_AES = 0x20,
    HKL_DSC_CODE_SM4 = 0x50,
} hkl_dsc_code;

typedef enum {
    HKL_CLR_KEY_SIZE_64BIT  = 0x00,
    HKL_CLR_KEY_SIZE_128BIT = 0x01,
    HKL_CLR_KEY_SIZE_192BIT = 0x02,
    HKL_CLR_KEY_SIZE_256BIT = 0x03,
} hkl_clr_key_size;

typedef struct {
    td_u32 slot_num;
    hkl_key_type type;
    hkl_dsc_code dsc_code;
    hkl_clr_key_size clr_size;
    td_u8 key[HKL_DATA_IN_16BYTES];
} hkl_clr_route;

typedef struct {
    td_u32 slot_num;
    td_u32 level;
    hkl_key_type type;
    hkl_dsc_code dsc_code;
    hkl_com_alg alg;                    /* klad level */
    hkl_com_key_size com_size;      /* com key size */
    td_u8 key[HKL_DATA_IN_16BYTES];
} hkl_com_route;

static td_u8 *g_klad_reg_base = TD_NULL;

#ifdef KLAD_INT_SUPPORT
typedef struct {
    td_bool done;
    KLAD_QUEUE_HEAD queue;
} klad_hard_context;

/* 0 - clear route, 1 - common route */
#define KLAD_CLR_IDX        0
#define KLAD_COM_IDX        1
static klad_hard_context g_klad_ctx[KLAD_ROUTE_NUM];
#endif

#define check_hkl_reg_base_fail_return()        \
    do {                                        \
        if (g_klad_reg_base == TD_NULL) {       \
            ot_klad_error("klad isn't init\n"); \
            return OT_ERR_KLAD_NOT_INIT;        \
        }                                       \
    } while (0)

static inline td_u32 _hkl_read(td_u32 offset)
{
    td_u32 val;
    val = klad_read(g_klad_reg_base + (offset));
    ot_klad_info(" offset %04x, read  %08x\n", offset, val);
    return val;
}

static inline td_void _hkl_write(td_u32 offset, td_u32 data)
{
    ot_klad_info("offset %04x, write %08x\n", offset, data);
    klad_write(g_klad_reg_base + (offset), data);
}

#ifdef KLAD_INT_SUPPORT
static td_s32 _hkl_queue_callback_func(const td_void *param)
{
    return *(td_bool *)param;
}

static td_void _hkl_print_int_status(td_void)
{
    kl_int_en int_en;
    kl_int_raw int_raw;
    kl_int int_sta;

    int_en.u32 = _hkl_read(KL_INT_EN);
    ot_klad_error("klad int en %08x\n", int_en.u32);

    int_raw.u32 = _hkl_read(KL_INT_RAW);
    ot_klad_error("klad int raw %08x\n", int_raw.u32);

    int_sta.u32 = _hkl_read(KL_INT);
    ot_klad_error("klad int sta %08x\n", int_sta.u32);
}

static td_void _hkl_enable_int(td_bool is_int)
{
    kl_int_en int_en;
    int_en.u32 = 0;
    int_en.bits.kl_int_en = is_int;
    int_en.bits.kl_lock_int_en = is_int;
    _hkl_write(KL_INT_EN, int_en.u32);
}

static td_void _hkl_clean_interrupt(td_void)
{
    kl_int_raw int_raw;

    int_raw.u32 = 0;
    int_raw.bits.kl_int_raw = 1;
    int_raw.bits.kl_com_lock_int_raw = 1;
    _hkl_write(KL_INT_RAW, int_raw.u32);
}

static td_s32 _hkl_interrupt_isr(td_s32 irq, td_void *dev_id)
{
    kl_int_raw int_raw;
    td_u32 idx;

    ot_unused(dev_id);

    _hkl_clean_interrupt();

    int_raw.u32 = _hkl_read(KL_INT_RAW);
    if (int_raw.bits.kl_int_num == KLAD_CLR_ROUTE) {
        idx = KLAD_CLR_IDX;
    } else {
        idx = KLAD_COM_IDX;
    }

    if (g_klad_ctx[idx].done == TD_FALSE) {
        g_klad_ctx[idx].done = TD_TRUE;
        klad_queue_wait_up(&g_klad_ctx[idx].queue);
    }

    return KLAD_IRQ_HANDLED;
}

static td_s32 _hkl_interrupt_init(td_void)
{
    td_s32 ret;
    td_u32 i;

    (td_void)memset_s(g_klad_ctx, sizeof(g_klad_ctx), 0, sizeof(g_klad_ctx));

    /* enable interrupt */
    _hkl_enable_int(TD_TRUE);

    /* the history of interception may trigger the system to
     * call the irq function before initialization
     * when register interrupt, this will cause a system abort. */
    _hkl_clean_interrupt();

    /* register interrupt */
    ret = klad_request_irq(ot_klad_get_klad_irq(), _hkl_interrupt_isr, KLAD_INT_NAME);
    ot_klad_func_fail_return(klad_request_irq, ret != TD_SUCCESS, OT_ERR_KLAD_FAILED_OPERATE);

    /* init queue */
    for (i = 0; i < KLAD_ROUTE_NUM; i++) {
        klad_queue_init(&g_klad_ctx[i].queue);
    }
    return ret;
}

static td_void _hkl_interrupt_deinit(td_void)
{
    td_u32 i;
    for (i = 0; i < KLAD_ROUTE_NUM; i++) {
        klad_queue_destroy(&g_klad_ctx[i].queue);
    }

    /* free irq */
    klad_free_irq(ot_klad_get_klad_irq(), KLAD_INT_NAME);

    _hkl_enable_int(TD_FALSE);
}
#endif

static td_s32 _hkl_com_lock_idle(td_void)
{
    kl_com_lock_info lock_info;
    td_u32 time_out = HKL_OP_TIMEOUT;

    while (time_out--) {
        lock_info.u32 = _hkl_read(KL_COM_LOCK_INFO);
        if (lock_info.bits.kl_com_lock_busy == 0x02) { /* 1 - busy, 2 - idle */
            break;
        }
    }

    if (time_out == 0) {
        ot_klad_error("common klad op timeout!");
        return OT_ERR_KLAD_TIMEOUT;
    }
    return TD_SUCCESS;
}

static hkl_com_lock_stat _hkl_com_lock_stat(td_void)
{
    kl_com_lock_status status;

    status.u32 = _hkl_read(KL_COM_LOCK_STATUS);
    return status.bits.kl_com_lock_stat;
}

static td_void _hkl_lock_ctrl(td_void)
{
    kl_lock_ctrl lock_ctl;

    lock_ctl.u32 = _hkl_read(KL_LOCK_CTRL);
    lock_ctl.bits.kl_lock = 1;
    _hkl_write(KL_LOCK_CTRL, lock_ctl.u32);
}

static td_void _hkl_unlock_ctrl(td_void)
{
    kl_unlock_ctrl unlock_ctl;

    unlock_ctl.u32 = _hkl_read(KL_UNLOCK_CTRL);
    unlock_ctl.bits.kl_unlock = 1;
    _hkl_write(KL_UNLOCK_CTRL, unlock_ctl.u32);
}

static td_s32 _hkl_com_wait_idle(void)
{
    kl_state state;
    td_u32 time_out = HKL_OP_TIMEOUT;

    while (time_out--) {
        state.u32 = _hkl_read(KL_STATE);
        if (state.bits.kl_busy == 0x02) {
            break;
        }
        klad_udelay(1);
    }

    if (time_out == 0) {
        ot_klad_error("hkl op timeout!");
        return OT_ERR_KLAD_TIMEOUT;
    }

    return TD_SUCCESS;
}

static td_s32 _hkl_com_route_op_finish(void)
{
    td_s32 ret;

#ifdef KLAD_INT_SUPPORT
    ret = klad_queue_wait_timeout(&g_klad_ctx[KLAD_COM_IDX].queue,
        _hkl_queue_callback_func, &g_klad_ctx[KLAD_COM_IDX].done, HKL_OP_TIMEOUT);
    if (ret == 0) {
        ot_klad_error("Failed call klad_queue_wait_timeout\n");
        _hkl_print_int_status();
        return OT_ERR_KLAD_TIMEOUT;
    }
#else
    ret = _hkl_com_wait_idle();
    ot_klad_func_fail_return(_hkl_com_wait_idle, ret != TD_SUCCESS, ret);
#endif

    return TD_SUCCESS;
}

static td_void _hkl_set_sec_cfg(td_void)
{
    kl_key_sec_cfg reg;

    reg.u32 = _hkl_read(KL_KEY_SEC_CFG);
    reg.bits.dest_sec = 1;
    reg.bits.dest_nsec = 1;
    reg.bits.src_sec = 1;
    reg.bits.src_nsec = 1;
    reg.bits.key_sec = 1;
    _hkl_write(KL_KEY_SEC_CFG, reg.u32);
}

static td_void _hkl_set_com_ctrl(td_u32 level, td_u32 alg, td_u32 key_size)
{
    kl_com_ctrl reg;

    reg.u32 = _hkl_read(KL_COM_CTRL);
    reg.bits.kl_com_start = 1;
    reg.bits.kl_com_level_sel = level;
    reg.bits.kl_com_alg_sel = alg;
    reg.bits.kl_com_key_size = key_size;
    _hkl_write(KL_COM_CTRL, reg.u32);
}

static td_s32 _hkl_check_error(td_void)
{
    kl_error kl_err;
    kc_error kc_err;

    kl_err.u32 = _hkl_read(KL_ERROR);
    if (kl_err.u32 != 0) {
        ot_klad_error("read KL_ERROR failed %x\n", kl_err.u32);
        return OT_ERR_KLAD_FAILED_OPERATE;
    }

    kc_err.u32 = _hkl_read(KC_ERROR);
    if (kc_err.u32 != 0) {
        ot_klad_error("read KC_ERROR failed %x\n", kc_err.u32);
        return OT_ERR_KLAD_FAILED_OPERATE;
    }
    return TD_SUCCESS;
}

static td_void _hkl_set_data_in(const td_u8 *data, td_u32 data_len)
{
    td_u32 i;
    td_u32 u32_data;

    if (data_len % WORD_WIDTH != 0) {
        ot_klad_error("Invalid date length %d\n", data_len);
        return;
    }

    for (i = 0; i < data_len; i += WORD_WIDTH) {
        u32_data = ((td_u32)data[i + 3] << 24) | /* 3 arr index, 24 bits shift */
                   ((td_u32)data[i + 2] << 16) | /* 2 arr index, 16 bits shift */
                   ((td_u32)data[i + 1] << 8) |  /* 1 arr index, 8 bits shift */
                   ((td_u32)data[i]);
        _hkl_write(KL_DATA_IN_0 + i, u32_data);
    }
}

static td_void _hkl_set_key_addr(td_u32 slot_num, hkl_key_type type)
{
    kl_key_addr addr;
    addr.u32 = ((slot_num << 1) | (td_u32)type); /* addr.key_addr is 10bits, can't assign */

    _hkl_write(KL_KEY_ADDR, addr.u32);
}

static td_void _hkl_set_key_cfg(hkl_dsc_code dsc)
{
    kl_key_cfg cfg;
    cfg.u32 = 0;
    cfg.bits.dsc_code = dsc;
    _hkl_write(KL_KEY_CFG, cfg.u32);
}

/* wait clear klad route idle */
static td_s32 _hkl_clr_route_idle(td_void)
{
    kl_clr_ctrl ctrl;
    td_u32 timeout_cnt = HKL_OP_TIMEOUT;

    while (timeout_cnt--) {
        ctrl.u32 = _hkl_read(KL_CLR_CTRL);
        if (ctrl.bits.kl_clr_start == 0) {
            break;
        }
        klad_udelay(1);
    }

    if (timeout_cnt == 0) {
        ot_klad_error("hkl wait op timeout!\n");
        return OT_ERR_KLAD_TIMEOUT;
    }

    return TD_SUCCESS;
}

/* wait clear klad route operate finish & check operate status */
static td_s32 _hkl_clr_route_op_finish(td_void)
{
    td_s32 ret;

#ifdef KLAD_INT_SUPPORT
    ret = klad_queue_wait_timeout(&g_klad_ctx[KLAD_CLR_IDX].queue,
        _hkl_queue_callback_func, &g_klad_ctx[KLAD_CLR_IDX].done, HKL_OP_TIMEOUT);
    if (ret == 0) {
        ot_klad_error("Failed call klad_queue_wait_timeout\n");
        _hkl_print_int_status();
        return OT_ERR_KLAD_TIMEOUT;
    }
#else
    ret = _hkl_clr_route_idle();
    ot_klad_func_fail_return(_hkl_clr_route_idle, ret != TD_SUCCESS, ret);
#endif

    ret = _hkl_check_error();
    ot_klad_func_fail_return(_hkl_check_error, ret != TD_SUCCESS, ret);

    return ret;
}

static td_void _hkl_startup_clr_route(hkl_clr_key_size key_size)
{
    kl_clr_ctrl ctrl;
    ctrl.u32 = 0;
    ctrl.bits.kl_clr_key_size = key_size;
    ctrl.bits.kl_clr_start = 1;
    _hkl_write(KL_CLR_CTRL, ctrl.u32);
}

static td_s32 _hal_com_lock(td_void)
{
    td_s32 ret;
    td_u32 timeout = HKL_OP_TIMEOUT;
    hkl_com_lock_stat stat;
    hkl_com_lock_stat target_stat = klad_is_secure_cpu() ? HKL_COM_LOCK_STAT_TEE : HKL_COM_LOCK_STAT_REE;

    stat = _hkl_com_lock_stat();
    if (stat == target_stat) {
        return TD_SUCCESS;
    }

    while (timeout--) {
        ret = _hkl_com_lock_idle();
        ot_klad_func_fail_return(_hkl_com_lock_idle, ret != TD_SUCCESS, ret);

        _hkl_lock_ctrl();

        ret = _hkl_com_lock_idle();
        ot_klad_func_fail_return(_hkl_com_lock_idle, ret != TD_SUCCESS, ret);

        stat = _hkl_com_lock_stat();
        if (stat == target_stat) {
            break;
        }
    }

    if (timeout == 0) {
        ot_klad_error("common klad op timeout!");
        return OT_ERR_KLAD_TIMEOUT;
    }

    return ret;
}

static td_s32 _hal_com_unlock(td_void)
{
    td_s32 ret;
    td_u32 timeout = HKL_OP_TIMEOUT;
    hkl_com_lock_stat stat;
    hkl_com_lock_stat cur_stat = klad_is_secure_cpu() ? HKL_COM_LOCK_STAT_TEE : HKL_COM_LOCK_STAT_REE;

    stat = _hkl_com_lock_stat();
    if (stat == HKL_COM_LOCK_STAT_UNLOCK) {
        return TD_SUCCESS;
    } else if (stat != cur_stat) {
        ot_klad_error("klad owner don't match %s\n", (stat == HKL_COM_LOCK_STAT_REE) ? "REE" : "TEE");
        return OT_ERR_KLAD_INVALID_OWNER;
    }

    /* unlock when stat = cur_stat */
    while (timeout--) {
        ret = _hkl_com_lock_idle();
        ot_klad_func_fail_return(_hkl_com_lock_idle, ret != TD_SUCCESS, ret);

        _hkl_unlock_ctrl();

        ret = _hkl_com_lock_idle();
        ot_klad_func_fail_return(_hkl_com_lock_idle, ret != TD_SUCCESS, ret);

        stat = _hkl_com_lock_stat();
        if (stat == HKL_COM_LOCK_STAT_UNLOCK) {
            break;
        }
    }

    if (timeout == 0) {
        ot_klad_error("common klad op timeout!");
        return OT_ERR_KLAD_TIMEOUT;
    }

    return ret;
}

static td_s32 _hal_klad_cfg_com_route(hkl_com_route *com_route,
    td_u32 level, td_u32 slot_num, hkl_key_type type, const klad_common_slot *common_slot)
{
    td_s32 ret;
    const klad_common_key *com_key = &common_slot->klad_key[level];

    (td_void)memset_s(com_route, sizeof(hkl_com_route), 0, sizeof(hkl_com_route));
    com_route->level = level;
    com_route->slot_num = slot_num;
    com_route->type = type;

    if (common_slot->crypto_alg == OT_KLAD_CRYPTO_ALG_AES) {
        com_route->dsc_code = HKL_DSC_CODE_AES;
#ifdef KLAD_SM4_SUPPORT
    } else if (common_slot->crypto_alg == OT_KLAD_CRYPTO_ALG_SM4) {
        com_route->dsc_code = HKL_DSC_CODE_SM4;
#endif
    } else {
        ot_klad_error("Invalid crypto alg %d\n", common_slot->crypto_alg);
        return OT_ERR_KLAD_INVALID_PARAM;
    }

    if (com_key->alg == OT_KLAD_ALG_TYPE_AES) {
        com_route->alg = HKL_COM_ALG_AES;
#ifdef KLAD_SM4_SUPPORT
    } else if (com_key->alg == OT_KLAD_ALG_TYPE_SM4) {
        com_route->alg = HKL_COM_ALG_SM4;
#endif
    } else {
        ot_klad_error("Invalid klad alg %d\n", com_key->alg);
        return OT_ERR_KLAD_INVALID_PARAM;
    }

    if (com_key->key_size == HKL_DATA_IN_16BYTES) {
        com_route->com_size = HKL_COM_KEY_SIZE_128BIT;
    } else if (com_key->key_size == HKL_DATA_IN_32BYTES) {
        com_route->com_size = HKL_COM_KEY_SIZE_256BIT;
    } else {
        ot_klad_error("Invalid key size %d\n", com_key->key_size);
        return OT_ERR_KLAD_INVALID_PARAM;
    }

    if (type == HKL_KEY_TYPE_EVEN) {
        ret = memcpy_s(com_route->key, sizeof(com_route->key),
            com_key->key, HKL_DATA_IN_16BYTES);
    } else {
        ret = memcpy_s(com_route->key, sizeof(com_route->key),
            &com_key->key[HKL_DATA_IN_16BYTES], OT_KLAD_MAX_KEY_LEN - HKL_DATA_IN_16BYTES);
    }
    ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);

    return TD_SUCCESS;
}

static td_s32 _hal_klad_startup_com_route(const hkl_com_route *com_route)
{
    td_s32 ret;

#ifdef KLAD_INT_SUPPORT
    g_klad_ctx[KLAD_COM_IDX].done = TD_FALSE;
#endif

    /* step 1. wait klad idle */
    ret = _hkl_com_wait_idle();
    ot_klad_func_fail_return(_hkl_com_wait_idle, ret != TD_SUCCESS, ret);

    /* step 2. set key data */
    _hkl_set_data_in(com_route->key, HKL_DATA_IN_16BYTES);

    /* step 3. bind key slot, set even&odd key */
    _hkl_set_key_addr(com_route->slot_num, com_route->type);

    /* step 4. set dsc alg */
    _hkl_set_key_cfg(com_route->dsc_code);

    /* step 5. set secure attribute */
    _hkl_set_sec_cfg();

    /* step 6. set klad level, alg, key size & startup clear route */
    _hkl_set_com_ctrl(com_route->level, com_route->alg, com_route->com_size);

    /* step 7. wait klad operation finish */
    ret = _hkl_com_route_op_finish();
    ot_klad_func_fail_return(_hkl_com_route_op_finish, ret != TD_SUCCESS, ret);

    /* step 8. wait klad idle */
    ret = _hkl_check_error();
    ot_klad_func_fail_return(_hkl_check_error, ret != TD_SUCCESS, ret);

    return ret;
}

static td_s32 _hal_klad_run_com_route(td_u32 slot_num, const klad_common_slot *common_slot)
{
    td_u32 i;
    td_s32 ret;
    hkl_com_route com_route;

    for (i = 0; i < common_slot->klad_level; i++) {
        if (common_slot->klad_key[i].key_size == HKL_DATA_IN_16BYTES) {
            /* mcipher default used even key */
            ret = _hal_klad_cfg_com_route(&com_route, i, slot_num, HKL_KEY_TYPE_EVEN, common_slot);
            ot_klad_func_fail_return(_hal_klad_cfg_com_route, ret != TD_SUCCESS, ret);

            ret = _hal_klad_startup_com_route(&com_route);
            ot_klad_func_fail_return(_hal_klad_startup_com_route, ret != TD_SUCCESS, ret);
        } else if (common_slot->klad_key[i].key_size == HKL_DATA_IN_32BYTES) {
            /* First step, mcipher used even key */
            ret = _hal_klad_cfg_com_route(&com_route, i, slot_num, HKL_KEY_TYPE_EVEN, common_slot);
            ot_klad_func_fail_return(_hal_klad_cfg_com_route, ret != TD_SUCCESS, ret);

            ret = _hal_klad_startup_com_route(&com_route);
            ot_klad_func_fail_return(_hal_klad_startup_com_route, ret != TD_SUCCESS, ret);

            /* Second step, mcipher used odd key */
            ret = _hal_klad_cfg_com_route(&com_route, i, slot_num, HKL_KEY_TYPE_ODD, common_slot);
            ot_klad_func_fail_return(_hal_klad_cfg_com_route, ret != TD_SUCCESS, ret);

            ret = _hal_klad_startup_com_route(&com_route);
            ot_klad_func_fail_return(_hal_klad_startup_com_route, ret != TD_SUCCESS, ret);
        } else {
            ot_klad_error("Invalid key size %d\n", common_slot->klad_key[i].key_size);
            return OT_ERR_KLAD_INVALID_PARAM;
        }
    }
    /* clean key */
    (td_void)memset_s(com_route.key, HKL_DATA_IN_16BYTES, 0, HKL_DATA_IN_16BYTES);

    return ret;
}

static td_s32 _hal_klad_cfg_clr_route(hkl_clr_route *clr_route,
    td_u32 slot_num, hkl_key_type type, const klad_clear_slot *clear_slot)
{
    td_s32 ret;
    const ot_klad_clear_key *clear_key = &clear_slot->clear_key;

    (td_void)memset_s(clr_route, sizeof(hkl_clr_route), 0, sizeof(hkl_clr_route));
    clr_route->slot_num = slot_num;
    clr_route->type = type;

    if (clear_key->crypto_alg == OT_KLAD_CRYPTO_ALG_AES) {
        clr_route->dsc_code = HKL_DSC_CODE_AES;
#ifdef KLAD_SM4_SUPPORT
    } else if (clear_key->crypto_alg == OT_KLAD_CRYPTO_ALG_SM4) {
        clr_route->dsc_code = HKL_DSC_CODE_SM4;
#endif
    } else {
        ot_klad_error("Invalid crypto alg %d\n", clear_key->crypto_alg);
        return OT_ERR_KLAD_INVALID_PARAM;
    }

    switch (clear_key->key_size) {
        case HKL_DATA_IN_16BYTES:
            clr_route->clr_size = HKL_CLR_KEY_SIZE_128BIT;

            /* for 128bits key format: even key: 8bytes 8bytes */
            ret = memcpy_s(clr_route->key, sizeof(clr_route->key), clear_key->key, HKL_DATA_IN_16BYTES);
            ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);
            break;
        case HKL_DATA_IN_24BYTES:
            clr_route->clr_size = HKL_CLR_KEY_SIZE_192BIT;

            /* for 192bits key format: even key: 0 8bytes, odd key: 8bytes 8bytes */
            if (type == HKL_KEY_TYPE_EVEN) {
                ret = memcpy_s(&clr_route->key[HKL_DATA_IN_8BYTES],
                    sizeof(clr_route->key) - HKL_DATA_IN_8BYTES, clear_key->key, HKL_DATA_IN_8BYTES);
            } else {
                ret = memcpy_s(clr_route->key, sizeof(clr_route->key),
                    &clear_key->key[HKL_DATA_IN_8BYTES], HKL_DATA_IN_16BYTES);
            }
            ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);
            break;
        case HKL_DATA_IN_32BYTES:
            clr_route->clr_size = HKL_CLR_KEY_SIZE_256BIT;

            /* for 256bits key format: even key: 8bytes 8bytes, odd key: 8bytes 8bytes */
            if (type == HKL_KEY_TYPE_EVEN) {
                ret = memcpy_s(clr_route->key, sizeof(clr_route->key), clear_key->key, HKL_DATA_IN_16BYTES);
            } else {
                ret = memcpy_s(clr_route->key, sizeof(clr_route->key),
                    &clear_key->key[HKL_DATA_IN_16BYTES], HKL_DATA_IN_16BYTES);
            }
            ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);
            break;
        default:
            ot_klad_error("Invalid key size %d\n", clear_key->key_size);
            return OT_ERR_KLAD_INVALID_PARAM;
    }
    return TD_SUCCESS;
}

static td_s32 _hal_klad_startup_clr_route(const hkl_clr_route *clr_route)
{
    td_s32 ret;

#ifdef KLAD_INT_SUPPORT
        g_klad_ctx[KLAD_CLR_IDX].done = TD_FALSE;
#endif

    /* step 1. wait clear route klad idle */
    ret = _hkl_clr_route_idle();
    ot_klad_func_fail_return(_hkl_clr_route_idle, ret != TD_SUCCESS, ret);

    /* step 2. set key data */
    _hkl_set_data_in(clr_route->key, HKL_DATA_IN_16BYTES);

    /* step 3. bind key slot, set even&odd key */
    _hkl_set_key_addr(clr_route->slot_num, clr_route->type);

    /* step 4. set dsc alg */
    _hkl_set_key_cfg(clr_route->dsc_code);

    /* step 5. set secure attribute */
    _hkl_set_sec_cfg();

    /* step 6. set key size & startup clear route */
    _hkl_startup_clr_route(clr_route->clr_size);

    /* step 7. wait clear route klad idle & check error */
    ret = _hkl_clr_route_op_finish();
    ot_klad_func_fail_return(_hkl_clr_route_idle, ret != TD_SUCCESS, ret);

    return ret;
}


td_s32 hal_klad_init(td_void)
{
    td_s32 ret;

    g_klad_reg_base = klad_ioremap_nocache(HKL_REG_BASE_ADDR_PHY, HKL_REG_BASE_ADDR_SIZE);
    if (g_klad_reg_base == TD_NULL) {
        ot_klad_error("hkl ioremap with nocache failed!!\n");
        return OT_ERR_KLAD_FAILED_MEM;
    }

    ret = hal_rkp_init();
    if (ret != TD_SUCCESS) {
        klad_iounmap(g_klad_reg_base, HKL_REG_BASE_ADDR_SIZE);
        g_klad_reg_base = TD_NULL;
        return ret;
    }

#ifdef KLAD_INT_SUPPORT
    ret = _hkl_interrupt_init();
    if (ret != TD_SUCCESS) {
        ot_klad_error("Failed call _hkl_interrupt_init\n");
        klad_iounmap(g_klad_reg_base, HKL_REG_BASE_ADDR_SIZE);
        g_klad_reg_base = TD_NULL;
        hal_rkp_deinit();
    }
#endif

    return TD_SUCCESS;
}

td_void hal_klad_deinit(td_void)
{
#ifdef KLAD_INT_SUPPORT
    _hkl_interrupt_deinit();
#endif

    if (g_klad_reg_base != TD_NULL) {
        klad_iounmap(g_klad_reg_base, HKL_REG_BASE_ADDR_SIZE);
        g_klad_reg_base = TD_NULL;
    }

    hal_rkp_deinit();
}

td_s32 hal_klad_common_route_startup(td_u32 slot_num, const klad_common_slot *common_slot)
{
    td_s32 ret;
    td_s32 ret1;

    ot_klad_enter();

    check_hkl_reg_base_fail_return();

    ret = _hal_com_lock();
    ot_klad_func_fail_return(_hal_com_lock, ret != TD_SUCCESS, ret);

    ret = hal_rkp_startup(&common_slot->key_attr);
    if (ret != TD_SUCCESS) {
        ot_klad_error("rkp failed, ret = %x\n", ret);
        goto klad_exit;
    }

    ret = _hal_klad_run_com_route(slot_num, common_slot);
    if (ret != TD_SUCCESS) {
        ot_klad_error("common route failed, ret = %x\n", ret);
        goto klad_exit;
    }

klad_exit:
    /* _hal_com_unlock can't use 'ret' as return value */
    ret1 = _hal_com_unlock();
    ot_klad_func_fail_return(_hal_com_unlock, ret1 != TD_SUCCESS, ret1);

    ot_klad_exit();
    return ret;
}

td_s32 hal_klad_clear_route_startup(td_u32 slot_num, const klad_clear_slot *clear_slot)
{
    td_s32 ret;
    hkl_clr_route clr_route;

    ot_klad_enter();

    check_hkl_reg_base_fail_return();

    if (clear_slot->clear_key.key_size == HKL_DATA_IN_16BYTES) {
        /* mcipher default used even key */
        ret = _hal_klad_cfg_clr_route(&clr_route, slot_num, HKL_KEY_TYPE_EVEN, clear_slot);
        ot_klad_func_fail_return(_hal_klad_cfg_clr_route, ret != TD_SUCCESS, ret);

        ret = _hal_klad_startup_clr_route(&clr_route);
        ot_klad_func_fail_return(_hal_klad_startup_clr_route, ret != TD_SUCCESS, ret);
    } else if ((clear_slot->clear_key.key_size == HKL_DATA_IN_24BYTES) ||
        (clear_slot->clear_key.key_size == HKL_DATA_IN_32BYTES)) {
        /* First step, mcipher used even key */
        ret = _hal_klad_cfg_clr_route(&clr_route, slot_num, HKL_KEY_TYPE_EVEN, clear_slot);
        ot_klad_func_fail_return(_hal_klad_cfg_clr_route, ret != TD_SUCCESS, ret);

        ret = _hal_klad_startup_clr_route(&clr_route);
        ot_klad_func_fail_return(_hal_klad_startup_clr_route, ret != TD_SUCCESS, ret);

        /* Second step, mcipher used odd key */
        ret = _hal_klad_cfg_clr_route(&clr_route, slot_num, HKL_KEY_TYPE_ODD, clear_slot);
        ot_klad_func_fail_return(_hal_klad_cfg_clr_route, ret != TD_SUCCESS, ret);

        ret = _hal_klad_startup_clr_route(&clr_route);
        ot_klad_func_fail_return(_hal_klad_startup_clr_route, ret != TD_SUCCESS, ret);
    } else {
        ot_klad_error("Invalid key size %d\n", clear_slot->clear_key.key_size);
        return OT_ERR_KLAD_INVALID_PARAM;
    }
    /* clean key */
    (td_void)memset_s(clr_route.key, HKL_DATA_IN_16BYTES, 0, HKL_DATA_IN_16BYTES);

    ot_klad_exit();

    return ret;
}

