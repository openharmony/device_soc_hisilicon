/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: Header file of wlan_ring.c.
 * Date: 2023-03-16
 */

#ifndef WLAN_RING_H
#define WLAN_RING_H

#include "securec.h"
#include "td_type.h"
#include "osal_types.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif

typedef struct {
    uintptr_t entries_addr;
    uintptr_t read_idx_addr;
    uintptr_t write_idx_addr;
    osal_u16 ring_depth;
    osal_u16 ring_entry_size;
} oal_ring_ctrl_stru;

typedef union {
    struct {
        osal_u32 index : 7;
        osal_u32 wrap_flag : 1;
        osal_u32 resv : 24;
    } bits;

    /* Define an unsigned member */
    osal_u32 u32;
} oal_ring_index_ctrl;

/* 判断ring是否为满，如果满，返回TRUE;反之，返回FALSE */
#define oal_ring_is_full(read_ctrl, write_ctrl) \
    ((((read_ctrl).bits.index) == ((write_ctrl).bits.index)) && \
    (((read_ctrl).bits.wrap_flag) != ((write_ctrl).bits.wrap_flag)))

/* 判断ring是否为空，如果空，返回TRUE;反之，返回FALSE */
#define oal_ring_is_empty(read_ctrl, write_ctrl) \
    (((read_ctrl).u32) == ((write_ctrl).u32))

/* 判断ring是否翻转，如果翻转，返回TRUE;反之，返回FALSE */
#define oal_ring_wrap_around(read_ctrl, write_ctrl) \
    (((read_ctrl).bits.wrap_flag) != ((write_ctrl).bits.wrap_flag))

#define OAL_WORD_TO_BYTE 4

osal_u32 oal_ring_write(oal_ring_ctrl_stru *ring_ctrl, osal_u32 *element);
osal_u32 oal_ring_read(oal_ring_ctrl_stru *ring_ctrl, osal_u32 *element);
osal_u32 oal_get_ring_element_num(oal_ring_ctrl_stru *ring_ctrl);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
#endif /* end of wlan_ring.h */
