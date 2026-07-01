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

#ifndef SECURITY_SUBSYS_CIPHER_V3_SRC_DRV_DRIVERS_CORE_DRV_SYMC_V300_H_BODY_PART01_H_
#define SECURITY_SUBSYS_CIPHER_V3_SRC_DRV_DRIVERS_CORE_DRV_SYMC_V300_H_BODY_PART01_H_


#include "drv_osal_lib.h"

/*************************** Internal Structure Definition ****************************/
/** \addtogroup      cipher drivers */
/** @{ */ /** <!-- [cipher] */

/* ! SYMC in entry list size */
#define SYMC_IN_NODE_SIZE (64 * SYMC_MAX_LIST_NUM) /* max: 64 * 255 ~ 16K */

#define SYMC_OUT_NODE_SIZE (16 * SYMC_MAX_LIST_NUM) /* max: 16 * 255 ~ 4k */

#define SYMC_NODE_SIZE (SYMC_IN_NODE_SIZE + SYMC_OUT_NODE_SIZE) /* max: 16k + 4k = 20K */

/* ! SYMC in entry list size */
#define SYMC_NODE_LIST_SIZE (SYMC_NODE_SIZE * CIPHER_HARD_CHANNEL_CNT)

/* ! \Define the offset of reg */
#define SPACC_IS                              0x0000
#define SPACC_IE                              0x0004
#define SPACC_SYM_CHN_LOCK                    0x0020
#define SPACC_INT_RAW_SYM_CLEAR_FINISH        0x0008
#define SPACC_EN_INT_RAW_SYM_CLEAR_FINISH     0x000c
#define SPACC_SYM_CHN_CLEAR_REQ               0x0060
#define SPACC_MMU_SEC_TYPE                    0x2000
#define SPACC_MMU_SEC_TLB                     0x2004
#define SPACC_MMU_NOSEC_TYPE                  0x2010
#define SPACC_MMU_NOSEC_TLB                   0x2014
#define SPACC_MMU_IN_SYM_SEC_EADDR_H          0x2100
#define SPACC_MMU_IN_SYM_SEC_EADDR_L          0x2104
#define SPACC_MMU_IN_SYM_NOSEC_EADDR_H        0x2110
#define SPACC_MMU_IN_SYM_NOSEC_EADDR_L        0x2114
#define SPACC_MMU_IN_SYM_TAG_ID_H             0x2120
#define SPACC_MMU_IN_SYM_TAG_ID_L             0x2124
#define SPACC_MMU_IN_SYM_CLEAR                0x2130
#define SPACC_MMU_IN_SYM_SEC_UNVLD_VA_TAB     0x2140
#define SPACC_MMU_IN_SYM_SEC_UNVLD_PA_TAB     0x2144
#define SPACC_MMU_IN_SYM_NOSEC_UNVLD_VA_TAB   0x2148
#define SPACC_MMU_IN_SYM_NOSEC_UNVLD_PA_TAB   0x214c
#define SPACC_MMU_OUT_SYM_SEC_EADDR_H         0x2500
#define SPACC_MMU_OUT_SYM_SEC_EADDR_L         0x2504
#define SPACC_MMU_OUT_SYM_NOSEC_EADDR_H       0x2510
#define SPACC_MMU_OUT_SYM_NOSEC_EADDR_L       0x2514
#define SPACC_MMU_OUT_SYM_TAG_ID_H            0x2520
#define SPACC_MMU_OUT_SYM_TAG_ID_L            0x2524
#define SPACC_MMU_OUT_SYM_CLEAR               0x2530
#define SPACC_MMU_OUT_SYM_SEC_UNVLD_VA_TAB    0x2540
#define SPACC_MMU_OUT_SYM_SEC_UNVLD_PA_TAB    0x2544
#define SPACC_MMU_OUT_SYM_NOSEC_UNVLD_VA_TAB  0x2548
#define SPACC_MMU_OUT_SYM_NOSEC_UNVLD_PA_TAB  0x254c
#define SPACC_AXI_IRAW                        0x3010

#define in_sym_chn_ctrl(id)                   (0x4000 + (id) * 0x100)
#define in_sym_out_ctrl(id)                   (0x4004 + (id) * 0x100)
#define in_sym_chn_key_ctrl(id)               (0x4010 + (id) * 0x100)
#define in_sym_chn_node_start_addr_h(id)      (0x4020 + (id) * 0x100)
#define in_sym_chn_node_start_addr_l(id)      (0x4024 + (id) * 0x100)
#define in_sym_chn_node_length(id)            (0x402c + (id) * 0x100)
#define in_sym_chn_node_wr_point(id)          (0x4030 + (id) * 0x100)
#define in_sym_chn_node_rd_point(id)          (0x4034 + (id) * 0x100)
#define in_sym_chn_node_ctrl(id)              (0x4040 + (id) * 0x100)
#define dbg_in_sym_chn_rd_addr_h(id)          (0x40a0 + (id) * 0x100)
#define dbg_in_sym_chn_rd_addr_l(id)          (0x40a4 + (id) * 0x100)
#define dbg_in_sym_chn_data_sa_h(id)          (0x40b0 + (id) * 0x100)
#define dbg_in_sym_chn_data_sa_l(id)          (0x40a4 + (id) * 0x100)
#define dbg_in_sym_chn_data_len(id)           (0x40c0 + (id) * 0x100)

#define IN_SYM_GLOBAL_CTRL                    0x6000
#define IRAW_IN_SYM_NODE_CNT                  0x6510
#define IENA_IN_SYM_NODE_CNT                  0x6514
#define ISTA_IN_SYM_NODE_CNT                  0x6518
#define ISTA_IN_HASH_NODE_CNT                 0x6A18
#define SPACC_CALC_INC_CFG                    0x8008
#define SPACC_CALC_CRG_CFG                    0x8030
#define HPP_SYM_CALC_CTRL_CHECK_ERR           0x8058
#define HPP_SYM_CALC_CTRL_CHECK_ERR_STATUS    0x805c
#define TEE_SYM_CALC_CTRL_CHECK_ERR           0x8060
#define TEE_SYM_CALC_CTRL_CHECK_ERR_STATUS    0x8064
#define REE_SYM_CALC_CTRL_CHECK_ERR           0x8070
#define REE_SYM_CALC_CTRL_CHECK_ERR_STATUS    0x8074

#define sym_calc_ctrl_check_err(cpu)        (0x8000 + 0x80 - (cpu) * 0x10)
#define sym_calc_ctrl_check_err_status(cpu) (0x8000 + 0x84 - (cpu) * 0x10)

#define HDCP_DEBUG                            0x8114
#define CIPHER_ROBUST_ALARM_DEBUG             0x8200

#define SYM_CHANN_RAW_INT                     0x8500
#define SYM_CHANN_RAW_INT_EN                  0x8504
#define SYM_CHANN_INT                         0x8508

#define SYM_CHANN_ERROR_RAW_INT               0x8520
#define SYM_CHANN_ERROR_RAW_INT_EN            0x8524
#define SYM_CHANN_ERROR_INT                   0x8528

#define HASH_CHANN_INT                        0x8608

#define chn_n_cipher_ivout(id)             (0x9000 + (id) * 0x80)
#define chn_n_cipher_ccm_gcm_tag(id, word) (0x9010 + (word) * 0x4 + (id) * 0x80)
#define chn_n_cipher_dfa_en(id)            (0x9020 + (id) * 0x80)

#define OUT_SYM_CHAN_RAW_LAST_NODE_INT     0xc000
#define OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN  0xc004
#define OUT_SYM_CHAN_LAST_NODE_INT         0xc008
#define OUT_SYM_CHAN_RAW_LEVEL_INT         0xc010
#define OUT_SYM_CHAN_RAW_LEVEL_INT_EN      0xc014
#define OUT_SYM_CHAN_LEVEL_INT             0xc018

#define out_sym_chn_ctrl(id)                  (0xcf00 + (id) * 0x100)
#define out_sym_chn_status(id)                (0xcf04 + (id) * 0x100)
#define out_sym_chn_node_start_addr_h(id)     (0xcf20 + (id) * 0x100)
#define out_sym_chn_node_start_addr_l(id)     (0xcf24 + (id) * 0x100)
#define out_sym_chn_node_length(id)           (0xcf28 + (id) * 0x100)
#define out_sym_chn_node_wr_point(id)         (0xcf30 + (id) * 0x100)
#define out_sym_chn_node_rd_point(id)         (0xcf34 + (id) * 0x100)
#define out_sym_chn_node_ctrl(id)             (0xcf40 + (id) * 0x100)
#define dbg_out_sym_chn_rd_addr_h(id)         (0xcfa0 + (id) * 0x100)
#define dbg_out_sym_chn_rd_addr_l(id)         (0xcfa4 + (id) * 0x100)
#define dbg_out_sym_chn_node_left_buf_len(id) (0xcfa8 + (id) * 0x100)

#define SYMC_LOW_POWER_ENABLE                  0x01

#define SPACC_CPU_HPP   0x03
#define SPACC_CPU_TEE   0x02
#define SPACC_CPU_REE   0x01
#define SPACC_CPU_IDLE  0x00

#define chn_who_used_clr(used, chn)      (used) &= ~(0x03U << ((chn) * 2U))
#define chn_who_used_set(used, chn, who) (used) |= (who) << ((chn) * 2U)
#define chn_who_used_get(used, chn)      (((used) >> ((chn) * 2U)) & 0x03U)

typedef enum {
    ECB = 0x01,
    CBC = 0x03,
    CTR = 0x06,
    OFB = 0x07,
    CFB = 0x08,
    CCM = 0x09,
    GCM = 0x0A,
} sym_alg_mode;

typedef enum {
    AES = 0x02,
    TDES = 0x07,
    SM4 = 0x05,
} sym_alg_sel;

typedef enum {
    KEY_64BIT = 0,
    KEY_128BIT = 1,
    KEY_192BIT = 2,
    KEY_256BIT = 3,
    KEY_CNT = 4
} sym_alg_key_len;

/* Define the union spacc_is : hpp | tee | ree */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int int_in_ctrl_ree : 1;      /* [0] */
        unsigned int int_in_ctrl_tee : 1;      /* [1] */
        unsigned int int_in_ctrl_hpp : 1;      /* [2] */
        unsigned int int_cal_ctrl_ree : 1;     /* [3] */
        unsigned int int_cal_ctrl_tee : 1;     /* [4] */
        unsigned int int_cal_ctrl_hpp : 1;     /* [5] */
        unsigned int int_out_ctrl_ree : 1;     /* [6] */
        unsigned int int_out_ctrl_tee : 1;     /* [7] */
        unsigned int int_out_ctrl_hpp : 1;     /* [8] */
        unsigned int int_mmu_ree : 1;          /* [9] */
        unsigned int int_mmu_tee : 1;          /* [10] */
        unsigned int int_mmu_hpp : 1;          /* [11] */
        unsigned int int_clear_finish_ree : 1; /* [12] */
        unsigned int int_clear_finish_tee : 1; /* [13] */
        unsigned int int_clear_finish_hpp : 1; /* [14] */
        unsigned int reserved : 17;            /* [31..15] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} spacc_is;

/* Define the union spacc_ie */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int spacc_ie_ree : 1; /* [0] */
        unsigned int reserved_0 : 3;   /* [3..1] */
        unsigned int spacc_ie_tee : 1; /* [4] */
        unsigned int reserved_1 : 3;   /* [7..5] */
        unsigned int spacc_ie_hpp : 1; /* [8] */
        unsigned int reserved_2 : 23;  /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} spacc_ie;

/* Define the union in_sym_out_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        u32 sym_dma_copy : 1; /* [0] */
        u32 reserved : 31;    /* [31..1] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} in_sym_out_ctrl;

/* Define the union in_sym_chn_key_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        u32 sym_key_chn_id : 9;     /* [8..0] */
        u32 reserved : 7;           /* [16..9] */
        u32 sym_alg_sel : 4;        /* [19..16] */
        u32 sym_alg_mode : 4;       /* [23..20] */
        u32 sym_alg_key_len : 2;    /* [25..24] */
        u32 sym_alg_data_width : 2; /* [27..26] */
        u32 sym_alg_decrypt : 1;    /* [28] */
        u32 reserved_2 : 3;         /* [31..29] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} in_sym_chn_key_ctrl;

/* Define the union in_sym_chn_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        u32 sym_chn_mmu_en : 1; /* [0] */
        u32 reserved : 7;       /* [7..1] */
        u32 sym_chn_ss : 4;     /* [8..11] */
        u32 sym_chn_ds : 4;     /* [12..15] */
        u32 reserved_3 : 15;    /* [30..16] */
        u32 sym_chn_en : 1;     /* [31] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} in_sym_chn_ctrl;

/* Define the union out_sym_chn_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        u32 reserved : 8;           /* [7..0] */
        u32 out_sym_chn_mmu_en : 1; /* [8] */
        u32 reserved_2 : 23;        /* [31..9] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chn_ctrl;

/* Define the union out_sym_chan_int */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_chan_int : 16; /* [15..0] */
        u32 reserved_1 : 16;       /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chan_int;

/* Define the union out_sym_chan_raw_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_chan_int_en : 16; /* [15..1] */
        u32 reserved_1 : 16;          /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chan_raw_int_en;

/* Define the union out_sym_chan_raw_int */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_chan_raw_int : 16; /* [15..0] */
        u32 reserved_1 : 16;           /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chan_raw_int;

/* Define the union out_sym_chan_raw_level_int */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_level_chan_raw_int : 16; /* [15..0] */
        u32 reserved_1 : 16;                 /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chan_raw_level_int;

/* Define the union out_sym_chan_raw_level_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_level_chan_int_en : 16; /* [15..0] */
        u32 reserved_1 : 16;                /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chan_raw_level_int_en;

/* Define the union out_sym_chan_level_int */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_level_chan_int : 16; /* [15..1] */
        u32 reserved_1 : 16;             /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chan_level_int;

/* Define the union out_sym_int_enable */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_last_node_int_enable : 1;  /* [0] */
        u32 out_sym_node_empty_int_enable : 1; /* [1] */
        u32 out_sym_node_level_int_enable : 1; /* [2] */
        u32 reserved : 29;                     /* [31..3] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_int_enable;

/* Define the union out_sym_chn_status */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_cur_sym_chn_node_level : 8; /* [7..0] */
        u32 reserved_0 : 8;                 /* [15..8] */
        u32 out_sym_chn_int_status : 2;     /* [16..17] */
        u32 reserved_1 : 14;                /* [31..18] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chn_status;

/* Define the union ista_in_sym_node_cnt */
typedef union {
    /* Define the struct bits */
    struct {
        u32 ista_in_sym_node_cnt : 16; /* [15..1] */
        u32 reserved_1 : 16;           /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} ista_in_sym_node_cnt;

/* Define the union iena_in_sym_node_cnt */
typedef union {
    /* Define the struct bits */
    struct {
        u32 iena_in_sym_node_cnt : 16; /* [15..1] */
        u32 reserved_1 : 16;           /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} iena_in_sym_node_cnt;

/* Define the union out_sym_chan_raw_int */
typedef union {
    /* Define the struct bits */
    struct {
        u32 iraw_in_sym_node_cnt : 16; /* [15..1] */
        u32 reserved_1 : 16;           /* [31..16] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} iraw_in_sym_node_cnt;

/* Define the union out_sym_chn_node_wr_point */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_chn_node_wr_point : 8; /* [7..0] */
        u32 reserved_1 : 24;               /* [31..8] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chn_node_wr_point;

/* Define the union out_sym_chn_node_wr_point */
typedef union {
    /* Define the struct bits */
    struct {
        u32 sym_chn_node_wr_point : 8; /* [7..0] */
        u32 reserved_1 : 24;           /* [31..8] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} in_sym_chn_node_wr_point;

/* Define the union out_sym_chn_node_rd_point */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_chn_node_rd_point : 8; /* [7..0] */
        u32 reserved_1 : 24;               /* [31..8] */
    } bits;

    /* Define an unsigned member */
    u32 u32;
} out_sym_chn_node_rd_point;

/* Define the union out_sym_chn_node_rd_point */
typedef union {


#endif /* SECURITY_SUBSYS_CIPHER_V3_SRC_DRV_DRIVERS_CORE_DRV_SYMC_V300_H_BODY_PART01_H_ */
