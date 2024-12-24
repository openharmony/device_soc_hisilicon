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
 * Description: header file for oal_netbuf.c
 * Create: 2022-04-26
 */
#ifndef __OAL_NETBUF_H__
#define __OAL_NETBUF_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "osal_types.h"
#include "oal_net.h"
#include "wlan_spec.h"
#include "oal_net.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
static inline oal_netbuf_stru* oal_netbuf_copy(oal_netbuf_stru *pst_netbuf, oal_gfp_enum_uint8 priority)
{
    return skb_copy(pst_netbuf, priority);
}

static inline oal_netbuf_stru* oal_netbuf_delist_nolock(oal_netbuf_head_stru *list_head)
{
    return __skb_dequeue(list_head);
}

static inline td_u32 oal_netbuf_free_list(oal_netbuf_head_stru *head, td_u32 num)
{
    td_u32 index;
    td_u32 ul_ret;

    for (index = 0; index < num; index++) {
        ul_ret = oal_netbuf_free(oal_netbuf_delist(head));
        if (OAL_SUCC != ul_ret) {
            return ul_ret;
        }
    }

    return OAL_SUCC;
}

typedef union {
    td_u8 byte;
    struct  {
        td_u8 high : 4,
        low : 4;
    } bits;
} bitfield;

static inline td_u8 oal_netbuf_get_bitfield(td_void)
{
    bitfield un_bitfield;
    un_bitfield.byte = 0x12;
    if (un_bitfield.bits.low == 0x2) {
        return OAL_BITFIELD_LITTLE_ENDIAN;
    } else {
        return OAL_BITFIELD_BIG_ENDIAN;
    }
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* end of oal_netbuf.h */
