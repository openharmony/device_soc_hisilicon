/*
# Copyright (C) 2026 HiHope Open Source Organization .
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
#include "lowpower.h"
#include "iot_errno.h"

unsigned int LpcInit(void)
{
    return IOT_SUCCESS;
}

unsigned int LpcSetType(LpcType type)
{
    if (type == NO_SLEEP) {
        return IOT_SUCCESS;
    }
    return IOT_FAILURE;
}
