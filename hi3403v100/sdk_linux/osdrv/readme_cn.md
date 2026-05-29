# osdrv 顶层 Makefile 使用说明
## 编译环境配置
1. 本 readme 针对 ss928v100 进行说明;

2. bootloader 使用 aarch64-openeuler-linux-gnu 64bit工具链进行编译，需要从https://gitee.com/openeuler/yocto-meta-openeuler/releases下载openEuler Embedded Toolchains，可以参考以下步骤：
    ```bash
    wget https://gitee.com/openeuler/yocto-meta-openeuler/releases/download/toolchains-v0.1.8/1_openeuler_gcc_arm64le.tar.gz
    wget https://gitee.com/openeuler/yocto-meta-openeuler/releases/download/toolchains-v0.1.8/1_openeuler_gcc_arm64le.tar.gz
    wget https://gitee.com/openeuler/yocto-meta-openeuler/releases/download/toolchains-v0.1.8/2_openeuler_gcc_arm64le.tar.gz
    wget https://gitee.com/openeuler/yocto-meta-openeuler/releases/download/toolchains-v0.1.8/3_openeuler_gcc_arm64le.tar.gz
    wget https://gitee.com/openeuler/yocto-meta-openeuler/releases/download/toolchains-v0.1.8/4_openeuler_gcc_arm64le.tar.gz
    cat 1_openeuler_gcc_arm64le.tar.gz 2_openeuler_gcc_arm64le.tar.gz 3_openeuler_gcc_arm64le.tar.gz 4_openeuler_gcc_arm64le.tar.gz > openeuler_gcc_arm64le.tar.gz
    tar zxf openeuler_gcc_arm64le.tar.gz
    ```

3. 使用前需要将aarch64-openeuler-linux-gnu添加到环境变量中，注意更换成实际的路径。
    ```bash
    export PATH=$PATH:/Download_Path/openeuler_gcc_arm64le/bin
    ```

4. 使用前需要将tools/pc/uboot_tools/regbin-v1.0.2.tgz工具从多个压缩包中恢复，命令为：
    ```bash
    cd tools/pc/uboot_tools
	cat regbin-v1.0.2.tar.gz.0* > regbin-v1.0.2.tar.gz
	tar zxf regbin-v1.0.2.tar.gz
    ````

5. 使用前需确认`u-boot-2020.01.tar.bz2`文件存在，路径如下：
    ```bash
    ls sdk_linux/open_source/u-boot/u-boot-2020.01.tar.bz2
    # 若不存在
    cd sdk_linux/open_source/u-boot
    wget https://ftp.denx.de/pub/u-boot/u-boot-2020.01.tar.bz2
    ```

## 编译命令参考

1. 编译整个osdrv目录：
    ```bash
    make all
    # 默认编译
    make LLVM=1 BOOT_MEDIA=emmc CHIP=ss928v100 all
    ```

    参数说明：

    - CHIP：可以是ss928v100或ss927v100,默认为ss928v100。

    - BOOT_MEDIA：spi nor或spi nand启动选择spi；并口nand启动选择nand；emmc启动选择emmc。

2. 清除整个osdrv目录的编译文件：
    ```bash
    make clean
    ```

3. 彻底清除整个osdrv目录的编译中间文件：
    ```bash
    make distclean
    ```

4. 单独编译kernel image，在 osdrv 目录执行以下命令：
    ```bash
    make LLVM=1 BOOT_MEDIA=spi atf -j 20
    ```
    - 说明：根据实际情况，BOOT_MEDIA 可选 spi、nand 或 emmc

5. 单独编译快速启动的 uboot 或非安全启动的 Boot Image，在 osdrv 目录执行以下命令：
    ```bash
    make BOOT_MEDIA=spi gslboot_build -j 20
    ```
    - 说明：根据实际情况，BOOT_MEDIA 可选 spi、nand 或 emmc

6. 编译生成的镜像位于 osdrv/pub/ 目录，包括：
    - boot_image.bin 为 bootloader 镜像
    - u-boot-ss928v100.bin 为 u-boot 镜像（OTP 使能快速启动时使用）
    - uImage_ss928v100 为 Linux 内核镜像

## 注意事项
1. 在windows下复制源码包时，linux下的可执行文件可能变为非可执行文件，导致无法编译使用；u-boot或内核下编译后，会有很多符号链接文件，在windows下复制这些源码包, 会使源码包变的巨大，因为linux下的符号链接文件变为windows下实实在在的文件，导致源码包膨胀。
因此**使用时请注意不要在windows下复制源代码包**。

2. SS928V100具有浮点运算单元和neon。文件系统中的库是采用软浮点和neon编译而成，因此请用户注意，所有SS928V100板端代码编译时需要在Makefile里面添加选项`mcpu=cortex-a55`。

