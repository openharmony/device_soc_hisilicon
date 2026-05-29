/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 */

#ifndef FF_BITS_READER_H
#define FF_BITS_READER_H

#include "ot_mw_type.h"

typedef struct {
    const TD_U8 *data;
    TD_U32 size;
    TD_U32 offset;
} FfBitReader;

TD_S32 FfBitsReaderCreate(FfBitReader **outReader, const TD_U8 *data, TD_U32 dataSize);

TD_S32 FfBitsReaderDestroy(FfBitReader* reader);

TD_BOOL FfBitReadU1(FfBitReader* reader);
TD_U8 FfBitReadU2(FfBitReader* reader);
TD_U8 FfBitReadU3(FfBitReader* reader);
TD_U8 FfBitReadU4(FfBitReader* reader);
TD_U8 FfBitReadU5(FfBitReader* reader);
TD_U8 FfBitReadU6(FfBitReader* reader);
TD_U8 FfBitReadU8(FfBitReader* reader);
TD_U16 FfBitReadU16(FfBitReader* reader);
TD_U32 FfBitReadU32(FfBitReader* reader);
TD_U32 FfBitReadUeV(FfBitReader* reader);
TD_S32 FfBitReadSeV(FfBitReader* reader);
TD_U32 FfBitReadUn(FfBitReader* reader, TD_U8 bits);

#endif // FF_BITS_READER_H
