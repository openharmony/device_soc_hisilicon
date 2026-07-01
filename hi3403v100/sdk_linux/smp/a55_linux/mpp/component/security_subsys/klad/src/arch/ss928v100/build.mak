
OT_KLAD_VERSION := v100

# KLAD_SECURE_CPU: force cpu to tee
# KLAD_SWITCH_CPU: switch ree or tee cpu
# else default ree cpu
KLAD_DRV_CFLAGS += -DKLAD_SWITCH_CPU
