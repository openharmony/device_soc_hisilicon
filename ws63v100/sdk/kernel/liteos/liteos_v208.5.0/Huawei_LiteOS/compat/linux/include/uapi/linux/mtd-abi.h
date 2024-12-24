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
#ifndef _HW_MTD_ABI_H_
#define _HW_MTD_ABI_H_
#include "linux/types.h"
#include "linux/kernel.h"
#include "linux/ioctl.h"

struct erase_info_user {
  __u32 start;
  __u32 length;
};
struct erase_info_user64 {
  __u64 start;
  __u64 length;
};

#define MTD_NORFLASH 3
#define MTD_NANDFLASH 4
#define MTD_WRITEABLE 0x400
#define MTD_BIT_WRITEABLE 0x800
#define MTD_CAP_NORFLASH (MTD_WRITEABLE | MTD_BIT_WRITEABLE)
#define MTD_CAP_NANDFLASH (MTD_WRITEABLE)

struct mtd_info_user {
  __u8 type;
  __u32 flags;
  __u32 size;
  __u32 erasesize;
  __u32 writesize;
  __u32 oobsize;
  __u64 padding;
};

#define MEMGETINFO _IOR('M', 1, struct mtd_info_user)
#define MEMERASE _IOW('M', 2, struct erase_info_user)
#define MEMGETBADBLOCK _IOW('M', 11, __kernel_loff_t)
#define MEMSETBADBLOCK _IOW('M', 12, __kernel_loff_t)
#define MEMERASE64 _IOW('M', 20, struct erase_info_user64)

#endif
