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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_CBB_BASED_EXT_INC_VALG_EXT_H_BODY_PART03_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_CBB_BASED_EXT_INC_VALG_EXT_H_BODY_PART03_H_

    /* 判断输入参数(长度)是否正确。
     * 要求:
     * 1, 若写头回绕，更新后的写头不超过读尾;
     * 2, 若写头没有回绕，则更新后的写头不允许超过buffer底部。
     * 3, 若写头触底，则回绕到buffer顶部。
     */
    wh = wr_head + wr_len;
    if (wr_head < rd_tail) {
        if (wh > rd_tail) {
            return TD_FAILURE;
        }
    } else if (wh > cb->buf_len) {
        /* update_wr_head操作，不允许写头回绕 */
        return TD_FAILURE;
    } else if (wh == cb->buf_len) {
        wh = 0;
    }

    (*(cb->write_head)) = wh;
    return TD_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_CBB_BASED_EXT_INC_VALG_EXT_H_BODY_PART03_H_ */
