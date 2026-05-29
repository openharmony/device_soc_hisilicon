# osdrv Top-level Makefile Usage Instructions

## Compilation Environment Configuration

1. This readme is for ss928v100;

2. The bootloader is compiled using the aarch64-openeuler-linux-gnu 64-bit toolchain. You need to download the openEuler Embedded Toolchains from https://gitee.com/openeuler/yocto-meta-openeuler/releases. Please refer to the following steps:
    ```bash
    wget https://gitee.com/openeuler/yocto-meta-openeuler/releases/download/toolchains-v0.1.8/1_openeuler_gcc_arm64le.tar.gz
    wget https://gitee.com/openeuler/yocto-meta-openeuler/releases/download/toolchains-v0.1.8/2_openeuler_gcc_arm64le.tar.gz
    wget https://gitee.com/openeuler/yocto-meta-openeuler/releases/download/toolchains-v0.1.8/3_openeuler_gcc_arm64le.tar.gz
    wget https://gitee.com/openeuler/yocto-meta-openeuler/releases/download/toolchains-v0.1.8/4_openeuler_gcc_arm64le.tar.gz
    cat 1_openeuler_gcc_arm64le.tar.gz 2_openeuler_gcc_arm64le.tar.gz 3_openeuler_gcc_arm64le.tar.gz 4_openeuler_gcc_arm64le.tar.gz > openeuler_gcc_arm64le.tar.gz
    tar zxf openeuler_gcc_arm64le.tar.gz
    ```

3. Before use, you need to add aarch64-openeuler-linux-gnu to the environment variable. Note to replace with the actual path.
    ```bash
    export PATH=$PATH:/Download_Path/openeuler_gcc_arm64le/bin
    ```

4. Before use, you need to restore the tools/pc/uboot_tools/regbin-v1.0.2.tgz tool from multiple compressed packages using the following command:
    ```bash
    cd tools/pc/uboot_tools
    cat regbin-v1.0.2.tar.gz.0* > regbin-v1.0.2.tar.gz
    tar zxf regbin-v1.0.2.tar.gz
    ```

5. Before use, please confirm that the `u-boot-2020.01.tar.bz2` file exists at the following path:
    ```bash
    ls sdk_linux/open_source/u-boot/u-boot-2020.01.tar.bz2
    # If it doesn't exist
    cd sdk_linux/open_source/u-boot
    wget https://ftp.denx.de/pub/u-boot/u-boot-2020.01.tar.bz2
    ```

## Compilation Command Reference

1. Compile the entire osdrv directory:
    ```bash
    make all
    # Default compilation
    make LLVM=1 BOOT_MEDIA=emmc CHIP=ss928v100 all
    ```

    Parameter description:

    - CHIP: Can be ss928v100 or ss927v100, default is ss928v100.

    - BOOT_MEDIA: Select spi for spi nor or spi nand boot; select nand for parallel nand boot; select emmc for emmc boot.

2. Clean the compilation files of the entire osdrv directory:
    ```bash
    make clean
    ```

3. Thoroughly clean all intermediate compilation files of the entire osdrv directory:
    ```bash
    make distclean
    ```

4. Compile kernel image separately, execute the following command in the osdrv directory:
    ```bash
    make LLVM=1 BOOT_MEDIA=spi atf -j 20
    ```
    - Note: According to actual situation, BOOT_MEDIA can be spi, nand, or emmc

5. Compile fast boot uboot or non-secure boot Boot Image separately, execute the following command in the osdrv directory:
    ```bash
    make BOOT_MEDIA=spi gslboot_build -j 20
    ```
    - Note: According to actual situation, BOOT_MEDIA can be spi, nand, or emmc

6. The compiled images are located in the osdrv/pub/ directory, including:
    - boot_image.bin is the bootloader image
    - u-boot-ss928v100.bin is the u-boot image (used when OTP enables fast boot)
    - uImage_ss928v100 is the Linux kernel image

## Notes

1. When copying source code packages under Windows, executable files in Linux may become non-executable files, causing compilation failure; after compilation under u-boot or kernel, there will be many symbolic link files. Copying these source code packages under Windows will make the source code package extremely large because symbolic link files in Linux become actual files in Windows, causing the source code package to bloat.
Therefore, **please be careful not to copy source code packages under Windows**.

2. SS928V100 has floating-point processing unit and neon. Libraries in the file system are compiled with soft-float and neon, so please note that when compiling all SS928V100 board-side code, you need to add the option `mcpu=cortex-a55` in the Makefile.