# $(ROOT)/mpp/component/securec/Makefile

ifeq ($(PARAM_FILE), ) 
    PARAM_FILE:=../../Makefile.param
    include $(PARAM_FILE)
endif


.PHONY:clean all
all:
	@echo -e "\e[0;32;1m--Compiling 'securec'... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- OT_ARCH=$(OT_ARCH), OT_CHIP=$(OT_CHIP), CVER=$(CVER), OT_DBG=$(OT_DBG)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@echo -e "\e[0m"
	@cd src; make

# --------------clean -----------------
clean:
	@cd src; make clean

