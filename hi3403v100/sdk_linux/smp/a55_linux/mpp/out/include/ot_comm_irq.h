/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 */
#ifndef __OT_COMM_IRQ_H__
#define __OT_COMM_IRQ_H__
#include "ot_defines.h"

/* For IRQ */
#define OT_MAX_STATE_NUM       2

typedef enum {
    OT_INT_NORMAL = 0,
    OT_INT_VICAP  = 1,
    OT_INT_SCD    = 2,
    OT_INT_VDH    = 3,
    OT_INT_NNIE   = 4,
    OT_INT_AIO    = 5,
    OT_INT_BUTT,
} ot_int_type;

typedef struct {
    volatile unsigned int state_num;                    /* Number of interrupt states */
    volatile unsigned int raw_state[OT_MAX_STATE_NUM];  /* raw interrupt states */
    volatile unsigned int mask_state[OT_MAX_STATE_NUM]; /* interrupt states after mask */
} ot_normal_int_state;

typedef struct {
    volatile unsigned int cap_state;
    volatile unsigned int src_state;
    volatile unsigned int pt_state[OT_VI_MAX_DEV_NUM];
    volatile unsigned int ch_state[OT_VI_MAX_PHYS_PIPE_NUM];
    volatile unsigned int isp_fe_state[OT_VI_MAX_PHYS_PIPE_NUM];
} ot_vicap_int_state;

typedef struct {
    unsigned int aio_int_status_state;
    unsigned int aio_int_raw_state;
    unsigned int tx_raw_state[OT_AO_DEV_MAX_NUM];
    unsigned int tx_stat_state[OT_AO_DEV_MAX_NUM];
    unsigned int rx_raw_state[OT_AI_DEV_MAX_NUM];
    unsigned int rx_stat_state[OT_AI_DEV_MAX_NUM];
} ot_aio_int_state;

typedef struct {
    ot_int_type type;
    union {
        ot_normal_int_state normal;
        ot_vicap_int_state  vicap;
        ot_aio_int_state    aio;
    };
} ot_int_state_info;

typedef struct {
    td_void *dev;
    ot_int_state_info int_info;
} ot_irq_arg;

typedef struct {
    unsigned int cap_reg;
    unsigned int src_reg;
    unsigned int pt_reg[OT_VI_MAX_DEV_NUM];
    unsigned int ch_reg[OT_VI_MAX_PHYS_PIPE_NUM];
    unsigned int isp_fe_reg[OT_VI_MAX_PHYS_PIPE_NUM];
} ot_vicap_int_reg;

typedef struct {
    unsigned int aio_int_status_reg;
    unsigned int aio_int_raw_reg;
    unsigned int tx_clr_reg[OT_AO_DEV_MAX_NUM];
    unsigned int tx_raw_reg[OT_AO_DEV_MAX_NUM];
    unsigned int tx_stat_reg[OT_AO_DEV_MAX_NUM];
    unsigned int rx_clr_reg[OT_AI_DEV_MAX_NUM];
    unsigned int rx_raw_reg[OT_AI_DEV_MAX_NUM];
    unsigned int rx_stat_reg[OT_AI_DEV_MAX_NUM];
} ot_aio_int_reg;


typedef struct {
    unsigned int reg_num;
    unsigned int raw_int_reg[OT_MAX_STATE_NUM];   /* raw int state */
    unsigned int mask_int_reg[OT_MAX_STATE_NUM];  /* int state after mask */
    unsigned int clr_int_reg[OT_MAX_STATE_NUM];
} ot_normal_int_reg;

typedef struct {
    ot_int_type type;
    union {
        ot_normal_int_reg normal;
        ot_vicap_int_reg  vicap;
        ot_aio_int_reg    aio;
    };
} td_int_reg_info;

#endif /* end of #ifndef __OT_COMM_IRQ_H__ */
