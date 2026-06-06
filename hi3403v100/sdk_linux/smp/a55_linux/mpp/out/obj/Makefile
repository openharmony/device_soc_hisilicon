#*******************************************************************************
#The rule of make Linux kernel module
#*******************************************************************************
ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

INIT_ROOT = $(REL_DIR)/init

EXTRA_CFLAGS += $(MPP_CFLAGS)
EXTRA_CFLAGS += -I$(REL_INC)
EXTRA_CFLAGS += -I$(INIT_ROOT)/include
EXTRA_CFLAGS += -I$(INIT_ROOT)/include/npu
EXTRA_CFLAGS += -I$(INIT_ROOT)/include/svp_npu
EXTRA_CFLAGS += -I$(OSAL_ROOT)/include
EXTRA_CFLAGS += -I$(REL_DIR)/../component/gfbg/drv/include

default:
	@echo -e "\e[0;32;1m--Compiling '$(TARGET)'... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS) REL_INC=$(REL_INC)
	@echo ---- OT_SUBARCH=$(OT_SUBARCH), CVER=$(CVER), DBG=$(OT_DBG), OT_FPGA=$(OT_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE) REL_KO=$(REL_KO)
	@echo ---- EXTRA_CFLAGS=$(EXTRA_CFLAGS)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@echo ---- KERNEL_ROOT=$(KERNEL_ROOT)
	@echo ---- ARCH_ROOT=$(ARCH_ROOT), ARCH_HAL=$(ARCH_HAL)
	@echo ---- obj-m=$(obj-m)
	@@echo -e "\e[0m"
	@cd $(SDK_PATH)/mpp/component/securec; make -j
	@cd $(SDK_PATH)/osal/$(OSTYPE)/kernel; make -j
	@cd $(SDK_PATH)/interdrv; make -j
	@cd $(SDK_PATH)/interdrv/ot_irq; make -j
	@cd $(SDK_PATH)/interdrv/ot_proc; make -j
	@cd $(SDK_PATH)/mpp/component/gfbg; make -j
	@cd $(SDK_PATH)/mpp/component/security_subsys; make -j
	@cd $(SDK_PATH)/mpp/cbb/based; make -j
	@cd $(SDK_PATH)/mpp/cbb/sysd; make -j
	@cd $(SDK_PATH)/mpp/cbb/hdmi/src; make -j
	@cd $(SDK_PATH)/mpp/cbb/tde/tde/drv; make -j
	@cd $(SDK_PATH)/mpp/cbb/vo/vo_dev; make -j
	@cd $(SDK_PATH)/mpp/component/svp/pico/svp_npu_dev/mkp; make -j
	@cd $(SDK_PATH)/mpp/sample/omnivision_os08a20; make -j
	@cd $(SDK_PATH)/mpp/sample/hy_s0603; make -j

clean:
	@rm -f *.ko
	@rm .tmp_versions  -rf
	@rm Module.symvers -f
	@rm modules.order  -f
	@rm .*.cmd  -f
	@rm *.mod.c  -f
	@rm *.mod.o  -f
	@rm $(KO_PREFIX)_*.o  -f
	@rm ../init/src/*.o  -f
	@rm ../init/src/.*.cmd  -f
	@rm ../init/src/npu/*.o  -f
	@rm ../init/src/npu/.*.cmd  -f
	@cd $(SDK_PATH)/mpp/component/securec; make clean
	@cd $(SDK_PATH)/interdrv; make clean
	@cd $(SDK_PATH)/interdrv/ot_irq; make clean
	@cd $(SDK_PATH)/interdrv/ot_proc; make clean
	@cd $(SDK_PATH)/osal/$(OSTYPE)/kernel; make clean
	@cd $(SDK_PATH)/mpp/component/gfbg; make clean
	@cd $(SDK_PATH)/mpp/component/security_subsys; make clean
	@cd $(SDK_PATH)/mpp/cbb/based; make clean
	@cd $(SDK_PATH)/mpp/cbb/sysd; make clean
	@cd $(SDK_PATH)/mpp/cbb/hdmi/src; make clean
	@cd $(SDK_PATH)/mpp/cbb/tde/tde/drv; make clean
	@cd $(SDK_PATH)/mpp/cbb/vo/vo_dev; make clean
	@cd $(SDK_PATH)/mpp/component/svp/pico/svp_npu_dev/mkp; make clean
	@cd $(SDK_PATH)/mpp/sample/hy_s0603; make clean
	@cd $(SDK_PATH)/mpp/sample/omnivision_os08a20; make clean
	@cd $(SDK_PATH)/mpp/sample/hy_s0603; make clean
