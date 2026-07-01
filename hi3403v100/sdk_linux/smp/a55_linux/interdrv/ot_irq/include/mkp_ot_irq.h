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

#ifndef __MKP_OT_IRQ_H__
#define __MKP_OT_IRQ_H__

#include "ot_type.h"
#include "ot_osal.h"
#include "ot_comm_irq.h"
#include "ot_common.h"
#include "ot_errno.h"
#include "osal_ioctl.h"

/* define device name */
#define OT_IRQ_DEVICE_NAME "ot_irq"

#define IOC_TYPE_IRQ 'Z'
#define MAX_IRQ_NAME_LEN 50

typedef struct {
    unsigned int irq_num;
    td_int_reg_info reg_info;
} ot_irq_set_irq_reg_info;

typedef enum {
    IRQ_TRIG_ONECE = 0,
    IRQ_TRIG_AS_CNT = 1
} irq_trig_mode;

typedef enum {
    IRQ_WAIT_TIMEOUT = 0,
    IRQ_WAIT_FOREVER = 1
} irq_wait_mode;

typedef struct {
    char          irq_name[MAX_IRQ_NAME_LEN];
    unsigned int  irq_num;
    ot_irq_arg    *dev;
    irq_trig_mode irq_mod; // 0,trigger once,1,trigger as count
    irq_wait_mode wait_mode; // 0,wait timeout,1,wait forever
    unsigned int  enable_flag; // 0 disable,1,enable
} ot_irq_irq_attr;

#define OT_ERR_IRQ_UNEXIST OT_DEFINE_ERR(OT_ID_IRQ, OT_DBG_ERR, OT_ERR_UNEXIST)

#define IRQ_IOC_REQUEST_FREE_IRQ         0x01
#define IRQ_IOC_WAIT_IRQ                 0x02
#define IRQ_IOC_GET_IRQ_NUM              0x03
#define IRQ_IOC_VENC_CACHE_FALG          0x04
#define IRQ_IOC_SET_IRQ_REG              0x05

#define IRQ_REQUEST_OR_FREE_IRQ_CTRL    OSAL_IOW(IOC_TYPE_IRQ, IRQ_IOC_REQUEST_FREE_IRQ, ot_irq_irq_attr)
#define IRQ_WAIT_IRQ_CTRL               OSAL_IOWR(IOC_TYPE_IRQ, IRQ_IOC_WAIT_IRQ, ot_irq_irq_attr)
#define IRQ_IOC_GET_IRQ_NUM_CTRL        OSAL_IOW(IOC_TYPE_IRQ, IRQ_IOC_GET_IRQ_NUM, ot_irq_irq_attr)
#define IRQ_IOC_SET_IRQ_REG_CTRL        OSAL_IOW(IOC_TYPE_IRQ, IRQ_IOC_SET_IRQ_REG, ot_irq_set_irq_reg_info)


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __MKP_OT_IRQ_H__ */
