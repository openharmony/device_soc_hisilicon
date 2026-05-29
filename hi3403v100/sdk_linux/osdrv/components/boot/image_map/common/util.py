"""
Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
import os
import sys
from sys import version_info
import binascii
import subprocess as sp
import shutil

SCRIPT_DIR = os.path.dirname(__file__)
sys.path.append(SCRIPT_DIR)

import logger as log
import check as chk

def run_cmd(cmd, stdin=None):
    args = cmd
    if version_info.major == 2 and isinstance(args, unicode):
        args = args.encode('utf-8')
    if isinstance(args, str):
        args = args.split(' ')
    po = sp.Popen(args=args, stdin=sp.PIPE, stdout=sp.PIPE, stderr=sp.PIPE)
    stdout, stderr = po.communicate(input=stdin)
    return stdout, stderr, po.returncode

def str2hex(hex_str):
    if hex_str[0:2] == '0x' or hex_str[0:2] == '0X':
        hex_str = hex_str[2:]
    if len(hex_str) % 2 != 0:
        hex_str = '0%s' % hex_str
    return binascii.unhexlify(hex_str)

def hex2str(hex_val):
    hex_str = binascii.hexlify(hex_val).upper()
    if version_info.major >= 3:
        hex_str = hex_str.decode('utf-8')
    return hex_str

def align(byte_array, n_align_bytes, fill=0x00, to_back=True):
    chk.check_empty(byte_array)
    n_bytes = len(byte_array)
    n_pads = (n_align_bytes - (n_bytes % n_align_bytes)) % n_align_bytes
    n_bytes += n_pads
    fill = str2hex('%02x' % fill)
    if to_back:
        byte_array = byte_array + fill * n_pads
    else:
        byte_array = fill * n_pads + byte_array
    return byte_array

def reverse_endian(byte_array, group_size):
    ret = None
    byte_array = align(byte_array, group_size, to_back=False)
    n_bytes = len(byte_array)
    for i in range(0, n_bytes, group_size):
        if ret == None:
            ret = byte_array[i:i+group_size][::-1]
        else:
            ret += byte_array[i:i+group_size][::-1]
    return ret

def str2le(hex_str, group_size):
    # to hexdump
    hexdump = str2hex(hex_str)
    # to little endian
    hexdump = reverse_endian(hexdump, group_size)
    return hexdump

def file_size(file_name):
    chk.check_file_exist(file_name)
    return os.path.getsize(file_name)

def read_file(file_name, data_name=None, binary=True):
    chk.check_file_exist(file_name)
    mode = 'rb' if binary else 'r'
    if data_name is not None:
        log.info('Loading %s from %s' % (data_name, file_name))
    with open(file_name, mode) as f:
        data = f.read()
    return data

def write_file(file_name, data, data_name=None):
    mode = 'w' if isinstance(data, str) else 'wb'
    if data_name is not None:
        log.info('Writing %s to %s' % (data_name, file_name))
    with open(file_name, mode) as f:
        data = f.write(data)
    return

def combine_bytes(*byte_arrays):
    ret = binascii.unhexlify('')
    for byte_arr in byte_arrays:
        ret += byte_arr
    return ret

def replace_bytes(old_bytes, pos, new_bytes):
    return old_bytes[:pos] + new_bytes + old_bytes[(pos + len(new_bytes)):]

# def combine_files(out_file, *in_files):
#     for in_file in in_files:
#         chk.check_file_exist(in_file)

#     with open(out_file, 'wb') as out_f:
#         for in_file in in_files:
#             with open(in_file, 'rb') as in_f:
#                 out_f.write(in_f.read())
#     return

def hex2int(hex_val):
    return int(hex2str(hex_val), 16)

def le2int(hex_val, group_size):
    val = reverse_endian(hex_val, group_size)
    return hex2int(val)

def rm_file(path, data_name=None):
    if data_name is not None:
        log.info('Remove \'%s\'.' % path)
    if os.path.isfile(path):
        os.remove(path)
    elif os.path.isdir(path):
        shutil.rmtree(path)

def choose_options(title, options, try_times=5):
    tips = "\n%s:\n" % title
    for i, opt in enumerate(options):
        tips += '%d. %s\n' % (i, opt)
    tips += 'Please select the number'
    print(tips)

    while (try_times > 0):
        try_times -= 1
        try:
            num = int(input('> '))
        except Exception:
            print('\nRetry. Required an integer.')
            continue

        if num in range(0, len(options)):
            return num, options[num]
        print('\nRetry. Input should be from 0 to %d.' % (len(options) - 1))

    log.error('Wrong input.')
    sys.exit(1)
    return None, None