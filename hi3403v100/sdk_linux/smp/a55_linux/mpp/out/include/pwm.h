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

#ifndef OT_PWM_H
#define OT_PWM_H
#include "osal_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    unsigned char pwm_num;  /* 0:PWM0,1:PWM1,2:PWM2,3:PWMII0,4:PWMII1,5:PWMII2 */
    unsigned int  duty;
    unsigned int  period;
    unsigned char enable;
} pwm_data;

typedef enum {
    IOC_PWM_ISP_CMD_WRITE = 0,
    IOC_PWM_ISP_CMD_READ,
    IOC_PWM_ISP_BUTT,
} ioc_pwm_isp;
#define PWM_IOC_MAGIC    'p'
#define PWM_CMD_WRITE  OSAL_IOW(PWM_IOC_MAGIC, IOC_PWM_ISP_CMD_WRITE, pwm_data)
#define PWM_CMD_READ  OSAL_IOR(PWM_IOC_MAGIC, IOC_PWM_ISP_CMD_READ, pwm_data)

int pwm_drv_write(unsigned char pwm_num, unsigned int duty, unsigned int period, unsigned char enable);
int pwm_init(void);
void pwm_exit(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* OT_PWM_H */
