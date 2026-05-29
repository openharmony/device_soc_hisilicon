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

#include "cryp_trng.h"
#include "drv_trng.h"
#include "drv_osal_lib.h"

/********************** Internal Structure Definition ************************/
/** \addtogroup      trng */
/** @{ */ /** <!-- [trng] */

/* the max continuous bits of randnum is allowed */
#define CONTINUOUS_BITS_ALLOWD 0x08

/* times try to read rang */
#define RANG_READ_TRY_TIME 0x40

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Code *****************************/
/** \addtogroup      trng drivers */
/** @{ */ /** <!-- [trng] */

#ifdef CHIP_TRNG_SUPPORT

td_s32 cryp_trng_init(void)
{
    td_s32 ret;

    func_enter();

    ret = drv_trng_init();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_trng_init, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 cryp_trng_deinit(void)
{
    td_s32 ret;

    func_enter();

    ret = drv_trng_deinit();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_trng_deinit, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 cryp_trng_get_random(td_u32 *randnum, td_u32 timeout)
{
    td_u32 i;
    td_s32 ret;

    func_enter();

    for (i = 0; i < RANG_READ_TRY_TIME; i++) {
        ret = drv_trng_randnum(randnum, timeout);
        if (ret != TD_SUCCESS) {
            return ret;
        }

        /* continuous 32 bits0 or bit1 is prohibited */
        if ((*randnum != 0xffffffff) && (*randnum != 0x00000000)) {
            break;
        }
    }

    if (i >= RANG_READ_TRY_TIME) {
        ot_cipher_error("error, trng randnum check failed\n");
        return OT_ERR_CIPHER_NO_AVAILABLE_RNG;
    }

    func_exit();
    return TD_SUCCESS;
}

#else

td_s32 cryp_trng_init(void)
{
    func_enter();

    func_exit();
    return TD_SUCCESS;
}

td_s32 cryp_trng_deinit(void)
{
    func_enter();

    func_exit();
    return TD_SUCCESS;
}

#include "drv_osal_tee.h"
#include <linux/random.h>

td_s32 cryp_trng_get_random(td_u32 *randnum, td_u32 timeout)
{
    func_enter();

#if defined(OT_TEE_SUPPORT) && !defined(TEE_CIPHER_TA_NONSUPPORT)
    /*
     * read trng from tee, require tee support cipher ta
     * used by ree with non-secure CPU
     * weakness vs get_random_bytes:
     * reduce the performance
     * unsupported multithread
     */
    if (drv_osal_tee_command(TEEC_CMD_GET_TRNG, randnum, WORD_WIDTH) != TD_SUCCESS) {
        print_func_errno(drv_osal_tee_command, ret);
        drv_osal_tee_close_session();
        return OT_ERR_CIPHER_NO_AVAILABLE_RNG;
    }
#else
    /* used by loader with non-secure CPU */
    get_random_bytes((td_u8 *)randnum, WORD_WIDTH);
#endif

    func_exit();
    return TD_SUCCESS;
}

#endif

/** @} */ /** <!-- ==== API Code end ==== */
