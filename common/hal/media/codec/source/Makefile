LOCAL_DIR := $(shell pwd)
MEDIA_BASE_DIR := $(LOCAL_DIR)/../..
include $(MEDIA_BASE_DIR)/build/base.mak

#===============================================================================
# common deps
#===============================================================================

#===============================================================================
# component deps
#===============================================================================
COMM_INC_DIR := $(MIDDLEWARE_DIR)/common/include

#===============================================================================
# self deps
#===============================================================================
CODEC_INCLUDE := $(LOCAL_DIR)/include

INC_ROOT    := $(LOCAL_DIR)
INC_PATH     := $(sort $(dir $(shell find $(INC_ROOT) -name '*.h')))

SRC_DEPS_INC_PATH := $(foreach dir, $(INC_PATH), -I$(dir))
SRC_DEPS_INC_PATH += -I$(CODEC_INCLUDE)\
                     -I$(MPP_INC_PATH)\
                     -I$(MEDIA_HAL_COMMN_INC_PATH)\
                     -I$(COMM_INC_DIR)\
                     -I$(CODEC_INTERFACE_PATH)\
                     -I$(BASE_INTERFACE_PATH)\
                     -I$(DISPLAY_INTERFACE_PATH)

SRC_ROOT    := $(LOCAL_DIR)/src
OBJ_ROOT    := $(SRC_ROOT)/obj
TRAGET_ROOT := $(LOCAL_DIR)/$(TARGET_DIR)
$(shell if [ ! -e $(TRAGET_ROOT) ]; then mkdir -p $(TRAGET_ROOT); fi)
SRCS := $(shell find $(SRC_ROOT) -type f -name '*.c' -print)
SRCS        := $(sort $(SRCS))
OBJS  := $(SRCS:%.c=%.o)
OBJS        := $(sort $(OBJS))
SRC_DEPS    := $(OBJS:%.o=%.d)

CFLAGS_CUR := -fdata-sections -fno-omit-frame-pointer

LDFLAGS_SO := $(LDFLAGS)

SRC_DEPS_LIB +=	-lmedia_hal_common -lsecurec -lpthread -ldl -lm
SRC_DEPS_LIB_PATH += -L$(SECUREC_LIB_PATH) -L$(MEDIA_HAL_COMMN_LIB_PATH)

LIB_SO := $(TRAGET_ROOT)/libcodec.so
LIB_A := $(TRAGET_ROOT)/libcodec.a

.PHONY : clean all

all: $(LIB_SO) $(LIB_A) plugin
	@-rm -f $(OBJS)

$(LIB_A): $(OBJS)
	$(AR) -rcv $@ $^

$(LIB_SO): $(OBJS)
	$(CC) $(LDFLAGS_SO) $(CFLAGS) $(CFLAGS_CUR) $(LFLAGS) -fPIC -shared -o $@ $^ $(SRC_DEPS_LIB_PATH) $(SRC_DEPS_LIB)
	$(STRIP) $(LIB_SO)


$(OBJS): %.o : %.c
	@$(CC) $(CFLAGS_SO) $(CFLAGS) $(CFLAGS_CUR) $(LFLAGS) $(SRC_DEPS_INC_PATH) -c $< -o $@

plugin:
	@pushd $(LOCAL_DIR);cd plugins;make || exit "$$?";popd

clean:
	@-rm -f $(OBJS) $(LIB_SO) $(LIB_A)
	@-pushd $(LOCAL_DIR);cd plugins;make clean;popd
