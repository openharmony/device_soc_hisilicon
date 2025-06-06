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

#ifndef CMSIS_OS_H_
#define CMSIS_OS_H_

#include "los_config.h"

#if (CMSIS_OS_VER == 1)
#error "cmsis version 1.0 is not supported now!"
#elif (CMSIS_OS_VER == 2)
#include "cmsis_os2.h"
#endif

#endif /* CMSIS_OS_H_ */
