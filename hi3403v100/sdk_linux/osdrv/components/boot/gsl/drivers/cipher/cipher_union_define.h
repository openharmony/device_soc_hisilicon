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

#ifndef __CIPHER_UNION_DEFINE_H__
#define __CIPHER_UNION_DEFINE_H__

/* Define the union u_in_sym_out_ctrl_fun */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t sym_dma_copy : 2; /* [1..0] */
        uint32_t reserved : 31;    /* [31..2] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} in_sym_out_ctrl;

/* Define the union u_out_sym_chna_int */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t out_sym_chan_int : 16; /* [15..1] */
        uint32_t reserved_1 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} out_sym_chan_int;

/* Define the union out_sym_chn_node_wr_point */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t sym_chn_node_wr_point : 8; /* [7..0] */
        uint32_t reserved_1 : 24;           /* [31..8] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} in_sym_chn_node_wr_point;

/* Define the union u_out_sym_chn_node_wr_point */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t out_sym_chn_node_wr_point : 8; /* [7..0] */
        uint32_t reserved_1 : 24;               /* [31..8] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} out_sym_chn_node_wr_point;

/* Define the union in_sym_chn_key_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t sym_key_chn_id : 9;     /* [8..0] */
        uint32_t reserved : 7;           /* [16..9] */
        uint32_t sym_alg_sel : 4;        /* [19..16] */
        uint32_t sym_alg_mode : 4;       /* [23..20] */
        uint32_t sym_alg_key_len : 2;    /* [25..24] */
        uint32_t sym_alg_data_width : 2; /* [27..26] */
        uint32_t sym_alg_decrypt : 1;    /* [28] */
        uint32_t reserved_2 : 3;         /* [31..29] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} in_sym_chn_key_ctrl;

/* Define the union u_in_sym_chn_node_length_fun */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t sym_chn_node_length : 8; /* [7..0] */
        uint32_t reserved_1 : 24;         /* [31..8] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} in_sym_chn_node_length;

/* Define the union u_out_sym_chn_node_length */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t out_sym_chn_node_length : 8; /* [7..0] */
        uint32_t reserved_1 : 24;             /* [31..8] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} out_sym_chn_node_length;

/* Define the union u_in_hash_chn_node_length_fun */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hash_chn_node_length : 8; /* [7..0] */
        unsigned int reserved_0 : 24;          /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} in_hash_chn_node_length;

/* Define the union u_in_hash_chn_node_wr_point_fun */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hash_chn_node_wr_point : 8; /* [7..0] */
        unsigned int reserved_0 : 24;            /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} in_hash_chn_node_wr_point;

/* Define the union u_in_hash_chn_node_rd_point_fun */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hash_chn_node_rd_point : 8; /* [7..0] */
        unsigned int reserved_0 : 24;            /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} in_hash_chn_node_rd_point;

/* Define the union u_out_sym_chn_node_rd_point */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t sym_chn_node_rd_point : 8; /* [7..0] */
        uint32_t reserved_1 : 24;           /* [31..8] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} in_sym_chn_node_rd_point;

/* Define the union u_out_sym_chn_node_rd_point */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t out_sym_chn_node_rd_point : 8; /* [7..0] */
        uint32_t reserved_1 : 24;               /* [31..8] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} out_sym_chn_node_rd_point;

/* Define the union u_in_sym_chn_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t sym_chn_mmu_en : 1; /* [0] */
        uint32_t reserved : 7;       /* [7..1] */
        uint32_t sym_chn_ss : 4;     /* [11:8] */
        uint32_t sym_chn_ds : 4;     /* [15:12] */
        uint32_t reserved_3 : 15;    /* [30..16] */
        uint32_t sym_chn_en : 1;     /* [31] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} in_sym_chn_ctrl;

/* Define the union u_in_hash_chn_ctrl_fun */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hash_chn_mmu_en : 1; /* [0] */
        unsigned int reserved_0 : 7;      /* [7..1] */
        unsigned int hash_chn_ss : 1;     /* [8] */
        unsigned int reserved_1 : 22;     /* [30..9] */
        unsigned int hash_chn_en : 1;     /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} in_hash_chn_ctrl;

/* Define the union u_spacc_sym_chn_clear_req */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sym_chn_clear_req : 16; /* [15..0] */
        unsigned int reserved_0 : 16;        /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} spacc_sym_chn_clear_req;

/* Define the union u_spacc_int_raw_sym_clear_finish */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_raw_sym_clear_finish : 16; /* [15..0] */
        unsigned int reserved_0 : 16;               /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} spacc_int_raw_sym_clear_finish;

/* Define the union u_spacc_hash_chn_clear_req */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hash_chn_clear_req : 16; /* [15..0] */
        unsigned int reserved_0 : 16;         /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} spacc_hash_chn_clear_req;

/* Define the union u_spacc_int_raw_hash_clear_finish */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_raw_hash_clear_finish : 16; /* [15..0] */
        unsigned int reserved_0 : 16;                /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} spacc_int_raw_hash_clear_finish;

/* Define the union u_hash_chann_raw_int */
typedef union {
    /* Define the struct bits */
    struct {
        uint32_t hash_chann_raw_int : 16; /* [15..0] */
        uint32_t reserved_0 : 16;         /* [31..16] */
    } bits;

    /* Define an unsigned member */
    uint32_t u32;
} hash_chann_raw_int;

/* Define the union u_in_hash_chn_key_ctrl_fun */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 16;       /* [15..0] */
        unsigned int hash_chn_alg_sel : 4;  /* [19..16] */
        unsigned int hash_chn_alg_mode : 4; /* [23..20] */
        unsigned int reserved_1 : 8;        /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} in_hash_chn_key_ctrl;

#endif /* __C_UNION_DEFINE_H__ */
