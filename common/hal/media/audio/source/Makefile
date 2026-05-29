LOCAL_DIR := $(shell pwd)
#vendor\hisi\hi35xx\hardware\media
MEDIA_BASE_DIR := $(LOCAL_DIR)/../..
include $(MEDIA_BASE_DIR)/build/base.mak

#===============================================================================
# self deps
#===============================================================================
AUDIO_INTERFACE_PATH := $(MEDIA_BASE_DIR)/../../../../../../drivers/peripheral/audio/interfaces/include

SRC_DEPS_INC_PATH += -I$(MEDIA_HAL_COMMN_INC_PATH) \
                     -I$(AUDIO_INTERFACE_PATH) \
					 -I$(LOCAL_DIR)/src/audio_plugins_manager/include \
					 -I$(LOCAL_DIR)/plugins/plugin_include \
					 -I$(LOCAL_DIR)/plugins/hi35xx/audio_primary_port_plugin \
					 -I$(SECUREC_INC_PATH)


SRC_ROOT    := $(LOCAL_DIR)
OBJ_ROOT    := $(SRC_ROOT)/obj
TRAGET_ROOT := $(SRC_ROOT)/$(TARGET_DIR)
$(shell if [ ! -e $(TRAGET_ROOT) ]; then mkdir -p $(TRAGET_ROOT); fi)
SRCS := $(shell find $(SRC_ROOT) -maxdepth 2 -type f -name '*.c' -print)
SRCS        := $(sort $(SRCS))
SRCS += $(LOCAL_DIR)/src/audio_plugins_manager/src/audio_plugins_manager.c
OBJS  := $(SRCS:%.c=%.o)
OBJS        := $(sort $(OBJS))
SRC_DEPS    := $(OBJS:%.o=%.d)

# create obj dir $$ target dir
CreateResult :=
dummy := $(call CreateDir, $(OBJ_ROOT))
dummy += $(call CreateDir, $(TRAGET_ROOT))
dummy += $(foreach dir, $(OBJ_DIR), CreateResult += $(call CreateDir, $(dir)))
ifneq ($(strip $(CreateResult)),)
    $(error $(CreateResult))
endif

CFLAGS_CUR := $(CFLAGS) -fdata-sections -fno-omit-frame-pointer -DENABLE_DL_AUDIO_PLUGIN
LDFLAGS_SO := $(LDFLAGS)

SRC_DEPS_LIB += -lsecurec -lpthread -ldl -lm -lmedia_hal_common
SRC_DEPS_LIB_PATH += -L$(MEDIA_HAL_COMMN_LIB_PATH)\
                     -L$(SECUREC_LIB_PATH)

LIB_SO := $(TRAGET_ROOT)/libaudio_hw.so
LIB_A := $(TRAGET_ROOT)/libaudio_hw.a

.PHONY : clean all

all: $(LIB_SO) $(LIB_A)
	@-rm -f $(OBJS)
	@-rm -r $(OBJ_ROOT)

$(LIB_A): $(OBJS)
	$(AR) -rcv $@ $^


$(LIB_SO): $(OBJS)
	$(CC) $(LDFLAGS_SO) $(CFLAGS_CUR) $(LFLAGS) -fPIC -shared -o $@ $^ $(SRC_DEPS_LIB_PATH) $(SRC_DEPS_LIB)
	@pushd $(LOCAL_DIR);cd plugins || exit "$$?";make || exit "$$?";popd
	$(STRIP) $(LIB_SO)


$(OBJS): %.o : %.c
	$(CC) $(CFLAGS_CUR) $(LFLAGS) $(SRC_DEPS_INC_PATH) -c $< -o $@

clean:
	@-rm -f $(OBJS) $(LIB_SO) $(LIB_A)
	@pushd $(LOCAL_DIR);cd plugins;make clean;popd
	@-rm -r $(TRAGET_ROOT) $(OBJ_ROOT)
