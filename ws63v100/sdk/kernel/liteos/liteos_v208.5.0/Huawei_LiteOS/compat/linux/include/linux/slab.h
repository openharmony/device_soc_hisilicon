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
#ifndef _LINUX_SLAB_H
#define _LINUX_SLAB_H

#include "stdlib.h"
#include "asm/page.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GFP_KERNEL    (0)
#define GFP_ATOMIC    (0)

#ifndef __kmalloc
#define __kmalloc(x)  malloc(x)
#endif

#ifndef kmalloc
#define kmalloc(x, y) __kmalloc(x)
#endif

#ifndef __kfree
#define __kfree(x)    free(x)
#endif
#ifndef kfree
#define kfree(x)      __kfree(x)
#endif

#ifndef __vmalloc
#define __vmalloc(x)  malloc(x)
#endif
#ifndef vmalloc
#define vmalloc(x)    __vmalloc(x)
#endif
#ifndef __vfree
#define __vfree(x)    free(x)
#endif
#ifndef vfree
#define vfree(x)      __vfree(x)
#endif
#ifndef kzalloc
#define kzalloc(s, f)   __kzalloc((s), (f))
#endif
#ifndef __kzalloc
#define __kzalloc(s, f) calloc(1, (s))
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LINUX_SLAB_H */
