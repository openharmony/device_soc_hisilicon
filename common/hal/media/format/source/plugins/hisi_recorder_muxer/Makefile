LOCAL_DIR := $(shell pwd)
MEDIA_BASE_DIR := $(LOCAL_DIR)/../../../..
include $(MEDIA_BASE_DIR)/build/base.mak

#===============================================================================
# common deps
#===============================================================================
FORMAT_INC_PATH := $(LOCAL_DIR)/../include

#===============================================================================
# component deps
#===============================================================================
REC_LIB_DIR := $(MIDDLEWARE_COMPONENT)/recorder_pro/lib/$(SOURCE_CHIP_TYPE)/$(CFG_COMPILE_TYPE)
REC_INC_DIR := $(MIDDLEWARE_COMPONENT)/recorder_pro/include
DTCF_LIB_DIR := $(MIDDLEWARE_COMPONENT_DIR)/dtcf/lib/$(SOURCE_CHIP_TYPE)/$(CFG_COMPILE_TYPE)
DTCF_INC_DIR := $(MIDDLEWARE_COMPONENT_DIR)/dtcf/include
MP4_LIB_DIR := $(MIDDLEWARE_COMPONENT)/fileformat/mp4/lib/$(SOURCE_CHIP_TYPE)/$(CFG_COMPILE_TYPE)
MP4_INC_DIR := $(MIDDLEWARE_COMPONENT)/fileformat/mp4/include
TS_LIB_DIR := $(MIDDLEWARE_COMPONENT)/fileformat/ts/lib/$(SOURCE_CHIP_TYPE)/$(CFG_COMPILE_TYPE)
TS_INC_DIR := $(MIDDLEWARE_COMPONENT)/fileformat/ts/include
FILEFORMAT_LIB_DIR := $(MIDDLEWARE_COMPONENT)/fileformat/common/lib/$(SOURCE_CHIP_TYPE)/$(CFG_COMPILE_TYPE)
FILEFORMAT_INC_DIR := $(MIDDLEWARE_COMPONENT)/fileformat/common/include
OT_TIMER_LIB_DIR  := $(MIDDLEWARE_COMMON_DIR)/ottimer/lib/$(SOURCE_CHIP_TYPE)/$(CFG_COMPILE_TYPE)
UPROC_LIB_DIR := $(MIDDLEWARE_COMMON_DIR)/uproc/lib/$(SOURCE_CHIP_TYPE)/$(CFG_COMPILE_TYPE)
FORMAT_INTERFACE_PATH := $(MEDIA_BASE_DIR)/../../../../../../drivers/peripheral/format/interfaces/include
DTCF_MANAGER_PATH := $(LOCAL_DIR)/../common/dtcf_manager
MWLOG_LIB_DIR := $(MIDDLEWARE_COMMON_DIR)/log/lib/$(SOURCE_CHIP_TYPE)/$(CFG_COMPILE_TYPE)
SS_DEMUXER_INC_DIR := $(MIDDLEWARE_COMPONENT)/liteplayer/include

#===============================================================================
# z deps
#===============================================================================
RECORDER_INCLUDE := $(LOCAL_DIR)/include

SRC_DEPS_INC_PATH := -I$(FORMAT_INC_PATH)
SRC_DEPS_INC_PATH += -I$(RECORDER_INCLUDE)\
                     -I$(MEDIA_HAL_COMMN_INC_PATH)\
                     -I$(SS_DEMUXER_INC_DIR)\
                     -I$(DTCF_INC_DIR)\
                     -I$(REC_INC_DIR)\
                     -I$(MP4_INC_DIR)\
                     -I$(TS_INC_DIR) \
                     -I$(FILEFORMAT_INC_DIR) \
                     -I$(MIDDLEWARE_COMMON_INC_DIR) \
                     -I$(MPP_INC_PATH) \
                     -I$(LOCAL_DIR)/../../include \
                     -I$(FORMAT_INTERFACE_PATH) \
                     -I$(DTCF_MANAGER_PATH)

$(warning "**********SRC_DEPS_INC_PATH: $(SRC_DEPS_INC_PATH) **********")
$(warning "**********MEDIA_USR_LIBS_LINK: $(MEDIA_USR_LIBS_LINK) **********")
$(warning "**********MEDIA_LIBS_PATH_LINK: $(MEDIA_LIBS_PATH_LINK) **********")

SRC_ROOT    := $(LOCAL_DIR)
SRC_ROOT    += $(LOCAL_DIR)/../common/dtcf_manager
OBJ_ROOT    := $(SRC_ROOT)/obj
TRAGET_ROOT := $(LOCAL_DIR)/../../$(TARGET_DIR)
$(shell if [ ! -e $(TRAGET_ROOT) ]; then mkdir -p $(TRAGET_ROOT); fi)

SRCS := $(shell find $(SRC_ROOT)  -maxdepth 1 -type f -name '*.c' -print)
SRCS        := $(sort $(SRCS))
OBJS  := $(SRCS:%.c=%.o)
OBJS        := $(sort $(OBJS))
SRC_DEPS    := $(OBJS:%.o=%.d)

CFLAGS += -fdata-sections -fno-omit-frame-pointer

LDFLAGS_SO := $(LDFLAGS)

SRC_DEPS_LIB := -lmp4 -lts -lrecorder_pro -lfileformat -ldtcf -lottimer -lmedia_hal_common -lsecurec -lmwlog -luproc

SRC_DEPS_LIB_PATH +=  -L$(MEDIA_HAL_COMMN_LIB_PATH) \
                    -L$(TS_LIB_DIR)\
                    -L$(REC_LIB_DIR) \
                    -L$(DTCF_LIB_DIR) \
                    -L$(FILEFORMAT_LIB_DIR) \
                    -L$(MP4_LIB_DIR)\
                    -L$(OT_TIMER_LIB_DIR) \
                    $(MEDIA_LIBS_PATH_LINK) \
                    -L$(SECUREC_LIB_PATH) \
		    -L$(MWLOG_LIB_DIR) \
                    -L$(UPROC_LIB_DIR)

CFLAGS_SO := $(CFLAGS)

$(warning "**********CFLAGS_SO: $(CFLAGS_SO) **********")

LIB_SO := $(TRAGET_ROOT)/libplugin_muxer_recorder.so
LIB_A := $(TRAGET_ROOT)/libplugin_muxer_recorder.a

.PHONY : clean all

all: $(LIB_SO) $(LIB_A)
	@-rm -f $(OBJS)

$(LIB_A): $(OBJS)
	$(AR) -rcv $@ $^

$(LIB_SO): $(OBJS)
	$(CC) $(LDFLAGS_SO) $(CFLAGS_SO) $(LFLAGS) -fPIC -shared -o $@ $^ $(SRC_DEPS_LIB_PATH) $(SRC_DEPS_LIB)
	$(STRIP) $(LIB_SO)

$(OBJS): %.o : %.c
	$(CC) $(CFLAGS_SO) $(SRC_DEPS_INC_PATH) -c $< -o $@

clean:
	@-rm -f $(OBJS) $(LIB_SO) $(LIB_A)
