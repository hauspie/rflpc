DIRS=lpc1768 sample

.PHONY: all $(DIRS)

all: $(patsubst %, _all_%, $(DIRS))
clean: $(patsubst %, _clean_%, $(DIRS))
dump: $(patsubst %, _dump_%, $(DIRS))

program:
	make -C sample program


$(patsubst %, _all_%, $(DIRS)):
	make -C $(patsubst _all_%, %, $@)

$(patsubst %, _clean_%, $(DIRS)):
	make -C $(patsubst _clean_%, %, $@) clean

$(patsubst %, _dump_%, $(DIRS)):
	make -C $(patsubst _dump_%, %, $@) dump
