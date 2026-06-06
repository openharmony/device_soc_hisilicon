# MPP Sample SS928V100 运行使用指导

---

## 目录

1. [总体概述](#1-总体概述)
2. [视频处理模块](#2-视频处理模块)
3. [音频处理模块](#3-音频处理模块)
4. [智能处理模块](#4-智能处理模块)
5. [图像处理模块](#5-图像处理模块)
6. [功能特性模块](#6-功能特性模块)
7. [高级功能模块](#7-高级功能模块)
8. [其他模块](#8-其他模块)

---

## 1. 总体概述

### 1.1 编译系统说明

本文中sample路径位于hi3403v100/sdk_linux/smp/a55_linux/mpp/sample下，后续取相对路径mpp/sample进行说明。

#### 编译结构
- **顶层Makefile**: 自动发现所有子模块并编译
- **Makefile.param**: 定义传感器类型、音频编解码器等关键配置
- **common/**: 公共函数库，包含各模块的公共初始化和操作函数

#### 编译准备

- 在编译之前需要配置好编译工具链，请参考[官方指导](https://gitcode.com/openharmony/docs/blob/master/zh-cn/device-dev/subsystems/subsys-build-all.md#%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE)配置编译环境

- sample的编译依赖安全函数库和sensor驱动库，因此要预先编译它们，编译方法为：

  ```shell
  # 编译 安全函数库
  cd hi3403v100/sdk_linux/smp/a55_linux/mpp/component/securec
  make
  
  # 编译 sensor 驱动
  cd hi3403v100/sdk_linux/smp/a55_linux/mpp/sample/hy_s0603
  make
  
  cd hi3403v100/sdk_linux/smp/a55_linux/mpp/sample/omnivision_os08a20
  make
  
  cd hi3403v100/sdk_linux/smp/a55_linux/mpp/sample/smart_sc450ai
  make
  ```

#### 编译命令

现在可以开始编译所有的sample，编译方法为：

```bash
# 编译所有sample
cd hi3403v100/sdk_linux/smp/a55_linux/mpp/sample/
make

# 编译特定sample
cd hi3403v100/sdk_linux/smp/a55_linux/mpp/sample/<module_name>
make

# 清理编译
cd hi3403v100/sdk_linux/smp/a55_linux/mpp/sample/
make clean

# 清理并重新编译
cd hi3403v100/sdk_linux/smp/a55_linux/mpp/sample/
make clean && make
```

#### 输出目录
编译后的可执行文件位于：
- `mpp/sample/sample_<module_name>`
- 例如：`mpp/sample/vio/sample_vio`

### 1.2 配置参数详解

#### 传感器类型配置
编辑 `mpp/sample/Makefile.param`：

```makefile
# 默认传感器类型
SENSOR0_TYPE ?= HY_S0603_MIPI_8M_30FPS_12BIT
```

**支持的传感器类型**（在Makefile.param中查看完整列表）：
- `HY_S0603_MIPI_8M_30FPS_12BIT` - 8MP传感器，30fps，12bit
- `HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1` - 8MP传感器，30fps，10bit，WDR 2:1
- 更多传感器类型请参考Makefile.param中的注释

#### 音频编解码器配置
```makefile
# 外部编解码器
# ACODEC_TYPE ?= ACODEC_TYPE_ES8388
# 内部编解码器
ACODEC_TYPE ?= ACODEC_TYPE_INNER
INNER_HDMI ?= y
```

#### 内存共享配置
```bash
# 当MMZ进程隔离启用时，需要设置MEM_SHARE=y
make MEM_SHARE=y
```

#### 音频模块库类型
```makefile
# 静态库（默认）
AUDIO_MODULE_LIB_TYPE ?= AUDIO_LIB_TYPE_STATIC
# 共享库
# AUDIO_MODULE_LIB_TYPE ?= AUDIO_LIB_TYPE_SHARE
```

### 1.3 通用注意事项

#### MMZ进程隔离
如果模块参数 `mem_process_isolation` 设置为 "1"，则启用了MMZ缓冲区的进程隔离属性。在这种情况下，需要编译时设置 `MEM_SHARE=y`：
```bash
make MEM_SHARE=y
```

#### 信号处理和优雅退出
- 所有sample支持Ctrl+C优雅退出
- 信号处理函数格式：`sample_<module>_handle_sig`
- 退出时会自动清理资源

#### 平台兼容性
- 部分特性需要特定编译选项（如 `CONFIG_OT_ISP_SUPPORT`, `CONFIG_OT_AUDIO_SUPPORT`）

#### 常见错误排查

| 错误现象 | 可能原因 | 解决方法 |
|----------|----------|----------|
| VI初始化失败 | 传感器类型配置错误 | 检查Makefile.param中的SENSOR0_TYPE配置 |
| MMZ分配失败 | MMZ进程隔离未处理 | 使用 `make MEM_SHARE=y` 重新编译 |
| 编译失败 | 缺少依赖库 | 确保SDK环境已正确配置 |
| 找不到设备文件 | HDMI未连接或配置错误 | 确认HDMI已连接，检查VO配置 |

### 1.4 快速开始指南

#### 编译和运行第一个Sample
```bash
# 1. 进入sample目录
cd mpp/sample/vio

# 2. 编译
make

# 3. 运行最常用的模式（all mode route）
./sample_vio 0

# 4. 按Ctrl+C退出
```

#### 预期结果
- 程序启动后会显示初始化信息
- HDMI输出应该显示视频画面
- 控制台会打印各模块的初始化状态
- 按回车键或Ctrl+C可以优雅退出

---

## 2. 视频处理模块

### 2.1 VIO Sample - 视频输入输出示例

#### 主要功能
VI（视频输入）到VPSS（视频处理）到VENC（视频编码）和VO（视频输出）的完整数据流演示。

#### 支持的运行模式
- `(0)` all mode route: vi linear(Online/Offline) -> vpss(Online/Offline) -> venc && vo
- `(1)` wdr route: vi wdr(Online) -> vpss(Offline) -> venc && vo
- `(2)` fpn calibrate & correct: vi fpn calibrate & correct -> vpss -> venc && vo
- `(3)` ldc & dis: vi ldc & dis -> vpss -> venc && vo
- `(4)` fisheye: vi fisheye(phys_chn + ext_chn) -> vpss -> venc && vo
- `(5)` low delay: vi(pipe & chn lowdelay) -> vpss(lowdelay) -> venc && vo
- `(6)` input yuv mux: vi 4mux yuv input -> vpss -> venc && vo
- `(7)` user pic: vi user pic (offline) -> vpss -> venc && vo
- `(8)` two sensor: vi two sensor (offline) -> vpss -> venc && vo
- `(9)` switch mode: vi linear switch to wdr -> vpss -> venc && vo
- `(10)` wdr send route: vi wdr(offline) -> isp_runonce -> vpss(Offline) -> venc && vo
- `(11)` switch resolution: vi 4K switch to FHD -> vpss -> venc && vo

#### 应用场景
- 视频采集和编码系统
- 实时视频预览和录制
- 多传感器视频处理
- 低延迟视频传输
- 鱼眼镜头校正
- WDR场景处理

#### 编译运行步骤

##### 编译依赖
- 确保SDK环境已正确配置
- 确保传感器类型已正确配置（默认：HY_S0603_MIPI_8M_30FPS_12BIT）

##### 编译命令
```bash
# 编译vio sample
cd mpp/sample/vio
make

# 启用内存共享（MMZ进程隔离时需要）
make MEM_SHARE=y
```

##### 运行命令
```bash
# 运行模式0（all mode route）
./sample_vio 0

# 运行模式1（wdr route）
./sample_vio 1

# 运行模式7（user pic）
./sample_vio 7
```

##### 输出文件
- 编译输出：`mpp/output/<arch>/<ostype>/sample_vio`
- 编码流文件：根据模式不同，会生成H264/H265编码流文件

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_vio_usage() (参数校验)
  └─ sample_register_sig_handler() (注册信号处理)
  └─ sample_ipc_server_init() (可选，初始化IPC服务)
  └─ SDK_init() (CONFIG_USER_SPACE时)
  └─ sample_vio_execute_case() (执行指定模式)
      └─ sample_vio_all_mode() / sample_vio_wdr() / ... (根据模式选择)
          └─ sample_comm_sys_init() (系统初始化)
          └─ sample_comm_vi_create_vi() (创建VI设备)
          └─ sample_comm_vpss_create() (创建VPSS)
          └─ sample_comm_venc_create() (创建VENC)
          └─ sample_comm_vo_create() (创建VO)
          └─ sample_pause() (等待用户输入)
          └─ sample_comm_sys_exit() (清理资源)
  └─ sample_ipc_server_deinit()
  └─ SDK_exit()
```

##### 关键函数调用顺序
1. 参数解析和校验
2. 信号处理注册（SIGINT, SIGTERM）
3. IPC服务器初始化（可选）
4. SDK初始化
5. 选择并执行特定模式的sample函数
6. 等待用户输入（getchar）
7. 资源清理和退出

##### 数据流向
```
VI -> VPSS -> VENC (编码输出)
       -> VO (显示输出)
```

#### 关键数据

##### 重要配置参数
| 参数名 | 默认值 | 说明 |
|--------|--------|------|
| SENSOR0_TYPE | HY_S0603_MIPI_8M_30FPS_12BIT | 传感器类型 |
| g_vo_cfg.vo_dev | SAMPLE_VO_DEV_UHD | VO设备号 |
| g_vo_cfg.intf_sync | HI_VO_OUT_1080P30 | 输出时序 |
| g_vo_cfg.pix_format | HI_PIXEL_FORMAT_YVU_SEMIPLANAR_420 | 像素格式 |
| g_venc_chn_param.frame_rate | 30 | 帧率 |
| g_venc_chn_param.gop | 30 | GOP大小 |

##### 关键数据结构
```c
typedef struct {
    hi_vo_dev vo_dev;              // VO设备号
    hi_vo_intf_type intf_type;     // 接口类型
    hi_vo_intf_sync intf_sync;     // 输出时序
    hi_color bg_color;             // 背景色
    hi_pixel_format pix_format;    // 像素格式
    hi_rect disp_rect;             // 显示区域
    hi_size image_size;            // 图像尺寸
    // ...
} sample_vo_cfg;
```

##### 重要宏定义
- `PIC_3840X2160`: 4K分辨率 (3840x2160)
- `PIC_1080P`: 1080P分辨率 (1920x1080)
- `SAMPLE_VO_DEV_UHD`: UHD设备号 (0)
- `HI_PIXEL_FORMAT_YVU_SEMIPLANAR_420`: YUV420半平面格式

#### 注意事项

##### MMZ进程隔离
如果模块参数 "mem_process_isolation" 设置为 "1"，则启用了MMZ缓冲区的进程隔离属性。在这种情况下，需要编译时设置 `MEM_SHARE=y`：
```bash
make MEM_SHARE=y
```

##### 特殊行为说明
- **vio sample第7个用例（user pic）**：第一次使能用户图片功能会显示用户图片，第二次使能会显示背景色。这是代码设计如此，属于正常现象。
  - 第一次调用时，传入的用户图片类型为 `VI_USER_PIC_FRAME`，显示用户图片
  - 第二次调用时，传入的类型为 `VI_USER_PIC_BGCOLOR`，用于配置背景色帧

##### 已知限制
- 需要正确配置传感器类型，否则可能导致VI初始化失败
- 某些模式需要特定的传感器支持（如WDR模式）
- 切换分辨率模式（mode 11）需要传感器支持分辨率切换

#### 快速上手指南

##### 最简单的运行示例
```bash
# 1. 编译sample
cd mpp/sample/vio
make

# 2. 运行最常用的模式（all mode route）
./sample_vio 0

# 3. 按Ctrl+C退出
```

##### 预期结果
- 程序启动后会显示初始化信息
- HDMI输出应该显示视频画面
- 控制台会打印各模块的初始化状态
- 按回车键或Ctrl+C可以优雅退出

##### 常见错误排查

| 错误现象 | 可能原因 | 解决方法 |
|----------|----------|----------|
| VI初始化失败 | 传感器类型配置错误 | 检查Makefile.param中的SENSOR0_TYPE配置 |
| 找不到设备文件 | HDMI未连接或配置错误 | 确认HDMI已连接，检查VO配置 |
| MMZ分配失败 | MMZ进程隔离未处理 | 使用 `make MEM_SHARE=y` 重新编译 |
| 编码流未保存 | 路径权限问题 | 检查当前目录的写权限 |

#### 参数定制方法

##### 修改传感器类型
编辑 `mpp/sample/Makefile.param` 文件：
```makefile
# 修改传感器类型为其他支持的传感器
SENSOR0_TYPE ?= HY_S0603_MIPI_8M_30FPS_12BIT
# 其他可选传感器类型（在Makefile.param中列出）
# HY_S0603_MIPI_8M_30FPS_10BIT_WDR2TO1
```

##### 修改分辨率和帧率
在代码中修改相关配置参数（以vio为例）：
```c
// 在sample_vio.c中修改g_venc_chn_param结构体
static sample_comm_venc_chn_param g_venc_chn_param = {
    .frame_rate = 30,        // 修改帧率（如改为25）
    .venc_size = {1920, 1080}, // 修改分辨率
    // ...
};
```

##### 修改编码参数
在代码中修改编码相关配置：
```c
static sample_comm_venc_chn_param g_venc_chn_param = {
    .type = HI_PT_H265,      // 编码类型：H264/H265/JPEG/MJPEG
    .gop = 30,               // GOP大小
    .rc_mode = SAMPLE_RC_CBR, // 码率控制模式
    // ...
};
```

##### 修改输出分辨率
修改VO配置：
```c
static sample_vo_cfg g_vo_cfg = {
    .intf_sync = HI_VO_OUT_1080P30,  // 修改输出时序（如改为3840x2160_30）
    .disp_rect = {0, 0, 1920, 1080}, // 修改显示区域
    .image_size = {1920, 1080},      // 修改图像尺寸
    // ...
};
```

---

### 2.2 VENC Sample - 视频编码示例

#### 主要功能
演示H.265/H.264/JPEG/MJPEG视频编码功能，支持多种编码特性和高级功能。

#### 支持的运行模式
- `(0)` normal: H.265e@1080P@30fps + h264e@D1@30fps
- `(1)` qpmap: H.265e@1080P@30fps + h264e@1080P@30fps
- `(2)` intra_refresh: H.265e@1080P@30fps(row) + h264e@1080P@30fps(column)
- `(3)` roi_bg_frame_rate: H.265e@1080P@30fps + H.264@1080P@30fps
- `(4)` debreath_effect: H.265e@1080P@30fps(enable) + H.265e@1080P@30fps(disable)
- `(5)` roi_set: Mjpege@1080P@30fps(user set roi info by API)
- `(6)` roimap: Mjpege@1080P@30fps(user customize every region)

#### 应用场景
- 视频录制和存储
- 实时视频压缩
- ROI（感兴趣区域）编码优化
- 去呼吸效应处理
- 码率控制和图像质量优化

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/venc
make
```

##### 运行命令
```bash
# 运行模式0（normal）
./sample_venc 0

# 运行模式0并保存HEIF文件
./sample_venc 0 1

# 运行模式1（qpmap）
./sample_venc 1
```

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_venc_usage() (参数校验)
  └─ sample_register_sig_handler() (注册信号处理)
  └─ sample_venc_case() (执行指定模式)
      └─ sample_venc_normal() / sample_venc_qpmap() / ... (根据模式选择)
          └─ sample_comm_sys_init() (系统初始化)
          └─ sample_comm_vi_create_vi() (创建VI设备)
          └─ sample_comm_vpss_create() (创建VPSS)
          └─ sample_comm_venc_create() (创建VENC)
          └─ sample_pause() (等待用户输入)
          └─ sample_comm_sys_exit() (清理资源)
```

##### 数据流向
```
VI -> VPSS -> VENC (编码输出文件)
```

#### 关键数据

##### 重要配置参数
| 参数名 | 默认值 | 说明 |
|--------|--------|------|
| BIG_STREAM_SIZE | PIC_3840X2160 | 大流分辨率 |
| SMALL_STREAM_SIZE | PIC_1080P | 小流分辨率 |
| frame_rate | 30 | 帧率 |
| gop | 30 | GOP大小 |

##### 编码类型
- `HI_PT_H265`: H.265/HEVC编码
- `HI_PT_H264`: H.264/AVC编码
- `HI_PT_JPEG`: JPEG编码
- `HI_PT_MJPEG`: Motion JPEG编码

#### 注意事项

##### HEIF文件保存
模式0支持保存HEIF文件，使用第二个参数控制：
- `./sample_venc 0 0`: 不保存HEIF文件
- `./sample_venc 0 1`: 保存HEIF文件

##### 编码特性说明
- **qpmap模式**: 支持QP映射，实现区域化质量控制
- **intra_refresh模式**: 支持行/列级别的帧内刷新，提高抗误码能力
- **roi_bg_frame_rate模式**: 支持ROI和背景帧率设置
- **debreath_effect模式**: 支持去呼吸效应，减少静态场景的闪烁
- **roi_set模式**: 支持通过API设置ROI信息
- **roimap模式**: 支持自定义每个区域的编码参数

#### 快速上手指南

##### 最简单的运行示例
```bash
cd mpp/sample/venc
make
./sample_venc 0
```

##### 预期结果
- 程序启动后会显示初始化信息
- 开始编码后会在当前目录生成编码流文件
- 按回车键或Ctrl+C可以优雅退出

#### 参数定制方法

##### 修改编码参数
```c
// 在sample_venc.c中修改相关配置
static sample_comm_venc_chn_param g_venc_chn_param = {
    .type = HI_PT_H265,      // 编码类型
    .frame_rate = 30,        // 帧率
    .gop = 30,               // GOP大小
    .rc_mode = SAMPLE_RC_CBR, // 码率控制模式
    // ...
};
```

##### 修改分辨率
```c
#define BIG_STREAM_SIZE     PIC_3840X2160  // 修改为其他分辨率
#define SMALL_STREAM_SIZE   PIC_1080P
```

---

### 2.3 VDEC Sample - 视频解码示例

#### 主要功能
演示H.265/H.264/JPEG/HEIC视频解码功能，支持播放和低延迟预览。

#### 支持的运行模式
- `(0)` VDEC(H265 PLAYBACK)-VPSS-VO: H.265文件播放
- `(1)` VDEC(H264 PLAYBACK)-VPSS-VO: H.264文件播放
- `(2)` VDEC(JPEG PLAYBACK)-VPSS-VO: JPEG文件播放
- `(3)` VDEC(H265 LOWDELAY PREVIEW)-VPSS-VO: H.265低延迟预览
- `(4)` VDEC(HEIC H265 PLAYBACK PREVIEW)-VPSS-VO: HEIC格式的H.265播放预览（需要编译选项支持）

#### 应用场景
- 视频文件播放
- 实时视频解码预览
- 低延迟视频传输显示
- HEIC格式文件显示

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/vdec
make
```

##### 运行命令
```bash
# 运行模式0（H265播放）
./sample_vdec 0

# 运行模式1（H264播放）
./sample_vdec 1

# 运行模式2（JPEG播放）
./sample_vdec 2

# 运行模式3（低延迟预览）
./sample_vdec 3
```

##### 测试数据
测试数据文件位于 `vdec/source_file/` 目录：
- H.265编码流文件
- H.264编码流文件
- JPEG图片文件
- HEIC文件（如果支持）

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_vdec_usage() (参数校验)
  └─ sample_register_sig_handler() (注册信号处理)
  └─ sample_vdec_case() (执行指定模式)
      └─ sample_vdec_h265_playback() / sample_vdec_h264_playback() / ...
          └─ sample_comm_sys_init() (系统初始化)
          └─ sample_comm_vdec_create() (创建VDEC)
          └─ sample_comm_vpss_create() (创建VPSS)
          └─ sample_comm_vo_create() (创建VO)
          └─ 发送编码流数据到VDEC
          └─ sample_pause() (等待用户输入)
          └─ sample_comm_sys_exit() (清理资源)
```

##### 数据流向
```
文件 -> VDEC -> VPSS -> VO (显示输出)
```

#### 关键数据

##### 重要配置参数
| 参数名 | 默认值 | 说明 |
|--------|--------|------|
| UHD_STREAN_WIDTH | 3840 | 解码宽度 |
| UHD_STREAM_HEIGHT | 2160 | 解码高度 |
| REF_NUM | 参考帧数 | 解码参考帧数量 |
| DISPLAY_NUM | 显示帧数 | 显示帧缓冲数量 |

##### 解码模式
- `HI_VDEC_SEND_MODE_FRAME`: 帧模式发送
- `HI_VDEC_SEND_MODE_COMPAT`: 兼容模式（低延迟）

#### 注意事项

##### 测试数据准备
确保 `vdec/source_file/` 目录下有对应的测试数据文件：
- H.265流文件用于模式0和3
- H.264流文件用于模式1
- JPEG文件用于模式2

##### 低延迟模式
模式3使用低延迟模式，需要配置：
```c
sample_comm_vdec_get_lowdelay_en() 返回HI_TRUE
```

##### HEIC支持
模式4需要编译选项 `SAMPLE_HEIC_SUPPORT` 支持。

#### 快速上手指南

##### 最简单的运行示例
```bash
cd mpp/sample/vdec
make
./sample_vdec 0
```

##### 预期结果
- 程序启动后会显示初始化信息
- HDMI输出显示解码后的视频画面
- 按回车键或Ctrl+C可以优雅退出

#### 参数定制方法

##### 修改解码分辨率
```c
// 在sample_vdec.c中修改相关配置
static hi_u32 sample_vdec_get_chn_width() {
    return 1920;  // 修改宽度
}

static hi_u32 sample_vdec_get_chn_height() {
    return 1080;  // 修改高度
}
```

##### 修改参考帧数
```c
#define REF_NUM 3  // 修改参考帧数量
```

---

### 2.4 VO Sample - 视频输出示例

#### 主要功能
演示视频输出功能，支持多种输出接口和显示模式。

#### 应用场景
- HDMI视频输出
- 显示设备适配
- 多屏显示

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/vo
make
```

##### 运行命令
```bash
./sample_vo
```

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_comm_sys_init() (系统初始化)
  └─ sample_comm_vo_create() (创建VO)
  └─ 发送测试数据到VO
  └─ sample_pause() (等待用户输入)
  └─ sample_comm_sys_exit() (清理资源)
```

#### 关键数据

##### VO设备类型
- `HI_VO_INTF_HDMI`: HDMI接口
- `HI_VO_INTF_BT656`: BT656接口
- `HI_VO_INTF_BT1120`: BT1120接口

##### 输出时序
- `HI_VO_OUT_1080P30`: 1080P@30fps
- `HI_VO_OUT_3840x2160_30`: 4K@30fps
- 更多时序请参考SDK文档

#### 参数定制方法

##### 修改输出接口
```c
static sample_vo_cfg g_vo_cfg = {
    .vo_intf_type = HI_VO_INTF_HDMI,  // 修改为其他接口
    // ...
};
```

##### 修改输出分辨率
```c
static sample_vo_cfg g_vo_cfg = {
    .intf_sync = HI_VO_OUT_1080P30,  // 修改输出时序
    // ...
};
```



---

### 2.5 VGS Sample - 视频图形子系统示例

#### 主要功能
演示VGS（Video Graphics Subsystem）视频图形处理功能，支持叠加、缩放、旋转等。

#### 应用场景
- OSD叠加
- 图形叠加
- 视频变换

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/vgs
make
```

##### 运行命令
```bash
./sample_vgs
```

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_comm_sys_init() (系统初始化)
  └─ HI_MPI_VGS_Create() (创建VGS任务)
  └─ HI_MPI_VGS_BeginJob() (开始VGS任务)
  └─ HI_MPI_VGS_AddRegion() (添加处理区域)
  └─ HI_MPI_VGS_EndJob() (结束VGS任务)
  └─ sample_pause() (等待用户输入)
  └─ sample_comm_sys_exit() (清理资源)
```

#### 关键数据

##### VGS任务类型
- 缩放
- 裁剪
- 覆盖/叠加
- 镜像
- 旋转

#### 参数定制方法

##### 修改VGS处理参数
根据具体需求修改VGS任务的参数，包括缩放比例、裁剪区域、叠加内容等。

---

## 3. 音频处理模块

### 3.1 Audio Sample - 音频处理示例

#### 主要功能
演示音频采集、编解码、播放等功能。

#### 支持的运行模式
- `(0)` AI to AO loop: 音频采集到音频输出循环
- `(1)` AI->AENC->save: 音频采集到编码并保存
- `(2)` file->ADEC->AO: 文件解码到音频输出
- `(3)` AI->AO VQE: 音频采集通过VQE处理到音频输出
- `(4)` AI->AO(HDMI): 音频采集到HDMI音频输出
- `(5)` AI->AO(sys_chn): 音频采集到系统通道输出
- `(6)` resample test: 重采样测试

#### 应用场景
- 音频录制和播放
- 音频编解码
- 音频质量增强（VQE）
- HDMI音频输出
- 音频重采样

#### 编译运行步骤

##### 编译依赖
- 确保音频编解码器类型已正确配置（ES8388或INNER）
- 确保音频模块库类型已配置

##### 编译命令
```bash
cd mpp/sample/audio
make
```

##### 运行命令
```bash
# 运行模式0（AI->AO loop）
./sample_audio 0

# 运行模式1（AI->AENC->save）
./sample_audio 1

# 运行模式3（AI->AO VQE）
./sample_audio 3
```

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_audio_usage() (参数校验)
  └─ sample_audio_handle_sig() (注册信号处理)
  └─ main_inner(index) (执行指定模式)
      └─ sample_audio_ai_ao() / sample_audio_ai_aenc() / ...
          └─ sample_comm_sys_init() (系统初始化)
          └─ HI_MPI_AI_Create() (创建AI设备)
          └─ HI_MPI_AO_Create() (创建AO设备)
          └─ HI_MPI_AENC_Create() (创建AENC，如果需要)
          └─ HI_MPI_ADEC_Create() (创建ADEC，如果需要)
          └─ sample_pause() (等待用户输入)
          └─ sample_comm_sys_exit() (清理资源)
```

##### 数据流向
```
AI -> AENC -> 文件 (录音模式)
文件 -> ADEC -> AO (播放模式)
AI -> AO (直通模式)
AI -> VQE -> AO (VQE处理模式)
```

#### 关键数据

##### 重要配置参数
| 参数名 | 默认值 | 说明 |
|--------|--------|------|
| ACODEC_TYPE | ACODEC_TYPE_INNER | 音频编解码器类型 |
| 音频采样率 | 48000 | 采样率（Hz） |
| 音频位宽 | 16 | 位宽（bit） |
| 音频通道数 | 2 | 通道数（立体声） |

##### 音频编解码器类型
- `ACODEC_TYPE_ES8388`: ES8388外部编解码器
- `ACODEC_TYPE_INNER`: 内部编解码器

##### 音频编码格式
- `HI_AUDIO_ADPCM_TYPE`: ADPCM编码
- `HI_AUDIO_PCM_TYPE`: PCM编码
- `HI_AUDIO_G711A_TYPE`: G.711A编码
- `HI_AUDIO_G711U_TYPE`: G.711U编码
- `HI_AUDIO_G726_TYPE`: G.726编码
- `HI_AUDIO_LPCM_TYPE`: LPCM编码

#### 注意事项

##### 音频编解码器配置
在Makefile.param中配置音频编解码器类型：
```makefile
# 外部编解码器
# ACODEC_TYPE ?= ACODEC_TYPE_ES8388
# 内部编解码器（默认）
ACODEC_TYPE ?= ACODEC_TYPE_INNER
```

##### HDMI音频输出
模式4使用HDMI输出音频，需要配置：
```makefile
INNER_HDMI ?= y
```

##### VQE功能
模式3使用VQE（Voice Quality Enhancement）功能，包括：
- 回声消除（AEC）
- 噪声抑制（NS）
- 自动增益控制（AGC）

#### 快速上手指南

##### 最简单的运行示例
```bash
cd mpp/sample/audio
make
./sample_audio 0
```

##### 预期结果
- 程序启动后会显示初始化信息
- 麦克风采集的音频会实时输出到扬声器
- 按回车键或Ctrl+C可以优雅退出

##### 常见错误排查

| 错误现象 | 可能原因 | 解决方法 |
|----------|----------|----------|
| 音频设备打开失败 | 音频编解码器配置错误 | 检查Makefile.param中的ACODEC_TYPE配置 |
| 没有音频输出 | 扬声器未连接或配置错误 | 确认扬声器已连接，检查AO配置 |
| 音频质量差 | VQE参数配置不当 | 调整VQE参数或使用直通模式 |

#### 参数定制方法

##### 修改音频编解码器类型
编辑 `mpp/sample/Makefile.param` 文件：
```makefile
# 外部编解码器
ACODEC_TYPE ?= ACODEC_TYPE_ES8388
# 内部编解码器
# ACODEC_TYPE ?= ACODEC_TYPE_INNER
```

##### 修改音频采样参数
```c
// 在sample_audio.c中修改相关配置
static hi_audio_dev ai_dev = 0;  // AI设备号
static hi_audio_dev ao_dev = 0;  // AO设备号
static hi_audio_chn ai_chn = 0;  // AI通道号
static hi_audio_chn ao_chn = 0;  // AO通道号

// 修改采样率等参数
```

##### 修改音频编码格式
```c
// 在创建AENC时修改编码格式
hi_audio_encoder_attr aenc_attr = {
    .type = HI_AUDIO_ADPCM_TYPE,  // 修改编码类型
    .sample_rate = 48000,         // 采样率
    .bit_width = HI_AUDIO_BIT_WIDTH_16,  // 位宽
    .sound_mode = HI_AUDIO_SOUND_MODE_STEREO,  // 声音模式
};
```

---

## 4. 智能处理模块

### 4.1 AIISP Sample - AI ISP示例

#### 主要功能
演示AI ISP（Artificial Intelligence Image Signal Processor）功能，包括AI去雾等。

#### 支持的运行模式
- `(0)` aidehaze normal: AI去雾-正常模式
- `(1)` aidehaze distant: AI去雾-远景模式
- `(2)` aidehaze distant_enlighten: AI去雾-远景增强模式

#### 应用场景
- 图像去雾
- 图像质量增强
- 恶劣环境图像处理

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/aiisp
make
```

##### 运行命令
```bash
# 运行模式0（aidehaze normal）
./sample_aiisp 0

# 运行模式1（aidehaze distant）
./sample_aiisp 1

# 运行模式2（aidehaze distant_enlighten）
./sample_aiisp 2
```

##### 测试数据
- RAW数据文件：`aiisp/raw/`
- 模型文件：`aiisp/model/` (.om文件)

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_aiisp_usage() (参数校验)
  └─ sample_aiisp_case() (执行指定模式)
      └─ sample_aiisp_aidehaze_normal() / sample_aiisp_aidehaze_distant() / ...
          └─ sample_comm_sys_init() (系统初始化)
          └─ 加载AI模型
          └─ 处理RAW数据
          └─ 输出处理结果
          └─ sample_pause() (等待用户输入)
          └─ sample_comm_sys_exit() (清理资源)
```

#### 关键数据

##### AI去雾模式
- **normal**: 适用于一般雾气场景
- **distant**: 适用于远距离雾气场景
- **distant_enlighten**: 适用于远距离雾气场景并进行增强

##### 模型文件格式
- `.om`: 模型文件格式

#### 注意事项

##### 模型文件准备
确保 `aiisp/model/` 目录下有对应的模型文件。

##### RAW数据准备
确保 `aiisp/raw/` 目录下有RAW格式的测试数据。

#### 参数定制方法

##### 修改AI去雾参数
根据具体场景选择不同的去雾模式，或调整模型参数。

---

### 4.2 SVP/IVE Sample - 智能视觉处理示例

#### 主要功能
演示IVE（Intelligent Vision Engine）智能视觉处理算法，包括边缘检测、运动检测、目标跟踪等。

#### 支持的运行模式
- `(0)` Canny: Canny边缘检测算法（FILE->IVE->FILE）
  - `complete=0`: 部分Canny
  - `complete=1`: 完整Canny
- `(1)` Gmm2: GMM背景建模算法（FILE->IVE->FILE）
- `(2)` MemoryTest: 内存测试算法（FILE->IVE->FILE）
- `(3)` Sobel: Sobel边缘检测算法（FILE->IVE->FILE）
- `(4)` St Lk: Lucas-Kanade光流算法（FILE->IVE）
- `(5)` PerspTrans: 透视变换算法（FILE->IVE->FILE）
- `(6)` Occlusion detected: 遮挡检测（VI->VPSS->IVE->VO_HDMI）
- `(7)` Motion detected: 运动检测（VI->VPSS->IVE->VGS->VO_HDMI）
- `(8)` Kcf track: KCF目标跟踪（FILE->VDEC->VPSS->SVP_NPU->IVE->VGS->VO_HDMI）

#### 应用场景
- 边缘检测
- 背景建模
- 运动检测
- 目标跟踪
- 光流计算
- 透视变换

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/svp/ive
make
```

##### 运行命令
```bash
# 运行Canny边缘检测（部分）
./sample_ive 0 0

# 运行Canny边缘检测（完整）
./sample_ive 0 1

# 运行GMM背景建模
./sample_ive 1

# 运行Sobel边缘检测
./sample_ive 3

# 运行运动检测
./sample_ive 7

# 运行KCF目标跟踪
./sample_ive 8
```

##### 测试数据
测试数据文件位于 `svp/ive/data/` 目录：
- 输入数据文件
- 输出数据文件

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_ive_usage() (参数校验)
  └─ sample_ive_case() (执行指定模式)
      └─ sample_ive_canny() / sample_ive_gmm2() / ...
          └─ 加载输入数据
          └─ 创建IVE任务
          └─ 执行IVE算法
          └─ 保存输出结果
          └─ sample_pause() (等待用户输入)
          └─ 清理资源
```

##### 数据流向（以运动检测为例）
```
VI -> VPSS -> IVE -> VGS -> VO_HDMI
```

#### 关键数据

##### IVE算法说明
- **Canny**: 经典边缘检测算法，检测图像中的边缘
- **GMM2**: 高斯混合模型背景建模，用于前景检测
- **MemoryTest**: 内存测试算法，验证IVE内存操作
- **Sobel**: Sobel边缘检测算法，基于梯度的边缘检测
- **St Lk**: Lucas-Kanade稀疏光流算法，跟踪特征点运动
- **PerspTrans**: 透视变换，用于图像几何变换
- **Occlusion detected**: 遮挡检测，检测场景中的遮挡物
- **Motion detected**: 运动检测，检测场景中的运动物体
- **Kcf track**: KCF目标跟踪，跟踪指定目标

##### 算法数据流向类型
- **FILE->IVE->FILE**: 从文件读取数据，IVE处理，结果保存到文件
- **FILE->IVE**: 从文件读取数据，IVE处理，无输出文件
- **VI->VPSS->IVE->VO_HDMI**: 实时视频流处理，结果显示在HDMI
- **VI->VPSS->IVE->VGS->VO_HDMI**: 实时视频流处理，经过VGS后显示在HDMI
- **FILE->VDEC->VPSS->SVP_NPU->IVE->VGS->VO_HDMI**: 文件解码后，经过NPU、IVE、VGS处理后显示

#### 注意事项

##### 测试数据准备
确保 `svp/ive/data/` 目录下有对应的测试数据文件。

##### 实时模式注意事项
模式6、7、8使用实时视频流，需要：
- VI设备正常工作
- HDMI显示设备连接

##### Canny算法参数
Canny算法支持两种模式：
- `complete=0`: 部分Canny，只执行部分处理步骤
- `complete=1`: 完整Canny，执行完整处理流程

#### 快速上手指南

##### 最简单的运行示例
```bash
cd mpp/sample/svp/ive
make
./sample_ive 1  # 运行GMM背景建模
```

##### 预期结果
- 程序启动后会显示初始化信息
- IVE算法处理输入数据
- 结果保存到输出文件或显示在HDMI
- 按回车键或Ctrl+C可以优雅退出

#### 参数定制方法

##### 修改算法参数
各个IVE算法都有相应的参数结构体，根据需要修改：
```c
// 例如：Canny算法参数
hi_ive_candle_ctrl candle_ctrl = {
    // 修改Canny算法参数
};

// 例如：GMM算法参数
hi_ive_gmm_ctrl gmm_ctrl = {
    // 修改GMM算法参数
};
```

---

## 5. 图像处理模块

### 5.1 Fisheye Sample - 鱼眼镜头示例

#### 主要功能
演示鱼眼镜头校正功能，包括鱼眼展开和畸变校正。

#### 应用场景
- 鱼眼摄像头
- 全景监控
- 360度视角

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/fisheye
make
```

##### 运行命令
```bash
./sample_fisheye
```

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_comm_sys_init() (系统初始化)
  └─ 创建鱼眼校正任务
  └─ 配置鱼眼校正参数
  └─ 处理视频流
  └─ sample_pause() (等待用户输入)
  └─ sample_comm_sys_exit() (清理资源)
```

#### 关键数据

##### 鱼眼校正类型
- 等距投影
- 等立体角投影
- 正射投影
- 立体投影

#### 参数定制方法

##### 修改鱼眼校正参数
根据鱼眼镜头的参数调整校正参数，包括：
- 校正中心点
- 校正半径
- 校正映射模式

---

## 6. 功能特性模块

### 6.1 Region Sample - 区域管理示例

#### 主要功能
演示Region（区域管理）功能，支持多种区域类型和操作。

#### 应用场景
- 遮挡区域设置
- ROI区域设置
- 运动区域检测

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/region
make
```

##### 运行命令
```bash
./sample_region
```

##### 测试数据
测试资源文件位于 `region/res/` 目录：
- BMP图片文件
- H265编码流文件

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_region_usage() (参数校验)
  └─ sample_region_case() (执行指定功能)
      └─ sample_comm_sys_init() (系统初始化)
      └─ 创建和管理Region
      └─ sample_pause() (等待用户输入)
      └─ sample_comm_sys_exit() (清理资源)
```

#### 关键数据

##### Region类型
- 遮挡区域
- ROI区域
- 运动区域

#### 参数定制方法

##### 修改Region参数
根据需求修改Region的参数，包括区域位置、大小、属性等。

---

### 6.2 Cipher Sample - 加密示例

#### 主要功能
演示Cipher加密功能，支持视频流加密和解密。

#### 应用场景
- 视频加密传输
- 数据安全保护
- 版权保护

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/cipher
make
```

##### 运行命令
```bash
./sample_cipher
```

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_cipher_usage() (参数校验)
  └─ sample_cipher_case() (执行指定功能)
      └─ sample_comm_sys_init() (系统初始化)
      └─ 配置加密参数
      └─ 执行加密/解密操作
      └─ sample_pause() (等待用户输入)
      └─ sample_comm_sys_exit() (清理资源)
```

#### 关键数据

##### 加密算法
- AES
- DES
- 其他支持的加密算法

#### 参数定制方法

##### 修改加密参数
根据安全需求修改加密算法、密钥等参数。

---

### 6.3 HDMI Sample - HDMI输出示例

#### 主要功能
演示HDMI视频输出功能。

#### 应用场景
- HDMI显示
- 外接显示器
- 视频展示

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/hdmi
make
```

##### 运行命令
```bash
./sample_hdmi
```

##### 测试数据
测试数据文件位于 `hdmi/source_file/` 目录

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_comm_sys_init() (系统初始化)
  └─ 配置HDMI输出参数
  └─ 输出视频到HDMI
  └─ sample_pause() (等待用户输入)
  └─ sample_comm_sys_exit() (清理资源)
```

#### 关键数据

##### HDMI输出时序
- 1080P@30fps
- 1080P@60fps
- 4K@30fps
- 更多时序请参考SDK文档

#### 参数定制方法

##### 修改HDMI输出参数
```c
// 修改HDMI输出时序
HI_VO_OUT_1080P30  // 1080P@30fps
HI_VO_OUT_3840x2160_30  // 4K@30fps
```

---

### 6.4 HNR Sample - 降噪示例

#### 主要功能
演示HNR（High Noise Reduction）降噪功能。

#### 应用场景
- 图像降噪
- 低光照场景
- 图像质量提升

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/hnr
make
```

##### 运行命令
```bash
./sample_hnr
```

#### 参数定制方法

##### 修改降噪参数
根据噪声强度调整降噪参数。

---

### 6.5 Composite Sample - 复合功能示例

#### 主要功能
演示多种功能的组合使用。

#### 应用场景
- 多功能组合
- 复杂场景应用

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/composite
make
```

##### 运行命令
```bash
./sample_composite
```

---

### 6.6 Snap Sample - 抓拍示例

#### 主要功能
演示视频抓拍功能，支持从视频流中抓取图片。

#### 应用场景
- 视频抓拍
- 图片保存
- 关键帧提取

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/snap
make
```

##### 运行命令
```bash
./sample_snap
```

#### 参数定制方法

##### 修改抓拍参数
根据需求修改抓拍的分辨率、格式、保存路径等。

---

## 7. 高级功能模块

---

### 7.1 Traffic Capture - 交通抓拍示例

#### 主要功能
演示交通抓拍功能，支持车辆检测、车牌识别等。

#### 应用场景
- 交通监控
- 车辆抓拍
- 车牌识别

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/traffic_capture
make
```

##### 运行命令
```bash
./sample_traffic_capture
```

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_comm_sys_init() (系统初始化)
  └─ 配置交通抓拍参数
  └─ 执行车辆检测和抓拍
  └─ sample_pause() (等待用户输入)
  └─ sample_comm_sys_exit() (清理资源)
```

#### 参数定制方法

##### 修改交通抓拍参数
根据实际场景调整车辆检测、抓拍等参数。

---

### 7.2 UVC App - UVC应用示例

#### 主要功能
演示UVC（USB Video Class）应用功能，支持USB摄像头。

#### 应用场景
- USB摄像头
- USB视频传输
- 即插即用视频设备

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/uvc_app
make
```

##### 运行命令
```bash
./sample_uvc_app
```

#### 注意事项

##### UVC设备支持
确保USB摄像头设备已正确连接和配置。

---

### 7.3 Host UVC - Host UVC示例

#### 主要功能
演示Host UVC功能，作为USB主机控制UVC设备。

#### 应用场景
- USB主机
- UVC设备控制
- USB视频采集

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/host_uvc
make
```

##### 运行命令
```bash
./sample_host_uvc
```

#### 注意事项

##### USB主机配置
确保USB主机功能已正确配置。

---

## 8. 其他模块

---

### 8.1 GFBG Sample - GF/BG示例

#### 主要功能
演示GF（Graphic Frame）和BG（Background Frame）功能。

#### 应用场景
- 图形层叠加
- 背景设置
- 图形显示

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/gfbg
make
```

##### 运行命令
```bash
./sample_gfbg
```

#### 参数定制方法

##### 修改GF/BG参数
根据显示需求调整图形层和背景层参数。

---

### 8.2 TDE Sample - 二维引擎示例

#### 主要功能
演示TDE（2D Engine）二维图形引擎功能。

#### 应用场景
- 二维图形处理
- 图像变换
- 图形加速

#### 编译运行步骤

##### 编译命令
```bash
cd mpp/sample/tde
make
```

##### 运行命令
```bash
./sample_tde
```

#### 调用流程

##### 代码执行流程
```
main()
  └─ sample_comm_sys_init() (系统初始化)
  └─ HI_MPI_TDE_Open() (打开TDE设备)
  └─ HI_MPI_TDE_BeginJob() (开始TDE任务)
  └─ 配置TDE操作（缩放、旋转、拷贝等）
  └─ HI_MPI_TDE_EndJob() (结束TDE任务)
  └─ HI_MPI_TDE_Close() (关闭TDE设备)
  └─ sample_pause() (等待用户输入)
  └─ sample_comm_sys_exit() (清理资源)
```

#### 关键数据

##### TDE操作类型
- 拷贝
- 缩放
- 旋转
- 镜像
- 填充
- 混合

#### 参数定制方法

##### 修改TDE参数
根据图形处理需求调整TDE的缩放、旋转、混合等参数。

---

## 附录

### A. 常用宏定义

#### 分辨率宏
```c
#define PIC_3840X2160    {3840, 2160}  // 4K分辨率
#define PIC_1920X1080    {1920, 1080}  // 1080P分辨率
#define PIC_1280X720     {1280, 720}   // 720P分辨率
#define PIC_640X480      {640, 480}    // VGA分辨率
```

#### 像素格式宏
```c
#define HI_PIXEL_FORMAT_YVU_SEMIPLANAR_420  // YUV420半平面格式
#define HI_PIXEL_FORMAT_YUV_SEMIPLANAR_420   // YUV420半平面格式（交替）
#define HI_PIXEL_FORMAT_YUV_400             // 灰度格式
#define HI_PIXEL_FORMAT_RGB_888             // RGB888格式
```

#### VO设备号
```c
#define SAMPLE_VO_DEV_UHD    0  // UHD设备号
#define SAMPLE_VO_DEV_HD     1  // HD设备号
```

### B. 编译选项

#### 传感器类型
在 `mpp/sample/Makefile.param` 中设置：
```makefile
SENSOR0_TYPE ?= HY_S0603_MIPI_8M_30FPS_12BIT
```

#### 内存共享
```bash
make MEM_SHARE=y
```

#### 音频编解码器
```makefile
ACODEC_TYPE ?= ACODEC_TYPE_INNER
```

### C. 常见问题

#### Q1: 编译失败，提示找不到头文件
**A:** 确保SDK环境已正确配置，检查 `Makefile.param` 中的路径设置。

#### Q2: 运行时提示MMZ分配失败
**A:** 使用 `make MEM_SHARE=y` 重新编译。

#### Q3: HDMI没有输出
**A:** 检查HDMI是否连接，确认VO配置正确。

#### Q4: VI初始化失败
**A:** 检查传感器类型配置是否正确，确认传感器已连接。

#### Q5: 音频没有输出
**A:** 检查音频编解码器配置，确认音频设备正常。
