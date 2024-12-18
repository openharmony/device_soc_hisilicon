/**
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
 *
 * Description: hal trng register header. \n
 *
 * History: \n
 * 2023-03-22, Create file. \n
 */
#ifndef HAL_TRNG_REG_H
#define HAL_TRNG_REG_H

#include "crypto_type.h"

/*! \Define the offset of TRNG reg */
#define HISC_COM_TRNG_FIFO_DATA             (0x100)
#define HISC_COM_TRNG_FIFO_READY            (0x104)
#define HISC_COM_TRNG_DATA_ST               (0x108)
#define HISC_COM_TRNG_FRO_SAMPLE_CLK_SEL    (0x174)
#define HISC_COM_TRNG_FRO_DIV_CNT           (0x178)
#define TRNG_SEC_CFB                        (0x52001600)

#define TRNG_DONE                           1
#define TRNG_DATA_READY                     1
#define TRNG_INNER_FRO_CLK                  0x1
#define TRNG_INNER_FRO_DIV_CNT              0x1b
#define TRNG_RING_ENABLE                    1
#define TRNG_RING_DISABLE                   0

/* Define the union hisc_com_trng_fifo_ready */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32    trng_data_ready             :  1;  /* [0]  */
        td_u32    trng_done                   :  1;  /* [1]  */
        td_u32    reserved_0                  :  30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    td_u32    u32;
} hisc_com_trng_fifo_ready;

/* Define the union U_HISC_COM_TRNG_FRO_SAMPLE_CLK_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32  fro_sample_clk_sel  : 1;    /* [0] */
        td_u32  reserved_0          : 31;    /* [31..1] */
    } bits;

    /* Define an unsigned member */
    td_u32  u32;
} hisc_com_trng_fro_sample_clk_sel;


/* Define the union U_HISC_COM_TRNG_FRO_DIV_CNT */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32  fro_div_cnt     : 8; /* [7..0] */
        td_u32  reserved_0      : 24;   /* [31..8] */
    } bits;

    /* Define an unsigned member */
    td_u32  u32;
} hisc_com_trng_fro_div_cnt;

/* Define the union TRNG_SEC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        td_u16  trng_ring_en     : 1;   /* [0] */
        td_u16  reserved_0       : 15;  /* [15..1] */
    } bits;

    /* Define an unsigned member */
    td_u16  u16;
} trng_sec_cfg;
#endif
