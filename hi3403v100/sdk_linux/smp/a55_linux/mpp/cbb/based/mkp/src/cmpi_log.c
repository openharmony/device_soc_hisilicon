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

#ifdef __LITEOS__
#include <pthread.h>
#include <shell.h>
#include <sys/prctl.h>
#include "linux/kernel.h"
#endif
#include "ot_osal.h"
#include "securec.h"
#include "ot_type.h"
#include "ot_debug.h"
#include "ot_common_sys.h"
#include "proc_ext.h"
#include "dev_ext.h"
#include "mod_ext.h"
#include "mkp_log.h"

static td_s32 g_log_level[OT_ID_BUTT + 1] = {
    [0 ... OT_ID_BUTT - 1] = OT_DBG_ERR
};

static td_u32 g_state = LOG_STATE_INIT;
static td_bool g_wait_data = TD_TRUE;

#define base_trace_cmpi(level, fmt, ...)                                                                         \
    do {                                                                                                     \
        OT_TRACE(level, OT_ID_CMPI, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)


typedef struct {
    td_u32 max_len;         /* the max length of ring buffer    */
    td_char *addr;          /* the address of ring buffer       */
    td_u32 read_pos;        /* the read pointer of ring buffer  */
    td_u32 write_pos;       /* the write pointer of ring buffer */
    td_u32 butt_pos;        /* leave a blank for circle round   */
    osal_spinlock_t lock;   /* the lock                         */
    osal_wait_t block_wait; /* wait for read                    */
} log_buffer;

/* All the mode use the same one buffer. so the time order can be record. */
static log_buffer g_log_buf = {0};

td_s32 cmpi_log_get_level(ot_mod_id id)
{
    return g_log_level[id];
}

td_void cmpi_log_set_level(ot_mod_id id, td_s32 level)
{
    int i = 0;
    if (id < OT_ID_BUTT) {
        g_log_level[id] = level;
    } else {
        for (i = 0; i < OT_ID_BUTT; i++) {
            g_log_level[i] = level;
        }
    }
}

static td_u32 search_mod(const char *s)
{
    td_u32 i;
    for (i = 0; i < OT_ID_BUTT; i++) {
        td_char *pa_name = cmpi_get_module_name(i);
        if (pa_name && !osal_strcmp(pa_name, s)) {
            break;
        }
    }
    return i;
}

/* ate all blanks in a line */
static char *strip_string(char *s, char *d, int len)
{
    int i = 0;
    char *p = d;
    do {
        if (*s == '\n') {
            *s = '\0';
        }
        if (*s != ' ') {
            *p++ = *s;
        }
        i++;
    } while ((*s++ != '\0') && (i < len));
    return d;
}

static int separate_string(char *s, char **left, char **right)
{
    char *p = s;
    /* find '=' */
    while (*p != '\0' && *p != '=') {
        p++;
    }

    if (*p != '=') {
        return -1;
    }

    /* separate left from right value by '=' */
    *p = '\0';
    *left = s;
    *right = p + 1;
    return 0;
}

static td_s32 chk_log_level(td_s32 level, ot_mod_id mod_id)
{
    if ((mod_id >= OT_ID_BUTT) || (level > g_log_level[mod_id])) {
        return 0;
    }
    return 1;
}

td_s32 log_write(td_s32 level, ot_mod_id mod_id, const char *fmt, osal_va_list args)
{
    td_s32 new_len;
    td_s32 tmp_len;
    td_u32 old_wp;
    td_u32 old_rp;
    td_u32 new_wp;
    td_u32 new_rp;
    unsigned long lock_flag;
    td_char *addr = TD_NULL;
    td_char *mod_name = cmpi_get_module_name(mod_id);

    if ((g_log_buf.addr == NULL) || (g_log_buf.write_pos > g_log_buf.max_len) ||
        (g_log_buf.read_pos > g_log_buf.max_len)) {
        return 0;
    }

    osal_spin_lock_irqsave(&(g_log_buf.lock), &lock_flag);
    old_wp = new_wp = g_log_buf.write_pos;
    old_rp = new_rp = g_log_buf.read_pos;
    addr = g_log_buf.addr + old_wp;

    new_len = snprintf_s(addr, LOG_MAX_ITEMLEN, LOG_MAX_ITEMLEN - 1, "<%d>[%6s] ", level, mod_name);
    if (new_len < 0) {
        osal_printk("[%6s]: log_write snprintf_s err!!\n", mod_name);
        new_len = 0;
    }
    tmp_len = vsnprintf_s(addr + new_len, LOG_MAX_ITEMLEN - new_len, LOG_MAX_ITEMLEN - new_len - 1, fmt, args);
    if (tmp_len < 0) {
        osal_printk("[%6s]: log_write vsnprintf_s err!!\n", mod_name);
        tmp_len = 0;
    }
    new_len = new_len + tmp_len;
    if (new_len >= LOG_MAX_ITEMLEN) {
        osal_printk("[%6s]: log message is too long!!\n", mod_name);
    } else {
        new_wp = old_wp + new_len;
        if (new_wp >= old_rp && old_wp < old_rp) {
            new_rp = new_wp + 1;
        }
    }

    if (new_wp > g_log_buf.butt_pos || g_log_buf.max_len - new_wp <= LOG_MAX_ITEMLEN) {
        g_log_buf.butt_pos = new_wp;
        g_log_buf.write_pos = 0;
    } else {
        g_log_buf.write_pos = new_wp;
    }
    g_log_buf.read_pos = new_rp;

    osal_spin_unlock_irqrestore(&(g_log_buf.lock), &lock_flag);
    osal_wakeup(&(g_log_buf.block_wait));

    return new_len;
}

td_s32 OT_LOG(td_s32 level, ot_mod_id mod_id, const char *fmt, ...)
{
    osal_va_list args;
    td_s32 r = 0;
    if (!chk_log_level(level, mod_id)) {
        return 0;
    }

    osal_va_start(args, fmt);
#ifndef CONFIG_RISCV
    r = log_write(level, mod_id, fmt, args);
#endif
    osal_va_end(args);
    return r;
}

/* use the "cat /proc/umap/log"  to get the debug level */
static td_s32 log_proc_read(struct osal_proc_dir_entry *s)
{
    td_u32 i;

    osal_seq_printf(s, "----------------------------------------log buffer state------------------"
        "----------------------------------------------\n");
    osal_seq_printf(s, "max_len(KB)  read_pos write_pos butt_pos\n");
    osal_seq_printf(s, "%11d %9d %9d %8d\n",
                    g_log_buf.max_len >> 10, g_log_buf.read_pos, /* 10:Divide by 1024, B transform to KB */
                    g_log_buf.write_pos, g_log_buf.butt_pos);
    osal_seq_printf(s, "\n");

    osal_seq_printf(s, "----------------------------------------current log level-----------------"
        "----------------------------------------------\n");
    for (i = 0; i < OT_ID_BUTT; i++) {
        td_char *tmp_name = cmpi_get_module_name(i);
        if (tmp_name == NULL) {
            continue;
        }
        osal_seq_printf(s, "%s\t:  %d\n", tmp_name, g_log_level[i]);
    }

    return 0;
}

static td_void log_set_proc_cfg(int level, const char *left)
{
    ot_log_level_cfg level_cfg;

    if (!osal_strcmp("all", left)) {
        cmpi_log_set_level(OT_ID_BUTT, level);
        return;
    }

    level_cfg.level = level;
    level_cfg.mod_id = search_mod(left);
    if (level_cfg.mod_id >= OT_ID_BUTT) {
        osal_printk("%s not found in array!\n", left);
        return;
    }

    cmpi_log_set_level(level_cfg.mod_id, level_cfg.level);

    return;
}

/* use the "echo 'mode = level' > /proc/umap/log"  to modify the debug level */
static td_s32 log_proc_write(struct osal_proc_dir_entry *s, const char *buf, int count, long long *ppos)
{
#define TMP_BUF_LEN     32
    char m[TMP_BUF_LEN] = {0};
    char d[TMP_BUF_LEN] = { [0 ... TMP_BUF_LEN - 1] = '\0' };
    int len = TMP_BUF_LEN;
    char *left = NULL;
    char *right = NULL;
    int level;

    ot_unused(s);
    if ((*ppos >= TMP_BUF_LEN) || (count <= 0)) {
        return TD_FAILURE;
    }

    len = osal_min(len, count);
    if (osal_copy_from_user(m, buf, len) != 0) {
        return TD_FAILURE;
    }

    m[len - 1] = '\0';
    strip_string(m, d, len);
    if (separate_string(d, &left, &right)) {
        osal_printk("string is unknown!\n");
        goto out;
    }

    level = osal_strtol(right, NULL, 10); /* 10:Convert to decimal */
    if (!osal_strcmp("wait", left)) {
        g_wait_data = level;
        goto out;
    }

    if (!level && *right != '0') {
        osal_printk("invalid value!\n");
        goto out;
    }

    if (level > OT_DBG_DEBUG) {
        osal_printk("level: %d illegal!\n", level);
        goto out;
    }

    log_set_proc_cfg(level, left);
out:
    return len;
#undef TMP_BUF_LEN
}

static td_s32 log_close(td_void *private_data);

static td_s32 log_set_level_cfg(ot_log_level_cfg *conf)
{
    conf->mod_name[sizeof(conf->mod_name) - 1] = '\0';
    if (!osal_strcmp("all", conf->mod_name)) {
        conf->mod_id = OT_ID_BUTT;
        cmpi_log_set_level(OT_ID_BUTT, conf->level);
    } else {
        if (conf->mod_id >= OT_ID_BUTT) {
            base_trace_cmpi(OT_DBG_ERR, "illegal parma, modeid: %d\n", conf->mod_id);
            return OT_ERR_SYS_ILLEGAL_PARAM;
        }
        cmpi_log_set_level(conf->mod_id, conf->level);
    }
    return TD_SUCCESS;
}

static td_s32 log_get_level_cfg(ot_log_level_cfg *conf)
{
    errno_t err_value;

    if (cmpi_get_module_name(conf->mod_id) == NULL) {
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }
    conf->level = cmpi_log_get_level(conf->mod_id);
    err_value = strncpy_s(conf->mod_name, sizeof(conf->mod_name),  cmpi_get_module_name(conf->mod_id),
        sizeof(conf->mod_name) - 1);
    if (err_value != EOK) {
        base_trace_cmpi(OT_DBG_ERR, "copy err!\n");
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 log_ioctl_set_level(unsigned long arg)
{
    ot_log_level_cfg *conf = (ot_log_level_cfg *)(td_uintptr_t)arg;

    if (conf == TD_NULL) {
        base_trace_cmpi(OT_DBG_ERR, "NULL PTR\n");
        return OT_ERR_SYS_NULL_PTR;
    }
    if (conf->level > OT_DBG_DEBUG) {
        base_trace_cmpi(OT_DBG_ERR, "illegal level: %d\n", conf->level);
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }
    if (log_set_level_cfg(conf) != TD_SUCCESS) {
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }
    return TD_SUCCESS;
}

static td_s32 log_ioctl_get_level(unsigned long arg)
{
    ot_log_level_cfg *conf = (ot_log_level_cfg *)(td_uintptr_t)arg;

    if (conf == TD_NULL) {
        base_trace_cmpi(OT_DBG_ERR, "NULL PTR\n");
        return OT_ERR_SYS_NULL_PTR;
    }
    if (conf->mod_id >= OT_ID_BUTT) {
        base_trace_cmpi(OT_DBG_ERR, "illegal parma, modeid: %d\n", conf->mod_id);
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }
    if (log_get_level_cfg(conf) != TD_SUCCESS) {
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }
    return TD_SUCCESS;
}

static td_s32 log_ioctl_set_wait_flag(unsigned long arg)
{
    td_bool wait;

    if (((td_bool *)(td_uintptr_t)arg) == TD_NULL) {
        base_trace_cmpi(OT_DBG_ERR, "NULL PTR\n");
        return OT_ERR_SYS_NULL_PTR;
    }
    wait = *((td_bool *)(td_uintptr_t)arg);
    if ((wait != TD_FALSE) && (wait != TD_TRUE)) {
        base_trace_cmpi(OT_DBG_ERR, "illegal parma, wait: %d\n", wait);
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    g_wait_data = wait;
    return TD_SUCCESS;
}

/* IOCTL operation for get and config the debug level */
static td_s32 do_ioctl(unsigned int cmd, unsigned long arg, td_void *private_data)
{
    ot_unused(private_data);
    switch (cmd) {
        case LOG_SETLOGLEVEL_CTRL:
            return log_ioctl_set_level(arg);
        case LOG_GETLOGLEVEL_CTRL:
            return log_ioctl_get_level(arg);
        case LOG_CLOSE_CTRL: {
            log_close(TD_NULL);
            break;
        }
        case LOG_SETWAITFLAG_CTRL:
            return log_ioctl_set_wait_flag(arg);
        default: {
            break;
        }
    }

    return TD_SUCCESS;
}

static long log_ioctl(unsigned int cmd, unsigned long arg, td_void *private_data)
{
    return do_ioctl(cmd, arg, private_data);
}

#ifdef __LITEOS__
td_void *read_q(td_void *data)
{
    int c;

    prctl(PR_SET_NAME, "ot_log_read_q", 0, 0, 0);
    while (1) {
        c = getchar();
        if ((c == 'q') || (c == 'Q')) {
            log_close(TD_NULL);
            return 0;
        }
    }
}
#endif

int wait_condition_call_back(const td_void *param)
{
    ot_unused(param);
    return ((g_log_buf.read_pos != g_log_buf.write_pos) ||
            (g_state == LOG_STATE_CLOSE) ||
            (g_wait_data == TD_FALSE));
}

static td_bool is_log_buf_empty(void)
{
    if (g_log_buf.read_pos > g_log_buf.butt_pos) {
        g_log_buf.read_pos = 0;
    }

    if (g_log_buf.write_pos == g_log_buf.read_pos) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

/* read operation for log information */
static int log_read(char *buf, int count, long *offset, td_void *private_data)
{
    td_s32 read_len = 0;
    td_s32 error;
    td_char c;
    unsigned long lock_flag;

    ot_unused(offset);
    ot_unused(private_data);
#ifdef __LITEOS__

    count = LOG_BUFFER_LEN * LOG_BUFFER_UNIT_LEN;
    pthread_t threadid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&threadid, &attr, read_q, NULL) != 0) {
        OT_PRINT("cmpi_log read pthread_create error!\n");
        return -1;
    }
#endif

    g_state = LOG_STATE_READ;

    error = osal_wait_event_interruptible(&g_log_buf.block_wait, wait_condition_call_back, NULL);
    if (error) {
        return error;
    }
    if (g_state != LOG_STATE_READ) {
        return 0;
    }

    osal_spin_lock_irqsave(&(g_log_buf.lock), &lock_flag);
    while ((!error) && (read_len < count) && (g_log_buf.write_pos != g_log_buf.read_pos)) {
            if (is_log_buf_empty()) {
                break;
            }

            c = *(g_log_buf.addr + g_log_buf.read_pos);
            g_log_buf.read_pos += 1;

            osal_spin_unlock_irqrestore(&(g_log_buf.lock), &lock_flag);

#ifdef __LITEOS__
            printk("%c", c);
#else
            error = (td_s32)osal_copy_to_user(buf, &c, 1);
            buf++;
#endif
            read_len++;
            osal_yield();
            osal_spin_lock_irqsave(&(g_log_buf.lock), &lock_flag);
        }
    osal_spin_unlock_irqrestore(&(g_log_buf.lock), &lock_flag);

    return read_len;
}

#ifdef __LITEOS__
void cat_log_shell(td_void)
{
    osCmdReg(CMD_TYPE_EX, "cat_logmpp", 0, (CMD_CBK_FUNC)log_read);
}
#endif

td_s32 cmpi_log_sys_init(td_void)
{
    return TD_SUCCESS;
}

td_s32 cmpi_log_sys_deinit(td_void)
{
    return TD_SUCCESS;
}

static td_s32 log_open(td_void *data)
{
    ot_unused(data);
    g_state = LOG_STATE_OPEN;
    return TD_SUCCESS;
}

static td_s32 log_close(td_void *data)
{
    ot_unused(data);
    g_state = LOG_STATE_CLOSE;
    g_wait_data = TD_TRUE;
    osal_wakeup(&(g_log_buf.block_wait));
    return TD_SUCCESS;
}

static struct osal_fileops g_log_file_op = {
    .open = log_open,
    .unlocked_ioctl = log_ioctl,
    .release = log_close,
    .read = log_read
};

static osal_dev_t *g_log_device = TD_NULL;

#ifndef CONFIG_RISCV
static td_s32 cmpi_log_alloc_log_buf(td_u32 log_buf_len)
{
    if ((log_buf_len >= 2) && (log_buf_len <= 128)) { /* 2,128:length range of the log buffer */
        g_log_buf.max_len = log_buf_len * LOG_BUFFER_UNIT_LEN;
    } else {
        g_log_buf.max_len = LOG_BUFFER_LEN * LOG_BUFFER_UNIT_LEN;
    }
    g_log_buf.butt_pos = g_log_buf.max_len;
    g_log_buf.read_pos = g_log_buf.write_pos = 0;
    g_log_buf.addr = (td_char *)osal_vmalloc(g_log_buf.max_len);
    if (g_log_buf.addr == NULL) {
        osal_printk("log buffer %d_b malloc faild.\n", g_log_buf.max_len);
        return TD_FAILURE;
    }
    (td_void)memset_s(g_log_buf.addr, g_log_buf.max_len, 0, g_log_buf.max_len);

    return TD_SUCCESS;
}

static td_void cmpi_log_free_log_buf(td_void)
{
    if (g_log_buf.addr != TD_NULL) {
        osal_vfree(g_log_buf.addr);
        g_log_buf.addr = NULL;
    }
}
#endif

static td_s32 cmpi_log_buf_init(td_u32 log_buf_len)
{
    if (osal_spin_lock_init(&(g_log_buf.lock)) < 0) {
        osal_printk("spinlock init failed!\n");
        return TD_FAILURE;
    }

    if (osal_wait_init(&(g_log_buf.block_wait)) < 0) {
        osal_printk("wait init failed!\n");
        osal_spin_lock_destroy(&(g_log_buf.lock));
        return TD_FAILURE;
    }

#ifndef CONFIG_RISCV
    if (cmpi_log_alloc_log_buf(log_buf_len) != TD_SUCCESS) {
        osal_wait_destroy(&(g_log_buf.block_wait));
        osal_spin_lock_destroy(&(g_log_buf.lock));
        return TD_FAILURE;
    }
#endif

    return TD_SUCCESS;
}

td_void cmpi_log_unregisterdevice(td_void)
{
    osal_deregisterdevice(g_log_device);
    osal_destroydev(g_log_device);
    g_log_device = TD_NULL;
}

td_s32 cmpi_log_registerdevice(td_void)
{
    g_log_device = osal_createdev(UMAP_DEVNAME_LOG_BASE);
    if (g_log_device == NULL) {
        osal_printk("log createdev failed!\n");
        return TD_FAILURE;
    }
    g_log_device->fops = &g_log_file_op;
    g_log_device->minor = UMAP_LOG_MINOR_BASE;

    if (osal_registerdevice(g_log_device)) {
        osal_printk("log register device failed!\n");
        osal_destroydev(g_log_device);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 cmpi_log_create_proc_entry(td_void)
{
    osal_proc_entry_t *item = NULL;
    item = osal_create_proc_entry(PROC_ENTRY_LOG, NULL);
    if (item == NULL) {
        return TD_FAILURE;
    }
    item->read = log_proc_read;
    item->write = log_proc_write;

    return TD_SUCCESS;
}

td_void cmpi_log_remove_proc_entry(td_void)
{
    osal_remove_proc_entry(PROC_ENTRY_LOG, NULL);
}

td_s32 cmpi_log_init(td_u32 log_buf_len)
{
    if (cmpi_log_registerdevice() != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (cmpi_log_create_proc_entry() != TD_SUCCESS) {
        cmpi_log_unregisterdevice();
        return TD_FAILURE;
    }
    if (cmpi_log_buf_init(log_buf_len) != TD_SUCCESS) {
        cmpi_log_unregisterdevice();
        cmpi_log_remove_proc_entry();
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_void cmpi_log_exit(td_void)
{
    if (g_state == LOG_STATE_READ) {
        g_state = LOG_STATE_CLOSE;
        g_wait_data = TD_TRUE;
        osal_wakeup(&(g_log_buf.block_wait));
    }
#ifndef CONFIG_RISCV
    cmpi_log_free_log_buf();
#endif
    osal_spin_lock_destroy(&(g_log_buf.lock));
    osal_wait_destroy(&(g_log_buf.block_wait));

    cmpi_log_remove_proc_entry();
    cmpi_log_unregisterdevice();
}
