CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump

LIB_DIR=../../rflpc17xx
LINK_SCRIPT=rflpc17xx.ld

MBED_PATH=/media/MBED

ELF=ethernet.elf
BIN=ethernet.bin

CFLAGS=-Os -Wall -I$(LIB_DIR) -DRFLPC_PLATFORM_MBED
LDFLAGS=-nostdlib -L$(LIB_DIR) -lrflpc17xx -T $(LINK_SCRIPT)

OBJS=main.o protocols.o

.PHONY: $(LIB_DIR)

all: $(LIB_DIR) $(BIN)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary -j .text -j .data $^ $@

$(ELF): $(OBJS) $(LIB_DIR)/librflpc17xx.a
	$(LD) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) *.o *~

mrproper: clean
	$(RM) $(ELF) $(BIN)

program: $(BIN)
	cp $+ $(MBED_PATH) && sync

dump: $(ELF)
	$(OBJDUMP) -sSt $^

$(LIB_DIR):
	make -C $(LIB_DIR)
