LOCAL_DIR := $(shell pwd)
MEDIA_BASE_DIR := $(LOCAL_DIR)/../..
include $(MEDIA_BASE_DIR)/build/base.mak

#===============================================================================
# common deps
#===============================================================================

SRC_DEPS_INC_PATH :=-I$(LOCAL_DIR)/include
SRC_DEPS_INC_PATH += -I$(MPP_INC_PATH)\
                     -I$(SECUREC_INC_PATH)

# target source
SRCS  := src/media_hal_common.c
SRCS  += src/list.c
ifeq ($(filter $(CFG_CHIP_TYPE),hi3403v100),$(CFG_CHIP_TYPE))
SRCS  += src/audio_aac_adp.c
SRCS  += src/audio_dl_adp.c
SRCS  += src/h8/media_hal_common_sys.c
CFLAGS += -D__SS626V100__
SRC_DEPS_LIB := -lpthread -ldl -lm -lsecurec -lss_mpi -lot_osal -lot_irq -lss_dnvqe -lss_upvqe -laac_dec -laac_comm  -lss_voice_engine -laac_sbr_enc -laac_sbr_dec -lss_otp
else
SRCS  += src/h9/media_hal_common_sys.c
SRC_DEPS_LIB := -lpthread -ldl -lm -lsecurec -lss_mpi_audio -ldnvqe -lupvqe -laac_dec -laac_comm  -lvoice_engine -lss_mpi_audio_adp -lopus -laac_sbr_enc -laac_sbr_dec -lmp3_dec -lmp3_enc
endif

OBJS  := $(SRCS:%.c=%.o)
SRC_DEPS_LIB_PATH += -L$(MPP_LIB_PATH)\
                    -L$(SECUREC_LIB_PATH)
LDFLAGS_SO := $(LDFLAGS)

TRAGET_ROOT := $(LOCAL_DIR)/$(TARGET_DIR)
$(shell if [ ! -e $(TRAGET_ROOT) ]; then mkdir -p $(TRAGET_ROOT); fi)
LIB_SO:= $(TRAGET_ROOT)/libmedia_hal_common.so
LIB_A:= $(TRAGET_ROOT)/libmedia_hal_common.a

.PHONY : clean all

all: $(LIB_SO) $(LIB_A)
	@-rm -f $(OBJS)

$(LIB_SO): $(OBJS)
	$(CC) $(LDFLAGS_SO) $(CFLAGS) $(LFLAGS) -fPIC -shared -o $@ $^ $(SRC_DEPS_LIB_PATH) $(SRC_DEPS_LIB)
	$(STRIP) $(LIB_SO)

$(LIB_A): $(OBJS)
	$(AT)$(AR) -rcv $@ $^

$(OBJS): %.o : %.c
	@$(CC) $(CFLAGS) $(CFLAGS_SO) $(SRC_DEPS_INC_PATH) -c $< -o $@

clean:
	@-rm -f $(OBJS)  $(LIB_SO)
	@-rm -f $(OBJS)  $(LIB_A)
