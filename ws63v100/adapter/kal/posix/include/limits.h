/*
# Copyright (C) 2024 HiHope Open Source Organization .
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
 */

#ifndef _KAL_LIMITS_H
#define _KAL_LIMITS_H

#include_next <limits.h>

#ifndef PTHREAD_STACK_MIN
#ifdef __LITEOS__
#define PTHREAD_STACK_MIN LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE
#endif
#endif

#endif
