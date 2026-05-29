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
    scen = get_scen_from_user(OWNER.OEM)
    cfgtor = Configurator(OEM_CFG_ITEM)
    cfgtor.dump_init(cfg_file, scen)
    log.info('Configuration is initialized in \'%s\'' % cfg_file)
    return

def load(cfg_file):
    cfgtor = Configurator(OEM_CFG_ITEM)
    cfgtor.load(cfg_file)
    return cfgtor

OEM_CFG_ITEM = CfgItem(
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
    CfgItem(
        key='TEE_Owner',
        typ=CfgValueType.STRING,
        usage='%s or %s' % (OWNER.OEM, OWNER.VENDOR),
        scens=[
            Scen(sec_mode=SEC_MODE.SECURE, start_flow=START_FLOW.TEE, tee_owner=OWNER.VENDOR, value=OWNER.VENDOR),
            Scen(sec_mode=SEC_MODE.SECURE, start_flow=START_FLOW.TEE, tee_owner=OWNER.OEM, value=OWNER.OEM),
        ],
        spec=Spec(options=[OWNER.OEM, OWNER.VENDOR])
    ),
    CfgItem(key='OEM_Info', scens=[Scen(sec_mode=SEC_MODE.SECURE)], sub_items=[
        CfgItem(
            key='OEM_MSID_Ext',
            typ=CfgValueType.UINT32,
            usage='defined by OEM',
            scens=[Scen(value='0x00000000')],
            spec=Spec(n_bytes=4)
        ),
        CfgItem(
            key='Mask_OEM_MSID_Ext',
            typ=CfgValueType.UINT32,
            usage='mask of the OEM_MSID',
            scens=[Scen(value='0x00000000')],
            spec=Spec(n_bytes=4)
        ),
        CfgItem(
            key='OEM_Version_Ext',
            typ=CfgValueType.UINT32,
            usage='the number of \'1\' bits indicates the version number',
            scens=[Scen(value='0x00000000')],
            spec=Spec(n_bytes=4)
        ),
        CfgItem(
            key='Mask_OEM_Version_Ext',
            typ=CfgValueType.UINT32,
            usage='mask of the OEM_Version',
            scens=[Scen(value='0x00000000')],
            spec=Spec(n_bytes=4)
        ),
    ]),
    CfgItem(key='Root', scens=[Scen(sec_mode=SEC_MODE.SECURE)], sub_items=[
        CfgItem(
            key='OEM_Root_Private_Key',
            typ=CfgValueType.PATH,
            usage='path of the private key in PEM format'
        ),
        CfgItem(key='OEM_Root_Public_Key_Area', sub_items=[
            CfgItem(
                key='OEM_Root_Public_Key',
                typ=CfgValueType.PATH,
                usage='path of the public key in PEM format',
                deprecated=True,
            ),
            CfgItem(
                key='OEM_ProtectionKey_L1_Enc',
                typ=CfgValueType.HEX,
                usage='an 128-bit hexadecimal value with leading \'0x\''
            ),
            CfgItem(
                key='OEM_ProtectionKey_L2_Enc',
                typ=CfgValueType.HEX,
                usage='a 256-bit hexadecimal value with leading \'0x\''
            ),
            CfgItem(
                key='OEM_ProtectionKey_L1',
                typ=CfgValueType.IN_FILE,
                usage='an 128-bit binary key file path',
                deprecated=True,
            ),
            CfgItem(
                key='OEM_ProtectionKey_L2',
                typ=CfgValueType.IN_FILE,
                usage='a 256-bit binary key file path',
                deprecated=True,
            ),
            CfgItem(
                key='OEM_Public_IV',
                typ=CfgValueType.HEX,
                usage='an 128-bit hexadecimal value with leading \'0x\'',
                spec=Spec(n_bits=128)
            ),
            CfgItem(
                key='OEM_Public_RootKey',
                typ=CfgValueType.IN_FILE,
                usage='path of the binary OEM_Public_RootKey'
            ),
        ]),
    ]),
    CfgItem(key='GSL', scens=[Scen(tee_owner=OWNER.OEM)], sub_items=[
        CfgItem(
            key='GSL_External_Private_Key',
            typ=CfgValueType.PATH,
            usage='path of the private key in PEM format',
            scens=[Scen(sec_mode=SEC_MODE.SECURE)]
        ),
        CfgItem(key='GSL_Key_Area', sub_items=[
            CfgItem(
                key='GSL_External_Public_Key',
                typ=CfgValueType.PATH,
                usage='path of the public key in PEM format',
                scens=[Scen(sec_mode=SEC_MODE.SECURE)],
                deprecated=True,
            ),
            CfgItem(
                key='GSL_ProtectionKey_L1_Enc',
                typ=CfgValueType.HEX,
                usage='an 128-bit hexadecimal value with leading \'0x\'',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, gsl_enc=ENC_FLAG.ENC)]
            ),
            CfgItem(
                key='GSL_ProtectionKey_L2_Enc',
                typ=CfgValueType.HEX,
                usage='a 256-bit hexadecimal value with leading \'0x\'',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, gsl_enc=ENC_FLAG.ENC)]
            ),
            CfgItem(
                key='GSL_ProtectionKey_L1',
                typ=CfgValueType.IN_FILE,
                usage='an 128-bit binary key file path',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, gsl_enc=ENC_FLAG.ENC)],
                deprecated=True,
            ),
            CfgItem(
                key='GSL_ProtectionKey_L2',
                typ=CfgValueType.IN_FILE,
                usage='a 256-bit binary key file path',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, gsl_enc=ENC_FLAG.ENC)],
                deprecated=True,
            ),
            CfgItem(
                key='GSL_IV',
                typ=CfgValueType.HEX,
                usage='an 128-bit hexadecimal value with leading \'0x\'',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, gsl_enc=ENC_FLAG.ENC)],
                spec=Spec(n_bits=128)
            ),
            CfgItem(
                key='GSL_RootKey',
                typ=CfgValueType.IN_FILE,
                usage='path of the binary GSL_RootKey',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, gsl_enc=ENC_FLAG.ENC)]
            ),
            CfgItem(
                key='GSL_Code_Enc_Flag',
                typ=CfgValueType.UINT32,
                usage='%s means non-encrypted' % ENC_FLAG.NON_ENC,
                scens=[
                    Scen(gsl_enc=ENC_FLAG.ENC, value=ENC_FLAG.ENC),
                    Scen(gsl_enc=ENC_FLAG.NON_ENC, value=ENC_FLAG.NON_ENC)
                ],
                spec=Spec(n_bytes=4)
            )
        ]),
        CfgItem(key='GSL_Code_Area', sub_items=[
            CfgItem(
                key='GSL_Image_Info',
                typ=CfgValueType.HEX,
                usage='hexdump of GSL information',
                scens=[Scen(value='0x00')],
                spec=Spec(max_bytes=0x20)
            ),
            CfgItem(
                key='GSL_Code',
                typ=CfgValueType.IN_FILE,
                usage='path of the binary GSL code '
            ),
        ])
    ]),
    CfgItem(key='Boot', sub_items=[
        CfgItem(
            key='REE_External_Private_Key',
            typ=CfgValueType.PATH,
            usage='path of the private key in PEM format',
            scens=[Scen(sec_mode=SEC_MODE.SECURE)]
        ),
        CfgItem(key='Boot_Key_Area', scens=[Scen(sec_mode=SEC_MODE.SECURE)], sub_items=[
            CfgItem(
                key='REE_External_Public_Key',
                typ=CfgValueType.PATH,
                usage='path of the public key in PEM format'
            ),
        ], deprecated=True,),
        CfgItem(key='Boot_Params_Area', sub_items=[
            CfgItem(
                key='Boot_Entry_Point',
                typ=CfgValueType.UINT64,
                usage='address of the Boot Code in menory',
                scens=[Scen(value='0x0000000041800000')],
                spec=Spec(n_bits=64)
            ),
            CfgItem(
                key='Boot_Enc_Flag',
                typ=CfgValueType.UINT32,
                usage='%s means non-encrypted' % ENC_FLAG.NON_ENC,
                scens=[
                    Scen(boot_enc=ENC_FLAG.ENC, value=ENC_FLAG.ENC),
                    Scen(boot_enc=ENC_FLAG.NON_ENC, value=ENC_FLAG.NON_ENC)
                ],
                spec=Spec(n_bytes=4)
            ),
            CfgItem(
                key='Flash_ProtectionKey_Boot_L1_Enc',
                typ=CfgValueType.HEX,
                usage='an 128-bit hexadecimal value with leading \'0x\'',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, boot_enc=ENC_FLAG.ENC)]
            ),
            CfgItem(
                key='Flash_ProtectionKey_Boot_L2_Enc',
                typ=CfgValueType.HEX,
                usage='a 256-bit hexadecimal value with leading \'0x\'',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, boot_enc=ENC_FLAG.ENC)]
            ),
            CfgItem(
                key='Flash_ProtectionKey_Boot_L1',
                typ=CfgValueType.IN_FILE,
                usage='an 128-bit binary key file path',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, boot_enc=ENC_FLAG.ENC)],
                deprecated=True,
            ),
            CfgItem(
                key='Flash_ProtectionKey_Boot_L2',
                typ=CfgValueType.IN_FILE,
                usage='a 256-bit binary key file path',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, boot_enc=ENC_FLAG.ENC)],
                deprecated=True,
            ),
            CfgItem(
                key='Flash_IV_Boot',
                typ=CfgValueType.HEX,
                usage='an 128-bit hexadecimal value with leading \'0x\'',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, boot_enc=ENC_FLAG.ENC)],
                spec=Spec(n_bits=128)
            ),
            CfgItem(
                key='FlashProt_MasterKey_Boot',
                typ=CfgValueType.IN_FILE,
                usage='path of the binary FlashProt_MasterKey_Boot',
                scens=[Scen(sec_mode=SEC_MODE.SECURE, boot_enc=ENC_FLAG.ENC)]
            ),
            CfgItem(
                key='Cfg_Param',
                typ=CfgValueType.IN_FILE,
                usage='path of .reg file used for building U-Boot'
            ),
        ]),
        CfgItem(key='Unchecked_Area_for_Vendor', sub_items=[
            CfgItem(
                key='SCS_simulate_flag',
                typ=CfgValueType.UINT32,
                usage='0x69875AB4 means no SCS authentication',
                scens=[
                    Scen(value='0x69875AB4')
                ],
                spec=Spec(n_bytes=4)
            )
        ]),
        CfgItem(key='Boot_Area', sub_items=[
            CfgItem(
                key='Unchecked_Area',
                typ=CfgValueType.HEX,
                usage='A reserved feild for OEM',
                scens=[Scen(value='0x0000000000000000')],
            ),
            CfgItem(
                key='Boot_Code',
                typ=CfgValueType.IN_FILE,
                usage='path of the binary U-Boot code'
            ),
        ])
    ]),
    CfgItem(key='TEE', scens=[Scen(sec_mode=SEC_MODE.SECURE, start_flow=START_FLOW.TEE, tee_owner=OWNER.OEM)], sub_items=[
        CfgItem(
            key='TEE_External_Private_Key',
            typ=CfgValueType.PATH,
            usage='path of the private key in PEM format'
        ),
        CfgItem(key='TEE_Key_Area', sub_items=[
            CfgItem(
                key='TEE_External_Public_Key',
                typ=CfgValueType.PATH,
                usage='path of the public key in PEM format',
                deprecated=True,
            ),
            CfgItem(
                key='TEE_MSID_Ext',
                typ=CfgValueType.UINT32,
                usage='defined by TEE Owner',
                scens=[Scen(value='0x00000000')],
                spec=Spec(n_bytes=4)
            ),
            CfgItem(
                key='Mask_TEE_MSID_Ext',
                typ=CfgValueType.UINT32,
                usage='mask of the TEE_MSID',
                scens=[Scen(value='0x00000000')],
                spec=Spec(n_bytes=4)
            ),
            CfgItem(
                key='TEE_SecVersion_Ext',
                typ=CfgValueType.UINT32,
                usage='the number of security version',
                scens=[Scen(value='0x00000000')],
                spec=Spec(n_bytes=4)
            ),
            CfgItem(
                key='Mask_TEE_SecVersion_Ext',
                typ=CfgValueType.UINT32,
                usage='mask of the TEE_SecVersion',
                scens=[Scen(value='0x00000000')],
                spec=Spec(n_bytes=4)
            ),
            CfgItem(
                key='ATF_ProtectionKey_L1_Enc',
                typ=CfgValueType.HEX,
                usage='an 128-bit hexadecimal value with leading \'0x\''
            ),
            CfgItem(
                key='ATF_ProtectionKey_L2_Enc',
                typ=CfgValueType.HEX,
                usage='a 256-bit hexadecimal value with leading \'0x\''
            ),
            CfgItem(
                key='TEE_ProtectionKey_L1_Enc',
                typ=CfgValueType.HEX,
                usage='an 128-bit hexadecimal value with leading \'0x\''
            ),
            CfgItem(
                key='TEE_ProtectionKey_L2_Enc',
                typ=CfgValueType.HEX,
                usage='a 256-bit hexadecimal value with leading \'0x\''
            ),
            CfgItem(
                key='ATF_ProtectionKey_L1',
                typ=CfgValueType.IN_FILE,
                usage='An 128-bitbinary key file path',
                deprecated=True,
            ),
            CfgItem(
                key='ATF_ProtectionKey_L2',
                typ=CfgValueType.IN_FILE,
                usage='a 256-bit binary key file path',
                deprecated=True,
            ),
            CfgItem(
                key='TEE_ProtectionKey_L1',
                typ=CfgValueType.IN_FILE,
                usage='An 128-bitbinary key file path',
                deprecated=True,
            ),
            CfgItem(
                key='TEE_ProtectionKey_L2',
                typ=CfgValueType.IN_FILE,
                usage='a 256-bit binary key file path',
                deprecated=True,
            ),
            CfgItem(
                key='ATF_IV',
                typ=CfgValueType.HEX,
                usage='an 128-bit hexadecimal value with leading \'0x\'',
                spec=Spec(n_bits=128)
            ),
            CfgItem(
                key='TEE_IV',
                typ=CfgValueType.HEX,
                usage='an 128-bit Hexadecimal value with leading \'0x\'',
                spec=Spec(n_bits=128)
            ),
            CfgItem(
                key='TEE_Func_Version',
                typ=CfgValueType.UINT32,
                usage='TEE fucntion version',
                scens=[Scen(value='0x00000000')],
                spec=Spec(n_bytes=4)
            ),
            CfgItem(
                key='TEE_DDR_Address',
                typ=CfgValueType.UINT64,
                usage='secure DDR start address',
                scens=[Scen(value='0x0000000052FFF000')],
                spec=Spec(n_bits=64)
            ),
            CfgItem(
                key='TEE_Reserved_DDR_Size',
                typ=CfgValueType.UINT32,
                usage='secure DDR size',
                scens=[Scen(value='0x01201000')],
                spec=Spec(max_bytes=4)
            ),
            CfgItem(
                key='ATF_RootKey',
                typ=CfgValueType.IN_FILE,
                usage='path of the binary ATF_RootKey'
            ),
            CfgItem(
                key='TEE_RootKey',
                typ=CfgValueType.IN_FILE,
                usage='path of the binary TEE_RootKey'
            ),
        ]),
        CfgItem(key='ATF_Area', sub_items=[
            CfgItem(
                key='ATF_Code',
                typ=CfgValueType.IN_FILE,
                usage='path of the binary ATF code'
            ),
        ]),
        CfgItem(key='TEE_Code_Area', sub_items=[
            CfgItem(
                key='TEE_Code',
                typ=CfgValueType.IN_FILE,
                usage='path of the binary TEE code'
            ),
        ])
    ])
]
)
