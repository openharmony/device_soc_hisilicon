# store_sample操作指导

## 1. 概述

store_sample基于OpenHarmony 小型系统开发，以Taurus套件为例，store_sample主要是介绍如何使用Taurus套件，通过Sensor采集视频数据，并将采集到的视频数据保存为H264文件至开发板中，开发者将H264码流解码成mp4码流，即可用于后面数据集的制作。

## 2. 目录

```shell
//device/soc/hisilicon/hi3516dv300/sdk_linux/sample/taurus/store_sample
├── BUILD.gn                 # 编译ohos store_sample需要的gn文件
├── sample_store_main.c      # ohos store_sample主函数入口
└── smp
    ├── sample_store.c        # ohos store_sample业务代码
    └── sample_store.h        # ohos store_sample业务代码所需的头文件
```

## 3. 编译

在编译store_sample之前，需确保OpenHarmony 小型系统的主干代码已经整编通过，**且已经按照《[修改源码及配置文件适配Taurus开发板](../doc/2.2.1.%E4%BF%AE%E6%94%B9%E6%BA%90%E7%A0%81%E5%8F%8A%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E9%80%82%E9%85%8DTaurus%E5%BC%80%E5%8F%91%E6%9D%BF.md)》的内容进行修改**。在单编store_sample之前，需修改目录下的一处依赖，进入//device/soc/hisilicon/hi3516dv300/sdk_linux目录下，通过修改BUILD.gn，在deps下面新增target，``"sample/taurus/store_sample:hi3516dv300_store_sample"``，如下图所示：

![](../doc/figures/hispark_taurus_store_sample/101%E4%BF%AE%E6%94%B9buildgn.png)

* 点击Deveco Device Tool工具的Build按键进行编译，具体的编译过程这里不再赘述，编译成功后，如下图所示：

![](../doc/figures/hispark_taurus_helloworld_sample/0002-build%20success.png)

* 编译成功后，即可在out/hispark_taurus/ipcamera_hispark_taurus_linux/rootfs/bin目录下，生成 ohos_store_demo可执行文件，如下图所示：

![](../doc/figures/hispark_taurus_store_sample/103%E8%8E%B7%E5%BE%97%E5%8F%AF%E6%89%A7%E8%A1%8C%E6%96%87%E4%BB%B6.png)

## 4. 拷贝可执行程序和依赖文件至开发板的mnt目录下

**方式一：使用SD卡进行资料文件的拷贝**

* 首先需要自己准备一张SD卡

* 步骤1：将编译后生成的可执行文件拷贝到SD卡中。

* 步骤2：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libvb_server.so和 libmpp_vbs.so**拷贝至SD卡中

![](../doc/figures/hispark_taurus_store_sample/104%E5%B0%86%E5%8F%AF%E6%89%A7%E8%A1%8C%E6%96%87%E4%BB%B6%E5%A4%8D%E5%88%B6%E5%88%B0SD%E5%8D%A1.png)

* 步骤3：可执行文件拷贝成功后，将内存卡插入开发板的SD卡槽中，可通过挂载的方式挂载到板端，可选择SD卡 mount指令进行挂载。

```shell
mount -t vfat /dev/mmcblk1p1 /mnt
# 其中/dev/mmcblk1p1需要根据实际块设备号修改
```

* 挂载成功后，如下图所示：

![](../doc/figures/hispark_taurus_store_sample/105%E6%8C%82%E8%BD%BDSD%E5%8D%A1.png)

**方式二：使用NFS挂载的方式进行资料文件的拷贝**

* 首先需要自己准备一根网线
* 步骤1：参考[博客链接](https://blog.csdn.net/Wu_GuiMing/article/details/115872995?spm=1001.2014.3001.5501)中的内容，进行nfs的环境搭建

* 步骤2：将编译后生成的可执行文件拷贝到Windows的nfs共享路径下

* 步骤3：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libvb_server.so和 libmpp_vbs.so**拷贝至Windows的nfs共享路径下

![](../doc/figures/hispark_taurus_store_sample/111%E5%A4%8D%E5%88%B6%E5%8F%AF%E6%89%A7%E8%A1%8C%E7%A8%8B%E5%BA%8F%E8%87%B3nfs%E8%B7%AF%E5%BE%84.png)

* 步骤4：依赖文件拷贝至Windows的nfs共享路径下后，执行下面的命令，将Windows的nfs共享路径挂载至开发板的mnt目录下

```
mount -o nolock,addr=192.168.200.1 -t nfs 192.168.200.1:/d/nfs /mnt
```

## 5.拷贝mnt目录下的文件至正确的目录下

* 执行下面的命令，拷贝mnt目录下面的ohos_store_demo至根目录，拷贝mnt目录下面的libvb_server.so和 libmpp_vbs.so至/usr/lib/目录下

```
cp /mnt/ohos_store_demo /userdata
cp /mnt/*.so /usr/lib/
```

![](../doc/figures/hispark_taurus_store_sample/106%E6%8B%B7%E8%B4%9D%E5%8F%AF%E6%89%A7%E8%A1%8C%E6%96%87%E4%BB%B6%E5%92%8Cso%E6%96%87%E4%BB%B6%E8%87%B3%E5%BC%80%E5%8F%91%E6%9D%BF.png)

* 执行下面的命令，给ohos_store_demo文件可执行权限

```
chmod 777 /userdata/ohos_store_demo
```

![](../doc/figures/hispark_taurus_store_sample/107%E7%BB%99%E5%8F%AF%E6%89%A7%E8%A1%8C%E6%9D%83%E9%99%90.png)

## 6. 功能验证

* 执行下面的命令，启动可执行文件，然后按照提示依次输入c和0

```
cd /userdata

./ohos_store_demo
```

![](../doc/figures/hispark_taurus_store_sample/108%E6%89%A7%E8%A1%8C%E5%8F%AF%E6%89%A7%E8%A1%8C%E7%A8%8B%E5%BA%8F.png)

* 当视频录制OK了，可以在终端敲两下回车，停止程序运行。
* 通过ls -ah 命令可以查看当前目录下生成了一个stream_chn1.h264的视频文件。

![](../doc/figures/hispark_taurus_store_sample/109%E5%BE%97%E5%88%B0%E5%BD%95%E5%88%B6%E5%A5%BD%E7%9A%84h264%E6%96%87%E4%BB%B6.png)

* 可以通过执行cp命令，将生成的视频文件拷贝到SD卡或者nfs共享目录中，进行后面的其他处理。

```
cp stream_chn1.h264 /mnt
```

![](../doc/figures/hispark_taurus_store_sample/110%E5%A4%8D%E5%88%B6h264%E6%96%87%E4%BB%B6%E5%88%B0SD%E5%8D%A1.png)

