/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
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


#include "iot_errno.h"
#include "iot_pwm.h"
#include "hi_pwm.h"
#include "hi_errno.h"

#define CLK_160M 160000000
#define DUTY_MIN 0
#define DUTY_MAX 100
#define SHORT_MAX 0xFFFF

unsigned int IoTPwmInit(unsigned int port)
{
    if (hi_pwm_set_clock(PWM_CLK_160M) != HI_ERR_SUCCESS) {
        return IOT_FAILURE;
    }
    return hi_pwm_init((hi_pwm_port)port);
}

unsigned int IoTPwmDeinit(unsigned int port)
{
    return hi_pwm_deinit((hi_pwm_port)port);
}

unsigned int IoTPwmStart(unsigned int port, unsigned short duty, unsigned int freq)
{
    unsigned short hiDuty;
    unsigned short hiFreq;

    if ((freq == 0) || (duty >= DUTY_MAX) || (duty == DUTY_MIN)) {
        return IOT_FAILURE;
    }

    if ((CLK_160M / freq) > SHORT_MAX) {
        return IOT_FAILURE;
    }

    hiFreq = (unsigned short)(CLK_160M / freq);
    hiDuty = (duty * hiFreq) / DUTY_MAX;

    return hi_pwm_start((hi_pwm_port)port, hiDuty, hiFreq);
}

unsigned int IoTPwmStop(unsigned int port)
{
    return hi_pwm_stop((hi_pwm_port)port);
}

