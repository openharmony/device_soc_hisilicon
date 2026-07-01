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

#ifndef CRYP_TRNG_H
#define CRYP_TRNG_H

#include "drv_osal_lib.h"
#include "drv_trng.h"

/******************************* API Declaration *****************************/
/** \addtogroup      trng */
/** @{ */ /** <!--[trng] */

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Code *****************************/
/** \addtogroup      trng drivers */
/** @{ */ /** <!-- [trng] */

td_s32 cryp_trng_init(void);
td_s32 cryp_trng_deinit(void);

/**
\brief get rand number.
\param[out]  randnum rand number.
\param[in]   timeout time out.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 cryp_trng_get_random(td_u32 *randnum, td_u32 timeout);

/**
\brief get rand bytes.
\param[out]  randnum rand number.
\param[in]   size size of rand number.
\param[in]   timeout time out.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 cryp_trng_get_random_bytes(td_u8 *randbyte, td_u32 size, td_u32 timeout);

/** @} */ /** <!-- ==== API declaration end ==== */

#endif /* CRYP_TRNG_H */
