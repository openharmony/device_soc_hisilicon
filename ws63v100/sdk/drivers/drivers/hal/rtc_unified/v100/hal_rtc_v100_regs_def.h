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
 * Description:  \n
 *
 * History: \n
 * 2023-03-02, Create file. \n
 */
#ifndef HAL_RTC_V100_REGS_DEF_H
#define HAL_RTC_V100_REGS_DEF_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_rtc_v100_regs_def RTC V100 Regs Defination
 * @ingroup  drivers_hal_rtc
 * @{
 */

#define RTC_REGS_MAX_NUM      4

/**
 * @brief  This union represents the bit fields in the Control. \n
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rtc_control_reg_data {
    uint32_t d32;                               /*!< Raw register data. */
    struct {
        uint32_t enable                 :   1;  /*!< RTC enable bit. */
        uint32_t mode                   :   1;  /*!< RTC mode bit. */
        uint32_t int_mask               :   1;
        uint32_t reserved3_31           :   29;
    } b;                                        /*!< Register bits. */
} rtc_control_reg_data_t;

/**
 * @brief  This union represents the bit fields in the End-of-Interrupt. \n
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rtc_eoi_data {
    uint32_t d32;                               /*!< Raw register data. */
    struct {
        uint32_t eoi                    :   1;  /*!< Reading from this register returns zeroes(0)
                                                     and clear the interrupt. */
        uint32_t reserved1_31           :   31;
    } b;                                        /*!< Register bits. */
} rtc_eoi_data_t;

typedef struct rtc_regs_info {
    volatile uint32_t load_count;       /*!< Value to be loaded into RTC.             <i>Offset: 00h</i>. */
    volatile uint32_t current_value;    /*!< Current Value of RTC.                    <i>Offset: 04h</i>. */
    volatile uint32_t control_reg;      /*!< Control Register for RTC.                <i>Offset: 08h</i>. */
    volatile uint32_t eoi;              /*!< Clears the interrupt from RTC.           <i>Offset: 0Ch</i>. */
    volatile uint32_t int_status;       /*!< Contains the interrupt status for RTC.   <i>Offset: 10h</i>. */
} rtc_regs_info_t;

typedef struct rtc_regs {
    volatile rtc_regs_info_t g_rtc_regs_info[RTC_REGS_MAX_NUM];                  /*!< RTC regs.
                                                                                      <i>Offset: 000h + i*0x14h</i>. */
} rtc_regs_t;
/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif