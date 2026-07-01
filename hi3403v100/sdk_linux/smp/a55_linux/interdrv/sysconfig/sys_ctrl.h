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

#ifndef SYS_CTRL_H
#define SYS_CTRL_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    VI_ONLINE_VIDEO_NORM_VPSS_ONLINE = 0, /* OT_VI_VIDEO_MODE_NORM OT_VI_ONLINE_VPSS_ONLINE */
    VI_ONLINE_VIDEO_NORM_VPSS_OFFLINE = 1, /* OT_VI_VIDEO_MODE_NORM OT_VI_ONLINE_VPSS_OFFLINE */
    VI_ONLINE_VIDEO_ADVANCED_VPSS_OFFLINE = 2, /* OT_VI_VIDEO_MODE_ADVANCED OT_VI_ONLINE_VPSS_OFFLINE */
    VI_OFFLINE_VIDEO_NORM_VPSS_ONLINE = 3, /* OT_VI_VIDEO_MODE_NORM OT_VI_OFFLINE_VPSS_ONLINE */
    VI_OFFLINE_VIDEO_ADVANCED_VPSS_OFFLINE = 4, /* OT_VI_VIDEO_MODE_ADVANCED OT_VI_OFFLINE_VPSS_OFFLINE */

    VI_ONLINE_VIDEO_ADVANCED_VPSS_ONLINE = 5, /* OT_VI_VIDEO_MODE_ADVANCED OT_VI_ONLINE_VPSS_ONLINE */
    VI_OFFLINE_VIDEO_NORM_VPSS_OFFLINE = 6, /* OT_VI_VIDEO_MODE_NORM OT_VI_OFFLINE_VPSS_OFFLINE */
    VI_OFFLINE_VIDEO_ADVANCED_VPSS_ONLINE = 7, /* OT_VI_VIDEO_MODE_ADVANCED OT_VI_OFFLINE_VPSS_ONLINE */

    VI_VPSS_ONLINE_BUTT
} vi_vpss_online_mode;

void sys_ctl(int online_flag);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SYS_CTRL_H */
