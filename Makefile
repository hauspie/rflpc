DIRS=rflpc17xx samples/basic_tests samples/ethernet samples/timer

.PHONY: all $(DIRS)

all: $(patsubst %, _all_%, $(DIRS))
clean: $(patsubst %, _clean_%, $(DIRS))
dump: $(patsubst %, _dump_%, $(DIRS))

program: all
	make -C samples/timer program


$(patsubst %, _all_%, $(DIRS)):
	make -C $(patsubst _all_%, %, $@)

$(patsubst %, _clean_%, $(DIRS)):
	$(RM) *~
	make -C $(patsubst _clean_%, %, $@) clean

$(patsubst %, _dump_%, $(DIRS)):
	make -C $(patsubst _dump_%, %, $@) dump
