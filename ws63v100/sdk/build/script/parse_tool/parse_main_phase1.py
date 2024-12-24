#!/usr/bin/env python
# coding:utf-8
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

import sys,string,re,os
sys.path.append(os.path.split(os.path.realpath(__file__))[0])
from ctypes import *
from parse_elf import *
import json

if __name__ == '__main__':
    print("Running parse step1 at", sys.executable,
          " version:%d.%d.%d " % (sys.version_info[0], sys.version_info[1], sys.version_info[2]))
    out_dir = sys.argv[1]
    debug_file = sys.argv[2]
    nm_file = sys.argv[3]
    xml_file = sys.argv[4]

    if not os.path.isdir(out_dir):
        os.makedirs(out_dir)
    parse_elf_step_1(debug_file,
        os.path.join(out_dir, 'auto_class.py'),
        os.path.join(out_dir, 'auto_struct.txt'),
        nm_file,
        os.path.join(out_dir, 'global.txt'),
        out_dir,
        xml_file
    )
    print("Build parse tool success.")
