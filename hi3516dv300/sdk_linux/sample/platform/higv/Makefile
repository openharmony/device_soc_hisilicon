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
export OSTYPE := Linux
WORKDIR = $(shell pwd)
include ./config.mk

SEC_LFLAGS += -pie
CFLAGS += $(SEC_LFLAGS)

SRC := $(notdir $(wildcard *.c))
OBJ := $(patsubst %.c,%.o,$(SRC))

EXE = $(notdir $(WORKDIR))

DEPEND = $(notdir $(wildcard .depend))

.PHONY: all obj exe mkxml

ifeq ($(build), )
all: tips mkxml $(EXE)
else
all: tips mkxml $(EXE)
endif

mkxml:
	cd xml; make
clxml:
	cd xml; make clean
tips:
	@echo -e  $(TIPS)

obj: $(OBJ)
exe: $(OBJ) $(EXE)

.depend: $(SRC)
	$(CC) $(CFLAGS) -w -MM $^ > $@

$(OBJ):%.o:%.c
	echo "  CXX      $@"
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	echo "  LD      $@"
	$(CC) $(CFLAGS) $(LFLAGS) -fPIE -pie $^ $(LDFLAGS) $(LIBS) -o $@
	$(STRIP) $(EXE)

clean: clxml
	echo "  CLEAN"
	$(RM) $(EXE) $(OBJ) $(LIB) $(DEPEND)
	$(RM) higv.bin

ifneq ($(DEPEND),)
include .depend
endif
