# MindSpore Lite 运行在OpenHarmony small系统的开发指南

本文档介绍如何编译一个能够在Hi3403V100芯片配套的hispark_aifly开发板的OpenHarmony小型系统上运行的MindSpore Lite应用，它将用于在设备上运行推理任务。

## 1. 下载代码

```bash
git clone -b r2.9 https://atomgit.com/mindspore/mindspore-lite.git
```

> **注意**：r2.9版本的OHOS编译时有一些报错需要额外处理，如果解决不了，可以使用下面的版本来编译：
> ```bash
> git clone https://atomgit.com/mindspore/mindspore-lite.git
> git fetch https://atomgit.com/mindspore/mindspore-lite.git +refs/merge-requests/817/head:pr_817
> git checkout pr_817
> ```

## 2. 准备编译环境

> **前置条件**：需要确保**已经编译过Hi3403V100的小型系统**，需要用到`ohos-sdk`目录下的文件。
> 
> 如果`out/preloader` 目录下没有`ohos-sdk`文件夹，需要使用下面的命令编译出ohos-sdk
> ```bash
> ./build.sh --product-name ipcamera_hispark_aifly_linux --ccache --prebuilt-sdk sdk_platform=default
> ```
> 或者从[每日构建](https://ci.openharmony.cn/workbench/cicd/dailybuild/dailylist)中下载对应的`ohos-sdk-full_6.1-LTS`

设置环境变量：

```bash
export OHOS_NDK=/data/ohos-sdk/linux/native
export CC=$OHOS_NDK/llvm/bin/aarch64-unknown-linux-ohos-clang
export CXX=$OHOS_NDK/llvm/bin/aarch64-unknown-linux-ohos-clang++
export TOOLCHAIN_NAME=ohos
```

更多编译选项可以参考：https://www.mindspore.cn/lite/docs/zh-CN/stable/use/build.html

## 3. 编译步骤

```bash
cd mindspore-lite

# 板端只进行推理，因此关闭编译训练相关的代码，加快编译速度
export MSLITE_ENABLE_TRAIN=off

# 开始编译
bash build.sh -I arm64 -j16
```

## 4. 编译产物说明

编译完成后，在 `output` 目录下会生成 `mindspore-lite-2.9.0-ohos-aarch64.tar.gz` 压缩包。

解压后的目录结构：

```
mindspore-lite-2.9.0-ohos-aarch64/
|-- runtime
|   |-- include
|   `-- lib
`-- tools
    `-- benchmark
```

## 5. 部署到开发板

### 5.1 准备共享目录

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

### 5.2 部署文件

1. 将编译产物解压到共享目录：

	```bash
	# 解压到共享目录
	tar -xzf output/mindspore-lite-2.9.0-ohos-aarch64.tar.gz -C /data/share/
	```

2. 还需要复制`libhilog_ndk.z.so`到共享目录，这个库的获取方式有两种:
	```bash
    # 使用ohos-sdk内的库
    cd $OHOS_NDK
    cp ./sysroot/usr/lib/aarch64-linux-ohos/libhilog_ndk.z.so /data/share/

    # 使用ohos-sdk内的库
    cd /data/oh5.1.0-3403
    cp ./prebuilts/ohos-sdk/linux/18/native/sysroot/usr/lib/aarch64-linux-ohos/libhilog_ndk.z.so /data/share/
    ```

3. 下载测试模型和输入输出文件
    ```bash
    cd /data/share
	wget https://download.mindspore.cn/model_zoo/official/lite/quick_start/mobilenetv2.ms
	wget https://download.mindspore.cn/model_zoo/official/lite/quick_start/input.bin
	wget https://download.mindspore.cn/model_zoo/official/lite/quick_start/output.txt
    ```

### 5.3 在开发板上挂载并测试

```bash
# 创建挂载点
mkdir -p /share

# 设置ip地址（根据实际情况设置）
ifconfig eth0 192.168.31.3

# 挂载共享目录（根据实际IP修改）
mount -t nfs -o nolock,addr=192.168.31.2 192.168.31.2:/data/share /share

# 设置库路径
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/share/mindspore-lite-2.9.0-ohos-aarch64/runtime/lib

# 添加libhilog_ndk.z.so库到系统里面
mkdir /lib64
cp /share/libhilog_ndk.z.so /lib64

# 进入benchmark工具目录
cd /share/tools/benchmark

# 运行benchmark测试
./benchmark --modelFile=/share/mobilenetv2.ms
```

### 5.4 运行结果

```
Model = mobilenetv2.ms, NumThreads = 2, MinRunTime = 98.071999 ms, MaxRuntime = 100.181000 ms, AvgRunTime = 98.552002 ms
Run Benchmark mobilenetv2.ms Success.
```

## 6. 注意事项

1. **交叉编译环境**：确保使用OpenHarmony提供的工具链，环境变量 `OHOS_NDK` 需要正确设置
2. **训练代码关闭**：板端只进行推理，设置 `MSLITE_ENABLE_TRAIN=off` 可以加快编译速度
3. **库路径设置**：在开发板上运行时，需要通过 `LD_LIBRARY_PATH` 指定运行时库的路径
4. **网络挂载**：开发板需要通过网络NFS挂载共享目录，确保网络连通性

## 7. 获取模型

MindSpore Lite 使用 `.ms` 格式的模型文件，可以通过以下方式获取：

**转换现有模型**：使用MindSpore Lite提供的转换工具将其它格式的模型转换为.ms格式
   - 参考文档：https://www.mindspore.cn/lite/docs/zh-CN/stable/converter/converter_tool.html

## 8. Benchmark工具使用说明

Benchmark工具是MindSpore Lite提供的性能测试工具，用于对MindSpore Lite模型进行基准测试。它不仅可以对MindSpore Lite模型前向推理执行耗时进行定量分析（性能），还可以通过指定模型输出进行可对比的误差分析（精度）。

### 基本用法

```bash
./benchmark --modelFile=<model.ms> [options]
```

### 参数说明

参考文档：https://www.mindspore.cn/lite/docs/zh-CN/stable/tools/benchmark_tool.html

## 9. 常见问题

### Q1: 编译时提示找不到ohos-sdk

**A:** 确保环境变量 `OHOS_NDK` 设置正确，路径应该指向ohos-sdk的native目录：
```bash
echo $OHOS_NDK
# 应该输出：/data/ohos-sdk/linux/native
```

### Q2: 运行时提示找不到共享库

**A:** 确保设置了正确的 `LD_LIBRARY_PATH`：
```bash
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/share/mindspore-lite-2.9.0-ohos-aarch64/runtime/lib
```

### Q3: NFS挂载失败

**A:** 检查以下几点：
- PC端的nfs服务是否正常运行：`sudo systemctl status nfs-server`
- PC端的 `/etc/exports` 配置是否正确
- 开发板和PC之间的网络是否连通
- 防火墙是否阻止了NFS连接
