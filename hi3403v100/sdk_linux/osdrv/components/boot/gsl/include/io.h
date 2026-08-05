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
#ifndef __ASM_ARM_IO_H
#define __ASM_ARM_IO_H

#include <types.h>
#include <barriers.h>

#define __arch_getl(a)      (*(volatile unsigned int *)(a))
#define __arch_putl(v, a)   (*(volatile unsigned int *)(unsigned long)(a) = (v))

#define mb()        dsb()
#define __iormb()   dmb()
#define __iowmb()   dmb()

#define writel(v, c) ({ uint32_t __v = v; __iowmb(); __arch_putl(__v, c); __v; })
#define readl(c)    ({ uint32_t __v = __arch_getl(c); __iormb(); __v; })

#endif  /* __ASM_ARM_IO_H */
