
SDK_LIB_PATH := -L$(REL_LIB)

SDK_LIB := $(SDK_LIB_PATH) --start-group -lot_osal -l$(KO_PREFIX)_base -l$(KO_PREFIX)_ir --end-group

LITEOS_LIBDEPS = --start-group $(LITEOS_LIBDEP) --end-group $(LITEOS_TABLES_LDFLAGS)

LDFLAGS := $(LITEOS_LDFLAGS) --gc-sections

# target source
SRCS  += $(OSTYPE)/app_init.c
SRCS  += $(OSTYPE)/sdk_init.c
OBJS  := $(SRCS:%.c=%.o)
OBJS += $(COMM_OBJ)

CFLAGS += -I$(OSAL_ROOT)/include

BIN := $(TARGET).bin
MAP := $(TARGET).map

.PHONY : clean all

all: $(BIN)
	@echo -e "\e[0;32;1m--Compiling 'sample_ot_ir'...\e[0;36;1m"
	@@echo -e "\e[0m"

$(BIN):$(TARGET)
	@$(OBJCOPY) -O binary $(TARGET) $(BIN)

$(TARGET):$(OBJS)
	@$(LD) $(LDFLAGS) -Map=$(MAP) -o $(TARGET) $(OBJS) $(SDK_LIB) $(LITEOS_LIBDEPS)
	@$(OBJDUMP) -d $(TARGET) > $(TARGET).asm

$(OBJS):%.o:%.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(TARGET) $(BIN) $(MAP) *.asm *.o
	@rm -f $(OBJS)
	@rm -f $(COMM_OBJ)

cleanstream:
	@rm -f *.h264
	@rm -f *.jpg
	@rm -f *.mjp
	@rm -f *.mp4
