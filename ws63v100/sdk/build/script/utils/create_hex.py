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

import sys
import struct


def create_hex_file(_bin, _hex):
    with open(str(_bin), "rb") as binfile, open(
                str(_hex), "wb") as hexfile:
        while True:
            bindata = binfile.read(4)
            if not bindata:
                break
            if len(bindata) != 4:
                bindata = bindata + b'\x00' * (4 - len(bindata))
            longdata, = struct.unpack("<L", bindata)
            hexstr = '{:x}'.format(longdata)  # dec to hex number str
            hexstr = '%s\r\n' % '{:0>8}'.format(hexstr).upper()
            hexfile.write(str.encode(hexstr))

if __name__ == "__main__":
    create_hex_file(sys.argv[1], sys.argv[2])
