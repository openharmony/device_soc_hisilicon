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

#ifndef KLAD_REG_DEFINE_H
#define KLAD_REG_DEFINE_H

/* Define the union kl_key_addr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_addr : 10;   /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_key_addr;

/* Define the union kl_key_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_cfg_rsv_0 : 2; /* [1..0]  */
        unsigned int reserved_0 : 2;   /* [3..2]  */
        unsigned int dsc_code : 8;     /* [11..4]  */
        unsigned int reserved_1 : 4;   /* [15..12]  */
        unsigned int kl_cfg_rsv_1 : 1; /* [16]  */
        unsigned int kl_cfg_rsv_2 : 1; /* [17]  */
        unsigned int reserved_2 : 14;  /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_key_cfg;

/* Define the union kl_key_sec_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_sec : 1;     /* [0]  */
        unsigned int src_nsec : 1;    /* [1]  */
        unsigned int src_sec : 1;     /* [2]  */
        unsigned int dest_nsec : 1;   /* [3]  */
        unsigned int dest_sec : 1;    /* [4]  */
        unsigned int reserved_0 : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_key_sec_cfg;

/* Define the union kl_state */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_busy : 2;     /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_state;

/* Define the union kl_crc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_crc : 8;      /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_crc;

/* Define the union kl_error */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int level_sel_err : 1; /* [0]  */
        unsigned int algo_sel_err : 1;  /* [1]  */
        unsigned int reserved_0 : 1;    /* [2]  */
        unsigned int dsc_code_err : 1;  /* [3]  */
        unsigned int reserved_1 : 2;    /* [5..4]  */
        unsigned int key_size_err : 1;  /* [6]  */
        unsigned int rk_busy_err : 1;   /* [7]  */
        unsigned int reserved_2 : 4;    /* [11..8]  */
        unsigned int rk_rdy_err : 1;    /* [12]  */
        unsigned int lv1_rdy_err : 1;   /* [13]  */
        unsigned int lv2_rdy_err : 1;   /* [14]  */
        unsigned int reserved_3 : 17;   /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_error;

/* Define the union kc_error */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 2;     /* [1..0]  */
        unsigned int tee_access_err : 1; /* [2]  */
        unsigned int ree_access_err : 1; /* [3]  */
        unsigned int reserved_1 : 10;    /* [13..4]  */
        unsigned int sm4_dis_err : 1;    /* [14]  */
        unsigned int reserved_2 : 6;     /* [20..15]  */
        unsigned int send_time_out : 1;  /* [21]  */
        unsigned int reserved_3 : 10;    /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kc_error;

/* Define the union kl_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_int_en : 1;      /* [0]  */
        unsigned int kl_lock_int_en : 1; /* [1]  */
        unsigned int reserved_0 : 30;    /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_int_en;

/* Define the union kl_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_int_raw : 1;          /* [0]  */
        unsigned int reserved_0 : 3;          /* [3..1]  */
        unsigned int kl_int_num : 5;          /* [8..4]  */
        unsigned int reserved_1 : 3;          /* [11..9]  */
        unsigned int kl_com_lock_int_raw : 1; /* [12]  */
        unsigned int reserved_2 : 19;         /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_int_raw;

/* Define the union kl_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_int : 1;      /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_int;

/* Define the union kl_power_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_power_en : 4; /* [3..0]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_power_en;

/* Define the union kl_power_en_lock */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_power_en_lock : 1; /* [0]  */
        unsigned int reserved_0 : 31;      /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_power_en_lock;

/* Define the union kl_rk_gen_status */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_rk_gen_busy : 9; /* [8..0]  */
        unsigned int reserved_0 : 23;    /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_rk_gen_status;

/* Define the union kl_lock_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_lock : 1;       /* [0]  */
        unsigned int reserved_0 : 3;    /* [3..1]  */
        unsigned int kl_lock_rsv_0 : 3; /* [6..4]  */
        unsigned int reserved_1 : 25;   /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_lock_ctrl;

/* Define the union kl_unlock_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_unlock : 1;       /* [0]  */
        unsigned int reserved_0 : 3;      /* [3..1]  */
        unsigned int kl_unlock_rsv_0 : 3; /* [6..4]  */
        unsigned int reserved_1 : 1;      /* [7]  */
        unsigned int kl_unlock_rsv_1 : 3; /* [10..8]  */
        unsigned int reserved_2 : 21;     /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_unlock_ctrl;

/* Define the union kl_com_lock_info */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_com_lock_busy : 2;   /* [1..0]  */
        unsigned int kl_com_lock_fail : 2;   /* [3..2]  */
        unsigned int kl_com_unlock_fail : 2; /* [5..4]  */
        unsigned int reserved_0 : 2;         /* [7..6]  */
        unsigned int reserved_1 : 3;         /* [10..8]  */
        unsigned int reserved_2 : 21;        /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_com_lock_info;

/* Define the union kl_com_lock_status */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_com_lock_stat : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;      /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_com_lock_status;

/* Define the union kl_com_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_com_start : 1;     /* [0]  */
        unsigned int kl_com_level_sel : 3; /* [3..1]  */
        unsigned int kl_com_alg_sel : 2;   /* [5..4]  */
        unsigned int reserved_0 : 2;       /* [7..6]  */
        unsigned int kl_com_key_size : 1;  /* [8]  */
        unsigned int kl_com_rsv_1 : 1;     /* [9]  */
        unsigned int reserved_1 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_com_ctrl;

/* Define the union kl_com_status */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_com_rk_rdy : 1;  /* [0]  */
        unsigned int kl_com_lv1_rdy : 1; /* [1]  */
        unsigned int kl_com_lv2_rdy : 1; /* [2]  */
        unsigned int kl_com_lv3_rdy : 1; /* [3]  */
        unsigned int reserved_0 : 28;    /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_com_status;

/* Define the union kl_clr_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int kl_clr_start : 1;    /* [0]  */
        unsigned int reserved_0 : 1;      /* [1]  */
        unsigned int kl_clr_key_size : 2; /* [3..2]  */
        unsigned int reserved_1 : 28;     /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_clr_ctrl;

/* Define the union kl_alarm_info */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rng_crc4_alarm : 1;        /* [0]  */
        unsigned int kl_cfg_sig_alarm : 1;      /* [1]  */
        unsigned int kl_rk_tag_sig_alarm : 1;   /* [2]  */
        unsigned int kl_rk_tag_crc16_alarm : 1; /* [3]  */
        unsigned int kl_rk_info_crc4_alarm : 1; /* [4]  */
        unsigned int kl_sel_sig_alarm : 1;      /* [5]  */
        unsigned int kl_com_crc16_alarm : 1;    /* [6]  */
        unsigned int reserved_0 : 17;           /* [23..7]  */
        unsigned int cm_core_alarm : 1;         /* [24]  */
        unsigned int reserved_1 : 7;            /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} kl_alarm_info;

#endif /* KLAD_REG_DEFINE_H */
