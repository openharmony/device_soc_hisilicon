uboot 编译指导

openeuler_gcc_arm64le.tar.gz u-boot-2020.01.tar.bz2 regbin-v1.0.2.tgz 文件获取参考 osdrv 目录下的readme_cn.md

1.进入osdrv目录下执行：tar -zxf openeuler_gcc_arm64le.tar.gz 解压交叉编译链
2.配置交叉编译：在osdrv 下执行 export PATH="$PWD/openeuler_gcc_arm64le/bin:$PATH" 配置编译
3.make BOOT_MEDIA=emmc gslboot_build -j 20 执行后在pub目录下会生成 boot_image.bin 这个就是 boot_image_4GB.bin
4.在osdrv目录下使用 mkenvimage 生成 uboot_env_4GB.bin，并将其输出到对应的 pub 镜像目录。
5.如果要生成fip.bin文件，需要执行 make all -j10 ，执行完成后修改open_source/trusted-firmware-a/arm-trusted-firmware-2.2/plat/vendor/ss928v100/platform.mk ：71 NEED_BL33		:= no 后 make clean 后再执行 make all -j10 ，在 open_source/trusted-firmware-a/arm-trusted-firmware-2.2/build/ss928v100/release/ 目录下生成 fip.bin

注：不想污染当前终端环境时，直接给单次编译命令设置：PATH="$PWD/openeuler_gcc_arm64le/bin:$PATH" make xxxx make后边跟命令