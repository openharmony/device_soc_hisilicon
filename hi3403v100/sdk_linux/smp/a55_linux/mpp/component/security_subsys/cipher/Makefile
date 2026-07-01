ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../../Makefile.param
        include $(PARAM_FILE)
endif

ifneq ($(filter $(OT_ARCH), ss528v100 ss524v100), )
CIPHER_VER_DIR := $(PWD)/v2
else
CIPHER_VER_DIR := $(PWD)/v3
endif

.PHONY: all clean

all:
	@test -z "$(CIPHER_VER_DIR)" || (cd $(CIPHER_VER_DIR) && make)

clean:
	@test -z "$(CIPHER_VER_DIR)" || (cd $(CIPHER_VER_DIR) && make clean)
