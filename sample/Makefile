CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy

ELF=sample.elf
BIN=sample.bin


OBJS=main.o

$(BIN): $(ELF)
	$(OBJCOPY) -O binary -j .text -j .data $^ $@

$(ELF): $(OBJS)
	$(LD) -o $@ $^

clean:
	$(RM) *.o *~
