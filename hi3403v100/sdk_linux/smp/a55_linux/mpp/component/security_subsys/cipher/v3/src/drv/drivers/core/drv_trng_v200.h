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

#ifndef DRV_TRNG_V200_H
#define DRV_TRNG_V200_H

#include "drv_osal_lib.h"

/*************************** Internal Structure Definition ****************************/
/** \addtogroup      trng drivers */
/** @{ */ /** <!-- [trng] */

/* ! \Define the offset of reg */
#define OT_SEC_COM_TRNG_CTRL       0x00
#define OT_SEC_COM_TRNG_FIFO_DATA  0x04
#define OT_SEC_COM_TRNG_DATA_ST    0x08
#define OT_SEC_COM_TRNG_POWER_EN   0xC4

/* Define the union ot_sec_com_trng_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 osc_sel : 2;             /* [1..0] */
        td_u32 cleardata : 1;           /* [2] */
        td_u32 drbg_enable : 1;         /* [3] */
        td_u32 pre_process_enable : 1;  /* [4] */
        td_u32 drop_enable : 1;         /* [5] */
        td_u32 mix_enable : 1;          /* [6] */
        td_u32 post_process_enable : 1; /* [7] */
        td_u32 post_process_depth : 8;  /* [15..8] */
        td_u32 reserved0 : 1;           /* [16] */
        td_u32 trng_sel : 2;            /* [18..17] */
        td_u32 pos_self_test_en : 1;    /* [19] */
        td_u32 pre_self_test_en : 1;    /* [20] */
        td_u32 reserved1 : 11;          /* [31..21] */
    } bits;

    /* Define an unsigned member */
    td_u32 u32;
} ot_sec_com_trng_ctrl;

/* Define the union ot_sec_com_trng_data_st */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 low_osc_st0 : 1;         /* [0] */
        td_u32 low_osc_st1 : 1;         /* [1] */
        td_u32 low_ro_st0 : 1;          /* [2] */
        td_u32 low_ro_st1 : 1;          /* [3] */
        td_u32 otp_trng_sel : 1;        /* [4] */
        td_u32 reserved0 : 3;           /* [7..5] */
        td_u32 trng_fifo_data_cnt : 8;  /* [15..8] */
        td_u32 sic_trng_alarm : 6;      /* [21..16] */
        td_u32 sic_trng_bist_alarm : 1; /* [22] */
        td_u32 reserved1 : 9;           /* [31..23] */
    } bits;

    /* Define an unsigned member */
    td_u32 u32;
} ot_sec_com_trng_data_st;

/* Define the union ot_sec_com_trng_power_st */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 trng_power_en : 4; /* [3..0] */
        td_u32 reserved1 : 28;    /* [31..4] */
    } bits;

    /* Define an unsigned member */
    td_u32 u32;
} ot_sec_com_trng_power_st;

/** @} */ /** <!-- ==== Structure Definition end ==== */

#endif /* DRV_TRNG_V200_H */
