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
#include <types.h>
#include <common.h>
#include <lib.h>
#include <platform.h>
#include <flash_map.h>
#include <checkup.h>
#include <atf_common.h>
#include <securecutil.h>
#include "../drivers/otp/otp.h"
#include "../drivers/ddr_init/init_regs.h"
#include "../drivers/ddr_init/ddr_interface.h"
#include "../drivers/pke/drv_pke.h"
#include "../drivers/pke/ot_drv_pke.h"
#include "../drivers/cipher/cipher.h"
#include "../drivers/uart/uart.h"
#include "../drivers/sdio/sdhci.h"
#include "../drivers/sdio/fat.h"
#include "../drivers/ddr_init/ddr_hal_context.h"
#include "../drivers/usb3/usb3.h"
#include "../drivers/emmc/emmc.h"
#include "../secure_failure.h"

/* Constants for reported magic-number values. */
#define SS927_BOOT_ARRAY_INDEX_2               2
#define SS927_BOOT_DDRC_INDEX_2                2
#define SS927_BOOT_DDRC_INDEX_3                3
#define SS927_BOOT_HPM_ARRAY_SIZE              4
#define SS927_BOOT_HPM_MASK                    0x3ff
#define SS927_BOOT_DELAY_MS_10                 10
#define SS927_BOOT_DELAY_MS_20                 20
#define SS927_BOOT_DELAY_MS_500                500
#define SS927_BOOT_DIVISOR_10                  10
#define SS927_BOOT_DIVISOR_100                 100
#define SS927_BOOT_FACTOR_10                   10
#define SS927_BOOT_RANDOM_INDEX_2              2
#define SS927_BOOT_RANDOM_INDEX_3              3
#define SS927_BOOT_SHIFT_1                     1
#define SS927_BOOT_SHIFT_16                    16
#define SS927_BOOT_SHIFT_2                     2
#define SS927_BOOT_SHIFT_3                     3
#define SS927_BOOT_SHIFT_4                     4
#define SS927_BOOT_SHIFT_8                     8
#define SS927_BOOT_TEMPERATURE_INDEX_0P5       0.5f
#define SS927_BOOT_TEMPERATURE_INDEX_2         2
#define SS927_BOOT_TEMPERATURE_SENSOR_COUNT    3
#define SS927_BOOT_TEMPERATURE_REG_STEP        0x100
#define SS927_BOOT_TEMPERATURE_DIVISOR         718
#define SS927_BOOT_TEMPERATURE_MULTIPLIER      165
#define SS927_BOOT_TEMPERATURE_OFFSET          40
#define SS927_BOOT_TEMPERATURE_HIGH            100
#define SS927_BOOT_TEMPERATURE_MIDDLE          70
#define SS927_BOOT_VALUE_146                   146
#define SS927_BOOT_VALUE_3                     3
#define SS927_BOOT_VALUE_4                     4
#define SS927_BOOT_VALUE_5                     5
#define SS927_BOOT_VALUE_8                     8
#define SS927_BOOT_VALUE_59                    59
#define SS927_BOOT_VALUE_61                    61
#define SS927_BOOT_VALUE_62                    62
#define SS927_BOOT_DELTA_SIGN_CORE_MASK        0x800
#define SS927_BOOT_DELTA_SIGN_MDA_MASK         0x80
#define SS927_BOOT_DELTA_SIGN_NPU_MASK         0x8
#define SS927_BOOT_DELTA_VALUE_MASK            0x7
#define SS927_BOOT_ADJUST_LOW_NPU              2
#define SS927_BOOT_ADJUST_LOW_MDA              3
#define SS927_BOOT_ADJUST_LOW_CORE             4
#define SS927_BOOT_ADJUST_HIGH_NPU             5
#define SS927_BOOT_ADJUST_HIGH_MDA             5
#define SS927_BOOT_ADJUST_HIGH_CORE            8
#define SS927_BOOT_DDRCA_ENABLE                5
#define SS927_BOOT_DDRC_CHANNEL_NUM            4
#define SS927_BOOT_DDRC_ENABLE                 1
#define SS927_BOOT_DDRC_DISABLE                0
#define SS927_BOOT_DDRC_VALID_MASK             0xf

/* The flag for auto update. 1:enable; 0:disable */
/* Note: The CONFIG_AUTO_SD_UPDATE and CONFIG_TOOLPLATFORM_USB_UPDATE cannot be set to 1 at the same time.
   Otherwise, the GSL image maybe exceed the maximum value, which causes the system startup failure.*/
#define CONFIG_AUTO_SD_UPDATE   0
#define CONFIG_TOOLPLATFORM_USB_UPDATE  1
#define SHIFT_OFF   32
#define EMMC_READ_BLOCK_MAX 10

unsigned long jump_addr;
backup_image_params_s g_backup_params;
unsigned long stack_chk_guard_value;
unsigned int get_random_num(void);

#ifndef LLVM_COMPILER
#pragma GCC push_options
#pragma GCC optimize ("-fno-stack-protector")
void stack_chk_fail_handler(void)
{
    err_print('S', 'P');
    call_reset();
}

#ifdef __LP64__
void stack_chk_guard_setup()
{
    unsigned random = 0;
    random = get_random_num();
    stack_chk_guard_value = random;
    stack_chk_guard_value <<= SHIFT_OFF;
    random = get_random_num();
    stack_chk_guard_value |= random;
}
#endif

#ifdef __LP32__
void stack_chk_guard_setup()
{
    unsigned random = 0;
    random = get_random_num();
    stack_chk_guard_value = random;
}
#endif

#pragma GCC pop_options
#else
__attribute__((no_stack_protector)) void stack_chk_fail_handler(void)
{
    err_print(STACK_CHK_FAIL);
    call_reset();
}
__attribute__((no_stack_protector)) void stack_chk_guard_setup()
{
    unsigned random = 0;
    random = get_random_num();
    stack_chk_guard_value = random;
    stack_chk_guard_value <<= SHIFT_OFF;
}
#endif

void call_reset(void)
{
    mdelay(SS927_BOOT_DELAY_MS_500);
    timer_deinit();
    reg_set((uint32_t *)(REG_BASE_SCTL + REG_SC_SYSRES), 0x1);
}

void err_print(uint8_t err_type, uint8_t err_idx)
{
    if (is_scs_dbg_enable() != AUTH_SUCCESS) {
        return;
    }

    if (uart_inited == 0) {
        uart_init();
        uart_reset();
        mdelay(SS927_BOOT_DELAY_MS_10);
    }
    serial_putc('\n');
    serial_putc('G');
    serial_putc(err_type);
    serial_putc('S');
    serial_putc(err_idx);
    mdelay(SS927_BOOT_DELAY_MS_10);
}

static void check_and_set_backup_image_flag(uint32_t offset_times)
{
    uint32_t val;
    uint32_t backup_image_enable_flag;

    if (is_backup_image_enable() != AUTH_SUCCESS) {
        return;
    }

    val = reg_get(REG_BASE_SCTL + BACKUP_IMAGE_FLG_REG);
    backup_image_enable_flag = get_backup_image_flag(val);
    if (backup_image_enable_flag == BACKUP_IMAGE_ENABLE) {
        val = reg_get(REG_BASE_SCTL + BACKUP_IMAGE_OFF_REG);
        val &= ~BACKUP_IMAGE_OFF_MASK;
        val |= (offset_times << BACKUP_IMAGE_OFF_BIT);
        reg_set(REG_BASE_SCTL + BACKUP_IMAGE_OFF_REG, val);
    } else {
        val = reg_get(REG_BASE_SCTL + BACKUP_IMAGE_FLG_REG);
        val &= ~BACKUP_IMAGE_FLG_MASK;
        val |= (BACKUP_IMAGE_ENABLE << BACKUP_IMAGE_FLG_BIT);
        reg_set(REG_BASE_SCTL + BACKUP_IMAGE_FLG_REG, val);

        val = reg_get(REG_BASE_SCTL + BACKUP_IMAGE_OFF_REG);
        val &= ~BACKUP_IMAGE_OFF_MASK;
        val |= (0x1 << BACKUP_IMAGE_OFF_BIT);
        reg_set(REG_BASE_SCTL + BACKUP_IMAGE_OFF_REG, val);
    }
}

void failure_process(void)
{
    check_and_set_backup_image_flag(g_backup_params.offset_times);

    if ((is_func_jtag_enable() == AUTH_FAILURE) ||
        ((is_backup_image_enable() == AUTH_SUCCESS)))
        call_reset();

    _secure_failure_process();

    return;
}

void tee_img_verify_failure()
{
    uint32_t failure_count;
    err_print('T', '3');
    /* add TEE_Launch_Failure register with 1 */
    failure_count = reg_get(REG_TEE_LAUNCH_FAILURE);
    failure_count++;
    reg_set(REG_TEE_LAUNCH_FAILURE, failure_count);
    /* soft reset */
    call_reset();
}

static void configure_trng_parameters(void)
{
}

static void start_flow_prepare(void)
{
    malloc_init();
    uart_port_init();
    configure_trng_parameters();
    return;
}

static void configure_ddr_parameters(void)
{
    uint32_t tables_base;
    uint32_t gsl_code_area_len;

    gsl_code_area_len = get_gsl_code_area_len();
    tables_base = (uint32_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR + BOOTLOADER_CFG_PARAMS_OFFSET +
                 gsl_code_area_len);
    init_registers(tables_base, 0);
}

static unsigned hpm_value_avg(const unsigned int *val, unsigned int num)
{
    unsigned int i;
    unsigned tmp = 0;

    /* 4: Cycle */
    for (i = 0; i < num; i++) {
        tmp += val[i];
    }

    return (tmp / CYCLE_NUM) >> SS927_BOOT_SHIFT_2;
}

static void get_hpm_value(unsigned int *hpm_core, unsigned int *hpm_npu,
    unsigned int *hpm_mda)
{
    int i;
    unsigned int temp;
    unsigned int core_value[SS927_BOOT_HPM_ARRAY_SIZE] = {0, 0, 0, 0};
    unsigned int mda_value[SS927_BOOT_HPM_ARRAY_SIZE] = {0, 0, 0, 0};
    unsigned int npu_value[SS927_BOOT_HPM_ARRAY_SIZE] = {0, 0, 0, 0};

    for (i = 0; i < CYCLE_NUM; i++) {
        /*  (at least 16us*4) */
        mdelay(5); /* delay 5 s */
        /* npu */
        temp = readl(SYSCTRL_REG + HPM_NPU_REG0);
        npu_value[1] += (temp >> SS927_BOOT_SHIFT_16) & SS927_BOOT_HPM_MASK;
        npu_value[0] += temp & SS927_BOOT_HPM_MASK;
        temp = readl(SYSCTRL_REG + HPM_NPU_REG1);
        npu_value[SS927_BOOT_SHIFT_3] += (temp >> SS927_BOOT_SHIFT_16) & SS927_BOOT_HPM_MASK;
        npu_value[SS927_BOOT_ARRAY_INDEX_2] += temp & SS927_BOOT_HPM_MASK;

        /* mda */
        temp = readl(SYSCTRL_REG + HPM_MDA_REG0);
        mda_value[1] += (temp >> SS927_BOOT_SHIFT_16) & SS927_BOOT_HPM_MASK;
        mda_value[0] += temp & SS927_BOOT_HPM_MASK;
        temp = readl(SYSCTRL_REG + HPM_MDA_REG1);
        mda_value[SS927_BOOT_SHIFT_3] += (temp >> SS927_BOOT_SHIFT_16) & SS927_BOOT_HPM_MASK;
        mda_value[SS927_BOOT_ARRAY_INDEX_2] += temp & SS927_BOOT_HPM_MASK;

        /* core */
        temp = readl(SYSCTRL_REG + HPM_CORE_REG0);
        core_value[1] += (temp >> SS927_BOOT_SHIFT_16) & SS927_BOOT_HPM_MASK;
        core_value[0] += temp & SS927_BOOT_HPM_MASK;
        temp = readl(SYSCTRL_REG + HPM_CORE_REG1);
        core_value[SS927_BOOT_SHIFT_3] += (temp >> SS927_BOOT_SHIFT_16) & SS927_BOOT_HPM_MASK;
        core_value[SS927_BOOT_ARRAY_INDEX_2] += temp & SS927_BOOT_HPM_MASK;
    }

    *hpm_core = hpm_value_avg(core_value, SS927_BOOT_HPM_ARRAY_SIZE);
    *hpm_mda = hpm_value_avg(mda_value, SS927_BOOT_HPM_ARRAY_SIZE);
    *hpm_npu = hpm_value_avg(npu_value, SS927_BOOT_HPM_ARRAY_SIZE);
}

static void start_hpm(unsigned int *hpm_core, unsigned int *hpm_npu,
    unsigned int *hpm_mda)
{
    get_hpm_value(hpm_core, hpm_npu, hpm_mda);
}

static void save_hpm(unsigned int hpm_core, unsigned int hpm_npu,
    unsigned int hpm_mda)
{
    writel(hpm_mda, REG_SYSCTRL_BASE + HPM_MDA_STORAGE_REG);
    writel(hpm_npu, REG_SYSCTRL_BASE + HPM_NPU_STORAGE_REG);
    writel(hpm_core, REG_SYSCTRL_BASE + HPM_CORE_STORAGE_REG);
}
static unsigned int calc_volt_regval(unsigned int volt_val, unsigned int volt_max,
    unsigned int volt_min)
{
    unsigned int duty;

    if (volt_val >= volt_max) {
        volt_val = volt_max - 1;
    }
    if (volt_val <= volt_min) {
        volt_val = volt_min + 1;
    }
    duty =  duty_formula(volt_max, volt_min, volt_val);

    return duty;
}

static void set_hpm_core_volt(unsigned int hpm_core_value, int delta_v)
{
    unsigned int volt_val;
    unsigned int reg_val;

    volt_val = (CORE_CURVE_B - CORE_CURVE_A * hpm_core_value) / SS927_BOOT_DIVISOR_10;
    if (volt_val > CORE_CURVE_VLOT_MAX) {
        volt_val = CORE_CURVE_VLOT_MAX;
    } else if (volt_val < CORE_CURVE_VLOT_MIN) {
        volt_val = CORE_CURVE_VLOT_MIN;
    }

    volt_val += delta_v;
    reg_val = calc_volt_regval(volt_val, CORE_VOLT_MAX, CORE_VOLT_MIN);
    writel(reg_val, REG_SYSCTRL_BASE + CORE_DUTY_STORAGE_REG);
    writel(reg_val, HPM_CORE_VOL_REG);
}

static void set_hpm_mda_volt(unsigned int hpm_mda_value, int delta_v)
{
    unsigned int volt_val;
    unsigned int reg_val;

    volt_val = (MEDIA_CURVE_B - MEDIA_CURVE_A * hpm_mda_value) / SS927_BOOT_DIVISOR_10;
    if (volt_val > MEDIA_CURVE_VLOT_MAX) {
        volt_val = MEDIA_CURVE_VLOT_MAX;
    } else if (volt_val < MEDIA_CURVE_VLOT_MIN) {
        volt_val = MEDIA_CURVE_VLOT_MIN;
    }

    volt_val += delta_v;
    reg_val = calc_volt_regval(volt_val, MDA_VOLT_MAX, MDA_VOLT_MIN);
    writel(reg_val, REG_SYSCTRL_BASE + MDA_DUTY_STORAGE_REG);
    writel(reg_val, HPM_MDA_VOL_REG);
}

static void set_hpm_npu_volt(unsigned int hpm_npu_value, int delta_v)
{
    unsigned int volt_val;
    unsigned int reg_val;

    volt_val = (NPU_CURVE_B - NPU_CURVE_A * hpm_npu_value) / SS927_BOOT_DIVISOR_100;
    if (volt_val > NPU_CURVE_VLOT_MAX) {
        volt_val = NPU_CURVE_VLOT_MAX;
    } else if (volt_val < NPU_CURVE_VLOT_MIN) {
        volt_val = NPU_CURVE_VLOT_MIN;
    }

    volt_val += delta_v;
    reg_val = calc_volt_regval(volt_val, NPU_VOLT_MAX, NPU_VOLT_MIN);
    writel(reg_val, REG_SYSCTRL_BASE+NPU_DUTY_STORAGE_REG);
    writel(reg_val, HPM_NPU_VOL_REG);
}
static void get_delta_v(int *core_delta_v, int *npu_delta_v,  int *mda_delta_v)
{
    unsigned int value = readl(SYSCTRL_REG + OTP_SHPM_MDA_OFFSET);
    writel(value, REG_SYSCTRL_BASE+DELTA_V_STORAGE_REG);
    /* core:bit 11-8,
     * bit11 equal to 1 means negative, equal to 0 means positive,
     * bit 8-10 is the  absolute delta_v
     */
    int flag =  value & SS927_BOOT_DELTA_SIGN_CORE_MASK ? -1 : 1;
    *core_delta_v = flag * (int)((value >> SS927_BOOT_SHIFT_8) & SS927_BOOT_DELTA_VALUE_MASK) *
        SS927_BOOT_FACTOR_10;

    /* mda:bit 7-4,
     * bit7 equal to 1 means negative, equal to 0 means positive,
     * bit 4-6 is the  absolute delta_v
     */
    flag =  value & SS927_BOOT_DELTA_SIGN_MDA_MASK ? -1 : 1;
    *mda_delta_v = flag * (int)((value >> SS927_BOOT_SHIFT_4) & SS927_BOOT_DELTA_VALUE_MASK) *
        SS927_BOOT_FACTOR_10;

    /* npu:bit 3-0,
     * bit3 equal to 1 means negative, equal to 0 means positive,
     * bit 0-2 is the  absolute delta_v
     */
    flag =  value & SS927_BOOT_DELTA_SIGN_NPU_MASK ? -1 : 1;
    *npu_delta_v = flag * (int)(value & SS927_BOOT_DELTA_VALUE_MASK)  * SS927_BOOT_FACTOR_10;
}

static void set_volt(unsigned int hpm_core, unsigned int hpm_npu,
    unsigned int hpm_mda)
{
    int  core_delta_v = 0;
    int  npu_delta_v = 0;
    int  mda_delta_v = 0;
    get_delta_v(&core_delta_v, &npu_delta_v, &mda_delta_v);

    set_hpm_core_volt(hpm_core, core_delta_v);
    set_hpm_mda_volt(hpm_mda, mda_delta_v);
    set_hpm_npu_volt(hpm_npu, npu_delta_v);

    /* delay 300 Cycle */
    mdelay(30);
}

static void get_temperature(int *temperature)
{
    int value = 0;
    int tsensor_chn;
    float m = SS927_BOOT_TEMPERATURE_INDEX_0P5;
    float temperature_temp[SS927_BOOT_TEMPERATURE_SENSOR_COUNT] = {0};
    for (tsensor_chn = 0; tsensor_chn < SS927_BOOT_TEMPERATURE_SENSOR_COUNT; tsensor_chn++) {
        value = (int)(readl(REG_TSENSOR_CTRL + TSENSOR_STATUS0 +
                    SS927_BOOT_TEMPERATURE_REG_STEP * tsensor_chn) & SS927_BOOT_HPM_MASK);

        m = ((float)value - SS927_BOOT_VALUE_146) / SS927_BOOT_TEMPERATURE_DIVISOR *
            SS927_BOOT_TEMPERATURE_MULTIPLIER - SS927_BOOT_TEMPERATURE_OFFSET;

        if (m > 0) {
            temperature_temp[tsensor_chn] = m + SS927_BOOT_TEMPERATURE_INDEX_0P5;
        } else {
            temperature_temp[tsensor_chn] = m - SS927_BOOT_TEMPERATURE_INDEX_0P5;
        }
    }

    *temperature = (temperature_temp[0] + temperature_temp[1] + temperature_temp[SS927_BOOT_TEMPERATURE_INDEX_2]) /
        SS927_BOOT_TEMPERATURE_SENSOR_COUNT;
    writel(*temperature, REG_SYSCTRL_BASE + TEMPERATURE_STORAGE_REG);
}

static void adjust_hpm(unsigned int *hpm_core, unsigned int *hpm_mda,
    unsigned int *hpm_npu, int temperature)
{
    unsigned int otp_hpm_core = readl(REG_SYSCTRL_BASE + OTP_HPM_CORE_OFFSET);
    unsigned int otp_hpm_npu  = readl(REG_SYSCTRL_BASE + OTP_HPM_NPU_OFFSET);
    unsigned int otp_hpm_mda  = readl(REG_SYSCTRL_BASE + OTP_HPM_MDA_OFFSET);

    if (temperature <= 0) {
        *hpm_npu  -= SS927_BOOT_ADJUST_LOW_NPU;
        *hpm_mda  -= SS927_BOOT_ADJUST_LOW_MDA;
        *hpm_core -= SS927_BOOT_ADJUST_LOW_CORE;
    } else if (temperature > SS927_BOOT_TEMPERATURE_HIGH) {
        *hpm_npu  += SS927_BOOT_ADJUST_HIGH_NPU;
        *hpm_mda  += SS927_BOOT_ADJUST_HIGH_MDA;
        *hpm_core += SS927_BOOT_ADJUST_HIGH_CORE;
    } else if (temperature > SS927_BOOT_TEMPERATURE_MIDDLE) {
        *hpm_npu  += SS927_BOOT_VALUE_3;
        *hpm_mda  += SS927_BOOT_VALUE_3;
        *hpm_core += SS927_BOOT_VALUE_4;
    }

    if (otp_hpm_core) {
        /* 10: Increment */
        if (*hpm_core > (otp_hpm_core + SS927_BOOT_VALUE_59)) {
            *hpm_core = otp_hpm_core + SS927_BOOT_VALUE_59;   /* 5: Increment */
        }
    }

    if (otp_hpm_npu) {
        /* 15: Increment */
        if (*hpm_npu > (otp_hpm_npu + SS927_BOOT_VALUE_62)) {
            *hpm_npu = otp_hpm_npu + SS927_BOOT_VALUE_62;
        }
    }

    if (otp_hpm_mda) {
        /* 15: Increment */
        if (*hpm_mda > (otp_hpm_mda + SS927_BOOT_VALUE_61)) {
            *hpm_mda = otp_hpm_mda + SS927_BOOT_VALUE_61;
        }
    }
}

void init_temperature(void)
{
    int tsensor_chn;

    for (tsensor_chn = 0; tsensor_chn < SS927_BOOT_TEMPERATURE_SENSOR_COUNT; tsensor_chn++) {
        writel(TSENSOR_CTRL0_CFG, REG_TSENSOR_CTRL +
                TSENSOR_CTRL0 + SS927_BOOT_TEMPERATURE_REG_STEP * tsensor_chn);
        writel(TSENSOR_CTRL1_CFG, REG_TSENSOR_CTRL +
                TSENSOR_CTRL1 + SS927_BOOT_TEMPERATURE_REG_STEP * tsensor_chn);
    }
}
void init_hpm(void)
{
    /* open hmp clock */

    writel(HPM_CLK_CFG, HPM_CLK_REG);

    /* npu  */
    writel(HPM_MONITOR_CFG, SYSCTRL_REG + HPM_NPU_OFFSET);
    /* mda  */
    writel(HPM_MONITOR_CFG, SYSCTRL_REG + HPM_MDA_OFFSET);
    /* core  */
    writel(HPM_MONITOR_CFG, SYSCTRL_REG + HPM_CORE_OFFSET);
}

static void svb_voltage_change(void)
{
    unsigned int hpm_core;
    unsigned int hpm_npu;
    unsigned int hpm_mda;
    int temperature;
    /* init temperature and hpm*/
    init_temperature();
    init_hpm();
    mdelay(SS927_BOOT_DELAY_MS_20);
    start_hpm(&hpm_core, &hpm_npu, &hpm_mda);

    /*get temperature */
    get_temperature(&temperature);

    adjust_hpm(&hpm_core, &hpm_mda, &hpm_npu, temperature);
    set_volt(hpm_core, hpm_npu, hpm_mda);
    save_hpm(hpm_core, hpm_npu, hpm_mda);

    /* add SVB VER*/
    writel(SVB_VER, SVB_VER_REG);
}

static void set_qosbuf_cfg(void)
{
    unsigned int val;

    val = readl(CH0_DMC_CFG_DDRMODE);
    writel(val, CH0_QOS_CFG_DDRMODE);
    val = readl(CH0_DMC_CFG_RNKVOL_0);
    writel(val, CH0_QOS_CFG_RNKVOL_0);
    val = readl(CH0_DMC_CFG_RNKVOL_1);
    writel(val, CH0_QOS_CFG_RNKVOL_1);

    val = readl(CH1_DMC_CFG_DDRMODE);
    writel(val, CH1_QOS_CFG_DDRMODE);
    val = readl(CH1_DMC_CFG_RNKVOL_0);
    writel(val, CH1_QOS_CFG_RNKVOL_0);
    val = readl(CH1_DMC_CFG_RNKVOL_1);
    writel(val, CH1_QOS_CFG_RNKVOL_1);

    val = readl(CH2_DMC_CFG_DDRMODE);
    writel(val, CH2_QOS_CFG_DDRMODE);
    val = readl(CH2_DMC_CFG_RNKVOL_0);
    writel(val, CH2_QOS_CFG_RNKVOL_0);
    val = readl(CH2_DMC_CFG_RNKVOL_1);
    writel(val, CH2_QOS_CFG_RNKVOL_1);

    val = readl(CH3_DMC_CFG_DDRMODE);
    writel(val, CH3_QOS_CFG_DDRMODE);
    val = readl(CH3_DMC_CFG_RNKVOL_0);
    writel(val, CH3_QOS_CFG_RNKVOL_0);
    val = readl(CH3_DMC_CFG_RNKVOL_1);
    writel(val, CH3_QOS_CFG_RNKVOL_1);
}

static void ddr_training(void)
{
    struct ddr_hal_context ctx;
    struct ddr_hal_phy_all phy_all;

    /* ddr ctx cfg */
    ddr_hal_set_cfg_addr((uintptr_t)&ctx, (uintptr_t)&phy_all);
    ddr_hal_cfg_init();

    set_qosbuf_cfg();

    ddr_set_rdqbdl_def_val();

    /* ddr hw training */
    ddr_hw_training_init();

    /* ddr sw training */
    ddr_sw_training_if();

    ddr_retrain_anti_aging_enable();
    /* ddr DMC auto power down config */
    ddr_dmc_auto_power_down_cfg();
}

static void ddr_debug(void)
{
}

#ifdef DDR_SCRAMB_ENABLE
unsigned int get_random_num(void)
{
    unsigned int reg_val;

    do {
        reg_val = reg_get(REG_BASE_RNG_GEN + TRNG_DATA_ST_OFST);
    } while (!((reg_val >> BIT_TRNG_FIFO_DATA_CNT) & TRNG_FIFO_DATA_CNT_MASK));
    reg_val = reg_get(REG_BASE_RNG_GEN + TRNG_DSTA_FIFO_DATA_OFST);

    return reg_val;
}

void ddr_scramb_start(const unsigned int *random, int size)
{
    if ((size < RANDOM_SIZE) || (random == NULL)) {
        return;
    }

    reg_set((unsigned int *)(REG_SYSCTRL_BASE + DDRCA_REE_RANDOM_L), random[0]);
    reg_set((unsigned int *)(REG_SYSCTRL_BASE + DDRCA_REE_RANDOM_H), random[1]);
    reg_set((unsigned int *)(REG_SYSCTRL_BASE + DDRCA_TEE_RANDOM_L), random[SS927_BOOT_RANDOM_INDEX_2]);
    reg_set((unsigned int *)(REG_SYSCTRL_BASE + DDRCA_TEE_RANDOM_H), random[SS927_BOOT_RANDOM_INDEX_3]);

    reg_set((unsigned int *)(REG_SYSCTRL_BASE + DDRCA_EN), SS927_BOOT_DDRCA_ENABLE);
    reg_set((unsigned int *)(REG_SYSCTRL_BASE + DDRCA_REE_UPDATE), SS927_BOOT_DDRC_ENABLE);
    reg_set((unsigned int *)(REG_SYSCTRL_BASE + DDRCA_TEE_UPDATE), SS927_BOOT_DDRC_ENABLE);
    reg_set((unsigned int *)(REG_SYSCTRL_BASE + DDR_CA_LOCK), SS927_BOOT_DDRC_ENABLE);
}

static void ddr_get_valid_channels(unsigned int *ddrc_isvalid)
{
    ddrc_isvalid[0] = (reg_get(REG_BASE_DDRC + DDRC_CFG_DDRMODE_OFST) & SS927_BOOT_DDRC_VALID_MASK) ?
        SS927_BOOT_DDRC_ENABLE : SS927_BOOT_DDRC_DISABLE;
    ddrc_isvalid[1] = (reg_get(REG_BASE_DDRC + DDRC1_CFG_DDRMODE_OFST) & SS927_BOOT_DDRC_VALID_MASK) ?
        SS927_BOOT_DDRC_ENABLE : SS927_BOOT_DDRC_DISABLE;
    ddrc_isvalid[SS927_BOOT_DDRC_INDEX_2] =
        (reg_get(REG_BASE_DDRC + DDRC2_CFG_DDRMODE_OFST) & SS927_BOOT_DDRC_VALID_MASK) ?
        SS927_BOOT_DDRC_ENABLE : SS927_BOOT_DDRC_DISABLE;
    ddrc_isvalid[SS927_BOOT_DDRC_INDEX_3] =
        (reg_get(REG_BASE_DDRC + DDRC3_CFG_DDRMODE_OFST) & SS927_BOOT_DDRC_VALID_MASK) ?
        SS927_BOOT_DDRC_ENABLE : SS927_BOOT_DDRC_DISABLE;
}

static void ddr_set_self_refresh(const unsigned int *ddrc_isvalid, unsigned int value)
{
    if (ddrc_isvalid[0]) {
        reg_set(REG_BASE_DDRC + DDRC_CTRL_SREF_OFST, value);
    }
    if (ddrc_isvalid[1]) {
        reg_set(REG_BASE_DDRC + DDRC1_CTRL_SREF_OFST, value);
    }
    if (ddrc_isvalid[SS927_BOOT_DDRC_INDEX_2]) {
        reg_set(REG_BASE_DDRC + DDRC2_CTRL_SREF_OFST, value);
    }
    if (ddrc_isvalid[SS927_BOOT_DDRC_INDEX_3]) {
        reg_set(REG_BASE_DDRC + DDRC3_CTRL_SREF_OFST, value);
    }
}

static void ddr_wait_self_refresh_status(const unsigned int *ddrc_isvalid, unsigned int expected)
{
    unsigned int reg_val[SS927_BOOT_DDRC_CHANNEL_NUM];

    do {
        reg_val[0] = ddrc_isvalid[0] ? (reg_get(REG_BASE_DDRC + DDRC_CURR_FUNC_OFST) & SS927_BOOT_DDRC_ENABLE) :
            expected;
        reg_val[1] = ddrc_isvalid[1] ? (reg_get(REG_BASE_DDRC + DDRC1_CURR_FUNC_OFST) & SS927_BOOT_DDRC_ENABLE) :
            expected;
        reg_val[SS927_BOOT_DDRC_INDEX_2] = ddrc_isvalid[SS927_BOOT_DDRC_INDEX_2] ?
            (reg_get(REG_BASE_DDRC + DDRC2_CURR_FUNC_OFST) & SS927_BOOT_DDRC_ENABLE) : expected;
        reg_val[SS927_BOOT_DDRC_INDEX_3] = ddrc_isvalid[SS927_BOOT_DDRC_INDEX_3] ?
            (reg_get(REG_BASE_DDRC + DDRC3_CURR_FUNC_OFST) & SS927_BOOT_DDRC_ENABLE) : expected;
    } while ((expected == SS927_BOOT_DDRC_ENABLE) ?
        !(reg_val[0] & reg_val[1] & reg_val[SS927_BOOT_DDRC_INDEX_2] & reg_val[SS927_BOOT_DDRC_INDEX_3]) :
        (reg_val[0] | reg_val[1] | reg_val[SS927_BOOT_DDRC_INDEX_2] | reg_val[SS927_BOOT_DDRC_INDEX_3]));
}

static void ddr_fill_random(unsigned int *random_num)
{
    unsigned int i;

    for (i = 0; i < RANDOM_SIZE; i++) {
        random_num[i] = get_random_num();
    }
}

static void ddr_scrambling(void)
{
    unsigned int random_num[RANDOM_SIZE];
    unsigned int ddrc_isvalid[SS927_BOOT_DDRC_CHANNEL_NUM] = {0, 0, 0, 0};

    /* read ddrc_cfg_ddrmode register,
     * if value[3:0] is not 0x0 ,the channel is valid.
     */
    ddr_get_valid_channels(ddrc_isvalid);

    /* set ddrc to do self-refurbish */
    ddr_set_self_refresh(ddrc_isvalid, SS927_BOOT_DDRC_ENABLE);
    ddr_wait_self_refresh_status(ddrc_isvalid, SS927_BOOT_DDRC_ENABLE);

    /* get random number */
    ddr_fill_random(random_num);

    /* start ddr scrambling */
    ddr_scramb_start(random_num, RANDOM_SIZE);

    /* clear random number */
    ddr_fill_random(random_num);

    /* set ddrc to exit self-refurbish */
    ddr_set_self_refresh(ddrc_isvalid, (SS927_BOOT_DDRC_ENABLE << SS927_BOOT_SHIFT_1));

    /* wait the status of ddrc to be normal */
    ddr_wait_self_refresh_status(ddrc_isvalid, SS927_BOOT_DDRC_DISABLE);

    return;
}
#endif

static void wait_pcie_slave_flag(int print_enable, uint32_t flag_val)
{
    uint32_t cnt = 0;
    uint32_t reg;

    do {
        if (cnt % PCIE_SLAVE_PRINT_INTERVAL == 0) {
            if (print_enable) {
                serial_putc('.');
            }
        }

        cnt++;

        reg = reg_get(REG_BASE_SCTL + PCIE_SLAVE_BOOT_CTL_REG);
    } while (reg != flag_val);

    if (print_enable) {
        serial_putc('O');
        serial_putc('K');
        serial_putc('\n');
    }
}

static void pcie_slave_set_ddr_init_done_flag(int print_enable)
{
    reg_set(REG_BASE_SCTL + PCIE_SLAVE_BOOT_CTL_REG,
        DDR_INIT_EXCUTE_OK_FLAG);

    if (print_enable) {
        serial_putc('\n');
        serial_putc('P');
        serial_putc('C');
        serial_putc('I');
        serial_putc('E');
        serial_putc(':');
        serial_putc('D');
        serial_putc('D');
        serial_putc('R');
        serial_putc('I');
        serial_putc('N');
        serial_putc('I');
        serial_putc('T');
        serial_putc(' ');
        serial_putc('D');
        serial_putc('O');
        serial_putc('N');
        serial_putc('E');
        serial_putc('\n');
    }
}

static int wait_pcie_slave_bootloader_download_ok(int print_enable)
{
    if (print_enable) {
        serial_putc('\n');
        serial_putc('P');
        serial_putc('C');
        serial_putc('I');
        serial_putc('E');
        serial_putc(':');
        serial_putc('D');
        serial_putc('O');
        serial_putc('W');
        serial_putc('N');
        serial_putc('L');
        serial_putc('O');
        serial_putc('A');
        serial_putc('D');
        serial_putc(' ');
        serial_putc('B');
        serial_putc('O');
        serial_putc('O');
        serial_putc('T');
        serial_putc('L');
        serial_putc('O');
        serial_putc('A');
        serial_putc('D');
        serial_putc('E');
        serial_putc('R');
        serial_putc(' ');
    }

    wait_pcie_slave_flag(print_enable, UBOOT_DOWNLOAD_OK_FLAG);

    return TD_SUCCESS;
}

static void clear_backup_image_flag_and_offset_times(void)
{
    uint32_t val;
    errno_t err;

    /* Clear backup image boot flag */
    val = reg_get(REG_BASE_SCTL + BACKUP_IMAGE_FLG_REG);
    val &= ~(0xffffff << 0x4);
    reg_set(REG_BASE_SCTL + BACKUP_IMAGE_FLG_REG, val);

    /* Clear bakup image offset times */
    val = reg_get(REG_BASE_SCTL + BACKUP_IMAGE_OFF_REG);
    val &= ~(0xfff << 0x4);
    reg_set(REG_BASE_SCTL + BACKUP_IMAGE_OFF_REG, val);

    err = memset_s(&g_backup_params, sizeof(backup_image_params_s), 0, sizeof(backup_image_params_s));
    if (err != EOK) {
        return;
    }
}

static int system_init()
{
    configure_ddr_parameters();
    svb_voltage_change();
    ddr_training();
    ddr_debug();
    ddr_capat_adapt_start();
#ifdef DDR_SCRAMB_ENABLE
    ddr_scrambling();
#endif
    return TD_SUCCESS;
}

static uint32_t get_data_channel_type(void)
{
    uint32_t channel_type;

    channel_type = reg_get(REG_BASE_SCTL + DATA_CHANNEL_TYPE_REG);
    switch (channel_type) {
        case BOOT_SEL_PCIE:
        case BOOT_SEL_UART:
        case BOOT_SEL_SDIO:
        case BOOT_SEL_USB:
        case BOOT_SEL_FLASH:
        case BOOT_SEL_EMMC:
            break;
        default:
            err_print('3', '1');
            channel_type = BOOT_SEL_UNKNOW;
            break;
    }

    return channel_type;
}

static void get_image_backup_params(uint32_t channel_type, backup_image_params_s *backup_params)
{
    uint32_t backup_image_enable_flag;
    uint32_t val;

    val = reg_get(REG_BASE_SCTL + BACKUP_IMAGE_FLG_REG);
    backup_image_enable_flag = get_backup_image_flag(val);
    if ((is_backup_image_enable() != AUTH_SUCCESS) ||
        (backup_image_enable_flag != BACKUP_IMAGE_ENABLE)) {
        return;
    }

    backup_params->offset_times = reg_get(REG_BASE_SCTL + BACKUP_IMAGE_TIMES_REG);
    backup_params->offset_addr = reg_get(REG_BASE_SCTL + BACKUP_IMAGE_ADDR_REG);
    backup_params->enable = 1;
}

static void pcie_slave_ready_for_head_area_flag(int print_enable)
{
    reg_set(REG_BASE_SCTL + PCIE_SLAVE_BOOT_CTL_REG,
        READ_FOR_HEAD_AREA_FLAG);

    if (print_enable) {
        serial_putc('\n');
        serial_putc('P');
        serial_putc('C');
        serial_putc('I');
        serial_putc('E');
        serial_putc(':');
        serial_putc('R');
        serial_putc('E');
        serial_putc('A');
        serial_putc('D');
        serial_putc(' ');
        serial_putc('F');
        serial_putc('O');
        serial_putc('R');
        serial_putc(' ');
        serial_putc('H');
        serial_putc('E');
        serial_putc('A');
        serial_putc('D');
        serial_putc(' ');
        serial_putc('A');
        serial_putc('R');
        serial_putc('E');
        serial_putc('A');
        serial_putc('\n');
    }
}


static int wait_pcie_slave_get_head_area_ok(int print_enable)
{
    if (print_enable) {
        serial_putc('\n');
        serial_putc('P');
        serial_putc('C');
        serial_putc('I');
        serial_putc('E');
        serial_putc(':');
        serial_putc('G');
        serial_putc('E');
        serial_putc('T');
        serial_putc(' ');
        serial_putc('H');
        serial_putc('E');
        serial_putc('A');
        serial_putc('D');
        serial_putc(' ');
        serial_putc('A');
        serial_putc('R');
        serial_putc('E');
        serial_putc('A');
    }

    wait_pcie_slave_flag(print_enable, HEAD_AREA_OK_FLAG);

    return TD_SUCCESS;
}

static int get_head_area_data_form_pcie()
{
    int ret;
    int print_enable = 0;

    if (is_boot_info_lv_debug_enable() == AUTH_SUCCESS) {
        print_enable = uart_boot_flag(reg_get(REG_BASE_SCTL + REG_SC_SYSSTAT));
    }
    pcie_slave_ready_for_head_area_flag(print_enable);
    ret = wait_pcie_slave_get_head_area_ok(print_enable);

    return ret;
}

static int get_head_area_data_form_sdio()
{
#if (CONFIG_AUTO_SD_UPDATE == 1)
    int ret;

    enable_sdio_dma();
    sdio_deinit();
    if (!self_sdio_check())
    {
        return TD_FAILURE;
    }
    /* Read the data from 64K to 79K(lenght is 15K) in the image */
    set_sdio_pos(SECURE_IMAGE_STEP1_SIZE);
    ret = copy_from_sdio((void *)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
        SECURE_IMAGE_STEP1_SIZE), SECURE_IMAGE_STEP2_SIZE);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    set_sdio_pos(0);
#endif

    return TD_SUCCESS;
}

static int get_head_area_data_form_usb()
{
#if (CONFIG_TOOLPLATFORM_USB_UPDATE == 1)
    int ret;

    ret = self_usb_check();
    if (ret != TRUE)
    {
        return TD_FAILURE;
    }
    ret = copy_from_usb((void *)(uintptr_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
                        SECURE_IMAGE_STEP1_SIZE), SECURE_IMAGE_STEP2_SIZE);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
#endif

    return TD_SUCCESS;
}

static int get_head_area_data_from_flash(backup_image_params_s *backup_params)
{
    uint32_t bootloader_key_area_addr;
    uint32_t gsl_code_area_len;
    uint32_t spacc_channel_number = 1;
    spacc_decrypt_params decrypt_params = {0};
    int ret;

    gsl_code_area_len = get_gsl_code_area_len();
    bootloader_key_area_addr = (uint32_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
                          BOOTLOADER_KEY_AREA_ADDR_OFFSET + gsl_code_area_len);
    get_image_backup_params(BOOT_SEL_FLASH, backup_params);
    decrypt_params.chn = spacc_channel_number;
    decrypt_params.dst_addr = bootloader_key_area_addr;
    decrypt_params.src_addr = (uint32_t)(SPI_BASE_ADDR +  backup_params->offset_addr +
                         BOOTLOADER_KEY_AREA_ADDR_OFFSET + gsl_code_area_len);
    decrypt_params.length = BOOTLOADER_KEY_PARAMS_AREA_SIZE;
    decrypt_params.iv_addr = 0;
    decrypt_params.iv_length = 0;
    decrypt_params.alg = SYMC_ALG_DMA;
    decrypt_params.mode = SYMC_MODE_CBC;

    /* Not an actual decryption operation, just get data from flash by cipher dma */
    ret = drv_spacc_decrypt(decrypt_params);
    if (ret != TD_SUCCESS) {
        err_print('4', '7');
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static int get_head_area_data_from_emmc(backup_image_params_s *backup_params)
{
    uint32_t bootloader_key_area_addr;
    uint32_t gsl_code_area_len;
    uint32_t align_addr;
    uint32_t align_len;
    uint32_t align_params_size;
    uint32_t tmp_addr;
    uint32_t data_addr = 0;
    uint32_t data_len;
    int ret;

    if (mmc_init() != 0) {
        err_print('4', '8');
        return TD_FAILURE;
    }

    gsl_code_area_len = get_gsl_code_area_len();
    bootloader_key_area_addr = (uint32_t)(VENDOR_ROOT_PUBLIC_KEY_ADDR +
                          BOOTLOADER_KEY_AREA_ADDR_OFFSET + gsl_code_area_len);

    get_image_backup_params(BOOT_SEL_EMMC, backup_params);
    /* The eMMC read data must be 512-byte aligned. */
    align_addr = (backup_params->offset_addr + BOOTLOADER_KEY_AREA_ADDR_OFFSET +
        gsl_code_area_len) / EMMC_BLOCK_SIZE * EMMC_BLOCK_SIZE;
    align_len = backup_params->offset_addr + BOOTLOADER_KEY_AREA_ADDR_OFFSET +
        gsl_code_area_len - align_addr;
    align_params_size = align_len + BOOTLOADER_KEY_PARAMS_AREA_SIZE;

    /* Here tmp_addr is used as the temporary storage address. */
    tmp_addr = bootloader_key_area_addr;

    /* The eMMC must read data from address 0. So Redundant data is discarded. */
    for (int i = 0; i < align_addr / EMMC_BLOCK_SIZE;) {
        if ((align_addr / EMMC_BLOCK_SIZE - i) >= EMMC_READ_BLOCK_MAX) {
            data_len = EMMC_READ_BLOCK_MAX * EMMC_BLOCK_SIZE;
            i += EMMC_READ_BLOCK_MAX;
        } else {
            data_len = EMMC_BLOCK_SIZE;
            i++;
        }

        /* get data from emmc by cipher dma */
        ret = mmc_read((void *)(uintptr_t)tmp_addr, data_addr,
            data_len, READ_DATA_BY_DMA);
        if (ret != TD_SUCCESS) {
            err_print('4', '9');
            return TD_FAILURE;
        }
        data_addr += data_len;
    }

    ret = mmc_read((void *)(uintptr_t)tmp_addr, data_addr,
        align_params_size, READ_DATA_BY_DMA);
    if (ret != TD_SUCCESS) {
        err_print('4', 'a');
        return TD_FAILURE;
    }
    ret = memmove_s((void *)(uintptr_t)bootloader_key_area_addr,
        BOOTLOADER_KEY_PARAMS_AREA_SIZE,
        (void *)(uintptr_t)(tmp_addr + align_len),
        BOOTLOADER_KEY_PARAMS_AREA_SIZE);
    if (ret != EOK) {
        err_print('4', 'b');
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static int get_head_area_data(uint32_t channel_type, backup_image_params_s *backup_params)
{
    int ret;

    switch (channel_type) {
        case BOOT_SEL_PCIE:
            ret = get_head_area_data_form_pcie();
            if (ret != TD_SUCCESS) {
                err_print('4', '1');
                return TD_FAILURE;
            }

            break;

        case BOOT_SEL_UART:
            ret = copy_from_uart((void *)(VENDOR_ROOT_PUBLIC_KEY_ADDR + SECURE_IMAGE_STEP1_SIZE),
                SECURE_IMAGE_STEP2_SIZE);
            if (ret != TD_SUCCESS) {
                err_print('4', '2');
                return TD_FAILURE;
            }
            break;

        case BOOT_SEL_SDIO:
            ret = get_head_area_data_form_sdio();
            if (ret != TD_SUCCESS) {
                err_print('4', '3');
                return TD_FAILURE;
            }

            break;

        case BOOT_SEL_USB:
            ret = get_head_area_data_form_usb();
            if (ret != TD_SUCCESS) {
                err_print('4', '4');
                return TD_FAILURE;
            }

            break;

        case BOOT_SEL_FLASH:
            ret = get_head_area_data_from_flash(backup_params);
            if (ret != TD_SUCCESS) {
                err_print('4', '5');
                return TD_FAILURE;
            }

            break;

        case BOOT_SEL_EMMC:
            ret = get_head_area_data_from_emmc(backup_params);
            if (ret != TD_SUCCESS) {
                err_print('4', '6');
                return TD_FAILURE;
            }

            break;

        default:
            break;
    }

    return TD_SUCCESS;
}

static int get_bootloader_code_area_from_pcie()
{
    int ret;
    int print_enable = 0;

    if (is_boot_info_lv_debug_enable() == AUTH_SUCCESS) {
        print_enable = uart_boot_flag(reg_get(REG_BASE_SCTL + REG_SC_SYSSTAT));
    }
    pcie_slave_set_ddr_init_done_flag(print_enable);
    ret = wait_pcie_slave_bootloader_download_ok(print_enable);

    return ret;
}

static int get_bootloader_code_area_from_uart()
{
    uint32_t image_int_ddr_addr;
    uint32_t image_total_len;
    int ret;

    image_int_ddr_addr = get_boot_image_int_ddr_addr();
    image_total_len = get_boot_image_total_len();

    ret = copy_from_uart((void *)(uintptr_t)image_int_ddr_addr, image_total_len);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static int get_bootloader_code_area_from_sdio()
{
#if (CONFIG_AUTO_SD_UPDATE == 1)
    uint32_t image_int_ddr_addr;
    uint32_t image_total_len;
    int ret;

    image_int_ddr_addr = get_boot_image_int_ddr_addr();
    image_total_len = get_boot_image_total_len();
    sdio_deinit();
    if (!self_sdio_check()) {
        return TD_FAILURE;
    }
    ret = copy_from_sdio((void *)(uintptr_t)image_int_ddr_addr, image_total_len);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

#endif
    return TD_SUCCESS;
}

static int get_bootloader_code_area_from_usb()
{
#if (CONFIG_TOOLPLATFORM_USB_UPDATE == 1)
    uint32_t image_int_ddr_addr;
    uint32_t image_total_len;
    int ret;

    image_int_ddr_addr = get_boot_image_int_ddr_addr();
    image_total_len = get_boot_image_total_len();

    ret = copy_from_usb((void *)(uintptr_t)image_int_ddr_addr, image_total_len);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

#endif

    return TD_SUCCESS;
}

    static int get_bootloader_code_area_from_flash(const backup_image_params_s *backup_params)
{
    uint32_t image_int_ddr_addr;
    uint32_t image_total_len;
    uint32_t spacc_channel_number = 1;
    int ret;
    spacc_decrypt_params decrypt_params = {0};

    image_int_ddr_addr = get_boot_image_int_ddr_addr();
    image_total_len = get_boot_image_total_len();

    decrypt_params.chn = spacc_channel_number;
    decrypt_params.dst_addr = image_int_ddr_addr;
    decrypt_params.src_addr = (uint32_t)(SPI_BASE_ADDR + backup_params->offset_addr);
    decrypt_params.length = image_total_len;
    decrypt_params.alg = SYMC_ALG_DMA;
    decrypt_params.mode = SYMC_MODE_CBC;
    /* Not an actual decryption operation, just get data from flash by cipher dma */
    ret = drv_spacc_decrypt(decrypt_params);

    return ret;
}

static int get_bootloader_code_area_from_emmc(const backup_image_params_s *backup_params)
{
    uint32_t image_int_ddr_addr;
    uint32_t image_total_len;
    uint32_t tmp_addr;
    int ret;

    image_int_ddr_addr = get_boot_image_int_ddr_addr();
    image_total_len = get_boot_image_total_len();

    mmc_deinit();
    if (mmc_init() != 0) {
        err_print('8', '7');
        return TD_FAILURE;
    }

    /* NOte: The start address for U-Boot backup must be 512-byte (BLOCK) aligned. */
    /* Redundant data is Discard during U-Boot backup. */
    if (backup_params->offset_addr != 0) {
        /* Here tmp_addr is used as the temporary storage address. */
        tmp_addr = image_int_ddr_addr;
        ret = mmc_read((void *)(uintptr_t)tmp_addr, 0,
            backup_params->offset_addr, READ_DATA_BY_DMA);
        if (ret != TD_SUCCESS) {
            err_print('8', '8');
            return TD_FAILURE;
        }
    }

    /* get data from emmc by cipher dma */
    ret = mmc_read((void *)(uintptr_t)image_int_ddr_addr, backup_params->offset_addr,
        image_total_len, READ_DATA_BY_DMA);
    if (ret != TD_SUCCESS) {
        err_print('8', '9');
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static int get_bootloader_code_area_data(uint32_t channel_type,
    const backup_image_params_s *backup_params)
{
    int ret;

    switch (channel_type) {
        case BOOT_SEL_PCIE:
            ret = get_bootloader_code_area_from_pcie();
            if (ret != TD_SUCCESS) {
                err_print('8', '1');
                return TD_FAILURE;
            }
            break;

        case BOOT_SEL_UART:
            ret = get_bootloader_code_area_from_uart();
            if (ret != TD_SUCCESS) {
                err_print('8', '2');
                return TD_FAILURE;
            }
            break;

        case BOOT_SEL_SDIO:
            ret = get_bootloader_code_area_from_sdio();
            if (ret != TD_SUCCESS) {
                err_print('8', '3');
                return TD_FAILURE;
            }

            break;

        case BOOT_SEL_USB:
            ret = get_bootloader_code_area_from_usb();
            if (ret != TD_SUCCESS) {
                err_print('8', '4');
                return TD_FAILURE;
            }

            break;

        case BOOT_SEL_FLASH:
            ret = get_bootloader_code_area_from_flash(backup_params);
            if (ret != TD_SUCCESS) {
                err_print('8', '5');
                return TD_FAILURE;
            }
            break;

        case BOOT_SEL_EMMC:
            ret = get_bootloader_code_area_from_emmc(backup_params);
            if (ret != TD_SUCCESS) {
                err_print('8', '6');
                return TD_FAILURE;
            }
            break;

        default:
            break;
    }

    return TD_SUCCESS;
}

static ALWAYS_INLINE void store_booting_param()
{
    uint32_t decrypt_bootloader_addr;
    uint32_t boot_code_addr;
    uint32_t gsl_code_area_len;
    uint32_t unchecked_area_len;

    if (g_backup_params.enable) {
        clear_backup_image_flag_and_offset_times();
    }

    gsl_code_area_len = get_gsl_code_area_len();
    unchecked_area_len = get_bootloader_unchecked_area_len();
    decrypt_bootloader_addr = get_ddr_decrypt_bootloader_addr();
    boot_code_addr = (uint32_t)(decrypt_bootloader_addr + BOOTLOADER_BOOT_CODE_OFFSET +
                    gsl_code_area_len + unchecked_area_len);

    /* The DDR is not initialized in the U-boot */
    reg_set(REG_BASE_SCTL + REG_SC_GEN4, 0);
    jump_addr = boot_code_addr;
}

static int sec_module_init(char err_code)
{
    uint32_t key_slot = 0;

    if (set_key_slot(key_slot) != TD_SUCCESS) {
        err_print(err_code, '1');
        goto err_exit;
    }

    if (drv_pke_init() != TD_SUCCESS) {
        err_print(err_code, '2');
        goto err_exit;
    }

    if (spacc_init(key_slot) != TD_SUCCESS) {
        err_print(err_code, '3');
        goto err_exit;
    }
    return TD_SUCCESS;
err_exit:
    return TD_FAILURE;
}

static void sec_module_deinit(void)
{
    spacc_deinit();
    drv_pke_deinit();
    unset_key_slot();
}

void main_entry(void)
{
    int ret;
    uint32_t channel_type;

    start_flow_prepare();

    ret = sec_module_init('2');
    if (ret != TD_SUCCESS) {
        failure_process();
    }

    channel_type = get_data_channel_type();
    if (channel_type == BOOT_SEL_UNKNOW) {
        failure_process();
    }

    ret = get_head_area_data(channel_type, &g_backup_params);
    if (ret != TD_SUCCESS) {
        failure_process();
    }

    ret = handle_bootloader_key_area();
    if (ret != TD_SUCCESS) {
        failure_process();
    }

    ret = handle_bootloader_params_area();
    if (ret != TD_SUCCESS) {
        failure_process();
    }

    ret = system_init();
    if (ret != TD_SUCCESS) {
        failure_process();
    }

    ret = get_bootloader_code_area_data(channel_type, &g_backup_params);
    if (ret != TD_SUCCESS) {
        failure_process();
    }

    ret = handle_bootloader_code_area();
    if (ret != TD_SUCCESS) {
        failure_process();
    }

    store_booting_param();

    sec_module_deinit();
}

static int load_secure_os_fip(void)
{
    entry_point_info_t *bl32_ep = (entry_point_info_t *)(uintptr_t)BL32_EP_INFO_ADDR;
    entry_point_info_t *bl33_ep = (entry_point_info_t *)(uintptr_t)BL33_EP_INFO_ADDR;
    bl31_params_t *bl31_p = (bl31_params_t *)(uintptr_t)BL31_PARAMS_ADDR;
    uint64_t bl31_pc;
    errno_t err;

    /* to do: add  return value judgment  */
    err = memset_s(bl32_ep, sizeof(entry_point_info_t), 0, sizeof(entry_point_info_t));
    if (err != EOK) {
        return -1;
    }

    err = memset_s(bl31_p, sizeof(bl31_params_t), 0, sizeof(bl31_params_t));
    if (err != EOK) {
        return -1;
    }

    bl31_p->bl32_ep_info = (uint64_t)(uintptr_t)bl32_ep;
    bl31_p->bl33_ep_info = (uint64_t)(uintptr_t)bl33_ep;

    bl31_pc = BL31_BASE;
    bl32_ep->pc = (uint64_t)(uintptr_t)BL32_BASE;
    bl32_ep->args.arg0 = 0; //no dtb

    (*(volatile uint64_t *)OS_SYS_CTRL_REG2) = (uint64_t)bl31_p;
    (*(volatile uint64_t *)OS_SYS_CTRL_REG4) = (uint64_t)NULL;

    void (*atf_entry)(void) = (void(*)(void))(uintptr_t)(bl31_pc);
    atf_entry();

    return 0;
}

void secure_os_entry(void)
{
    int ret;

    ret = sec_module_init('i');
    if (ret != TD_SUCCESS) {
        tee_img_verify_failure();
    }

    /* configure ree DDR for tee image */
    config_tee_img_non_sec_ddr();
    /* configure secure DDR for tee key area */
    config_key_area_sec_ddr();

    /* disable secure DRR bypass */
    disable_sec_ddr_bypass();

    ret = handle_tee_key_area();
    if (ret != TD_SUCCESS) {
        tee_img_verify_failure();
    }

    /* configure secure DDR for tee image */
    config_tee_img_sec_ddr();

    ret = handle_atf_area();
    if (ret != TD_SUCCESS) {
        tee_img_verify_failure();
    }

    ret = handle_tee_code_area();
    if (ret != TD_SUCCESS) {
        tee_img_verify_failure();
    }

    sec_module_deinit();

    config_non_sec_ddr();

    load_secure_os_fip();
}
