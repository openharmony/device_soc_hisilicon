LOCAL_DIR := $(shell pwd)
MEDIA_BASE_DIR := $(LOCAL_DIR)/../../../..
include $(MEDIA_BASE_DIR)/build/base.mak


#===============================================================================
# self deps
#===============================================================================
AUDIO_PLUGIN_INC_PATH := $(LOCAL_DIR)/../plugin_include
AUDIO_PORT_PLUGIN_COM_INC_PATH := $(LOCAL_DIR)/audio_primary_port_plugin_common/include
AUDIO_SUB_PORT_IN_INC_PATH := $(LOCAL_DIR)/audio_primary_sub_port_in
AUDIO_SUB_PORT_OUT_INC_PATH := $(LOCAL_DIR)/audio_primary_sub_port_out
AUDIO_EFFECT_MANAGER_INC_PATH := $(LOCAL_DIR)/audio_effect_manager
AUDIO_INTERFACE_PATH := $(MEDIA_BASE_DIR)/../../../../../../drivers/peripheral/audio/interfaces/include

OTCONFACCESS_INC_PATH := $(MEDIA_BASE_DIR)/../middleware/source/component/otconfaccess/include/
MW_COMMON_INC_PATH := $(MEDIA_BASE_DIR)/../middleware/source/common/include/

SRC_DEPS_INC_PATH := -I$(AUDIO_HAL_INC_PATH)
SRC_DEPS_INC_PATH += -I$(AUDIO_DRIVERS_INC_PATH)\
                     -I$(MPP_INC_PATH)\
                     -I$(AUDIO_PORT_PLUGIN_COM_INC_PATH)\
                     -I$(MEDIA_HAL_COMMN_INC_PATH) \
                     -I$(AUDIO_INTERFACE_PATH) \
                     -I$(AUDIO_PLUGIN_INC_PATH) \
                     -I$(AUDIO_SUB_PORT_IN_INC_PATH) \
                     -I$(AUDIO_SUB_PORT_OUT_INC_PATH) \
                     -I$(AUDIO_EFFECT_MANAGER_INC_PATH) \
                     -I$(OTCONFACCESS_INC_PATH) \
                     -I$(MW_COMMON_INC_PATH) \
                     -I$(SECUREC_INC_PATH)

#mod dir
MOD_ROOT    := $(LOCAL_DIR)/../..
SRC_ROOT    := $(LOCAL_DIR)
OBJ_ROOT    := $(SRC_ROOT)/obj
TRAGET_ROOT := $(MOD_ROOT)/$(TARGET_DIR)
$(shell if [ ! -e $(TRAGET_ROOT) ]; then mkdir -p $(TRAGET_ROOT); fi)
SRCS := $(LOCAL_DIR)/audio_primary_port_plugin/audio_primary_port_plugin.c
SRCS += $(LOCAL_DIR)/audio_primary_sub_port_in/audio_primary_sub_port_in.c
SRCS += $(LOCAL_DIR)/audio_primary_sub_port_out/audio_primary_sub_port_out.c
SRCS += $(LOCAL_DIR)/audio_effect_manager/audio_effect_manager.c
SRCS += $(LOCAL_DIR)/audio_primary_port_plugin_common/src/audio_port_plugin_common.c
SRCS := $(sort $(SRCS))


OBJS  := $(SRCS:%.c=%.o)
OBJS        := $(sort $(OBJS))
SRC_DEPS    := $(OBJS:%.o=%.d)

CFLAGS_CUR := $(CFLAGS) -fdata-sections -fno-omit-frame-pointer

LDFLAGS_SO := $(LDFLAGS)

ifeq ($(filter $(CFG_CHIP_TYPE),hi3403v100),$(CFG_CHIP_TYPE))
CFLAGS_CUR += -D__SS626V100__ -D__LINUX__
SRC_DEPS_LIB +=	-lmedia_hal_common -lss_mpi -lsecurec -lpthread -ldl -lm -lmwlog -lotconfaccess  -laac_sbr_enc -laac_sbr_dec -lvqe_record -lvqe_res -lss_otp
else
SRC_DEPS_LIB += -lmedia_hal_common -lss_mpi_audio -lsecurec -lpthread -ldl -lm -lmwlog -lotconfaccess -lss_mpi_audio_adp -lopus -laac_sbr_enc -laac_sbr_dec -lmp3_dec -lmp3_enc -lvqe_record -lvqe_res
endif
SRC_DEPS_LIB_PATH += -L$(MPP_LIB_PATH)\
                     -L$(SECUREC_LIB_PATH) \
                     -L$(MEDIA_HAL_COMMN_LIB_PATH) 

ifeq ($(filter $(CFG_CHIP_TYPE),hi3403v100),$(CFG_CHIP_TYPE))
SRC_DEPS_LIB_PATH += -L$(MEDIA_BASE_DIR)/../middleware/source/component/otconfaccess/lib/ss928v100/clang_musl \
                     -L$(MEDIA_BASE_DIR)/../middleware/source/common/log/lib/ss928v100/clang_musl
else
SRC_DEPS_LIB_PATH += -L$(MEDIA_BASE_DIR)/../middleware/source/component/otconfaccess/lib/hi3516dv500/gcc_musl \
                     -L$(MEDIA_BASE_DIR)/../middleware/source/common/log/lib/hi3516dv500/gcc_musl
endif

ifeq ($(CFG_OS_TYPE),OHOS)
SRC_DEPS_LIB_PATH += -L$(OUT_DIR)/$(CFG_CHIP_TYPE)/ipcamera_$(CFG_CHIP_TYPE)_liteos \
                     -L$(OUT_DIR)/$(CFG_CHIP_TYPE)/ipcamera_$(CFG_CHIP_TYPE)_liteos/usr/lib
else
SRC_DEPS_LIB_PATH += -L$(OUT_DIR)/$(CFG_CHIP_TYPE)/ipcamera_$(CFG_CHIP_TYPE)_linux \
                     -L$(OUT_DIR)/$(CFG_CHIP_TYPE)/ipcamera_$(CFG_CHIP_TYPE)_linux/usr/lib
endif

LIB_SO := $(TRAGET_ROOT)/libaudio_primary_port.so
LIB_A := $(TRAGET_ROOT)/libaudio_primary_port.a

.PHONY : clean all

all: $(LIB_SO) $(LIB_A)
	@-rm -f $(OBJS)

$(LIB_A): $(OBJS)
	$(AR) -rcv $@ $^


$(LIB_SO): $(OBJS)
	$(CC) $(LDFLAGS_SO) $(CFLAGS_CUR) $(LFLAGS) -fPIC -shared -o $@ $^ $(SRC_DEPS_LIB_PATH) $(SRC_DEPS_LIB)
	$(STRIP) $(LIB_SO)


$(OBJS): %.o : %.c
	$(CC) $(CFLAGS_CUR) $(LFLAGS) $(SRC_DEPS_INC_PATH) -c $< -o $@

clean:
	@-rm -f $(OBJS) $(LIB_SO) $(LIB_A)
