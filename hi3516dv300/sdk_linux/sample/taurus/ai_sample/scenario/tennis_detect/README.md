# 5.4、网球检测实验功能验证

* 步骤1：在SD卡或Windows的nfs共享目录下，创建一个**sample_ai.conf**的文件，然后把下面的内容拷贝到此文件中

```cobol
; ai sample configuration file

[audio_player]
support_audio = false ; 垃圾识别语音播放

[ai_function]
support_ai = true ; 是否支持AI

[trash_classify_switch]
support_trash_classify = false ; 是否支持垃圾分类功能

[hand_classify_switch]
support_hand_classify = false ; 是否手势检测识别功能

[tennis_detect_switch]
support_tennis_detect = true ; 是否支持网球检测功能
```

![](../../../doc/figures/hispark_taurus_ai_sample/052%E4%BF%AE%E6%94%B9opencv%E7%9A%84ai%20config.png)

* 步骤2：再通过下面的挂载命令，把SD卡或者Windows的nfs共享目录挂载到开发板上

  * 方式1：SD卡

  ```
  mount -t vfat /dev/mmcblk1p1 /mnt
  # 其中/dev/mmcblk1p1需要根据实际块设备号修改
  ```

  * 方式2：Windows的nfs共享目录

    ```
    mount -o nolock,addr=192.168.200.1 -t nfs 192.168.200.1:/d/nfs /mnt
    ```

* 步骤3：将/mnt目录下的sample_ai.conf文件复制到userdata目录下。

```
cp /mnt/sample_ai.conf  /userdata
```

![](../../../doc/figures/hispark_taurus_ai_sample/039%E6%8B%B7%E8%B4%9Dai%20sample%20config%E8%87%B3userdata.png)

* 步骤4：执行下面的命令：进入/ko目录，加载mipi_tx驱动。

```
cd /ko
insmod hi_mipi_tx.ko
```

![](../../../doc/figures/hispark_taurus_ai_sample/040%E5%8A%A0%E8%BD%BDmipi_txko.png)

* 步骤5：执行下面的命令，把userdata/lib 加到环境变量里面

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/userdata/lib
```

![](../../../doc/figures/hispark_taurus_ai_sample/053%20%E6%B7%BB%E5%8A%A0lib%E8%B7%AF%E5%BE%84%E5%88%B0%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F.png)

* 步骤6：执行下面的命令，进行网球检测sample的验证

```
cd  /userdata
./ohos_camera_ai_demo 2
```

![](../../../doc/figures/hispark_taurus_ai_sample/054%E8%BF%90%E8%A1%8Copencv.png)

* 具体的现象如下图所示

![](../../../doc/figures/hispark_taurus_ai_sample/055.png)

* 敲两下回车即可关闭程序

![](../../../doc/figures/hispark_taurus_helloworld_sample/0007-helloworld%20log.png)
