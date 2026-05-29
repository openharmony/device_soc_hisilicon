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

import common.config as cfg
from common.config import CfgValue
from common.area_tool import AreaTool, AreaCfg
import common.logger as log
import common.util as utl
import common.check as chk

class ThirdPartyAreaBuilder:
    def __init__(self, cfgtor):
        self.cfgtor = cfgtor    # Configuartor
        self.area_tool = AreaTool(self.cfgtor.scenario().crypto_alg())

    def build_root_pub_key_area(self, area_out_file, checksum_out_file):
        log.debug('--------- Build Third_party_Root_Public_Key Area ---------')
        self.cfgtor.reset_path('Root', 'Third_party_Root_Public_Key_Area')

        self.cfgtor.reset_path('Root', 'Third_party_Root_Public_Key_Area')
        tp_key_file = self.cfgtor.get_value('Third_party_Root_Public_Key')
        if tp_key_file == None:
            log.info("Third_party_Root_Public_Key is not exits.")
            self.cfgtor.reset_path('Root')
            tp_key_file = self.cfgtor.get_value('Third_party_Root_Private_Key')

        pub_key = self.area_tool.import_pub_key(
            tp_key_file,
            key_name='Third_party_Root_Public_Key'
        )

        self.cfgtor.reset_path('Root', 'Third_party_Root_Public_Key_Area')
        rootkey     = self.cfgtor.get_value('Third_party_Public_RootKey')
        l1_enc      = self.cfgtor.get_value('Third_party_ProtectionKey_L1_Enc')
        l2_enc      = self.cfgtor.get_value('Third_party_ProtectionKey_L2_Enc')
        l1_clr      = self.cfgtor.get_value('Third_party_ProtectionKey_L1')
        l2_clr      = self.cfgtor.get_value('Third_party_ProtectionKey_L2')
        iv          = self.cfgtor.get_value('Third_party_Public_IV')
        tp_key_file = self.cfgtor.get_value('Third_party_Root_Public_Key')
        # encryt Root_Public_Key
        l1_enc, l1_clr = self.area_tool.try_convert_enc_dec(l1_enc, l1_clr, rootkey)
        l2_enc, l2_clr = self.area_tool.try_convert_enc_dec(l2_enc, l2_clr, l1_clr)
        pub_key_enc    = self.area_tool.enc_pub_key(pub_key, l2_clr, iv)
        reserved       = utl.str2hex('0x00') * 0x180
        area           = utl.combine_bytes(pub_key_enc, l1_enc, l2_enc, iv, reserved)
        # calculate the checksum written to OTP
        chksum_str  = '0x' + utl.hex2str(self.area_tool.digest(area))

        utl.write_file(area_out_file, area, 'Third_party_Root_Public_Key Area')
        utl.write_file(checksum_out_file, chksum_str, 'Third_party_Root_Public_Key Area Checksum')
        log.debug('Done.')

    def build_gls_third_party_key_area(self, out_file):
        log.debug('------------- Build GSL_Third_party_Key Area -------------')
        self.cfgtor.reset_path('Root')
        tp_root_priv_key = self.cfgtor.get_value('Third_party_Root_Private_Key')

        self.cfgtor.reset_path('GSL', 'GSL_Third_party_Key_Area')
        tp_ext_key_file = self.cfgtor.get_value('Third_party_External_Public_Key')
        if tp_ext_key_file == None:
            log.info("Third_party_External_Public_Key is not exits.")
            self.cfgtor.reset_path('GSL')
            tp_ext_key_file = self.cfgtor.get_value('Third_party_External_Private_Key')

        pub_key = self.area_tool.import_pub_key(
            tp_ext_key_file,
            key_name='Third_party_External_Public_Key'
        )

        self.cfgtor.reset_path('GSL', 'GSL_Third_party_Key_Area')
        tp_msid_ext         = self.cfgtor.get_value('Third_party_MSID_Ext')
        mask_tp_msid_ext    = self.cfgtor.get_value('Mask_Third_party_MSID_Ext')
        tp_version_ext      = self.cfgtor.get_value('Third_party_Version_Ext')
        mask_tp_version_ext = self.cfgtor.get_value('Mask_Third_party_Version_Ext')

        data_to_sign = utl.combine_bytes(
            utl.str2le('0x3C78961E', group_size=4),
            utl.str2le('0x00000000', group_size=4),
            utl.str2hex('00') * 0x4,
            pub_key.e,
            pub_key.n,
            tp_msid_ext,
            mask_tp_msid_ext,
            tp_version_ext,
            mask_tp_version_ext,
            utl.str2hex('00') * 0x1E0,
        )
        sign_gsl_tp_key = self.area_tool.sign(
            data=data_to_sign,
            priv_key_file=tp_root_priv_key,
            data_name='area data'
        )

        area = utl.combine_bytes(
            data_to_sign,
            sign_gsl_tp_key
        )

        utl.write_file(out_file, area, 'GSL_Third_party_Key Area')
        log.debug('Done.')
        return


class ThirdPartyDoubleSigner:

    def __init__(self, cfgtor):
        self.cfgtor = cfgtor    # Configuartor
        self.area_tool = AreaTool(self.cfgtor.scenario().crypto_alg())

    def sign_boot_image(self, oem_boot_image_file, tp_root_pub_key_area_file, gsl_tp_key_area_file, out_file):
        len_width = 4
        sign_len = AreaCfg.SIGNATURE_SIZE

        log.debug('=================== Sign Boot Image =====================')
        chk.check_empty(tp_root_pub_key_area_file, 'Third_party_Root_Public_Key Area')
        chk.check_empty(gsl_tp_key_area_file, 'GSL_Third_party_Key Area')
        chk.check_empty(oem_boot_image_file, 'Boot Image')

        self.cfgtor.reset_path('GSL')
        tp_ext_priv_key_file = self.cfgtor.get_value('Third_party_External_Private_Key')

        boot_image = utl.read_file(oem_boot_image_file, 'Boot Image')

        tp_root_pub_key_area = utl.read_file(
            tp_root_pub_key_area_file,
            'Third_party_Root_Public_Key Area'
        )

        gsl_tp_key_area = utl.read_file(
            gsl_tp_key_area_file,
            'GSL_Third_party_Key'
        )

        # sign GSL Code Area
        gsl_code_area_len_offset = 0x1208
        gsl_code_area_offset = 0x1800
        gsl_code_area_len = self._get_area_len(boot_image, gsl_code_area_len_offset)
        gsl_code_area_data_end = gsl_code_area_offset + gsl_code_area_len - sign_len
        gsl_code_area_data = boot_image[gsl_code_area_offset:gsl_code_area_data_end]
        sig_gsl_code_tp = self.area_tool.sign(
            data=gsl_code_area_data,
            priv_key_file=tp_ext_priv_key_file,
            data_name='GSL_Code Area'
        )

        # sign Boot Area
        unchked_area_len_offset = 0x1E10 + gsl_code_area_len
        checked_area_len_offset = unchked_area_len_offset + len_width
        unchked_area_len = self._get_area_len(boot_image, unchked_area_len_offset)
        checked_area_len = self._get_area_len(boot_image, checked_area_len_offset)
        checked_area_offset = 0x20F0 + AreaCfg.CFG_PARAM_SIZE + gsl_code_area_len + unchked_area_len
        checked_area_data_end = checked_area_offset + checked_area_len
        chked_area_data = boot_image[checked_area_offset:checked_area_data_end]
        sig_boot_tp = self.area_tool.sign(
            data=chked_area_data,
            priv_key_file=tp_ext_priv_key_file,
            data_name='Boot (Checked) Area'
        )

        # put key areas and signatures into boot image
        tp_root_pub_key_area_offset = 0x800
        boot_image = utl.replace_bytes(
            old_bytes=boot_image,
            pos=tp_root_pub_key_area_offset,
            new_bytes=tp_root_pub_key_area
        )

        gsl_tp_key_area_offset = 0xC00
        boot_image = utl.replace_bytes(
            old_bytes=boot_image,
            pos=gsl_tp_key_area_offset,
            new_bytes=gsl_tp_key_area
        )

        sig_gsl_code_tp_offset = gsl_code_area_data_end
        boot_image = utl.replace_bytes(
            old_bytes=boot_image,
            pos=sig_gsl_code_tp_offset,
            new_bytes=sig_gsl_code_tp
        )

        sig_boot_tp_offset = checked_area_data_end + sign_len
        boot_image = utl.replace_bytes(
            old_bytes=boot_image,
            pos=sig_boot_tp_offset,
            new_bytes=sig_boot_tp
        )

        utl.write_file(out_file, boot_image, 'Double Signed Boot Image')
        log.debug('Done.')
        return

    def sign_tee_image(self, oem_tee_image_file, out_file):
        sign_len = AreaCfg.SIGNATURE_SIZE
        log.debug('=================== Sign TEE Image =====================')
        chk.check_empty(oem_tee_image_file, 'TEE Image')

        self.cfgtor.reset_path('GSL')
        tp_ext_priv_key_file = self.cfgtor.get_value('Third_party_External_Private_Key')

        tee_image = utl.read_file(oem_tee_image_file, 'TEE Image')

        # sign ATF Area
        atf_area_len_offset = 0x2A4
        atf_area_offset = 0x600
        atf_area_len = self._get_area_len(tee_image, atf_area_len_offset)
        atf_area_data_end = atf_area_offset + atf_area_len - (sign_len * 2)
        atf_area_data = tee_image[atf_area_offset:atf_area_data_end]
        sig_atf_tp = self.area_tool.sign(
            data=atf_area_data,
            priv_key_file=tp_ext_priv_key_file,
            data_name='ATF Area'
        )

        # sign TEE Code Area
        tee_code_area_len_offset = 0x8
        tee_code_area_offset = 0x600 + atf_area_len
        tee_code_area_len = self._get_area_len(tee_image, tee_code_area_len_offset)
        tee_code_area_data_end = tee_code_area_offset + tee_code_area_len - (sign_len * 2)
        tee_code_area_data = tee_image[tee_code_area_offset:tee_code_area_data_end]
        sig_tee_code_tp = self.area_tool.sign(
            data=tee_code_area_data,
            priv_key_file=tp_ext_priv_key_file,
            data_name='TEE Code Area'
        )

        # put signatures into tee image
        sig_atf_tp_offset = atf_area_offset + atf_area_len - sign_len
        tee_image = utl.replace_bytes(
            old_bytes=tee_image,
            pos=sig_atf_tp_offset,
            new_bytes=sig_atf_tp
        )

        sig_tee_code_tp_offset = tee_code_area_offset + tee_code_area_len - sign_len
        tee_image = utl.replace_bytes(
            old_bytes=tee_image,
            pos=sig_tee_code_tp_offset,
            new_bytes=sig_tee_code_tp
        )

        utl.write_file(out_file, tee_image, 'Double Signed TEE Image')
        log.debug('Done.')
        return

    def _get_area_len(self, image, offset):
        len_width = 4
        len_bytes = image[offset:(offset+len_width)]
        len_bytes = utl.reverse_endian(len_bytes, group_size=4)
        return int(utl.hex2str(len_bytes), 16)

