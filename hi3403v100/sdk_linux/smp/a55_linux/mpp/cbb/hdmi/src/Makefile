# $(ROOT)/mpp/component/hdmi/src/Makefile

ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../../Makefile.param
    include $(CBB_PARAM_FILE)
endif

.PHONY: all clean

ifeq ($(CONFIG_OT_HDMI_SUPPORT), y)
HDMI_PATH:=$(PWD)
HDMI_INC_PATH:=$(HDMI_PATH)/include
export HDMI_PATH HDMI_INC_PATH
all:
	@cd $(PWD)/mpi;make
	@cd $(PWD)/mkp;make
clean:
	@cd $(PWD)/mpi;make clean
	@cd $(PWD)/mkp;make clean
else
all:
	@echo "hdmi module not compile"
clean:
	@echo "hdmi module don't need clean"
endif
