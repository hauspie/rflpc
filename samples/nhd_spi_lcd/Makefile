OUTPUT_NAME=$(shell basename $(shell pwd))
SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
PERIPHERALS=nhd_spi_lcd

# Modify this variable at your own risk
RFLPC_DIR=../..
include $(RFLPC_DIR)/Makefile.in

