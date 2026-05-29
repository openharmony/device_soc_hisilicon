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

import json
import os
import sys
import copy
from collections import OrderedDict

SCRIPT_DIR = os.path.dirname(__file__)
sys.path.append(SCRIPT_DIR)

import check as chk
import util as utl
import logger as log

class CfgValueType():
    UINT32  = 'UINT32'
    UINT64  = 'UINT64'
    HEX     = 'HEX'
    STRING  = 'STRING'
    PATH    = 'PATH'
    IN_FILE = 'IN_FILE'

class CfgValue:
    class SecMode():
        NON_SECURE = 'Non-Secure'
        SECURE = 'Secure'

    class CryptoAlg():
        RSA = 'RSA+SHA256+AES'

    class StartFlow():
        NON_TEE = 'Non-TEE'
        TEE = 'TEE'

    class Owner():
        VENDOR = 'Vendor'
        OEM = 'OEM'
        THIRD_PARTY = 'Third_party'

    class EncFlag():
        NON_ENC = '0x3C7896E1'
        ENC     = '0x00000000'


class Scenario:
    def __init__(self, sec_mode=None, crypto_alg=None, start_flow=None,
                 tee_owner=None, gsl_enc=None, boot_enc=None, value=None):
        chk.check_option(sec_mode, [None, CfgValue.SecMode.NON_SECURE, CfgValue.SecMode.SECURE], 'Security_Mode')
        chk.check_option(crypto_alg, [None, CfgValue.CryptoAlg.RSA], 'Crypto_Algorithm')
        chk.check_option(start_flow, [None, CfgValue.StartFlow.NON_TEE, CfgValue.StartFlow.TEE], 'Start_Flow')
        chk.check_option(tee_owner, [None, CfgValue.Owner.OEM, CfgValue.Owner.VENDOR], 'TEE_Owner')

        self._sec_mode   = sec_mode
        self._crypto_alg = crypto_alg
        self._start_flow = start_flow
        self._tee_owner  = tee_owner
        self._gsl_enc    = gsl_enc
        self._boot_enc   = boot_enc

        # scenario defualt value
        self._value = value

        return

    def log(self):
        d = {CfgValue.EncFlag.ENC: 'Yes', CfgValue.EncFlag.NON_ENC: 'No'}
        log.info('Scenario: ')
        log.info('  - Security Mode: %s' % self._sec_mode)
        if self._crypto_alg is not None:
            log.info('  - Crypto Algoritm: %s' % self._crypto_alg)
        if self._start_flow is not None:
            log.info('  - Start Flow: %s' % self._start_flow)
        if self._tee_owner is not None:
            log.info('  - TEE Owner: %s' % self._tee_owner)
        if self._gsl_enc is not None:
            log.info('  - Encrypt GSL Code: %s' % d.get(self._gsl_enc))
        if self._boot_enc is not None:
            log.info('  - Encrypt Boot Code: %s' % d.get(self._boot_enc))
        return

    def __str__(self):
        return 'Scenario:%s' % str(vars(self))

    def __repr__(self):
        return 'Scenario:%s' % str(vars(self))

    def value(self):
        return self._value

    def sec_mode(self):
        return self._sec_mode

    def crypto_alg(self):
        return self._crypto_alg

    def start_flow(self):
        return self._start_flow

    def tee_owner(self, default=CfgValue.Owner.OEM):
        if self._tee_owner is None:
            return default
        return self._tee_owner

    def gsl_enc(self):
        return self._gsl_enc

    def boot_enc(self):
        return self._boot_enc

    def tee_owner_is_oem(self):
        if self._tee_owner is None:
            return True
        return self._tee_owner == CfgValue.Owner.OEM

    def is_sec_boot_enable(self):
        return self._sec_mode == CfgValue.SecMode.SECURE

    def is_tee_enbale(self):
        return self._start_flow == CfgValue.StartFlow.TEE

    def is_gsl_enc(self):
        return self._gsl_enc != CfgValue.EncFlag.NON_ENC

    def is_boot_enc(self):
        return self._boot_enc != CfgValue.EncFlag.NON_ENC

    def is_matched(self, scen):
        if self._sec_mode is not None and self.sec_mode() != scen.sec_mode():
            return False
        if self._crypto_alg is not None and self.crypto_alg() != scen.crypto_alg():
            return False
        if self._start_flow is not None and self.start_flow() != scen.start_flow():
            return False
        if self._tee_owner is not None and self.tee_owner() != scen.tee_owner():
            return False
        if self._gsl_enc is not None and self.is_gsl_enc() != scen.is_gsl_enc():
            return False
        if self._boot_enc is not None and self.is_boot_enc() != scen.is_boot_enc():
            return False
        return True

class Spec:
    def __init__(self, rng=None, n_bytes=None, n_bits=None,
                 max_len=None, max_bytes=None, options=None):
        self._rng = rng              # list, example: [0, 255]
        self._n_bytes = n_bytes      # int, value of bytes, example: 16
        self._n_bits = n_bits        # int, value of bits,  exmaple: 256
        self._max_bytes = max_bytes  # int, the max number of bytes, example: 4
        self._options = options      # list, options of data value, example: ['Secure', 'Non-Secure']

    def check(self, value, notice):
        # check specification
        if self._rng is not None:
            chk.check_range(value, self._rng[0], self._rng[1], notice)
        if self._n_bytes is not None:
            chk.check_bytes_len(value, self._n_bytes, notice)
        if self._n_bits is not None:
            chk.check_bit_len(value, self._n_bits, notice)
        if self._max_bytes is not None:
            chk.check_max_bytes_len(value, self._max_bytes, notice)
        if self._options is not None:
            chk.check_option(value, self._options, notice)


class CfgItem:
    def __init__(self, key=None, typ=None, usage='', scens=[Scenario()],
            spec=Spec(), sub_items=[], deprecated=False):
        # check input
        for scen in scens:
            chk.check_type(scen, Scenario)
        for item in sub_items:
            chk.check_type(item, CfgItem)
        if len(sub_items) > 1 and len(scens) > 1:
            log.error("A parent item must be with sigle scenario")
            exit(1)
        self._key = key
        self._typ = typ
        self._usage = usage          # presented how is the filed used.
        self._scens = copy.deepcopy(scens)
        self._spec = copy.deepcopy(spec)
        self._sub_items = copy.deepcopy(sub_items)
        self._deprecated = deprecated
        self._sub_item_map = {}
        # build index
        for index, item in enumerate(self._sub_items):
            self._sub_item_map[item.key()] = index
        return

    def __str__(self):
        return 'CfgItem(%s,%s,%s)' % (self._key, self._scens, self._sub_items)

    def __repr__(self):
        return '\nCfgItem(%s,%s,%s)' % (self._key, self._scens, self._sub_items)

    def __getitem__(self, key):
        chk.check_key_in_dict(key, self._sub_item_map, 'CfgItem')
        index = self._sub_item_map[key]
        return self._sub_items[index]

    def __len__(self):
        return len(self._sub_items)

    def __convert_val(self, value):
        ret = value
        if self._typ == CfgValueType.UINT32:
            chk.check_hex_str(value, notice=self._key)
            ret = utl.str2le(value, group_size=4)
        elif self._typ == CfgValueType.UINT64:
            chk.check_hex_str(value, notice=self._key)
            ret = utl.str2le(value, group_size=8)
        elif self._typ == CfgValueType.HEX:
            chk.check_hex_str(value, notice=self._key)
            ret = utl.str2hex(value)
        elif self._typ == CfgValueType.PATH:
            path = os.path.abspath(value)
            chk.check_file_exist(path)
            ret = path
        elif self._typ == CfgValueType.IN_FILE:
            path = os.path.abspath(value)
            chk.check_file_exist(value)
            ret = utl.read_file(path, data_name=self._key)
        self._spec.check(ret, notice=self._key)
        return ret

    def key(self):
        return self._key

    def usage(self):
        return self._usage

    def is_deprecated(self):
        return self._deprecated

    def contains(self, key):
        return key in self._sub_item_map

    def has_sub_item(self):
        return len(self._sub_items) > 0

    def get_sub_item(self, key):
        chk.check_key_in_dict(key, self._sub_item_map, 'CfgItem')
        index = self._sub_item_map[key]
        return self._sub_items[index]

    def match_scen(self, scen):
        for s in self._scens:
            if s.is_matched(scen):
                return s
        return None

    def init_cfg(self, scen):
        cfg_obj = OrderedDict()
        for item in self._sub_items:
            if item._deprecated:
                continue
            matched = item.match_scen(scen)
            if matched == None: # failed to match
                continue
            if item.has_sub_item():
                cfg_obj[item.key()] = item.init_cfg(scen)
                continue
            cfg_obj[item.key()] = '/* %s */' % item.usage()
            if matched.value() != None:
                cfg_obj[item.key()] = matched.value()
        return cfg_obj

    def convert_cfg(self, cfg_obj, scenario):
        if len(self._sub_items) == 0:
            return self.__convert_val(cfg_obj)
        ret_obj = copy.deepcopy(cfg_obj)
        for key, val in ret_obj.items():
            if not self.contains(key):
                continue
            sub_item = self.get_sub_item(key)
            ret_obj[key] = sub_item.convert_cfg(ret_obj[key], scenario)
        return ret_obj

    def add_sub_item(self, item):
        chk.check_type(item, CfgItem)
        self._sub_item_map[item.key()] = len(self._sub_items)
        self._sub_items.append(item)

    def add_scen(self, scen):
        self.add_scens([scen])
        return

    def add_scens(self, scens):
        for scen in scens:
            chk.check_type(scen, Scenario)
        tmp = self._scens
        self._scens = copy.deepcopy(list(scens))
        self._scens.extend(tmp)
        return

    def merge_scen(self, scen):
        chk.check_type(scen, Scenario)
        for s in self._scens:
            s.merge(scen)
        return

    def replace_scens(self, scens):
        for scen in scens:
            chk.check_type(scen, Scenario)
        self._scens = copy.deepcopy(scens)
        return

class Configurator:
    def __init__(self, cfg_item=None):
        self._cfg_item = cfg_item
        self._cfg_obj = None
        self._sub_obj = None
        self._scenario = None

    def dump_init(self, cfg_file, scenario):
        if self._cfg_item is None:
            chk.check_type(self._cfg_item, CfgItem)
        cfg_dict = self._cfg_item.init_cfg(scenario)
        with open(cfg_file, 'w') as f:
            json.dump(cfg_dict, f, indent=4)
        return

    def load(self, cfg_file):
        cfg_dict = None
        chk.check_file_exist(cfg_file)
        # load configuration from file
        with open(cfg_file, 'r') as f:
            cfg_dict = json.load(f, object_pairs_hook=OrderedDict)
        sec_mode   = cfg_dict.get('Security_Mode')
        crypto_alg = cfg_dict.get('Crypto_Algorithm', None)
        start_flow = cfg_dict.get('Start_Flow', None)
        tee_owner  = cfg_dict.get('TEE_Owner', None)
        gsl_enc    = cfg_dict.get('GSL', {}).get('GSL_Key_Area', {}).get('GSL_Code_Enc_Flag', None)
        boot_enc   = cfg_dict.get('Boot', {}).get('Boot_Params_Area', {}).get('Boot_Enc_Flag', None)
        self._scenario = Scenario(
            sec_mode   = sec_mode,
            crypto_alg = crypto_alg,
            start_flow = start_flow,
            tee_owner  = tee_owner,
            gsl_enc    = gsl_enc,
            boot_enc   = boot_enc
        )
        self._cfg_obj = self._cfg_item.convert_cfg(cfg_dict, self._scenario)
        self.reset_path()

    def scenario(self):
        return self._scenario

    def reset_path(self, *path):
        self._sub_obj = self._cfg_obj
        # reset path as far as possible
        for key in path:
            if key not in self._sub_obj:
                return False
            self._sub_obj = self._sub_obj.get(key, None)
        return True

    def get_value(self, key, default=None):
        value = self._sub_obj.get(key, default)
        return value

def _get_scen_item_from_user(item):
    chk.check_option(
            val=item, notice='Scenario Item', options=[
                'Security_Mode', 'Crypto_Algorithm',
                'Start_Flow', 'TEE_Owner',
                'GSL_Code_Enc_Flag', 'Boot_Enc_Flag'
            ],
        )

    item_options = {
        'Security_Mode': [
            'Security Mode',
            [CfgValue.SecMode.NON_SECURE, CfgValue.SecMode.SECURE]
        ],
        'Start_Flow': [
            'Start Flow',
            [CfgValue.StartFlow.NON_TEE, CfgValue.StartFlow.TEE]
        ],
        'TEE_Owner': [
            'TEE Owner',
            [CfgValue.Owner.OEM, CfgValue.Owner.VENDOR]
        ],
        'GSL_Code_Enc_Flag': [
            'Encrypt GSL Code',
            [CfgValue.EncFlag.NON_ENC, CfgValue.EncFlag.ENC]
        ],
        'Boot_Enc_Flag': [
            'Encrypt Boot Code',
            [CfgValue.EncFlag.NON_ENC, CfgValue.EncFlag.ENC]
        ],
    }
    tag = item_options.get(item)[0]     # description
    options = item_options.get(item)[1] # options

    tips = "\n%s:\n" % tag
    for i, opt in enumerate(options):
        opt = 'No' if opt == CfgValue.EncFlag.NON_ENC else opt
        opt = 'YES' if opt == CfgValue.EncFlag.ENC else opt
        tips += '%d.%s\n' % (i, opt)
    tips+='> '

    cnt = 5
    while (1):
        if cnt == 0:
            log.error('Failure to get scenario.')
            sys.exit(1)
        cnt -= 1

        try:
            num = int(input(tips))
        except Exception:
            print('\nRetry. Required an integer as input.')
            continue

        if num < 0 or num >= len(options):
            print('\nRetry. Please input 0 ~ %d.' % (len(options) - 1))
        else:
            break

    print('Input:%s' % num)
    return options[num]


def get_scen_from_user(owner):
    chk.check_option(
        owner, [
            CfgValue.Owner.VENDOR,
            CfgValue.Owner.OEM,
            CfgValue.Owner.THIRD_PARTY
        ],
        'Owner'
    )

    sec_mode   = None
    crypto_alg = None
    start_flow = None
    tee_owner  = None
    gsl_enc    = None
    boot_enc   = None

    if owner == CfgValue.Owner.OEM:
        sec_mode = _get_scen_item_from_user('Security_Mode')
    else:
        sec_mode = CfgValue.SecMode.SECURE

    if sec_mode != CfgValue.SecMode.SECURE:
        return Scenario(
            sec_mode   = sec_mode,
            start_flow = CfgValue.StartFlow.NON_TEE,
            tee_owner  = CfgValue.Owner.OEM,
            gsl_enc    = CfgValue.EncFlag.NON_ENC,
            boot_enc   = CfgValue.EncFlag.NON_ENC
        )

    crypto_alg = CfgValue.CryptoAlg.RSA

    if owner == CfgValue.Owner.VENDOR:
        # Vendor
        start_flow = CfgValue.StartFlow.TEE
        tee_owner  = CfgValue.Owner.VENDOR
        gsl_enc    = _get_scen_item_from_user('GSL_Code_Enc_Flag')

    elif owner == CfgValue.Owner.OEM:
        # OEM
        start_flow = _get_scen_item_from_user('Start_Flow')
        if start_flow == CfgValue.StartFlow.TEE:
            tee_owner = _get_scen_item_from_user('TEE_Owner')
        else:
            tee_owner = CfgValue.Owner.OEM
        if tee_owner == CfgValue.Owner.OEM:
            gsl_enc = _get_scen_item_from_user('GSL_Code_Enc_Flag')
        boot_enc = _get_scen_item_from_user('Boot_Enc_Flag')

    elif owner == CfgValue.Owner.THIRD_PARTY:
        # Third party
        start_flow = _get_scen_item_from_user('Start_Flow')

    return Scenario(
        sec_mode   = sec_mode,
        crypto_alg = crypto_alg,
        start_flow = start_flow,
        tee_owner  = tee_owner,
        gsl_enc    = gsl_enc,
        boot_enc   = boot_enc
    )
