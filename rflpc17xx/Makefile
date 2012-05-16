include ../Makefile.vars
CFLAGS=$(shell ../rflpc-config --cflags)

LIB=librflpc17xx.a

CFILES=$(wildcard *.c)
CFILES+=$(wildcard drivers/*.c)
CFILES+=$(wildcard nxp/*.c)
CFILES+=$(wildcard tinylibc/*.c)
COBJS=$(CFILES:.c=.o)
HEADERS=$(wildcard *.h)
HEADERS+=$(wildcard drivers/*.h)
HEADERS+=$(wildcard nxp/*.h)
HEADERS+=$(wildcard tinylibc/*.h)

CONFIG_FILE=config/config-options.h

.PHONY: check_config

all: check_config $(LIB)

# Checks for the presence of the config file. If not, generate a full library by default
check_config:
	@if ! test -f $(CONFIG_FILE) ; then make -C ../config full_config ; make mrproper ; fi

$(LIB): $(COBJS)
	$(AR) rs $@ $^


makefile.dep: $(CFILES) $(HEADERS)
	@echo "Generating dependencies"
	@$(CC) $(CFLAGS) -MM $(CFILES) > $@

dump: $(LIB)
	$(OBJDUMP) -sSt $(LIB)

clean:
	$(RM) $(COBJS) *~ *.dep drivers/*~ nxp/*~ tinylibc/*~

mrproper: clean
	$(RM) $(LIB)

# Dependencies rules so that compilation works well
include makefile.dep
