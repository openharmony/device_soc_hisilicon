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
#include "drv_hdmi_debug.h"
#include <linux/file.h>
#include <linux/major.h>
#include <linux/version.h>
#include "drv_hdmi_common.h"
#include "drv_hdmi_intf.h"
#include "drv_hdmi_event.h"
#include "drv_hdmi_edid.h"
#include "hdmi_osal.h"
#include "drv_hdmi_compatibility.h"

/* Constants for reported magic-number values. */
#define HDMI_DEBUG_ARG_INDEX_2                 2
#define HDMI_DEBUG_ARRAY_INDEX_2               2

#define DEBUG_MAX_CMD_CHAR_NUM  128
#define DEBUG_CHAR_SPACE        0x20
#define DEBUG_CHAR_TAB          0x09
#define DEBUG_CHAR_END          0x0a
#define DEBUG_MAX_EDIDBUF_SIZE  2048
#define DEBUG_PROC_BUFFER_SIZE  1024
#define SERIAL_MAJOR            204
#define DEBUG_FRL_CTRL_TYPE_MAX 0x3
#define STR_SUCC                "success"
#define STR_FAIL                "fail"
#define DEBUG_CMD_ARGC_COUNT_1  2
#define DEBUG_CMD_ARGC_COUNT_2  3
#define DEBUG_CMD_ARGC_COUNT_3  4
#define DEBUG_MAX_DATA_SIZE     5000
#define DEBUG_TIME_TEST_MAX     10
#define SCDC_SET_WAIT           90
#define SCDC_GET_WAIT           3000
#define FRL_CRC_WAIT            1000
#define FRL_WAIT                50
#define DEBUG_MAX_FRL_TIME      5000
#define DEBUG_NUM_2             2
#define DEBUG_NUM_3             3
#define DEBUG_NUM_4             4
#define DEBUG_NUM_5             5
#define DEBUG_NUM_8             8
#define DEBUG_THOUSAND          1000
#define to_lower(x)             ((x) | 0x20)
#define DEBUG_MAX_DELAY_MS      100000
#define DEFAULT_ECHO_DEVICE_HANDLE 0
#define DEBUG_RC_MAX            4
#define DEBUG_CMD_ARGC_NUM      2
#define DEBUG_NUMBER_STR_MAX_LEN 12
#define DEBUG_MAX_THREAD_TIME 100

#define hdmi_dbg_err(param...)                                                    \
    do {                                                                          \
        hdmi_dbg_print("[HDMI DBG ERROR]: %s [%u],", __func__, (td_u32)__LINE__); \
        hdmi_dbg_print(param);                                                    \
    } while (0)

#define debug_null_check_return(p)                  \
    do {                                            \
        if ((p) == TD_NULL) {                       \
            hdmi_warn("%s null pointer!\n", #p);    \
            hdmi_dbg_err("%s null pointer!\n", #p); \
            return TD_FAILURE;                      \
        }                                           \
    } while (0)

#define debug_failure_check_return(ret)        \
    do {                                       \
        if ((ret) != TD_SUCCESS) {             \
            hdmi_warn("return failure!\n");    \
            hdmi_dbg_err("return failure!\n"); \
            return TD_FAILURE;                 \
        }                                      \
    } while (0)

typedef td_s32 (*debug_func)(hdmi_debug_cmd_arg *cmd_arg, hdmi_device *hdmi_dev);
typedef struct {
    td_char cmd_index;
    debug_func cmd_func;
} frl_debug_func;

typedef struct {
    td_u8 *num;
    td_u32 min;
    td_u32 max;
    td_u8 *error_info;
    td_u8 *success_info;
} compatibility_debug_info;

static hdmi_debug_edid g_debug_edid = {0};

static const char *g_event_str_table[] = {
    "HPD",                  // HDMI_EVENT_HOTPLUG  0x10
    "HOTUNPLUG",            // HDMI_EVENT_HOTUNPLUG
    "EDID_FAIL",            // HDMI_EVENT_EDID_FAIL
    "HDCP_FAIL",            // HDMI_EVENT_HDCP_FAIL
    "HDCP_SUCCESS",         // HDMI_EVENT_HDCP_SUCCESS
    "RSEN_CONNECT",         // HDMI_EVENT_RSEN_CONNECT
    "RSEN_DISCONNECT",      // HDMI_EVENT_RSEN_DISCONNECT
    "HDCP_USERSETTING",     // HDMI_EVENT_HDCP_USERSETTING
    "HDCP_OFF",             // HDMI_EVENT_HDCP_OFF
    "SCRAMBLE_FAIL",        // HDMI_EVENT_SCRAMBLE_FAIL
    "SCRAMBLE_SUCCESS",     // HDMI_EVENT_SCRAMBLE_SUCCESS
    "ZERO_DRM_TIMEOUT",     // HDMI_EVENT_ZERO_DRMIF_TIMEOUT
    "HDR_SWITCH_TIMEOUT"    // HDMI_EVENT_SWITCH_TO_HDRMODE_TIMEOUT
};

typedef enum {
    COMPAT_MUTE_SEND,
    COMPAT_AVI_USE_GEN5,
    COMPAT_COMPAT_INFO_MAX
} compat_info_type;

static const compatibility_debug_info g_debug_compat_table[COMPAT_COMPAT_INFO_MAX] = {
    {  "0", 0,  1, "setmute_send: 0 ~ 1 \n", "\n setmute_send change success! \n" }
};

static td_s32 debug_print_help(const hdmi_debug_cmd_arg *cmd_arg);

static td_void debug_echo_helper_vargs(td_char *buf, td_u32 size, const td_char *fmt, osal_va_list args)
{
    td_s32 ret;
    struct kstat stat = {0};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
        struct file *fi = fget(DEFAULT_ECHO_DEVICE_HANDLE);
        ret = vfs_getattr(&(fi->f_path), &stat, STATX_BASIC_STATS, AT_NO_AUTOMOUNT);
#else
        ret = vfs_fstat(DEFAULT_ECHO_DEVICE_HANDLE, &stat);
#endif
    if (ret) {
        hdmi_err("default echo device handle(%u) invalid!\n", DEFAULT_ECHO_DEVICE_HANDLE);
        return;
    }

    if (size == 0) {
        return;
    }

    ret = vsnprintf_s(buf, size, size - 1, fmt, args);
    if (ret < 0) {
        hdmi_err("vsnprintf_s err\n");
        return;
    }
    /* echo device must be chrdev and major number must be SERIAL_MAJOR or TTYAUX_MAJOR or UNIX98_PTY_SLAVE_MAJOR */
    if (S_ISCHR(stat.mode) &&
        (MAJOR(stat.rdev) == SERIAL_MAJOR || MAJOR(stat.rdev) == TTYAUX_MAJOR ||
        MAJOR(stat.rdev) == UNIX98_PTY_SLAVE_MAJOR)) {
        struct file *f = fget(DEFAULT_ECHO_DEVICE_HANDLE);
        if (f != TD_NULL) {
            /* file pos is invalid for chrdev */
            hdmi_osal_file_write(f, buf, osal_strlen(buf));
            fput(f);
        }
    } else {
        hdmi_err("default echo device is invalid!\n");
    }
}

static td_void hdmi_dbg_print(const td_char *fmt, ...)
{
    osal_va_list args;
    td_char *buf = TD_NULL;

    if (fmt == TD_NULL) {
        hdmi_err("invalid argument fmt!\n");
        return;
    }
    buf = osal_vmalloc(DEBUG_PROC_BUFFER_SIZE);
    if (buf == TD_NULL) {
        hdmi_err("memory allocate failed for proc\n");
        return;
    }
    (td_void)memset_s(buf, DEBUG_PROC_BUFFER_SIZE, 0, DEBUG_PROC_BUFFER_SIZE);

    osal_va_start(args, fmt);
    debug_echo_helper_vargs(buf, DEBUG_PROC_BUFFER_SIZE, fmt, args);
    osal_va_end(args);
    osal_vfree(buf);
    buf = TD_NULL;

    return;
}

static td_bool is_hex_space(td_char a)
{
    td_bool is_space;

    is_space = (a == DEBUG_CHAR_SPACE || a == DEBUG_CHAR_TAB ||
                a == ',' || a == ';' || a == '\0' || a == '\n' || a == '\r');

    return is_space;
}

static td_bool is_hex_char(td_char a)
{
    return ((a >= '0' && a <= '9') || (a >= 'a' && a <= 'f') || (a >= 'A' && a <= 'F'));
}

static td_u8 hex_value_get(td_char a)
{
    if ((a >= '0' && a <= '9')) {
        return a - '0';
    } else if (a >= 'a' && a <= 'f') {
        return a - 'a' + 0xa;
    } else if (a >= 'A' && a <= 'F') {
        return a - 'A' + 0xa;
    } else {
        return 0xff;
    }
}

static td_bool is_hex_start(const td_char *p, const td_char **end)
{
    debug_null_check_return(p);

    if (*p == '0' && (p[1] == 'x' || p[1] == 'X')) {
        if (end != TD_NULL) {
            *end = p + 2; /* P + 2, mean 16 hex value addr */
        }
        return TD_TRUE;
    } else if (is_hex_char(p[0]) &&
        (is_hex_space(p[1]) || (is_hex_char(p[1]) && is_hex_space(p[HDMI_DEBUG_ARRAY_INDEX_2])))) {
        if (end != TD_NULL) {
            *end = p;
        }
        return TD_TRUE;
    } else {
        if (end != TD_NULL) {
            *end = p;
        }
        return TD_FALSE;
    }
}

static td_s32 debug_str_to_hex(td_char *dest, td_s32 dlen, const td_char *src, td_u32 slen)
{
    td_u8 result;
    const td_char *p = src;
    td_s32 old_len = dlen;

    while (p < (src + slen) && dlen) {
        if (is_hex_start(p, &p) == TD_FALSE) {
            p++;
            continue;
        }
        if (is_hex_char(p[0]) == TD_FALSE) {
            return TD_FAILURE;
        }
        result = hex_value_get(p[0]);
        p++;
        if (is_hex_char(p[0])) {
            result <<= 4; /* 4, means get the high 8 bit data of 16 bit data */
            result |= hex_value_get(p[0]);
            p++;
        }

        if (dest != TD_NULL) {
            *dest++ = result;
            dlen--;
        }
    }

    return old_len - dlen;
}

static hdmi_debug_base debug_base_get(const td_char *str)
{
    if (str[0] == '0') {
        if (to_lower((td_u32)str[1]) == 'x') {
            return HDMI_DEBUG_BASE_HEX;
        }
        return HDMI_DEBUG_BASE_OSD;
    }
    return HDMI_DEBUG_BASE_DEC;
}

static td_u32 debug_str_to_u32(const td_char *str, td_char **end, hdmi_debug_base base)
{
    td_u32 result = 0;

    debug_null_check_return(str);

    if (strlen(str) > DEBUG_NUMBER_STR_MAX_LEN) {
        hdmi_warn("number is too large!\n");
        return TD_FAILURE;
    }

    if (base != HDMI_DEBUG_BASE_OSD && base != HDMI_DEBUG_BASE_DEC && base != HDMI_DEBUG_BASE_HEX) {
        base = debug_base_get(str);
    }

    if (base == HDMI_DEBUG_BASE_HEX && str[0] == '0' && to_lower((td_u8)str[1]) == 'x') {
        str += 2; /* str + 2, mean 16 hex value addr */
    }

    while (*str) {
        td_u32 value;

        /* value size of 10 - 15 */
        value = (((*str) >= '0') && ((*str) <= '9')) ? ((*str) - '0') : (to_lower((td_u8)(*str)) - 'a' + 10);
        if (value >= base) {
            break;
        }
        result = result * base + value;
        str++;
    }
    if (end != TD_NULL) {
        *end = (td_char *)str;
    }

    return result;
}

static struct file *debug_file_open(const char *filename, int flags, int mode)
{
    return osal_klib_fopen(filename, flags, mode);
}

static void debug_file_close(struct file *filp)
{
    if (filp != TD_NULL) {
        osal_klib_fclose(filp);
    }
}

static int debug_file_read(char *buf, unsigned int len, struct file *filp)
{
    int readlen;
    /* use osal public interface. */
    readlen = osal_klib_fread(buf, len, filp);
    return readlen;
}

static td_s32 debug_thread(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_s32 ret = TD_FAILURE;
    hdmi_device *hdmi_dev = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        ret = hdmi_thread_state_set(hdmi_dev, HDMI_THREAD_STATE_STOP);
        hdmi_dbg_print("\n thread set OFF %s! \n\n", (ret != TD_SUCCESS) ? STR_FAIL : STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        ret = hdmi_thread_state_set(hdmi_dev, HDMI_THREAD_STATE_RUN);
        hdmi_dbg_print("\n thread set ON %s! \n\n", (ret != TD_SUCCESS) ? STR_FAIL : STR_SUCC);
    } else {
        hdmi_dbg_err("error argvs!\n");
        goto usage_error;
    }

    return ret;

usage_error:
    hdmi_dbg_print("[usage  ]: echo thread argv1 > /proc/umap/hdmi0\n");
    hdmi_dbg_print("[argv1  ]: 0\t--thread off.\n"
                   "           1\t--thread on.\n");
    hdmi_dbg_print("[example]: echo thread 1 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_oe(const hdmi_debug_cmd_arg *cmd_arg)
{
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_black_frame_info black_info = {0};

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    black_info.in_bit_depth = hdmi_dev->attr.vo_attr.in_bit_depth;
    black_info.in_color_space = hdmi_dev->attr.vo_attr.in_color_space;
    black_info.in_quantization = (black_info.in_color_space == HDMI_COLORSPACE_RGB) ?
        hdmi_dev->attr.vo_attr.rgb_quantization : hdmi_dev->attr.vo_attr.ycc_quantization;

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        hal_call_void(hdmi_dev, hal_hdmi_phy_output_enable_set, TD_FALSE);
        hdmi_dbg_print("\n oe set OFF %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        hal_call_void(hdmi_dev, hal_hdmi_phy_output_enable_set, TD_TRUE);
        hdmi_dbg_print("\n oe set ON %s! \n\n", STR_SUCC);
    } else {
        hdmi_dbg_err("error argvs!\n");
        goto usage_error;
    }

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo oe argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0\t--oe off.\n"
                   "           1\t--oe on.\n");
    hdmi_dbg_print("[example]: echo oe 1 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_event(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_s32      ret = TD_FAILURE;
    hdmi_event  event;
    hdmi_device *hdmi_dev = TD_NULL;
    td_char     *str      = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    str = cmd_arg->argv[1];

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || str == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    event = (hdmi_event)debug_str_to_u32(str, &str, HDMI_DEBUG_BASE_DEC);
    if ((event >= HDMI_EVENT_HOTPLUG) &&
        (event < HDMI_EVENT_BUTT)) {
        ret = drv_hdmi_event_pool_write(cmd_arg->hdmi_id, event);
        if ((event - HDMI_EVENT_HOTPLUG) < hdmi_array_size(g_event_str_table)) {
            hdmi_dbg_print("hardware event %s simulate %s.\n",
                g_event_str_table[event - HDMI_EVENT_HOTPLUG], (ret != TD_SUCCESS) ? STR_FAIL : STR_SUCC);
        }
    } else {
        goto usage_error;
    }

    return ret;

usage_error:
    hdmi_dbg_print("[usage  ]: echo event argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: event type(DECMICAL) as follow\n");
    for (event = HDMI_EVENT_HOTPLUG;
         event < HDMI_EVENT_BUTT && ((event - HDMI_EVENT_HOTPLUG) < hdmi_array_size(g_event_str_table));
         event++) {
        hdmi_dbg_print("%-10s %u\t-- (0x%02x)%s\n", "", event, event,
            g_event_str_table[event - HDMI_EVENT_HOTPLUG]);
    }
    hdmi_dbg_print("[example]: echo event 16 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_edid_from_file(hdmi_device *hdmi_dev, const hdmi_debug_cmd_arg *cmd_arg)
{
    struct file *f;
    td_u8 *buf = TD_NULL;
    td_s32 ret;
    td_s32 len = 0;
    hdmi_debug_edid *dbg_edid = &g_debug_edid;

    f = debug_file_open(cmd_arg->argv[HDMI_DEBUG_ARG_INDEX_2], OSAL_O_RDONLY, 0);
    if (f == TD_NULL) {
        hdmi_dbg_err("open file %s fail!\n", cmd_arg->argv[2]);
        return TD_FAILURE;
    }

    buf = (td_u8 *)osal_vmalloc(DEBUG_MAX_EDIDBUF_SIZE);
    if (buf == TD_NULL) {
        hdmi_dbg_err("vmalloc error!\n");
        goto err;
    }
    (td_void)memset_s(buf, DEBUG_MAX_EDIDBUF_SIZE, 0, DEBUG_MAX_EDIDBUF_SIZE);
    (td_void)memset_s(dbg_edid, sizeof(g_debug_edid), 0, sizeof(hdmi_debug_edid));

    len = debug_file_read(buf, DEBUG_MAX_EDIDBUF_SIZE, f);
    if (len > 0) {
        len = debug_str_to_hex(dbg_edid->data, HDMI_EDID_TOTAL_SIZE, buf, (td_u32)len);
        if (len >= 0) {
            dbg_edid->len = len;
            dbg_edid->data_valid = TD_TRUE;
            hdmi_dev->edid_info.status.raw_len = len;
            hdmi_dev->edid_info.status.raw_valid = TD_TRUE;
            hdmi_dev->edid_info.status.cap_valid = TD_TRUE;
            ret = memcpy_s(hdmi_dev->edid_info.edid_raw, HDMI_EDID_TOTAL_SIZE, dbg_edid->data, len);
            if (ret != EOK) {
                hdmi_dbg_err("memcpy_s err!\n");
                osal_vfree(buf);
                buf = TD_NULL;
                return TD_FAILURE;
            }
            drv_hdmi_edid_update(&hdmi_dev->edid_info, HDMI_EDID_UPDATE_DEBUG);
        }
    }
    osal_vfree(buf);
    buf = TD_NULL;

err:
    debug_file_close(f);
    if (len < 0) {
        hdmi_dbg_err("error hex raw data format!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void print_raw_edid(hdmi_edid_info *edid_info)
{
    td_s32 i;
    td_s32 ret;
    td_u8 *buf = TD_NULL;

    buf = (td_u8 *)osal_vmalloc(HDMI_EDID_TOTAL_SIZE);
    if (buf != TD_NULL) {
        (td_void)memset_s(buf, HDMI_EDID_TOTAL_SIZE, 0, HDMI_EDID_TOTAL_SIZE);
        ret = drv_hdmi_edid_raw_get(edid_info, buf, HDMI_EDID_TOTAL_SIZE);
        for (i = 0; (i < ret) && (i < HDMI_EDID_TOTAL_SIZE); i++) {
            if ((i % 16) == 0) { /* each Line length of 16 characters  */
                hdmi_dbg_print("\n");
            }
            hdmi_dbg_print(" %02x", buf[i]);
        }

        hdmi_dbg_print("\n\n");
        osal_vfree(buf);
        buf = TD_NULL;
    } else {
        hdmi_dbg_err("vmalloc fail!\n");
    }

    return;
}

static td_s32 debug_edid(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_s32 ret = TD_FAILURE;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_debug_edid *dbg_edid = &g_debug_edid;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc < DEBUG_CMD_ARGC_COUNT_1) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    debug_null_check_return(cmd_arg->argv[1]);

    if (is_str_equal(cmd_arg->argv[1], "s") ||
        is_str_equal(cmd_arg->argv[1], "0")) {
        (td_void)memset_s(dbg_edid, sizeof(g_debug_edid), 0, sizeof(hdmi_debug_edid));
        ret = drv_hdmi_edid_update(&hdmi_dev->edid_info, HDMI_EDID_UPDATE_SINK);
        if (ret != TD_SUCCESS) {
            hdmi_dbg_err("update edid from sink fail!make sure cable connect!\n");
        }
    } else if (is_str_equal(cmd_arg->argv[1], "f") ||
               is_str_equal(cmd_arg->argv[1], "1")) {
        if (cmd_arg->argc < DEBUG_CMD_ARGC_COUNT_2) {
            hdmi_dbg_err("error argcs!\n");
            goto usage_error;
        }
        debug_null_check_return(cmd_arg->argv[2]);
        ret = debug_edid_from_file(hdmi_dev, cmd_arg);
        debug_failure_check_return(ret);
    } else {
        hdmi_dbg_err("error argvs!\n");
        goto usage_error;
    }

    hdmi_dbg_print(" edid load %s! \n", (ret < 0) ? STR_FAIL : STR_SUCC);
    hdmi_dbg_print("[edid data]:");
    print_raw_edid(&hdmi_dev->edid_info);

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo edid argv1 argv2 > /proc/umap/hdmi0\n");
    hdmi_dbg_print("[argv1  ]: edid operation mode as below\n");
    hdmi_dbg_print("%10s 0(s)  --finish using debug edid and read from sink.\n", "");
    hdmi_dbg_print("%10s 1(f)  --read debug edid from argv2(a hex-string-file-path).\n", "");
    hdmi_dbg_print("[argv2  ]: you have to use at least 1 character of 'SPACE|TAB|,|0x' between 2 hex-bytes.\n");
    hdmi_dbg_print("[example]: echo edid s > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[example]: echo edid f /mnt/edid_skyworth_300m.txt > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_avmute(const hdmi_debug_cmd_arg *cmd_arg)
{
    hdmi_device *hdmi_dev = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        drv_hdmi_avmute_set(hdmi_dev, TD_FALSE);
        hdmi_dbg_print("\n avmute set OFF %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        drv_hdmi_avmute_set(hdmi_dev, TD_TRUE);
        hdmi_dbg_print("\n avmute set ON %s! \n\n", STR_SUCC);
    } else {
        hdmi_dbg_err("error argvs!\n");
        goto usage_error;
    }

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo avmute argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0\t--avmute off.\n"
                   "           1\t--avmute on.\n");
    hdmi_dbg_print("[example]: echo avmute 1 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_cbar(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_bool enable;
    hdmi_device *hdmi_dev = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        enable = TD_FALSE;
        hal_call_void(hdmi_dev, hal_hdmi_debug, HDMI_DEBUG_CMD_COLOR_BAR, &enable);
        hdmi_dbg_print("\n color_bar is OFF %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        enable = TD_TRUE;
        hal_call_void(hdmi_dev, hal_hdmi_debug, HDMI_DEBUG_CMD_COLOR_BAR, &enable);
        hdmi_dbg_print("\n color_bar is ON %s! \n\n", STR_SUCC);
    } else {
        hdmi_dbg_err("error argvs!\n");
        goto usage_error;
    }

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo cbar argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0\t--colorbar off.\n"
                   "           1\t--colorbar on.\n");
    hdmi_dbg_print("[example]: echo cbar 1 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

#ifdef HDMI_SCDC_SUPPORT
static td_s32 debug_scdc_off(const hdmi_device *hdmi_dev, hdmi_scdc_status *scdc_status)
{
    scdc_status->source_scramble_on = TD_FALSE;
    scdc_status->sink_scramble_on = TD_FALSE;
    scdc_status->tmds_bit_clk_ratio = SCDC_TMDS_BIT_CLK_RATIO_10X;
    hal_call_void(hdmi_dev, hal_hdmi_phy_output_enable_set, TD_FALSE);

    hal_call_void(hdmi_dev, hal_hdmi_scdc_status_set, scdc_status);
    osal_msleep(SCDC_SET_WAIT);
    hal_call_void(hdmi_dev, hal_hdmi_phy_output_enable_set, TD_TRUE);

    (td_void)memset_s(scdc_status, sizeof(hdmi_scdc_status), 0, sizeof(hdmi_scdc_status));
    osal_msleep(SCDC_GET_WAIT);
    hal_call_void(hdmi_dev, hal_hdmi_scdc_status_get, scdc_status);
    hdmi_dbg_print("\n scdc set OFF %s! \n\n",
        (scdc_status->source_scramble_on == TD_FALSE) && (scdc_status->sink_scramble_on == TD_FALSE) &&
        (scdc_status->tmds_bit_clk_ratio == SCDC_TMDS_BIT_CLK_RATIO_10X) ? STR_SUCC : STR_FAIL);

    return TD_SUCCESS;
}

static td_s32 debug_scdc_on(const hdmi_device *hdmi_dev, hdmi_scdc_status *scdc_status)
{
    scdc_status->source_scramble_on = TD_TRUE;
    scdc_status->sink_scramble_on = TD_TRUE;
    scdc_status->tmds_bit_clk_ratio = SCDC_TMDS_BIT_CLK_RATIO_40X;
    hal_call_void(hdmi_dev, hal_hdmi_phy_output_enable_set, TD_FALSE);
    hal_call_void(hdmi_dev, hal_hdmi_scdc_status_set, scdc_status);
    osal_msleep(SCDC_SET_WAIT);
    hal_call_void(hdmi_dev, hal_hdmi_phy_output_enable_set, TD_TRUE);

    (td_void)memset_s(scdc_status, sizeof(hdmi_scdc_status), 0, sizeof(hdmi_scdc_status));

    osal_msleep(SCDC_GET_WAIT);
    hal_call_void(hdmi_dev, hal_hdmi_scdc_status_get, scdc_status);

    hdmi_dbg_print("\n scdc set ON %s! \n\n", (scdc_status->source_scramble_on == TD_TRUE) &&
        (scdc_status->sink_scramble_on == TD_TRUE) &&
        (scdc_status->tmds_bit_clk_ratio == SCDC_TMDS_BIT_CLK_RATIO_40X) ? STR_SUCC : STR_FAIL);

    return TD_SUCCESS;
}

static td_s32 debug_scdc_status_get(const hdmi_device *hdmi_dev, hdmi_scdc_status *scdc_status)
{
    hal_call_void(hdmi_dev, hal_hdmi_scdc_status_get, scdc_status);
    hdmi_dbg_print("scdc get source_scramble_on[%u] \n"
                   "           sink_scramble_on[%u] \n"
                   "         tmds_bit_clk_ratio[%u] \n\n",
                   scdc_status->source_scramble_on,
                   scdc_status->sink_scramble_on,
                   scdc_status->tmds_bit_clk_ratio);

    return TD_SUCCESS;
}

static td_void debug_scdc_print(td_void)
{
    hdmi_dbg_print("[usage  ]: echo scdc argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0\t--scdc off.\n"
                   "           1\t--scdc on.\n"
                   "           2\t--scdc status get.\n");
    hdmi_dbg_print("[example]: echo scdc 1 > /proc/umap/hdmi0 \n");

    return;
}

static td_s32 debug_scdc(const hdmi_debug_cmd_arg *cmd_arg)
{
    hdmi_device     *hdmi_dev    = TD_NULL;
    hdmi_scdc_status scdc_status = {0};
    td_s32           ret;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);

    if (cmd_arg->argc > DEBUG_CMD_ARGC_COUNT_3 || cmd_arg->argv[1] == TD_NULL) {
        goto usage_error;
    }

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        ret = debug_scdc_off(hdmi_dev, &scdc_status);
        debug_failure_check_return(ret);
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        ret = debug_scdc_on(hdmi_dev, &scdc_status);
        debug_failure_check_return(ret);
    } else if (is_str_equal(cmd_arg->argv[1], "2")) {
        ret = debug_scdc_status_get(hdmi_dev, &scdc_status);
        debug_failure_check_return(ret);
    } else {
        goto usage_error;
    }

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_err("error argvs!\n");
    debug_scdc_print();

    return TD_FAILURE;
}
#endif

static td_s32 debug_mute_delay(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_s32 ret;
    td_u32 old_delay;
    hdmi_delay delay = {0};
    td_char *str = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;

    debug_null_check_return(cmd_arg);
    str = cmd_arg->argv[0];
    debug_null_check_return(str);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    ret = memcpy_s(&delay, sizeof(delay), &hdmi_dev->delay, sizeof(hdmi_delay));
    if (ret != EOK) {
        hdmi_dbg_err("memcpy_s err!\n");
        return TD_FAILURE;
    }

    old_delay = delay.mute_delay;
    str = cmd_arg->argv[1];
    debug_null_check_return(str);
    delay.mute_delay = debug_str_to_u32(str, &str, HDMI_DEBUG_BASE_DEC);
    if (delay.mute_delay > DEBUG_MAX_DELAY_MS) {
        hdmi_dbg_err("error delay time!\n");
        goto usage_error;
    }
    ret = memcpy_s(&hdmi_dev->delay, sizeof(hdmi_dev->delay), &delay, sizeof(hdmi_delay));
    if (ret != EOK) {
        hdmi_dbg_err("memcpy_s err!\n");
        return TD_FAILURE;
    }
    hdmi_dbg_print("\n avmute delay set %u -> %u ms %s! \n\n", old_delay, delay.mute_delay,
                   (ret == TD_SUCCESS) ? STR_SUCC : STR_FAIL);

    return ret;

usage_error:
    hdmi_dbg_print("[Usage  ]: echo mutedelay argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: a delay time value in range [0 , %u] ms \n", DEBUG_MAX_DELAY_MS);
    hdmi_dbg_print("[example]: echo mutedelay 100 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_fmt_delay(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_s32 ret;
    td_u32 old_delay;
    hdmi_delay delay = {0};
    td_char *str = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;

    debug_null_check_return(cmd_arg);
    str = cmd_arg->argv[0];
    debug_null_check_return(str);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    ret = memcpy_s(&delay, sizeof(delay), &hdmi_dev->delay, sizeof(hdmi_delay));
    if (ret != EOK) {
        hdmi_dbg_err("memcpy_s err!\n");
        return TD_FAILURE;
    }

    old_delay = delay.fmt_delay;
    str = cmd_arg->argv[1];
    debug_null_check_return(str);
    delay.fmt_delay = debug_str_to_u32(str, &str, HDMI_DEBUG_BASE_DEC);
    if (delay.fmt_delay > DEBUG_MAX_DELAY_MS) {
        hdmi_dbg_err("error delay time!\n");
        goto usage_error;
    }
    ret = memcpy_s(&hdmi_dev->delay, sizeof(hdmi_dev->delay), &delay, sizeof(hdmi_delay));
    if (ret != EOK) {
        hdmi_dbg_err("memcpy_s err!\n");
        return TD_FAILURE;
    }

    hdmi_dbg_print("\n format delay set %u -> %u ms %s! \n\n",
                   old_delay, delay.fmt_delay, (ret == TD_SUCCESS) ? STR_SUCC : STR_FAIL);

    return ret;

usage_error:
    hdmi_dbg_print("[Usage  ]: echo fmtdelay argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: a delay time value in range [0 , %u] ms \n", DEBUG_MAX_DELAY_MS);
    hdmi_dbg_print("[example]: echo fmtdelay 100 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_emi(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_s32 ret;
    hdmi_attr attr = {0};
    hdmi_device *hdmi_dev = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    ret = drv_hdmi_attr_get(hdmi_dev, &attr);
    if (ret != TD_SUCCESS) {
        hdmi_dbg_err("get attr error!\n");
        return TD_FAILURE;
    }

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        drv_hdmi_stop(hdmi_dev);
        hdmi_dev->mode_param.emi_en = TD_FALSE;
        ret = drv_hdmi_attr_set(hdmi_dev, &attr);
        drv_hdmi_start(hdmi_dev);
        hdmi_dbg_print("disable emi.\n");
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        drv_hdmi_stop(hdmi_dev);
        hdmi_dev->mode_param.emi_en = TD_TRUE;
        ret = drv_hdmi_attr_set(hdmi_dev, &attr);
        drv_hdmi_start(hdmi_dev);
        hdmi_dbg_print("enable emi.\n");
    }

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[Usage  ]: echo emi argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0\t--emi off.\n"
                   "           1\t--emi on.\n");
    hdmi_dbg_print("[example]: echo emi 1 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_ddc(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_u32 ddc_freq;
    hdmi_device *hdmi_dev = TD_NULL;
    td_char *str = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    str = cmd_arg->argv[1];

    ddc_freq = debug_str_to_u32(str, &str, HDMI_DEBUG_BASE_DEC);
    if (ddc_freq > 300 || ddc_freq == 0) { /* ddc frequency extent is 1-300 kHz */
        goto usage_error;
    }

    hal_call_void(hdmi_dev, hal_hdmi_debug, HDMI_DEBUG_CMD_DDC_FREQ, &ddc_freq);
    hdmi_dbg_print("\n ddc frequency set %u_k_hz! \n\n", ddc_freq);

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo ddc argv1 > /proc/umap/hdmi0\n");
    hdmi_dbg_print("[argv1  ]: ddc frequency [1,300] k_hz\n");

    return TD_FAILURE;
}

static td_s32 debug_hdmi_mode(const hdmi_debug_cmd_arg *cmd_arg)
{
    hdmi_device *hdmi_dev = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        hal_call_void(hdmi_dev, hal_hdmi_tmds_mode_set, HDMI_TMDS_MODE_DVI);
        hdmi_dbg_print("\n hdmi tmds mode set DVI %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        hal_call_void(hdmi_dev, hal_hdmi_tmds_mode_set, HDMI_TMDS_MODE_HDMI_1_4);
        hdmi_dbg_print("\n hdmi tmds mode set HDMI1.4 %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "2")) {
        hal_call_void(hdmi_dev, hal_hdmi_tmds_mode_set, HDMI_TMDS_MODE_HDMI_2_0);
        hdmi_dbg_print("\n hdmi tmds mode set HDMI2.0 %s! \n\n", STR_SUCC);
    } else {
        hdmi_dbg_err("error argvs!\n");
        goto usage_error;
    }

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo hdmimode argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0\t--DVI.\n"
                   "           1\t--HDMI1.4.\n"
                   "           2\t--HDMI2.0.\n");
    hdmi_dbg_print("[example]: echo hdmimode 1 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_out_color_space(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_s32       ret;
    td_u32       old_mode;
    hdmi_attr    attr = {0};
    hdmi_device *hdmi_dev = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    ret = drv_hdmi_attr_get(hdmi_dev, &attr);
    debug_failure_check_return(ret);
    old_mode = (td_u32)attr.app_attr.out_color_space;

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        attr.app_attr.out_color_space = HDMI_COLORSPACE_RGB;
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        attr.app_attr.out_color_space = HDMI_COLORSPACE_YCBCR422;
    } else if (is_str_equal(cmd_arg->argv[1], "2")) {
        attr.app_attr.out_color_space = HDMI_COLORSPACE_YCBCR444;
    } else if (is_str_equal(cmd_arg->argv[1], "3")) {
        attr.app_attr.out_color_space = HDMI_COLORSPACE_YCBCR420;
    } else {
        hdmi_dbg_err("error argvs!\n");
        goto usage_error;
    }

    drv_hdmi_stop(hdmi_dev);
    ret = drv_hdmi_attr_set(hdmi_dev, &attr);
    drv_hdmi_start(hdmi_dev);
    hdmi_dbg_print("\n output colorspace mode %u->%u %s! \n\n", old_mode,
                   attr.app_attr.out_color_space, (ret == TD_SUCCESS) ? STR_SUCC : STR_FAIL);

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo outclrspace argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0\t--RGB.\n"
                   "           1\t--YUV422.\n"
                   "           2\t--YUV444.\n"
                   "           3\t--YUV420.\n");
    hdmi_dbg_print("[example]: echo outclrspace 1 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_deep_color(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_s32       ret;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_attr    attr = {0};
    td_u32       old_mode;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    ret = drv_hdmi_attr_get(hdmi_dev, &attr);
    debug_failure_check_return(ret);
    old_mode = (td_u32)attr.app_attr.deep_color_mode;

    if (attr.app_attr.out_color_space == HDMI_COLORSPACE_YCBCR422) {
        hdmi_dbg_err("colorspace YUV422 not support deepcolor set! force set\n");
    }

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        attr.app_attr.deep_color_mode = HDMI_DEEP_COLOR_24BIT;
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        attr.app_attr.deep_color_mode = HDMI_DEEP_COLOR_30BIT;
    } else if (is_str_equal(cmd_arg->argv[1], "2")) {
        attr.app_attr.deep_color_mode = HDMI_DEEP_COLOR_36BIT;
    } else {
        hdmi_dbg_err("error argvs!\n");
        goto usage_error;
    }

    drv_hdmi_stop(hdmi_dev);
    ret = drv_hdmi_attr_set(hdmi_dev, &attr);
    drv_hdmi_start(hdmi_dev);
    hdmi_dbg_print("\n deppcolor colorspace mode %u->%u %s! \n\n", old_mode,
                   attr.app_attr.deep_color_mode, (ret == TD_SUCCESS) ? STR_SUCC : STR_FAIL);

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo deepclr argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0\t--08 bit.\n"
                   "           1\t--10 bit.\n"
                   "           2\t--12 bit.\n");
    hdmi_dbg_print("[example]: echo deepclr 1 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_dither(const hdmi_debug_cmd_arg *cmd_arg)
{
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_video_dither dither_mode = HDMI_VIDEO_DITHER_DISALBE;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        dither_mode = HDMI_VIDEO_DITHER_DISALBE;
        hdmi_dbg_print("\n dither is disable %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        dither_mode = HDMI_VIDEO_DITHER_10_8;
        hdmi_dbg_print("\n dither is enable 10_08 %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "2")) {
        dither_mode = HDMI_VIDEO_DITHER_12_8;
        hdmi_dbg_print("\n dither is enable 12_08 %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "3")) {
        dither_mode = HDMI_VIDEO_DITHER_12_10;
        hdmi_dbg_print("\n dither is enable 12_10 %s! \n\n", STR_SUCC);
    } else {
        hdmi_dbg_err("error argvs!\n");
        goto usage_error;
    }

    hal_call_void(hdmi_dev, hal_hdmi_debug, HDMI_DEBUG_CMD_DITHER, &dither_mode);

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo dither argv1 > /proc/umap/hdmi0\n");
    hdmi_dbg_print("[argv1  ]: 0\t--dither disable.\n"
                   "           1\t--10_08.\n"
                   "           2\t--12_08.\n"
                   "           3\t--12_10.\n");
    hdmi_dbg_print("[example]: echo dither 1 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_s32 debug_authenticate(const hdmi_debug_cmd_arg *cmd_arg)
{
    hdmi_device *hdmi_dev = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        hdmi_dev->attr.app_attr.auth_mode = TD_FALSE;
        hdmi_dbg_print("\n aut disable %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        hdmi_dev->attr.app_attr.auth_mode = TD_TRUE;
        hdmi_dbg_print("\n aut enable %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "2")) {
        hdmi_dbg_print("\n aut status %u! \n\n", hdmi_dev->attr.app_attr.auth_mode);
    } else {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo aut argv1 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0\t--aut off.\n"
                   "           1\t--aut on.\n"
                   "           2\t--aut status get.\n");
    hdmi_dbg_print("[example]: echo aut 1 > /proc/umap/hdmi0 \n");

    return TD_FAILURE;
}

static td_void debug_cmd_help(td_void)
{
    hdmi_dbg_err("error args!\n");
    hdmi_dbg_print("[usage  ]: echo cmd argv1 argv2(optional) > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0\t--hdmi stop.\n"
                   "           1\t--hdmi start.\n"
                   "           2\t--black frame enable.\n"
                   "           3\t--black frame disable.\n"
                   "           4\t--PCB length set (must set argv2).\n");
    hdmi_dbg_print("[example]: echo cmd 1 > /proc/umap/hdmi0 \n");
    return;
}

static td_s32 debug_cmd(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_u32 data;
    td_char *str = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_black_frame_info black_info = {0};

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    black_info.in_bit_depth = hdmi_dev->attr.vo_attr.in_bit_depth;
    black_info.in_color_space = hdmi_dev->attr.vo_attr.in_color_space;
    black_info.in_quantization = (black_info.in_color_space == HDMI_COLORSPACE_RGB) ?
        hdmi_dev->attr.vo_attr.rgb_quantization : hdmi_dev->attr.vo_attr.ycc_quantization;
    /* input require argc is 2, argv 1 is none null */
    if ((cmd_arg->argc != DEBUG_CMD_ARGC_NUM) || cmd_arg->argv[1] == TD_NULL) {
        goto usage_error;
    }

    if (is_str_equal(cmd_arg->argv[1], "0")) {
        drv_hdmi_stop(hdmi_dev);
        hdmi_dbg_print("\n HDMI stop %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "1")) {
        drv_hdmi_start(hdmi_dev);
        hdmi_dbg_print("\n HDMI start %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "2")) {
        black_info.black_enable = TD_TRUE;
        hal_call_void(hdmi_dev, hal_hdmi_black_data_set, &black_info);
        hdmi_dbg_print("\n black frame enable %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "3")) {
        black_info.black_enable = TD_FALSE;
        hal_call_void(hdmi_dev, hal_hdmi_black_data_set, &black_info);
        hdmi_dbg_print("\n black frame disable %s! \n\n", STR_SUCC);
    } else if (is_str_equal(cmd_arg->argv[1], "4")) {
        if (cmd_arg->argc < DEBUG_CMD_ARGC_COUNT_2) {
            goto usage_error;
        }

        str = cmd_arg->argv[HDMI_DEBUG_ARG_INDEX_2];
        debug_null_check_return(str);
        data = debug_str_to_u32(str, &str, HDMI_DEBUG_BASE_DEC);
        data = (data > HDMI_TRACE_LEN_1) ? HDMI_TRACE_LEN_1 : data;
        hdmi_dbg_print("\n PCB trace len old(%u)->new(%u)! \n\n", hdmi_dev->mode_param.trace_len, data);
        hdmi_dev->mode_param.trace_len = (hdmi_trace_len)data;
    } else {
        goto usage_error;
    }

    return TD_SUCCESS;

usage_error:
    debug_cmd_help();

    return TD_FAILURE;
}

static td_void compat_help_info(td_void)
{
    hdmi_dbg_print("[Usage  ]: echo chcompat argv1 argv2 > /proc/umap/hdmi0 \n");
    hdmi_dbg_print("[argv1  ]: 0 \t--setmute_send 0~1.\n");
    hdmi_dbg_print("[argv2  ]: 0-1 \t--set enable.\n");
    hdmi_dbg_print("[example]: echo chcompat 1 1 > /proc/umap/hdmi0 \n");
}

static td_void set_compat_debug_info(compatibility_info *compat_info, compat_info_type type, td_u32 value)
{
    switch (type) {
        case COMPAT_AVI_USE_GEN5:
            hdmi_dbg_print("set AVI by gen5 old(%u)->new(%u).\n", compat_info->avi_use_gen5, value);
                compat_info->avi_use_gen5 = (value == 0) ? TD_FALSE : TD_TRUE;
            break;
        case COMPAT_MUTE_SEND:
            compat_info->setmute_send = (value == 0) ? TD_FALSE : TD_TRUE;
            break;
        default:
            break;
    }
    hdmi_dbg_print(g_debug_compat_table[type].success_info);

    return;
}

static td_s32 debug_compat(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_u32 data;
    td_u32 i;
    td_char *str = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;
    compatibility_info *compat_info = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);

    compat_info = compat_info_get(hdmi_dev->hdmi_dev_id);
    debug_null_check_return(compat_info);

    if (cmd_arg->argc < DEBUG_CMD_ARGC_COUNT_2 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }
    str = cmd_arg->argv[HDMI_DEBUG_ARG_INDEX_2];
    debug_null_check_return(str);
    data = debug_str_to_u32(str, &str, HDMI_DEBUG_BASE_DEC);

    for (i = 0; i < hdmi_array_size(g_debug_compat_table); i++) {
        if (is_str_equal(cmd_arg->argv[1], g_debug_compat_table[i].num)) {
            if ((data > g_debug_compat_table[i].max) || (data < g_debug_compat_table[i].min)) {
                hdmi_dbg_print(g_debug_compat_table[i].error_info);
                return TD_FAILURE;
            }
            set_compat_debug_info(compat_info, i, data);
            break;
        }
    }
    if (i == hdmi_array_size(g_debug_compat_table)) {
        hdmi_dbg_err("error argvs!\n");
        goto usage_error;
    }

    return TD_SUCCESS;

usage_error:
    compat_help_info();

    return TD_FAILURE;
}

static td_s32 debug_thread_cycle_time(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_u32 thread_time;
    hdmi_device *hdmi_dev = TD_NULL;
    td_char *str = TD_NULL;

    debug_null_check_return(cmd_arg);
    hdmi_dev = get_hdmi_device(cmd_arg->hdmi_id);
    debug_null_check_return(hdmi_dev);
    debug_null_check_return(hdmi_dev->hal);

    if (cmd_arg->argc != DEBUG_CMD_ARGC_COUNT_1 || cmd_arg->argv[1] == TD_NULL) {
        hdmi_dbg_err("error argcs!\n");
        goto usage_error;
    }

    str = cmd_arg->argv[1];

    thread_time = debug_str_to_u32(str, &str, HDMI_DEBUG_BASE_DEC);
    if (thread_time < 10 || thread_time > DEBUG_MAX_THREAD_TIME) { /* The evnet thread time limit is [10, 100]. */
        goto usage_error;
    }

    hdmi_dev->debug.intf_status.event_thread_cycle_time = thread_time;
    hdmi_dbg_print("\n thread cycle time set %d success! \n", thread_time);

    return TD_SUCCESS;

usage_error:
    hdmi_dbg_print("[usage  ]: echo time argv1 > /proc/umap/hdmi0\n");
    hdmi_dbg_print("[argv1  ]: thread time [10,100] ms\n");

    return TD_FAILURE;
}

static const hdmi_debug_cmd_info g_cmd_table[] = {
    /* name     short name    func                       help info */
    { "help",         "h", debug_print_help,      "print these help info." },
    { "thread",       "t", debug_thread,          "thread status set on/off." },
    { "oe",           "o", debug_oe,              "phyoutput enable(oe) set on/off." },
    { "event",        "e", debug_event,           "hardware event [16,25] simulate." },
    { "edid",        "ed", debug_edid,            "edid raw data load." },
    { "avmute",       "a", debug_avmute,          "audio & video mute set on/off." },
    { "cbar",         "c", debug_cbar,            "colorbar set on/off." },
#ifdef HDMI_SCDC_SUPPORT
    { "scdc",        "sc", debug_scdc,            "scdc set on/off." },
#endif
    { "mutedelay",   "md", debug_mute_delay,      "avmute delay time(ms) [0,100000] set." },
    { "fmtdelay",    "fd", debug_fmt_delay,       "format setting delay time(ms) [0,100000] set." },
    { "emi",         "em", debug_emi,             "emi set on/off." },
    { "ddc",         "dd", debug_ddc,             "ddc frequency set in range [0,11]." },
    { "hdmimode",     "m", debug_hdmi_mode,       "hdmi tmds mode set dvi/1.4/2.0." },
    { "outclrspace", "oc", debug_out_color_space, "output colorspace mode set rgb/yuv422/yuv444/yuv420." },
    { "deepclr",      "d", debug_deep_color,      "output color depth  set 08/10/12bit." },
    { "dither",      "di", debug_dither,          "dither set on/off." },
    { "aut",         "au", debug_authenticate,    "authentication mode set on/off." },
    { "cmd",        "cmd", debug_cmd,             "cmd set ." },
    { "compat",     "cmp", debug_compat,          "compat test." },
    { "thread_time", "time", debug_thread_cycle_time, "set the time of the evnet thread [10,100]."},
};

static td_s32 debug_print_help(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_u32 i;

    ot_unused(cmd_arg);
    for (i = 0; i < hdmi_array_size(g_cmd_table); i++) {
        if (g_cmd_table[i].comment_help && g_cmd_table[i].name) {
            hdmi_dbg_print("    %02u. %-20s", i, g_cmd_table[i].name);
            hdmi_dbg_print("--[%-2s]%s\n", g_cmd_table[i].short_name, g_cmd_table[i].comment_help);
        } else {
            hdmi_dbg_err("g_cmd_table[%u].comment_help or name is null!\n", i);
        }
    }

    hdmi_dbg_print("\n");
    hdmi_dbg_print("    you can use 'echo cmd argv... > /proc/umap/hdmi0' execute a debug command.\n"
                   "    you can use 'echo cmd > /proc/umap/hdmi0' for more details about each command.\n"
                   "    here, 'cmd' refers to NO./complete-name/short-name above.\n");

    return TD_SUCCESS;
}

static td_s32 debug_cmd_handle(const hdmi_debug_cmd_arg *cmd_arg)
{
    td_u32 i = 0;
    td_u32 cmd_num;
    td_char *str = TD_NULL;

    str = cmd_arg->argv[0];
    if (str == TD_NULL) {
        goto error_help;
    }

    if (cmd_arg->argc > 0) {
        cmd_num = debug_str_to_u32(str, &str, HDMI_DEBUG_BASE_DEC);
        for (i = 0; i < hdmi_array_size(g_cmd_table); i++) {
            if (g_cmd_table[i].name == TD_NULL) {
                hdmi_dbg_err("g_cmd_table[%u].name null\n", i);
                goto error_help;
            }
            if (((cmd_num != i) || (*str != '\0')) &&
                (is_str_equal(g_cmd_table[i].name, cmd_arg->argv[0]) == TD_FALSE) &&
                (is_str_equal(g_cmd_table[i].short_name, cmd_arg->argv[0]) == TD_FALSE)) {
                continue;
            }
            if (g_cmd_table[i].fn_cmd_func != TD_NULL) {
                g_cmd_table[i].fn_cmd_func(cmd_arg);
            } else {
                hdmi_dbg_err("g_cmd_table[%u].fn_cmd_func null\n", i);
                goto error_help;
            }
            break;
        }
    }

    if (i == hdmi_array_size(g_cmd_table)) {
        goto error_help;
    }

    return TD_SUCCESS;

error_help:

    return debug_print_help(cmd_arg);
}

static td_s32 debug_cmd_line_parse(td_char *cmd_str, td_u32 len, hdmi_debug_cmd_arg *cmd_arg)
{
    td_u32 i;
    td_u32 tmp_argc = 0;
    td_char *tmp_str = NULL;

    if (len == 0 || len >= DEBUG_MAX_CMD_CHAR_NUM) {
        hdmi_dbg_err("invalid param!\n");
        return TD_FAILURE;
    }

    tmp_str = cmd_str;
    cmd_arg->argc = 0;

    for (i = 0; i < DEBUG_MAX_ARGV_NUM; i++) {
        cmd_arg->argv[i] = TD_NULL;
    }

    if ((*tmp_str != DEBUG_CHAR_SPACE) && (*tmp_str != DEBUG_CHAR_TAB)) {
        cmd_arg->argv[tmp_argc] = tmp_str;
        tmp_argc++;
        tmp_str++;
    }

    for (i = 0; i < len; tmp_str++, i++) {
        if ((*tmp_str == DEBUG_CHAR_SPACE) || (*tmp_str == DEBUG_CHAR_TAB)) {
            *tmp_str = '\0';
            if ((*(tmp_str + 1) == DEBUG_CHAR_SPACE) || (*(tmp_str + 1) == DEBUG_CHAR_TAB)) {
                continue;
            }

            if (tmp_argc == 3) { /* 3, is tmp argument cont */
                cmd_arg->remain_len = len - i;
            } else if (tmp_argc >= DEBUG_MAX_ARGV_NUM) {
                hdmi_dbg_print(" too much argcs.max argc = %u\n", DEBUG_MAX_ARGV_NUM);
                tmp_argc = DEBUG_MAX_ARGV_NUM;
                break;
            }
            cmd_arg->argv[tmp_argc] = tmp_str + 1;
            tmp_argc++;
        }
    }

    cmd_arg->argc = tmp_argc;

    return TD_SUCCESS;
}

td_s32 drv_hdmi_debug_edid_raw_read(hdmi_edid_updata_mode mode, td_u8 *data, td_u32 len)
{
    errno_t ret;
    hdmi_debug_edid *dbg_edid = &g_debug_edid;

    if ((mode != HDMI_EDID_UPDATE_DEBUG) || (dbg_edid->data_valid == TD_FALSE) ||
        (data == TD_NULL) || (len > HDMI_EDID_TOTAL_SIZE)) {
        hdmi_err("debug edid read fail,data_valid=%u !\n", dbg_edid->data_valid);
        return TD_FAILURE;
    }
    ret = memcpy_s(data, len, dbg_edid->data, dbg_edid->len);
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    return dbg_edid->len;
}

td_s32 drv_hdmi_debug_source_write(hdmi_proc_entry *file, const char *buf, td_u32 count, td_s64 *data)
{
    td_s32 ret;
    td_char *tmp_buf = TD_NULL;
    hdmi_debug_cmd_arg cmd_arg = {0};
    hdmi_device *hdmi_dev = TD_NULL;

    debug_null_check_return(buf);
    debug_null_check_return(data);
    debug_null_check_return(file);

    hdmi_dev = (hdmi_device *)file->private;
    debug_null_check_return(hdmi_dev);
    cmd_arg.hdmi_id = hdmi_dev->hdmi_dev_id;

    tmp_buf = (td_char *)osal_vmalloc(DEBUG_MAX_CMD_CHAR_NUM);
    if (tmp_buf == TD_NULL) {
        hdmi_dbg_err("vmalloc fail!\n");
        return EFAULT;
    }
    if (memset_s(tmp_buf, DEBUG_MAX_CMD_CHAR_NUM, '\0', DEBUG_MAX_CMD_CHAR_NUM) != EOK) {
        hdmi_err("memset_s error\n");
        goto error;
    }

    if (count >= DEBUG_MAX_CMD_CHAR_NUM || count == 0) {
        hdmi_dbg_err("\n echo chars err. count=%lu, max cnt=%u\n", count, DEBUG_MAX_CMD_CHAR_NUM);
        goto error;
    }
    if (osal_copy_from_user(tmp_buf, buf, count) != TD_SUCCESS) {
        hdmi_dbg_err("\n copy from user failed. \n");
        goto error;
    }
    tmp_buf[count - 1] = '\0';
    if (debug_cmd_line_parse(tmp_buf, count - 1, &cmd_arg) != TD_SUCCESS) {
        goto error;
    }

    hdmi_mutex_lock(hdmi_dev->mutex_proc);
    if (hdmi_dev->user_cnt == 0 && hdmi_dev->kernel_cnt == 0) {
        hdmi_mutex_unlock(hdmi_dev->mutex_proc);
        hdmi_dbg_err("\n HDMI not open! \n");
        goto error;
    }
    ret = debug_cmd_handle(&cmd_arg);
    hdmi_mutex_unlock(hdmi_dev->mutex_proc);

    if (ret != TD_SUCCESS) {
        goto error;
    }

    osal_vfree(tmp_buf);
    return count;

error:
    osal_vfree(tmp_buf);

    return -EFAULT;
}

