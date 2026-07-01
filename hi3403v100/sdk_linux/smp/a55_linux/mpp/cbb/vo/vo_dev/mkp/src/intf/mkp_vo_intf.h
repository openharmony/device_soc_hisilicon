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
#ifndef MKP_VO_INTF_H
#define MKP_VO_INTF_H

#include "ot_common_vo.h"
#include "mkp_vo.h"
#include "mkp_vo_dev.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")
#ifdef CONFIG_OT_VO_VGA
td_void vo_init_vga_param(ot_vo_vga_param *vga_param);
#endif
#ifdef CONFIG_OT_HDMI_SUPPORT
td_void vo_init_hdmi_param(ot_vo_hdmi_param *hdmi_param);
#endif
#ifdef CONFIG_OT_VO_RGB
td_void vo_init_rgb_param(ot_vo_rgb_param *rgb_param);
#endif
#ifdef CONFIG_OT_VO_BT1120
td_void vo_init_bt_param(ot_vo_bt_param *bt_param);
#endif
#ifdef CONFIG_OT_VO_MIPI
td_void vo_init_mipi_param(ot_vo_mipi_param *mipi_param);
#endif
td_void vo_init_dev_intf_param(vo_dev_info *dev_ctx);
td_void vo_set_dev_intf_param(ot_vo_dev dev);
#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")
#ifdef CONFIG_OT_VO_VGA
td_s32 vo_ioctl_vga_param(vo_ioctl_vga_param_param *ioctl_vga_param, vo_ioctl_type ioctl_type);
#endif

#ifdef CONFIG_OT_VO_HDMI
td_s32 vo_ioctl_hdmi_param(vo_ioctl_hdmi_param_param *ioctl_hdmi_param, vo_ioctl_type ioctl_type);
#endif

#ifdef CONFIG_OT_VO_HDMI1
td_s32 vo_ioctl_hdmi1_param(vo_ioctl_hdmi_param_param *ioctl_hdmi_param, vo_ioctl_type ioctl_type);
#endif

#ifdef CONFIG_OT_VO_RGB
td_s32 vo_ioctl_rgb_param(vo_ioctl_rgb_param_param *ioctl_rgb_param, vo_ioctl_type ioctl_type);
#endif

#ifdef CONFIG_OT_VO_BT1120
td_s32 vo_ioctl_bt_param(vo_ioctl_bt_param_param *ioctl_bt_param, vo_ioctl_type ioctl_type);
#endif

#ifdef CONFIG_OT_VO_MIPI
td_s32 vo_ioctl_mipi_param(vo_ioctl_mipi_param_param *ioctl_mipi_param, vo_ioctl_type ioctl_type);
#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef MKP_VO_INTF_H */
