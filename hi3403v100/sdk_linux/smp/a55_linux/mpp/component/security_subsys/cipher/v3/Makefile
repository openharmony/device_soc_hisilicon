
.PHONY: default clean

default:
	@cp -r src/common/common_check_param.c src/api/
	@cp -r src/common/common_check_param.c src/drv/drivers/
	@cd src && make mbedtls_patch && make

clean:
	@rm -rf src/api/common_check_param.c
	@rm -rf src/drv/drivers/common_check_param.c
	@cd src && make mbedtls_clean && make clean
