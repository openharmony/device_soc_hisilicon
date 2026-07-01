# Enum the C files needed to be compiled, using the relative path
GFBG_SRC:=src/gfbg_main.c
ifdef CONFIG_OT_PROC_SHOW_SUPPORT
GFBG_SRC+=src/gfbg_proc.c
endif
ifeq ($(OSTYPE),linux)
    ifdef GFBG_HW_VSYNC_SUPPORT
		EXTRA_CFLAGS += DCONFIG_VSYNC_SUPPORT
    endif
	GFBG_SRC += src/init/linux/gfbg_init.c
else ifeq ($(OSTYPE),liteos)
	GFBG_SRC += src/init/liteos/gfbg_init.c
endif
