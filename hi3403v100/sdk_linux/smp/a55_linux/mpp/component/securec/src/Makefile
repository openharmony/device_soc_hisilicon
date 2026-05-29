# $(ROOT)/mpp/component/securec/src/Makefile

ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../Makefile.param
    include $(PARAM_FILE)
endif
BUILD_DIR = $(TEMP_PATH)/$(subst $(SDK_PATH)/,,$(CURDIR))
BUILD_DIR_MAKEFILE := $(BUILD_DIR)/Makefile
BUILD_OBJ = $(BUILD_DIR)/../obj
BUILD_LIB = $(BUILD_DIR)/../lib
BUILD_KO = $(BUILD_DIR)/../ko

securec_target :=

ifeq ($(CONFIG_OT_SECUREC_KERNEL_SUPPORT),y)
securec_target += lib
securec_target += kernel
else
securec_target += lib
endif

securec_clean := $(foreach dir, $(securec_target), $(patsubst %, %_clean, $(dir)))

LIBA=libsecurec.a
LIBS=libsecurec.so

PROJECT=libsecurec


#if you need a debug version library, use "-g" instead of "-s -DNDEBUG -O2".
# If you compiler report a warning on "break strict-aliasing rules", there is no problem. If you need to clear all warnings, you can add "-fno-strict-aliasing" option to your compiler, but this will impact the performance a little.
GCC=$(CC)

#for linux secure compile options from stdandard 2018.8
SECURE_CFLAG_FOR_SHARED_LIBRARY  = $(LIBS_CFLAGS)
SECURE_LDFLAG_FOR_SHARED_LIBRARY += $(LIBS_LD_CFLAGS)
#This option ensure forced links to functions in the library. If these functions are already contained in other libraries, cross-calls will occur
#SECURE_LDFLAG_FOR_SHARED_LIBRARY += -Wl,-Bsymbolic

GCC_HAVE_STRONG=$(shell $(CC) -fstack-protector-strong -E -dM - < /dev/null 2>&1 |grep -- "fstack-protector-strong" >/dev/null ;if [ $$? -ne 0 ] ;then echo yes;fi)
ifeq ($(GCC_HAVE_STRONG),yes)
SECURE_CFLAG_FOR_SHARED_LIBRARY  += -fstack-protector-strong
else
SECURE_CFLAG_FOR_SHARED_LIBRARY  += -fstack-protector-all
endif


#-fvisibility=hidden need modify source code
#SECURE_CFLAG_FOR_SHARED_LIBRARY_OPTIONAL += -fvisibility=hidden

#-ftrapv -D_FORTIFY_SOURCE=2 -fstack-check May result in performance degradation after opening
#SECURE_CFLAG_FOR_SHARED_LIBRARY_OPTIONAL += -D_FORTIFY_SOURCE=2 -O2
#SECURE_CFLAG_FOR_SHARED_LIBRARY_OPTIONAL += -ftrapv
#SECURE_CFLAG_FOR_SHARED_LIBRARY_OPTIONAL += -fstack-check


SECURE_CFLAG_FOR_SHARED_LIBRARY_OPTIONAL += -s
SECURE_CFLAG_FOR_SHARED_LIBRARY_OPTIONAL +=  -Wformat=2 -Wfloat-equal -Wshadow
# about pie option , We compiled a dynamic library, so we did not use it. ,If you want to compile executable files, please open this option
#SECURE_CFLAG_FOR_EXE = -fPIE -pie



## code standard options
SECUREC_CODE_STANDARD_OPTION = -Wconversion
SECUREC_CODE_STANDARD_OPTION += -Wformat-security
SECUREC_CODE_STANDARD_OPTION += -Wextra
SECUREC_CODE_STANDARD_OPTION += --param ssp-buffer-size=4

#repeat options
#SECUREC_CODE_STANDARD_OPTION += -D_FORTIFY_SOURCE=2
#SECUREC_CODE_STANDARD_OPTION += -Wl,-z,relro,-z,now
#SECUREC_CODE_STANDARD_OPTION += -fstack-protector
#this option just break compile
#SECUREC_CODE_STANDARD_OPTION  += -Werror

#from product options
PRODUCT_OPTION  = -Warray-bounds
PRODUCT_OPTION += -Wpointer-arith
PRODUCT_OPTION += -Wcast-qual
PRODUCT_OPTION += -Wstrict-prototypes
PRODUCT_OPTION += -Wmissing-prototypes
PRODUCT_OPTION += -Wstrict-overflow=1
PRODUCT_OPTION += -Wstrict-aliasing=2
PRODUCT_OPTION += -Wswitch -Wswitch-default
PRODUCT_OPTION += # -fno-inline-small-functions -fno-indirect-inlining -fno-inline-functions-called-once -fno-early-inlining -fno-inline
PRODUCT_OPTION += # -Wframe-larger-than=1024
PRODUCT_OPTION += # -ftrapv

CFLAG   =  -Wall  -s -DNDEBUG -O2 $(SECURE_CFLAG_FOR_SHARED_LIBRARY) $(SECURE_CFLAG_FOR_EXE) $(SECURE_CFLAG_FOR_SHARED_LIBRARY_OPTIONAL) $(SECUREC_CODE_STANDARD_OPTION) $(PRODUCT_OPTION)
#CFLAG += -DSECUREC_VXWORKS_PLATFORM
#CFLAG += -DSECUREC_SUPPORT_STRTOLD
#CFLAG += -DSECUREC_VXWORKS_VERSION_5_4
#CFLAG += -D__STDC_WANT_LIB_EXT1__=0
CFLAG  += $(CFLAG_EXT)

#ARCH:=$(shell getconf LONG_BIT)


ifeq ($(MAKECMDGOALS),lib)
#Set static library related options
CFLAG :=$(filter-out "xxxxx",$(CFLAG))
endif

#SOURCES=$(wildcard *.c)
SOURCES = fscanf_s.c gets_s.c memcpy_s.c memmove_s.c memset_s.c scanf_s.c securecutil.c secureinput_a.c secureprintoutput_a.c snprintf_s.c sprintf_s.c sscanf_s.c strcat_s.c strcpy_s.c strncat_s.c strncpy_s.c strtok_s.c vfscanf_s.c vscanf_s.c vsnprintf_s.c vsprintf_s.c vsscanf_s.c

SOURCES += fwscanf_s.c secureinput_w.c secureprintoutput_w.c swprintf_s.c swscanf_s.c vfwscanf_s.c vswprintf_s.c vswscanf_s.c vwscanf_s.c wcscat_s.c wcscpy_s.c wcsncat_s.c wcsncpy_s.c wcstok_s.c wmemcpy_s.c wmemmove_s.c wscanf_s.c

OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

.PHONY:clean all $(securec_target) $(securec_clean)

ENABLE_SCANF_FILE=$(findstring SECUREC_ENABLE_SCANF_FILE=0,$(CFLAG))
ifeq ($(ENABLE_SCANF_FILE),SECUREC_ENABLE_SCANF_FILE=0)
OBJECTS:=$(filter-out fscanf_s.o vfscanf_s.o vscanf_s.o scanf_s.o vwscanf_s.o wscanf_s.o fwscanf_s.o vfwscanf_s.o,$(OBJECTS))
endif

all:$(securec_target)
	@echo "securec compile ok======"

ifneq ($(CFLAGS),)
CFLAG :=$(CFLAGS)
endif
CFLAG += -I../include
LD_FLAG ?= $(SECURE_LDFLAG_FOR_SHARED_LIBRARY) $(SECURE_CFLAG_FOR_SHARED_LIBRARY)
AR ?=ar
RANLIB ?=ranlib

$(PROJECT):$(OBJECTS)
	@mkdir -p $(BUILD_OBJ)
	@mkdir -p $(BUILD_LIB)
	$(GCC) $(LIBS_CFLAGS) -shared -o $(BUILD_OBJ)/$(LIBS) $(patsubst %.o,$(BUILD_OBJ)/%.o,$^)  $(LD_FLAG)
	ar crv $(BUILD_LIB)/$(LIBA) $(patsubst %.o,$(BUILD_OBJ)/%.o,$^)
	ranlib $(BUILD_LIB)/$(LIBA)
	@mkdir -p $(REL_LIB);cp $(BUILD_LIB)/* $(REL_LIB) -rf
	@mkdir -p $(REL_INC);cp ../include/* $(REL_INC) -rf

lib:$(OBJECTS)
	@mkdir -p $(BUILD_OBJ)
	mkdir -p $(BUILD_LIB)
	$(GCC) $(LIBS_CFLAGS) -shared -o $(BUILD_LIB)/$(LIBS) $(patsubst %.o,$(BUILD_OBJ)/%.o,$^)  $(LD_FLAG)
	ar crv $(BUILD_LIB)/$(LIBA) $(patsubst %.o,$(BUILD_OBJ)/%.o,$^)
	ranlib $(BUILD_LIB)/$(LIBA)
	@mkdir -p $(REL_LIB);cp $(BUILD_LIB)/* $(REL_LIB) -rf
	@mkdir -p $(REL_INC);cp ../include/* $(REL_INC) -rf

.c.o:
	@mkdir -p $(BUILD_OBJ)
	$(GCC) -c $< $(CFLAG) -o $(BUILD_OBJ)/$(patsubst %.c,%.o,$<)

EXTRA_CFLAGS += -I$(INCDIR) -fstack-protector $(CFLAG_EXT) $(DRV_CFLAGS) $(MPP_CFLAGS)

# provide the default value to module name and ccflags-y
ifeq ($(OSTYPE),linux)
ifeq ($(MODULE),)
	MODULE := ot_securec
endif
ifeq ($(DEBUG),y)
	ccflags-y += -DDEBUG
endif

ifneq ($(KERNELRELEASE),)
    obj-m := ot_securec.o
ifeq ($(SECUREC_KERNEL_ALL),)
    #ot_securec-objs := memcpy_s.o memmove_s.o memset_s.o  securecutil.o  strcat_s.o strcpy_s.o strncat_s.o strncpy_s.o
    ot_securec-objs := memcpy_s.o memmove_s.o memset_s.o  securecutil.o  strcat_s.o strcpy_s.o strncat_s.o strncpy_s.o sprintf_s.o vsprintf_s.o snprintf_s.o vsnprintf_s.o secureprintoutput_a.o sscanf_s.o vsscanf_s.o secureinput_a.o strtok_s.o
else
    ot_securec-objs := memcpy_s.o memmove_s.o memset_s.o  securecutil.o  strcat_s.o strcpy_s.o strncat_s.o strncpy_s.o sprintf_s.o vsprintf_s.o snprintf_s.o vsnprintf_s.o secureprintoutput_a.o sscanf_s.o vsscanf_s.o secureinput_a.o strtok_s.o
endif
else
    KERNELDIR := /lib/modules/$(shell uname -r)/build
    PWD := $(shell pwd)
kernel:
	@mkdir -p $(BUILD_KO)
	@touch $(BUILD_DIR_MAKEFILE)
	@$(MAKE) -C $(KERNEL_ROOT) M=$(BUILD_DIR) src=$(CURDIR) INCDIR=$(PWD)/../include O=$(KERNEL_ROOT) V=10  modules -j 24
	@cp $(BUILD_DIR)/ot_securec.ko $(BUILD_KO) -rf
	@mkdir -p $(REL_KO);cp $(BUILD_KO)/* $(REL_KO) -rf
	@mkdir -p $(REL_INC);cp ../include/* $(REL_INC) -rf
endif
endif

clean: $(securec_clean)
	@echo "cleaning ...."

kernel_clean:
	@rm -f $(BUILD_DIR)/modules.order $(BUILD_DIR)/Module.symvers  $(BUILD_DIR)/$(MODULE).ko $(BUILD_DIR)/$(MODULE).mod.c
	@rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.cmd $(BUILD_OBJ) $(BUILD_LIB) $(BUILD_KO) $(PROJECT)
	@rm $(REL_KO)/ot_securec.ko -rf;

lib_clean:
	@rm $(REL_LIB)/$(LIBA) -rf;
	@rm $(REL_LIB)/$(LIBS) -rf;
	@rm $(REL_INC)/securec.h -rf;
	@rm $(REL_INC)/securectype.h -rf;

