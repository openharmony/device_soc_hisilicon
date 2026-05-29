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

SCRIPT_DIR = os.path.dirname(__file__)
sys.path.append(SCRIPT_DIR)

import logger as log

def check_bit_len(val, length, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    n_bits = len(val) * 8
    if n_bits != length:
        log.error('%sExpected %d bits, not %d.' % (notice, length, n_bits))
        sys.exit(1)
    return

def check_bytes_len(val, length, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    n_bytes = len(val)
    if n_bytes != length:
        log.error('%sExpected %d bytes, not %d.' % (notice, length, n_bytes))
        sys.exit(1)
    return

def check_max_bytes_len(val, length, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    n_bytes = len(val)
    if n_bytes > length:
        log.error('%sMust less than %d bytes, now is %d.' % (notice, length, n_bytes))
        sys.exit(1)
    return

def check_range(val, minimum=None, maximum=None, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    if minimum != None and val < minimum:
        log.error('%s%d is less than %d.' % (notice, val, minimum))
        sys.exit(1)
    if maximum != None and val > maximum:
        log.error('%s%d is larger than %d.' % (notice, val, maximum))
        sys.exit(1)
    return

def check_file_exist(file_name, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    if not os.path.exists(file_name):
        log.error('%s%s : No such file.' % (notice, file_name))
        sys.exit(1)
    return

def check_option(val, options, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    if val not in options:
        log.error('%sRequire %s, not %s' % (notice, str(options), val))
        sys.exit(1)
    return

def check_equal(val1, val2, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    if val1 != val2:
        log.error('%s%s is not equal to %s.' % (notice, val1, val2))
        sys.exit(1)
    return

def check_type(val, val_type, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    if not isinstance(val, val_type):
        log.error('%sRequire %s. not %s.' % (notice, str(val_type), str(type(val))))
        sys.exit(1)
    return

def check_empty(val, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    if val == None or val == '' or val == b'':
        log.error('%sVariable cannot be None or empty.' % (notice))
        sys.exit(1)
    return

def check_key_in_dict(key, dic, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    if key not in dic:
        log.error('%sKey \'%s\' not found.' % (notice, key))
        sys.exit(1)
    return

def check_hex_str(hex_str, notice=''):
    notice = '%s: ' % notice if notice != '' else ''
    try:
        int(hex_str, 16)
    except Exception:
        log.error('%sRequire a hexadecimal value, not \'%s\'' % (notice, hex_str))
        sys.exit(1)
    return

def check_match(a, b, val_name='Values'):
    if a != b:
        log.error('{} mismatch'.format(val_name))
        sys.exit(1)
    return

def check_ok(ok, action_name='Check'):
    if not ok:
        log.error('%s failed.' % action_name)
        sys.exit(1)
    return

def check_greater_equal(large, small, val_name='Values'):
    if large < small:
        log.error('%s mismatch (%#x < %#x).' % (val_name, large, small))
        sys.exit(1)
    return