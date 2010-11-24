CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
AR=arm-none-eabi-ar
OBJDUMP=arm-none-eabi-objdump
CFLAGS=-Wall

LIB=liblpc1768.a

ASOBJS=start.o
CFILES=$(wildcard *.c)
COBJS=$(CFILES:.c=.o)
HEADERS=$(wildcard *.h)

$(LIB): $(ASOBJS) $(COBJS)
	$(AR) rs $@ $^

start.o: start.s
	$(AS) -o $@ $^

init.o: init.c $(HEADERS)
	$(CC) -Wall -o $@ -c $<

makefile.dep: $(CFILES) $(HEADERS)
	@echo "Generating dependencies"
	@$(CC) $(CFLAGS) -MM $(CFILES) > $@

dump: $(LIB)
	$(OBJDUMP) -sSt $(LIB)

clean:
	$(RM) *.o *~ *.dep

# Dependencies rules so that compilation works well
include makefile.dep
