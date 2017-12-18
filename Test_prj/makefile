PROJ_NAME ?= h3_app

SRCS = main.c startup.s \
       Library/SerialPortLib/SerialPortLib.c

CC=arm-linux-gnueabihf-gcc
LD=arm-linux-gnueabihf-ld
OBJCOPY=arm-linux-gnueabihf-objcopy

OBJDIR = build

OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(addprefix $(OBJDIR)/,$(OBJS))

LDFLAGS = -Wl,-Map,$(OBJDIR)/$(PROJ_NAME).map -g -T h3.lds
CFLAGS  = -g -Wall -Wno-missing-braces -std=c99 -IInclude -marm -shared

all: $(OBJDIR)/$(PROJ_NAME).elf $(OBJDIR)/$(PROJ_NAME).hex $(OBJDIR)/$(PROJ_NAME).bin $(OBJDIR)/$(PROJ_NAME)_spl.bin

$(OBJDIR)/$(PROJ_NAME)_spl.bin: $(OBJDIR)/$(PROJ_NAME).elf
	cat u-boot-spl.bin $(OBJDIR)/$(PROJ_NAME).bin > $@

$(OBJDIR)/%.elf: $(OBJS)
	#$(CC) -o $@ $^ $(LDFLAGS)
	${LD} -T h3.lds -o $@ $^ -Map=$(OBJDIR)/$(PROJ_NAME).map
	#${OBJCOPY} -I elf32-littlearm -O binary testapp testapp.bin

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
$(OBJDIR)/$(PROJ_NAME).elf: $(OBJS) | $(OBJDIR)
