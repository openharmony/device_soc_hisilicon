1. built-in.sh 文件的 KERN_DIR 必须指定内核目录
2. klad 模块依赖 osal, sysconfig, base, sys 等基础 ko, 需适配后才能将 klad 的 *.c 文件拷贝到指定目录
