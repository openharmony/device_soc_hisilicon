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

#ifndef __SYS_H__
#define __SYS_H__
#include <types.h>
static inline void writel(uint32_t val, unsigned long int addr)
{
    (*(volatile uint32_t *)(addr)) = (val);
}

static inline void writew(uint16_t val, unsigned long int addr)
{
    (*(volatile uint16_t *)(addr)) = (val);
}

static inline void writeb(uint8_t val, unsigned long int addr)
{
    (*(volatile uint8_t *)(addr)) = (val);
}

static inline uint32_t readl(unsigned long int addr)
{
    return (*(volatile uint32_t *)(addr));
}

static inline uint16_t readw(unsigned long int addr)
{
    return (*(volatile uint16_t *)(addr));
}

static inline uint8_t readb(unsigned long int addr)
{
    return (*(volatile uint8_t *)(addr));
}
#endif /* end of __SYS_H__ */
