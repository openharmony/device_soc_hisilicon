# device_soc_hisilicon

## 简介

本仓用于存放上海海思芯片相关内容，包含HDI实现（media、display、camera、codec、audio、ai等）、芯片驱动、外设驱动、芯片SDK库和头文件等，具体内容参考各soc子目录下的readme说明，对应开发板详细信息请点击下表开发板名称跳转。

|                 SoC型号                 |                                             对应开发板                                              |      系统类型      |   领域   |
| :-------------------------------------: | :-------------------------------------------------------------------------------------------------: | :----------------: | :------: |
| [Hi3518EV300](hi3518ev300/README_zh.md) |   [HiSpark Aries](https://gitee.com/openharmony/device_board_hisilicon/tree/master/hispark_aries)   |      小型系统      | 智慧视觉 |
|  [Hi3861V100](hi3861v100/README_zh.md)  | [HiSpark Pegasus](https://gitee.com/openharmony/device_board_hisilicon/tree/master/hispark_pegasus) |      轻量系统      | 智慧IOT  |
|  [Hi3751V351](hi3751v350/README_zh.md)  | [HiSpark Phoenix](https://gitee.com/openharmony/device_board_hisilicon/tree/master/hispark_phoenix) |      标准系统      | 智慧媒体 |
| [Hi3516DV300](hi3516dv300/README_zh.md) |  [HiSpark Taurus](https://gitee.com/openharmony/device_board_hisilicon/tree/master/hispark_taurus)  | 小型系统、标准系统 | 智慧视觉 |


## 目录
```
/device/soc/hisilicon
├── common         # 平台驱动、HDI实现相关
├── hi3516dv300    # hi3516dv300芯片相关内容
├── hi3518ev300    # hi3518ev300芯片相关内容
├── hi3751v350     # hi3751v350芯片相关内容
├── hi3861v100     # hi3861v100芯片相关内容
```


## 使用方法

参考各开发板子目录下readme说明


## 许可说明

参见对应目录的LICENSE文件及代码声明


## 相关仓

[device_board_hisilicon](https://gitee.com/openharmony/device_board_hisilicon)

[vendor_hisilicon](https://gitee.com/openharmony/vendor_hisilicon)

[third_party_u-boot](https://gitee.com/openharmony/third_party_u-boot)