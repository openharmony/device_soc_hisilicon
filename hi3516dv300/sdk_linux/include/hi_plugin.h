/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#ifndef __HI_PLUGIN_TYPE_H__
#define __HI_PLUGIN_TYPE_H__

#include "hi_type.h"

#if defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define EXPORT __attribute__((visibility("default")))
#else
#define EXPORT
#pragma warning cant export dynamic symbos
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

enum {
    MAX_OPERAND_NAME_LEN = 64,
};

typedef struct hiNodePlugin_Shape_S {
    HI_S32 s32H;
    HI_S32 s32W;
    HI_S32 s32C;
} HI_NodePlugin_Shape_S;

typedef enum hiNodePlugin_ElemType_E {
    ELEM_TYPE_U8,
    ELEM_TYPE_U16,
    ELEM_TYPE_U32
} HI_NodePlugin_ElemType_E;

typedef struct hiNodePlugin_Operand_S {
    HI_U64 u64Offset;  // addr
    HI_CHAR mName[MAX_OPERAND_NAME_LEN + 1];
    HI_NodePlugin_ElemType_E enElemType;
    HI_U32 u32Num;
    HI_U32 u32Stride;
    HI_NodePlugin_Shape_S stShape;
} HI_NodePlugin_Operand_S;

typedef struct HiNodeParam {
    HI_VOID *pParam;
    HI_U32 u32Size;
} HI_NodePlugin_NodeParam_S;

typedef HI_S32 (*NodePluginCompute)(const HI_NodePlugin_Operand_S *, HI_U32, HI_NodePlugin_Operand_S *,
    HI_U32, HI_NodePlugin_NodeParam_S *, HI_NodePlugin_NodeParam_S *);

typedef HI_S32 (*NodePluginGetType)(HI_CHAR pszNodeType[]);

EXPORT HI_S32 HI_NodePlugin_Compute(const HI_NodePlugin_Operand_S *pstInputs, HI_U32 u32InputNum,
                                    HI_NodePlugin_Operand_S *pstOutputs,
                                    HI_U32 u32Outputs, HI_NodePlugin_NodeParam_S *pstHyperParam,
                                    HI_NodePlugin_NodeParam_S *pstTrainingParam);

EXPORT HI_S32 HI_NodePlugin_getNodeType(HI_CHAR pszNodeType[]);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
