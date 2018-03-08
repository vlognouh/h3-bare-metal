obj-$(PERIP_LIB) += CcuLib.o
obj-$(PERIP_LIB) += GicLib.o
obj-$(PERIP_LIB) += I2cLib.o
obj-$(PERIP_LIB) += SerialPortLib.o
obj-$(PERIP_LIB) += SpiLib.o
obj-$(PERIP_LIB) += TimerLib.o
obj-$(PERIP_LIB) += UartLib.o

vpath %.c Peripherals/src

SRC_DIR := $(BUILD_DIR)/Libraries/Peripherals
export POBJS := $(addprefix $(SRC_DIR)/,$(obj-y))

all: $(POBJS)
	@echo "######## Build Library ########"
	@echo $(BUILD_DIR)
	mkdir -p $(SRC_DIR)
	@echo $(POBJS)

$(SRC_DIR)/%.o : %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^