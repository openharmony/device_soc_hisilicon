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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_CBB_BASED_EXT_INC_VALG_EXT_H_BODY_PART02_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_CBB_BASED_EXT_INC_VALG_EXT_H_BODY_PART02_H_

    src = (cpu_word *)virt_src;
    dst = (cpu_word *)wr_info->dst[0];
    i = wr_info->len[0] / sizeof(cpu_word);

#ifdef __LITEOS__
    while (i > 0) {
        (td_void)memcpy_s(dst, sizeof(cpu_word), src, sizeof(cpu_word));
        dst++;
        src++;
        i--;
    }
#else
    while (i > 0) {
        *dst++ = *src++;
        i--;
    }
#endif

    /* 第二段输入数据复制到buffer; */
    dst = (cpu_word *)wr_info->dst[1];
    i = wr_info->len[1] / sizeof(cpu_word);
#ifdef __LITEOS__
    while (i > 0) {
        (td_void)memcpy_s(dst, sizeof(cpu_word), src, sizeof(cpu_word));
        dst++;
        src++;
        i--;
    }
#else
    while (i > 0) {
        *dst++ = *src++;
        i--;
    }
#endif
    return;
}

VALG_STATIC td_s32 valg_cb_write(valg_crcl_buf *cb, td_void *virt_src, td_u32 wr_len)
{
    td_u32 free_len;
    valg_cb_wrinfo wr_info;
    td_u32 wr_head_new;
    td_u32 rd_tail = (*(cb->read_tail));
    td_u32 wr_head = (*(cb->write_head));

    if ((wr_len & (WORD_ALIGN - 1)) != 0) {
        OT_TRACE(OT_DBG_ERR, OT_ID_VALG, "\n_lenght writing in must be 4B aligned!");
        return TD_FAILURE;
    }

    /* 写头在读尾前面，说明buffer未折回 */
    if (wr_head >= rd_tail) {
        /* 计算写头未折回时buffer中的数据长度; */
        free_len = cb->buf_len - (wr_head - rd_tail) - cb->rsv_byte;
    } else {
        /* 计算写头折回时buffer中的数据长度; */
        free_len = rd_tail - wr_head - cb->rsv_byte;
    }

    /* 剩余空间不够写入新的数据包 */
    if (free_len < wr_len) {
        OT_TRACE(OT_DBG_WARN, OT_ID_VALG, "\n_warning! buffer left is small than the data writing in!");
        return TD_FAILURE;
    }

    /*
     * 调用写头前进函数，获取可以写入的两段数据包长度;
     * 根据写入数据后的写头是否超过buffer底部，来判断循环buffer是否折回;
     */
    if ((wr_head + wr_len) >= cb->buf_len) { /* buffer折回,恰好到达buffer底部也要折回 */
        /* 计算两段数据包的地址和长度 */
        wr_info.dst[0] = (addr_unit *)cb->base + wr_head;
        wr_info.len[0] = cb->buf_len - wr_head;
        wr_info.dst[1] = cb->base;
        wr_info.len[1] = wr_len - wr_info.len[0];
        wr_head_new = wr_info.len[1]; /* 写头指针指向下一数据包起始位置;  */
    } else {
        wr_info.dst[0] = (addr_unit *)cb->base + wr_head;
        wr_info.len[0] = wr_len;
        wr_info.dst[1] = (addr_unit *)wr_info.dst[0] + wr_len;
        wr_info.len[1] = 0; /* 第二段数据包长度等于0; */
        wr_head_new = wr_head + wr_len; /* 写头指针指向下一数据包起始位置; */
    }

    valg_cb_copy_write(&wr_info, virt_src);

    (*(cb->write_head)) = wr_head_new;
    return TD_SUCCESS;
}

VALG_STATIC td_s32 valg_cb_read(valg_crcl_buf *cb, td_u32 rd_len, valg_cb_rdinfo *rd_info)
{
    td_u32 rh;
    td_u32 rd_head = (*(cb->read_head));
    td_u32 wr_tail = (*(cb->write_tail));

    /*
     * 判断输入的读取长度是否正确。
     * 要求:更新后的读头不超过写尾。
     * 注意区分两种情况:
     * 1、写尾指针未发生回转。需判断;
     * 2、写尾指针发生回转。
     *    1)读头没有回转，读头不会超过写尾，无需判断;
     *    2)读头也发生回转，需判断;
     */
    rh = rd_head + rd_len;
    if (rd_head <= wr_tail) {
        if (rh > wr_tail) {
            return TD_FAILURE;
        }
    } else if (rh >= cb->buf_len) {
        rh -= cb->buf_len;
        if (rh > wr_tail) {
            return TD_FAILURE;
        }
    }

    /* 根据输入的读出数据包长度，判断循环buffer是否折回; */
    if ((rd_head + rd_len) >= cb->buf_len) { /* buffer折回，恰好到达buffer底部也要折回 */
        /* 计算两段数据包地址和长度 */
        rd_info->src[0] = (addr_unit *)cb->base + rd_head;
        rd_info->len[0] = cb->buf_len - rd_head;
        rd_info->src[1] = cb->base; /* 第二段数据包地址等于buffer首地址 */
        rd_info->len[1] = rd_len - rd_info->len[0];
        rd_head = rd_info->len[1]; /* 读头指针指向下一数据包起始位置 */
    } else {
        rd_info->src[0] = (addr_unit *)cb->base + rd_head;
        rd_info->len[0] = rd_len;
        rd_info->src[1] = (addr_unit *)rd_info->src[0] + rd_len;
        rd_info->len[1] = 0; /* 第二段数据包长度为0 */
        rd_head += rd_len; /* 读头指针指向下一数据包起始位置; */
    }

    /* 虚拟地址转换为物理地址 */
    if (cb->phy_base == NOT_SET_PHY_BASE) {
        /* if phy base addr equal 0, means not set, must return 0. */
        rd_info->phy_addr[0] = NOT_SET_PHY_BASE;
        rd_info->phy_addr[1] = NOT_SET_PHY_BASE;
    } else {
        /* 只要中间值不溢出，无符号数/有符号数加减不影响最后结果 */
        rd_info->phy_addr[0] = virt_to_phy((td_u64)(td_uintptr_t)rd_info->src[0],
            (td_u64)(td_uintptr_t)cb->base, cb->phy_base);
        rd_info->phy_addr[1] = virt_to_phy((td_u64)(td_uintptr_t)rd_info->src[1],
            (td_u64)(td_uintptr_t)cb->base, cb->phy_base);
    }

    (*(cb->read_head)) = rd_head;
    return TD_SUCCESS;
}

VALG_STATIC td_bool valg_cb_is_empty(const valg_crcl_buf *cb)
{
    td_u32 wr_head = (*(cb->write_head));
    td_u32 rd_tail = (*(cb->read_tail));

    if (wr_head == rd_tail) {
        return TD_TRUE;
    } else {
        return TD_FALSE;
    }
}

VALG_STATIC td_bool valg_cb_is_full(const valg_crcl_buf *cb)
{
    td_u32 wh;
    td_u32 rd_tail = (*(cb->read_tail));
    td_u32 wr_head = (*(cb->write_head));

    wh = wr_head + cb->rsv_byte;
    if (wh >= cb->buf_len) {
        wh -= cb->buf_len;
    }

    if (wh == rd_tail) {
        return TD_TRUE;
    } else {
        return TD_FALSE;
    }
}

VALG_STATIC td_s32 valg_cb_is_valid(const valg_crcl_buf *cb)
{
    td_u32 rd_head = (*(cb->read_head));
    td_u32 rd_tail = (*(cb->read_tail));
    td_u32 wr_head = (*(cb->write_head));
    td_u32 wr_tail = (*(cb->write_tail));

    /* 判断输入的读取长度是否正确。
     * 要求:更新后的读头不超过写尾。
     * 注意区分两种情况:
     * 1、写尾指针未发生回转。需判断;
     * 2、写尾指针发生回转。
     *    1)读头没有回转，读头不会超过写尾，无需判断;
     *    2)读头也发生回转，需判断;
     */
    if (((rd_tail <= rd_head) && (rd_head <= wr_tail) && (wr_tail <= wr_head)) ||
        ((wr_head <= rd_tail) && (rd_tail <= rd_head) && (rd_head <= wr_tail)) ||
        ((wr_tail <= wr_head) && (wr_head <= rd_tail) && (rd_tail <= rd_head)) ||
        ((rd_head <= wr_tail) && (wr_tail <= wr_head) && (wr_head <= rd_tail))) {
            return TD_SUCCESS;
        } else {
            return TD_FAILURE;
        }
}

VALG_STATIC td_u32 valg_cb_get_data_len(valg_crcl_buf *cb)
{
    td_u32 data_len;
    td_u32 rd_head = (*(cb->read_head));
    td_u32 wr_tail = (*(cb->write_tail));

    if (wr_tail >= rd_head) { /* 写尾在读头前面，说明buffer未折回 */
        /* 计算写尾折回时buffer中的数据长度 */
        data_len = wr_tail - rd_head;
    } else {
        /* 计算写尾折回时buffer中的数据长度 */
        data_len = cb->buf_len - (rd_head - wr_tail);
    }
    return data_len;
}

VALG_STATIC td_u32 valg_cb_get_free_len(valg_crcl_buf *cb)
{
    td_u32 free_len;
    td_u32 rd_tail = (*(cb->read_tail));
    td_u32 wr_head = (*(cb->write_head));

    if (wr_head >= rd_tail) { /* 写头在读尾前面，说明buffer未折回 */
        /* 计算写头未折回时buffer中的数据长度; */
        free_len = cb->buf_len - (wr_head - rd_tail) - cb->rsv_byte;
    } else {
        /* 计算写头折回时buffer中的数据长度; */
        free_len = rd_tail - wr_head - cb->rsv_byte;
    }

    return free_len;
}

VALG_STATIC td_u32 valg_cb_rd_region_len(valg_crcl_buf *cb)
{
    td_u32 rd_region_len;
    td_u32 rd_head = (*(cb->read_head));
    td_u32 rd_tail = (*(cb->read_tail));

    if (rd_head >= rd_tail) {
        rd_region_len = rd_head - rd_tail;
    } else {
        rd_region_len = cb->buf_len - (rd_tail - rd_head);
    }
    return rd_region_len;
}

VALG_STATIC td_u32 valg_cb_wr_region_len(valg_crcl_buf *cb)
{
    td_u32 wr_region_len;
    td_u32 wr_head = (*(cb->write_head));
    td_u32 wr_tail = (*(cb->write_tail));

    if (wr_head >= wr_tail) {
        wr_region_len = wr_head - wr_tail;
    } else {
        wr_region_len = cb->buf_len - (wr_tail - wr_head);
    }
    return wr_region_len;
}

VALG_STATIC td_void *valg_cb_get_rd_head(valg_crcl_buf *cb)
{
    td_void *rh;
    td_u32 rd_head = (*(cb->read_head));

    rh = (addr_unit *)(cb->base) + rd_head;
    return rh;
}

VALG_STATIC td_u64 valg_cb_get_rd_head_phy(valg_crcl_buf *cb)
{
    td_u32 rd_head = (*(cb->read_head));
    return cb->phy_base + rd_head;
}

VALG_STATIC td_void *valg_cb_get_rd_tail(valg_crcl_buf *cb)
{
    td_void *rt;
    td_u32 rd_tail = (*(cb->read_tail));

    rt = (addr_unit *)(cb->base) + rd_tail;
    return rt;
}

VALG_STATIC td_s32 valg_cb_update_rp(valg_crcl_buf *cb, td_u32 rd_len)
{
    td_u32 rt;
    td_u32 rd_head = (*(cb->read_head));
    td_u32 rd_tail = (*(cb->read_tail));

    if ((rd_len & (WORD_ALIGN - 1)) != 0) {
        OT_TRACE(OT_DBG_ERR, OT_ID_VALG, "%s err! len must be multiples of word(32-bit)!", __FUNCTION__);
        return TD_FAILURE;
    }

    /* 输入长度正确性判断 */
    rt = rd_tail + rd_len;

    /* 判断输入的读取长度是否正确。
     * 要求:更新后的读头不超过读尾。
     * 注意区分两种情况:
     * 1、读头未发生回转。需判断;
     * 2、读头发生回转。
     *    1)读尾没有回转，读头不会超过读尾，无需判断;
     *    2)读尾也发生回转，需判断;
     */
    if (rd_head >= rd_tail) {
        if (rt > rd_head) {
            return TD_FAILURE;
        }
    } else if (rt >= cb->buf_len) {
        rt -= cb->buf_len;
        if (rt > rd_head) {
            return TD_FAILURE;
        }
    }

    (*(cb->read_tail)) = rt;
    return TD_SUCCESS;
}

VALG_STATIC td_void valg_cb_update_wp(valg_crcl_buf *cb)
{
    (*(cb->write_tail)) = (*(cb->write_head));
    return;
}

VALG_STATIC td_void valg_cb_back_wp(valg_crcl_buf *cb)
{
    (*(cb->write_head)) = (*(cb->write_tail));
    return;
}

VALG_STATIC td_void valg_cb_back_rp(valg_crcl_buf *cb)
{
    (*(cb->read_head)) = (*(cb->read_tail));
    return;
}

VALG_STATIC td_void valg_cb_reset(valg_crcl_buf *cb)
{
    (*(cb->read_head)) = 0;
    (*(cb->read_tail)) = 0;
    (*(cb->write_head)) = 0;
    (*(cb->write_tail)) = 0;

    return;
}

VALG_STATIC td_void valg_cb_reset_master_ex(valg_crcl_buf *cb)
{
    (*(cb->read_head)) = 0;
    return;
}

VALG_STATIC td_s32 valg_cb_get_copy(valg_crcl_buf *org_cb, valg_crcl_buf *copy_cb)
{
    if ((org_cb == TD_NULL) || (copy_cb == TD_NULL)) {
        return TD_FAILURE;
    }

    /* make a copy of original CB. */
    copy_cb->base = org_cb->base;
    copy_cb->phy_base = org_cb->phy_base;
    copy_cb->rd_head = org_cb->rd_head;
    copy_cb->rd_tail = org_cb->rd_tail;
    copy_cb->wr_head = org_cb->wr_head;
    copy_cb->wr_tail = org_cb->wr_tail;
    copy_cb->buf_len = org_cb->buf_len;
    copy_cb->rsv_byte = org_cb->rsv_byte;

    /* reinitiate the pointer to the copy CB. */
    copy_cb->read_head = &copy_cb->rd_head;
    copy_cb->read_tail = &copy_cb->rd_tail;
    copy_cb->write_head = &copy_cb->wr_head;
    copy_cb->write_tail = &copy_cb->wr_tail;

    return TD_SUCCESS;
}

VALG_STATIC td_void *valg_cb_get_wr_head(valg_crcl_buf *cb)
{
    td_void *wh;
    td_u32 wr_head = (*(cb->write_head));

    wh = (addr_unit *)(cb->base) + wr_head;
    return wh;
}

VALG_STATIC td_u64 valg_cb_get_wr_head_phy(valg_crcl_buf *cb)
{
    td_u32 wr_head = (*(cb->write_head));
    return cb->phy_base + wr_head;
}

VALG_STATIC td_void *valg_cb_get_wr_tail(valg_crcl_buf *cb)
{
    td_void *wt;
    td_u32 wr_tail = (*(cb->write_tail));

    wt = (addr_unit *)(cb->base) + wr_tail;
    return wt;
}

VALG_STATIC td_u64 valg_cb_get_wr_tail_phy(valg_crcl_buf *cb)
{
    td_u32 wr_tail = (*(cb->write_tail));
    return cb->phy_base + wr_tail;
}

VALG_STATIC td_s32 valg_cb_update_wr_head(valg_crcl_buf *cb, td_u32 wr_len)
{
    td_u32 wh; /* 更新后的写头 */
    td_u32 wr_head = (*(cb->write_head));
    td_u32 rd_tail = (*(cb->read_tail));

    if ((wr_len & (WORD_ALIGN - 1)) != 0) {
        OT_TRACE(OT_DBG_ERR, OT_ID_VALG, "%s err! len(%d) must be 4B aligned!", __FUNCTION__, wr_len);
        return TD_FAILURE;
    }

    /* 输入长度正确性判断 */


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_CBB_BASED_EXT_INC_VALG_EXT_H_BODY_PART02_H_ */
