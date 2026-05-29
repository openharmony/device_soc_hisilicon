LOCAL_DIR := $(shell pwd)

COMPILE_ROOT := $(LOCAL_DIR)

MODNAME :=
MODNAME +=  $(shell test -d hi35xx && echo hi35xx)

SUBDIRS =$(MODNAME)

.PHONY: all clean

all:
	for MOD in $(MODNAME);do\
		pushd $(LOCAL_DIR);cd $(COMPILE_ROOT)/$$MOD ;$(MAKE) $(MFLAGS)  || exit "$$?"; popd ;done

clean:
	@for MOD in $(MODNAME);do\
		pushd $(LOCAL_DIR);cd $(COMPILE_ROOT)/$$MOD ;$(MAKE) $(MFLAGS) clean; popd ;done

