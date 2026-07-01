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

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <linux/device.h>
#include "osal_mmz.h"

#define NAME_LEN_MAX 64

struct mmz_allocator {
    int (*init)(char *args);
    ot_mmb_t *(*mmb_alloc)(const ot_mmz_alloc_para_in *para_in);
    ot_mmb_t *(*mmb_alloc_v2)(const ot_mmz_alloc_para_in *para_in);
    ot_mmb_t *(*mmb_alloc_v3)(const ot_mmz_alloc_para_in *para_in, ot_mmz_alloc_para_out *para_out);
    void *(*mmb_map2kern)(ot_mmb_t *mmb, int cached);
    int (*mmb_unmap)(ot_mmb_t *mmb);
    void (*mmb_free)(ot_mmb_t *mmb);
    void *(*mmf_map)(phys_addr_t phys, int len, int cache);
    void (*mmf_unmap)(void *virt);
};

int cma_allocator_setopt(struct mmz_allocator *allocator);
int ot_allocator_setopt(struct mmz_allocator *allocator);

#endif
