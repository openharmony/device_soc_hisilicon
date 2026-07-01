CFLAGS += -I$(OSAL_ROOT)/include

# compile options
CFLAGS += -fstack-protector-all    # SP
CFLAGS += -pie -fPIE               # PIE
CFLAGS += -s                       # STRIP
LIB_CFLAGS += -Wl,-z,relro,-z,now  # BIND NOW

# target source

.PHONY : clean all

all: $(TARGET)

$(TARGET): $(SMP_SRCS) $(COMM_SRC)
	@echo $(CFLAGS)
	@$(CC) $(CFLAGS) $(LIB_CFLAGS) -o $@ $^ $(MPI_LIBS) $(AUDIO_LIBA) $(JPEGD_LIBA)

clean:
	@rm -f $(LIBA) $(LIBS)
	@rm -f $(OBJ)
	@rm -f $(COMM_OBJ)
	@rm -f sample_hdmi_display
