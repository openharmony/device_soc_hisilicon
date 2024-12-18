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
#ifndef _LINUX_MODULE_H
#define _LINUX_MODULE_H

#include "linux/kernel.h"
#include "linux/moduleparam.h"
#include "linux/list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int (*InitcallFunc)(void);

#define __used  __attribute__((__used__))
#define DEFINE_INITCALL(f, level) \
    static InitcallFunc sfn_##f##level __used __attribute__((section(".initcall"#level".init"))) = {f}

/* startup levels follow linux kernel levels */
#define LEVEL_PURE      0
#define LEVEL_CORE      1
#define LEVEL_POSTCORE  2
#define LEVEL_ARCH      3
#define LEVEL_SUBSYS    4
#define LEVEL_FS        5
#define LEVEL_DEVICE    6
#define LEVEL_LATE      7

#define INITCALL(f, l)          DEFINE_INITCALL(f, l)

/* liteos startup initcalls declaration interface */
#define PURE_INITCALL(f)        INITCALL(f, LEVEL_PURE)
#define CORE_INITCALL(f)        INITCALL(f, LEVEL_CORE)
#define PPOSTCORE_INITCALL(f)   INITCALL(f, LEVEL_POSTCORE)
#define ARCH_INITCALL(f)        INITCALL(f, LEVEL_ARCH)
#define SUBSYS_INITCALL(f)      INITCALL(f, LEVEL_SUBSYS)
#define FS_INITCALL(f)          INITCALL(f, LEVEL_FS)
#define DEVICE_INITCALL(f)      INITCALL(f, LEVEL_DEVICE)
#define LATE_INITCALL(f)        INITCALL(f, LEVEL_LATE)

/* compatible with Linux initcalls declaration interface */
#define pure_initcall(f)        PURE_INITCALL(f)
#define core_initcall(f)        CORE_INITCALL(f)
#define postcore_initcall(f)    PPOSTCORE_INITCALL(f)
#define arch_initcall(f)        ARCH_INITCALL(f)
#define subsys_initcall(f)      SUBSYS_INITCALL(f)
#define fs_initcall(f)          FS_INITCALL(f)
#define device_initcall(f)      DEVICE_INITCALL(f)
#define late_initcall(f)        LATE_INITCALL(f)
#define _initcall(f)            device_initcall(f)
#define module_init(f)          device_initcall(f)
#define module_exit(f)

/* execute initcalls, the order of dependencies at different startup levels is guaranteed by the registered user */
extern UINT32 DoInitcalls(void);

#define module_param_named(u, v, t, f)
#define MODULE_PARM_DESC(_parm, desc)
#define MODULE_AUTHOR(a)
#define MODULE_DESCRIPTION(d)
#ifndef MODULE_LICENSE
#define MODULE_LICENSE(s)
#endif
#define MODULE_VERSION(v)
#define module_param_array(name, type, nump, perm)
#define THIS_MODULE ((struct module *)0)

struct module {
};

static inline int try_module_get(struct module *module)
{
    return 1;
}

static inline void module_put(struct module *module)
{
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LINUX_MODULE_H */
