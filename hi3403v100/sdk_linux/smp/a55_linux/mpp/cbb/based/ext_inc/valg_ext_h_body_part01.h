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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_CBB_BASED_EXT_INC_VALG_EXT_H_BODY_PART01_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_CBB_BASED_EXT_INC_VALG_EXT_H_BODY_PART01_H_


#include "ot_type.h"
#include "ot_debug.h"
#include "ot_osal.h"
#ifdef __LITEOS__
#include "securec.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef OT_DEBUG
#define base_alg_assert(expr)    ot_assert(expr)
#else
#define base_alg_assert(expr)
#endif

#if defined(__GNUC__) || defined(__clang__)
#define VALG_STATIC static __attribute__((unused))
#else
#define VALG_STATIC static
#endif

/* circular quenue */
typedef struct {
    td_ulong *base; /* queue base addr    */
    td_s32 max_len; /* queue max length   */
    td_s32 tail; /* queue tail pointer */
    td_s32 head; /* queue head pointer */
    td_s32 real_len; /* queue real lentth  */
} valg_queue;

VALG_STATIC td_void valg_queue_init(valg_queue *que, const td_void *base, td_s32 max_len);
VALG_STATIC td_s32 valg_queue_put_to_head(valg_queue *que, td_ulong data);
VALG_STATIC td_s32 valg_queue_get_from_head(valg_queue *que, td_ulong *data);
VALG_STATIC td_s32 valg_queue_put_to_tail(valg_queue *que, td_ulong data);
VALG_STATIC td_s32 valg_queue_get_from_tail(valg_queue *que, td_ulong *data);
VALG_STATIC td_s32 valg_queue_search(const valg_queue *que, td_ulong *data, td_s32 *index);
VALG_STATIC td_s32 valg_queue_search_from_head(const valg_queue *que, td_ulong *data, td_s32 index);
VALG_STATIC td_s32 valg_queue_search_from_tail(const valg_queue *que, td_ulong *data, td_s32 index);
VALG_STATIC td_s32 valg_queue_get_real_len(const valg_queue *que);
VALG_STATIC td_bool valg_queue_is_full(const valg_queue *que);
VALG_STATIC td_bool valg_queue_is_empty(const valg_queue *que);

/* 读循环buffer的输出数据类型 */
typedef struct {
    td_void *src[2];
    td_u64 phy_addr[2];
    td_u32 len[2];
} valg_cb_rdinfo;

/* 写循环buffer的输出数据类型 */
typedef struct {
    td_void *dst[2];
    td_u32 len[2];
} valg_cb_wrinfo;

/* 循环buffer */
typedef struct {
    td_u64 phy_base; /* 0循环buffer首地址物理地址, call valg_cb_set_phy_base to set. */

    td_u32 buf_len; /* 8循环buffer的总长度，单位为字节 */
    td_u32 rsv_byte; /* 12用于判断buffer空满的保留空间，单位为字节 */
    td_u32 au32res0[4]; /* 16占空位 */
    td_u32 rd_head; /* 32循环buffer读头，偏移量，单位为字节 */
    td_u32 au32res1[7]; /* 36占空位 */
    td_u32 rd_tail; /* 64循环buffer读尾，偏移量，单位为字节 */
    td_u32 au32res2[7]; /* 68占空位 */
    td_u32 wr_tail; /* 96循环buffer写头，偏移量，单位为字节 */
    td_u32 au32res3[7]; /* 100占空位 */
    td_u32 wr_head; /* 128循环buffer写尾，偏移量，单位为字节 */
    td_u32 au32res4[7]; /* 132占空位,逻辑数据的写出每次是以128bit为单位,所以这里至少128bit */
    td_u32 *read_head; /* 160循环buffer读头，偏移量，单位为字节 */
    td_u32 *read_tail; /* 168循环buffer读尾，偏移量，单位为字节 */
    td_u32 *write_head; /* 176循环buffer写头，偏移量，单位为字节 */
    td_u32 *write_tail; /* 184循环buffer写尾，偏移量，单位为字节 */
    td_void *base; /* 192虚拟基地址 */
    /* 200 */
} valg_crcl_buf;

/* 供外部调用的接口函数 */
VALG_STATIC td_s32 valg_cb_init_ex(valg_crcl_buf *cb, td_void *virt_base, td_u32 buf_len, td_u32 rsv_byte);
VALG_STATIC td_s32 valg_cb_init(valg_crcl_buf *cb, td_void *virt_base, td_u32 buf_len, td_u32 rsv_byte);
VALG_STATIC td_s32 valg_cb_write(valg_crcl_buf *cb, td_void *virt_src, td_u32 wr_len);
VALG_STATIC td_s32 valg_cb_read(valg_crcl_buf *cb, td_u32 rd_len, valg_cb_rdinfo *rd_info);
VALG_STATIC td_s32 valg_cb_update_rp(valg_crcl_buf *cb, td_u32 rd_len);
VALG_STATIC td_void valg_cb_update_wp(valg_crcl_buf *cb);
VALG_STATIC td_void valg_cb_back_wp(valg_crcl_buf *cb);
VALG_STATIC td_void *valg_cb_get_rd_head(valg_crcl_buf *cb);
VALG_STATIC td_void *valg_cb_get_rd_tail(valg_crcl_buf *cb);
VALG_STATIC td_u32 valg_cb_get_data_len(valg_crcl_buf *cb);
VALG_STATIC td_u32 valg_cb_get_free_len(valg_crcl_buf *cb);
VALG_STATIC td_u32 valg_cb_rd_region_len(valg_crcl_buf *cb);
VALG_STATIC td_u32 valg_cb_wr_region_len(valg_crcl_buf *cb);
VALG_STATIC td_void valg_cb_reset(valg_crcl_buf *cb);
VALG_STATIC td_s32 valg_cb_set_phy_base(valg_crcl_buf *cb, td_u64 phy_base);

/************************************************
 处理器相关的定义

 注意事项：
 1)所有的长度单位均是处理器的寻址步长，ARM处理器的地址单位是"字节";
 2)所有的读写操作按照word进行，所以要求：
   buffer中数据包的长度应按word对齐;
   队列中每个元素的长度应按word对齐;
*************************************************/
/* 处理器的寻址步长。表示地址长度的单位 */
typedef td_u8 addr_unit;

/* 处理器字长。表示读写操作的单位 */
typedef td_u64 cpu_word;

/* 每一个word包含的字节数 */
#define WORD_ALIGN       0x04

VALG_STATIC td_void valg_queue_init(valg_queue *que, const td_void *base, td_s32 max_len)
{
    base_alg_assert(que != NULL);
    base_alg_assert(base != NULL);
    base_alg_assert(max_len > 0);

    que->base = (td_ulong *)base;
    que->max_len = max_len;
    que->tail = 0;
    que->head = 0;
    que->real_len = 0;
}

VALG_STATIC td_s32 valg_queue_put_to_head(valg_queue *que, td_ulong data)
{
    td_s32 head;
    td_s32 real_len;

    base_alg_assert(que != NULL);

    head = que->head;
    real_len = que->real_len;

    if (real_len < que->max_len) {
        que->base[head] = data;
        if ((++head) >= que->max_len) {
            head = 0;
        }
        que->head = head;
        que->real_len = (real_len + 1);

        return TD_SUCCESS;
    }

    return TD_FAILURE;
}

VALG_STATIC td_s32 valg_queue_get_from_head(valg_queue *que, td_ulong *data)
{
    td_s32 head;
    td_s32 real_len;

    base_alg_assert(que != NULL);
    base_alg_assert(data != NULL);

    head = que->head;
    real_len = que->real_len;

    if (real_len > 0) {
        if ((--head) < 0) {
            head += que->max_len;
        }
        *data = que->base[head];
        que->head = head;
        que->real_len = (real_len - 1);

        return TD_SUCCESS;
    }

    return TD_FAILURE;
}

VALG_STATIC td_s32 valg_queue_put_to_tail(valg_queue *que, td_ulong data)
{
    td_s32 tail;
    td_s32 real_len;

    base_alg_assert(que != NULL);

    tail = que->tail;
    real_len = que->real_len;

    if (real_len < que->max_len) {
        if ((--tail) < 0) {
            tail += que->max_len;
        }
        que->base[tail] = data;

        que->tail = tail;
        que->real_len = (real_len + 1);

        return TD_SUCCESS;
    }

    return TD_FAILURE;
}

VALG_STATIC td_s32 valg_queue_get_from_tail(valg_queue *que, td_ulong *data)
{
    td_s32 tail;
    td_s32 real_len;

    base_alg_assert(que != NULL);
    base_alg_assert(data != NULL);

    tail = que->tail;
    real_len = que->real_len;

    if (real_len > 0) {
        *data = que->base[tail];
        if ((++tail) >= que->max_len) {
            tail = 0;
        }
        que->tail = tail;
        que->real_len = (real_len - 1);

        return TD_SUCCESS;
    }

    return TD_FAILURE;
}

VALG_STATIC td_s32 valg_queue_search(const valg_queue *que, td_ulong *data, td_s32 *index)
{
    td_s32 tail;
    td_s32 real_len;

    base_alg_assert(que != NULL);
    base_alg_assert(data != NULL);
    base_alg_assert(index != NULL);

    tail = que->tail + (*index);
    real_len = que->real_len;

    if ((real_len > 0) && ((*index) < real_len)) {
        if ((tail) >= que->max_len) {
            tail -= que->max_len;
        }

        *data = que->base[tail];

        (*index)++;

        return TD_SUCCESS;
    }

    return TD_FAILURE;
}

VALG_STATIC td_s32 valg_queue_search_from_head(const valg_queue *que, td_ulong *data, td_s32 index)
{
    td_s32 head;
    td_s32 real_len;

    base_alg_assert(que != NULL);
    base_alg_assert(data != NULL);
    base_alg_assert(index >= 0);

    head = que->head - index - 1;
    real_len = que->real_len;

    if ((real_len > 0) && (index < real_len)) {
        if ((head) < 0) {
            head += que->max_len;
        }

        *data = que->base[head];

        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

VALG_STATIC td_s32 valg_queue_search_from_tail(const valg_queue *que, td_ulong *data, td_s32 index)
{
    td_s32 tail;
    td_s32 real_len;

    base_alg_assert(que != NULL);
    base_alg_assert(data != NULL);
    base_alg_assert(index >= 0);

    tail = que->tail + index;
    real_len = que->real_len;

    if ((real_len > 0) && (index < real_len)) {
        if ((tail) >= que->max_len) {
            tail -= que->max_len;
        }

        *data = que->base[tail];

        return TD_SUCCESS;
    }

    return TD_FAILURE;
}

VALG_STATIC td_s32 valg_queue_get_real_len(const valg_queue *que)
{
    base_alg_assert(que != NULL);

    return que->real_len;
}

VALG_STATIC td_bool valg_queue_is_full(const valg_queue *que)
{
    base_alg_assert(que != NULL);

    return (td_bool)(que->real_len == que->max_len);
}

VALG_STATIC td_s32 valg_queue_get_from_head_for_index(valg_queue *que, td_ulong *data, td_s32 index)
{
    td_s32 head;
    td_s32 tail;
    td_s32 real_len;
    td_s32 i;

    base_alg_assert(que != NULL);
    base_alg_assert(data != NULL);

    head = que->head - index - 1;
    real_len = que->real_len;
    tail = que->tail;

    if ((real_len > 0) && (index < real_len)) {
        if ((head) < 0) {
            head += que->max_len;
        }

        *data = que->base[head];
        que->real_len = (real_len - 1);

        if (head > tail) {
            for (i = head; i > tail; i--) {
                que->base[i] = que->base[i - 1];
            }
            if ((++tail) >= que->max_len) {
                tail = 0;
            }
            que->tail = tail;
        } else if (head < tail) {
            for (i = head; i > 0; i--) {
                que->base[i] = que->base[i - 1];
            }
            que->base[0] = que->base[que->max_len - 1];
            for (i = que->max_len - 1; i > tail; i--) {
                que->base[i] = que->base[i - 1];
            }
            if ((++tail) >= que->max_len) {
                tail = 0;
            }
            que->tail = tail;
        } else {
            if ((++tail) >= que->max_len) {
                tail = 0;
            }
            que->tail = tail;
        }

        return TD_SUCCESS;
    }

    return TD_FAILURE;
}

VALG_STATIC td_bool valg_queue_is_empty(const valg_queue *que)
{
    base_alg_assert(que != NULL);

    return (td_bool)(que->real_len == 0);
}

#define NOT_SET_PHY_BASE 0
#define virt_to_phy(va, V, P) ((va) - (V) + (P))
VALG_STATIC td_s32 valg_cb_init_ex(valg_crcl_buf *cb, td_void *virt_base, td_u32 buf_len, td_u32 rsv_byte)
{
    if (((buf_len & (WORD_ALIGN - 1)) != 0) || (buf_len < rsv_byte)) {
        OT_TRACE(OT_DBG_ERR, OT_ID_VALG, "buf_len must be 4B aligned!");
        return TD_FAILURE;
    }

    if ((rsv_byte == 0) || ((rsv_byte & (WORD_ALIGN - 1)) != 0)) {
        OT_TRACE(OT_DBG_ERR, OT_ID_VALG, "rsv_byte must be 4B aligned!");
        return TD_FAILURE;
    }

    cb->base = virt_base;
    cb->phy_base = NOT_SET_PHY_BASE;
    cb->read_head = &(cb->rd_head);
    cb->read_tail = &(cb->rd_tail);
    cb->write_head = &(cb->wr_head);
    cb->write_tail = &(cb->wr_tail);

    cb->buf_len = buf_len;
    cb->rsv_byte = rsv_byte;
    *(cb->read_head) = 0;
    *(cb->read_tail) = 0;
    *(cb->write_head) = 0;
    *(cb->write_tail) = 0;
    return TD_SUCCESS;
}
/* 从arm侧的初始化使用,使用本身的默认内存 */
VALG_STATIC td_s32 valg_cb_init(valg_crcl_buf *cb, td_void *virt_base, td_u32 buf_len, td_u32 rsv_byte)
{
    return valg_cb_init_ex(cb, virt_base, buf_len, rsv_byte);
}

VALG_STATIC td_s32 valg_cb_set_phy_base(valg_crcl_buf *cb, td_u64 phy_base)
{
    cb->phy_base = phy_base;
    return TD_SUCCESS;
}

VALG_STATIC td_void valg_cb_copy_write(valg_cb_wrinfo *wr_info, td_void *virt_src)
{
    td_u32 i;
    cpu_word *dst = TD_NULL;
    cpu_word *src = TD_NULL;

    /* 第一段输入数据复制到buffer; */


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_CBB_BASED_EXT_INC_VALG_EXT_H_BODY_PART01_H_ */
