/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description:  log module id define
 * Author:
 * Create:
 */

#ifndef LOG_MODULE_ID_H
#define LOG_MODULE_ID_H

enum OML_LOG_MODULEID_ENUM {
    LOG_WIFIMODULE      = 0,
    LOG_BTMODULE        = 1,
    LOG_GNSSMODULE      = 2,
    LOG_DSPMODULE       = 3,
    LOG_PFMODULE        = 4,
    LOG_MEDIAMODULE     = 5,
    LOG_NFCMODULE       = 6,
    LOG_APPMODULE       = 7,
    LOG_GPUMODULE       = 8,
    LOG_GUIMODULE       = 9,
    LOG_SLPMODULE       = 10,
    LOG_BTHMODULE       = 11,
    LOG_OHOSMODULE      = 12,
    LOG_EXTMODULE       = 15,
    MODULEID_BUTT       = 16
};

enum OM_MSG_MODULEID {
    OM_WIFI = 0x00,
    OM_BT = 0x10,
    OM_GNSS = 0x20,
    OM_DSP = 0x30,
    OM_PF = 0x40,
    OM_NFC = 0x60,
    OM_IR = 0x70,
    OM_SLP = 0XA0,
    OM_BTH = 0xB0,
    OM_MODULEID_BUTT,
};

#endif /* LOG_MODULE_ID_H */
