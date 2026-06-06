# ModelZoo NPU推理案例开发指导

本文档介绍如何使用ModelZoo中的YOLOv9s模型，在Hi3403V100芯片配套的hispark_aifly开发板的鸿蒙系统上，通过NPU进行推理。以YOLOv9s目标检测模型为例，说明从代码拉取、交叉编译、部署到运行推理的完整流程。

## 1. 下载代码

```bash
git clone https://gitee.com/HiSpark/modelzoo.git
```

## 2. 准备编译环境

> **前置条件**：需要确保**已经编译过hispark_aifly**，需要用到编译产物中的`sysroot`目录和鸿蒙提供的交叉编译工具链。

设置环境变量：

```bash
# 鸿蒙系统sysroot路径
export SYSROOT_PATH=/data/oh5.1.0-3403/out/hispark_aifly/ipcamera_hispark_aifly_linux/sysroot

# 鸿蒙交叉编译工具链路径
export PATH=$PATH:/data/oh5.1.0-3403/prebuilts/clang/ohos/linux-x86_64/llvm/bin

# NPU相关头文件和库路径
export NPU_INCLUDE_PATH=$SYSROOT_PATH/../sdk_linux/src_tmp/smp/a55_linux/mpp/out/include
export NPU_LIB_PATH=$SYSROOT_PATH/../sdk_linux/src_tmp/smp/a55_linux/mpp/out/lib
export NPU_HOST_LIB=$SYSROOT_PATH/../rootfs/usr/lib
```

> **注意**：`NPU_INCLUDE_PATH`和`NPU_LIB_PATH`指向SDK中NPU MPP模块的输出目录，包含NPU推理所需的头文件和库文件。请根据实际SDK安装路径进行调整。

## 3. 编译步骤

```bash
cd modelzoo/samples/samples_GPL/built-in/yolov9s
mkdir -p build
cd build/
```

创建protobuf-c的符号链接：

```bash
cd $NPU_HOST_LIB && ln -s libprotobuf-c.so.1 libprotobuf-c.so && cd -
```

执行cmake配置和编译：

```bash
cmake ../src -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/data/modelzoo/samples/common/cmake/toolchain_aarch64_ohos.cmake \
    -DSOC_VERSION=SS928V100

make -j$(nproc)
```

> **参数说明**：
> - `CMAKE_TOOLCHAIN_FILE`：指定鸿蒙系统的aarch64交叉编译工具链文件
> - `SOC_VERSION=SS928V100`：指定目标芯片版本为SS928V100（即Hi3403V100）

## 4. 编译产物说明

编译完成后，可执行文件生成在 `out` 目录下：

```bash
ls ../out/main
```

目录结构：

```
modelzoo/samples/samples_GPL/built-in/yolov9s/
|-- src/          # 源代码
|-- model/        # 模型文件（.om格式）
|-- data/         # 输入数据（file_list.json等）
|-- out/          # 编译产物
|   `-- main      # 可执行文件
`-- build/        # 构建目录
```

## 5. 获取模型

参考：https://gitee.com/HiSpark/modelzoo/blob/master/samples/samples_GPL/built-in/yolov9s/README.md#%E8%8E%B7%E5%8F%96om%E6%A8%A1%E5%9E%8B%E6%96%87%E4%BB%B6


## 6. 部署到开发板

### 6.1 准备共享目录

需要在PC端通过 `nfs-kernel-server` 工具创建共享文件夹：

```bash
# 安装nfs服务
sudo apt install nfs-kernel-server -y

# 配置共享目录
sudo vim /etc/exports
# 写入以下内容
/share *(rw,sync,no_root_squash,no_subtree_check,insecure)

# 重启nfs服务
sudo exportfs -arv
sudo systemctl restart nfs-server
```

### 6.2 部署文件

将 `modelzoo` 文件夹上传到共享目录：

```bash
cp -r modelzoo /data/share/
```

### 6.3 在开发板上挂载并运行

```bash
# 创建挂载点
mkdir -p /share

# 挂载共享目录（根据实际IP修改）
mount -t nfs -o nolock,addr=192.168.31.2 192.168.31.2:/data/share /share

# 设置库路径
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/share/modelzoo/samples/samples_GPL/opensource/opencv/lib:/usr/lib/"

# 替换libopencv_world.so.412为ohos版本
cp /share/modelzoo/samples/samples_GPL/opensource/opencv/lib/aarch64_ohos/libopencv_world.so.412 \
   /share/modelzoo/samples/samples_GPL/opensource/opencv/lib/libopencv_world.so.412

# 进入可执行文件目录
cd /share/modelzoo/samples/samples_GPL/built-in/yolov9s/out

# 运行NPU推理
./main --model ../model/yolov9s.om --input ../data/file_list.json
```

> **注意**：OpenCV库需要替换为鸿蒙系统编译的版本（`aarch64_ohos`目录下），否则运行时会出现动态库不兼容的问题。

## 7. 调试

如果推理运行出现问题，可以使用GDB进行调试：

```bash
/share/gdb ./main
(gdb) b osal_opendev
(gdb) run --model ../model/yolov9s.om --input ../data/file_list.json
```

> **说明**：`osal_opendev` 是NPU设备打开的入口函数，在此处设置断点可以排查NPU设备初始化相关的问题。也可以根据需要设置其他断点。

## 8. 注意事项

1. **交叉编译环境**：确保使用鸿蒙提供的工具链，`SYSROOT_PATH` 和 `PATH` 需要正确设置
2. **NPU库路径**：`NPU_INCLUDE_PATH` 和 `NPU_LIB_PATH` 需要指向SDK中MPP模块的正确输出目录
3. **OpenCV库替换**：开发板上运行前必须将OpenCV库替换为鸿蒙版本（`aarch64_ohos`），否则会出现动态库不兼容
4. **库路径设置**：在开发板上运行时，需要通过 `LD_LIBRARY_PATH` 指定OpenCV库和系统库的路径
5. **网络挂载**：开发板需要通过网络NFS挂载共享目录，确保网络连通性
6. **SOC版本**：编译时 `SOC_VERSION` 必须与目标芯片一致，当前为 `SS928V100`

## 9. 常见问题

### Q1: 编译时提示找不到sysroot或工具链

**A:** 确保环境变量设置正确，且鸿蒙系统已经编译过：
```bash
echo $SYSROOT_PATH
# 应该输出：/data/oh5.1.0-3403/out/hispark_aifly/ipcamera_hispark_aifly_linux/sysroot
ls $SYSROOT_PATH
# 确认目录存在且包含必要的头文件和库
```

### Q2: 运行时提示找不到共享库

**A:** 确保设置了正确的 `LD_LIBRARY_PATH`，并且已替换OpenCV库：
```bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/share/modelzoo/samples/samples_GPL/opensource/opencv/lib:/usr/lib/"
```

### Q3: 运行时报错Signal 11

**A:** 编译/etc/init.cfg，注释掉下面几行内容，重启后重新验证。
```bash
"start media_server",
"start wms_server",
```

### Q4: NFS挂载失败

**A:** 检查以下几点：
- PC端的nfs服务是否正常运行：`sudo systemctl status nfs-server`
- PC端的 `/etc/exports` 配置是否正确
- 开发板和PC之间的网络是否连通
- 防火墙是否阻止了NFS连接

### Q5: OpenCV库不兼容

**A:** 确保已将OpenCV库替换为鸿蒙版本：
```bash
cp /share/modelzoo/samples/samples_GPL/opensource/opencv/lib/aarch64_ohos/libopencv_world.so.412 \
   /share/modelzoo/samples/samples_GPL/opensource/opencv/lib/libopencv_world.so.412
```
