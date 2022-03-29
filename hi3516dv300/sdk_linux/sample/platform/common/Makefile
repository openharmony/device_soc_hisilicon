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
param_file=$(realpath $(dir $(firstword $(MAKEFILE_LIST))))/../Makefile.param
include $(param_file)
.PHONY : clean all
all:$(COMM_OBJ)
clean:
	@rm $(COMM_OBJ) -f
%.o:%.c
	@$(CC) $(COMM_INC) $(CFLAGS) -c $< -o $@
