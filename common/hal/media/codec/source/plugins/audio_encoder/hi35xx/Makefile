LOCAL_DIR := $(shell pwd)
MEDIA_BASE_DIR := $(LOCAL_DIR)/../../../../..
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
CODEC_DIR := $(LOCAL_DIR)/../../../
CODEC_INCLUDE := $(CODEC_DIR)/include

INC_ROOT    := $(LOCAL_DIR)
INC_PATH     := $(sort $(dir $(shell find $(INC_ROOT) -name '*.h')))

SRC_DEPS_INC_PATH := $(foreach dir, $(INC_PATH), -I$(dir))
SRC_DEPS_INC_PATH += -I$(CODEC_INCLUDE)\
                     -I$(MPP_INC_PATH)\
                     -I$(MEDIA_HAL_COMMN_INC_PATH)\
                     -I$(COMM_INC_DIR)\
                     -I$(CODEC_DIR)/src/pluginManager/codec_plugin_base \
                     -I$(CODEC_DIR)/src/common\
                     -I$(CODEC_INTERFACE_PATH)

SRC_ROOT    := $(LOCAL_DIR)/
OBJ_ROOT    := $(SRC_ROOT)/obj
TRAGET_ROOT := $(CODEC_DIR)/$(TARGET_DIR)
$(shell if [ ! -e $(TRAGET_ROOT) ]; then mkdir -p $(TRAGET_ROOT); fi)
SRCS := $(shell find $(SRC_ROOT) -type f -name '*.c' -print)
SRCS        := $(sort $(SRCS))
OBJS  := $(SRCS:%.c=%.o)
OBJS        := $(sort $(OBJS))
SRC_DEPS    := $(OBJS:%.o=%.d)

CFLAGS_CUR := $(CFLAGS) -fdata-sections -fno-omit-frame-pointer
LDFLAGS_SO := $(LDFLAGS)

ifeq ($(filter $(CFG_CHIP_TYPE),hi3403v100),$(CFG_CHIP_TYPE))
CFLAGS_CUR += -D__SS626V100__
SRC_DEPS_LIB += -lmedia_hal_common -lss_mpi -lss_dnvqe -lss_upvqe -laac_enc -laac_comm  -lss_voice_engine -lsecurec -lpthread -ldl -lm
else
SRC_DEPS_LIB += -lmedia_hal_common -lss_mpi -lss_mpi_audio -lss_mpi_sysbind -ldnvqe -lupvqe -laac_enc -laac_comm  -lvoice_engine -lsecurec -lpthread -ldl -lm
endif

SRC_DEPS_LIB_PATH += -L$(MPP_LIB_PATH)\
					 -L$(SECUREC_LIB_PATH)\
					 -L$(MEDIA_HAL_COMMN_LIB_PATH)

LIB_SO := $(TRAGET_ROOT)/libplugin_aenc_35xx.so
LIB_A := $(TRAGET_ROOT)/libplugin_aenc_35xx.a

.PHONY : clean all

all: $(LIB_SO) $(LIB_A)
	@-rm -f $(OBJS)

$(LIB_A): $(OBJS)
	$(AR) -rcv $@ $^

$(LIB_SO): $(OBJS)
	$(CC) $(LDFLAGS_SO) $(CFLAGS_CUR) $(LFLAGS) -fPIC -shared -o $@ $^ $(SRC_DEPS_LIB_PATH) $(SRC_DEPS_LIB)
	$(STRIP) $(LIB_SO)


$(OBJS): %.o : %.c
	@$(CC) $(CFLAGS_SO) $(CFLAGS_CUR) $(LFLAGS) $(SRC_DEPS_INC_PATH) -c $< -o $@

clean:
	@-rm -f $(OBJS) $(LIB_SO) $(LIB_A)
