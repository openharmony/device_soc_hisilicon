ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../Makefile.param
    include $(PARAM_FILE)
endif

security_subsys :=

ifeq ($(CONFIG_OT_CIPHER_SUPPORT),y)
security_subsys += cipher
endif

ifeq ($(CONFIG_OT_OTP_SUPPORT),y)
security_subsys += otp
endif

ifeq ($(CONFIG_OT_KLAD_SUPPORT),y)
security_subsys += klad
endif


security_subsys_clean := $(security_subsys:%=%_clean)

.PHONY: all clean $(security_subsys) $(security_subsys_clean) copy copy_clean

all: $(security_subsys)
	@echo -e "\e[0;32;1m===========security_subsys compile ok!============ \e[0m"

clean: $(security_subsys_clean) copy_clean
	@echo -e "\e[0;32;1m===========security_subsys clean ok!============ \e[0m"

cipher: copy
	@echo -e "\e[32;1m--Compiling 'cipher'...\e[0m"
	@cd cipher && make

cipher_clean:
	@cd cipher && make clean

otp: copy
	@echo -e "\e[32;1m--Compiling 'otp'...\e[0m"
	@cd otp && make

otp_clean:
	@cd otp && make clean

klad: copy
	@echo -e "\e[32;1m--Compiling 'klad'...\e[0m"
	@cd klad && make

klad_clean:
	@cd klad && make clean

hardware_cryptodev: copy
	@echo -e "\e[32;1m--Compiling 'hardware_cryptodev'...\e[0m"
	@cd hardware_cryptodev && make

hardware_cryptode_clean:
	@cd hardware_cryptodev && make clean

include $(PWD)/Makefile.copy
