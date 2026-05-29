export PARAM_FILE := ../Makefile.param
include $(PARAM_FILE)

rel:
	@cd ive;     make
	@cd mau;     make
	@cd dpu;     make
	@cd svp_npu; make

clean:
	@cd ive;     make clean
	@cd mau;     make clean
	@cd dpu;     make clean
	@cd svp_npu; make clean

cleanall:
	@cd ive;     make clean;   make cleanstream
	@cd mau;     make clean;   make cleanstream
	@cd dpu;     make clean;   make cleanstream
	@cd svp_npu; make clean;   make cleanstream
