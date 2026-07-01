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

#ifndef VALG_TOOL_H
#define VALG_TOOL_H

#include "ot_type.h"
#include "ot_math.h"
#include "valg_fraction.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#if defined(__GNUC__) || defined(__clang__)
#define VALG_TOOL_STATIC static __attribute__((unused))
#else
#define VALG_TOOL_STATIC static
#endif

/*
 * 速率匹配器
 * 问题: 输入速率为in_rate，要求从中选取部分进行输出，且必须满足以下条件:
 *  1，输出速率为out_rate。
 *  2, 输出必须平稳。
 * 设计一种算法实现。
 *
 * 算法简介: 按照输出速率累加，当累加值 >= 输入速率时，输出当前输入，便可达到速率匹配。
 * 应用：帧率控制
 */
typedef struct {
    td_u32 in_rate_org; /* 原始的输入速率 */
    td_fr32 fr32_out_rate_org; /* 原始的输出速率，支持分数 */

    td_u32 in_rate; /* 输入速率 */
    td_u32 out_rate; /* 输出速率 */
    td_s32 out_rate_acc; /* 输出速率累计 */

    td_u32 out_cnt; /* 输出计数器 */
    td_u32 in_cnt; /* 输入计数器 */
} rate_match;

VALG_TOOL_STATIC td_void mtch_init_match(rate_match *match, td_u32 in_rate, td_fr32 fr32_out_rate)
{
    match->in_rate_org = in_rate;
    match->fr32_out_rate_org = fr32_out_rate;
    match->in_cnt = 0;
    match->out_cnt = 0;
}

VALG_TOOL_STATIC td_void mtch_init_fraction(rate_match *match, td_u32 in_rate, td_fr32 fr32_out_rate)
{
    match->in_rate = match->in_rate_org * DENOMINATOR32(fr32_out_rate);
    match->out_rate = NUMERATOR32(fr32_out_rate);
}

VALG_TOOL_STATIC td_void mtch_init_integer(rate_match *match, td_u32 in_rate, td_fr32 fr32_out_rate)
{
    match->in_rate = in_rate;
    match->out_rate = fr32_out_rate;
}

VALG_TOOL_STATIC td_void mtch_init(rate_match *match, td_u32 in_rate, td_fr32 fr32_out_rate)
{
    /* 输入帧率不支持分数 */
    ot_assert(!is_fraction32(in_rate));

    mtch_init_match(match, in_rate, fr32_out_rate);

    /* 分母为0表示整数 */
    if (is_fraction32(fr32_out_rate)) {
        mtch_init_fraction(match, in_rate, fr32_out_rate);
    } else {
        mtch_init_integer(match, in_rate, fr32_out_rate);
    }

    match->out_rate_acc = match->in_rate - match->out_rate;
}

/* 1，必须按照输入速率调用该接口 */
VALG_TOOL_STATIC td_void mtch_in_one(rate_match *match)
{
    match->out_rate_acc += (td_s32)match->out_rate;
    match->in_cnt++;
    return;
}

/* 2，每输入一次，都要查询该接口，确定是否输出 */
VALG_TOOL_STATIC td_bool mtch_time_to_out(rate_match *match)
{
    return (td_bool)(match->out_rate_acc >= (td_s32)match->in_rate);
}

/* 3，当确定输出时，调用该接口 */
VALG_TOOL_STATIC td_void mtch_out_one(rate_match *match)
{
    match->out_rate_acc -= (td_s32)match->in_rate;
    match->out_cnt++;
    return;
}

/* 超大帧重编时回退码率匹配信息，超大帧丢帧不回退 */
VALG_TOOL_STATIC td_void mtch_roll_back(rate_match *match)
{
    match->out_rate_acc = (match->out_rate_acc + (td_s32)match->in_rate - (td_s32)match->out_rate);
    match->in_cnt--;
    match->out_cnt--;
}

#define WATER_LINE_MAX (0x20000000)
#define WATER_LINE_MIN (0x0)

/* 虚拟缓存的特点: 输入流量不定，输出流量一定。 */
typedef struct {
    /* attribute of virtual buffer */
    td_s32 out_rate; /* 缓存流出速率, 单位: byte/second */
    td_s32 out_frequency; /* 缓存输出频率，单位: 次/秒 */
    td_s32 out_per_time; /* 每一次缓存的输出量 = out_rate/out_frequency */

    /* state of the virtual buffer. */
    td_s32 waterline; /* buffer waterline */
    td_s32 waterline_last; /* delta_waterline is waterline - waterline_last */
    td_s32 waterline_max; /* control waterline not overflow */
    td_s32 waterline_min; /* control waterline not underflow */

    td_u32 in_count; /* counter, total numbers in */
    td_u32 out_count; /* counter, total numbers out */
} virt_buf;

/* 初始化 */
VALG_TOOL_STATIC td_void virb_init(virt_buf *virb,
                                   td_s32 out_rate,
                                   td_s32 out_frequency,
                                   td_u32 max_delay)
{
    virb->out_rate = out_rate;
    virb->out_frequency = out_frequency;

    if (out_frequency != 0) {
        virb->out_per_time = virb->out_rate / virb->out_frequency;
    } else {
        virb->out_per_time = 0;
    }

    virb->waterline_max = max_delay * out_rate;
    virb->waterline_min = WATER_LINE_MIN;

    /* todo:从配置文件中读取 */
    virb->waterline = virb->waterline_max / 8; /* 8:default base rate */
    virb->waterline_last = virb->waterline;
    virb->in_count = 0;
    virb->out_count = 0;
    return;
}

VALG_TOOL_STATIC td_void virb_set(virt_buf *virb, td_s32 out_rate, td_s32 out_frequency, td_u32 max_delay)
{
    virb->out_rate = out_rate;
    virb->out_frequency = out_frequency;

    if (out_frequency != 0) {
        virb->out_per_time = virb->out_rate / virb->out_frequency;
    } else {
        virb->out_per_time = 0;
    }

    virb->waterline_max = max_delay * out_rate;
    virb->waterline_min = WATER_LINE_MIN;
    return;
}

VALG_TOOL_STATIC td_void virb_reset(virt_buf *virb)
{
    virb->waterline = 0;
    virb->waterline_last = 0;
    virb->in_count = 0;
    virb->out_count = 0;
    return;
}

/* 1, 每完成一次编码，就向虚拟buffer中注入码流 */
VALG_TOOL_STATIC td_void virb_in(virt_buf *virb, td_s32 amount)
{
    td_s32 waterline = virb->waterline;
    waterline += amount;
    waterline = MIN2(waterline, (virb->waterline_max));
    virb->waterline = waterline;
    virb->in_count++;
    return;
}

/* 2, 每处理完一帧图像，就模拟网络发送，从虚拟buffer中抽出相应数目的码流 */
VALG_TOOL_STATIC td_void virb_out(virt_buf *virb)
{
    td_s32 waterline = virb->waterline;
    waterline -= virb->out_per_time;
    waterline = MAX2(waterline, (virb->waterline_min));
    virb->waterline = waterline;
    virb->out_count++;
    return;
}

/* 连续丢帧时，调用该接口处理 */
VALG_TOOL_STATIC td_void virb_lost(virt_buf *virb, td_s32 num_lost)
{
    /* 适用: 图像丢失，无法进行VIRB更新。
      * 可以使用策略 */
    if (num_lost >= virb->out_frequency) {
        /* 丢失了一个输入周期的图像，说明系统出现过暂停，则重新开始 */
        virb_reset(virb);
    } else {
        /* 否则，虚拟buffer漏出码流
           * do
           * {
           *     virb_out(virb);
           *  } while(--num_lost);
           * 改用下面的代码，以提高效率
           */
        virb->waterline -= virb->out_per_time * num_lost;
        virb->out_count += num_lost;
    }
    return;
}

/* 获取当前水线情况 */
VALG_TOOL_STATIC td_s32 virb_get_waterline(virt_buf *virb)
{
    return virb->waterline;
}

/* 功能: 获取短期水线起伏情况。
 * 返回: 上次调用到本次调用的时间内，水线的变化情况 */
VALG_TOOL_STATIC td_s32 virb_get_delta_waterline(virt_buf *virb)
{
    td_s32 waterline_now;
    td_s32 waterline_last;
    waterline_now = virb->waterline;
    waterline_last = virb->waterline_last;
    virb->waterline_last = virb->waterline;
    return (waterline_now - waterline_last);
}

VALG_TOOL_STATIC td_u32 virb_get_in_count(virt_buf *virb)
{
    return virb->in_count;
}

VALG_TOOL_STATIC td_u32 virb_get_out_count(virt_buf *virb)
{
    return virb->out_count;
}

/*
 * 返回逝去的总时间，以秒为单位。
 * 应用: 1, 编码---可用于分段统计码率情况。
 */
VALG_TOOL_STATIC td_u32 virb_get_time(virt_buf *virb)
{
    td_u32 second;

    /* 总时间 = 输出总数/输出频率 */
    second = virb->out_count / virb->out_frequency;

    return second;
}

/* 下面的是辅助接口 */
VALG_TOOL_STATIC td_void virb_set_waterline_max(virt_buf *virb, td_s32 max)
{
    virb->waterline_max = max;
}

VALG_TOOL_STATIC td_void virb_set_waterline_min(virt_buf *virb, td_s32 min)
{
    virb->waterline_min = min;
}

VALG_TOOL_STATIC td_s32 virb_get_waterline_max(virt_buf *virb)
{
    return virb->waterline_max;
}

VALG_TOOL_STATIC td_s32 virb_get_waterline_min(virt_buf *virb)
{
    return virb->waterline_min;
}

/*********************************************************************
 *   FIFO
 *
 *********************************************************************/
#define FIFO_LEN_MAX   64
typedef struct {
    td_s32 elem[FIFO_LEN_MAX]; /* 存放元素的空间 */
    td_s32 len; /* FIFO 的真正长度 */
    td_s32 wr; /* 控制写入，避免元素移动 */
    td_s32 init_value; /* FIFO 初始值 */
} fifo;

VALG_TOOL_STATIC td_void fifo_in(fifo *fi_fo, td_s32 value)
{
    td_s32 wr;

    wr = fi_fo->wr;

    fi_fo->elem[wr] = value;

    wr++;
    if (wr >= fi_fo->len) {
        wr = 0;
    }

    fi_fo->wr = wr;
    return;
}

VALG_TOOL_STATIC td_void fifo_reset(fifo *fi_fo)
{
    td_s32 cnt;

    fi_fo->wr = 0;

    for (cnt = 0; cnt < fi_fo->len; cnt++) {
        fifo_in(fi_fo, fi_fo->init_value);
    }

    return;
}

VALG_TOOL_STATIC td_s32 fifo_len(fifo *fi_fo)
{
    return fi_fo->len;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* VALG_TOOL_H */
