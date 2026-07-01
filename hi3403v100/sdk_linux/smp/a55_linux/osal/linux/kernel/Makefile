ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

src = $(PWD)
EXTRA_CFLAGS := -I$(src)
EXTRA_CFLAGS += -I$(src)/ot_media
EXTRA_CFLAGS += -I$(src)/mmz
EXTRA_CFLAGS += -I$(src)/../../include/
EXTRA_CFLAGS += -DOT_CHIP=$(OT_CHIP)
ifeq ($(MPP_BUILD), y)
    EXTRA_CFLAGS += $(MPP_CFLAGS)
else
    EXTRA_CFLAGS += -I$(REL_INC)
endif
ifeq ($(CONFIG_DRIVER_SPACE_TYPE),user_space)
EXTRA_CFLAGS :=$(subst base/,based/,$(EXTRA_CFLAGS))
EXTRA_CFLAGS :=$(subst sys/,sysd/,$(EXTRA_CFLAGS))
endif
csrcs = $(wildcard *.c ./ot_media/*.c)
oobjs = $(patsubst %.c,%.o,$(csrcs))
BUILD_DIR = $(TEMP_PATH)/$(subst $(SDK_PATH)/,,$(CURDIR))
BUILD_DIR_MAKEFILE := $(BUILD_DIR)/Makefile

ifeq ($(CONFIG_OT_SYS_SMMU_SUPPORT),y)
EXTRA_CFLAGS += -DCONFIG_OT_SYS_SMMU_SUPPORT
endif


obj-m += ot_osal.o
ot_osal-y := osal_fileops.o osal_vmalloc.o osal_addr.o osal_init.o osal_atomic.o osal_barrier.o osal_cache.o \
			osal_debug.o osal_device.o osal_interrupt.o osal_math.o osal_mutex.o osal_proc.o osal_schedule.o \
			osal_semaphore.o osal_spinlock.o osal_string.o osal_task.o osal_timer.o osal_wait.o osal_workqueue.o \
			osal_notifier.o osal_platform.o ./ot_media/base.o ./ot_media/ot_media.o ./mmz/media_mem.o \
			./mmz/mmz_userdev.o ./mmz/ot_allocator.o ./mmz/mmz_comm.o ./mmz/cmpi_mm.o

ot_osal-$(CONFIG_CMA) += ./mmz/cma_allocator.o
ot_osal-$(CONFIG_CMA) += ./mmz/cmpi_mm.o

CLEAN_FILE := $(ot_osal-y) .tmp_versions ot_osal.ko ot_osal.mod.o ot_osal.o modules.order Module.symvers ./mmz/*.o

all:
	@mkdir -p $(BUILD_DIR)
	@touch $(BUILD_DIR_MAKEFILE)
	@make -C $(KERNEL_ROOT) $(LLVM_KERNEL_TOOLCHAIN_ARGS) M=$(BUILD_DIR) src=$(CURDIR) O=$(KERNEL_ROOT) KBUILD_EXTRA_SYMBOLS="" V=10 -j 24 modules
	@mkdir -p $(REL_KO) && cp $(BUILD_DIR)/ot_osal.ko $(REL_KO)
	@mkdir -p $(REL_INC) && cp $(src)/../../include/* $(REL_INC)
clean:
	@mkdir -p $(BUILD_DIR)
	@rm -rf $(addprefix $(BUILD_DIR)/,$(CLEAN_FILE))
	@mkdir -p $(REL_KO) && rm $(REL_KO)/ot_osal.ko -f
	@mkdir -p $(REL_INC) && cd  $(REL_INC)/ && rm ot_osal.h ot_osal_user.h osal_ioctl.h  osal_list.h  osal_mmz.h  -f
	@for x in `find ${BUILD_DIR} -name "*.mod.c"`;do rm -rf $$x;done
	@for x in `find ${BUILD_DIR} -name "*.cmd"`;do rm -rf $$x;done
