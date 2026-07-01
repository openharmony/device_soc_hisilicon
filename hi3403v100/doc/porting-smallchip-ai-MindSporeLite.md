# MindSpore Lite 集成适配 Small 系统

## 1. 概述

### 1.1 目标
在 OpenHarmony small 系统（ipcamera_hispark_aifly_linux）上集成 MindSpore Lite CPU 推理库，支持模型推理功能。

### 1.2 硬件平台
- **芯片**: Hi3403V100
- **CPU**: 四核 ARM Cortex-A55@1.4GHz
- **架构**: ARMv8.2-A（支持 NEON、FP16、dotprod 扩展）
- **系统类型**: small 系统（lite 内核）

### 1.3 MindSpore Lite 版本
- 基于 MindSpore 2.3.0
- 仅包含 CPU 推理功能（不含训练、NPU、GPU 等）

---

## 2. 修改文件清单

### 2.1 产品配置

#### 2.1.1 `vendor/hisilicon/hispark_aifly_linux/config.json`
**修改内容**: 添加 mindspore 组件到 thirdparty subsystem

```json
{
  "subsystem": "thirdparty",
  "components": [
    { "component": "libuv", "features":[] },
    { "component": "mindspore", "features":[] }
  ]
}
```

#### 2.1.2 `device/board/hisilicon/hispark_aifly/linux/config.gni`
**修改内容**: 修正 board_arch 配置

```gn
# 修改前
board_arch = "armv8-a"

# 修改后
board_arch = "armv8.2-a"
```

**原因**: Hi3403V100 使用 Cortex-A55，属于 ARMv8.2-A 架构，支持 dotprod 和 FP16 扩展指令集。

---

### 2.2 MindSpore 组件配置

#### 2.2.1 `third_party/mindspore/bundle.json`
**修改内容**:
- `adapted_system_type` 添加 "small"
- 添加 `hilog_lite` 依赖

```json
{
  "adapted_system_type": ["standard", "small"],
  "deps": {
    "third_party": [
      "bounds_checking_function",
      "flatbuffers",
      "hilog_lite"
    ]
  }
}
```

#### 2.2.2 `third_party/mindspore/mindspore-src/source/mindspore/lite/BUILD.gn`
**修改内容**:

1. **mindspore group deps** - 添加 benchmark 工具（仅 small 系统）
```gn
if (is_small_system) {
  deps += [ "tools/benchmark:benchmark_bin" ]
}
```

2. **ENABLE_FP16 定义** - 恢复 small 系统 FP16 支持
```gn
} else if (target_cpu == "arm64") {
  defines += [
    "ENABLE_ARM",
    "ENABLE_ARM64",
    "ENABLE_NEON",
    "ENABLE_FP16",  // small 系统也启用
    ...
  ]
}
```

3. **hilog 依赖** - 使用 standard 隔离
```gn
if (is_standard_system) {
  external_deps = [ "hilog:libhilog" ]
  remove_configs = [ "//build/config/compiler:no_rtti" ]
} else {
  external_deps = [ "hilog_lite:hilog_shared" ]
}
```

---

### 2.3 核心库适配

#### 2.3.1 `third_party/mindspore/mindspore-src/source/mindspore/core/mindrt/BUILD.gn`
**修改内容**:
- 添加 `-fPIC` 编译标志（small 系统隔离）
- 使用 standard 隔离 hilog 依赖

```gn
# Small system specific flags
if (is_small_system) {
  cflags_cc = [ "-fPIC" ]
}

# Hilog dependency
if (is_standard_system) {
  external_deps = [ "hilog:libhilog" ]
  remove_configs = [ "//build/config/compiler:no_rtti" ]
} else {
  external_deps = [ "hilog_lite:hilog_shared" ]
}
```

#### 2.3.2 `third_party/mindspore/mindspore-src/source/mindspore/core/mindrt/src/thread/threadlog.h`
**修改内容**: 添加 LOG_APP 兼容定义

```c
#ifndef LOG_APP
#define LOG_APP LOG_CORE
#endif
```

**原因**: hilog_lite 无 LOG_APP 定义，需要兼容。

#### 2.3.3 `third_party/mindspore/mindspore-src/source/mindspore/lite/src/common/log.cc`
**修改内容**: 添加 LOG_APP 兼容定义

```c
#ifndef LOG_APP
#define LOG_APP LOG_CORE
#endif
```

#### 2.3.4 `third_party/mindspore/mindspore-src/source/mindspore/lite/mindir/src/log.cc`
**修改内容**: 添加 LOG_APP 兼容定义

```c
#ifndef LOG_APP
#define LOG_APP LOG_CORE
#endif
```

---

### 2.4 通用库适配

#### 2.4.1 `third_party/mindspore/mindspore-src/source/mindspore/lite/src/common/BUILD.gn`
**修改内容**:
- 使用 standard 隔离 hi_app_event 源文件
- 使用 standard 隔离 hilog 依赖
- 添加 `-fPIC` 编译标志（small 系统隔离）

```gn
if (is_standard_system) {
  sources += [
    "hi_app_event/hi_app_event.cc",
    "hi_app_event/hi_app_event_thread.cc",
  ]
}

if (is_standard_system) {
  defines += [ "ENABLE_HI_APP_EVENT" ]
  external_deps = [
    "hiappevent:hiappevent_innerapi",
    "hilog:libhilog",
  ]
} else {
  external_deps = [ "hilog_lite:hilog_shared" ]
}

# Small system specific flags
if (is_small_system) {
  cflags_cc += [ "-fPIC" ]
}
```

---

### 2.5 NNACL 库适配

#### 2.5.1 `third_party/mindspore/mindspore-src/source/mindspore/ccsrc/plugin/device/cpu/kernel/nnacl/BUILD.gn`
**修改内容**:

1. **编译标志** - 添加 small 系统特定配置
```gn
if (is_small_system) {
  cflags_c += [
    "-fasm",
    "-fPIC",
  ]
  asmflags = [
    "-march=armv8.2-a+fp16+dotprod",
  ]
}
```

2. **FP16 汇编和 optimizing 汇编** - small 系统也包含
```gn
# source files on arm64
arm64_only_sources = fp16_kernel_sources
arm64_only_sources += fp16_grad_sources
arm64_only_sources += arm64_fp16_assembly_sources
arm64_only_sources += optimizing_assembly_sources
arm64_only_sources += arm64_assembly_sources
arm64_only_sources += arm64_fp32_kernel_sources
```

**原因**: 通过 `asmflags` 指定正确的架构标志，使 small 系统可以编译 FP16 和 dotprod 优化汇编代码。

---

### 2.6 CPU Kernel 适配

#### 2.6.1 `third_party/mindspore/mindspore-src/source/mindspore/lite/src/litert/kernel/cpu/BUILD.gn`
**修改内容**: 添加 `-fPIC` 编译标志（small 系统隔离）

```gn
cflags_cc = [
  "-Wno-ignored-qualifiers",
  "-Wunused-private-field",
  "-Wno-unused-private-field",
  "-Wno-inconsistent-missing-override",
  "-Wno-macro-redefined",
  "-Wno-constant-conversion",
]

# Small system specific flags
if (is_small_system) {
  cflags_cc += [ "-fPIC" ]
}
```

---

### 2.7 MindIR 库适配

#### 2.7.1 `third_party/mindspore/mindspore-src/source/mindspore/lite/mindir/BUILD.gn`
**修改内容**: 使用 standard 隔离 NNRT 相关依赖

```gn
# Standard system: NNRT support + full dependencies
if (is_standard_system) {
  sources += [ "src/mindir_nnrt_lite_graph.cc" ]
  external_deps = [
    "bounds_checking_function:libsec_shared",
    "c_utils:utils",
    "drivers_interface_nnrt:libnnrt_proxy_1.0",
    "drivers_interface_nnrt:libnnrt_proxy_2.0",
    "drivers_interface_nnrt:nnrt_idl_headers",
    "hdf_core:libhdi",
    "hilog:libhilog",
  ]
} else {
  # Small system: basic dependencies only
  external_deps = [
    "bounds_checking_function:libsec_shared",
    "hilog_lite:hilog_shared",
  ]
}
```

---

### 2.8 测试代码适配

#### 2.8.1 `third_party/mindspore/test/BUILD.gn`
**修改内容**: 测试代码仅 standard 系统编译

```gn
if (is_standard_system) {
  # 测试代码...
}
```

---

### 2.9 工具适配

#### 2.9.1 `third_party/mindspore/mindspore-src/source/mindspore/lite/tools/benchmark/BUILD.gn`
**修改内容**: 为 small 系统创建独立的 executable 目标

```gn
if (is_small_system) {
  executable("benchmark_bin") {
    sources = [...]
    include_dirs = [
      "//third_party/flatbuffers/include",
      "//third_party/json/single_include",
      ...
    ]
    defines = [ "MS_COMPILE_OHOS" ]
    output_name = "benchmark"
    output_dir = "$root_out_dir/usr"
  }
} else {
  ohos_executable("benchmark_bin") {
    ...
  }
}
```

**关键点**:
- small 系统使用 `executable` 模板（非 `ohos_executable`）
- 添加 `//third_party/json/single_include` 解决 nlohmann/json.hpp 缺失问题
- 使用 `output_dir = "$root_out_dir/usr"` 控制输出位置

---

## 3. 编译验证

### 3.1 编译命令
```bash
./build.sh --product-name ipcamera_hispark_aifly_linux --ccache --no-prebuilt-sdk
```

### 3.2 编译产物

| 文件 | 大小 | 位置 |
|------|------|------|
| libmindspore-lite.so | 4.0M | `/usr/lib/` |
| libmindspore_lite_ndk.so | 152K | `/usr/lib/` |
| benchmark | 427K | `/usr/bin/` |

### 3.3 依赖检查

**检查方法**: 使用 `readelf -d` 命令查看动态库的 NEEDED 依赖项

```bash
# 检查 libmindspore-lite.so 依赖
readelf -d /usr/lib/libmindspore-lite.so | grep NEEDED

# 检查 libmindspore_lite_ndk.so 依赖
readelf -d /usr/lib/libmindspore_lite_ndk.so | grep NEEDED

# 检查 benchmark 依赖
readelf -d /usr/bin/benchmark | grep NEEDED
```

**libmindspore-lite.so 依赖**:
```
libhilog_shared.so  ✅
libsec_shared.so    ✅
libc++.so           ✅
libc.so             ✅
```

**libmindspore_lite_ndk.so 依赖**:
```
libmindspore-lite.so  ✅
libhilog_shared.so    ✅
libsec_shared.so      ✅
libc++.so             ✅
libc.so               ✅
```

**benchmark 依赖**:
```
libmindspore-lite.so      ✅
libmindspore_lite_ndk.so  ✅
libc++.so                 ✅
libc.so                   ✅
```

### 3.4 符号检查

**核心 API 符号**:
- `Model::Build()` ✅
- `Model::Predict()` ✅
- `Model::Prepare()` ✅
- `Context` 构造函数 ✅
- `ContextUtils` 工具函数 ✅

**CPU Kernel 符号**:
- `MatmulKernel` ✅
- `Conv2DKernel` ✅
- `PoolingKernel` ✅
- `LstmKernel` ✅

---

## 4. 隔离策略说明

### 4.1 隔离变量使用原则

| 场景 | 使用变量 | 原因 |
|------|----------|------|
| **新增代码**（适配 small 系统） | `is_small_system` | 明确针对 small 系统的特殊处理 |
| **原有代码**（standard 系统已有功能） | `is_standard_system` | 保持原有逻辑不变，避免影响 standard 系统 |

### 4.2 隔离示例

```gn
# 新增代码：使用 is_small_system
if (is_small_system) {
  cflags_cc += [ "-fPIC" ]
  asmflags = [ "-march=armv8.2-a+fp16+dotprod" ]
}

# 原有代码：使用 is_standard_system
if (is_standard_system) {
  external_deps = [ "hilog:libhilog" ]
  remove_configs = [ "//build/config/compiler:no_rtti" ]
} else {
  external_deps = [ "hilog_lite:hilog_shared" ]
}
```

---

## 5. 技术要点

### 5.1 FP16 和 dotprod 支持

**问题**: small 系统 lite toolchain 默认不传递完整架构标志给汇编器

**解决方案**: 通过 `asmflags` 显式指定
```gn
asmflags = [ "-march=armv8.2-a+fp16+dotprod" ]
```

**效果**: small 系统可以编译完整的 FP16 和 dotprod 优化汇编代码，获得更好的推理性能。

### 5.2 -fPIC 必要性

**问题**: lite toolchain 静态库默认无 `-fPIC`，链接共享库时报 `R_AARCH64_ADR_PREL_PG_HI21` 错误

**解决方案**: 为 small 系统添加 `-fPIC` 编译标志
```gn
if (is_small_system) {
  cflags_cc += [ "-fPIC" ]
}
```

### 5.3 LOG_APP 兼容

**问题**: hilog_lite 无 LOG_APP 定义，只有 LOG_CORE

**解决方案**: 添加兼容定义
```c
#ifndef LOG_APP
#define LOG_APP LOG_CORE
#endif
```

### 5.4 board_arch 修正

**问题**: 原配置 `board_arch = "armv8-a"` 过于保守

**修正**: 改为 `board_arch = "armv8.2-a"`

**原因**: Cortex-A55 属于 ARMv8.2-A 架构，支持 dotprod 和 FP16 扩展。

---

## 6. 使用说明

### 6.1 模型转换

在 PC 端使用 MindSpore 官方构建脚本转换模型：

```bash
cd /path/to/mindspore
bash build.sh -I x86_64 -j$(nproc)

# 转换模型
./output/tools/converter/converter_lite/converter_lite \
  --fmk=MINDIR \
  --modelFile=model.mindir \
  --outputFile=model
```

### 6.2 模型部署

1. 将 `.ms` 模型文件部署到设备
2. 使用 `libmindspore-lite.so` 进行推理

### 6.3 性能测试

```bash
# 在设备上运行 benchmark
/usr/bin/benchmark --modelFile=/mnt/mobilenetv2.ms --loopCount=10
```

---

## 7. 注意事项

1. **converter 工具不在 small 系统编译**: converter 依赖复杂（protobuf、opencv 等），建议在 PC 端编译使用
2. **NNRT 未适配**: small 系统不包含 NNRT 相关依赖，仅支持 CPU 推理
3. **训练功能未包含**: small 系统不包含训练相关代码
4. **存储空间**: 编译产物约 4.5MB，需确保设备有足够存储空间
