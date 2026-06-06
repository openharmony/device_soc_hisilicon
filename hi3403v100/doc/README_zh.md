# Hi3403V100 文档索引

本目录包含 Hi3403V100 芯片配套 hispark_aifly 开发板的完整文档，涵盖快速开始、小型系统移植案例、媒体处理、智能视觉、AI 模型、外设接口、安全等模块的开发参考、API 参考和开发指南。

- [文档介绍](#文档介绍)

## 文档介绍

### 快速开始

| 序号 | 文档 | 说明 |
|------|------|------|
| 1 | [HiSpark_AiFly开发板](https://gitcode.com/openharmony-sig/device_board_hisilicon_hispark_aifly/blob/master/hispark_aifly/README_zh.md) | 开发板规格、开发环境搭建、源码获取、构建编译、烧录方法（ToolPlatform工具和命令行两种方式）、快速体验（桌面展示、相机应用、媒体播放） |

### 小型系统移植案例

| 序号 | 文档 | 说明 |
|------|------|------|
| 2 | [OpenHarmony 小型系统集成 Hi3403V100 移植案例](./porting-hispark_aifly-on_small-demo.md) | 介绍 OpenHarmony 小型系统在 Hi3403V100 芯片上的完整移植适配过程，包括解决方案集成、产品配置、内核移植、编译配置、XTS 认证、HUKS/图形/媒体增强特性等 |
| 3 | [U-Boot 技术服务文档](./porting-smallchip-uboot.md) | U-Boot 引导加载程序相关技术服务文档 |

### 媒体处理

| 序号 | 文档 | 说明 |
|------|------|------|
| 4 | [MPP 媒体处理软件 V5.0 开发参考](./MPP%20媒体处理软件%20V5.0%20开发参考/00%20前言.md) | MPP 媒体处理软件开发包完整参考，涵盖 VI/VO/VPSS/VENC/VDEC/AUDIO/RGN/SYS 各模块 API、数据结构、错误码 |
| 5 | [MPP 媒体处理软件 V5.0 FAQ](./MPP%20媒体处理软件%20V5.0%20FAQ/MPP%20媒体处理软件%20V5.0%20FAQ.md) | MPP 开发常见问题解答（编译、运行、功能问题），帮助快速定位和解决问题 |
| 6 | [ISP 开发参考](./ISP%20开发参考/ISP%20开发参考（1--2）.md) | ISP 图像信号处理器各模块功能、参数配置、调试方法，包括 AE/AWB/AF 3A 算法、降噪、锐化、宽动态等 |
| 7 | [HNR 开发参考](./HNR%20开发参考/HNR%20开发参考.md) | HNR 高灵敏度降噪算法原理、接口说明、参数配置，用于低照度环境下的图像降噪处理 |
| 8 | [GFBG 开发指南](./GFBG%20开发指南/GFBG%20开发指南.md) | GFBG 图形帧缓冲组模块功能特性、使用场景、配置方法和开发流程 |
| 9 | [GFBG API 参考](./GFBG%20API%20参考/GFBG%20API%20参考.md) | GFBG 模块 API 接口和数据类型参考，含函数说明、参数定义、返回值和使用示例 |
| 10 | [TDE API 参考](./TDE%20API参考/TDE%20API参考.md) | TDE 二维图形加速引擎 API，包括位图操作、图形填充、缩放、旋转、格式转换等 |

### 智能视觉处理

| 序号 | 文档 | 说明 |
|------|------|------|
| 11 | [SVP2.0 开发指南](./SVP2.0%20开发指南/SVP2.0%20开发指南.md) | SVP 2.0 智能视觉平台硬件架构、计算单元、工具链、模型转换和部署流程 |
| 12 | [SVP2.0 API 参考](./SVP2.0%20API%20参考/SVP2.0%20API%20参考.md) | SVP 2.0 平台编程接口，包括 NNIE 神经网络推理引擎、IVE 智能视频引擎等 API |
| 13 | [SVP ACL 开发指南](./SVP%20ACL%20开发指南/SVP%20ACL%20开发指南.md) | 基于 SVP ACL 的 C 语言 API 库开发图像分析工具应用，涵盖初始化、运行管理、模型加载/执行、内存管理 |
| 14 | [IVE API 参考](./IVE%20API%20参考/IVE%20API%20参考（1--2）.md) | IVE 智能视频引擎硬件加速器 API，包括图像处理、特征提取、目标检测等智能分析功能接口 |
| 15 | [IVS API 参考](./IVS%20API参考/IVS%20API参考.md) | IVS 智能视频监控 API，包括运动检测、遮挡检测、周界防范等智能监控算法接口 |

### AI 模型推理案例

| 序号 | 文档 | 说明 |
|------|------|------|
| 15 | [AI模型推理概述](./porting-smallchip-ai-overview.md) | Hi3403V100 CPU/NPU 推理方案介绍，包括 llamacpp、MindSporeLite 框架和 ModelZoo 模型库 |
| 16 | [llamacpp 开发指南](./porting-smallchip-ai-CPU-llamacpp.md) | llamacpp 大语言模型推理框架在 OpenHarmony 系统上的编译和部署指南 |
| 17 | [MindSporeLite 开发指南](./porting-smallchip-ai-CPU-MindSporeLite.md) | MindSporeLite 轻量级 AI 推理框架开发指导 |
| 18 | [ModelZoo NPU 推理案例](./porting-smallchip-ai-NPU-ModelZoo.md) | ModelZoo NPU 推理案例开发指导，包含模型转换和部署流程 |

### 外设与接口

| 序号 | 文档 | 说明 |
|------|------|------|
| 19 | [HDMI 开发参考](./HDMI%20开发参考/HDMI%20开发参考.md) | HDMI 高清多媒体接口配置、使用方法、支持的分辨率和音频格式 |
| 20 | [MIPI 使用指南](./MIPI%20使用指南/MIPI%20使用指南.md) | MIPI CSI-2（摄像头接口）和 MIPI DSI（显示接口）的配置和使用方法 |
| 21 | [DHCP 使用指南](./porting-smallchip-dhcp.md) | DHCP 使用参考，含 DHCP 配置和使用说明 |
| 22 | [HDC 开发指南](./porting-smallchip-hdc.md) | HDC（OpenHarmony Device Connector）开发参考 |

### 安全

| 序号 | 文档 | 说明 |
|------|------|------|
| 23 | [KLAD API 参考](./KLAD%20API%20参考/KLAD%20API%20参考.md) | KLAD 密钥加载与分发 API，用于安全启动和密钥安全存储 |