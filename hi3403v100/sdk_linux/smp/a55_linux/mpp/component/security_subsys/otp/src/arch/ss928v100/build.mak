
OTP_DRV_CFLAGS += -I$(OTP_BASE_DIR)/arch/$(INTER_DRV)/

DRV_OBJS += arch/$(INTER_DRV)/otp_data.o

OT_OTP_VERSION := v200

# OTP_SECURE_CPU: force cpu to tee
# OTP_SWITCH_CPU: switch ree or tee cpu
# else default ree cpu
OTP_DRV_CFLAGS += -DOTP_SWITCH_CPU
