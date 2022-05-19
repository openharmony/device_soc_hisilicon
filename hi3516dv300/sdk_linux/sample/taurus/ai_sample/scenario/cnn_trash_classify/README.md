# 5.3、垃圾分类实验功能验证

* 步骤1：在SD卡或Windows的nfs共享目录下，创建一个**sample_ai.conf**的文件，然后把下面的内容拷贝到此文件中

```cobol
; ai sample configuration file

[audio_player]
support_audio = true ; 垃圾识别语音播放

[ai_function]
support_ai = true ; 是否支持AI

[trash_classify_switch]
support_trash_classify = true ; 是否支持垃圾分类功能

[hand_classify_switch]
support_hand_classify = false ; 是否手势检测识别功能

```

![](../../../doc/figures/hispark_taurus_ai_sample/042%E4%BF%AE%E6%94%B9conf%E6%96%87%E4%BB%B6%E4%BD%BF%E8%83%BD%E5%9E%83%E5%9C%BE%E5%88%86%E7%B1%BB.png)

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

* 执行下面的命令，进行垃圾分类sample的验证

```
cd  /userdata
./ohos_camera_ai_demo 0
```

![](../../../doc/figures/hispark_taurus_ai_sample/043%E6%89%A7%E8%A1%8C%E5%9E%83%E5%9C%BE%E5%88%86%E7%B1%BB.png)

* 推理结果打印信息如下图所示：

![](../../../doc/figures/hispark_taurus_ai_sample/049%E6%8E%A8%E7%90%86%E7%BB%93%E6%9E%9C%E5%9B%BE.png)

![](../../../doc/figures/hispark_taurus_ai_sample/056%E4%BD%BF%E7%94%A8OSD%E6%98%BE%E7%A4%BA%E5%85%B7%E4%BD%93%E7%9A%84%E6%96%87%E5%AD%97.png)

**注意：**

​	（1）支持osd在MIPI屏上叠加字符，字库文件为：ai_sample/mpp_help/include/simsunb_16x32.txt；
​	（2）字库支持的格式为：英文字母、符号和数字，不支持汉字



* 敲两下回车即可关闭程序

![](../../../doc/figures/hispark_taurus_helloworld_sample/0007-helloworld%20log.png)



