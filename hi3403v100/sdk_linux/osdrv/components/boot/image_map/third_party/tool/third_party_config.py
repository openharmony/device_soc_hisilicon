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
from collections import OrderedDict

SCRIPT_DIR = os.path.dirname(__file__)
WORK_DIR = os.path.abspath('%s/../..' % SCRIPT_DIR)
sys.path.append(WORK_DIR)

from common.config import CfgItem, Spec, Configurator, CfgValueType, CfgValue
from common.config import Scenario as Scen
from common.config import get_scen_from_user
import common.logger as log


SEC_MODE=CfgValue.SecMode
ALG = CfgValue.CryptoAlg
START_FLOW=CfgValue.StartFlow
OWNER = CfgValue.Owner
ENC_FLAG = CfgValue.EncFlag


def init(cfg_file):
    scen = get_scen_from_user(OWNER.THIRD_PARTY)
    cfgtor = Configurator(TP_CFG_ITEM)
    cfgtor.dump_init(cfg_file, scen)
    log.info('Configuration is initialized in \'%s\'' % cfg_file)
    return

def load(cfg_file):
    cfgtor = Configurator(TP_CFG_ITEM)
    cfgtor.load(cfg_file)
    return cfgtor

TP_CFG_ITEM = CfgItem(
sub_items= [
    CfgItem(
        key='Security_Mode',
        typ=CfgValueType.STRING,
        usage='%s or %s' % (SEC_MODE.NON_SECURE, SEC_MODE.SECURE),
        scens=[
            Scen(sec_mode=SEC_MODE.NON_SECURE, value=SEC_MODE.NON_SECURE),
            Scen(sec_mode=SEC_MODE.SECURE, value=SEC_MODE.SECURE),
        ],
        spec=Spec(options=[SEC_MODE.NON_SECURE, SEC_MODE.SECURE])
    ),
    CfgItem(
        key='Crypto_Algorithm',
        typ=CfgValueType.STRING,
        usage='must be %s' % ALG.RSA,
        scens=[
            Scen(sec_mode=SEC_MODE.SECURE, value=ALG.RSA)
        ],
        spec=Spec(options=[ALG.RSA])
    ),
    CfgItem(
        key='Start_Flow',
        typ=CfgValueType.STRING,
        usage='%s or %s' % (START_FLOW.NON_TEE, START_FLOW.TEE),
        scens=[
            Scen(sec_mode=SEC_MODE.SECURE, start_flow=START_FLOW.NON_TEE, value=START_FLOW.NON_TEE),
            Scen(sec_mode=SEC_MODE.SECURE, start_flow=START_FLOW.TEE, value=START_FLOW.TEE),
        ],
        spec=Spec(options=[START_FLOW.NON_TEE, START_FLOW.TEE])
    ),
    CfgItem(key='Root', sub_items=[
        CfgItem(
            key='Third_party_Root_Private_Key',
            typ=CfgValueType.PATH,
            usage='path of the private key in PEM format',
        ),
        CfgItem(key='Third_party_Root_Public_Key_Area', sub_items=[
            CfgItem(
                key='Third_party_Root_Public_Key',
                typ=CfgValueType.PATH,
                usage='path of the public key in PEM format',
                deprecated=True,
            ),
            CfgItem(
                key='Third_party_ProtectionKey_L1_Enc',
                typ=CfgValueType.HEX,
                usage='a 128-bit hexadecimal value with leading \'0x\''
            ),
            CfgItem(
                key='Third_party_ProtectionKey_L2_Enc',
                typ=CfgValueType.HEX,
                usage='a 256-bit hexadecimal value with leading \'0x\''
            ),
            CfgItem(
                key='Third_party_ProtectionKey_L1',
                typ=CfgValueType.IN_FILE,
                usage='an 128-bit binary key file path',
                deprecated=True,
            ),
            CfgItem(
                key='Third_party_ProtectionKey_L2',
                typ=CfgValueType.IN_FILE,
                usage='a 256-bit binary key file path',
                deprecated=True,
            ),
            CfgItem(
                key='Third_party_Public_IV',
                typ=CfgValueType.HEX,
                usage='a 128-bit hexadecimal value with leading \'0x\'',
                spec=Spec(n_bits=128)
            ),
            CfgItem(
                key='Third_party_Public_RootKey',
                typ=CfgValueType.IN_FILE,
                usage='path of the binary Third_party_Public_RootKey'
            ),
        ]),
    ]),
    CfgItem(key='GSL', scens=[Scen(sec_mode=SEC_MODE.SECURE)], sub_items=[
        CfgItem(
            key='Third_party_External_Private_Key',
            typ=CfgValueType.PATH,
            usage='path of the private key in PEM format'
        ),
        CfgItem(key='GSL_Third_party_Key_Area', sub_items=[
            CfgItem(
                key='Third_party_External_Public_Key',
                typ=CfgValueType.PATH,
                usage='path of the public key in PEM format',
                scens=[Scen(sec_mode=SEC_MODE.SECURE)],
                deprecated=True,
            ),
            CfgItem(
                key='Third_party_MSID_Ext',
                typ=CfgValueType.UINT32,
                usage='defined by Third_party',
                scens=[Scen(value='0x00000000')],
                spec=Spec(n_bytes=4)
            ),
            CfgItem(
                key='Mask_Third_party_MSID_Ext',
                typ=CfgValueType.UINT32,
                usage='mask of the Third_party_MSID',
                scens=[Scen(value='0x00000000')],
                spec=Spec(n_bytes=4)
            ),
            CfgItem(
                key='Third_party_Version_Ext',
                typ=CfgValueType.UINT32,
                usage='the number of \'1\' bits indicates the version number',
                scens=[Scen(value='0x00000000')],
                spec=Spec(n_bytes=4)
            ),
            CfgItem(
                key='Mask_Third_party_Version_Ext',
                typ=CfgValueType.UINT32,
                usage='mask of the Third_party_Version',
                scens=[Scen(value='0x00000000')],
                spec=Spec(n_bytes=4)
            )
        ]),
    ])
]
)
