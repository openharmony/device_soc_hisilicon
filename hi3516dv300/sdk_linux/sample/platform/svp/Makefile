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
rel:
	@cd ive;     make
	@cd nnie;    make
ifeq ($(CONFIG_HI_SVP_RUNTIME),y)
	@cd hirt; make
endif
clean:
	@cd ive;     make clean
	@cd nnie;    make clean
ifeq ($(CONFIG_HI_SVP_RUNTIME),y)
	@cd hirt;    make clean
endif
cleanall:
	@cd ive;     make clean;   make cleanstream
	@cd nnie;    make clean;   make cleanstream
ifeq ($(CONFIG_HI_SVP_RUNTIME),y)
	@cd hirt;    make clean
endif
