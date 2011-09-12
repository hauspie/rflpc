CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump

LIB_DIR=../../rflpc17xx
LINK_SCRIPT=rflpc17xx.ld

MBED_PATH=/media/MBED

ELF=timer.elf
BIN=timer.bin

CFLAGS=-Os -Wall -I$(LIB_DIR) -DRFLPC_PLATFORM_MBED
LDFLAGS=-nostartfiles -static -L$(LIB_DIR) -lrflpc17xx -Wl,-T,$(LINK_SCRIPT),-L$(LIB_DIR)

OBJS=main.o

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
