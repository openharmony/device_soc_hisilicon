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

#ifndef __OT_IRQ_H__
#define __OT_IRQ_H__

#include <linux/types.h>
#include "ot_type.h"
#include "ot_osal.h"
#include "ot_comm_irq.h"
#include "mkp_ot_irq.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int ot_irq_init(struct platform_device *pdev);
void ot_irq_exit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __OT_IRQ_H__ */
