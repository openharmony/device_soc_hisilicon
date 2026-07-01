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
from common.area_tool import PublicKey, AreaTool, AreaCfg

class TeeAreaBuilder:
    def __init__(self, cfgtor):
        cfgtor.reset_path()
        self.cfgtor = cfgtor    # Configuartor
        self.area_tool = AreaTool(self.cfgtor.scenario().crypto_alg())
        self.gsl_code_area_file     = None
        self.atf_area_file          = None
        self.tee_code_area_file     = None

    def __get_enc_dec_protkey(self, cfgtor, prefix):
        rootkey = cfgtor.get_value(prefix + '_RootKey')
        l1_enc  = cfgtor.get_value(prefix + '_ProtectionKey_L1_Enc')
        l2_enc  = cfgtor.get_value(prefix + '_ProtectionKey_L2_Enc')
        l1_clr  = cfgtor.get_value(prefix + '_ProtectionKey_L1')
        l2_clr  = cfgtor.get_value(prefix + '_ProtectionKey_L2')
        iv      = cfgtor.get_value(prefix + '_IV')
        l1_enc, l1_clr = self.area_tool.try_convert_enc_dec(l1_enc, l1_clr, rootkey)
        l2_enc, l2_clr = self.area_tool.try_convert_enc_dec(l2_enc, l2_clr, l1_clr)
        return l1_enc, l2_enc, l1_clr, l2_clr, iv

    def build_gsl_key_area(self, out_file):
        owner = self.cfgtor.scenario().tee_owner()
        log.debug('------------------- Build GSL_Key Area -------------------')
        chk.check_empty(self.gsl_code_area_file, 'GSL_Code Area is not built yet')
        chk.check_file_exist(self.gsl_code_area_file, 'GSL_Code Area')

        self.cfgtor.reset_path('Root')
        owner_priv_key_file = self.cfgtor.get_value('%s_Root_Private_Key' % owner)
        self.cfgtor.reset_path('OEM_Info')
        oem_msid_ext      = self.cfgtor.get_value('OEM_MSID_Ext',         utl.str2hex('00') * 4)
        mask_oem_msid_ext = self.cfgtor.get_value('Mask_OEM_MSID_Ext',    utl.str2hex('00') * 4)
        oem_version_ext   = self.cfgtor.get_value('OEM_Version_Ext',      utl.str2hex('00') * 4)
        mask_oem_ver_ext  = self.cfgtor.get_value('Mask_OEM_Version_Ext', utl.str2hex('00') * 4)
        self.cfgtor.reset_path('GSL', 'GSL_Key_Area')
        gsl_code_enc_flag = self.cfgtor.get_value('GSL_Code_Enc_Flag')

        pub_key = PublicKey(
            n=utl.str2hex('00' * 0x200),
            e=utl.str2hex('00' * 0x4)
        )
        if self.cfgtor.scenario().is_sec_boot_enable():
            self.cfgtor.reset_path('GSL', 'GSL_Key_Area')
            gsl_key_file = self.cfgtor.get_value('GSL_External_Public_Key')
            if gsl_key_file == None:
                self.cfgtor.reset_path('GSL')
                gsl_key_file = self.cfgtor.get_value('GSL_External_Private_Key')

            pub_key = self.area_tool.import_pub_key(
                gsl_key_file,
                key_name='GSL_External_Public_Key'
            )

        l1_enc = utl.str2hex('00' * 0x10)
        l2_enc = utl.str2hex('00' * 0x20)
        iv     = utl.str2hex('00' * 0x10)
        if self.cfgtor.scenario().is_gsl_enc():
            self.cfgtor.reset_path('GSL', 'GSL_Key_Area')
            l1_enc, l2_enc, _, _, iv = self.__get_enc_dec_protkey(self.cfgtor, prefix='GSL')

        gsl_code_area_len = os.path.getsize(self.gsl_code_area_file)

        data_to_sign = utl.combine_bytes(
            utl.str2le('0x3C78962D', group_size=4),
            utl.str2le('0x00000000', group_size=4),
            utl.str2le('%x' %  gsl_code_area_len, group_size=4),
            pub_key.e,
            pub_key.n,
            oem_msid_ext,
            mask_oem_msid_ext,
            oem_version_ext,
            mask_oem_ver_ext,
            l1_enc,
            l2_enc,
            iv,
            gsl_code_enc_flag,
            utl.str2hex('00') * 0x19C,
        )
        sign_gsl_key = self.area_tool.sign(
            data=data_to_sign,
            priv_key_file=owner_priv_key_file ,
            data_name='GSL_Key Area'
        )

        area = utl.combine_bytes(
            data_to_sign,
            sign_gsl_key
        )

        utl.write_file(out_file, area, 'GSL_Key Area')
        log.debug('Done.')
        return

    def build_gsl_code_area(self, out_file):
        log.debug('------------------ Build GSL_Code Area ------------------')
        self.cfgtor.reset_path('GSL')
        gsl_priv_key_file = self.cfgtor.get_value('GSL_External_Private_Key')
        self.cfgtor.reset_path('GSL', 'GSL_Code_Area')
        gsl_image_info = utl.align(self.cfgtor.get_value('GSL_Image_Info'), n_align_bytes=0x20)
        gsl_code       = utl.align(self.cfgtor.get_value('GSL_Code'), n_align_bytes=0x20)

        gsl_code_id       = utl.str2le('0x3C78691E', group_size=4)
        structure_version = utl.str2le('0x00000000', group_size=4)
        reserved0         = utl.str2hex('00') * 0x18

        data_to_sign = utl.combine_bytes(
            gsl_code_id,
            structure_version,
            reserved0,
            gsl_image_info,
            gsl_code,
        )
        sign_gsl_code = self.area_tool.sign(
            data=data_to_sign,
            priv_key_file=gsl_priv_key_file,
            data_name='GSL_Code Area'
        )

        if self.cfgtor.scenario().is_gsl_enc():
            self.cfgtor.reset_path('GSL', 'GSL_Key_Area')
            _, _, _, l2_clr, iv = self.__get_enc_dec_protkey(self.cfgtor, prefix='GSL')
            gsl_code = self.area_tool.enc_cbc(gsl_code, l2_clr, iv, data_name='GSL code')

        sign_gsl_code_tp = utl.str2hex('00') * AreaCfg.SIGNATURE_SIZE

        area = utl.combine_bytes(
            gsl_code_id,
            structure_version,
            reserved0,
            gsl_image_info,
            gsl_code,
            sign_gsl_code,
            sign_gsl_code_tp
        )

        utl.write_file(out_file, area, 'GSL_Code Area')
        log.debug('Done.')

        self.gsl_code_area_file = out_file
        return

    def build_tee_key_area(self, out_file):
        log.debug('------------------- Build TEE_Key Area -------------------')
        chk.check_empty(self.atf_area_file, 'ATF Area is not built yet')
        chk.check_empty(self.tee_code_area_file, 'TEE_Code Area is not built yet')
        chk.check_file_exist(self.atf_area_file, 'ATF Area')
        chk.check_file_exist(self.tee_code_area_file, 'TEE_Code Area')

        self.cfgtor.reset_path('GSL')
        gsl_priv_key_file = self.cfgtor.get_value('GSL_External_Private_Key')
        self.cfgtor.reset_path('TEE', 'TEE_Key_Area')
        atf_l1_enc, atf_l2_enc, _, _, atf_iv = self.__get_enc_dec_protkey(self.cfgtor, prefix='ATF')
        tee_l1_enc, tee_l2_enc, _, _, tee_iv = self.__get_enc_dec_protkey(self.cfgtor, prefix='TEE')
        tee_msid_ext      = self.cfgtor.get_value('TEE_MSID_Ext')
        mask_tee_msid_ext = self.cfgtor.get_value('Mask_TEE_MSID_Ext')
        tee_version_ext   = self.cfgtor.get_value('TEE_SecVersion_Ext')
        mask_tee_ver_ext  = self.cfgtor.get_value('Mask_TEE_SecVersion_Ext')
        tee_func_ver      = self.cfgtor.get_value('TEE_Func_Version')
        tee_ddr_addr      = self.cfgtor.get_value('TEE_DDR_Address')
        tee_ddr_size      = self.cfgtor.get_value('TEE_Reserved_DDR_Size')
        tee_key_file      = self.cfgtor.get_value('TEE_External_Public_Key')
        if tee_key_file == None:
            log.info("TEE_External_Public_Key is not exits.")
            self.cfgtor.reset_path('TEE',)
            tee_key_file   = self.cfgtor.get_value('TEE_External_Private_Key')
        pub_key = self.area_tool.import_pub_key(tee_key_file, key_name='TEE_External_Public_Key')

        atf_area_len      = os.path.getsize(self.atf_area_file)
        tee_code_area_len = os.path.getsize(self.tee_code_area_file)

        data_to_sign = utl.combine_bytes(
            utl.str2le('0x3C78964B', group_size=4),
            utl.str2le('0x00000000', group_size=4),
            utl.str2le('%x' % tee_code_area_len, group_size=4),
            pub_key.e,
            pub_key.n,
            tee_msid_ext,
            mask_tee_msid_ext,
            tee_version_ext,
            mask_tee_ver_ext,
            atf_l1_enc,
            atf_l2_enc,
            tee_l1_enc,
            tee_l2_enc,
            atf_iv,
            tee_iv,
            tee_func_ver,
            utl.str2le('%x' %  atf_area_len, group_size=4),
            tee_ddr_addr,
            tee_ddr_size,
            utl.str2hex('00') * 0x14C,
        )
        sign_tee_key = self.area_tool.sign(
            data=data_to_sign,
            priv_key_file=gsl_priv_key_file,
            data_name='TEE_Key Area'
        )

        area = utl.combine_bytes(
            data_to_sign,
            sign_tee_key
        )

        utl.write_file(out_file, area, 'TEE_Key Area')
        log.debug('Done.')
        return

    def build_atf_area(self, out_file):
        log.debug('--------------------- Build ATF Area ---------------------')
        self.cfgtor.reset_path('TEE')
        tee_priv_key_file = self.cfgtor.get_value('TEE_External_Private_Key')
        self.cfgtor.reset_path('TEE', 'ATF_Area')
        atf_code = utl.align(self.cfgtor.get_value('ATF_Code'), n_align_bytes=16)

        atf_code_id         = utl.str2le('0x3C78694B', group_size=4)
        structure_version   = utl.str2le('0x00000000', group_size=4)
        reserved            = utl.str2hex('00') * 0x18

        data_to_sign = utl.combine_bytes(
            atf_code_id,
            structure_version,
            reserved,
            atf_code,
        )

        sign_atf = self.area_tool.sign(
            data=data_to_sign,
            priv_key_file=tee_priv_key_file,
            data_name='ATF Area'
        )

        self.cfgtor.reset_path('TEE', 'TEE_Key_Area')
        _, _, _, l2_clr, iv = self.__get_enc_dec_protkey(self.cfgtor, prefix='ATF')
        atf_code = self.area_tool.enc_cbc(atf_code, l2_clr, iv, data_name='ATF code')

        sign_atf_tp = utl.str2hex('00') * AreaCfg.SIGNATURE_SIZE

        area = utl.combine_bytes(
            atf_code_id,
            structure_version,
            reserved,
            atf_code,
            sign_atf,
            sign_atf_tp
        )

        utl.write_file(out_file, area, 'ATF Area')
        log.debug('Done.')

        self.atf_area_file = out_file
        return

    def build_tee_code_area(self, out_file):
        log.debug('------------------ Build TEE_Code Area ------------------')
        self.cfgtor.reset_path('TEE')
        tee_priv_key_file = self.cfgtor.get_value('TEE_External_Private_Key')
        self.cfgtor.reset_path('TEE', 'TEE_Code_Area')
        tee_code = utl.align(self.cfgtor.get_value('TEE_Code'), n_align_bytes=16)

        tee_code_id       = utl.str2le('0x3C78695A', group_size=4)
        structure_version = utl.str2le('0x00000000', group_size=4)
        reserved          = utl.str2hex('00') * 0x18

        data_to_sign = utl.combine_bytes(
            tee_code_id,
            structure_version,
            reserved,
            tee_code
        )
        sign_tee_code = self.area_tool.sign(
            data=data_to_sign,
            priv_key_file=tee_priv_key_file,
            data_name='TEE Code Area'
        )

        self.cfgtor.reset_path('TEE', 'TEE_Key_Area')
        _, _, _, l2_clr, iv = self.__get_enc_dec_protkey(self.cfgtor, prefix='TEE')
        tee_code = self.area_tool.enc_cbc(tee_code, l2_clr, iv, data_name='TEE code')

        sign_tee_code_tp = utl.str2hex('00') * AreaCfg.SIGNATURE_SIZE

        area = utl.combine_bytes(
            tee_code_id,
            structure_version,
            reserved,
            tee_code,
            sign_tee_code,
            sign_tee_code_tp
        )

        utl.write_file(out_file, area, 'TEE_Code Area')
        log.debug('Done.')

        self.tee_code_area_file = out_file
        return