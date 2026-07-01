LOCAL_DIR := $(shell pwd)

COMPILE_ROOT := $(LOCAL_DIR)

MODNAME :=
MODNAME +=  $(shell test -f audio/source/Makefile && echo audio/source)
MODNAME +=  $(shell test -f codec/source/Makefile && echo codec/source)
MODNAME +=  $(shell test -f format/source/Makefile && echo format/source)
#MODNAME +=  $(shell test -f video_display/source/Makefile && echo video_display/source)

SUBDIRS =$(MODNAME)

.PHONY: all clean

#all: common_hal
#	for MOD in $(MODNAME);do \
#		pushd $(LOCAL_DIR); cd $(COMPILE_ROOT)/$$MOD ;$(MAKE) $(MFLAGS); popd ;done

all: subdirs

subdirs: $(SUBDIRS)
$(SUBDIRS): common_hal
	$(MAKE) $(MFLAGS) -C $(COMPILE_ROOT)/$@  VERBOSE=1

common_hal:
	$(LOCAL_DIR); cd $(COMPILE_ROOT)/common/source ;$(MAKE) $(MFLAGS)  VERBOSE=1;

clean:
	@for MOD in $(MODNAME);do\
		cd $(COMPILE_ROOT)/$$MOD ;$(MAKE) $(MFLAGS) clean;done
	pushd $(LOCAL_DIR); cd $(COMPILE_ROOT)/common/source ;$(MAKE) $(MFLAGS) clean; popd

