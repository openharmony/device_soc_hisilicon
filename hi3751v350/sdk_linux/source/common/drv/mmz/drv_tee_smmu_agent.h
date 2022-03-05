/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _DRV_TEE_SMMU_AGENT_H
#define _DRV_TEE_SMMU_AGENT_H

#ifdef CONFIG_TZDRIVER

int tee_mem_agent_register(void);

int tee_mem_agent_unregister(void);

#else
static inline int tee_mem_agent_register(void)
{
    return 0;
}

static inline int tee_mem_agent_unregister(void)
{
    return 0;
}

#endif

#endif
