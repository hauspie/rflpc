OUTPUT_NAME=$(shell basename $(shell pwd))
SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
EXTERNAL_LIBS=nhd_spi_lcd

# Modify this variable at your own risk
RFLPC_DIR=../..
include $(RFLPC_DIR)/Makefile.in

