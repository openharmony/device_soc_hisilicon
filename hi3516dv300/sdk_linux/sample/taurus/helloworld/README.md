# helloworld操作指导

## 1. 概述

helloworld  sample基于OpenHarmony 小型系统开发，以Taurus套件为例，helloworld sample详细介绍Sensor至MIPI屏的整个视频通路实现方式（VI-VPSS-VO-MIPI），从编码的角度引导开发者跑通Hi3516DV300的媒体通路，并将视频流显示到MIPI屏上。

## 2. 目录

```shell
//device/soc/hisilicon/hi3516dv300/sdk_linux/sample/taurus/helloworld
├── BUILD.gn                # 编译ohos helloworld sample需要的gn文件
├── sample_lcd_main.c       # ohos helloworld sample主函数入口
└── smp
    ├── sample_lcd.c        # ohos helloworld sample业务代码
    └── sample_lcd.h        # ohos helloworld sample业务代码所需的头文件
```

## 3. 编译

在编译helloworld sample之前，需确保OpenHarmony 小型系统的主干代码已经整编通过，**且已经按照《[修改源码及配置文件适配Taurus开发板](../doc/2.2.1.%E4%BF%AE%E6%94%B9%E6%BA%90%E7%A0%81%E5%8F%8A%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E9%80%82%E9%85%8DTaurus%E5%BC%80%E5%8F%91%E6%9D%BF.md)》的内容进行修改**。在单编helloworld sample之前，需修改目录下的一处依赖，进入//device/soc/hisilicon/hi3516dv300/sdk_linux目录下，通过修改BUILD.gn，在deps下面新增target，``"sample/taurus/helloworld:hi3516dv300_helloworld_sample"``，如下图所示：

![](../doc/figures/hispark_taurus_helloworld_sample/0001-sdk%20gn.png)

* 点击Deveco Device Tool工具的Build按键进行编译，具体的编译过程这里不再赘述，编译成功后，如下图所示：

![](../doc/figures/hispark_taurus_helloworld_sample/0002-build%20success.png)

* 编译成功后，即可在out/hispark_taurus/ipcamera_hispark_taurus_linux/rootfs/bin目录下，生成 ohos_helloworld_demo可执行文件，如下图所示：

![](../doc/figures/hispark_taurus_helloworld_sample/0003-helloworld%20demo.png)

## 4. 拷贝可执行程序和依赖文件至开发板的mnt目录下

**方式一：使用SD卡进行资料文件的拷贝**

* 首先需要自己准备一张SD卡
* 步骤1：将编译后生成的可执行文件拷贝到SD卡中。

* 步骤2：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libvb_server.so和 libmpp_vbs.so**拷贝至SD卡中

![](../doc/figures/hispark_taurus_helloworld_sample/190%E5%B0%86%E5%8F%AF%E6%89%A7%E8%A1%8C%E7%A8%8B%E5%BA%8F%E6%94%BE%E5%88%B0SD%E5%8D%A1%E4%B8%AD.png)



* 步骤3：可执行文件拷贝成功后，将内存卡插入开发板的SD卡槽中，可通过挂载的方式挂载到板端，可选择SD卡 mount指令进行挂载。

```shell
mount -t vfat /dev/mmcblk1p1 /mnt
# 其中/dev/mmcblk1p1需要根据实际块设备号修改
```

* 挂载成功后，如下图所示：

![](../doc/figures/hispark_taurus_helloworld_sample/191%E6%8C%82%E8%BD%BDSD%E5%8D%A1.png)

**方式二：使用NFS挂载的方式进行资料文件的拷贝**

* 首先需要自己准备一根网线
* 步骤1：参考[博客链接](https://blog.csdn.net/Wu_GuiMing/article/details/115872995?spm=1001.2014.3001.5501)中的内容，进行nfs的环境搭建

* 步骤2：将编译后生成的可执行文件拷贝到Windows的nfs共享路径下

* 步骤3：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libvb_server.so和 libmpp_vbs.so**拷贝至Windows的nfs共享路径下

![](../doc/figures/readme/011%E9%80%9A%E8%BF%87nfs%E6%8C%82%E8%BD%BD%E7%9A%84%E6%96%B9%E5%BC%8F%E6%8B%B7%E8%B4%9D%E4%BE%9D%E8%B5%96%E6%96%87%E4%BB%B6.png)

* 步骤4：执行下面的命令，将Windows的nfs共享路径挂载至开发板的mnt目录下

```
mount -o nolock,addr=192.168.200.1 -t nfs 192.168.200.1:/d/nfs /mnt
```

## 5.拷贝mnt目录下的文件至正确的目录下

* 执行下面的命令，拷贝mnt目录下面的ohos_helloworld_demo至根目录，拷贝mnt目录下面的libvb_server.so和 libmpp_vbs.so至/usr/lib/目录下

```
cp /mnt/ohos_helloworld_demo  /userdata
cp /mnt/*.so /usr/lib/
```

![](../doc/figures/hispark_taurus_helloworld_sample/192%E6%8B%B7%E8%B4%9D%E4%BE%9D%E8%B5%96%E6%96%87%E4%BB%B6%E8%87%B3%E5%BC%80%E5%8F%91%E6%9D%BF.png)

* 执行下面的命令，给ohos_helloworld_demo文件可执行权限

```
chmod 777 /userdata/ohos_helloworld_demo
```

## 6. 功能验证

* 在运行ohos_helloworld_demo可执行文件之前，需要加载hi_mipi_tx.ko文件

```shell
insmod /ko/hi_mipi_tx.ko
```

![](../doc/figures/hispark_taurus_helloworld_sample/194%E5%8A%A0%E8%BD%BDmipi_tx%E9%A9%B1%E5%8A%A8.png)

* 加载成功后，即可执行下面的命令，启动可执行文件

```
cd /userdata

./ohos_helloworld_demo
```

![](../doc/figures/hispark_taurus_helloworld_sample/195%E6%89%A7%E8%A1%8C%E5%8F%AF%E6%89%A7%E8%A1%8C%E7%A8%8B%E5%BA%8F.png)

* 此时，MIPI屏幕即可出现实时码流，如下图所示：

![](../doc/figures/hispark_taurus_helloworld_sample/0008-clip_image002-1647943194629.jpg)

* 敲两下回车即可关闭程序

![](../doc/figures/hispark_taurus_helloworld_sample/0007-helloworld%20log.png)

