#!/usr/bin/env python3
# coding=utf-8

# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

import os
import re

def genarate_reserve_config(reserve_config, file_path):
        with open(file_path, 'r') as fp:
            lines = fp.read()
        reserve_lines = ""
        for config in reserve_config:
            r = re.search("['\"]%s['\"](\s*):[\d\D\r\n]*?\}(\s*)," % config, lines)
            reserve_lines += "    %s\n" % r.group()
        # 替换([\w\W]*?\n)里匹配的内容为reserve_lines
        lines = re.sub("(target|target_template) = \{\n([\w\W]*?\n)\}",
                       lambda x: x.group().replace(x.group(2), reserve_lines), lines)
        lines = re.sub("target_copy = \{\n([\w\W]*?\n)\}",
                       lambda x: x.group().replace(x.group(1), "\n"), lines)
        lines = re.sub("target_group = \{\n([\w\W]*?\n)\}",
                       lambda x: x.group().replace(x.group(1), "\n"), lines)
        with open(file_path, 'w') as fp:
            fp.write(lines)
