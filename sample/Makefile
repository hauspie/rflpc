CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
LINK_SCRIPT=../lpc1768/lpc1768.ld
START=../lpc1768/start.s
START_OBJ=start.o
MBED_PATH=/media/MBED

ELF=sample.elf
BIN=sample.bin

CFLAGS=-Wall -I../lpc1768

OBJS=main.o

$(BIN): $(ELF)
	$(OBJCOPY) -O binary -j .text -j .data $^ $@

$(ELF): $(START_OBJ) $(OBJS)
	$(LD) -T $(LINK_SCRIPT) -o $@ $^

$(START_OBJ): $(START)
	$(AS) -o $@ $^

clean:
	$(RM) *.o *~

program: $(BIN)
	cp $+ $(MBED_PATH)
	sync

dump: $(ELF)
	$(OBJDUMP) -sSt $^
