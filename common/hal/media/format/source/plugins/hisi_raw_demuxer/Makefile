#===============================================================================
# export variable
#===============================================================================
LOCAL_DIR := $(shell pwd)
MEDIA_BASE_DIR := $(LOCAL_DIR)/../../../..
include $(MEDIA_BASE_DIR)/build/base.mak

#===============================================================================
# local variable
#===============================================================================
#mod dir
MOD_ROOT    := $(LOCAL_DIR)/../..
OBJ_ROOT    := $(LOCAL_DIR)/obj
TRAGET_ROOT := $(MOD_ROOT)/$(TARGET_DIR)
$(shell if [ ! -e $(TRAGET_ROOT) ]; then mkdir -p $(TRAGET_ROOT); fi)
SRC_ROOT    := $(LOCAL_DIR)
TASK_DIR    := $(LOCAL_DIR)/task
HAL_FORMAT_DIR := $(LOCAL_DIR)/../..
#src files
SRCS        := $(shell find $(SRC_ROOT)  -type f -name '*.c' -print)
SRCS        := $(sort $(SRCS))

#obj files
OBJS        := $(SRCS:$(SRC_ROOT)%.c=$(OBJ_ROOT)%.o)
OBJS        := $(sort $(OBJS))
OBJ_DIR     := $(dir $(OBJS))

SRC_DEPS    := $(OBJS:%.o=%.d)

#include path
FORMAT_INTERFACE_PATH := $(MEDIA_BASE_DIR)/../../../../../../drivers/peripheral/format/interfaces/include

MEDIA_INCLUDE_PATH += -I$(MIDDLEWARE_COMMON_INC_DIR)
MEDIA_INCLUDE_PATH += -I$(MPP_INC_PATH)
MEDIA_INCLUDE_PATH += -I$(SECUREC_INC_PATH)
MEDIA_INCLUDE_PATH += -I$(HAL_FORMAT_DIR)/include
MEDIA_INCLUDE_PATH += -I$(MEDIA_HAL_COMMN_INC_PATH)
MEDIA_INCLUDE_PATH += -I$(FORMAT_INTERFACE_PATH)
MEDIA_INCLUDE_PATH += -I$(MIDDLEWARE_COMPONENT)/liteplayer/include

# create obj dir $$ target dir
CreateResult :=
dummy := $(call CreateDir, $(OBJ_ROOT))
dummy += $(call CreateDir, $(TRAGET_ROOT))
dummy += $(foreach dir, $(OBJ_DIR), CreateResult += $(call CreateDir, $(dir)))
ifneq ($(strip $(CreateResult)),)
    $(error $(CreateResult))
endif

MUXER_LIB = -L$(MEDIA_HAL_COMMN_LIB_PATH) -lmedia_hal_common -L$(SECUREC_LIB_PATH) -lsecurec -lm
$(warning "COMPILER_BASE_PATH-------> $(COMPILER_BASE_PATH)")
#target
TARGET_A := $(TRAGET_ROOT)/libplugin_demuxer_raw.a
TARGET_SO := $(TRAGET_ROOT)/libplugin_demuxer_raw.so

#===============================================================================
# rules
#===============================================================================
COMPILE    = $(CC) $(CFLAGS) -c  "$<" -o "$@" $(MEDIA_INCLUDE_PATH)
COMPILEDEP = $(CC) -MM "$<"  $(MEDIA_INCLUDE_PATH)
LINK       = $(CC) $(CFLAGS)

.PHONY: all clean install uninstall

all: $(TARGET_A) $(TARGET_SO)
	rm -rf $(OBJ_ROOT)

$(TARGET_A): $(OBJS)
	$(AT)$(AR) -rcv $@ $^

$(TARGET_SO): $(OBJS)
	$(AT)$(CC) $(LDFLAGS) $(LFLAGS) $(CFLAGS) -fPIC -shared -o $@ $^ $(MUXER_LIB)
	$(STRIP) $(TARGET_SO)

$(OBJS): $(OBJ_ROOT)/%.o :$(SRC_ROOT)/%.c
	$(AT)$(COMPILE)

$(SRC_DEPS): $(OBJ_ROOT)/%.d : $(SRC_ROOT)/%.c
	$(AT)set -e;rm -f $@;$(COMPILEDEP) > $@.$$$$; \
	sed 's,.*\.o[ :]*,$(@:%.d=%.o) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(SRC_DEPS)

install: all
#	$(AT)mkdir -p $(OUT_LIB_DIR)
#	$(AT)mkdir -p $(OUT_HEADER_DIR)
	$(AT)cp -af $(TARGET_A) $(OUT_LIB_DIR)/
	$(AT)cp -af $(TARGET_SO) $(OUT_LIB_DIR)/

uninstall:
	$(AT)rm -rf $(OUT_LIB_DIR)/$(notdir $(TARGET_A))
	$(AT)rm -rf $(OUT_LIB_DIR)/$(notdir $(TARGET_SO))

clean:
	$(AT)rm -rf $(SRC_DEPS)
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(OBJ_ROOT)
	$(AT)rm -rf $(TARGET_A)
	$(AT)rm -rf $(TARGET_SO)
