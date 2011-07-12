DIRS=rflpc17xx sample/basic_tests sample/ethernet

.PHONY: all $(DIRS)

all: $(patsubst %, _all_%, $(DIRS))
clean: $(patsubst %, _clean_%, $(DIRS))
dump: $(patsubst %, _dump_%, $(DIRS))

program: all
	make -C sample/ethernet program


$(patsubst %, _all_%, $(DIRS)):
	make -C $(patsubst _all_%, %, $@)

$(patsubst %, _clean_%, $(DIRS)):
	$(RM) *~
	make -C $(patsubst _clean_%, %, $@) clean

$(patsubst %, _dump_%, $(DIRS)):
	make -C $(patsubst _dump_%, %, $@) dump
