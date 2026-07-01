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
#include "drv_osal_lib.h"
#include "cryp_trng.h"
#include "cryp_symc.h"
#include "sys_ext.h"

/* ********************** Internal Structure Definition ********************** */
/* crg register addr size. */
#define CRG_REG_ADDR_SIZE        0x100
#define TIMEOUT                4294967295

/* set a bit within a word. */
#undef  set_bit
#define set_bit(src, bit)        ((src) |= (1 << (bit)))

/* clear a bit within a word. */
#undef  clear_bit
#define clear_bit(src, bit)      ((src) &= ~(1 << (bit)))

/* module already initialize or not. */
static td_u32 g_module_initialize = TD_FALSE;

/* struct resource_channel
 * the tbale of base info of module, such as addr/reset/clk/ver.
 */
struct sys_arch_hardware_info {
    /* the name of module, used for debug print and request interrupt. */
    const td_char *name;

    /* some field may be needn't to used. */
    td_u32 reset_valid : 1; /* bis[0], reset availability, 0-valid, 1-invalid. */
    td_u32 clk_valid : 1;   /* bis[1], clock availability, 0-valid, 1-invalid. */
    td_u32 phy_valid : 1;   /* bis[2], system address availability, 0-valid, 1-invalid. */
    td_u32 crg_valid : 1;   /* bis[3], CRG availability, 0-valid, 1-invalid. */
    td_u32 ver_valid : 1;   /* bis[4], version reg availability, 0-valid, 1-invalid. */
    td_u32 int_valid : 1;   /* bis[5], interrupt availability, 0-valid, 1-invalid. */
    td_u32 res_valid : 25;  /* bis[6..31]. */

    /* module base addr. */
    td_u32 reg_addr_phy;

    /* base logic addr size. */
    td_u32 reg_addr_size;

    /* crg register addr, which provide the switch of reset and clock. */
    td_u32 crg_addr_phy;

    /* the clk switch bit index within the crg register,
     * if the switch bis is provided by second crg register, you need to add 32. */
    td_u32 clk_bit : 8;

    /* the reset switch bit index within the crg register,
     * if the switch bis is provided by second crg register, you need to add 32. */
    td_u32 reset_bit : 8;

    /* the interrupt number. */
    td_u32 int_num : 16;

    /* the offset of version register. */
    td_u32 version_reg;

    /* the value of version register, you can used it to check the active module. */
    td_u32 version_val;

    /* cpu address of module register. */
    td_void *reg_addr_via;

    /* cpu address of crg register. */
    td_void *crg_addr_via;
};

static struct sys_arch_hardware_info g_hard_info_table[CRYPTO_MODULE_ID_CNT] = {
    SEC_HARD_INFO_CIPHER,
    SEC_HARD_INFO_CIPHER_KEY,
    SEC_HARD_INFO_HASH,
    SEC_HARD_INFO_IFEP_RSA,
    SEC_HARD_INFO_SIC_RSA,
    SEC_HARD_INFO_TRNG,
    SEC_HARD_INFO_PKE,
    SEC_HARD_INFO_SM4,
    SEC_HARD_INFO_SMMU,
};

#ifdef CRYPTO_SWITCH_CPU
static struct sys_arch_hardware_info g_nsec_hard_info_table[CRYPTO_MODULE_ID_CNT] = {
    NSEC_HARD_INFO_CIPHER,
    NSEC_HARD_INFO_CIPHER_KEY,
    NSEC_HARD_INFO_HASH,
    NSEC_HARD_INFO_IFEP_RSA,
    NSEC_HARD_INFO_SIC_RSA,
    NSEC_HARD_INFO_TRNG,
    NSEC_HARD_INFO_PKE,
    NSEC_HARD_INFO_SM4,
    NSEC_HARD_INFO_SMMU,
};

static td_u32 g_secure_cpu = TD_FALSE;
#endif

/* ****************************** API Code **************************** */
static td_s32 module_addr_map_reg(td_void)
{
    td_u32 i;
    struct sys_arch_hardware_info *table = TD_NULL;

    func_enter();

    if (g_module_initialize == TD_TRUE) {
        return TD_SUCCESS;
    }

    for (i = 0; i < CRYPTO_MODULE_ID_CNT; i++) {
        table = &g_hard_info_table[i];

        ot_cipher_dbg("[%u] %s\n", i, table->name);

        /* io remap crg register. */
        if (table->crg_valid) {
            table->crg_addr_via = crypto_ioremap_nocache(table->crg_addr_phy, CRG_REG_ADDR_SIZE);
            if (table->crg_addr_via == TD_NULL) {
                ot_cipher_error("iomap reg of module failed\n");
                module_addr_unmap();
                return OT_ERR_CIPHER_FAILED_MEM;
            }
            ot_cipher_dbg("crg phy 0x%x, via 0x%pK\n", table->crg_addr_phy, table->crg_addr_via);
        }

        /* io remap module register. */
        if (table->phy_valid) {
            table->reg_addr_via = crypto_ioremap_nocache(table->reg_addr_phy,
                                                         table->reg_addr_size);
            if (table->reg_addr_via == TD_NULL) {
                ot_cipher_error("iomap reg of module failed\n");
                module_addr_unmap();
                print_errno(OT_ERR_CIPHER_FAILED_MEM);
                return OT_ERR_CIPHER_FAILED_MEM;
            }
            ot_cipher_dbg("reg phy 0x%x, via 0x%pK, size 0x%x\n", table->reg_addr_phy,
                table->reg_addr_via, table->reg_addr_size);
        }
    }

    g_module_initialize = TD_TRUE;
    func_exit();
    return TD_SUCCESS;
}

/*
 * brief  unmap the physics addr to cpu within the base table, contains the base addr and crg addr.
 */
td_s32 module_addr_unmap(td_void)
{
    td_u32 i;
    struct sys_arch_hardware_info *table = TD_NULL;

    func_enter();

    if (g_module_initialize == TD_FALSE) {
        return TD_SUCCESS;
    }

    for (i = 0; i < CRYPTO_MODULE_ID_CNT; i++) {
        table = &g_hard_info_table[i];

        ot_cipher_dbg("[%u] %s\n", i, table->name);

        /* io unmap crg register. */
        if (table->crg_valid && table->crg_addr_via) {
            ot_cipher_dbg("crg via addr 0x%pK\n", table->crg_addr_via);
            crypto_iounmap(table->crg_addr_via, CRG_REG_ADDR_SIZE);
            table->crg_addr_via = TD_NULL;
        }

        /* io unmap module register. */
        if (table->phy_valid && table->reg_addr_via) {
            ot_cipher_dbg("reg via addr 0x%pK\n", table->reg_addr_via);
            crypto_iounmap(table->reg_addr_via, table->reg_addr_size);
            table->reg_addr_via = TD_NULL;
        }
    }

    g_module_initialize = TD_FALSE;

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief  map the physics addr to cpu within the base table, contains the base addr and crg addr.
 */
td_s32 module_addr_map(td_void)
{
    td_s32 ret;

    func_enter();

    ret = module_addr_map_reg();
    if (ret != TD_SUCCESS) {
        print_func_errno(module_addr_map_reg, ret);
        return ret;
    }

#ifdef CRYPTO_SWITCH_CPU
    if (g_secure_cpu == TD_TRUE) {
        /* default secure cpu, do nothing */
        return TD_SUCCESS;
    }

    ot_cipher_dbg("non-secure CPU need to remap reg addr\n");

    /* use non-secure info */
    if (memcpy_s(g_hard_info_table, sizeof(g_hard_info_table),
        g_nsec_hard_info_table, sizeof(g_nsec_hard_info_table)) != EOK) {
        print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }

    /* remap module addr */
    ret = module_addr_unmap();
    if (ret != TD_SUCCESS) {
        print_func_errno(module_addr_unmap, ret);
        return ret;
    }
    ret = module_addr_map_reg();
    if (ret != TD_SUCCESS) {
        print_func_errno(module_addr_map_reg, ret);
        return ret;
    }
#endif

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief  get secure cpu type.
 */
td_u32 module_get_secure(td_void)
{
#ifdef CRYPTO_SEC_CPU
    return TD_TRUE;
#elif defined(CRYPTO_SWITCH_CPU)
    return TD_TRUE;
#else
    return TD_FALSE;
#endif
}

/*
 * brief  open clock.
 */
static td_void module_clock(module_id id, td_u32 open)
{
    td_u32 *addr = TD_NULL;
    struct sys_arch_hardware_info *table = TD_NULL;

    func_enter();

    if (id >= CRYPTO_MODULE_ID_CNT) {
        ot_cipher_error("invalid module id %d\n", id);
        return;
    }

    table = &g_hard_info_table[id];

    if (table->clk_valid) {
        td_u32 val;

        if (table->crg_addr_via == TD_NULL) {
            ot_cipher_error("%s clock failed, crg addr is null\n", table->name);
            return;
        }

        /* open clk, the addr may be need to add 1. */
        addr = table->crg_addr_via;
        addr += table->clk_bit / WORD_BIT_WIDTH;

        val = crypto_read(addr);

        if (open) {
            set_bit(val, table->clk_bit % WORD_BIT_WIDTH);
        } else {
            clear_bit(val, table->clk_bit % WORD_BIT_WIDTH);
        }

        crypto_write(addr, val);

        /* wait hardware clock active. */
        crypto_udelay(1);

        ot_cipher_dbg("%s clock, open %u, bit %u, phy 0x%x, via 0x%pK\n",
            table->name, open, table->clk_bit, table->crg_addr_phy, table->crg_addr_via);
    }

    func_exit();

    return;
}

static td_void module_reset(module_id id, td_u32 enable)
{
    td_u32 *addr = TD_NULL;
    struct sys_arch_hardware_info *table = TD_NULL;

    func_enter();

    if (id >= CRYPTO_MODULE_ID_CNT) {
        ot_cipher_error("invalid module id %d\n", id);
        return;
    }

    table = &g_hard_info_table[id];

    if (table->reset_valid) {
        td_u32 val;
        td_u32 expect;

        if (table->crg_addr_via == TD_NULL) {
            ot_cipher_error("%s reset failed, crg addr is null\n", table->name);
            return;
        }

        /* the addr may be need to add 1. */
        addr = table->crg_addr_via;
        addr += table->reset_bit / WORD_BIT_WIDTH;

        val = crypto_read(addr);

        if (enable) {
            set_bit(val, table->reset_bit % WORD_BIT_WIDTH);
            expect = 0;
        } else {
            clear_bit(val, table->reset_bit % WORD_BIT_WIDTH);
            expect = table->version_val;
        }

        crypto_write(addr, val);

        /* wait hardware reset finish. */
        crypto_udelay(1);

        ot_cipher_dbg("%s reset, enable %u, bit %u, phy 0x%x, via 0x%pK\n",
            table->name, enable, table->reset_bit, table->crg_addr_phy, table->crg_addr_via);

        /* check the value of version reg to make sure reset success. */
        if (table->ver_valid && table->reg_addr_via) {
            val = crypto_read((td_u8 *)table->reg_addr_via + table->version_reg);
            if (val != expect) {
                ot_cipher_error("%s reset failed, version reg should be 0x%x but 0x%x\n", table->name, expect, val);
                return;
            }

            ot_cipher_dbg("%s version reg, offset 0x%x, expect val 0x%x, real val 0x%x\n",
                table->name, table->version_reg, expect, val);
        }
    }

    func_exit();

    return;
}

/*
 * brief  enable a module, open clock  and remove reset signal.
 */
td_void module_enable(module_id id)
{
    module_clock(id, TD_TRUE);
    module_reset(id, TD_FALSE);
    return;
}

/*
 * brief  disable a module, close clock and set reset signal.
 */
td_void module_disable(module_id id)
{
    module_reset(id, TD_TRUE);
    module_clock(id, TD_FALSE);
    return;
}

/*
 * brief  get attribute of module.
 */
td_void module_get_attr(module_id id, td_u32 *int_valid, td_u32 *int_num, const td_char **name)
{
    if (id >= CRYPTO_MODULE_ID_CNT) {
        ot_cipher_error("invalid module id %d\n", id);
        return;
    }

    *int_valid = g_hard_info_table[id].int_valid;
    *int_num = g_hard_info_table[id].int_num;
    if (name != TD_NULL) {
        *name = g_hard_info_table[id].name;
    }

    return;
}

void module_get_info(module_id id, td_u32 *info, module_info_option option)
{
    *info = 0x00;

    if (option == MODULE_INFO_RESET_VALID) {
        *info = g_hard_info_table[id].reset_valid;
    }

    return;
}

/*
 * brief  get base address of module.
 */
td_void module_set_irq(module_id id, td_u32 irq)
{
    if (id >= CRYPTO_MODULE_ID_CNT) {
        ot_cipher_error("invalid module id %d\n", id);
        return;
    }

    g_hard_info_table[id].int_num = irq;
    ot_cipher_dbg("%s set irq number 0x%x\n", g_hard_info_table[id].name, irq);
}

#ifdef CRYPTO_SWITCH_CPU
td_void module_set_nsec_irq(module_id id, td_u32 irq)
{
    if (id >= CRYPTO_MODULE_ID_CNT) {
        ot_cipher_error("invalid module id %d\n", id);
        return;
    }

    g_nsec_hard_info_table[id].int_num = irq;
    ot_cipher_dbg("%s set irq number 0x%x\n", g_nsec_hard_info_table[id].name, irq);
}
#endif

/*
 * brief  read a register.
 */
td_u32 module_reg_read(module_id id, td_u32 offset)
{
    td_u32 val;
    td_void *addr = TD_NULL;
    td_s32 ret;

    chk_param_err_return(id >= CRYPTO_MODULE_ID_CNT);
    chk_param_err_return(offset >= g_hard_info_table[id].reg_addr_size);

    /* tee may be read trng before cipher module init. */
    if (g_hard_info_table[id].reg_addr_via == TD_NULL) {
        ret = module_addr_map();
        if (ret != TD_SUCCESS) {
            print_func_errno(module_addr_map, ret);
            return 0;
        }
    }

    /* get the absolute address of reg. */
    addr = (td_u8 *)(g_hard_info_table[id].reg_addr_via) + offset;
    val = crypto_read(addr);

    return val;
}

/*
 * brief  write a register.
 */
td_void module_reg_write(module_id mod_id, td_u32 offset, td_u32 val)
{
    td_void *addr = TD_NULL;
    td_s32 ret;

    /* check if module is valid. */
    if (mod_id >= CRYPTO_MODULE_ID_CNT) {
        ot_cipher_error("error, invalid mod_id %d\n", mod_id);
        return;
    }

    /* check if offset is valid. */
    if (offset >= g_hard_info_table[mod_id].reg_addr_size) {
        ot_cipher_error("error, reg offset overflow 0x%x\n", offset);
        return;
    }

    /* tee may be read trng before cipher module init. */
    if (g_hard_info_table[mod_id].reg_addr_via == TD_NULL) {
        ret = module_addr_map();
        if (ret != TD_SUCCESS) {
            print_func_errno(module_addr_map, ret);
            return;
        }
    }

    /* get the absolute address of reg. */
    addr = (td_u8 *)g_hard_info_table[mod_id].reg_addr_via + offset;
    crypto_write(addr, val);

    return;
}

/*
 * brief  Initialize the channel list.
 */
td_s32 crypto_channel_init(channel_context *ctx, td_u32 num, td_u32 ctx_size)
{
    td_u32 size;
    td_u32 i;
    td_u8 *buf = TD_NULL;

    func_enter();

    chk_ptr_err_return(ctx);

    /* clear context. */
    size = sizeof(channel_context) * num;
    (td_void)memset_s(ctx, size, 0, size);

    /* set context buffer. */
    if (ctx_size > 0) {
        buf = (td_u8 *)crypto_malloc(ctx_size * num);
        if (buf == TD_NULL) {
            return OT_ERR_CIPHER_FAILED_MEM;
        }
        (td_void)memset_s(buf, ctx_size * num, 0, ctx_size * num);

        /* the buffer address is stored at ctx[0].ctx. */
        for (i = 0; i < num; i++) {
            ctx[i].ctx = buf;
            buf += ctx_size;
        }
    }

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief  denit the channel list.
 */
td_s32 crypto_channel_deinit(channel_context *ctx, td_u32 num)
{
    td_u32 size;

    func_enter();

    chk_ptr_err_return(ctx);

    /* the buffer address is stored at ctx[0].ctx. */
    crypto_free(ctx[0].ctx);

    /* clear context. */
    size = sizeof(channel_context) * num;
    (td_void)memset_s(ctx, size, 0, size);

    func_exit();
    return TD_SUCCESS;
}

/*
 * brief  allocate a channel.
 */
td_s32 crypto_channel_alloc(channel_context *ctx, td_u32 num, td_u32 mask, td_u32 *id)
{
    td_u32 i;

    func_enter();

    chk_ptr_err_return(ctx);
    chk_ptr_err_return(id);

    for (i = 0; i < num; i++) {
        /* check the valid channel. */
        if (mask & (0x01 << i)) {
            if (!ctx[i].open) { /* found a free channel. */
                ctx[i].open = TD_TRUE; /* alloc channel. */
                *id = i;
                return TD_SUCCESS;
            }
        }
    }
    if (i == num) {
        ot_cipher_error("error, all channels are busy.\n");
    }

    func_exit();

    return OT_ERR_CIPHER_BUSY;
}

/*
 * brief  free a  channel.
 */
td_void crypto_channel_free(channel_context *ctx, td_u32 num, td_u32 id)
{
    crypto_unused(num);

    func_enter();

    /* free channel. */
    ctx[id].open = TD_FALSE;

    func_exit();
    return;
}

/*
 * brief  get the private data of channel.
 */
td_void *crypto_channel_get_context(const channel_context *ctx, td_u32 num, td_u32 id)
{
    if ((id >= num) || (!ctx[id].open)) {
        ot_cipher_error("crypto_channel_get_context()- error, id %u, open %u, num %u\n", id, ctx[id].open, num);
        print_errno(OT_ERR_CIPHER_INVALID_POINT);
        return TD_NULL;
    }

    return ctx[id].ctx;
}

td_s32 crypto_get_cpu_secure_sta(td_void)
{
#if defined(CRYPTO_SWITCH_CPU)
    if (!ckfn_sys_entry()) {
        ot_cipher_error("no sys ko\n");
        return OT_ERR_CIPHER_FAILED_INIT;
    }

    if (ckfn_sys_is_tee_enable_otp() && !call_sys_is_tee_enable_otp()) {
        g_secure_cpu = TD_TRUE;
    } else {
        g_secure_cpu = TD_FALSE;
    }
#endif
    return TD_SUCCESS;
}

/* proc function begin */
#ifdef OT_PROC_SUPPORT
void hex2str(char *buf, td_u8 val)
{
    td_u8 high;
    td_u8 low;

    high = (val >> 4) & 0x0F; /* high 4 bit */
    low = val & 0x0F;

    if (high <= 9) {  /* 0~9 */
        buf[0] = high + '0';
    } else {
        buf[0] = (high - 0x0A) + 'A'; /* 0x0A~0x0F */
    }

    if (low <= 9) {  /* 0~9 */
        buf[1] = low + '0';
    } else {
        buf[1] = (low - 0x0A) + 'A'; /* 0x0A~0x0F */
    }
}
#endif
/* proc function end */

td_void *crypto_calloc(size_t n, size_t size)
{
    td_void *ptr = TD_NULL;

    ptr = crypto_malloc(n * size);
    if (ptr == TD_NULL) {
        print_func_errno(crypto_malloc, OT_ERR_CIPHER_INVALID_POINT);
        return TD_NULL;
    }
    (td_void)memset_s(ptr, n * size, 0, n * size);

    return ptr;
}

td_u32 get_rand(td_void)
{
    td_s32 ret;
    td_u32 randnum = 0;

    ret = cryp_trng_get_random(&randnum, TIMEOUT);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_trng_get_random, ret);
    }

    return randnum;
}

/** @} */ /** <!-- ==== API Code end ==== */
