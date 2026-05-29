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
#include <linux/platform_device.h>
#include "ot_irq.h"
#include "ot_osal.h"
#include "securec.h"
#include "mm_ext.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define SINGLE_REG_SIZE   4
#define MAX_NODE_LIMIT    64
#define OT_IRQ_PAGE_SHIFT  12

static int node_cnt = 0;
static osal_spinlock_t g_irq_spin_lock = { 0 };
#define irq_spin_lock(flags) osal_spin_lock_irqsave(&g_irq_spin_lock, &(flags))
#define irq_spin_unlock(flags) osal_spin_unlock_irqrestore(&g_irq_spin_lock, &(flags))

#define OT_IRQ_PFX "ot_irq: "

#ifdef CONFIG_OT_LOG_TRACE_SUPPORT
#define ot_irq_trace osal_printk
#else
#define ot_irq_trace(str, fmt...)
#endif

typedef struct {
    volatile unsigned int reg_num;
    volatile unsigned int *raw_int_reg[OT_MAX_STATE_NUM];
    volatile unsigned int *mask_int_reg[OT_MAX_STATE_NUM];
    volatile unsigned int *clr_int_reg[OT_MAX_STATE_NUM];
} ot_irq_normal_reg_map;

typedef struct {
    volatile unsigned int *cap_int_reg;
    volatile unsigned int *src_int_reg;
    volatile unsigned int *pt_int_reg[OT_VI_MAX_DEV_NUM];
    volatile unsigned int *ch_int_reg[OT_VI_MAX_PHYS_PIPE_NUM];
    volatile unsigned int *isp_fe_int_reg[OT_VI_MAX_PHYS_PIPE_NUM];
} ot_irq_vicap_reg_map;

typedef struct {
    volatile unsigned int *aio_int_status_reg;
    volatile unsigned int *aio_int_raw_reg;
    volatile unsigned int *tx_clr_reg[OT_AO_DEV_MAX_NUM];
    volatile unsigned int *tx_raw_reg[OT_AO_DEV_MAX_NUM];
    volatile unsigned int *tx_stat_reg[OT_AO_DEV_MAX_NUM];
    volatile unsigned int *rx_clr_reg[OT_AI_DEV_MAX_NUM];
    volatile unsigned int *rx_raw_reg[OT_AI_DEV_MAX_NUM];
    volatile unsigned int *rx_stat_reg[OT_AI_DEV_MAX_NUM];
} ot_irq_aio_reg_map;

typedef struct {
    ot_int_type type;
    union {
        ot_irq_normal_reg_map normal;
        ot_irq_vicap_reg_map vicap;
        ot_irq_aio_reg_map aio;
    };
} ot_irq_reg_map_info;

typedef struct irq_strct {
    int                  irq_cnt;
    ot_irq_irq_attr      irq_attr;
    osal_wait_t          irq_wait;
    ot_irq_reg_map_info  map_info;
    ot_int_state_info    int_info;
    struct irq_strct     *next;
} ot_irq_irq_list;

static ot_irq_irq_list *head = NULL;

static ot_irq_irq_list *add_list(ot_irq_irq_attr *irqattr)
{
    ot_irq_irq_list *tmp;

    tmp = (ot_irq_irq_list *)osal_kmalloc(sizeof(ot_irq_irq_list), osal_gfp_kernel);
    if (tmp == NULL) {
        return NULL;
    }
    (void)memset_s(tmp, sizeof(ot_irq_irq_list), 0, sizeof(ot_irq_irq_list));
    (void)memcpy_s(&(tmp->irq_attr), sizeof(ot_irq_irq_attr), irqattr, sizeof(ot_irq_irq_attr));
    tmp->irq_attr.enable_flag = 0;
    tmp->irq_cnt = 0;
    osal_wait_init(&(tmp->irq_wait));
    tmp->next = head;
    head = tmp;
    return tmp;
}

static ot_irq_irq_list *get_list_node(int irq, void *dev_id)
{
    ot_irq_irq_list *tmp = head;

    node_cnt = 0;
    while (tmp != NULL) {
        if (tmp->irq_attr.irq_num == irq && ((tmp->irq_attr.dev == dev_id) || (tmp->irq_attr.dev == NULL))) {
            if (tmp->irq_attr.dev == NULL) {
                tmp->irq_attr.dev = dev_id;
            }
            return tmp;
        }
        node_cnt++;
        tmp = tmp->next;
    }
    return NULL;
}

static void unmap_int_reg_for_aio(ot_irq_aio_reg_map *map)
{
    int i;

    if (map->aio_int_status_reg != NULL) {
        osal_iounmap((void *)map->aio_int_status_reg, SINGLE_REG_SIZE);
        map->aio_int_status_reg = NULL;
    }
    if (map->aio_int_raw_reg != NULL) {
        osal_iounmap((void *)map->aio_int_raw_reg, SINGLE_REG_SIZE);
        map->aio_int_raw_reg = NULL;
    }

    for (i = 0; i < OT_AO_DEV_MAX_NUM; i++) {
        if (map->tx_clr_reg[i] != NULL) {
            osal_iounmap((void *)map->tx_clr_reg[i], SINGLE_REG_SIZE);
            map->tx_clr_reg[i] = NULL;
        }
        if (map->tx_raw_reg[i] != NULL) {
            osal_iounmap((void *)map->tx_raw_reg[i], SINGLE_REG_SIZE);
            map->tx_raw_reg[i] = NULL;
        }
        if (map->tx_stat_reg[i] != NULL) {
            osal_iounmap((void *)map->tx_stat_reg[i], SINGLE_REG_SIZE);
            map->tx_stat_reg[i] = NULL;
        }
    }

    for (i = 0; i < OT_AI_DEV_MAX_NUM; i++) {
        if (map->rx_clr_reg[i] != NULL) {
            osal_iounmap((void *)map->rx_clr_reg[i], SINGLE_REG_SIZE);
            map->rx_clr_reg[i] = NULL;
        }
        if (map->rx_raw_reg[i] != NULL) {
            osal_iounmap((void *)map->rx_raw_reg[i], SINGLE_REG_SIZE);
            map->rx_raw_reg[i] = NULL;
        }
        if (map->rx_stat_reg[i] != NULL) {
            osal_iounmap((void *)map->rx_stat_reg[i], SINGLE_REG_SIZE);
            map->rx_stat_reg[i] = NULL;
        }
    }

    return;
}

static void unmap_int_reg_for_vicap(ot_irq_vicap_reg_map *map)
{
    int i;

    if (map->cap_int_reg != NULL) {
        osal_iounmap((void *)map->cap_int_reg, SINGLE_REG_SIZE);
        map->cap_int_reg = NULL;
    }

    if (map->src_int_reg != NULL) {
        osal_iounmap((void *)map->src_int_reg, SINGLE_REG_SIZE);
        map->src_int_reg = NULL;
    }

    for (i = 0; i < OT_VI_MAX_DEV_NUM; i++) {
        if (map->pt_int_reg[i] != NULL) {
            osal_iounmap((void *)map->pt_int_reg[i], SINGLE_REG_SIZE);
            map->pt_int_reg[i] = NULL;
        }
    }

    for (i = 0; i < OT_VI_MAX_PHYS_PIPE_NUM; i++) {
        if (map->ch_int_reg[i] != NULL) {
            osal_iounmap((void *)map->ch_int_reg[i], SINGLE_REG_SIZE);
            map->ch_int_reg[i] = NULL;
        }
    }

    for (i = 0; i < OT_VI_MAX_PHYS_PIPE_NUM; i++) {
        if (map->isp_fe_int_reg[i] != NULL) {
            osal_iounmap((void *)map->isp_fe_int_reg[i], SINGLE_REG_SIZE);
            map->isp_fe_int_reg[i] = NULL;
        }
    }

    return;
}

static void unmap_int_reg_for_normal(ot_irq_normal_reg_map *map)
{
    int i;

    for (i = 0; i < map->reg_num; i++) {
        if (map->raw_int_reg[i] != NULL) {
            osal_iounmap((void *)map->raw_int_reg[i], SINGLE_REG_SIZE);
            map->raw_int_reg[i] = NULL;
        }
        if (map->mask_int_reg[i] != NULL) {
            osal_iounmap((void *)map->mask_int_reg[i], SINGLE_REG_SIZE);
            map->mask_int_reg[i] = NULL;
        }
        if (map->clr_int_reg[i] != NULL) {
            osal_iounmap((void *)map->clr_int_reg[i], SINGLE_REG_SIZE);
            map->clr_int_reg[i] = NULL;
        }
    }
    map->reg_num = 0;

    return;
}

static void unmap_int_reg(ot_irq_reg_map_info *map_info)
{
    if (map_info->type == OT_INT_VICAP) {
        unmap_int_reg_for_vicap(&map_info->vicap);
    } else if (map_info->type == OT_INT_AIO) {
        unmap_int_reg_for_aio(&map_info->aio);
    } else {
        unmap_int_reg_for_normal(&map_info->normal);
    }
    return;
}

static void del_list(void)
{
    ot_irq_irq_list *tmp = head;
    ot_irq_irq_list *tmp2 = NULL;

    while (tmp != NULL) {
        tmp2 = tmp->next;
        osal_wait_destroy(&(tmp->irq_wait));
        unmap_int_reg(&(tmp->map_info));
        osal_kfree(tmp);
        tmp = tmp2;
    }
    head = NULL;
}

static void read_int_status_for_aio(ot_irq_aio_reg_map *map, ot_aio_int_state *state)
{
    int i;

    state->aio_int_status_state = *map->aio_int_status_reg;
    state->aio_int_raw_state = *map->aio_int_raw_reg;

    for (i = 0; i < OT_AO_DEV_MAX_NUM; i++) {
        state->tx_raw_state[i] = *map->tx_raw_reg[i];
        state->tx_stat_state[i] = *map->tx_stat_reg[i];
    }
    for (i = 0; i < OT_AI_DEV_MAX_NUM; i++) {
        state->rx_raw_state[i] = *map->rx_raw_reg[i];
        state->rx_stat_state[i] = *map->rx_stat_reg[i];
    }
    return;
}

static void read_int_status_for_vicap(ot_irq_vicap_reg_map *map, ot_vicap_int_state *state)
{
    int i;

    state->cap_state = *map->cap_int_reg;
    state->src_state = *map->src_int_reg;
    for (i = 0; i < OT_VI_MAX_DEV_NUM; i++) {
        state->pt_state[i] = *map->pt_int_reg[i];
    }
    for (i = 0; i < OT_VI_MAX_PHYS_PIPE_NUM; i++) {
        state->ch_state[i] = *map->ch_int_reg[i];
    }
    for (i = 0; i < OT_VI_MAX_PHYS_PIPE_NUM; i++) {
        state->isp_fe_state[i] = *map->isp_fe_int_reg[i];
    }
    return;
}

static void read_int_status_for_normal(ot_irq_normal_reg_map *map, ot_normal_int_state *state)
{
    int i;

    state->state_num = map->reg_num;
    for (i = 0; i < map->reg_num; i++) {
        state->raw_state[i] = *(map->raw_int_reg[i]);
        state->mask_state[i] = *(map->mask_int_reg[i]);
    }

    return;
}

static void read_int_status(ot_irq_irq_list *irq_node, ot_int_state_info *int_info)
{
    int_info->type = irq_node->map_info.type;
    if (irq_node->map_info.type == OT_INT_VICAP) {
        read_int_status_for_vicap(&irq_node->map_info.vicap, &int_info->vicap);
    } else if (irq_node->map_info.type == OT_INT_AIO) {
        read_int_status_for_aio(&irq_node->map_info.aio, &int_info->aio);
    } else {
        read_int_status_for_normal(&irq_node->map_info.normal, &int_info->normal);
    }
    return;
}

static void clear_int_for_aio(ot_irq_aio_reg_map *map, ot_aio_int_state *state)
{
    int i;

    for (i = 0; i < OT_AI_DEV_MAX_NUM; i++) {
        *map->rx_clr_reg[i] = state->rx_raw_state[i];
    }

    for (i = 0; i < OT_AO_DEV_MAX_NUM; i++) {
        *map->tx_clr_reg[i] = state->tx_raw_state[i];
    }

    return;
}

static void clear_int_for_vicap(ot_irq_vicap_reg_map *map, ot_vicap_int_state *state)
{
    int i;

    *map->cap_int_reg = state->cap_state;
    *map->src_int_reg = state->src_state;
    for (i = 0; i < OT_VI_MAX_DEV_NUM; i++) {
        *map->pt_int_reg[i] = state->pt_state[i];
    }
    for (i = 0; i < OT_VI_MAX_PHYS_PIPE_NUM; i++) {
        *map->ch_int_reg[i] = state->ch_state[i];
    }
    for (i = 0; i < OT_VI_MAX_PHYS_PIPE_NUM; i++) {
        *map->isp_fe_int_reg[i] = state->isp_fe_state[i];
    }
    return;
}

static void clear_int_for_scd(ot_irq_normal_reg_map *map)
{
    int i;
    for (i = 0; i < map->reg_num; i++) {
        *map->clr_int_reg[i] = 1;
    }
    return;
}

static void clear_int_for_vdh(ot_irq_normal_reg_map *map, ot_normal_int_state *state)
{
    int i;
    for (i = 0; i < map->reg_num; i++) {  /* 16 17 2bit */
        *map->clr_int_reg[i] = ((state->raw_state[i] >> 16) & 0x1 << 2) + ((state->raw_state[i] >> 17) & 0x1);
    }
    return;
}

static void clear_int_for_nnie(ot_irq_normal_reg_map *map, ot_normal_int_state *state)
{
    int i;
    for (i = 0; i < map->reg_num; i++) {
        *map->clr_int_reg[i] = state->mask_state[i];
    }
    return;
}

static void clear_int_for_normal(ot_irq_normal_reg_map *map, ot_normal_int_state *state)
{
    int i;
    for (i = 0; i < map->reg_num; i++) {
        *map->clr_int_reg[i] = state->raw_state[i];
    }
    return;
}

static void clear_int(ot_irq_irq_list *irq_node, ot_int_state_info *int_info)
{
    if (irq_node->map_info.type == OT_INT_VICAP) {
        clear_int_for_vicap(&irq_node->map_info.vicap, &int_info->vicap);
    } else if (irq_node->map_info.type == OT_INT_AIO) {
        clear_int_for_aio(&irq_node->map_info.aio, &int_info->aio);
    } else if (irq_node->map_info.type == OT_INT_SCD) {
        clear_int_for_scd(&irq_node->map_info.normal);
    } else if (irq_node->map_info.type == OT_INT_VDH) {
        clear_int_for_vdh(&irq_node->map_info.normal, &int_info->normal);
    } else if (irq_node->map_info.type == OT_INT_NNIE) {
        clear_int_for_nnie(&irq_node->map_info.normal, &int_info->normal);
    } else {
        clear_int_for_normal(&irq_node->map_info.normal, &int_info->normal);
    }
    return;
}

static void save_int_status_for_aio(ot_aio_int_state *save, ot_aio_int_state *state)
{
    int i;

    save->aio_int_status_state |= state->aio_int_status_state;
    save->aio_int_raw_state |= state->aio_int_raw_state;
    for (i = 0; i < OT_AO_DEV_MAX_NUM; i++) {
        save->tx_raw_state[i] |= state->tx_raw_state[i];
        save->tx_stat_state[i] |= state->tx_stat_state[i];
    }
    for (i = 0; i < OT_AI_DEV_MAX_NUM; i++) {
        save->rx_raw_state[i] |= state->rx_raw_state[i];
        save->rx_stat_state[i] |= state->rx_stat_state[i];
    }
    return;
}

static void save_int_status_for_vicap(ot_vicap_int_state *save, ot_vicap_int_state *state)
{
    int i;

    save->cap_state |= state->cap_state;
    save->src_state |= state->src_state;
    for (i = 0; i < OT_VI_MAX_DEV_NUM; i++) {
        save->pt_state[i] |= state->pt_state[i];
    }
    for (i = 0; i < OT_VI_MAX_PHYS_PIPE_NUM; i++) {
        save->ch_state[i] |= state->ch_state[i];
    }
    for (i = 0; i < OT_VI_MAX_PHYS_PIPE_NUM; i++) {
        save->isp_fe_state[i] |= state->isp_fe_state[i];
    }
    return;
}

static void save_int_status_for_normal(ot_normal_int_state *save, ot_normal_int_state *state)
{
    int i;

    save->state_num = state->state_num;
    for (i = 0; i < state->state_num; i++) {
        save->raw_state[i] |= state->raw_state[i];
        save->mask_state[i] |= state->mask_state[i];
    }
    return;
}

static void save_int_status(ot_irq_irq_list *irq_node, ot_int_state_info *int_info)
{
    irq_node->int_info.type = int_info->type;
    if (int_info->type == OT_INT_VICAP) {
        save_int_status_for_vicap(&irq_node->int_info.vicap, &int_info->vicap);
    } else if (int_info->type == OT_INT_AIO) {
        save_int_status_for_aio(&irq_node->int_info.aio, &int_info->aio);
    } else {
        save_int_status_for_normal(&irq_node->int_info.normal, &int_info->normal);
    }
}

static int ot_irq_interrupt(int irq, void *dev_id)
{
    ot_irq_irq_list *irq_node = NULL;
    ot_int_state_info int_info = { 0 };
    unsigned long flags;

    irq_node = get_list_node(irq, dev_id);
    if (irq_node == NULL || irq_node->irq_attr.enable_flag != 1) {
        ot_irq_trace("[%s,line:%d]invalid irq state:%d, irq:%d\n", OT_IRQ_PFX, __LINE__, irq_node == NULL, irq);
        return OSAL_IRQ_NONE;
    } else {
        irq_spin_lock(flags);
        irq_node->irq_cnt++;
        read_int_status(irq_node, &int_info);
        osal_isb();
        clear_int(irq_node, &int_info);
        save_int_status(irq_node, &int_info);
        irq_spin_unlock(flags);
        osal_wakeup(&(irq_node->irq_wait));
    }
    return OSAL_IRQ_HANDLED;
}

static int ot_irq_wait_condition_callback(const void *param)
{
    ot_irq_irq_list *irq = (ot_irq_irq_list *)param;
    return (irq->irq_cnt != 0);
}

typedef struct {
    ot_irq_irq_list *irq_node;
    unsigned int enable_flag;
    irq_trig_mode irq_mod;
    irq_wait_mode wait_mode;
    ot_irq_arg *dev;
    int irq_cnt;
} ot_irq_attr_param;

static void ot_irq_set_irq_attr(const ot_irq_attr_param *param)
{
    param->irq_node->irq_attr.enable_flag = param->enable_flag;
    param->irq_node->irq_attr.irq_mod = param->irq_mod;
    param->irq_node->irq_attr.wait_mode = param->wait_mode;
    param->irq_node->irq_attr.dev = param->dev;
    param->irq_node->irq_cnt = param->irq_cnt;
}

int ot_irq_request_or_free_irq(uintptr_t arg, void *private_data)
{
    ot_irq_irq_attr *p = (ot_irq_irq_attr *)arg;
    ot_irq_irq_list *irq_node = NULL;
    ot_irq_irq_list irq_tmp = { 0 };
    int ret;

    if (p == NULL) {
        ot_irq_trace("[%s,line:%d]ot_irq_set_irq_param arg is null\n", OT_IRQ_PFX, __LINE__);
        return -1;
    }

    irq_node = get_list_node(p->irq_num, p->dev);
    if (irq_node == NULL) {
        ot_irq_trace("[%s,line:%d] not find irq_node.\n", OT_IRQ_PFX, __LINE__);
        return -1;
    }

    if (p->enable_flag == irq_node->irq_attr.enable_flag) {
        return -1;
    }

    if (p->enable_flag) { // add irq
        ot_irq_set_irq_attr(&(ot_irq_attr_param) {
            &irq_tmp, irq_node->irq_attr.enable_flag, irq_node->irq_attr.irq_mod,
            irq_node->irq_attr.wait_mode, irq_node->irq_attr.dev, irq_node->irq_cnt
        });
        (void)memcpy_s(irq_node->irq_attr.irq_name, MAX_IRQ_NAME_LEN, p->irq_name, MAX_IRQ_NAME_LEN);
        ot_irq_set_irq_attr(&(ot_irq_attr_param) {
            irq_node, p->enable_flag, p->irq_mod, p->wait_mode, p->dev, 0
        });
        ret = osal_request_irq(p->irq_num, ot_irq_interrupt, NULL, irq_node->irq_attr.irq_name, p->dev);
        if (ret != 0) {
            ot_irq_trace("[%s,line:%d]ot_irq: failed to register (%s),irq_num:%d\n", OT_IRQ_PFX, __LINE__, p->irq_name,
                p->irq_num);
            ot_irq_set_irq_attr(&(ot_irq_attr_param) {
                irq_node, irq_tmp.irq_attr.enable_flag, irq_tmp.irq_attr.irq_mod,
                irq_tmp.irq_attr.wait_mode, irq_tmp.irq_attr.dev, irq_tmp.irq_cnt
            });
            ret = -1;
            return ret;
        }
    }
    if (!p->enable_flag) { // free irq
        ot_irq_trace("ot_irq:disable irq_num:%d\n", p->irq_num);
        irq_node->irq_cnt = -1;
        osal_wakeup(&(irq_node->irq_wait));
        osal_free_irq(p->irq_num, p->dev);
        ot_irq_set_irq_attr(&(ot_irq_attr_param) {
            irq_node, p->enable_flag, p->irq_mod, p->wait_mode, p->dev, -1
        });
    }

    return 0;
}

int ot_irq_wait_irq(uintptr_t arg, void *private_data)
{
    ot_irq_irq_attr *para = (ot_irq_irq_attr *)arg;
    ot_irq_irq_list *irq_node = NULL;
    ot_irq_arg user_dev = { 0 };
    unsigned long flags;
    int ret;

    if (para == NULL) {
        ot_irq_trace("[%s,line:%d] error invalid arg\n", OT_IRQ_PFX, __LINE__);
        return -1;
    }
    osal_copy_from_user(&user_dev, para->dev, sizeof(ot_irq_arg));
    irq_node = get_list_node(para->irq_num, para->dev);
    if (irq_node == NULL || irq_node->irq_attr.enable_flag == 0) {
        ot_irq_trace("[%s,line:%d]irq_num:%d not enable\n", OT_IRQ_PFX, __LINE__, para->irq_num);
        return -1;
    }
retry:
    if (irq_node->irq_cnt == 0) {
        ret = osal_wait_event_interruptible(&(irq_node->irq_wait), ot_irq_wait_condition_callback, irq_node);
        if (ret != 0) {
            ot_irq_trace("[%s,line:%d]osal_wait_event_interruptible return error:%d, irq: %d\n", OT_IRQ_PFX, __LINE__,
                ret, para->irq_num);
            return ret;
        }
        if (irq_node->irq_cnt < 0) {
            return -1;
        }
        if (irq_node->irq_attr.wait_mode == IRQ_WAIT_FOREVER) { // wait forever
            goto retry;
        }
    } else {
        if (irq_node->irq_cnt < 0) {
            return -1;
        }
    }
    irq_spin_lock(flags);
    if (irq_node->irq_attr.irq_mod == IRQ_TRIG_ONECE) { // clear irqcnt for trigger mode 0
        irq_node->irq_cnt = 0;
    } else {
        irq_node->irq_cnt--;
    }
    (void)memcpy_s(&user_dev.int_info, sizeof(ot_int_state_info), &irq_node->int_info, sizeof(ot_int_state_info));
    (void)memset_s(&irq_node->int_info, sizeof(ot_int_state_info), 0, sizeof(ot_int_state_info));
    irq_spin_unlock(flags);

    osal_copy_to_user(para->dev, &user_dev, sizeof(ot_irq_arg));

    return 0;
}

static struct osal_dev *g_ot_irq_dev = NULL;
static struct platform_device *g_ot_irq_pdev = NULL;
int ot_irq_get_irq_num(uintptr_t arg, void *private_data)
{
    ot_irq_irq_attr *para = (ot_irq_irq_attr *)arg;

    if (para == NULL) {
        ot_irq_trace("[%s,line:%d] error invalid arg\n", OT_IRQ_PFX, __LINE__);
        return -1;
    }
    if (para->irq_name[0] != 0) {
        return platform_get_irq_byname(g_ot_irq_pdev, para->irq_name);
    }
    return platform_get_irq(g_ot_irq_pdev, para->irq_num);
}

static int ot_irq_map_irq_reg_for_aio(ot_irq_aio_reg_map *map, ot_aio_int_reg *reg)
{
    int i;

    map->aio_int_status_reg = osal_ioremap(reg->aio_int_status_reg, SINGLE_REG_SIZE);
    map->aio_int_raw_reg = osal_ioremap(reg->aio_int_raw_reg, SINGLE_REG_SIZE);
    if ((map->aio_int_status_reg == NULL) || (map->aio_int_raw_reg == NULL)) {
        ot_irq_trace("[%s,line:%d] osal_ioremap failed\n", OT_IRQ_PFX, __LINE__);
        goto fail;
    }

    for (i = 0; i < OT_AO_DEV_MAX_NUM; i++) {
        map->tx_clr_reg[i] = osal_ioremap(reg->tx_clr_reg[i], SINGLE_REG_SIZE);
        map->tx_raw_reg[i] = osal_ioremap(reg->tx_raw_reg[i], SINGLE_REG_SIZE);
        map->tx_stat_reg[i] = osal_ioremap(reg->tx_stat_reg[i], SINGLE_REG_SIZE);
        if ((map->tx_clr_reg[i] == NULL) || (map->tx_raw_reg[i] == NULL) || (map->tx_stat_reg[i] == NULL)) {
            ot_irq_trace("[%s,line:%d] osal_ioremap failed\n", OT_IRQ_PFX, __LINE__);
            goto fail;
        }
    }
    for (i = 0; i < OT_AI_DEV_MAX_NUM; i++) {
        map->rx_clr_reg[i] = osal_ioremap(reg->rx_clr_reg[i], SINGLE_REG_SIZE);
        map->rx_raw_reg[i] = osal_ioremap(reg->rx_raw_reg[i], SINGLE_REG_SIZE);
        map->rx_stat_reg[i] = osal_ioremap(reg->rx_stat_reg[i], SINGLE_REG_SIZE);
        if ((map->rx_clr_reg[i] == NULL) || (map->rx_raw_reg[i] == NULL) || (map->rx_stat_reg[i] == NULL)) {
            ot_irq_trace("[%s,line:%d] osal_ioremap failed\n", OT_IRQ_PFX, __LINE__);
            goto fail;
        }
    }
    return 0;

fail:
    unmap_int_reg_for_aio(map);
    return -1;
}


static int ot_irq_map_irq_reg_for_vicap(ot_irq_vicap_reg_map *map, ot_vicap_int_reg *reg)
{
    int i;

    map->cap_int_reg = osal_ioremap(reg->cap_reg, SINGLE_REG_SIZE);
    map->src_int_reg = osal_ioremap(reg->src_reg, SINGLE_REG_SIZE);
    for (i = 0; i < OT_VI_MAX_DEV_NUM; i++) {
        map->pt_int_reg[i] = osal_ioremap(reg->pt_reg[i], SINGLE_REG_SIZE);
        if (map->pt_int_reg[i] == NULL) {
            ot_irq_trace("[%s,line:%d] osal_ioremap failed\n", OT_IRQ_PFX, __LINE__);
            goto fail;
        }
    }
    for (i = 0; i < OT_VI_MAX_PHYS_PIPE_NUM; i++) {
        map->ch_int_reg[i] = osal_ioremap(reg->ch_reg[i], SINGLE_REG_SIZE);
        if (map->ch_int_reg[i] == NULL) {
            ot_irq_trace("[%s,line:%d] osal_ioremap failed\n", OT_IRQ_PFX, __LINE__);
            goto fail;
        }
    }
    for (i = 0; i < OT_VI_MAX_PHYS_PIPE_NUM; i++) {
        map->isp_fe_int_reg[i] = osal_ioremap(reg->isp_fe_reg[i], SINGLE_REG_SIZE);
        if (map->isp_fe_int_reg[i] == NULL) {
            ot_irq_trace("[%s,line:%d] osal_ioremap failed\n", OT_IRQ_PFX, __LINE__);
            goto fail;
        }
    }
    return 0;

fail:
    unmap_int_reg_for_vicap(map);
    return -1;
}

static int ot_irq_map_irq_reg_for_normal(ot_irq_normal_reg_map *map, ot_normal_int_reg *reg)
{
    int i;

    if (reg->reg_num > OT_MAX_STATE_NUM) {
        ot_irq_trace("[%s,line:%d] error invalid reg_num:%d\n", OT_IRQ_PFX, __LINE__, reg->reg_num);
        return -1;
    }

    for (i = 0; i < reg->reg_num; i++) {
        map->raw_int_reg[i] = osal_ioremap(reg->raw_int_reg[i], SINGLE_REG_SIZE);
        if (map->raw_int_reg[i] == NULL) {
            ot_irq_trace("[%s,line:%d] osal_ioremap failed\n", OT_IRQ_PFX, __LINE__);
            goto fail;
        }

        map->mask_int_reg[i] = osal_ioremap(reg->mask_int_reg[i], SINGLE_REG_SIZE);
        if (map->mask_int_reg[i] == NULL) {
            ot_irq_trace("[%s,line:%d] osal_ioremap failed\n", OT_IRQ_PFX, __LINE__);
            goto fail;
        }

        map->clr_int_reg[i] = osal_ioremap(reg->clr_int_reg[i], SINGLE_REG_SIZE);
        if (map->clr_int_reg[i] == NULL) {
            ot_irq_trace("[%s,line:%d] osal_ioremap failed\n", OT_IRQ_PFX, __LINE__);
            goto fail;
        }
    }

    map->reg_num = reg->reg_num;
    return 0;

fail:
    unmap_int_reg_for_normal(map);
    return -1;
}

int ot_irq_set_irq_reg(uintptr_t arg, void *private_data)
{
    ot_irq_set_irq_reg_info *para = (ot_irq_set_irq_reg_info *)arg;
    ot_irq_irq_list *irq_node = NULL;
    ot_irq_irq_attr irqattr = { 0 };
    int ret;

    if (para == NULL) {
        ot_irq_trace("[%s,line:%d] error invalid arg\n", OT_IRQ_PFX, __LINE__);
        return -1;
    }

    irq_node = get_list_node(para->irq_num, 0);
    if (irq_node == NULL) {
        if (node_cnt >= MAX_NODE_LIMIT) {
            ot_irq_trace("ot_irq: node_cnt out of limit!\n");
            return -1;
        }
        irqattr.irq_num = para->irq_num;
        irq_node = add_list(&irqattr);
        if (irq_node == NULL) {
            ot_irq_trace("ot_irq: add irq node failed!\n");
            return -1;
        }
    }

    irq_node->map_info.type = para->reg_info.type;
    if (para->reg_info.type == OT_INT_VICAP) {
        ret = ot_irq_map_irq_reg_for_vicap(&irq_node->map_info.vicap, &para->reg_info.vicap);
    } else if (para->reg_info.type == OT_INT_AIO) {
        ret = ot_irq_map_irq_reg_for_aio(&irq_node->map_info.aio, &para->reg_info.aio);
    } else {
        ret = ot_irq_map_irq_reg_for_normal(&irq_node->map_info.normal, &para->reg_info.normal);
    }

    return ret;
}

static long ot_irq_ioctl(unsigned int cmd, unsigned long para, void *private_data)
{
    uintptr_t arg = (uintptr_t)para;
    switch (cmd) {
        case IRQ_REQUEST_OR_FREE_IRQ_CTRL:
            return ot_irq_request_or_free_irq(arg, private_data);

        case IRQ_WAIT_IRQ_CTRL:
            return ot_irq_wait_irq(arg, private_data);

        case IRQ_IOC_GET_IRQ_NUM_CTRL:
            return ot_irq_get_irq_num(arg, private_data);

        case IRQ_IOC_SET_IRQ_REG_CTRL:
            return ot_irq_set_irq_reg(arg, private_data);

        default:
            ot_irq_trace("[%s,line:%d]Error: Inappropriate ioctl for device. cmd=%u\n", OT_IRQ_PFX, __LINE__, cmd);
            return -1;
    }
    return 0;
}
static int irq_mmap(osal_vm_t *vm, unsigned long start, unsigned long end, unsigned long vm_pgoff, void *private_data)
{
    int size;
    unsigned long long phy_addr;

    if (start > end) {
        return -1;
    }

    size = (end - start) / 2; // 2

    /* it's necessary for the variable "size" to align 4k(page_size). */
#define PAGE_SIZE_MASK 0xfffff000
    if ((unsigned int)size & (~PAGE_SIZE_MASK)) {
        return -1;
    }
#undef PAGE_SIZE_MASK

    phy_addr = (unsigned long long)(vm_pgoff << OT_IRQ_PAGE_SHIFT);
    if (cmpi_check_mmz_phy_addr(phy_addr, size) != 0) {
        ot_irq_trace("addr: %#llx, size: %d, invalid phyaddr!\n", phy_addr, size);
        return -1;
    }

    osal_pgprot_writecombine(vm);

    if (osal_remap_pfn_range(vm, start, vm_pgoff, size)) {
        return -1;
    }
    if (osal_remap_pfn_range(vm, start + size, vm_pgoff, size)) {
        return -1;
    }

    return 0;
}

static int ot_irq_open(void *private_data)
{
    ot_irq_trace("Enter ot_irq_open\n");
    return 0;
}


static void free_list(void)
{
    ot_irq_irq_list *tmp = NULL;

    while (head != NULL) {
        tmp = head->next;

        if (head->irq_attr.enable_flag) {
            ot_irq_trace("unreg irq:%d, %s\n", head->irq_attr.irq_num, head->irq_attr.irq_name);
            osal_wait_destroy(&(head->irq_wait));
            unmap_int_reg(&(head->map_info));
            osal_free_irq(head->irq_attr.irq_num, head->irq_attr.dev);
        }

        osal_kfree(head);
        head = tmp;
    }
}

static int ot_irq_release(void *private_data)
{
    ot_irq_trace("Enter ot_irq_release.\n");
    free_list();
    return 0;
}

static struct osal_fileops g_ot_irq_fops = {
    open :           ot_irq_open,
    unlocked_ioctl : ot_irq_ioctl,
    mmap :           irq_mmap,
    release :        ot_irq_release,
};

int ot_irq_init(struct platform_device *pdev)
{
    osal_spin_lock_init(&g_irq_spin_lock);
    g_ot_irq_dev = osal_createdev(OT_IRQ_DEVICE_NAME);
    if (g_ot_irq_dev == NULL) {
        ot_irq_trace("[%s,line:%d]Error: can't create dev\n", OT_IRQ_PFX, __LINE__);
        return -1;
    }

    g_ot_irq_dev->minor = 255; /* define a macro to substitute 255 */
    g_ot_irq_dev->fops = &g_ot_irq_fops;
    if (osal_registerdevice(g_ot_irq_dev) != 0) {
        ot_irq_trace("[%s,line:%d]Error: can't register\n", OT_IRQ_PFX, __LINE__);
        osal_destroydev(g_ot_irq_dev);
        return -1;
    }
    g_ot_irq_pdev = pdev;

    ot_irq_trace("ot_irq init ok. ver=%s, %s.\n", __DATE__, __TIME__);
    return 0;
}

void ot_irq_exit(void)
{
    del_list();
    osal_deregisterdevice(g_ot_irq_dev);
    osal_destroydev(g_ot_irq_dev);
    g_ot_irq_pdev = NULL;
    osal_spin_lock_destroy(&g_irq_spin_lock);

    ot_irq_trace("ot_irq exit ok.\n");
}
