CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AS=arm-none-eabi-as
AR=arm-none-eabi-ar
OBJDUMP=arm-none-eabi-objdump
CFLAGS=-Wall

LIB=liblpc1768.a

OBJS=start.o init.o uart.o
HEADERS=$(wildcard *.h)

$(LIB): $(OBJS)
	$(AR) rs $@ $^

start.o: start.s
	$(AS) -o $@ $^

init.o: init.c $(HEADERS)
	$(CC) -Wall -o $@ -c $<

dump: $(LIB)
	$(OBJDUMP) -sSt $(LIB)

clean:
	$(RM) *.o *~
