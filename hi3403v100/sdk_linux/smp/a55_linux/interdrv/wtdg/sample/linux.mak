CFLAGS += -I$(OSAL_ROOT)/include -I./../

# compile options
CFLAGS += -fstack-protector-all    # SP
CFLAGS += -pie -fPIE               # PIE
CFLAGS += -s                       # STRIP
LIB_CFLAGS += -Wl,-z,relro,-z,now  # BIND NOW

# target source
OBJS  := $(SRCS:%.c=%.o)

.PHONY : clean all

all: $(TARGET)

$(TARGET):$(COMM_OBJ) $(OBJS)
	@$(CC) $(CFLAGS) $(LIB_CFLAGS) -lpthread -lm -o $@ $^ $(LIBA) $(MPI_LIBS)

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJS)
	@rm -f $(COMM_OBJ)
	@rm -f *.bin
	@rm -f *.asm

cleanstream:
	@rm -f *.h264
	@rm -f *.h265
	@rm -f *.jpg
	@rm -f *.mjp
	@rm -f *.mp4
