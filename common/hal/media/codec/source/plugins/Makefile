LOCAL_DIR := $(shell pwd)

MEDIA_BASE_DIR := $(LOCAL_DIR)/../../..
include $(MEDIA_BASE_DIR)/build/cfg.mak

COMPILE_ROOT := $(LOCAL_DIR)

MODNAME :=
MODNAME +=  $(shell test -d aac && echo aac)
MODNAME +=  $(shell test -d audio_encoder && echo audio_encoder)
MODNAME +=  $(shell test -d video_encoder && echo video_encoder)

SUBDIRS =$(MODNAME)

.PHONY: all clean

all:
	for MOD in $(MODNAME);do\
		pushd $(LOCAL_DIR);cd $(COMPILE_ROOT)/$$MOD ;$(MAKE) $(MFLAGS) || exit "$$?"; popd ;done
#all: subdirs

#subdirs: $(SUBDIRS)
#$(SUBDIRS):	
	#$(MAKE) $(MFLAGS) -C $(COMPILE_ROOT)/$@

clean:
	@for MOD in $(MODNAME);do\
		pushd $(LOCAL_DIR);cd $(COMPILE_ROOT)/$$MOD ;$(MAKE) $(MFLAGS) clean; popd ;done

