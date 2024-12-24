#!/usr/bin/env python3
# encoding=utf-8
# ============================================================================
# @brief    build system entry, receive param & start to build

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
# ============================================================================

import struct
import sys

def concat_bins(file1, file2, file1_size, output):
    # 读取两个bin文件
    with open(file1, 'rb') as f1, open(file2, 'rb') as f2:
        data_file1 = f1.read()
        data_file2 = f2.read()

    with open(output, 'wb') as out:
        out.seek(0, 0)
        out.write(data_file1)
        out.seek(int(file1_size, 16), 0)
        out.write(data_file2)

def main():
    # 获取命令行传入的参数列表
    args = sys.argv[1:]
    # 判断参数是否为文件路径
    if len(args) == 4:
        concat_bins(args[0], args[1], args[2], args[3])
    else:
        sys.exit(0)

if __name__ == '__main__':
    main()
