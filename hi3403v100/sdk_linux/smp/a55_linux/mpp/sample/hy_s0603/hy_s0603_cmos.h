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
 
#ifndef HY_S0603_CMOS_H
#define HY_S0603_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

#define HY_S0603_I2C_ADDR    0x60
#define HY_S0603_ADDR_BYTE   2
#define HY_S0603_DATA_BYTE   1
#define hy_s0603_sensor_get_ctx(dev, ctx)   ((ctx) = hy_s0603_get_ctx(dev))

#define HY_S0603_FULL_LINES_MAX          0x6978 /* 0x6978 = 27000 = Min:2.5fps */
#define HY_S0603_FULL_LINES_MAX_2TO1_WDR 0x34BC /* 13500 = Min:10fps */
#define HY_S0603_INCREASE_LINES 0  /* make real fps less than stand fps because NVR require */
#define HY_S0603_VMAX_8M_30FPS_12BIT_LINEAR     (2250 + HY_S0603_INCREASE_LINES) /* 0x8CA */
#define HY_S0603_VMAX_8M_30FPS_10BIT_2TO1_WDR   (4500 + HY_S0603_INCREASE_LINES)   /* 0x1194 */

/******** HY_S0603 Register Address ********/
#define HY_S0603_EXPO_H_ADDR          0x3E00
#define HY_S0603_EXPO_M_ADDR          0x3E01
#define HY_S0603_EXPO_L_ADDR          0x3E02

#define HY_S0603_AGAIN_H_ADDR         0x3E08
#define HY_S0603_AGAIN_L_ADDR         0x3E09

#define HY_S0603_DGAIN_H_ADDR         0x3E06
#define HY_S0603_DGAIN_L_ADDR         0x3E07

#define HY_S0603_SHORT_EXPO_H_ADDR    0x3E22
#define HY_S0603_SHORT_EXPO_M_ADDR    0x3E04
#define HY_S0603_SHORT_EXPO_L_ADDR    0x3E05

#define HY_S0603_SHORT_AGAIN_H_ADDR   0x3E12
#define HY_S0603_SHORT_AGAIN_L_ADDR   0x3E13

#define HY_S0603_SHORT_DGAIN_H_ADDR   0x3E10
#define HY_S0603_SHORT_DGAIN_L_ADDR   0x3E11

#define HY_S0603_VMAX_H_ADDR          0x320E
#define HY_S0603_VMAX_L_ADDR          0x320F

#define HY_S0603_WDR_LOGIC_H_ADDR          0x3230
#define HY_S0603_WDR_LOGIC_L_ADDR          0x3231

#define HY_S0603_SHORT_EXP_MAX_H_ADDR     0x3E23
#define HY_S0603_SHORT_EXP_MAX_L_ADDR     0x3E24

#define HY_S0603_LOGIC_ADDR_2     0x363c

#define HY_S0603_FRAME_RATE_MIN       0xD2F0 /* Min:2.5fps */
#define EXP_OFFSET_LINEAR            4
#define EXP_OFFSET_WDR_LONG          8
#define EXP_OFFSET_WDR_SHORT         2
#define HY_S0603_SEXP_MAX_DEFAULT     0x10A

typedef enum {
    HY_S0603_8M_30FPS_12BIT_LINEAR_MODE = 0,
    HY_S0603_8M_30FPS_10BIT_2TO1_VC_MODE,
    HY_S0603_MODE_BUTT
} hy_s0603_res_mode;

typedef struct {
    td_u32      ver_lines;
    td_u32      max_ver_lines;
    td_float    max_fps;
    td_float    min_fps;
    td_u32      width;
    td_u32      height;
    td_u8       sns_mode;
    ot_wdr_mode wdr_mode;
    const char *mode_name;
} hy_s0603_video_mode_tbl;

typedef enum {
    EXPO_L_IDX = 0,
    EXPO_M_IDX,
    EXPO_H_IDX,
    AGAIN_L_IDX,
    AGAIN_H_IDX,
    DGAIN_L_IDX,
    DGAIN_H_IDX,
    VMAX_L_IDX,
    VMAX_H_IDX,
    SNS_LOGIC_REG363C,
    REG_MAX_IDX
} hy_s0603_linear_reg_index;

typedef enum {
    WDR_EXPO_L_IDX = 0,
    WDR_EXPO_M_IDX,
    WDR_EXPO_H_IDX,
    WDR_AGAIN_L_IDX,
    WDR_AGAIN_H_IDX,
    WDR_DGAIN_L_IDX,
    WDR_DGAIN_H_IDX,
    WDR_VMAX_L_IDX,
    WDR_VMAX_H_IDX,
    WDR_SNS_LOGIC_REG363C,
    WDR_SHORT_EXPO_L_IDX,
    WDR_SHORT_EXPO_M_IDX,
    WDR_SHORT_AGAIN_L_IDX,
    WDR_SHORT_AGAIN_H_IDX,
    WDR_SHORT_DGAIN_L_IDX,
    WDR_SHORT_DGAIN_H_IDX,
    SHORT_EXP_MAX_L_ADDR_IDX,
    SHORT_EXP_MAX_H_ADDR_IDX,
    WDR_LOGIC_L_IDX,
    WDR_LOGIC_H_IDX,
    WDR_REG_MAX_IDX
}hy_s0603_wdr_reg_index;


ot_isp_sns_state *hy_s0603_get_ctx(ot_vi_pipe vi_pipe);
ot_isp_sns_commbus *hy_s0603_get_bus_info(ot_vi_pipe vi_pipe);
td_void hy_s0603_set_blc_clamp_value(ot_vi_pipe vi_pipe, td_bool clamp_en);

td_void hy_s0603_init(ot_vi_pipe vi_pipe);
td_void hy_s0603_exit(ot_vi_pipe vi_pipe);
td_void hy_s0603_standby(ot_vi_pipe vi_pipe);
td_void hy_s0603_restart(ot_vi_pipe vi_pipe);
td_void hy_s0603_blc_clamp(ot_vi_pipe vi_pipe, ot_isp_sns_blc_clamp blc_clamp);

td_s32  hy_s0603_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data);
td_s32  hy_s0603_read_register(ot_vi_pipe vi_pipe, td_u32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* HY_S0603_CMOS_H */
