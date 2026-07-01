CSRC  = $(wildcard *.c wildcard common/*.c wildcard otp/*.c wildcard ddr_init/*.c wildcard key_slot/*.c wildcard klad/*.c wildcard pke/*.c wildcard cipher/*.c wildcard securelib/*.c wildcard uart/*.c wildcard emmc/*.c wildcard sdio/*.c wildcard usb3/*.c wildcard tzasc/*.c)
OBJS := $(patsubst %.c,%.o,$(CSRC) )

#.SILENT:

CFLAGS += -I./
CFLAGS += -I./common/
CFLAGS += -I./otp/
CFLAGS += -I./ddr_init/
CFLAGS += -I./key_slot/
CFLAGS += -I./klad/
CFLAGS += -I./klad/include/
CFLAGS += -I./pke/
CFLAGS += -I./cipher/
CFLAGS += -I./securelib/
CFLAGS += -I./uart/
CFLAGS += -I./emmc/
CFLAGS += -I./sdio/
CFLAGS += -I./usb3/
CFLAGS += -I./tzasc/

default: libdrv.a

libdrv.a: $(OBJS)
	echo "  AR    libdrv.a"
	$(AR) r libdrv.a $(OBJS)
	echo "  RL    $@"
	$(RANLIB) $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
#.c.o:
#	echo "  CC    $<"
#	$(CC) $(CFLAGS) $<

clean:
	rm *.o *.a -f
	rm common/*.o    common/*.a -f
	rm otp/*.o       otp/*.a -f
	rm ddr_init/*.o  ddr_init/*.a -f
	rm key_slot/*.o  key_slot/*.a -f
	rm klad/*.o      klad/*.a -f
	rm pke/*.o       pke/*.a -f
	rm cipher/*.o    cipher/*.a -f
	rm securelib/*.o securelib/*.a -f
	rm uart/*.o      uart/*.a -f
	rm emmc/*.o      emmc/*.a -f
	rm sdio/*.o      sdio/*.a -f
	rm usb3/*.o      usb3/*.a -f
	rm tzasc/*.o     tzasc/*.a -f
