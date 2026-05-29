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

#include "ot_ir.h"
#include "ot_osal.h"
#include "ot_ir_codedef.h"
#include "ot_ir_hal.h"

#ifndef NULL
#define NULL  ((void *)0)
#endif

/* irq */
static uintptr_t g_ir_irq = 0;

/* io */
#define IR_REG_LENGTH 0x1000
static volatile void *g_ir_reg_base = NULL;

#define IR_ENABLE   ((uintptr_t)g_ir_reg_base + 0x00)
#define IR_CONFIG   ((uintptr_t)g_ir_reg_base + 0x04)
#define CNT_LEADS   ((uintptr_t)g_ir_reg_base + 0x08)
#define CNT_LEADE   ((uintptr_t)g_ir_reg_base + 0x0c)
#define CNT_SLEADE  ((uintptr_t)g_ir_reg_base + 0x10)
#define CNT0_B      ((uintptr_t)g_ir_reg_base + 0x14)
#define CNT1_B      ((uintptr_t)g_ir_reg_base + 0x18)
#define IR_BUSY     ((uintptr_t)g_ir_reg_base + 0x1c)
#define IR_DATAH    ((uintptr_t)g_ir_reg_base + 0x20)
#define IR_DATAL    ((uintptr_t)g_ir_reg_base + 0x24)
#define IR_INTM     ((uintptr_t)g_ir_reg_base + 0x28)
#define IR_INTS     ((uintptr_t)g_ir_reg_base + 0x2c)
#define IR_INTC     ((uintptr_t)g_ir_reg_base + 0x30)
#define IR_START    ((uintptr_t)g_ir_reg_base + 0x34)

/* interrupt mask */
#define INTMS_RCV      (1L << 16)
#define INTMS_FRAMERR  (1L << 17)
#define INTMS_OVERFLOW (1L << 18)
#define INTMS_RELEASE  (1L << 19)

#define INT_MASK    (INTMS_RCV | INTMS_FRAMERR | INTMS_OVERFLOW | INTMS_RELEASE)

/* define macro */
#define write_reg(addr, value) ((*(volatile unsigned int *)(addr)) = (value))
#define read_reg(addr)         (*(volatile unsigned int *)(addr))

/* debug */
static int g_dbg_flag = 0;
#define OT_IR_PFX "ot_ir: "
#define ot_ir_dbg(params...) \
    do { \
        if (g_dbg_flag) { \
            osal_printk(params); \
        } \
    } while (0)

/* unit is ms */
#define DEFAULT_DELAYTIME (200 * 10)

#define OT_IR_MAX_BUF 100
#define DEFAULT_BUF_LEN 8
#define OT_IR_TIMING_MAX 0xffffU
#define BUF_HEAD g_ot_ir_dev.buf[g_ot_ir_dev.head]
#define BUF_TAIL g_ot_ir_dev.buf[g_ot_ir_dev.tail]
#define BUF_LAST g_ot_ir_dev.buf[(g_ot_ir_dev.head == 0) ? (g_ot_ir_dev.buf_len - 1) : (g_ot_ir_dev.head - 1)]

typedef struct {
    ot_ir_dev_param dev_parm;
    unsigned int head;
    unsigned int tail;
    irkey_info_s buf[OT_IR_MAX_BUF + 1]; /* 1:fifo reserve */
    unsigned int buf_len;

    unsigned int enable_repkey;
    unsigned int repkey_checkflag;
    unsigned int repkey_delaytime; /* ms */

    unsigned int enable_keyup;

    osal_wait_t ir_key_wait;
} ot_ir_dev_struct;

static ot_ir_dev_struct g_ot_ir_dev;

static void repkey_timeout_handler(unsigned long data);
static struct osal_timer g_repkey_timeout_timer;
static int g_need_iounmap = 0;

uintptr_t *get_ir_irq(void)
{
    return &g_ir_irq;
}

volatile void **get_ir_reg_base(void)
{
    return &g_ir_reg_base;
}

static void ot_ir_show_reg(void)
{
    if (g_dbg_flag) {
        osal_printk("OT_IR11 REG:###################################################\n");
        osal_printk("%.8x ", read_reg(IR_ENABLE));
        osal_printk("%.8x ", read_reg(IR_CONFIG));
        osal_printk("%.8x ", read_reg(CNT_LEADS));
        osal_printk("%.8x ", read_reg(CNT_LEADE));
        osal_printk("%.8x ", read_reg(CNT_SLEADE));
        osal_printk("%.8x ", read_reg(CNT0_B));
        osal_printk("%.8x ", read_reg(CNT1_B));
        osal_printk("\n");
        osal_printk("%.8x ", read_reg(IR_BUSY));
        osal_printk("%.8x ", read_reg(IR_DATAH));
        osal_printk("%.8x ", read_reg(IR_DATAL));
        osal_printk("%.8x ", read_reg(IR_INTM));
        osal_printk("%.8x ", read_reg(IR_INTS));
        osal_printk("%.8x ", read_reg(IR_INTC));
        osal_printk("%.8x ", read_reg(IR_START));
        osal_printk("\n");
    }
}

static inline unsigned int inc_buf(unsigned int x, unsigned int len)
{
    ++x;

    if (len == 0) {
        return x % DEFAULT_BUF_LEN;
    } else {
        return x % len;
    }
}

static void write_reg32(unsigned int value, unsigned int mask, const void *addr)
{
    unsigned int t;

    t = osal_readl((void *)addr);
    t &= ~mask;
    t |= value & mask;

    osal_writel(t, (void *)addr);
}

static int ot_ir_enable_clock(void)
{
    void *ir_crg_addr = NULL;

    ir_crg_addr = osal_ioremap(IR_CRG_ADDR, (unsigned long)IR_CLOCK_REG_LENGTH);
    if (ir_crg_addr == NULL) {
        return -1;
    }
#ifdef IR_IS_DEFAULT_RESET
    write_reg32(0x0, 0x1, ir_crg_addr); /* 0x0: [1] bit for clock reset */
#endif
    write_reg32(0x1 << IR_BIT, 0x1 << IR_BIT, ir_crg_addr); /* 0x1: [1] bit for clock control */

    osal_iounmap((void *)ir_crg_addr, (unsigned long)IR_CLOCK_REG_LENGTH);

    return 0;
}

static void ot_ir_disable_clock(void)
{
    void *ir_crg_addr = NULL;

    ir_crg_addr = osal_ioremap(IR_CRG_ADDR, (unsigned long)IR_CLOCK_REG_LENGTH);
    if (ir_crg_addr == NULL) {
        return;
    }

    write_reg32(0x0 << IR_BIT, 0x1 << IR_BIT, ir_crg_addr); /* 0x1: [1] bit for clock control */

    osal_iounmap((void *)ir_crg_addr, (unsigned long)IR_CLOCK_REG_LENGTH);
}

static void ot_ir_config(void)
{
    unsigned int value;

    write_reg(IR_ENABLE, 0x01); /* 0x01: for IR control */
    while (read_reg(IR_BUSY)) {
        ot_ir_dbg("IR_BUSY. Wait...\n");
    }

    value = (g_ot_ir_dev.dev_parm.codetype << 14);       /* 14: [15:14] bits for code type */
    value |= (g_ot_ir_dev.dev_parm.code_len - 1) << 8;   /* 8: [13:8] bits for  code len */
    value |= (g_ot_ir_dev.dev_parm.frequence - 1);       /* [6:0] bits for frequence */
    write_reg(IR_CONFIG, value);

    value = g_ot_ir_dev.dev_parm.leads_min << 16;    /* 16:  [25:16] bits for min leads */
    value |= g_ot_ir_dev.dev_parm.leads_max;         /* [9:0] bits for max leads */
    write_reg(CNT_LEADS, value);

    value = g_ot_ir_dev.dev_parm.leade_min << 16;    /* 16, bits [24:16] for min leads */
    value |= g_ot_ir_dev.dev_parm.leade_max;         /* [8:0] bits for max leads */
    write_reg(CNT_LEADE, value);

    value = g_ot_ir_dev.dev_parm.sleade_min << 16;   /* 16, bits [24:16] for min sleade */
    value |= g_ot_ir_dev.dev_parm.sleade_max;        /* [8:0] bits for max sleade */
    write_reg(CNT_SLEADE, value);

    value = g_ot_ir_dev.dev_parm.cnt0_b_min << 16;   /* 16, bits [24:16] for min cnt0 */
    value |= g_ot_ir_dev.dev_parm.cnt0_b_max;        /* [8:0] bits for max cnt0 */
    write_reg(CNT0_B, value);

    value = g_ot_ir_dev.dev_parm.cnt1_b_min << 16;   /* 16, bits [24:16] for min cnt1 */
    value |= g_ot_ir_dev.dev_parm.cnt1_b_max;        /* [8:0] bits for max cnt1 */
    write_reg(CNT1_B, value);

    write_reg(IR_INTM, 0x00);
    write_reg(IR_START, 0x00);
}

static void repkey_timeout_handler(unsigned long data)
{
    ot_ir_unused(data);
    osal_del_timer(&g_repkey_timeout_timer);
    g_ot_ir_dev.repkey_checkflag = 0;
}

static void interrupt_rcv(void)
{
    ot_ir_dbg("receive data interrupt. 0x%.8x-0x%.8x\n", read_reg(IR_DATAH), read_reg(IR_DATAL));
    write_reg(IR_INTC, 0x01 << 0); /* 0x01: clear interrupt */

    if (g_ot_ir_dev.enable_repkey) {
        if ((g_ot_ir_dev.repkey_checkflag) &&
            (BUF_LAST.irkey_datah == read_reg(IR_DATAH)) &&
            (BUF_LAST.irkey_datal == read_reg(IR_DATAL))) {
            ot_ir_dbg("repeart key [0x%.8x]-[0x%.8x] detective\n", read_reg(IR_DATAH), read_reg(IR_DATAL));
        } else {
            /* repeat key check */
            osal_del_timer(&g_repkey_timeout_timer);
            osal_set_timer(&g_repkey_timeout_timer, g_ot_ir_dev.repkey_delaytime / 10); /* 10: delaytime means ms*10 */
            g_ot_ir_dev.repkey_checkflag = 1;
            BUF_HEAD.irkey_datah = read_reg(IR_DATAH);
            BUF_HEAD.irkey_datal = read_reg(IR_DATAL);
            BUF_HEAD.irkey_state_code = OT_IR_KEY_DOWN;
            g_ot_ir_dev.head = inc_buf(g_ot_ir_dev.head, g_ot_ir_dev.buf_len);
            osal_wakeup(&(g_ot_ir_dev.ir_key_wait));
        }
    } else if (!((BUF_LAST.irkey_datah == read_reg(IR_DATAH)) &&
        (BUF_LAST.irkey_datal == read_reg(IR_DATAL)) &&
        (BUF_LAST.irkey_state_code == OT_IR_KEY_DOWN))) {
        BUF_HEAD.irkey_datah = read_reg(IR_DATAH);
        BUF_HEAD.irkey_datal = read_reg(IR_DATAL);
        BUF_HEAD.irkey_state_code = OT_IR_KEY_DOWN;
        g_ot_ir_dev.head = inc_buf(g_ot_ir_dev.head, g_ot_ir_dev.buf_len);
        osal_wakeup(&(g_ot_ir_dev.ir_key_wait));
    }
}

static int ot_ir_interrupt(int irq, void *dev_id)
{
    unsigned int int_state = read_reg(IR_INTS);

    ot_ir_unused(irq);
    ot_ir_unused(dev_id);
    if ((int_state & INT_MASK) == 0) {
        return OSAL_IRQ_NONE;
    }

    ot_ir_dbg("Enter ot_ir_interrupt.\n");
    ot_ir_show_reg();

    if (int_state & INTMS_FRAMERR) {
        ot_ir_dbg("frame error.\n");
        write_reg(IR_INTC, 0x01 << 1); /* 1: write 0x01 to [1] bit, clear frame error interrupt */
    } else if (int_state & INTMS_OVERFLOW) {
        ot_ir_dbg("hardware overflow.\n");
        write_reg(IR_INTC, 0x01 << 2); /* 2: write 0x01 to [2] bit, clear hardware overflow interrupt */
    } else if (int_state & INTMS_RELEASE) {
        ot_ir_dbg("release key interrupt.\n");
        write_reg(IR_INTC, 0x01 << 3); /* 3: write 0x01 to [3] bit, clear release key interrupt */

        osal_del_timer(&g_repkey_timeout_timer);
        g_ot_ir_dev.repkey_checkflag = 0;

        if ((g_ot_ir_dev.enable_keyup) && (BUF_LAST.irkey_state_code != OT_IR_KEY_UP)) {
            BUF_HEAD = BUF_LAST;
            BUF_HEAD.irkey_state_code = OT_IR_KEY_UP;
            g_ot_ir_dev.head = inc_buf(g_ot_ir_dev.head, g_ot_ir_dev.buf_len);
            osal_wakeup(&(g_ot_ir_dev.ir_key_wait));
        }
    } else if (int_state & INTMS_RCV) {
        interrupt_rcv();
    } else {
        osal_printk("logic Error: int_mask=0x%.8x, int_state=0x%.8x.\n", read_reg(IR_INTM), int_state);
    }

    ot_ir_dbg("Exit ot_ir_interrupt.\n");

    return OSAL_IRQ_HANDLED;
}

static int ot_ir_read_range(unsigned int *min_val, unsigned int *max_val, int *p)
{
    if (osal_copy_from_user(min_val, p, sizeof(unsigned int)) ||
        osal_copy_from_user(max_val, p + 1, sizeof(unsigned int))) {
        return -1;
    }
    return 0;
}

static int ot_ir_ioctl_set_basic(unsigned int cmd, unsigned long arg, int *handled)
{
    switch (cmd) {
        case IR_IOC_SET_BUF:
            if ((arg > 0) && (arg <= OT_IR_MAX_BUF)) {
                g_ot_ir_dev.buf_len = arg + 1;
                g_ot_ir_dev.head = 0;
                g_ot_ir_dev.tail = 0;
            }
            ot_ir_dbg("IR_IOC_SET_BUF->buf_len=%u\n", g_ot_ir_dev.buf_len);
            break;
        case IR_IOC_SET_ENABLE_KEYUP:
            g_ot_ir_dev.enable_keyup = arg;
            ot_ir_dbg("IR_IOC_SET_ENABLE_KEYUP->enable_keyup=%u\n", g_ot_ir_dev.enable_keyup);
            break;
        case IR_IOC_SET_ENABLE_REPKEY:
            g_ot_ir_dev.enable_repkey = arg;
            ot_ir_dbg("IR_IOC_SET_ENABLE_REPKEY->enable_repkey=%u\n", g_ot_ir_dev.enable_repkey);
            break;
        case IR_IOC_SET_REPKEY_TIMEOUTVAL:
            if (arg > 0) {
                g_ot_ir_dev.repkey_delaytime = arg;
            }
            ot_ir_dbg("IR_IOC_SET_REPKEY_TIMEOUTVAL->repkey_delaytime=%u\n", g_ot_ir_dev.repkey_delaytime);
            break;
        case IR_IOC_SET_FORMAT:
            if (arg > 3) { /* format:[0,3] */
                osal_printk("Error: IR_IOC_SET_FORMAT->invalid args=%lu\n", arg);
                return -1;
            }
            g_ot_ir_dev.dev_parm.codetype = arg;
            ot_ir_dbg("IR_IOC_SET_FORMAT->codetype=%u\n", g_ot_ir_dev.dev_parm.codetype);
            break;
        case IR_IOC_SET_CODELEN:
            g_ot_ir_dev.dev_parm.code_len = arg;
            ot_ir_dbg("IR_IOC_SET_CODELEN->code_len=%u\n", g_ot_ir_dev.dev_parm.code_len);
            break;
        case IR_IOC_SET_FREQ:
            if ((arg <= 0) || (arg > 128)) { /* 128:max freq */
                osal_printk("Error: IR_IOC_SET_FREQ->invalid args=%lu\n", arg);
                return -1;
            }
            g_ot_ir_dev.dev_parm.frequence = arg;
            ot_ir_dbg("IR_IOC_SET_FREQ->frequence=%u\n", g_ot_ir_dev.dev_parm.frequence);
            break;
        default:
            *handled = 0;
            break;
    }
    return 0;
}

static int ot_ir_ioctl_basic(unsigned int cmd, unsigned long arg, int *handled)
{
    *handled = 1;
    switch (cmd) {
        case IR_IOC_SET_BUF:
        case IR_IOC_SET_ENABLE_KEYUP:
        case IR_IOC_SET_ENABLE_REPKEY:
        case IR_IOC_SET_REPKEY_TIMEOUTVAL:
        case IR_IOC_SET_FORMAT:
        case IR_IOC_SET_CODELEN:
        case IR_IOC_SET_FREQ:
            return ot_ir_ioctl_set_basic(cmd, arg, handled);
        case IR_IOC_GET_CONFIG:
            if (osal_copy_to_user((void *)(uintptr_t)arg, &(g_ot_ir_dev.dev_parm), sizeof(ot_ir_dev_param))) {
                osal_printk("%s: copy_to_user fail!\n", __func__);
            }
            ot_ir_dbg("IR_IOC_GET_CONFIG\n");
            break;
        case IR_IOC_ENDBG:
            g_dbg_flag = 1;
            ot_ir_dbg("IR_IOC_ENDBG\n");
            break;
        case IR_IOC_DISDBG:
            g_dbg_flag = 0;
            ot_ir_dbg("IR_IOC_DISDBG\n");
            break;
        default:
            *handled = 0;
            break;
    }
    return 0;
}

static int ot_ir_ioctl_set_timing_range(int *p, unsigned short int *min_cfg, unsigned short int *max_cfg,
    const char *name)
{
    unsigned int min_val;
    unsigned int max_val;

    if (ot_ir_read_range(&min_val, &max_val, p)) {
        return -1;
    }
    if (min_val > OT_IR_TIMING_MAX || max_val > OT_IR_TIMING_MAX) {
        osal_printk("%s: invalid range min=%u, max=%u\n", __func__, min_val, max_val);
        return -1;
    }
    *min_cfg = (unsigned short int)min_val;
    *max_cfg = (unsigned short int)max_val;
    ot_ir_dbg("%s->min=%u, max=%u\n", name, (unsigned int)*min_cfg, (unsigned int)*max_cfg);
    return 0;
}

static int ot_ir_ioctl_timing(unsigned int cmd, int *p, int *handled)
{
    *handled = 1;
    switch (cmd) {
        case IR_IOC_SET_LEADS:
            return ot_ir_ioctl_set_timing_range(p, &g_ot_ir_dev.dev_parm.leads_min,
                &g_ot_ir_dev.dev_parm.leads_max, "IR_IOC_SET_LEADS");
        case IR_IOC_SET_LEADE:
            return ot_ir_ioctl_set_timing_range(p, &g_ot_ir_dev.dev_parm.leade_min,
                &g_ot_ir_dev.dev_parm.leade_max, "IR_IOC_SET_LEADE");
        case IR_IOC_SET_SLEADE:
            return ot_ir_ioctl_set_timing_range(p, &g_ot_ir_dev.dev_parm.sleade_min,
                &g_ot_ir_dev.dev_parm.sleade_max, "IR_IOC_SET_SLEADE");
        case IR_IOC_SET_CNT0_B:
            return ot_ir_ioctl_set_timing_range(p, &g_ot_ir_dev.dev_parm.cnt0_b_min,
                &g_ot_ir_dev.dev_parm.cnt0_b_max, "IR_IOC_SET_CNT0_B");
        case IR_IOC_SET_CNT1_B:
            return ot_ir_ioctl_set_timing_range(p, &g_ot_ir_dev.dev_parm.cnt1_b_min,
                &g_ot_ir_dev.dev_parm.cnt1_b_max, "IR_IOC_SET_CNT1_B");
        default:
            *handled = 0;
            break;
    }
    return 0;
}

static long ot_ir_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    int ret;
    int handled;
    int *p = (int *)(uintptr_t)arg;

    ot_ir_unused(private_data);
    ret = ot_ir_ioctl_basic(cmd, arg, &handled);
    if (!handled) {
        ret = ot_ir_ioctl_timing(cmd, p, &handled);
    }
    if (!handled) {
        osal_printk("Error: Inappropriate ioctl for device. cmd=%u\n", cmd);
        return -1;
    }
    if (ret != 0) {
        return ret;
    }

    ot_ir_config();
    return 0;
}

static int ot_ir_wait_condition_callback(const void *param)
{
    ot_ir_unused(param);
    return (g_ot_ir_dev.head != g_ot_ir_dev.tail);
}

static int ot_ir_read(char *buf, int count, long *f_pos, void *private_data)
{
    irkey_info_s irkey_to_user;
    int res = 0;

    ot_ir_unused(f_pos);
    ot_ir_unused(private_data);
retry :
    ot_ir_dbg("Enter ot_ir_read : head=%u, tail=%u, buf_len=%u\n",
        g_ot_ir_dev.head, g_ot_ir_dev.tail, g_ot_ir_dev.buf_len);

    if ((g_ot_ir_dev.head) == (g_ot_ir_dev.tail)) {
        res = osal_wait_event_interruptible(&(g_ot_ir_dev.ir_key_wait), ot_ir_wait_condition_callback, &g_ot_ir_dev);
        if (res != 0) {
            return res;
        }
        goto retry;
    } else {
        while (((g_ot_ir_dev.head) != (g_ot_ir_dev.tail)) && ((res + (int)sizeof(irkey_info_s)) <= count)) {
            irkey_to_user = BUF_TAIL;
            g_ot_ir_dev.tail = inc_buf(g_ot_ir_dev.tail, g_ot_ir_dev.buf_len);
            if (osal_copy_to_user((buf + res), &irkey_to_user, sizeof(irkey_info_s))) {
                osal_printk("%s: copy_to_user fail!\n", __func__);
            }
            res += sizeof(irkey_info_s);
        }
    }
    return res;
}

static unsigned int ot_ir_select(osal_poll_t *osal_poll, void *private_data)
{
    ot_ir_unused(private_data);
    ot_ir_dbg("Enter ot_ir_select.\n");
    if ((g_ot_ir_dev.head) != (g_ot_ir_dev.tail)) {
        return 1;
    }
    osal_poll_wait(osal_poll, &(g_ot_ir_dev.ir_key_wait));
    return 0;
}

static osal_atomic_t g_ot_ir_s_available;

static int ot_ir_open(void *private_data)
{
    int ret;

    ot_ir_unused(private_data);
    ot_ir_dbg("Enter ot_ir_open.\n");

    if (osal_atomic_dec_return(&g_ot_ir_s_available) != 0) {
        ret = osal_atomic_inc_return(&g_ot_ir_s_available);
        osal_printk("Error: device already open:%d.\n", ret);
        return -1;
    }

    osal_del_timer(&g_repkey_timeout_timer);

    g_dbg_flag = 0;

    /* init g_ot_ir_dev */
    g_ot_ir_dev.dev_parm = g_static_dev_param[0]; /* nec format */
    g_ot_ir_dev.head = 0;
    g_ot_ir_dev.tail = 0;
    g_ot_ir_dev.buf_len = DEFAULT_BUF_LEN;
    g_ot_ir_dev.enable_repkey = 1;
    g_ot_ir_dev.repkey_checkflag = 0;
    g_ot_ir_dev.repkey_delaytime = DEFAULT_DELAYTIME;
    g_ot_ir_dev.enable_keyup = 1;
    osal_wait_init(&g_ot_ir_dev.ir_key_wait);

    ot_ir_config();

    return 0;
}

static int ot_ir_release(void *private_data)
{
    ot_ir_unused(private_data);
    ot_ir_dbg("Enter ot_ir_release.\n");

    /* disable OT_IR11 */
    write_reg(IR_ENABLE, 0x0);

    if (osal_atomic_inc_return(&g_ot_ir_s_available) != 1) {
        osal_atomic_dec_return(&g_ot_ir_s_available);
        return 0;
    }

    osal_del_timer(&g_repkey_timeout_timer);

    osal_wait_destroy(&g_ot_ir_dev.ir_key_wait);

    return 0;
}

static int ot_ir_write(const char *buf, int size, long *offset, void *private_data)
{
    ot_ir_unused(buf);
    ot_ir_unused(size);
    ot_ir_unused(offset);
    ot_ir_unused(private_data);
    return 0;
}

static struct osal_fileops g_ot_ir_fops = {
    open            : ot_ir_open,
    unlocked_ioctl  : ot_ir_ioctl,
    poll            : ot_ir_select,
    read            : ot_ir_read,
    write           : ot_ir_write,
    release         : ot_ir_release,
};

static struct osal_dev *g_ot_ir_miscdev = NULL;

static int ot_ir_sw_init(void)
{
    int res;

    if (g_ir_irq <= 0) {
        g_ir_irq = OT_IR_DEVICE_IRQ_NO;
    }

    if (g_ir_reg_base == NULL) {
        g_ir_reg_base = (volatile void *)osal_ioremap(IR_REG_BASE, IR_REG_LENGTH);
        if (g_ir_reg_base == NULL) {
            osal_printk("osal_ioremap err. \n");
            return 1;
        }
        g_need_iounmap = 1;
    }

    res = osal_atomic_init(&g_ot_ir_s_available);
    if (res != 0) {
        osal_printk("Error: init atomic\n");
        if (g_need_iounmap) {
            osal_iounmap((void *)g_ir_reg_base, IR_REG_LENGTH);
            g_need_iounmap = 0;
            g_ir_reg_base = NULL;
        }
        return 1;
    }
    osal_atomic_set(&g_ot_ir_s_available, 1);

    g_repkey_timeout_timer.function = repkey_timeout_handler;
    res = osal_timer_init(&g_repkey_timeout_timer);
    if (res != 0) {
        if (g_need_iounmap) {
            osal_iounmap((void *)g_ir_reg_base, IR_REG_LENGTH);
            g_need_iounmap = 0;
            g_ir_reg_base = NULL;
        }
        osal_atomic_destroy(&g_ot_ir_s_available);
        return 1;
    }
    return 0;
}

int ot_ir_init(void)
{
    if (ot_ir_sw_init() != 0) {
        return -1;
    }

    if (ot_ir_enable_clock() != 0) {
        osal_printk("Error: enable clock fail\n");
        goto enable_clock_fail;
    }

    /* disable OT_IR */
    write_reg(IR_ENABLE, 0x00);

    g_ot_ir_miscdev = osal_createdev(OT_IR_DEVICE_NAME);
    if (g_ot_ir_miscdev == NULL) {
        osal_printk("Error: can't create dev\n");
        goto create_dev_fail;
    }

    g_ot_ir_miscdev->minor = 255; /* 255:dev_minor */
    g_ot_ir_miscdev->fops = &g_ot_ir_fops;
    if (osal_registerdevice(g_ot_ir_miscdev) != 0) {
        osal_printk("Error: can't register\n");
        goto register_device_fail;
    }

    if (osal_request_irq(g_ir_irq, ot_ir_interrupt, NULL, OT_IR_DEVICE_NAME, &ot_ir_interrupt) != 0) {
        osal_printk("Error: request IRQ(%lu) failed\n", g_ir_irq);
        goto request_irq_fail;
    }

    osal_printk("ot_ir init ok. ver=%s, %s.\n", __DATE__, __TIME__);
    return 0;

request_irq_fail:
    osal_deregisterdevice(g_ot_ir_miscdev);
register_device_fail:
    osal_destroydev(g_ot_ir_miscdev);
create_dev_fail:
    ot_ir_disable_clock();
enable_clock_fail:
    osal_timer_destroy(&g_repkey_timeout_timer);
    osal_atomic_destroy(&g_ot_ir_s_available);
    if (g_need_iounmap) {
        osal_iounmap((void *)g_ir_reg_base, IR_REG_LENGTH);
        g_need_iounmap = 0;
        g_ir_reg_base = NULL;
    }
    return -1;
}

void ot_ir_exit(void)
{
    osal_free_irq(g_ir_irq, &ot_ir_interrupt);

    osal_deregisterdevice(g_ot_ir_miscdev);
    osal_destroydev(g_ot_ir_miscdev);

    osal_del_timer(&g_repkey_timeout_timer);
    osal_timer_destroy(&g_repkey_timeout_timer);

    osal_atomic_destroy(&g_ot_ir_s_available);
    if (g_need_iounmap) {
        osal_iounmap((void *)g_ir_reg_base, IR_REG_LENGTH);
        g_need_iounmap = 0;
        g_ir_reg_base = NULL;
    }

    ot_ir_disable_clock();

    osal_printk("ot_ir exit ok.\n");
}
