LOCAL_DIR := $(shell pwd)

COMPILE_ROOT := $(LOCAL_DIR)



.PHONY: all clean hal component

all: hal

hal: component
	make -C $(COMPILE_ROOT)/../hal VERBOSE=1

component:
	make -C $(COMPILE_ROOT)/../component VERBOSE=1

clean:
	@cd $(COMPILE_ROOT)/../hal ;$(MAKE) $(MFLAGS) clean; cd -
	@cd $(COMPILE_ROOT)/../component;$(MAKE) $(MFLAGS) clean; cd -
