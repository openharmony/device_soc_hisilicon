LOCAL_DIR := $(shell pwd)
MEDIA_BASE_DIR := $(LOCAL_DIR)/../..
include $(MEDIA_BASE_DIR)/build/base.mak

COMMON_DIR := $(MIDDLEWARE_COMMON_DIR)
FORMAT_INTERFACE_PATH := $(MEDIA_BASE_DIR)/../../../../../../drivers/peripheral/format/interfaces/include
SS_DEMUXER_INC_DIR := $(MIDDLEWARE_COMPONENT)/liteplayer/include
SS_DEMUXER_LIB_DIR := $(MIDDLEWARE_COMPONENT)/liteplayer/lib/$(SOURCE_CHIP_TYPE)/$(CFG_COMPILE_TYPE)
FILEFORMAT_LIB_DIR := $(MIDDLEWARE_COMPONENT)/fileformat/common/lib/$(SOURCE_CHIP_TYPE)/$(CFG_COMPILE_TYPE)

APP_INC :=
APP_INC += -I$(MIDDLEWARE_COMMON_INC_DIR) \
           -I$(SECUREC_INC_PATH) \
           -I$(MPP_INC_PATH) \
           -I$(MEDIA_HAL_COMMN_INC_PATH)\
           -I$(SS_DEMUXER_INC_DIR)\
           -I$(FORMAT_INTERFACE_PATH)

$(warning "APP_INC: $(APP_INC)")

SRCS := $(sort $(shell find $(LOCAL_DIR)/src -name '*.c' -print))
$(warning "SRCS: $(SRCS)")

OBJS := $(patsubst %.c,%.o,$(SRCS))

INC_FLAGS += -I$(LOCAL_DIR)/include \
             -I$(LOCAL_DIR)/src/format_common/include \
             -I$(LOCAL_DIR)/src/plugins_manager_common/include \
             -I$(LOCAL_DIR)/src/demuxer_plugins_manager/include \
             -I$(LOCAL_DIR)/src/muxer_plugins_manager/include \
             -I$(LOCAL_DIR)/src/protocol_plugins_manager/include \
             -I$(LOCAL_DIR)/src/muxer_manager/include \
             -I$(LOCAL_DIR)/src/demuxer_manager/include \

CFLAGS_SO := -L$(MEDIA_HAL_COMMN_LIB_PATH) -L$(SECUREC_LIB_PATH) -L$(SS_DEMUXER_LIB_DIR) -L$(FILEFORMAT_LIB_DIR) -lfileformat -lmedia_hal_common -lpthread -ldl -lm -lsecurec -lplugin_demuxer_hmf -lmpf_demuxer 
LDFLAGS_SO := $(LDFLAGS)

APP_INC += $(INC_FLAGS)
CFLAGS_CUR := $(CFLAGS) -DENABLE_DEMUXER -DENABLE_MUXER

TRAGET_ROOT := $(LOCAL_DIR)/$(TARGET_DIR)
$(shell if [ ! -e $(TRAGET_ROOT) ]; then mkdir -p $(TRAGET_ROOT); fi)
LIB_A := $(TRAGET_ROOT)/libformat_hw.a
LIB_SO := $(TRAGET_ROOT)/libformat_hw.so

.PHONY : all clean

all: format_demuxer

format_demuxer: $(LIB_SO) $(LIB_A)
	@-rm -f $(OBJS)

$(LIB_SO): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS_CUR) $(LFLAGS) -fPIC -shared $(CFLAGS_SO) -o $@ $^
	$(STRIP) $(LIB_SO)

$(LIB_A): $(OBJS)
	$(AT)$(AR) -rcv $@ $^

$(OBJS): %.o : %.c
	$(CC) $(CFLAGS_CUR) $(APP_INC) -c $< -o $@
	pushd $(LOCAL_DIR);cd $(LOCAL_DIR)/plugins ;$(MAKE) $(MFLAGS) || exit "$$?"; popd ;

clean:
	pushd $(LOCAL_DIR);cd $(LOCAL_DIR)/plugins ;$(MAKE) clean; popd ;
	@-rm -f $(OBJS)  $(LIB_SO)  $(LIB_A)


