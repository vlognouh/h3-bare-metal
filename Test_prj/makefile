PRJ_NAME ?= h3_app
PRJ_DIR ?= ../

SRCS = main.c syscall.c h3_startup.S \
       ../../Libraries/Peripherals/src/SerialPortLib.c \
       ../../Libraries/Peripherals/src/TimerLib.c

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy

OBJDIR = build

OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(OBJS:.S=.o)
OBJS := $(addprefix $(OBJDIR)/,$(OBJS))

CFLAGS  = -g -Wno-missing-braces -Wall -std=c11 -marm -I $(PRJ_DIR)/Libraries/Peripherals/inc

LDFILE = h3_dram.ld
LDFLAGS = -Wl,-Map,$(OBJDIR)/$(PRJ_NAME).map -g -T $(LDFILE) -nostartfiles -Wl,--gc-sections

all: $(OBJDIR)/$(PRJ_NAME).elf $(OBJDIR)/$(PRJ_NAME).hex $(OBJDIR)/$(PRJ_NAME).bin $(OBJDIR)/$(PRJ_NAME)_spl.bin

$(OBJDIR)/$(PRJ_NAME)_spl.bin: $(OBJDIR)/$(PRJ_NAME).elf
	cat u-boot-spl.bin $(OBJDIR)/$(PRJ_NAME).bin > $@

$(OBJDIR)/%.elf: $(OBJS)
	#${LD} -T $(LDFILE) -o $@ $^ -Map=$(OBJDIR)/$(PRJ_NAME).map $(LDFLAGS2) 
	#${OBJCOPY} -I elf32-littlearm -O binary testapp testapp.bin
	$(CC) -o $@ $^ $(LDFLAGS) $(LDFLAGS2)

%.hex: %.elf
	$(OBJCOPY) -O ihex $^ $@

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@

$(OBJDIR)/%.o: %.c
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
