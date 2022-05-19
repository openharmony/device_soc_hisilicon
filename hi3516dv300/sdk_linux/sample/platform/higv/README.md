# higv sample 操作指导

## 1. 概述

* HiGV是一个轻量级的 GUI 系统，主要是为芯片平台提供统一的轻量级、高效、易用的 GUI解决方案，higv  sample是海思Hi3516DV300的SDK sample，本文以Taurus套件为例，higv  sample主要介绍了如何使用海思的HIGV进行UI应用程序设计。

* higv的开发工具可以访问[higvbuilder下载链接](https://repo.harmonyos.com/#/cn/bundles/@huawei%2Fhigvbuilder)进行下载。
* higv开发相关的参考文档可[访问链接](https://gitee.com/openharmony/device_soc_hisilicon/tree/master/hi3516dv300/sdk_linux/sample/doc)进行下载，主要包括《GVBuilderTool 使用指南.pdf》、《HiGV API参考.pdf》、《HiGV 开发指南.pdf》、《HiGV 标签 使用指南.pdf》。

## 2. 目录

```shell
//device/soc/hisilicon/hi3516dv300/sdk_linux/sample/platform/higv
├── app.c                # ohos higv sample主函数入口
├── button.c             #
├── BUILD.gn             # 编译ohos higv sample需要的gn文件
├── higv_cextfile.c      # 各回调函数的指针
├── higv_cextfile.h
├── higv_language.h
├── higv_mw_media.c      # 媒体组件初始化
├── higv_mw_media.h
├── image.c              # image控件的事件回调函数
├── label.c              # label控件的事件回调函数
├── listbox.c            # listbox控件的事件回调函数
├── README.md
├── sample_utils.h
├── scrollview.c         # scrollview控件的事件回调函数
├── include              # 程序所依赖的头文件
├── lib                  # 应用程序依赖的图片资源和字体资源
├── tool                 # 将xml转换成bin文件的工具
└── xml                  # 应用程序的UI布局
```

## 3.代码修改

* 由于我的显示器是只支持1080P60帧，所以需要修改device/soc/hisilicon/hi3516dv300/sdk_linux/sample/platform/higv/目录下的higv_mw_media.c的**HI_MW_DISP_Open()**函数中的VO 输出配置，这个请根据自己的显示器支持的参数进行配置。

```c++
pubAttr.enIntfSync = VO_OUTPUT_1080P60; // 第259行的VO_OUTPUT_1080P30; 改成 VO_OUTPUT_1080P60;
```

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/175%E9%80%82%E9%85%8D%E6%98%BE%E7%A4%BA%E5%B1%8F.png)

* **按照《[获取sample依赖的资源文件到本地](../../taurus/doc/6.2.%E8%8E%B7%E5%8F%96sample%E4%BE%9D%E8%B5%96%E7%9A%84%E8%B5%84%E6%BA%90%E6%96%87%E4%BB%B6%E5%88%B0%E6%9C%AC%E5%9C%B0.md)》文档获取higv sample编译时依赖的资源文件**

## 4. 编译

在编译higv sample之前，需确保OpenHarmony 小型系统的主干代码已经整编通过，**且已经按照《[修改源码及配置文件适配Taurus开发板](../../taurus/doc/2.2.1.%E4%BF%AE%E6%94%B9%E6%BA%90%E7%A0%81%E5%8F%8A%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E9%80%82%E9%85%8DTaurus%E5%BC%80%E5%8F%91%E6%9D%BF.md)》的内容进行修改**。在单编higv sample之前，需修改目录下的一处依赖，进入//device/soc/hisilicon/hi3516dv300/sdk_linux目录下，通过修改BUILD.gn，在deps下面新增target，``"sample/platform/higv:higv_sample"``，如下图所示：

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/174higv%E4%BF%AE%E6%94%B9buildgn.png)

* 执行下面的命令，进入device\soc\hisilicon\hi3516dv300\sdk_linux\sample\platform\higv\xml\目录下，执行执行 make命令，生成higv.bin文件并自动更新higv_cextfile.c和higv_cextfile.c文件。

```
cd device/soc/hisilicon/hi3516dv300/sdk_linux/sample/platform/higv/xml

make clean && make
```

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/173higv%20make%20xml%E6%96%87%E4%BB%B6.png)

* 点击Deveco Device Tool工具的Build按键进行编译，具体的编译过程这里不再赘述，编译成功后，如下图所示：

![](../../taurus/doc/figures/hispark_taurus_helloworld_sample/0002-build%20success.png)

* 编译成功后，即可在out/hispark_taurus/ipcamera_hispark_taurus_linux/rootfs/bin目录下，生成 ohos_higv_demo可执行文件，如下图所示：

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/176%E7%94%9F%E6%88%90hig%20%E5%8F%AF%E6%89%A7%E8%A1%8C%E7%A8%8B%E5%BA%8F.png)

## 5. 拷贝可执行程序和依赖文件至开发板的mnt目录下

**方式一：使用SD卡进行资料文件的拷贝**

* 首先需要自己准备一张SD卡
* 步骤1：将编译后生成的可执行文件拷贝到SD卡中。
* 步骤2：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libvb_server.so和 libmpp_vbs.so**拷贝至SD卡中
* 步骤3：拷贝依赖文件，将device\soc\hisilicon\hi3516dv300\sdk_linux\sample\platform\higv 目录下的higv.bin和**res**文件夹拷贝到SD卡中（前提是按照《[获取sample依赖的资源文件到本地](../../taurus/doc/6.2.%E8%8E%B7%E5%8F%96sample%E4%BE%9D%E8%B5%96%E7%9A%84%E8%B5%84%E6%BA%90%E6%96%87%E4%BB%B6%E5%88%B0%E6%9C%AC%E5%9C%B0.md)》文档获取了资源文件）

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/177%E5%A4%8D%E5%88%B6higv%E6%96%87%E4%BB%B6%E8%87%B3U%E7%9B%98.png)

* 步骤4：可执行文件拷贝成功后，将内存卡插入开发板的SD卡槽中，可通过挂载的方式挂载到板端，可选择SD卡 mount指令进行挂载。

```shell
mount -t vfat /dev/mmcblk1p1 /mnt
# 其中/dev/mmcblk1p1需要根据实际块设备号修改
```

* 挂载成功后，如下图所示：

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/179%E5%B0%86higv%E6%96%87%E4%BB%B6%E8%BF%9B%E8%A1%8CSD%E5%8D%A1%E7%9A%84%E6%8C%82%E8%BD%BD.png)

**方式二：使用NFS挂载的方式进行资料文件的拷贝**

* 首先需要自己准备一根网线
* 步骤1：参考[博客链接](https://blog.csdn.net/Wu_GuiMing/article/details/115872995?spm=1001.2014.3001.5501)中的内容，进行nfs的环境搭建
* 步骤2：将编译后生成的可执行文件拷贝到Windows的nfs共享路径下
* 步骤3：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libvb_server.so和 libmpp_vbs.so**拷贝至Windows的nfs共享路径下
* 步骤4：拷贝依赖文件，将device\soc\hisilicon\hi3516dv300\sdk_linux\sample\platform\higv 目录下的higv.bin和**res**文件夹拷贝到Windows的nfs共享路径下（前提是《[获取sample依赖的资源文件到本地](../../taurus/doc/6.2.%E8%8E%B7%E5%8F%96sample%E4%BE%9D%E8%B5%96%E7%9A%84%E8%B5%84%E6%BA%90%E6%96%87%E4%BB%B6%E5%88%B0%E6%9C%AC%E5%9C%B0.md)》文档获取了资源文件）

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/178%E5%A4%8D%E5%88%B6higv%E6%96%87%E4%BB%B6%E8%87%B3nfs%E8%B7%AF%E5%BE%84.png)

* 步骤5：执行下面的命令，将Windows的nfs共享路径挂载至开发板的mnt目录下

```
mount -o nolock,addr=192.168.200.1 -t nfs 192.168.200.1:/d/nfs /mnt
```

## 6.拷贝mnt目录下的文件至正确的目录下

* 执行下面的命令，拷贝mnt目录下面的ohos_higv_demo至根目录，拷贝mnt目录下面的libvb_server.so和 libmpp_vbs.so至/usr/lib/目录下

```
cp /mnt/ohos_higv_demo  /userdata

cp /mnt/*.so /usr/lib/

cp /mnt/higv.bin /userdata

cp /mnt/res /userdata -rf
```

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/180%E6%8B%B7%E8%B4%9Dhigv%E4%BE%9D%E8%B5%96%E6%96%87%E4%BB%B6%E8%87%B3userdate.png)

* 执行下面的命令，给ohos_higv_demo文件可执行权限

```
chmod 777 /userdata/ohos_higv_demo
```

## 7. 功能验证

* 上述步骤完成后，需要将mini HDMI线一端接口Tauru套件的HDMI口（mini HDMI线需要开发者自行购买），另一端接显示器，接下来执行下面的命令，启动可执行文件

```
cd /userdata

./ohos_higv_demo
```

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/181%E8%BF%90%E8%A1%8Chigv%E7%A8%8B%E5%BA%8F.png)

* 此时，通过HDMI线，在显示器上面显示higv的应用程序的UI界面，如下图所示：

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/182higv%E8%BF%90%E8%A1%8C%E5%90%8E%E7%9A%84%E7%BB%93%E6%9E%9C.png)

* 输入quit或q，然后敲回车即可关闭程序

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/182%E8%BE%93%E5%85%A5q%E9%80%80%E5%87%BAhigv%E7%A8%8B%E5%BA%8F.png)
