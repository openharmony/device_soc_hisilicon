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

###########AMP SRCS & INC ############
AMP_CLIENT_SRCS := $(wildcard $(PWD)/$(ARM_ARCH)/client/*.c)
AMP_CLIENT_SRCS += $(wildcard $(PWD)/*.c)
AMP_SERVER_SRCS := $(wildcard $(PWD)/$(ARM_ARCH)/server/*.c)
AMP_INC += -I$(PWD)/
AMP_INC += -I$(PWD)/$(ARM_ARCH)/include

###########SMP SRCS & INC ############
SMP_SRCS := $(wildcard $(PWD)/$(ARM_ARCH)/*.c)
SMP_SRCS += $(wildcard $(PWD)/*.c)
SMP_INC := -I$(PWD)/

ifeq ($(ARM_ARCH), amp)

ifeq ($(OSTYPE), liteos)
TARGET := sample_vio_server
else
TARGET := sample_vio
endif

else
TARGET := sample_vio
endif

TARGET_PATH := $(PWD)/$(ARM_ARCH)

include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak