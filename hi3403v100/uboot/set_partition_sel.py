# Copyright (c) 2026 Nanjing Qinuo Information Technology Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""修改 emmc_burn_table.xml 中指定分区的 Sel 值。

用法:
    python set_partition_sel.py                                    # 等同于 emmc_burn_table.xml fastboot 0
    python set_partition_sel.py <xml_file> [PartitionName] [Sel]
    python set_partition_sel.py <PartitionName> <Sel>
"""

import sys
import os
import re

DEFAULT_XML = os.path.join(os.path.dirname(os.path.abspath(__file__)), "emmc_burn_table.xml")
DEFAULT_PARTITION = "fastboot"
DEFAULT_SEL = "0"


def _is_xml_file(path):
    """判断参数是否像 XML 文件路径（以 .xml 结尾或文件已存在）。"""
    return path.endswith(".xml") or os.path.exists(path)


def main():
    # 解析参数
    args = sys.argv[1:]
    xml_file = DEFAULT_XML
    partition_name = DEFAULT_PARTITION
    sel_value = DEFAULT_SEL

    if len(args) == 0:
        pass  # 全部使用默认值
    elif len(args) == 1:
        if _is_xml_file(args[0]):
            xml_file = args[0]
        else:
            partition_name = args[0]
    elif len(args) == 2:
        if _is_xml_file(args[0]):
            xml_file = args[0]
            partition_name = args[1]
        else:
            partition_name = args[0]
            sel_value = args[1]
    elif len(args) == 3:
        xml_file, partition_name, sel_value = args
    else:
        print(f"用法: python {sys.argv[0]} [xml_file] [PartitionName] [Sel]")
        print(f"示例: python {sys.argv[0]}                            # emmc_burn_table.xml fastboot 0")
        print(f"      python {sys.argv[0]} emmc_burn_table.xml fastboot 1")
        print(f"      python {sys.argv[0]} fastboot 1                 # fastboot 1")
        print(f"      python {sys.argv[0]} kernel 0                   # kernel 0")
        sys.exit(1)

    # 校验 Sel 值
    if sel_value not in ("0", "1"):
        print(f"错误: Sel 值必须为 0 或 1，当前值为 '{sel_value}'")
        sys.exit(1)

    # 读取 XML
    if not os.path.exists(xml_file):
        print(f"错误: 找不到文件 '{xml_file}'")
        sys.exit(1)

    with open(xml_file, "r", encoding="utf-8") as f:
        content = f.read()

    # 匹配 PartitionName="xxx" 所在行，替换 Sel 值
    pattern = rf'(Sel=")[01](".*PartitionName="{re.escape(partition_name)}")'
    new_content, count = re.subn(pattern, rf"\g<1>{sel_value}\g<2>", content)

    if count == 0:
        print(f"错误: 未找到 PartitionName='{partition_name}' 的分区")
        sys.exit(1)

    with open(xml_file, "w", encoding="utf-8") as f:
        f.write(new_content)

    print(f"已将 [{partition_name}] 的 Sel 修改为 {sel_value}")


if __name__ == "__main__":
    main()
