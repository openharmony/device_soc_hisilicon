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
#ifndef _LINUX_PAGEMAP_H
#define _LINUX_PAGEMAP_H

#include "asm/bug.h"
#include "asm/page.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define COMPAT_PAGE_SHIFT        12
#define COMPAT_PAGE_CACHE_SHIFT  COMPAT_PAGE_SHIFT
#define COMPAT_PAGE_CACHE_SIZE   PAGE_SIZE

#define PageLocked(page)    1
#define Page_Uptodate(page) 0
#define UnlockPage(page)
#define PAGE_BUG(page) BUG()
#define ClearPageUptodate(page)
#define SetPageError(page)
#define ClearPageError(page)
#define SetPageUptodate(page)

#define PAGE_SHIFT  COMPAT_PAGE_SHIFT
#define PAGE_CACHE_SHIFT        COMPAT_PAGE_CACHE_SHIFT
#define PAGE_CACHE_SIZE         COMPAT_PAGE_CACHE_SIZE

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LINUX_PAGEMAP_H */
