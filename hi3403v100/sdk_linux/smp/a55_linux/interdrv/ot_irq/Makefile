local_dir=$(shell ls -d */ | sed "s;/;;g" | grep -v "include")


.PHONY: all clean $(local_dir) $(clean_dir)

all: $(local_dir)

$(local_dir):
	cd $@; make || exit1;

clean_dir:= $(foreach dir, $(local_dir), $(patsubst %, %_clean, $(dir)))

clean: $(clean_dir)

$(clean_dir):
	@cd $(patsubst %_clean, %, $@) ; make clean;

