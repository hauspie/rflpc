CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump

LIB_DIR=../..

MBED_PATH=/media/MBED

OUTPUT_NAME=spi

ELF=$(OUTPUT_NAME).elf
BIN=$(OUTPUT_NAME).bin

CFLAGS=`$(LIB_DIR)/rflpc-config --cflags`
LDFLAGS=`$(LIB_DIR)/rflpc-config --libs`

OBJS=main.o

all: $(BIN)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary -j .text -j .data $^ $@

$(ELF): $(OBJS)
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
