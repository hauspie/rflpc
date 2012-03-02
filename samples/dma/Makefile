LIB_DIR=../..
include $(LIB_DIR)/Makefile.in

MBED_PATH=/media/MBED

OUTPUT_NAME=dma

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
