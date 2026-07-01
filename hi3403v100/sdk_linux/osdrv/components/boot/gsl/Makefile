include cfg.mk

export CROSS_COMPILE := aarch64-v01c01-linux-gnu-

export CC := $(CROSS_COMPILE)gcc
export AR := $(CROSS_COMPILE)ar
export RANLIB := $(CROSS_COMPILE)ranlib
export LD := $(CROSS_COMPILE)ld
export OBJCOPY := $(CROSS_COMPILE)objcopy
export OBJDUMP := $(CROSS_COMPILE)objdump

export CHIP?=ss928v100

CSRC  = $(wildcard boot/*.c wildcard lib/*.c)
SSRC  = $(wildcard boot/*.S)

OBJS := $(patsubst %.c,%.o,$(CSRC) )
OBJS += $(patsubst %.S,%.o,$(SSRC) )

export CFLAGS := -fno-builtin -fno-common
CFLAGS += -Wall
CFLAGS += -I$(CURDIR)/include/
CFLAGS += -I$(CURDIR)/include/$(CHIP)/

CFLAGS += -DCFG_RAM_BASE_ADDR=$(RAM_BASE)

CFLAGS += -march=armv8-a

CFLAGS += -c

CFLAGS += -Os -fno-store-merging -fomit-frame-pointer -fno-exceptions -fno-asynchronous-unwind-tables -fno-unwind-tables -mstrict-align -fstack-protector-strong

TARGET = gsl
BIN_PATH = pub

LDSCRIPT := linker.lds
LDFLAGS := -Bstatic -T $(LDSCRIPT) -Ttext $(TEXT_BASE)
LDFLAGS += drivers/libdrv.a

.SILENT:

default:
	make -C drivers/
	make -C boot/
	sed -e 's/TEXT_BASE/$(TEXT_BASE)/' \
		-e 's/RAM_BASE/$(RAM_BASE)/' \
		$(LDSCRIPT).mk > $(LDSCRIPT)
	echo "  $(LD) $(OBJS) $(LDFLAGS) -Map $(TARGET).map -o $(TARGET) $(OBJCOPY) -O binary $(TARGET) $(TARGET).bin"
	mkdir -p $(BIN_PATH)
	$(LD) $(OBJS) $(LDFLAGS) -Map $(BIN_PATH)/$(TARGET).map -o $(BIN_PATH)/$(TARGET)
	$(OBJCOPY) -O binary $(BIN_PATH)/$(TARGET) $(BIN_PATH)/$(TARGET).bin

clean:
	make -C drivers/ clean
	make -C boot/ clean
	rm -rf $(BIN_PATH)
	rm -rf $(LDSCRIPT)

