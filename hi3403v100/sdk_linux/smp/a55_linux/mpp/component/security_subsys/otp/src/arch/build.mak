
OTP_DRV_CFLAGS += -I$(OTP_BASE_DIR)/arch/include
OTP_DRV_CFLAGS += -I$(OTP_BASE_DIR)/arch/hal/

# INTER_DRV select chip
include $(OTP_BASE_DIR)/arch/$(INTER_DRV)/build.mak

DRV_OBJS += arch/hal/hal_otp_comm.o

DRV_OBJS += arch/hal/$(OT_OTP_VERSION)/hal_otp.o
OTP_DRV_CFLAGS += -I$(OTP_BASE_DIR)/arch/hal/$(OT_OTP_VERSION)

# lower to upper
OTP_DRV_CFLAGS += -DOT_OTP_$(shell echo $(OT_OTP_VERSION) | tr a-z A-Z)
