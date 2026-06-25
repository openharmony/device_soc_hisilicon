# HDC 开发参考

## 文档简介

本目录包含Hi3403V100芯片的HDC功能适配文档，涵盖HDC组件编译、HDC内核配置、HDC开机初始化、hdcd服务启动等开发参考。

### 文档目录

#### HDC组件编译

打开vendor/hisilicon/hispark_aifly_linux/config.json添加hdc组件。
  
   ```shell
    {
      "subsystem": "developtools",
      "components": [
        { "component": "syscap_codec", "features":[] },
        { "component": "hdc" }  # HDC组件
      ]
    },
   ```
执行编译命令，解决HDC组件编译报错，主要移除Lite版用不到的组件，使用ohos_lite做编译隔离

#### HDC组件新增宏定义

编译hdcd进程时，因该进程需要用到系统SystemParameter功能，但L1系统不具有该功能，后续待L1实现系统param功能之后，可以去掉该宏定义，使用init进程监听cfg脚本方式来实现hdc功能的配置

   ```shell
    if (is_standard_system) {
      defines += [ "HDC_SUPPORT_PARAM" ]
    }
   ```


#### HDC内核配置

使用USB HDC功能，主要依赖于USB支持Device模式及直接USB FFS功能。内核配置项如下
  
    ```shell
    CONFIG_USB_DWC3=y             # UDC控制器
    CONFIG_USB_DWC3_DUAL_ROLE=y   # UDC支持HOST及Device模式切换
    CONFIG_USB_GADGET=y           # USB支持Device设备
    CONFIG_USB_ROLE_SWITCH=y      # USB支持角色切换
    CONFIG_USB_GADGET_VBUS_DRAW=2
    CONFIG_USB_GADGET_STORAGE_NUM_BUFFERS=2

    CONFIG_USB_LIBCOMPOSITE=y     # USB支持FunctionFS
    CONFIG_USB_F_FS=y             # USB支持FunctionFS
    CONFIG_USB_CONFIGFS=y         # USB支持FunctionFS
    CONFIG_USB_CONFIGFS_F_FS=y    # USB支持FunctionFS
    ```
#### HDC开机初始化

新增L1系统，hdc开始初始化脚本文件init_hdc,开机时执行init_hdc，主要流程为：挂载configfs -> 创建gadget -> 设置设备描述符 -> 创建配置 -> 添加FunctionFS函数 -> 链接到配置 -> 创建挂载点并挂载functionfs，该脚本将一个Linux USB设备配置为一个 USB 复合设备，后续需结合hdcd进程，把USB描述符写给内核，并将UDC控制器10320000.dwc3写入/config/usb_gadget/g1/UDC来绑定UDC，激活USB设备
  
    ```shell
    #!/bin/sh

    #挂载configfs
    mkdir /config
    mount -t configfs none /config

    #创建USB Gadget
    mkdir /config/usb_gadget/g1 -m 0770
    chown shell:shell /config/usb_gadget/g1

    #设置USB设备描述符
    echo "0x2207" > /config/usb_gadget/g1/idVendor
    echo "0x5000" > /config/usb_gadget/g1/idProduct
    echo "0x0223" > /config/usb_gadget/g1/bcdDevice
    echo "0x0200" > /config/usb_gadget/g1/bcdUSB

    #设置USB字符串描述符
    mkdir /config/usb_gadget/g1/strings/0x409 -m 0770
    cp /sys/block/mmcblk0/device/cid /config/usb_gadget/g1/strings/0x409/serialnumber
    echo "HISILICON" > /config/usb_gadget/g1/strings/0x409/manufacturer 
    echo "HDC Device" > /config/usb_gadget/g1/strings/0x409/product

    #配置 OS 描述符（用于 Windows 的扩展属性）
    echo "0x1" > /config/usb_gadget/g1/os_desc/b_vendor_code  
    echo "MSFT100" > /config/usb_gadget/g1/os_desc/qw_sign 
    echo "1" > /config/usb_gadget/g1/os_desc/use

    #创建配置
    mkdir /config/usb_gadget/g1/configs/c.1 -m 0770
    chown shell:shell /config/usb_gadget/g1/configs/c.1
    mkdir /config/usb_gadget/g1/configs/c.1/strings/0x409 -m 0770
    chown shell:shell /config/usb_gadget/g1/configs/c.1/strings/0x409 
    echo "500" > /config/usb_gadget/g1/configs/c.1/MaxPower 
    echo "hdc" > /config/usb_gadget/g1/configs/c.1/strings/0x409/configuration
    ln -s /config/usb_gadget/g1/configs/c.1 /config/usb_gadget/g1/os_desc/c.1 

    #添加 FunctionFS 函数并关联到配置
    mkdir -p /config/usb_gadget/g1/functions/ffs.hdc
    ln -s /config/usb_gadget/g1/functions/ffs.hdc /config/usb_gadget/g1/configs/c.1

    #准备 FunctionFS 挂载点并挂载
    mkdir /dev/usb-ffs -m 0770 
    chown shell:shell /dev/usb-ffs 
    mkdir -p /dev/usb-ffs/hdc -m 0770
    chown shell:shell /dev/usb-ffs/hdc 
    mount -t functionfs hdc /dev/usb-ffs/hdc -o uid=2000,gid=2000
    ```

#### hdcd服务启动  

  启动hdcd服务，写入USB设备描述符给内核，供windows枚举USB设备使用，同时实现hdc命令行功能。

    ```shell
     "jobs" : [{
            "name" : "init",
            "cmds" : [
                "start hdcd"
        }]
    "services" : [
        {
            "name" : "hdcd",
            "path" : ["/bin/hdcd"],
            "uid" : 0,
            "gid" : 0,
            "once" : 0,
            "importance" : 0,
            "caps" : []
        }
    ]
    ```
###  特别说明

标准系统在启动hdcd进程之后，hdcd进程将名称sys.usb.ffs.ready.hdc的环境变量赋值为1，触发init进程执行写入UDC控制器的脚本，但小型系统不支持param功能，需要在hdcd进程里面添加代码写入UDC控制器，代替上面代码

    ```shell
    int HdcDaemonUSB::WriteUdc()
    {
        WRITE_LOG(LOG_DEBUG, "WriteUdc");

        int fd = -1;
        ssize_t bytes_written = -1;
        size_t len = strlen(UDC_NAME);

        fd = open(UDC_FILE_PATH, O_WRONLY | O_TRUNC); //UDC_FILE_PATH "/config/usb_gadget/g1/UDC"
        if (fd == -1) {
            WRITE_LOG(LOG_DEBUG, "WriteUdc Error: Cannot open file '%s'. %s\n", UDC_FILE_PATH, strerror(errno));
            return -1;
        }

        bytes_written = write(fd, UDC_NAME, len); //UDC_NAME "10320000.dwc3
        if (bytes_written == -1) {
            WRITE_LOG(LOG_DEBUG, "WriteUdc Error: Failed to write to file. %s\n", strerror(errno));
            close(fd);
            return -1;
        }

        if (bytes_written != (ssize_t)len) {
            WRITE_LOG(LOG_DEBUG, "WriteUdc Warning: Wrote %zd bytes, but expected %zu bytes.\n", bytes_written, len);
        } else {
            WRITE_LOG(LOG_DEBUG, "WriteUdc Success: UDC '%s' has been bound to gadget 'g1'.\n", UDC_NAME);
        }

        close(fd);
        return 0;
}
    ```

###  常见问题

**USB插入电脑时不能识别HDC Device设备**
```
解决：需要先配置好USB设备描述符，再启动hdcd进程，后面写入UDC控制器/config/usb_gadget/g1/UDC。
```
### HDC的下载

下载链接：https://cidownload.openharmony.cn/version/Master_Version/OpenHarmony_5.1.0.108/20260417_020157/version-Master_Version-OpenHarmony_5.1.0.108-20260417_020157-ohos-sdk-full_5.1.0-Release.tar.gz

解压下载后的文件toolchains-windows-x64-5.1.0.108-Release.zip，在toolchains目录下有hdc.exe及libusb_shared.dll，拷贝到windows目录下，如F:\Program Files\hdc_standard

配置系统环境变量：F:\Program Files\hdc_standard


###  HDC的使用

以下是常用hdc命令示例，供开发者参考：

查看设备连接信息

hdc list targets

往设备中推送文件

hdc file send  E:\a.txt  /data/local/tmp/a.txt

从设备中拉取文件

hdc file recv  /data/local/tmp/a.txt   ./a.txt

安装应用

hdc install E:\***.hap

查看日志

hdc shell hilogcat

进入命令行交互模式

hdc shell

TCP网络连接

hdc tconn 192.168.0.100:8710

hdc shell
