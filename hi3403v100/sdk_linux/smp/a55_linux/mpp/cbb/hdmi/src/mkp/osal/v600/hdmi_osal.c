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
#include "hdmi_product_define.h" /* because of refined warning, hdmi_product_define.h go first. */
#include "hdmi_osal.h"
#include "ot_osal.h"

#define HDMI_MILLION 1000000

td_s32 hdmi_osal_file_write(struct file *file, const td_char *buf, td_u32 len)
{
    td_s32 write_len;

    if (buf == TD_NULL) {
        return TD_SUCCESS;
    }
    write_len = osal_klib_fwrite(buf, len, file);

    return write_len;
}

td_u32 hdmi_osal_get_time_in_ms(td_void)
{
    td_u64 sys_time;
    sys_time = osal_div_u64(osal_sched_clock(), HDMI_MILLION);
    return (td_u32)sys_time;
}

td_u64 hdmi_osal_get_time_in_us(td_void)
{
    td_u64 curr_us;
    osal_timeval_t current_time = {0};

    osal_gettimeofday(&current_time);
    curr_us = ((td_u64)current_time.tv_sec * HDMI_MILLION + current_time.tv_usec);

    return curr_us;
}

