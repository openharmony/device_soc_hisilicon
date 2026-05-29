LOCAL_DIR := $(shell pwd)
MEDIA_BASE_DIR := $(LOCAL_DIR)/../../..
include $(MEDIA_BASE_DIR)/build/base.mak
COMPILE_ROOT := $(LOCAL_DIR)
MODNAME :=
ifeq ($(strip $(CFG_CHIP_TYPE)),$(filter $(CFG_CHIP_TYPE),hi3518ev300 hi3516dv300 hi3403v100))
# $(warning "the value of CFG_CHIP_TYPE is $(CFG_CHIP_TYPE)")
MODNAME += $(shell test -d hi35xx && echo hi35xx)
MODNAME += $(shell test -d usb && echo usb)
else ifeq ($(strip $(CFG_CHIP_TYPE)),$(filter $(CFG_CHIP_TYPE),hi3862v100))
MODNAME +=  $(shell test -d iot/audio_input_port_plugin && echo iot/audio_input_port_plugin)
MODNAME +=  $(shell test -d iot/audio_output_port_plugin && echo iot/audio_output_port_plugin)
endif
SUBDIRS =$(MODNAME)


all: subdirs

.PHONY: subdirs $(SUBDIRS) clean
subdirs: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) $(MFLAGS) -C $(COMPILE_ROOT)/$@  || exit "$$?"

clean:
	@for MOD in $(MODNAME);do\
		cd $(COMPILE_ROOT)/$$MOD ;$(MAKE) $(MFLAGS) clean;done


install:
	@for MOD in $(MODNAME);do\
		cd $(COMPILE_ROOT)/$$MOD ;$(MAKE) $(MFLAGS) install || exit 1;done
