# ws63v100<a name="ZH-CN_TOPIC_0000001130176841"></a>

-   [简介](#section11660541593)
-   [特点](#section12212842173518)
-   [目录](#section1464106163817)
-   [许可协议](#section1478215290)
-   [开发环境搭建](#section1371113476307)

## 简介<a name="section11660541593"></a>

**WS63V100**系列是2.4GHz Wi-Fi 6 星闪多模IoT SoC芯片，其中增强款芯片**WS63V100**支持2.4GHz的雷达人体活动检测功能，适用于大小家电、电工照明及对人体出没检测有需求的常电类物联网智能场景。集成IEEE 802.11 b/g/n/ax基带和RF电路，包括功率放大器PA、低噪声放大器LNA、RF balun、天线开关以及电源管理模块等，支持20MHz频宽，提供最大114.7Mbps物理层速率，支持更大的发射功率和更远的覆盖距离，支持星闪SLE 1MHz/2MHz/4MHz频宽、SLE1.0协议、支持SLE网关功能，最大空口速率12Mbps。

基于软总线让更多业务透明接入星闪如下：

**图 1**  基于软总线让更多业务透明接入星闪图<a name="fig4460722185514"></a>  


![image-20250305154026679](figures/image-20250305154026679.png)

## 特点<a name="section12212842173518"></a>

**丰富的通信接口**

-   支持多种通信协议和接口，如星闪、Wi-Fi、BLE等，设备之间的通信更加灵活和可靠，可以确保数据在不同网络环境中的传输效率和安全性

**兼容性和开放性**

-   支持开放操作系统0penHarmony，提供开放、高效、安全的系统开发运行环境，丰富的低功耗、小内存、高稳定性、高实时性机制，提供灵活的协议支撑、扩展能力和多层级开发接口

**模块化设计**

-   采用模块化设计，允许开发者根据具体项目需求选择适合的硬件和软件模块。使得开发过程更加灵活，能够快速构建原型和验证概念

**易于上手性**

-   附带详细的文档、示例代码和教程，为初学者和开发者提供了友好的入门体验，可以帮助开发者快速了解套件的功能和使用方法，降低学习成本

**可扩展性**

- 硬件和软件模块支持多种扩展接口，如GPI0、SPI、I2C等，允许开发者根据需要添加更多功能或连接其他设备，使得开发套件能够满足不同项目的需求

**安全性**

- 开发套件加强了安全性因素，如数据加密、访问控制、固件升级等，以确保设备的安全运行和数据的安全传输

## 目录<a name="section1464106163817"></a>

WS63的SDK软件包根目录结构所在位置device\\soc\\hisilicon\\ws63v100\\sdk，如下图所示：

```
device/soc/hisilicon/ws63v100/sdk
├── application         # 应用层代码（其中包含demo程序为参考示例）。
├── bootloader          # boot(Flashboot/SSB)代码。
├── build               # SDK构建所需的脚本、配置文件。
├── drivers             # 驱动代码。
├── include             # API 头文件存放目录。
├── interim_binary      # 库存放目录。
├── kernel              # 内核头文件及库。
├── libs_url            # 库文件。
├── middleware          # 中间件代码。
├── open_source         # 开源代码。
├── protocol            # WiFi、BT、Radar 等组件代码。
├── test                # testsuite 代码。
├── tools               # 包含编译工具链（包括linux和windows）、镜像打包脚本、NV制作工具和签名脚本等
├── BUILD.gn            # 支持make编译，使用“make”或“make all”启动编译。
├── build.py            # 编译入口脚本。
├── CMakeLists.txt      # Cmake 工程顶层“CMakeLists.txt”文件
├── config.in           # Kconfig 配置文件。
├── hm_build.sh         # 适配OpenHarmony构建脚本。
└── LICENSE             # SDK开源license声明。
```

## 许可协议<a name="section1478215290"></a>

-   ws63v100自研代码使用基于Apache License Version 2.0许可的hisi版权声明。许可信息和版权信息通常可以在代码开头看到：

```
  / * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2022. All rights reserved. * / 
```

-   ws63V100使用的第三方代码遵循软件版本自带的开源许可声明。
-   ws63V100可能会使用一些开源软件组件。如果这些开源软件组件所适用的许可与本协议内容冲突，则以该开源软件组件的许可为准。

## 环境搭建<a name="section1371113476307"></a>

参考[WS63环境搭建指南](WS63环境搭建.md)

