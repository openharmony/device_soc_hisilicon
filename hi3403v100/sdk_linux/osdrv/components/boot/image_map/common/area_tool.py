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

import check as chk
import logger as log
import security as sec
import util as utl
from config import CfgValue

class AreaCfg:
    CFG_PARAM_SIZE = 0x2B10     # reg table size
    SIGNATURE_SIZE = 0x200

# alg: RSA, SM2
class PublicKey:
    def __init__(self, n, e, name=None):
        self.n = n
        self.e = e
        self.name = name

# function:
#   eliminate algorithm differences,
#   provide the unify interface for callers
class AreaTool:
    def __init__(self, alg):
        self._asym_alg    = None
        self._digest_alg  = None
        self._enc_ecb_alg = None
        self._enc_cbc_alg = None

        chk.check_option(alg, [CfgValue.CryptoAlg.RSA, None], 'Crypto_Algorithm')
        if alg == CfgValue.CryptoAlg.RSA:
            self._asym_alg     = 'RSA'
            self._digest_alg   = 'SHA256'
            self._enc_ecb_alg  = 'AES-128-ECB'
            self._enc_cbc_alg  = 'AES-256-CBC'

        return

    def import_pub_key(self, key_file, key_name=None):
        if key_name != None and self._asym_alg != None:
            log.info('[%s] Importing %s from %s' % (self._asym_alg, key_name, key_file))
        modulus  = utl.str2hex('00') * 0x200
        exponent = utl.str2hex('00') * 0x4
        if self._asym_alg == 'RSA':
            chk.check_empty(key_file, key_name)
            modulus = sec.get_rsa_modulus(key_file)
            exponent = sec.get_rsa_exponent(key_file)
        return PublicKey(modulus, exponent, key_name)

    def export_pub_key(self, pub_key, out_file, key_name=None):
        if key_name != None and self._asym_alg != None:
            log.info('[%s] Exporting %s (%s)' % (self._asym_alg, key_name, in_file))
        if self._asym_alg == 'RSA':
            sec.export_rsa_pub_key(pub_key.n, pub_key.e, out_file)
            return

        return None

    def sign(self, data, priv_key_file, data_name=None):
        if data_name != None and self._asym_alg != None:
            log.info('[%s] Signing %s' % (self._asym_alg, data_name))

        if self._asym_alg == 'RSA':
            chk.check_empty(data, notice=data_name)
            return sec.sign_rsa_sha256(data, priv_key_file)

        return utl.str2hex('00') * AreaCfg.SIGNATURE_SIZE

    def verify(self, pub_key, data, sign, data_name=None):
        if data_name != None and self._asym_alg != None:
            log.info('[%s] Verifying %s' % (self._asym_alg, data_name))

        if self._asym_alg == 'RSA':
            chk.check_empty(data, notice=data_name)
            tmp_key_file = 'tmp_pub_key.pem'
            sec.export_rsa_pub_key(pub_key.n, pub_key.e, tmp_key_file)
            ok = sec.verify_rsa_sha256(data, sign, tmp_key_file)
            utl.rm_file(tmp_key_file)
            return ok

        return False

    def digest(self, data, data_name=None):
        if data_name != None and self._digest_alg != None:
            log.info('[%s] Hashing %s' % (self._digest_alg, data_name))

        if self._digest_alg == 'SHA256':
            chk.check_empty(data, notice=data_name)
            return sec.dgst_sha256(data)

        return utl.str2hex('00') * 0x20

    def enc_ecb(self, data, key, decrypt=False, data_name=None):
        action = 'Encrypting' if not decrypt else 'Decrypting'
        if data_name != None and self._enc_ecb_alg != None:
            log.info('[%s] %s %s' % (self._enc_ecb_alg, action, data_name))

        if self._enc_ecb_alg == 'AES-128-ECB':
            chk.check_empty(data, notice=data_name)
            return sec.enc_aes_128_ecb(data, key, decrypt)

        log.warn('No algorithm specified. Do nothing to input data.')
        return data

    def enc_cbc(self, data, key, iv, decrypt=False, data_name=None):
        action = 'Encrypting' if not decrypt else 'Decrypting'
        if data_name != None and self._enc_cbc_alg != None:
            log.info('[%s] %s %s' % (self._enc_cbc_alg, action, data_name))

        if self._enc_cbc_alg == 'AES-256-CBC':
            chk.check_empty(data, notice=data_name)
            return sec.enc_aes_256_cbc(data, key, iv, decrypt)

        log.warn('No algorithm specified. Do nothing to input data.')
        return data

    def try_convert_enc_dec(self, enc, dec, key, data_name=None):
        chk.check_empty(key)
        if enc == None and dec == None:
            chk.check_empty(None)
        new_enc = enc
        new_dec = dec
        if new_enc == None:
            new_enc = self.enc_ecb(dec, key, data_name=data_name)
        if new_dec == None:
            new_dec = self.enc_ecb(enc, key, decrypt=True, data_name=data_name)
        return new_enc, new_dec

    def gen_empty_area(self, size, fill):
        head_size = 8
        max_size = 0xFFFFFFFF
        chk.check_range(size, minimum=head_size, maximum=max_size, notice='Area Size')
        chk.check_range(fill, minimum=0x00, maximum=0xFF, notice='Fill Value')

        size_bytes = utl.str2hex('%08x' % size)
        size_bytes = utl.reverse_endian(size_bytes, 4)
        size_str = utl.hex2str(size_bytes)
        head = 'EAFF00%02x%s' % (fill, size_str)
        content = ('%02x' % fill) * (size - head_size)

        head = utl.str2hex(head)
        content = utl.str2hex(content)

        return head + content

    def enc_pub_key(self, pub_key, cbc_key, iv):
        modulus  = pub_key.n
        exponent = pub_key.e
        key_name = pub_key.name
        checksum = self.digest(modulus + exponent, data_name=key_name)
        reserved = utl.str2hex('0x00') * 0x1c
        clr_data = utl.combine_bytes(modulus, exponent, checksum, reserved)
        enc_data = self.enc_cbc(clr_data, cbc_key, iv, data_name=key_name)
        return enc_data


