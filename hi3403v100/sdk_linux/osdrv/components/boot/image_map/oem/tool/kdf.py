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

import sys
import os

SCRIPT_DIR = os.path.dirname(__file__)
sys.path.append(SCRIPT_DIR)

import common.logger as log
import common.util as utl
import common.check as chk

class KdfTool:
    def __init__(self, root_symc_key, rk_deob_en, oem_static=1):
        self.__root_symc_key = root_symc_key
        self.__rk_deob_en = 1 if rk_deob_en else 0
        self.__oem_static = oem_static
        self.__kdf_exec = './KDF_Linux'
        self.__param_file = './parameter.bin'

    def set_kdf_exec(self, exec_file):
        self.__kdf_exec = exec_file

    def set_param_file(self, param_file):
        self.__param_file = param_file

    def __run_kdf(self, cfg_items):
        chk.check_file_exist(self.__kdf_exec, 'KDFTool')
        chk.check_file_exist(self.__param_file, 'param_file_path')

        tmp_kdf_ini = 'tmp_kdf.ini'
        tmp_kdf_out = 'tmp_kdf_out.bin'

        ini_items = [
            'param_file_path = %s'   % self.__param_file,
            'oem_static = %s'        % self.__oem_static,
            'oem_rk_deob_en = %s'    % self.__rk_deob_en,
            'oem_root_symc_key = %s' % utl.hex2str(self.__root_symc_key),
            'output_file_path = %s'  % tmp_kdf_out,
        ]
        ini_items.extend(cfg_items)
        ini_text = "\n".join(ini_items)
        utl.write_file(tmp_kdf_ini, ini_text)

        cmd = '%s %s' % (self.__kdf_exec, tmp_kdf_ini)
        stdout, _, _= utl.run_cmd(cmd)
        stdout = stdout.decode("utf-8") if sys.version_info.major > 2 else stdout
        if stdout.find('Processed successfully !') == -1:
            log.error('KDF Failed!\nKDF: %s' % stdout)
            exit(1)

        key = utl.read_file(tmp_kdf_out)
        utl.rm_file(tmp_kdf_ini)
        utl.rm_file(tmp_kdf_out)
        return key

    def derive_rootkey(self, boot_flag, sw_reg):
        return self.__run_kdf([
            'function = 2',
            'encryption = 0',
            'boot_flag = %s' % boot_flag,
            'sw_reg = %s'    % '{:08x}'.format(sw_reg),
        ])

    def derive_l2(self, boot_flag, sw_reg, prot_l1_enc, prot_l2_enc):
        return self.__run_kdf(cfg_items = [
            'function = 3',
            'encryption = 0',
            'boot_flag = %s' % boot_flag,
            'sw_reg = %s'    % '{:08x}'.format(sw_reg),
            'protection_key_l1 = %s' % utl.hex2str(prot_l1_enc),
            'protection_key_l2 = %s' % utl.hex2str(prot_l2_enc),
        ])