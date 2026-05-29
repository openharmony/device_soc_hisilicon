ifeq ($(PARAM_FILE), )
	PARAM_FILE:=../mpp/Makefile.param
	include $(PARAM_FILE)
endif

sub_dir:=

ifeq ($(CONFIG_OT_USER), y)
    sub_dir += ot_user
endif

ifeq ($(CONFIG_MIPI_RX), y)
    sub_dir += mipi_rx
endif

ifeq ($(CONFIG_MIPI_TX), y)
    sub_dir += mipi_tx
endif

ifeq ($(CONFIG_OT_IR), y)
    sub_dir += ot_ir
endif

ifeq ($(CONFIG_OT_WDG), y)
     sub_dir += wtdg
endif

ifeq ($(CONFIG_OT_SYSCFG), y)
    sub_dir += sysconfig
endif

clean_dir:= $(foreach dir, $(sub_dir), $(patsubst %, %_clean, $(dir)))

.PHONY: all clean $(sub_dir) $(clean_dir)

all: $(sub_dir)

$(sub_dir):
	@cd $@ && make

clean: $(clean_dir)

$(clean_dir):
	@cd $(patsubst %_clean, %, $@) && make clean
