#/*
# * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# * http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# */

TOOL_DIR := $(shell pwd)/../tool/
LAN_DIR = $(shell pwd)/../res/lan
LAN_DIR_EXIST = $(shell if [ -d $(LAN_DIR) ]; then echo "exist"; else echo "notexist"; fi;)

all: build
build: lan_dir
	rm -rf higv.bin
	rm -rf *.c *.h
	rm -rf xml2bin
	cp $(TOOL_DIR)/xml2bin  ./
	chmod +x xml2bin
	./xml2bin -b ./higv.bin -e ./ -t c -x ./ -l ../res/lan
	chmod +x ../res/lan/*.lang
	cp *.bin ../ -rf
	cp higv*.c ../ -rf
	cp higv*.h ../ -rf
clean:
	rm -rf *.c *.h
	rm -rf higv.bin
	rm -rf xml2bin
	rm -rf ../res/lan/*.lang

.PHONY: lan_dir
lan_dir:
	if [ $(LAN_DIR_EXIST) = notexist ]; then  mkdir $(LAN_DIR);fi
