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
import copy

SCRIPT_DIR = os.path.dirname(__file__)
SCRIPT_DIR = SCRIPT_DIR if len(SCRIPT_DIR) != 0 else '.'
WORK_DIR = os.path.abspath('%s/../..' % SCRIPT_DIR)
sys.path.append(WORK_DIR)

import common.check as chk
import common.security as sec
import common.util as util
import common.logger as log
from area_tool import AreaCfg, AreaTool, PublicKey
from config import CfgValue
from kdf import KdfTool

class OneTimeProgrammable:
    def __init__(self, file = None):
        self.__secure_boot_en = None
        self.__soc_tee_enable = None
        self.__gsl_dec_en = None
        self.__bload_dec_en = None
        self.__double_sign_en = None
        self.__tee_owner_sel = None
        self.__oem_rk_deob_en = None
        self.__oem_root_symc_key0 = None
        self.__oem_root_public_key_sha256 = None
        self.__oem_msid = None
        self.__oem_version = None
        self.__tee_msid = None
        self.__tee_sec_version = None

        self.load(file)

    def load(self, file):
        import json
        with open(file, 'r') as f:
            obj = json.load(f)
            self.__secure_boot_en  = int(obj['secure_boot_en'], 16)
            self.__soc_tee_enable  = int(obj['soc_tee_enable'], 16)
            self.__gsl_dec_en      = int(obj['gsl_dec_en'], 16)
            self.__bload_dec_en    = int(obj['bload_dec_en'], 16)
            self.__tee_owner_sel   = int(obj['tee_owner_sel'], 16)
            self.__oem_rk_deob_en  = int(obj['oem_rk_deob_en'], 16)
            self.__oem_root_symc_key0 = \
                util.str2hex(obj['oem_root_symc_key0'])
            self.__oem_root_public_key_sha256 = \
                util.str2hex(obj["oem_root_public_key_sha256"])
            self.__oem_msid        = int(obj['oem_msid'], 16)
            self.__oem_version     = int(obj['oem_version'], 16)
            self.__tee_msid        = int(obj['tee_msid'], 16)
            self.__tee_sec_version = int(obj['tee_sec_version'], 16)

    def sec_boot_enable(self):
        return self.__secure_boot_en != 0x42
    def tee_enable(self):
        return self.__soc_tee_enable != 0x42
    def gsl_dec_enable(self):
        return self.__gsl_dec_en != 0xA
    def bload_dec_enable(self):
        return self.__bload_dec_en == 0x1
    def tee_owner_is_oem(self):
        return self.__tee_owner_sel == 0x0
    def oem_rk_deob_enable(self):
        return self.__oem_rk_deob_en == 0x1
    def oem_version(self, mask):
        return bin(self.__oem_version & mask).count('1')
    def oem_msid(self, mask):
        return self.__oem_msid & mask
    def oem_root_pub_key_sha256(self):
        return self.__oem_root_public_key_sha256
    def oem_root_symc_key0(self):
        return self.__oem_root_symc_key0
    def tee_sec_version(self, mask):
        bit_mask = 0 if mask == 0 else int('1' * mask, 2)
        return bin(self.__tee_sec_version & bit_mask).count('1')
    def tee_msid(self, mask):
        return self.__tee_msid & mask

class OemChecker:
    def __init__(self, otp_file):
        self.__otp = OneTimeProgrammable(otp_file)
        self.__kdf = KdfTool(
            self.__otp.oem_root_symc_key0(),
            self.__otp.oem_rk_deob_enable()
        )
        self.__area_tool = AreaTool(CfgValue.CryptoAlg.RSA)
        self.__scs_simulate_enbale = False

    def __scs_enable(self):
        return self.__otp.sec_boot_enable() or self.__scs_simulate_enbale

    def check(self, boot_image, tee_image=None):
        image_bytes = util.read_file(boot_image)

        pub_key_area = image_bytes[0x400 : 0x800]
        pub_key_info = self.__parse_oem_root_pub_key_area(pub_key_area)

        gsl_key_area = image_bytes[0x1200 : 0x1800]
        gsl_key_info = self.__parse_gsl_key_area(gsl_key_area, pub_key_info)

        gsl_code_area_len = gsl_key_info['GSL_Code_Area_Len']
        gsl_code_area_end = 0x1800 + gsl_code_area_len
        gsl_code_area = image_bytes[0x1800 : gsl_code_area_end]
        gls_code_info = self.__parse_gsl_code_area(gsl_code_area, gsl_key_info)

        boot_key_area_len   = 0x600
        boot_key_area_begin = gsl_code_area_end
        boot_key_area_end   = boot_key_area_begin + boot_key_area_len

        params_area_len  = 0x2B0 + AreaCfg.CFG_PARAM_SIZE
        param_area_begin = boot_key_area_end
        params_area_end  = param_area_begin + params_area_len

        unchk_area_vendor_len   = 0x40
        unchk_area_vendor_begin = params_area_end
        unchk_area_vendor_end   = unchk_area_vendor_begin + unchk_area_vendor_len
        unchk_area_vendor = image_bytes[unchk_area_vendor_begin : unchk_area_vendor_end]
        unchk_info_vendor = self.__parse_unchecked_area_vendor(unchk_area_vendor)

        boot_key_area = image_bytes[boot_key_area_begin : boot_key_area_end]
        boot_key_info = self.__parse_boot_key_area(boot_key_area, pub_key_info)

        params_area = image_bytes[param_area_begin : params_area_end]
        params_info = self.__parse_params_area(params_area, boot_key_info)

        boot_area_len   = params_info['Boot_Area_Len']
        boot_area_begin = unchk_area_vendor_end
        boot_area_end   = boot_area_begin + boot_area_len
        boot_area = image_bytes[boot_area_begin : boot_area_end]
        boot_info = self.__parse_boot_area(boot_area, boot_key_info, params_info)

        print('\nBoot Image is OK.\n')

        if not (self.__otp.tee_enable() and self.__otp.tee_owner_is_oem()):
            return True

        chk.check_empty(tee_image, "tee_image.bin")
        image_bytes  = util.read_file(tee_image)

        tee_key_area_end = 0x600
        tee_key_area = image_bytes[: tee_key_area_end]
        tee_key_info = self.__parse_tee_key_area(tee_key_area, gsl_key_info)

        atf_area_len    = tee_key_info['ATF_Area_Len']
        atf_area_begin  = tee_key_area_end
        atf_area_end    = atf_area_begin + atf_area_len

        tee_code_area_len   = tee_key_info['TEE_Code_Area_Len']
        tee_code_area_begin = atf_area_end
        tee_code_area_end   = tee_code_area_begin + tee_code_area_len

        atf_area = image_bytes[atf_area_begin : atf_area_end]
        area_info = self.__parse_atf_area(atf_area, tee_key_info)

        tee_code_area = image_bytes[tee_code_area_begin : tee_code_area_end]
        tee_code_info = self.__parse_tee_code_area(tee_code_area, tee_key_info)

        print('\nTEE Image is OK.\n')

    def __parse_oem_root_pub_key_area(self, area):
        log.info('---- Parsing OEM_Root_Public_Key Area ----')
        if self.__otp.sec_boot_enable() or self.__otp.tee_enable():
            log.info('checking oem_root_public_key_sha256')
            digest = self.__area_tool.digest(area)
            chk.check_match(self.__otp.oem_root_pub_key_sha256(), digest, 'oem_root_public_key_sha256 (OTP)')

        # derive protection key L2
        l1_enc = area[0x240 : 0x250]
        l2_enc = area[0x250 : 0x270]
        l2_clr = self.__kdf.derive_l2(1, 0x00000002, l1_enc, l2_enc)

        # decrypt data of public key
        pub_key_enc = area[0x0 : 0x240]
        iv = area[0x270 : 0x280]
        pub_key_clr = self.__area_tool.enc_cbc(pub_key_enc, l2_clr, iv, decrypt=True)

        if self.__otp.sec_boot_enable() or self.__otp.tee_enable():
            log.info('checking clear Root_Public_Key hash')
            pub_key = pub_key_clr[0x0 : 0x204]
            chksum  = pub_key_clr[0x204 : 0x224]
            digest  = self.__area_tool.digest(pub_key)
            chk.check_match(chksum, digest, 'OEM_Root_Public_Key Hash (Image)')

        # extract public key
        modulus  = pub_key_clr[0x0 : 0x200]
        exponent = pub_key_clr[0x200 : 0x204]
        area_info = {'Root_Public_Key' : PublicKey(modulus, exponent)}
        return area_info

    def __parse_gsl_key_area(self, area, pub_key_info):
        if not self.__otp.tee_owner_is_oem():
            gsl_code_area_len = util.le2int(area[0x8 : 0xc], 4)
            area_info = {'GSL_Code_Area_Len' : gsl_code_area_len,}
            return area_info

        log.info('---- Parsing GSL_Key Area ----')
        if self.__otp.sec_boot_enable():
            log.info('verifying signature')
            pub_key = pub_key_info['Root_Public_Key']
            data = area[: -AreaCfg.SIGNATURE_SIZE]
            sign = area[-AreaCfg.SIGNATURE_SIZE :]
            ok = self.__area_tool.verify(pub_key, data, sign)
            chk.check_ok(ok, 'verify GSL_Key Area signature')

            log.info('checking Certificate_ID')
            cert_id = util.le2int(area[0x0 : 0x4], 4)
            chk.check_match(cert_id, 0x3C78962D, 'Certificate_ID')

            log.info('checking Structure_Version')
            strct_ver = util.le2int(area[0x4 : 0x8], 4)
            chk.check_match(strct_ver, 0x00000000, 'Structure_Version')

            log.info('checking OEM_MSID')
            oem_msid_ext      = util.le2int(area[0x210 : 0x214], 4)
            mask_oem_msid_ext = util.le2int(area[0x214 : 0x218], 4)
            oem_msid = mask_oem_msid_ext & oem_msid_ext
            chk.check_match(oem_msid, self.__otp.oem_msid(mask_oem_msid_ext), 'OEM_MSID')

            log.info('checking OEM_Version')
            oem_ver_ext      = util.le2int(area[0x218 : 0x21C], 4)
            mask_oem_ver_ext = util.le2int(area[0x21c : 0x220], 4)
            oem_ver = bin(mask_oem_ver_ext & oem_ver_ext).count('1')
            chk.check_greater_equal(oem_ver, self.__otp.oem_version(mask_oem_ver_ext), 'OEM_Version')

        # extract public key
        exponent = area[0xc : 0x10]
        modulus  = area[0x10 : 0x210]
        public_key = PublicKey(modulus, exponent)

        # derive protection key L2
        l1_enc = area[0x220 : 0x230]
        l2_enc = area[0x230 : 0x250]
        l2_clr = self.__kdf.derive_l2(1, 0x00000004, l1_enc, l2_enc)

        gsl_code_area_len = util.le2int(area[0x8 : 0xc], 4)
        gsl_code_enc_flag = util.le2int(area[0x260 : 0x264], 4)
        iv = area[0x250 : 0x260]

        area_info = {
            'GSL_External_Public_Key' : public_key,
            'GSL_Code_Area_Len'       : gsl_code_area_len,
            'GSL_ProtectionKey_L2'    : l2_clr,
            'GSL_IV'                  : iv,
            'GSL_Code_Enc_Flag'   : gsl_code_enc_flag,
        }
        return area_info

    def __parse_gsl_code_area(self, area, gsl_key_info):
        if not self.__otp.tee_owner_is_oem():
            return None

        log.info('---- Parsing GSL_Code Area ----')
        area_len = gsl_key_info['GSL_Code_Area_Len']
        code_begin = 0x40
        code_len   = area_len - (AreaCfg.SIGNATURE_SIZE * 2) - code_begin
        code_end   = code_begin + code_len
        code       = area[code_begin : code_end]

        gsl_enc_flag = gsl_key_info['GSL_Code_Enc_Flag']
        if self.__otp.gsl_dec_enable() or gsl_enc_flag != 0x3C7896E1:
            l2 = gsl_key_info['GSL_ProtectionKey_L2']
            iv = gsl_key_info['GSL_IV']
            log.info('decrypting GSL Code')
            code = self.__area_tool.enc_cbc(code, l2, iv, decrypt=True)
            area = copy.deepcopy(area)
            area = area[: code_begin] + code + area[code_end :]

        if self.__otp.sec_boot_enable():
            log.info('verifying signature')
            pub_key = gsl_key_info['GSL_External_Public_Key']
            data = area[: -(AreaCfg.SIGNATURE_SIZE * 2)]
            sign = area[-(AreaCfg.SIGNATURE_SIZE * 2) : -AreaCfg.SIGNATURE_SIZE]
            ok = self.__area_tool.verify(pub_key, data, sign)
            chk.check_ok(ok, 'verify GSL_Code Area signature')

            log.info('checking Certificate_ID')
            cert_id = util.le2int(area[0x0 : 0x4], 4)
            chk.check_match(cert_id, 0x3C78691E, 'Certificate_ID')

            log.info('checking Structure_Version')
            strct_ver = util.le2int(area[0x4 : 0x8], 4)
            chk.check_match(strct_ver, 0x00000000, 'Structure_Version')

        gsl_image_info    = area[0x20 : 0x40]
        area_info = {
            'GSL_Image_info' : gsl_image_info,
            'GSL_Code'       : code
        }
        return area_info

    def __parse_boot_key_area(self, area, pub_key_info):
        log.info('---- Parsing Boot_Key Area ----')
        if self.__scs_enable():
            log.info('verifying signature')
            pub_key = pub_key_info['Root_Public_Key']
            data = area[: -AreaCfg.SIGNATURE_SIZE]
            sign = area[-AreaCfg.SIGNATURE_SIZE :]
            ok = self.__area_tool.verify(pub_key, data, sign)
            chk.check_ok(ok, 'verify')

            log.info('checking Certificate_ID')
            cert_id = util.le2int(area[0x0 : 0x4], 4)
            chk.check_match(cert_id, 0x3C78963C, 'Certificate_ID')

            log.info('checking Structure_Version')
            strct_ver = util.le2int(area[0x4 : 0x8], 4)
            chk.check_match(strct_ver, 0x00000000, 'Structure_Version')

            log.info('checking OEM_MSID')
            oem_msid_ext      = util.le2int(area[0x210 : 0x214], 4)
            mask_oem_msid_ext = util.le2int(area[0x214 : 0x218], 4)
            oem_msid = mask_oem_msid_ext & oem_msid_ext
            chk.check_match(oem_msid, self.__otp.oem_msid(mask_oem_msid_ext), 'OEM_MSID')

            log.info('checking OEM_Version')
            oem_ver_ext      = util.le2int(area[0x218 : 0x21C], 4)
            mask_oem_ver_ext = util.le2int(area[0x21c : 0x220], 4)
            oem_ver = bin(mask_oem_ver_ext & oem_ver_ext).count('1')
            chk.check_greater_equal(oem_ver, self.__otp.oem_version(mask_oem_ver_ext), 'OEM_Version')

        # extract public key
        exponent = area[0xc : 0x10]
        modulus  = area[0x10 : 0x210]
        public_key = PublicKey(modulus, exponent)

        area_info = {
            'REE_External_Public_Key' : public_key
        }
        return area_info

    def __parse_params_area(self, area, boot_key_info):
        log.info('---- Parsing Boot_Params Area ----')
        if self.__scs_enable():
            log.info('verifying signature')
            pub_key = boot_key_info['REE_External_Public_Key']
            data = area[: -AreaCfg.SIGNATURE_SIZE]
            sign = area[-AreaCfg.SIGNATURE_SIZE :]
            ok = self.__area_tool.verify(pub_key, data, sign)
            chk.check_ok(ok, 'verify Boot_Params Area signature')

            log.info('checking Params_ID')
            cert_id = util.le2int(area[0x0 : 0x4], 4)
            chk.check_match(cert_id, 0x3C78692D, 'Params_ID')

            log.info('checking Structure_Version')
            strct_ver = util.le2int(area[0x4 : 0x8], 4)
            chk.check_match(strct_ver, 0x00000000, 'Structure_Version')

            log.info('checking OEM_MSID')
            oem_msid_ext      = util.le2int(data[-0x10 : -0xC], 4)
            mask_oem_msid_ext = util.le2int(data[-0xC : -0x8], 4)
            oem_msid = mask_oem_msid_ext & oem_msid_ext
            chk.check_match(oem_msid, self.__otp.oem_msid(mask_oem_msid_ext), 'OEM_MSID')

            log.info('checking OEM_Version')
            oem_ver_ext      = util.le2int(data[-0x8 : -0x4], 4)
            mask_oem_ver_ext = util.le2int(data[-0x4 :], 4)
            oem_ver = bin(mask_oem_ver_ext & oem_ver_ext).count('1')
            chk.check_greater_equal(oem_ver, self.__otp.oem_version(mask_oem_ver_ext), 'OEM_Version')

        boot_entry_point = util.le2int(area[0x8 : 0x10], 8)
        unchk_area_len   = util.le2int(area[0x10 : 0x14], 4)
        chk_area_len     = util.le2int(area[0x14 : 0x18], 4)
        boot_area_len    = util.le2int(area[0x18 : 0x1C], 4)
        boot_enc_flag    = util.le2int(area[0x1C : 0x20], 4)

        # derive protection key L2
        l1_enc = area[0x20 : 0x30]
        l2_enc = area[0x30 : 0x50]
        l2_clr = self.__kdf.derive_l2(1, 0x00000005, l1_enc, l2_enc)
        iv = area[0x50 : 0x60]

        area_info = {
            'Boot_Entry_Point'   : boot_entry_point,
            'Unchecked_Area_Len' : unchk_area_len,
            'Checked_Area_Len'   : chk_area_len,
            'Boot_Area_Len'      : boot_area_len,
            'Boot_Enc_Flag'      : boot_enc_flag,
            'Flash_ProtectionKey_Boot_L2' : l2_clr,
            'Flash_IV_Boot'      : iv,
        }
        return area_info

    def __parse_unchecked_area_vendor(self, area):
        log.info('---- Parsing Unchecked Area for Vendor ----')
        scs_simulate_flag = util.le2int(area[: 4], 4)
        self.__scs_simulate_enbale = (scs_simulate_flag != 0x69875AB4)
        area_info = {'SCS_simulate_flag' : scs_simulate_flag}
        return area_info

    def __parse_boot_area(self, area, boot_key_info, params_info):
        log.info('---- Parsing Boot Area ----')

        unchk_area_len = params_info['Unchecked_Area_Len']
        chk_area_len   = params_info['Checked_Area_Len']
        boot_area_len  = params_info['Boot_Area_Len']
        total_len = unchk_area_len + chk_area_len + (AreaCfg.SIGNATURE_SIZE * 2)
        chk.check_match(boot_area_len, total_len, "Length of Boot_Area, Unchecked_Area, Checked_Area")

        unchecked_area = area[: unchk_area_len]
        check_area     = area[unchk_area_len : -(AreaCfg.SIGNATURE_SIZE * 2)]
        code = check_area[0x10 : -0x10]

        boot_enc_flag  = params_info['Boot_Enc_Flag']
        if self.__otp.bload_dec_enable() or boot_enc_flag != 0x3C7896E1:
            check_area = copy.deepcopy(check_area)
            log.info('decrypting Boot Code')
            l2 = params_info['Flash_ProtectionKey_Boot_L2']
            iv = params_info['Flash_IV_Boot']
            code = self.__area_tool.enc_cbc(code, l2, iv, decrypt=True)
            check_area = check_area[: 0x10] + code + check_area[-0x10 :]

        if self.__scs_enable():
            log.info('verifying signature')
            pub_key = boot_key_info['REE_External_Public_Key']
            sign = area[-(AreaCfg.SIGNATURE_SIZE * 2) : -AreaCfg.SIGNATURE_SIZE]
            ok = self.__area_tool.verify(pub_key, check_area, sign)
            chk.check_ok(ok, 'verify Boot Area signature')

            log.info('checking Code_ID')
            cert_id = util.le2int(check_area[0x0 : 0x4], 4)
            chk.check_match(cert_id, 0x3C78693C, 'Code_ID')

            log.info('checking Structure_Version')
            strct_ver = util.le2int(check_area[0x4 : 0x8], 4)
            chk.check_match(strct_ver, 0x00000000, 'Structure_Version')

            log.info('checking OEM_MSID')
            oem_msid_ext      = util.le2int(check_area[-0x10 : -0xC], 4)
            mask_oem_msid_ext = util.le2int(check_area[-0xC : -0x8], 4)
            oem_msid = mask_oem_msid_ext & oem_msid_ext
            chk.check_match(oem_msid, self.__otp.oem_msid(mask_oem_msid_ext), 'OEM_MSID')

            log.info('checking OEM_Version')
            oem_ver_ext      = util.le2int(check_area[-0x8 : -0x4], 4)
            mask_oem_ver_ext = util.le2int(check_area[-0x4 :], 4)
            oem_ver = bin(mask_oem_ver_ext & oem_ver_ext).count('1')
            chk.check_greater_equal(oem_ver, self.__otp.oem_version(mask_oem_ver_ext), 'OEM_Version')

        area_info = {'Boot_Code' : code}
        return area_info

    def __parse_tee_key_area(self, area, gsl_key_info):
        log.info('---- Parsing TEE_Key Area ----')
        log.info('verifying signature')
        pub_key = gsl_key_info['GSL_External_Public_Key']
        data = area[: -AreaCfg.SIGNATURE_SIZE]
        sign = area[-AreaCfg.SIGNATURE_SIZE :]
        ok = self.__area_tool.verify(pub_key, data, sign)
        chk.check_ok(ok, 'verify TEE_Key Area signature')

        log.info('checking Certificate_ID')
        cert_id = util.le2int(area[0x0 : 0x4], 4)
        chk.check_match(cert_id, 0x3C78964B, 'Certificate_ID')

        log.info('checking Structure_Version')
        strct_ver = util.le2int(area[0x4 : 0x8], 4)
        chk.check_match(strct_ver, 0x00000000, 'Structure_Version')

        log.info('checking TEE_MSID')
        tee_msid_ext      = util.le2int(area[0x210 : 0x214], 4)
        mask_tee_msid_ext = util.le2int(area[0x214 : 0x218], 4)
        tee_msid = mask_tee_msid_ext & tee_msid_ext
        chk.check_match(tee_msid, self.__otp.tee_msid(mask_tee_msid_ext), 'TEE_MSID')

        log.info('checking TEE_Version')
        tee_sec_ver_ext      = util.le2int(area[0x218 : 0x21C], 4)
        mask_tee_sec_ver_ext = util.le2int(area[0x21C : 0x220], 4)
        tee_sec_ver_otp      = self.__otp.tee_sec_version(mask_tee_sec_ver_ext)
        chk.check_greater_equal(tee_sec_ver_ext, tee_sec_ver_otp, 'TEE_Version')

        # extract public key
        exponent = area[0xc : 0x10]
        modulus  = area[0x10 : 0x210]
        public_key = PublicKey(modulus, exponent)

        # derive protection key L2
        atf_l1_enc = area[0x220 : 0x230]
        atf_l2_enc = area[0x230 : 0x250]
        tee_l1_enc = area[0x250 : 0x260]
        tee_l2_enc = area[0x260 : 0x280]
        atf_l2_clr = self.__kdf.derive_l2(0, 0x00000006, atf_l1_enc, atf_l2_enc)
        tee_l2_clr = self.__kdf.derive_l2(0, 0x00000007, tee_l1_enc, tee_l2_enc)

        atf_iv = area[0x280 : 0x290]
        tee_iv = area[0x290 : 0x2A0]

        tee_code_area_len = util.le2int(area[0x8 : 0xC], 4)
        tee_func_version  = util.le2int(area[0x2A0 : 0x2A4], 4)
        atf_area_len      = util.le2int(area[0x2A4 : 0x2A8], 4)
        tee_ddr_address   = util.le2int(area[0x2A8 : 0x2B0], 8)
        tee_reserved_ddr_size = util.le2int(area[0x2B0 : 0x2B4], 4)

        area_info = {
            'TEE_Code_Area_Len'     : tee_code_area_len,
            'TEE_External_Public_Key' : public_key,
            'ATF_ProtectionKey_L2'  : atf_l2_clr,
            'TEE_ProtectionKey_L2'  : tee_l2_clr,
            'ATF_IV'                : atf_iv,
            'TEE_IV'                : tee_iv,
            'ATF_Area_Len'          : atf_area_len,
            'TEE_DDR_Address'       : tee_ddr_address,
            'TEE_Reserved_DDR_Size' : tee_reserved_ddr_size,
        }
        return area_info

    def __parse_atf_area(self, area, tee_key_info):
        log.info('---- Parsing ATF Area ----')
        l2 = tee_key_info['ATF_ProtectionKey_L2']
        iv = tee_key_info['ATF_IV']
        area_len = tee_key_info['ATF_Area_Len']

        area = copy.deepcopy(area)
        log.info('decrypting ATF Code')
        code_begin = 0x20
        code_len   = area_len - (AreaCfg.SIGNATURE_SIZE * 2) - code_begin
        code_end   = code_begin + code_len
        code = area[code_begin : code_end]
        code = self.__area_tool.enc_cbc(code, l2, iv, decrypt=True)
        area = area[: code_begin] + code + area[code_end :]

        log.info('verifying signature')
        pub_key = tee_key_info['TEE_External_Public_Key']
        data = area[: -(AreaCfg.SIGNATURE_SIZE * 2)]
        sign = area[-(AreaCfg.SIGNATURE_SIZE * 2) : -AreaCfg.SIGNATURE_SIZE]
        ok = self.__area_tool.verify(pub_key, data, sign)
        chk.check_ok(ok, 'verify ATF Area signature')

        log.info('checking ATF_Code_ID')
        cert_id = util.le2int(area[0x0 : 0x4], 4)
        chk.check_match(cert_id, 0x3C78694B, 'ATF_Code_ID')

        log.info('checking Structure_Version')
        strct_ver = util.le2int(area[0x4 : 0x8], 4)
        chk.check_match(strct_ver, 0x00000000, 'Structure_Version')

        area_info = {'ATF_Code' : code}
        return area_info

    def __parse_tee_code_area(self, area, tee_key_info):
        log.info('---- Parsing TEE_Code Area ----')
        l2 = tee_key_info['TEE_ProtectionKey_L2']
        iv = tee_key_info['TEE_IV']
        area_len = tee_key_info['TEE_Code_Area_Len']

        area = copy.deepcopy(area)
        log.info('decrypting TEE Code')
        code_begin = 0x20
        code_len   = area_len - (AreaCfg.SIGNATURE_SIZE * 2) - code_begin
        code_end   = code_begin + code_len
        code = area[code_begin : code_end]
        code = self.__area_tool.enc_cbc(code, l2, iv, decrypt=True)
        area = area[: code_begin] + code + area[code_end :]

        log.info('verifying signature')
        pub_key = tee_key_info['TEE_External_Public_Key']
        data = area[: -(AreaCfg.SIGNATURE_SIZE * 2)]
        sign = area[-(AreaCfg.SIGNATURE_SIZE * 2) : -AreaCfg.SIGNATURE_SIZE]
        ok = self.__area_tool.verify(pub_key, data, sign)
        chk.check_ok(ok, 'verify TEE_Code Area signature')

        log.info('checking TEE_Code_ID')
        cert_id = util.le2int(area[0x0 : 0x4], 4)
        chk.check_match(cert_id, 0x3C78695A, 'TEE_Code_ID')

        log.info('checking Structure_Version')
        strct_ver = util.le2int(area[0x4 : 0x8], 4)
        chk.check_match(strct_ver, 0x00000000, 'Structure_Version')

        area_info = {'TEE_Code' : code}
        return area_info
