
RFLPC_DIR=../

# Defines the compiler and binutils tools for the current host
include $(RFLPC_DIR)/Makefile.vars

RFLPC_LIB_DIR=$(RFLPC_DIR)/rflpc17xx
RFLPC_LIB_FILE=$(shell $(RFLPC_DIR)/rflpc-config --libfile)

# Multiline variable for use with -F grep option
ALLOWED_SYMBOLS_FILE := \
_start \
_default_exception_handler \
_default_peripheral_handler \
_init_stack \
rflpc_clock_init \
rflpc_clock_get_system_clock \
rflpc_irq_init \
rflpc_irq_set_handler \
__LDREXB \
__LDREXH \
__LDREXW \
__RBIT \
__REV \
__REV16 \
__REVSH \
__STREXB \
__STREXH \
__STREXW \
__get_BASEPRI \
__get_CONTROL \
__get_FAULTMASK \
__get_MSP \
__get_PRIMASK \
__get_PSP \
__set_BASEPRI \
__set_CONTROL \
__set_FAULTMASK \
__set_MSP \
__set_PRIMASK \
__set_PSP


.PHONY: all $(RFLPC_LIB_FILE)

check_empty_config: $(RFLPC_LIB_FILE)
# Get the list of symbols in the library */	
	$(eval SYMBOLS := $(shell $(NM) $(RFLPC_LIB_FILE) | grep -i ' T ' | cut -d' ' -f 3))
	$(eval BAD_SYMBOLS := $(sort $(filter-out $(ALLOWED_SYMBOLS_FILE), $(SYMBOLS))))
# Check if at least one symbol is there and should not		
	$(if $(BAD_SYMBOLS), @echo "Some symbols are not directed by a #ifdef RFLPC_XXXX_ENABLE rule")
	@for symbol in $(BAD_SYMBOLS) ; do echo $$symbol ; done
	@echo "Size report:"
	$(SIZE) -t $(RFLPC_LIB_FILE)

extract_config_vars:
	@grep -R " RFLPC_CONFIG" $(RFLPC_LIB_DIR)/* | cut -d\  -f2 | sort | uniq

$(RFLPC_LIB_FILE):
	@echo "Cleaning"
	@make -C $(RFLPC_LIB_DIR) mrproper >& /dev/null
	@echo "Compiling"
	@make -C $(RFLPC_LIB_DIR) >& /dev/null
