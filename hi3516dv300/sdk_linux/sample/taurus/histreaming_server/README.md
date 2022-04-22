# Pegasus与Taurus串口互联通信<a name="ZH-CN_TOPIC_0000001130176841"></a>

-    前言：HiStreaming 组件作为一种技术基础设施，使得海思芯片可以通过WiFi或有线网络实现物联网设备之间的设备自动发现、服务注册与识别、服务操作。HiStreaming把物联网设备分为两类角色，对外部提供服务的设备称之为 Server 设备，而使用其他设备提供的服务的设备称之为 Client 设备。

     ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/1.jpg)
    
## 硬件环境搭建
-    硬件要求：Taurus开发板；硬件搭建如下图所示。注意这里需要跟Taurus同时使用，详情可以参考[WiFi互联client端](http://gitee.com/openharmony/vendor_hisilicon/blob/master/hispark_pegasus/demo/histreaming_client_demo/README.md)端。

     ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/260.jpg)

     ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/12.jpg)

## 组网方式
-    组网方案1：将Taurus开发套件设置成为WiFi AP模式，Pegasus开发套件和手机直接连接到Taurus的WiFi AP热点。Taurus开发板上跑的是HiStreaming-Server和HiStreaming-Client程序，Pegasus开发板上跑的是HiStreaming-Server程序，手机上跑的是HiStreaming-Client程序。当三者在同一局域网内，手机能够同时发现Taurus和Pegasus上的HiStreaming-Server，且Taurus上的HiStreaming-Client也能发现Pegasus上的HiStreaming-Server。Taurus端、Pegasus端、手机端，三者之间的组网方式如下图所示。

     ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/2.jpg) 

-    组网方案2：Pegasus端、Taurus端、手机端都配置成为STA模式，使Taurus开发套件、Pegasus开发套件以及手机都连接在同一路由器发出的WiFi AP热点下面，组成一个局域网。其中，Taurus开发板上跑的是HiStreaming-Server和HiStreaming-Client程序，Pegasus开发板上跑的是HiStreaming-Server程序，手机上跑的是HiStreaming-Client程序。当三者在同一局域网内，手机能够同时发现Taurus和Pegasus上的HiStreaming-Server，且Taurus上的HiStreaming-Client也能发现Pegasus上的HiStreaming-Server。Taurus端、Pegasus端、手机端，三者之间的组网方式如下图所示。（其实手机作为热点代替路由器也是可行的）

     ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/3.jpg)

## 串口通信控制协议HiSignalling介绍
-    为了便于Taurus与Pegasus开发套件之间进行通信和控制，定义了一套简易的HiSignalling通信控制协议，数据帧格式如下表所示，并提供相关参考代码，大家也可以根据自己的需要使用其他协议。

| 帧头（2Byte）  | Payload Len (2Byte)  | payload  | 帧尾（1Byte）  | CRC32(4Byte)  |
|---|---|---|---|---|
| 0xAA,0x55  |   |   | 0xFF  | CRC32 |
			
例如一组数据帧为：AA5500020003FF8ED2BEDF (十六进制不区分大小写)
-    0AA55:       帧头
-    0002：       Payload Len
-    0003:        Payload
-    FF:          帧尾
-    8ED2BEDF:    CRC32校验码

## 软件介绍
-    注意这里需要跟Taurus同时使用，Taurus软件介绍详情可以参考[WiFi互联client端](http://gitee.com/openharmony/vendor_hisilicon/blob/master/hispark_pegasus/demo/histreaming_client_demo/README.md)。
-    1.代码目录结构及相应接口功能介绍

-    HiStreaming接口：

| API                 | 描述               |
| ------------------- | ------------------ |
| LinkPlatformGe | 获得HiStreamingLinkLite组件对象 |
| LinkPlatformFree | 释放HiStreamingLinkLite组件对象     |
| LinkServiceAgentFree  | 释放从设备列表中pop出来的LinkServiceAgent对象     |
| LinkAgentGet  | 获得LinkAgent对象     |
| LinkAgentFree  | 释放LinkAgent对象    |
| QueryResultFree  | 释放设备列表QueryResult。同时也释放设备列表关联的LinkServiceAgent对象    |

**使用NFS挂载的方式进行资料文件的拷贝**

* 首先需要自己准备一根网线

* 步骤1：参考[博客链接](https://blog.csdn.net/Wu_GuiMing/article/details/115872995?spm=1001.2014.3001.5501)中的内容，进行nfs的环境搭建

* 步骤2：将编译后生成的可执行文件拷贝到Windows的nfs共享路径下

* 步骤3：将device\soc\hisilicon\hi3516dv300\sdk_linux\out\lib\目录下的**libhistreaminglink.a**和 out\hispark_taurus\ipcamera_hispark_taurus_linux\libs\libcoap_static.a**拷贝至Windows的nfs共享路径下

* 步骤4：依赖文件拷贝至Windows的nfs共享路径下后，执行下面的命令，将Windows的nfs共享路径挂载至开发板的mnt目录下

```
mount -o nolock,addr=192.168.200.1 -t nfs 192.168.200.1:/d/nfs /mnt
```

## 5.拷贝mnt目录下的文件至正确的目录下

* 执行下面的命令，拷贝mnt目录下面的ohos_camera_ai_demo至userdata目录，拷贝mnt目录下面的libvb_server.so和 libmpp_vbs.so至/usr/lib/目录下，再将models和aac_file文件夹拷贝至userdata目录下

```
cp /mnt/ohos_histreaming_server  /userdata/
cp /mnt/*.so /usr/lib/
```

* 拷贝完成后参考WiFi使用说明。

* 执行下面的命令，给ohos_histreaming_server  文件可执行权限,同时运行

```
chmod 777 /userdata/ohos_histreaming_server
cd /userdata
./ohos_histreaming_server
```
* 板端运行后，参考[WiFi互联client端](http://gitee.com/openharmony/vendor_hisilicon/blob/master/hispark_pegasus/demo/histreaming_client_demo/README.md)

* Taurus端或者路由器需要发出热点，同时Taurus端运行ohos_histreaming_server可执行文件，再次点击Hi3861核心板上的“RST”复位键，此时开发板的系统会运行起来。运行结果:打开串口工具，可以看到如下打印,同时3861主板灯闪亮一下。

  ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/260.jpg)

  ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/22.jpg)

  ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/24.jpg)

* 如果你想使用手机APP来控制Pegasus或者Taurus，手机端APP安装及使用（[histreaming APP源码](http://gitee.com/leo593362220/sources-histreaming-app.git)），然后进入app-release.rar目录，将app-debug.apk安装到手机上，具体的安装过程这里就不介绍了(通过数据线复制到手机，或使用微信、QQ等方式发送到手机再安装)。
APP安装成功后，打开手机的WiFi列表，连接到Taurus开发板的AP热点或者路由器热点，再打开刚安装好的HiStreaming APP，下拉刷新几次，手机会发现两个设备，分别是Pegasus开发板设备和Taurus开发板设备。

  ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/20.jpg)

  ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/21.jpg)

* 任意点击一个设备进行操作，点击LED灯控制按钮，会进入一个灯的控制界面。点击图片会发生变化，且会给对应的设备发送数据,同时控制灯亮与熄。

  ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/6.jpg)

  ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/25.jpg)
    
  ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/23.jpg)

  ![输入图片说明](https://gitee.com/asd1122/tupian/raw/master/%E5%9B%BE%E7%89%87/wifi%E4%BA%92%E8%81%94/22.jpg)