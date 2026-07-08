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

#ifndef PROTOBUF_C_dump_5fdata_2eproto__INCLUDED
#define PROTOBUF_C_dump_5fdata_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error Incompatible protobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error Incompatible protoc-c output. Please regenerate this file.
#endif


typedef struct _Toolkit__Dumpdata__OriginalOp Toolkit__Dumpdata__OriginalOp;
typedef struct _Toolkit__Dumpdata__Shape Toolkit__Dumpdata__Shape;
typedef struct _Toolkit__Dumpdata__OpOutput Toolkit__Dumpdata__OpOutput;
typedef struct _Toolkit__Dumpdata__DumpData Toolkit__Dumpdata__DumpData;


/* --- enums --- */

typedef enum _Toolkit__Dumpdata__OutputDataType {
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_UNDEFINED = 0,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_FLOAT = 1,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_FLOAT16 = 2,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_INT8 = 3,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_UINT8 = 4,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_INT16 = 5,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_UINT16 = 6,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_INT32 = 7,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_INT64 = 8,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_UINT32 = 9,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_UINT64 = 10,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_BOOL = 11,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_DOUBLE = 12,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_STRING = 13,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_DUAL_SUB_INT8 = 14,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_DUAL_SUB_UINT8 = 15,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_COMPLEX64 = 16,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_COMPLEX128 = 17,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_QINT8 = 18,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_QINT16 = 19,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_QINT32 = 20,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_QUINT8 = 21,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_QUINT16 = 22,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_RESOURCE = 23,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_STRING_REF = 24,
  TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_DUAL = 25
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE)
} Toolkit__Dumpdata__OutputDataType;
typedef enum _Toolkit__Dumpdata__OutputFormat {
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_NCHW = 0,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_NHWC = 1,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_ND = 2,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_NC1HWC0 = 3,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_FRACTAL_Z = 4,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_NC1C0HWPAD = 5,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_NHWC1C0 = 6,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_FSR_NCHW = 7,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_FRACTAL_DECONV = 8,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_C1HWNC0 = 9,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_FRACTAL_DECONV_TRANSPOSE = 10,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_FRACTAL_DECONV_SP_STRIDE_TRANS = 11,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_NC1HWC0_C04 = 12,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_FRACTAL_Z_C04 = 13,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_CHWN = 14,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_FRACTAL_DECONV_SP_STRIDE8_TRANS = 15,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_HWCN = 16,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_NC1KHKWHWC0 = 17,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_BN_WEIGHT = 18,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_FILTER_HWCK = 19,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_HASHTABLE_LOOKUP_LOOKUPS = 20,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_HASHTABLE_LOOKUP_KEYS = 21,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_HASHTABLE_LOOKUP_VALUE = 22,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_HASHTABLE_LOOKUP_OUTPUT = 23,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_HASHTABLE_LOOKUP_HITS = 24,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_C1HWNCoC0 = 25,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_MD = 26,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_NDHWC = 27,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_FRACTAL_ZZ = 28,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_FRACTAL_NZ = 29,
  TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_RESERVED = 30
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(TOOLKIT__DUMPDATA__OUTPUT_FORMAT)
} Toolkit__Dumpdata__OutputFormat;

/* --- messages --- */

struct  _Toolkit__Dumpdata__OriginalOp {
    ProtobufCMessage base;
    char *name;
    uint32_t output_index;
    Toolkit__Dumpdata__OutputDataType data_type;
    Toolkit__Dumpdata__OutputFormat format;
};
#define TOOLKIT__DUMPDATA__ORIGINAL_OP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&toolkit__dumpdata__original_op__descriptor), \
    (char *)protobuf_c_empty_string, 0, \
    TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_UNDEFINED, \
    TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_NCHW }


struct  _Toolkit__Dumpdata__Shape {
    ProtobufCMessage base;
    size_t n_dim;
    uint64_t *dim;
};
#define TOOLKIT__DUMPDATA__SHAPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&toolkit__dumpdata__shape__descriptor), \
    0, NULL }


struct  _Toolkit__Dumpdata__OpOutput {
    ProtobufCMessage base;
    Toolkit__Dumpdata__OutputDataType data_type;
    Toolkit__Dumpdata__OutputFormat format;
    Toolkit__Dumpdata__Shape *shape;
    /*
   * the original op corresponding to the output
   */
  Toolkit__Dumpdata__OriginalOp *original_op;
  ProtobufCBinaryData data;
};
#define TOOLKIT__DUMPDATA__OP_OUTPUT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&toolkit__dumpdata__op_output__descriptor), \
    TOOLKIT__DUMPDATA__OUTPUT_DATA_TYPE__DT_UNDEFINED, \
    TOOLKIT__DUMPDATA__OUTPUT_FORMAT__FORMAT_NCHW, \
    NULL, NULL, {0, NULL} }


struct  _Toolkit__Dumpdata__DumpData {
    ProtobufCMessage base;
    char *version;
    uint64_t dump_time;
    size_t n_output;
    Toolkit__Dumpdata__OpOutput **output;
};
#define TOOLKIT__DUMPDATA__DUMP_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&toolkit__dumpdata__dump_data__descriptor), \
    (char *)protobuf_c_empty_string, 0, 0, NULL }


/* Toolkit__Dumpdata__OriginalOp methods */
void   toolkit__dumpdata__original_op__init
                     (Toolkit__Dumpdata__OriginalOp         *message);
size_t toolkit__dumpdata__original_op__get_packed_size
                     (const Toolkit__Dumpdata__OriginalOp   *message);
size_t toolkit__dumpdata__original_op__pack
                     (const Toolkit__Dumpdata__OriginalOp   *message,
                      uint8_t             *out);
size_t toolkit__dumpdata__original_op__pack_to_buffer
                     (const Toolkit__Dumpdata__OriginalOp   *message,
                      ProtobufCBuffer     *buffer);
Toolkit__Dumpdata__OriginalOp *toolkit__dumpdata__original_op__unpack(
    ProtobufCAllocator  *allocator,
    size_t               len,
    const uint8_t       *data);
void   toolkit__dumpdata__original_op__free_unpacked
                     (Toolkit__Dumpdata__OriginalOp *message,
                      ProtobufCAllocator *allocator);
/* Toolkit__Dumpdata__Shape methods */
void   toolkit__dumpdata__shape__init
                     (Toolkit__Dumpdata__Shape         *message);
size_t toolkit__dumpdata__shape__get_packed_size
                     (const Toolkit__Dumpdata__Shape   *message);
size_t toolkit__dumpdata__shape__pack
                     (const Toolkit__Dumpdata__Shape   *message,
                      uint8_t             *out);
size_t toolkit__dumpdata__shape__pack_to_buffer
                     (const Toolkit__Dumpdata__Shape   *message,
                      ProtobufCBuffer     *buffer);
Toolkit__Dumpdata__Shape *toolkit__dumpdata__shape__unpack(
    ProtobufCAllocator  *allocator,
    size_t               len,
    const uint8_t       *data);
void   toolkit__dumpdata__shape__free_unpacked
                     (Toolkit__Dumpdata__Shape *message,
                      ProtobufCAllocator *allocator);
/* Toolkit__Dumpdata__OpOutput methods */
void   toolkit__dumpdata__op_output__init
                     (Toolkit__Dumpdata__OpOutput         *message);
size_t toolkit__dumpdata__op_output__get_packed_size
                     (const Toolkit__Dumpdata__OpOutput   *message);
size_t toolkit__dumpdata__op_output__pack
                     (const Toolkit__Dumpdata__OpOutput   *message,
                      uint8_t             *out);
size_t toolkit__dumpdata__op_output__pack_to_buffer
                     (const Toolkit__Dumpdata__OpOutput   *message,
                      ProtobufCBuffer     *buffer);
Toolkit__Dumpdata__OpOutput *toolkit__dumpdata__op_output__unpack(
    ProtobufCAllocator  *allocator,
    size_t               len,
    const uint8_t       *data);
void   toolkit__dumpdata__op_output__free_unpacked
                     (Toolkit__Dumpdata__OpOutput *message,
                      ProtobufCAllocator *allocator);
/* Toolkit__Dumpdata__DumpData methods */
void   toolkit__dumpdata__dump_data__init
                     (Toolkit__Dumpdata__DumpData         *message);
size_t toolkit__dumpdata__dump_data__get_packed_size
                     (const Toolkit__Dumpdata__DumpData   *message);
size_t toolkit__dumpdata__dump_data__pack
                     (const Toolkit__Dumpdata__DumpData   *message,
                      uint8_t             *out);
size_t toolkit__dumpdata__dump_data__pack_to_buffer
                     (const Toolkit__Dumpdata__DumpData   *message,
                      ProtobufCBuffer     *buffer);
Toolkit__Dumpdata__DumpData *toolkit__dumpdata__dump_data__unpack(
    ProtobufCAllocator *allocator, size_t len, const uint8_t *data);
void   toolkit__dumpdata__dump_data__free_unpacked
                     (Toolkit__Dumpdata__DumpData *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Toolkit__Dumpdata__OriginalOp_Closure)
                 (const Toolkit__Dumpdata__OriginalOp *message,
    void *closure_data);
typedef void (*Toolkit__Dumpdata__Shape_Closure)
                 (const Toolkit__Dumpdata__Shape *message,
                  void *closure_data);
typedef void (*Toolkit__Dumpdata__OpOutput_Closure)
                 (const Toolkit__Dumpdata__OpOutput *message,
                  void *closure_data);
typedef void (*Toolkit__Dumpdata__DumpData_Closure)
                 (const Toolkit__Dumpdata__DumpData *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    toolkit__dumpdata__output_data_type__descriptor;
extern const ProtobufCEnumDescriptor    toolkit__dumpdata__output_format__descriptor;
extern const ProtobufCMessageDescriptor toolkit__dumpdata__original_op__descriptor;
extern const ProtobufCMessageDescriptor toolkit__dumpdata__shape__descriptor;
extern const ProtobufCMessageDescriptor toolkit__dumpdata__op_output__descriptor;
extern const ProtobufCMessageDescriptor toolkit__dumpdata__dump_data__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_dump_5fdata_2eproto__INCLUDED */
