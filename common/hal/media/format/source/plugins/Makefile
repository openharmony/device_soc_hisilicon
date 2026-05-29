LOCAL_DIR := $(shell pwd)

COMPILE_ROOT := $(LOCAL_DIR)

MODNAME :=
MODNAME +=  $(shell test -d hisi_raw_demuxer && echo hisi_raw_demuxer)
MODNAME +=  $(shell test -d hisi_recorder_muxer && echo hisi_recorder_muxer)
MODNAME +=  $(shell test -d hisi_m4a_muxer && echo hisi_m4a_muxer)

SUBDIRS =$(MODNAME)

.PHONY: all clean

all:
	for MOD in $(MODNAME);do\
		pushd $(LOCAL_DIR);cd $(COMPILE_ROOT)/$$MOD ;$(MAKE) $(MFLAGS) || exit "$$?" ; popd ;done

clean:
	@for MOD in $(MODNAME);do\
		pushd $(LOCAL_DIR);cd $(COMPILE_ROOT)/$$MOD ;$(MAKE) $(MFLAGS) clean; popd ;done

