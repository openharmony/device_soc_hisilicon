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
#include <linux/module.h>
#include "ot_type.h"
#include "ot_common.h"
#include "mod_ext.h"
#include "mm_ext.h"
#include "ot_base_mod_init.h"
#include "ot_debug.h"

static td_u32 g_vb_force_exit = 0;
module_param(g_vb_force_exit, uint, S_IRUGO);

EXPORT_SYMBOL(OT_LOG);
EXPORT_SYMBOL(cmpi_get_module_name);
EXPORT_SYMBOL(cmpi_get_module_func_by_id);
EXPORT_SYMBOL(cmpi_stop_modules);
EXPORT_SYMBOL(cmpi_query_modules);
EXPORT_SYMBOL(cmpi_exit_modules);
EXPORT_SYMBOL(cmpi_init_modules);
EXPORT_SYMBOL(cmpi_register_module);
EXPORT_SYMBOL(cmpi_unregister_module);

static int __init base_mod_init(void)
{
    vb_set_force_exit(g_vb_force_exit);
    comm_init();
    return 0;
}
static void __exit base_mod_exit(void)
{
    comm_exit();
}

module_init(base_mod_init);
module_exit(base_mod_exit);

MODULE_LICENSE("GPL");
