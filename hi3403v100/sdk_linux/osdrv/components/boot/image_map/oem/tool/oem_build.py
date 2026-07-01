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
SCRIPT_DIR = SCRIPT_DIR if len(SCRIPT_DIR) != 0 else '.'
WORK_DIR = os.path.abspath('%s/../..' % SCRIPT_DIR)
sys.path.append(WORK_DIR)

import common.logger as log
import common.util as utl
import common.check as chk
import common.config as cfg
from common.config import CfgValue
from common.area_tool import PublicKey, AreaTool
from common.area_tool import AreaCfg

class OemAreaBuilder:
    def __init__(self, cfgtor):
        self.cfgtor = cfgtor    # Configuartor
        self.area_tool = AreaTool(self.cfgtor.scenario().crypto_alg())
        self.boot_area_file = None

        if self.cfgtor.scenario().is_sec_boot_enable():
            self.cfgtor.reset_path()

        self.cfgtor.reset_path('OEM_Info')
        self.msid         = self.cfgtor.get_value('OEM_MSID_Ext',         utl.str2hex('00') * 4)
        self.mask_msid    = self.cfgtor.get_value('Mask_OEM_MSID_Ext',    utl.str2hex('00') * 4)
        self.version      = self.cfgtor.get_value('OEM_Version_Ext',      utl.str2hex('00') * 4)
        self.mask_version = self.cfgtor.get_value('Mask_OEM_Version_Ext', utl.str2hex('00') * 4)

    def build_root_pub_key_area(self, area_out_file, checksum_out_file):
        log.debug('------------- Build OEM_Root_Public_Key Area -------------')

        area = self.area_tool.gen_empty_area(size=0x400, fill=0x00)

        if self.cfgtor.scenario().is_sec_boot_enable():
            self.cfgtor.reset_path('Root', 'OEM_Root_Public_Key_Area')
            oem_key_file = self.cfgtor.get_value('OEM_Root_Public_Key')
            if oem_key_file == None:
                self.cfgtor.reset_path('Root')
                oem_key_file = self.cfgtor.get_value('OEM_Root_Private_Key')

            pub_key = self.area_tool.import_pub_key(
                oem_key_file,
                key_name='OEM_Root_Public_Key'
            )

            self.cfgtor.reset_path('Root', 'OEM_Root_Public_Key_Area')
            l1_enc  = self.cfgtor.get_value('OEM_ProtectionKey_L1_Enc')
            l2_enc  = self.cfgtor.get_value('OEM_ProtectionKey_L2_Enc')
            l1_clr  = self.cfgtor.get_value('OEM_ProtectionKey_L1')
            l2_clr  = self.cfgtor.get_value('OEM_ProtectionKey_L2')
            iv      = self.cfgtor.get_value('OEM_Public_IV')
            rootkey = self.cfgtor.get_value('OEM_Public_RootKey')
            # encryt Root_Public_Key
            l1_enc, l1_clr = self.area_tool.try_convert_enc_dec(l1_enc, l1_clr, rootkey)
            l2_enc, l2_clr = self.area_tool.try_convert_enc_dec(l2_enc, l2_clr, l1_clr)
            pub_key_enc    = self.area_tool.enc_pub_key(pub_key, l2_clr, iv)
            reserved       = utl.str2hex('0x00') * 0x180
            area           = utl.combine_bytes(pub_key_enc, l1_enc, l2_enc, iv, reserved)
            # calculate the checksum written to OTP
            chksum_str = '0x' + utl.hex2str(self.area_tool.digest(area))
            utl.write_file(checksum_out_file, chksum_str, 'OEM_Root_Public_Key Area Checksum')

        utl.write_file(area_out_file, area, 'OEM_Root_Public_Key Area')
        log.debug('Done.')

    def __get_boot_enc_dec_protkey(self):
        self.cfgtor.reset_path('Boot', 'Boot_Params_Area')
        rootkey = self.cfgtor.get_value('FlashProt_MasterKey_Boot')
        l1_enc  = self.cfgtor.get_value('Flash_ProtectionKey_Boot_L1_Enc')
        l2_enc  = self.cfgtor.get_value('Flash_ProtectionKey_Boot_L2_Enc')
        l1_clr  = self.cfgtor.get_value('Flash_ProtectionKey_Boot_L1')
        l2_clr  = self.cfgtor.get_value('Flash_ProtectionKey_Boot_L2')
        iv      = self.cfgtor.get_value('Flash_IV_Boot')
        l1_enc, l1_clr = self.area_tool.try_convert_enc_dec(l1_enc, l1_clr, rootkey)
        l2_enc, l2_clr = self.area_tool.try_convert_enc_dec(l2_enc, l2_clr, l1_clr)
        return l1_enc, l2_enc, l1_clr, l2_clr, iv

    def build_boot_key_area(self, out_file):
        log.debug('------------------ Build Boot_Key Area ------------------')
        self.cfgtor.reset_path('Root')
        oem_root_priv_key_file = self.cfgtor.get_value('OEM_Root_Private_Key')

        pub_key = PublicKey(
            n=utl.str2hex('00' * 0x200),
            e=utl.str2hex('00' * 0x4)
        )
        if self.cfgtor.scenario().is_sec_boot_enable:
            self.cfgtor.reset_path('Boot', 'Boot_Key_Area')
            ree_key_file = self.cfgtor.get_value('REE_External_Public_Key')
            if ree_key_file == None:
                self.cfgtor.reset_path('Boot')
                ree_key_file = self.cfgtor.get_value('REE_External_Private_Key')

            pub_key = self.area_tool.import_pub_key(
                ree_key_file,
                key_name='REE_External_Public_Key'
            )

        data_to_sign = utl.combine_bytes(
            utl.str2le('0x3C78963C', group_size=4),
            utl.str2le('0x00000000', group_size=4),
            utl.str2hex('00') * 0x4,
            pub_key.e,
            pub_key.n,
            self.msid,
            self.mask_msid,
            self.version,
            self.mask_version,
            utl.str2hex('00') * 0x1E0,
        )

        sign_key = self.area_tool.sign(
            data=data_to_sign,
            priv_key_file=oem_root_priv_key_file,
            data_name='Boot_Key Area'
        )

        area = utl.combine_bytes(
            data_to_sign,
            sign_key
        )

        utl.write_file(out_file, area, 'Boot_Key Area')
        log.debug('Done.')
        return

    def build_boot_params_area(self, out_file):
        log.debug('----------------- Build Boot_Params Area -----------------')
        self.cfgtor.reset_path('Boot')
        ree_priv_key_file = self.cfgtor.get_value('REE_External_Private_Key')
        self.cfgtor.reset_path('Boot', 'Boot_Area')
        unchked_area     = utl.align(self.cfgtor.get_value('Unchecked_Area'), n_align_bytes=16)
        self.cfgtor.reset_path('Boot', 'Boot_Params_Area')
        boot_entry_point = self.cfgtor.get_value('Boot_Entry_Point')
        boot_enc_flag    = self.cfgtor.get_value('Boot_Enc_Flag')
        cfg_param        = self.cfgtor.get_value('Cfg_Param')

        chk.check_empty(self.boot_area_file, 'Boot Area is not built yet')
        chk.check_file_exist(self.boot_area_file, 'Boot_Code Area')
        chk.check_greater_equal(AreaCfg.CFG_PARAM_SIZE, len(cfg_param), "CFG_PARAM_SIZE")

        boot_area_len    = os.path.getsize(self.boot_area_file)
        unchked_area_len = len(unchked_area)
        checked_area_len = boot_area_len - unchked_area_len - 0x400

        l1_enc = utl.str2hex('00' * 0x10)
        l2_enc = utl.str2hex('00' * 0x20)
        iv     = utl.str2hex('00' * 0x10)
        if self.cfgtor.scenario().is_boot_enc():
            l1_enc, l2_enc, _, _, iv = self.__get_boot_enc_dec_protkey()

        data_to_sign = utl.combine_bytes(
            utl.str2le('0x3C78692D',            group_size=4),
            utl.str2le('0x00000000',            group_size=4),
            boot_entry_point,
            utl.str2le('%x' % unchked_area_len, group_size=4),
            utl.str2le('%x' % checked_area_len, group_size=4),
            utl.str2le('%x' % boot_area_len,    group_size=4),
            boot_enc_flag,
            l1_enc,
            l2_enc,
            iv,
            utl.str2hex('00') * 0x40,
            utl.align(cfg_param, n_align_bytes=AreaCfg.CFG_PARAM_SIZE),
            self.msid,
            self.mask_msid,
            self.version,
            self.mask_version
        )

        sign_params = self.area_tool.sign(
            data=data_to_sign,
            priv_key_file=ree_priv_key_file,
            data_name='Boot_Params Area'
        )

        area = utl.combine_bytes(
            data_to_sign,
            sign_params
        )

        utl.write_file(out_file, area, 'Build Boot_Params Area')
        log.debug('Done.')
        return

    def build_unchecked_area_for_vendor(self, out_file):
        log.debug('------------- Build Unchecked Area for Vendor -------------')
        self.cfgtor.reset_path('Boot', 'Unchecked_Area_for_Vendor')
        scs_simulate_flag = self.cfgtor.get_value('SCS_simulate_flag')

        area = utl.combine_bytes(
            scs_simulate_flag,
            utl.str2hex('00') * 0x3C,
        )

        utl.write_file(out_file, area, 'Unchecked Area for Vendor')
        log.debug('Done.')
        return

    def build_boot_area(self, out_file):
        log.debug('-------------------- Build Boot Area --------------------')
        self.cfgtor.reset_path('Boot')
        ree_priv_key_file = self.cfgtor.get_value('REE_External_Private_Key')
        self.cfgtor.reset_path('Boot', 'Boot_Area')
        unchked_area = utl.align(self.cfgtor.get_value('Unchecked_Area'), n_align_bytes=16)
        boot_code    = utl.align(self.cfgtor.get_value('Boot_Code'), n_align_bytes=16)

        boot_code_id      = utl.str2le('0x3C78693C', group_size=4)
        structure_version = utl.str2le('0x00000000', group_size=4)
        reserved          = utl.str2hex('00') * 0x8

        data_to_sign = utl.combine_bytes(
            boot_code_id,
            structure_version,
            reserved,
            boot_code,
            self.msid,
            self.mask_msid,
            self.version,
            self.mask_version
        )
        sign_boot_oem = self.area_tool.sign(
            data=data_to_sign,
            priv_key_file=ree_priv_key_file,
            data_name='Boot Area'
        )

        if self.cfgtor.scenario().is_boot_enc():
            _, _, _, l2_clr, iv = self.__get_boot_enc_dec_protkey()
            boot_code = self.area_tool.enc_cbc(boot_code, l2_clr, iv, data_name='Boot code')

        sign_boot_tp = utl.str2hex('00') * AreaCfg.SIGNATURE_SIZE

        area = utl.combine_bytes(
            unchked_area,
            boot_code_id,
            structure_version,
            reserved,
            boot_code,
            self.msid,
            self.mask_msid,
            self.version,
            self.mask_version,
            sign_boot_oem,
            sign_boot_tp
        )

        utl.write_file(out_file, area, 'Boot Area')
        log.debug('Done.')

        self.boot_area_file = out_file
        return

class OemImageBuilder:
    def __init__(self, cfgtor):
        self.cfgtor = cfgtor
        self.area_tool = AreaTool(self.cfgtor.scenario().crypto_alg())
        self.vendor_root_pub_key_area_file = None
        self.oem_root_pub_key_area_file    = None
        self.gsl_key_area_file             = None
        self.gsl_code_area_file            = None
        self.boot_key_area_file            = None
        self.boot_params_area_file         = None
        self.unchk_area_for_vendor_file    = None
        self.boot_area_file                = None
        self.tee_key_area_file             = None
        self.atf_area_file                 = None
        self.tee_code_area_file            = None

    def build_images(self, boot_out_file, tee_out_file=None):
        self._build_boot_image(boot_out_file)
        if self.cfgtor.scenario().is_tee_enbale():
            self._build_tee_image(tee_out_file)
        return

    def _build_boot_image(self, out_file):
        log.debug('=========================================================')
        log.debug('                    Build Boot Image')
        log.debug('---------------------------------------------------------')
        log.debug('        Input               |          File')
        log.debug('----------------------------+------------------------------')
        if self.cfgtor.scenario().tee_owner() == 'Vendor':
            log.debug('Vendor_Root_Public_Key Area | %s' % self.vendor_root_pub_key_area_file)
        log.debug('OEM_Root_Public_Key Area    | %s' % self.oem_root_pub_key_area_file)
        log.debug('GSL_Key Area                | %s' % self.gsl_key_area_file)
        log.debug('GSL_Code Area               | %s' % self.gsl_code_area_file)
        log.debug('Boot_Key Area               | %s' % self.boot_key_area_file)
        log.debug('Boot_Params Area            | %s' % self.boot_params_area_file)
        log.debug('Unchcked Area for Vendor    | %s' % self.unchk_area_for_vendor_file)
        log.debug('Boot Area                   | %s' % self.boot_area_file)
        log.debug('---------------------------------------------------------')

        chk.check_empty(self.oem_root_pub_key_area_file, 'OEM_Root_Public_Key Area')
        chk.check_empty(self.gsl_key_area_file, 'GSL_KEY Area')
        chk.check_empty(self.gsl_code_area_file, 'GSL_Code Area')
        chk.check_empty(self.boot_key_area_file, 'Boot_Key Area')
        chk.check_empty(self.boot_params_area_file, 'Boot_Params Area')
        chk.check_empty(self.unchk_area_for_vendor_file, 'Unchcked Area for Vendor')
        chk.check_empty(self.boot_area_file, 'Boot Area')

        vendor_root_pub_key_area = self.area_tool.gen_empty_area(size=0x400, fill=0x00)
        oem_root_pub_key_area    = utl.read_file(self.oem_root_pub_key_area_file)
        tp_root_pub_key_area     = self.area_tool.gen_empty_area(size=0x400, fill=0x00)
        gsl_tp_key_area          = self.area_tool.gen_empty_area(size=0x600, fill=0x00)
        gsl_key_area             = utl.read_file(self.gsl_key_area_file)
        gsl_code_area            = utl.read_file(self.gsl_code_area_file)
        boot_key_area            = utl.read_file(self.boot_key_area_file)
        boot_params_area         = utl.read_file(self.boot_params_area_file)
        unchk_area_for_vendor    = utl.read_file(self.unchk_area_for_vendor_file)
        boot_area                = utl.read_file(self.boot_area_file)

        if self.cfgtor.scenario().tee_owner() == 'Vendor':
            chk.check_empty(self.vendor_root_pub_key_area_file, 'Vendor_Root_Public_Key Area')
            vendor_root_pub_key_area = utl.read_file(self.vendor_root_pub_key_area_file)

        image = utl.combine_bytes(
            vendor_root_pub_key_area,
            oem_root_pub_key_area,
            tp_root_pub_key_area,
            gsl_tp_key_area,
            gsl_key_area,
            gsl_code_area,
            boot_key_area,
            boot_params_area,
            unchk_area_for_vendor,
            boot_area
        )

        utl.write_file(out_file, image, 'Boot Image')
        log.debug('Done')

        return

    def _build_tee_image(self, out_file):
        log.debug('=========================================================')
        log.debug('                    Build TEE Image')
        log.debug('---------------------------------------------------------')
        log.debug('        Input             |          File')
        log.debug('--------------------------+------------------------------')
        log.debug('TEE Key Area              | %s' % self.tee_key_area_file)
        log.debug('ATF Area                  | %s' % self.atf_area_file)
        log.debug('TEE_Code Area             | %s' % self.tee_code_area_file)
        log.debug('---------------------------------------------------------')

        chk.check_empty(self.tee_key_area_file, 'TEE Key Area')
        chk.check_empty(self.atf_area_file, 'GSL_KEY Area')
        chk.check_empty(self.tee_code_area_file, 'TEE_Code Area')

        tee_key_area  = utl.read_file(self.tee_key_area_file)
        atf_area      = utl.read_file(self.atf_area_file)
        tee_code_area = utl.read_file(self.tee_code_area_file)

        image = utl.combine_bytes(
            tee_key_area,
            atf_area,
            tee_code_area
        )

        utl.write_file(out_file, image, 'TEE Image')
        log.debug('Done')

        return

class OEM_Checker:
    def __init__():
        pass