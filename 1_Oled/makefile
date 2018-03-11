# Temporary Makefile for compile allwinner H3 baremetak proj
#
PRJ_NAME ?= h3_oled
PRJ_DIR := $(shell pwd)
LIB_DIR ?= $(PRJ_DIR)/..
OBJDIR = build

vpath %.c $(LIB_DIR)/Libraries/Periph

SRCS = main.c syscall.c h3_startup.S SSD1306/SSD1306.c
		
CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy


OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(OBJS:.S=.o)
OBJS := $(addprefix $(OBJDIR)/,$(OBJS))

PERIPH_LIB := y
ifeq ($(PERIPH_LIB),y)
include $(LIB_DIR)/Libraries/Libraries.mk
OBJS += $(addprefix $(OBJDIR)/Libraries/,$(periph-y))
CFLAGS += -I ${LIB_DIR}/Libraries/Include/
endif


# Cortex A7: -mfloat-abi=softfp -mfpu=vfpv4-d16 -mcpu=cortex-a7

CFLAGS += -mfloat-abi=softfp -mfpu=vfpv4-d16 
CFLAGS += -g -Wall -std=c11 -mcpu=cortex-a7 -marm
CFLAGS += -I ./SSD1306


LDFILE = h3_dram.ld
LDFLAGS = -Wl,-Map,$(OBJDIR)/$(PRJ_NAME).map -g -T $(LDFILE) -nostartfiles -Wl,--gc-sections

all: $(OBJDIR)/$(PRJ_NAME).elf $(OBJDIR)/$(PRJ_NAME).hex $(OBJDIR)/$(PRJ_NAME).bin

$(OBJDIR)/$(PRJ_NAME)_spl.bin: $(OBJDIR)/$(PRJ_NAME).elf
	@cat u-boot-spl.bin $(OBJDIR)/$(PRJ_NAME).bin > $@

$(OBJDIR)/%.elf: $(OBJS)
	@echo "   [LD]  	$@"
	@$(CC) -o $@ $^ $(LDFLAGS)

%.hex: %.elf
	@echo "   [OBJCOPY]	$@"
	@$(OBJCOPY) -O ihex $^ $@

%.bin: %.elf
	@echo "   [OBJCOPY]	$@"
	@$(OBJCOPY) -O binary $^ $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "   [CC]     	$(subst $(LIB_DIR)/,,$^)"
	@$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR)/Libraries/Periph/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "   [CC]     	$(subst $(LIB_DIR)/,,$^)"
	@$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.S
	@echo "   [AS]     	$(subst $(LIB_DIR)/,,$^)"
	@$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR):
	@mkdir -p $@

clean:
	@echo "   [RM]		build"
	@rm -rf $(OBJDIR)

# Dependdencies
$(OBJDIR)/$(PRJ_NAME).elf: $(OBJS) | $(OBJDIR)
