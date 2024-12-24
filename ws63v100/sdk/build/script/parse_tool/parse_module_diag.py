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
from ctypes import *
from auto_class import *
from config import *

class _PhaseCtrl:
    def __init__(self):
        self.dfx_stat=None

    def make_diag_info(self):
        addr = parse_get_symbol_addr('g_zdiag_dfx_stat')
        obj=parse_memory_2_class(addr,zdiag_dfx_stat,sizeof(zdiag_dfx_stat))
        self.dfx_stat=obj
    def print_diag_info(self):
        uapi_print_ctypes_obj('diag_dfx_stat',self.dfx_stat)

_g_parse_ctrl=_PhaseCtrl()

def parase_diag_info():
    _g_parse_ctrl.make_diag_info()
    _g_parse_ctrl.print_diag_info()