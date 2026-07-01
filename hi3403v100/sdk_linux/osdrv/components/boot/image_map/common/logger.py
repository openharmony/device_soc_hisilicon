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

from __future__ import print_function
import traceback
import sys

class LOG_LEVEL:
    DEBUG = 1
    INFO  = 2
    WARN  = 3
    ERROR = 4

# defualt
CURR_LEVEL = LOG_LEVEL.INFO

def print_to_stderr(msg):
    print(msg, file=sys.stderr)

def print_to_stdout(msg):
    print(msg, file=sys.stdout)

def error(err_msg):
    if CURR_LEVEL > LOG_LEVEL.ERROR:
        return
    print_to_stderr('ERROR: %s' % err_msg)
    traceback.print_stack()

def warn(warn_msg):
    if CURR_LEVEL > LOG_LEVEL.WARN:
        return
    print_to_stderr('WARNING: %s' % warn_msg)

def info(info_msg):
    if CURR_LEVEL > LOG_LEVEL.INFO:
        return
    print_to_stdout('INFO : %s' % info_msg)

def debug(info_msg):
    if CURR_LEVEL > LOG_LEVEL.DEBUG:
        return
    print_to_stdout('DEBUG: %s' % info_msg)