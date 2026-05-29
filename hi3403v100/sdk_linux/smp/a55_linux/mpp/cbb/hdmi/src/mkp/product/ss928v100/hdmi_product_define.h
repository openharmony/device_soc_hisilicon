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

#ifndef HDMI_PRODUCT_DEFINE_H
#define HDMI_PRODUCT_DEFINE_H

#include <linux/fs.h>
#include "dev_ext.h"
#include "mod_ext.h"
#include "ot_osal.h"
#include "ot_type.h"
#include "drv_hdmi_common.h"
#include "hdmi_reg_crg.h"

/*
 * ------------- reg base addr --------
 *            ctrl    |     dphy      |
 * hdmi0 : 0x17B40000 |  0x17BC0000   |
 * -------------------|----------------
 */
#define CRG_BASE_ADDR              0x11010000
#define HDMI_CRG_OFFSET            0x7F40
#define HDMI_CRG_ADDR             ((CRG_BASE_ADDR) + (HDMI_CRG_OFFSET))
/* pin mux */
#define HDMI_ADDR_BASE_IO_CFG      0x102F0000
#define HDMI_ADDR_IO_CFG_HOTPLUG   (HDMI_ADDR_BASE_IO_CFG + 0xE4)
#define HDMI_ADDR_IO_CFG_SDA       (HDMI_ADDR_BASE_IO_CFG + 0xE8)
#define HDMI_ADDR_IO_CFG_SCL       (HDMI_ADDR_BASE_IO_CFG + 0xEC)
/* color bar */
#define HDMI_COLOR_BAR_MASK        0x60000000
#define HDMI_COLOR_BAR_UPDATE_MASK 0x00000001
#define HDMI_COLOR_BAR_BASE        0x17A0D000
/* sub-module offset */
#define HDMI_TX_BASE_ADDR_CTRL     0x0000
#define HDMI_TX_BASE_ADDR_VIDEO    0x0800
#define HDMI_TX_BASE_ADDR_AUDIO    0x1000
#define HDMI_TX_BASE_ADDR_HDMITX   0x1800
#define HDMI_TX_BASE_ADDR_AON      0x4000
/* other macro */
#define HDMI_FILE_MODE             0777
#ifdef OT_ADVCA_FUNCTION_RELEASE
#define CONFIG_HDMI_PROC_DISABLE
#define CONFIG_HDMI_DEBUG_DISABLE
#endif
#define hdmi_mutex_unlock(a) osal_up((osal_semaphore_t *)&(a))
#define VERSION_STRING       ("[HDMI] Version: [" OT_MPP_VERSION "], Build Time["__DATE__", "__TIME__"]")
#define hdmi_get_current_id()  (get_current()->tgid)

#define hdmi_err_trace(fmt, ...)          \
        OT_ERR_TRACE(OT_ID_HDMI, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define hdmi_warn_trace(fmt, ...)         \
        OT_WARN_TRACE(OT_ID_HDMI, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define hdmi_info_trace(fmt, ...)         \
        OT_INFO_TRACE(OT_ID_HDMI, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define hdmi_fatal_trace(fmt, ...)         \
        OT_EMERG_TRACE(OT_ID_HDMI, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define hdmi_mutex_lock(a)                                    \
    do {                                                      \
        td_s32 retsult = osal_down((osal_semaphore_t *)&(a)); \
        if (retsult != 0) {                                   \
            hdmi_err("osal_down failed! ret=%d\n", retsult);  \
        }                                                     \
    } while (0)

#ifdef HDMI_LOG_SUPPORT
#ifndef OT_ADVCA_FUNCTION_RELEASE
#define hdmi_printk(fmt, args...) osal_printk(fmt, ##args)
#else
#define hdmi_printk(fmt, args...)
#endif
#else
#define hdmi_printk(fmt, args...)
#endif

#ifdef CONFIG_HDMI_DEBUG_DISABLE
#define edid_info(fmt...)
#define edid_warn(fmt...)
#define edid_err(fmt...)
#define edid_faital(fmt...)
#define hdmi_info(fmt...)
#define hdmi_warn(fmt...)
#define hdmi_err(fmt...)
#define hdmi_fatal(fmt...)
#elif defined(HDMI_LOG_SUPPORT)
#define edid_info(fmt...)
#define edid_warn(fmt...)
#define edid_err(fmt...)
#define edid_faital(fmt...)
#define hdmi_info(fmt...)   hdmi_info_trace(fmt)
#define hdmi_warn(fmt...)   hdmi_warn_trace(fmt)
#define hdmi_err(fmt...)    hdmi_err_trace(fmt)
#define hdmi_fatal(fmt...)  hdmi_fatal_trace(fmt)
#else
#define edid_info(fmt...)   hdmi_info_trace(fmt)
#define edid_warn(fmt...)   hdmi_warn_trace(fmt)
#define edid_err(fmt...)    hdmi_err_trace(fmt)
#define edid_faital(fmt...) hdmi_fatal_trace(fmt)
#define hdmi_fatal(fmt...)  hdmi_fatal_trace(fmt)
#define hdmi_err(fmt...)    hdmi_err_trace(fmt)
#define hdmi_warn(fmt...)   hdmi_warn_trace(fmt)
#define hdmi_info(fmt...)   hdmi_info_trace(fmt)
#endif

typedef osal_proc_entry_t hdmi_proc_entry;
typedef struct {
    td_void *private;
    td_s32 (*read)(hdmi_proc_entry *file);
    td_s32 (*write)(hdmi_proc_entry *file, const char *buf, td_u32 count, td_s64 *data);
} hdmi_proc_item;

typedef struct {
    td_u32 ssc_bypass_div;
    td_u32 tmds_clk_div;
} hdmi_crg_cfg;

td_void hdmi_tx_reg_write(td_u32 *reg_addr, td_u32 value);

td_u32 hdmi_tx_reg_read(const td_u32 *reg_addr);

td_void hdmi_reg_write_u32(td_u32 reg_addr, td_u32 value);

td_u32 hdmi_reg_read_u32(td_u32 reg_addr);

td_void drv_hdmi_prod_io_cfg_set(td_void);

td_void drv_hdmi_prod_crg_all_reset_set(td_bool enable);

td_void drv_hdmi_prod_crg_gate_set(td_bool enable);

td_void drv_hdmi_prod_crg_phy_reset_set(td_bool enable);

td_void drv_hdmi_prod_crg_phy_reset_get(td_bool *enable);

td_void drv_hdmi_prod_crg_init(td_void);

td_void drv_hdmi_proc_crg_deinit(td_void);

td_void drv_hdmi_hardware_reset(td_u32 id);

td_void drv_hdmi_low_power_set(td_bool enable);

#endif  /* HDMI_PRODUCT_DEFINE_H */

