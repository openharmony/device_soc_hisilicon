# Hi3518ev300介绍<a name="ZH-CN_TOPIC_0000001083397464"></a>

-   [简介](#section11660541593)
-   [目录](#section161941989596)
-   [约束](#section119744591305)
-   [编译构建](#section137768191623)
-   [hi3518ev300协议说明](#section1312121216216)
    -   [third\_party许可说明](#section129654513264)

-   [相关仓](#section1371113476307)

## 简介<a name="section11660541593"></a>

上海海思媒体软件开发包用于适配不同芯片复杂的底层处理，为“媒体子系统”提供基础的多媒体处理功能。主要功能有：音视频采集、音视频编解码、音视频输出、视频前处理、封装、解封装、文件管理、存储管理、日志系统等。该软件包功能对应"媒体子系统"框架中红色框标注部分，如图1所示。

**图 1**  多媒体子系统架构图<a name="fig4460722185514"></a>  


![](figures/zh-cn_image_0000001133374179.png)

## 目录<a name="section161941989596"></a>

```
/device/soc/hisilicon/hi3518ev300/
├── hdf_config            # Hi3518EV300设备hdf配置信息
└── mpp
    ├── lib               # Hi3518EV300芯片的媒体库文件、LICENSE文件
    └── module_init       # Hi3518EV300芯片媒体各模块驱动对应的库、LICENSE文件
```

## 约束<a name="section119744591305"></a>

当前支持Hi3518EV300芯片。

## 编译构建<a name="section137768191623"></a>

hb set
. (当前路径)
ipcamera_hi3518ev300
hb build -f

## hi3518ev300协议说明<a name="section1312121216216"></a>

-   hi3518ev300\\mpp\\module\_init\\lib和hi3518ev300\\mpp\\lib里面为上海海思的自研库，遵循上海海思的LICENSE，这两个目录下均有LICENSE文件，LICENSE文件结尾可以看到版权信息：

    ```
    Copyright (C) 2020 Hisilicon (Shanghai) Technologies Co., Ltd. All rights reserved.
    ```

-   hi3518ev300\\mpp\\module\_init\\src目录下为上海海思自研代码，使用基于Apache License Version 2.0许可的Hisilicon \(Shanghai\) 版权声明，在该目录下有Apache License Version 2.0的LICENSE文件，许可信息和版权信息通常可以在文件开头看到：

    ```
     / *Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED. Licensed under the Apache License,* ... * / 
    ```

### third\_party许可说明<a name="section129654513264"></a>


third\_party\\uboot\\u-boot-2020.01为uboot开源代码，遵循软件版本自带的开源许可声明，具体请参看third\_party\\uboot\\u-boot-2020.01\\Licenses目录下的README。

## 相关仓<a name="section1371113476307"></a>

[vendor_hisilicon](https://gitee.com/openharmony/vendor_hisilicon)

[device_board_hisilicon](https://gitee.com/openharmony/device_board_hisilicon)

