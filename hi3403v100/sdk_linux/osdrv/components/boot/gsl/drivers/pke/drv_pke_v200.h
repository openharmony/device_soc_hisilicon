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

#ifndef _DRV_PKE_V200_H_
#define _DRV_PKE_V200_H_

/*! \Define the offset of reg */
#define PKE_VERSION_DATE                 0x0
#define PKE_BUSY                         0x4
#define PKE_WORK_MODE                    0x8
#define PKE_START                        0xc
#define PKE_INT_ENABLE                   0x10
#define PKE_INT_STATUS                   0x14
#define PKE_INT_NOMASK_STATUS            0x18
#define PKE_POWER_NOISE_LOCK             0x1c
#define PKE_KEY_CRC                      0x20
#define PKE_KEY_RANDOM_1                 0x24
#define PKE_KEY_RANDOM_2                 0x28
#define PKE_RESULT_FLAG                  0x30
#define PKE_FAILURE_FLAG                 0x34
#define PKE_OTP_KEY_SEL                  0x38
#define PKE_CFG_KEY_FLAG                 0x3c
#define PKE_POWER_EN                     0x40
#define PKE_NOISE_EN                     0x44
#define PKE_MRAM                         0x200
#define PKE_PRAM                         0x300
#define PKE_NRAM                         0x600
#define PKE_KRAM                         0xa00
#define PKE_RRAM                         0xe00

/* PKE_LOCK_CTRL Register */
#define REG_PKELOCK_BASE         0x10119C00
#define PKE_LOCK_CTRL                    (REG_PKELOCK_BASE + 0x010)
#define PKE_LOCK_STATUS                  (REG_PKELOCK_BASE + 0x014)
#define PKE_ERROR                        (REG_PKELOCK_BASE + 0x020)

#define PKE_LOCK_TYPE_LOCK               0x00
#define PKE_LOCK_TYPE_UNLOCK             0x01

#define PKE_LOCK_UNLOCK                  0x00
#define PKE_LOCK_TPP                     0x55
#define PKE_LOCK_HPP                     0x95  /* HSL */
#define PKE_LOCK_TEE                     0xA5
#define PKE_LOCK_REE                     0xAA

/*! \Define the pke data len */
typedef enum {
    PKE_DATA_LEN_BYTE_32  = 4,
    PKE_DATA_LEN_BYTE_48  = 6,
    PKE_DATA_LEN_BYTE_64  = 8,
    PKE_DATA_LEN_BYTE_72  = 9,
    PKE_DATA_LEN_BYTE_128 = 16,
    PKE_DATA_LEN_BYTE_256 = 32,
    PKE_DATA_LEN_BYTE_384 = 48,
    PKE_DATA_LEN_BYTE_512 = 64,
} pke_data_len;

/*! \Define the ram sel for clear */
typedef enum {
    PKE_CLR_RAM_SEL_NONE = 0x00,
    PKE_CLR_RAM_SEL_MRAM = 0x01,
    PKE_CLR_RAM_SEL_NRAM = 0x02,
    PKE_CLR_RAM_SEL_KRAM = 0x04,
    PKE_CLR_RAM_SEL_RRAM = 0x08,
    PKE_CLR_RAM_SEL_RAM  = 0x10,
    PKE_CLR_RAM_SEL_ALL  = 0x1F,
} pke_clr_ram_sel;

/*! \Define the ram type */
typedef enum {
    PKE_RAM_TYPE_MRAM = 0,   /* 0x0200 ~ 0x05FF */
    PKE_RAM_TYPE_NRAM,       /* 0x0600 ~ 0x09FF */
    PKE_RAM_TYPE_KRAM,       /* 0x0a00 ~ 0x0dFF */
    PKE_RAM_TYPE_RRAM,       /* 0x0e00 ~ 0x11FF */
} pke_ram_type;

/*! \Define the operaion mode */
typedef enum {
    PKE_MODE_EXP_MOD        = 0x1,
    PKE_MODE_MUL_DOT        = 0x2,
    PKE_MODE_ADD_DOT        = 0x3,
    PKE_MODE_TIMES_DOT      = 0x4,
    PKE_MODE_ADD_MOD        = 0x5,
    PKE_MODE_SUB_MOD        = 0x6,
    PKE_MODE_MUL_MOD        = 0x7,
    PKE_MODE_MINV_MOD       = 0x8,
    PKE_MODE_MOD            = 0x9,
    PKE_MODE_MUL            = 0xa,
    PKE_MODE_KGEN_NO_E      = 0x10,
    PKE_MODE_KGEN_WITH_E    = 0x11,
    PKE_MODE_KTRANS         = 0x12,
    PKE_MODE_CLR_RAM        = 0x1f,
} pke_mode;

/*! \Define the result flag */
typedef enum {
    PKE_RESULT_FLAG_IDLE  = 0x00,
    PKE_RESULT_FLAG_OK    = 0x05,
    PKE_RESULT_FLAG_FAIL  = 0x0A,
} pke_result_code;

/*! \Define the result flag */
typedef enum {
    PKE_FAIL_FLAG_N_EVEN            = 0x01,
    PKE_FAIL_FLAG_GET_RAND          = 0x02,
    PKE_FAIL_FLAG_MOD_INFINITY      = 0x04,
    PKE_FAIL_FLAG_POINT_INFINITY    = 0x08,
    PKE_FAIL_FLAG_POINT_INVALID     = 0x10,
    PKE_FAIL_FLAG_DFX               = 0x20,
    PKE_FAIL_FLAG_DFA               = 0x40,
    PKE_FAIL_FLAG_CALL_UNIT         = 0x80,
} pke_fail_flag;

#define PKE_FAIL_FLAG_CNT  0x08
#define PKE_SECURE_TRUE    0x05
#define PKE_SECURE_FALSE   0x0A

/* Define the union U_PKE_BUSY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    pke_busy              : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_busy;

/* Define the union U_PKE_WORK_MODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    mode_sel              : 5; /* [4..0]  */
        unsigned int    reserved_2            : 3; /* [7..5]  */
        unsigned int    ram_sel               : 5; /* [12..8]  */
        unsigned int    reserved_1            : 3; /* [15..13]  */
        unsigned int    data_len              : 7; /* [22..16]  */
        unsigned int    reserved_0            : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_work_mode;

/* Define the union U_PKE_START */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    pke_start             : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_start;

/* Define the union U_PKE_INT_ENABLE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    finish_int_enable     : 1; /* [0]  */
        unsigned int    alarm_int_enable      : 1; /* [1]  */
        unsigned int    reserved_0            : 29; /* [30..2]  */
        unsigned int    int_enable            : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_int_enable;

/* Define the union U_PKE_INT_STATUS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    finish_int            : 4; /* [3..0]  */
        unsigned int    alarm_int             : 4; /* [7..4]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_int_status;

/* Define the union U_PKE_INT_NOMASK_STATUS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    finish_int_nomask     : 4; /* [3..0]  */
        unsigned int    alarm_int_nomask      : 4; /* [7..4]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_int_nomask_status;

/* Define the union U_PKE_POWER_NOISE_LOCK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    power_noise_lock      : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_power_noise_lock;

/* Define the union U_PKE_KEY_CRC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    key_crc_in            : 16; /* [15..0]  */
        unsigned int    key_crc_out           : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_key_crc;

/* Define the union U_PKE_RESULT_FLAG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    pke_result_flag       : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_result_flag;

/* Define the union U_PKE_FAILURE_FLAG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    pke_failure_flag      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_failure_flag;

/* Define the union U_PKE_OTP_KEY_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    otp_key_sel           : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_otp_key_sel;

/* Define the union U_PKE_CFG_KEY_FLAG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    key_cfg_flag          : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_cfg_key_flag;

/* Define the union U_PKE_POWER_EN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    power_en              : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_power_on;

/* Define the union U_PKE_NOISE_EN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    noise_en              : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_noise_en;

/* Define the union U_PKE_INT_EN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    pke_lock_int_en       : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_int_en;

/* Define the union U_PKE_LOCK_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    pke_lock              : 1; /* [0]  */
        unsigned int    pke_lock_type         : 1; /* [1]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_lock_ctrl;

/* Define the union U_PKE_LOCK_STATUS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    pke_lock_busy         : 1; /* [0]  */
        unsigned int    pke_unlock_fail       : 1; /* [1]  */
        unsigned int    reserved_0            : 2; /* [3..2]  */
        unsigned int    pke_lock_cnt          : 3; /* [6..4]  */
        unsigned int    reserved_1            : 1; /* [7]  */
        unsigned int    pke_lock_stat         : 8; /* [15..8]  */
        unsigned int    reserved_2            : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_lock_status;

/* Define the union U_PKE_ERROR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    pke_access_err        : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_error;

#endif /* _DRV_SM2_V1_H_ */
