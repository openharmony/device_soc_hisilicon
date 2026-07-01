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

SCRIPT_DIR = os.path.dirname(__file__)
sys.path.append(SCRIPT_DIR)

import logger as log
import check as chk
import util as utl

def cmd_failure_process(notice, stderr):
    stderr = stderr.decode("utf-8") if version_info.major > 2 else stderr
    stderr = stderr.strip()
    log.error('%s\n----stderr----\n%s\n--------------' % (notice, stderr))
    sys.exit(1)

def gen_rsa_priv_key(priv_key_file):
    cmd = ['openssl', 'genrsa', '-f4', '-out', priv_key_file, '4096']
    stdout, stderr, return_code = utl.run_cmd(cmd)

    if return_code != 0:
        cmd_failure_process('Failed to generate RSA public key', stderr)

    return stdout

def gen_rsa_pub_key(priv_key_file, pub_key_file):
    chk.check_file_exist(priv_key_file, 'RSA Private Key')

    cmd = ['openssl', 'rsa', '-pubout', '-in', priv_key_file,
           '-out', pub_key_file]
    stdout, stderr, return_code = utl.run_cmd(cmd)

    if return_code != 0:
        cmd_failure_process('Failed to generate RSA public key', stderr)

    return stdout

def is_rsa_priv_key(key_file):
    chk.check_file_exist(key_file)
    cmd = 'openssl rsa -check -noout -in %s' % key_file
    stdout, stderr, return_code = utl.run_cmd(cmd)
    return (return_code == 0)

def get_rsa_modulus(key_file):
    chk.check_file_exist(key_file, 'RSA Key')

    options = '-modulus -noout -in %s' % key_file
    if not is_rsa_priv_key(key_file):
        options +=' -pubin'
    cmd = 'openssl rsa %s' % options
    stdout, stderr, return_code = utl.run_cmd(cmd)

    if return_code != 0:
        cmd_failure_process('Failed to get RSA modulus', stderr)

    stdout = stdout.decode("utf-8") if version_info.major > 2 else stdout
    modulus = stdout.replace('Modulus=', '').replace('\n', '')
    modulus = utl.str2hex(modulus)
    return modulus


def get_rsa_exponent(key_file):
    chk.check_file_exist(key_file, 'RSA Key')

    options = '-text -noout -in %s' % key_file
    if not is_rsa_priv_key(key_file):
        options +=' -pubin'
    cmd = 'openssl rsa %s' % options
    stdout, stderr, return_code = utl.run_cmd(cmd)

    if return_code != 0:
        cmd_failure_process('Failed to get RSA exponent', stderr)

    stdout = stdout.decode("utf-8") if version_info.major > 2 else stdout
    exponent = '%08x' % int(stdout.split('Exponent:')[1].split('(')[0][:-1])
    exponent = utl.str2hex(exponent)
    return exponent


def enc_aes_128_ecb(data, key, decrypt=False):
    chk.check_bit_len(key, 128, notice='AES-128 Key')

    mode = '-d' if (decrypt) else '-e'
    cmd = ['openssl', 'enc', '-aes-128-ecb', '-nopad',
           mode, '-K', utl.hex2str(key)]
    stdout, stderr, return_code = utl.run_cmd(cmd, data)

    if return_code != 0:
        cmd_failure_process('AES-128-ECB: Enc/Dec Error', stderr)

    return stdout


def enc_aes_256_cbc(data, key, iv, decrypt=False):
    chk.check_bit_len(key, 256, notice='AES-256 Key')
    chk.check_bit_len(iv, 128, notice='AES-256 IV')

    mode = '-d' if (decrypt) else '-e'
    cmd = ['openssl', 'enc', '-aes-256-cbc', '-nopad', mode,
           '-K', utl.hex2str(key), '-iv', utl.hex2str(iv)]
    stdout, stderr, return_code = utl.run_cmd(cmd, data)

    if return_code != 0:
        cmd_failure_process('AES-256-CBC: Enc/Dec Error', stderr)

    return stdout


def dgst_sha256(data):
    cmd = ['openssl', 'dgst', '-sha256', '-binary', '-r']
    stdout, stderr, return_code = utl.run_cmd(cmd, data)

    if return_code != 0:
        cmd_failure_process('SHA256: Digest Error', stderr)

    return stdout


def sign_rsa_sha256(data, priv_key_file):
    chk.check_file_exist(priv_key_file, 'RSA Private Key')

    cmd = ['openssl', 'dgst', '-sha256', '-sign', priv_key_file,
           '-sigopt', 'rsa_padding_mode:pss', '-sigopt', 'rsa_pss_saltlen:-1']
    stdout, stderr, return_code = utl.run_cmd(cmd, data)

    if return_code != 0:
        cmd_failure_process('RSA_SHA256: Signature Error', stderr)

    return stdout


def verify_rsa_sha256(data, sign, pub_key_file):
    sign_file = 'rsa_sha256_sign.tmp'
    chk.check_file_exist(pub_key_file, 'RSA Public Key')
    utl.write_file(sign_file, sign)

    cmd = 'openssl dgst -sha256'\
        ' -sigopt rsa_padding_mode:pss' \
        ' -sigopt rsa_pss_saltlen:-1'\
        ' -verify %s -signature %s' % (pub_key_file, sign_file)

    stdout, stderr, return_code = utl.run_cmd(cmd, data)
    if return_code == 0:
        os.remove(sign_file)
        return True

    if len(stderr) != 0:
        cmd_failure_process('RSA_SHA256: Verification Error', stderr)

    return False


from Crypto.PublicKey import RSA
from Crypto.Util import number as crypto_num
def export_rsa_pub_key(modulus, exponent, out_pem):
    n = crypto_num.bytes_to_long(modulus)
    e = crypto_num.bytes_to_long(exponent)
    pub_key = RSA.construct((n, e))
    utl.write_file(out_pem, pub_key.exportKey('PEM'))
