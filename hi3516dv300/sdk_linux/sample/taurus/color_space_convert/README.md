# color_space_convert sample操作指导

## 1. 概述

color_space_convert  sample基于OpenHarmony 小型系统开发，以Taurus套件为例，color_space_convert sample 介绍了常用色彩空间转换算子的具体实现过程。

## 2. 目录

```shell
//device/soc/hisilicon/hi3516dv300/sdk_linux/sample/taurus/color_space_convert
├── BUILD.gn                   				 # 编译ohos color_space_convert sample需要的gn文件
├── sample_color_space_convert_main.c        # ohos color_space_convert sample主函数入口
└── smp
    ├── smp_color_space_convert.c       	 # ohos color_space_convert sample业务代码
    └── smp_color_space_convert.h       	 # ohos color_space_convert sample业务代码所需的头文件
```

## 3. 编译

* 在编译color_space_convert  sample前，需确保OpenHarmony 小型系统的主干代码已整编通过，且**已经按照《[修改源码及配置文件适配Taurus开发板](../doc/2.2.1.%E4%BF%AE%E6%94%B9%E6%BA%90%E7%A0%81%E5%8F%8A%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E9%80%82%E9%85%8DTaurus%E5%BC%80%E5%8F%91%E6%9D%BF.md)》的内容进行修改**。
* 在单编color_space_convert  sample之前，需修改目录下的一处依赖，进入//device/soc/hisilicon/hi3516dv300/sdk_linux目录下，通过修改BUILD.gn，在deps下面新增target，``"sample/taurus/color_space_convert:hi3516dv300_color_space_convert_sample"``，如下图所示：

![](../doc/figures/hispark_taurus_color_space_convert_sample/061.%E4%BF%AE%E6%94%B9buildgn.png)

* 点击Deveco Device Tool工具的Build按键进行编译，具体的编译过程这里不再赘述，编译成功后，如下图所示：

![](../doc/figures/hispark_taurus_helloworld_sample/0002-build%20success.png)

* 编译成功后，即可在out/hispark_taurus/ipcamera_hispark_taurus_linux/rootfs/bin目录下，生成 ohos_color_space_convert_demo可执行文件，如下图所示：

![](../doc/figures/hispark_taurus_color_space_convert_sample/058%E7%94%9F%E6%88%90%E5%8F%AF%E6%89%A7%E8%A1%8C%E7%A8%8B%E5%BA%8F.png)

## 4. 拷贝可执行程序和依赖文件至开发板的mnt目录下

**方式一：使用SD卡进行资料文件的拷贝**

* 首先需要自己准备一张SD卡

* 步骤1：将编译后生成的可执行文件拷贝到SD卡中。

* 步骤2：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libvb_server.so和 libmpp_vbs.so**拷贝至SD卡中
* 步骤3：将device/soc/hisilicon/hi3516dv300/sdk_linux/sample/taurus/data/目录下的 UsePic_1920_1080_420.yuv文件拷贝至SD卡中。（前提是按照《[获取sample依赖的资源文件到本地](../doc/6.2.%E8%8E%B7%E5%8F%96sample%E4%BE%9D%E8%B5%96%E7%9A%84%E8%B5%84%E6%BA%90%E6%96%87%E4%BB%B6%E5%88%B0%E6%9C%AC%E5%9C%B0.md)》文档获取了资源文件）

![](../doc/figures/hispark_taurus_color_space_convert_sample/062%E5%A4%8D%E5%88%B6%E5%8F%AF%E6%89%A7%E8%A1%8C%E7%A8%8B%E5%BA%8F%E5%92%8C%E4%BE%9D%E8%B5%96%E6%96%87%E4%BB%B6%E8%87%B3SD%E5%8D%A1.png)

* 步骤4：可执行文件拷贝成功后，将内存卡插入开发板的SD卡槽中，可通过挂载的方式挂载到板端，可选择SD卡 mount指令进行挂载。

```shell
mount -t vfat /dev/mmcblk1p1 /mnt
# 其中/dev/mmcblk1p1需要根据实际块设备号修改
```

* 挂载成功后，如下图所示：

![](../doc/figures/hispark_taurus_color_space_convert_sample/063%E5%8A%A0%E8%BD%BDSD%E5%8D%A1%E8%87%B3%E5%BC%80%E5%8F%91%E6%9D%BF.png)

**方式二：使用NFS挂载的方式进行资料文件的拷贝**

* 首先需要自己准备一根网线
* 步骤1：参考[博客链接](https://blog.csdn.net/Wu_GuiMing/article/details/115872995?spm=1001.2014.3001.5501)中的内容，进行nfs的环境搭建

* 步骤2：将编译后生成的可执行文件拷贝到Windows的nfs共享路径下

* 步骤3：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libvb_server.so和 libmpp_vbs.so**拷贝至Windows的nfs共享路径下

* 步骤4：将device/soc/hisilicon/hi3516dv300/sdk_linux/sample/taurus/data/目录下的 UsePic_1920_1080_420.yuv文件拷贝至Windows的nfs共享路径下。（前提是按照《[获取sample依赖的资源文件到本地](../doc/6.2.%E8%8E%B7%E5%8F%96sample%E4%BE%9D%E8%B5%96%E7%9A%84%E8%B5%84%E6%BA%90%E6%96%87%E4%BB%B6%E5%88%B0%E6%9C%AC%E5%9C%B0.md)》文档获取了资源文件）

![](../doc/figures/hispark_taurus_color_space_convert_sample/066%E6%8A%8A%E5%8F%AF%E6%89%A7%E8%A1%8C%E6%96%87%E4%BB%B6%E5%92%8C%E4%BE%9D%E8%B5%96%E6%96%87%E4%BB%B6%E6%8B%B7%E8%B4%9D%E8%87%B3nfs.png)

* 步骤5：依赖文件拷贝至Windows的nfs共享路径下后，执行下面的命令，将Windows的nfs共享路径挂载至开发板的mnt目录下

```
mount -o nolock,addr=192.168.200.1 -t nfs 192.168.200.1:/d/nfs /mnt
```

## 5.拷贝mnt目录下的文件至正确的目录下

* 执行下面的命令，拷贝mnt目录下面的color_space_convert_demo至/userdata目录，拷贝mnt目录下面的libvb_server.so和 libmpp_vbs.so至/usr/lib/目录下，再创建一个/userdata/data/input/color_convert_img/目录和一个/userdata/data/output/color_convert_res/目录

```
mkdir -p /userdata/data/input/color_convert_img/
mkdir -p /userdata/data/output/color_convert_res/
cp /mnt/ohos_color_space_convert_demo  /
cp /mnt/*.so /usr/lib/
cp /mnt/UsePic_1920_1080_420.yuv /userdata/data/input/color_convert_img/
```

![](../doc/figures/hispark_taurus_color_space_convert_sample/064%E5%88%9B%E5%BB%BA%E4%B8%A4%E4%B8%AA%E6%96%87%E4%BB%B6%E5%A4%B9%E5%B9%B6%E6%8B%B7%E8%B4%9D%E4%BE%9D%E8%B5%96%E6%96%87%E4%BB%B6%E8%87%B3%E5%AF%B9%E5%BA%94%E7%9B%AE%E5%BD%95%E4%B8%8B.png)

* 执行下面的命令，给color_space_convert_demo文件可执行权限

```
chmod 777 ohos_color_space_convert_demo
```

## 6. 功能验证

### 6.1、ive image to video frame

* 执行下面的命令，即可完成ive image to video frame的转换，如下图所示：

```
./ohos_color_space_convert_demo 0
```

![](../doc/figures/hispark_taurus_color_space_convert_sample/043videoframetoiveimage.png)

* 转换成功后，即可在/userdata/data/output/color_convert_res/目录下，生成complete_ive_to_video.yuv图片，如下图所示：

![](../doc/figures/hispark_taurus_color_space_convert_sample/043%E5%BE%97%E5%88%B0yuv%E7%9A%84output%E6%96%87%E4%BB%B6.png)

### 6.2、video frame to ive image

* 执行下面的命令，即可完成video frame to ive imag的转换，如下图所示：

```
./ohos_color_space_convert_demo 1
```

![](../doc/figures/hispark_taurus_color_space_convert_sample/044videoyuvframetoiveimage.png)

* 转换成功后，即可在/userdata/data/output/color_convert_res/目录下，生成complete_frm_orig_img.yuv图片，如下图所示：

![](../doc/figures/hispark_taurus_color_space_convert_sample/044%E5%BE%97%E5%88%B0orig_img%E6%96%87%E4%BB%B6.png)

### 6.3、video YUV frame to ive image (U8C1)

* 执行下面的命令，即可完成video YUV frame to ive image (U8C1)的转换，如下图所示：

  ```
  ./ohos_color_space_convert_demo 2
  ```

![](../doc/figures/hispark_taurus_color_space_convert_sample/045yuvvideoframetorgbiveimage.png)

* 转换成功后，即可在/userdata/data/output/color_convert_res/目录下，生成complete_u8c1.yuv图片，如下图所示：

![](../doc/figures/hispark_taurus_color_space_convert_sample/045%E5%BE%97%E5%88%B0yuv%E7%9A%84u8c1%E6%96%87%E4%BB%B6.png)

### 6.4、YUV video frame to RGB ive image - ive image RGB to YUV

* 执行下面的命令，即可完成YUV video frame to RGB ive image - ive image RGB to YUV的转换，如下图所示：

```
./ohos_color_space_convert_demo 3
```

![](../doc/figures/hispark_taurus_color_space_convert_sample/065bgrimgtoyuv.png)

* 转换成功后，即可在/userdata/data/output/color_convert_res/目录下，生成complete_frm_rgb_yuv.yuv图片，如下图所示：

![](../doc/figures/hispark_taurus_color_space_convert_sample/065%E5%BE%97%E5%88%B0bgrtoyuv%E6%96%87%E4%BB%B6.png)

### 6.5、YUV video frame to RGB ive image - ive image RGB to BGR

* 执行下面的命令，即可完成YUV video frame to RGB ive image - ive image RGB to BGR的转换，如下图所示：

```
./ohos_color_space_convert_demo 4
```

![](../doc/figures/hispark_taurus_color_space_convert_sample/046yuvvideoframetorgbiveimage.png)







