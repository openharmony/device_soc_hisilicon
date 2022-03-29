#/*
# * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *     http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# */

include ../Makefile.param

CFLAGS += -I$(PWD)/sample

SMP_SRCS := $(wildcard *.c)
SMP_SRCS += $(wildcard ./sample/*.c)
SMP_SRCS += $(wildcard $(PWD)/../common/*.c)
TARGET := sample_ive_main

TARGET_PATH := $(PWD)

# compile linux or HuaweiLite
include $(PWD)/../../$(ARM_ARCH)_$(OSTYPE).mak
