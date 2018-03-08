# Temporary Makefile for compile allwinner H3 baremetak proj
#
PRJ_NAME ?= h3_app
PRJ_DIR := $(shell pwd)
LIB_DIR ?= $(PRJ_DIR)/..
OBJDIR = build

export BUILD_DIR := $(addprefix $(shell pwd)/, $(OBJDIR))
export PERIP_LIB := y

PERIPS_DIR := $(LIB_DIR)/

vpath %.c $(LIB_DIR)/Libraries/Peripherals/src
vpath %.o $(BUILD_DIR)/Libraries/Peripherals

SRCS = main.c syscall.c h3_startup.S
		
export CC=arm-none-eabi-gcc
export LD=arm-none-eabi-ld
export OBJCOPY=arm-none-eabi-objcopy


OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(OBJS:.S=.o)
OBJS := $(addprefix $(OBJDIR)/,$(OBJS))

AOBJS = $(shell find $(PWD)/$(OBJDIR) -type f -iname '*.o')
AOBJS = $(shell find $(PWD)/$(OBJDIR)/Libraries/Peripherals -name '*.o')
# -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mcpu=cortex-a7 

export CFLAGS  = -g -Wall -std=c11 -marm -I $(LIB_DIR)/Libraries/Peripherals/inc -I ${LIB_DIR}/Libraries

LDFILE = h3_dram.ld
LDFLAGS = -Wl,-Map,$(OBJDIR)/$(PRJ_NAME).map -g -T $(LDFILE) -nostartfiles -Wl,--gc-sections

all: $(OBJDIR)/$(PRJ_NAME).elf $(OBJDIR)/$(PRJ_NAME).hex $(OBJDIR)/$(PRJ_NAME).bin $(OBJDIR)/$(PRJ_NAME)_spl.bin

$(OBJDIR)/$(PRJ_NAME)_spl.bin: $(OBJDIR)/$(PRJ_NAME).elf
	cat u-boot-spl.bin $(OBJDIR)/$(PRJ_NAME).bin > $@

$(OBJDIR)/%.elf: $(OBJS) $(POBJS)
	make -C $(LIB_DIR)/Libraries/
	@echo "??????????"
	@echo $(POBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDFLAGS2)

%.hex: %.elf
	$(OBJCOPY) -O ihex $^ $@

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@

$(OBJDIR)/%.o: %.c
	@echo "<<<>>>"
	@echo $^
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.s
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR):
	mkdir -p $@

clean: 
	rm -rf $(OBJDIR)/*.bin $(OBJDIR)/*

# Dependdencies
$(OBJDIR)/$(PRJ_NAME).elf: $(OBJS) | $(OBJDIR)
