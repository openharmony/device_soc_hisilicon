/*
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
 */
#ifndef _LINUX_DMA_DIRECTION_H
#define _LINUX_DMA_DIRECTION_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * These definitions mirror those in pci.h, so they can be used
 * interchangeably with their PCI_ counterparts.
 */
#define    DMA_BIDIRECTIONAL  0
#define    DMA_TO_DEVICE      1
#define    DMA_FROM_DEVICE    2
#define    DMA_NONE           3

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LINUX_DMA_DIRECTION_H */
