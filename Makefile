DIRS=rflpc17xx doc $(wildcard samples/*)

.PHONY: all $(DIRS)

all: $(patsubst %, _all_%, $(DIRS))
clean: $(patsubst %, _clean_%, $(DIRS))
mrproper: $(patsubst %, _mrproper_%, $(DIRS))
dump: $(patsubst %, _dump_%, $(DIRS))

$(patsubst %, _all_%, $(DIRS)):
	make -C $(patsubst _all_%, %, $@)

$(patsubst %, _clean_%, $(DIRS)):
	$(RM) *~
	make -C $(patsubst _clean_%, %, $@) clean

$(patsubst %, _mrproper_%, $(DIRS)):
	$(RM) *~
	make -C $(patsubst _mrproper_%, %, $@) mrproper

$(patsubst %, _dump_%, $(DIRS)):
	make -C $(patsubst _dump_%, %, $@) dump
