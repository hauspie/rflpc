CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
AR=arm-none-eabi-ar
OBJDUMP=arm-none-eabi-objdump
CFLAGS=`../rflpc-config --cflags`

LIB=librflpc17xx.a

CFILES=$(wildcard *.c)
CFILES+=$(wildcard drivers/*.c)
CFILES+=$(wildcard nxp/*.c)
COBJS=$(CFILES:.c=.o)
HEADERS=$(wildcard *.h)
HEADERS+=$(wildcard drivers/*.h)

$(LIB): $(COBJS)
	$(AR) rs $@ $^


makefile.dep: $(CFILES) $(HEADERS)
	@echo "Generating dependencies"
	@$(CC) $(CFLAGS) -MM $(CFILES) > $@

dump: $(LIB)
	$(OBJDUMP) -sSt $(LIB)

clean:
	$(RM) $(COBJS) *~ *.dep drivers/*~ nxp/*~

mrproper: clean
	$(RM) $(LIB)
	

# Dependencies rules so that compilation works well
include makefile.dep
