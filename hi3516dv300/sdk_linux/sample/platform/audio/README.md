# audio sample操作指导

## 1. 概述

audio sample是海思Hi3516DV300的SDK sample，本文以Taurus套件为例，audio sample详细介绍整个音频通路实现方式，从编码的角度引导开发者跑通Hi3516DV300的音频通路，并将MIC采集到音频数据保存到本地，并将本地的音频通过喇叭进行播放。

## 2. 目录

```shell
//device/soc/hisilicon/hi3516dv300/sdk_linux/sample/platform/audio
├── BUILD.gn                # 编译ohos audio  sample需要的gn文件
├── sample_audio.c          # ohos audio  sample主函数入口及具体的业务代码实现
└── adp                     # ohos audio  sample依赖的一些audio编解码的接口
    ├── audio_aac_adp.c
    ├── audio_aac_adp.h
    ├── audio_dl_adp.c
    └── audio_dl_adp.h
```

## 3. 编译

在编译vio sample之前，需确保OpenHarmony 小型系统的主干代码已经整编通过，**且已经按照《[修改源码及配置文件适配Taurus开发板](../../taurus/doc/2.2.1.%E4%BF%AE%E6%94%B9%E6%BA%90%E7%A0%81%E5%8F%8A%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E9%80%82%E9%85%8DTaurus%E5%BC%80%E5%8F%91%E6%9D%BF.md)》的内容进行修改**。在单编vio sample之前，需修改目录下的一处依赖，进入//device/soc/hisilicon/hi3516dv300/sdk_linux目录下，通过修改BUILD.gn，在deps下面新增target，``"sample/platform/audio:audio_sample"``，如下图所示：

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/172%E4%BF%AE%E6%94%B9audio%E7%9A%84buildgn%E6%96%87%E4%BB%B6.png)

* 点击Deveco Device Tool工具的Build按键进行编译，具体的编译过程这里不再赘述，编译成功后，如下图所示：

![](../../taurus/doc/figures/hispark_taurus_helloworld_sample/0002-build%20success.png)

* 编译成功后，即可在out/hispark_taurus/ipcamera_hispark_taurus_linux/rootfs/bin目录下，生成 ohos_audio _demo可执行文件，如下图所示：

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/161audio%E5%BE%97%E5%88%B0%E5%8F%AF%E6%89%A7%E8%A1%8C%E6%96%87%E4%BB%B6.png)

## 4. 拷贝可执行程序和依赖文件至开发板的mnt目录下

**方式一：使用SD卡进行资料文件的拷贝**

* 首先需要自己准备一张SD卡
* 步骤1：将编译后生成的可执行文件拷贝到SD卡中。

* 步骤2：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libvb_server.so和 libmpp_vbs.so**拷贝至SD卡中

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/159audio_U%E7%9B%98%E6%8B%B7%E8%B4%9D.png)

* 步骤3：可执行文件拷贝成功后，将内存卡插入开发板的SD卡槽中，可通过挂载的方式挂载到板端，可选择SD卡 mount指令进行挂载。

```shell
mount -t vfat /dev/mmcblk1p1 /mnt
# 其中/dev/mmcblk1p1需要根据实际块设备号修改
```

* 挂载成功后，如下图所示：

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/162audio_sd%E5%8D%A1%E6%8C%82%E8%BD%BD.png)

**方式二：使用NFS挂载的方式进行资料文件的拷贝**

* 首先需要自己准备一根网线
* 步骤1：参考[博客链接](https://blog.csdn.net/Wu_GuiMing/article/details/115872995?spm=1001.2014.3001.5501)中的内容，进行nfs的环境搭建

* 步骤2：将编译后生成的可执行文件拷贝到Windows的nfs共享路径下

* 步骤3：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libvb_server.so和 libmpp_vbs.so**拷贝至Windows的nfs共享路径下

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/160audio_nfs%E6%8B%B7%E8%B4%9D.png)

* 步骤4：执行下面的命令，将Windows的nfs共享路径挂载至开发板的mnt目录下

```
mount -o nolock,addr=192.168.200.1 -t nfs 192.168.200.1:/d/nfs /mnt
```

## 5.拷贝mnt目录下的文件至正确的目录下

* 执行下面的命令，拷贝mnt目录下面的ohos_audio _demo至根目录，拷贝mnt目录下面的libvb_server.so和 libmpp_vbs.so至/usr/lib/目录下

```
cp /mnt/ohos_audio_demo  /userdata
cp /mnt/*.so /usr/lib/
```

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/163%E6%8B%B7%E8%B4%9Daudio%E4%BE%9D%E8%B5%96%E6%96%87%E4%BB%B6%E8%87%B3%E5%BC%80%E5%8F%91%E6%9D%BF.png)

* 执行下面的命令，给ohos_audio _demo文件可执行权限

```
chmod 777 /userdata/ohos_audio _demo
```

## 6. 功能验证

### 6.1.验证Ai至Ao功能

* 加载成功后，即可执行下面的命令，启动可执行文件，测试的时候，我们可以对着Taurus开发板的mic讲话，此时你发出的声音会从下方的喇叭播放出来。

```
cd /userdata

./ohos_audio_demo 0
```

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/164%E6%89%A7%E8%A1%8Caudio_index_0.png)

* Taurus开发板的mic位置图：

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/170Taurus%E7%9A%84mic.png)

* Taurus开发板的喇叭位置图：

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/171Taurus%E7%9A%84%E5%96%87%E5%8F%AD.png)

* 关于audio sample的退出，在终端敲两下回车即可

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/168audio%E9%80%80%E5%87%BA%E6%8F%90%E7%A4%BA.png)

### 6.2.验证保存音频文件至本地的功能

* 加载成功后，即可执行下面的命令，启动可执行文件，测试的时候，我们可以对着Taurus开发板的mic讲话，此时你发出的声音会保存到开发板中。

```
cd /userdata

./ohos_audio_demo 1
```

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/165%E6%89%A7%E8%A1%8Caudio_index_1.png)

* 执行成功后，会在可执行文件的同级目录下生成一个audio_chn0.aac的音频文件

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/166%E5%BE%97%E5%88%B0audio%E7%9A%84%E5%BD%95%E5%88%B6%E6%96%87%E4%BB%B6.png)

### 6.3.验证播放本地音频的功能

* 加载成功后，即可执行下面的命令，启动可执行文件，此时就会播放我们保存到开发板的audio_chn0.aac文件，注意这个audio.chn0.aac文件要和可执行文件在同一个目录下。
* 注意：此sample对audio.chn0.aac的参数有一定要求，具有可参考[博客链接来制作aac音频](https://blog.csdn.net/Wu_GuiMing/article/details/116425367)，然后把制作的音频文件重命名为audio_chn0.aac，再通过SD卡挂载或者nfs挂载的方式拷贝至开发板的指定路径下。这里就不再赘述如何拷贝aac文件了。可参考本文第4，5节的内容。

```
cd /userdata

./ohos_audio_demo 2
```

![](../../taurus/doc/figures/hispark_taurus_nnie_sample/167%E6%92%AD%E6%94%BE%E6%8F%90%E5%89%8D%E5%87%86%E5%A4%87%E5%A5%BD%E7%9A%84audio%E6%96%87%E4%BB%B6.png)

