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
#ifndef HDMI_OSAL_H
#define HDMI_OSAL_H

#include "ot_type.h"

typedef enum {
    OSAL_RDONLY,
    OSAL_WRONLY,
    OSAL_RDWR,
    OSAL_BUTT
} osal_file_flag;

#define is_str_equal(p1, p2) ((((p1) != TD_NULL) && ((p2) != TD_NULL) && (osal_strlen(p1) == osal_strlen(p2)) && \
    (osal_strncmp(p1, p2, osal_strlen(p1)) == 0)))

td_s32 hdmi_osal_file_write(struct file *file, const td_char *buf, td_u32 len);

td_u32 hdmi_osal_get_time_in_ms(td_void);

td_u64 hdmi_osal_get_time_in_us(td_void);

#endif /* HDMI_OSAL_H */

