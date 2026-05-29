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

#include "hal_rkp.h"
#include "klad_cfg.h"
#include "rkp_reg_define.h"
#include "ot_debug_klad.h"
#include "ot_common_klad.h"
#include "drv_lib.h"
#include "klad_reg_base.h"
#include "drv_osal_init.h"
#include "securec.h"

#define RKP_OP_TIMEOUT              6000

typedef enum {
    RKP_STATIC_SEL_REE = 0x0,
    RKP_STATIC_SEL_TEE,
} rkp_static_sel;

typedef enum {
    RKP_KEY_SEL_OEM0 = 0x0,
    RKP_KEY_SEL_OEM1,
    RKP_KEY_SEL_OEM2,
    RKP_KEY_SEL_OEM3,
    RKP_KEY_SEL_VENDOR,
} rkp_key_sel;

typedef struct {
    td_u32 owner_id;
    rkp_static_sel static_sel;
    rkp_key_sel key_sel;
} rkp_materials;

#ifdef RKP_INT_SUPPORT
typedef struct {
    td_bool done;
    KLAD_QUEUE_HEAD queue;
} rkp_hard_context;

static rkp_hard_context g_rkp_ctx;
#endif


static td_u8 *g_rkp_reg_base = TD_NULL;

#define check_rkp_reg_base_fail_return()        \
    do {                                        \
        if (g_rkp_reg_base == TD_NULL) {        \
            ot_klad_error("rkp isn't init\n");  \
            return OT_ERR_KLAD_NOT_INIT;        \
        }                                       \
    } while (0)

static inline td_u32 _rkp_read(td_u32 offset)
{
    td_u32 val;
    val = klad_read(g_rkp_reg_base + (offset));
    ot_klad_info(" offset %04x, read  %08x\n", offset, val);
    return val;
}

static inline td_void _rkp_write(td_u32 offset, td_u32 data)
{
    ot_klad_info("offset %04x, write %08x\n", offset, data);
    klad_write(g_rkp_reg_base + (offset), data);
}

#ifdef RKP_INT_SUPPORT
static td_s32 _rkp_queue_callback_func(const td_void *param)
{
    return (td_s32) *(td_bool *)param;
}

static td_void _rkp_print_int_status(td_void)
{
    rkp_int_en int_en;
    rkp_int_raw int_raw;
    rkp_int int_sta;

    int_en.u32 = _rkp_read(RKP_INT_EN);
    ot_klad_error("klad int enable %08x\n", int_en.u32);

    int_raw.u32 = _rkp_read(RKP_INT_RAW);
    ot_klad_error("klad int raw %08x\n", int_raw.u32);

    int_sta.u32 = _rkp_read(RKP_INT);
    ot_klad_error("klad int status %08x\n", int_sta.u32);
}

static td_void _rkp_enable_int(td_bool is_int)
{
    rkp_int_en int_en;
    int_en.u32 = 0;
    int_en.bits.rkp_int_en = is_int;
    _rkp_write(RKP_INT_EN, int_en.u32);
}

static td_void _rkp_clean_interrupt(td_void)
{
    rkp_int_raw int_raw;

    int_raw.u32 = 0;
    int_raw.bits.rkp_int_raw = 1;
    _rkp_write(RKP_INT_RAW, int_raw.u32);
}

static td_s32 _rkp_interrupt_isr(td_s32 irq, td_void *dev_id)
{
    ot_unused(dev_id);

    _rkp_clean_interrupt();

    if (g_rkp_ctx.done == TD_FALSE) {
        g_rkp_ctx.done = TD_TRUE;
        klad_queue_wait_up(&g_rkp_ctx.queue);
    }

    return KLAD_IRQ_HANDLED;
}

static td_s32 _rkp_interrupt_init(td_void)
{
    td_s32 ret;

    (td_void)memset_s(&g_rkp_ctx, sizeof(g_rkp_ctx), 0, sizeof(g_rkp_ctx));

    _rkp_enable_int(TD_TRUE);

    /* the history of interception may trigger the system to
     * call the irq function before initialization
     * when register interrupt, this will cause a system abort. */
    _rkp_clean_interrupt();

    ret = klad_request_irq(ot_klad_get_rkp_irq(), _rkp_interrupt_isr, RKP_INT_NAME);
    ot_klad_func_fail_return(klad_request_irq, ret != TD_SUCCESS, OT_ERR_KLAD_FAILED_OPERATE);

    klad_queue_init(&g_rkp_ctx.queue);

    return ret;
}

static td_void _rkp_interrupt_deinit(td_void)
{
    klad_queue_destroy(&g_rkp_ctx.queue);

    klad_free_irq(ot_klad_get_rkp_irq(), RKP_INT_NAME);

    _rkp_enable_int(TD_FALSE);
}
#endif

static td_s32 _rkp_check_error(td_void)
{
    rkp_error error;

    error.u32 = _rkp_read(RKP_ERROR);
    if (error.u32 != 0) {
        ot_klad_error("read RKP_ERROR failed 0x%08X\n", error.u32);
        ot_klad_error("read RKP_DBG_STAT %08x\n", _rkp_read(RKP_DBG_STAT));
        return OT_ERR_KLAD_FAILED_OPERATE;
    }
    return TD_SUCCESS;
}

static td_void _rkp_set_sw_reg(td_u32 sw_reg)
{
    _rkp_write(RKP_SW_REG, sw_reg);
}

static td_void _rkp_set_sec_cfg(rkp_static_sel static_sel)
{
    rkp_sec_cfg cfg;

    cfg.u32 = _rkp_read(RKP_SEC_CFG);
    cfg.bits.rkp_calc_type = 0; /* fixed to 0, 0 - generate rootkey, 1 - generate tee_user_password */
    cfg.bits.rkp_static_sel = static_sel;
    _rkp_write(RKP_SEC_CFG, cfg.u32);
}

static td_void _rkp_startup(rkp_key_sel key_sel)
{
    rkp_calc_ctrl ctrl;

    ctrl.u32 = _rkp_read(RKP_CALC_CTRL);
    ctrl.bits.rkp_calc_start = 1;
    ctrl.bits.rkp_rk_sel = key_sel;
    ctrl.bits.rkp_remap = 0; /* uboot/kernel default value 0 */
    _rkp_write(RKP_CALC_CTRL, ctrl.u32);
}

static td_s32 _rkp_wait_idle(void)
{
    td_u32 time_out = RKP_OP_TIMEOUT;
    rkp_calc_ctrl ctrl;

    while (--time_out) {
        ctrl.u32 = _rkp_read(RKP_CALC_CTRL);
        if (ctrl.bits.rkp_calc_start == 0x0) {
            break;
        }
        klad_udelay(1);
    }

    if (time_out == 0) {
        ot_klad_error("rkp wait op timeout!");
        return OT_ERR_KLAD_TIMEOUT;
    }

    return TD_SUCCESS;
}

static td_s32 _rkp_wait_op_finish(void)
{
    td_s32 ret;
#ifdef RKP_INT_SUPPORT
    ret = klad_queue_wait_timeout(&g_rkp_ctx.queue,
        _rkp_queue_callback_func, &g_rkp_ctx.done, RKP_OP_TIMEOUT);
    if (ret == 0) {
        ot_klad_error("Failed call klad_queue_wait_timeout\n");
        _rkp_print_int_status();
        return OT_ERR_KLAD_TIMEOUT;
    }
#endif
    ret = _rkp_wait_idle();
    ot_klad_func_fail_return(_rkp_wait_idle, ret != TD_SUCCESS, ret);

    ret = _rkp_check_error();
    ot_klad_func_fail_return(_rkp_check_error, ret != TD_SUCCESS, ret);

    return TD_SUCCESS;
}

/* config rkp derived material */
static td_s32 drv_rkp_cfg_materials(rkp_materials *materials, const ot_klad_rootkey_attr *key_attr)
{
    (td_void)memset_s(materials, sizeof(rkp_materials), 0, sizeof(rkp_materials));

    materials->owner_id = key_attr->owner_id;

    switch (key_attr->key_sel) {
        case OT_KLAD_ROOTKEY_SEL_OEM0:
            materials->key_sel = RKP_KEY_SEL_OEM0;
            break;
        case OT_KLAD_ROOTKEY_SEL_OEM1:
            materials->key_sel = RKP_KEY_SEL_OEM1;
            break;
        case OT_KLAD_ROOTKEY_SEL_OEM2:
            materials->key_sel = RKP_KEY_SEL_OEM2;
            break;
        case OT_KLAD_ROOTKEY_SEL_OEM3:
            materials->key_sel = RKP_KEY_SEL_OEM3;
            break;
        case OT_KLAD_ROOTKEY_SEL_VENDOR:
            materials->key_sel = RKP_KEY_SEL_VENDOR;
            break;
        default:
            ot_klad_error("Invalid rootkey sel %d\n", key_attr->key_sel);
            return OT_ERR_KLAD_INVALID_PARAM;
    }

    if (key_attr->key_secure == OT_KLAD_ROOTKEY_SEC_REE) {
        materials->static_sel = RKP_STATIC_SEL_REE;
    } else if (klad_is_secure_cpu() && key_attr->key_secure == OT_KLAD_ROOTKEY_SEC_TEE) {
        materials->static_sel = RKP_STATIC_SEL_TEE;
    } else {
        ot_klad_error("Invalid rootkey secure %d\n", key_attr->key_secure);
        return OT_ERR_KLAD_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 hal_rkp_init(td_void)
{
    g_rkp_reg_base = klad_ioremap_nocache(RKP_REG_BASE_ADDR_PHY, RKP_REG_BASE_ADDR_SIZE);
    if (g_rkp_reg_base == TD_NULL) {
        ot_klad_error("ERROR: rkp ioremap with nocache failed!!\n");
        return OT_ERR_KLAD_FAILED_MEM;
    }

#ifdef RKP_INT_SUPPORT
    if (_rkp_interrupt_init() != TD_SUCCESS) {
        ot_klad_error("Failed call _rkp_interrupt_init\n");
        klad_iounmap(g_rkp_reg_base, HKL_REG_BASE_ADDR_SIZE);
        g_rkp_reg_base = TD_NULL;
        return OT_ERR_KLAD_FAILED_INIT;
    }
#endif

    return TD_SUCCESS;
}

td_void hal_rkp_deinit(td_void)
{
#ifdef RKP_INT_SUPPORT
    _rkp_interrupt_deinit();
#endif

    if (g_rkp_reg_base != TD_NULL) {
        klad_iounmap(g_rkp_reg_base, RKP_REG_BASE_ADDR_SIZE);
        g_rkp_reg_base = TD_NULL;
    }
}

/* param: sw_reg, key_sel, static_sel, rootkey or user password */
td_s32 hal_rkp_startup(const ot_klad_rootkey_attr *key_attr)
{
    td_s32 ret;
    rkp_materials materials;

    ot_klad_enter();

    check_rkp_reg_base_fail_return();

    /* config rkp derived material */
    ret = drv_rkp_cfg_materials(&materials, key_attr);
    ot_klad_func_fail_return(drv_rkp_cfg_materials, ret != TD_SUCCESS, ret);

    /* step 1. wait rkp control idle */
    ret = _rkp_wait_idle();
    ot_klad_func_fail_return(_rkp_wait_idle, ret != TD_SUCCESS, ret);

    /* step 2. select static value */
    _rkp_set_sec_cfg(materials.static_sel);

    /* step 3. set derived material: sw_reg */
    _rkp_set_sw_reg(materials.owner_id);

    /* step 4. select otp key & startup rkp control */
    _rkp_startup(materials.key_sel);

    /* step 5. wait rkp control operate finish */
    ret = _rkp_wait_op_finish();
    ot_klad_func_fail_return(_rkp_wait_op_finish, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return ret;
}
