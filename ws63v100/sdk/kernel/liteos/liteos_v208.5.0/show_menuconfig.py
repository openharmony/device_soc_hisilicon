# encoding=utf-8
# ============================================================================
# @brief    Show the menuconfig of LiteOS

# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
# ============================================================================
import os, sys
from kconfiglib import Kconfig
from menuconfig import menuconfig

def mconf_set_env(style, conf, header):
    """
    Set Kconfig Env
    """
    os.environ["MENUCONFIG_STYLE"] = style
    os.environ["KCONFIG_CONFIG"] = conf
    os.environ["KCONFIG_CONFIG_HEADER"] = header
    os.environ["KCONFIG_AUTOHEADER"] = os.path.join("targets", "menuconfig.h")
    os.environ["CONFIG_"] = ""
    os.environ["KCONFIG_ARCH_PATH"] = "arch"
    os.environ["KCONFIG_KERNEL_PATH"] = "kernel"

def mconfig(config):
    kconfig = os.path.join("build", "menuconfig", "config.in")
    display_style = "default selection=fg:white,bg:blue"
    target_conf = config
    header = "# Generated by LiteOS Kconfig Tool"
    mconf_set_env(display_style, target_conf, header)
    kconf = Kconfig(filename=kconfig)
    menuconfig(kconf)   # menuconfig
    kconf.write_autoconf()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Parameters invalid! You should type like this: python(3) show_menuconfig.py ws63")
        exit(1)

    os.chdir("Huawei_LiteOS")
    mconfig("tools/build/config/" + sys.argv[1] + ".config")